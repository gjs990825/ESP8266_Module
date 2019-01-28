#include "esp8266.h"
#include "string.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t *ESP8266_CheckCmd(uint8_t *cmd)
{
    uint8_t *ptr = NULL;
    if (USART2_RX_STA & 0x8000)
    {
        ptr = (uint8_t *)strstr((char *)USART2_RX_BUF, (char *)cmd);
    }
    return ptr;
}

ErrorStatus ESP8266_SendCmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime)
{
    USART2_RX_STA = 0;
    if (cmd != NULL)
    {
        USART2_print("%s\r\n", cmd);
    }

    if (ack && waittime)
    {
        for (; waittime > 0; waittime--)
        {
            delay_ms(10);
            if (USART2_RX_STA & 0x8000)
            {
                if (ESP8266_CheckCmd(ack))
                {
                    printf("ACK:%s\r\n", USART2_RX_BUF);
                    break;
                }
                else
                {
                    USART2_RX_STA = 0;
                }
            }
        }
    }
    USART2_RX_STA = 0;
    return (ErrorStatus)(waittime != 0);
}

ErrorStatus ESP8266_Test(void)
{
    return ESP8266_SendCmd("AT", "OK", 20);
}

ErrorStatus ESP8266_Reset(void)
{
    uint8_t result = 1;
    result &= ESP8266_SendCmd("AT+RST", "OK", 30);
    delay_ms(500);
    return (ErrorStatus)result;
}

ErrorStatus ESP8266_GetVersionNumber(void)
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
            return SUCCESS;
        }
    }
    printf("ESP8266 No response\r\n");
    return ERROR;
}

ErrorStatus ESP8266_APInit(ESP8266_APConfig_t *APConfig)
{
    uint8_t result = 1;
    char *ptr = malloc(64);

    result &= ESP8266_SendCmd("AT+CWMODE=2", (uint8_t *)"OK", 30);

    sprintf(ptr, "AT+CWSAP=\"%s\",\"%s\",%d,%d",
            APConfig->AP_SSID,
            APConfig->AP_Password,
            APConfig->ChannelN,
            APConfig->Encryption);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"OK", 30);
    free(ptr);
    result &= ESP8266_Reset();

    return (ErrorStatus)result;
}

ErrorStatus ESP8266_StationInit(ESP8266_StationConfig_t *StationConfig)
{
    uint8_t result = 1;
    char *ptr = malloc(64);

    result &= ESP8266_SendCmd("AT+CWMODE=1", (uint8_t *)"OK", 20);
    sprintf(ptr, "AT+CWJAP=\"%s\",\"%s\"", StationConfig->SSID, StationConfig->Password);
    result &= ESP8266_SendCmd((uint8_t *)ptr, (uint8_t *)"WIFI CONNECTED", 300);
    free(ptr);
    result &= ESP8266_SendCmd(NULL, "WIFI GOT IP", 300);
    result &= ESP8266_SendCmd(NULL, "OK", 200);

    return (ErrorStatus)result;
}

ErrorStatus ESP8266_Init(ESP8266_Config_t *Config)
{
    uint8_t result = 1;
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
    return (ErrorStatus)result;
}
