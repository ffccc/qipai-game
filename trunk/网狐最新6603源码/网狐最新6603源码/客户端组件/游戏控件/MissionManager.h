#ifndef MISSION_MANAGER_HEAD_FILE
#define MISSION_MANAGER_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//����ص�
interface IMissionSocketSink
{
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode)=NULL;
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason)=NULL;
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class SHARE_CONTROL_CLASS CMissionSocket : public ITCPSocketSink
{
	//״̬����
protected:
	bool							m_bSwitchDns;						//DNS ת��
	bool							m_bTryDefault;						//����Ĭ��
	WORD							m_wSwitchIndex;						//�л�����

	//������Ϣ
protected:
	WORD							m_wCurrentPort;						//��ǰ�˿�
	DWORD							m_dwCurrentServer;					//��ǰ��ַ
	TCHAR							m_szCustomServer[LEN_SERVER];		//ָ����ַ
	TCHAR							m_szCurrentServer[LEN_SERVER];		//��ǰ��ַ

	//�������
protected:
	CTCPSocketHelper				m_TCPSocketModule;					//��������

	//�ӿڱ���
protected:
	IMissionSocketSink *			m_pIMissionSocketSink;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CMissionSocket();
	//��������
	virtual ~CMissionSocket();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
protected:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//״̬����
public:
	//����״̬
	BYTE GetSocketStatus();
	//��ǰ����
	LPCTSTR GetCurrentServer() { return m_szCurrentServer; }

	//���ú���
public:
	//���õ�ַ
	VOID SetCustomServer(LPCTSTR pszCustomServer);
	//���ýӿ�
	VOID SetMissionSocketSink(IMissionSocketSink * pIMissionSocketSink) { m_pIMissionSocketSink=pIMissionSocketSink; }

	//���ܺ���
public:
	//�ж�����
	bool PerformClose();
	//��������
	bool PerformConnect(bool bContinue);
	//���ͺ���
	bool PerformSendData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	bool PerformSendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	//�ڲ�����
private:
	//��ַ��Ϣ
	bool GetServerInfo();
	//�л�����
	bool SwitchServerItem();
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class SHARE_CONTROL_CLASS CMissionItem : public IMissionSocketSink
{
	//��Ԫ����
	friend class CMissionManager;

	//��������
private:
	bool						m_bActiveStatus;						//����״̬
	CMissionManager *			m_pMissionManager;						//�������

	//��������
public:
	//���캯��
	CMissionItem();
	//��������
	virtual ~CMissionItem();

	//���ܺ���
public:
	//����״̬
	bool GetActiveStatus() { return m_bActiveStatus; }
	//�������
	CMissionManager * GetMissionManager() { return m_pMissionManager; }
};

//////////////////////////////////////////////////////////////////////////////////

//�����ǲ
class SHARE_CONTROL_CLASS CMissionManager : public CWnd, public IMissionSocketSink
{
	//�������
protected:
	CMissionSocket					m_MissionSocket;					//��������

	//��������
protected:
	CWHArray<CMissionItem *>		m_MissionItemArray;					//��������

	//��������
public:
	//���캯��
	CMissionManager();
	//��������
	virtual ~CMissionManager();

	//����ӿ�
protected:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//������
public:
	//���õ�ַ
	VOID SetCustomServer(LPCTSTR pszCustomServer);
	//��ȡ��ַ
	LPCTSTR GetCurrentServer() { return m_MissionSocket.GetCurrentServer(); }

	//�������
public:
	//��������
	bool InsertMissionItem(CMissionItem * pMissionItem);
	//ɾ������
	bool DeleteMissionItem(CMissionItem * pMissionItem);

	//���ܺ���
public:
	//��������
	bool AvtiveMissionItem(CMissionItem * pMissionItem, bool bContinue);
	//��ֹ����
	bool ConcludeMissionItem(CMissionItem * pMissionItem, bool bDeferIntermit);

	//���ܺ���
public:
	//���ͺ���
	bool SendData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif