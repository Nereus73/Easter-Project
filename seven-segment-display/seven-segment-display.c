<<<<<<< Updated upstream
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
=======
#include "seven-segment-display/seven-segment-display.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti/driverlib/dl_timer.h"
#include "ti_msp_dl_config.h"


SPIInterface SevenSegInt;
uint32_t brightness;
volatile uint32_t timCount = 0;

struct Symbol{
    uint8_t value;
    uint8_t mode;
    uint8_t masks[8];
    uint8_t maskLen;
    uint8_t curPos;
};

volatile struct Symbol symbol;


void initSevenSegment(void) {
    SevenSegInt.DummyByte = 0x00;
    SevenSegInt.csMask = GPIO_INT_CS2_PIN; 
    SevenSegInt.gpioInt = GPIO_INT_PORT;
    SevenSegInt.spiInt = SPI_0_INST;
    DL_TimerG_setLoadValue(PWM_0_INST, LOADVAL);
    DL_TimerG_setCaptureCompareValue(PWM_0_INST, LOADVAL >> 1, DL_TIMER_CC_0_INDEX); // default brightness 50%
    DL_TimerG_startCounter(PWM_0_INST);
}

void SevenSegmentUpdate(const uint8_t number, uint8_t mode) {
    uint8_t tempNum;
    switch (number) {
        case 0: tempNum = 0b00111111; break;
        case 1: tempNum = 0b00000110; break;
        case 2: tempNum = 0b01011011; break;
        case 3: tempNum = 0b01001111; break;
        case 4: tempNum = 0b01100110; break;
        case 5: tempNum = 0b01101101; break;
        case 6: tempNum = 0b01111101; break;
        case 7: tempNum = 0b00000111; break;
        case 8: tempNum = 0b01111111; break;
        case 9: tempNum = 0b01101111; break;
        case 10: tempNum = 0b01011100; break;  // o
        default: tempNum = 0b01000000; break;
    }
    if ((tempNum == symbol.value) && (mode == symbol.mode)) {
        return;
    }
    symbol.value = tempNum;
    symbol.maskLen = 0;
    symbol.curPos = 0;
    symbol.mode = mode;
    for(uint8_t i = 0; i < 8; i++) {
        if ((symbol.value & (0x01 << i)) > 0) {
            symbol.masks[symbol.maskLen++] = 0x01 << i;
        }
    }
    if (mode == SEVENSEG_BLINK) {
        symbol.maskLen = 1;
        symbol.masks[0] = symbol.value;
    }
    if (symbol.maskLen == 1) {
        symbol.masks[symbol.maskLen] = 0;
        symbol.maskLen++;
    }
    if (symbol.maskLen == 0) {
        symbol.mode = SEVENSEG_ON;
    }
    if (symbol.mode == SEVENSEG_ON) {
        NVIC_DisableIRQ(PWM_0_INST_INT_IRQN);
        SPI_write(symbol.value, NULL, 0, &SevenSegInt);     
    } else {
        NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);
    }
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

void sweepRefresh(void) {
    if (symbol.mode == SEVENSEG_ON) { return; }
    symbol.curPos++;
    if (symbol.curPos >= symbol.maskLen) {
        symbol.curPos = 0;
    }
    SPI_write(symbol.masks[symbol.curPos], NULL, 0, &SevenSegInt);
}

void PWM_0_INST_IRQHandler(void) {
    switch (DL_TimerG_getPendingInterrupt(PWM_0_INST)) {
        case DL_TIMERG_IIDX_LOAD:
            break;
        case DL_TIMERG_IIDX_ZERO:
            if (timCount > SWEEP_LIMIT) {
                timCount = 0;
                sweepRefresh();
            }
            timCount++;
            break;
    }   
>>>>>>> Stashed changes
}