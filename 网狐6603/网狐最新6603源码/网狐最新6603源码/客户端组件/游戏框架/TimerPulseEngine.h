#ifndef TIMER_PULSE_ENGINE_HEAD_FILE
#define TIMER_PULSE_ENGINE_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CTimerPulseEngine;

//////////////////////////////////////////////////////////////////////////////////

//脉冲线程
class GAME_FRAME_CLASS CTimerPulseThread : public CWHThread
{
	//变量定义
protected:
	DWORD							m_dwLastTickCount;					//处理时间
	DWORD							m_dwTimerPulseSpace;				//时间间隔

	//组件指针
protected:
	CTimerPulseEngine *				m_pTimerPulseEngine;				//时间引擎

	//函数定义
public:
	//构造函数
	CTimerPulseThread();
	//析构函数
	virtual ~CTimerPulseThread();

	//功能函数
public:
	//配置函数
	bool InitThread(CTimerPulseEngine * pTimerPulseEngine, DWORD dwTimerPulseSpace);

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////////////

//时间引擎
class GAME_FRAME_CLASS CTimerPulseEngine
{
	friend class CTimerPulseThread;

	//状态变量
protected:
	bool							m_bService;							//运行标志
	DWORD							m_dwTimePass;						//经过时间
	DWORD							m_dwTimeLeave;						//倒计时间

	//配置定义
protected:
	HWND							m_hWndKernel;						//内核窗口
	DWORD							m_dwTimerPulseSpace;				//时间间隔

	//组件变量
protected:
	CWHArray<WORD>					m_TimerPulseItem;					//脉冲子项
	CCriticalSection				m_CriticalSection;					//锁定对象
	CTimerPulseThread				m_TimerPulseThread;					//线程对象

	//函数定义
public:
	//构造函数
	CTimerPulseEngine();
	//析构函数
	virtual ~CTimerPulseEngine();

	//服务接口
public:
	//启动服务
	bool StartService();
	//停止服务
	bool ConcludeService();

	//配置接口
public:
	//设置接口
	VOID SetKernelWnd(HWND hWndKernel) { m_hWndKernel=hWndKernel; }

	//接口函数
public:
	//停止脉冲
	bool StopTimerPulse();
	//停止脉冲
	bool StopTimerPulse(WORD wPulseID);
	//启动脉冲
	bool StartTimerPulse(WORD wPulseID);

	//内部函数
private:
	//脉冲通知
	VOID OnTimerPulseThreadSink();
};

//////////////////////////////////////////////////////////////////////////////////

#endif