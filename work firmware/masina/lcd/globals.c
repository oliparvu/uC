#include "globals.h"



void delay_us(unsigned long delay)
{
	while(delay--) asm volatile("nop");
}


