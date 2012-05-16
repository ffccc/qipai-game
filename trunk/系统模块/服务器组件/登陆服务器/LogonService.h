#ifndef LOGON_SERVICE_HEAD_FILE
#define LOGON_SERVICE_HEAD_FILE

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

//��¼������
class CLogonService
{
	//���ñ���
protected:
	CInitParamter					m_InitParamter;						//���ò���
	enServiceStatus					m_ServiceStatus;					//����״̬

	//�������
protected:
	CDataBaseSink					m_DataBaseEngineSink;				//���ݹ���
	CAttemperEngineSink				m_AttemperEngineSink;				//���ȹ���

	//�ں����
protected:
	CTimerEngineHelper				m_TimerEngine;						//ʱ������
	CAttemperEngineHelper			m_AttemperEngine;					//��������
	CDataBaseEngineHelper			m_DataBaseEngine;					//��������
	CTCPNetworkEngineHelper			m_TCPNetworkEngine;					//��������
	CTCPSocketServiceHelper			m_TCPSocketCorrespond;				//Э������

	//��������
public:
	//���캯��
	CLogonService();
	//��������
	virtual ~CLogonService();

	//���ܺ���
public:
	//��ʼ����
	bool StartService();
	//ֹͣ����
	bool StopService();
	//����״̬
	bool IsService() { return m_ServiceStatus==ServiceStatus_Service; }
};

//////////////////////////////////////////////////////////////////////////

#endif