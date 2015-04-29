/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    /* TODO Add clock switching code if appropriate.  */

    /* Typical actions in this function are to tweak the oscillator tuning
    register, select new clock sources, and to wait until new clock sources
    are stable before resuming execution of the main project. */

    // PIC18LF2525
    OSCCONbits.IRCF &= ~(0x07);     // Clear oscillator frequency bits
    //OSCCONbits.IRCF |= 0x07;      // 8000 KHz
    //OSCCONbits.IRCF |= 0x03;      //  500 KHz
    OSCCONbits.IRCF |= 0x00;      //   31 KHz

    // Double the clock rate?
    //OSCTUNEbits.PLLEN = 1;
    // Don't double the clock rate!
    OSCTUNEbits.PLLEN = 0;

    ADCON0bits.ADON = 0; // Disable A/D module
    ADCON1bits.PCFG0 = 1; // Make all A/D pins digital
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG3 = 1;

    CMCON = 0x07; // Capacitors for A/D?
}
