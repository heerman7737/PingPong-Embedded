/*	Author:Giang To lab
 *  Partner(s) Name: none
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Lock_States {Lock_Start, Lock_Locked,Lock_Unlocked, Lock_Press, Lock_Release} Lock_State;




void Lock_Toggle(){

        unsigned char x = PINA & 0x01;
	unsigned char y = PINA & 0x02;
	unsigned char z = PINA & 0x04;
	unsigned char in = PINA & 0x80;

	switch(Lock_State){

		case Lock_Start:
			Lock_State = Lock_Locked;
			break;

		case Lock_Locked:

			if(!x && !y && z && !in){
				Lock_State = Lock_Press;
			}
			else{
				Lock_State = Lock_Locked;
			}

			break;
	
		case Lock_Unlocked:
			if(in){
				Lock_State = Lock_Locked;
			}
			else {
				Lock_State = Lock_Unlocked;
			}

			break;

		case Lock_Press:

			if(in){
				Lock_State = Lock_Locked;
			}
			else if(!x && !y && !z && !in){
				Lock_State =  Lock_Release;
			}
			else if(y){
				Lock_State = Lock_Locked;
			}
			else {
				Lock_State = Lock_Press;
			}
			break;

		case Lock_Release:

			if(in){
				Lock_State =  Lock_Locked;
			}
			else if(!x && y && !z && !in){
				Lock_State = Lock_Unlocked;
			}
			else {
				Lock_State = Lock_Release;
			}
			break;

		default:
			Lock_State = Lock_Start;
			break;

		}
	switch(Lock_State){
		case Lock_Start:
			break;
		case Lock_Locked:
			PORTB = 0x00;
			break;
		case Lock_Unlocked:
			PORTB = 0x01;
			break;
		case Lock_Press:
			break;
		case Lock_Release:
			break;
	}

}	

int main(void) {

    /* Insert DDR and PORT initializations */

    DDRA = 0x00;PORTA = 0xFF;
    DDRB = 0xFF;PORTB = 0x00;

    Lock_State = Lock_Start;

    /* Insert your solution below */

    while (1) {
	Lock_Toggle();
    }

    return 1;

}



