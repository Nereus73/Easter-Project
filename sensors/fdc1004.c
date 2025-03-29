#include <stdint.h>

#include "fdc1004.h"
#include "i2c/i2c.h"

const uint8_t FDC1004_ADDRESS = 0x50;

void init_fdc1004(void) {
    uint8_t buffer[16] = {FDC_CONF, 0x05, 0xf0};

    I2C_write(FDC1004_ADDRESS, buffer, 3);
}

uint16_t get_device_id(void) {
    uint8_t buffer[16] = {DEVICE_ID};

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

void configure_channel(const uint8_t measurement, const FDC1004_config_channel_item cha, const FDC1004_config_channel_item chb, const uint8_t capdac) {
    uint16_t config = 0;
    uint8_t buffer[1] = {measurement};

    if (cha <= CIN4){
        config |= cha << 13;
    }
        
    config |= chb << 10;

    if (capdac <= 31) {
        config |= capdac << 5;
    }

    if (config == 5){}

    I2C_write(FDC1004_ADDRESS, buffer, 1);
    I2C_write(FDC1004_ADDRESS, (void*)&config, 2);
}