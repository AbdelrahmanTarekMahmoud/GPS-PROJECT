#ifndef LCD_H_
#define LCD_H_

#define lcd_Clear            0x01
#define lcd_Home             0x02
#define lcd_EntryMode        0x06
#define lcd_DisplayOff       0x08
#define lcd_DisplayOn        0x0F
#define lcd_FunctionReset    0x30
#define lcd_FunctionSet8bit  0x38
#define lcd_SetCursor        0x80
#define first_line1          0x80
#define first_line2          0xC0

void command(unsigned char cmd); 

void lcd_init(void);   
       
void data(unsigned char data);

void data_string(); 

void position(unsigned char x,unsigned char y);

#endif