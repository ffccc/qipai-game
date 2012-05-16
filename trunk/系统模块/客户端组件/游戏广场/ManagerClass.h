#ifndef MANAGER_CLASS_HEAD_FILE
#define MANAGER_CLASS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//���;���
class CManagerSendWarning : public CSkinDialogEx
{
	//��������
protected:
	DWORD							m_dwUserID;							//�û���ʶ
	IUserItem *						m_pIUserItem;						//�û��ӿ�

	//�ӿڶ���
protected:
	ITCPSocket *					m_pIClientSocket;					//����ӿ�
	IClientUserManager	*			m_pIClientUserManager;				//�û��ӿ�

	//�ؼ�����
protected:
	CSkinButton						m_btSend;							//���Ͱ�ť
	CSkinButton						m_btClose;							//�رհ�ť
	CSkinButton						m_btPreView;						//Ԥ����ť
	CSkinRichEdit					m_WarningMessage;					//������Ϣ

	//��������
public:
	//���캯��
	CManagerSendWarning();
	//��������
	virtual ~CManagerSendWarning();

	//���ܺ���
public:
	//�����û�
	bool SetUserItem(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	bool SetClientSocket(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	bool SetClientUserManager(IUnknownEx * pIUnknownEx);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//��Ϣ����
public:
	//Ԥ����Ϣ
	afx_msg void OnPreViewMessage();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//Ȩ�޹���
class CManagerUserRight : public CSkinDialogEx
{
	//��������
protected:
	DWORD							m_dwUserID;							//�û���ʶ
	IUserItem *						m_pIUserItem;						//�û��ӿ�

	//Ȩ�ޱ���
protected:
	DWORD							m_dwUserRight;						//�û�Ȩ��
	//DWORD							m_dwLastUserRight;					//�û�Ȩ��
	DWORD							m_dwMasterRight;					//����Ȩ��

	//�ӿڶ���
protected:
	ITCPSocket *					m_pIClientSocket;					//����ӿ�
	IClientUserManager	*			m_pIClientUserManager;				//�û��ӿ�

	//�ؼ�����
protected:
	CSkinButton						m_btOk;								//ȷ����ť
	CSkinButton						m_btClose;							//�رհ�ť

	//��������
public:
	//���캯��
	CManagerUserRight();
	//��������
	virtual ~CManagerUserRight();

	//���ܺ���
public:
	//����Ȩ��
	bool SetMasterRight(DWORD dwMasterRight);
	//�����û�
	bool SetUserItem(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	bool SetClientSocket(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	bool SetClientUserManager(IUnknownEx * pIUnknownEx);

	//�ڲ�����
protected:
	//��������
	bool UpdateRightData();
	//���¿���
	bool UpdateMasterControl();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//ϵͳ��Ϣ
class CManagerMessage : public CSkinDialogEx
{
	//�ӿڶ���
protected:
	ITCPSocket *					m_pIClientSocket;					//����ӿ�

	//�ؼ�����
protected:
	CSkinButton						m_btSend;							//���Ͱ�ť
	CSkinButton						m_btClose;							//�رհ�ť
	CSkinButton						m_btPreView;						//Ԥ����ť
	CSkinRichEdit					m_SystemMessage;					//ϵͳ��Ϣ

	//��������
public:
	//���캯��
	CManagerMessage();
	//��������
	virtual ~CManagerMessage();

	//���ܺ���
public:
	//���ýӿ�
	bool SetClientSocket(IUnknownEx * pIUnknownEx);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//��Ϣ����
public:
	//Ԥ����Ϣ
	afx_msg void OnPreViewMessage();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//�������
class CManagerServer : public CSkinDialogEx
{
	//�ӿڶ���
protected:
	ITCPSocket *					m_pIClientSocket;					//����ӿ�

	//�ؼ�����
protected:
	CSkinButton						m_btLimitGameChat;					//���ư�ť
	CSkinButton						m_btAllowGameChat;					//���ư�ť
	CSkinButton						m_btLimitRoomChat;					//���ư�ť
	CSkinButton						m_btAllowRoomChat;					//���ư�ť
	CSkinButton						m_btLimitEnterRoom;					//���ư�ť
	CSkinButton						m_btAllowEnterRoom;					//���ư�ť
	CSkinButton						m_btLimitEnterGame;					//���ư�ť
	CSkinButton						m_btAllowEnterGame;					//���ư�ť
	CSkinButton						m_btSetShallClose;					//���ư�ť
	CSkinButton						m_btReSetShallClose;				//���ư�ť
	CSkinButton						m_btLimitRoomWisper;				//���ư�ť
	CSkinButton						m_btAllowRoomWisper;				//���ư�ť

	//��������
public:
	//���캯��
	CManagerServer();
	//��������
	virtual ~CManagerServer();

	//���غ��� 
protected:
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//���ýӿ�
	bool SetClientSocket(IUnknownEx * pIUnknownEx);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//��Ϣ����
public:

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif