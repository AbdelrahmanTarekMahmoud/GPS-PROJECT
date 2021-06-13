#include "Initializations.h"
#include "tm4c123gh6pm.h"
#include <Math.h>
#define PI 3.14159265359
#include <string.h> // strtok
#include <stdlib.h> // strtod
#include "LCD.h"

void init ()
{ // PORT E lcd controls
	SYSCTL_RCGCGPIO_R |= 0x00000010;
  while ((SYSCTL_PRGPIO_R&0x10) == 0){};
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_CR_R |= 0x3E;
	GPIO_PORTE_DIR_R = 0x0E;
	GPIO_PORTE_DEN_R = 0x3E;
	GPIO_PORTE_AMSEL_R = 0x00;
	GPIO_PORTE_AFSEL_R = 0x30;
	GPIO_PORTE_PCTL_R = 0x00110000;
  GPIO_PORTE_PUR_R = 0x00;
	
	// UART5 PORT E
	SYSCTL_RCGCUART_R |= 0x0020;
	while((SYSCTL_PRUART_R & 0x0020)==0){};
	UART5_CTL_R &= ~0x0001;
	UART5_IBRD_R = 104;
	UART5_FBRD_R = 11; 
	UART5_LCRH_R = 0x0070;
	UART5_CTL_R = 0x0301;
  
	// PORT B lcd data
	SYSCTL_RCGCGPIO_R |= 0x00000002;
  while ((SYSCTL_PRGPIO_R&0x02) == 0){};
	GPIO_PORTB_LOCK_R = 0x4C4F434B;
	GPIO_PORTB_CR_R |= 0xFF;
	GPIO_PORTB_DIR_R = 0xFF;
	GPIO_PORTB_DEN_R = 0xFF;
	GPIO_PORTB_AMSEL_R = 0x00;
	GPIO_PORTB_AFSEL_R = 0x00; 
	GPIO_PORTB_PCTL_R = 0x00000000;
  GPIO_PORTB_PUR_R = 0x00;
		
	//PORT F
	SYSCTL_RCGCGPIO_R |= 0x00000020;
  while ((SYSCTL_PRGPIO_R&0x20) == 0){};
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;
	GPIO_PORTF_AMSEL_R = 0x00;
	GPIO_PORTF_PCTL_R = 0x00000000; 
	GPIO_PORTF_DIR_R = 0x0E;
  GPIO_PORTF_AFSEL_R = 0x00; 
  GPIO_PORTF_PUR_R = 0x00;
  GPIO_PORTF_DEN_R = 0x1F;
	GPIO_PORTF_DATA_R &= ~0x0E;
}


void check_100(unsigned short int d) // turns LED on if distance > 100m
{
	if(d > 100){
		GPIO_PORTF_DATA_R |= 0x0E;
	}

}

unsigned char UART5_in(void) // checks if FIFO is empty to start read from UART
{
	while((UART5_FR_R & 0x10)!=0);
	return ((unsigned char)(UART5_DR_R & 0xFF));
}

void Parse(float array[])
{
	unsigned char in; // used to read from UART data
	unsigned char flag=0;  // we use this flag to enter and exit the parsing while loop
	unsigned char* y;  // used to store the value of latitude
	unsigned char* x;  // used to store the value of londitude
	unsigned char* n;  // used to store the value of north or south
	unsigned char* e;  // used to store the value of east or west
	unsigned char* gps_fix;  // check if gps finished its fix
	float yfloat;  // used to store the value of float part of latitude
	float xfloat;  // used to store the value of float part of longitude
	unsigned char yint;  // used to store the value of intger part of latitude
	unsigned char xint;  // used to store the value of intger part of longitude
	float yreal;  // used to store the real and final value of float part of latitude
	float xreal;  // used to store the real and final value of float part of longitude
	unsigned char inc[50];  // we use this array to store the data of the gps
		while(flag==0){
			    in = UART5_in();
				if(in =='$'){
					in = UART5_in();
				if(in=='G'){
					in = UART5_in();
				if(in=='P'){
					in = UART5_in();
	 		  if(in=='G'){
					in = UART5_in();
				if(in=='G'){
					in = UART5_in();
				if(in=='A'){
					in = UART5_in();
				if(in==','){
					for(int i=0;i<50;i++){
				  	inc[i]= UART5_in(); // if gps sends "$GPGGA," we will take the data after it
					}
				}
				}
				}
	  		}
				}
				}
				}
			y = strtok(inc, ","); // strtok function is used to divide the data in the array
			y = strtok(NULL, ",");
			n = strtok(NULL, ",");
			x = strtok(NULL, ",");
			e = strtok(NULL, ",");
		  gps_fix = strtok(NULL, ",");
			if(gps_fix[0]=='1'){ 
				flag=1;
			}
		}
		yfloat= strtod(y,NULL); // dividing the gps degree latitude to convert it to decimal using equations
		xfloat= strtod(x,NULL);  // dividing the gps degree longitude to convert it to decimal using equations
		yfloat=yfloat/100;
		xfloat=xfloat/100;
		yint=(unsigned char)yfloat;
		xint=(unsigned char)xfloat;
		yfloat=yfloat-yint;
		xfloat=xfloat-xint;
		yfloat=(yfloat*100)/60;
		xfloat=(xfloat*100)/60;
		yreal=yint+yfloat; // final latitude value
		xreal=xint+xfloat; // final longitude value
		if(n[0] == 'S'){
			yreal*=-1;
		}
		if(e[0]=='W'){
			xreal*=-1;
		}
		array[0]=yreal;
		array[1]=xreal;
}


float dist_calc(float lat_old,float lon_old,float lat_new,float lon_new){
	// we will use the Haversine formula to calculate the distance
	float dy = (lat_new-lat_old)* PI/180.0;
	float dx = (lon_new-lon_old)* PI/180.0;
	

	lat_old = (lat_old)* PI/180.0;
	lat_new = (lat_new)* PI/180.0;
	

	float a = pow(sin(dy / 2), 2) + pow(sin(dx / 2), 2) * cos(lat_old) * cos(lat_new);
	float r = 6371, d = 2 * asin(sqrt(a));
	return r * d * 1000;
}

float distance_total (float lat_old, float lon_old, float lat_new, float lon_new)
{ // used to accumlate the distance to get the total
	float Total=0;
	if(lon_old!=0 && lat_old !=0){ 
		Total = dist_calc(lat_old,lon_old,lat_new,lon_new);
	}
	return Total;
}

void delay(int n)
{
	int i;
	int j;
	for(i=0 ; i<n ; i++)
	{
		for(j=0; j<3180; j++)
		{
			
		}
	}
}
