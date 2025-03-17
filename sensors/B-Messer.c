#include "sensors/bmi2.h"
#include "sensors/bmi2_defs.h"
#include "ti_msp_dl_config.h"

#include "sensors/pressure.h"
#include "sensors/bmi270.h"
#include "sensors/iis2mdc_reg.h"

// variables for main BMI270 driver
static DL_SPI_CHIP_SELECT chipSelectAcc = DL_SPI_CHIP_SELECT_0;
struct bmi2_dev dev;

// variables for main Magnetometer driver
stmdev_ctx_t devMag;

// Variables for data
uint16_t temperature_data;
float temperature_value;

int32_t bmi2_aux_i2c_write(void * dev, uint8_t reg_addr, const uint8_t *aux_data, uint16_t len) {
    int32_t rslt;
    rslt = (int32_t)bmi2_write_aux_man_mode(reg_addr, aux_data, len, (struct bmi2_dev*) dev);
    return rslt;
}

int32_t bmi2_aux_i2c_read(void * dev, uint8_t reg_addr, uint8_t *aux_data, uint16_t len) {
    int32_t rslt;
    rslt = (int32_t)bmi2_read_aux_man_mode(reg_addr, aux_data, len, (struct bmi2_dev*) dev);
    return rslt;
}

/*!
 * @brief This function configures auxiliary sensor (IIS2MDC)
 */
void bmi2_aux_conf(struct bmi2_dev *bmi) {
    int8_t rslt;
    struct bmi2_sens_config bmi2_conf;
    bmi2_conf.type = BMI2_AUX;
    bmi2_conf.cfg.aux.odr = BMI2_AUX_ODR_12_5HZ;
    bmi2_conf.cfg.aux.aux_en = BMI2_ENABLE;
    bmi2_conf.cfg.aux.i2c_device_addr = 0x1E;
    bmi2_conf.cfg.aux.read_addr = IIS2MDC_OUTX_L_REG;
    bmi2_conf.cfg.aux.man_rd_burst = BMI2_AUX_READ_LEN_0; // 1 Byte to read from Magnetometer
    bmi2_conf.cfg.aux.aux_rd_burst = BMI2_AUX_READ_LEN_2; // 6 Bytes to read from Magnetometer
    bmi2_conf.cfg.aux.fcu_write_en = BMI2_ENABLE;
    bmi2_conf.cfg.aux.manual_en = BMI2_ENABLE;
    rslt = bmi2_set_sensor_config(&bmi2_conf, 1, bmi);
}

void initIIS2MDC(void) {
    // Initialize all BMI values for IIS
    bmi2_aux_conf(&dev);
    // Initialize all IIS values
    devMag.mdelay = Delayms;
    devMag.read_reg = bmi2_aux_i2c_read;
    devMag.write_reg = bmi2_aux_i2c_write;
    devMag.handle = &dev;
    uint8_t deviceID = 0;
    iis2mdc_device_id_get(&devMag, &deviceID);
}

void initBMI270(void) {
    // BMI2 Interface init
    dev.intf = BMI2_SPI_INTF;
    dev.read = SPI_read;
    dev.write = SPI_write;
    dev.intf_ptr = &chipSelectAcc;
    dev.delay_us = Delayus;
    dev.read_write_len = 32;
    dev.config_file_ptr = NULL; // default config file
    //init
    bmi270_init(&dev);
    // settings should be set here
}

void Refresh_Temperature(void) {
    bmi2_get_temperature_data(&temperature_data, &dev);
    temperature_value = (float)((((float)((int16_t)temperature_data)) / 512.0) + 23.0);
    dev.delay_us(10000,dev.intf_ptr); // Delay according to datasheet for temperature reading
}
