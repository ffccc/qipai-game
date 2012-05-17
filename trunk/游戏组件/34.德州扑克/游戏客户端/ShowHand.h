#ifndef SHOWHAND_VIEW_HEAD_FILE
#define SHOWHAND_VIEW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Stdafx.h"
#include "Resource.h"

class ShowHand : public CDialog
{
	DECLARE_DYNAMIC(ShowHand)

public:
	BYTE		m_bTimes;		//ʱ����Ŀ

	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��

	//�ؼ�����
public:
	CSkinButton						m_btOK;						//������ť
	CSkinButton						m_btCancel;						//�˳���ť

public:
	//���캯��
	ShowHand();   
	//��������
	virtual ~ShowHand();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//
	afx_msg void OnBnClickedOk();
	//
	afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif