#include "StdAfx.h"
#include "SystemTrayIcon.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSystemTrayIcon::CSystemTrayIcon()
{
	//属性变量
	m_uTrayID=0L;
	m_bIconVisible=false;

	//配置变量
	m_hWndSend=NULL;
	m_hIconTray=NULL;
	m_uMessage=WM_TRAY_ICON;

	return;
}

//析构函数
CSystemTrayIcon::~CSystemTrayIcon()
{
	//隐藏图标
	HideTrayIcon();

	return;
}

//配置图标
bool CSystemTrayIcon::InitTrayIcon(HWND hWndSend, UINT uTrayID, UINT uMessage)
{
	//设置变量
	m_uTrayID=uTrayID;
	m_hWndSend=hWndSend;
	m_uMessage=uMessage;

	return true;
}

//隐藏图标
bool CSystemTrayIcon::HideTrayIcon()
{
	//删除图标
	if (m_bIconVisible==true)
	{
		//通知变量
		NOTIFYICONDATA NotifyIconData;
		ZeroMemory(&NotifyIconData,sizeof(NotifyIconData));

		//设置变量
		NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);
		NotifyIconData.uID=m_uTrayID;
		NotifyIconData.hWnd=m_hWndSend;
		NotifyIconData.uFlags=NIF_ICON|NIF_MESSAGE;

		//设置图标
		m_bIconVisible=false;
		Shell_NotifyIcon(NIM_DELETE,&NotifyIconData);
	}

	return true;
}

//显示图标
bool CSystemTrayIcon::ShowTrayIcon(HICON hIcon)
{
	//设置变量
	m_hIconTray=hIcon;

	//通知变量
	NOTIFYICONDATA NotifyIconData;
	ZeroMemory(&NotifyIconData,sizeof(NotifyIconData));

	//构造数据
	NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);
	NotifyIconData.uID=m_uTrayID;
	NotifyIconData.hWnd=m_hWndSend;
	NotifyIconData.hIcon=m_hIconTray;
	NotifyIconData.uFlags=NIF_ICON;

	//设置图标
	if (m_bIconVisible==false)
	{
		m_bIconVisible=true;
		Shell_NotifyIcon(NIM_ADD,&NotifyIconData);
	}
	else
	{
		Shell_NotifyIcon(NIM_MODIFY,&NotifyIconData);
	}

	return true;
}

//显示图标
bool CSystemTrayIcon::ShowTrayIcon(HICON hIcon, LPCTSTR pszDescribe)
{
	//设置变量
	m_hIconTray=hIcon;

	//通知变量
	NOTIFYICONDATA NotifyIconData;
	ZeroMemory(&NotifyIconData,sizeof(NotifyIconData));

	//设置属性
	NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);

	//图标属性
	NotifyIconData.uID=m_uTrayID;
	NotifyIconData.hWnd=m_hWndSend;
	NotifyIconData.hIcon=m_hIconTray;
	NotifyIconData.uCallbackMessage=m_uMessage;
	NotifyIconData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	lstrcpyn(NotifyIconData.szTip,pszDescribe,CountArray(NotifyIconData.szTip));

	//设置图标
	if (m_bIconVisible==false)
	{
		m_bIconVisible=true;
		Shell_NotifyIcon(NIM_ADD,&NotifyIconData);
	}
	else
	{
		Shell_NotifyIcon(NIM_MODIFY,&NotifyIconData);
	}

	return true;
}

//显示提示
bool CSystemTrayIcon::ShowBalloonTip(LPCTSTR szMesssage, LPCTSTR szTitle)
{
	return true;
}

//显示提示
bool CSystemTrayIcon::ShowBalloonTip(LPCTSTR szMesssage, LPCTSTR szTitle, DWORD dwInfoFlags, UINT uTimeOut)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
