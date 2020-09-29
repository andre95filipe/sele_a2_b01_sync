#include "ponglib.h"

paddleEntity player1,player2;

//Ball Info
double ballPos[2]; //[x,y]
double ballSpeed[2];
uint16_t width,height;
double speedMultiplier;

uint8_t players;




//

/*
Initialize Pong board with defined width, height and paddleSize.
Must be called before doing anything else.
*/
void pongInit(uint16_t widthT, uint16_t heightT, uint8_t playersT,uint16_t paddleSize){

    //Init paddles and ball

    

    height = heightT;
    width = widthT;
    players = playersT;

    player1.points = 0;
    player2.points = 0;
    player1.size = paddleSize;
    player2.size = paddleSize;

    players = playersT;
    randomSeed(analogRead(ADC1D));
    Serial.printf("weight %d %d\n",  height, width);
    pongReset();
    
}


/*
Reset ball location and speed and get new random direction for ball start.
*/
void pongReset(){

    ballPos[0] = (double)width/2;
    ballPos[1] = (double)height/2;
    //Serial.printf("weikofoi %d %d\n",  (uint16_t)ballPos[0], (uint16_t)ballPos[1]);
   
    speedMultiplier = INITIAL_BALL_SPEED;

    //Random initial direction
    short direction = random()%2;
    if (direction) ballSpeed[0] = INITIAL_BALL_SPEED;
    else ballSpeed[0] = -INITIAL_BALL_SPEED;
    //first ball horizontal
    ballSpeed[1] = 0;

}

/*
Call each loop cycle to get next board state.
Updates Ball location, calculates colision with board boarders and paddles.
Detects when a player scores a point and resets board.
*/
void pongProc(){

    uint16_t tempPos[2];
    double relativePos;
    double bounceAngle = 0;

    tempPos[0] = ballPos[0] + (ballSpeed[0] * speedMultiplier);
    tempPos[1] = ballPos[1] + (ballSpeed[1] * speedMultiplier);
    //paddle colision

    if ((uint16_t)tempPos[0] == 0){
        if (tempPos[1] >= player1.pos && tempPos[1] <= player1.pos + player1.size){
            
            relativePos = (player1.pos + (player1.size/2)) - (double)tempPos[1];
            relativePos = relativePos/(player1.size/2);
            bounceAngle = relativePos * MAXBOUNCEANGLE;
            ballSpeed[0] = speedMultiplier * cos(bounceAngle);
            ballSpeed[1] = speedMultiplier * (sin(bounceAngle));
        }else {
            player2.points ++;
            pongReset();
        }
    }else if((uint16_t)tempPos[0] >= width){
        if (tempPos[1] >= player2.pos && tempPos[1] <= player2.pos + player2.size){
            Serial.write('w');
            relativePos = (player2.pos +(player2.size/2)) - (double)tempPos[1];
            relativePos = relativePos/(player2.size/2);
            bounceAngle = relativePos * MAXBOUNCEANGLE;
            ballSpeed[0] = -(speedMultiplier * cos(bounceAngle));
            ballSpeed[1] = speedMultiplier * (sin(bounceAngle));
        }else {
            player1.points ++;
            pongReset();
        }

    }
    if((uint16_t)tempPos[1] == height || (uint16_t)tempPos[1] == 0){
        ballSpeed[1] = -ballSpeed[1];
    }

    ballPos[0] = ballPos[0] + (ballSpeed[0] * speedMultiplier);
    ballPos[1] = ballPos[1] + (ballSpeed[1] * speedMultiplier);

}


/*
Set a giver player position to a valid position on the board.
The position passed should be the top row of the paddle
*/
uint8_t setPlayerPos(uint8_t player, uint16_t pos){
    switch(player){
        case(1):
            player1.pos = pos;
            break;
        case(2):
            player2.pos = pos;
            break;
        default:
            return -1;
    }
    return pos;
}

/*
Update a uint16_t array with at least size 2 with ball position
[0] - Coordinate X.
[1] - Coordinate Y.
*/
void getBallPosInt(uint16_t *eBall){
    
    eBall[0] = (uint16_t) ballPos[0] + 3;
    eBall[1] = (uint16_t) ballPos[1] + 1;
}

/*
Retrieve a given player position.
*/
uint16_t getPlayerPosition(uint16_t nplayer){
    switch(nplayer){
        case(1):
            return player1.pos + 1;
        case(2):
            return player2.pos + 1;
    }
    return -1;
}
/*
Retrieve a given player paddle size
*/
uint16_t getPlayerSize(uint16_t nplayer){
    switch(nplayer){
        case(1):
            return player1.size;
        case(2):
            return player2.size;
    }
    return -1;
}

/*
Retrieve a given player Points.
*/
uint16_t getPlayerPoints(uint16_t nplayer){
    switch(nplayer){
        case(1):
            return player1.points;
        case(2):
            return player2.points;
    }
    return -1;
}

/*
Print ball position
*/
void printBallPos(){
    Serial.printf("BallPos: %d, %d\n", (uint16_t)ballPos[0], (uint16_t)ballPos[1]); 
    
}