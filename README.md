# Electronic Systems - Sistemas Eletrónicos


## Synchronous communication system 

### Description

* This system is composed of an arduino connected to a sonar, SRF08, supporting I2C and to a display PCD8544 support SPI.
* The I2Clib lib supports starting and stopping transmissions when the arduino is acting as a Master receiver or transmitter using the Two Wire Interface available on the ATmega328 microcontroller.
    * Writing and reading the Two Wire Interface data register is done through the Two Wire Interface interrupt service routine. 
* The i2csonar lib supports initiating reads on sonar, setting sonar range and gain, choosing sonar output between centimeters, microseconds and inches, and reading the sonar distance readings on first register.

* The spi lib supports the use of the SPI port that Arduino offers. This driver only supports only master mode, since it was the mode used in this project.
* The PCD8544 is a 84x48 pixels LCD that supports spi interface. It is used the developed spi library to control this LCD.


### Firmware implementation

#### i2clib

Library to use Arduino I2C port.

`void setupI2C()`:

* Initialize SDA and SCL lines to HIGH.
* Set Two Wire Interface Prescaler.
* Initialize I2Cstate.

`void void attachErrorHandler(void (*function)(uint8_t,uint8_t)`:

* Attach a function to be run when an unsupported command is detected.

`void i2cstop()`:
* Set TWCR to stop current I2C communication with STOP bit.

`void writeI2C(uint8_t address, uint8_t value[], uint8_t size)`:

* Wait until I2Cstate is I2CREADY.
* Set I2Cstate to I2CBUSY.
* If error is detected run error_handler.
* Reset error flag.
* Copy from value[] to internal write buffer.
* Set I2Caddress to write from address.
* Send START signal through the Two Wire Interface.

`void readI2C(uint8_t address, uint8_t values[], uint8_t size)`:

* Wait until I2Cstate is I2CREADY.
* Set I2Cstate to I2CBUSY.
* If error is detected run error_handler.
* Reset error flag.
* Set I2Caddress to read from address.
* Send START signal through the Two Wire Interface.
* Wait until I2Cstate is I2CREADY signalling end of read from device.
* Write from internal read buffer to values[].

`ISR(TWI_vect)`:

* Reads from Two Wire Interface Status register.
* Wait for aknowledged START.
* While write buffer is not empty Write to Two Wire interface Register.
* While read buffer is not full read from Two Wire interface Register.
* Set STOP at the end of operation
* If not supported status is detected send STOP and set error flag.

#### i2csonar

Library to interface the sonar with I2C.

`setupSonar()`:

* Set up I2C.
* Set I2C error handler.

`setSonarRange()`:

* Write Sonar Range address into the write buffer.
* Write range into write buffer.
* Send buffer to be written through I2C on address.

`setSonarRange()`:

* Write Sonar Gain address into the write buffer.
* Write gain into write buffer.
* Send buffer to be written through I2C on address.

`void InitMeasure(uint8_t sonar_mode, uint8_t address)`:

* Write Sonar command address into buffer.
* Write Initiate Readings command with sonar_mode offset to choose reading mode.
* Send buffer to be written through I2C on address.

`uint16_t readSonarValue(uint8_t address)`:

* Write to buffer the first sonar reading address
* Send buffer to be written through I2C on address.
* Read 2 values from I2C on address.
* Return value from joining the 2 values.

#### ponglib

Library used for Pong game logic;

`void pongInit(uint16_t widthT, uint16_t heightT, uint8_t playersT,uint16_t paddleSize)`:

* Initialize pong board with heightT and widthT.
* Initialize player stats and size.
* Use analogRead to get a random seed for initial ball direction.
* Reset board state.

`void pongReset()`:

* Set ball to screen center.
* Set initial ball speed and  random direction.

`void pongProc()`:

* Calculate next ball position.
* Detect ball collision with paddles or board limits.
* Calculate new ball Speed and speed Multiplier if collision detected.
* Award points and reset board game state if Ball reached either end of the board.
* Update Ball position.

`uint8_t setPlayerPos(uint8_t player, uint16_t pos)`:

* Get player struct from player.
* Update player paddle position with pos.
* return new position.

`void getBallPosInt(uint16_t *eBall)`:

* Updates eBall with Ball position casted to uint16_t;

`uint16_t getPlayerPosition(uint16_t nplayer)`:

* Get player struct from player.
* return player position.
 
 `uint16_t getPlayerSize(uint16_t nplayer)`:

* Get player struct from player.
* return player Size.

`uint16_t getPlayerPoints(uint16_t nplayer)`:

* Get player struct from player.
* return player points.


#### spi

Library used to control the Arduino SPI port.

`void SPIClass::begin(uint8_t data_order, uint8_t master_slave, uint8_t mode, uint8_t fosc)`:

* Setup SPI pins direction;
* Set SPI control register parameters;
* Set Slave Select pin High;
* Doesn't return.

`uint8_t SPIClass::transmit(char data)`:

* Select slave;
* Put data to SPI data regiter;
* Wait for transmission to be completed;
* Return SPI data register which contains received data.

`uint8_t SPIClass::read()`:

* Sends zeros in transmit method in order to get data;
* Return received data.

#### pcd8544

Library used to control the LCD screen PCD8544.

`void PCD8544::begin()`:

* Initialize SPI communication;
* Set pin directions;
* Set reset pulse during 100 miliseconds;
* Setup LCD screen througn a command list;
* Doesn't return.


`void PCD8544::setCursor(uint8_t x, uint8_t y)`:

* Set cursor in X, LINE postion;
* Doesn't return.

`void PCD8544::clear()`:

* Writes zeros all over the screen to clear;
* Doesn't return.

`void PCD8544::drawBitmap()`:

* Like the clear method, but this one prints given data;
* Doesn't return.

`void PCD8544::write(uint8_t data, uint8_t dc)`:

* Send data or command;
* Doesn't return.

`void PCD8544::print(const char c)`:

* Prints a given character from a matrix present in `pcd8544_ascii.h`;
* Doesn't return;
 
 `void PCD8544::print(const char *c)`:

* Prints a string by printing each character;
* Doesn't return.

#### pong_graphics

Library used to print Pong game graphics.

`void PongGraphics::drawFrame()`:

* Draws upper, lower and side lines in order to create a frame;
* Returns nothing;

`void PongGraphics::drawPaddle(uint8_t lrPaddle, uint8_t yPos)`:

* Draws left or rigth paddle according to a Y coordinate that corresponds to the coordinate of the top of the paddle.
* The paddle is 9 bits tall, so it can occupy at most two lines.
* Calculate both lines pixels;
* Doesn't return.

`void PongGraphics::erasePaddle(uint8_t lrPaddle)`:

* Erase paddle so it can print another one;
* Down't return.

`void PongGraphics::drawBall(uint8_t x, uint8_t y)`:

* Draw ball in the given X and Y coordinates;
* Doesn't return.

`void PongGraphics::eraseBall()`:

* Erase ball so it can print the next one;
* Doesn't return.


### Firmware test

* Clone this repository with `https://git.fe.up.pt/up201808899/sele_a2_b01_sync.git`;
* Compile the *main.cpp* file with PlatformIO to get *.hex* file;
* Flash the *.hex* file to the Arduino board;
* Enjoy the game :).


### Schematic

![Schematic](https://image.easyeda.com/histories/a3626a8db777473eabb22a960ebfe04c.png)

### PCB layout

![Pcb](https://image.easyeda.com/histories/5b2d65338e8f468fa396a43fa75c85d7.png)
