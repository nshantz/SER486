/*
	Author: Nathaniel Shantz
	Description: Read the readme packaged with this
*/

#include "Adafruit_CAP1188.h"

//used to hard-reset the sensor
#define RESET 7
//communication channels for I2C
#define SDA 8
#define SCL 9

//Running I2C with reset enabled
Adafruit_CAP1188 sensor = Adafruit_CAP1188(RESET);

int initialize(){
	printf("Initializing CAP1188 Touch Sensor\n");
	wiringPiSetup();
	//hardware address is 0x28 based on specs
	if(!sensor.begin(0x28)){
		printf("Error: Device not found\n");
		return 0;
	}
	else{
		printf("Device found\n");
		return 1;
	}
}

int main(){
	if(!initialize){
		printf("Error: can't run program");
	}
	
	uint8_t touched;
	while(1){
		touched = sensor.touched();
		
		if(touched != 0){
			for(uint8_t i = 0; i < 8; i++){
				if(touched & (1 << i)){
					printf("C%d\n",i+1);
				}
			}
		}
	}
}
