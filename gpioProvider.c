#include "gpioProvider.h"


GpioProvider gpioProvider_createGpioProvider (uint8_t (*getPin)(uint8_t), void (*clearPin)(uint8_t), void (*setPin)(uint8_t), void (*setupPin)(uint8_t, uint8_t)){
    GpioProvider provider;
    provider.getPin = getPin;
    provider.clearPin = clearPin;
    provider.setPin = setPin;
    provider.setupPin = setupPin;

    return provider;
}

volatile unsigned char* gpioProvider_standardTris = 0;
volatile unsigned char* gpioProvider_standardLat = 0;
volatile unsigned char* gpioProvider_standardPort = 0;
uint8_t gpioProvider_standardGetPin (uint8_t pin){
    uint8_t ored = *gpioProvider_standardPort & (0x01 << pin);
    uint8_t shifted = ored >> pin;
    return shifted;
}
void gpioProvider_standardSetPin (uint8_t pin){
    *gpioProvider_standardLat |= (0x01 << pin);
}
void gpioProvider_standardClearPin (uint8_t pin){
    *gpioProvider_standardLat &= ~(0x01 << pin);
}
void gpioProvider_standardSetupPin (uint8_t pin, uint8_t mode){
    uint8_t clearMask = ~(0x01 << pin);
    uint8_t setMask = mode << pin;

    uint8_t tris = *gpioProvider_standardTris;
    tris &= clearMask;
    tris |= setMask;

    *gpioProvider_standardTris = tris;
}

GpioProvider gpioProvider_createStandardGpioProvider (volatile unsigned char* tris, volatile unsigned char* lat, volatile unsigned char* port){
    gpioProvider_standardTris = tris;
    gpioProvider_standardLat = lat;
    gpioProvider_standardPort = port;
    
    return gpioProvider_createGpioProvider (&gpioProvider_standardGetPin, &gpioProvider_standardClearPin, &gpioProvider_standardSetPin, &gpioProvider_standardSetupPin);
}
