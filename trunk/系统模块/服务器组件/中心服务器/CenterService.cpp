#include "StdAfx.h"
#include "CenterService.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CCenterService::CCenterService()
{
	//���ñ���
	m_ServiceStatus=ServiceStatus_Stop;

	return;
}

//��������
CCenterService::~CCenterService()
{
	//ֹͣ����
	StopService();

	return;
}

//��ʼ����
bool CCenterService::StartService()
{
	//ֹͣ����
	ASSERT(m_ServiceStatus!=ServiceStatus_Service);
	if (m_ServiceStatus==ServiceStatus_Service) return false;
	
	//��ֹ����
	StopService();

	//���ز���
	m_InitParamter.LoadInitParamter();

	//�������
	if ((m_DataBaseEngine.GetInterface()==NULL)&&(m_DataBaseEngine.CreateInstance()==false)) return false;
	if ((m_AttemperEngine.GetInterface()==NULL)&&(m_AttemperEngine.CreateInstance()==false)) return false;
	if ((m_TCPNetworkEngine.GetInterface()==NULL)&&(m_TCPNetworkEngine.CreateInstance()==false)) return false;

	//����ӿ�
	IUnknownEx * pIAttemperEngine=m_AttemperEngine.GetInterface();
	IUnknownEx * pITCPNetworkEngine=m_TCPNetworkEngine.GetInterface();

	//�ص��ӿ�
	IUnknownEx * pIDataBaseEngineSink=QUERY_OBJECT_INTERFACE(m_DataBaseEngineSink,IUnknownEx);
	IUnknownEx * pIAttemperEngineSink=QUERY_OBJECT_INTERFACE(m_AttemperEngineSink,IUnknownEx);

	//�󶨽ӿ�
	if (m_AttemperEngine->SetAttemperEngineSink(pIAttemperEngineSink)==false) return false;
	if (m_DataBaseEngine->SetDataBaseEngineSink(pIDataBaseEngineSink)==false) return false;

	//�ں����
	if (m_AttemperEngine->SetNetworkEngine(pITCPNetworkEngine)==false) return false;
	if (m_TCPNetworkEngine->SetTCPNetworkEngineEvent(pIAttemperEngine)==false) return false;

	//���Ȼص�
	m_AttemperEngineSink.m_pInitParamter=&m_InitParamter;
	m_AttemperEngineSink.m_pIDataBaseEngine=m_DataBaseEngine.GetInterface();
	m_AttemperEngineSink.m_pITCPNetworkEngine=m_TCPNetworkEngine.GetInterface();

	//���ݿ�ص�
	m_DataBaseEngineSink.m_pInitParamter=&m_InitParamter;
	m_DataBaseEngineSink.m_pIDataBaseEngineEvent=QUERY_OBJECT_PTR_INTERFACE(pIAttemperEngine,IDataBaseEngineEvent);

	//��������
	WORD wMaxConnect=m_InitParamter.m_wMaxConnect;
	WORD wServicePort=m_InitParamter.m_wListenPort;
	if (m_TCPNetworkEngine->SetServiceParameter(wServicePort,wMaxConnect)==false) return false;

	//��������
	if (m_AttemperEngine->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_DataBaseEngine->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_TCPNetworkEngine->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_ServiceStatus=ServiceStatus_Service;

	return true;
}

//ֹͣ����
bool CCenterService::StopService()
{
	//���ñ���
	m_ServiceStatus=ServiceStatus_Stop;

	//ֹͣ����
	if (m_AttemperEngine.GetInterface()!=NULL) m_AttemperEngine->ConcludeService();
	if (m_DataBaseEngine.GetInterface()!=NULL) m_DataBaseEngine->ConcludeService();
	if (m_TCPNetworkEngine.GetInterface()!=NULL) m_TCPNetworkEngine->ConcludeService();

	return true;
}

//////////////////////////////////////////////////////////////////////////
