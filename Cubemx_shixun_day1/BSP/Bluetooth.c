#include "Bluetooth.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "motor.h"
#include "steer.h"


extern uint8_t steer_dir;//0停止，1抬头，2 低头，3左转，4右转
extern int16_t lspeed1;
extern int16_t rspeed1;

BLE ble = {0};

void USART5_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart5,UART_IT_RXNE);//开启串口的接收中断
	__HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);//开启串口的空闲中断,这两行和普通数据串口发送有冲突
}

void USART2_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);//开启串口的接收中断
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);//开启串口的空闲中断,这两行和普通数据串口发送有冲突
}

void Ble_Anylze(void)
{
	if(ble.recvflag != 1)
		return ;
	if(ble.buff[0] != 0xAA || ble.buff[3] != 0x55)
	{
		printf("帧头帧尾出错\r\n");
		memset(&ble,0,sizeof(ble));
		return ;
	}
	switch(ble.buff[1])
	{
		case 0x01://蜂鸣器
			if(ble.buff[2])
			{
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
			}else{
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
			}
			break;
		case 0x02://灯光
			if(ble.buff[2])
			{
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
			}else{
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
			}
			break;
		case 0x03://小车，暂未测试
			switch(ble.buff[2])
			{
				case 0x01:
					lspeed1 = 0;
					rspeed1 = 0;
					break;
				case 0x02://前进
					lspeed1 = 600;
					rspeed1 = 600;
					break;
				case 0x03://后退
					lspeed1 = -600;
					rspeed1 = -600;
					break;
				case 0x04://左转
					lspeed1 = 600;
					rspeed1 = -600;
					break;
				case 0x05://右转
					lspeed1 = -600;
					rspeed1 = 600;
					break;
			}
			break;
		case 0x04://云台
			switch(ble.buff[2])
			{
				case 0x01:
					steer_dir = 0;//0停止，1左转，2 右转，3下降，4上升
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



