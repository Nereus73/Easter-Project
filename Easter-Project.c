/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_spi.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti_msp_dl_config.h"
#include "spi/spi.h"
#include "sensors/pressure.h"
#include "seven-segment-display/seven-segment-display.h"
#include "uart/uart.h"
#include "sensors/B-Messer.h"

/* Number of bytes for UART packet size */
#define UART_PACKET_SIZE (26)

/* Delay for 5ms to ensure UART TX is idle before starting transmission */
#define UART_TX_DELAY (160000)

/* Data packets to transmit over UART */
uint8_t gTxPacket1[UART_PACKET_SIZE] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'x', 'y', 'z'};
uint8_t gTxPacket2[UART_PACKET_SIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z'};

#define SPI_PACKET_SIZE (4)

/* Data for SPI to transmit */
uint8_t gTxPacket[SPI_PACKET_SIZE] = {'M', 'S', 'P', '!'};
volatile uint8_t gRxPacket[SPI_PACKET_SIZE];
const uint32_t DELAY = 10000000;
uint8_t uartData[32];

int main(void)
{
    SYSCFG_DL_init();
    initSevenSegment();
    SevenSegmentUpdate(0);
    /* Optional delay to ensure UART TX is idle before starting transmission */
    delay_cycles(UART_TX_DELAY);

    uart_write_blocking("Hello World!");
    

    uint32_t counter = 0;
    uint8_t reg = 0;
    SPIInterface spi;
    spi.DummyByte = 0x00;
    spi.csMask = GPIO_INT_CS0_PIN; 
    spi.gpioInt = GPIO_INT_PORT;
    spi.spiInt = SPI_0_INST;
    initBMI270(&spi);

    while (1) {      
        //counter = uart_receive_blocking(uartData);
        reg = uartData[0];

        if (counter == 3) { // write
           // SPI_write(uartData[0], &uartData[1], 1, &spi);
        }  
        if (counter == 2) { // read
           // SPI_read(reg, uartData, 5, &spi);                 
           // uartData[5] = 0x0a;
            //counter = 6;
        }     
        uart_transmit_blocking(uartData, counter);

        //uart_write_blocking("S");
        SPI_Delayms(10);
        }
}
