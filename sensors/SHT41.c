#include "sensors/SHT41.h"



static sht4x_handle_t SHT41_Handle;

void initSHT41(void) {
    SHT41_Handle.iic_write_cmd = I2C_write;
    SHT41_Handle.iic_read_cmd = I2C_read;
    SHT41_Handle.delay_ms = I2C_Delayms;
    SHT41_Handle.debug_print = debug_print;
    SHT41_Handle.iic_init = iic_init;
    SHT41_Handle.iic_deinit = iic_deinit;
    sht4x_set_addr(&SHT41_Handle, 0x44);
    sht4x_init(&SHT41_Handle);
}

static uint8_t iic_init(void) {
    I2C_Delayus(10, NULL);  // Not necessary but driver need init function
    return 0;
}

static uint8_t iic_deinit(void) {
    I2C_Delayus(10, NULL);  // Not necessary but driver need init function
    return 0;
}

uint8_t getTempHum(uint16_t *temperature_raw, float *temperature_s, uint16_t *humidity_raw, float *humidity_s) {
    return sht4x_read(&SHT41_Handle, SHT4X_MODE_HIGH_PRECISION_WITH_NO_HEATER, 
                   temperature_raw, temperature_s, 
                   humidity_raw, humidity_s);
}