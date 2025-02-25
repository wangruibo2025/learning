#ifndef _MAIN_H
#define _MAIN_H

#include "stdio.h"
#include "stm32f10x.h"
#include "IO_BIT.h"//----------------?

//�ر�JTAG���ܣ�����SWD����----------��
inline void JTAG_SWDConfig(void)
{
	//��AFIOʱ��---------------------��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//�ر�JTAG���ܣ���SWD����
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}

#endif //_MAIN_H
