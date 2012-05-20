#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
#define MAX_COUNT					5									//�����Ŀ
//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����
//�˿�����
#define OX_VALUE0					0									//�������  ��ţ

#define OX_SMALL_WANG				11									//С��ţ
#define OX_BIG_WANG					12									//����ţ
#define OX_DOUBLECOW				13									//ţţ

//#define OX_FOUR_SAME				13									//��������
//#define OX_FOURKING					14									//��������
//#define OX_FIVEKING					15									//��������

//#define OX_THREE_SAME				12									//��������
//////////////////////////////////////////////////////////////////////////
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[54];				//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }


	BYTE GetMaxCardPos(BYTE cbCardData[],BYTE cbCardCount);

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardPip(BYTE cbCardData);
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	
public:
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡţţ
	bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);

	void SortByWang(BYTE cbCardData[],BYTE cbCardCount);

	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);

};

//////////////////////////////////////////////////////////////////////////

#endif
