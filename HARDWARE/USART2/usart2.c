#include "usart2.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "timer.h"

uint8_t USART2_RX_BUF[USART2_RX_LEN] = {0};
uint8_t USART2_TX_BUF[USART2_TX_LEN] = {0};
uint16_t USART2_RX_STA = 0;

void USART2_Init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    TIM2_IntInit(1000 - 1, 7200 - 1);
    TIM_Cmd(TIM2, DISABLE);

    USART2_RX_STA = 0;

    USART_Cmd(USART2, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void USART2_SendChar(uint8_t ch)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, ch);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

void USART2_print(char *str, ...)
{
    uint16_t len;
    va_list ap;
    va_start(ap, str);
    vsprintf((char *)USART2_TX_BUF, str, ap);
    va_end(ap);
    len = strlen((char *)USART2_TX_BUF);

    for (uint16_t i = 0; i < len; i++)
    {
        USART2_SendChar(USART2_TX_BUF[i]);
    }
}

void USART2_IRQHandler(void)
{
    uint8_t ch;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        ch = USART_ReceiveData(USART2);

        if ((USART2_RX_STA & (1 << 15)) == 0)
        {
            if (USART2_RX_STA < USART2_RX_LEN - 1)
            {
                TIM_SetCounter(TIM2, 0);
                if (USART2_RX_STA == 0)
                {
                    TIM_Cmd(TIM2, ENABLE);
                }
                USART2_RX_BUF[USART2_RX_STA++] = ch;
            }
            else
            {
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
                TIM_Cmd(TIM2, DISABLE);
                USART2_RX_BUF[USART2_RX_STA] = '\0';
                USART2_RX_STA |= 1 << 15;
            }
        }
    }
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}
