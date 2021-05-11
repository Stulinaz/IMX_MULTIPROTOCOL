/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#ifndef INC_IMX_MULTIPROTOCOL_LEDMANAGER_H_
#define INC_IMX_MULTIPROTOCOL_LEDMANAGER_H_

#include <stdint.h>
#include "IMX_MULTIPROTOCOL_command_process.h"

#define TX_LED_ON_TIME       (uint32_t) 180
#define RX_LED_ON_TIME       (uint32_t) 180
#define LED_ERROR_BLINK_TIME (uint32_t) 250
#define LED_ERROR_DEAD_TIME  (uint32_t) 1500

void LedManager(void);
void ErrorCodeSet(uint8_t err_num);
void LedInterfaceSel(command_t cmd);

#endif /* INC_IMX_MULTIPROTOCOL_LEDMANAGER_H_ */
