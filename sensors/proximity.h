#pragma once

#include "ti_msp_dl_config.h"
#include "sensors/LDC1314.h"


void inductiveInit(void);
uint8_t inductiveGetData(uint16_t* data);
uint8_t inductiveGetManID(uint16_t* manID);
