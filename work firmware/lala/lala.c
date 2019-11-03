#define F_CPU 1000000               //FCPU=1MHz

#include <avr/io.h>
#include <util/delay.h>

#define dataport PORTC
#define commport PORTD
#define rs PD2
#define wr PD1
#define en PD0

unsigned int LCD_init();
unsigned int LCD_SendData();
unsigned int wrcomm();
unsigned int wrdata();
unsigned int read_adc();



int main(void)
{
	DDRA = 0x00;							 //PortA in
    DDRC = 0xFF;                             //PortC out
    DDRD = 0x07;                             //PortD 0, 1, 2 pin out
	LCD_init(); 							 //Initializare LCD
while(1)
{_delay_ms(100);
    LCD_SendData();                      //Afisare
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
    dataport = 0x80;                   //Fara shipt
    wrcomm();                              //Scrie comanda
return 1;
}

unsigned int LCD_SendData (void)
{unsigned char l1[]="     ";
 unsigned int val,i,j;
val=read_adc();

val=val*(503/1024);
i=4;
while(val)
	{
	l1[i]=val%10;
	val=val/10;
	i--;
	}


if("0"==l1[


dataport= 0x00;					   //Cursor la linia 1 coloana 1
wrcomm();

for (i=0;i<sizeof l1-1;i++)
{
dataport = l1[i];
wrdata();
}
   

//dataport = 0xC0;                //Cursoar la linia 2 coloana 1
//wrcomm();



 return 1;}

unsigned int wrcomm(void)
{
    commport &= ~(1 << rs);                            //Setting RS = 0, selecting command register
    commport &= ~(1 << wr);                           //Setting RW = 0
    commport |= (1 << en);                                //EN = 1
    commport &= ~(1 << en);                           //EN = 0, thus giving high to low pulse on Enable pin
    _delay_ms(2);                                            //10ms delay
    return 1;
}

unsigned int wrdata(void)
{
    commport |= (1 << rs);                            //Setting RS = 1, selecting data register
    commport &= ~(1 << wr);                      //Setting RW = 0
    commport |= (1 << en);                           //EN = 1
    commport &= ~(1 << en);                      //EN = 0, thus giving high to low pulse on Enable pin
    _delay_ms(2) ;                                      //10ms delay
    return 1;
}  

unsigned int read_adc()
{
  ADMUX=( (1<< MUX0) | (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) ); //Canal 1, Referinta AVCC, Aliniere la dreapta
  ADCSRA=( (1<<ADEN) | (1<<ADSC) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS2) );//Incepe conversia, Seteaza bitii de prescalare
  while(!(ADCSRA & 0x10)); //Asteapta sa termine conversia
  ADCSRA|=0x10;
  return ADCW;
}
  
