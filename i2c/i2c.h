#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct I2C_Transaction {
	uint8_t    *writeBuf;
    size_t  writeCount;   
    uint8_t    *readBuf;     
    size_t  readCount;    
    unsigned char slaveAddress; 
} I2C_Transaction;

void i2c_transfer_blocking(uint32_t i2c_handle, I2C_Transaction* i2c_transaction);
