/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/
#include "IMX_MULTIPROTOCOL_appdata.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"
#include "IMX_MULTIPROTOCOL_i2c.h"
#include "IMX_MULTIPROTOCOL_i2c.h"
#include "IMX_MULTIPROTOCOL_lpuart.h"
#include "IMX_MULTIPROTOCOL_ledmanager.h"

command_t communication_mode = NO_COMMAND;

static void AppToTx(command_t mode);
static void I2cScanQueue(void);

void Application (command_t * user_cmd, uint8_t * const user_data)
{
	//TODO se seriale o spi abilitata dovrebbe mandare avanti tutti i messaggi
	switch(*user_cmd)
	{
		case NO_COMMAND:
			break;

		case USER_SERIAL_INTERFACE_SELECTED:
		if(communication_mode == NO_COMMAND)
		{
			LedInterfaceSel(USER_SERIAL_INTERFACE_SELECTED);
			LpuartInit();
			communication_mode = USER_SERIAL_INTERFACE_SELECTED;
			UsbPrintString("Serial interface selected", TRUE);
		}
		break;

		case USER_I2C_INTERFACE_SELECTED:
		LedInterfaceSel(USER_I2C_INTERFACE_SELECTED);
		communication_mode = USER_I2C_INTERFACE_SELECTED;
		UsbPrintString("I2C interface selected", TRUE);
		break;

		case USER_SPI_INTERFACE_SELECTED:
		LedInterfaceSel(USER_SPI_INTERFACE_SELECTED);
		communication_mode = USER_SPI_INTERFACE_SELECTED;
		UsbPrintString("SPI interface selected", TRUE);
		break;

		case HELP:
		break;

		case USER_SER_SET_BAUDRATE:
		if(SerBaudrateSel(*user_data))
			UsbPrintString("Serial baudrate set", TRUE);
		else
			UsbPrintString("Wrong parameter", TRUE);
		break;

		case USER_I2C_SET_SLAVE_ADDRESS:
		I2cSetSlaveAddress( (*user_data ) );
		UsbPrintString("Slave address set", TRUE);
		break;

		case USER_IC2_ACCESS_MODE_TRANSMITTER:
			if (I2cEnqeue(MASTER_TRANSMITTER , ADDRESS_TRANSFER))
				UsbPrintString("Sending address in write mode", TRUE);
			else
				UsbPrintString("Queue full", TRUE);
		break;

		case USER_IC2_ACCESS_MODE_RECEIVER:
			if (I2cEnqeue(MASTER_RECEIVER , ADDRESS_TRANSFER))
				UsbPrintString("Sending address in read mode", TRUE);
			else
				UsbPrintString("Queue full", TRUE);
		break;

		case USER_I2C_WRITE_DATA:
		if (I2cEnqeue( (*user_data), BYTE_WRITE) )
			UsbPrintString("Byte transfer enqueued", TRUE);
		else
			UsbPrintString("Queue full", TRUE);
		break;

		case USER_I2C_READ_DATA:
		if (I2cEnqeue( NUL, BYTE_READ) )
			UsbPrintString("Byte read enqueued", TRUE);
		else
			UsbPrintString("Queue full", TRUE);
		break;

		case USER_I2C_REPEAT_START:
/*		if(I2cEnqeue( NUL, REPEAT_START))
			UsbPrintString("Repeat start condition enqueued", TRUE);
		else
			UsbPrintString("Queue full", TRUE);*/
		break;

		case USER_I2C_REQUEST_QUEUE_DATA:
		I2cScanQueue();
		break;

		case USER_I2C_REQUEST_QUEUE_DELETE:
		I2cQueueRelease();
		UsbPrintString("I2C queue free", TRUE);
		break;

		case UNKNOWN_COMMAND:
		if(communication_mode == USER_SERIAL_INTERFACE_SELECTED)
			AppToTx(USER_SERIAL_INTERFACE_SELECTED);
		else
			UsbPrintString("unknown command", TRUE);
		break;

		case USER_TRANSFER_REQUEST:
			if(communication_mode != NO_COMMAND)
				AppToTx(communication_mode);
			else
				UsbPrintString("No Interface selected", TRUE);
		break;
		default:
			break;
	}
}

static void I2cScanQueue(void)
{
	i2c_queue_t cmd;
	uint8_t byte        = 0;
	uint16_t conversion = 0;
	uint8_t i           = 0;

	while(I2cGetqueue( &byte, &cmd, i++))
	{
		switch(cmd)
		{
/*			case REPEAT_START:
			UsbPrintString("Start", TRUE);
			break;*/

			case ADDRESS_TRANSFER:

			if ( (byte & MASTER_RECEIVER) != 0 )
				UsbPrintString("Read mode ", FALSE);
			else
			UsbPrintString("Write mode ", FALSE);
			conversion = DecToChar(byte);
			UsbPrintString("Addr: ", FALSE);
			putbyte(USB_INTERFACE, (uint8_t)(conversion >> 8));
			putbyte(USB_INTERFACE, (uint8_t) conversion );
			UsbPrintString("h ", TRUE);
			break;

			case BYTE_WRITE:
			conversion = DecToChar(byte);
			UsbPrintString("Byte write: ", FALSE);
			putbyte(USB_INTERFACE, (uint8_t)(conversion >> 8));
			putbyte(USB_INTERFACE, (uint8_t) conversion );
			UsbPrintString("h ", TRUE);
			break;

			case BYTE_READ:
			UsbPrintString("Byte Read", TRUE);
			break;
			default:
			break;
		}
	}
}

static void AppToTx(command_t mode)
{
	i2c_queue_t cmd;
	uint16_t data = 0;
	uint8_t byte  = 0;
	uint8_t i     = 0;
	switch (mode)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
		SerialTransfer();
		break;
		case USER_I2C_INTERFACE_SELECTED:
		data = (uint16_t) I2cTransfer();
		if(data)
		{
			/* Error on I2C bus*/
			data = DecToChar(data);
			UsbPrintString("Transfer Failure - error code:", FALSE);
			putbyte(USB_INTERFACE, (uint8_t)(data  >> 8));
			putbyte(USB_INTERFACE, (uint8_t) data  );
			UsbPrintString("h ", TRUE);
		}
		else
		{
			/* No Error on I2C bus*/
			/* Output data if user request i2c read*/
			while(I2cGetqueue( &byte, &cmd, i++))
			{
				if(cmd == BYTE_READ)
				{
					data = DecToChar(byte);
					UsbPrintString("Byte read: ", TRUE);
					putbyte(USB_INTERFACE, (uint8_t)(data >> 8));
					putbyte(USB_INTERFACE, (uint8_t) data );
				}
			}
			UsbPrintString("Transfer Success", TRUE);
		}
		case USER_SPI_INTERFACE_SELECTED:
		break;
		default:
			break;
	}
}

void TransferToPc(void)
{
	switch (communication_mode)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
			SerialToUsb();
		break;
		case USER_I2C_INTERFACE_SELECTED:
		case USER_SPI_INTERFACE_SELECTED:
		break;
		default:
			break;
	}

}
