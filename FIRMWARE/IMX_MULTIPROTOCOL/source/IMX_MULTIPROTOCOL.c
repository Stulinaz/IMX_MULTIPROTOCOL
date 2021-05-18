/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdlib.h>
/*${standard_header_anchor}*/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "IMX_MULTIPROTOCOL_usb.h"
#include "IMX_MULTIPROTOCOL_gpio.h"
#include "IMX_MULTIPROTOCOL_gpt.h"
#include "IMX_MULTIPROTOCOL_ledmanager.h"
#include "IMX_MULTIPROTOCOL_lpuart.h"
#include "IMX_MULTIPROTOCOL_buffers_manager.h"
#include "IMX_MULTIPROTOCOL_app.h"
#include "IMX_MULTIPROTOCOL_lpspi.h"

int main(void)
{
	//uint32_t x ;
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    GpioInit();
    GptInit();
    LpuartInit();
    USBInit();
//    __NVIC_SetPriority(USB_OTG1_IRQn, 1);
   // __NVIC_SetPriority(GPT2_IRQn, 5);
    //__NVIC_SetPriority(LPUART2_IRQn, 4);
//    x = __NVIC_GetPriority( USB_OTG1_IRQn);
//    x = __NVIC_GetPriority( GPT2_IRQn);
//    x = __NVIC_GetPriority( LPUART2_IRQn);
    while (1)
    {
//    	putbyte(SER_INTERFACE, 0x55);
//    	putbyte(SER_INTERFACE, 0x55);
//    	putbyte(SER_INTERFACE, 0x55);
//    	SerStartTransmit();
//    	Delay(100);

    	//SerialTransfer();
//    	GPIO_PinWrite(GPIO1, 11 , SET);
//    	while(1)
//    	{
//    	GPIO_PinWrite(GPIO1, 11 , RESET);
//    	Delay(100);
//    	GPIO_PinWrite(GPIO1, 11 , SET);
//    	Delay(100);
//    	}
    	LedManager();
    	Application();
    	//UsbVcpTask();
    }
}

