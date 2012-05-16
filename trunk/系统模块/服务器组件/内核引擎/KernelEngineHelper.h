#ifndef SERVER_KERNEL_HELPER_HEAD_FILE
#define SERVER_KERNEL_HELPER_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"
#include "KernelEngine.h"
//////////////////////////////////////////////////////////////////////////

//队列引擎辅助类
class CQueueServiceHelper : public CTempldateHelper<IQueueServiceEngine>
{
	//函数定义
public:
	//构造函数
	CQueueServiceHelper(void) : CTempldateHelper<IQueueServiceEngine>(IID_IQueueServiceEngine,
		VER_IQueueServiceEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateQueueService")) { }
};

//////////////////////////////////////////////////////////////////////////

////网络引擎辅助类
//class CTCPSocketEngineHelper : public CTempldateHelper<ITCPSocketEngine>
//{
//	//函数定义
//public:
//	//构造函数
//	CTCPSocketEngineHelper(void) : CTempldateHelper<ITCPSocketEngine>(IID_ITCPSocketEngine,
//		VER_ITCPSocketEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateTCPSocketEngine")) { }
//};

//////////////////////////////////////////////////////////////////////////

//定时器引擎辅助类
class CTimerEngineHelper : public CTempldateHelper<ITimerEngine>
{
	//函数定义
public:
	//构造函数
	CTimerEngineHelper(void) : CTempldateHelper<ITimerEngine>(IID_ITimerEngine,
		VER_ITimerEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateTimerEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//数据库引擎辅助类
class CDataBaseHelper : public CTempldateHelper<IDataBase>
{
	//函数定义
public:
	//构造函数
	CDataBaseHelper(void) : CTempldateHelper<IDataBase>(IID_IDataBase,
		VER_IDataBase,SERVER_KERNEL_DLL_NAME,TEXT("CreateDataBase")) { }
};

//////////////////////////////////////////////////////////////////////////

//数据库引擎辅助类
class CDataBaseEngineHelper : public CTempldateHelper<IDataBaseEngine>
{
	//函数定义
public:
	//构造函数
	CDataBaseEngineHelper(void) : CTempldateHelper<IDataBaseEngine>(IID_IDataBaseEngine,
		VER_IDataBaseEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateDataBaseEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//调度引擎辅助类
class CAttemperEngineHelper : public CTempldateHelper<IAttemperEngine>
{
	//函数定义
public:
	//构造函数
	CAttemperEngineHelper(void) : CTempldateHelper<IAttemperEngine>(IID_IAttemperEngine,
		VER_IAttemperEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateAttemperEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//TCPNetWork引擎辅助类
class CTCPNetworkEngineHelper : public CTempldateHelper<ITCPNetworkEngine>
{
	//函数定义
public:
	//构造函数
	CTCPNetworkEngineHelper(void) : CTempldateHelper<ITCPNetworkEngine>(IID_ITCPNetworkEngine,
		VER_ITCPNetworkEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateTCPNetworkEngine")) { }
};

//////////////////////////////////////////////////////////////////////////

//TCPNetWork引擎辅助类
class CTCPSocketServiceHelper : public CTempldateHelper<ITCPSocketService>
{
	//函数定义
public:
	//构造函数
	CTCPSocketServiceHelper(void) : CTempldateHelper<ITCPSocketService>(IID_ITCPSocketService,
		VER_ITCPSocketService,SERVER_KERNEL_DLL_NAME,TEXT("CreateTCPSocketService")) { }
};
//////////////////////////////////////////////////////////////////////////

////服务引擎辅助类
//class CServiceEngineHelper : public CTempldateHelper<IServiceEngine>
//{
//	//函数定义
//public:
//	//构造函数
//	CServiceEngineHelper(void) : CTempldateHelper<IServiceEngine>(IID_IServiceEngine,
//		VER_IServiceEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateServiceEngine")) { }
//};

//////////////////////////////////////////////////////////////////////////

//异步引擎辅助类
class CAsynchronismEngineHelper : public CTempldateHelper<IAsynchronismEngine>
{
	//函数定义
public:
	//构造函数
	CAsynchronismEngineHelper(void) : CTempldateHelper<IAsynchronismEngine>(IID_IAsynchronismEngine,
		VER_IAsynchronismEngine,SERVER_KERNEL_DLL_NAME,TEXT("CreateAsynchronismEngine")) { }
};
//
////////////////////////////////////////////////////////////////////////////
//
////自动清理参数辅助类
//class CAutoCleanParameters
//{
//	//变量定义
//private:
//	CDataBaseHelper						& m_DataBaseHelper;				//数据库对象
//
//	//函数定义
//public:
//	//构造函数
//	CAutoCleanParameters(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
//	//析构函数	
//	virtual ~CAutoCleanParameters()
//	{
//		if (AfxIsValidAddress(this,sizeof(CDataBaseHelper))) m_DataBaseHelper->ClearParameters();
//	}
//};
//
////自动关闭记录集辅助类
//class CAutoCloseRecord
//{
//	//变量定义
//private:
//	CDataBaseHelper						& m_DataBaseHelper;				//数据库对象
//
//	//函数定义
//public:
//	//构造函数
//	CAutoCloseRecord(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
//	//析构函数	
//	virtual ~CAutoCloseRecord()
//	{
//		if (AfxIsValidAddress(this,sizeof(CDataBaseHelper))) m_DataBaseHelper->CloseRecordset();
//	}
//};
//
////断开数据库连接辅助类
//class CAutoCloseDataBase
//{
//	//变量定义
//private:
//	CDataBaseHelper						& m_DataBaseHelper;				//数据库对象
//
//	//函数定义
//public:
//	//构造函数
//	CAutoCloseDataBase(CDataBaseHelper & DataBaseHelper) : m_DataBaseHelper(DataBaseHelper) { }
//	//析构函数	
//	virtual ~CAutoCloseDataBase()
//	{
//		if (AfxIsValidAddress(this,sizeof(CDataBaseHelper))) m_DataBaseHelper->CloseConnection();
//	}
//};

//////////////////////////////////////////////////////////////////////////

#endif