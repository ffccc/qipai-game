#pragma once

#ifndef __AFXWIN_H__
#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "Resource.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//应用程序类
class CParrentApp : public CWinApp
{
	//变量定义
private:
	CGameParrentDlg					* m_pGameFrame;					//框架窗口
	CSkinRecordFile					m_SkinRecordFile;				//界面设置

	//函数定义
public:
	//构造函数
	CParrentApp();
	//析构函数
	virtual ~CParrentApp();

	//重载函数
public:
	//初始化函数
	virtual BOOL InitInstance();
	//退出消息
	virtual int ExitInstance();

	//继承函数
public:
	//创建主窗口
	virtual CGameParrentDlg * GetGameFrameDlg()=NULL;

	DECLARE_MESSAGE_MAP()
};

//应用程序对象
//extern CParrentApp theApp;

//////////////////////////////////////////////////////////////////////////
