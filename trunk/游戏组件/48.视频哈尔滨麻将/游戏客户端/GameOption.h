#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameOption : public CSkinDialogEx
{
	//��������
public:
	bool							m_bEnableSound;						//��������
	bool							m_bAllowLookon;						//�����Թ�

	//�ؼ�����
public:
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	//��������
public:
	//���캯��
	CGameOption();
	//��������
	virtual ~CGameOption();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
