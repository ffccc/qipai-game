#ifndef SYSTEM_TRAY_ICON_HEAD_FILE
#define SYSTEM_TRAY_ICON_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//宏定义

#define WM_TRAY_ICON				(WM_USER+100)						//默认消息

//////////////////////////////////////////////////////////////////////////////////

//事件图标
class SHARE_CONTROL_CLASS CSystemTrayIcon
{
	//配置定义
protected:
	UINT							m_uMessage;							//发送消息
	HWND							m_hWndSend;							//发送窗口
	HICON							m_hIconTray;						//托盘图标

	//属性变量
protected:
	UINT							m_uTrayID;							//托盘标识
	bool							m_bIconVisible;						//可视标志

	//函数定义
public:
	//构造函数
	CSystemTrayIcon();
	//析构函数
	virtual ~CSystemTrayIcon();

	//配置函数
public:
	//配置图标
	bool InitTrayIcon(HWND hWndSend, UINT uTrayID, UINT uMessage=WM_TRAY_ICON);

	//图标函数
public:
	//隐藏图标
	bool HideTrayIcon();
	//显示图标
	bool ShowTrayIcon(HICON hIcon);
	//显示图标
	bool ShowTrayIcon(HICON hIcon, LPCTSTR pszDescribe);

	//提示函数
public:
	//显示提示
	bool ShowBalloonTip(LPCTSTR szMesssage, LPCTSTR szTitle);
	//显示提示
	bool ShowBalloonTip(LPCTSTR szMesssage, LPCTSTR szTitle, DWORD dwInfoFlags, UINT uTimeOut);
};

//////////////////////////////////////////////////////////////////////////////////

#endif