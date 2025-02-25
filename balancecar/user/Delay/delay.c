#include "delay.h"

vu32 System_Time = 0;//用于记录系统运行时间
vu32 Led_TaskTime[2] = {0, 5};//led   灯的运行时间
vu32 Encoder_TaskTime[2] = {0, 5};//encoder   编码器的运行时间
vu32 Steer_TaskTime[2] = {0, 30};//steer    驾驶，行驶，驱动方式

//系统定时器的中断服务函数
void SysTick_Handler(void)//曾被重复定义过，后注释了stm32f10x_it.c里第139行的空函数
{
	System_Time++;
	Led_TaskTime[0]++;
	Encoder_TaskTime[0]++;
	Steer_TaskTime[0]++;
}

//ms级别延时函数
void Delay_ms(uint32_t time)
{
	uint32_t tick = System_Time + time;
	while(System_Time < tick)
	{}
}

//系统定时器的初始化
//想要1ms来一次中断    reload = 72000
//参数reload；重装载值
void SysTick_Init(uint32_t reload)
{
	if(SysTick_Config(reload))
	{
		while(1);
	}
}


//us级别的延时函数
void Delay_us(uint32_t time)
{
	while(time--)
		delay_lus();
}

