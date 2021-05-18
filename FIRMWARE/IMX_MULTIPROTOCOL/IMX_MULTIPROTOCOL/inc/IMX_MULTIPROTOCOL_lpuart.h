/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/
#ifndef IMX_MULTIPROTOCOL_LPUART_H_
#define IMX_MULTIPROTOCOL_LPUART_H_

#include "fsl_lpuart.h"

#define DEMO_LPUART            LPUART2
#define DEMO_LPUART_CLK_FREQ   BOARD_DebugConsoleSrcFreq()
#define DEMO_LPUART_IRQn       LPUART2_IRQn
#define DEMO_LPUART_IRQHandler LPUART2_IRQHandler
#define BAUDRATE_DEFAULT       (uint32_t) 9600

typedef enum{
BAUDRATE_1200 = 0u,
BAUDRATE_4800,
BAUDRATE_9600,
BAUDRATE_19200,
BAUDRATE_38400,
BAUDRATE_57600,
BAUDRATE_115200
}ser_baudrate_t;

void LpuartInit(void);
void LpuartStop(void);
void SerStartTransmit(void);
uint8_t SerBaudrateSel(ser_baudrate_t baudrate);

#endif /* IMX_MULTIPROTOCOL_LPUART_H_ */
