#include "encoder.h"
#include "stdio.h"


//编码器初始化
void Encoder_Init(void)
{
	Encoder_TIM2Config();
	Encoder_TIM4Config();
}
//编码器1初始化函数
void Encoder_TIM2Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //编码器1：PA0/PA1
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        // 不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    // 向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 65535;                      // 重装载值65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;                       // 分频系数0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
  TIM_ARRPreloadConfig(TIM2, DISABLE);//??????????????????????????
	
	// 编码器配置函数: 定时器2，模式3，上升沿
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  
  
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInit(TIM2, &TIM_ICInitStruct);
  
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // 清除定时器标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  // 定时器2，溢出更新，使能
	TIM_SetCounter(TIM2, 0);                   // 定时数据清零
	TIM_Cmd(TIM2, ENABLE);                     // 定时器2使能
	
}
//编码器2初始化函数
void Encoder_TIM4Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);  // 开启定时器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE); // 开启GPIO时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;    // 浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   // 编码器2:PB6/PB7
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
  
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      // 不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 65535;                    // 重装载值65535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;                     // 分频系数0
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct); 
	TIM_ARRPreloadConfig(TIM4, DISABLE);//自动预装载不使能
	
	// 编码器配置函数：定时器4，模式3，上升沿
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  
  
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInit(TIM4, &TIM_ICInitStruct);
  
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);      // 清除定时器溢出更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);  // 定时器4，溢出更新，使能
	TIM_SetCounter(TIM4, 0);                   // 定时数据清零
	TIM_Cmd(TIM4, ENABLE);                     // 定时器2使能
}

//定时器溢出次数
__IO int16_t EncoderA_Overflow_Count = 0;
__IO int16_t EncoderB_Overflow_Count = 0;

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)// 中断标志位置1
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // 清楚中断标志位
		if((TIM2->CR1 & (1<<4)))
		{	                          //反向
			EncoderA_Overflow_Count--;
		}
		else
		{											      //正向
			EncoderA_Overflow_Count++;
		}
	}
}

// 定时器4中断服务函数
void TIM4_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)  // 中断标志位置1
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  // 清楚中断标志位
		if((TIM4->CR1 & (1<<4))) {
			EncoderB_Overflow_Count--;
		}
		else {
			EncoderB_Overflow_Count++;
		}
  }
}

__IO int8_t MotorA_Direction = 0; //电机旋转方向 
__IO int32_t CaptureA_Count = 0; //当前时刻总计数值
__IO int32_t LastA_Count = 0; //上一时刻总计数值
__IO float ShaftA_Speed = 0.0f; //电机转轴转速

__IO int8_t MotorB_Direction = 0; //电机旋转方向 
__IO int32_t CaptureB_Count = 0; //当前时刻总计数值
__IO int32_t LastB_Count = 0; //上一时刻总计数值
__IO float ShaftB_Speed = 0.0f; //电机转轴转速


void Encoder_ReadSpeedTest(void)   //100ms调用一次
{
	MotorA_Direction = !!(TIM2->CR1 & (1<<4));//?????????????????????????????
	MotorB_Direction = !!(TIM4->CR1 & (1<<4));
	CaptureA_Count = TIM_GetCounter(TIM2) + (EncoderA_Overflow_Count * 65535);
	CaptureB_Count = TIM_GetCounter(TIM4) + (EncoderB_Overflow_Count * 65535);
	/* 转轴转速 = 单位时间内的计数值 / 编码器总分辨率 * 时间系数  */
	ShaftA_Speed = (float)(CaptureA_Count - LastA_Count) / (11*4) * 10 ;
	ShaftB_Speed = (float)(CaptureB_Count - LastB_Count) / (11*4) * 10 ;
	printf("方向：%d\t%d\r\n", MotorA_Direction, MotorB_Direction);
	printf("有效计数值：%d\t%d\r\n", CaptureA_Count - LastA_Count, CaptureB_Count - LastB_Count);/* 单位时间计数值 = 当前时刻总计数值 - 上一时刻总计数值 */
	printf("轴处转速：%.2f\t %.2f 转/秒 \r\n", ShaftA_Speed, ShaftB_Speed);
	printf("出轴转速：%.2f\t %.2f 转/秒 \r\n", ShaftA_Speed/34, ShaftB_Speed/34);/* 输出轴转速 = 转轴转速 / 减速比 */
	
	LastA_Count = CaptureA_Count;
	LastB_Count = CaptureB_Count;
}


int Read_MotorASpeed(void)
{
	int value = 0;;
	CaptureA_Count = TIM_GetCounter(TIM2) + (EncoderA_Overflow_Count * 65535);
	value = CaptureA_Count - LastA_Count;  // 采集编码器的计数值并保存
	LastA_Count = CaptureA_Count;
	return value;
}

int Read_MotorBSpeed(void)
{
	int value = 0;;
	CaptureB_Count = TIM_GetCounter(TIM4) + (EncoderB_Overflow_Count * 65535);
	value = CaptureB_Count - LastB_Count;  // 采集编码器的计数值并保存
	LastB_Count = CaptureB_Count;
	return value;
}


// 编码器速度读取函数   5ms读取一次数据
// 入口参数：定时器
int Read_Speed(int TIMx)
{
	int value_1;
  switch(TIMx)
  {
    case 2:
      value_1 = (short)TIM_GetCounter(TIM2);  // 采集编码器的计数值并保存
      TIM_SetCounter(TIM2, 0);   // 将定时器的计数值清零
      break;
    case 4:
      value_1 = (short)TIM_GetCounter(TIM4);  // 采集编码器的计数值并保存
      TIM_SetCounter(TIM4, 0);   // 将定时器的计数值清零
      break; 
    default: value_1 = 0;
  }
  return value_1;
}












