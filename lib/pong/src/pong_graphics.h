#include <stdint.h>

#define X               84
#define Y_PIXELS        48

#define LEFT_PADDLE     3
#define RIGHT_PADDLE    (X - 4)
#define PADDLE_HEIGHT   9
#define PADDLE_CENTER   (((Y_PIXELS - PADDLE_HEIGHT) / 2) + 1)

class PongGraphics {
    private:
        uint8_t ball[2], ylPaddle, yrPaddle;
    public:
        void drawFrame();
        void drawPaddle(uint8_t lrPaddle, uint8_t yPos);
        void drawBall(uint8_t x, uint8_t y);
        void drawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
        void eraseBall();
        void erasePaddle(uint8_t lrPaddle);
};