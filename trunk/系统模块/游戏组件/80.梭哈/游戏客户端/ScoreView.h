#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
	//��������
protected:
	LONG							m_lTax;								//��Ϸ˰��
	LONG							m_lScore[GAME_PLAYER];				//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//�û�����

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetTax(LONG lTax);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif