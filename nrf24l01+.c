#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "nrf24l01+.h"


volatile unsigned char* nrf24l01p_tris = &TRISB;
volatile unsigned char* nrf24l01p_lat = &LATB;
volatile unsigned char* nrf24l01p_port = &PORTB;

uint8_t nrf24l01p_misoPin = 0;
uint8_t nrf24l01p_sckPin = 1;
uint8_t nrf24l01p_cePin = 2;
uint8_t nrf24l01p_irqPin = 3;
uint8_t nrf24l01p_mosiPin = 4;
uint8_t nrf24l01p_csnPin = 5;

uint8_t nrf24l01p_payloadWidth = 8;
uint8_t nrf24l01p_addressWidth = 5;
uint8_t nrf24l01p_registerConfig = 0x00;

uint8_t nrf24l01p_currentTargetAddress[5];

void nrf24l01p_setClock(uint8_t enabled){
    if (enabled) *nrf24l01p_lat |= 0x01 << nrf24l01p_sckPin; // Set clock high
    else *nrf24l01p_lat &= ~(0x01 << nrf24l01p_sckPin); // Set clock low
}
void nrf24l01p_setData(uint8_t enabled){
    if (enabled) *nrf24l01p_lat |= 0x01 << nrf24l01p_mosiPin; // Set data high
    else *nrf24l01p_lat &= ~(0x01 << nrf24l01p_mosiPin); // Set data low
}
void nrf24l01p_startFrame(){
    nrf24l01p_setClock(false);
    nrf24l01p_setData(false);
    *nrf24l01p_lat &= ~(0x01 << nrf24l01p_csnPin); // Set CSN low
}
void nrf24l01p_endFrame(){
    *nrf24l01p_lat |= 0x01 << nrf24l01p_csnPin; // Set CSN high
}
uint8_t nrf24l01p_readWriteByte(uint8_t data){
    uint8_t shifted = 0x00;

    for (uint8_t x = 0x80; x != 0; x = x >> 1){
        if ((data & x) != 0) nrf24l01p_setData(true);
        else nrf24l01p_setData(false);
        nrf24l01p_setClock(true);
        if ((*nrf24l01p_port & (0x01 << nrf24l01p_misoPin)) != 0){
            shifted |= x; // Read the next bit
        }
        nrf24l01p_setClock(false);
    }

    return shifted;
}
void nrf24l01p_writeByte(uint8_t data){
    nrf24l01p_readWriteByte(data);
}
uint8_t nrf24l01p_readByte(){
    return nrf24l01p_readWriteByte(0xff);
}
void nrf24l01p_writeBytes(uint8_t* data, uint8_t byteCount){
    for (int8_t x = 0; x < byteCount; x++) nrf24l01p_writeByte(data[x]);
}
uint8_t nrf24l01p_readRegister(NRF24L01P_REGISTER reg, uint8_t* result){ // result array must be nrf24l01p_addressWidth bytes or more. returns length of result in bytes
    uint8_t byteCount = 1;

    if (reg == NRF24L01P_REGISTER_RX_ADDR_P0
        || reg == NRF24L01P_REGISTER_TX_ADDR
        || reg == NRF24L01P_REGISTER_RX_PW_P0
        || reg == NRF24L01P_REGISTER_RX_PW_P1
    ){
        byteCount = nrf24l01p_addressWidth;
    }

    nrf24l01p_startFrame();
        if (reg == NRF24L01P_REGISTER_STATUS){
            uint8_t status = nrf24l01p_readWriteByte(NRF24L01P_COMMAND_NOP);
            result[0] = status;
        }
        else{
            nrf24l01p_writeByte(reg);
            for (int8_t x = byteCount - 1; x >= 0; x--){
                result[x] = nrf24l01p_readByte();
            }
        }
    nrf24l01p_endFrame();
    return byteCount;
}
uint8_t nrf24l01p_getStatus(){
    uint8_t byte = 0x00;
    nrf24l01p_readRegister(NRF24L01P_REGISTER_STATUS, &byte);
    return byte;
}
void nrf24l01p_writeRegister(uint8_t reg, uint8_t* data, uint8_t dataLength){
    nrf24l01p_startFrame();
        nrf24l01p_writeByte(NRF24L01P_COMMAND_W_REGISTER | reg);
        for (int8_t x = dataLength - 1; x >= 0; x--){
            nrf24l01p_writeByte(data[x]);
        }
    nrf24l01p_endFrame();
}
void nrf24l01p_writeCommand(uint8_t command){
    nrf24l01p_startFrame();
        nrf24l01p_writeByte(command);
    nrf24l01p_endFrame();
}
void nrf24l01p_writeCommandWithData(uint8_t command, uint8_t* data, uint8_t dataLength){
    nrf24l01p_startFrame();
        nrf24l01p_writeByte(command);
        nrf24l01p_writeBytes(data, dataLength);
    nrf24l01p_endFrame();
}
uint8_t nrf24l01p_isMessageWaiting(){
    uint8_t status = 0x00;
    nrf24l01p_readRegister(NRF24L01P_REGISTER_FIFO_STATUS, &status);
    return !(status & 0x01);
}
void nrf24l01p_listenForMessage(uint8_t* address){ // message array must be large enough to hold payloadWidth in bytes.
    *nrf24l01p_lat &= ~(0x01 << nrf24l01p_cePin); // Set CE low to reset (not sure if this is necessary)
    nrf24l01p_registerConfig |= 0x01; // Set RX/TX to RX mode
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_CONFIG, &nrf24l01p_registerConfig, 1);
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_RX_PW_P0, &nrf24l01p_payloadWidth, 1);
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_RX_ADDR_P0, address, nrf24l01p_addressWidth);
    *nrf24l01p_lat |= (0x01 << nrf24l01p_cePin); // Set CE high to start listening
}
uint8_t nrf24l01p_getMessage(uint8_t* message){ // message array must be large enough to hold payloadWidth in bytes. returns length of payload in bytes
    nrf24l01p_startFrame();
        nrf24l01p_writeByte(NRF24L01P_COMMAND_R_RX_PAYLOAD);
        for (int8_t x = 0; x < nrf24l01p_payloadWidth; x++){
            message[x] = nrf24l01p_readByte(0x00);
        }
    nrf24l01p_endFrame();

    return nrf24l01p_payloadWidth;
}
uint8_t nrf24l01p_getRetransmittedPacketsCount(){
    uint8_t count = 0x00;
    nrf24l01p_readRegister(NRF24L01P_REGISTER_OBSERVE_TX, &count);
    count &= 0x0f;
    return count;
}
uint8_t nrf24l01p_getLostPacketsCount(){
    uint8_t count = 0x00;
    nrf24l01p_readRegister(NRF24L01P_REGISTER_OBSERVE_TX, &count);
    count &= 0xf0;
    return count >> 4;
}

void nrf24l01p_setTargetAddress(uint8_t* targetAddress){
    bool changed = false;
    for (uint8_t x = 0; x < nrf24l01p_addressWidth; x++){
        if (nrf24l01p_currentTargetAddress[x] != targetAddress[x]){
            nrf24l01p_currentTargetAddress[x] = targetAddress[x];
            changed = true;
        }
    }

    if (changed){
        nrf24l01p_writeRegister(NRF24L01P_REGISTER_TX_ADDR, targetAddress, nrf24l01p_addressWidth);
        nrf24l01p_writeRegister(NRF24L01P_REGISTER_RX_ADDR_P0, targetAddress, nrf24l01p_addressWidth); // Address needs to be written here for auto ACK
    }
}

void nrf24l01p_sendMessage(uint8_t* targetAddress, uint8_t* message){ // targetAddress must be at least nrf24l01p_addressWidth wide. payload array must be at least nrf24l01p_payloadWidth wide
    *nrf24l01p_lat &= ~(0x01 << nrf24l01p_cePin); // Set CE low to reset (not sure if this is necessary)
    uint8_t status = nrf24l01p_getStatus();
    status |= 0x30; // Clear TX_DS and MAX_RT flags
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_STATUS, &status, 1);
    nrf24l01p_registerConfig &= ~(0x01); // Set RX/TX to TX mode
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_CONFIG, &nrf24l01p_registerConfig, 1);
    nrf24l01p_writeCommand(NRF24L01P_COMMAND_FLUSH_TX);
    nrf24l01p_setTargetAddress(targetAddress);
    nrf24l01p_writeCommandWithData(NRF24L01P_COMMAND_W_TX_PAYLOAD, message, nrf24l01p_payloadWidth);
    *nrf24l01p_lat |= (0x01 << nrf24l01p_cePin); // Set CE high to start transmission
    //for (unsigned long long x = 0; x < 100; x++); // Hold CE high for 10uS to kickstart transmission
    *nrf24l01p_lat &= ~(0x01 << nrf24l01p_cePin); // Set CE low to end transmission
}

void nrf24l01p_init(uint8_t payloadWidth, volatile unsigned char* tris, volatile unsigned char* lat, volatile unsigned char* port,
        uint8_t misoPin, uint8_t sckPin, uint8_t cePin, uint8_t irqPin, uint8_t mosiPin, uint8_t csnPin, uint8_t maxRetriesPerPacket){
    nrf24l01p_tris = tris;
    nrf24l01p_lat = lat;
    nrf24l01p_port = port;
    nrf24l01p_misoPin = misoPin;
    nrf24l01p_sckPin = sckPin;
    nrf24l01p_cePin = cePin;
    nrf24l01p_irqPin = irqPin;
    nrf24l01p_mosiPin = mosiPin;
    nrf24l01p_csnPin = csnPin;

    nrf24l01p_payloadWidth = payloadWidth;

    *nrf24l01p_tris = (0x01 << nrf24l01p_misoPin) | (0x01 << nrf24l01p_irqPin);
    *nrf24l01p_lat = (0x01 << nrf24l01p_csnPin);

    //for (unsigned long long x = 0; x < 100; x++); // Delay for power-on
    if (maxRetriesPerPacket > 15) maxRetriesPerPacket = 15;

    nrf24l01p_registerConfig = (0x01 << 3) | (0x01 << 2) | (0x01 << 1); // PWR_UP, CRC 2 bytes
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_CONFIG, &nrf24l01p_registerConfig, 1);

    uint8_t byte = 0x27; // 250 Kbps data rate, max Tx power, setup LNA (low noise amplifier) gain
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_RF_SETUP, &byte, 1);

    byte = 0x10; // Set delay of 500uS between retries
    byte |= maxRetriesPerPacket; // Retransmit up to maxRetriesPerPacket times
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_SETUP_RETR, &byte, 1);
}

void nrf24l01p_sleep(){
    nrf24l01p_registerConfig &= ~(0x01 << 1); // Clear PWR_UP bit
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_CONFIG, &nrf24l01p_registerConfig, 1);
}
void nrf24l01p_wake(){
    nrf24l01p_registerConfig |= (0x01 << 1); // Set PWR_UP bit
    nrf24l01p_writeRegister(NRF24L01P_REGISTER_CONFIG, &nrf24l01p_registerConfig, 1);
}