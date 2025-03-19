#pragma once


#include "ti_msp_dl_config.h"
#include "spi/spi.h"

#define LOADVAL 100  // With 50kHz Clock -> 500Hz PWM

void initSevenSegment(void);
void setBrightness(uint32_t setting);
uint32_t getBrightness(void);
void SevenSegmentUpdate(const uint8_t number);