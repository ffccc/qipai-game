#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define WM_SELECT_CHESS			WM_USER+300							//ѡ������

//////////////////////////////////////////////////////////////////////////

//����ѡ��
class CChessSelect : public CDialog
{
	//��Դ����
protected:
	CSkinImage						m_ImageBack;						//������Դ

	//�ؼ�����
protected:
	CSkinButton						m_btRook;							//���Ӱ�ť
	CSkinButton						m_btHorse;							//���Ӱ�ť
	CSkinButton						m_btQueen;							//���Ӱ�ť
	CSkinButton						m_btElephant;						//���Ӱ�ť

	//��������
public:
	//���캯��
	CChessSelect();
	//��������
	virtual ~CChessSelect();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	//��ť��Ϣ
protected:
	//���Ӱ�ť
	afx_msg void OnRook();
	//���Ӱ�ť
	afx_msg void OnHorse();
	//���Ӱ�ť
	afx_msg void OnQueen();
	//���Ӱ�ť
	afx_msg void OnElephant();

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
