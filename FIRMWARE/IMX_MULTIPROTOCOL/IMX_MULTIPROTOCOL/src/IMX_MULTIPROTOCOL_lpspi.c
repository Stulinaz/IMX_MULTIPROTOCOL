/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#include "IMX_MULTIPROTOCOL_lpspi.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"

static void LpSpiInitPins(void);

lpspi_master_config_t masterConfig;

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

void LpSpiStop(void)
{
	LPSPI_Deinit(EXAMPLE_LPSPI_MASTER_BASEADDR);
}

static void LpSpiInitPins(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);
  IOMUXC_SetPinMux(
       IOMUXC_GPIO_AD_B0_10_LPSPI1_SCK,        /* GPIO_AD_B0_10 is configured as LPSPI1_SCK */
       0U);                                    /* Software Input On Field: Input Path is determined by functionality */
   IOMUXC_SetPinMux(
       IOMUXC_GPIO_AD_B0_11_LPSPI1_PCS0,       /* GPIO_AD_B0_11 is configured as LPSPI1_PCS0 */
       0U);                                    /* Software Input On Field: Input Path is determined by functionality */
   IOMUXC_SetPinMux(
       IOMUXC_GPIO_AD_B0_12_LPSPI1_SDO,        /* GPIO_AD_B0_12 is configured as LPSPI1_SDO */
       0U);                                    /* Software Input On Field: Input Path is determined by functionality */
   IOMUXC_SetPinMux(
       IOMUXC_GPIO_AD_B0_13_LPSPI1_SDI,        /* GPIO_AD_B0_13 is configured as LPSPI1_SDI */
       0U);                                    /* Software Input On Field: Input Path is determined by functionality */

   IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_10_LPSPI1_SCK,        /* GPIO_AD_B0_10 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                   Drive Strength Field: R0/6
                                                   Speed Field: medium(100MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_11_LPSPI1_PCS0,       /* GPIO_AD_B0_11 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                   Drive Strength Field: R0/6
                                                   Speed Field: medium(100MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_12_LPSPI1_SDO,        /* GPIO_AD_B0_12 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                   Drive Strength Field: R0/6
                                                   Speed Field: medium(100MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_13_LPSPI1_SDI,        /* GPIO_AD_B0_13 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                   Drive Strength Field: R0/6
                                                   Speed Field: medium(100MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Disabled */
}

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
