#ifndef __WIFI_H
#define __WIFI_H

#include "main.h"

typedef struct{
	uint8_t buff[20];//用于存储接收到的数据
	uint8_t length;//用于确定接受数据的长度
	uint8_t recvflag;//接收完成标志
	
}WIFI;

void WIFI_Init(void);
void WIFI_Anylze(void);

extern WIFI wifi;

#endif
