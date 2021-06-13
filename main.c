#include "Initializations.h"
#include "tm4c123gh6pm.h"
#include "Lcd.h"

int main(void)
{
  init();
	lcd_init();
	unsigned short int buff[10];
	float total=0;
	float lat_old=0; 
	float lon_old=0;
	float array[2];
	
	command(lcd_Clear);
			
	while (1)
	{
			Parse(array);
	  	total+= distance_total(lat_old, lon_old , array[0], array[1]);
			lat_old=array[0];
			lon_old=array[1];
			position(1,1); // go to line 1
			data_string("Distance =");
			position(1,2); // go to line 2
			sprintf(buff,"%d",(unsigned short int)total); // returns the ffloat number into string to display on LCD
		  data_string(buff);
			check_100(total);
			delay(1000);
	}
		
}
