/*****************************************************************/
/*
	@file Adafruit_C_ADS1X15.c
	@author Nathaniel Shantz (Arizona State University Student
	@license BSD (see license.txt)

	This is a C library for the Adafruit ADS1015 breakout board
	---> 

	Adafruit's Information:
	Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

	Personal Information:
	This header file is modified from Adafruit's official library found
	---> 
	This is written to fit a Raspberry Pi using the WiringPi libraries.
	This is also written to be compatible with the C language rather
	than C++ and Arduino.

*/
/*****************************************************************/


/*================================================================
	I2C ADRESS/BITS
	--------------------------------------------------------------*/
	#define ADS1015_ADDRESS (0x48)
/*================================================================*/

/*================================================================
	CONVERSION DELAY (in mS)
	--------------------------------------------------------------*/
	#define ADS1015_CONVERSIONDELAY (1)
	#define ADS1115_CONVERSIONDELAY (8)
/*================================================================*/

/*================================================================
	POINTER REGISTER
	--------------------------------------------------------------*/
	#define ADS1015_REG_POINTER_MASK (0x03)
	#define ADS1015_REG_POINTER_CONVERT (0x00)
	#define ADS1015_REG_POINTER_CONFIG (0x01)
	#define ADS1015_REG_POINTER_LOWTHRESH (0x02)
	#define ADS1015_REG_POINTER_HITHRESH (0x03)
/*================================================================*/

/*================================================================
	CONFIG REGISTER
	--------------------------------------------------------------*/
    #define ADS1015_REG_CONFIG_OS_MASK      (0x8000)
    #define ADS1015_REG_CONFIG_OS_SINGLE    (0x8000)  // Write: Set to start a single-conversion
    #define ADS1015_REG_CONFIG_OS_BUSY      (0x0000)  // Read: Bit = 0 when conversion is in progress
    #define ADS1015_REG_CONFIG_OS_NOTBUSY   (0x8000)  // Read: Bit = 1 when device is not performing a conversion

    #define ADS1015_REG_CONFIG_MUX_MASK     (0x7000)
    #define ADS1015_REG_CONFIG_MUX_DIFF_0_1 (0x0000)  // Differential P = AIN0, N = AIN1 (default)
    #define ADS1015_REG_CONFIG_MUX_DIFF_0_3 (0x1000)  // Differential P = AIN0, N = AIN3
    #define ADS1015_REG_CONFIG_MUX_DIFF_1_3 (0x2000)  // Differential P = AIN1, N = AIN3
    #define ADS1015_REG_CONFIG_MUX_DIFF_2_3 (0x3000)  // Differential P = AIN2, N = AIN3
    #define ADS1015_REG_CONFIG_MUX_SINGLE_0 (0x4000)  // Single-ended AIN0
    #define ADS1015_REG_CONFIG_MUX_SINGLE_1 (0x5000)  // Single-ended AIN1
    #define ADS1015_REG_CONFIG_MUX_SINGLE_2 (0x6000)  // Single-ended AIN2
    #define ADS1015_REG_CONFIG_MUX_SINGLE_3 (0x7000)  // Single-ended AIN3

    #define ADS1015_REG_CONFIG_PGA_MASK     (0x0E00)
    #define ADS1015_REG_CONFIG_PGA_6_144V   (0x0000)  // +/-6.144V range = Gain 2/3
    #define ADS1015_REG_CONFIG_PGA_4_096V   (0x0200)  // +/-4.096V range = Gain 1
    #define ADS1015_REG_CONFIG_PGA_2_048V   (0x0400)  // +/-2.048V range = Gain 2 (default)
    #define ADS1015_REG_CONFIG_PGA_1_024V   (0x0600)  // +/-1.024V range = Gain 4
    #define ADS1015_REG_CONFIG_PGA_0_512V   (0x0800)  // +/-0.512V range = Gain 8
    #define ADS1015_REG_CONFIG_PGA_0_256V   (0x0A00)  // +/-0.256V range = Gain 16

    #define ADS1015_REG_CONFIG_MODE_MASK    (0x0100)
    #define ADS1015_REG_CONFIG_MODE_CONTIN  (0x0000)  // Continuous conversion mode
    #define ADS1015_REG_CONFIG_MODE_SINGLE  (0x0100)  // Power-down single-shot mode (default)

    #define ADS1015_REG_CONFIG_DR_MASK      (0x00E0)  
    #define ADS1015_REG_CONFIG_DR_128SPS    (0x0000)  // 128 samples per second
    #define ADS1015_REG_CONFIG_DR_250SPS    (0x0020)  // 250 samples per second
    #define ADS1015_REG_CONFIG_DR_490SPS    (0x0040)  // 490 samples per second
    #define ADS1015_REG_CONFIG_DR_920SPS    (0x0060)  // 920 samples per second
    #define ADS1015_REG_CONFIG_DR_1600SPS   (0x0080)  // 1600 samples per second (default)
    #define ADS1015_REG_CONFIG_DR_2400SPS   (0x00A0)  // 2400 samples per second
    #define ADS1015_REG_CONFIG_DR_3300SPS   (0x00C0)  // 3300 samples per second

    #define ADS1015_REG_CONFIG_CMODE_MASK   (0x0010)
    #define ADS1015_REG_CONFIG_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
    #define ADS1015_REG_CONFIG_CMODE_WINDOW (0x0010)  // Window comparator

    #define ADS1015_REG_CONFIG_CPOL_MASK    (0x0008)
    #define ADS1015_REG_CONFIG_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
    #define ADS1015_REG_CONFIG_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active

    #define ADS1015_REG_CONFIG_CLAT_MASK    (0x0004)  // Determines if ALERT/RDY pin latches once asserted
    #define ADS1015_REG_CONFIG_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
    #define ADS1015_REG_CONFIG_CLAT_LATCH   (0x0004)  // Latching comparator

    #define ADS1015_REG_CONFIG_CQUE_MASK    (0x0003)
    #define ADS1015_REG_CONFIG_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
    #define ADS1015_REG_CONFIG_CQUE_2CONV   (0x0001)  // Assert ALERT/RDY after two conversions
    #define ADS1015_REG_CONFIG_CQUE_4CONV   (0x0002)  // Assert ALERT/RDY after four conversions
    #define ADS1015_REG_CONFIG_CQUE_NONE    (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)
/*================================================================*/

/*
typedef enum{
  GAIN_TWOTHIRDS    = ADS1015_REG_CONFIG_PGA_6_144V,
  GAIN_ONE          = ADS1015_REG_CONFIG_PGA_4_096V,
  GAIN_TWO          = ADS1015_REG_CONFIG_PGA_2_048V,
  GAIN_FOUR         = ADS1015_REG_CONFIG_PGA_1_024V,
  GAIN_EIGHT        = ADS1015_REG_CONFIG_PGA_0_512V,
  GAIN_SIXTEEN      = ADS1015_REG_CONFIG_PGA_0_256V
} adsGain_t;
*/


#include "Adafruit_C_ADS1X15.h"

static uint8_t i2cread(void* obj){
	//TODO: Insert read code based on struct values
	struct Adafruit_ADS1115* tmp = obj;
	return wiringPiI2CRead(tmp->m_wiringPiAddress);
}

static void i2cwrite(int i2cAddress, uint8_t x){
	//TODO: Insert right code based on struct values
	printf("Entered i2cWrite function\n");
	wiringPiI2CWrite(i2cAddress, (int)x);
}

static void writeRegister(int i2cAddress, uint8_t reg, uint16_t value){
	//TODO: Insert write code for registers with I2C protocol
	wiringPiI2CWriteReg16(i2cAddress, (int) reg, (int) (((value & 0x00FF) << 8) | (value & 0xFF00) >> 8));
	delay(200);
}

static uint16_t readRegister(int i2cAddress, uint8_t reg){
	//TODO: Insert Read code for registers with I2C protocol
	printf("Entered readRegister function\n");
	i2cwrite(i2cAddress, ADS1015_REG_POINTER_CONVERT);
	printf("Completed i2cWrite function\n");
	uint16_t tmp = (uint16_t)wiringPiI2CReadReg16(i2cAddress, (int) reg);
	printf("wiringPiRead function completed\n");
	return tmp;
	//return (uint16_t)wiringPiI2CReadReg16( i2cAddress, (int) reg);
}
//input choice of module 1(0)15 or 1(1)15 and the pointer that will by used to house it
//instantiates the struct to fit your choice
void initModule_default(int choice, struct Adafruit_ADS1115* obj){
	printf("Initializing ADS with default...\n");
	obj = malloc(sizeof(struct Adafruit_ADS1115));
	printf("Wrapping the pointer\n");
	if(choice == 0){
		//TODO: Write Code for 1015 inita
		printf("Trying to init the 10115\n");
		obj->m_i2cAddress = ADS1015_ADDRESS;
		obj->m_conversionDelay = ADS1015_CONVERSIONDELAY;
		obj->m_bitShift = 4;
		obj->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144v range (limited to VDD +0.3v max!) */
	}
	else if(choice == 1){
		printf("Trying to init the 1115\n");
		uint8_t address = ADS1015_ADDRESS;
		uint8_t cDelay = ADS1115_CONVERSIONDELAY;
		obj->m_i2cAddress = address;
		obj->m_conversionDelay = cDelay;
		obj->m_bitShift = 0;
		obj->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144v range (limited to VDD +0.3V max!) */
	}
	else{
		printf("Error Initializing\n");
	}
}
//input choice of module 1(0)15 or 1(1)15, the i2cAddress, and the pointer that will be used to house it
//instantiates the struct to fit your choice
void initModule_Address(int choice, uint8_t i2cAddress, struct Adafruit_ADS1115* obj){

	if(choice == 0){
		//TODO: Write Code for 1015 init
		obj->m_i2cAddress = i2cAddress;
		obj->m_conversionDelay = ADS1015_CONVERSIONDELAY;
		obj->m_bitShift = 4;
		obj->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
	}
	else if(choice == 1){
		//TODO: Write code for 1115 init
		obj->m_i2cAddress = i2cAddress;
		obj->m_conversionDelay = ADS1115_CONVERSIONDELAY;
		obj->m_bitShift = 0;
		obj->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144v range (limited to VDD +0.3V max!) */
	}
	else{
		printf("Error Initialziing\n");
	}
}
//sets up the HW (reads coefficients values, etc.)
void begin(struct Adafruit_ADS1115* obj){
	//TODO: Write code to begin WiringPi
	printf("Setting up wiringPi library\n");
	wiringPiSetup();
	printf("Trying to assign the wiringPiAddress to the wrapping pointer...\n");
	obj->m_wiringPiAddress = wiringPiI2CSetup(obj->m_i2cAddress);
	printf("Success! I assigned the value!\n");
}

//sets the gain and input voltage range
void setGain(struct Adafruit_ADS1115* obj, adsGain_t gain){
	obj->m_gain = gain;
}

adsGain_t getGain(struct Adafruit_ADS1115* obj){
	return obj->m_gain;
}

uint16_t readADC_SingleEnded(struct Adafruit_ADS1115* obj, uint8_t channel){
	if(channel > 3){
		return 0;
	}
	printf("Entered SingleEnded function...\n");
	//starting with default values
	uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE		| //Disable the comparator (default)
					  ADS1015_REG_CONFIG_CLAT_NONLAT 	| //Non-latching (default)
					  ADS1015_REG_CONFIG_CPOL_ACTVLOW	| //Alert/Rdy active low (default)
					  ADS1015_REG_CONFIG_CMODE_TRAD		| //Traditional comparator (default)
					  ADS1015_REG_CONFIG_DR_1600SPS		| //1600 samples per second (default)
					  ADS1015_REG_CONFIG_MODE_SINGLE;	  //Single-shot mode (default)

	//Set PGA/voltage range
	config |= obj->m_gain;
	
	//set single-ended input channel
	switch (channel){
		case(0):
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
			break;
		case(1):
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
			break;
		case(2):
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
			break;
		case(3):
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
			break;
	}
	
	//set 'start sngle-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
	
	printf("Attempting to write config settings to config register\n");

	//write config register to the ADC
	writeRegister(obj->m_wiringPiAddress, ADS1015_REG_POINTER_CONFIG, config);

	printf("Successfully wrote config settings\n");

	//Wait for the conversion to complete
	delay(obj->m_conversionDelay);

	printf("Attempting to read conversion results\n");
	//Read the conversion results
	//shift 12-bit results right 4 bits for the ADS1015
	return readRegister(obj->m_wiringPiAddress, ADS1015_REG_POINTER_CONVERT) >> obj->m_bitShift;
}

//reads conversion results, measuring the voltage difference
//between the P (AIN0) and N (AIN1) input. Generates a signed
//value since the difference can be either positive or negative
int16_t readADC_Differential_0_1(struct Adafruit_ADS1115* obj){
	//start with default values
	uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE		| //Disable the comparator (default)
					  ADS1015_REG_CONFIG_CLAT_NONLAT	| //Non-latching (default)
					  ADS1015_REG_CONFIG_CPOL_ACTVLOW	| //Alert/Rdy active low (default)
					  ADS1015_REG_CONFIG_CMODE_TRAD		| //Traditional comparator (default)
					  ADS1015_REG_CONFIG_DR_1600SPS		| //1600 samples per second (default)
					  ADS1015_REG_CONFIG_MODE_SINGLE;	  //Single-shot mode (default)

	//Set PGA/voltage range
	config |= obj->m_gain;

	//Set channels
	config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1; 			  //AIN0 = P, AIN1 = N

	//Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;

	//Write config register to the ADC
	writeRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

	//wait for the conversion to complete 
	delay(obj->m_conversionDelay);

	//Read the conversion results
	uint16_t res = readRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> obj->m_bitShift;
	if(obj->m_bitShift == 0){
		return (int16_t)res;
	}
	else{
		//Shift 12-bit results right 4 bits for the ADS1015,
		//making sure we keep the sign bit intact
		if(res > 0x07FF){
			//negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		return (int16_t)res;
	}
}

int16_t readADC_Differential_2_3(struct Adafruit_ADS1115* obj){
	//start with default values
	uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE		| //Disable the comparator (default)
					  ADS1015_REG_CONFIG_CLAT_NONLAT	| //Non-latching (default)
					  ADS1015_REG_CONFIG_CPOL_ACTVLOW	| //Alert/Rdy active low (default)
					  ADS1015_REG_CONFIG_CMODE_TRAD		| //Traditional comparator (default)
					  ADS1015_REG_CONFIG_DR_1600SPS		| //1600 samples per second (default)
					  ADS1015_REG_CONFIG_MODE_SINGLE;	  //Single-shot mode (default)

	//Set PGA/voltage range
	config |= obj->m_gain;

	//Set channels
	config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3; 			  //AIN2 = P, AIN3 = N

	//Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;

	//Write config register to the ADC
	writeRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

	//wait for the conversion to complete 
	delay(obj->m_conversionDelay);

	//Read the conversion results
	uint16_t res = readRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> obj->m_bitShift;
	if(obj->m_bitShift == 0){
		return (int16_t)res;
	}
	else{
		//Shift 12-bit results right 4 bits for the ADS1015,
		//making sure we keep the sign bit intact
		if(res > 0x07FF){
			//negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		return (int16_t)res;
	}
}
//setup the comparator to operate in basic mode, causing the ALERT/RDY
//pin to assert (go from high to low) when the ADC value exceeds the specified 
//threshold.
//This all sets the ADC in continuous conversion mode.
void startComparator_SingleEnded(struct Adafruit_ADS1115* obj, uint8_t channel, int16_t threshold){
	//start with default values
	uint16_t config = ADS1015_REG_CONFIG_CQUE_1CONV   | // Comparator enabled and asserts on 1 match
                      ADS1015_REG_CONFIG_CLAT_LATCH   | // Latching mode
                      ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                      ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                      ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                      ADS1015_REG_CONFIG_MODE_CONTIN  | // Continuous conversion mode
                      ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

	//Set PGA/voltage range
	config |= obj->m_gain;

	//Set single-ended input channel
	switch (channel){
		case(0):
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
			break;
		case(1):
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
			break;
		case(2):
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
			break;
		case(3):
			config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
			break;
	}
	
	//Set the high threshold register
	//Shift 12-bit results left 4 bits for the ADS1015
	writeRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_HITHRESH, threshold << obj->m_bitShift);

	//Write config registerto the ADC
	writeRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
}
//In order to clear the comparator, we need to read the conversion results.
//This function reads the last conversion results without changing config value
int16_t getLastConversionResults(struct Adafruit_ADS1115* obj){
	//wait for conversion to complete
	delay(obj->m_conversionDelay);

	//read conversion results
	uint16_t res = readRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> obj->m_bitShift;
	if(obj->m_bitShift == 0){
		return (int16_t)res;
	}
	else{
		//Shift 12-bit results right 4 bits for the ADS1015,
		//making sure we keep sign bit intact
		if(res > 0x07FF){
			//negative number - extend the sign to 16th bit
			res |= 0xF000;
		}

		return (int16_t)res;
	}
}
