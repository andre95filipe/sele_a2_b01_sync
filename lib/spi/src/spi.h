#include <Arduino.h>

#define CLK     PORTB5 // 13 - SCK
#define MISO    PORTB4 // 12 - NONE
#define MOSI    PORTB3 // 11 - DNK
#define SS      PORTB2 // 10 - SCE

#define MSB       0
#define LSB       1

#define MASTER    1
#define SLAVE     0

#define MODE0     0
#define MODE1     1
#define MODE2     2
#define MODE3     3

#define FOSC_4    0
#define FOSC_16   1
#define FOSC_64   2
#define FOSC_128  3

#define FOSC_DOUBLE_2    4
#define FOSC_DOUBLE_8    5
#define FOSC_DOUBLE_32   6
#define FOSC_DOUBLE_64   7

class SPIClass {
    private:
        
    public:
        void begin(uint8_t data_order, uint8_t master_slave, uint8_t mode, uint8_t fosc);
        uint8_t transmit(char data);
        uint8_t read();
        void close();
};

extern SPIClass SPI;