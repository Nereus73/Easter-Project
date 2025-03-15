#include <stdint.h>

#include "ti/driverlib/dl_i2c.h"
#include "ti_msp_dl_config.h"
#include "i2c.h"

#define I2C_TX_PACKET_SIZE (1)
#define I2C_RX_PACKET_SIZE (2)

uint8_t gTxPacket[I2C_TX_PACKET_SIZE] = { 0xff };

volatile uint8_t gRxPacket[I2C_RX_PACKET_SIZE];

#define I2C_TARGET_ADDRESS (0x50)

void i2c_transfer_blocking(uint32_t i2c_handle, I2C_Transaction* i2c_transaction) {  
    
    DL_I2C_fillControllerTXFIFO(I2C_1_INST, i2c_transaction->writeBuf, i2c_transaction->writeCount);

    /* Wait for I2C to be Idle */
    while (!(DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send the packet to the controller.
    * This function will send Start + Stop automatically.
    */
    DL_I2C_startControllerTransfer(I2C_1_INST, I2C_TARGET_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, I2C_TX_PACKET_SIZE);

    /* Poll until the Controller writes all bytes */
    while (DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    /* Trap if there was an error */
    if (DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_ERROR) {
        /* LED will remain high if there is an error */
        __BKPT(0);
    }

    /* Wait for I2C to be Idle */
    while (!(DL_I2C_getControllerStatus(I2C_1_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send a read request to Target */
    DL_I2C_startControllerTransfer(I2C_1_INST, I2C_TARGET_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_RX, I2C_RX_PACKET_SIZE);

    /*
    * Receive all bytes from target. LED will remain high if not all bytes
    * are received
    */
    for (uint8_t i = 0; i < I2C_RX_PACKET_SIZE; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_1_INST))
            ;
        gRxPacket[i] = DL_I2C_receiveControllerData(I2C_1_INST);
    }
}
