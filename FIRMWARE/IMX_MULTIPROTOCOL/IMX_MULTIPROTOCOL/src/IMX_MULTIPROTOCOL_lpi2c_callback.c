/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#include "IMX_MULTIPROTOCOL_lpi2c_callback.h"
#include "IMX_MULTIPROTOCOL_gpt.h"
#include "IMX_MULTIPROTOCOL_gpio.h"

LPI2C_Type *i2c = II2C_MASTER;
uint32_t time_start;

static void I2cPollFlagMSR(i2c_error_t *const status, iic_mtdr_cmd_t check_cmd);
static i2c_error_t I2cFailureDetect(void);

void I2C1_Init(void)
{
    /* Pin configuration*/
    I2cInitPins();
	lpi2c_master_config_t masterConfig;
	LPI2C_MasterReset(i2c);
    masterConfig.baudRate_Hz = II2C_BAUDRATE;
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    LPI2C_MasterInit(II2C_MASTER, &masterConfig, II2C_MASTER_CLOCK_FREQUENCY);
    /* Clear all flags. */
    LPI2C_MasterClearStatusFlags(i2c, (uint32_t)kLPI2C_MasterClearFlags);
    /* Turn off auto-stop option. */
    i2c->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;
}

static void I2cPollFlagMSR(i2c_error_t *const status, iic_mtdr_cmd_t check_cmd)
{
	i2c_error_t bus_err = NO_ERROR;
	while (!bus_err)
	{
		bus_err = I2cFailureDetect();
		if (bus_err)
		{
			(*status) = bus_err;
		}
		else
		{
			switch(check_cmd)
			{
				case CMD_TRANSMIT:
				{
					/*Check empty TX FIFO*/
					if ( (i2c->MFSR & LPI2C_MFSR_TXCOUNT_MASK) == 0)
							return;
					break;
				}

				case CMD_RECEIVE:
				{
					/*Check non empty RX FIFO*/
					if ( (i2c->MFSR & LPI2C_MFSR_RXCOUNT_MASK) != 0)
							return;
					break;
				}

				case CMD_STOP:
				{
					/* stop condition detected*/
					if ( (i2c->MSR & LPI2C_MIER_SDIE_MASK) != 0)
						return;
					break;
				}

				case CMD_STARTnADDRESS_ACK:
				{
					/*Check TX FIFO*/
					if ( (i2c->MFSR & LPI2C_MFSR_TXCOUNT_MASK) == 0)
					{
						return;
					}
					if ( (i2c->MSR & LPI2C_MSR_NDF_MASK) != 0)
					{
						(*status) = I2C_ADDRESS_NACK;
						return;
					}
				}
				default:
				break;
			}

		}
	}
}

static i2c_error_t I2cFailureDetect(void)
{
	/* timeout check - TIMEOUT bit is SR1 is set in 25ms*/
	if ((GetTick() - time_start) >= IIC_TIMEOUT)
		return I2C_TIMEOUT;
	/*Arbitration Lost - another master on the bus*/
	if ((i2c->MSR & LPI2C_MSR_ALF_MASK) != 0)
		return I2C_GENERAL_ERROR;
	return NO_ERROR;
}

void Call_I2cCheckActivation (i2c_error_t *const status_transfer)
{
	time_start = GetTick();
	/* Check for peripheral activation */
	if ((i2c->MCR & LPI2C_MCR_MEN_MASK) == 0)
		(*status_transfer) = I2C_NOT_ACTIVATED;
	/* Check for idle bus status*/
	if ((i2c->MCR & LPI2C_MSR_BBF_MASK) != 0)
		(*status_transfer) = I2C_BUS_BUSY;
}

void Call_I2cTransferData(i2c_error_t *const status_transfer, iic_mtdr_cmd_t cmd, uint8_t *const byte, uint8_t dir)
{
	switch(cmd)
	{
		case CMD_TRANSMIT:
		{
			/* Data transmit */
			i2c->MTDR = (uint32_t)IIC_CMD_TRANSMIT | ( ((uint32_t)(*byte)) );
			I2cPollFlagMSR(status_transfer, cmd);
			break;
		}

		case CMD_RECEIVE:
		{
			/* Receive data */
			i2c->MTDR = (uint32_t)IIC_CMD_RECEIVE | ( ((uint32_t)(*byte)) );
			I2cPollFlagMSR(status_transfer, cmd);
			*byte = (uint8_t) i2c->MRDR;
			break;
		}

		case CMD_STARTnADDRESS_ACK:
		{
			/* Send address - ack mode*/
			i2c->MTDR = (uint32_t)IIC_CMD_STARTnADDRESS_ACK  | (((uint32_t)(*byte) << 1U) | (uint32_t) dir);
			I2cPollFlagMSR(status_transfer, cmd);
			break;
		}


		case CMD_STOP:
		{
			/* Generation of stop condition*/
			i2c->MTDR = (uint32_t)IIC_CMD_STOP;
			I2cPollFlagMSR(status_transfer, cmd);
			break;
		}
		default:
		break;
	}
}

void Call_I2cReset(void)
{
	/*Clear Nack Detect Flag*/
	i2c->MSR &= (~ (uint32_t)LPI2C_MSR_NDF_MASK);
	/* CLear Fifo Error Flag*/
	i2c->MSR &= (~ (uint32_t)LPI2C_MSR_FEF_MASK );
	/* periph reset*/
	LPI2C_MasterReset(i2c);
}

void I2cStop(void)
{
	LPI2C_MasterDeinit(II2C_MASTER);
}
