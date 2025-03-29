#pragma once

#include <stdint.h>

void init_fdc1004(void);
uint16_t get_device_id(void);
uint32_t get_measurement(uint8_t channel);