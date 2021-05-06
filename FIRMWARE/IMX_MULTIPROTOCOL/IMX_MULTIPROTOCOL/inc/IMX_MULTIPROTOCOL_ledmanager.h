/*
 * IMX_MULTIPROTOCOL_ledmanager.h
 *
 *  Created on: 6 mag 2021
 *      Author: Enrico
 */

#ifndef INC_IMX_MULTIPROTOCOL_LEDMANAGER_H_
#define INC_IMX_MULTIPROTOCOL_LEDMANAGER_H_

#include <stdint.h>

#define TX_LED_ON_TIME       (uint32_t) 180
#define RX_LED_ON_TIME       (uint32_t) 180
#define LED_ERROR_BLINK_TIME (uint32_t) 250
#define LED_ERROR_DEAD_TIME  (uint32_t) 1500

void LedManager(void);
void ErrorCodeSet(uint8_t err_num);
#endif /* INC_IMX_MULTIPROTOCOL_LEDMANAGER_H_ */
