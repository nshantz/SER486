#include "Adafruit_C_ADS1X15.h"

#define ANALOG_IN 

struct Adafruit_ADS1115 ads;

void init(){
	printf("Ok, now I made to the beginning of the 'Init' function...\n");
	initModule_default(1, &ads);
	begin(&ads);
	printf("Woo! I initialized!\n");
}

int main(){
	printf("Just checking to see if I start...\n");
	init();
	//Analog channel 0
	int16_t adc0;
	printf("Attempting to access struct values\n");

	printf("I2C Address: %d\n", ads.m_i2cAddress);
	printf("Conversion Delay: %d\n", ads.m_conversionDelay);
	printf("Bit Shift: %d\n", ads.m_bitShift);
	printf("Wiring Pi Address: %d\n", ads.m_wiringPiAddress);

	/*while(1){
		printf("Beginning of loop\n");
		adc0 = (int16_t)readADC_SingleEnded(&ads, 0);
		printf("Returned Value to adc0\n");
		printf("Analog 0 input value: %d\n", (int)adc0);
	}*/
}
