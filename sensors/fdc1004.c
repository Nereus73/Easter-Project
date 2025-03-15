#include "fdc1004.h"
#include "i2c/i2c.h"
#include <stdint.h>

uint16_t getDeviceID(void){
    uint8_t buffer[64] = {};
    uint8_t writeBuf[1] = { 0xff };
    uint8_t readBuf[2] = {};
    I2C_Transaction i2c_transaction = {};
    i2c_transaction.writeBuf = writeBuf;
    i2c_transaction.writeCount = 1;
    i2c_transaction.readBuf = readBuf;
    i2c_transaction.readCount = 2;
    i2c_transaction.slaveAddress = 0x50;

    i2c_transfer_blocking(1, &i2c_transaction);

    const uint16_t deviceID = i2c_transaction.readBuf[1] | (i2c_transaction.readBuf[0] << 8);

    return deviceID;
}