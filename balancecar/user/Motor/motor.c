#include "motor.h"
#include "delay.h"

void Motor_Init(void)
{
	Motor_PWMConfig(10, 7200);
}

//定时器初始化，为两个电机提供PWM波
//psc定时器的分频，per定时器的周期
void Motor_PWMConfig(uint16_t psc, uint16_t per)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB2PeriphClockCmd(PWM1_CLK | PWM2_CLK| PWM3_CLK | PWM4_CLK, ENABLE);
	PWMTIM_CLKCMD(PWMTIM_CLK, ENABLE);
	
	//配置四个PWM输出引脚模式，复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PWM1_PIN;
	GPIO_Init(PWM1_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PWM2_PIN;
	GPIO_Init(PWM2_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PWM3_PIN;
	GPIO_Init(PWM3_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PWM4_PIN;
	GPIO_Init(PWM4_PORT, &GPIO_InitStructure);
	
	//定时器的基本定时功能，PWM的周期配置
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//输入捕获时才有作用
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = per-1;//周期 装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc-1;//分频器
	TIM_TimeBaseInit(PWMTIM, &TIM_TimeBaseInitStruct);
	
	//定时器的输出比较功能
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//有效电平的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出作用到IO上
	TIM_OCInitStructure.TIM_Pulse = 0;//占空比 比较值
	PWM1_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM2_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM3_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM4_INIT(PWMTIM, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(PWMTIM, TIM_OCPreload_Enable);//OC1预装载寄存器使能
	TIM_OC2PreloadConfig(PWMTIM, TIM_OCPreload_Enable);//OC2预装载寄存器使能
	TIM_OC3PreloadConfig(PWMTIM, TIM_OCPreload_Enable);//OC3预装载寄存器使能
	TIM_OC4PreloadConfig(PWMTIM, TIM_OCPreload_Enable);//OC4预装载寄存器使能
	TIM_ARRPreloadConfig(PWMTIM, ENABLE);//TIM3在ARR预装载寄存器使能
	
	//启动寄存器
	TIM_Cmd(PWMTIM, ENABLE);
	
}


//限幅函数
void Limit(int* pwmA, int* pwmB)
{
	if(*pwmA > PWM_MAX)
		*pwmA = PWM_MAX;
	if(*pwmA < PWM_MIN)
		*pwmA = PWM_MIN;
	if(*pwmB > PWM_MAX)
		*pwmB = PWM_MAX;
	if(*pwmB < PWM_MIN)
		*pwmB = PWM_MIN;
}

//绝对值函数
int GFP_abs(int p)
{
	int q;
	q = p > 0? p:(-p);
	return q;
}



//设置两个电机的PWM占空比，调节转速
//电机A脉冲个数，电机B脉冲个数
void Motor_PWMLoad(int motoA, int motoB)
{
	if(motoA > 0)
	{
		PWM1_DISABLE();
		__NOP();__NOP();
		PWM2_ENABLE();
		PWM2_SET(PWMTIM, GFP_abs(motoA));
	}
	else
	{
		PWM2_DISABLE();
		__NOP();__NOP();
		PWM1_ENABLE();
		PWM1_SET(PWMTIM, GFP_abs(motoA));
	}
	if(motoB > 0)
	{
		PWM4_DISABLE();
		__NOP();__NOP();
		PWM3_ENABLE();
		PWM3_SET(PWMTIM, GFP_abs(motoB));
	}
	else
	{
		PWM3_DISABLE();
		__NOP();__NOP();
		PWM4_ENABLE();
		PWM4_SET(PWMTIM, GFP_abs(motoB));
	}
	
}


