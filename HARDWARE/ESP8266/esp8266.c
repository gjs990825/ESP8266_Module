#include "esp8266.h"
#include "string.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>

const char *WIFI_SSID = "Mr.Gao";
const char *WIFI_PASSWORD = "15855440262";
const char *WIFI_AP_SSID = "ESP8266_Test";
const char *WIFI_AP_PASSWORD = "23333333";

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

ESP8266_ReturnSta_t ESP8266_SendCmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime)
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
                if (ESP8266_CheckCmd(ack))
                {
                    printf("ACK:%s\r\n", USART2_RX_BUF);
                    break;
                }
            }
        }
    }
    USART2_RX_STA = 0;
    return ((waittime == 0) ? ESP8266_FAIL : ESP8266_SUCCESS);
}

ESP8266_ReturnSta_t ESP8266_Test(void)
{
    return ESP8266_SendCmd("AT", "OK", 20);
}

ESP8266_ReturnSta_t ESP8266_Reset(void)
{
    ESP8266_ReturnSta_t result = ESP8266_SendCmd("AT+RST", "OK", 20);
    delay_ms(1000);
    delay_ms(1000);
    USART2_RX_STA = 0;
    return result;
}

ESP8266_ReturnSta_t ESP8266_GetVersionNumber(void)
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
            return ESP8266_SUCCESS;
        }
    }
    printf("ESP8266 No response\r\n");
    return ESP8266_FAIL;
}

ESP8266_ReturnSta_t ESP8266_APInit(ESP8266_APConfig_t *APConfig)
{
    uint8_t result = 0;
    char *ptr = malloc(64);

    sprintf(ptr, "AT+CWSAP=\"%s\",\"%s\",%d,%d",
            APConfig->AP_SSID,
            APConfig->AP_Password,
            APConfig->ChannelN,
            APConfig->Encryption);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);
    free(ptr);
    result &= ESP8266_Reset();

    return result ? ESP8266_SUCCESS : ESP8266_FAIL;
}

ESP8266_ReturnSta_t ESP8266_StationInit(ESP8266_StationConfig_t *StationConfig)
{
    ESP8266_ReturnSta_t result = 0;
    char *ptr = malloc(64);

    sprintf(ptr, "AT+CWJAP=\"%s\",\"%s\"", StationConfig->SSID, StationConfig->Password);
    result = ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"WIFI GOT IP", 300);
    free(ptr);

    return result;
}

ESP8266_ReturnSta_t ESP8266_Init(ESP8266_Config_t *Config)
{
    uint8_t result = 0;
    char *ptr = malloc(18);

    sprintf(ptr, "AT+CWMODE=%d", Config->CWMODE);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);
    sprintf(ptr, "AT+CIPMUX=%d", Config->CIPMUX);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);
    if (Config->CIPMUX == ENABLE)
    {
        sprintf(ptr, "AT+CIPSERVER=%d", Config->CIPSERVER);
        result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);
    }
    sprintf(ptr, "AT+CIPMODE=%d", Config->CIPMODE);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 20);

    result &= ESP8266_Reset();

    free(ptr);
    return result ? ESP8266_SUCCESS : ESP8266_FAIL;
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
