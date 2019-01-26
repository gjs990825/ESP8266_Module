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
    ESP8266_Config_t ESP8266_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    USART1_Init(115200);
    LED_Init();

    USART2_Init(115200);
    // TIM3_IntInit(7200 - 1, 2000 - 1);

    ESP8266_InitStructure.CWMODE = CWMODE_AP;
    ESP8266_InitStructure.CIPMUX = DISABLE;
    ESP8266_InitStructure.CIPSERVER = DISABLE;
    ESP8266_InitStructure.CIPMODE = DISABLE;
    ESP8266_InitStructure.APConfig.AP_SSID = "Test_WIFI";
    ESP8266_InitStructure.APConfig.AP_Password = "23333333";
    ESP8266_InitStructure.APConfig.ChannelN = 5;
    ESP8266_InitStructure.APConfig.Encryption = ESP8266_Ecryption_WPA2_PSK;

    ESP8266_APInit(&ESP8266_InitStructure.APConfig);
    // ESP8266_Init(&ESP8266_InitStructure);

    while (1)
    {
		// USART2_print("AT\r\n");
        // ESP8266_SendCmd("AT", "OK", 20);
        ESP8266_GetVersionNumber();
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
