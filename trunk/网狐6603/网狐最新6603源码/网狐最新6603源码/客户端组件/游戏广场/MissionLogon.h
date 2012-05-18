#ifndef MISSION_LOGON_HEAD_FILE
#define MISSION_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgLogon.h"
#include "DlgStatus.h"
#include "DlgRegister.h"

//////////////////////////////////////////////////////////////////////////////////

//��¼����
class CMissionLogon : public CDlgStatus, public CMissionItem, public IStatusViewSink
{
	//��Ԫ����
	friend class CDlgLogon;
	friend class CDlgRegister;

	//��������
protected:
	bool							m_bRegister;						//ע���־
	bool							m_bRemPassword;						//��ס����

	//�ؼ�ָ��
protected:
	CDlgLogon *						m_pDlgLogon;						//��¼����
	CDlgRegister *					m_pDlgRegister;						//ע�ᴰ��

	//��̬����
protected:
	static CMissionLogon *			m_pMissionLogon;					//����ָ��

	//��������
public:
	//���캯��
	CMissionLogon();
	//��������
	virtual ~CMissionLogon();

	//״̬�ӿ�
public:
	//ȡ������
	virtual VOID OnStatusCancel();

	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	//��ʾ��¼
	VOID ShowLogon();
	//��ʾע��
	VOID ShowRegister();

	//��������
protected:
	//�����û�
	VOID UpdateUserInfo();
	//ִ�е�¼
	bool PerformLogonMission(bool bRemPassword);

	//�����¼�
protected:
	//��¼�ɹ�
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//��¼���
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//������ʾ
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);

	//��̬����
public:
	//��ȡ����
	static CMissionLogon * GetInstance() { return m_pMissionLogon; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif