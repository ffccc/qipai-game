#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Commonality.h"
#include "ServerList.h"
#include "InitParamter.h"

//////////////////////////////////////////////////////////////////////////

//�󶨲���
struct tagBindParameter
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////

//�������湳��
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��Ԫ����
	friend class CLogonService;

	//��������
protected:
	CInitParamter *					m_pInitParamter;					//���ò���
	tagBindParameter *				m_pBindParameter;					//������Ϣ

	//�������
protected:
	CServerList						m_ServerList;						//�������б�

	//�ӿڱ���
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	ITCPSocketService *				m_pITCPSocketCorrespond;			//Э������

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { return; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool __cdecl OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool __cdecl OnAttemperEngineStop(IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool __cdecl OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize) { return true; }
	//�Զ��¼�
	virtual bool __cdecl OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize) { return true; }

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool __cdecl OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool __cdecl OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool __cdecl OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPSocketRead(WORD wServiceID, CMD_Command Command, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool __cdecl OnEventTCPNetworkBind(DWORD dwClientIP, DWORD dwSocketID);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���ݿ⺯��
private:
	//�û���¼ʧ��
	bool OnDBLogonError(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���¼�ɹ�
	bool OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���سɹ�
	bool OnDBDownloadFaceSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ɹ���Ϣ
	bool OnDBRCustomFaceUploadResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ɾ�����
	bool OnDBRCustomFaceDeleteResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸Ľ��
	bool OnDBRModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���纯��
private:
	//��¼��Ϣ����
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�û���Ϣ����
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��������
private:
	//�б���Ϣ����
	bool OnCenterMainServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif