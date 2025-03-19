#pragma once


#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/peripherals/hw_spi.h"
#include "ti/driverlib/dl_gpio.h"
#include <stdint.h>

typedef struct {
    SPI_Regs *spiInt;
    GPIO_Regs *gpioInt;
    uint32_t csMask;
    uint8_t DummyByte;
} SPIInterface;

uint8_t SPI_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
uint8_t SPI_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
void SPI_Delayms(uint32_t period);
void SPI_Delayus(uint32_t period, void *intf_ptr);