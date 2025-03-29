#include <stdint.h>

#include "fdc1004.h"
#include "i2c/i2c.h"

const uint8_t FDC1004_ADDRESS = 0x50;

void init_fdc1004(void) {
    uint8_t buffer[16] = {0x0c, 0x05, 0xf0};

    I2C_write(FDC1004_ADDRESS, buffer, 3);
}

uint16_t get_device_id(void) {
    uint8_t buffer[16] = {0xff};

    I2C_write(FDC1004_ADDRESS, buffer, 1);
    I2C_read(FDC1004_ADDRESS, buffer, 2);

    return buffer[1] | (buffer[0] << 8);
}

uint32_t get_measurement(const uint8_t channel) {
    uint8_t buffer[16] = {channel};
    I2C_write(FDC1004_ADDRESS, buffer, 1);
    I2C_read(FDC1004_ADDRESS, buffer, 4);

    const uint32_t result = buffer[2] | (buffer[1] << 8) | (buffer[0] << 16);

    return result;
}