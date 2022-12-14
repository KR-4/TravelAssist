#include "bmp180.h"
#include "i2c.h"
#include "delay.h"
#include "math.h"
#define BMP_180_add 		0x77

#define EEPROM_St_add  	0xAA

uint8_t eeprom_data[22];

short AC1,AC2,AC3,B1,B2,MB,MC,MD;
unsigned short AC4,AC5,AC6;

long UT,UP;
uint8_t data_tem[2],data_pres[3];


long X1 = 0;
long X2 = 0;
long X3 = 0;
long B3 = 0;
long B5 = 0;
unsigned long B4 = 0;
long B6 = 0;
unsigned long B7 = 0;
long Press = 0;

typedef struct
	{
	float temperature;
	float pressure;
	}BMP180Sensor_t;
	BMP180Sensor_t BMP180;
void BMP180_init()
		{
			SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2)); //enable FPU

			i2c_init();
			i2c_ReadMulti(BMP_180_add,EEPROM_St_add,22,(char *)eeprom_data);
			AC1=(eeprom_data[0]<<8)+eeprom_data[1];
			AC2=(eeprom_data[2]<<8)+eeprom_data[3];
			AC3=(eeprom_data[4]<<8)+eeprom_data[5];
			AC4=(eeprom_data[6]<<8)+eeprom_data[7];
			AC5=(eeprom_data[8]<<8)+eeprom_data[9];
			AC6=(eeprom_data[10]<<8)+eeprom_data[11];
			B1=(eeprom_data[12]<<8)+eeprom_data[13];
			B2=(eeprom_data[14]<<8)+eeprom_data[15];
			MB=(eeprom_data[16]<<8)+eeprom_data[17];
			MC=(eeprom_data[18]<<8)+eeprom_data[19];
			MD=(eeprom_data[20]<<8)+eeprom_data[21];
		}

void update_BMP180(OSS OSS)
		{

		//update temperature

			i2c_writeByte(BMP_180_add, 0xF4, 0x2E);
			delayuS(4500);
			i2c_ReadMulti(BMP_180_add, 0xF6, 2, (char *)data_tem);
			UT=(data_tem[0]<<8)+data_tem[1];
			//update pressure
			uint8_t oss;
			switch (OSS)
					{
					case OSS0: oss=0; break;
					case OSS1: oss=1; break;
					case OSS2: oss=2; break;
					case OSS3: oss=3; break;
					}

			i2c_writeByte(BMP_180_add, 0xF4, 0x34+(oss<<6));

			switch (OSS)
				{
			case OSS0: delay(5);	oss=0; break;
			case OSS1: delay(8);	oss=1; break;
			case OSS2: delay(14);	oss=2; break;
			case OSS3: delay(26);	oss=3; break;
				}

			i2c_ReadMulti(BMP_180_add, 0xF6, 3, (char *)data_pres);
			UP=((data_pres[0]<<16)+(data_pres[1]<<8)+(data_pres[2]))>>(8-oss);

			//calculate temperature
			long x1=((UT-AC6) * (AC5/(pow(2,15))));
			long x2= ((MC)*(pow(2,11)))/(x1+MD);
			long B5= (x1+x2);
			BMP180.temperature= ((B5+8)/(pow(2,4)))/10;


			//calculate pressure
				B6 = B5-4000;
				X1 = (B2 * (B6*B6/(pow(2,12))))/(pow(2,11));
				X2 = AC2*B6/(pow(2,11));
				X3 = X1+X2;
				B3 = (((AC1*4+X3)<<oss)+2)/4;
				X1 = AC3*B6/pow(2,13);
				X2 = (B1 * (B6*B6/(pow(2,12))))/(pow(2,16));
				X3 = ((X1+X2)+2)/pow(2,2);
				B4 = AC4*((unsigned long)(X3+32768))/(pow(2,15));
				B7 = ((unsigned long)UP-B3)*(50000>>oss);
				if (B7<0x80000000) {Press = (B7*2)/B4;}
				else {Press = (B7/B4)*2;}
				X1 = (Press/(pow(2,8)))*(Press/(pow(2,8)));
				X1 = (X1*3038)/(pow(2,16));
				X2 = (-7357*Press)/(pow(2,16));
				Press = Press + (X1+X2+3791)/(pow(2,4));
			BMP180.pressure = Press;

		}

float get_temperature(void)
{
	return BMP180.temperature;
}
float get_pressure(void)
{
	return BMP180.pressure;
}
