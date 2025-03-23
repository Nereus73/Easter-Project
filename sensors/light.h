#pragma once


#include "OPT4001.h"
#include "i2c/i2c.h"
#include "uart/uart.h"
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

void lightInit(void);
void light_getData(uint32_t *lux_raw);

#ifdef __cplusplus
}
#endif
