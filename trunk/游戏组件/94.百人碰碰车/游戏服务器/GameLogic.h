#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����


enum emCardType
{
	CT_SPECIAL_DA_BEN = 1,
	CT_SPECIAL_XIAO_BEN = 2,
	CT_SPECIAL_DA_BAO_MA = 3,
	CT_SPECIAL_XIAO_BAO_MA = 4,
	CT_SPECIAL_DA_OUDI = 5	,	                	//��������
	CT_SPECIAL_XIAO_OUDI = 6	,						//��������
	CT_SPECIAL_DA_DAZONG = 7	,						//��������
	CT_SPECIAL_XIAO_DAZONG = 8	,						//��������

};
//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define	ST_VALUE					1									//��ֵ����
#define	ST_NEW					    3									//��ֵ����
#define	ST_LOGIC					2									//�߼�����

//�˿���Ŀ
#define CARD_COUNT					32									//�˿���Ŀ
//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static  BYTE				m_cbCardListData[172-4];		//�˿˶���

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
	   return cbCardData&LOGIC_MASK_VALUE; 
	}
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData)
	{
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
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE *bcOutCadData = NULL);
	//��С�Ƚ�
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple);
	//�߼���С
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
