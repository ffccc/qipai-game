#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardDataArray[80];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ƺ���
public:
	//�����˿�
	void RandCardData(BYTE cbCardData[], BYTE bMaxCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE bRemoveCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardData[], BYTE bCardCount, BYTE cbRemoveCard[], BYTE bRemoveCount);

	//�߼�����
public:
	//�����ж�
	BYTE GetAcitonTiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbTiCardIndex[5]);
	//η���ж�
	BYTE GetActionWeiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbWeiCardIndex[7]);
	//�����ж�
	BYTE GetActionChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, tagChiCardInfo ChiCardInfo[6]);

	//�жϺ���
public:
	//�Ƿ����
	bool IsChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�Ƿ�����
	bool IsTiPaoCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�Ƿ�����
	bool IsWeiPengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//���ƽ��
	bool GetHuCardInfo(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbHuXiWeave, tagHuCardInfo & HuCardInfo);

	//��������
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�˿���Ŀ
	BYTE GetCardCount(BYTE cbCardIndex[MAX_INDEX]);
	//��ȡ��Ϣ
	BYTE GetWeaveHuXi(const tagWeaveItem & WeaveItem);

	//ת������
public:
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[], BYTE bMaxCount);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE bCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//�ڲ�����
private:
	//�����˿�
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], CAnalyseItemArray & AnalyseItemArray);
	//��ȡ����
	BYTE TakeOutChiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbResultCard[3]);
};

//////////////////////////////////////////////////////////////////////////

#endif