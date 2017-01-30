#include <wiringPi.h>
#include <stdio.h>

//maximum number of lights available
#define LIGHT_NUM 8
//LED Light pin numbers
#define GPIO1 0
#define GPIO2 2
#define GPIO3 3
#define GPIO4 12
#define GPIO5 6
#define GPIO6 5
#define GPIO7 4
#define GPIO8 1
//button pin location
#define BUT1 7

//value to handle direction of lights
int direction;
unsigned long prev_interrupt_time = 0;
//interupt handler changes direction of lights
void ISR(void)
{
	//code to add delay between interupts came from:
	//http://raspberrypi.stackexchange.com/questions/8544/gpio-interrupt-debounce
	unsigned long interrupt_time = millis();
	if(interrupt_time - prev_interrupt_time > 300)
	{
		if(direction == 1)
			direction = 0;
		else if(direction == 0)
			direction = 1;
		else
			direction = 0;
	}
	prev_interrupt_time = interrupt_time;
}


//function to toggle light on and off
int toggle(int status, int location);

int main(void)
{
	printf("Initializing\n");
	wiringPiSetup();
	//Layout of array is Pin number, status value (0 == off, 1 == on)
	int array[] = { GPIO1, 0, GPIO2, 0, GPIO3, 0,
					GPIO4, 0, GPIO5, 0, GPIO6, 0, 
					GPIO7, 0, GPIO8, 0
					};
	//initializing all lights to off				
	int i = 0;
	printf("Configuring Pins\n");
	while(i < LIGHT_NUM * 2)
	{
		pinMode(array[i], OUTPUT);
		digitalWrite(array[i], LOW); delay(100);
		i = i + 2;
	}
	//initialize interupt routine
	if( wiringPiISR(BUT1, INT_EDGE_FALLING, &ISR) < 0)
	{
		printf("Unable to set Interupt");
		return 1;
	}
	//counter variable for array location
	int j = -2;
	printf("Toggling Pins\n");
	//0 = increase counter variable, 1 = decrease counter variable
	direction = 0;
	
	//toggle first pin before starting loop (allows toggle function to work
	//properly
	//digitalWrite(GPIO1, HIGH); delay(500);
	for(;;)
	{
		
		//skipping by 2 to access the next pin and not the status pin
		if(direction == 0)
			j = j+2;
		else if (direction == 1)
			j = j-2;
			
		//toggle status value for LED
		array[j+1] = toggle(array[j+1], j);
		//if 1, turn LED high
		if(array[j+1] == 1)
		{
			digitalWrite(array[j], HIGH); delay(500);
		}
		//if 0, turn LED low
		else
		{
			digitalWrite(array[j], LOW); delay(500);
		}

		//since there are two values per pin, the max number must be multiplied by 2
		//if number is twice the max, then the loop starts back at 0
		if(j == LIGHT_NUM * 2)
			j = -2;
		//if number is below 0, then the loop starts back at the tail of the loop
		else if(j < 0)
			j = (LIGHT_NUM * 2);
	}

	return 1;
}
//insert the status value to read in
//this section inverts status value
int toggle(int status, int location)
{
	//this prevents lights from being off/on opposite of the chain.
	int prev_led;
	int next_led;
	//TODO: this doesn't check for the edge lights
	if(direction == 0)
	{
		//if at the end, next led is starting at opposite side
		if(location == LIGHT_NUM *2)
		{
			next_led = 1;
			prev_led = status-2;
		}
		//if at the beginning of the loop, prev is at the end of the line
		else if(location == 0)
		{
			next_led = status+2;
			prev_led = (LIGHT_NUM*2)-1;
		}
		//this is the standard in the middle of cycle
		else
		{
			next_led = status+2;
			prev_led = status-2;
		}

	}
	else if(direction == 1)
	{
		//if at end of loop, start at opposing side
		if(location == 0)
		{
			next_led = (LIGHT_NUM*2)-1;
			prev_led = status+2;
		}
		else if(location == LIGHT_NUM *2)
		{
			prev_led = 1;
			next_led = status-2;
		}
		else
		{
			prev_led = status+2;
			next_led = status-2;
		}
	}

	/*if(prev_led == 0 && next_led == 0)
		status = 1;
	else if(prev_led == 1 && next_led == 1)
		status = 0;
	else */if(/*status == 1*/prev_led == 0)
		status = 0;
	else if (/*status == 0*/prev_led == 1)
		status = 1;
	else
		printf("Error toggling light\n");

	return status;
}
