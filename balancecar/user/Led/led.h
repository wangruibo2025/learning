#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define LED1_CLK      RCC_APB2Periph_GPIOC
#define LED2_CLK      RCC_APB2Periph_GPIOC

#define LED1_PORT     GPIOC
#define LED1_PIN      GPIO_Pin_9
#define LED2_PORT     GPIOC
#define LED2_PIN      GPIO_Pin_8

#define Led_On(port, pin)          (GPIO_ResetBits(port, pin))
#define Led_Off(port, pin)         (GPIO_SetBits(port, pin))
#define Led_Toggle(port, pin)      (port->ODR ^= pin)//感觉能用另一个函数代替

void Led_Config(void);


#endif
