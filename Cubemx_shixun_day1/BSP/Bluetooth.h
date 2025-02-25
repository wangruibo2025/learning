#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "main.h"

typedef struct{
	uint8_t buff[20];//用于存储接收到的数据
	uint8_t length;//用于确定接受数据的长度
	uint8_t recvflag;//接收完成标志
	
}BLE;

extern BLE ble;
void USART5_Init(void);
void USART2_Init(void);
void Ble_Anylze(void);

#endif
