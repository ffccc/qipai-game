#ifndef DLG_HINT_HEAD_FILE
#define DLG_HINT_HEAD_FILE

#pragma once

#include "Stdafx.h"
//#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////

//�û�����
class CDlgHint : public CDialog
{
	//��������
public:
	LONG							m_nResidualTime;					//ʣ��ʱ��
	CSkinImage						m_ImageViewBack;					//������Դ

	//�ؼ�����
protected:
	CSkinButton						m_btOk;								//��ť�ؼ�

	//��������
public:
	//���캯��
	CDlgHint();
	//��������
	virtual ~CDlgHint();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual VOID OnOK();

	//��Ϣӳ��
protected:
	//�ػ�����
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

	//��������
public:
	//����ʱ��
	VOID InitTimerCount();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
