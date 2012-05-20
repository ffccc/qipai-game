#ifndef HISTORY_RECORD_HEAD_FILE
#define HISTORY_RECORD_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��


//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagHistoryScore
{
	LONGLONG						lTurnScore;								//������Ϣ
	LONGLONG						lCollectScore;							//������Ϣ
};

//////////////////////////////////////////////////////////////////////////

//��ʷ����
class CHistoryScore
{
	//��������
protected:
	tagHistoryScore					m_HistoryScore[GAME_PLAYER];			//������Ϣ

	//��������
public:
	//���캯��
	CHistoryScore();
	//��������
	virtual ~CHistoryScore();

	//���ܺ���
public:
	//��������
	VOID ResetData();
	//��ȡ����
	tagHistoryScore * GetHistoryScore(WORD wChairID);

	//�¼�����
public:
	//�û�����
	VOID OnEventUserEnter(WORD wChairID);
	//�û��뿪
	VOID OnEventUserLeave(WORD wChairID);
	//�û�����
	VOID OnEventUserScore(WORD wChairID, LONGLONG lGameScore);
};

//////////////////////////////////////////////////////////////////////////

#endif