#include "Adafruit_C_ADS1X15.h"

#define ANALOG_IN 

struct Adafruit_ADS1115 ads;
/* Anemometer values */
//value in mV per bit value
float voltageConversionConstant = 0.0635;

//these are tech specs given with the anemometer
float voltageMin = 0.4;
float voltageMax = 2.0;

//0 m/s
float windSpeedMin = 0;
//32 m/s
float windSpeedMax = 32;
/* Initialization function */
void init(){
	printf("Ok, now I made to the beginning of the 'Init' function...\n");
	initModule_default(1, &ads);
	setGain(&ads, GAIN_TWO);
	begin(&ads);
	printf("Woo! I initialized!\n");
}

/* Returns windspeed value after conversion */
float getSpeed(int16_t input){
	//converting input to actual voltage
	float voltage = (input * voltageConversionConstant) / 1000;
	
	printf("Voltage: %.5f\n", voltage);

	float windSpeed;
	//if voltage is below minimum threshold, then speed is 0
	if (voltage <= voltageMin){
		windSpeed = 0;
	}
	//if not, calculate windspeed based on formula from internet
	else{
		windSpeed = (voltage - voltageMin) * windSpeedMax /(voltageMax - voltageMin);
	}

	return windSpeed;
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

	while(1){
		//printf("Beginning of loop\n");
		adc0 = (int16_t)readADC_SingleEnded(&ads, 0);
		//printf("Returned Value to adc0\n");
		printf("Wind Speed in m/s: %.5f\n", getSpeed(adc0));
	}
}
