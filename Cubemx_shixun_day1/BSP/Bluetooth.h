#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "main.h"

typedef struct{
	uint8_t buff[20];//���ڴ洢���յ�������
	uint8_t length;//����ȷ���������ݵĳ���
	uint8_t recvflag;//������ɱ�־
	
}BLE;

extern BLE ble;
void USART5_Init(void);
void USART2_Init(void);
void Ble_Anylze(void);

#endif
