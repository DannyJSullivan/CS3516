#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"
#include "queue.h"
 
/* ***************************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for Project 2, unidirectional or bidirectional
   data transfer protocols from A to B and B to A.
   Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets may be delivered out of order.

   Compile as gcc -g project2.c student2.c -o p2
**********************************************************************/



/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
/* 
 * The routines you will write are detailed below. As noted above, 
 * such procedures in real-life would be part of the operating system, 
 * and would be called by other procedures in the operating system.  
 * All these routines are in layer 4.
 */

int statusA;
struct pkt packetA;
int lastACK;

void makeACK(struct pkt*);
int isCorrupt(struct pkt*);
int checkSum(struct pkt*);
unsigned short crcCalc(char* buff, int size);

#define ready 0
#define wait 1

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */

void A_output(struct msg message){
    if(statusA==wait){
        queue(message);
        return;
    }
    packetA.seqnum=(packetA.seqnum+1)%2;
    packetA.acknum=0;
    packetA.checksum=0;
    strncpy(packetA.payload,message.data, 20);
    packetA.checksum=checkSum(&packetA);
    tolayer3(AEntity, packetA);
    statusA=wait;
    startTimer(AEntity, 1000);

}

/*
 * Just like A_output, but residing on the B side.  USED only when the 
 * implementation is bi-directional.
 */

void B_output(struct msg message){

}

/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */

void A_input(struct pkt packet){
    if(isCorrupt(&packet)||packet.acknum!=packetA.seqnum){

    }
    else{
        stopTimer(AEntity);
        statusA=ready;
        if(emptyBuff()==0){
            struct msg newFront=firstStatus();
            A_output(newFront);
            unqueue();
        }
    }
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */

void A_timerinterrupt(){
    stopTimer(AEntity);
    tolayer3(AEntity, packetA);
    startTimer(AEntity, 1000);
}  

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */

void A_init(){
    statusA=ready;
    packetA.seqnum=1;
}

/* 
 * Note that with simplex transfer from A-to-B, there is no routine  B_output() 
 */

/*
 * B_input(packet),where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the A-side (i.e., as a result 
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side. 
 * packet is the (possibly corrupted) packet sent from the A-side.
 */

void B_input(struct pkt packet) {
    if (isCorrupt(&packet)||packet.seqnum!=((lastACK+1)%2)){
        makeACK(&packet);
        tolayer3(BEntity, packet);
    }
    else{
        struct msg message;
        strcpy(message.data, packet.payload);
        tolayer5(BEntity, message);
        lastACK=(lastACK+1)%2;
        makeACK(&packet);
        tolayer3(BEntity, packet);
    }
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */

void  B_timerinterrupt(){

}

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */

void B_init(){
    lastACK = 1;
}

unsigned short crcCalc(char* buff, int size){
    unsigned short cword = 0;
    unsigned short ch;
    for (int i=0; i<size; i++){
        ch = buff[i]<<8;
        for(int j=0; j<8; j++){
            if((ch & 0x8000)^(cword&0x8000)){
                cword=(cword<<=1)^4129;
            }
            else{
                cword<<=1;
            }
            ch<<=1;
        }
    }
    return cword;
}


int checkSum(struct pkt* somePkt){
    int sum=0;
    struct pkt temp=*somePkt;
    char tempChar[sizeof(temp)];
    temp.checksum=0;
    memcpy(tempChar, &temp, sizeof(temp));
    sum=crcCalc(tempChar, sizeof(tempChar));
    return sum;
}


void makeACK(struct pkt* somePkt){
    somePkt->acknum=lastACK;
    somePkt->seqnum=1;
    somePkt->checksum=0;
    somePkt->checksum = checkSum(somePkt);
}

int isCorrupt(struct pkt* somePkt){
    if(checkSum(somePkt) == somePkt->checksum){
        return 0;
    }
    else{
        return 1;
    }
}