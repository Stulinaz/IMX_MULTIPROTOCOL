/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/

#include "IMX_MULTIPROTOCOL_ledmanager.h"
#include "IMX_MULTIPROTOCOL_gpio.h"
#include "IMX_MULTIPROTOCOL_gpt.h"
#include "IMX_MULTIPROTOCOL_definitions.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"

typedef enum
{
ERR_NO_ERROR = 0u,
ERR_STATUS_ON,
ERR_STATUS_OFF
}led_error_states;

typedef struct{
	uint32_t led_time;
	uint8_t  err_num;
	uint8_t  curr_cnt;
	led_error_states led_stat;
}led_error_manager;

static void LedErrorManager(void);

static led_error_manager display_error = {0, 0, 0, ERR_NO_ERROR};
static comm_inerface_t visualize_trasnfer = USB_INTERFACE;

void LedManager(void)
{
	LedErrorManager();
}

void LedInterfaceSel(command_t cmd)
{
	switch(cmd)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
		{
			visualize_trasnfer = SER_INTERFACE;
//			GPIO_PinWrite(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin , SET);
//			GPIO_PinWrite(I2C_LED_GPIO_Port   , I2C_LED_Pin    , RESET);
//			GPIO_PinWrite(SPI_LED_GPIO_Port   , SPI_LED_Pin    , RESET);
			break;
		}
		case USER_I2C_INTERFACE_SELECTED:
		{
			visualize_trasnfer = I2C_INTERFACE;
//			GPIO_PinWrite(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin , RESET);
//			GPIO_PinWrite(I2C_LED_GPIO_Port   , I2C_LED_Pin    , SET);
//			GPIO_PinWrite(SPI_LED_GPIO_Port   , SPI_LED_Pin    , RESET);
			break;
		}
		case USER_SPI_INTERFACE_SELECTED:
		{
			visualize_trasnfer = SPI_INTERFACE;
//			GPIO_PinWrite(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin , RESET);
//			GPIO_PinWrite(I2C_LED_GPIO_Port   , I2C_LED_Pin    , RESET);
//			GPIO_PinWrite(SPI_LED_GPIO_Port   , SPI_LED_Pin    , SET);
			break;
		}
		default:
			break;
	}
}

void ErrorCodeSet(uint8_t err_num)
{
	display_error.err_num = err_num;
}

static void LedErrorManager(void)
{
	switch(display_error.led_stat)
	{
		case ERR_NO_ERROR:
		{
			if(display_error.curr_cnt != 0)
			{
				GPIO_PinWrite(IMX_1020_EVK_LED_GPIO, IMX_1020_EVK_LED_GPIO_PIN, RESET);
				display_error.led_time = GetTick();
				display_error.led_stat = ERR_STATUS_ON;
			}
			else
			{
				if(display_error.err_num != 0)
					display_error.curr_cnt = display_error.err_num;
			}
			break;
		}

		case ERR_STATUS_ON:
		{
			if ((GetTick() - display_error.led_time) >= LED_ERROR_BLINK_TIME)
			{
				display_error.led_time = GetTick();
				GPIO_PinWrite(IMX_1020_EVK_LED_GPIO, IMX_1020_EVK_LED_GPIO_PIN, SET);
				display_error.curr_cnt --;
				display_error.led_stat = ERR_STATUS_OFF;
			}
			break;
		}

		case ERR_STATUS_OFF:
		{
			if(display_error.curr_cnt == 0)
			{
				if ((GetTick() - display_error.led_time) >= LED_ERROR_DEAD_TIME)
					display_error.led_stat = ERR_NO_ERROR;
			}
			else
			{
				if ((GetTick() - display_error.led_time) >= LED_ERROR_BLINK_TIME)
				{
					display_error.led_time = GetTick();
					GPIO_PinWrite(IMX_1020_EVK_LED_GPIO, IMX_1020_EVK_LED_GPIO_PIN, RESET);
					display_error.led_stat = ERR_STATUS_ON;
				}
			}
			break;
		}
	}
}
