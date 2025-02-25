#include "sr04.h"
#include "tim.h"
#include "motor.h"
//���������

_SR04_TypeDef sr04;

//��ʼ�� SR04 �õ��Ķ�ʱ�� 6
void SR04_Init(void)
{
HAL_TIM_Base_Start(&htim6);
__HAL_TIM_ENABLE_IT(&htim6,TIM_IT_UPDATE);
}

void OA(float lens)
{
	
	if(lens < 25.00)
	{
		Motor_Move(600,-600);
		HAL_Delay(550);//�ݶ����������
		
	}
	Motor_Move(600,600);
}

void Follow(float lens)
{
	if(lens < 20.00)
	{
		Motor_Move(-600,-600);
	}
	
	else if(lens > 30.00)
	{
		Motor_Move(600,600);
	}
	
	else Motor_Move(0,0);
	
}



float Sr04_GetLength(void)
{
	/*�������ʱ����ȥ������� CNT 10us
	340m/s
	34000cm/s s->ms 10us
	34cm/ms
	0.34cm/10us
	������㣺CNT*0.34/2 ���ӷ�ĸ����1000 �����·��Ĵ��빫ʽ
	*/
	if(sr04.recvCountFlag == 0)
		sr04.leng = sr04.recvCount*340/2000.0;//cm
	if(sr04.leng > 400) sr04.leng = 380;
	return sr04.leng;
}


/*���ջ����źŵ������ж� */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/*��⵽ IO �Ǹߵ�ƽ������ߵ�ƽ�źŵ���*/
	if(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin) == SET)
	{
		//�ı俪ʼ��ʼ������־ �� 1
		sr04.recvCount = 0;
		sr04.recvCountFlag = 1;
	}else/*��⵽ IO �ǵ͵�ƽ������ߵ�ƽ����*/
	{
		//�������
		sr04.recvCountFlag = 0;
	}
}

//���ʹ����ź�
void Sr04_SendTTL(void) 
{
	sr04.sendCount++;//���ͼ�ʱ
	/*��һ�ν��ж�����Ϊ�ߵ�ƽ*/
	if(sr04.sendCount == 1 && HAL_GPIO_ReadPin(Trig_GPIO_Port, Trig_Pin) == RESET) {
		HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin,GPIO_PIN_SET);
	}
	/*�����ν��жϣ�����Ϊ�͵�ƽ���൱�ڷ����� 20us �ĸߵ�ƽ�����ź�*/
	else if(sr04.sendCount == 3 && HAL_GPIO_ReadPin(Trig_GPIO_Port, Trig_Pin) == SET) {
		HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin,GPIO_PIN_RESET);
	}
	/*���Ʒ��͵�����Ϊ 60MS 6000*10us = 60ms*/
	else if(sr04.sendCount > 6000){
		sr04.sendCount = 0;
		HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin,GPIO_PIN_RESET);
	}
}


//���Ӽ���
//10us ����һ�Σ���������
void Sr04_CountInc(void)
{
	if(sr04.recvCountFlag == 1)//�ߵ�ƽ�ڼ����
		sr04.recvCount++;
}


/*��ʱ�� 6 ���жϻص�������10us ִ��һ��*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	Sr04_SendTTL();//�������岨 10us ����һ��
	Sr04_CountInc();//����
}
