#include "ir.h"
#include "tim.h"
#include "stdio.h"
#include "string.h"

//����ң��

extern uint16_t beep_flag;

IR_DATA_TypeDef ir;

uint8_t IR_key[18] ="1234567890-C<S>T+R";//���±�һһ��Ӧ
uint8_t RemoteControl_Table[RCKeyNum][RCIRLength] = {
	{0x00,0xFF,0x0C,0xF3},//0x00,0xff,data,data��
	{0x00,0xFF,0x18,0xE7},//ǰһ���ַ��
	{0x00,0xFF,0x5E,0xA1},	
	{0x00,0xFF,0x08,0xF7},	
	{0x00,0xFF,0x1C,0xE3},	
	{0x00,0xFF,0x5A,0xA5},	
	{0x00,0xFF,0x42,0xBD},	
	{0x00,0xFF,0x52,0xAD},	
	{0x00,0xFF,0x4A,0xB5},	
	{0x00,0xFF,0x16,0xE9},	
	{0x00,0xFF,0x19,0xE6},	
	{0x00,0xFF,0x0D,0xF2},		
	{0x00,0xFF,0x07,0xF8},		
	{0x00,0xFF,0x15,0xEA},	
	{0x00,0xFF,0x09,0xF6},	
	{0x00,0xFF,0x44,0xBB},		
	{0x00,0xFF,0x40,0xBF},	
	{0x00,0xFF,0x43,0xBC},		
};

uint16_t ir_buf[1024] = {0};
uint16_t ir_count = 0;
uint16_t ir_flag = 0;

void IR_Init(void)
{
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_2);
	__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);
	__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_CC2);
}
//�ж����ݷ�Χ����
//�����ڸ����ķ�Χ֮�ڣ�����1
//�����ڸ����ķ�Χ֮�⣺����0
//time1:���ж�ʱ��
//time2:��׼ʱ��
uint8_t Time_Range(uint16_t time1, uint16_t time2, uint16_t range1, uint16_t range2)
{
	if((time1 > (time2-range1)) && (time1 < (time2+range2)))
		return 1;
	else 
		return 0;
}

//У��ң��������
//��ȷ������ң������������Ӧ�������к�
//���� -1  ������ַ���� �������ң����
//���� -2  ��ַ��ȷ�����ǵ�ǰ�汾ң����û���������ֵ
int Check_RemoteControlKey(uint8_t *buff)
{
	uint8_t i=0;
	int retn;
	if((buff[0]!=0x00) || (buff[1]!=0xFF))
		retn = -1;//��ַ����
	for(i=0; i<RCKeyNum; i++)
	{
		if((buff[2]==RemoteControl_Table[i][2]) && (buff[3]==RemoteControl_Table[i][3]))
		{
			retn = i;
			break;
		}
		
	}
	if(i>=RCKeyNum)	retn = -2;
	return retn;
}


//����ֵ 
//0 �����ɹ�
//1 û�н������
//2 ���������
//3 ǰ������ݴ���
//4 �������ݴ���
IR_DATA_TypeDef ir = {0};
uint8_t Ir_RecvAnalysis(void)
{
	int keyIndex = -1;
	uint16_t i = 0;
	uint8_t err;
	if(ir_flag == 0) return 1;
	if(Time_Range(ir_buf[1],9000,1000,1000) == 0){//�ж�9ms��������
		err = 2;
		goto error;
	}
	if(Time_Range(ir_buf[2],4500,500,500) == 0){//�ж�4.5ms��������
		err = 2;
		goto error;
	}
	for(i=3; i<ir_count-1; i++)
	{
		if(Time_Range(ir_buf[i],560,200,200))//���ж�0.56ms�͵�ƽ
		{
			i++;
			if(Time_Range(ir_buf[i],560,200,200))//0.56��ʾ������0
			{
				//����0  0-7  data[0]  8-15 data[1]
				//���յ���λ���浽��λ��Ҳ���Է���
				ir.Ir_Data[ir.Ir_Length/8] &= ~(1<<(ir.Ir_Length%8));
				ir.Ir_Length++;
			}
			else if(Time_Range(ir_buf[i],1690,200,200)==1)//��ʾ����1
			{
				//����1
				ir.Ir_Data[ir.Ir_Length/8] |= (1<<(ir.Ir_Length%8));
				ir.Ir_Length++;
			}
			else{
				err = 4;
				goto error;
			}			
		}
		else{
			err = 3;
			goto error;
		}
	}
//	printf("recv:");
//	for(uint8_t i=0;i<5;i++)
//	{
//		printf("%X ",ir.Ir_Data[i]);
//	}
//	printf("\r\n");
	keyIndex = Check_RemoteControlKey(ir.Ir_Data);
//	printf("keyIndex:%d\r\n",keyIndex);
	if(keyIndex < 18 && keyIndex >=0)
	{
		ir.Ir_Key = IR_key[keyIndex];//�������
		beep_flag = 4;
		//�������
		ir.Ir_Length = 0;
		ir_count = 0;
		ir_flag = 0;
		memset(ir_buf, 0, sizeof(ir_buf));
		memset(ir.Ir_Data,0,sizeof(ir.Ir_Data));
		
		return 0;
	}
error:
	memset(ir_buf, 0, sizeof(ir_buf));
	memset(ir.Ir_Data,0,sizeof(ir.Ir_Data));
	ir.Ir_Length = 0;
	ir_count = 0;
	ir_flag = 0;
	HAL_TIM_Base_Start(&htim2);
	if(err)	return err;
	else	return 0;
}





