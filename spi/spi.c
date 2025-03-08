#include <stdint.h>

#include "ti/driverlib/dl_spi.h"
#include "ti_msp_dl_config.h"

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