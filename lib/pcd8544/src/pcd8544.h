#include <Arduino.h>

/*
#define CLK     PORTB5 // 13 - SCK
#define MISO    PORTB4 // 12 - NONE
#define MOSI    PORTB3 // 11 - DNK
#define SS      PORTB2 // 10 - SCE
*/

#define DC  PORTB0  // 8
#define RST PORTB1  // 9

#define DATA    1
#define COMMAND 0

#define X           84
#define Y           6
#define Y_PIXELS    48


class PCD8544 {
    private:

    public:
        void begin();
        void clear();
        void write(uint8_t data, uint8_t dc);
        void print(const char c);
        void print(const char *c);
        void setCursor(uint8_t column, uint8_t line);
        void drawBitmap();
};

extern PCD8544 LCD;
