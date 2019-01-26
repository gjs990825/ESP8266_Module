#include "esp8266.h"
#include "string.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>

const uint8_t *WIFI_SSID = "Mr.Gao";
const uint8_t *WIFI_PASSWORD = "15855440262";
const uint8_t *WIFI_AP_SSID = "ESP8266_Test";
const uint8_t *WIFI_AP_PASSWORD = "23333333";

uint8_t Network_Mode = 0;

uint8_t *ESP8266_CheckCmd(uint8_t *cmd)
{
    uint8_t *ptr = NULL;
    if (USART2_RX_STA & 0x8000)
    {
        ptr = (uint8_t *)strstr((char *)USART2_RX_BUF, (char *)cmd);
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

uint8_t ESP8266_Test(void)
{
    return ESP8266_SendCmd("AT", "OK", 20);
}

void ESP8266_Reset(void)
{
    ESP8266_SendCmd("AT+RST", "OK", 20);
}

void ESP8266_GetVersionNumber(void)
{
    uint8_t waitT = 20;
    USART2_print("AT+GMR\r\n");
    while (waitT--)
    {
        delay_ms(10);
        if (USART2_RX_STA & 0x8000)
        {
            printf("ESP8266:%s\r\n", USART2_RX_BUF);
            USART2_RX_STA = 0;
            return;
        }
    }
    printf("ESP8266 No response\r\n");
}

//uint8_t ESP8266_WIFIStaTest(void)
//{
//    uint8_t *ptr = malloc(32);

//    ESP8266_SendCmd("AT+CWMODE=1", "OK", 50);
//    ESP8266_Reset();
//    delay_ms(1500);
//    delay_ms(1500);
//    sprintf((char *)ptr, "AT+CWJAP\"%s\",\"%s\"", WIFI_SSID, WIFI_PASSWORD);
//    while (ESP8266_SendCmd(ptr, "WIFI GOT IP", 300))
//        ;
//    switch (Network_Mode)
//    {
//    case 0x01:
//        printf("UDP MODE\r\n");

//        break;

//    default:
//        break;
//    }
//    free(ptr);
//}

ESP8266_InitReturn_t ESP8266_Init(ESP8266_Config_t *Config)
{
    uint8_t result = 0;
    char *ptr = malloc(24);

    sprintf(ptr, "AT+CWMODE=%d", Config->CWMODE);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);
    sprintf(ptr, "AT+CIPMUX=%d", Config->CIPMUX);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);
    sprintf(ptr, "AT+CIPSERVER=%d", Config->CIPSERVER);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);
    sprintf(ptr, "AT+CIPMODE=%d", Config->CIPMODE);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);

    free(ptr);
    return result ? ESP8266_InitOK : ESP8266_InitFail;
}
