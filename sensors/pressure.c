#include "sensors/pressure.h"
#include "spi/spi.h"

#include "ti/driverlib/dl_spi.h"
#include "ti_msp_dl_config.h"
#include "uart/uart.h"


#define readByte 0x80
#define writeByte 0x7F
volatile uint8_t gRxData[16];
volatile uint8_t pressureSPIState = 0;



SPIInterface BMP390SPI_Interface;

void initSensor(void)
{
    BMP390SPI_Interface.chipSelect = DL_SPI_CHIP_SELECT_1;
    BMP390SPI_Interface.rxDataCount = 0;
    BMP390SPI_Interface.txDataCount = 0;
    // read default registers
    BMP390_RegRead(BMP390_CHIP_ID, 1);
    uart_transmit_blocking(BMP390SPI_Interface.rxData, BMP390SPI_Interface.rxDataCount);
}

void BMP390_RegWrite(uint8_t address, uint8_t data) {
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