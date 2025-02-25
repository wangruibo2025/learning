#ifndef __IR_H
#define __IR_H

#include "main.h"
#define RCKeyNum 		18
#define RCIRLength 	4
#define MAX_DATALEN 5
typedef struct{
	uint8_t Ir_Key;
	uint8_t Ir_Data[MAX_DATALEN];
	uint16_t Ir_Length;
}IR_DATA_TypeDef;
extern IR_DATA_TypeDef ir;

extern uint16_t ir_buf[1024];
extern uint16_t ir_count;
extern uint16_t ir_flag;


void IR_Init(void);
uint8_t Ir_RecvAnalysis(void);

#endif
