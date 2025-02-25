#include "motor.h"

#include "tim.h"
//IA1,IB1��ǰ��
//IA2,IB2�Һ���
//IA3,IB3��ǰ��
//IA4,IB4�����

int16_t lspeed1 = 0;
int16_t rspeed1 = 0;

void Motor_Init(void)
{
	HAL_TIM_Base_Start(&htim4);
	//IA3,IB3��ǰ��
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);//IA3
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);//IB3
	//IA1,IB1��ǰ��
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);//IA1
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);//IB1
	
	
	
	HAL_TIM_Base_Start(&htim3);
	//IA4,IB4�����
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);//IA4
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);//IB4
	//IA2,IB2�Һ���
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);//IA2
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);//IB2

}

/*������ǰ�ֵ��ٶ�*/
void Right_Front_Speed(int16_t speed)
{
	if(speed >999) speed = 999;
	else if(speed < -999) speed = -999;
	if(speed >= 0)//�ٶȴ���0��ת
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,speed);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);//IB1ռ�ձ�Ϊ0�����ֵ͵�ƽ
	}else//�ٶ�С��0��ת
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,0);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,-speed);
	}
}

void Right_Back_Speed(int16_t speed)
{
	if(speed >999) speed = 999;
	else if(speed < -999) speed = -999;
	if(speed >= 0)//�ٶȴ���0��ת
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,speed);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);//IB1ռ�ձ�Ϊ0�����ֵ͵�ƽ
	}else//�ٶ�С��0��ת
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,-speed);
	}
}

void Left_Back_Speed(int16_t speed)
{
	if(speed >999) speed = 999;
	else if(speed < -999) speed = -999;
	if(speed >= 0)//�ٶȴ���0��ת
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,speed);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);//IB1ռ�ձ�Ϊ0�����ֵ͵�ƽ
	}else//�ٶ�С��0��ת
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,-speed);
	}
}

void Left_Front_Speed(int16_t speed)
{
	if(speed >999) speed = 999;
	else if(speed < -999) speed = -999;
	if(speed >= 0)//�ٶȴ���0��ת
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,speed);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);//IB1ռ�ձ�Ϊ0�����ֵ͵�ƽ
	}else//�ٶ�С��0��ת
	{
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,-speed);
	}
}

/*С��ǰ��*/
void Car_Front(int16_t speed)
{
	Right_Front_Speed(speed);
	Right_Back_Speed(speed);
	Left_Front_Speed(speed);
	Left_Back_Speed(speed);
}

/*С������ת*/
void Car_LeftRight(int16_t speedRight,int16_t speedLeft)
{
	Right_Front_Speed(speedRight);
	Right_Back_Speed(speedRight);
	Left_Front_Speed(speedLeft);
	Left_Back_Speed(speedLeft);
}

/*IO����*/
void Motor_Front(void)
{
	/*��ǰ����ת*/
	HAL_GPIO_WritePin(IA1_GPIO_Port,IA1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IB1_GPIO_Port,IB1_Pin,GPIO_PIN_RESET);
	/*�Һ�����ת*/
	HAL_GPIO_WritePin(IA2_GPIO_Port,IA2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IB2_GPIO_Port,IB2_Pin,GPIO_PIN_RESET);
	/*�������ת*/
	HAL_GPIO_WritePin(IA3_GPIO_Port,IA3_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IB3_GPIO_Port,IB3_Pin,GPIO_PIN_RESET);	
	/*��ǰ����ת*/
	HAL_GPIO_WritePin(IA4_GPIO_Port,IA4_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IB4_GPIO_Port,IB4_Pin,GPIO_PIN_RESET);	
}

void Motor_Back(void)
{
	/*��ǰ�ַ�ת*/
	HAL_GPIO_WritePin(IA1_GPIO_Port,IA1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB1_GPIO_Port,IB1_Pin,GPIO_PIN_SET);
	/*�Һ��ַ�ת*/
	HAL_GPIO_WritePin(IA2_GPIO_Port,IA2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB2_GPIO_Port,IB2_Pin,GPIO_PIN_SET);
	/*����ַ�ת*/
	HAL_GPIO_WritePin(IA3_GPIO_Port,IA3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB3_GPIO_Port,IB3_Pin,GPIO_PIN_SET);	
	/*��ǰ�ַ�ת*/
	HAL_GPIO_WritePin(IA4_GPIO_Port,IA4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB4_GPIO_Port,IB4_Pin,GPIO_PIN_SET);		
}

void Motor_Left(void)
{
	/*��ǰ����ת*/
	HAL_GPIO_WritePin(IA1_GPIO_Port,IA1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IB1_GPIO_Port,IB1_Pin,GPIO_PIN_RESET);
	/*�Һ�����ת*/
	HAL_GPIO_WritePin(IA2_GPIO_Port,IA2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IB2_GPIO_Port,IB2_Pin,GPIO_PIN_RESET);
	/*����ַ�ת*/
	HAL_GPIO_WritePin(IA3_GPIO_Port,IA3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB3_GPIO_Port,IB3_Pin,GPIO_PIN_SET);	
	/*��ǰ�ַ�ת*/
	HAL_GPIO_WritePin(IA4_GPIO_Port,IA4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB4_GPIO_Port,IB4_Pin,GPIO_PIN_SET);		
}

void Motor_Right(void)
{
	/*��ǰ�ַ�ת*/
	HAL_GPIO_WritePin(IA1_GPIO_Port,IA1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB1_GPIO_Port,IB1_Pin,GPIO_PIN_SET);
	/*�Һ��ַ�ת*/
	HAL_GPIO_WritePin(IA2_GPIO_Port,IA2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB2_GPIO_Port,IB2_Pin,GPIO_PIN_SET);	
	/*�������ת*/
	HAL_GPIO_WritePin(IA3_GPIO_Port,IA3_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IB3_GPIO_Port,IB3_Pin,GPIO_PIN_RESET);	
	/*��ǰ����ת*/
	HAL_GPIO_WritePin(IA4_GPIO_Port,IA4_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IB4_GPIO_Port,IB4_Pin,GPIO_PIN_RESET);	
}

void Motor_Stop1(void)
{
	/*��ǰ��ͣת*/
	HAL_GPIO_WritePin(IA1_GPIO_Port,IA1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB1_GPIO_Port,IB1_Pin,GPIO_PIN_RESET);
	/*�Һ���ͣת*/
	HAL_GPIO_WritePin(IA2_GPIO_Port,IA2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB2_GPIO_Port,IB2_Pin,GPIO_PIN_RESET);	
	/*�����ͣת*/
	HAL_GPIO_WritePin(IA3_GPIO_Port,IA3_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB3_GPIO_Port,IB3_Pin,GPIO_PIN_RESET);	
	/*��ǰ��ͣת*/
	HAL_GPIO_WritePin(IA4_GPIO_Port,IA4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IB4_GPIO_Port,IB4_Pin,GPIO_PIN_RESET);		
}


void Motor_Move(int16_t lspeed, int16_t rspeed)
{
	if(rspeed > 999) rspeed = 999;
	else if(rspeed < -999) rspeed = -999;
	if(lspeed > 999) lspeed = 999;
	else if(lspeed < -999) lspeed = -999;
	
	if(rspeed >= 0)
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,rspeed);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,rspeed);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
	}
	else if(rspeed < 0)
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,0);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,-rspeed);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,-rspeed);
	}
	if(lspeed >= 0)
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,lspeed);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,lspeed);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);
	}
	else if(lspeed < 0)
	{
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,-lspeed);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,-lspeed);
	}


}
	


void Motor_Stop(void)
{
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,0);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);

}
