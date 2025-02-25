#include "sys.h"
#include "usart.h"	
static GPIO_InitTypeDef 		GPIO_InitStructure;
static NVIC_InitTypeDef 		NVIC_InitStructure;
static USART_InitTypeDef 		USART_InitStructure;




rt_mailbox_t g_mail_data;
/*
#pragma import(__use_no_semihosting_swi)
struct __FILE { int handle; /* Add whatever you need here * };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	USART_ClearFlag(USART1,USART_FLAG_TXE);
	
	return ch;
}
void _sys_exit(int return_code) {

}

*/

void usart_init(uint32_t baud)
{
	
	//��PAӲ��ʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	

	//�򿪴���1Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//����PA9��PA10Ϊ���ù���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;		//��9 10������
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;	//�๦��ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������������������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//������Ӧ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//û��ʹ������������

	GPIO_Init(GPIOA,&GPIO_InitStructure);


	//��PA9��PA10�������ӵ�����1��Ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	
	
	
	
	//���ô���1��ز����������ʡ���У��λ��8λ����λ��1��ֹͣλ......
	USART_InitStructure.USART_BaudRate = baud;										//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�����շ�����
	USART_Init(USART1, &USART_InitStructure);
	
	
	//���ô���1���жϴ�������������һ���ֽڴ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	//���ô���1���ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	//ʹ�ܴ���1����
	USART_Cmd(USART1, ENABLE);
}
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t d;
	
	//����־λ
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//��������
		/*d=USART_ReceiveData(USART1);
		if((d == 0x00 | d == 0x01))
		{
			rt_kprintf("���㵽����\r\n");
			CD100M_uart_decode(d, &g_CD100M_res_info, &g_CD100M_dbg_info, &g_CD100M_paras);	
		}
		//��ձ�־λ*/
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}


}

void uart2_init(uint32_t baud)
{
	
	//��PAӲ��ʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	

	//�򿪴���1Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//����PA9��PA10Ϊ���ù���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;		//��9 10������
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;	//�๦��ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������������������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//������Ӧ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//û��ʹ������������

	GPIO_Init(GPIOA,&GPIO_InitStructure);


	//��PA9��PA10�������ӵ�����1��Ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);	
	
	
	
	//���ô���1��ز����������ʡ���У��λ��8λ����λ��1��ֹͣλ......
	USART_InitStructure.USART_BaudRate = baud;										//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�����շ�����
	USART_Init(USART2, &USART_InitStructure);
	
	
	//���ô���1���жϴ�������������һ���ֽڴ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	
	//���ô���1���ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	//ʹ�ܴ���1����
	USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void)                	//����1�жϷ������
{	
	uint8_t d;
	
	// ����־λ
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); // ��ձ�־λ
		d = USART_ReceiveData(USART2); // ��������
		g_recv_da[g_cnt ++] = d;
		
		USART_SendData(USART1,d);
		if((d == 0x00 | d == 0x01) && g_cnt == 3)
		{
			rt_kprintf("���㵽����\r\n");
			for(uint16_t i=0;i < 3;i++)
			{
				CD100M_uart_decode(g_recv_da[i], &g_CD100M_res_info, &g_CD100M_dbg_info, &g_CD100M_paras);	
			}
		}
		// ������յ�������
		
		
	}
}

void  uart2_send(uint8_t* msg,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		while (RESET == USART_GetFlagStatus(USART2, USART_FLAG_TXE));
        USART_SendData(USART2, msg[i]);
        while (RESET == USART_GetFlagStatus(USART2, USART_FLAG_TC));
		
	}
	// �����ʱ�ȴ��������
    delay_ms(10);
}

void uart3_init(uint32_t baud)
{
	
	//��PAӲ��ʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	

	//�򿪴���1Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//����PA9��PA10Ϊ���ù���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;		//��9 10������
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;	//�๦��ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������������������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//������Ӧ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//û��ʹ������������

	GPIO_Init(GPIOB,&GPIO_InitStructure);

    //��PB10��PB11�������ӵ�����1��Ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);	
	
	//���ô���3��ز����������ʡ���У��λ��8λ����λ��1��ֹͣλ......
	USART_InitStructure.USART_BaudRate = baud;										//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�����շ�����
	USART_Init(USART3, &USART_InitStructure);
	
	
	//���ô���3���жϴ�������������һ���ֽڴ����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	
	//���ô���3���ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//ʹ�ܴ���3����
	USART_Cmd(USART3, ENABLE);
}

void USART3_IRQHandler(void)                	//����1�жϷ������
{	
	uint8_t res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//���־λ
		res = USART_ReceiveData(USART3);//��ȡ����

		if(BC28_RX_FLAG == BC28_RX_OFF)
		{
			rt_timer_start(&bc28_10ms);
		}
		BC28_RX_FLAG = BC28_RX_ON;
		BC28_RX_BUF[BC28_RX_DATA_LEN++] = res;
	}
}

void USART3_Send_Data(uint8_t *data,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TXE));
		USART_SendData(USART3, data[i]);
	}
}

