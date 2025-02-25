#ifndef __MOTOR_H
#define __MOTOR_H	

#include "main.h"
void Motor_Init(void);
void Motor_Move(int16_t lspeed, int16_t rspeed);
void Car_Front(int16_t speed);
void Car_LeftRight(int16_t speedRight,int16_t speedLeft);
void Motor_Stop(void);
void Motor_Stop1(void);	

#endif
