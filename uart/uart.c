#include <stdint.h>
#include <string.h>

#include "ti/driverlib/dl_uart_main.h"
#include "ti/driverlib/dl_uart_extend.h"
#include "ti_msp_dl_config.h"

void uart_transmit_blocking(const uint8_t* const data, const uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        DL_UART_transmitDataBlocking(UART_0_INST, data[i]);
    }
}

void uart_write_blocking(const char* const data) {
    const uint32_t length = strlen(data);

    for (uint8_t i = 0; i < length; i++) {
        DL_UART_transmitDataBlocking(UART_0_INST, data[i]);
    }

    DL_UART_transmitDataBlocking(UART_0_INST, 0x0a);
}