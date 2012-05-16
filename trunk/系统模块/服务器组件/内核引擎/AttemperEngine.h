#ifndef ATTEMPER_ENGINE_HEAD_FILE
#define ATTEMPER_ENGINE_HEAD_FILE

#pragma once

//组件头文件

#include "KernelEngineHead.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//调度管理类
class CAttemperEngine : public IAttemperEngine, public IQueueServiceSink,
		public IDataBaseEngineEvent, public ITCPNetworkEngineEvent
{
	//核心变量
protected:
	bool							m_bService;							//运行标志
	CQueueService					m_QueueService;						//队列对象
	CCriticalSection				m_CriticalSection;					//同步锁定

	//接口变量
protected:
	ITCPSocketService				* m_pITCPSocketService;				//网络引擎
	ITCPNetworkEngine				* m_pTCPNetworkEngine;				//网络引擎
	IAttemperEngineSink				* m_pIAttemperEngineSink;			//挂接接口

	//机器人链接维护
protected:
	CArrayTemplate<DWORD>			m_AndroidUserSocketIDArray;					//机器人连接

	//函数定义
public:
	//构造函数
	CAttemperEngine(void);
	//析构函数
	virtual ~CAttemperEngine(void);

	//基础接口
public:
	//释放对象
	virtual VOID __cdecl Release()
	{
		if (IsValid()) delete this;
	}
	//是否有效
	virtual bool __cdecl IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CAttemperEngine)) ? true : false;
	}
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//启动服务
	virtual bool __cdecl StartService();
	//停止服务
	virtual bool __cdecl ConcludeService();

	//调度引擎
public:
	//设置网络
	virtual bool __cdecl SetNetworkEngine(IUnknownEx * pIUnknownEx);
	//注册钩子
	virtual bool __cdecl SetAttemperEngineSink(IUnknownEx * pIUnknownEx);
	//自定事件
	virtual bool __cdecl OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize);
	//控制事件
	virtual bool __cdecl OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize);

	//队列接口
public:
	//触发接口
	virtual void __cdecl OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize/*, DWORD dwInsertTime*/);

	//接口定义
public:
	//数据库结果
	virtual bool __cdecl OnEventDataBaseResult(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//机器模拟接口
public:
	//应答事件
	virtual bool __cdecl OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientIP);
	//关闭事件
	virtual bool __cdecl OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientIP, DWORD dwActiveTime);
	//读取事件
	virtual bool __cdecl OnEventTCPNetworkRead(DWORD dwSocketID, CMD_Command Command, VOID * pData, WORD wDataSize);

public:
	//设置网络
	virtual bool __cdecl SetSocketEngine(IUnknownEx * pIUnknownEx);
	//获取接口
	virtual void * __cdecl GetQueueService(const IID & Guid, DWORD dwQueryVer);
};

//////////////////////////////////////////////////////////////////////////

#endif