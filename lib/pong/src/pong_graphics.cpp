#include <pong_graphics.h>
#include <pcd8544.h>


/*
Draws Rectangle in outermost pixels of the screen to give a visible playing area
*/

void PongGraphics::drawFrame() {
    LCD.setCursor(0, 0);

    LCD.write(0xFF, DATA);
    for(uint8_t i = 0; i < X - 2; i++)
        LCD.write(0x01, DATA);

    LCD.write(0xFF, DATA);

    for(uint8_t i = 0; i < Y - 1; i++) {
        LCD.setCursor(0, i);
        LCD.write(0xFF, DATA);
        LCD.setCursor(83, i);
        LCD.write(0xFF, DATA);
    }

    LCD.write(0xFF, DATA);
    for(uint8_t i = 0; i < X - 2; i++)
        LCD.write(0x80, DATA);
    LCD.write(0xFF, DATA);
}

/*
Draws the left or right paddle according to lrPaddle on a given yPos. 
Accounts for frame when it shares the pixel array.
Erases Previous drawn paddle
*/
void PongGraphics::drawPaddle(uint8_t lrPaddle, uint8_t yPos) {
    
    this->erasePaddle(lrPaddle);

    uint8_t nPixels = (yPos % 8);
    uint8_t yLine = yPos / 8;
    uint8_t framePixel = 0;
    if(lrPaddle == LEFT_PADDLE) 
        ylPaddle = yPos;
    else
        yrPaddle = yPos;
    
    LCD.setCursor(lrPaddle, yLine);
    LCD.write((uint8_t)(0xFF << nPixels) | (yLine == 0 ? 0x01 : 0x00), DATA);

    LCD.setCursor(lrPaddle, yLine + 1);
    framePixel = (yLine + 1) == 5 ? 0x80 : 0x00;
    LCD.write((uint8_t)(0xFF >> (nPixels / 8 >= 1 ? 0 : (8 - nPixels))) | framePixel, DATA);
}

/*
Erase Previuos drawn paddle. 
*/
void PongGraphics::erasePaddle(uint8_t lrPaddle) {
    uint8_t yPos = lrPaddle == LEFT_PADDLE ? ylPaddle : yrPaddle;
    uint8_t nPixels = (yPos % 8);
    uint8_t yLine = yPos / 8;
    uint8_t framePixel = 0;

    LCD.setCursor(lrPaddle, yLine);
    LCD.write((uint8_t)(0x00 << nPixels) | (yLine == 0 ? 0x01 : 0x00), DATA);

    LCD.setCursor(lrPaddle, yLine + 1);
    framePixel = (yLine + 1) == 5 ? 0x80 : 0x00;
    LCD.write((uint8_t)(0x00 >> (nPixels / 8 >= 1 ? 0 : (8 - nPixels))) | framePixel, DATA);
}

/*
Draws ball on a given x, y position.
Accounts for frame when it shares the pixel array.
Erases previous Ball
*/
void PongGraphics::drawBall(uint8_t x, uint8_t y) {
    this->eraseBall();
    uint8_t yLine = y / 8;
    ball[0] = x;
    ball[1] = y;
    LCD.setCursor(x, (uint8_t)yLine);
    LCD.write((0x01 << (y % 8)  | (yLine == 0 ? 0x01 : (yLine == 5 ? 0x80 : 0x00))), DATA);
}

/*
Erases previous drawn Ball.
*/

void PongGraphics::eraseBall() {
    if(ball[0] == 0) return;
    uint8_t x = ball[0];
    uint8_t y = ball[1];
    uint8_t yLine = y / 8;
    LCD.setCursor(x, (uint8_t)yLine);
    Serial.println(x);
    Serial.println(y);
    Serial.println(yLine);
    LCD.write((0x00 << (y % 8)  | (yLine == 0 ? 0x01 : (yLine == 5 ? 0x80 : 0x00))), DATA);
}