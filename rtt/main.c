#include "app.h"



rt_thread_t start = NULL;
#define Test 1
#define SHIZHAN 0
void APP_Start(void *parameter)
{
	
	
	//��̬�ź�������
	BC28_Respond = rt_sem_create("BC28_Respond",0,RT_IPC_FLAG_FIFO);//�����Ƚ��ȳ���ʽ��ȡ��Դ
	CD100M_RECV  = rt_sem_create("CD100M_RECV",0,RT_IPC_FLAG_FIFO);//�����Ƚ��ȳ���ʽ��ȡ��Դ
	g_mail_data =  rt_mb_create("my_mailbox", 1 , RT_IPC_FLAG_FIFO);
	//printf("��ǰ�����к�:%d\r\n", __LINE__);
	//��̬Ӳ��ʱ����
	/*rt_timer_init(&bc28_10ms,"bc28_10ms",bc28_callback,NULL,10,//1sִ��һ�� 
	RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_HARD_TIMER);//һ���� Ӳ��ʱ */
	
	//�����ʼ��
	//rt_thread_init(&th1,"MQTT_INIT",MQTT_INIT,NULL,th1_stack,sizeof(th1_stack),th1_test_priority,5);
	//rt_thread_init(&th2,"MQTT_PUB_DATA",MQTT_PUB_DATA,NULL,th2_stack,sizeof(th2_stack),th2_test_priority,5);
	rt_thread_init(&th3,"CD100M_INFO",CD100M_INFO,NULL,th3_stack,sizeof(th3_stack),th3_test_priority,5);
	//rt_thread_init(&th4,"CD100M_SEM_POST",CD100M_CLEAR,NULL,th4_stack,sizeof(th4_stack),th4_test_priority,5);
	//printf("��ǰ�����к�:%d\r\n", __LINE__);
	//��������
	rt_thread_startup(&th3);
	//rt_thread_startup(&th4);
}
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	usart_init(115200);  	//��ʼ�����Դ���
	uart2_init(9600);		//��ʼ��CD100M����
	
	//uart3_init(9600);		//��ʼ��BC28���� û�а��� 
	
	//Init_Hx711();			//���س�ʼ��
#if 1	
	//��ʼ����
	start = rt_thread_create("Task Start",APP_Start,NULL,1024,20,5);
	rt_thread_startup(start); //��������
#endif	
#if  0
	uint8_t g_cmd[]		=	{0x28,0x21,0x00,0x00};
	uint8_t g_cmd_init[]             =   {0x28,0x9F,0x00,0x00}; //ģ���ʼ�� 
	uint8_t g_cmd_start_app[]        = 	{0x28,0x9E,0x00,0x00}; //��������㷨 
	rt_enter_critical();
		uart2_send(g_cmd_init,4);
		rt_exit_critical();
	delay_ms(1000);
	
	rt_enter_critical();
		uart2_send(g_cmd_start_app,4);
		rt_exit_critical();
	delay_ms(1000);
	while(1)
	{
		g_uart_sta=0;
		rt_enter_critical();
		uart2_send(g_cmd,4);
		rt_exit_critical();
		delay_ms(2000);
		
	}
#endif
}







