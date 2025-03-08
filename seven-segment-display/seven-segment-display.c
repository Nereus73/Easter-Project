#include "spi/spi.h"
#include "ti/driverlib/dl_spi.h"

void SevenSegmentUpdate(const char number) {
    uint8_t spiData; 

    switch (number) {
        case '0': spiData = 0b00111111; break;
        case '1': spiData = 0b00000110; break;
        case '2': spiData = 0b01011011; break;
        case '3': spiData = 0b01001111; break;
        case '4': spiData = 0b01100110; break;
        case '5': spiData = 0b01101101; break;
        case '6': spiData = 0b01111101; break;
        case '7': spiData = 0b00000111; break;
        case '8': spiData = 0b01111111; break;
        case '9': spiData = 0b01101111; break;
        default: spiData = 0b01000000; break;
    }
    SPI_Controller_transmitData(&spiData, 1);
}