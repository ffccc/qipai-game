#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreWnd : public CWnd
{
	//��������
protected:
	LONG							m_lTax;								//��Ϸ˰��
	LONG							m_lScore[MAX_CHAIR];				//��Ϸ�÷�
	TCHAR							m_szUserName[MAX_CHAIR][NAME_LEN];	//�û�����

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��

	//��������
public:
	//���캯��
	CScoreWnd(void);
	//��������
	virtual ~CScoreWnd(void);
	
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

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
