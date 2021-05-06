/*
 * IMX_MULTIPROTOCOL_definitions.h
 *
 *  Created on: 6 mag 2021
 *      Author: Enrico
 */

#ifndef INC_IMX_MULTIPROTOCOL_DEFINITIONS_H_
#define INC_IMX_MULTIPROTOCOL_DEFINITIONS_H_

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
