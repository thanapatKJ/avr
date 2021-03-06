/*
 *      timer0ctc.h
 *      
 *      Copyright 2009 Nathaniel Robert Lewis <linux.robotdude@gmail.com>
 *      
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 * 
 *      This header controls timer0 for ctc
 * 
 */

#ifndef _TIMER_0_CTC_H
#define _TIMER_0_CTC_H 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//timer 0 prescalers
#define T0_PRESCALER_1 (1 << CS00)                        //fosc / 1
#define T0_PRESCALER_8 (1 << CS01)                        //fosc / 8
#define T0_PRESCALER_64 (1 << CS01) | (1 << CS00)         //fosc / 64
#define T0_PRESCALER_256 (1 << CS02)                      //fosc / 256
#define T0_PRESCALER_1024 (1 << CS02) | (1 << CS00)       //fosc / 1024
#define T0_PRESCALER_EXTERNAL_RISING (1 << CS02) | (1 << CS01) //external clock, clock on rising edge of t0 pin
#define T0_PRESCALER_EXTERNAL_FALLING (1 << CS02) | (1 << CS01) | (1 << CS00) //external clock, clock on falling edge of t0 pin

//pin functions for ctc on timer 0
#define PIN_DISCONNECT 0                                  //disconnect pin from timer
#define PIN_TOGGLE 1                                      //toggle pin on compare match
#define PIN_CLEAR 2                                       //clear (set to 0) pin on compare match 
#define PIN_SET 3                                         //set (set to 1) pin on compare match

void init_timer0_ctc ( uint8_t prescaler, uint8_t pinfunca, uint8_t pinfuncb ) {
	if(pinfunca > 0) {DDRB |= (1 << PORTB3);} //id ocr0a is used, set pin as ouput
	if(pinfuncb > 0) {DDRB |= (1 << PORTB4);} //if ocr0b is used, set pin as output
	if(prescaler > 5) {DDRB &= ~(1 << PORTB0);} //if an external clock source is used, set T0 pin as input
	OCR0A = 0;                        //clear the ocr0a and ocr0b registers
	OCR0B = 0;
	TCCR0A = ((pinfunca << 6) & 0xC0) | ((pinfuncb << 4) & 0x30) | (1 << WGM01);//set the mode of the timer and the pin modes
	TCCR0B = prescaler & 0x07;        //set the prescaler and mask bits that aren't the prescaler
}

void set_ocr0a ( uint8_t v ) {
	OCR0A = v;                       //set the output compare register 0a
}

void set_ocr0b ( uint8_t v ) {
	OCR0B = v;                       //set the output compare register 0b
}

void enable_interrupt_t0a ( void ) {
	TIMSK0 = (1 << OCIE0A);            //set the timer output compare interrupt enable 0a bit
	sei();                            //set the global interrupt flag
}

void enable_interrupt_t0b ( void ) {
	TIMSK0 = (1 << OCIE0B);            //set the timer output compare interrupt enable 0b bit
	sei();                            //set the global interrupt flag
}

void disable_interrupt_t0a ( void ) {
	TIMSK0 &= ~(1 << OCIE0A);           //clear the timer output compare interrupt enable 0a bit
}

void disable_interrupt_t0b ( void ) {
	TIMSK0 &= ~(1 << OCIE0B);           //clear the timer output compare interrupt enable 0b bit
}

#endif
