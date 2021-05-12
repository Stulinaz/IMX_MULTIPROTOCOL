/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#ifndef INC_IMX_MULTIPROTOCOL_COMMAND_PROCESS_H_
#define INC_IMX_MULTIPROTOCOL_COMMAND_PROCESS_H_

#include <stdint.h>

#define MIN_CMD_LEN           0x05
#define MAIN_COMMAND_DIM_SIZE 0x03

typedef enum{
NO_COMMAND = 0u,
UNKNOWN_COMMAND ,
USER_TRANSFER_REQUEST,
USER_SERIAL_INTERFACE_SELECTED,
USER_I2C_INTERFACE_SELECTED,
USER_SPI_INTERFACE_SELECTED,
USER_COMMUNICATION_ABORT,

USER_SER_SET_BAUDRATE,

USER_I2C_SET_SLAVE_ADDRESS,
USER_IC2_ACCESS_MODE_TRANSMITTER,
USER_IC2_ACCESS_MODE_RECEIVER,
USER_I2C_WRITE_DATA,
USER_I2C_READ_DATA,
USER_I2C_REPEAT_START,
USER_I2C_SEND_ADDRESS_READ,
USER_I2C_REQUEST_QUEUE_DATA,
USER_I2C_REQUEST_QUEUE_DELETE,

HELP
}command_t;

extern const char general_command[];
extern const char spi_command[];
extern const char i2c_command[];
extern const char ser_command[];
extern const char serial_cmd_select[];
extern const char i2c_cmd_select[];
extern const char spi_cmd_select[];
extern const char close[];
extern const char help[];
extern const char serial_bausel[];
extern const char i2c_address[];
extern const char i2c_transmitter[];
extern const char i2c_receiver[];
extern const char i2c_access_write[];
extern const char i2c_access_read[];
extern const char i2c_repeat_start[];
extern const char i2c_queue_req[];
extern const char i2c_queue_delete[];

command_t Decode  (uint16_t cmd_len, uint8_t *const param);
uint16_t DecToChar(uint8_t value);
#endif /* INC_IMX_MULTIPROTOCOL_COMMAND_PROCESS_H_ */
