/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#ifndef INC_IMX_MULTIPROTOCOL_BUFFERS_MANAGER_H_
#define INC_IMX_MULTIPROTOCOL_BUFFERS_MANAGER_H_

#include <stdint.h>
#include "IMX_MULTIPROTOCOL_definitions.h"

#define SERIAL_TX_BUFF_DIM 25
#define SERIAL_RX_BUFF_DIM 25
#define I2C_TX_BUFF_DIM    0
#define I2C_RX_BUFF_DIM    0
#define SPI_TX_BUFF_DIM    25
#define SPI_RX_BUFF_DIM    25
#define USB_TX_BUFF_DIM    512
#define USB_RX_BUFF_DIM    512

typedef struct{
uint32_t tx_buff_write_index;
uint32_t tx_buff_read_index;
uint32_t rx_buff_write_index;
uint32_t rx_buff_read_index;
uint16_t tx_max_buff_dim;
uint16_t rx_max_buff_dim;
}comm_index_t;

typedef enum{
SER_INTERFACE = 0u,
I2C_INTERFACE,
SPI_INTERFACE,
USB_INTERFACE
}comm_inerface_t;

extern volatile comm_index_t ser_comm_type;
extern uint8_t ser_tx_buff[];
extern uint8_t ser_rx_buff[];
extern comm_index_t spi_comm_type;
extern uint8_t spi_tx_buff[];
extern uint8_t spi_rx_buff[];
extern comm_index_t i2c_comm_type;
extern uint8_t i2c_tx_buff[];
extern uint8_t i2c_rx_buff[];
extern volatile comm_index_t usb_comm_type;
extern uint8_t usb_tx_buff[];
extern uint8_t usb_rx_buff[];

void putbyte(comm_inerface_t comm_type, uint8_t data);
uint8_t getbyte(comm_inerface_t comm_type, uint8_t * const data);
uint32_t data_avail(comm_inerface_t comm_type);
uint32_t data_toprocess(comm_inerface_t comm_type);
void clear_buff(comm_inerface_t comm_type);
void USB_set_rx_data_len(uint16_t len);
uint32_t USB_get_rx_data_len(void);
void UsbPrintString(const char *buff, _bool append_newline);
uint32_t SetBuffer(comm_inerface_t comm_type);
void SerialToUsb(void);

#endif /* INC_IMX_MULTIPROTOCOL_BUFFERS_MANAGER_H_ */
