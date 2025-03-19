#include "sensors/pressure.h"
#include "sensors/bmp3.h"
#include "spi/spi.h"

#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include "uart/uart.h"

// defines for test communication
#define readByte 0x80
#define writeByte 0x7F
volatile uint8_t gRxData[16];
volatile uint8_t pressureSPIState = 0;

// variables for main driver
static DL_SPI_CHIP_SELECT chipSelectPressure = DL_SPI_CHIP_SELECT_1;
struct bmp3_dev dev;


void initSensor(void) {
    // declatration  
    uint16_t settings_sel;
    struct bmp3_settings settings = { 0 };
    dev.read = SPI_read;
    //dev.write = SPI_write;
    dev.intf = BMP3_SPI_INTF;
    dev.intf_ptr = &chipSelectPressure;
    //init
    bmp3_init(&dev);
    // set settings
    settings.int_settings.drdy_en = BMP3_ENABLE;
    settings.press_en = BMP3_ENABLE;
    settings.temp_en = BMP3_ENABLE;
    settings.odr_filter.press_os = BMP3_OVERSAMPLING_2X;
    settings.odr_filter.temp_os = BMP3_OVERSAMPLING_2X;
    settings.odr_filter.odr = BMP3_ODR_100_HZ;
    settings.op_mode = BMP3_MODE_NORMAL;
    settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN | BMP3_SEL_PRESS_OS | BMP3_SEL_TEMP_OS | BMP3_SEL_ODR |
                   BMP3_SEL_DRDY_EN;
    bmp3_set_sensor_settings(settings_sel, &settings, &dev);
    bmp3_set_op_mode(&settings, &dev);
}