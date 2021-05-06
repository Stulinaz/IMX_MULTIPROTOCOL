/*
 * IMX_MULTIPROTOCOL_gpt.c
 *
 *  Created on: 27 apr 2021
 *      Author: Enrico
 */
#include "IMX_MULTIPROTOCOL_gpt.h"

#ifdef DEBUG_GPT
#include "IMX_MULTIPROTOCOL_gpio.h"
#endif
volatile uint32_t tick_cnt = 0;

void GptInit(void)
{
    uint32_t gptFreq;
    gpt_config_t gptConfig;

    GPT_GetDefaultConfig(&gptConfig);
    /* Initialize GPT module */
    GPT_Init(EXAMPLE_GPT, &gptConfig);
    /* Divide GPT clock source frequency by 3 inside GPT module */
    GPT_SetClockDivider(EXAMPLE_GPT, 3);
    /* Get GPT clock frequency */
    gptFreq = EXAMPLE_GPT_CLK_FREQ;
    /* GPT frequency is divided by 3 inside module */
    gptFreq /= 3000;
    /* Set both GPT modules to 1ms second duration */
    GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);
    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);
    /* Enable at the Interrupt */
    EnableIRQ(GPT_IRQ_ID);
    GPT_StartTimer(EXAMPLE_GPT);
}

uint32_t GetTick(void)
{
	return tick_cnt;
}

void Delay(uint32_t ms_wait)
{
	uint32_t ticks =  GetTick();
	while( !((GetTick() - ticks) >= ms_wait) );
}

void EXAMPLE_GPT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag);
    tick_cnt++;
	#ifdef DEBUG_GPT
    GPIO_PortToggle(IMX_1020_EVK_LED_GPIO, 1u << IMX_1020_EVK_LED_GPIO_PIN);
	#endif
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}
