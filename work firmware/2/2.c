#define F_CPU 1000000               //FCPU=1MHz

#include <avr/io.h>
#include <util/delay.h>

#define dataport PORTC				//Port C pt date
#define commport PORTD				//PortD pt comenzi
#define rs PD2
#define wr PD1
#define en PD0
#define adc1 0x41					//Canal 1, Referinta AVCC, Aliniere la dreapta
//#define led PD6
//#define v SREG.3
unsigned int LCD_init();
unsigned int Mean();
unsigned int process();
unsigned int LCD_SendData();
unsigned int wrcomm();
unsigned int wrdata();
unsigned int read_adc();
unsigned char l1[]="U=  . V";
unsigned int val,v1,v2,v3,channel;
//unsigned long val;

int main(void)
{
	DDRA = 0x00;							 //PortA in
    DDRC = 0xFF;                             //PortC out
    DDRD = 0xFF;                             //PortD 0, 1, 2 pin out
	LCD_init(); 							 //Initializare LCD



	while(1)
{ 
	process();
    LCD_SendData();                    //Afisare
}
return 1;
}
 


unsigned int LCD_init()
{
    dataport = 0x38;                    //LCD 2 linii 5x7 
    wrcomm();                              //Scrie comanda
    dataport = 0x0E;                    //Clear LCD
    wrcomm();                              //Scrie comanda
    dataport = 0x0C;                    //Setari cursor
    wrcomm();                              //Scrie comanda
    dataport = 0x01;                    //Cursor la linia 1, col 1
    wrcomm();                              //Scrie comanda
    dataport = 0x80;                   //Fara shift
    wrcomm();                              //Scrie comanda
return 1;
}



unsigned int Mean()					//Citeste 3 valori ale tensiunii la intervale de 300ms si face media
{
 v1=read_adc(adc1);
 _delay_ms(300);
 v2=read_adc(adc1);
 _delay_ms(300);
 v3=read_adc(adc1);
 
 val=(v1+v2+v3)/3;
 //val*=5.03/254;						//Vin(mV)=(ADCW*Vref[V])/10.24
 val*=503/254;
return val;
}


unsigned int process()
{

 unsigned int u;
 u=Mean(adc1)*3/20;
//u=Mean(adc1);
l1[2]=u+48;

 if(((u/100)%1000)==0x01) 		//Mascarea primului 0
  	l1[2]=(u/100)%1000+48;
 else
 	l1[2]=0x20;						// 0x20=" " ASCII
 if((((u/10)%100)>=0x0A)&&(((u/10)%100)<0x0F))
 {
	l1[3]=(u/10)%100+38;
	l1[5]=(u)%10+47;
 }
 	else
		if (((u/10)%100)==0x0F)
		{ 	l1[3]=(u/10)%100+38;
			l1[5]=(u)%10+48;
		}
			else
			{	l1[3]=(u/10)%100+48;
				l1[5]=(u)%10+48;
			}
			
return 1;
}


unsigned int LCD_SendData ()
{
 unsigned int i;

dataport= 0x02;					   //Cursor la linia 1 coloana 1
wrcomm();

for (i=0;i<sizeof l1-1;i++)
{
dataport = l1[i];
wrdata();
}
   
//dataport = 0xC0;                //Cursor la linia 2 coloana 1
//wrcomm();

 return 1;}

unsigned int wrcomm(void)
{
    commport &= ~(1 << rs);                            //Setting RS = 0, selecting command register
    commport &= ~(1 << wr);                           //Setting RW = 0
    commport |= (1 << en);                                //EN = 1
    commport &= ~(1 << en);                           //EN = 0, thus giving high to low pulse on Enable pin
    _delay_ms(1);                                            //1ms delay
    return 1;
}

unsigned int wrdata(void)
{
    commport |= (1 << rs);                            //Setting RS = 1, selecting data register
    commport &= ~(1 << wr);                      //Setting RW = 0
    commport |= (1 << en);                           //EN = 1
    commport &= ~(1 << en);                      //EN = 0, thus giving high to low pulse on Enable pin
    _delay_ms(1) ;                                      //1ms delay
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
  
