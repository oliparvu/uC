#define F_CPU 1000000               //FCPU=1MHz

#include <avr/io.h>
#include <util/delay.h>
#include "lcd_lib.h"

#define adc1 0x41					//Canal 1, Referinta AVCC, Aliniere la dreapta

unsigned int process();
unsigned int mean();
unsigned int read_adc(unsigned int channel);
unsigned char v1,v2,v3,val;
unsigned char l1[]="U=  . V";

int main(void)
{DDRA = 0x00;
LCDinit();
LCDclr();
LCDhome();

while(1)
{process();
LCDstring(l1,7);
LCDhome();}
return 1;
}



unsigned int read_adc(unsigned int channel)
{	
  ADMUX=channel;
  ADCSRA=( (1<<ADEN) | (1<<ADSC) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS2) );//Incepe conversia, Seteaza bitii de prescalare
  while(!(ADCSRA & 0x10)); //Asteapta sa termine conversia
  ADCSRA|=0x10;
  return ADCW;
}


unsigned int mean()					//Citeste 3 valori ale tensiunii la intervale de 100ms si face media
{
 v1=read_adc(adc1);
 _delay_ms(300);
 v2=read_adc(adc1);
 _delay_ms(300);
 v3=read_adc(adc1);
 val=(v1+v2+v3)/3;
val*=503/1024;						//Vin(mV)=(ADCW*Vref[V])/10.24
return val;
}

unsigned int process()
{

 unsigned int u;
 u=(mean(adc1)*3)/10;

 if(((u/100)%1000)==0x0001) 		//Mascarea primului 0
  	l1[2]=(u/100)%1000+48;
 else
 	l1[2]=0x20;
 if((((u/10)%100)>=0x000A)&&(((u/10)%100)<0x000F))
 {
	l1[3]=(u/10)%100+38;
	l1[5]=(u)%10+47;
 }
 	else
		if (((u/10)%100)==0x000F)
		{ 	l1[3]=(u/10)%100+38;
			l1[5]=(u)%10+48;
		}
			else
			{	l1[3]=(u/10)%100+48;
				l1[5]=(u)%10+48;
			}
return 1;
}
