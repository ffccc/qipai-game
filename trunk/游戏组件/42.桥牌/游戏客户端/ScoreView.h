#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreViewInfo
{
	WORD							wConcealTime;						//��Ϸ����

};

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
	//������Ϣ
protected:
	WORD							m_wConcealTime;						//��Ϸ����

	//������Ϣ
protected:
	LONG							m_lScore[GAME_PLAYER];				//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//�û�����

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��

	//��������
public:
	//���캯��
	CScoreView();
	//��������
	virtual ~CScoreView();
	
	//���ܺ���
public:
	//������Ϣ
	void SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif