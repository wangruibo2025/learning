#include "tcrt.h"
#include "motor.h"
//循迹模式
uint8_t GetTcrtValue(void)
{
	uint8_t data = 0;
	data = HAL_GPIO_ReadPin(OUT1_GPIO_Port,OUT1_Pin);
	data |= (HAL_GPIO_ReadPin(OUT2_GPIO_Port,OUT2_Pin))<<1;
	data |= (HAL_GPIO_ReadPin(OUT3_GPIO_Port,OUT3_Pin))<<1;
	data |= (HAL_GPIO_ReadPin(OUT4_GPIO_Port,OUT4_Pin))<<1;
	data |= (HAL_GPIO_ReadPin(OUT5_GPIO_Port,OUT5_Pin))<<1;
	return data;
}

void XJ_Mode(void)
{
	uint8_t temp = GetTcrtValue();

switch(temp)
	{
		case 0://超出范围时
			Car_Front(-400);
			break;
				
		case 0x04://直走
		case 0xE:
			Car_Front(600);
			break;
		
		case 0x10://右转大弯
		case 0x18:
			
			Car_LeftRight(800,-400);
			HAL_Delay(100);
			break;
		
		case 0x07://右转直角	
		case 0xF:
			HAL_Delay(100);
			Car_LeftRight(800,-600);
			HAL_Delay(200);
			break;
		
		case 0x2://右转小弯
		case 0x6:				
			Car_LeftRight(500,100);
			HAL_Delay(100);
			break;		
		
		
		case 0x01://左转大弯
		case 0x03:			
			Car_LeftRight(-400,800);
			HAL_Delay(100);
			break;		
		
		case 0x1C://左转直角	
		case 0x1E:
			HAL_Delay(100);
			Car_LeftRight(-600,800);
			HAL_Delay(200);
			break;	
		
		case 0x8://左转小弯	
		case 0xC:	
			Car_LeftRight(100,500);
			HAL_Delay(100);
			break;	
		
		case 0x1F:
			Car_Front(0);
			break;
			
	}

}




