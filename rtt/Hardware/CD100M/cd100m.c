#include "cd100m.h"

static uint8_t g_cmd_get_id[]           =   {0x28,0x2C,0x00,0x00}; // ��ȡģ��ID   
static uint8_t g_cmd_get_RevID[]      	=  	{0x28,0x2F,0x00,0x00}; //��ȡ�̼��汾
static uint8_t g_cmd_get_debug_info[]  	=	{0x28,0x2E,0x00,0x00}; //��ȡ������Ϣ
static uint8_t g_cmd_get_paras[] 		= 	{0x28,0x2D,0x00,0x00}; //��ȡ������
static uint8_t g_cmd_get_modu_sta[]		=	{0x28,0x20,0x00,0x00}; //��ȡ״̬ 
static uint8_t g_cmd_get_cup_sta[]		=	{0x28,0x21,0x00,0x00}; //��ȡ����״̬
static uint8_t g_cmd_get_xtalk[]		=	{0x28,0x22,0x00,0x00}; //��ȡxtalk
static uint8_t g_cmd_hot_sta[]          = 	{0x28,0x9C,0x3C,0x00}; //�����ǰ���� 
static uint8_t g_cmd_hot_stop[]         = 	{0x28,0x9B,0x00,0x00}; //�رոǰ����

static uint8_t g_cmd_init[]             =   {0x28,0x9F,0x00,0x00}; //ģ���ʼ�� 
static uint8_t g_cmd_start_app[]        = 	{0x28,0x9E,0x00,0x00}; //��������㷨 
static uint8_t g_cmd_stop_app[]         =   {0x28,0x9D,0x00,0x00}; //ֹͣ����㷨 
static uint8_t g_cmd_set_iterat[]       =   {0x28,0x90,0x84,0x03}; //���õ������� 
static uint8_t g_cmd_set_para1[]        =   {0x28,0x91,0xE6,0x00}; //���ý�ˮ̨����(Ĭ��230) E6 Ĭ����230
static uint8_t g_cmd_set_para2[]        =   {0x28,0x92,0x90,0x01}; //������������(Ĭ��400)
static uint8_t g_cmd_set_para3[]        =   {0x28,0x93,0x1E,0x00}; //����������ֵ(Ĭ��30) 
static uint8_t g_cmd_set_para4[]        =   {0x28,0x94,0x00,0x00}; //���ó�ˮ״̬(Ĭ��1) 
static uint8_t g_cmd_set_para5[]        =   {0x28,0x95,0x05,0x00}; //�����б��˲�����(Ĭ��5) 
static uint8_t g_cmd_set_para6[]        =   {0x28,0x96,0x05,0x00}; //�����ޱ��˲�����(Ĭ��5) 
static uint8_t g_cmd_set_para7[]        =   {0x28,0x97,0x05,0x00}; //����Ԥ���˲�����(Ĭ��5) 
static uint8_t g_cmd_set_para8[]        =   {0x28,0x98,0x08,0x00}; //���������˲�����(Ĭ��8) 
static uint8_t g_cmd_set_para9[]        =   {0x28,0x99,0xa4,0x06}; //����Һ�沨����ֵ(Ĭ��1500)
static uint8_t g_cmd_set_para10[]       =   {0x28,0x9A,0xE8,0x03}; //���ñ��ز�����ֵ(Ĭ��1000) 
static uint8_t g_cmd_set_para11[]       =   {0x28,0x80,0x14,0x05}; //���ñ��ز�����ֵ(Ĭ��1300) 

static uint8_t g_cmd_sta_auto_en[]      =   {0x28,0xCF,0x00,0x00}; //ʹ�ܱ���״̬�Զ���� 
static uint8_t g_cmd_sta_auto_dis[]     =   {0x28,0xCE,0x00,0x00}; //���ܱ���״̬�Զ���� 
static uint8_t g_cmd_dbg_auto_en[]      =   {0x28,0xCD,0x00,0x00}; //ʹ�ܵ�����Ϣ�Զ���� 
static uint8_t g_cmd_dbg_auto_dis[]     =   {0x28,0xCC,0x00,0x00}; //���ܵ�����Ϣ�Զ���� 
static uint8_t g_cmd_set_baud[]         =   {0x28,0xC0,0x03,0x00}; //���ô��ڲ����� 
static uint8_t g_cmd_clr_cupsta[]       =   {0x28,0xC3,0x00,0x00}; //���ô��ڲ����� 

static uint8_t g_sta_auto_output = 0;          /**< ����״̬�Զ������־ */
static uint8_t g_dbg_auto_output = 0;          /**< ������Ϣ�Զ������־ */

result_info_t       g_CD100M_res_info;   
debuge_info_t       g_CD100M_dbg_info;
/*
	��ˮ̨�����������Ҫ�����޸�

*/
cup_detect_para_t   g_CD100M_paras = {
    .paras = {
        .platform_dis 		= 260,      //��ˮ̨���� mm
		.work_max_dis       = 400,      //�������� mm
        .remain_thr      	= 30,       //�豸�ر��ؾ���߰�λ
        .water_start    	= 0,        //��ˮ״̬ 0��û��ˮ 1����ˮ
        .no_cup_cnt         = 5,        //�б��˲�����
        .get_cup_cnt     	= 5,        //�ޱ��˲�����
        .pre_full_cnt    	= 5,        //Ԥ���˲�����
        .full_cup_cnt    	= 8,        //�����˲�����
        .tof2_obj1_water    = 1500,     //T2Һ���ź���ֵ
        .tof2_obj1_edge     = 1000,     //T2���ź���ֵ
        .tof1_obj1_edge     = 1300,     //T1���ź���ֵ
    }
};

uint8_t g_uart_sta = 0;

static uint8_t write_cmd_wait(uint8_t *p_cmd)
{
	uint32_t cnt_10ms = 0;

	rt_enter_critical();

	uart2_send(p_cmd,4);

	rt_exit_critical();
	
	while(1)
	{
		if(g_uart_sta & 0x80)
		{
		
			if(g_CD100M_res_info.module_status & 0x0F) {
                
                return 0xFF;
            }
			
			g_uart_sta = 0;
			
			break;
			
		}
		delay_ms(10);
		
		cnt_10ms++;
		
		if (cnt_10ms > 100) {   /* 100ms��ʱ */
            return 0xFF;
			//printf("�������������к�:%d\r\n", __LINE__);
        }
		//printf("�������������к�:%d\r\n", __LINE__);
	}
	//delay_ms(30);
	//printf("�������������к�:%d\r\n", __LINE__);
	return g_CD100M_res_info.module_status;
	
}
/*
		�������豸����㷨ǰ��һ��Ҫ����ʵ�ʰ�װ��������ˮ̨���豸�ľ��룬��λ�� mm��
	Ȼ��ͨ������ 91 ��������뷢�͸��豸������ʵ�ʽ�ˮ̨���豸���� 230mm��ת��Ϊ 16
	������ E6mm��
	��ϸ�������� 3.8��
*/
uint8_t CD100M_set_platform_dis(uint16_t platform_dis)
{
	uint8_t status = 0;
	g_cmd_set_para1[2] = (uint8_t)(platform_dis & 0x00FF);
	g_cmd_set_para1[3] = (uint8_t)(platform_dis  >> 8);
	status += write_cmd_wait(g_cmd_set_para1);
	if (status & 0x0F) {
        return status;
    }
	
	return 0;
}
/*
		�豸Ĭ����������Ϊ 400mm����ʾ���� 400mm �����Ŀ�겻�ᱻ�豸��⵽��һ
	����˵���Ĭ��ֵ������������ˮ��Ӧ�ã����Ըò���һ�������������á�����Ҫ���ò���
	����Ϊ 350mm��ת��Ϊ 16 ������ 15Emm��ʾ������������ 3.21 ��ʾ���豸���صĲ�����
	Bit2~Bit0 ��ʾ����������н��
	��ϸ�������� 3.8��
*/
uint8_t CD100M_set_work_max_dis(uint16_t work_max_dis)
{
	uint8_t status = 0;
	g_cmd_set_para2[2] = (uint8_t)(work_max_dis & 0x00FF);
	g_cmd_set_para2[3] = (uint8_t)(work_max_dis  >> 8);
	status +=write_cmd_wait(g_cmd_set_para2);
	if (status & 0x0F) {
        return status;
    }
	return 0;
}
/*
		����������ֵ
		�豸Ĭ��������ֵΪ 30mm����ʾ�豸�⵽Һ����뱭��С�� 30mm ʱ����Ϊ�����Ѿ�
	װ��ˮ��ʵ���豸���������ź�ʱ��Һ���뱭�ؾ��벻һ��Ϊ 30mm��һ����˵��С�� 30mm��
	ԭ���ж���һ���豸����������ؾ����Һ���������������豸�ڽ���״̬�л���ʱ
	���ж���˲�������֤״̬�л����ȶ��ԣ�����Ҳ�����״̬�л����ӳ١�����Ҫ���ò�
	������Ϊ 20mm��ת��Ϊ 16 ������ 14mm��ʾ��������� 3.22 ��ʾ���豸���صĲ�����
	Bit2~Bit0 ��ʾ����������н��
*/
uint8_t CD100M_set_remain_thr (uint16_t remain_thr)
{
	uint8_t status = 0;
    g_cmd_set_para3[2] = remain_thr;
    status += write_cmd_wait(g_cmd_set_para3);
	if (status & 0x0F) {
        return status;
    }
	return 0;
}
/*
		�豸Ĭ���б��˲�����Ϊ 5����ʾ���� 5 ���ж������б�����ʱ����������б���״̬��
	�ò���ԽС�������б�״̬����ӦԽ�쵫��������Ҳ��Խ�������������� 1���ò���һ
	�������������Ĭ��ֵ���ɡ�ʾ��������� 3.24 ��ʾ���豸���صĲ����� Bit2~Bit0 ��ʾ��
	��������н������ϸ�������� 3.8��
*/
uint8_t CD100M_set_get_cup_cnt (uint8_t sta)
{
    uint8_t status = 0;
    g_cmd_set_para5[2] = sta;
    status =write_cmd_wait(g_cmd_set_para5);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		�豸Ĭ���ޱ��˲�����Ϊ 5����ʾ���� 5 ���ж������ޱ�����ʱ����������ޱ���״̬��
	�ò���ԽС�������ޱ�״̬����ӦԽ�쵫��������Ҳ��Խ�������������� 1���ò���һ
	�������������Ĭ��ֵ���ɡ�ʾ��������� 3.25 ��ʾ���豸���صĲ����� Bit2~Bit0 ��ʾ��
	��������н������ϸ�������� 3.8��
*/
uint8_t CD100M_set_no_cup_cnt (uint8_t sta)
{
    uint8_t status = 0;
    g_cmd_set_para6[2] = sta;
    write_cmd_wait(g_cmd_set_para6);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		�豸Ĭ��Ԥ���˲�����Ϊ 5����ʾ���� 5 ���ж�����Ԥ������ʱ���Ž���Ԥ��״̬����
	״̬��ʾ���ӿ�Ҫ���ˣ�������������ò���ԽС������Ԥ��״̬����ӦԽ�쵫��������Ҳ
	��Խ�������������� 1���ò���һ�������������Ĭ��ֵ���ɡ�ʾ��������� 3.26 ��ʾ��
	�豸���صĲ����� Bit2~Bit0 ��ʾ����������н��
*/
uint8_t CD100M_set_pre_full_cnt (uint8_t sta)
{
    uint8_t status = 0;
    g_cmd_set_para7[2] = sta;
    status = write_cmd_wait(g_cmd_set_para7);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		�豸Ĭ�������˲�����Ϊ 8����ʾ���� 8 ���ж�������������ʱ���������������״̬��
	�ò���ԽС����������״̬����ӦԽ�쵫��������Ҳ��Խ�������������� 1���ò���һ
	�������������Ĭ��ֵ���ɡ�ʾ��������� 3.27 ��ʾ���豸���صĲ����� Bit2~Bit0 ��ʾ��
	��������н������ϸ�������� 3.8��
*/
uint8_t CD100M_set_full_cup_cnt (uint8_t sta)
{
    uint8_t status = 0;
    g_cmd_set_para8[2] = sta;
	status =write_cmd_wait(g_cmd_set_para8);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		�豸Ĭ��Һ���ź���ֵΪ 1500����ʾ�������� T2 ��⵽Һ���ź�ǿ�ȸ��� 1500 ʱ��
	��ΪҺ���ź���Ч���ò�����С�ᵼ�²��ֱ��ӽӲ���ˮ������ᵼ�²��ֱ���������ò�
	��һ�������������Ĭ��ֵ���ɡ�
*/
uint8_t CD100M_set_tof2_obj1_water (uint16_t tof2_obj1_water)
{
    uint8_t status = 0;
    g_cmd_set_para9[2] = (uint8_t)(tof2_obj1_water & 0x00FF);
    g_cmd_set_para9[3] = (uint8_t)(tof2_obj1_water >> 8);
    status +=write_cmd_wait(g_cmd_set_para9);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		�豸Ĭ�ϱ����ź���ֵΪ 1000����ʾ�������� T2 ��⵽�����ź�ǿ�ȵ��� 1000 ����
	Ϊ�����ź���Ч���ò����롰T1 �����ź���ֵ�����ʹ�����Ʊ��Ӱڷ�λ�ã��ò���ԽС��
	���ӵİڷ�λ��Ҫ��Խ���̡��ò���һ�������������Ĭ��ֵ���ɡ�
*/
uint8_t CD100M_set_tof2_obj1_edge (uint16_t tof2_obj1_edge)
{
    uint8_t status = 0;
    g_cmd_set_para10[2] = (uint8_t)(tof2_obj1_edge & 0x00FF);
    g_cmd_set_para10[3] = (uint8_t)(tof2_obj1_edge >> 8);
    status += write_cmd_wait(g_cmd_set_para10);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		�豸Ĭ�ϱ����ź���ֵΪ 1200����ʾ�������� T1 ��⵽�����ź�ǿ�ȸ��� 1200 ����
	Ϊ�����ź���Ч���ò����롰T2 �����ź���ֵ�����ʹ�����Ʊ��Ӱڷ�λ�ã��ò���Խ���
	���ӵİڷ�λ��Ҫ��Խ���̡��ò���һ�������������Ĭ��ֵ���ɡ�
*/
uint8_t CD100M_set_tof1_obj1_edge (uint16_t tof1_obj1_edge)
{
    uint8_t status = 0;
    g_cmd_set_para11[2] = (uint8_t)(tof1_obj1_edge & 0x00FF);
    g_cmd_set_para11[3] = (uint8_t)(tof1_obj1_edge >> 8);
    status += write_cmd_wait(g_cmd_set_para11);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
	��ʼ��
*/
uint8_t CD100M_init (uint16_t iter, cup_detect_para_t *p_paras)
{
	uint8_t  status = 0;

	/* ��ѯCD100M��ʼ�����״̬ */
	status = write_cmd_wait(g_cmd_init);
	if (status & 0x0F) {
        return status;
    }
	//------------------------------------------
	/* ���õ������� */
	g_cmd_set_iterat[2] = (uint8_t)(iter & 0x00FF);
	g_cmd_set_iterat[3] = (uint8_t)(iter >> 8);
	status += write_cmd_wait(g_cmd_set_iterat);
	if (status & 0x0F) {
        return status;
    }
	/* ���ò���1~10 */
	g_cmd_set_para1[2] = (uint8_t)(p_paras->paras.platform_dis & 0x00FF);
	g_cmd_set_para1[3] = (uint8_t)(p_paras->paras.platform_dis >> 8);
	status += write_cmd_wait(g_cmd_set_para1);
	if (status & 0x0F) {
        return status;
    }
	
	g_cmd_set_para2[2] = (uint8_t)(p_paras->paras.work_max_dis & 0x00FF);
	g_cmd_set_para2[3] = (uint8_t)(p_paras->paras.work_max_dis >> 8);
	status += write_cmd_wait(g_cmd_set_para2);
	if (status & 0x0F) {
        return status;
    }
	
	g_cmd_set_para3[2] = p_paras->paras.remain_thr;
	status += write_cmd_wait(g_cmd_set_para3);
	if (status & 0x0F) {
        return status;
    }
	
	g_cmd_set_para4[2] = p_paras->paras.water_start;
	status += write_cmd_wait(g_cmd_set_para4);
	if (status & 0x0F) {
		return status;
	}
	g_cmd_set_para5[2] = p_paras->paras.get_cup_cnt;
	status += write_cmd_wait(g_cmd_set_para5);
	if (status & 0x0F) {
		return status;
	}
	g_cmd_set_para6[2] = p_paras->paras.no_cup_cnt;
	status += write_cmd_wait(g_cmd_set_para6);
	if (status & 0x0F) {
		return status;
	}
	g_cmd_set_para7[2] = p_paras->paras.pre_full_cnt;
	status += write_cmd_wait(g_cmd_set_para7);
	if (status & 0x0F) {
		return status;
	}
	g_cmd_set_para8[2] = p_paras->paras.full_cup_cnt;
	status += write_cmd_wait(g_cmd_set_para8);
	if (status & 0x0F) {
		return status;
	}
	g_cmd_set_para9[2] = (uint8_t)(p_paras->paras.tof2_obj1_water & 0x00FF);
	g_cmd_set_para9[3] = (uint8_t)(p_paras->paras.tof2_obj1_water >> 8);
	status += write_cmd_wait(g_cmd_set_para9);
	if (status & 0x0F) {
		return status;
	}
	g_cmd_set_para10[2] = (uint8_t)(p_paras->paras.tof2_obj1_edge & 0x00FF);
	g_cmd_set_para10[3] = (uint8_t)(p_paras->paras.tof2_obj1_edge >> 8);
	status += write_cmd_wait(g_cmd_set_para10);
	if (status & 0x0F) {
		return status;
	}
	g_cmd_set_para11[2] = (uint8_t)(p_paras->paras.tof1_obj1_edge & 0x00FF);
	g_cmd_set_para11[3] = (uint8_t)(p_paras->paras.tof1_obj1_edge >> 8);
	status += write_cmd_wait(g_cmd_set_para11);
	
	if (status & 0x0F) {
		return status;
	}
	
	//-------------------------------------------
    return 0;
}
/*
		������������ 9E ��������㷨��Ȼ������豸���صĲ����ж��豸�Ƿ�ɹ��������
	�㷨����������㷨��ʾ��������� 3.15 ��ʾ���豸���صĲ����� Bit2~Bit0 ��ʾ�������
	���н����
*/
uint8_t CD100M_start (void)
{
    uint8_t status = 0;
    status =write_cmd_wait(g_cmd_start_app);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		������������ 9D ֹͣ����㷨��Ȼ������豸���صĲ����ж��豸�Ƿ�ɹ�ֹͣ���
	�㷨��ֹͣ����㷨��ʾ��������� 3.16 ��ʾ���豸���صĲ����� Bit2~Bit0 ��ʾ�������
	���н����
*/
uint8_t CD100M_stop (void)
{
    uint8_t status = 0;
	status =write_cmd_wait(g_cmd_stop_app);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		�豸�ǰ�Ϊ���粣�������ڳ���������ʹ��ʱ����ͨ�����ȸǰ��ֹ����ˮ��Ӱ�칦
	�ܡ������������� 9C �������ȣ�������� 1 ����������ʾ����Ŀ���¶ȣ���Ŀ���¶ȵ�ȡ
	ֵ��Χ�� 30��~70�档Ȼ������豸���صĲ����ж��豸�Ƿ�ɹ��������ȡ�
	����Ҫ���ü��ȵ���Ĥ�� 60�棬ת��Ϊ 16 ���ƣ���Ϊ 3C�档�ǾͿ��Է��� 28 9C 3C 00
	������á������������ȵ�ʾ��������� 3.17 ��ʾ���豸���صĲ����� Bit2~Bit0 ��ʾ����
	������н��
*/
uint8_t CD100M_hot_start (void)
{
//    uint8_t status = 0;
    write_cmd_wait(g_cmd_hot_sta);
    return 0;
}
/*
		������Ҫ���ȸǰ�ʱ������������ 9B ����ֹͣ���ȣ�Ȼ������豸���صĲ����ж���
	���Ƿ�ɹ�ֹͣ���ȡ�ֹͣ���ȵ�ʾ��������� 3.18 ��ʾ���豸���صĲ����� Bit2~Bit0 ��
	ʾ����������н����
*/
uint8_t CD100M_hot_stop (void)
{
    uint8_t status = 0;
    status =write_cmd_wait(g_cmd_hot_stop);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		���豸��������֡ǰ�󣬶��б�Ҫ���豸��״̬���в�ѯ�������ɷ������� 20 ����ȡ
	�豸��ǰ״̬��
*/
uint8_t CD100M_get_cup_status(void)
{
	 uint8_t  status     =  0;
    //printf("��ǰ�����к�:%d\r\n", __LINE__);
    if (g_sta_auto_output) {      /* ʹ�����Զ���� */
		//printf("��ǰ�����к�:%d\r\n", __LINE__);
        if (g_uart_sta & 0x80) {
			//printf("��ǰ�����к�:%d\r\n", __LINE__);
            g_uart_sta = 0;
        } else {
			//printf("��ǰ�����к�:%d\r\n", __LINE__);
            return 0xFF;
        } 
    } else {
     
        /* �ж�TOF����Ƿ������� */
		//printf("��ǰ�����к�:%d\r\n", __LINE__);
        /*status = write_cmd_wait(g_cmd_get_modu_sta);
		//printf("��ǰ�����к�:%d\r\n", __LINE__);
        if (!(status & 0x10)) {    // �ж�bit4 
			printf("�豸δ����:%d\r\n", __LINE__);			
            return status;
        }
		printf("�豸�����Ѿ�����:%d\r\n", __LINE__);	
        //printf("��ǰ�����к�:%d\r\n", __LINE__);   */   
        /* ��ȡ������/�� */
        write_cmd_wait(g_cmd_get_cup_sta);
        if (status & 0x0F) {
			//printf("��ǰ�����к�:%d\r\n", __LINE__);
            return status;
        }
    }
	//printf("��ǰ�����к�:%d\r\n", __LINE__);
    return 0;
}
/*
		������Ϣ���豸�ڵ��Խ׶ι۲����Ϣ����Щ��Ϣ���豸�İ�װ��������ָ�������á�
	����Խṹ�ڱ��Ƿ���豸��Ӱ��ʱ�����޷����κα��ӵ���������´�ӡ��������Ϣ����
	�� T1 �ľ�����Ϣ�Ƿ�ֻ��һ����ˮ̨���룬�������ڱڽṹ��Ӱ��
	������������ 2E ��ȡ������Ϣ���豸���صĵ�����Ϣ������ 19 ���ֽڹ��ɡ���ȡ��
	����Ϣ��ʾ��������� 3.2 ��ʾ���豸���صĵ�����Ϣ����������� 3.3 ��ʾ��
*/
uint8_t CD100M_get_debuge_info (void) 
{
	uint8_t  status     =  0;
    
    if (g_dbg_auto_output) {     /* ʹ�����Զ���� */
        if (g_uart_sta & 0x80) {
            g_uart_sta = 0;
        } else {
            #if CD100M_TestDebug
                printf("__g_dbg_auto_output 0xFF\r\n");
            #endif
            return 0xFF;
        } 
    } else {
     
        /* �ж�TOF����Ƿ������� */
        status = write_cmd_wait(g_cmd_get_modu_sta);
        if ((status & 0xD0) != 0xD0) {    /* �ж�bit4 */
            return status;
        }

        /* ��ȡ������Ϣ */
        status = write_cmd_wait(g_cmd_get_debug_info);
        if (status & 0x0F) {
            return status;
        }
    }
    return 0;
}	
/*
		�豸���ܼ���㷨֮ǰ�����Ƚ����������õļ�������ͬʱ����Ҳ���Է������� 2D
	��ȡ��Щ��������ȡ�豸��������ʾ��������� 3.4 ��ʾ���豸���صļ�������������
	��� 3.5 ��ʾ��
*/
uint8_t CD100M_get_detect_paras (void) 
{
    uint8_t status = 0;
    
    status =write_cmd_wait(g_cmd_get_paras);
   if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		��ȡ�ǰ崮��ֵ
	����ֵָʾ�ǰ尲װ�û��̶ȣ����豸װ�ϸǰ��Ҫ��֤����ֵ�� 200~6000 ֮�䣨��
	������Ϊ 900k ʱ����
	�豸��������㷨�������ɷ������� 22 ����ȡ�ǰ崮��ֵ��ע�⣺�豣֤�����豸
	40cm ��û��Ŀ���40~100cm ����Ŀ�������ȡ�ǰ崮��ֵ��ʾ��������� 3.11 ��ʾ��
	�豸���صĴ�������������� 3.12 ��ʾ��
*/
uint8_t CD100M_get_xtalk (void) 
{
    uint8_t status = 0;
    status =write_cmd_wait(g_cmd_get_xtalk);  
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
		������ͨ������ C0 �����豸 UART ͨ�Ų����ʣ�������֡�Ĳ��� 1 ��������ͨ�Ų���
	�ʣ������������ 3.35 ��ʾ������ 2 Ϊ 0���豸�ϵ���²����ʲ���ʧ��
*/
uint8_t CD100M_set_baud (uint32_t baud)
{
	uint8_t  status   = 0;
    uint8_t  val      = 0;
    uint32_t cnt_10ms = 0;
    
    switch (baud) {
        case 2400:   val = 1; break;
        case 4800:   val = 2; break;
        case 9600:   val = 3; break;
        case 56000:  val = 4; break;
        case 115200: val = 5; break;
        default: break;
    }
    
    g_cmd_set_baud[2] = val;
     
    uart2_send(g_cmd_set_baud, 4);         /* �������� */
    uart2_init(baud);                  /* ������������������ */
    g_uart_sta = 0;
    
    while (1) {                             /* �ȴ����ڽ��յ����� */
        if (g_uart_sta & 0x80) {
            g_uart_sta = 0;
            break;
        }
        delay_ms(10);
        cnt_10ms++;
        if (cnt_10ms > 200) {               /* ��ʱ */
            #if CD100M_TestDebug
            printf("tof10x_set_baud tmo!\r\n");
            #endif
            return 0xFF;
        }
    }
    
    if (g_CD100M_res_info.module_status & 0x0F) {
        return status;
    }
    return 0;
	
}
/*
		������ͨ������ CF ʹ�ܱ���״̬�Զ���������豸���µ�����״̬ʱ���Զ������ݷ�
	�͸����������ͱ���״̬��ʽ�� 3.1.6��ʹ�ܱ���״̬�Զ������ʾ��������� 3.31 ��ʾ��
*/
uint8_t CD100M_sta_auto_en (void)
{
	uint8_t status = 0;
    
    status = write_cmd_wait(g_cmd_sta_auto_en);
    if (status & 0x0F) {
        return status;
    }
    g_sta_auto_output = 1;    
    return 0;
}
/*
	���ܱ���״̬�Զ������ʾ��������� 3.32 ��ʾ
*/
uint8_t CD100M_sta_auto_dis (void)
{
    uint8_t status = 0;
    write_cmd_wait(g_cmd_sta_auto_dis);
	if (status & 0x0F) {
        return status;
    }
    g_sta_auto_output = 0; 
    return 0;
}
/*
		������ͨ������ CD ʹ�ܵ�����Ϣ�Զ���������豸���µ�������Ϣʱ���Զ������ݷ�
	�͸����������͵ĵ�����Ϣ��ʽ�� 3.1.2��ʹ�ܵ�����Ϣ�Զ������ʾ��������� 3.33 ��ʾ
*/
uint8_t CD100M_dbg_auto_en (void)
{
    uint8_t status = 0;
    status =write_cmd_wait(g_cmd_dbg_auto_en);
	if (status & 0x0F) {
        return status;
    }
    g_dbg_auto_output = 1;    
    return 0;
}
/*
	���ܵ�����Ϣ�Զ������ʾ��������� 3.34 ��ʾ��
*/
uint8_t CD100M_dbg_auto_dis (void)
{
    uint8_t status = 0;
    status =write_cmd_wait(g_cmd_dbg_auto_dis);
	 if (status & 0x0F) {
        return status;
    }
    g_dbg_auto_output = 0; 
    return 0;
}
/*
		�豸Ҫ������ˮ���ĳ�ˮ״̬��������㷨����˵���ˮ��Ҫ���г�ˮ��ͣˮ��״̬��
	��ʱ��Ҫ����������״̬���͸��豸�������豸��⵽�б������û����³�ˮ��ťʱ������
	Ҫ���� 1����ʾ��ˮ������Ҫ��ˮ�����豸��ע�⴫���������Ϊ 0 �� 1��
	
*/
uint8_t CD100M_set_water_status (uint8_t sta)
{
    uint8_t status = 0;
    g_cmd_set_para4[2] = sta;
    status = write_cmd_wait(g_cmd_set_para4);
	if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
	���ô��ڲ�����
*/
uint8_t CD100M_clr_cup_status (void)
{
    uint8_t status = 0;
	status = write_cmd_wait(g_cmd_clr_cupsta);
	 if (status & 0x0F) {
        return status;
    }
    return 0;
}
/*
	g_cmd_get_RevID
		�����ɷ������� 2F ����ȡ�豸�Ĺ̼��汾�ţ���ȡ�豸�̼��汾��ʾ��������� 3.7
	��ʾ���豸�Ĺ̼��汾��Ϣ�������ֽڹ��ɣ�ǰһ���ֽڵĵ� 4 λ�����޶��ţ��� 4 λ����
	�ΰ汾�ţ���һ���ֽڵĵ� 4 λ�������汾�ţ��� 4 λ�����豸�ͺţ�CD100 Ϊ 0����
	��������������ֽڷֱ�Ϊ 16 ���Ƶ� 01 01�����ʾ���豸�ͺ�Ϊ CD100���̼��汾
	Ϊ V1.0.1��
	g_cmd_get_id
		���豸��������֡ǰ�󣬶��б�Ҫ���豸��״̬���в�ѯ�������ɷ������� 20 ����ȡ
	�豸��ǰ״̬����ȡ�豸״̬��ʾ��������� 3.7 ��ʾ���豸���ص�״̬����������� 3.8
	��ʾ��
*/
uint8_t CD100M_get_id (void) 
{
  uint8_t status = 0;
	//printf("��ǰ�����к�:%d\r\n", __LINE__);
    status =write_cmd_wait(g_cmd_get_RevID);
	if (status & 0x0F) {
        return status;
    }
	//printf("��ǰ�����к�:%d\r\n", __LINE__);
    status =write_cmd_wait(g_cmd_get_id);
	if (status & 0x0F) {
        return status;
    }
	//printf("��ǰ�����к�:%d\r\n", __LINE__);
    return 0;
}
/*
	���ڽ��룺
		���ݻ�ȡ����������
*/

void CD100M_uart_decode(uint8_t uart_rx,result_info_t *p_res_info,debuge_info_t *p_dbg_info, cup_detect_para_t *p_paras)
{
	
	static uint8_t frame_id       =  0;             /* ֡��ʶ */
    static uint8_t frame_data[19] = {0};            /* ֡���� */
    static uint8_t data_len       =  0;             /* ֡���ݳ��� */
    static uint8_t data_cnt       =  0;             /* ����֡���ݼ��� */
	
    if ((g_uart_sta & 0x80) == 0) {                 /* ����δ��� */
		
        if (g_uart_sta & 0x40) {                    /* ���յ���֡ͷ */
			//rt_kprintf("���������к�:%d------%d\r\n", __LINE__,uart_rx);
            if (g_uart_sta & 0x20) {                /* ���յ���֡��ʶ */
				//rt_kprintf("���������к�:%d------%d\r\n", __LINE__,uart_rx);
                frame_data[data_cnt] = uart_rx;     /* �洢֡���� */
                data_cnt++;                         /* ���յ���֡���ݼ�1 */
                if (data_cnt == data_len) {         /* ����������֡���� */
					//rt_kprintf("���������к�:%d------%d\r\n", __LINE__,uart_rx);
                    data_cnt = 0;
                    
                    g_uart_sta |= 0x80;             /* �����֡���ݽ������ */
                    switch (frame_id) {             /* ���� */
                        case 0x20:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x9F:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x9E:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x9D:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x90:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x91:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x92:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x93:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x94:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x95:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x96:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x97:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x98:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x99:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x9A:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x80:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0xCF:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0xCE:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0xCD:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0xCC:
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0xC0: 
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0xC1: 
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0xC2: 
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0xC3: 
                            p_res_info->module_status = frame_data[0];
                            break;
                        case 0x2C:
                            p_res_info->id = (frame_data[3] << 24) | frame_data[2] << 16 | frame_data[1] << 8 | frame_data[0];
                            break;
                        case 0x2F:
                            p_res_info->Revid = ((uint16_t)frame_data[1] << 8) | frame_data[0];
                            break;
                        case 0x2E:
                            for (uint8_t i = 0; i < 19; i++) {
                                p_dbg_info->info_buf[i] = frame_data[i];
                            }
                            break;
                        case 0x2D:
                            for (uint8_t i = 0; i < 16; i++) {
                                p_paras->REG_PARAS[i] = frame_data[i];
                            }
                            break;
                        case 0x21:                      
                            p_res_info->cup_status = frame_data[0];
							rt_kprintf("--------------���������к�:%d------%d----------%d\r\n", __LINE__,uart_rx,frame_data[0]);
                            break;
                        case 0x22:                   
                            for (uint8_t i = 0; i < 8; i++) {
                                p_res_info->xtalk[i] = ((uint16_t)frame_data[2*i+1] << 8) | frame_data[2*i];
                            }
                            break;
                    }
                }
            } else {                                /* ֮ǰû���յ�֡��ʶ */
				
                switch (uart_rx) {
                    case 0x20:                      /* ֡���ݳ���Ϊ1 */ 
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x21:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
						//rt_kprintf("���������к�:%d------%d\r\n", __LINE__,uart_rx);
                        break;
                    case 0x9F:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x9E:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x9D:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x9C:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x9B:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x90:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x91:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x92:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x93:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x94:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x95:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x96:
                        frame_id = uart_rx;
                        data_len = 1;
                        g_uart_sta |= 0x20;
                        break;
                    case 0x97:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x98:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x99:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x9A:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x80:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0xCF:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0xCE:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0xCD:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0xCC:
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0xC0: 
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0xC1: 
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0xC2: 
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0xC3: 
                        frame_id = uart_rx;
                        data_len = 1; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x2F:                      /* ֡���ݳ���Ϊ2 */
                        frame_id = uart_rx;
                        data_len = 2; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x2E:                        
                        frame_id = uart_rx;
                        data_len = 19; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x2D:
                        frame_id = uart_rx;
                        data_len = 16; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x2C:                      /* ֡���ݳ���Ϊ2 */
                        frame_id = uart_rx;
                        data_len = 4; 
                        g_uart_sta |= 0x20;
                        break;
                    case 0x22:
                        frame_id = uart_rx;
                        data_len = 16; 
                        g_uart_sta |= 0x20;
                        break;
                    default  :                      /* ���մ������½��� */
						//rt_kprintf("���մ������½���:%d------%d\r\n", __LINE__,uart_rx);
                        g_uart_sta = 0;        
                        data_cnt   = 0;
                }
            }
        } else {                                    /* ֮ǰû���յ�֡ͷ */
			//rt_kprintf("���������к�:%d------%d\r\n", __LINE__,uart_rx);
            if (uart_rx == 0x28) {                  /* ���յ�0x28 */
                g_uart_sta |= 0x40;                 /* ��ǽ��յ���֡ͷ */
				//rt_kprintf("���������к�:%d------%d\r\n", __LINE__,uart_rx);
            }
        }
    }


}

