#pragma once

#include <stdint.h>

typedef enum {
    MEAS1_MSB         = 0x00, // MSB portion of Measurement 1
    MEAS1_LSB         = 0x01, // LSB portion of Measurement 1
    MEAS2_MSB         = 0x02, // MSB portion of Measurement 2
    MEAS2_LSB         = 0x03, // LSB portion of Measurement 2
    MEAS3_MSB         = 0x04, // MSB portion of Measurement 3
    MEAS3_LSB         = 0x05, // LSB portion of Measurement 3
    MEAS4_MSB         = 0x06, // MSB portion of Measurement 4
    MEAS4_LSB         = 0x07, // LSB portion of Measurement 4
    CONF_MEAS1        = 0x08, // Measurement 1 Configuration
    CONF_MEAS2        = 0x09, // Measurement 2 Configuration
    CONF_MEAS3        = 0x0A, // Measurement 3 Configuration
    CONF_MEAS4        = 0x0B, // Measurement 4 Configuration
    FDC_CONF          = 0x0C, // Capacitance to Digital Configuration
    OFFSET_CAL_CIN1   = 0x0D, // CIN1 Offset Calibration
    OFFSET_CAL_CIN2   = 0x0E, // CIN2 Offset Calibration
    OFFSET_CAL_CIN3   = 0x0F, // CIN3 Offset Calibration
    OFFSET_CAL_CIN4   = 0x10, // CIN4 Offset Calibration
    GAIN_CAL_CIN1     = 0x11, // CIN1 Gain Calibration
    GAIN_CAL_CIN2     = 0x12, // CIN2 Gain Calibration
    GAIN_CAL_CIN3     = 0x13, // CIN3 Gain Calibration
    GAIN_CAL_CIN4     = 0x14, // CIN4 Gain Calibration
    MANUFACTURER_ID   = 0xFE, // ID of Texas Instruments
    DEVICE_ID         = 0xFF  // ID of FDC1004 device
} FD1004CRegister;

void initFDC1004(void);
uint16_t getDeviceID(void);
uint16_t getMeasurement(uint8_t channel);