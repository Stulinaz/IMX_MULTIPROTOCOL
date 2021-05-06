/*
 * IMX_MULTIPROTOCOL_gpio.h
 *
 *  Created on: 27 apr 2021
 *      Author: Enrico
 */

#ifndef IMX_MULTIPROTOCOL_GPIO_H_
#define IMX_MULTIPROTOCOL_GPIO_H_

#include "fsl_gpio.h"

#define IMX_1020_EVK_LED_GPIO     GPIO1
#define IMX_1020_EVK_LED_GPIO_PIN (5U)
#define SERIAL_LED_Pin                (6U)
#define SERIAL_LED_GPIO_Port          GPIO1
#define SPI_LED_Pin                   (3U)
#define SPI_LED_GPIO_Port             GPIO1
#define I2C_LED_Pin                   (4U)
#define I2C_LED_GPIO_Port             GPIO1
#define TX_LED_Pin                    (7U)
#define TX_LED_GPIO_Port              GPIO1
#define RX_LED_Pin                    (8U)
#define RX_LED_GPIO_Port              GPIO1

void GpioInit(void);
#endif /* IMX_MULTIPROTOCOL_GPIO_H_ */
