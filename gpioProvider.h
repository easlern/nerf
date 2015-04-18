/* 
 * File:   gpioProvider.h
 * Author: easlern
 *
 * Created on January 19, 2015, 8:27 PM
 */

#ifndef GPIOPROVIDER_H
#define	GPIOPROVIDER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */


#define GPIOPROVIDER_OUTPUT 0
#define GPIOPROVIDER_INPUT  1


typedef struct{
    uint8_t (*getPin)(uint8_t);
    void (*setPin)(uint8_t);
    void (*clearPin)(uint8_t);
    void (*setupPin)(uint8_t, uint8_t);
} GpioProvider;

GpioProvider gpioProvider_createGpioProvider (uint8_t (*getPin)(uint8_t), void (*clearPin)(uint8_t), void (*setPin)(uint8_t), void (*setupPin)(uint8_t, uint8_t));
GpioProvider gpioProvider_createStandardGpioProvider (volatile unsigned char* tris, volatile unsigned char* lat, volatile unsigned char* port);


#ifdef	__cplusplus
}
#endif

#endif	/* GPIOPROVIDER_H */

