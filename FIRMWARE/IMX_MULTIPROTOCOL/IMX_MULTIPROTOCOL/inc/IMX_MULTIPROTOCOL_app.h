/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#ifndef INC_IMX_MULTIPROTOCOL_APP_H_
#define INC_IMX_MULTIPROTOCOL_APP_H_

#include <stdint.h>

#define INACTIVITY_TIMEOUT (uint32_t) 10000
#define MAN_TIMEOUT        (uint32_t) 50

typedef enum
{
NO_DATA_TO_PROCESS = 0u,
CHECK_FOR_INACTIVITY,
WAIT_FOR_DATA,
DECODE_MESSAGE,
RESTART
}command_manager_s;

void DecodeFromPc(void);
void TransferToPc(void);

#endif /* INC_IMX_MULTIPROTOCOL_APP_H_ */
