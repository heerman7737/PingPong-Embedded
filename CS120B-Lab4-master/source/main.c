/*	Author:Giang To lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *	Demo: https://drive.google.com/file/d/1w5O1hmxl_b8QWh6Zy8rk2VK0wU7F1Ika/view?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short input = 0x00;
volatile unsigned char TimerFlag = 0;
void Set_A2D_Pin(unsigned char pinNum){
	ADMUX = (pinNum <=0x07) ? pinNum:ADMUX;
	static unsigned char i = 0;
	for(i=0;i<15;i++){
		asm("nop");
	}

}
void TimerISR() { TimerFlag = 1;}
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff() {
	TCCR1B = 0x00;
}
ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet (unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
void A2D_init(){
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE);

}

void transmit_data(unsigned char data1,unsigned char data2){

	int i;
	int u;




		for(i=0;i<8;i++){
			PORTC =0x08;
			PORTC |= ((data1>>i) & 0x01);
			PORTC |= 0x02;
		}
		PORTC |=0x04;
		PORTC =0x00;


		for(u = 0;u<8;u++){
			PORTD = 0x08;
			PORTD |= ((data2>>u)& 0x01);
			PORTD |= 0x02;
		}
		PORTD |= 0x04;
		PORTD = 0x00;


}
unsigned char P1_holder[2] = {0x1C,0x01};
unsigned char P1_corner_left= 0x10;
unsigned char P1_center = 0x08;
unsigned char P1_corner_right = 0x04;
enum P1_states { P1_init, P1_right,P1_left} P1_state;

unsigned char AI_holder[2] = {0x1C,0x10};
unsigned char AI_corner_left = 0x10;
unsigned char AI_center = 0x08;
unsigned char AI_corner_right = 0x04;
enum AI_states { AI_init, AI_right,AI_left} AI_state;

unsigned char B_holder[2] = {0x08,0x04};
enum B_states {B_start,B_init,B_up,B_down,B_di_ri_up,B_di_ri_down,B_di_le_up,B_di_le_down,B_miss1,B_miss2} B_state;

void P1_Toggle(){
	input =ADC;
	switch(P1_state){
		case P1_init:
			if(input<200){
				P1_state = P1_left;
			}
			else if(input>800){
				P1_state = P1_right;
			}
			else{
				P1_state = P1_init;
			}
			break;
		case P1_left:
			P1_state = P1_init;
			break;
		case P1_right:
			P1_state = P1_init;
			break;
		
	}
	switch(P1_state){
		case P1_init:
			break;
		case P1_left:
			if(P1_corner_right == 0x20){
				P1_corner_left = 0x80;
				P1_center = 0x40;
				P1_corner_right = 0x20;
			}
			else{
				P1_corner_left = P1_corner_left<<1;
				P1_center = P1_center <<1;
				P1_corner_right = P1_corner_right<<1;
			}
			break;
		case P1_right:
			if(P1_corner_right == 0x01){
				P1_corner_right = 0x01;
				P1_center = 0x02;
				P1_corner_left  = 0x04;
			}
			else{
				P1_corner_right = P1_corner_right>>1;
				P1_center = P1_center >>1;
				P1_corner_left = P1_corner_left>>1;
			}
			break;
	}

	P1_holder[0] = P1_corner_left|P1_center|P1_corner_right;

}
enum P2_states { P2_init, P2_right, P2_left,P2_wait} P2_state;
void P2_Toggle(){
	unsigned char tpA = ~PINA & 0x0C;
	switch(P2_state){
		case P2_init:
			if(tpA == 0x04){
				P2_state = P2_left;
			}
			else if(tpA == 0x08){
				P2_state = P2_right;
			}
			else{
				P2_state = P2_init;
			}
			break;
		case P2_left:
			P2_state = P2_wait;
			break;
		case P2_right:
			P2_state = P2_wait;
			break;
		case P2_wait:
			if(tpA == 0x00){
				P2_state = P2_init;
			}
			else{
				P2_state = P2_wait;
			} 
			break;
		
	}
	switch(P2_state){
		case P2_init:
			break;
		case P2_left:
			if(AI_corner_right == 0x20){
				AI_corner_left = 0x80;
				AI_center = 0x40;
				AI_corner_right = 0x20;
			}
			else{
				AI_corner_left = AI_corner_left<<1;
				AI_center = AI_center <<1;
				AI_corner_right = AI_corner_right<<1;
			}
			break;
		case P2_right:
			if(AI_corner_right == 0x01){
				AI_corner_right = 0x01;
				AI_center = 0x02;
				AI_corner_left  = 0x04;
			}
			else{
				AI_corner_right = AI_corner_right>>1;
				AI_center = AI_center >>1;
				AI_corner_left = AI_corner_left>>1;
			}
			break;
	}

	AI_holder[0] = AI_corner_left|AI_center|AI_corner_right;

}
//unsigned char AI_holder[2] = {0x1C,0x10};
//unsigned char AI_corner_left = 0x10;
//unsigned char AI_center = 0x08;
//unsigned char AI_corner_right = 0x04;
//enum AI_states { AI_init, AI_right,AI_left,AI_wait} AI_state;
void AI_Toggle(int chance){

	switch(AI_state){
		case AI_init:
			if(chance){
				if(B_state == B_up || B_state == B_di_le_up || B_state == B_di_ri_up){

					if(B_holder[0]>AI_corner_left){
						AI_state = AI_left;
					}
					else if(B_holder[0]<AI_corner_right){
						AI_state = AI_right;
					}
	
				}
				else{
					AI_state = AI_init;
				}
			}
			break;
		case AI_left:
			AI_state = AI_init;
			break;
		case AI_right:
			AI_state = AI_init;
			break;
		
	}
	switch(AI_state){
		case AI_init:
			break;
		case AI_left:
			if(AI_corner_right == 0x20){
				AI_corner_left = 0x80;
				AI_center = 0x40;
				AI_corner_right = 0x20;
			}
			else{
				AI_corner_left = AI_corner_left<<1;
				AI_center = AI_center <<1;
				AI_corner_right = AI_corner_right<<1;
			}
			break;
		case AI_right:
			if(AI_corner_right == 0x01){
				AI_corner_right = 0x01;
				AI_center = 0x02;
				AI_corner_left  = 0x04;
			}
			else{
				AI_corner_right = AI_corner_right>>1;
				AI_center = AI_center >>1;
				AI_corner_left = AI_corner_left>>1;
			}
			break;
	}

	AI_holder[0] = AI_corner_left|AI_center|AI_corner_right;
}
//unsigned char B_holder[2] = {0x08,0x08};
//enum B_states {B_init,B_up,B_down,B_di_ri_up,B_di_ri_down,B_di_le_up,B_di_le_down,B_miss1,B_miss2} B_state;
unsigned short speed = 400;
unsigned char combineB= 0x00;
unsigned char score1 = 0x00;
unsigned char score2 = 0x00;
void Ball(){
	switch(B_state){
		case B_start:
			B_state = B_init;
			break;
		case B_init:
			B_state = B_down;
			break;
		case B_down:
			if(B_holder[1]==2 && B_holder[0]==P1_center){
				B_state = B_up;
				speed = 500;
			}
			else if(B_holder[1]==2 && B_holder[0] == P1_corner_right){
				B_state = B_di_ri_up;
				speed = 300;
			}
			else if(B_holder[1]==2 && B_holder[0] == P1_corner_left){
				B_state = B_di_le_up;
				speed = 300;
			}
			else if(B_holder[1]==1){
				B_state = B_miss1;
			}
			break;
		case B_di_le_down:
			if(B_holder[1]==2 &&  B_holder[0] == P1_center){
				B_state = B_up;
				speed = 500;
			}
			else if(B_holder[1]==2 && B_holder[0]<<1==256 ){
				B_state = B_di_ri_up;
				speed = 300;
			}			
			else if(B_holder[1]==2 && B_holder[0] == P1_corner_left){
				B_state = B_di_le_up;
				speed = 300;
			}
			else if(B_holder[1]==2 && B_holder[0] == P1_corner_right){
				B_state = B_di_le_up;
				speed = 300;
			}

			else if(B_holder[1]>1 && B_holder[1]<16 && B_holder[0]<<1==256){
				B_state = B_di_ri_down;
				speed = 300;
			}
			else if(B_holder[1]==1){
				B_state = B_miss1;
			}
			break;
		case B_di_ri_down:
			if(B_holder[1]==2 && B_holder[0] == P1_center){
				B_state = B_up; 
				speed = 500;
			}
			else if(B_holder[1]==2 && B_holder[0]>>1==0){
				B_state = B_di_le_up;
				speed = 300;
			}
			else if(B_holder[1] ==2 && B_holder[0] == P1_corner_left){
				B_state = B_di_ri_up;
				speed = 300;
			}
			else if(B_holder[1]== 2 && B_holder[0] == P1_corner_right){
				B_state = B_di_ri_up;
				speed = 300;
			}
			else if(B_holder[1]>1 && B_holder[1]<16 && B_holder[0]>>1==0){
				B_state = B_di_le_down;
				speed = 300;
			}
			else if(B_holder[1]==1){
				B_state = B_miss1;
			}
			break;
		case B_up:
			if(B_holder[1]==8 && B_holder[0] == AI_center){
				B_state = B_down;
				speed = 500;
			}
			else if(B_holder[1]==8 && B_holder[0]==AI_corner_left){
				B_state = B_di_le_down;
				speed = 300;
			}
			else if(B_holder[1]==8 && B_holder[0] == AI_corner_right){
				B_state = B_di_ri_down;
				speed = 300;
			}
			else if(B_holder[1]==16){
				B_state = B_miss2;
			}
			break;
		case B_di_ri_up:
			if(B_holder[1]==8 && B_holder[0] == AI_center){
				B_state = B_down; 
				speed = 500;
			}
			else if(B_holder[1]==8 && B_holder[0]>>1==0){
				B_state = B_di_le_down;
				speed = 300;
			}
			else if(B_holder[1] == 8 && B_holder[0] == AI_corner_left){
				B_state = B_di_ri_down;
				speed = 300;
			}
			else if(B_holder[1]== 8 && B_holder[0] == AI_corner_right){
				B_state = B_di_ri_down;
				speed = 300;
			}
			else if(B_holder[1]>1 && B_holder[1]<16 && B_holder[0]>>1==0){
				B_state = B_di_le_up;
				speed = 300;
			}
			else if(B_holder[1]==16){
				B_state = B_miss2;
			}
			break;
		case B_di_le_up:
			if(B_holder[1]==8 && B_holder[0] == AI_center){
				B_state = B_down;
				speed = 500;
			}
			else if(B_holder[1]==8 && B_holder[0]<<1==256 ){
				B_state = B_di_ri_down;
				speed = 300;
			}
			else if(B_holder[1] == 8 && B_holder[0] == AI_corner_left){
				B_state = B_di_le_down;
				speed = 300;
			}
			else if(B_holder[1]== 8 && B_holder[0] == AI_corner_right){
				B_state = B_di_le_down;
				speed = 300;
			}
			else if(B_holder[1]>1 && B_holder[1]<16 && B_holder[0]<<1==256){
				B_state = B_di_ri_up;
				speed = 300;
			}
			else if(B_holder[1]==16){
				B_state = B_miss2;
			}
			break;
		case B_miss1:
			B_state = B_miss1;
			break;
		case B_miss2:
			B_state = B_miss2;
			break;
	}
	switch(B_state){
		
		case B_init:
			break;
		case B_down:
			B_holder[1] = B_holder[1]>>1;
			break;
		case B_di_ri_down:
			B_holder[1] = B_holder[1]>>1;
			B_holder[0] = B_holder[0]>>1;
			break;
		case B_di_le_down:
			B_holder[1] = B_holder[1]>>1;
			B_holder[0] = B_holder[0]<<1;
			break;
		case B_up:
			B_holder[1] = B_holder[1]<<1;
			break;
		case B_di_le_up:
			B_holder[1] = B_holder[1]<<1;
			B_holder[0] = B_holder[0]<<1;
		        break;
		case B_di_ri_up:
			B_holder[1] = B_holder[1]<<1;
		 	B_holder[0] = B_holder[0]>>1;
			break;
		case B_miss1:
			score1 = 0x01;
			B_holder[1] = 0x00;
			B_holder[0] = 0x00;
			break;
		case B_miss2:
			score2 = 0x02;
			B_holder[1] = 0x00;
			B_holder[0] = 0x00;
			break;	
	}

}
unsigned short timer1 = 0;
unsigned short timer2 = 0;
unsigned char Combine_holder[2];
void Combine(){
	unsigned char tmpA = ~PINA & 0x02;
	transmit_data(B_holder[0],~B_holder[1]);
	transmit_data(0x00,~0x00);
	transmit_data(AI_holder[0],~AI_holder[1]);
	transmit_data(0x00,~0x00);
	transmit_data(P1_holder[0],~P1_holder[1]);
	transmit_data(0x00,~0x00);
	if(tmpA == 0x02){
		B_state = B_start;
		B_holder[0] = 0x08;
		B_holder[1] = 0x04;
		P1_corner_left = 0x10;
		P1_center = 0x08;
		P1_corner_right = 0x04;
		AI_corner_left = 0x10;
		AI_center = 0x08;
		AI_corner_right = 0x04;
	}
}




int main(void) {
//	unsigned short timer1 = 0;
//	unsigned short timer2 = 0;
    /* Insert DDR and PORT initializations */

	int timer3 = 0;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(1);
	TimerOn();
	unsigned char multi = 0x00;
	unsigned char multi_button;
//	A2D_init();
    /* Insert your solution below */
	Set_A2D_Pin(0x00);
	int t;
	srand((unsigned) time(&t));
	int chance;
    while (1) {
	    A2D_init();
	    chance = rand()%10;
	    multi_button = ~PINA & 0x10;
	    if(multi_button){
		multi=~multi;

	    }

	    
	    
	    if(timer1>=speed){
		Ball();
		timer1=0;
	    }
	    if(timer2>=100){
		
	    	P1_Toggle();
		if(multi == 0x00){
			AI_Toggle(chance);
			combineB = 0x00;
		}
		else{
			P2_Toggle();
			combineB = 0x04;
		}
		
		timer2 = 0;
	    }
	    timer1++;
	    timer2++;
	    Combine();
	    PORTB = combineB | score1|score2;
	    while(!TimerFlag){}
	    TimerFlag=0;

    }
    return 1;
}
