#ifndef __WIFI_H
#define __WIFI_H

#include "main.h"

typedef struct{
	uint8_t buff[20];//���ڴ洢���յ�������
	uint8_t length;//����ȷ���������ݵĳ���
	uint8_t recvflag;//������ɱ�־
	
}WIFI;

void WIFI_Init(void);
void WIFI_Anylze(void);

extern WIFI wifi;

#endif
