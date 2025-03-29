#include "sensors/proximity.h"
#include "i2c/i2c.h"
#include "sensors/LDC1314.h"
#include "sensors/SHT41.h"


LDC1314_handle_t dev;

void inductiveInit(void) {
    dev.iic_init = iic_init;
    dev.iic_deinit = iic_deinit;
    dev.debug_print = debug_print;
    dev.delay_ms = I2C_Delayms;
    dev.iic_addr = 0x2A;
    dev.iic_read_cmd = I2C_read;
    dev.iic_write_cmd = I2C_write;
    uint16_t devID = 0;
    uint8_t rslt = 0;
    LDC1314_getDevID(&dev, &devID);
    if ((devID != 0x3054) || (rslt != 0)) {
        return;
    }
    // coil parameter on PCB:
    // L = 22.394 µH
    // C = 1 nF
    // Q = 24.825
    // Rac = 6.016 Ohm
    // Rp = 3682.14 Ohm
    LDC1314_enableSleepMode(&dev, 1);       // must be in sleep to change config data
    LDC1314_enableAutoscan(&dev, 0);        // Conversion only for 1 channel
    LDC1314_selectActiveChannel(&dev, 2);   // select Channel 2 as active channel
    LDC1314_enableRpOverride(&dev, 1);      // Manual Rp/ Current setting
    LDC1314_setActivationMode(&dev, 1);     // Low power activation mode, only possible due to long settling time
    LDC1314_disableAutoAmplitude(&dev, 1);  // Disable amplitude autocorrection
    LDC1314_setReferenceCLK(&dev, 4, 2);    // 10.85 Mhz reference clock (43.4/4)
    LDC1314_setInputCLK(&dev, 1, 2);        // 1.061 MHz input clock (design dependend)
    LDC1314_setSettlingTime(&dev, 678, 2);   // calculated 15.86, higher value for more tolerance -> 1ms settling time
    LDC1314_setConversionTime(&dev, 678, 2);  // 1ms conversion time, overall update rate -> 2 ms
    LDC1314_setDriveCurrent(&dev, 20, 2);   // dependend on Rp, 20 -> 297µA
    LDC1314_setDeglitch(&dev, 4);           // Filter for 3.3 MHz
    LDC1314_enableSleepMode(&dev, 0);       // exit sleep mode to start conversions
}

uint8_t inductiveGetData(uint16_t* data) {
    uint8_t rslt = 0;
    rslt = LDC1314_getSensorData(&dev, data, 2);
    return rslt;
}

uint8_t inductiveGetManID(uint16_t* manID) {
    uint8_t rslt = 0;
    rslt = LDC1314_getManID(&dev, manID);
    return rslt;
}

static uint8_t iic_init(void) {
    I2C_Delayus(10, NULL);  // Not necessary but driver need init function
    return 0;
}

static uint8_t iic_deinit(void) {
    I2C_Delayus(10, NULL);  // Not necessary but driver need init function
    return 0;
}