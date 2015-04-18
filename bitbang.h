#ifndef BITBANG__H
#define	BITBANG__H

#ifdef	__cplusplus
extern "C" {
#endif

#include "gpioProvider.h"

    
#define BITBANG_CLK  0
#define BITBANG_MISO 1
#define BITBANG_MOSI 2

// Assumes that on LAT, 0 = clock, 1 = MOSI, 2 = MISO
void bitbang_init (GpioProvider portProvider);
void bitbang_sendByteAsSlave (const uint8_t byte);
uint8_t bitbang_receiveByteAsSlave();
void bitbang_setDiagnosticMessage (const uint8_t byte);


#ifdef	__cplusplus
}
#endif

#endif	/* BITBANG__H */

