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

<<<<<<< Updated upstream
#include "ti_msp_dl_config.h"
#include "spi/spi.h"
#include "seven-segment-display/seven-segment-display.h"
=======
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
#include "sensors/proximity.h"
>>>>>>> Stashed changes




// State machine
enum SysState{
    IDLE = 0,
    SHT41,
    OPT4001,
    LDC1314,
    IIS2MDC,
    GETSENSORVAL,
    SENDING,
    ANZEIGEN,
} gSysState;

// State machine kids
enum LockState{
    LOCK1 = 0,
    LOCK2,
    LOCK3,
    LOCK4,
} gLockState;

struct Solution {
    uint8_t counter;
    bool found;
};

struct Solution sol;

/* Delay for 5ms to ensure UART TX is idle before starting transmission */
#define UART_TX_DELAY (160000)


const uint32_t DELAY = 10000000;
<<<<<<< Updated upstream
=======
char uartData[128];


// Humidity and Temperature Sensor values
uint16_t SHT41_temperature_raw, SHT41_humidity_raw;
float SHT41_temperature_s, SHT41_humidity_s;

// light sensor values
uint32_t lux_raw;

// Inductive sensor values
uint16_t LDC1314_data_ch2;

// Mag sensor values
uint16_t magX, magY, magZ;

// debug values
uint16_t sysTimCounter = 0;
uint8_t anzeige =0;


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

void printLDC1314(void) {
    uint32_t posPointer = 0;
    strcpy(uartData, "Inductive_Raw: ");
    posPointer += 15;
    int ret = snprintf(&uartData[posPointer], 32, "%d", LDC1314_data_ch2);
    posPointer += ret;  
    uartData[posPointer++] = 0x0a;
    uart_transmit_blocking(uartData, posPointer);
}


void printIIS2MDC(void) {
    uint32_t posPointer = 0;
    strcpy(uartData, "Magnetic_Raw: ");
    posPointer += 14;
    int ret = snprintf(&uartData[posPointer], 32, "%d", magX);
    posPointer += ret;  
    uartData[posPointer++] = ',';
    ret = snprintf(&uartData[posPointer], 32, "%d", magY);
    posPointer += ret;
    uartData[posPointer++] = ',';
    ret = snprintf(&uartData[posPointer], 32, "%d", magZ);
    posPointer += ret;
    uartData[posPointer++] = 0x0a;
    uart_transmit_blocking(uartData, posPointer);
}

void adjustBrightness(void) {
    light_getData(&lux_raw);
    if (lux_raw > 10000) {
        setBrightness(lux_raw/10000);
    }
}

void GretaValidate(void) {
    if (gSysState == ANZEIGEN) {
        SevenSegmentUpdate(6, SEVENSEG_ON);
        return;
    }
    switch (gLockState) {
        case LOCK1: //Light sensor
            if (sol.found) {
                SevenSegmentUpdate(5, SEVENSEG_BLINK);
            } else {
                SevenSegmentUpdate(10, SEVENSEG_SWEEP);
            }  
            if (lux_raw < 15000) {
                sol.counter++;
            } else {
                sol.counter = 0;
            }
            if (sol.counter > 100) {
                sol.found = true;
            }
            printOPT4001();
            break;
        case LOCK2:
            SevenSegmentUpdate(10, SEVENSEG_SWEEP);
            break;
        case LOCK3:
            SevenSegmentUpdate(10, SEVENSEG_SWEEP);
            break;
        case LOCK4:
            SevenSegmentUpdate(10, SEVENSEG_SWEEP);
            break;
    }
}

>>>>>>> Stashed changes

int main(void)
{
    SYSCFG_DL_init();
<<<<<<< Updated upstream

    /* Optional delay to ensure UART TX is idle before starting transmission */
    delay_cycles(UART_TX_DELAY);

    while (1) {
        SevenSegmentUpdate('0');
        delay_cycles(DELAY);
        SevenSegmentUpdate('1');
        delay_cycles(DELAY);
        SevenSegmentUpdate('2');
        delay_cycles(DELAY);
        SevenSegmentUpdate('3');
        delay_cycles(DELAY);
        SevenSegmentUpdate('4');
        delay_cycles(DELAY);
        SevenSegmentUpdate('5');
        delay_cycles(DELAY);
        SevenSegmentUpdate('6');
        delay_cycles(DELAY);
        SevenSegmentUpdate('7');
        delay_cycles(DELAY);
        SevenSegmentUpdate('8');
        delay_cycles(DELAY);
        SevenSegmentUpdate('9');
        delay_cycles(DELAY);
    }
=======
    initSevenSegment();
    initSHT41();
    lightInit();
    inductiveInit();    // does not work if device ID is wrong
    NVIC_EnableIRQ(GPIO_INT_INT_IRQN);
    SevenSegmentUpdate(0, SEVENSEG_ON);
    /* Optional delay to ensure UART TX is idle before starting transmission */
    delay_cycles(UART_TX_DELAY);
    gSysState = IDLE;
    uart_write_blocking("Hello World!");
    gLockState = LOCK1;

    uint32_t counter = 0;
    uint8_t reg = 0;
    
    initBMI270();
    initIIS2MDC();

    // init kids variables
    sol.counter = 0;
    sol.found = false;

    DL_SYSTICK_config(320000); // Start Systick Timer 10ms
    

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
        //
        /*
        switch (gSysState) {
            case IDLE:
                SevenSegmentUpdate(0, true);
                break;
            case SHT41:
                SevenSegmentUpdate(1, true);
                getTempHum(&SHT41_temperature_raw, &SHT41_temperature_s, &SHT41_humidity_raw, &SHT41_humidity_s);
                printSHT41();
                break;
            case OPT4001:
                SevenSegmentUpdate(2, false);
                light_getData(&lux_raw);
                printOPT4001();
                break;
            case LDC1314:
                SevenSegmentUpdate(3, false);
                inductiveGetData(&LDC1314_data_ch2);
                printLDC1314();
                break;
            case IIS2MDC:
                SevenSegmentUpdate(4, false);
                getSensorDataMag(&magX, &magY, &magZ);
                printIIS2MDC();
                break;
        }*/
       // __WFI();
    }
    
>>>>>>> Stashed changes
}


void SysTick_Handler(void) {
    sysTimCounter++;
    if ((sysTimCounter % 10) == 0) {
        //adjustBrightness();
    }
    if (gSysState == ANZEIGEN) {
        if (sysTimCounter > 100) {
            gSysState = IDLE;
            sysTimCounter = 0;
        }
    } else {
        if (sysTimCounter > 15) {
            GretaValidate();
            sysTimCounter = 0;
        }
    }
}

void GROUP1_IRQHandler(void) {
    //switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
    switch (DL_GPIO_getPendingInterrupt(GPIO_INT_PORT)) {
        case GPIO_INT_BUT_POS_IIDX:
            if (gSysState == LDC1314) {
                gSysState = IDLE;
            } else {
                gSysState++;
            }
            if (gLockState == LOCK4) {
                gLockState = LOCK1;
            } else {
                gLockState++;
            }
            //SevenSegmentUpdate(gSysState, false);
            gSysState = ANZEIGEN;
            sysTimCounter = 0;
            SevenSegmentUpdate(gLockState + 1, SEVENSEG_ON);
            break;
        case GPIO_INT_BUT_ANZ_IIDX:
            gSysState = ANZEIGEN;
            sysTimCounter = 0;
            SevenSegmentUpdate(gLockState + 1, SEVENSEG_ON);
            break;
        case GPIO_INT_INT_CAP_IIDX:
            // Interrupt of the inductive sensor
            break;
    }
}


