/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#ifndef IMX_MULTIPROTOCOL_GPIO_H_
#define IMX_MULTIPROTOCOL_GPIO_H_

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

#define IMX_1020_EVK_LED_GPIO         GPIO1
#define IMX_1020_EVK_LED_GPIO_PIN     (5U)

#define SERIAL_LED_Pin                (6U)
#define SERIAL_LED_GPIO_Port          GPIO1

#define SPI_LED_Pin                   (23U)
#define SPI_LED_GPIO_Port             GPIO1

#define I2C_LED_Pin                   (15U)
#define I2C_LED_GPIO_Port             GPIO1

#define TX_LED_Pin                    (27U)
#define TX_LED_GPIO_Port              GPIO1

#define RX_LED_Pin                    (26U)
#define RX_LED_GPIO_Port              GPIO1

void GpioInit(void);
void LpSpiInitPins(void);
void LpuartInitPins(void);
void I2cInitPins(void);
void I2cSCLPushPull(void);

#endif /* IMX_MULTIPROTOCOL_GPIO_H_ */
