#include <Arduino.h>
#include <stdlib.h>

#define MULT_PROG 0.00
#define INITIAL_BALL_SPEED 1
#define MAXBOUNCEANGLE 1.10





typedef struct paddleEntity{
    uint16_t pos;
    uint16_t size;
    uint16_t points;
} paddleEntity;

void pongInit(uint16_t widthT, uint16_t heightT, uint8_t playersT,uint16_t paddleSize);
void pongProc();
void pongReset();
void printBallPos();
uint8_t setPlayerPos(uint8_t player, uint16_t pos);
void getBallPosInt(uint16_t *eBall);
uint16_t getPlayerPoints(uint16_t nplayer);
uint16_t getPlayerSize(uint16_t nplayer);
uint16_t getPlayerPosition(uint16_t nplayer);
