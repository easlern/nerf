#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "bitbang.h"


GpioProvider bitbang_gpioProvider;

void bitbang_set(uint8_t line){
    bitbang_gpioProvider.setPin (line);
}
void bitbang_clear (uint8_t line){
    bitbang_gpioProvider.clearPin (line);
}
uint8_t bitbang_get (uint8_t line){
    return bitbang_gpioProvider.getPin (line);
}

void bitbang_setMISO(){
    bitbang_set (BITBANG_MISO);
}
void bitbang_clearMISO(){
    bitbang_clear (BITBANG_MISO);
}
uint8_t bitbang_getMOSI(){
    return bitbang_get (BITBANG_MOSI);
}
uint8_t bitbang_getClock(){
    return bitbang_get (BITBANG_CLK);
}

void bitbang_init (GpioProvider gpioProvider){
    bitbang_gpioProvider = gpioProvider;

    bitbang_gpioProvider.setupPin (BITBANG_CLK, 1);
    bitbang_gpioProvider.setupPin (BITBANG_MISO, 0);
    bitbang_gpioProvider.setupPin (BITBANG_MOSI, 1);
}

uint8_t bitbang_diagnosticMessage = 0x00;
void bitbang_setDiagnosticMessage (const uint8_t byte){
    bitbang_diagnosticMessage = byte;
}

// Assumes that on LAT, 0 = clock, 1 = MOSI, 2 = MISO
void bitbang_sendByteAsSlave (const uint8_t byte){
    // Set MISO so the master knows we're ready to send a byte
    bitbang_setMISO();

    for (int8_t nextBit = 7; nextBit >= 0; nextBit--){
        while (bitbang_getClock()); // Wait for the clock to go low
        if (byte & (0x01 << nextBit)){
            bitbang_setMISO();
        }
        else{
            bitbang_clearMISO();
        }
        while (!bitbang_getClock()); // Wait for clock to go high
    }
    while (bitbang_getClock()); // Wait for the clock to go low

    bitbang_clearMISO();
}

uint8_t bitbang_receiveByteAsSlave(){
    // Set MISO so the master knows we're ready for a byte
    bitbang_setMISO();

    uint8_t received = 0x00;
    for (int8_t nextBit = 7; nextBit >= 0; nextBit--){
        while (!bitbang_getClock()); // Wait for the clock to go high
        if (bitbang_getMOSI()){
            received |= (0x01 << nextBit);
        }

        // Begin send diagnostic message
        if (bitbang_diagnosticMessage & (0x01 << nextBit)){
            bitbang_setMISO();
        }
        else{
            bitbang_clearMISO();
        }
        // End send diagnostic message

        while (bitbang_getClock()); // Wait for the clock to go low
    }

    bitbang_clearMISO();

    return received;
}