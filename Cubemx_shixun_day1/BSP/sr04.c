#include "sr04.h"
#include "tim.h"
#include "motor.h"
//超声波测距

_SR04_TypeDef sr04;

//初始化 SR04 用到的定时器 6
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
		HAL_Delay(550);//暂定，具体调试
		
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
	/*计数完成时，再去计算距离 CNT 10us
	340m/s
	34000cm/s s->ms 10us
	34cm/ms
	0.34cm/10us
	距离计算：CNT*0.34/2 分子分母都×1000 就是下方的代码公式
	*/
	if(sr04.recvCountFlag == 0)
		sr04.leng = sr04.recvCount*340/2000.0;//cm
	if(sr04.leng > 400) sr04.leng = 380;
	return sr04.leng;
}


/*接收回响信号的引脚中断 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/*检测到 IO 是高电平，代表高电平信号到来*/
	if(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin) == SET)
	{
		//改变开始开始计数标志 置 1
		sr04.recvCount = 0;
		sr04.recvCountFlag = 1;
	}else/*检测到 IO 是低电平，代表高电平结束*/
	{
		//计数完成
		sr04.recvCountFlag = 0;
	}
}

//发送触发信号
void Sr04_SendTTL(void) 
{
	sr04.sendCount++;//发送计时
	/*第一次进中断设置为高电平*/
	if(sr04.sendCount == 1 && HAL_GPIO_ReadPin(Trig_GPIO_Port, Trig_Pin) == RESET) {
		HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin,GPIO_PIN_SET);
	}
	/*第三次进中断，设置为低电平，相当于发送了 20us 的高电平脉冲信号*/
	else if(sr04.sendCount == 3 && HAL_GPIO_ReadPin(Trig_GPIO_Port, Trig_Pin) == SET) {
		HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin,GPIO_PIN_RESET);
	}
	/*控制发送的周期为 60MS 6000*10us = 60ms*/
	else if(sr04.sendCount > 6000){
		sr04.sendCount = 0;
		HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin,GPIO_PIN_RESET);
	}
}


//增加计数
//10us 调用一次，用来计数
void Sr04_CountInc(void)
{
	if(sr04.recvCountFlag == 1)//高电平期间计数
		sr04.recvCount++;
}


/*定时器 6 的中断回调函数，10us 执行一次*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	Sr04_SendTTL();//发送脉冲波 10us 发送一次
	Sr04_CountInc();//计数
}
