//struct.h

#pragma once

//λ�ö���
#define PS_BTX_START					346											//��ʼ��Ťx����
#define PS_BTY_START					400											//��ʼ��Ťy����

#define PS_BTX_HU						698											//���ư�Ťx����
#define PS_BTY_HU						442											//���ư�Ťy����
//#define PS_BTX_TING						160											//
//#define PS_BTY_TING						180											//
#define PS_BTX_GANG						671											//���ư�Ťx����
#define PS_BTY_GANG						442											//���ư�Ťy����
#define PS_BTX_PENG						644											//���ư�Ťx����
#define PS_BTY_PENG						442											//���ư�ťy����
#define PS_BTX_CHI						617											//���ư�ťx����
#define PS_BTY_CHI						442											//���ư�ťy����
#define PS_BTX_GIVEUP					739											//������ťx����
#define PS_BTY_GIVEUP					442											//������ťy����

#define PS_BTX_BTBACK					600											//��Ť����
#define PS_BTY_BTBACK					433											//��Ť����

#define PS_BTX_SHOWMINI					730											//��С����Ťx����
#define PS_BTY_SHOWMINI					10											//��С����Ťy����
#define PS_BTX_CANCEL					750											//�رհ�Ťx����
#define PS_BTY_CANCEL					10											//�رհ�Ťy����
#define PS_BTX_GAMESET					690											//���ð�Ťx����
#define PS_BTY_GAMESET					10											//���ð�Ťy����

//#define PS_BTX_HU						160											//���ư�Ťx����
//#define PS_BTY_HU						180											//���ư�Ťy����
////#define PS_BTX_TING						160											//
////#define PS_BTY_TING						180											//
//#define PS_BTX_GANG						130											//���ư�Ťx����
//#define PS_BTY_GANG						180											//���ư�Ťy����
//#define PS_BTX_PENG						100											//���ư�Ťx����
//#define PS_BTY_PENG						180											//���ư�ťy����
//#define PS_BTX_CHI						70											//���ư�ťx����
//#define PS_BTY_CHI						180											//���ư�ťy����
//#define PS_BTX_GIVEUP					40											//������ťx����
//#define PS_BTY_GIVEUP					180											//������ťy����

#define PS_BTX_QUANFENG					18											//Ȧ��λXУ��
#define PS_BTY_QUANFENG					10											//Ȧ��λYУ��

#define PS_BX_DICE						315											//��������
#define PS_BY_DICE						125											//��������

#define PS_BX_DICE1						316											//���Ӷ�λ1
#define PS_BY_DICE1						259											//���Ӷ�λ1

#define PS_BX_DICE2						(498-30)											//���Ӷ�λ2
#define PS_BY_DICE2						305											//���Ӷ�λ2

#define REVISE_PS						10											//����ͼ��У��
#define REVISE_MJ_TOUCH					30											//����������У��
#define REVISE_MJ_FACE					10											//��������У��
#define REVISE_FENGWEI				    15											//��λ����
#define REVISE_MJ_OUT					10											//������ʼλУ��
#define REVISE_MJ_SELF					15											//�Լ�������У��,ͻ��,���Ʋ���

#define REVISE_MJ_OUT_H0				28		
#define REVISE_MJ_OUT_H1				10											
#define REVISE_MJ_OUT_H2				28											
#define REVISE_MJ_OUT_H3				10											

#define REVISE_MJ_OUT_H12				10											
#define REVISE_MJ_OUT_H32				10											

//�齫����λ��
#define PS_MJ_FACE2_X					101											//�����齫
#define PS_MJ_FACE2_Y					477
#define PS_MJ_FACE0_X					231											
#define PS_MJ_FACE0_Y					44
#define PS_MJ_FACE1_X					704
#define PS_MJ_FACE1_Y					126
#define PS_MJ_FACE3_X					79
#define PS_MJ_FACE3_Y					126

//������λ��
#define PS_MJ_TOUCH0_X					229+20
#define PS_MJ_TOUCH0_Y					95
#define PS_MJ_TOUCH1_X					652	
#define PS_MJ_TOUCH1_Y					130
#define PS_MJ_TOUCH2_X					536+20
#define PS_MJ_TOUCH2_Y					420
#define PS_MJ_TOUCH3_X					125	
#define PS_MJ_TOUCH3_Y					400

//ͼ��..λ��
#define PS_FACE0_X						386				
#define PS_FACE0_Y						11
#define PS_FACE1_X						744
#define PS_FACE1_Y						234
#define PS_FACE2_X						386
#define PS_FACE2_Y						554
#define PS_FACE3_X						23	
#define PS_FACE3_Y						234

//������λ��
#define PS_MJ_OUT0_X					231				
#define PS_MJ_OUT0_Y					131
#define PS_MJ_OUT1_X					607
#define PS_MJ_OUT1_Y					173
#define PS_MJ_OUT2_X					231
#define PS_MJ_OUT2_Y					377
#define PS_MJ_OUT3_X					157
#define PS_MJ_OUT3_Y					173

////������λ��
//#define PS_MJ_HU0_X					231				
//#define PS_MJ_HU0_Y					131
//#define PS_MJ_HU1_X					607
//#define PS_MJ_HU1_Y					173
//#define PS_MJ_HU2_X					231
//#define PS_MJ_HU2_Y					377
//#define PS_MJ_HU3_X					157
//#define PS_MJ_HU3_Y					173

//Ȧ����
#define PS_FENG_QUAN_X					644
#define PS_FENG_QUAN_Y					30
//������	
#define REVISE_MJ_HU					20
#define REVISE_MJ_HU_FACE				5										//���������
#define REVISE_MJ_HU1_X					19										//1��λ������������������
#define REVISE_MJ_HU3_X					17										//3��λ�����������������
//Ȧ����
#define REVISE_FENG_QUAN				25

//������
#define COUNT_MJ_TOUCH					34											//����ϴ��������
#define COUNT_MJ_FACE					14											//��������

#define COUNT_MJ_OUT_LINE0				13											//���������и���--���0
#define COUNT_MJ_OUT_LINE1				10											//���������и���--���1
#define COUNT_MJ_OUT_LINE2				13											//���������и���--���2
#define COUNT_MJ_OUT_LINE3				10											//���������и���--���3

#define ID_DICE_TIME					1001										//���Ӽ�ʱ��
#define ID_OUT_TIME						1002										//���Ƽ�ʱ
#define ID_OUTFLAG_TIME					1003										//���Ʊ�־��ʱ��

//��ɫ����
#define COLORTRANS						RGB( 255, 0, 255 )							//͸����ɫֵ