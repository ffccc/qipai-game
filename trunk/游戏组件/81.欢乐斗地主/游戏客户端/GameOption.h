#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//���ض���
#define MAX_PELS					25									//��С����
#define LESS_PELS					10									//��С����
#define DEFAULT_PELS				20									//Ĭ������

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameOption : public CSkinDialogEx
{
	//��������
public:
	bool							m_bDeasilOrder;						//����˳��
	bool							m_bEnableSound;						//��������
	DWORD							m_dwCardHSpace;						//�˿�����
	bool							m_bAllowLookOn;						//�����Թ�

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
