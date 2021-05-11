/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/
#include "IMX_MULTIPROTOCOL_buffers_manager.h"
#include "IMX_MULTIPROTOCOL_lpuart.h"
#include "IMX_MULTIPROTOCOL_usb.h"
#include "usb_device_cdc_acm.h"

uint8_t ser_tx_buff[SERIAL_TX_BUFF_DIM];
uint8_t ser_rx_buff[SERIAL_RX_BUFF_DIM];

uint8_t i2c_tx_buff[I2C_TX_BUFF_DIM];
uint8_t i2c_rx_buff[I2C_RX_BUFF_DIM];

uint8_t spi_tx_buff[SPI_TX_BUFF_DIM];
uint8_t spi_rx_buff[SPI_RX_BUFF_DIM];

uint8_t usb_tx_buff[USB_TX_BUFF_DIM];
uint8_t usb_rx_buff[USB_RX_BUFF_DIM];

static void interface_selection (comm_inerface_t type_t);

volatile comm_index_t ser_comm_type = { 0, 0, 0, 0, SERIAL_TX_BUFF_DIM, SERIAL_RX_BUFF_DIM };
comm_index_t spi_comm_type = { 0, 0, 0, 0, 0, 0};
comm_index_t i2c_comm_type = { 0, 0, 0, 0,  I2C_TX_BUFF_DIM, I2C_RX_BUFF_DIM};
volatile comm_index_t usb_comm_type = { 0, 0, 0, 0,  USB_TX_BUFF_DIM, USB_RX_BUFF_DIM};

volatile comm_index_t *idx_strct_ptr;
volatile uint8_t      *buff_tx_ptr;
volatile uint8_t      *buff_rx_ptr;

static void interface_selection (comm_inerface_t type_t)
{
	switch(type_t)
	{
		case SER_INTERFACE:
		{
			idx_strct_ptr = &ser_comm_type;
			buff_tx_ptr   = ser_tx_buff;
			buff_rx_ptr   = ser_rx_buff;
			break;
		}
		case I2C_INTERFACE:
		{
			idx_strct_ptr = &i2c_comm_type;
			buff_tx_ptr   = i2c_tx_buff;
			buff_rx_ptr   = i2c_rx_buff;
			break;
		}
		case SPI_INTERFACE:
		{
			idx_strct_ptr = &i2c_comm_type;
			buff_tx_ptr   = spi_tx_buff;
			buff_rx_ptr   = spi_rx_buff;
			break;
		}
		case USB_INTERFACE:
		{
			idx_strct_ptr = &usb_comm_type;
			buff_tx_ptr   = usb_tx_buff;
			buff_rx_ptr   = usb_rx_buff;
			break;
		}
		default:
			break;
	}
}

void putbyte(comm_inerface_t comm_type, uint8_t data)
{
	interface_selection (comm_type);
	if (idx_strct_ptr->tx_buff_write_index < (idx_strct_ptr->tx_max_buff_dim - 1))
	{
		*(buff_tx_ptr + idx_strct_ptr->tx_buff_write_index) = data;
		(idx_strct_ptr->tx_buff_write_index)++;
	}
}

uint8_t getbyte(comm_inerface_t comm_type, uint8_t * const data)
{
	uint8_t stat = FAIL;
	interface_selection (comm_type);
	if(idx_strct_ptr->rx_buff_write_index != 0)
	{
		/*buffer not empty*/
		stat = PASS;
		*data = *(buff_rx_ptr + idx_strct_ptr->rx_buff_read_index);
		idx_strct_ptr->rx_buff_read_index++;
		if (idx_strct_ptr->rx_buff_write_index == idx_strct_ptr->rx_buff_read_index)
		{
			if(comm_type != USB_INTERFACE)
			{
				idx_strct_ptr->rx_buff_write_index = 0;
				idx_strct_ptr->rx_buff_read_index  = 0;
			}
		}
	}
	return stat;
}

uint32_t data_avail(comm_inerface_t comm_type)
{
	interface_selection (comm_type);
	return idx_strct_ptr->rx_buff_write_index;
}

uint32_t data_toprocess(comm_inerface_t comm_type)
{
	interface_selection (comm_type);
	return  idx_strct_ptr->tx_buff_write_index;
}

void clear_buff(comm_inerface_t comm_type)
{
	interface_selection (comm_type);
	idx_strct_ptr->rx_buff_write_index = 0;
	idx_strct_ptr->rx_buff_read_index  = 0;
}

void USB_set_rx_data_len(uint16_t len)
{
	interface_selection (USB_INTERFACE);
	idx_strct_ptr->rx_buff_write_index = len;
}

uint32_t USB_get_rx_data_len(void)
{
	interface_selection (USB_INTERFACE);
	return idx_strct_ptr->rx_buff_write_index;
}

void UsbPrintString(const char *buff, _bool append_newline)
{
	if(usb_comm_type.tx_buff_write_index == 0)
	{
		putbyte(USB_INTERFACE, '[');
		putbyte(USB_INTERFACE, 'I');
		putbyte(USB_INTERFACE, 'M');
		putbyte(USB_INTERFACE, 'X');
		putbyte(USB_INTERFACE, ']');
		putbyte(USB_INTERFACE, ' ');
	}
	while(*buff != '\0')
	{
		putbyte(USB_INTERFACE, *buff);
		buff++;
	}
	if(append_newline == TRUE)
	{
		putbyte(USB_INTERFACE, CR_);
		USBTransmit(usb_comm_type.tx_buff_write_index);
		usb_comm_type.tx_buff_write_index = 0;
	}
}

void SerialTransfer(void)
{
	uint32_t data_len, i;
	uint8_t byte;
	data_len = data_avail(USB_INTERFACE);
	if(data_len)
	{
		for(i=0; i<data_len; i++)
		{
			byte = usb_rx_buff[i];
			putbyte(SER_INTERFACE,byte);
		}
		UsbPrintString("Hit", TRUE);
		SerStartTransmit();
		usb_comm_type.rx_buff_write_index = 0;
	}
}

void SerialToUsb(void)
{
	uint8_t byte;
	if( data_avail(SER_INTERFACE) )
	{
		while( getbyte(SER_INTERFACE, &byte) )
			putbyte(USB_INTERFACE, byte);
		USBTransmit(usb_comm_type.tx_buff_write_index);
	}
}
