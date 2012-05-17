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
	WORD							wGameScore;							//��Ϸ�÷�
};

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
	//������Ϣ
protected:
	LONG							m_lTax[GAME_PLAYER];				//��Ϸ˰��
	LONG							m_lScore[GAME_PLAYER];				//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//�û�����

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CSkinImage						m_ImageWinLose;						//ʤ����־

	//��������
public:
	//���캯��
	CScoreView();
	//��������
	virtual ~CScoreView();
	
	//���ܺ���
public:
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore);
	//����˰��
	void SetGameTax(LONG lTax,WORD wChairID);
	//��������
	void ResetScore();

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
};

//////////////////////////////////////////////////////////////////////////

#endif