#ifndef _USART2_H_
#define _USART2_H_
#include "sys.h"

#define USART2_RX_LEN 200
#define USART2_TX_LEN 200

void USART2_Init(uint32_t baud);
void USART2_SendChar(uint8_t ch);
void USART2_print(char *str, ...);

extern uint8_t USART2_RX_BUF[USART2_RX_LEN];
extern uint8_t USART2_TX_BUF[USART2_TX_LEN];
extern uint16_t USART2_RX_STA;

#endif //_USART2_H_
