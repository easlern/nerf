/* 
 * File:   nerf.h
 * Author: easlern
 *
 * Created on December 22, 2014, 9:24 PM
 */

#ifndef NERF_H
#define	NERF_H

#ifdef	__cplusplus
extern "C" {
#endif


void nerf_init (volatile unsigned char* hostTris, volatile unsigned char* hostLat, volatile unsigned char* hostPort,
        volatile unsigned char* nrfTris, volatile unsigned char* nrfLat, volatile unsigned char* nrfPort, uint8_t* myAddress);
void nerf_receiveAndRespondToCommand();
void nerf_sleep();
void nerf_wake();


#ifdef	__cplusplus
}
#endif

#endif	/* NERF_H */

