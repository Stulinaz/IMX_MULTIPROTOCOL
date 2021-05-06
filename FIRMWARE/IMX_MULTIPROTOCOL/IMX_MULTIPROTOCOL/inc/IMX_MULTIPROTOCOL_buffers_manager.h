/*
 * IMX_MULTIPROTOCOL_buffers_manager.h
 *
 *  Created on: 6 mag 2021
 *      Author: Enrico
 */

#ifndef INC_IMX_MULTIPROTOCOL_BUFFERS_MANAGER_H_
#define INC_IMX_MULTIPROTOCOL_BUFFERS_MANAGER_H_

#include <stdint.h>

#define SERIAL_TX_BUFF_DIM 100
#define SERIAL_RX_BUFF_DIM 100
#define I2C_TX_BUFF_DIM    100
#define I2C_RX_BUFF_DIM    100
#define SPI_TX_BUFF_DIM    20
#define SPI_RX_BUFF_DIM    20
#define USB_TX_BUFF_DIM    100
#define USB_RX_BUFF_DIM    100

typedef struct{
uint16_t tx_buff_write_index;
uint16_t tx_buff_read_index;
uint16_t rx_buff_write_index;
uint16_t rx_buff_read_index;
uint16_t tx_max_buff_dim;
uint16_t rx_max_buff_dim;
}comm_index_t;

typedef enum{
SER_INTERFACE = 0u,
I2C_INTERFACE,
SPI_INTERFACE,
USB_INTERFACE
}comm_inerface_t;

extern comm_index_t ser_comm_type;
extern uint8_t ser_tx_buff[];
extern uint8_t ser_rx_buff[];
extern comm_index_t i2c_comm_type;
extern uint8_t i2c_tx_buff[];
extern uint8_t i2c_rx_buff[];
extern comm_index_t usb_comm_type;
extern uint8_t usb_tx_buff[];
extern uint8_t usb_rx_buff[];

void putbyte(comm_inerface_t comm_type, uint8_t data);
uint8_t getbyte(comm_inerface_t comm_type, uint8_t * const data);
uint16_t data_avail(comm_inerface_t comm_type);
uint16_t data_toprocess(comm_inerface_t comm_type);
void clear_buff(comm_inerface_t comm_type);

#endif /* INC_IMX_MULTIPROTOCOL_BUFFERS_MANAGER_H_ */
