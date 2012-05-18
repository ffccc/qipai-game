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
	LONG							lDrawScore[2];						//��Ϸ����
};

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CDialog
{
	//������Ϣ
protected:
	LONG							m_lScore[GAME_PLAYER];				//��Ϸ�÷�
	LONG							m_lDrawScore[2];					//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];//�û�����

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CBitmap                         m_BTbitmap;

	//��������
public:
	//���캯��
	CScoreView();
	//��������
	virtual ~CScoreView();
	
	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

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
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

//////////////////////////////////////////////////////////////////////////

#endif