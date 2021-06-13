#ifndef GPIO_INIT_H_
#define GPIO_INIT_H_

void init(void);
unsigned char UART5_in(void);
void Parse(float array[]);
void check_100 (unsigned short int d);
float distance_total (float lat_old, float lon_old, float lat_new, float lon_new);
float dist_calc(float lat_old,float lon_old,float lat_new,float lon_new);
void delay(int n);

#endif