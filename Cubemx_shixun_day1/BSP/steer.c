#include "steer.h"
#include "tim.h"

int16_t angle1 = 90;//���ҽǶ�
int16_t angle2 = 90;//���½Ƕ�
uint8_t steer_dir = 0;//0ֹͣ��1̧ͷ��2 ��ͷ��3��ת��4��ת


//��ʼ�����  TIM1 CH1 CH4
void Steer_Init(void)
{
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	
	Steer_SetAngle(90,90);
}


//ת���ǶȺ���
void Steer_SetAngle(uint16_t Steer1,uint16_t Steer2)
{
//	return (data * 11) + 500;
	uint16_t data1;
	data1 = (Steer1 * (2000/180.0)) + 500;
	if(data1 > 2500) data1 = 2500;
	if(data1 < 500) data1 = 500;
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,data1);
	
	data1 = (Steer2 * (2000/180.0)) + 500;
	if(data1 > 2500) data1 = 2500;
	if(data1 < 500) data1 = 500;
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,data1);
	
}

void Ble_CtrSteer(void)
{
	switch(steer_dir)
	{
		case 0:
			break;
		case 1:
			if(angle1 > 180) break;
				angle1 += 1;
			break;
		case 2:
			if(angle1 < 0) break;
				angle1 -= 1;
			break;
		case 3:
			if(angle2 > 180) break;
				angle2 += 1;
			break;
		case 4:
			if(angle2 < 0) break;
				angle2 -= 1;
			break;
	}
}


