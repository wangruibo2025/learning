#include "ble.h"
#include "main.h"

uint8_t CarDir = CarBalance;			//小车方向
uint8_t SteerDir = SteerStop;		//云台方向
uint8_t SPEED_X = 10;				//小车速度
void USART2_IRQHandler(void)
{
	uint8_t data = 0;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
	{
		data = USART2->DR;
		USART1->DR = data;//蓝牙收到的数据，转发到串口1
		switch(data)
		{
			case 0x00: CarDir = CarBalance; break;// 刹车
			case 0x01: CarDir = CarFore; break;		// 前进
			case 0x02: CarDir = CarBack; break;		// 后退		
			case 0x03: CarDir = CarLeft; break;		// 左转		
			case 0x04: CarDir = CarRight; break;	// 右转
			case 0x05: SteerDir = SteerStop; break;		// 停止
			case 0x06: SteerDir = SteerLeft; break;		// 左转	
			case 0x07: SteerDir = SteerRight; break;	// 右转		
			case 0x08: SteerDir = SteerUp; break;			// 向上		
			case 0x09: SteerDir = SteerDown; break;		// 向下	
			case 0x0A: SPEED_X = 10; break;		//低速
			case 0x0B: SPEED_X = 15; break;		//正常
			case 0x0C: SPEED_X = 20; break;		//高速			
			default: CarDir = CarBalance; SteerDir = SteerStop; break;
		}
	}
//	if(USART_GetFlagStatus(USART2, USART_FLAG_IDLE) == SET)
//	{
//		data = USART2->DR;
//	}	
}

void Ble_Init(void)
{
	//开时钟
	Usart2_Config(9600);
}


void Usart2_Config(uint32_t brr)
{
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//配置IO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置USART2
	USART_InitTypeDef USART_InitStructuer;
	USART_InitStructuer.USART_BaudRate = brr;
	USART_InitStructuer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructuer.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructuer.USART_Parity = USART_Parity_No;
	USART_InitStructuer.USART_StopBits = USART_StopBits_1;
	USART_InitStructuer.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructuer);
	//使能接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	//配置NVIC
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	//使能串口
	USART_Cmd(USART2, ENABLE);
}



void Usart2_SendByte(uint8_t data)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_SendData(USART2, data);
}


void Usart2_SendString(uint8_t * str, uint16_t len)
{
	for(int i=0; i<len; i++)
	{
		Usart2_SendByte(str[i]);
	}
}

