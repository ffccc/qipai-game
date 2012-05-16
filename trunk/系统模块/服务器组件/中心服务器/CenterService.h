#ifndef CENTER_SERVICE_HEAD_FILE
#define CENTER_SERVICE_HEAD_FILE

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

//���ķ�����
class CCenterService
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
	CAttemperEngineHelper			m_AttemperEngine;					//��������
	CDataBaseEngineHelper			m_DataBaseEngine;					//��������
	CTCPNetworkEngineHelper			m_TCPNetworkEngine;					//��������

	//��������
public:
	//���캯��
	CCenterService();
	//��������
	virtual ~CCenterService();

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
