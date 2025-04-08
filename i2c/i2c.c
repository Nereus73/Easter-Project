#include "i2c/i2c.h"

static enum I2cControllerStatus {
    I2C_STATUS_IDLE = 0,
    I2C_STATUS_TX_STARTED,
    I2C_STATUS_TX_INPROGRESS,
    I2C_STATUS_TX_COMPLETE,
    I2C_STATUS_RX_STARTED,
    I2C_STATUS_RX_INPROGRESS,
    I2C_STATUS_RX_COMPLETE,
    I2C_STATUS_ERROR,
} gI2cControllerStatus;

static uint16_t gTxCount, gTxLen, gRxLen, gRxCount;
static uint8_t *gTxPacket, *gRxPacket;

//I2c write command
uint8_t I2C_write(uint8_t addr, uint8_t *buf, uint16_t len) {
    NVIC_EnableIRQ(I2C_1_INST_INT_IRQN);
    gTxLen = len;
    gTxPacket = buf;
    DL_SYSCTL_disableSleepOnExit();
    gI2cControllerStatus = I2C_STATUS_IDLE;
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_1_INST, buf, gTxLen);
     if (gTxCount < gTxLen) {
        DL_I2C_enableInterrupt(I2C_1_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    } else {
        DL_I2C_disableInterrupt(I2C_1_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    }
    /*
     * Send the packet to the controller.
     * This function will send Start + Stop automatically.
     */
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    while (!(DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_startControllerTransfer(I2C_1_INST, addr, DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen);
    while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) && (gI2cControllerStatus != I2C_STATUS_ERROR)) {
        __WFI();
    }
    while (DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ; //Wait  as long as Bus is busy
    /* Trap if there was an error */
    if (DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_ERROR) {
        return 1;
    }
    while (!(DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    return 0;
}

// I2C read command
uint8_t I2C_read(uint8_t addr, uint8_t *buf, uint16_t len) {
   /* Send a read request to Target */
    gRxLen = len;
    gRxCount = 0;
    gRxPacket = buf;
    gI2cControllerStatus = I2C_STATUS_RX_STARTED;
    DL_I2C_startControllerTransfer(I2C_1_INST, addr, DL_I2C_CONTROLLER_DIRECTION_RX, gRxLen);
    /* Wait for all bytes to be received in interrupt */
    while (gI2cControllerStatus != I2C_STATUS_RX_COMPLETE) {
        __WFI();
    }
    if (gRxCount != gRxLen) {
        return 1; // Error if not all Bytes were received
    }
    while (DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;
    if (DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_ERROR) {
        return 1;
    }
    return 0;
}

// i2c interrupt
void I2C_1_INST_IRQHandler(void) {
    switch (DL_I2C_getPendingInterrupt(I2C_1_INST)) {
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_1_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] = DL_I2C_receiveControllerData(I2C_1_INST);
                } else {
                    /* Ignore and remove from FIFO if the buffer is full */
                    DL_I2C_receiveControllerData(I2C_1_INST);
                }
            }
            gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(I2C_1_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
            /* Receive all bytes from target */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_1_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] = DL_I2C_receiveControllerData(I2C_1_INST);
                } else {
                    /* Ignore and remove from FIFO if the buffer is full */
                    DL_I2C_receiveControllerData(I2C_1_INST);
                }
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with next bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillControllerTXFIFO(I2C_1_INST, &gTxPacket[gTxCount], gTxLen - gTxCount);
            }
            break;
            /* Not used for this example */
        case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
        case DL_I2C_IIDX_CONTROLLER_NACK:
            if ((gI2cControllerStatus == I2C_STATUS_RX_STARTED) || (gI2cControllerStatus == I2C_STATUS_TX_STARTED)) {
                /* NACK interrupt if I2C Target is disconnected */
                gI2cControllerStatus = I2C_STATUS_ERROR;
            }
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_FULL:
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_EMPTY:
            break;
        case DL_I2C_IIDX_CONTROLLER_START:
            break;
        case DL_I2C_IIDX_CONTROLLER_STOP:
            break;
        case DL_I2C_IIDX_CONTROLLER_EVENT1_DMA_DONE:
        case DL_I2C_IIDX_CONTROLLER_EVENT2_DMA_DONE:
        default:
            break;
    }
}


void I2C_Delayms(uint32_t period) {
    while (period--) {
        SPI_Delayus(1000,NULL);
    }
}

void I2C_Delayus(uint32_t period, void *intf_ptr) {
    // assuming 32 MHz -> Factor 32 -> Shifting 5
    delay_cycles(period << 5);
}