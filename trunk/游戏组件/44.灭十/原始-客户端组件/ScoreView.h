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
	LONG							m_lScore[MAX_CHAIR];				//��Ϸ�÷�
	TCHAR							m_szUserName[MAX_CHAIR][NAME_LEN];	//�û�����

	CBitmap bImage;

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
	void SetHwnd(HWND hWnd);
	//���û���
	void ResetScore();
	//����˰��
	void SetTax(LONG lTax);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore);

	//�����򴰿ڵ������趨
	void SetupRgn(	CDC *pDC,
					CBitmap &cBitmap,
					//CSkinImage skinImage,
					COLORREF TransColor);
	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif