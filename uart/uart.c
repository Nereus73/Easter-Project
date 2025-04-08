#include <stdint.h>
#include <string.h>

#include "ti_msp_dl_config.h"

const uint8_t LINE_FEED = 0x0a;
const uint8_t CARRIAGE_RETURN = 0x0d;

void uart_transmit_blocking(const uint8_t* const data, const uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        DL_UART_Main_transmitDataBlocking(UART_0_INST, data[i]);
    }
}

void uart_write_blocking(const char* const data) {
    const uint32_t length = strlen(data);
    for (uint8_t i = 0; i < length; i++) {
        DL_UART_Main_transmitDataBlocking(UART_0_INST, data[i]);
        
    }

    DL_UART_Main_transmitDataBlocking(UART_0_INST, LINE_FEED);
}

uint32_t uart_receive_blocking(uint8_t* data) {
    uint32_t maxRead = sizeof(data);
    uint32_t counter = 0;
    char lastSymbol = 'a';
    while ((counter < maxRead) && (lastSymbol != LINE_FEED)) {
        data[counter] = DL_UART_Main_receiveDataBlocking(UART_0_INST);
        lastSymbol = data[counter];
        counter++;
    }
    return counter;
}

void debug_print(const char *const fmt, ...) {
    const uint32_t length = strlen(fmt);
    for (uint8_t i = 0; i < length; i++) {
        DL_UART_Main_transmitDataBlocking(UART_0_INST, fmt[i]);
        
    }
}