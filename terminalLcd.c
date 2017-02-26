#define F_CPU 16000000
#define UBRR 103 // 9600
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "lcd.h"
#define LINE_W 16
void USART_Setup(){
	/* set baud rate */
	int ubrr = UBRR;
	UBRR0L = (unsigned char) ubrr;
	UBRR0H = 0x0F & (ubrr >> 8);
	/* enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) |	(1 << TXEN0);
	/* set frame, 8 bit data, 1 bit stop, no parity */
	UCSR0C = 0x00;
	UCSR0C |= 0x06;
	
}

	
void USART_Transmit(unsigned char data){
	/* wait the last transferred data */
	
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;	
}

void USART_Transmit_Block(char * s, int n){
	int i = 0;
	for(i = 0; i < n; i++){
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = s[i];
	}
}
unsigned char USART_Receive(void){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

int read_block(char * buffer, int MAX){
	int i;
	for(i = 0; i < MAX; i++){
		buffer[i] = USART_Receive();
		if(buffer[i] == '\n'){
			return i+1;
		}
	}
	return MAX;
}
int read_block2(char * buffer, int MAX){
	int i = 0;
	while(i < MAX){
		while(!(UCSR0A & (1<<RXC0)));
		buffer[i] = UDR0;
		if(buffer[i] == '\n'){
			return i;
		}
		i++;
	}
	return MAX;
}

int main(){
	DDRB =0x20 ;// 0b0010 0000 from 0 to 7
	PORTB = 0x00;
	setupLCD();
	PORTB = 0x20;
	USART_Setup();
	char buffer[128];
	int num;
	while(1){

		num = read_block2(buffer, 128);
		//USART_Transmit_Block(buffer, num);
		
		
		if(num > LINE_W){
			showData(buffer, LINE_W, LINE_1);
			showData(buffer+LINE_W, num - LINE_W, LINE_2);
		}else{
			showData(buffer, num, LINE_1);
		}
	
		//PORTB = PORTB? 0x00: 0x20;
		
	}

	
	
	return 0;
}

