#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_INVALID					0									//��������

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[52];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:	
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } 
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; } 

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:
	//�ڲ�����
	BYTE GetCardLogicValue(BYTE bCardData) ;
	//��ȡ����
	bool GetScoreCard(BYTE bCardData[], BYTE bScoreCardData[], BYTE bCardDataCount, BYTE &bScoreCount);
};

//////////////////////////////////////////////////////////////////////////

#endif