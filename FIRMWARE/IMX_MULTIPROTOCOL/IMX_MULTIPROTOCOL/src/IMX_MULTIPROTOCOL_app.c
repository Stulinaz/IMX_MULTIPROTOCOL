/*
 _   _   _   ___ _  __   _   _____ _   _  ___  _   _
| |_| | /_\ / __| |/ /  /_\ |_   _| |_| |/ _ \| \ | |
|  _  |/ _ \ (__| ' <  / _ \  | | |  _  | (_) |  \| |
|_| |_/_/ \_\___|_|\_\/_/ \_\_|_| |_| |_|\___/|_|\__|
IMX RT MCU Embedded contest 2021
*/
#include "IMX_MULTIPROTOCOL_app.h"
#include "IMX_MULTIPROTOCOL_command_process.h"
#include "IMX_MULTIPROTOCOL_gpt.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"
#include "IMX_MULTIPROTOCOL_appdata.h"

typedef struct{
command_manager_s cmd_manager_states;
uint8_t           connection_status;
uint32_t          time_app;
uint32_t          inactivity_time;
uint8_t           cmd_len;
command_t         user_cmd;
}interface_t;

const char *ready = "Ready";

interface_t user_interface = { NO_DATA_TO_PROCESS, DISCONNECTED, 0, 0, 0, NO_COMMAND};

void DecodeFromPc (void)
{
	/* Compatible with USB_INTERFACE and SER_INTERFACE*/
	switch(user_interface.cmd_manager_states)
	{
		case NO_DATA_TO_PROCESS:
		{
			if(user_interface.inactivity_time == 0)
				user_interface.inactivity_time = GetTick();
			if (data_avail(USB_INTERFACE))
			{
				user_interface.time_app = GetTick();
				user_interface.connection_status  = CONNECTED;
				user_interface.cmd_manager_states = WAIT_FOR_DATA;
			}
			else
				user_interface.cmd_manager_states = CHECK_FOR_INACTIVITY;
			break;
		}

		case CHECK_FOR_INACTIVITY:
		{
			if(user_interface.connection_status ==  CONNECTED)
			{
				if ((GetTick() - user_interface.inactivity_time >= INACTIVITY_TIMEOUT))
				{
					/* Inactivity timeout elapsed - send data over usb*/
					user_interface.inactivity_time = 0;
					UsbPrintString(ready, TRUE);
				}
			}
			user_interface.cmd_manager_states = NO_DATA_TO_PROCESS;
			break;
		}

		case WAIT_FOR_DATA:
		{
			if ((GetTick() - user_interface.time_app) >= MAN_TIMEOUT)
				user_interface.cmd_manager_states = RESTART;
			else
			{
				if (data_avail(USB_INTERFACE))
					user_interface.cmd_manager_states = DECODE_MESSAGE;
				else
					user_interface.cmd_manager_states = RESTART;
			}
			break;
		}

		case DECODE_MESSAGE:
		{
			uint8_t conf;
			user_interface.user_cmd = Decode(data_avail(USB_INTERFACE) , &conf);
			Application(&user_interface.user_cmd, &conf);
			user_interface.cmd_manager_states = RESTART;
			break;
		}

		case RESTART:
		{
			clear_buff(USB_INTERFACE);
			user_interface.inactivity_time = 0;
			user_interface.cmd_manager_states = NO_DATA_TO_PROCESS;
			break;
		}
	}
}
