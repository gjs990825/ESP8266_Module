#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "sys.h"
#include "usart2.h"

typedef enum
{
    ESP8266_FAIL = 0,
    ESP8266_SUCCESS = 1
} ESP8266_ReturnSta_t;

typedef enum
{
    CWMODE_Station = 1,
    CWMODE_AP = 2,
    CWMODE_APAndStation = 3
} ESP8266_CWMODE_t;

typedef enum
{
    ESP8266_Ecryption_OPEN = 0,
    ESP8266_Ecryption_WPA_PSK = 2,
    ESP8266_Ecryption_WPA2_PSK = 3,
    ESP8266_Ecryption_WPA_WPA2_PSK = 4
} ESP8266_Ecryption_t;

typedef struct
{
    char *AP_SSID;
    char *AP_Password;
    uint8_t ChannelN;
    ESP8266_Ecryption_t Encryption;
} ESP8266_APConfig_t;

typedef struct
{
    char *SSID;
    char *Password;
} ESP8266_StationConfig_t;


typedef struct
{
    ESP8266_CWMODE_t CWMODE;
    FunctionalState CIPMUX;
    FunctionalState CIPSERVER;
    FunctionalState CIPMODE;
    ESP8266_APConfig_t APConfig;
    ESP8266_StationConfig_t StationConfig;
} ESP8266_Config_t;

ESP8266_ReturnSta_t ESP8266_Init(ESP8266_Config_t *Config);
ESP8266_ReturnSta_t ESP8266_SendCmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime);
uint8_t *ESP8266_CheckCmd(uint8_t *cmd);
ESP8266_ReturnSta_t ESP8266_Test(void);
ESP8266_ReturnSta_t ESP8266_Reset(void);
ESP8266_ReturnSta_t ESP8266_GetVersionNumber(void);
ESP8266_ReturnSta_t ESP8266_APInit(ESP8266_APConfig_t *APConfig);
ESP8266_ReturnSta_t ESP8266_WIFIStaTest(void);

#endif //_ESP8266_H_
