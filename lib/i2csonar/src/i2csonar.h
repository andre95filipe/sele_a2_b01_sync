#ifndef Sonar_header
#define Sonar_header


#include <Arduino.h>
#include <util/twi.h>
#include <i2clib.h> 

//Reading modes
#define SONAR_INCHES        0
#define SONAR_CENTIMETERS   1
#define SONAR_MICROSECONDS  2

//addresses
#define SONAR_BROADCAST 0
#define SONAR_1         0xF8 //This one wonky
#define SONAR_2         0xF0

//Gains

#define MAX_GAIN_94     0x00
#define MAX_GAIN_110    0x05
#define MAX_GAIN_152    0x0D
#define MAX_GAIN_212    0x14
#define MAX_GAIN_317    0x18
#define MAX_GAIN_626    0x1D

#define D1300_DELAY     28  

void setupSonar();
void InitMeasure(uint8_t sonar_mode);
uint16_t readSonarValue(uint8_t address);
uint16_t convertMicroInSec(uint16_t micro);
void setSonarRange(uint8_t address, uint16_t range);
void setSonarGain(uint8_t address, uint8_t gain);
void sonarI2CFullLoopExample(); 
void printError(uint8_t address);
void errorHandler(uint8_t address, uint8_t error);
uint8_t smoothedInput(uint8_t* buffer, uint8_t size, uint8_t* index, uint8_t newValue, float coef);


#endif