/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#include "IMX_MULTIPROTOCOL_gpio.h"
#include "IMX_MULTIPROTOCOL_definitions.h"

void GpioInit(void)
{
	gpio_pin_config_t led_config;

	led_config.direction     = kGPIO_DigitalOutput;
	led_config.interruptMode = kGPIO_NoIntmode;
	led_config.outputLogic   = SET;
	GPIO_PinInit(IMX_1020_EVK_LED_GPIO, IMX_1020_EVK_LED_GPIO_PIN, &led_config);
	IOMUXC_SetPinMux   (IOMUXC_GPIO_AD_B0_05_GPIO1_IO05, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_05_GPIO1_IO05, 0x70A0u);

	led_config.direction     = kGPIO_DigitalOutput;
	led_config.interruptMode = kGPIO_NoIntmode;
	led_config.outputLogic   = RESET;
	GPIO_PinInit(RX_LED_GPIO_Port, RX_LED_Pin , &led_config);
	IOMUXC_SetPinMux   (IOMUXC_GPIO_AD_B1_10_GPIO1_IO26, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26, 0x70A0u);

	led_config.direction     = kGPIO_DigitalOutput;
	led_config.interruptMode = kGPIO_NoIntmode;
	led_config.outputLogic   = RESET;
	GPIO_PinInit(TX_LED_GPIO_Port, TX_LED_Pin, &led_config);
	IOMUXC_SetPinMux   (IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0x70A0u);

	led_config.direction     = kGPIO_DigitalOutput;
	led_config.interruptMode = kGPIO_NoIntmode;
	led_config.outputLogic   = RESET;
	GPIO_PinInit(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin, &led_config);
	IOMUXC_SetPinMux   (IOMUXC_GPIO_AD_B0_06_GPIO1_IO06, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_06_GPIO1_IO06, 0x70A0u);

	led_config.direction     = kGPIO_DigitalOutput;
	led_config.interruptMode = kGPIO_NoIntmode;
	led_config.outputLogic   = RESET;
	GPIO_PinInit(SPI_LED_GPIO_Port, SPI_LED_Pin, &led_config);
	IOMUXC_SetPinMux   (IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0x70A0u);

	led_config.direction     = kGPIO_DigitalOutput;
	led_config.interruptMode = kGPIO_NoIntmode;
	led_config.outputLogic   = RESET;
	GPIO_PinInit(I2C_LED_GPIO_Port, I2C_LED_Pin, &led_config);
	IOMUXC_SetPinMux   (IOMUXC_GPIO_AD_B0_15_GPIO1_IO15, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_15_GPIO1_IO15, 0x70A0u);
}
