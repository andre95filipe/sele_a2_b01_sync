#include "i2csonar.h"

uint8_t sonarAddress;
uint16_t *sonarRange;
uint8_t *sonarGain;

/*
Setup I2C for sonar communication with error handling in case the sonar gets disconnected.
Needs to be called first in setup.
*/
void setupSonar(){


	setupI2C();
	attachErrorHandler(&errorHandler);

	sonarRange = (uint16_t*)malloc(2);
	sonarGain = (uint8_t*)malloc(1);
}

/*
To be called by I2C when an unsupported command is detected.
Usually means that the sonar has been disconnected and configuration needs to be reloaded.
*/
void errorHandler(uint8_t address, uint8_t error){
	Serial.println(error);
	clearI2Cerror();

	//uint8_t values[2];

	setSonarRange(address, *sonarRange);
	// values[0] = 0x02;
	// values[1] = *sonarRange/43;
	// Serial.println("range");
	// Serial.println(*sonarRange);

	setSonarGain(address, *sonarGain);
	//writeI2C(address, values, 2);

	// values[0] = 0x01;
	// values[1] = *sonarGain;

	// writeI2C(address, values, 2);


}

void printError(uint8_t address){



	
	
	//Serial.print(error);
	//If error detected try to reset sonar settings.
	//Without this when trying to read faster than 67ms at lower range if the sonar disconnects it will reset to max range and be unable to get distance on time
	while(getI2CError()){
		Serial.print(getI2CError());
		clearI2Cerror();
		setSonarRange(address & ~0x01, 1300); 			
		setSonarGain(address & ~0x01,MAX_GAIN_626);
		delay(5);
	
	}
	


}

/*
Default Function to initiate measure on all conected sonars.
Broadcasts,sends to address 0, Initiate Measure command.
sonar_mode can be SONAR_MICROSECONDS, SONAR_CENTIMETERS, SONAR_INCHES which makes the sonar return the distance in microseconds, centimeters or inches respectively.
Must wait Dxxxx_DELAY before ReadSonarValue can return a valid answer. 
*/
void InitMeasure(uint8_t sonar_mode){

	uint8_t values[2];

	if(sonar_mode > 2) return;

	values[0] = 0x00;
	values[1] = 0x50 + sonar_mode;
	//if(getI2CError()) printError(address);
	writeI2C(0,values,2);

}

/*
Same as default function but addressed to a single sonar.
*/
void InitMeasure(uint8_t sonar_mode, uint8_t address){

	uint8_t values[2];

	if(sonar_mode > 2) return;

	values[0] = 0x00;
	values[1] = 0x50 + sonar_mode;
	//if(getI2CError()) printError(address);
	writeI2C(address,values,2);

}

/*
Reads and returns first pair of registers on sonar wich represent the first echo. 
*/
uint16_t readSonarValue(uint8_t address){

	uint8_t buffer[2];
	uint16_t res=0;

	
	buffer[0] = 0x02;
	writeI2C(address,buffer,1);
	//if(getI2CError()) printError(address);

	

	buffer[0] = 0;
	readI2C(address, buffer, 2);

	//if(getI2CError()) printError(address);

	res = buffer[0] << 8;
	res |= buffer[1];

	return res;
}


/*
If reading in microseconds for better accuracy, can use this function to convert into centimenters.
*/
uint16_t convertMicroInSec(uint16_t micro){

	float temp = micro;			
 	temp = temp / 20000;               //Convert microseconds to seconds
  	temp = temp * 343;                 //Get the distance in meters using the speed of sound (343m/s)               

	return temp;   

}

/*
Sets the sonar with a given address to max distance readings at a certain range in millimeters with an accuracy of 43 millimiters
A smaller distance makes for faster sonar readings
*/
void setSonarRange(uint8_t address, uint16_t range){

	uint8_t values[2];

	values[0] = 0x02;
	values[1] = range/43;
	*sonarRange = range;
	writeI2C(address, values, 2);


}

/*
When working with several sonars or trying to read from sonar too fast, set smaller gain to avoid misreadings from previous runs.
*/
void setSonarGain(uint8_t address, uint8_t gain){

	uint8_t values[2];


	values[0] = 0x01;
	values[1] = gain;
	*sonarGain = gain;
	writeI2C(address, values, 2);


}
/*
Smooths input reading from sonars to avoid jerking motion.
Buffer to save previous readings, size of buffer and pointer to keep track of new readings position.
newValue is the most recent value read from sonar.
coef is used for calculating the Exponential Moving Average. Bigger coefs give bigger weight to newest readings. Must be smaller than 1.
*/
uint8_t smoothedInput(uint8_t* buffer, uint8_t size, uint8_t* index, uint8_t newValue, float coef){
	
	
	double output = 0;
	
	buffer[*index] = newValue;

	uint8_t tempIndex = *index;


	//Exponential Moving average
	for(int i =0; i< size;i++){
		double kek = pow(1-coef, i);
		output += buffer[tempIndex] * kek;
		tempIndex = (tempIndex == size-1) ? 0 : tempIndex+1;	
	}
	//End math and add 1 to round up
	output = coef * output + 1;

	if(*index == (size-1)) *index = 0;
	else *index = *index + 1;

	return output;

}
/*
uint8_t pointer = 0;
uint16_t buffer[8] ={0} ;
void sonarI2CFullLoopExample(){

	InitMeasure(SONAR_MICROSECONDS);
	delay(D1300_DELAY);
	
	

	uint16_t temp2;
	//uint16_t temp2 = readSonarValue(SONAR_1);
	//temp2 = convertMicroInSec(temp2);
	//Serial.println("NewReadings1");
	//Serial.println(temp2);
	temp2 = readSonarValue(SONAR_2);
	temp2 = convertMicroInSec(temp2);
	uint16_t temp3 = smoothedInput(buffer, 8,&pointer,temp2,0.5);
	Serial.println("NewReadings2");
	Serial.printf("Immediate %d: \tAverage: %d \n", temp2, temp3);

	

}
*/