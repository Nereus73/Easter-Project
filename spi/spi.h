#pragma once

#include <stdint.h>

void transmitPacketBlocking(const uint8_t* const buffer, uint32_t size);
void SPI_Controller_transmitData(const uint8_t* const data, uint8_t dataLength);