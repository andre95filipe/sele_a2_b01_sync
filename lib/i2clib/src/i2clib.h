#include <Arduino.h>
#include <util/twi.h>


//I2C states
#define I2CREADY 	0
#define I2CBUSY 	1

#define TWI_FREQ 100000L

#define TWSRMASK 0xF8



void setupI2C();
void writeI2C(uint8_t address, uint8_t value[], uint8_t size);
void readI2C(uint8_t address, uint8_t values[], uint8_t size);
uint8_t getI2CError();
uint8_t getI2Caddress();
void clearI2Cerror();
void attachErrorHandler(void (*function)(uint8_t,uint8_t));