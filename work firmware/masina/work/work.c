#include "hd44780.h"

void main (void)
{
	lcd_init();
  lcd_clrscr();
  
  lcd_puts("BuHuHu");

}
