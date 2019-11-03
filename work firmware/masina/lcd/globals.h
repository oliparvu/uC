#pragma once

#include <avr/io.h> 
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

//delay function
#define F_CPU 16000000UL 		//Your clock speed in Hz (16MHz here)

#define LOOP_CYCLES 8 				//Number of cycles that the loop takes

#define us(num) (num/(LOOP_CYCLES*(1/(F_CPU/1000000.0))))

void delay_us(unsigned long delay);

#define PORT_ON( port_letter, number )			port_letter |= (1<<number)
#define PORT_OFF( port_letter, number )			port_letter &= ~(1<<number)
#define PORT_ALL_ON( port_letter, number )		port_letter |= (number)
#define PORT_ALL_OFF( port_letter, number )		port_letter &= ~(number)
#define FLIP_PORT( port_letter, number )		port_letter ^= (1<<number)
#define PORT_IS_ON( port_letter, number )		( port_letter & (1<<number) )
#define PORT_IS_OFF( port_letter, number )		!( port_letter & (1<<number) )
