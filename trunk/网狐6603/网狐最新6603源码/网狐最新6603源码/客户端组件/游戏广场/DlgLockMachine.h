#ifndef DLG_LOCK_MACHINE_HEAD_FILE
#define DLG_LOCK_MACHINE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgLockMachine : public CSkinDialog, public CMissionItem
{
	//��������
protected:
	BYTE							m_cbMachine;						//�󶨱�־
	TCHAR							m_szPassword[LEN_PASSWORD];			//�û�����

	//�ؼ�����
protected:
	CSkinButton						m_btOk;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť
	CPasswordControl				m_PasswordControl;					//�û�����

	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

	//��������
public:
	//���캯��
	CDlgLockMachine();
	//��������
	virtual ~CDlgLockMachine();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif