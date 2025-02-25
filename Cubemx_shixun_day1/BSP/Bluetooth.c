#include "Bluetooth.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "motor.h"
#include "steer.h"


extern uint8_t steer_dir;//0ֹͣ��1̧ͷ��2 ��ͷ��3��ת��4��ת
extern int16_t lspeed1;
extern int16_t rspeed1;

BLE ble = {0};

void USART5_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart5,UART_IT_RXNE);//�������ڵĽ����ж�
	__HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);//�������ڵĿ����ж�,�����к���ͨ���ݴ��ڷ����г�ͻ
}

void USART2_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);//�������ڵĽ����ж�
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);//�������ڵĿ����ж�,�����к���ͨ���ݴ��ڷ����г�ͻ
}

void Ble_Anylze(void)
{
	if(ble.recvflag != 1)
		return ;
	if(ble.buff[0] != 0xAA || ble.buff[3] != 0x55)
	{
		printf("֡ͷ֡β����\r\n");
		memset(&ble,0,sizeof(ble));
		return ;
	}
	switch(ble.buff[1])
	{
		case 0x01://������
			if(ble.buff[2])
			{
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
			}else{
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
			}
			break;
		case 0x02://�ƹ�
			if(ble.buff[2])
			{
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
			}else{
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
			}
			break;
		case 0x03://С������δ����
			switch(ble.buff[2])
			{
				case 0x01:
					lspeed1 = 0;
					rspeed1 = 0;
					break;
				case 0x02://ǰ��
					lspeed1 = 600;
					rspeed1 = 600;
					break;
				case 0x03://����
					lspeed1 = -600;
					rspeed1 = -600;
					break;
				case 0x04://��ת
					lspeed1 = 600;
					rspeed1 = -600;
					break;
				case 0x05://��ת
					lspeed1 = -600;
					rspeed1 = 600;
					break;
			}
			break;
		case 0x04://��̨
			switch(ble.buff[2])
			{
				case 0x01:
					steer_dir = 0;//0ֹͣ��1��ת��2 ��ת��3�½���4����
					break;
				case 0x02:
					steer_dir = 1;
					break;
				case 0x03:
					steer_dir = 2;
					break;
				case 0x04:
					steer_dir = 3;
					break;
				case 0x05:
					steer_dir = 4;
					break;
			}
			break;
	}

	memset(&ble,0,sizeof(ble));
}



