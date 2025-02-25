#include "led.h"

void Led_Config(void)
{
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(LED1_CLK | LED2_CLK, ENABLE);//���䲻�ι̣�Ҫע��
	
	//��ʼ��LED1��LED2��gpio
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);
	
	//��ʼ��LED1��LED2���ص�
	Led_Off(LED1_PORT, LED1_PIN);
	Led_Off(LED2_PORT, LED2_PIN);
}




