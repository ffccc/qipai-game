#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const BYTE				m_cbCardListData[52*8];				//�˿˶���
	static const BYTE				m_cbSpeedListData[8*8];				//�˿˶���

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

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//��������
	void RandNumList(BYTE cbBuffer[], BYTE cbCount);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardPip(BYTE cbCardData);
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	//���
	BYTE GetNumListPip(const BYTE cbNumData[], BYTE cbCount);
	//�����		
	BYTE GetNumBigList(const BYTE cbNumData[8][8], BYTE cbCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
