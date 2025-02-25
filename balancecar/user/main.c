#include "main.h"
#include "delay.h"
#include "led.h"
#include "motor.h"
#include "usart.h"
#include "encoder.h"
#include "steer.h"
#include "ble.h"

int main(void)
{
	char showBuff[20] = {0};
	JTAG_SWDConfig();
	SysTick_Init(72000);
	Led_Config();
	Usart1_Config(115200);
	printf("Hello XYD\r\n");
	Motor_Init();
	Encoder_Init();
	Steer_Init();
	Ble_Init();
	
	
	while(1)
	{
		
	}
}
