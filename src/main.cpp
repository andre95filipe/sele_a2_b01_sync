#include <Arduino.h>
#include <i2csonar.h> 
#include <pcd8544.h>
#include <ponglib.h>
#include <pong_graphics.h>

PongGraphics Graphics;


void setup() {
	setupSonar();
	setSonarRange(0, 1300); 			//In milimeters, lower range quicker reads
	setSonarGain(0, MAX_GAIN_626);		//Ignore to allow MAXGAIN	

	LCD.begin();

	LCD.setCursor(0, 0);

	Serial.begin(9600);
	Serial.println(X - LEFT_PADDLE - RIGHT_PADDLE);
	Serial.println(Y_PIXELS - 2);

	pongInit(X - LEFT_PADDLE - 4, Y_PIXELS - 1, 2, 9);

	setPlayerPos(1, 19);
	setPlayerPos(2,  19);


	//

	LCD.drawBitmap();
	delay(5000);
	LCD.clear();
	Graphics.drawFrame();
	InitMeasure(SONAR_MICROSECONDS);
	delay(D1300_DELAY);
	
} // ((Y_PIXELS - 2) / 2)

uint8_t buff[5], ptr, sonar;
uint16_t ball[2];
uint8_t read = 0;

void loop() {
	pongProc();
	getBallPosInt(ball);
	// Draw 
	Graphics.drawPaddle(LEFT_PADDLE, read + 1);

	uint8_t ypos2 = ball[1] > 41 ? 38 : ball[1] < 3 ? 0 : ball[1] - 3;
	Graphics.drawPaddle(RIGHT_PADDLE, ypos2);
	setPlayerPos(2,ypos2);
	printBallPos();
	Graphics.drawBall(ball[0], ball[1]);
	
	InitMeasure(SONAR_CENTIMETERS);

	

	delay(120);
	sonar = readSonarValue(SONAR_2);
	Serial.println(sonar);
	read = smoothedInput(buff, 5, &ptr, sonar, 0.5);

	read = (read < 15) ? 15 : read;
	read = (read > 53) ? 53 : read;

	read = read - 15;

	setPlayerPos(1,read);



}

