#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�����Ƶ�����
#define CARDTYPE_ERROR						0									//��������
#define	CARDTYPE_SAN_ZHANG					1									//����
#define	CARDTYPE_SAN_GONG					2									//����
#define	CARDTYPE_SAN_PAI					3									//ɢ��
#define	CARDTYPE_BI_SHI						4									//��ʮ

//////////////////////////////////////////////////////////////////////////

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
	BYTE GetCardColor(BYTE cbCardData) { return (cbCardData&LOGIC_MASK_COLOR)>>4 ; }

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//����
	void SortCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//��ȡ�����Ƶĵ���
	BYTE GetCardPoint(BYTE cbCardData);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardPip(BYTE cbCardData);
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

	//�Ƿ�������
	bool IsSanGong(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//�Ƿ�����
	bool IsSanZhang(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//�Ƿ��Ǳ�ʮ
	bool IsBiShi(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//�Ƿ�ɢ��
	bool IsSanPai(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//��ȡ�Ƶ�����
	BYTE GetCardType(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//��ȡ����
	BYTE GetPoint(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
	//�Ƚ��ƴ�С, ���First ������ Next  
	bool IsBigger(const BYTE cbFirstCard[], const BYTE cbNextCard[], const BYTE cbFirstCount, const BYTE cbNextCount);
	//��ȡ���ı���
	BYTE GetMultiple(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
