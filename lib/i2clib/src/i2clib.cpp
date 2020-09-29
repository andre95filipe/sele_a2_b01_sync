#include "i2clib.h"

volatile uint8_t I2Cstate;

uint8_t i2caddress;

//Send buffer info
uint8_t sendBuffer[2];
uint8_t sendPointer = 0;
uint8_t sendSize = 0;

//Receive buffer info
uint8_t receiveBuffer[10];
uint8_t receivePointer = 0;
uint8_t receiveSize = 0;

uint8_t i2cerror;

static void (*i2c_errorHandler)(uint8_t, uint8_t);


/*
Attach a function to be run when an unsupported command is detected
*/
void attachErrorHandler(void (*function)(uint8_t,uint8_t)){

	i2c_errorHandler = function;

}


/*
Initialize ATMega328p TwoWire hardware interface
*/

void setupI2C(){
    
    digitalWrite(SDA, 1);
	digitalWrite(SCL, 1);

	// initialize twi prescaler and bit rate
	/* twi bit rate formula from atmega128 manual pg 204
	SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
	note: TWBR should be 10 or higher for master mode
	It is 72 for a 16mhz Wiring board with 100kHz TWI */
	TWSR |= _BV(TWPS0) | _BV(TWPS1);

	TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

	I2Cstate = I2CREADY;

	
}

/*
Set TWCR to stop current I2C communication;
*/

void i2cstop(){

	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

	I2Cstate = I2CREADY;

}

/*
Prepare an array of uint8_t with a given size to be written on to a I2C device through the ISR.
Start I2C sommunication
*/
void writeI2C(uint8_t address, uint8_t value[], uint8_t size){


	
	//Wait for i2c to terminate comms
	while(I2Cstate != I2CREADY){
		continue;
	}
	if(i2cerror)i2c_errorHandler(i2caddress,i2cerror);
	I2Cstate = I2CBUSY;

	i2cerror = 0;

	//copy from passed buffer
	for(int i =0; i< size ; i++){
		sendBuffer[i] = value[i];
	}
	
	//Set write address
	i2caddress = address;

	sendSize = size;
	sendPointer = 0;

	//Send Start
	TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA);
}


/*
Try to read from a given device until array values is filled according to size.
Start I2C.
*/
void readI2C(uint8_t address, uint8_t values[], uint8_t size){

	//Wait for i2c to terminate comms
	while(I2Cstate != I2CREADY){
		continue;
	}
	if(i2cerror)i2c_errorHandler(i2caddress,i2cerror);

	i2cerror = 0;
	I2Cstate = I2CBUSY;

	receiveSize = size;
	receivePointer = 0;

	i2caddress = address;
	i2caddress |= TW_READ;

	TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA);

	while(I2Cstate != I2CREADY){
		continue;
	}
	if(i2cerror)i2c_errorHandler(i2caddress,i2cerror);
	for(int i =0; i< size ; i++){
		values[i] = receiveBuffer[i];
	}

}

/*
Get I2C error detected
*/
uint8_t getI2CError(){
	return i2cerror;
}

void clearI2Cerror(){
	i2cerror = 0;
}

/*
Get I2C address loaded 
*/
uint8_t getI2Caddress(){
	return i2caddress;
}

/*
Interrupt service routine for the Two Wire Interface.
Implements: START; Master Transmission of addresses and data; Master Reception of data; STOP; Aknowledgment for all previous commands.
*/

ISR(TWI_vect)
{

	uint8_t twsr0 = TWSR & TWSRMASK;

	switch (twsr0)
	{

	case (TW_START):
	case (TW_REP_START):
		TWDR = i2caddress;
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
		break;

	case(TW_MT_SLA_ACK):
	case(TW_MT_DATA_ACK):
		if(sendPointer != sendSize){
			TWDR = sendBuffer[sendPointer];
			sendPointer ++;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
		}else {
			i2cstop();
		}
		
		break;
	case(TW_MR_DATA_NACK):
		receiveBuffer[receivePointer] = TWDR;
		i2cstop();
		break;
	case(TW_MR_DATA_ACK):
		receiveBuffer[receivePointer] = TWDR;
		receivePointer++;
	case(TW_MR_SLA_ACK):
		if(receivePointer < receiveSize -1){
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
		} else TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) ;
		break;
	

	default:
		i2cerror = twsr0;
		i2cstop();
		break;
	}

}