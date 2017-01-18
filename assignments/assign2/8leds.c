#include <wiringPi.h>
#include <stdio.h>

//maximum number of lights available
#define LIGHT_NUM 8

#define GPIO1 0
#define GPIO2 2
#define GPIO3 3
#define GPIO4 12
#define GPIO5 6
#define GPIO6 5
#define GPIO7 4
#define GPIO8 1

#define BUT1 10

int toggle(int status);

int main(void)
{
	printf("Initializing\n");
	wiringPiSetup();
	//Layout of array is Pin number, status value (0 == off, 1 == on)
	int array[] = { GPIO1, 0, GPIO2, 0, GPIO3, 0,
					GPIO4, 0, GPIO5, 0, GPIO6, 0, 
					GPIO7, 0, GPIO8, 0
					};
	int i = 0;
	printf("Configuring Pins\n");
	while(i < LIGHT_NUM * 2)
	{
		pinMode(array[i], OUTPUT);
		digitalWrite(array[i], LOW); delay(100);
		i = i + 2;
	}
	
	//counter variable for array location
	int j = 0;
	printf("Toggling Pins\n");
	for(;;)
	{
		array[j+1] = toggle(array[j+1]);
		if(array[j+1] == 1)
		{
			digitalWrite(array[j], HIGH); delay(500);
		}
		else
		{
			digitalWrite(array[j], LOW); delay(500);
		}

		j = j + 2;
		if(j == LIGHT_NUM * 2)
			j = 0;
		else if(j == -1)
			j = (LIGHT_NUM * 2) - 1;
	}

	return 1;
}

int toggle(int status)
{
	if(status == 1)
		status = 0;
	else
		status = 1;

	return status;
}
