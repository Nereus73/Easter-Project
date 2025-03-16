#include "fdc1004.h"
#include "i2c/i2c.h"
#include <stdint.h>

#define SLAVE_ADDRESS (0x50)

uint16_t getDeviceID(void) {
    uint8_t writeBuf[1] = { 0xff };
    uint8_t readBuf[2] = {};
    I2C_Transaction i2c_transaction = {};
    i2c_transaction.writeBuf = writeBuf;
    i2c_transaction.writeCount = 1;
    i2c_transaction.readBuf = readBuf;
    i2c_transaction.readCount = 2;
    i2c_transaction.slaveAddress = SLAVE_ADDRESS;

    i2c_transfer_blocking(1, &i2c_transaction);

    const uint16_t deviceID = i2c_transaction.readBuf[1] | (i2c_transaction.readBuf[0] << 8);

    return deviceID;
}

void initFDC1004(void) {
    uint8_t writeBuf[3] = { 0x0c, 0x5, 0xf0 };
    uint8_t readBuf[2] = {};
    I2C_Transaction i2c_transaction = {};
    i2c_transaction.writeBuf = writeBuf;
    i2c_transaction.writeCount = 3;
    i2c_transaction.readBuf = readBuf;
    i2c_transaction.readCount = 0;
    i2c_transaction.slaveAddress = SLAVE_ADDRESS;

    i2c_transfer_blocking(1, &i2c_transaction);
}

uint16_t getMeasurement(uint8_t channel) {
    const uint8_t registerNumber = (channel - 1) * 2;
    uint8_t writeBuf[1] = { registerNumber };
    uint8_t readBuf[2] = {};
    I2C_Transaction i2c_transaction = {};
    i2c_transaction.writeBuf = writeBuf;
    i2c_transaction.writeCount = 1;
    i2c_transaction.readBuf = readBuf;
    i2c_transaction.readCount = 2;
    i2c_transaction.slaveAddress = SLAVE_ADDRESS;

    i2c_transfer_blocking(1, &i2c_transaction);

    const uint16_t measurement = i2c_transaction.readBuf[1] | (i2c_transaction.readBuf[0] << 8);

    return measurement;
}