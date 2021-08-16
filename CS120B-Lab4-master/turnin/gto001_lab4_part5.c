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

unsigned char button[4] = {0x04,0x01,0x02,0x01};
int index = 0;

unsigned char tmpA;
unsigned char tmpB;
void Lock_Toggle(){
   tmpA = PINA ;
   

	
   switch(Lock_State){
	   case Lock_Start:
		   Lock_State =  Lock_Locked;
		   break;
	   case Lock_Locked:
		   
		   if(tmpA == button[index]&&index<=3){
		   	Lock_State =  Lock_Locked;
			index++;
		   }
		   else if(tmpA == button[index] && index ==4){
		   	Lock_State =Lock_Unlocked;
		   }
		   else if(tmpA == 0x00){
		   	Lock_State =  Lock_Locked;
		   }
		   
		   else{
		   	Lock_State = Lock_Locked;
		        	
		   }
		   if(tmpA & 0x80 ==0x80){
		   	PORTB =0x00;
			Lock_State = Lock_Press;
		   }
		   break;
	   case Lock_Press:
		   break;
	   	
		   
   }     
   switch(Lock_State){
	case Lock_Locked:
	   PORTB =0x00;
		break;
	case Lock_Unlocked:
	   PORTB =0x01;
	   	break;
	}

//if(PINA  & 0x80 ==0x80&& PORTB == 0x01){
//	PORTB =0x00;
//	Lock_State =Lock_Locked;
//}
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



