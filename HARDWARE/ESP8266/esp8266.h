#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "sys.h"
#include "usart2.h"

uint8_t ESP8266_SendCmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime);
uint8_t* ESP8266_CheckCmd(uint8_t *cmd);

#endif //_ESP8266_H_

