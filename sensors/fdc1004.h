#pragma once

#include <stdint.h>

typedef enum {
    FDC1004_CHA,
    FDC1004_CHB 
} FDC1004_config_channel;

typedef enum {
    CIN1,
    CIN2,
    CIN3,
    CIN4,
    CAPDAC,
    DISABLED = 0b111
} FDC1004_config_channel_item;

typedef enum {
    MEAS1_MSB,
    MEAS1_LSB,
    MEAS2_MSB,
    MEAS2_LSB,
    MEAS3_MSB,
    MEAS3_LSB,
    MEAS4_MSB,
    MEAS4_LSB,
    CONF_MEAS1,
    CONF_MEAS2,
    CONF_MEAS3,
    CONF_MEAS4,
    FDC_CONF,
    OFFSET_CAL_CIN1,
    OFFSET_CAL_CIN2,
    OFFSET_CAL_CIN3,
    OFFSET_CAL_CIN4,
    GAIN_CAL_CIN1,
    GAIN_CAL_CIN2,
    GAIN_CAL_CIN3,
    GAIN_CAL_CIN4,
    MANUF_ID = 0xfe,
    DEVICE_ID = 0xff
};

void init_fdc1004(void);
uint16_t get_device_id(void);
uint32_t get_measurement(uint8_t channel);
void configure_channel(uint8_t measurement, FDC1004_config_channel_item cha, FDC1004_config_channel_item chb, uint8_t capdac);