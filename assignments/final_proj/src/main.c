#import "Adafruit_C_ADS1X15.h"

#define ANALOG_IN 

void* ads;

void init(){
	initModule_default(1, ads);
	begin(ads);
}

int main(){
	//Analog channel 0
	int16_t adc0;
	adc0 = readADC_SingleEnded(ads->m_wiringPiAddress, 0);

	printf("Analog 0 input value: %s\n", adc0);
}
