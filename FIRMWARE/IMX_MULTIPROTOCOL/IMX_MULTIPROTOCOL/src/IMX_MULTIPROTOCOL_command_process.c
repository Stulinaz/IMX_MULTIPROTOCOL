/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/
#include "IMX_MULTIPROTOCOL_command_process.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"
#include <string.h>

const char ascii_char[]     = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const uint8_t ascii_value[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

/*general commands list*/
const char general_command[MAIN_COMMAND_DIM_SIZE] = "imx";
const char spi_command    [MAIN_COMMAND_DIM_SIZE] = "spi";
const char i2c_command    [MAIN_COMMAND_DIM_SIZE] = "i2c";
const char ser_command    [MAIN_COMMAND_DIM_SIZE] = "ser";

/*options for general command*/
const char serial_cmd_select[] = " -s";
const char i2c_cmd_select[]    = " -i";
const char spi_cmd_select[]    = " -p";
const char close[]             = " -a";
const char help[]              = " -h";
const char transfer_start[]    = " -x";

/*options for ser command*/
const char serial_bausel[]     = " -b";

/*options for i2c command*/
const char i2c_address[]       = " -a 0x";

const char i2c_transmitter[]   = " -mt";
const char i2c_receiver[]      = " -mr";

const char i2c_access_write[]  = " -w 0x";
const char i2c_access_read[]   = " -r";

const char i2c_repeat_start[]  = " -s";
const char i2c_queue_req[]     = " -q";
const char i2c_queue_delete[]  = " -d";

static uint8_t CharToHex(char msb, char lsb, uint8_t *const value);

command_t Decode(uint16_t cmd_len, uint8_t *const param)
{
	uint8_t main_command[MAIN_COMMAND_DIM_SIZE];
	uint8_t i;
	uint8_t opt_cmd_len;

	if(cmd_len < MIN_CMD_LEN)
		return UNKNOWN_COMMAND;

	opt_cmd_len = cmd_len - MAIN_COMMAND_DIM_SIZE;

	/*Get first MAIN_COMMAND_DIM_SIZE bytes from usb interface*/
	for(i=0; i < MAIN_COMMAND_DIM_SIZE; i++)
		getbyte(USB_INTERFACE,main_command + i);

	if(memcmp(main_command, general_command, MAIN_COMMAND_DIM_SIZE) == 0)
	{
		/* help */
		if(memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, help, opt_cmd_len) == 0)
			return HELP;

		/* serial interface selected */
		if(memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, serial_cmd_select, opt_cmd_len) == 0)
			return USER_SERIAL_INTERFACE_SELECTED;

		/* i2c interface selected */
		if( memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_cmd_select, opt_cmd_len) == 0)
			return USER_I2C_INTERFACE_SELECTED;

		/* spi interface selected */
		if( memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, spi_cmd_select, opt_cmd_len) == 0)
			return USER_SPI_INTERFACE_SELECTED;

		/* communication abort */
		if( memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, close, opt_cmd_len) == 0)
			return USER_COMMUNICATION_ABORT;

		/* start data transfer on selected interface */
		if( memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, transfer_start, opt_cmd_len) == 0)
			return USER_TRANSFER_REQUEST;
	}

	/*serial commands*/
	if(memcmp(main_command, ser_command, MAIN_COMMAND_DIM_SIZE) == 0)
	{
	    if( memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, serial_bausel, opt_cmd_len - 1) == 0)
		{
			if( !CharToHex('0', usb_rx_buff[cmd_len - 1], param) )
				return USER_SER_SET_BAUDRATE;
		}
	}

	/* i2c commands*/
	if(memcmp(main_command, i2c_command, MAIN_COMMAND_DIM_SIZE) == 0)
	{

	    if( memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_address, opt_cmd_len - 2) == 0)
		{
			if( !CharToHex(usb_rx_buff[cmd_len - 2], usb_rx_buff[cmd_len - 1], param) )
				return USER_I2C_SET_SLAVE_ADDRESS;
		}

	    if( memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_access_write, opt_cmd_len - 3) == 0)
		{
			if( !CharToHex(usb_rx_buff[cmd_len - 2], usb_rx_buff[cmd_len - 1], param) )
				return USER_I2C_WRITE_DATA;
		}

	    if(memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_transmitter, opt_cmd_len) == 0)
	    	return USER_IC2_ACCESS_MODE_TRANSMITTER;

	    if(memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_receiver, opt_cmd_len) == 0)
	    	return USER_IC2_ACCESS_MODE_RECEIVER;

	    if(memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_access_read, opt_cmd_len) == 0)
	    	return USER_I2C_READ_DATA;

	    if(memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_repeat_start, opt_cmd_len) == 0)
	    	return USER_I2C_REPEAT_START;

	    if(memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_queue_req, opt_cmd_len) == 0)
	    	return USER_I2C_REQUEST_QUEUE_DATA;

	    if(memcmp(usb_rx_buff + MAIN_COMMAND_DIM_SIZE, i2c_queue_delete, opt_cmd_len) == 0)
	    	return USER_I2C_REQUEST_QUEUE_DELETE;
	}
	return UNKNOWN_COMMAND;
}

static uint8_t CharToHex(char msb, char lsb, uint8_t *const value)
{
	uint8_t i;
	uint8_t stat = 2;
	(*value) = 0;
	for(i =0; i<16; i++)
	{
		if(msb == ascii_char[i])
		{
			stat--;
			(*value) += ascii_value[i] * 16;
		}
		if(lsb == ascii_char[i])
		{
			stat--;
			(*value) += ascii_value[i];
		}
	}
	if (stat)
		(*value) = 0;
	return stat;
}

uint16_t DecToChar(uint8_t value)
{
	uint8_t i;
	uint8_t msb_nibble = value / 16;
	uint8_t lsb_nibble = value % 16;
	uint8_t conversion_lsb;
	uint16_t conversion_msb;

	for(i =0; i<16; i++)
	{
		if(msb_nibble == ascii_value[i])
			conversion_msb = ((uint16_t)ascii_char[i]) & 0x00FF;
		if(lsb_nibble == ascii_value[i])
			conversion_lsb = ((uint16_t)ascii_char[i]) & 0x00FF;
	}
	conversion_msb = conversion_msb << 8;
	return (conversion_msb | (uint16_t)conversion_lsb);
}
