#include <wiringPi.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

//my motor wires reflect these colors
#define BLUE_WIRE 4 //pin 23
#define YLLW_WIRE 5 //pin 24
//button pin location
#define BUTTON 7	//pin 4

//tracks direction of motor turning
int direction;
//tracks whether there has been an interupt
int flag;
//previous interrupt time for ISR
unsigned long prev_interrupt_time = 0;

//interupt handler changes direction of motor
void ISR(void)
{
	unsigned long interrupt_time = millis();
	if(interrupt_time - prev_interrupt_time > 300)
	{
		if (direction == 1)
			direction = 0;
		else if(direction == 0)
			direction = 1;
		
		flag = 1;
		printf("Flag set\n");
	}
	prev_interrupt_time = interrupt_time;
}
//handler for keyboard interrupt
void ctrlC(int tmp)
{
	digitalWrite(BLUE_WIRE, LOW);
	digitalWrite(YLLW_WIRE, LOW);
	exit(0);
}

//main function
int main(void)
{
/********* Initialization ********/
	printf("Initializing\n");
	wiringPiSetup();
	signal(SIGINT, ctrlC);
	direction = 0;
	flag = 0;

/********* Configuration ********/
	printf("Configuring\n");
	pinMode(BLUE_WIRE, OUTPUT);
	pinMode(YLLW_WIRE, OUTPUT);
	//starts the motor spinning initially
	digitalWrite(BLUE_WIRE, HIGH);
	digitalWrite(YLLW_WIRE, LOW);
	//establish interrupt service routine with button
	if(wiringPiISR(BUTTON, INT_EDGE_FALLING, &ISR) < 0)
	{
		printf("Unable to set Interrupt\n");
		return -1;
	}

/********* Implementation *******/
	printf("Running\n");
	for(;;)
	{
		if(flag == 1)
		{
			if(direction == 1)
			{
				digitalWrite(BLUE_WIRE, LOW); delay(200);
				digitalWrite(YLLW_WIRE, HIGH); delay(200);
			}
			else if(direction == 0)
			{
				digitalWrite(YLLW_WIRE, LOW); delay(200);
				digitalWrite(BLUE_WIRE, HIGH); delay(200);
			}
			flag = 0;
			printf("Flag Reset\n");
		}
	}
	return 1;
}
