#include "bsp_key.h"
#include "usart.h"
#include "main.h"
#include "lcd.h"

void knobCallback_1(void *p_btn);
struct Button KEY1;
const char chineseString[] = "汪瑞博\r\n";


extern uint16_t KeyMode;

uint8_t KEY_Read(uint8_t button_id)
{
	switch(button_id)
	{
		case 0:
			return HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);
		default:
      return 0;
	}
}

void BSP_Key_Init(void)
{
	button_init(&KEY1,KEY_Read,0,0);//初始化按键
	button_attach(&KEY1,SINGLE_CLICK,knobCallback_1);//绑定按键的事件回调函数
	button_attach(&KEY1,DOUBLE_CLICK,knobCallback_1);
	button_attach(&KEY1,LONG_PRESS_START,knobCallback_1);
	button_start(&KEY1);
}


void knobCallback_1(void *p_btn)
{
    uint8_t btn_event_val; 
    
    btn_event_val = get_button_event((struct Button *)p_btn); 
  
    switch(btn_event_val)
    {
        case SINGLE_CLICK:
						KeyMode++;
						KeyMode = KeyMode%2;
            break ;
        case DOUBLE_CLICK:
//            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//						HAL_UART_Transmit(&huart1,(uint8_t *)"312109010816\r\n",15,100);
//				LCD_Fill(0,0,128,160,WHITE);
//				LCD_ShowChinese(0,0,(u8*)"河南理工",BLUE,WHITE,16,0);
            break ;
				case LONG_PRESS_START:
//						HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//						HAL_UART_Transmit(&huart1,(uint8_t *)"17814550509\r\n",15,100);
        default:
            break ;
    }
}





