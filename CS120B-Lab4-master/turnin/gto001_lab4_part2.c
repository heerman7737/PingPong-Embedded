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

enum Count_States {Count_Start, Count_Init, Count_Increment, Count_Decrement, Count_Reset}  Count_State;

//unsigned char count =0x00;

unsigned char tmpA = 0x00;


void Count_Toggle(){

	tmpA = PINA & 0x03;

	switch(Count_State){

		case Count_Start:

			PORTC = 7;
			Count_State = Count_Init;
			break;

		case Count_Init:

			if(tmpA == 0x01){
				Count_State = Count_Increment;
				if(PORTC<9){
					PORTC++;
				}
				else if(PORTC ==9){
					PORTC = 9;
				}
				else{
					PORTC = 0;
				}
			}
			else if(tmpA == 0x02){
				Count_State = Count_Decrement;

				if(PORTC >0){
					PORTC--;
				}

				else{
					PORTC = 0;
				}

			}

			else if(tmpA == 0x03){
				Count_State = Count_Reset;
				PORTC = 0;
			}

			else{
				Count_State = Count_Init;
			}

			break;
	
		case Count_Increment:
			if(tmpA == 0x01){
				Count_State = Count_Increment;
			}
			else if(tmpA ==0x03){
				Count_State =Count_Reset;
				PORTC = 0;
			}	
			else {
				Count_State = Count_Init;
				
			}

			break;

		case Count_Decrement:

			if(tmpA == 0x02){
				Count_State = Count_Decrement;
			}
			else if(tmpA ==0x03){
				Count_State =Count_Reset;
				PORTC=0;
			}
			else{
				Count_State = Count_Init;
				
			}
			break;

		case Count_Reset:

			if(tmpA == 0x03){
				Count_State = Count_Reset;
			}
			else{
				Count_State = Count_Init;
			}
			break;

		default:
			Count_State = Count_Start;
			break;

		}

}	

int main(void) {

    /* Insert DDR and PORT initializations */

    DDRA = 0x00;PORTA = 0xFF;
    DDRC = 0xFF;PORTC = 0x00;

    Count_State = Count_Start;

    /* Insert your solution below */

    while (1) {
	Count_Toggle();
    }

    return 1;

}



