#pragma once

#include "StdAfx.h"
#include "Resource.h"


class CNtList : public CDialog
{
	//�ؼ�����
public:
	CBitmapButton		m_Bm4;							//��ť
	CBitmapButton		m_Bm3;							//��ť
	CBitmapButton		m_Bm2;							//��ť
	CBitmapButton		m_Bm1;							//��ť
	CBitmapButton		m_Bm5;							//��ť
	// ����
	CBitmapButton		m_bt20;

	enum				{ IDD = IDD_NT_LIST };			//ID ����
	//��������
public:
	//���캯��
	CNtList(CWnd * pParent = NULL);
	//�����б�
	void UpdateNtList(BOOL b[]);

protected:
	//DDX ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//��Ϣ����
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��ť��Ϣ
	afx_msg void On1();
	//��ť��Ϣ
	afx_msg void On2();
	//��ť��Ϣ
	afx_msg void On3();
	//��ť��Ϣ
	afx_msg void On4();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked5();
	
	afx_msg void OnBnClicked7();
};

