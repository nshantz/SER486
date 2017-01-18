#include <wiringPi.h>
#include <stdio.h>

//maximum number of lights available
#define LIGHT_NUM 8

#define GPIO1 17
#define GPIO2 27
#define GPIO3 22
#define GPIO4 5
#define GPIO5 6
#define GPIO6 13
#define GPIO7 19
#define GPIO8 26

#define BUT1 18

typedef struct
{
	int idNum;
	int status;
}lights;

int toggle(lights light);

int main(void)
{
	printf("Initializing\n");
	wiringPiSetup();
	lights array[] = {
		{GPIO1, 0},
		{GPIO2, 0},
		{GPIO3, 0},
		{GPIO4, 0},
		{GPIO5, 0},
		{GPIO6, 0},
		{GPIO7, 0},
		{GPIO8, 0}
	};
	int i = 0;
	printf("Configuring Pins\n");
	while(i < LIGHT_NUM)
	{
		pinMode(array[i].idNum, OUTPUT);
		digitalWrite(array[i].idNum, LOW); delay(100);
		++i;
	}
	
	//counter variable for array location
	int j = 0;
	printf("Toggling Pins\n");
	for(;;)
	{
		if(toggle(array[j]) == 1)
		{
			digitalWrite(array[j].idNum, HIGH); delay(500);
		}
		else
		{
			digitalWrite(array[j].idNum, LOW); delay(500);
		}
		++j;
		if(j == LIGHT_NUM)
			j = 0;
		else if(j == -1)
			j = LIGHT_NUM - 1;
	}

	return 1;
}

int toggle(lights light)
{
	if(light.status == 1)
		light.status = 0;
	else
		light.status = 1;

	return light.status;
}
