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

static void LpuartInitPins(void);

static lpuart_config_t config;

void DEMO_LPUART_IRQHandler(void)
{
	/*Check Transmit register data empty flag & transmit it enable*/
	if ( (kLPUART_TxDataRegEmptyInterruptEnable) & LPUART_GetStatusFlags(DEMO_LPUART))
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
	//SDK_ISR_EXIT_BARRIER;
}

void LpuartInit(void)
{
    LpuartInitPins();
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 9600;
    config.enableTx     = true;
    config.enableRx     = true;
    LPUART_Init(DEMO_LPUART, &config, DEMO_LPUART_CLK_FREQ);

    /* Enable RX interrupt. */
    LPUART_EnableInterrupts(DEMO_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(DEMO_LPUART_IRQn);
}

static void LpuartInitPins(void)
{
	  CLOCK_EnableClock(kCLOCK_Iomuxc);
	  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_LPUART2_TX ,0U);
	  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_LPUART2_RX, 0U);
	  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_LPUART2_TX, 0x10B0u);
	  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_LPUART2_RX, 0x10B0u);
}

void SerStartTransmit(void)
{
	 LPUART_EnableInterrupts(DEMO_LPUART, kLPUART_TxDataRegEmptyInterruptEnable);
}

uint16_t data_Avail(void)
{
	return ser_comm_type.rx_buff_write_index;
}

uint8_t SerBaudrateSel(ser_baudrate_t baudrate)
{
	uint8_t stat = PASS;
	switch(baudrate)
	{
		case BAUDRATE_1200:
		config.baudRate_Bps = 1200;
		break;
		case BAUDRATE_4800:
		config.baudRate_Bps = 4800;
		break;
		case BAUDRATE_9600:
		config.baudRate_Bps = 9600;
		break;
		case BAUDRATE_19200:
		config.baudRate_Bps = 19200;
		break;
		case BAUDRATE_38400:
		config.baudRate_Bps = 38400;
		break;
		case BAUDRATE_57600:
		config.baudRate_Bps = 57600;
		break;
		case BAUDRATE_115200:
		config.baudRate_Bps = 115200;
		break;
		default:
		/* wrong parameter.. setting to default*/
		config.baudRate_Bps = 9600;
		stat = FAIL;
		break;
	}
	return stat;
}
