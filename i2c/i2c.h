#pragma once

#include "ti_msp_dl_config.h"


uint8_t I2C_write(uint8_t addr, uint8_t *buf, uint16_t len);
uint8_t I2C_read(uint8_t addr, uint8_t *buf, uint16_t len);
void I2C_Delayms(uint32_t period);
void I2C_Delayus(uint32_t period, void *intf_ptr);