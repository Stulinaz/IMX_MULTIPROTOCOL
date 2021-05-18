/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#include "IMX_MULTIPROTOCOL_lpspi.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"
#include "IMX_MULTIPROTOCOL_definitions.h"
#include "IMX_MULTIPROTOCOL_gpio.h"

lpspi_master_config_t masterConfig;

/****************************************************************************
Function:			LpSpiInit
Input:				none
Output:				none
PreCondition:		none
Overview:			SPI configuration
****************************************************************************/
void LpSpiInit(void)
{
    uint32_t srcClock_Hz;
    LpSpiInitPins();
    /*Master config*/
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pinCfg   = kLPSPI_SdiInSdoOut;
    srcClock_Hz = LPSPI_MASTER_CLK_FREQ;
    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);
}

/****************************************************************************
Function:			LpspiTransfer
Input:				none
Output:				Transfer status (0 == Ok)
PreCondition:		SPI interface must be enabled enabled
Overview:			SPI transfer (Polling mode)
****************************************************************************/
void LpSpiStop(void)
{
	LPSPI_Deinit(EXAMPLE_LPSPI_MASTER_BASEADDR);
}

/****************************************************************************
Function:			LpspiTransfer
Input:				none
Output:				Transfer status (0 == Ok)
PreCondition:		SPI interface enabled
Overview:			SPI transfer (Polling mode)
****************************************************************************/
status_t LpspiTransfer(void)
{
	status_t stat = kStatus_Fail;
	lpspi_transfer_t masterXfer;

	/* Assign Buffers pointers*/
    masterXfer.txData      = spi_tx_buff;
    masterXfer.rxData      = spi_rx_buff;
    masterXfer.dataSize    = data_toprocess(SPI_INTERFACE);

    if(masterXfer.dataSize)
    {
		spi_comm_type.rx_buff_write_index  = masterXfer.dataSize;
		masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterByteSwap;
		stat = LPSPI_MasterTransferBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterXfer);
		spi_comm_type.tx_buff_read_index  = 0;
		spi_comm_type.tx_buff_write_index = 0;
    }
    return stat;
}
