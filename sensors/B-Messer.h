#pragma once


void bmi2_aux_conf(struct bmi2_dev *bmi);
int8_t set_accel_config(struct bmi2_dev *bmi);
void getSensorData(uint16_t *accX, uint16_t *accY, uint16_t *accZ);
int32_t bmi2_aux_i2c_write(void * dev, uint8_t reg_addr, const uint8_t *aux_data, uint16_t len);
int32_t bmi2_aux_i2c_read(void * dev, uint8_t reg_addr, uint8_t *aux_data, uint16_t len);
void initIIS2MDC(void);
void initBMI270(SPIInterface *spiInt);
void Refresh_Temperature(void);