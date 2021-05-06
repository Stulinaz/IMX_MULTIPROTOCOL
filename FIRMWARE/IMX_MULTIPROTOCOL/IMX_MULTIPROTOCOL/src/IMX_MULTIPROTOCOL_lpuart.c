/*
 * IMX_MULTIPROTOCOL_lpuart.c
 *
 *  Created on: 30 apr 2021
 *      Author: Enrico
 */

#include "IMX_MULTIPROTOCOL_lpuart.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"

static void LpuartInitPins(void);

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
	SDK_ISR_EXIT_BARRIER;
}

void LpuartInit(void)
{
    lpuart_config_t config;
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
