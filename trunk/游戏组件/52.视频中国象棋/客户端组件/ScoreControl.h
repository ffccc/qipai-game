#ifndef SCORE_CONTROL_HEAD_FILE
#define SCORE_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreInfo
{
	//������Ϣ
	LONGLONG						lGameScore[GAME_PLAYER];				//��Ϸ����
	TCHAR							szUserName[GAME_PLAYER][LEN_ACCOUNTS];	//�û�����

	//������Ϣ
	BYTE							cbReason;								//����ԭ��
	FLOAT							fWinRate[GAME_PLAYER];					//�û�ʤ��
	TCHAR							szLevelName[GAME_PLAYER][32];			//�û��ȼ�
	WORD							wMeChiarID;								//����λ��
};

//////////////////////////////////////////////////////////////////////////

//���ֿؼ�
class CScoreControl : public CWnd
{
	//״̬����
protected:
	tagScoreInfo					m_GameScoreInfo;					//������Ϣ

	//��Դ����
protected:
	CSkinImage						m_ImageWinLose;						//ʤ��ʧ��
	CSkinImage						m_ImageGameScore;					//������ͼ

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
	VOID SetScoreInfo(const tagScoreInfo & ScoreInfo);

	//��Ϣӳ��
protected:
	//�ػ�����
	VOID OnPaint();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
