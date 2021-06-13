#include "Initializations.h"
#include "tm4c123gh6pm.h"
#include "Lcd.h"

void lcd_init (void)                     
{
	command(lcd_FunctionSet8bit);
	delay(1);

	command(lcd_EntryMode); // from left to right
	delay(1);
	
	command(lcd_DisplayOn); // turn on LCD
	delay(1);
	
	command(lcd_Clear); // clear LCD
	delay(2);
	
	command(lcd_Home); // put cursor on (0,0)
	delay(2);
	
	return;
}

void command(unsigned char cmd)            
{
	GPIO_PORTB_DATA_R=cmd;
	GPIO_PORTE_DATA_R=(0<<1)|(0<<2)|(1<<3);
	delay(1);
	GPIO_PORTE_DATA_R=(0<<1)|(0<<2)|(0<<3);
	delay(50);
	return;
}

void data(unsigned char data)
{
	GPIO_PORTB_DATA_R=data;
	GPIO_PORTE_DATA_R=(1<<1)|(0<<2)|(1<<3);
	delay(1);
	GPIO_PORTE_DATA_R=(1<<1)|(0<<2)|(0<<3);
	delay(50);
	return;
}

void data_string(unsigned char *str)         
{
	int		i=0;
	while(str[i]!='\0')
	{
		data(str[i]);
		i++;
	}
	delay(100);
	return;
}

void position(unsigned char x,unsigned char y) // control cursor position            
{
	unsigned char firstcharadr[]={0x80,0xC0};
	command(firstcharadr[y-1] + x - 1);
	delay(50);
}
