#include "usart.h"
#include "stdio.h"
#include "string.h"

//���ڳ�ʼ��
//USART1
//TX: PA9    �����������
//RX: PA10   ��������/����
//1 8 0 1    //??????????
void Usart1_Config(u32 brr)
{
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	//��ʼ��GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����USART1
	USART_InitTypeDef USART_InitStructuer;
	USART_InitStructuer.USART_BaudRate             = brr;
	USART_InitStructuer.USART_WordLength           = USART_WordLength_8b;
	USART_InitStructuer.USART_StopBits             = USART_StopBits_1;
	USART_InitStructuer.USART_Parity               = USART_Parity_No;
	USART_InitStructuer.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructuer.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructuer);
	
	//ʹ�ܴ���   //����������������������������������
	USART_Cmd(USART1, ENABLE);
	
}



