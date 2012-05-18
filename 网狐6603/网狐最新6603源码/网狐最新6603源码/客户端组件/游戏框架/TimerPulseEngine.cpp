#include "StdAfx.h"
#include "TimerPulseEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//宏定义
#define PULSE_TIMER					1000L								//脉冲时间
#define NO_TIME_LEAVE				DWORD(-1)							//忽略时间

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTimerPulseThread::CTimerPulseThread()
{
	//设置变量
	m_dwLastTickCount=0L;
	m_dwTimerPulseSpace=PULSE_TIMER;

	//组件接口
	m_pTimerPulseEngine=NULL;

	return;
}

//析构函数
CTimerPulseThread::~CTimerPulseThread()
{
}

//配置函数
bool CTimerPulseThread::InitThread(CTimerPulseEngine * pTimerPulseEngine, DWORD dwTimerPulseSpace)
{
	//设置变量
	m_dwLastTickCount=0L;
	m_dwTimerPulseSpace=dwTimerPulseSpace;

	//设置指针
	m_pTimerPulseEngine=pTimerPulseEngine;

	return true;
}

//运行函数
bool CTimerPulseThread::OnEventThreadRun()
{
	//效验参数
	ASSERT(m_pTimerPulseEngine!=NULL);

	//获取时间
	DWORD dwNowTickCount=GetTickCount();
	DWORD dwTimerPulseSpace=m_dwTimerPulseSpace;

	//等待调整
	if ((m_dwLastTickCount!=0L)&&(dwNowTickCount>m_dwLastTickCount))
	{
		DWORD dwHandleTickCount=dwNowTickCount-m_dwLastTickCount;
		dwTimerPulseSpace=(dwTimerPulseSpace>dwHandleTickCount)?(dwTimerPulseSpace-dwHandleTickCount):0L;
	}

	//定时处理
	Sleep(dwTimerPulseSpace);
	m_dwLastTickCount=GetTickCount();

	//时间处理
	m_pTimerPulseEngine->OnTimerPulseThreadSink();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTimerPulseEngine::CTimerPulseEngine()
{
	//设置变量
	m_bService=false;

	//状态变量
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEAVE;

	//配置变量
	m_hWndKernel=NULL;
	m_dwTimerPulseSpace=PULSE_TIMER;

	return;
}

//析构函数
CTimerPulseEngine::~CTimerPulseEngine()
{
	//停止服务
	ConcludeService();

	return;
}

//开始服务
bool CTimerPulseEngine::StartService()
{
	//状态效验
	ASSERT(m_bService==false);
	if (m_bService==true) return false;

	//设置变量
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEAVE;
	if (m_TimerPulseThread.InitThread(this,m_dwTimerPulseSpace)==false) return false;

	//启动服务
	if (m_TimerPulseThread.StartThread()==false) return false;

	//设置变量
	m_bService=true;

	return true;
}

//停止服务
bool CTimerPulseEngine::ConcludeService()
{
	//设置变量
	m_bService=false;

	//删除脉冲
	StopTimerPulse();

	//停止线程
	m_TimerPulseThread.ConcludeThread(INFINITE);

	return true;
}

//删除脉冲
bool CTimerPulseEngine::StopTimerPulse()
{
	//锁定资源
	CWHDataLocker DataLocker(m_CriticalSection);

	//设置变量
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEAVE;

	//删除脉冲
	m_TimerPulseItem.RemoveAll();

	return true;
}

//删除时间
bool CTimerPulseEngine::StopTimerPulse(WORD wPulseID)
{
	//锁定资源
	CWHDataLocker DataLocker(m_CriticalSection);

	//查找脉冲
	for (INT_PTR i=0;i<m_TimerPulseItem.GetCount();i++)
	{
		if (m_TimerPulseItem[i]==wPulseID)
		{
			//设置对象
			m_TimerPulseItem.RemoveAt(i);

			//系统参数
			if (m_TimerPulseItem.GetCount()==0)
			{
				m_dwTimePass=0L;
				m_dwTimeLeave=NO_TIME_LEAVE;
			}

			return true;
		}
	}

	return false;
}

//设置时间
bool CTimerPulseEngine::StartTimerPulse(WORD wPulseID)
{
	//锁定资源
	CWHDataLocker DataLocker(m_CriticalSection);

	//变量定义
	bool bPulseExist=false;

	//查找子项
	for (INT_PTR i=0;i<m_TimerPulseItem.GetCount();i++)
	{
		if (m_TimerPulseItem[i]==wPulseID)
		{
			bPulseExist=true;
			break;
		}
	}

	//创建子项
	if (bPulseExist==false)
	{
		m_TimerPulseItem.Add(wPulseID);
	}

	//系统参数
	m_dwTimePass=0L;
	m_dwTimeLeave=PULSE_TIMER;

	return false;
}

//脉冲通知
VOID CTimerPulseEngine::OnTimerPulseThreadSink()
{
	//锁定资源
	CWHDataLocker DataLocker(m_CriticalSection);

	//倒计时间
	if (m_dwTimeLeave==NO_TIME_LEAVE)
	{
		ASSERT(m_TimerPulseItem.GetCount()==0);
		return;
	}

	//效验状态
	ASSERT(m_dwTimeLeave>=m_dwTimerPulseSpace);
	if (m_dwTimeLeave<m_dwTimerPulseSpace) m_dwTimeLeave=m_dwTimerPulseSpace;

	//减少时间
	m_dwTimePass+=m_dwTimerPulseSpace;
	m_dwTimeLeave-=m_dwTimerPulseSpace;

	//查询子项
	if (m_dwTimeLeave==0)
	{
		//发送通知
		for (INT_PTR i=0;i<m_TimerPulseItem.GetCount();i++)
		{
			PostMessage(m_hWndKernel,WM_TIMER,m_TimerPulseItem[i],0L);
		}

		//系统参数
		m_dwTimePass=0L;
		m_dwTimeLeave=PULSE_TIMER;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
