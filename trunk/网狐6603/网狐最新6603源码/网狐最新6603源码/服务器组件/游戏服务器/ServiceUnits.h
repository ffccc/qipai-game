#ifndef SERVICE_UNITS_HEAD_FILE
#define SERVICE_UNITS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "AttemperEngineSink.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//�����ʾ
#define NETWORK_CORRESPOND			1									//Э������

//��Ϣ����
#define WM_UICONTROL_REQUEST		(WM_USER+100)						//��������

//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����״̬
enum enServiceStatus
{
	ServiceStatus_Stop,				//ֹͣ״̬
	ServiceStatus_Config,			//����״̬
	ServiceStatus_Service,			//����״̬
};

//////////////////////////////////////////////////////////////////////////////////

//״̬�ӿ�
interface IServiceUnitsSink
{
	//�ӿڶ���
public:
	//����״̬
	virtual VOID OnServiceUnitsStatus(enServiceStatus ServiceStatus)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//����Ԫ
class CServiceUnits : public CWnd
{
	//��Ԫ����
	friend class CAttemperEngineSink;
	friend class CDataBaseEngineSink;

	//״̬����
public:
	CInitParameter					m_InitParameter;					//���ò���
	enServiceStatus					m_ServiceStatus;					//����״̬

	//�������
protected:
	tagGameParameter				m_GameParameter;					//���ò���
	tagDataBaseParameter			m_DataBaseParameter;				//���Ӳ���
	tagGameServiceAttrib			m_GameServiceAttrib;				//��������
	tagGameServiceOption			m_GameServiceOption;				//��������

	//�������
private:
	CWHDataQueue					m_DataQueue;						//���ݶ���
	CCriticalSection				m_CriticalSection;					//ͬ������

	//�������
public:
	CAttemperEngineSink				m_AttemperEngineSink;				//���ȹ���
	CDataBaseEngineSink				m_DataBaseEngineSink;				//���ݹ���
	
	//�������
public:
	CDataBaseEngineHelper			m_RecordDataBaseEngine;				//��������
	CDataBaseEngineHelper			m_KernelDataBaseEngine;				//��������

	//�ں����
public:
	CTimerEngineHelper				m_TimerEngine;						//ʱ������
	CAttemperEngineHelper			m_AttemperEngine;					//��������
	CTCPNetworkEngineHelper			m_TCPNetworkEngine;					//��������
	CTCPSocketServiceHelper			m_TCPSocketService;					//�������
	CGameServiceManagerHelper		m_GameServiceManager;				//��Ϸģ��

	//�ӿڱ���
protected:
	IServiceUnitsSink *				m_pIServiceUnitsSink;				//״̬�ӿ�

	//��̬����
public:
	static CServiceUnits *			g_pServiceUnits;					//����ָ��

	//��������
public:
	//���캯��
	CServiceUnits();
	//��������
	virtual ~CServiceUnits();

	//��Ϣ����
public:
	//��ȡ״̬
	enServiceStatus GetServiceStatus() { return m_ServiceStatus; }

	//�������
public:
	//���ýӿ�
	bool SetServiceUnitsSink(IServiceUnitsSink * pIServiceUnitsSink);
	//Ͷ������
	bool PostControlRequest(WORD wIdentifier, VOID * pData, WORD wDataSize);

	//�������
public:
	//��������
	bool StartService();
	//ֹͣ����
	bool ConcludeService();
	//��Ϸ����
	bool CollocateService(LPCTSTR pszGameModule, tagGameServiceOption & GameServiceOption);

	//��������
protected:
	//����ģ��
	bool CreateServiceDLL();
	//�������
	bool InitializeService();
	//�����ں�
	bool StartKernelService();
	//��������
	bool StartNetworkService();
	//��������
	bool RectifyServiceParameter();

	//�ڲ�����
private:
	//����״̬
	bool SetServiceStatus(enServiceStatus ServiceStatus);
	//���Ϳ���
	bool SendControlPacket(WORD wControlID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool LoadDataBaseParameter(LPCTSTR pszDataBaseAddr, LPCTSTR pszDataBaseName, tagDataBaseParameter & DataBaseParameter);

	//��Ϣӳ��
protected:
	//������Ϣ
	LRESULT OnUIControlRequest(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif