#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CDialog
{
	//��������
protected:
	LONG							m_lGameTax;							//��Ϸ˰��
	LONG							m_lGameScore;						//��Ϸ�÷�

	//��Դ����
protected:
	CSkinImage						m_ImageBackWin;						//����ͼ��
	CSkinImage						m_ImageBackLost;					//����ͼ��

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetGameTax(LONG lGameTax);
	//���û���
	void SetGameScore(LONG lScore);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif