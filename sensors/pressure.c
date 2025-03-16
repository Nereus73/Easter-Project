#include "sensors/pressure.h"
#include "sensors/bmp3.h"
#include "spi/spi.h"

#include "ti/driverlib/dl_spi.h"
#include "ti_msp_dl_config.h"
#include "uart/uart.h"

// defines for test communication
#define readByte 0x80
#define writeByte 0x7F
volatile uint8_t gRxData[16];
volatile uint8_t pressureSPIState = 0;
SPIInterface BMP390SPI_Interface;

// variables for main driver
static DL_SPI_CHIP_SELECT chipSelectPressure = DL_SPI_CHIP_SELECT_1;
struct bmp3_dev dev;


void initSensorDebug(void) {
    // Test alt
    BMP390SPI_Interface.chipSelect = DL_SPI_CHIP_SELECT_1;
    BMP390SPI_Interface.rxDataCount = 0;
    BMP390SPI_Interface.txDataCount = 0;
    // read default registers
    BMP390_RegRead(BMP390_CHIP_ID, 1);
    uart_transmit_blocking(BMP390SPI_Interface.rxData, BMP390SPI_Interface.rxDataCount);
}

void initSensor(void) {
    // declatration  
    uint16_t settings_sel;
    struct bmp3_settings settings = { 0 };
    dev.read = SPI_read;
    dev.write = SPI_write;
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

// Driver compatible SPI read
uint8_t SPI_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    DL_SPI_CHIP_SELECT chipSelect = *(DL_SPI_CHIP_SELECT*)intf_ptr;
    // (void)intf_ptr; notwendig?
    uint32_t readCounter = 0;
    uint32_t writeCounter = 0;
    DL_SPI_setChipSelect(SPI_0_INST, chipSelect);
    while (readCounter <= len) {
        if (writeCounter <= len) {
            if (writeCounter == 0) {
                if (DL_SPI_transmitDataCheck8(SPI_0_INST, reg_addr)) writeCounter++;
            } else {
                if (DL_SPI_transmitDataCheck8(SPI_0_INST, 0)) writeCounter++;
            } 
        }        
        if (!DL_SPI_isRXFIFOEmpty(SPI_0_INST)) {
            reg_data[readCounter] = DL_SPI_receiveDataBlocking8(SPI_0_INST);
            readCounter++;
        }
    }
    return 0;
}

//Driver compatible SPI write
uint8_t SPI_write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    DL_SPI_CHIP_SELECT chipSelect = *(DL_SPI_CHIP_SELECT*)intf_ptr;
    // (void)intf_ptr; notwendig?
    uint32_t readCounter = 0;
    DL_SPI_setChipSelect(SPI_0_INST, chipSelect);
    while (writeCounter <= len) {
        if (writeCounter == 0) {
            if (DL_SPI_transmitDataCheck8(SPI_0_INST, reg_addr)) writeCounter++;
        } else {
            if (DL_SPI_transmitDataCheck8(SPI_0_INST, reg_data[writeCounter-1])) writeCounter++;
        }
    }  
    return 0;
}

// Debug functions
void BMP390_RegWrite(uint8_t reg_addr, uint8_t *read_data, uint32_t len, void *intf_ptr) {
    BMP390SPI_Interface.rxDataCount = 0;
    BMP390SPI_Interface.txData[0] = address & writeByte;
    BMP390SPI_Interface.txData[1] = data;
    BMP390SPI_Interface.txDataCount = 2;
    RegisterWrite(&BMP390SPI_Interface);
}

void BMP390_RegRead(uint8_t address, uint8_t count) {
    BMP390SPI_Interface.rxDataCount = 0;
    BMP390SPI_Interface.txData[0] = address | readByte;
    BMP390SPI_Interface.txDataCount = count + 2;
    RegisterRead(&BMP390SPI_Interface);
}

void RegisterWrite(SPIInterface* spiStruct) {
    DL_SPI_setChipSelect(SPI_0_INST, spiStruct->chipSelect);
    for (uint32_t i = 0; i < spiStruct->txDataCount; i++) {
        while (DL_SPI_isBusy(SPI_0_INST))
            ;
        DL_SPI_transmitData8(SPI_0_INST, spiStruct->txData[i]);
    }
}

void RegisterRead(SPIInterface* spiStruct) {
    DL_SPI_setChipSelect(SPI_0_INST, spiStruct->chipSelect);
    for (uint32_t i = 0; i < spiStruct->txDataCount; i++) {
        while (DL_SPI_isBusy(SPI_0_INST));
        DL_SPI_transmitData8(SPI_0_INST, spiStruct->txData[i]);
    }
    spiStruct->txDataCount = 0;
    while (!DL_SPI_isRXFIFOEmpty(SPI_0_INST)) {
        spiStruct->rxData[spiStruct->rxDataCount] = DL_SPI_receiveData8(SPI_0_INST);
        spiStruct->rxDataCount++;
    }
}