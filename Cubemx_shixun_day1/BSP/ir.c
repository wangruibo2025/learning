#include "ir.h"
#include "tim.h"
#include "stdio.h"
#include "string.h"

//红外遥控

extern uint16_t beep_flag;

IR_DATA_TypeDef ir;

uint8_t IR_key[18] ="1234567890-C<S>T+R";//与下边一一对应
uint8_t RemoteControl_Table[RCKeyNum][RCIRLength] = {
	{0x00,0xFF,0x0C,0xF3},//0x00,0xff,data,data反
	{0x00,0xFF,0x18,0xE7},//前一半地址码
	{0x00,0xFF,0x5E,0xA1},	
	{0x00,0xFF,0x08,0xF7},	
	{0x00,0xFF,0x1C,0xE3},	
	{0x00,0xFF,0x5A,0xA5},	
	{0x00,0xFF,0x42,0xBD},	
	{0x00,0xFF,0x52,0xAD},	
	{0x00,0xFF,0x4A,0xB5},	
	{0x00,0xFF,0x16,0xE9},	
	{0x00,0xFF,0x19,0xE6},	
	{0x00,0xFF,0x0D,0xF2},		
	{0x00,0xFF,0x07,0xF8},		
	{0x00,0xFF,0x15,0xEA},	
	{0x00,0xFF,0x09,0xF6},	
	{0x00,0xFF,0x44,0xBB},		
	{0x00,0xFF,0x40,0xBF},	
	{0x00,0xFF,0x43,0xBC},		
};

uint16_t ir_buf[1024] = {0};
uint16_t ir_count = 0;
uint16_t ir_flag = 0;

void IR_Init(void)
{
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_2);
	__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);
	__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_CC2);
}
//判断数据范围函数
//数据在给定的范围之内：返回1
//数据在给定的范围之外：返回0
//time1:待判断时间
//time2:标准时间
uint8_t Time_Range(uint16_t time1, uint16_t time2, uint16_t range1, uint16_t range2)
{
	if((time1 > (time2-range1)) && (time1 < (time2+range2)))
		return 1;
	else 
		return 0;
}

//校验遥控器按键
//正确，返回遥控器按键所对应的数组行号
//错误 -1  按键地址错误 不是这个遥控器
//错误 -2  地址正确，但是当前版本遥控器没有这个按键值
int Check_RemoteControlKey(uint8_t *buff)
{
	uint8_t i=0;
	int retn;
	if((buff[0]!=0x00) || (buff[1]!=0xFF))
		retn = -1;//地址错误
	for(i=0; i<RCKeyNum; i++)
	{
		if((buff[2]==RemoteControl_Table[i][2]) && (buff[3]==RemoteControl_Table[i][3]))
		{
			retn = i;
			break;
		}
		
	}
	if(i>=RCKeyNum)	retn = -2;
	return retn;
}


//返回值 
//0 解析成功
//1 没有接收完成
//2 引导码错误
//3 前半段数据错误
//4 后半段数据错误
IR_DATA_TypeDef ir = {0};
uint8_t Ir_RecvAnalysis(void)
{
	int keyIndex = -1;
	uint16_t i = 0;
	uint8_t err;
	if(ir_flag == 0) return 1;
	if(Time_Range(ir_buf[1],9000,1000,1000) == 0){//判断9ms的引导码
		err = 2;
		goto error;
	}
	if(Time_Range(ir_buf[2],4500,500,500) == 0){//判断4.5ms的引导码
		err = 2;
		goto error;
	}
	for(i=3; i<ir_count-1; i++)
	{
		if(Time_Range(ir_buf[i],560,200,200))//先判断0.56ms低电平
		{
			i++;
			if(Time_Range(ir_buf[i],560,200,200))//0.56表示数据是0
			{
				//数据0  0-7  data[0]  8-15 data[1]
				//先收到低位保存到低位，也可以反着
				ir.Ir_Data[ir.Ir_Length/8] &= ~(1<<(ir.Ir_Length%8));
				ir.Ir_Length++;
			}
			else if(Time_Range(ir_buf[i],1690,200,200)==1)//表示数据1
			{
				//数据1
				ir.Ir_Data[ir.Ir_Length/8] |= (1<<(ir.Ir_Length%8));
				ir.Ir_Length++;
			}
			else{
				err = 4;
				goto error;
			}			
		}
		else{
			err = 3;
			goto error;
		}
	}
//	printf("recv:");
//	for(uint8_t i=0;i<5;i++)
//	{
//		printf("%X ",ir.Ir_Data[i]);
//	}
//	printf("\r\n");
	keyIndex = Check_RemoteControlKey(ir.Ir_Data);
//	printf("keyIndex:%d\r\n",keyIndex);
	if(keyIndex < 18 && keyIndex >=0)
	{
		ir.Ir_Key = IR_key[keyIndex];//接收完成
		beep_flag = 4;
		//清除数据
		ir.Ir_Length = 0;
		ir_count = 0;
		ir_flag = 0;
		memset(ir_buf, 0, sizeof(ir_buf));
		memset(ir.Ir_Data,0,sizeof(ir.Ir_Data));
		
		return 0;
	}
error:
	memset(ir_buf, 0, sizeof(ir_buf));
	memset(ir.Ir_Data,0,sizeof(ir.Ir_Data));
	ir.Ir_Length = 0;
	ir_count = 0;
	ir_flag = 0;
	HAL_TIM_Base_Start(&htim2);
	if(err)	return err;
	else	return 0;
}





