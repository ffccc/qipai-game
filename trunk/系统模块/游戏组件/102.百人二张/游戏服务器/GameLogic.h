#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����
#define	CT_ERROR					0									//��������
#define	CT_POINT					1									//��������
#define	CT_SPECIAL_11				2									//��������
#define	CT_SPECIAL_10				3									//��������
#define	CT_SPECIAL_9				4									//��������
#define	CT_SPECIAL_8				5									//��������
#define	CT_SPECIAL_7				6									//��������
#define	CT_SPECIAL_6				7									//��������
#define	CT_SPECIAL_5				8									//��������
#define	CT_SPECIAL_4				9									//��������
#define	CT_SPECIAL_3				10									//��������
#define	CT_SPECIAL_2				11									//��������
#define	CT_SPECIAL_1				12									//��������

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define	ST_VALUE					1									//��ֵ����
#define	ST_LOGIC					2									//�߼�����

//�˿���Ŀ
#define CARD_COUNT					32									//�˿���Ŀ
//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[CARD_COUNT];		//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
		if(cbCardData == 0x42)
		{
			BYTE bcTemp = 0x31;
			return  bcTemp&LOGIC_MASK_VALUE; 
		}
		if(cbCardData==0x23)
		{
			BYTE bcTemp = 0x33;
			return  bcTemp&LOGIC_MASK_VALUE; 

		}
		if(cbCardData == 0x0B)
		{
			BYTE bcTemp = 0x1B;
			return  bcTemp&LOGIC_MASK_VALUE; 
		}
		if(cbCardData == 0x2B)
		{
			BYTE bcTemp = 0x3B;
			return  bcTemp&LOGIC_MASK_VALUE; 
		}
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData)
	{
		if(cbCardData == 0x42)
		{
			BYTE bcTemp = 0x31;
			return  (bcTemp&LOGIC_MASK_COLOR)>>4; 
		}
		if(cbCardData==0x23)
		{
			BYTE bcTemp = 0x33;
			return  (bcTemp&LOGIC_MASK_COLOR)>>4; 

		}
		if(cbCardData == 0x0B)
		{
			BYTE bcTemp = 0x1B;
			return  (bcTemp&LOGIC_MASK_COLOR)>>4; 
		}
		if(cbCardData == 0x2B)
		{
			BYTE bcTemp = 0x3B;
			 return  (bcTemp&LOGIC_MASK_COLOR)>>4; 
		}

		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��С�Ƚ�
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount);
	//�߼���С
	BYTE GetCardLogicValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
