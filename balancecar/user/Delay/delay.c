#include "delay.h"

vu32 System_Time = 0;//���ڼ�¼ϵͳ����ʱ��
vu32 Led_TaskTime[2] = {0, 5};//led   �Ƶ�����ʱ��
vu32 Encoder_TaskTime[2] = {0, 5};//encoder   ������������ʱ��
vu32 Steer_TaskTime[2] = {0, 30};//steer    ��ʻ����ʻ��������ʽ

//ϵͳ��ʱ�����жϷ�����
void SysTick_Handler(void)//�����ظ����������ע����stm32f10x_it.c���139�еĿպ���
{
	System_Time++;
	Led_TaskTime[0]++;
	Encoder_TaskTime[0]++;
	Steer_TaskTime[0]++;
}

//ms������ʱ����
void Delay_ms(uint32_t time)
{
	uint32_t tick = System_Time + time;
	while(System_Time < tick)
	{}
}

//ϵͳ��ʱ���ĳ�ʼ��
//��Ҫ1ms��һ���ж�    reload = 72000
//����reload����װ��ֵ
void SysTick_Init(uint32_t reload)
{
	if(SysTick_Config(reload))
	{
		while(1);
	}
}


//us�������ʱ����
void Delay_us(uint32_t time)
{
	while(time--)
		delay_lus();
}

