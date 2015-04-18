/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

#include "nrf24l01+.h"
#include "nerf.h"
#include "tests.h"
#include "bitbang.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

void blink(){
    TRISC = 0x00;
    uint8_t glow = true;
    while (1){
        LATC = glow ? 0xff : 0x00;
        for (unsigned long long x = 0; x < 100000; x++);
        glow = !glow;
    }
}

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int8_t strcmp (const char* a, const char* b){
    int8_t next = -1;
    do{
        next++;
        if (a [next] < b [next]) return -1;
        if (a [next] > b [next]) return 1;
    }
    while (a [next] != 0 && b [next] != 0);

    return 0;
}

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */

    TRISA = 0x00; // These are the pull-ups apparently? And if you don't pull down (0x00) to start, some charge remains and it reads high if the input line is low when you use it as an input.  :P


    /*
    const uint8_t pinToTest = 2;
    
    TRISA = 0xff;
    TRISC = 0x00;
    while (true){
        if (PORTA & (0x01 << pinToTest)) LATC = 0xff;
        else LATC = 0x00;
    }
    */
    

    uint8_t myAddress [5];
    for (int x = 0; x < 5; x++) myAddress [x] = 0x01;
    
    uint8_t receivedMessage [9];


    //runTests();


    /*
    // set up ports to input on RA0 and RA2 (clock and MOSI)
    volatile unsigned char* trisA = &TRISA;
    volatile unsigned char* latA = &LATA;
    volatile unsigned char* portA = &PORTA;
    *trisA = 0x01 | (0x01 << 2);
    while (true){
        // set MISO high so the master will start banging
        *latA |= (0x01 << 1);
        for (unsigned int x = 0; x < 8; x++){
            while (!(*portA & 0x01)); // wait for clock to go high
                //if (x > 0){
                    if (*portA & (0x01 << 2)) *latA |= (0x01 << 1); // if MOSI is high, set MISO high
                    else *latA &= ~(0x01 << 1); // otherwise, set MISO low
                //}
            //if (x == 7) LATA &= ~(0x01 << 1);
            while (*portA & 0x01); // wait for clock to go low
        }
    }
    */

    /*
    bitbang_init (gpioProvider_createStandardGpioProvider (&TRISA, &LATA, &PORTA));
    while (true){
        uint8_t receivedByte = bitbang_receiveByteAsSlave();
        bitbang_setDiagnosticMessage (receivedByte);
    }
    */
    

    nerf_init (&TRISA, &LATA, &PORTA, &TRISB, &LATB, &PORTB, myAddress);

    while (true){
        nerf_receiveAndRespondToCommand();
    }

    /*
    nrf24l01p_init (8, &TRISB, &LATB, &PORTB, 0, 1, 2, 3, 4, 5);
    while(1)
    {
        if (ping){
            for (unsigned long long x = 0; x < 1000; x++){
                LATA = ~PORTA;
                nrf24l01p_sendMessage(otherAddress, message);
            }
            //uint8_t retransmitted = nrf24l01p_getRetransmittedPacketsCount();
        }
        else{
            LATA = 0x00;
            for (uint8_t x = 0; x < 9; x++) receivedMessage[x] = '\0';
            nrf24l01p_listenForMessage(myAddress);
            for (unsigned long long x = 0; x < 10000; x++){
                if (nrf24l01p_isMessageWaiting()){
                    nrf24l01p_getMessage(receivedMessage);
                    if (strcmp (message, receivedMessage) == 0){
                    //if (receivedMessage [0] == 'H' && receivedMessage [1] == 'E' && receivedMessage [2] == 'L' && receivedMessage [3] == 'L'
                    //        && receivedMessage [4] == 'O' && receivedMessage [5] == ' ' && receivedMessage [6] == 'Y' && receivedMessage [7] == 'O' && receivedMessage [8] == '\0'){
                    //if (receivedMessage [0] != 0){
                        LATC |= 0x01 << 3;
                    }
                    else{
                        LATC &= ~(0x01 << 3);
                    }
                }
            }
        }
        ping = !ping;
    }
    */
}

