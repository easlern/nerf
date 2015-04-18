#include "gpioProviderTests.h"
#include "testing.h"


void gpioProviderTests_standardGetPin(){
    uint8_t tris;
    uint8_t lat;
    uint8_t port = 0x00;

    GpioProvider provider = gpioProvider_createStandardGpioProvider (&tris, &lat, &port);

    port = 0xff;

    testing_shouldEqual (1, provider.getPin (0));
    testing_shouldEqual (1, provider.getPin (7));
    testing_shouldEqual (1, provider.getPin (4));

    port = 0x00;

    testing_shouldEqual (0, provider.getPin (0));
    testing_shouldEqual (0, provider.getPin (7));
    testing_shouldEqual (0, provider.getPin (4));

    port = 0x01 << 2;

    testing_shouldEqual (0, provider.getPin (1));
    testing_shouldEqual (1, provider.getPin (2));
    testing_shouldEqual (0, provider.getPin (3));
}
void gpioProviderTests_standardSetPin(){
    uint8_t tris;
    uint8_t lat = 0x00;
    uint8_t port;

    GpioProvider provider = gpioProvider_createStandardGpioProvider (&tris, &lat, &port);

    provider.setPin (0);
    testing_shouldEqual ((0x01 << 0), lat);
    provider.setPin (7);
    testing_shouldEqual ((0x01 << 0) | (0x01 << 7), lat);
}
void gpioProviderTests_standardClearPin(){
    uint8_t tris;
    uint8_t lat = 0xff;
    uint8_t port;

    GpioProvider provider = gpioProvider_createStandardGpioProvider (&tris, &lat, &port);

    provider.clearPin (0);
    testing_shouldEqual (~(0x01 << 0), lat);
    provider.clearPin (7);
    testing_shouldEqual (~((0x01 << 0) | (0x01 << 7)), lat);
}
void gpioProviderTests_standardSetupPin(){
    uint8_t tris = 0x00;
    uint8_t lat;
    uint8_t port;

    GpioProvider provider = gpioProvider_createStandardGpioProvider (&tris, &lat, &port);

    provider.setupPin (0, GPIOPROVIDER_INPUT);
    testing_shouldEqual (0x01, tris);
    provider.setupPin (0, GPIOPROVIDER_OUTPUT);
    testing_shouldEqual (0x00, tris);
    provider.setupPin (2, GPIOPROVIDER_INPUT);
    testing_shouldEqual (0x04, tris);

    tris = 0xff;
    provider.setupPin (0, GPIOPROVIDER_OUTPUT);
    testing_shouldEqual (~0x01, tris);
    provider.setupPin (7, GPIOPROVIDER_OUTPUT);
    testing_shouldEqual (~((0x01 << 0) | (0x01 << 7)), tris);
}

uint8_t gpioProviderTests_functionSetCalledWithPin = 0;
void gpioProviderTests_functionSet (uint8_t pin){
    gpioProviderTests_functionSetCalledWithPin = pin;
}
uint8_t gpioProviderTests_functionGetCalledWithPin = 0;
uint8_t gpioProviderTests_functionGet (uint8_t pin){
    gpioProviderTests_functionGetCalledWithPin = pin;
    return 0;
}
uint8_t gpioProviderTests_functionClearCalledWithPin = 0;
void gpioProviderTests_functionClear (uint8_t pin){
    gpioProviderTests_functionClearCalledWithPin = pin;
}
uint8_t gpioProviderTests_functionSetupCalledWithPin = 0;
uint8_t gpioProviderTests_functionSetupCalledWithMode = 0;
void gpioProviderTests_functionSetup (uint8_t pin, uint8_t mode){
    gpioProviderTests_functionSetupCalledWithPin = pin;
    gpioProviderTests_functionSetupCalledWithMode = mode;
}
void gpioProviderTests_createGpioProvider(){
    GpioProvider provider = gpioProvider_createGpioProvider (&gpioProviderTests_functionGet, &gpioProviderTests_functionClear, &gpioProviderTests_functionSet, &gpioProviderTests_functionSetup);
    testing_shouldEqual ((uint8_t) &gpioProviderTests_functionGet, (uint8_t) provider.getPin);
    testing_shouldEqual ((uint8_t) &gpioProviderTests_functionClear, (uint8_t) provider.clearPin);
    testing_shouldEqual ((uint8_t) &gpioProviderTests_functionSet, (uint8_t) provider.setPin);
    testing_shouldEqual ((uint8_t) &gpioProviderTests_functionSetup, (uint8_t) provider.setupPin);
}

void gpioProviderTests_run(){
    gpioProviderTests_standardGetPin();
    gpioProviderTests_standardSetPin();
    gpioProviderTests_standardClearPin();
    gpioProviderTests_standardSetupPin();
    gpioProviderTests_createGpioProvider();
}