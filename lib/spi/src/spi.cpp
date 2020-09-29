#include <spi.h>


void SPIClass::begin(uint8_t data_order, uint8_t master_slave, uint8_t mode, uint8_t fosc) {
    DDRB |= _BV(CLK) | 
            _BV(MOSI) |
            _BV(SS);

    SPCR |= _BV(SPE) | 
            (data_order << DORD) | 
            (master_slave << MSTR) | 
            (mode << CPHA) |
            (fosc << SPR0);

    SPSR |= ((fosc & 4) << SPI2X);
    PORTB |= _BV(SS);
}

uint8_t SPIClass::transmit(char data) {
    PORTB &= ~_BV(SS);
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));
    PORTB |= _BV(SS);
    return SPDR;
}

uint8_t SPIClass::read() {
    return this->transmit(0x00);
}
