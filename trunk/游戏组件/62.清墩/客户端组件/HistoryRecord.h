#ifndef HISTORY_RECORD_HEAD_FILE
#define HISTORY_RECORD_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//������־
#define AF_NULL						0										//û�ж���
#define AF_PASS						1										//��������
#define AF_OUT_CARD					2										//���ƶ���

//��Ϣ����
#define WM_ENABLE_HISTORY			(WM_USER+500)							//������Ϣ

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagHistoryScore
{
	LONGLONG						lTurnScore;								//������Ϣ
	LONGLONG						lCollectScore;							//������Ϣ
};

//////////////////////////////////////////////////////////////////////////

//��ʷ�˿�
class CHistoryCard
{
	//��������
protected:
	bool							m_bHistoryCard;							//��ʷ��־
	BYTE							m_cbEventFlag[GAME_PLAYER][2];			//������־
	BYTE							m_cbCardCount[GAME_PLAYER][2];			//�˿���Ŀ
	BYTE							m_cbCardData[GAME_PLAYER][2][MAX_COUNT];//�˿�����

	//��������
public:
	//���캯��
	CHistoryCard();
	//��������
	virtual ~CHistoryCard();

	//���ܺ���
public:
	//��������
	VOID ResetData();
	//��ȡ����
	BYTE GetCurrentEvent(WORD wChairID);
	//��ȡ����
	BYTE GetHistoryEvent(WORD wChairID);
	//��ȡ�˿�
	BYTE GetCurrentCard(WORD wChairID, BYTE cbCardBuffer[], BYTE cbBufferCount);
	//��ȡ�˿�
	BYTE GetHistoryCard(WORD wChairID, BYTE cbCardBuffer[], BYTE cbBufferCount);

	//�¼�����
public:
	//�����¼�
	VOID OnEventSaveData(WORD wChairID);
	//�û�����
	VOID OnEventUserPass(WORD wChairID);
	//�û�����
	VOID OnEventUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif