#include <16F877.h>
#fuses HS,NOWDT,NOPROTECT,BROWNOUT,PUT,NOLVP 
#use delay(clock=4000000)
#include "lcd_x1.c"

void main ()
{

lcd_init();  
lcd_putc('\f');
lcd_putc("HaHaHa"); 
while(1);
}