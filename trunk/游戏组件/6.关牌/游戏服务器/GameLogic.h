#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//�߼�����
#define MAX_COUNT					17									//�����Ŀ
#define FULL_COUNT					48									//ȫ����Ŀ

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_SINGLE_LINK				4									//��������
#define CT_DOUBLE_LINK				5									//���ö���
#define CT_THREE_LINK				6									//���ö���
#define CT_THREE_DOUBLE_LINK		7									//��������
#define CT_BOMB						8									//ը������

//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];					//�����Ŀ
	BYTE							cbCardData[4][MAX_COUNT];			//�˿�����
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
};

//�ֲ���Ϣ
struct tagDistributing
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbDistributing[15][6];				//�ֲ���Ϣ
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardData[FULL_COUNT];			//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���Ժ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);

	//���ƺ���
public:
	//�����˿�
	VOID SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(BYTE cbFirstCard, BYTE cbNextCard);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//��������
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);

	//�ڲ�����
private:
	//�����˿�
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
	//�Ƿ�����
	bool IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCellCount);
	//�����˿�
	VOID AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//�����ֲ�
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
};

//////////////////////////////////////////////////////////////////////////

#endif