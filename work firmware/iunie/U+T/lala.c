#define F_CPU 1000000               //FCPU=1MHz

#include <avr/io.h>
#include <util/delay.h>

#define dataport PORTC
#define commport PORTD
#define rs PD2
#define wr PD1
#define en PD0
#define temp 0x43
#define temp2 0x41

unsigned int Temp();
unsigned int Mean();
unsigned int LCD_SendData();
unsigned int wrcomm();
unsigned int wrdata();
unsigned int read_adc();
unsigned int LCD_init(); 		//Initializare LCD

unsigned char l1[]="35   T=  .      ";		//Initializare sir pt linia 1
unsigned char l2[]="335  T=   .      ";		//Sir linia 2


int main(void)
{
	DDRA = 0x00;							 //PortA in
    DDRC = 0xFF;                             //PortC out
    DDRD = 0x07;                             //PortD 0, 1, 2 pin out
	LCD_init(); 							 //Initializare LCD
	l1[14]=0xB2;							 //Caracter pt grade

while(1)
{
		Temp();
		LCD_SendData();

}
return 1;
}

unsigned int Temp(void)
{
unsigned int i=3,j=3,val,val2;
unsigned char t[]="   ", u[]="   ";

val=10*Mean(temp);

while(val)				//separa cifrele si le salveaza in sirul t
	{
	t[i]=val%10;
	val=val/10;
	i--;
	}

l1[7]=48+t[0];			//cifra zecilor pe pozitia 10 de pe LCD
if(l1[7]==0x50)		//ascunde primul 0
{
	l1[7]=0x20;			//0x20 ascii = " "
}

l1[8]=48+t[1];			//cifra unitatilor pe pozitia 11
if(l1[8]==0x50)
{
	l1[8]=0x30;
}

l1[10]=48+t[2];			//prima zecimala pe pozitia 13
if(l1[10]==0x50)
{
	l1[10]=0x30;
}


val2=10*Mean(temp2);
if (val2>=2731)
{
	val2-=2731;
}
else
{
	val2=2731-val2;
	l1[8]=0x2D;
}

while(val2)				//separa cifrele si le salveaza in sirul t
	{
	u[j]=val2%10;
	val2=val2/10;
	j--;
	}

l2[8]=48+u[0];			//cifra zecilor pe pozitia 10 de pe LCD
if(l2[8]==0x50)		//ascunde primul 0
{
	l2[8]=0x20;			//0x20 ascii = " "
}

l2[9]=48+u[1];			//cifra unitatilor pe pozitia 11
if(l2[9]==0x50)
{
	l2[9]=0x30;
}

l2[11]=48+u[2];			//prima zecimala pe pozitia 13
if(l2[11]==0x50)
{
	l2[11]=0x30;
}

return 1;
}

unsigned int Mean(unsigned int n)		//achizitioneaza  3 esantioane la interval de 100ms, apoi face media aritmetica
{
unsigned int v1,v2,v3,val;

v1=read_adc(n);
_delay_ms(100);
v2=read_adc(n);
_delay_ms(100);
v3=read_adc(n);
val=(v1+v2+v3)/3;
val=val*49;
val=val/10;

return val;

}



unsigned int LCD_SendData (void)    //Afiseaza pe LCD sirul 
{
unsigned int i,j;
dataport= 0x01;					   //Cursor la linia 1 coloana 1
wrcomm();

for (i=0;i<16;i++)
{
dataport=l1[i];
wrdata();
} 

dataport = 0xC0;                //Cursor la linia 2 coloana 1
wrcomm();

for (j=0;j<16;j++)
{
dataport=l2[j];
wrdata();
} 

return 1;
}




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

unsigned int read_adc(unsigned int channel)
{	ADMUX=channel;
  ADCSRA=( (1<<ADEN) | (1<<ADSC) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS2) );//Incepe conversia, Seteaza bitii de prescalare
  while(!(ADCSRA & 0x10)); //Asteapta sa termine conversia
  ADCSRA|=0x10;
  return ADCW;
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
  
