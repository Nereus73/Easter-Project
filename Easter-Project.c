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

#include "ti/devices/msp/m0p/mspm0l130x.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_spi.h"
#include "ti/driverlib/dl_timerg.h"
#include "ti_msp_dl_config.h"
#include "spi/spi.h"
#include "sensors/pressure.h"
#include "sensors/driver_sht4x.h"
#include "seven-segment-display/seven-segment-display.h"
#include "uart/uart.h"
#include "sensors/B-Messer.h"
#include "sensors/SHT41.h"
#include "sensors/light.h"




// State machine
enum SysState{
    IDLE = 0,
    SHT41,
    OPT4001,
} gSysState;

/* Delay for 5ms to ensure UART TX is idle before starting transmission */
#define UART_TX_DELAY (160000)


const uint32_t DELAY = 10000000;
char uartData[128];


// Humidity and Temperature Sensor values
uint16_t SHT41_temperature_raw, SHT41_humidity_raw;
float SHT41_temperature_s, SHT41_humidity_s;

// light sensor values
uint32_t lux_raw;


void printSHT41(void) {
    uint32_t posPointer = 0;
    strcpy(uartData, "Temp: ");
    posPointer += 6;
    int ret = snprintf(&uartData[posPointer], 32, "%f", SHT41_temperature_s);
    posPointer += ret;  
    strcpy(&uartData[posPointer], "°C , Hum: ");
    posPointer += 10;
    ret = snprintf(&uartData[posPointer], 32, "%f", SHT41_humidity_s);
    posPointer += ret; 
    uartData[posPointer++] = 0x25;
    uartData[posPointer++] = 0x0a;
    uart_transmit_blocking(uartData, posPointer);
}

void printOPT4001(void) {
    uint32_t posPointer = 0;
    strcpy(uartData, "Lux_Raw: ");
    posPointer += 9;
    int ret = snprintf(&uartData[posPointer], 32, "%d", lux_raw);
    posPointer += ret;  
    uartData[posPointer++] = 0x0a;
    uart_transmit_blocking(uartData, posPointer);
}

int main(void)
{
    SYSCFG_DL_init();
    initSevenSegment();
    initSHT41();
    lightInit();
    NVIC_EnableIRQ(GPIO_INT_INT_IRQN);
    SevenSegmentUpdate(0);
    /* Optional delay to ensure UART TX is idle before starting transmission */
    delay_cycles(UART_TX_DELAY);
    gSysState = OPT4001;
    uart_write_blocking("Hello World!");
    

    uint32_t counter = 0;
    uint8_t reg = 0;
    
    //initBMI270(&spi);

    
    

    while (1) {  
        // Debug I2C Interface
        /*  
        counter = uart_receive_blocking(uartData);
        reg = uartData[0];
        if (counter > 2) { // write
            uartData[0] = I2C_write(uartData[0], &uartData[1], counter - 2);
            uartData[1]= 0x0a;
            counter = 2;
        }  else {
            if (counter == 2) { // read
                I2C_read(uartData[0], &uartData[1], i2c_readLength);
                uartData[7]= 0x0a;
                counter = 8;
            } 
        }  
        uart_transmit_blocking(uartData, counter);
         */

        switch (gSysState) {
            case IDLE:
                SevenSegmentUpdate(0);
                break;
            case SHT41:
                SevenSegmentUpdate(1);
                getTempHum(&SHT41_temperature_raw, &SHT41_temperature_s, &SHT41_humidity_raw, &SHT41_humidity_s);
                printSHT41();
                break;
            case OPT4001:
                SevenSegmentUpdate(2);
                light_getData(&lux_raw);
                printOPT4001();
                break;
        }
        SPI_Delayms(1000);
    }
    
}


void GROUP1_IRQHandler(void) {
    //switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
    switch (DL_GPIO_getPendingInterrupt(GPIO_INT_PORT)) {
        case GPIO_INT_BUT_POS_IIDX:
            if (gSysState == OPT4001) {
                gSysState = IDLE;
            } else {
                gSysState++;
            }
            break;
        case GPIO_INT_BUT_ANZ_IIDX:
            SevenSegmentUpdate(12);
            break;
    }
}


