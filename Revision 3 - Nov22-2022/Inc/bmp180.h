#ifndef BMP180_H_
#define BMP180_H_
#include "stdio.h"


typedef enum
	{
	OSS0=0,
	OSS1=1,
	OSS2=2,
	OSS3=3
	}OSS;


void BMP180_init();

void update_BMP180(OSS OSS);
float get_temperature(void);
float get_pressure(void);

#endif /* BMP180_H_ */


