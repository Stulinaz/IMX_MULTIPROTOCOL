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
#include "IMX_MULTIPROTOCOL_lpi2c_callback.h"
#include "IMX_MULTIPROTOCOL_lpuart.h"
#include "IMX_MULTIPROTOCOL_ledmanager.h"
#include "IMX_MULTIPROTOCOL_lpspi.h"
#include "IMX_MULTIPROTOCOL_definitions.h"
#include "IMX_MULTIPROTOCOL_gpt.h"
#include "IMX_MULTIPROTOCOL_gpio.h"

command_t communication_mode = NO_COMMAND;

static void AppToTx(command_t mode);
static void I2cScanQueue(void);
static void PrintHelp(void);

/****************************************************************************
Function:			ApplicationData
Input:				(command_t * user_cmd) last command correctly decoded
					(uint8_t * const user_data) parameter decoded
Overview:			Application Data Transfer
****************************************************************************/
void ApplicationData (command_t * user_cmd, uint8_t * const user_data)
{
	uint8_t i;

	switch(*user_cmd)
	{
		case NO_COMMAND:
			break;

		case UNKNOWN_COMMAND:
		{
			if ( (communication_mode == USER_SERIAL_INTERFACE_SELECTED) || (communication_mode == USER_SPI_INTERFACE_SELECTED) )
			{
				/* with serial and Spi interface selected, device must forward all data*/
				AppToTx(communication_mode);
			}
			else
			{
				/* What did you say?*/
				UsbPrintString("unknown command", TRUE);
			}
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
			else if(communication_mode == USER_I2C_INTERFACE_SELECTED)
			{
				LpSpiStop();
				UsbPrintString("I2C interface closed", TRUE);
				communication_mode = NO_COMMAND;
			}
			else
				UsbPrintString("No interface selected", TRUE);
			break;
		}

		case USER_SERIAL_INTERFACE_SELECTED:
		{
			if(communication_mode == NO_COMMAND)
			{
				LedInterfaceSel(USER_SERIAL_INTERFACE_SELECTED);
				LpuartInit();
				communication_mode = USER_SERIAL_INTERFACE_SELECTED;
			}
			UsbPrintString("Serial interface selected", TRUE);
			break;
		}

		case USER_I2C_INTERFACE_SELECTED:
		{
			LedInterfaceSel(USER_I2C_INTERFACE_SELECTED);
			#ifdef I2C_SCL_RELEASE
			if(communication_mode != USER_I2C_INTERFACE_SELECTED)
			{
				/* Slave device could get stuck on bus holding SDA to low*/
				/* Generate 8 clock endges to release it*/
				I2cSCLPushPull();
				for(i=0;i<8;i++)
				{
					Delay(2);
					GPIO_PinWrite(GPIO1, 30 , RESET);
					Delay(2);
					GPIO_PinWrite(GPIO1, 30 , SET);
				}
				Delay(2);
			}
			#endif
			I2cInitPins();
			communication_mode = USER_I2C_INTERFACE_SELECTED;
			UsbPrintString("I2C interface selected", TRUE);
			break;
		}

		case USER_SPI_INTERFACE_SELECTED:
		{
			if(communication_mode == NO_COMMAND)
			{
				LedInterfaceSel(USER_SPI_INTERFACE_SELECTED);
				LpSpiInit();
				communication_mode = USER_SPI_INTERFACE_SELECTED;
			}
			UsbPrintString("SPI interface selected", TRUE);
			break;
		}

		/* SPI Options */
		case USER_NSS_ACTIVE_HIGH:
		{
			#ifdef SPI_NSS_DRIVEN_MANUALLY
			masterNss = NSS_ACTIVE_HIGH;
			UsbPrintString("SPI NSS active high", TRUE);
			/* Put NSS to IDLE state*/
			GPIO_PinWrite(GPIO1, 11 , RESET);
			#endif
			break;
		}

		case USER_NSS_ACTIVE_LOW:
		{
			#ifdef SPI_NSS_DRIVEN_MANUALLY
			masterNss = NSS_ACTIVE_LOW;
			/* Put NSS to IDLE state*/
			GPIO_PinWrite(GPIO1, 11 , SET);
			UsbPrintString("SPI NSS active low", TRUE);
			#endif
			break;
		}

		/* Serial Options */
		case USER_SER_SET_BAUDRATE:
		{
			if(communication_mode != USER_SERIAL_INTERFACE_SELECTED)
			{
				/* Changing serial baudrate on the fly not supported*/
				if(SerBaudrateSel(*user_data))
					UsbPrintString("Serial baudrate set", TRUE);
			}
			else
				UsbPrintString("Error - Serial interface must be closed", TRUE);
			break;
		}

		/* I2C Options */
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
				UsbPrintString("Byte write enqueued", TRUE);
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

		case USER_I2C_REQUEST_QUEUE_DATA:
		{
			I2cScanQueue();
			break;
		}

		case USER_I2C_REQUEST_QUEUE_DELETE:
		{
			I2cQueueRelease();
			UsbPrintString("Queue free", TRUE);
			break;
		}

		case USER_I2C_TRANSFER_REQUEST:
		{
			if(communication_mode == USER_I2C_INTERFACE_SELECTED)
				AppToTx(USER_I2C_INTERFACE_SELECTED);
			else
				UsbPrintString("No I2c Interface selected", TRUE);
			break;
		}

		case HELP:
		{
			/* Help me! */
			PrintHelp();
			break;
		}

		default:
			break;
	}
}

/****************************************************************************
Function:			I2cScanQueue
Input:				none
Output:				none
PreCondition:		none
Overview:			Scan the I2c commands queue and output to the user
****************************************************************************/
static void I2cScanQueue(void)
{
	i2c_queue_t cmd;
	uint8_t byte        = 0;
	uint16_t conversion = 0;
	uint8_t i           = 0;

	/* check empty queue*/
	I2cGetqueue( &byte, &cmd, 0);
	if(cmd == NO_COMMAND_SELECTED)
	{
		UsbPrintString("I2C queue free", TRUE);
		return;
	}

	while(I2cGetqueue( &byte, &cmd, i++))
	{
		switch(cmd)
		{
			case ADDRESS_TRANSFER:
			{
				if ( (byte & MASTER_RECEIVER) != 0 )
					UsbPrintString("Read mode ", FALSE);
				else
				UsbPrintString("Write mode ", FALSE);
				conversion = DecToChar(byte);
				UsbPrintString("Addr: ", FALSE);
				putbyte(USB_INTERFACE, (uint8_t)(conversion >> 8));
				putbyte(USB_INTERFACE, (uint8_t) conversion );
				UsbPrintString("h ", FALSE);
				putbyte(USB_INTERFACE, CR_);
				break;
			}

			case BYTE_WRITE:
			{
				conversion = DecToChar(byte);
				UsbPrintString("Byte write: ", FALSE);
				putbyte(USB_INTERFACE, (uint8_t)(conversion >> 8));
				putbyte(USB_INTERFACE, (uint8_t) conversion );
				UsbPrintString("h ", FALSE);
				putbyte(USB_INTERFACE, CR_);
				break;
			}

			case BYTE_READ:
			{
				UsbPrintString("Byte Read", FALSE);
				putbyte(USB_INTERFACE, CR_);
				break;
			}

			case NO_COMMAND_SELECTED:
			{
				UsbPrintString("Queue End", FALSE);
				putbyte(USB_INTERFACE, CR_);
			}
			default:
				break;
		}
	}
	UsbPrintString(" ", TRUE);
}

/****************************************************************************
Function:			AppToTx
Input:				command_t mode (selected working interface UART - SPI - I2C)
Output:				none
PreCondition:		none
Overview:			Start data transfer on selected working interface
****************************************************************************/
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
			{
				/* Enable serial TX interrupt*/
				SerStartTransmit();
			}
			break;
		}

		case USER_I2C_INTERFACE_SELECTED:
		{
			data = (uint16_t) I2cTransfer();
			if(data)
			{
				/* Error on I2C bus*/
				#ifndef I2C_ERROR_VERBOSE
				data = DecToChar(data);
				UsbPrintString("Transfer Failure - error code:", FALSE);
				putbyte(USB_INTERFACE, (uint8_t)(data  >> 8));
				putbyte(USB_INTERFACE, (uint8_t) data  );
				UsbPrintString("h ", TRUE);
				#else
				UsbPrintString("Transfer Failure ",FALSE);
				switch(data)
				{
					case I2C_BUS_BUSY:
					UsbPrintString("Bus busy", TRUE);
					break;
					case I2C_TIMEOUT:
					UsbPrintString("Timeout", TRUE);
					break;
					case I2C_ADDRESS_NACK:
					UsbPrintString("Address NACK", TRUE);
					break;
					default:
						UsbPrintString("General Error", TRUE);
				}
				#endif
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
						UsbPrintString("h ", FALSE);
					}
				}
				UsbPrintString("Transfer Success", TRUE);
			}
			break;
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
					/* No error on SPI transfer */
					UsbPrintString("Transfer Success Rx DATA:", FALSE);
					for(i=0; i<data_len; i++)
					{
						/* Output to the user the received characters*/
						getbyte(SPI_INTERFACE, &byte);
						data = DecToChar(byte);
						putbyte(USB_INTERFACE, (uint8_t)(data >> 8));
						putbyte(USB_INTERFACE, (uint8_t) data );
						UsbPrintString("h ", FALSE);
					}
					UsbPrintString(" ", TRUE);
				}
				else
					UsbPrintString("Transfer Fail", TRUE);
			}
			break;
		}

		default:
			break;
	}
}

/****************************************************************************
Function:			AppToRx
Input:				none
Output:				none
PreCondition:		none
Overview:			Check data on UART interface
case Data avalible: Output data to the user
case NO data abaliable: Output "NO data String"
****************************************************************************/
void AppToRx(void)
{
	switch (communication_mode)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
		{
			/* Serial receiver must be ready at anytime*/
			if( SerialToUsb() == 0)
				UsbPrintString("No Data", TRUE);
			break;
		}
		case USER_I2C_INTERFACE_SELECTED:
		case USER_SPI_INTERFACE_SELECTED:
		/* SPI and I2C receiver starts in async mode*/
		default:
		break;
	}
}

/****************************************************************************
Function:			PrintHelp
Input:				none
Output:				none
PreCondition:		none
Overview:			Print a help string to the User on Virtual Com Port
****************************************************************************/
static void PrintHelp(void)
{
	/*general command + options*/
	UsbPrintString("imx ", FALSE);
	UsbPrintString(serial_cmd_select, FALSE);
	UsbPrintString( " :Enables serial interface", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("imx ", FALSE);
	UsbPrintString(i2c_cmd_select, FALSE);
	UsbPrintString( " :Enables i2c interface", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("imx ", FALSE);
	UsbPrintString(spi_cmd_select, FALSE);
	UsbPrintString( " :Enables spi interface", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("imx ", FALSE);
	UsbPrintString(close, FALSE);
	UsbPrintString( " :Close Communication", FALSE);
	putbyte(USB_INTERFACE, CR_);

	putbyte(USB_INTERFACE, CR_);

	/*serial command + options*/
	UsbPrintString("ser ", FALSE);
	UsbPrintString(serial_bausel, FALSE);
	UsbPrintString(" :Selection of serial badrate", FALSE);
	putbyte(USB_INTERFACE, CR_);

	/*spi command + options*/
	UsbPrintString("spi ", FALSE);
	UsbPrintString(spinss_active_high, FALSE);
	UsbPrintString(" :spinss_active_high", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("spi ", FALSE);
	UsbPrintString(spinss_active_low, FALSE);
	UsbPrintString(" :spinss_active_low", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("spi ", FALSE);
	UsbPrintString(spinss_active_high, FALSE);
	UsbPrintString(" :spinss_active_high", FALSE);
	putbyte(USB_INTERFACE, CR_);

	/*12c command + options*/
	UsbPrintString("i2c ", FALSE);
	UsbPrintString(i2c_address, FALSE);
	UsbPrintString(" :select slave address", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("i2c ", FALSE);
	UsbPrintString(i2c_transmitter, FALSE);
	UsbPrintString(" :address write mode", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("i2c ", FALSE);
	UsbPrintString(i2c_access_write, FALSE);
	UsbPrintString(" :byte write", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("i2c ", FALSE);
	UsbPrintString(i2c_access_read, FALSE);
	UsbPrintString(" :byte read", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("i2c ", FALSE);
	UsbPrintString(i2c_queue_req, FALSE);
	UsbPrintString(" :print i2c commands in the queue", FALSE);
	putbyte(USB_INTERFACE, CR_);

	UsbPrintString("i2c ", FALSE);
	UsbPrintString(i2c_queue_delete, FALSE);
	UsbPrintString(" :delete i2c commands queue", FALSE);
	putbyte(USB_INTERFACE, CR_);

	/* End string*/
	putbyte(USB_INTERFACE, CR_);
	UsbPrintString("Electromaker - Hackathon 2021", TRUE);
}

command_t GetWorkingInterface(void)
{
	return communication_mode;
}
