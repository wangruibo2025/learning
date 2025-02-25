#ifndef __CD100M_H
#define __CD100M_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "sys.h"
#include "usart.h"
#include "delay.h"

#define ONE_BYTE_ALIGNED __attribute__((packed, aligned(1)))

#define ID_GET_PARAS  0x2D 	//��ȡ������
#define ID_GET_STATUS 0x20 	//��ȡ״̬
#define ID_SET_PARA1  0x91  //��ˮ̫����
#define ID_SET_PARA2  0x92	//��������
#define ID_SET_PARA3  0x93	//������ֵ
#define ID_SET_PARA4  0x94  //��ˮ״̬
#define ID_SET_PARA5  0x95  //�б��˲�����
#define ID_SET_PARA6  0x96  //�ޱ��˲�����
#define ID_SET_PARA7  0x97  //Ԥ���˲�����
#define ID_SET_PARA8  0x98	//�����˲�����
#define ID_SET_PARA9  0x99	//T2Һ���ź���ֵ
#define ID_SET_PARA10 0x9A	//T2�����ź���ֵ
#define ID_SET_PARA11 0x80  //T1�����ź���ֵ

typedef enum {
    E_OK                    = 0,
    E_UNK                   = 1,
    E_PARA                  = 2,     /* �������� */
} tof_error_t;

typedef struct result_info {
    uint8_t  module_status;
    uint8_t  cup_status;
    uint16_t xtalk[8];          /**< \brief ���ţ�ȷ���ǰ����� */
    uint32_t id;                /**< \brief ģ��ID */
    uint16_t Revid;             /**< \brief ģ��̼��汾ID */
} result_info_t;

/*
	������Ϣ
*/
typedef union {
	struct{
		uint8_t   cup_status; /*����״̬ 0���ޱ��� 1���б� 2������ 3��Ԥ��*/
		uint16_t  bottom_dis; /*���׾���*/ 
		uint16_t  level_dis;  /*Һ�����*/
		uint16_t  edge_dis;	  /*���ؾ���*/
		uint16_t  T1_dis1;	  /*T1�⵽Ŀ����1����*/
		uint16_t  T1_dis2;	  /*T1�⵽Ŀ����2����*/
		uint16_t  T1_dis3;	  /*T1�⵽Ŀ����3����*/
		uint16_t  T2_dis1;	  /*T2�⵽Ŀ����1����*/
		uint16_t  T2_dis2;	  /*T2�⵽Ŀ����2����*/
		uint16_t  T2_dis3;	  /*T2�⵽Ŀ����3����*/
	}ONE_BYTE_ALIGNED  info;
	uint8_t             info_buf[19];
}debuge_info_t;
/*
	������
*/
typedef union{
	struct {
		uint16_t platform_dis;		//��ˮ̨����
		uint16_t work_max_dis;		//��������
		uint8_t	 remain_thr;		//�豸�ر��ؾ���߰�λ
		uint8_t  water_start;		//��ˮ״̬ 0��û��ˮ 1����ˮ
		uint8_t  no_cup_cnt;		//�б��˲�����
		uint8_t  get_cup_cnt;		//�ޱ��˲�����
		uint8_t  pre_full_cnt; 		//Ԥ���˲�����
		uint8_t  full_cup_cnt;		//�����˲�����
		uint16_t tof2_obj1_water;	//T2Һ���ź���ֵ
		uint16_t tof2_obj1_edge; 	//T2���ź���ֵ
		uint16_t tof1_obj1_edge; 	//T1���ź���ֵ
	}ONE_BYTE_ALIGNED paras;
	uint8_t 			REG_PARAS[16];
}cup_detect_para_t;

extern result_info_t       g_CD100M_res_info;   
extern debuge_info_t       g_CD100M_dbg_info;
extern cup_detect_para_t   g_CD100M_paras   ;
extern uint8_t 			   g_uart_sta;


uint8_t CD100M_set_platform_dis(uint16_t platform_dis); //����ƽ̨����
uint8_t CD100M_set_work_max_dis(uint16_t work_max_dis); //������������(Ĭ��400)
uint8_t CD100M_set_remain_thr (uint16_t remain_thr);	//����������ֵ
uint8_t CD100M_set_get_cup_cnt (uint8_t sta);			//�����б��˲�
uint8_t CD100M_set_no_cup_cnt (uint8_t sta);			//�����ޱ��˲�
uint8_t CD100M_set_pre_full_cnt (uint8_t sta);			//����Ԥ���˲�
uint8_t CD100M_set_full_cup_cnt (uint8_t sta);			//���������˲�
uint8_t CD100M_set_tof2_obj1_water (uint16_t tof2_obj1_water);  //����Һ���ź���ֵ
uint8_t CD100M_set_tof2_obj1_edge (uint16_t tof2_obj1_edge);    //����T2�����ź���ֵ
uint8_t CD100M_set_tof1_obj1_edge (uint16_t tof1_obj1_edge);    //����T1�����ź���ֵ
uint8_t CD100M_init (uint16_t iter, cup_detect_para_t *p_paras);// ��ʼ��
uint8_t CD100M_start (void);							//��������㷨
uint8_t CD100M_stop (void); 							//ֹͣ����㷨
uint8_t CD100M_hot_start (void);						//��������
uint8_t CD100M_hot_stop (void);							//ֹͣ����
uint8_t CD100M_get_cup_status(void);					//��ȡ�豸��ǰ״̬
uint8_t CD100M_get_debuge_info (void);					//��ȡ������Ϣ
uint8_t CD100M_get_detect_paras (void);					//��ȡ�豸������
uint8_t CD100M_get_xtalk (void) ;						//��ȡ�ǰ崮��ֵ
uint8_t CD100M_set_baud (uint32_t baud);				//���ò�����
uint8_t CD100M_sta_auto_en (void);						//ʹ�ܱ���״̬�Զ����
uint8_t CD100M_sta_auto_dis (void);						//���ܱ���״̬�Զ����
uint8_t CD100M_dbg_auto_en (void);						//ʹ�ܵ�����Ϣ�Զ����
uint8_t CD100M_dbg_auto_dis (void);						//���ܵ�����Ϣ�Զ����
uint8_t CD100M_set_water_status (uint8_t sta);			//ˮ״̬��������㷨
uint8_t CD100M_clr_cup_status (void);					//���ô��ڲ�����
uint8_t CD100M_get_id (void) ;							//��ȡid
void CD100M_uart_decode(uint8_t uart_rx,result_info_t *p_res_info,debuge_info_t *p_dbg_info, cup_detect_para_t *p_paras); //���ݴ���


#endif
