/*
	Author: Nathaniel Shantz
	Description: Read the readme packaged with this
*/
#include <string.h>
#include <stdio.h>
#include "wrapper.h"

//used to hard-reset the sensor
#define RESET 7
//communication channels for I2C
#define SDA 8
#define SCL 9

//Running I2C with reset enabled
void* sensor;

int initialize(){
	printf("%s\n","Initializing CAP1188 Touch Sensor");
	wiringPiSetup();
	sensor = call_CAP1188_I2C(RESET);
	//hardware address is 0x28 based on specs
	if(!call_CAP1188_begin(sensor, 0x28)){
		printf("%s\n","Error: Device not found");
		return 0;
	}
	else{
		printf("%s\n","Device found");
		return 1;
	}
}

int main(){
	//if initialize fails, 
	if(!initialize){
		printf("%s\n","Error: can't run program");
		return 0;
	}
	
	uint8_t touched;
	while(1){
		touched = call_CAP1188_touched(sensor);
		
		if(touched != 0){
			uint8_t i = 0;
			for(i < 8; i++;){
				if(touched & (1 << i)){
					printf("C%d\n",i+1);
				}
			}
		}
	}
}
