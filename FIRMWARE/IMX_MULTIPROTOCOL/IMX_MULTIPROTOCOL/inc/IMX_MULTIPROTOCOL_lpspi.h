/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#ifndef INC_IMX_MULTIPROTOCOL_LPSPI_H_
#define INC_IMX_MULTIPROTOCOL_LPSPI_H_

#include "fsl_lpspi.h"

#define EXAMPLE_LPSPI_MASTER_BASEADDR   (LPSPI1)
#define EXAMPLE_LPSPI_MASTER_IRQN       LPSPI1_IRQn
#define EXAMPLE_LPSPI_MASTER_IRQHandler LPSPI1_IRQHandler
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define EXAMPLE_LPSPI_DEALY_COUNT 0x4fffffU
#define EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT (1U)
#define EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER (7U)
#define LPSPI_MASTER_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER + 1U))
#define TRANSFER_SIZE     64U     /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 100000U /*! Transfer baudrate - 100k */

typedef enum{
NSS_ACTIVE_LOW = 0,
NSS_ACTIVE_HIGH,
}spi_nss_t;

extern spi_nss_t masterNss;

void LpSpiInit(void);
void LpSpiStop(void);
status_t LpspiTransfer(void);

#endif /* INC_IMX_MULTIPROTOCOL_LPSPI_H_ */
