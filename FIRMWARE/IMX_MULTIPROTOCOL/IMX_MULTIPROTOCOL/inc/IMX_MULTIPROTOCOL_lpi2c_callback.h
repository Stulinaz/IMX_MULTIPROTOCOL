/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#ifndef IMX_MULTIPROTOCOL_LPI2C_CALLBACK_H_
#define IMX_MULTIPROTOCOL_LPI2C_CALLBACK_H_

#include "fsl_lpi2c.h"
#include "fsl_iomuxc.h"

#define II2C_BAUDRATE     100000U
#define II2C_MASTER_BASE  (LPI2C1_BASE)
#define II2C_MASTER       ((LPI2C_Type *)II2C_MASTER_BASE)
#define IIC_TIMEOUT 10

#define II2C_MASTER_CLOCK_FREQUENCY II2C_CLOCK_FREQUENCY
#define II2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (II2C_CLOCK_SOURCE_DIVIDER + 1U))
#define II2C_CLOCK_SOURCE_DIVIDER (5U)

/*MTDR register CMD options*/
#define IIC_CMD_TRANSMIT              LPI2C_MTDR_CMD (0x00)
#define IIC_CMD_RECEIVE               LPI2C_MTDR_CMD (0x01)
#define IIC_CMD_STOP                  LPI2C_MTDR_CMD (0x02)
#define IIC_CMD_RECEIVE_DISCARD       LPI2C_MTDR_CMD (0x03)
#define IIC_CMD_STARTnADDRESS_ACK     LPI2C_MTDR_CMD (0x04)
#define IIC_CMD_STARTnADDRESS_NACK    LPI2C_MTDR_CMD (0x05)

typedef enum{
CMD_TRANSMIT = 0,
CMD_RECEIVE,
CMD_STOP,
CMD_RECEIVE_DISCARD,
CMD_STARTnADDRESS_ACK,
CMD_STARTnADDRESS_NACK
}iic_mtdr_cmd_t;

typedef enum{
NO_ERROR = 0,
I2C_NOT_ACTIVATED,
I2C_BUS_BUSY,
I2C_TIMEOUT,
I2C_GENERAL_ERROR,
I2C_ADDRESS_NACK
}i2c_error_t;

void I2C1_Init(void);
void I2cInitPins(void);
void Call_I2cCheckActivation (i2c_error_t *const status_transfer);
void Call_I2cTransferData(i2c_error_t *const status_transfer, iic_mtdr_cmd_t cmd, uint8_t *const byte,uint8_t dir);
uint32_t GetTick(void);

#endif /* IMX_MULTIPROTOCOL_LPI2C_CALLBACK_H_ */
