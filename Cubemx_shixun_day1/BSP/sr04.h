#ifndef __SR04_H
#define __SR04_H	

#include "main.h"

typedef struct
{
	uint32_t sendCount;
	uint8_t recvCountFlag;
	uint32_t recvCount;
	float leng;
}_SR04_TypeDef;

 extern _SR04_TypeDef sr04;

void SR04_Init(void);
float Sr04_GetLength(void);
void OA(float lens);
void Follow(float lens);

#endif
