#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"
 
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

int first=0;
int next=0;
int expected=0;

int checkSum(struct pkt*);
int isCorrupt(struct pkt*);

#define ready 0
#define waiting 1
#define max 99999999
#define window 8

struct pkt lastAck;
struct pkt* pkts[max];

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */

void A_output(struct msg message){
	pkts[next]=(struct pkt*)malloc(sizeof(struct pkt));
	memcpy(pkts[next]->payload, message.data, MESSAGE_LENGTH);
	pkts[next]->seqnum=next;
	pkts[next]->acknum=0;
	pkts[next]->checksum=0;
	pkts[next]->checksum=checkSum(pkts[next]);
	if(next<(first+window)){
		tolayer3(AEntity, *pkts[next]);
		if(first==next){
			startTimer(AEntity, 5000);
		}
	}
	next++;
	return;
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
	if(isCorrupt(&packet)){
		return;
	}
	if(packet.acknum==1){
		if(first>packet.seqnum){
			return;
		}
		if((packet.seqnum++)==next){
			stopTimer(AEntity);
			return;
		}
		first=packet.seqnum++;
		if(!(first==next)){
			stopTimer(AEntity);
			startTimer(AEntity, 5000);
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
	for(int i=first; i<next, i<(first+window), i<max; i++){
		if(pkts[i]==NULL){
			continue;
		}
		tolayer3(AEntity, *pkts[i]);
	}
	startTimer(AEntity, 5000);
	return;
}  

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */

void A_init(){

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

void B_input(struct pkt packet){
	struct msg message;
	if(isCorrupt(&packet)){
		return;
	}
	if(!isCorrupt(&packet)&&packet.seqnum!=expected){
		tolayer3(BEntity, lastAck);
		return;
	}
	if(!isCorrupt(&packet)&&packet.seqnum==expected){
		strcpy(message.data, packet.payload);
		tolayer5(BEntity, message);
		expected++;
		packet.checksum=0;
		packet.acknum=0;
		packet.checksum=checkSum(&packet);
		tolayer3(BEntity, packet);
		lastAck=packet;
		return;
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
	lastAck.acknum=1;
	lastAck.seqnum=0;
	lastAck.checksum=0;
	strncpy(lastAck.payload, "\0", 20);
	lastAck.checksum=checkSum(&lastAck);
}

unsigned short calcCRC(char* buff, int size){
    unsigned short cword = 0;
    unsigned short ch;
    for (int i = 0; i < size; i++){
        ch = buff[i] << 8;
        for (int j = 0; j < 8; j++){
            if ((ch & 0x8000)^(cword&0x8000)){
                cword=(cword <<= 1)^4129;
            }
            else{
                cword<<= 1;
            }
            ch<<= 1;
        }
    }
    return cword;
}

int checkSum(struct pkt* somePacket){
  	int sum = 0;
  	for(int i=0; i<20;){
  		sum+=((int)somePacket->payload[i])*(++i);
	}
  	sum+=somePacket->seqnum*(somePacket->payload[1]);
  	sum+=somePacket->acknum*somePacket->payload[2];
  	struct pkt temp=*somePacket; 
  	char tempChar[sizeof(temp)];
  	temp.checksum=0;
  	memcpy(tempChar, &temp, sizeof(temp));
  	sum=calcCRC(tempChar, sizeof(tempChar)); 
  	return sum;
}

int isCorrupt( struct pkt* somePacket){
  	if(checkSum(somePacket) == somePacket->checksum){
    	return 0;
	}
  	return 1;
}