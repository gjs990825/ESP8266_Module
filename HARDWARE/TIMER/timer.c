#include "timer.h"
#include "led.h"
#include <stdio.h>
#include "usart2.h"

void TIM2_IntInit(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitSturcture;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitSturcture.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitSturcture.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitSturcture.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitSturcture.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitSturcture);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        //LED_Blink(0); //For debug
        
        if (USART2_RX_STA != 0) 
        {
            USART2_RX_BUF[USART2_RX_STA] = '\0';
            USART2_RX_STA |= 1 << 15;
        }
        
        TIM_Cmd(TIM2, DISABLE);
    }
}

void TIM3_IntInit(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitSturcture;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitSturcture.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitSturcture.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitSturcture.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitSturcture.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitSturcture);

    TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        if (USART2_RX_STA & (1 << 15))
        {
            printf("%s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
        }
    }
}
