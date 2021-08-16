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
enum LED_States {LED_Start, LED_OnPress, LED_OnRelease, LED_OffPress, LED_OffRelease}LED_State;
void LEDToggle(){
	unsigned char A0 = PINA & 0x01;
	switch(LED_State){
		case LED_Start:
			LED_State = LED_OnPress;
			break;
		case LED_OnPress:
			if(A0){
				LED_State = LED_OnRelease;
			}
			break;
		case LED_OnRelease:
			if(!A0){
				LED_State = LED_OffPress;
			}
			break;
		case LED_OffPress:
			if(A0){
				LED_State  = LED_OffRelease;
			}
			break;
		case LED_OffRelease:
			if(!A0){
				LED_State = LED_OnPress;
			}
			break;
		default:
			LED_State = LED_Start;
			break;
			
	}
	switch(LED_State){
		case LED_Start:
			PORTB = 0x00;
			break;
		case LED_OnPress:
			PORTB = 0x01;
			break;
		case LED_OnRelease:
			PORTB = 0x02;
			break;
		case LED_OffPress:
			PORTB = 0x02;
			break;
		case LED_OffRelease:
			PORTB = 0x01;
			break;
		default:
			break;
		
	}
	
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;PORTA = 0xFF;
    DDRB = 0xFF;PORTB = 0x00;
    /* Insert your solution below */
    while (1) {
	LEDToggle();
    }
    return 1;
}
