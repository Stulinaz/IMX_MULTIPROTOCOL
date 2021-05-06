/*
 * IMX_MULTIPROTOCOL_gpt.h
 *
 *  Created on: 27 apr 2021
 *      Author: Enrico
 */

#ifndef IMX_MULTIPROTOCOL_GPT_H_
#define IMX_MULTIPROTOCOL_GPT_H_

#include "fsl_gpt.h"

/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ   CLOCK_GetFreq(kCLOCK_PerClk)
#define GPT_IRQ_ID             GPT2_IRQn
#define EXAMPLE_GPT            GPT2
#define EXAMPLE_GPT_IRQHandler GPT2_IRQHandler

void GptInit(void);
uint32_t GetTick(void);
void Delay(uint32_t ms_wait);

#endif /* IMX_MULTIPROTOCOL_GPT_H_ */
