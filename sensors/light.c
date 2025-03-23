#include "light.h"
#include "i2c/i2c.h"
#include "sensors/OPT4001.h"




opt4_handle_t opt4001_handle;


static uint8_t opt_iic_init(void) {
    I2C_Delayus(10, NULL);  // Not necessary but driver need init function
    return 0;
}

static uint8_t opt_iic_deinit(void) {
    I2C_Delayus(10, NULL);  // Not necessary but driver need init function
    return 0;
}

void lightInit(void)
{
    opt4001_handle.iic_init = opt_iic_init;
    opt4001_handle.iic_deinit = opt_iic_deinit;
    opt4001_handle.iic_write_cmd = I2C_write;
    opt4001_handle.iic_read_cmd = I2C_read;
    opt4001_handle.debug_print = debug_print;
    opt4001_handle.delay_ms = I2C_Delayms; 
    opt4001_handle.iic_addr = 0x45;
    opt4_init(&opt4001_handle);                                                   
}

void light_getData(uint32_t *lux_raw) {
    uint8_t res = 0;
    res = setOneShot(&opt4001_handle);
    if (res != 0) {
        return;
    }
    I2C_Delayms(500);
    opt4_getData(&opt4001_handle, lux_raw);
}



