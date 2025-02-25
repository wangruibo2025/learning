#include "encoder.h"
#include "stdio.h"


//��������ʼ��
void Encoder_Init(void)
{
	Encoder_TIM2Config();
	Encoder_TIM4Config();
}
//������1��ʼ������
void Encoder_TIM2Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //������1��PA0/PA1
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        // ����Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    // ���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 65535;                      // ��װ��ֵ65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;                       // ��Ƶϵ��0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
  TIM_ARRPreloadConfig(TIM2, DISABLE);//??????????????????????????
	
	// ���������ú���: ��ʱ��2��ģʽ3��������
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  
  
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInit(TIM2, &TIM_ICInitStruct);
  
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // �����ʱ����־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  // ��ʱ��2��������£�ʹ��
	TIM_SetCounter(TIM2, 0);                   // ��ʱ��������
	TIM_Cmd(TIM2, ENABLE);                     // ��ʱ��2ʹ��
	
}
//������2��ʼ������
void Encoder_TIM4Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);  // ������ʱ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE); // ����GPIOʱ��
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;    // ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   // ������2:PB6/PB7
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
  
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      // ����Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // ���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 65535;                    // ��װ��ֵ65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;                     // ��Ƶϵ��0
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct); 
	TIM_ARRPreloadConfig(TIM4, DISABLE);//�Զ�Ԥװ�ز�ʹ��
	
	// ���������ú�������ʱ��4��ģʽ3��������
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  
  
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInit(TIM4, &TIM_ICInitStruct);
  
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);      // �����ʱ��������±�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);  // ��ʱ��4��������£�ʹ��
	TIM_SetCounter(TIM4, 0);                   // ��ʱ��������
	TIM_Cmd(TIM4, ENABLE);                     // ��ʱ��2ʹ��
}

//��ʱ���������
__IO int16_t EncoderA_Overflow_Count = 0;
__IO int16_t EncoderB_Overflow_Count = 0;

//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)// �жϱ�־λ��1
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // ����жϱ�־λ
		if((TIM2->CR1 & (1<<4)))
		{	                          //����
			EncoderA_Overflow_Count--;
		}
		else
		{											      //����
			EncoderA_Overflow_Count++;
		}
	}
}

// ��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)  // �жϱ�־λ��1
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  // ����жϱ�־λ
		if((TIM4->CR1 & (1<<4))) {
			EncoderB_Overflow_Count--;
		}
		else {
			EncoderB_Overflow_Count++;
		}
  }
}

__IO int8_t MotorA_Direction = 0; //�����ת���� 
__IO int32_t CaptureA_Count = 0; //��ǰʱ���ܼ���ֵ
__IO int32_t LastA_Count = 0; //��һʱ���ܼ���ֵ
__IO float ShaftA_Speed = 0.0f; //���ת��ת��

__IO int8_t MotorB_Direction = 0; //�����ת���� 
__IO int32_t CaptureB_Count = 0; //��ǰʱ���ܼ���ֵ
__IO int32_t LastB_Count = 0; //��һʱ���ܼ���ֵ
__IO float ShaftB_Speed = 0.0f; //���ת��ת��


void Encoder_ReadSpeedTest(void)   //100ms����һ��
{
	MotorA_Direction = !!(TIM2->CR1 & (1<<4));//?????????????????????????????
	MotorB_Direction = !!(TIM4->CR1 & (1<<4));
	CaptureA_Count = TIM_GetCounter(TIM2) + (EncoderA_Overflow_Count * 65535);
	CaptureB_Count = TIM_GetCounter(TIM4) + (EncoderB_Overflow_Count * 65535);
	/* ת��ת�� = ��λʱ���ڵļ���ֵ / �������ֱܷ��� * ʱ��ϵ��  */
	ShaftA_Speed = (float)(CaptureA_Count - LastA_Count) / (11*4) * 10 ;
	ShaftB_Speed = (float)(CaptureB_Count - LastB_Count) / (11*4) * 10 ;
	printf("����%d\t%d\r\n", MotorA_Direction, MotorB_Direction);
	printf("��Ч����ֵ��%d\t%d\r\n", CaptureA_Count - LastA_Count, CaptureB_Count - LastB_Count);/* ��λʱ�����ֵ = ��ǰʱ���ܼ���ֵ - ��һʱ���ܼ���ֵ */
	printf("�ᴦת�٣�%.2f\t %.2f ת/�� \r\n", ShaftA_Speed, ShaftB_Speed);
	printf("����ת�٣�%.2f\t %.2f ת/�� \r\n", ShaftA_Speed/34, ShaftB_Speed/34);/* �����ת�� = ת��ת�� / ���ٱ� */
	
	LastA_Count = CaptureA_Count;
	LastB_Count = CaptureB_Count;
}


int Read_MotorASpeed(void)
{
	int value = 0;;
	CaptureA_Count = TIM_GetCounter(TIM2) + (EncoderA_Overflow_Count * 65535);
	value = CaptureA_Count - LastA_Count;  // �ɼ��������ļ���ֵ������
	LastA_Count = CaptureA_Count;
	return value;
}

int Read_MotorBSpeed(void)
{
	int value = 0;;
	CaptureB_Count = TIM_GetCounter(TIM4) + (EncoderB_Overflow_Count * 65535);
	value = CaptureB_Count - LastB_Count;  // �ɼ��������ļ���ֵ������
	LastB_Count = CaptureB_Count;
	return value;
}


// �������ٶȶ�ȡ����   5ms��ȡһ������
// ��ڲ�������ʱ��
int Read_Speed(int TIMx)
{
	int value_1;
  switch(TIMx)
  {
    case 2:
      value_1 = (short)TIM_GetCounter(TIM2);  // �ɼ��������ļ���ֵ������
      TIM_SetCounter(TIM2, 0);   // ����ʱ���ļ���ֵ����
      break;
    case 4:
      value_1 = (short)TIM_GetCounter(TIM4);  // �ɼ��������ļ���ֵ������
      TIM_SetCounter(TIM4, 0);   // ����ʱ���ļ���ֵ����
      break; 
    default: value_1 = 0;
  }
  return value_1;
}












