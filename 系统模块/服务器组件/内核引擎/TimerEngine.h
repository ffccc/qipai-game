#ifndef TIMERENGINE_HEAD_FILE
#define TIMERENGINE_HEAD_FILE

#pragma once

//系统头文件

#include "KernelEngineHead.h"
#include "QueueServiceEvent.h"

#define TIMER_SPACE								25				//时间间隔

//类说明
class CTimerEngine;

//////////////////////////////////////////////////////////////////////////

//定时器线程
class CTimerThread : public CServiceThread
{
	//变量定义
protected:
	CTimerEngine						* m_pTimerEngine;				//定时器引擎
	//函数定义
public:
	//构造函数
	CTimerThread(void);
	//析构函数
	virtual ~CTimerThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(CTimerEngine * pTimerEngine);

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//定时器子项
struct tagTimerItem
{
	DWORD								wTimerID;						//定时器 ID
	DWORD								dwElapse;						//定时时间
	DWORD								dwTimeLeave;					//倒计时间
	DWORD								dwRepeatTimes;					//重复次数
	WPARAM								wBindParam;						//绑定参数
};

//定时器子项数组定义
typedef CArrayTemplate<tagTimerItem *> CTimerItemPtr;

//////////////////////////////////////////////////////////////////////////

//定时器引擎
class CTimerEngine : public ITimerEngine
{
	friend class CTimerThread;

	//状态变量
protected:
	bool								m_bService;						//运行标志
	CTimerItemPtr						m_TimerItemFree;				//空闲数组
	CTimerItemPtr						m_TimerItemActive;				//活动数组

	//组件变量
protected:
	CCriticalSection					m_CriticalSection; //	//线程锁
	CTimerThread						m_TimerThread;					//定时器线程
	IQueueServiceSink					*m_pIQueueServiceSink;			//通知回调

	//函数定义
public:
	//构造函数
	CTimerEngine(void);
	//析构函数
	virtual ~CTimerEngine(void);

	//基础接口
public:
	//释放对象
	virtual VOID __cdecl Release()
	{
		if (IsValid()) delete this;    //
	}
	//是否有效
	virtual bool __cdecl IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CTimerEngine)) ? true : false;
	}
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//接口函数
public:
	//设置定时器
	virtual bool __cdecl SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//删除定时器
	virtual bool __cdecl KillTimer(DWORD dwTimerID);
	//删除定时器
	virtual bool __cdecl KillAllTimer();
	//设置接口
	virtual bool __cdecl SetTimerEngineEvent(IUnknownEx * pIUnknownEx);

	//管理接口
public:
	//开始服务
	virtual bool __cdecl StartService();
	//停止服务
	virtual bool __cdecl ConcludeService();

	//内部函数
private:
	//定时器通知
	void OnTimerThreadSink();
};

//////////////////////////////////////////////////////////////////////////

#endif