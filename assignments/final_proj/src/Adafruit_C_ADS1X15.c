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

#import "Adafruit_ADS1015.h"

static uint8_t i2cread(void* obj){
	//TODO: Insert read code based on struct values
	return wiringPiI2CRead(obj->m_wiringPiAddress);
}

static void i2cwrite(void* obj, uint8_t x){
	//TODO: Insert right code based on struct values
	wiringPiI2CWrite(obj->m_wiringPiAddress, (int)x);
}

static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value){
	//TODO: Insert write code for registers with I2C protocol
	wiringPiI2CWriteReg16((int) i2cAddress, (int) reg, (int) ((value >> 8) & 0xFF));
}

static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg){
	//TODO: Insert Read code for registers with I2C protocol
	wiringPiI2CReadReg16((int) i2cAddress, (int) reg);
}
//input choice of module 1(0)15 or 1(1)15 and the pointer that will by used to house it
//instantiates the struct to fit your choice
void initModule_default(int choice, void* ptr){
	if(choice == 0){
		//TODO: Write Code for 1015 init
		ptr->m_i2cAddress = i2cAddress;
		ptr->m_conversionDelay = ADS1015_CONVERSIONDELAY;
		ptr->m_bitShift = 4;
		ptr->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144v range (limited to VDD +0.3v max!) */
	}
	else if(choice == 1){
		//TODO: write code for 1115 init
		ptr->m_i2cAddress = i2cAddress;
		ptr->m_conversionDelay = ADS1115_CONVERSIONDELAY;
		ptr->m_bitShift = 0;
		ptr->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144v range (limited to VDD +0.3V max!) */
	}
	else{
		printf("Error Initializing\n");
	}
}
//input choice of module 1(0)15 or 1(1)15, the i2cAddress, and the pointer that will be used to house it
//instantiates the struct to fit your choice
void initModule_Address(int choice, uint8_t i2cAddress, void* ptr){
	if(choice == 0){
		//TODO: Write Code for 1015 init
		ptr->m_i2cAddress = i2cAddress;
		ptr->m_conversionDelay = ADS1015_CONVRSIONDELAY;
		ptr->m_bitShift = 4;
		ptr->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
	}
	else if(choice == 1){
		//TODO: Write code for 1115 init
		ptr->m_i2cAddress = i2cAddress;
		ptr->m_conversionDelay = ADS1115_CONVERSIONDELAY;
		ptr->m_bitShift = 0;
		ptr->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144v range (limited to VDD +0.3V max!) */
	}
	else{
		printf("Error Initialziing\n");
	}
}
//sets up the HW (reads coefficients values, etc.)
void begin(void* obj){
	//TODO: Write code to begin WiringPi
	wiringPiSetup();
	obj->m_wiringPiAddress = wiringPiI2CSetup(obj->m_i2cAddress);
}

//sets the gain and input voltage range
void setGain(void* obj, adsGain_t gain){
	obj->m_gain = gain;
}

adsGain_t getGain(void* obj){
	return obj->m_gain;
}

uint16_t readADC_SingleEnded(void* obj, uint8_t channel){
	if(channel > 3){
		return 0;
	}
	
	//starting with default values
	uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE		| //Disable the comparator (default)
					  ADS1015_REG_CONFIG_CFLAT_NONLAT 	| //Non-latching (default)
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
	
	//write config register to the ADC
	writeRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

	//Wait for the conversion to complete
	delay(obj->m_conversionDelay);

	//Read the conversion results
	//shift 12-bit results right 4 bits for the ADS1015
	return readRegister(obj->m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> obj->m_bitShift;
}

//reads conversion results, measuring the voltage difference
//between the P (AIN0) and N (AIN1) input. Generates a signed
//value since the difference can be either positive or negative
int16_t readADC_Differential_0_1(void* obj){
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

int16_t readADC_Differential_2_3(void* obj){
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
void startComparator_SingleEnded(void* obj, uint8_t channel){
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
int16_t getLastConversionResults(void* obj){
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
