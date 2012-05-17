#include "StdAfx.h"
#include "PlatformEvent.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CPlatformEvent * CPlatformEvent::m_pPlatformEvent=NULL;					//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlatformEvent::CPlatformEvent()
{
	//设置对象
	ASSERT(m_pPlatformEvent==NULL);
	if (m_pPlatformEvent==NULL) m_pPlatformEvent=this;

	return;
}

//析构函数
CPlatformEvent::~CPlatformEvent()
{
	//释放对象
	ASSERT(m_pPlatformEvent==this);
	if (m_pPlatformEvent==this) m_pPlatformEvent=NULL;

	return;
}

//注册窗口
bool CPlatformEvent::RegisterEventWnd(HWND hEventWnd)
{
	//效验参数
	ASSERT(IsWindow(hEventWnd));
	if (IsWindow(hEventWnd)==FALSE) return false;

	//存在判断
	for (INT_PTR i=0;i<m_EventWndArray.GetCount();i++)
	{
		if (m_EventWndArray[i]==hEventWnd)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//插入数组
	m_EventWndArray.Add(hEventWnd);

	return true;
}

//注销窗口
bool CPlatformEvent::UnRegisterEventWnd(HWND hEventWnd)
{
	//效验参数
	ASSERT(IsWindow(hEventWnd));
	if (IsWindow(hEventWnd)==FALSE) return false;

	//删除判断
	for (INT_PTR i=0;i<m_EventWndArray.GetCount();i++)
	{
		if (m_EventWndArray[i]==hEventWnd)
		{
			m_EventWndArray.RemoveAt(i);
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//发送事件
VOID CPlatformEvent::SendPlatformEvent(WORD wEventID, LPARAM lParam)
{
	//发送事件
	for (INT_PTR i=0;i<m_EventWndArray.GetCount();i++)
	{
		HWND hWnd=m_EventWndArray[i];
		if (IsWindow(hWnd)) SendMessage(hWnd,WM_PLATFORM_EVENT,wEventID,lParam);
	}

	return;
}

//发送事件
VOID CPlatformEvent::PostPlatformEvent(WORD wEventID, LPARAM lParam)
{
	//发送事件
	for (INT_PTR i=0;i<m_EventWndArray.GetCount();i++)
	{
		HWND hWnd=m_EventWndArray[i];
		if (IsWindow(hWnd)) PostMessage(hWnd,WM_PLATFORM_EVENT,wEventID,lParam);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
