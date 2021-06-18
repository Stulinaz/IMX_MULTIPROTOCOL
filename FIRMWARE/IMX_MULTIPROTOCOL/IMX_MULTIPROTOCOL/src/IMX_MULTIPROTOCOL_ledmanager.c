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
static void CommunicationLedSetTime(void);

static led_error_manager display_error = {0, 0, 0, ERR_NO_ERROR};
static comm_inerface_t visualize_transfer = USB_INTERFACE;
static uint32_t tx_led_countdown = 0;
static uint32_t rx_led_countdown = 0;

/****************************************************************************
Function:			LedManager
Input:				none
Output:				none
PreCondition:		Timer reference os systick
Overview:			Time reference should tick every 1ms
****************************************************************************/
void LedManager(void)
{
	LedErrorManager();
}

/****************************************************************************
Function:			LedInterfaceSel
Input:				none
Output:				none
PreCondition:		none
Overview:			visual activation of woring interface (SPI - UART - I2C)
****************************************************************************/
void LedInterfaceSel(command_t cmd)
{
	switch(cmd)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
		{
			/* Serial interface selected - turn on serial led pin  - off others*/
			visualize_transfer = SER_INTERFACE;
			GPIO_PinWrite(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin , SET);
			GPIO_PinWrite(I2C_LED_GPIO_Port   , I2C_LED_Pin    , RESET);
			GPIO_PinWrite(SPI_LED_GPIO_Port   , SPI_LED_Pin    , RESET);
			break;
		}
		case USER_I2C_INTERFACE_SELECTED:
		{
			/* I2C interface selected - turn on I2C led pin - off others*/
			visualize_transfer = I2C_INTERFACE;
			GPIO_PinWrite(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin , RESET);
			GPIO_PinWrite(I2C_LED_GPIO_Port   , I2C_LED_Pin    , SET);
			GPIO_PinWrite(SPI_LED_GPIO_Port   , SPI_LED_Pin    , RESET);
			break;
		}
		case USER_SPI_INTERFACE_SELECTED:
		{
			/* SPI interface selected - turn on SPI led pin  - off others*/
			visualize_transfer = SPI_INTERFACE;
			GPIO_PinWrite(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin , RESET);
			GPIO_PinWrite(I2C_LED_GPIO_Port   , I2C_LED_Pin    , RESET);
			GPIO_PinWrite(SPI_LED_GPIO_Port   , SPI_LED_Pin    , SET);
			break;
		}
		default:
			break;
	}
}

/****************************************************************************
Function:			CommunicationLedSetTime
Input:				none
Output:				none
PreCondition:		none
Overview:			Visualize on-goin transfer on selected interface (SPI - UART - I2C)
****************************************************************************/
static void CommunicationLedSetTime(void)
{
	switch(visualize_transfer)
	{
		case SER_INTERFACE:
		{
			if (data_avail(visualize_transfer) != 0)
				tx_led_countdown = TX_LED_ON_TIME;
			if (data_toprocess(visualize_transfer) != 0)
				tx_led_countdown = RX_LED_ON_TIME;
		}
		case I2C_INTERFACE:
		case SPI_INTERFACE:
		break;
		default:
			break;
	}
}

/****************************************************************************
Function:			CommunicationLedVisualize
Input:				none
Output:				none
PreCondition:		must be call in gpt interrupt
Overview:			Visualize on-goin transfer on selected interface (SPI - UART - I2C)
****************************************************************************/
void CommunicationLedVisualize(void)
{
	CommunicationLedSetTime();
	if ( (tx_led_countdown == 0) && (tx_led_countdown == 0) )
	{
		GPIO_PinWrite(TX_LED_GPIO_Port, TX_LED_Pin, RESET);
		GPIO_PinWrite(RX_LED_GPIO_Port, RX_LED_Pin, RESET);
		return;
	}

	if(tx_led_countdown > 0)
	{
		tx_led_countdown--;
		GPIO_PinWrite(TX_LED_GPIO_Port, TX_LED_Pin, SET);
	}
	else
		GPIO_PinWrite(TX_LED_GPIO_Port, TX_LED_Pin, RESET);

	if(rx_led_countdown > 0)
	{
		GPIO_PinWrite(RX_LED_GPIO_Port, RX_LED_Pin, SET);
		rx_led_countdown--;
	}
	else
		GPIO_PinWrite(RX_LED_GPIO_Port, RX_LED_Pin, RESET);
}

/****************************************************************************
Function:			ErrorCodeSet
Input:				(uint8_t) Number of blinks to be displayed on EVK's LED Board
Output:				none
PreCondition:		none
Overview:			Debug purpose
****************************************************************************/
void ErrorCodeSet(uint8_t err_num)
{
	display_error.err_num = err_num;
}

/****************************************************************************
Function:			ErrorCodeSet
Input:				(uint8_t) Number of blinks to be displayed on EVK's LED Board
Output:				none
PreCondition:		Timer reference os systick
Overview:			Time reference should tick every 1ms
****************************************************************************/
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
