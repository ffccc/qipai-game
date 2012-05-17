#include "StdAfx.h"
#include "HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//////////////////////////////////////////////////////////////////////////

//���캯��
CHistoryScore::CHistoryScore()
{
	//���ñ���
	ZeroMemory(m_HistoryScore,sizeof(m_HistoryScore));

	return;
}

//��������
CHistoryScore::~CHistoryScore()
{
}

//��������
VOID CHistoryScore::ResetData()
{
	//���ñ���
	ZeroMemory(m_HistoryScore,sizeof(m_HistoryScore));

	return;
}

//��ȡ����
tagHistoryScore * CHistoryScore::GetHistoryScore(WORD wChairID)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return NULL;

	//��ȡ����
	return &m_HistoryScore[wChairID];
}

//�û�����
VOID CHistoryScore::OnEventUserEnter(WORD wChairID)
{
	//Ч������
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;
	
	//���ñ���
	m_HistoryScore[wChairID].lTurnScore=0;
	m_HistoryScore[wChairID].lCollectScore=0;

	return;
}

//�û��뿪
VOID CHistoryScore::OnEventUserLeave(WORD wChairID)
{
	//Ч������
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;
	
	//���ñ���
	m_HistoryScore[wChairID].lTurnScore=0;
	m_HistoryScore[wChairID].lCollectScore=0;

	return;
}

//�û�����
VOID CHistoryScore::OnEventUserScore(WORD wChairID, LONGLONG lGameScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	m_HistoryScore[wChairID].lTurnScore=lGameScore;
	m_HistoryScore[wChairID].lCollectScore+=lGameScore;

	return;
}

//////////////////////////////////////////////////////////////////////////
