#pragma once

<<<<<<< Updated upstream
void SevenSegmentUpdate(char number);
=======

#include "ti_msp_dl_config.h"
#include "spi/spi.h"


#define LOADVAL 100  // With 50kHz Clock -> 500Hz PWM
#define SWEEP_LIMIT 100  // With PWM Frequency of 500 Hz -> 5 Hz Sweep Frequency
#define SEVENSEG_ON 0
#define SEVENSEG_SWEEP 1
#define SEVENSEG_BLINK 2

void initSevenSegment(void);
void setBrightness(uint32_t setting);
uint32_t getBrightness(void);
void SevenSegmentUpdate(const uint8_t number, uint8_t mode);
>>>>>>> Stashed changes
