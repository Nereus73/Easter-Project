#pragma once

#include <stdint.h>
#include "ti/driverlib/dl_spi.h"

// Register Map BMP390
#define BMP390_CHIP_ID 0x00
#define BMP390_REV_ID 0x01
#define BMP390_ERR_REG 0x02
#define BMP390_STATUS 0x03
#define BMP390_DATA 0x04
#define BMP390_SENSORTIME 0x0C
#define BMP390_EVENT 0x10
#define BMP390_INT_STATUS 0x11
#define BMP390_FIFO_LENGTH 0x12
#define BMP390_FIFO_DATA 0x14
#define BMP390_FIFO_WTM 0x15
#define BMP390_FIFO_CONFIG 0x17
#define BMP390_INT_CTRL 0x19
#define BMP390_IF_CONF 0x1A
#define BMP390_PWR_CTRL 0x1B
#define BMP390_OSR 0x1C
#define BMP390_ODR 0x1D
#define BMP390_CONFIG 0x1F
#define BMP390_CMD 0x7E

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
void RegisterWrite(SPIInterface* spiStruct);
void RegisterRead(SPIInterface* spiStruct);
void SPI_0_INST_IRQHandler(void);