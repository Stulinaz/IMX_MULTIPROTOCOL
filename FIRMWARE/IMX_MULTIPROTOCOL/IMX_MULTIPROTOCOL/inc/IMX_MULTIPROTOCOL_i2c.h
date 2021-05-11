/*
 * IMX_MULTIPROTOCOL_i2c.h
 *
 *  Created on: 1 mag 2021
 *      Author: Enrico
 */

#ifndef IMX_MULTIPROTOCOL_I2C_H_
#define IMX_MULTIPROTOCOL_I2C_H_

#include "IMX_MULTIPROTOCOL_definitions.h"
#include "IMX_MULTIPROTOCOL_i2c.h"
#include "fsl_lpi2c.h"

#define I2C_QUEUE_COMMAND_DIM 20
#define MASTER_TRANSMITTER    0x00
#define MASTER_RECEIVER       0x01
#define I2C_OFFSET_TRANSF     0x00

typedef enum{
NO_COMMAND_SELECTED = 0u,
BYTE_WRITE,
BYTE_READ,
ADDRESS_TRANSFER,
ACK_EXPECTED,
NACK_EXPECTED,
}i2c_queue_t;


_bool I2cEnqeue( uint8_t data, i2c_queue_t param);
_bool I2cGetqueue( uint8_t *const data, i2c_queue_t *const param, uint8_t queue_pos);
uint8_t I2cTransfer(void);
void DBG_i2c(void);
void I2cSetSlaveAddress(uint8_t addr);
void I2cQueueRelease(void);

#endif /* IMX_MULTIPROTOCOL_I2C_H_ */
