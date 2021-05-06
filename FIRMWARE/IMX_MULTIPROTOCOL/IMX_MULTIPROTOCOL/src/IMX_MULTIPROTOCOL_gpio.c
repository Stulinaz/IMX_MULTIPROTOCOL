/*
 * IMX_MULTIPROTOCOL_gpio.c
 *
 *  Created on: 27 apr 2021
 *      Author: Enrico
 */

#include "IMX_MULTIPROTOCOL_gpio.h"

void GpioInit(void)
{
	gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

	GPIO_PinInit(IMX_1020_EVK_LED_GPIO, IMX_1020_EVK_LED_GPIO_PIN, &led_config);
//	GPIO_PinInit(SERIAL_LED_Pin, SERIAL_LED_GPIO_Port, &led_config);
//	GPIO_PinInit(SPI_LED_Pin, SPI_LED_GPIO_Port, &led_config);
//	GPIO_PinInit(TX_LED_Pin, TX_LED_GPIO_Port , &led_config);
//	GPIO_PinInit(RX_LED_Pin, RX_LED_GPIO_Port, &led_config);
}
