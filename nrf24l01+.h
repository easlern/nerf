/* 
 * File:   nrf24l01+.h
 * Author: easlern
 *
 * Created on July 23, 2014, 9:14 PM
 *
 * Version:
 * 1.0 2014?        Created
 */

#ifndef NRF24L01__H
#define	NRF24L01__H

#ifdef	__cplusplus
extern "C" {
#endif


// 1 byte each unless otherwise indicated
// Least significant byte first for all registers unless otherwise indicated
#define NRF24L01P_REGISTER uint8_t
#define NRF24L01P_REGISTER_CONFIG       0x00
#define NRF24L01P_REGISTER_EN_AA        0x01
#define NRF24L01P_REGISTER_EN_RXADDR    0x02
#define NRF24L01P_REGISTER_SETUP_AW     0x03
#define NRF24L01P_REGISTER_SETUP_RETR   0x04
#define NRF24L01P_REGISTER_RF_CH        0x05
#define NRF24L01P_REGISTER_RF_SETUP     0x06
#define NRF24L01P_REGISTER_STATUS       0x07
#define NRF24L01P_REGISTER_OBSERVE_TX   0x08
#define NRF24L01P_REGISTER_RPD          0x09
#define NRF24L01P_REGISTER_RX_ADDR_P0   0x0a // 5 bytes. write number of bytes set in SETUP_AW
#define NRF24L01P_REGISTER_TX_ADDR      0x10 // 5 bytes
#define NRF24L01P_REGISTER_RX_PW_P0     0x11 // 5 bytes
#define NRF24L01P_REGISTER_RX_PW_P1     0x12 // 5 bytes
#define NRF24L01P_REGISTER_RX_PW_P2     0x13
#define NRF24L01P_REGISTER_RX_PW_P3     0x14
#define NRF24L01P_REGISTER_RX_PW_P4     0x15
#define NRF24L01P_REGISTER_RX_PW_P5     0x16
#define NRF24L01P_REGISTER_FIFO_STATUS  0x17
#define NRF24L01P_REGISTER_DYN_PD       0x1c
#define NRF24L01P_REGISTER_FEATURE      0x1d

#define NRF24L01P_COMMAND_W_REGISTER    0x20
#define NRF24L01P_COMMAND_R_RX_PAYLOAD  0x61
#define NRF24L01P_COMMAND_W_TX_PAYLOAD  0xa0
#define NRF24L01P_COMMAND_FLUSH_TX      0xe1
#define NRF24L01P_COMMAND_NOP           0xff

    

void nrf24l01p_init(uint8_t payloadWidth, volatile unsigned char* tris, volatile unsigned char* lat, volatile unsigned char* port,
        uint8_t misoPin, uint8_t sckPin, uint8_t cePin, uint8_t irqPin, uint8_t mosiPin, uint8_t csnPin);

void nrf24l01p_sendMessage(uint8_t* targetAddress, uint8_t* message);
void nrf24l01p_listenForMessage(uint8_t* address);
uint8_t nrf24l01p_isMessageWaiting();
uint8_t nrf24l01p_getMessage(uint8_t* message);
uint8_t nrf24l01p_getRetransmittedPacketsCount();
uint8_t nrf24l01p_getLostPacketsCount();



#ifdef	__cplusplus
}
#endif

#endif	/* NRF24L01__H */

