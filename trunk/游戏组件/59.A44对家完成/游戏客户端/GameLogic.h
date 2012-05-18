#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define ST_ORDER					0									//��С����
#define ST_COUNT					1									//��Ŀ����

//////////////////////////////////////////////////////////////////////////
//��Ŀ����

#define MAX_COUNT					14									//�����Ŀ
#define FULL_COUNT					54									//ȫ����Ŀ
#define BACK_COUNT					4									//������Ŀ
#define NORMAL_COUNT				14									//������Ŀ

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_SINGLE_LINE				3									//��������

#define CT_THREE					4									//��������
#define CT_DOUBLE_LINE				5									//��������

#define CT_FOUR						6									//��������
#define CT_MISSILE_CARD				7									//�������
#define	CT_A44_red_black			8									//��ɫA44
#define	CT_A44_black				9									//��ɫA44
#define	CT_A44_red					10									//��ɫA44
//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbDoubleCount;						//������Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE							cbFourCardData[MAX_COUNT];			//�����˿�
	BYTE							cbThreeCardData[MAX_COUNT];			//�����˿�
	BYTE							cbDoubleCardData[MAX_COUNT];		//�����˿�
	BYTE							cbSignedCardData[MAX_COUNT];		//�����˿�
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
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

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//����˿�
	bool CompareChaCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//����˿�
	bool CompareDianCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount);
	//��������
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);

	//�ڲ�����
public:
	//�����˿�
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);

public:
	//�Ƿ���1
	inline BOOL IsOnlyOne(const BYTE iCardList[], int iCardCount) { return iCardCount==1; };
	//�Ƿ����2
	BOOL IsDouble(const BYTE iCardList[], int iCardCount);
	//�Ƿ�����3
	BOOL IsThree(const BYTE iCardList[], int iCardCount);
	//�Ƿ�˳4
	BOOL IsSingleContinue(const BYTE iCardList[], int iCardCount);
	//�Ƿ�˫˳5
	BOOL IsDoubleContinue(const BYTE iCardList[], int iCardCount);
	//�Ƿ�ը��4��6
	BOOL IsBomb(const BYTE iCardList[], int iCardCount);
	//�Ƿ�˫��7
	BOOL IsDoubleKong(const BYTE iCardList[], int iCardCount);
	//�Ƿ�44��8910
	BOOL Is44A(const BYTE iCardList[], int iCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif