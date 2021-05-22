#include "IMX_MULTIPROTOCOL_i2c.h"
#include"IMX_MULTIPROTOCOL_lpi2c_callback.h"

typedef struct{
uint8_t            slave_addr;
uint8_t            transmission_type;
uint8_t     	   data[I2C_QUEUE_COMMAND_DIM];
i2c_queue_t        iic_queue[I2C_QUEUE_COMMAND_DIM];
i2c_error_t        iic_transfer_status;
}i2c_comm_param;

typedef struct
{
	void (*Start)         (void);
	void (*Check)         (i2c_error_t *const status_transfer);
	void (*TransferData)  (i2c_error_t *const status_transfer, iic_mtdr_cmd_t cmd, uint8_t *const byte,uint8_t dir);
	void (*Reset)         (void);
}I2c_callback_t;

i2c_comm_param i2c_transfer  = {0x50,
								MASTER_TRANSMITTER,
								{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
								NO_ERROR};

I2c_callback_t iic_f = {I2C1_Init,
						Call_I2cCheckActivation,
					    Call_I2cTransferData,
						Call_I2cReset
						};

_bool I2cEnqeue( uint8_t data, i2c_queue_t param)
{
	uint8_t i;
	uint8_t quque_pos = 0;
	_bool stat = FALSE;

	/* check queue */
	for(i=0; i<I2C_QUEUE_COMMAND_DIM; i++)
	{
		if(i2c_transfer.iic_queue[i] == NO_COMMAND_SELECTED)
		{
			quque_pos = i;
			stat = TRUE;
			break;
		}
	}
	if(!stat)
		return stat;

	/* check commands */
	switch(param)
	{
		case BYTE_READ:
		{
			i2c_transfer.iic_queue[quque_pos] =  param;
			i2c_transfer.data[quque_pos]      =  NUL;
			stat = TRUE;
			break;
		}
		case ADDRESS_TRANSFER:
		case BYTE_WRITE:
		{
			i2c_transfer.iic_queue[quque_pos] =  param;
			i2c_transfer.data[quque_pos]      =  data;
			stat = TRUE;
			break;
		}
		default:
			break;
	}
	return stat;
}

void I2cQueueRelease(void)
{
	uint8_t i;
	/* first start condition and address is not deleted*/
	for(i=I2C_OFFSET_TRANSF; i<I2C_QUEUE_COMMAND_DIM; i++)
	{
		i2c_transfer.iic_queue[i] =  NO_COMMAND_SELECTED;
		i2c_transfer.data[i]      =  NUL;
	}
}

_bool I2cGetqueue( uint8_t *const data, i2c_queue_t *const param, uint8_t queue_pos)
{
	_bool stat = FALSE;
	if(i2c_transfer.iic_queue[queue_pos] != NO_COMMAND_SELECTED)
	{
		(*param) = i2c_transfer.iic_queue[queue_pos];
		(*data)  = i2c_transfer.data[queue_pos];
		stat = TRUE;
	}
	return stat;
}

uint8_t I2cTransfer(void)
{
	uint8_t i = I2C_OFFSET_TRANSF;
	i2c_transfer.iic_transfer_status = NO_ERROR;
	/* Check activation of i2c peripheral*/
	iic_f.Check(&i2c_transfer.iic_transfer_status);
	if (i2c_transfer.iic_transfer_status != NO_ERROR)
	{
		iic_f.Start();
		i2c_transfer.iic_transfer_status = NO_ERROR;
	}
	/* keep calling commands untill error or empty command*/
	while ( (i2c_transfer.iic_queue[i] != NO_COMMAND_SELECTED) && (i2c_transfer.iic_transfer_status == NO_ERROR) )
	{
		switch(i2c_transfer.iic_queue[i])
		{
			case ADDRESS_TRANSFER:
			{
				/* Send Slave address in master transmitter or master receiver mode*/
				iic_f.TransferData(&i2c_transfer.iic_transfer_status, CMD_STARTnADDRESS_ACK, &i2c_transfer.slave_addr, i2c_transfer.data[i]);
				break;
			}

			case BYTE_WRITE:
			{
				iic_f.TransferData(&i2c_transfer.iic_transfer_status, CMD_TRANSMIT, &i2c_transfer.data[i], NUL);
				/* generate stop condition if there are no more commands in the queue*/
				if (i2c_transfer.iic_queue[i+1] == NO_COMMAND_SELECTED)
					iic_f.TransferData(&i2c_transfer.iic_transfer_status, CMD_STOP, NUL, NUL);
				break;
			}

			case BYTE_READ:
			{
				iic_f.TransferData(&i2c_transfer.iic_transfer_status, CMD_RECEIVE, &i2c_transfer.data[i], NUL);
				/* generate stop condition if there are no more commands in the queue*/
				if (i2c_transfer.iic_queue[i+1] == NO_COMMAND_SELECTED)
					iic_f.TransferData(&i2c_transfer.iic_transfer_status, CMD_STOP, NUL, NUL);
				break;
			}

			default:
				break;
		}
		i++;
	}
	/* I2C reset*/
	iic_f.Reset();
	return i2c_transfer.iic_transfer_status;
}

void DBG_i2c(void)
{
	//#define READ
	#define MICROCHIP_EEPROM_24LC512

	#ifdef MICROCHIP_EEPROM_24LC512
	#ifdef READ
	I2cEnqeue( MASTER_TRANSMITTER, ADDRESS_TRANSFER);
	I2cEnqeue( 0x01, BYTE_WRITE);
	I2cEnqeue( 0x02, BYTE_WRITE);
	I2cEnqeue( MASTER_RECEIVER, ADDRESS_TRANSFER);
	I2cEnqeue( NUL, BYTE_READ);
	I2cEnqeue( NUL, BYTE_READ);
	#else
	I2cEnqeue( MASTER_TRANSMITTER, ADDRESS_TRANSFER);
	/* MSB address*/
	I2cEnqeue( 0x01, BYTE_WRITE);
	/* LSB adrress*/
	I2cEnqeue( 0x02, BYTE_WRITE);
	/* Data*/
	I2cEnqeue( 0x02, BYTE_WRITE);
	#endif
	#endif
}

void I2cSetSlaveAddress(uint8_t addr)
{
	i2c_transfer.slave_addr = addr;
}
