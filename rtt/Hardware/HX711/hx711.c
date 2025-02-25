#include "hx711.h"
#include "delay.h"

long HX711_Buffer = 0;
long Weight_Maopi = 0,Weight_Shiwu = 0;

#define GapValue 430
#define HIGH 1
#define LOW  0


void digitalWrite(GPIO_TypeDef * GPIO,int pin,int value)
{
	if(value==HIGH)//set 1
	{
		GPIO_SetBits(GPIO,pin);
	}
	if(value==LOW)//reset 0
	{
		GPIO_ResetBits(GPIO,pin);
	}
}

int digitalRead(GPIO_TypeDef * GPIO,int pin)
{
	return GPIO_ReadInputDataBit(GPIO,pin);
}

//****************************************************
//��ʼ��HX711
//****************************************************
void Init_Hx711()
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	  //SCK
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	  GPIO_Init(GPIOF, &GPIO_InitStructure);

	  //DOUT
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  //GPIO_InitStructure1.GPIO_OType = GPIO_OType_OD; 
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOF, &GPIO_InitStructure);

}


//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();		
} 

//****************************************************
//����
//****************************************************
long Get_Weight()
{
	HX711_Buffer = HX711_Read();
	Weight_Shiwu = HX711_Buffer;
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��
	Weight_Shiwu = (long)((float)Weight_Shiwu/GapValue); 	
	return Weight_Shiwu;
}

//****************************************************
//��ȡHX711
//****************************************************
unsigned long HX711_Read(void)	//����128
{
	unsigned long count; 
	unsigned char i; 

  	HX711_DOUT=1; 

	delay_us(1);
	
  	HX711_SCK=0; 
	
  	count=0; 

  	while(HX711_DOUT); 

  	for(i=0;i<24;i++)
	{ 
	
	  	HX711_SCK=1; 
	  	count=count<<1; 
		delay_us(1);
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
		delay_us(1);
	
	} 

 	HX711_SCK=1; 

    count=count^0x800000;//��25�������½�����ʱ��ת������
	delay_us(1);

	HX711_SCK=0;  

	return(count);
}
