#ifndef SERVER_KERNEL_HELPER_HEAD_FILE
#define SERVER_KERNEL_HELPER_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"
#include "KernelEngine.h"
//////////////////////////////////////////////////////////////////////////

//�������渨����
class CQueueServiceHelper : public CTempldateHelper<IQueueServiceEngine>
{
	//��������
public:
	//���캯��
	CQueueServiceHelper(void) : CTempldateHelper<IQueueServiceEngine>(IID_IQueueServiceEngine,
		VER_IQueueServiceEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateQueueService")) { }
};

//////////////////////////////////////////////////////////////////////////

////�������渨����
//class CTCPSocketEngineHelper : public CTempldateHelper<ITCPSocketEngine>
//{
//	//��������
//public:
//	//���캯��
//	CTCPSocketEngineHelper(void) : CTempldateHelper<ITCPSocketEngine>(IID_ITCPSocketEngine,
//		VER_ITCPSocketEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateTCPSocketEngine")) { }
//};

//////////////////////////////////////////////////////////////////////////

//��ʱ�����渨����
class CTimerEngineHelper : public CTempldateHelper<ITimerEngine>
{
	//��������
public:
	//���캯��
	CTimerEngineHelper(void) : CTempldateHelper<ITimerEngine>(IID_ITimerEngine,
		VER_ITimerEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateTimerEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//���ݿ����渨����
class CDataBaseHelper : public CTempldateHelper<IDataBase>
{
	//��������
public:
	//���캯��
	CDataBaseHelper(void) : CTempldateHelper<IDataBase>(IID_IDataBase,
		VER_IDataBase,SERVER_KERNEL_DLL_NAME,TEXT("CreateDataBase")) { }
};

//////////////////////////////////////////////////////////////////////////

//���ݿ����渨����
class CDataBaseEngineHelper : public CTempldateHelper<IDataBaseEngine>
{
	//��������
public:
	//���캯��
	CDataBaseEngineHelper(void) : CTempldateHelper<IDataBaseEngine>(IID_IDataBaseEngine,
		VER_IDataBaseEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateDataBaseEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//�������渨����
class CAttemperEngineHelper : public CTempldateHelper<IAttemperEngine>
{
	//��������
public:
	//���캯��
	CAttemperEngineHelper(void) : CTempldateHelper<IAttemperEngine>(IID_IAttemperEngine,
		VER_IAttemperEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateAttemperEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//TCPNetWork���渨����
class CTCPNetworkEngineHelper : public CTempldateHelper<ITCPNetworkEngine>
{
	//��������
public:
	//���캯��
	CTCPNetworkEngineHelper(void) : CTempldateHelper<ITCPNetworkEngine>(IID_ITCPNetworkEngine,
		VER_ITCPNetworkEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateTCPNetworkEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//TCPNetWork���渨����
class CTCPSocketServiceHelper : public CTempldateHelper<ITCPSocketService>
{
	//��������
public:
	//���캯��
	CTCPSocketServiceHelper(void) : CTempldateHelper<ITCPSocketService>(IID_ITCPSocketService,
		VER_ITCPSocketService,SERVER_KERNEL_DLL_NAME,TEXT("CreateTCPSocketService")) { }
};
//////////////////////////////////////////////////////////////////////////

////�������渨����
//class CServiceEngineHelper : public CTempldateHelper<IServiceEngine>
//{
//	//��������
//public:
//	//���캯��
//	CServiceEngineHelper(void) : CTempldateHelper<IServiceEngine>(IID_IServiceEngine,
//		VER_IServiceEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateServiceEngine")) { }
//};

//////////////////////////////////////////////////////////////////////////

//�첽���渨����
class CAsynchronismEngineHelper : public CTempldateHelper<IAsynchronismEngine>
{
	//��������
public:
	//���캯��
	CAsynchronismEngineHelper(void) : CTempldateHelper<IAsynchronismEngine>(IID_IAsynchronismEngine,
		VER_IAsynchronismEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateAsynchronismEngine")) { }
};
//
////////////////////////////////////////////////////////////////////////////
//
////�Զ��������������
//class CAutoCleanParameters
//{
//	//��������
//private:
//	CDataBaseHelper						& m_DataBaseHelper;				//���ݿ����
//
//	//��������
//public:
//	//���캯��
//	CAutoCleanParameters(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
//	//��������	
//	virtual ~CAutoCleanParameters()
//	{
//		if (AfxIsValidAddress(this,sizeof(CDataBaseHelper))) m_DataBaseHelper->ClearParameters();
//	}
//};
//
////�Զ��رռ�¼��������
//class CAutoCloseRecord
//{
//	//��������
//private:
//	CDataBaseHelper						& m_DataBaseHelper;				//���ݿ����
//
//	//��������
//public:
//	//���캯��
//	CAutoCloseRecord(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
//	//��������	
//	virtual ~CAutoCloseRecord()
//	{
//		if (AfxIsValidAddress(this,sizeof(CDataBaseHelper))) m_DataBaseHelper->CloseRecordset();
//	}
//};
//
////�Ͽ����ݿ����Ӹ�����
//class CAutoCloseDataBase
//{
//	//��������
//private:
//	CDataBaseHelper						& m_DataBaseHelper;				//���ݿ����
//
//	//��������
//public:
//	//���캯��
//	CAutoCloseDataBase(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
//	//��������	
//	virtual ~CAutoCloseDataBase()
//	{
//		if (AfxIsValidAddress(this,sizeof(CDataBaseHelper))) m_DataBaseHelper->CloseConnection();
//	}
//};

//////////////////////////////////////////////////////////////////////////

#endif