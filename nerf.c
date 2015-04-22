#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "nerf.h"
#include "bitbang.h"
#include "nrf24l01+.h"


#define OVER_AND_OUT            0x00
#define SEND_A_MESSAGE          0x01
#define SET_TARGET_ADDRESS      0x02
#define RECEIVE_A_MESSAGE       0x03

#define NERF_ADDRESS_LENGTH     5
#define NERF_MAX_MESSAGE_LENGTH 8

uint8_t nerf_targetAddress [NERF_ADDRESS_LENGTH];
uint8_t nerf_outgoingMessageLength = 0;
uint8_t nerf_outgoingMessage [NERF_MAX_MESSAGE_LENGTH];
uint8_t nerf_incomingMessageLength = 0;
uint8_t nerf_incomingMessage [NERF_MAX_MESSAGE_LENGTH];

void nerf_clearOutgoingMessage(){
    for (unsigned x = 0; x < NERF_MAX_MESSAGE_LENGTH; x++) nerf_outgoingMessage [x] = 0x00;
    nerf_outgoingMessageLength = 0;
}
void nerf_clearIncomingMessage(){
    for (unsigned x = 0; x < NERF_MAX_MESSAGE_LENGTH; x++) nerf_incomingMessage [x] = 0x00;
    nerf_incomingMessageLength = 0;
}

void nerf_init (volatile unsigned char* hostTris, volatile unsigned char* hostLat, volatile unsigned char* hostPort,
        volatile unsigned char* nrfTris, volatile unsigned char* nrfLat, volatile unsigned char* nrfPort, uint8_t* myAddress){
    GpioProvider standardProvider = gpioProvider_createStandardGpioProvider (hostTris, hostLat, hostPort);
    bitbang_init (standardProvider);
    nrf24l01p_init (NERF_MAX_MESSAGE_LENGTH, nrfTris, nrfLat, nrfPort, 0, 1, 2, 3, 4, 5);
    nrf24l01p_listenForMessage (myAddress);
    for (unsigned x = 0; x < NERF_ADDRESS_LENGTH; x++) nerf_targetAddress [x] = 0x00;
    nerf_clearOutgoingMessage();
    nerf_clearIncomingMessage();
}
void nerf_commandSendMessage(){
    bitbang_setDiagnosticMessage (0x01);
    nerf_outgoingMessageLength = bitbang_receiveByteAsSlave();
    for (unsigned int x = 0; x < nerf_outgoingMessageLength; x++){
        nerf_outgoingMessage [x] = bitbang_receiveByteAsSlave();
    }
}
void nerf_commandReceiveMessage(){
    bitbang_setDiagnosticMessage (0x02);
    bitbang_sendByteAsSlave (nerf_incomingMessageLength);
    for (unsigned int x = 0; x < nerf_incomingMessageLength; x++){
        bitbang_sendByteAsSlave (nerf_incomingMessage [x]);
    }

    nerf_clearIncomingMessage();
}
void nerf_commandSetTargetAddress(){
    bitbang_setDiagnosticMessage (0x03);
    for (unsigned x = 0; x < NERF_ADDRESS_LENGTH; x++){
        nerf_targetAddress [x] = bitbang_receiveByteAsSlave();
    }
}
void nerf_receiveAndRespondToCommand(){
    bitbang_setDiagnosticMessage (0x04);
    uint8_t command = bitbang_receiveByteAsSlave();
    if (command == SEND_A_MESSAGE){
        nerf_commandSendMessage();
    }
    if (command == RECEIVE_A_MESSAGE){
        nerf_commandReceiveMessage();
    }
    if (command == SET_TARGET_ADDRESS){
        nerf_commandSetTargetAddress();
    }

    bitbang_setDiagnosticMessage (command);
    uint8_t overAndOut = bitbang_receiveByteAsSlave(); // Every command is followed by an OVER_AND_OUT.

    if (nerf_outgoingMessageLength > 0){
        nrf24l01p_sendMessage (nerf_targetAddress, nerf_outgoingMessage);
        nerf_clearOutgoingMessage();
    }
    if (nerf_incomingMessageLength == 0 && nrf24l01p_isMessageWaiting()){
        /*
        TRISC = 0x00;
        LATC |= 0x01;
        */
        nerf_incomingMessageLength = nrf24l01p_getMessage (nerf_incomingMessage);
        //LATC &= ~(0x01);
    }
}
