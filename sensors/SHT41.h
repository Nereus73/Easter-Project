#pragma once

#include "sensors/driver_sht4x.h"
#include "i2c/i2c.h"
#include "uart/uart.h"

void initSHT41(void);
static uint8_t iic_init(void);
static uint8_t iic_deinit(void);
uint8_t getTempHum(uint16_t *temperature_raw, float *temperature_s, uint16_t *humidity_raw, float *humidity_s);


