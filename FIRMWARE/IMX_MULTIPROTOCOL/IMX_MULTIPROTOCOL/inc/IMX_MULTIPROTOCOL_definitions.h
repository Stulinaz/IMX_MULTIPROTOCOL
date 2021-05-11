/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/
#ifndef INC_IMX_MULTIPROTOCOL_DEFINITIONS_H_
#define INC_IMX_MULTIPROTOCOL_DEFINITIONS_H_

#define NUL                   0x00
#define TAB         (uint8_t) 0x09
#define CR_         (uint8_t) 0x0D
#define LF_         (uint8_t) 0x0A
#define END_CHAR    '\0'

#define SET   1U
#define RESET 0U

#define PASS 1U
#define FAIL 0U

typedef enum{
FALSE = 0,
TRUE = 1
}_bool;

#endif /* INC_IMX_MULTIPROTOCOL_DEFINITIONS_H_ */
