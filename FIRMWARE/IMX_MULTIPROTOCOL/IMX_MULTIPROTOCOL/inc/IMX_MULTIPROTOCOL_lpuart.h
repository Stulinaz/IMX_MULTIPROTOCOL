/*
 * IMX_MULTIPROTOCOL_lpuart.h
 *
 *  Created on: 30 apr 2021
 *      Author: Enrico
 */

#ifndef IMX_MULTIPROTOCOL_LPUART_H_
#define IMX_MULTIPROTOCOL_LPUART_H_

#include "fsl_lpuart.h"
#include "fsl_iomuxc.h"
#include "board.h"

#define DEMO_LPUART            LPUART2
#define DEMO_LPUART_CLK_FREQ   BOARD_DebugConsoleSrcFreq()
#define DEMO_LPUART_IRQn       LPUART2_IRQn
#define DEMO_LPUART_IRQHandler LPUART2_IRQHandler

void LpuartInit(void);
void SerStartTransmit(void);
uint16_t data_Avail(void);
#endif /* IMX_MULTIPROTOCOL_LPUART_H_ */
