#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "sys.h"
#include "usart2.h"

typedef enum
{
    CWMODE_Station = 1,
    CWMODE_AP = 2,
    CWMODE_APAndStation = 3
} ESP8266_CWMODE_t;

typedef enum
{
    ESP8266_InitFail = 0,
    ESP8266_InitOK = 1
} ESP8266_InitReturn_t;

typedef struct
{
    ESP8266_CWMODE_t CWMODE;
    FunctionalState CIPMUX;
    FunctionalState CIPSERVER;
    FunctionalState CIPMODE;
//    char *SSID;
} ESP8266_Config_t;


ESP8266_InitReturn_t ESP8266_Init(ESP8266_Config_t *Config);
uint8_t ESP8266_SendCmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime);
uint8_t *ESP8266_CheckCmd(uint8_t *cmd);
uint8_t ESP8266_Test(void);
void ESP8266_Reset(void);
void ESP8266_GetVersionNumber(void);
uint8_t ESP8266_WIFIStaTest(void);

// const uint8_t *WIFI_SSID = "Mr.Gao";
// const uint8_t *WIFI_PASSWORD = "15855440262";
// const uint8_t *WIFI_AP_SSID = "ESP8266_Test";
// const uint8_t *WIFI_AP_PASSWORD = "23333333";


#endif //_ESP8266_H_
