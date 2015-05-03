/******************************************************************************/
/* Main Files to Include                                                      */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

/******************************************************************************/
/* Configuration Bits                                                         */
/*                                                                            */
/* Refer to your Hi-Tech User Manual in the PICC installation directory       */
/* /doc folder for more information on filling in configuration bits.         */
/* In addition, configuration bit mnemonics can be found in your              */
/* PICC\version\include\<processor name>.h file for your device.  The XC8     */
/* compiler contains documentation on the configuration bit macros within     */
/* the compiler installation /docs folder in a file called                    */
/* pic18_chipinfo.html.                                                       */
/*                                                                            */
/* For additional information about what the hardware configurations mean in  */
/* terms of device operation, refer to the device datasheet.                  */
/*                                                                            */
/* A feature of MPLAB X is the 'Generate Source Code to Output' utility in    */
/* the Configuration Bits window.  Under Window > PIC Memory Views >          */
/* Configuration Bits, a user controllable configuration bits window is       */
/* available to Generate Configuration Bits source code which the user can    */
/* paste into this project.                                                   */
/*                                                                            */
/******************************************************************************/

/* TODO Fill in your configuration bits here using the config generator.      */

/*
// PIC12LF1840
#pragma config FOSC=INTOSC
//#pragma config OSC=RC
//#pragma config PBADEN=OFF
//#pragma config DEBUG=OFF
#pragma config PLLEN=ON
#pragma config LVP=OFF
#pragma config MCLRE=OFF
#pragma config WDTE=OFF
*/

// PIC18LF2525
#pragma config OSC=INTIO67
#pragma config PBADEN=OFF
#pragma config DEBUG=OFF
#pragma config LVP=OFF
#pragma config MCLRE=OFF
#pragma config WDT=OFF          // Watchdog timer off so we can control it from software
#pragma config WDTPS = 1024     // Watchdog Timer Postscale Select bits (1:1024 is about 4 seconds, regardless of oscillator settings)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)