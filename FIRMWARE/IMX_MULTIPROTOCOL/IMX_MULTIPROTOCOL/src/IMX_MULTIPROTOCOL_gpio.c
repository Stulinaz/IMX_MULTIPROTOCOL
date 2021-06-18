/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#include "IMX_MULTIPROTOCOL_gpio.h"
#include "IMX_MULTIPROTOCOL_definitions.h"

/****************************************************************************
Function:			GpioInit
Input:				none
Output:				none
PreCondition:		none
Overview:			Initialize hv gpios
****************************************************************************/
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

	#ifdef SPI_NSS_DRIVEN_MANUALLY
	led_config.direction     = kGPIO_DigitalOutput;
	led_config.interruptMode = kGPIO_NoIntmode;
	led_config.outputLogic   = SET;
	GPIO_PinInit(GPIO1, 11, &led_config);
    IOMUXC_SetPinMux   (IOMUXC_GPIO_AD_B0_11_GPIO1_IO11,  0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11, 0x70A0u);
	#endif
}

/****************************************************************************
Function:			LpSpiInitPins
Input:				none
Output:				none
PreCondition:		none
Overview:			LPSPI1 pin configuration
****************************************************************************/
void LpSpiInitPins(void)
{
	CLOCK_EnableClock(kCLOCK_Iomuxc);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_LPSPI1_SCK, 0U);
	#ifndef SPI_NSS_DRIVEN_MANUALLY
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_LPSPI1_PCS0, 0U);
	#endif
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPSPI1_SDO, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPSPI1_SDI, 0U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_10_LPSPI1_SCK, 0x10B0u);
	#ifndef SPI_NSS_DRIVEN_MANUALLY
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_11_LPSPI1_PCS0, 0x10B0u);
	#endif
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPSPI1_SDO, 0x10B0u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPSPI1_SDI, 0x10B0u);
}

/****************************************************************************
Function:			LpuartInitPins
Input:				none
Output:				none
PreCondition:		none
Overview:			LPUART2 pin configuration
****************************************************************************/
void LpuartInitPins(void)
{
	  CLOCK_EnableClock(kCLOCK_Iomuxc);
	  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_LPUART2_TX ,0U);
	  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_LPUART2_RX, 0U);
	  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_LPUART2_TX, 0x10B0u);
	  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_LPUART2_RX, 0x10B0u);
}

/****************************************************************************
Function:			I2cInitPins
Input:				none
Output:				none
PreCondition:		none
Overview:			LPI2C1 pin configuration
****************************************************************************/
void I2cInitPins(void)
{
	#ifndef I2C_INTERNAL_PULLUP_ENABLED
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_LPI2C1_SCL, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_LPI2C1_SDA, 1U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_LPI2C1_SCL, 0xD8B0u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_LPI2C1_SDA, 0xD8B0u);
	#else
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_LPI2C1_SCL, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_LPI2C1_SDA, 1U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_LPI2C1_SCL, 0x18B0u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_LPI2C1_SDA, 0x18B0u);
	#endif
}


/****************************************************************************
Function:			I2cSCLPushPull
Input:				none
Output:				none
PreCondition:		none
Overview:			Configure LPI2C1 SCL pin as push pull
****************************************************************************/
void I2cSCLPushPull(void)
{
	gpio_pin_config_t i2c_scl_config;
	i2c_scl_config.direction     = kGPIO_DigitalOutput;
	i2c_scl_config.interruptMode = kGPIO_NoIntmode;
	i2c_scl_config.outputLogic   = SET;
	GPIO_PinInit(GPIO1, 30, &i2c_scl_config);
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30,  0x70A0u);
}
