#include "motor.h"
#include "delay.h"

void Motor_Init(void)
{
	Motor_PWMConfig(10, 7200);
}

//��ʱ����ʼ����Ϊ��������ṩPWM��
//psc��ʱ���ķ�Ƶ��per��ʱ��������
void Motor_PWMConfig(uint16_t psc, uint16_t per)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB2PeriphClockCmd(PWM1_CLK | PWM2_CLK| PWM3_CLK | PWM4_CLK, ENABLE);
	PWMTIM_CLKCMD(PWMTIM_CLK, ENABLE);
	
	//�����ĸ�PWM�������ģʽ�������������
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
	
	//��ʱ���Ļ�����ʱ���ܣ�PWM����������
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//���벶��ʱ��������
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = per-1;//���� װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc-1;//��Ƶ��
	TIM_TimeBaseInit(PWMTIM, &TIM_TimeBaseInitStruct);
	
	//��ʱ��������ȽϹ���
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//��Ч��ƽ�ļ���
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//������õ�IO��
	TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ� �Ƚ�ֵ
	PWM1_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM2_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM3_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM4_INIT(PWMTIM, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(PWMTIM, TIM_OCPreload_Enable);//OC1Ԥװ�ؼĴ���ʹ��
	TIM_OC2PreloadConfig(PWMTIM, TIM_OCPreload_Enable);//OC2Ԥװ�ؼĴ���ʹ��
	TIM_OC3PreloadConfig(PWMTIM, TIM_OCPreload_Enable);//OC3Ԥװ�ؼĴ���ʹ��
	TIM_OC4PreloadConfig(PWMTIM, TIM_OCPreload_Enable);//OC4Ԥװ�ؼĴ���ʹ��
	TIM_ARRPreloadConfig(PWMTIM, ENABLE);//TIM3��ARRԤװ�ؼĴ���ʹ��
	
	//�����Ĵ���
	TIM_Cmd(PWMTIM, ENABLE);
	
}


//�޷�����
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

//����ֵ����
int GFP_abs(int p)
{
	int q;
	q = p > 0? p:(-p);
	return q;
}



//�������������PWMռ�ձȣ�����ת��
//���A������������B�������
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


