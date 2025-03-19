

#include "seven-segment-display/seven-segment-display.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti/driverlib/dl_timer.h"
#include "ti_msp_dl_config.h"

SPIInterface SevenSegInt;
uint32_t brightness;

void initSevenSegment(void) {
    SevenSegInt.DummyByte = 0x00;
    SevenSegInt.csMask = GPIO_INT_CS2_PIN; 
    SevenSegInt.gpioInt = GPIO_INT_PORT;
    SevenSegInt.spiInt = SPI_0_INST;
    DL_TimerG_setLoadValue(PWM_0_INST, LOADVAL);
    DL_TimerG_setCaptureCompareValue(PWM_0_INST, LOADVAL >> 1, DL_TIMER_CC_0_INDEX); // default brightness 50%
    DL_TimerG_startCounter(PWM_0_INST);
}

void SevenSegmentUpdate(const uint8_t number) {
    uint8_t spiData; 
    switch (number) {
        case 0: spiData = 0b00111111; break;
        case 1: spiData = 0b00000110; break;
        case 2: spiData = 0b01011011; break;
        case 3: spiData = 0b01001111; break;
        case 4: spiData = 0b01100110; break;
        case 5: spiData = 0b01101101; break;
        case 6: spiData = 0b01111101; break;
        case 7: spiData = 0b00000111; break;
        case 8: spiData = 0b01111111; break;
        case 9: spiData = 0b01101111; break;
        default: spiData = 0b01000000; break;
    }
    SPI_write(spiData, NULL, 0, &SevenSegInt);
}

uint32_t getBrightness(void) {
    return brightness;   
}

// Brightness in Prozent
void setBrightness(uint32_t setting) {
    brightness = 100;
    if (setting < 100) {
        brightness = setting;
    }
    DL_TimerG_setCaptureCompareValue(PWM_0_INST, brightness, DL_TIMER_CC_0_INDEX); 
}