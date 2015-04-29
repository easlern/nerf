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

void sleepForABit(){
    nerf_sleep();
    SWDTEN = 1; // Enable the watchdog timer
    CLRWDT();   // Reset the watchdog timer
    SLEEP();    // Go to sleep until the watchdog timer goes off, then resume execution
    SWDTEN = 0; // Disable the watchdog timer
    nerf_wake();
}

uint8_t senderAddress [5];
uint8_t receiverAddress [5];
uint8_t message [9] = "TESTING!";
void loopSendingTestMessage(){
    TRISC = 0x00;

    message [8] = 0x00;
    nrf24l01p_init (8, &TRISB, &LATB, &PORTB, 0, 1, 2, 3, 4, 5);
    while(1)
    {
        LATC = 0xff;
        nrf24l01p_sendMessage(receiverAddress, message);
        LATC = 0x00;
        
        sleepForABit();
    }
}

void loopReceivingTestMessage(){
    TRISC = 0x00;
    LATC = 0x00;

    nrf24l01p_init (8, &TRISB, &LATB, &PORTB, 0, 1, 2, 3, 4, 5);
    nrf24l01p_listenForMessage(receiverAddress);
    while (1)
    {
        for (unsigned int x = 0; x < 9; x++) message [x] = 0x00;
        if (nrf24l01p_isMessageWaiting()){
            nrf24l01p_getMessage (message);
            if (message [0] == 'T') LATC = 0xff;
        }
    }
}

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    TRISA = 0x00; // If you use RA2 as an input without first setting it as an output, some charge lingers and the input reads high even if the pin is tied to ground. I have a theory as to why this happens but all I really know for sure is that setting it as output first seems to prevent the weird thing from happening.

    for (int x = 0; x < 5; x++) senderAddress [x] = 0x02;
    for (int x = 0; x < 5; x++) receiverAddress [x] = 0x01;

    
    uint8_t receivedMessage [9];

    //runTests();

    //loopReceivingTestMessage();
    loopSendingTestMessage();


    nerf_init (&TRISA, &LATA, &PORTA, &TRISB, &LATB, &PORTB, receiverAddress);

    while (true){
        nerf_receiveAndRespondToCommand();
    }
}

