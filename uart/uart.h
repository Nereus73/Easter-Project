#pragma once

void uart_transmit_blocking(const uint8_t* const data, const uint8_t dataLength);
void uart_write_blocking(const char* const data);
uint32_t uart_receive_blocking(uint8_t* data);
