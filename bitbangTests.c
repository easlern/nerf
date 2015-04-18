#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "bitbang.h"
#include "bitbangTests.h"
#include "testing.h"
#include "gpioProvider.h"


struct BitbangTests_GpioUsages;
typedef struct{
    uint8_t setPinsUsed [100];
    uint8_t setPinsUsedCount;
    uint8_t clearPinsUsed [100];
    uint8_t clearPinsUsedCount;
    uint8_t setupPinsUsed [100];
    uint8_t setupPinsUsedCount;
    uint8_t setupModesUsed [100];
    uint8_t setupModesUsedCount;
} BitbangTests_GpioUsages;

void bitbangTests_resetGpioUsages (BitbangTests_GpioUsages* usages){
    usages->clearPinsUsedCount = 0;
    usages->setPinsUsedCount = 0;
    usages->setupPinsUsedCount = 0;
    usages->setupModesUsedCount = 0;
}
BitbangTests_GpioUsages bitbangTests_gpioUsages;

void bitbangTests_pushArray (uint8_t* array, uint8_t* count, uint8_t value){
    array [*count] = value;
    *count += 1;
}

uint8_t bitbangTests_testPort = 0x00;
uint8_t bitbangTests_masterByte = 0x00;
int bitbangTests_masterByteCurrentBit = 7;
void bitbangTests_nextClock(){
    uint8_t currentClock = bitbangTests_testPort & (0x01 << BITBANG_CLK);
    if (!currentClock){ //If the clock is low, set MOSI to the next bit in the simulated byte.
        bitbangTests_testPort |= (0x01 << BITBANG_CLK);
        if (bitbangTests_masterByte & (0x01 << bitbangTests_masterByteCurrentBit)){
            bitbangTests_testPort |= (0x01 << BITBANG_MOSI);
        }
        else{
            bitbangTests_testPort &= ~(0x01 << BITBANG_MOSI);
        }
        bitbangTests_masterByteCurrentBit--;
        if (bitbangTests_masterByteCurrentBit < 0) bitbangTests_masterByteCurrentBit = 7;
    }
    else{
        bitbangTests_testPort &= ~(0x01 << BITBANG_CLK);
    }
}
uint8_t bitbangTests_getPin (uint8_t pin){
    if (pin == BITBANG_CLK){
        bitbangTests_nextClock();
    }
    return bitbangTests_testPort & (0x01 << pin);
}
void bitbangTests_setPin (uint8_t pin){
    bitbangTests_pushArray (&bitbangTests_gpioUsages.setPinsUsed, &bitbangTests_gpioUsages.setPinsUsedCount, pin);
}
void bitbangTests_clearPin (uint8_t pin){ 
    bitbangTests_pushArray (&bitbangTests_gpioUsages.clearPinsUsed, &bitbangTests_gpioUsages.clearPinsUsedCount, pin);
}
void bitbangTests_setupPin (uint8_t pin, uint8_t mode){ 
    bitbangTests_pushArray (&bitbangTests_gpioUsages.setupPinsUsed, &bitbangTests_gpioUsages.setupPinsUsedCount, pin);
    bitbangTests_pushArray (&bitbangTests_gpioUsages.setupModesUsed, &bitbangTests_gpioUsages.setupModesUsedCount, mode);
}

void bitbangTests_testInit(){
    bitbangTests_testPort = 0x00;
    bitbangTests_masterByte = 0x00;
    bitbangTests_masterByteCurrentBit = 7;

    bitbangTests_resetGpioUsages (&bitbangTests_gpioUsages);
}

bool bitbangTests_init(){
    bitbangTests_testInit();

    GpioProvider provider = gpioProvider_createGpioProvider (&bitbangTests_getPin, &bitbangTests_clearPin, &bitbangTests_setPin, &bitbangTests_setupPin);
    bitbang_init (provider);

    bool passed = testing_shouldEqual (3, bitbangTests_gpioUsages.setupPinsUsedCount);
    passed &= testing_shouldEqual (0, bitbangTests_gpioUsages.setupPinsUsed[0]);
    passed &= testing_shouldEqual (1, bitbangTests_gpioUsages.setupPinsUsed[1]);
    passed &= testing_shouldEqual (2, bitbangTests_gpioUsages.setupPinsUsed[2]);

    passed &= testing_shouldEqual (3, bitbangTests_gpioUsages.setupModesUsedCount);
    passed &= testing_shouldEqual (1, bitbangTests_gpioUsages.setupModesUsed[0]);
    passed &= testing_shouldEqual (0, bitbangTests_gpioUsages.setupModesUsed[1]);
    passed &= testing_shouldEqual (1, bitbangTests_gpioUsages.setupModesUsed[2]);

    return passed;
}

bool bitbangTests_receiveByteAsSlave(){
    bitbangTests_testInit();

    GpioProvider provider = gpioProvider_createGpioProvider (&bitbangTests_getPin, &bitbangTests_clearPin, &bitbangTests_setPin, &bitbangTests_setupPin);
    bitbang_init (provider);

    bitbangTests_masterByte = 0x01;

    uint8_t received = bitbang_receiveByteAsSlave();

    return testing_shouldEqual (bitbangTests_masterByte, received);
}

bool bitbangTests_sendByteAsSlave_0x00(){
    bitbangTests_testInit();

    GpioProvider provider = gpioProvider_createGpioProvider (&bitbangTests_getPin, &bitbangTests_clearPin, &bitbangTests_setPin, &bitbangTests_setupPin);
    bitbang_init (provider);

    bitbang_sendByteAsSlave (0x00);

    // Should set MOSI at beginning of loop to send byte. Should set no other pins.
    bool passed = testing_shouldEqual (1, bitbangTests_gpioUsages.setPinsUsedCount);
    passed &= testing_shouldEqual (1, bitbangTests_gpioUsages.setPinsUsed [0]);
    // Should clear the pins 8 times because there are 9 bits to send that are 0 and it has to clear MOSI to indicate the end of a transmission.
    passed &= testing_shouldEqual (9, bitbangTests_gpioUsages.clearPinsUsedCount);
    passed &= testing_shouldEqual (1, bitbangTests_gpioUsages.clearPinsUsed [0]);

    return passed;
}

bool bitbangTests_sendByteAsSlave_0xff(){
    bitbangTests_testInit();

    GpioProvider provider = gpioProvider_createGpioProvider (&bitbangTests_getPin, &bitbangTests_clearPin, &bitbangTests_setPin, &bitbangTests_setupPin);
    bitbang_init (provider);

    bitbang_sendByteAsSlave (0xff);

    // Should set MOSI at beginning of loop to send byte, then should set eight pins.
    bool passed = testing_shouldEqual (9, bitbangTests_gpioUsages.setPinsUsedCount);
    passed &= testing_shouldEqual (1, bitbangTests_gpioUsages.setPinsUsed [1]);
    passed &= testing_shouldEqual (1, bitbangTests_gpioUsages.setPinsUsed [8]);
    // Should clear the pins 1 time because it has to clear MOSI to indicate the end of a transmission.
    passed &= testing_shouldEqual (1, bitbangTests_gpioUsages.clearPinsUsedCount);

    return passed;
}

void bitbangTests_runTests(){
    bitbangTests_init();
    bitbangTests_receiveByteAsSlave();
    bitbangTests_sendByteAsSlave_0x00();
    bitbangTests_sendByteAsSlave_0xff();
}
