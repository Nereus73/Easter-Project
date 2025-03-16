#pragma once

#include <stdint.h>
#include "ti/driverlib/dl_spi.h"


typedef struct  {
    uint8_t rxData[32];
    uint8_t rxDataCount;
    uint8_t txData[32];
    uint8_t txDataCount;
    DL_SPI_CHIP_SELECT chipSelect;
} SPIInterface;

void initSensor(void);
void BMP390_RegRead(uint8_t address, uint8_t count);
void BMP390_RegWrite(uint8_t address, uint8_t data);
uint8_t SPI_read(uint8_t reg_addr, uint8_t *read_data, uint32_t len, void *intf_ptr);
uint8_t SPI_write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
void RegisterWrite(SPIInterface* spiStruct);
void RegisterRead(SPIInterface* spiStruct);
void SPI_0_INST_IRQHandler(void);