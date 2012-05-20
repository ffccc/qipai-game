#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

#define	DLG_IDI_CLOSE				29									//������ʱ��
#define IDM_START_TIMES				WM_USER+107							//ʱ����Ϣ
//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CDialog
{
	//��������
protected:
	LONG							m_lGameScore;						//��Ϸ�÷�
	int								m_bTimes;							//����ʱ��
	bool							m_bStart;							//��ʼ��־

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��

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
	void SetGameScore(WORD wChairID, LONG lScore);
	//��ʼʱ��
	void SetStartTimes(bool bStart = true);
	//��ʾʱ��
	void SetShowTimes();

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////

#endif