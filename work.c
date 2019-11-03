#include "hd44780.h"
#include <avr/io.h>
#include <util/delay.h>

#define temp 0x43
#define temp2 0x41
unsigned int Mean();
unsigned int read_adc();
unsigned int Temp();

char l1[16]="I   .    E   .   ", l2[16]="     XX.XXV     ";

void main (void)
{

DDRA = 0x00;
l1[6]=0xB2;							 //Caracter pt grade
	l1[15]=0xB2;
	lcd_init();
  lcd_clrscr();
  while (1)
  {Temp();
  lcd_goto(0x00);
  lcd_puts(l1);
  lcd_goto(0x40);
  lcd_puts(l2);
  }
}


unsigned int Mean(unsigned int n)		//achizitioneaza  6 esantioane la interval de 100ms, apoi face media aritmetica
{
unsigned int v1,v2,v3,v4,v5,v6,val;


v1=read_adc(n);
_delay_ms(100);
v2=read_adc(n);
_delay_ms(100);
v3=read_adc(n);
v4=read_adc(n);
_delay_ms(100);
v5=read_adc(n);
_delay_ms(100);
v6=read_adc(n);

val=(v1+v2+v3+v5+v6+v4)/6;
val=val*49;
//val=val/10;

return val;

}

unsigned int read_adc(unsigned int channel)
{	ADMUX=channel;
  ADCSRA=( (1<<ADEN) | (1<<ADSC) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS2) );//Incepe conversia, Seteaza bitii de prescalare
  while(!(ADCSRA & 0x10)); //Asteapta sa termine conversia
  ADCSRA|=0x10;
  return ADCW;
}

unsigned int Temp(void)
{
unsigned int val,val2, i=3, j=4;
unsigned char t[]="   ",u[]="      ";


val=Mean(temp);
while(val)				//separa cifrele si le salveaza in sirul t
	{
	t[i]=val%10;
	val=val/10;
	i--;
	}

l1[11]=48+t[0];			//cifra zecilor pe pozitia 10 de pe LCD
if(l1[11]==0x50)		//ascunde primul 0
{
	l1[11]=0x20;			//0x20 ascii = " "
}

l1[12]=48+t[1];			//cifra unitatilor pe pozitia 11
if(l1[12]==0x50)
{
	l1[12]=0x30;
}

l1[14]=48+t[2];			//prima zecimala pe pozitia 13
if(l1[14]==0x50)
{
	l1[14]=0x30;
}




val2=Mean(temp2);
if (val2>=27310)
{
	val2=val2-27310;
	l1[1]=0x20; 
}
else
{
	val2=27310-val2;
	l1[1]=0x2D;
}
while(val2)				//separa cifrele si le salveaza in sirul t
	{
	u[j]=val2%10;
	val2=val2/10;
	j--;
	}

l1[2]=48+u[1];			//cifra zecilor pe pozitia 10 de pe LCD
if(l1[2]==0x50)		//ascunde primul 0
{
	l1[2]=0x20;			//0x20 ascii = " "
}

l1[3]=48+u[2];			//cifra zecilor pe pozitia 10 de pe LCD
if(l1[3]==0x50)		//ascunde primul 0
{
	l1[3]=0x30;			//0x20 ascii = " "
}


l1[5]=48+u[3];			//cifra zecilor pe pozitia 10 de pe LCD
//l1[6]=48+u[4];			//cifra zecilor pe pozitia 10 de pe LCD

return 1;
}
