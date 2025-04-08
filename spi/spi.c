
#include <stdint.h>

#include "ti/driverlib/dl_spi.h"
#include "ti_msp_dl_config.h"

<<<<<<< Updated upstream
void transmitPacketBlocking(const uint8_t* const buffer, const uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        /*
         * The transmitter will automatically check that the CTS signal is
         * asserted (low) before transmitting the next byte. If CTS is
         * de-asserted (high), the byte will not be transmitted out of the TX
         * FIFO. Therefore block if the TX FIFO is full before attempting
         * to fill it.
         */
        DL_UART_Main_transmitDataBlocking(UART_0_INST, buffer[i]);
=======



// Driver compatible SPI read
uint8_t SPI_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    if (reg_data == NULL) return 1;
    if (intf_ptr == NULL) return 1;
    SPIInterface *spiStruc = (SPIInterface*)intf_ptr;
    uint32_t readCounter = 0;
    uint32_t writeCounter = 0;
    while (DL_SPI_isBusy(spiStruc->spiInt));
    DL_GPIO_clearPins(spiStruc->gpioInt,spiStruc->csMask);
    while (!DL_SPI_isRXFIFOEmpty(spiStruc->spiInt)) {
        reg_data[0] = DL_SPI_receiveDataBlocking8(spiStruc->spiInt);
    }
    while (!DL_SPI_transmitDataCheck8(spiStruc->spiInt, reg_addr));
    writeCounter++;
    while (readCounter <= len) {
        if (writeCounter <= len) {
            if (DL_SPI_transmitDataCheck8(spiStruc->spiInt, spiStruc->DummyByte))  writeCounter++;
        }
        if (!DL_SPI_isRXFIFOEmpty(spiStruc->spiInt)) { 
            if (readCounter > 0 ) {
                reg_data[readCounter - 1] = DL_SPI_receiveDataBlocking8(spiStruc->spiInt);
            } else {
                reg_data[readCounter] = DL_SPI_receiveDataBlocking8(spiStruc->spiInt);
            }
            readCounter++;
        }
    }
    while (DL_SPI_isBusy(spiStruc->spiInt));
    DL_GPIO_setPins(spiStruc->gpioInt,spiStruc->csMask);
    SPI_Delayus(100, NULL);
    return 0;
}

// Driver compatible SPI write
uint8_t SPI_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    if ((reg_data == NULL) && (len > 0)) return 1;
    if (intf_ptr == NULL) return 1;
    SPIInterface *spiStruc = (SPIInterface*)intf_ptr;
    uint32_t readCounter = 0;
    uint32_t writeCounter = 0;
    uint8_t readTemp;
    while (DL_SPI_isBusy(spiStruc->spiInt));
    DL_GPIO_clearPins(spiStruc->gpioInt,spiStruc->csMask);
    while (!DL_SPI_isRXFIFOEmpty(spiStruc->spiInt)) {
        readTemp = DL_SPI_receiveDataBlocking8(spiStruc->spiInt);
    }
    while (!DL_SPI_transmitDataCheck8(spiStruc->spiInt, reg_addr));
    writeCounter++;
    while (readCounter <= len) {
        if (writeCounter <= len) {
            if (DL_SPI_transmitDataCheck8(spiStruc->spiInt, reg_data[writeCounter - 1]))  {
                writeCounter++;
            }
        }
        if (!DL_SPI_isRXFIFOEmpty(spiStruc->spiInt)) { 
            readTemp = DL_SPI_receiveDataBlocking8(spiStruc->spiInt);
            readCounter++;
        }
    }
    while (DL_SPI_isBusy(spiStruc->spiInt));
    DL_GPIO_setPins(spiStruc->gpioInt,spiStruc->csMask);
    return 0;
}

// Driver Magnetometer compatible Delay function
void SPI_Delayms(uint32_t period) {
    while (period--) {
        SPI_Delayus(1000,NULL);
>>>>>>> Stashed changes
    }
}

void SPI_Controller_transmitData(const uint8_t* const data, const uint8_t dataLength)
{
    DL_SPI_setChipSelect(SPI_0_INST, DL_SPI_CHIP_SELECT_2);
    for (uint32_t i = 0; i < dataLength; i++) {
        while (DL_SPI_isBusy(SPI_0_INST))
            ;
        DL_SPI_transmitData8(SPI_0_INST, data[i]);
    }
}