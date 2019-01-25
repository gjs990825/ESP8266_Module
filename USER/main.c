/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "timer.h"
#include "esp8266.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    USART1_Init(115200);
    LED_Init();

    USART2_Init(115200);
    // TIM3_IntInit(7200 - 1, 2000 - 1);

    while (1)
    {
		// USART2_print("AT\r\n");
        ESP8266_SendCmd("AT", "OK", 20);
        LED_Blink(0);
        delay_ms(500);
    }
    return 0;
}

void assert_failed(uint8_t *file, uint32_t line)
{
    while (1)
    {
        printf("File: %s, line: %d\r\n", file, line);
        delay_ms(500);
    }
}
