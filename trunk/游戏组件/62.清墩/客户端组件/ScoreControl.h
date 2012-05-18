#ifndef SCORE_CONTROL_HEAD_FILE
#define SCORE_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

#define LEN_ACCOUNTS 32
//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreInfoC
{
	LONGLONG						lGameScore[GAME_PLAYER];				//��Ϸ����
	TCHAR							szUserName[GAME_PLAYER][LEN_ACCOUNTS];	//�û�����
};

//////////////////////////////////////////////////////////////////////////

//���ֿؼ�
class CScoreControl
{
	//��������
protected:
	bool							m_bValid;								//��Ч��־
	tagScoreInfoC					m_ScoreInfo;							//������Ϣ

	//��Դ����
protected:
	CSkinImage						m_ImageWinLose;							//ʤ��ʧ��
	CSkinImage						m_ImageGameScore;						//������ͼ

	//��������
public:
	//���캯��
	CScoreControl();
	//��������
	virtual ~CScoreControl();

	//���ܺ���
public:
	//��λ����
	VOID RestorationData();
	//���û���
	VOID SetScoreInfo(const tagScoreInfoC & ScoreInfo);
	//�滭����
	VOID DrawScoreView(CDC * pDC, INT nXPos, INT nYPos);
};

//////////////////////////////////////////////////////////////////////////

#endif