#ifndef GAME_SERVICE_HEAD_FILE
#define GAME_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

//����״̬
enum enServiceStatus
{
	ServiceStatus_Stop,				//ֹͣ״̬
	ServiceStatus_Service,			//����״̬
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CGameService : public IGameService
{
	//��������
protected:
	enServiceStatus					m_ServiceStatus;					//����״̬

	//�������
protected:
	CDataBaseSink					m_DataBaseEngineSink;				//���ݿ⹳��
	CAttemperEngineSink				m_AttemperEngineSink;				//���ȹ���

	//�ں����
protected:
	CTimerEngineHelper				m_TimerEngine;						//ʱ������
	CAttemperEngineHelper			m_AttemperEngine;					//��������
	CDataBaseEngineHelper			m_DataBaseEngine;					//��������
	CTCPNetworkEngineHelper			m_TCPNetworkEngine;					//��������
	CTCPSocketServiceHelper			m_TCPSocketCorrespond;				//Э������

	//���ñ���
protected:
	TCHAR							m_szModule[32];						//ģ������
	tagDataBaseInfo					m_GameUserDBInfo;					//��������
	tagDataBaseInfo					m_GameScoreDBInfo;					//��������
	tagGameServiceAttrib			m_GameServiceAttrib;				//��������
	tagGameServiceOption			m_GameServiceOption;				//��������

	//����ӿ�
protected:
	HINSTANCE						m_hDllInstance;						//��Ϸʵ��
	IGameServiceManager	*			m_pIGameServiceManager;				//����ӿ�

	//��������
public:
	//���캯��
	CGameService(void);
	//��������
	virtual ~CGameService(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ����
	virtual bool __cdecl StartService(tagGameServiceParameter * pGameServiceParameter);
	//ֹͣ����
	virtual bool __cdecl StopService();
	//����״̬
	virtual bool __cdecl IsService() { return m_ServiceStatus==ServiceStatus_Service; }

	//�ڲ�����
private:
	//�ͷ�ģ��
	bool UnLoadGameServiceModule();
	//����ģ��
	bool LoadGameServiceModule(LPCTSTR pszModuleName);
	//��ʾ����
	void ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel);
};

//////////////////////////////////////////////////////////////////////////

#endif