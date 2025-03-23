#include "sensors/B-Messer.h"


// variables for main BMI270 driver
static DL_SPI_CHIP_SELECT chipSelectAcc = DL_SPI_CHIP_SELECT_0;
struct bmi2_dev dev;

// variables for main Magnetometer driver
stmdev_ctx_t devMag;

SPIInterface spi;
    

// Variables for data
uint16_t temperature_data;
float temperature_value;
#define GRAVITY_EARTH  (9.80665f)
    /* Assign accel sensor to variable. */
    uint8_t sensor_list = BMI2_ACCEL;
    struct bmi2_sens_config config;
    struct bmi2_sens_data sens_data = { { 0 } };

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

int8_t set_accel_config(struct bmi2_dev *bmi)
{
    /* Status of api are returned to this variable. */
    int8_t rslt;

    /* Structure to define accelerometer configuration. */
    struct bmi2_sens_config config;

    /* Configure the type of feature. */
    config.type = BMI2_ACCEL;

    /* Get default configurations for the type of feature selected. */
    rslt = bmi2_get_sensor_config(&config, 1, bmi);

    if (rslt == BMI2_OK)
    {
        /* NOTE: The user can change the following configuration parameters according to their requirement. */
        /* Set Output Data Rate */
        config.cfg.acc.odr = BMI2_ACC_ODR_200HZ;

        /* Gravity range of the sensor (+/- 2G, 4G, 8G, 16G). */
        config.cfg.acc.range = BMI2_ACC_RANGE_2G;

        /* The bandwidth parameter is used to configure the number of sensor samples that are averaged
         * if it is set to 2, then 2^(bandwidth parameter) samples
         * are averaged, resulting in 4 averaged samples.
         * Note1 : For more information, refer the datasheet.
         * Note2 : A higher number of averaged samples will result in a lower noise level of the signal, but
         * this has an adverse effect on the power consumed.
         */
        config.cfg.acc.bwp = BMI2_ACC_NORMAL_AVG4;

        /* Enable the filter performance mode where averaging of samples
         * will be done based on above set bandwidth and ODR.
         * There are two modes
         *  0 -> Ultra low power mode
         *  1 -> High performance mode(Default)
         * For more info refer datasheet.
         */
        config.cfg.acc.filter_perf = BMI2_PERF_OPT_MODE;

        /* Set the accel configurations. */
        rslt = bmi2_set_sensor_config(&config, 1, bmi);

        /* Map data ready interrupt to interrupt pin. */
        rslt = bmi2_map_data_int(BMI2_DRDY_INT, BMI2_INT1, bmi);
    }

    return rslt;
}

void initIIS2MDC(void) {
    // Initialize all BMI values for IIS
    bmi2_aux_conf(&dev);
    // Initialize all IIS values
    devMag.mdelay = SPI_Delayms;
    devMag.read_reg = bmi2_aux_i2c_read;
    devMag.write_reg = bmi2_aux_i2c_write;
    devMag.handle = &dev;
    uint8_t deviceID = 0;
    iis2mdc_device_id_get(&devMag, &deviceID);
}

void initBMI270(void) {
    // SPI init
    spi.DummyByte = 0x00;
    spi.csMask = GPIO_INT_CS0_PIN; 
    spi.gpioInt = GPIO_INT_PORT;
    spi.spiInt = SPI_0_INST;
    // BMI2 Interface init
    dev.intf = BMI2_SPI_INTF;
    dev.read = SPI_read;
    dev.write = SPI_write;
    dev.intf_ptr = &spi;
    dev.delay_us = SPI_Delayus;
    dev.read_write_len = 20;
    dev.config_file_ptr = NULL; // default config file
    //init
    volatile int8_t statusByte = 0;
    statusByte = bmi270_init(&dev);
    // settings should be set here
    set_accel_config(&dev);
    bmi2_sensor_enable(&sensor_list, 1, &dev);
    config.type = BMI2_ACCEL;
    bmi2_get_sensor_config(&config, 1, &dev);
}

void Refresh_Temperature(void) {
    bmi2_get_temperature_data(&temperature_data, &dev);
    temperature_value = (float)((((float)((int16_t)temperature_data)) / 512.0) + 23.0);
    dev.delay_us(10000,dev.intf_ptr); // Delay according to datasheet for temperature reading
}

void getSensorData(uint16_t *accX, uint16_t *accY, uint16_t *accZ) {
    bmi2_get_sensor_data(&sens_data, &dev);
    *accX = sens_data.acc.x;
    *accY = sens_data.acc.y;
    *accZ = sens_data.acc.z;
}