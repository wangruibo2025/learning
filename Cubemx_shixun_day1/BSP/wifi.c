#include "wifi.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "motor.h"

WIFI wifi = {0};
extern uint8_t steer_dir;//0停止，1抬头，2 低头，3左转，4右转
extern int16_t lspeed1;
extern int16_t rspeed1;

//配置WiFi串口三
void WIFI_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);//开启串口的接收中断
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);//开启串口的空闲中断,这两行和普通数据串口发送有冲突

}

//WiFi数据解析
/*wifi数据解析
数据：小车控制 	0x00 刹车      舵机控制    		0x05  	停止		车速控制  	0x0A  	低速
								0x01 前进											0x06		左转								0x0B		中速
								0x02 后退											0x07		右转								0x0C		高速
								0x03 左转											0x08		向上
								0x04 右转											0x09		向下
*/
void WIFI_Anylze(void)
{
	uint8_t check = 0;
	if(wifi.recvflag != 1)
		return ;
	if(wifi.buff[0] != 0xAA || wifi.buff[1] != 0x55)
	{
		printf("帧头出错\r\n");
		memset(&wifi,0,sizeof(wifi));
		return ;
	}
	check = wifi.buff[0] + wifi.buff[1] + wifi.buff[2] + wifi.buff[3] + wifi.buff[4];
	if(check != wifi.buff[5])
	{
		printf("校验出错\r\n");
		memset(&wifi,0,sizeof(wifi));
		return ;
	}
	switch(wifi.buff[2])//小车
	{
		case 0x00://小车停止
			lspeed1 = 0;
			rspeed1 = 0;
			break;
		case 0x01://小车前进
			lspeed1 = 600;
			rspeed1 = 600;
			break;
		case 0x02://小车后退
			lspeed1 = -600;
			rspeed1 = -600;
			break;
		case 0x03://小车左转
			lspeed1 = -600;
			rspeed1 = 600;
			break;
		case 0x04://小车右转
			lspeed1 = 600;
			rspeed1 = -600;
			break;

		case 0x0A://小车低速
			lspeed1 -= 100;
			rspeed1 -= 100;
			break;
		case 0x0B://小车中速
			
			break;
		case 0x0C://小车高速
			lspeed1 += 100;
			rspeed1 += 100;
			break;
	}
	switch(wifi.buff[3])//小车
	{
		case 0x05://云台停止
			steer_dir = 0;
			break;
		case 0x06://云台左转
			steer_dir = 1;
			break;
		case 0x07://云台右转
			steer_dir = 2;
			break;
		case 0x08://云台上升
			steer_dir = 4;
			break;
		case 0x09://云台下降
			steer_dir = 3;
			break;
	
	}
	

}



//			switch(wifi.buff[3])
//			{
//				case 0x01:
//					lspeed1 = 0;
//					rspeed1 = 0;
//					break;
//				case 0x02://前进
//					lspeed1 = 600;
//					rspeed1 = 600;
//					break;
//				case 0x03://后退
//					lspeed1 = -600;
//					rspeed1 = -600;
//					break;
//				case 0x04://左转
//					lspeed1 = 600;
//					rspeed1 = -600;
//					break;
//				case 0x05://右转
//					lspeed1 = -600;
//					rspeed1 = 600;
//					break;
//			}
//			break;



//switch(wifi.buff[3])
//			{
//				case 0x01:
//					steer_dir = 0;//0停止，1抬头，2 低头，3左转，4右转
//					break;
//				case 0x02:
//					steer_dir = 1;
//					break;
//				case 0x03:
//					steer_dir = 2;
//					break;
//				case 0x04:
//					steer_dir = 3;
//					break;
//				case 0x05:
//					steer_dir = 4;
//					break;
//			}


