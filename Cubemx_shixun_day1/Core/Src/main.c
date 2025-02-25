/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "bsp_key.h"
#include "lcd.h"
#include "motor.h"
#include "sr04.h"
#include "tcrt.h"
#include "ir.h"
#include "Bluetooth.h"
#include "steer.h"
#include "wifi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t led1_tick = 0;
//uint16_t OaMode = 0;
//uint16_t FollowMode = 0;
uint16_t Lcd_tick = 0;
uint16_t beep_flag = 0;

uint16_t KeyMode = 0;

//uint16_t led2_tick = 0;
//uint16_t led3_tick = 0;
extern uint8_t recvbuff[10];//保存接收数据
extern uint8_t recvcnt;//接收数据计数
extern uint8_t recvover;//接收完成标志
extern const unsigned char gImage_logo[100160];
extern int16_t angle1;//上下角度
extern int16_t angle2;
extern int16_t lspeed1;
extern int16_t rspeed1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void DATA_Anylze(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_UART5_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	
	BSP_Key_Init();
	LCD_Init();
	Motor_Init();
	SR04_Init();//初始化超声波
	IR_Init();//初始化遥控器
	USART5_Init();//初始化蓝牙串口
	USART2_Init();
	Steer_Init();//初始化舵机
	WIFI_Init();
	
	
	LCD_Fill(0,0,128,160,WHITE);
//	void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个12x12汉字
	LCD_ShowChinese(0,0,(u8 *)"河南理工大学",RED,WHITE,16,0);
	LCD_ShowChinese(0,16,(u8 *)"汪瑞博",RED,WHITE,16,0);
	LCD_ShowChinese(0,32,(u8 *)"敖栋",RED,WHITE,16,0);
	LCD_ShowChinese(0,48,(u8 *)"第十三组",RED,WHITE,16,0);
	LCD_ShowChinese(0,64,(u8 *)"超声波距离",RED,WHITE,16,0);
	LCD_ShowChinese(0,80,(u8 *)"当前模式",RED,WHITE,16,0);
	
	__HAL_RCC_AFIO_CLK_ENABLE();//开启复用时钟
	__HAL_AFIO_REMAP_SWJ_NOJTAG();//关闭JTAG，保留SW下载
	

//__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);//开启串口的接收中断
//__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);//开启串口的空闲中断,这两行和普通数据串口发送有冲突


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	
	
//	uint8_t data;
char buff[50] = {0};
int8_t mode = 5;
extern const unsigned char gImage_dongtu1[20000];


  while (1)
  {
		//void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)


		if(beep_flag != 0)
		{
			HAL_GPIO_TogglePin(BEEP_GPIO_Port,BEEP_Pin);
			HAL_Delay(20);
			beep_flag--;
		}
		switch(ir.Ir_Key)
		{
			case '>':
				mode += 1;
				if(mode > 5) mode = 5;
				break;
			case '<':
				if(mode == 0) break;
				mode -= 1;
				break;
			case '1':
				LCD_ShowChinese(64,80,(u8 *)"蓝牙模式",RED,WHITE,16,0);
				Ble_Anylze();//蓝牙分析协议
				Ble_CtrSteer();//蓝牙分析角度
				Steer_SetAngle(angle1,angle2);//蓝牙控制舵机角度
				Motor_Move(lspeed1,rspeed1);
				break;
			case '2':
				LCD_ShowChinese(64,80,(u8 *)"寻迹模式",RED,WHITE,16,0);
				XJ_Mode();
				break;
			case '3':
				LCD_ShowChinese(64,80,(u8 *)"监控模式",RED,WHITE,16,0);
				WIFI_Anylze();
				Ble_CtrSteer();//蓝牙分析角度
				
				Motor_Move(lspeed1,rspeed1);
				Steer_SetAngle(angle1,angle2);//蓝牙控制舵机角度
				break;
			case '4':
				LCD_ShowChinese(64,80,(u8 *)"避障模式",RED,WHITE,16,0);
				OA(Sr04_GetLength());
				break;
			
		}
		switch(mode)
		{
			case 0://蓝牙模式
				LCD_Fill(0,0,128,160,WHITE);
				LCD_ShowChinese(0,0,(u8 *)"蓝牙模式",RED,WHITE,16,0);
				LCD_ShowPicture(0,16,100,116,gImage_dongtu1);
				break;
			case 1://寻迹模式
				LCD_Fill(0,0,128,160,WHITE);
				break;
			case 2://视频监控模式
				LCD_Fill(0,0,128,160,WHITE);
				break;
			case 3://避障模式
				LCD_Fill(0,0,128,160,WHITE);
				break;
			case 4://动图
				LCD_Fill(0,0,128,160,WHITE);
				break;
		}
		
//		DATA_Anylze();
//		Ble_Anylze();//蓝牙分析协议
//		Ble_CtrSteer();//蓝牙分析角度
//		Steer_SetAngle(angle1,angle2);//蓝牙控制舵机角度
//		Motor_Move(lspeed1,rspeed1);

//		WIFI_Anylze();
		if(led1_tick > 200)
		{
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
//			printf("KEY = %c\r\n",ir.Ir_Key);
			led1_tick = 0;
		}
		if(Lcd_tick > 200)
		{
			sprintf(buff,"%.2f",Sr04_GetLength());
			LCD_Fill(80,64,128,80,WHITE);
			LCD_ShowString(80,64,(u8*)buff,RED,WHITE,16,0);
			Lcd_tick =0;
		}
//		if(KeyMode == 0)
//		{
//			
//			OA(Sr04_GetLength());
//			
//		}
//		if(KeyMode == 1)
//		{
//			Follow(Sr04_GetLength());
//			
//		}
		
		
		
		
		
		
		
//		printf("%.2f\r\n",Sr04_GetLength());
	
		
//		if((USART1->SR & (0x1<<5)) != 0)
//		{
//			data = USART1->DR;
//			if(data == 'A')
//			{
////				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
//					__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,800);
//			}
//			if(data =='B')
//			{
////				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
//					__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,800);
//				
//			}

//			if(data =='C')
//			{
//				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
////				__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,800);
//			}
//			if(data =='D')
//			{
//				HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
////				__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,800);
//			}
//		
//				
//		}
		
				

		
		
		
//		button_ticks();
//		HAL_Delay(5);
		
		

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

int fputc(int c,FILE *f)
{
	uint8_t ch;
	ch = c;
	HAL_UART_Transmit(&huart1,&ch,1,1000);
	return c;
}

void DATA_Anylze(void)
{
	if(recvover != 1) return;
	if((recvbuff[0] != 0x55) || (recvbuff[5] != 0xAA))
	{
		HAL_UART_Transmit(&huart1,(uint8_t *)"帧头帧尾出错\r\n",15,100);
		recvcnt = 0;//清除错误数据
		recvover = 0;
		memset(recvbuff,0,10);
		return;
	}
	if(recvbuff[1] == 0x01) HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
	else if(recvbuff[1] == 0x00) HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
	
	if(recvbuff[2] == 0x01) HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);
	else if(recvbuff[2] == 0x00) HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
	
	if(recvbuff[3] == 0x01) HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
	else if(recvbuff[3] == 0x00) HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);
	
	if(recvbuff[4] == 0x01) HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
	else if(recvbuff[4] == 0x00) HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
	recvcnt = 0;//清除错误数据
	recvover = 0;
	memset(recvbuff,0,10);
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
