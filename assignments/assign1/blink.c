#include <wiringPi.h>

#define GPIO 0
int main (void)
{
	wiringPiSetup();
	pinMode(GPIO, OUTPUT);
	for (;;)
	{
		digitalWrite (GPIO, HIGH); delay (500);
		digitalWrite (GPIO, LOW); delay (500);
	}
	return 0;
}
