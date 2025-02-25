#include "wifi.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "motor.h"

WIFI wifi = {0};
extern uint8_t steer_dir;//0ֹͣ��1̧ͷ��2 ��ͷ��3��ת��4��ת
extern int16_t lspeed1;
extern int16_t rspeed1;

//����WiFi������
void WIFI_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);//�������ڵĽ����ж�
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);//�������ڵĿ����ж�,�����к���ͨ���ݴ��ڷ����г�ͻ

}

//WiFi���ݽ���
/*wifi���ݽ���
���ݣ�С������ 	0x00 ɲ��      �������    		0x05  	ֹͣ		���ٿ���  	0x0A  	����
								0x01 ǰ��											0x06		��ת								0x0B		����
								0x02 ����											0x07		��ת								0x0C		����
								0x03 ��ת											0x08		����
								0x04 ��ת											0x09		����
*/
void WIFI_Anylze(void)
{
	uint8_t check = 0;
	if(wifi.recvflag != 1)
		return ;
	if(wifi.buff[0] != 0xAA || wifi.buff[1] != 0x55)
	{
		printf("֡ͷ����\r\n");
		memset(&wifi,0,sizeof(wifi));
		return ;
	}
	check = wifi.buff[0] + wifi.buff[1] + wifi.buff[2] + wifi.buff[3] + wifi.buff[4];
	if(check != wifi.buff[5])
	{
		printf("У�����\r\n");
		memset(&wifi,0,sizeof(wifi));
		return ;
	}
	switch(wifi.buff[2])//С��
	{
		case 0x00://С��ֹͣ
			lspeed1 = 0;
			rspeed1 = 0;
			break;
		case 0x01://С��ǰ��
			lspeed1 = 600;
			rspeed1 = 600;
			break;
		case 0x02://С������
			lspeed1 = -600;
			rspeed1 = -600;
			break;
		case 0x03://С����ת
			lspeed1 = -600;
			rspeed1 = 600;
			break;
		case 0x04://С����ת
			lspeed1 = 600;
			rspeed1 = -600;
			break;

		case 0x0A://С������
			lspeed1 -= 100;
			rspeed1 -= 100;
			break;
		case 0x0B://С������
			
			break;
		case 0x0C://С������
			lspeed1 += 100;
			rspeed1 += 100;
			break;
	}
	switch(wifi.buff[3])//С��
	{
		case 0x05://��ֹ̨ͣ
			steer_dir = 0;
			break;
		case 0x06://��̨��ת
			steer_dir = 1;
			break;
		case 0x07://��̨��ת
			steer_dir = 2;
			break;
		case 0x08://��̨����
			steer_dir = 4;
			break;
		case 0x09://��̨�½�
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
//				case 0x02://ǰ��
//					lspeed1 = 600;
//					rspeed1 = 600;
//					break;
//				case 0x03://����
//					lspeed1 = -600;
//					rspeed1 = -600;
//					break;
//				case 0x04://��ת
//					lspeed1 = 600;
//					rspeed1 = -600;
//					break;
//				case 0x05://��ת
//					lspeed1 = -600;
//					rspeed1 = 600;
//					break;
//			}
//			break;



//switch(wifi.buff[3])
//			{
//				case 0x01:
//					steer_dir = 0;//0ֹͣ��1̧ͷ��2 ��ͷ��3��ת��4��ת
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


