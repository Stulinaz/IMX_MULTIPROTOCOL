/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/
#ifndef INC_IMX_MULTIPROTOCOL_APPDATA_H_
#define INC_IMX_MULTIPROTOCOL_APPDATA_H_

#include "IMX_MULTIPROTOCOL_command_process.h"

void Application (command_t * user_cmd, uint8_t * const user_data);
void AppToRx(void);

#endif /* INC_IMX_MULTIPROTOCOL_APPDATA_H_ */
