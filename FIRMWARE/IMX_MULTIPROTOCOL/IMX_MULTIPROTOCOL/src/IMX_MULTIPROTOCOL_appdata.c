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
#include "IMX_MULTIPROTOCOL_lpspi.h"

command_t communication_mode = NO_COMMAND;

static void AppToTx(command_t mode);
static void I2cScanQueue(void);
static void PrintHelp(void);

void Application (command_t * user_cmd, uint8_t * const user_data)
{
	switch(*user_cmd)
	{
		case NO_COMMAND:
			break;

		case UNKNOWN_COMMAND:
		{
			if ( (communication_mode == USER_SERIAL_INTERFACE_SELECTED) || (communication_mode == USER_SPI_INTERFACE_SELECTED) )
				AppToTx(communication_mode);
			else
				UsbPrintString("unknown command", TRUE);
			break;
		}


		case USER_COMMUNICATION_ABORT:
		{
			/* Stop streaming data*/
			if(communication_mode == USER_SERIAL_INTERFACE_SELECTED)
			{
				LpuartStop();
				UsbPrintString("Serial interface closed", TRUE);
				communication_mode = NO_COMMAND;
			}
			else if(communication_mode == USER_SPI_INTERFACE_SELECTED)
			{
				LpSpiStop();
				UsbPrintString("SPI interface closed", TRUE);
				communication_mode = NO_COMMAND;
			}
			break;
		}

		case USER_SERIAL_INTERFACE_SELECTED:
		if(communication_mode == NO_COMMAND)
		{
			LedInterfaceSel(USER_SERIAL_INTERFACE_SELECTED);
			LpuartInit();
			communication_mode = USER_SERIAL_INTERFACE_SELECTED;
			UsbPrintString("Serial interface selected", TRUE);
			break;
		}

		case USER_I2C_INTERFACE_SELECTED:
		{
			LedInterfaceSel(USER_I2C_INTERFACE_SELECTED);
			communication_mode = USER_I2C_INTERFACE_SELECTED;
			UsbPrintString("I2C interface selected", TRUE);
			break;
		}

		case USER_SPI_INTERFACE_SELECTED:
		if(communication_mode == NO_COMMAND)
		{
			LedInterfaceSel(USER_SPI_INTERFACE_SELECTED);
			LpSpiInit();
			communication_mode = USER_SPI_INTERFACE_SELECTED;
			UsbPrintString("SPI interface selected", TRUE);
			break;
		}

		case USER_SER_SET_BAUDRATE:
		if(communication_mode != USER_SERIAL_INTERFACE_SELECTED)
		{
			/*changing serial baudrate on the fly not supported*/
			if(SerBaudrateSel(*user_data))
				UsbPrintString("Serial baudrate set", TRUE);
			break;
		}

		/* I2C Handler */
		case USER_I2C_SET_SLAVE_ADDRESS:
		{
			I2cSetSlaveAddress( (*user_data ) );
			UsbPrintString("Slave address set", TRUE);
			break;
		}

		case USER_IC2_ACCESS_MODE_TRANSMITTER:
		{
			if (I2cEnqeue(MASTER_TRANSMITTER , ADDRESS_TRANSFER))
				UsbPrintString("Sending address in write mode", TRUE);
			else
				UsbPrintString("Queue full", TRUE);
			break;
		}

		case USER_IC2_ACCESS_MODE_RECEIVER:
		{
			if (I2cEnqeue(MASTER_RECEIVER , ADDRESS_TRANSFER))
				UsbPrintString("Sending address in read mode", TRUE);
			else
				UsbPrintString("Queue full", TRUE);
			break;
		}

		case USER_I2C_WRITE_DATA:
		{
			if (I2cEnqeue( (*user_data), BYTE_WRITE) )
				UsbPrintString("Byte transfer enqueued", TRUE);
			else
				UsbPrintString("Queue full", TRUE);
			break;
		}

		case USER_I2C_READ_DATA:
		{
			if (I2cEnqeue( NUL, BYTE_READ) )
				UsbPrintString("Byte read enqueued", TRUE);
			else
				UsbPrintString("Queue full", TRUE);
			break;
		}

		case USER_I2C_REPEAT_START:
/*		if(I2cEnqeue( NUL, REPEAT_START))
			UsbPrintString("Repeat start condition enqueued", TRUE);
		else
			UsbPrintString("Queue full", TRUE);*/
		break;

		case USER_I2C_REQUEST_QUEUE_DATA:
		{
			I2cScanQueue();
			break;
		}

		case USER_I2C_REQUEST_QUEUE_DELETE:
		{
			I2cQueueRelease();
			UsbPrintString("I2C queue free", TRUE);
			break;
		}

		case USER_TRANSFER_REQUEST:
		{
			/* solo per i2c*/
			if(communication_mode != NO_COMMAND)
				AppToTx(communication_mode);
			else
				UsbPrintString("No Interface selected", TRUE);
			break;
		}

		case HELP:
		PrintHelp();
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
	uint32_t data_len;
	i2c_queue_t cmd;
	uint16_t data = 0;
	uint8_t byte  = 0;
	uint8_t i     = 0;
	switch (mode)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
		{
			data_len = SetBuffer(SER_INTERFACE);
			if(data_len)
				SerStartTransmit();
			break;
		}

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
		{
			status_t spi_transfer_status;
			data_len = SetBuffer(SPI_INTERFACE);
			if(data_len)
			{
				spi_transfer_status = LpspiTransfer();
				if(spi_transfer_status == kStatus_Success)
				{
					UsbPrintString("Transfer Success Rx DATA:", FALSE);
					for(i=0; i<data_len; i++)
					{
						getbyte(SPI_INTERFACE, &byte);
						putbyte(USB_INTERFACE, byte);
					}
					UsbPrintString("h ", TRUE);
				}
				else
					UsbPrintString("Transfer Fail", TRUE);
			}
		}
		break;
		default:
			break;
	}
}

void AppToRx(void)
{
	switch (communication_mode)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
			SerialToUsb();
		break;
		case USER_I2C_INTERFACE_SELECTED:
		case USER_SPI_INTERFACE_SELECTED:
		default:
		break;
	}
}

static void PrintHelp(void)
{
	/*general command + options*/
	UsbPrintString("imx ", FALSE);
	UsbPrintString(" options: ", FALSE);
	putbyte(USB_INTERFACE, CR_);

	putbyte(USB_INTERFACE, TAB);
	UsbPrintString(serial_cmd_select, FALSE);
	UsbPrintString( " :Enables serial interface", FALSE);
	putbyte(USB_INTERFACE, CR_);

	putbyte(USB_INTERFACE, TAB);
	UsbPrintString(i2c_cmd_select, FALSE);
	UsbPrintString( " :Enables i2c interface", FALSE);
	putbyte(USB_INTERFACE, CR_);

	putbyte(USB_INTERFACE, TAB);
	UsbPrintString(spi_cmd_select, FALSE);
	UsbPrintString( " :Enables spi interface", FALSE);
	putbyte(USB_INTERFACE, CR_);

	putbyte(USB_INTERFACE, TAB);
	UsbPrintString(close, FALSE);
	UsbPrintString( " :Close Communication", FALSE);
	putbyte(USB_INTERFACE, CR_);

	putbyte(USB_INTERFACE, CR_);

	/*serial command + options*/
	UsbPrintString("ser", FALSE);
	putbyte(USB_INTERFACE, TAB);
	UsbPrintString(serial_bausel, FALSE);
	UsbPrintString(" :Selection of serial badrate", FALSE);

	/*12c command + options*/

	UsbPrintString("Hacathon 2021", TRUE);
}
