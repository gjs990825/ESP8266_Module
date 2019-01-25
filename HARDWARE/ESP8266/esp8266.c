#include "esp8266.h"
#include "string.h"
#include "delay.h"
#include <stdio.h>

uint8_t* ESP8266_CheckCmd(uint8_t *cmd)
{
    uint8_t *ptr = NULL;
    if (USART2_RX_STA & 0x8000)
    {
        ptr = (uint8_t *)strstr((char *) USART2_RX_BUF, (char *)cmd);
    }
    return ptr;
}


uint8_t ESP8266_SendCmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime)
{
    USART2_RX_STA = 0;
    USART2_print("%s\r\n", cmd);
    
    if (ack && waittime) 
    {
        while (waittime--)
        {
            delay_ms(10);
            if (USART2_RX_STA & 0x8000)
            {
                printf("ACK:%s\r\n", ack);
                break;
            }
        }
    }
	if (waittime == 0)
		return 0;
	else
		return 1;
}


