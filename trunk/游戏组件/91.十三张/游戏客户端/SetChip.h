#pragma once

#include "Stdafx.h"
#include "Resource.h"

//�궨��
#define SET_CHIP_COUNT					4			//Ѻע����

//��Ϸ����
class CSetChip : public CSkinDialogEx
{
	//��������
public:
	LONG							m_nChip;							//ѹע��С
	LONG							m_wMaxChip ;						//�����ע

	//�ؼ�����
public:
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	//��������
public:
	//���캯��
	CSetChip();
	//��������
	virtual ~CSetChip();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
//	virtual void OnOK();

	//���ú���
public:
	//��ʼѺע
	void SetMaxChips( LONG lMaxChip ) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

//////////////////////////////////////////////////////////////////////////
