/*
 * IMX_MULTIPROTOCOL_ledmanager.c
 *
 *  Created on: 6 mag 2021
 *      Author: Enrico
 */

#include "IMX_MULTIPROTOCOL_ledmanager.h"
#include "IMX_MULTIPROTOCOL_gpio.h"
#include "IMX_MULTIPROTOCOL_gpt.h"
#include "IMX_MULTIPROTOCOL_definitions.h"

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

void LedManager(void)
{
	LedErrorManager();
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
