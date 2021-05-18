/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/
#include "IMX_MULTIPROTOCOL_lpuart.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"
#include "IMX_MULTIPROTOCOL_definitions.h"
#include "IMX_MULTIPROTOCOL_gpio.h"

static lpuart_config_t config;
static uint32_t lpuart_baudrate = BAUDRATE_DEFAULT;

void DEMO_LPUART_IRQHandler(void)
{
	/*Check Transmit register data empty flag & transmit it enable*/
	if ( (kLPUART_TxDataRegEmptyInterruptEnable & LPUART_GetStatusFlags(DEMO_LPUART)) && (ser_comm_type.tx_buff_write_index != 0) )
	{
		LPUART_WriteByte(DEMO_LPUART, ser_tx_buff[ser_comm_type.tx_buff_read_index]);
		ser_comm_type.tx_buff_read_index++;
		if (ser_comm_type.tx_buff_read_index >= ser_comm_type.tx_buff_write_index)
		{
			LPUART_DisableInterrupts(DEMO_LPUART, kLPUART_TxDataRegEmptyInterruptEnable);
			ser_comm_type.tx_buff_read_index  = 0;
			ser_comm_type.tx_buff_write_index = 0;
		}
	}
	else
	{
		/*Check Read data register not empty flag*/
		if ( (kLPUART_RxDataRegFullFlag) & LPUART_GetStatusFlags(DEMO_LPUART))
		{
			if(ser_comm_type.rx_buff_write_index < (ser_comm_type.rx_max_buff_dim - 1))
			{
				ser_rx_buff[ser_comm_type.rx_buff_write_index] = LPUART_ReadByte(DEMO_LPUART);
				ser_comm_type.rx_buff_write_index++;
			}
			else
				LPUART_ReadByte(DEMO_LPUART);
		}
	}
	SDK_ISR_EXIT_BARRIER;
}

void LpuartInit(void)
{
    LpuartInitPins();
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = lpuart_baudrate;
    config.enableTx     = true;
    config.enableRx     = true;
    LPUART_Init(DEMO_LPUART, &config, DEMO_LPUART_CLK_FREQ);
    /* Enable RX interrupt. */
    LPUART_EnableInterrupts(DEMO_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(DEMO_LPUART_IRQn);
}

void LpuartStop(void)
{
    /* Disable RX interrupt. */
	DisableIRQ(DEMO_LPUART_IRQn);
	LPUART_Deinit(DEMO_LPUART);
}

void SerStartTransmit(void)
{
	 /* Enable TX interrupt. */
	 LPUART_EnableInterrupts(DEMO_LPUART, kLPUART_TxDataRegEmptyInterruptEnable);
}

uint8_t SerBaudrateSel(ser_baudrate_t baudrate)
{
	uint8_t stat = PASS;
	switch(baudrate)
	{
		case BAUDRATE_1200:
			lpuart_baudrate = 1200;
		break;
		case BAUDRATE_4800:
			lpuart_baudrate = 4800;
		break;
		case BAUDRATE_9600:
			lpuart_baudrate = 9600;
		break;
		case BAUDRATE_19200:
			lpuart_baudrate = 19200;
		break;
		case BAUDRATE_38400:
			lpuart_baudrate = 38400;
		break;
		case BAUDRATE_57600:
			lpuart_baudrate = 57600;
		break;
		case BAUDRATE_115200:
			lpuart_baudrate = 115200;
		break;
		default:
		/* wrong parameter.. setting to default*/
			lpuart_baudrate = 9600;
		stat = FAIL;
		break;
	}
	return stat;
}
