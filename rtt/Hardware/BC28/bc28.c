#include "bc28.h"

rt_uint8_t BC28_RX_BUF[200] = {0};//BC28��������
rt_uint8_t BC28_RX_FLAG = BC28_RX_OFF;//BC28���ձ�־λ
rt_uint16_t BC28_RX_DATA_LEN = 0;//BC28�������ݳ���


/*
�ȴ���Ӧ��ack �ȴ���Ӧ�Ĳ����� waitTime �ȴ���Ӧʱ��
*/
static int BC28_Reply(char *ack,rt_uint16_t waitTime)
{
	if(rt_sem_take(BC28_Respond,waitTime) == 0)//�жϽ����Ƿ����
	{
		if(strstr((const char*)BC28_RX_BUF,ack))//�Ƚ���Ҫ�Ļ�Ӧֵ�Ƿ�ΪUSART3���յ����ݵ��Ӵ�
		{
			return SUCCESS;//�����յ���Ч��Ӧʱ�˳�ѭ��
		}
	}

	return ERROR;
}

/*
cmd����������
ack:�ȴ���Ӧ
frequency:�ȴ��Ĵ���
waitTime��ÿ�εȴ��Ľ���
*/
int BC28_Send_Check(char *cmd,char *ack,rt_uint8_t frequency,rt_uint16_t waitTime)
{	
	unsigned char MAX_Repeat = 10;
	
	unsigned char _time = frequency;
	
	
	while(MAX_Repeat--)
	{
		
		memset(BC28_RX_BUF, 0, 200);
			
		BC28_SEND_DATA("%s\r\n",cmd);//����ATָ��
		
		while(_time)
		{
			if(BC28_Reply(ack,waitTime) == SUCCESS)
				return SUCCESS;
			_time--;
		}
		_time = frequency;
	}		
	
	return ERROR;//����ò�����Ӧ �򷵻�ERROR
	
}


/*
�������������������Ϣ
������data��������������(JSON��ʽ)
����ֵ��SUCCESS���ɹ�  ERROR��ʧ��
��ע��
*/
int MQTT_PUB(char *data)
{
	char buf[200];
	
	int flag;
	
	sprintf(buf,"AT+QMTPUB=0,0,0,0,\"%s\"",PubTopic);
	
	if(!BC28_Send_Check(buf,">",2,300))return ERROR;
	
	
	BC28_SEND_DATA("%s\r\n",data);
	
	rt_thread_mdelay(50);
	
	BC28_SEND_DATA("%c",0x1A);	
	
	for(rt_uint8_t i = 0; i < 2; i ++)
	{
		flag = BC28_Reply("0,1,0",4000);
	}
	
	return flag;
}



/*
������BC28��ʼ������
������void
����ֵ��SUCCESS����ʼ���ɹ�  ERROR����ʼ��ʧ��
��ע��
*/
int BC28_Init(void)
{
	
	rt_kprintf("Start initializing BC28\n\n");
	
	//���ܣ�����BC28
	rt_kprintf("Restarting...\n\n");
	if(!(BC28_Send_Check("AT+NRB","OK",5,300)))return ERROR ;
	rt_kprintf("\nReboot successful\n\n");
	
	
	//���ܣ�����
	rt_kprintf("connecting...\n\n");
	if(!(BC28_Send_Check("AT","OK",1,300)))return ERROR;
	rt_kprintf("\nSuccessfully connected\n\n");
	
	//���ܣ�����UE��������
	rt_kprintf("Setting all UE features...\n\n");
	if(!(BC28_Send_Check("AT+CFUN=1","OK",5,600)))return ERROR;
	rt_kprintf("\nSuccessfully set\n\n");
	
	//���ܣ���ȡ����
	rt_kprintf("Getting CIMI...\n\n");
	if(!(BC28_Send_Check("AT+CIMI","460",1,300)))return ERROR;
	rt_kprintf("\nobtain\n\n");
	
	//���ܣ���������
	rt_kprintf("Activating network...\n\n");
	if(!(BC28_Send_Check("AT+CGATT=1","OK",1,300)))return ERROR;
	rt_kprintf("\nobtain\n\n");
	
	//���ܣ��鿴�Ƿ񼤻�ɹ�
	rt_kprintf("Checking if activation was successful...\n\n");
	if(!(BC28_Send_Check("AT+CGATT?","+CGATT:1",3,600)))return ERROR;
	rt_kprintf("\nSuccessfully activated network\n\n");
	
	rt_kprintf("BC28 initialization successful\r\n");
	
	return SUCCESS;//���سɹ�

}
/*
������MQTT��ʼ������
������void
����ֵ��SUCCESS����ʼ���ɹ�  ERROR����ʼ��ʧ��
��ע��
*/
int MQTT_Init(void)
{
	
	char Buf[100];
	
	rt_kprintf("Start initializing MQTT\n");
	
	//���ܣ������Ʒ���3Ԫ��
	rt_kprintf("Sending triplets to Aliyun...\n");
	memset(Buf,0,100);
	sprintf(Buf,"AT+QMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"", ProductKey, DeviceName, DeviceSecret);
	if(!BC28_Send_Check(Buf,"OK",1,1000))return ERROR ;
	rt_kprintf("\nSending completed\n");
	
	rt_thread_mdelay(2000);
	
	//���ܣ����Ӱ����Ʒ�����
	rt_kprintf("Connecting to Aliyun server...\n");
	memset(Buf,0,100);
	sprintf(Buf,"AT+QMTOPEN=0,\"%s\",%s", Domain, Port);
	if(!BC28_Send_Check(Buf,": 0,0",5,1000))return ERROR ;
	rt_kprintf("\nSuccessfully connected to Aliyun server\n");
	
	rt_thread_mdelay(2000);
	
	
	//���ܣ����Ӱ������豸
	rt_kprintf("Connecting device...\n");
	memset(Buf,0,100);
	sprintf(Buf,"AT+QMTCONN=0,\"%s\"", ConnectID);
	if(!BC28_Send_Check(Buf,"OK",2,1000))return ERROR ;
	rt_kprintf("\nSuccessfully connected device\n");
	
	//���ܣ���������
	rt_kprintf("Subscribing to topic...\n");
	memset(Buf,0,100);
	sprintf(Buf,"AT+QMTSUB=0,1,\"%s\",0", SubTopic);
	if(!BC28_Send_Check(Buf,"OK",2,1000))return ERROR ;
	rt_kprintf("\nSubscription successful\n");
	rt_kprintf("MQTT initialization successful\n");
	return SUCCESS;

}
