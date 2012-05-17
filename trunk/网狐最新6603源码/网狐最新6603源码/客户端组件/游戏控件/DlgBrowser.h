#ifndef DLG_BROWSER_HEAD_FILE
#define DLG_BROWSER_HEAD_FILE

#pragma once

#include "WebPublicize.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//浏览窗口
class SHARE_CONTROL_CLASS CDlgBrowser : public CSkinDialog
{
	//变量定义
protected:
	CSize							m_SizeWindow;						//窗口大小
	CString							m_strBrowserURL;					//连接地址
	CString							m_strWindowTitle;					//标题信息

	//组件变量
protected:
	CWebPublicize					m_WebPublicize;						//浏览窗口

	//函数定义
public:
	//构造函数
	CDlgBrowser();
	//析构函数
	virtual ~CDlgBrowser();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//功能函数
public:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//创建窗口
	VOID CreateBrowserControl(LPCTSTR pszTitle, LPCTSTR pszBrowserURL, CSize SizeWindow);

	//消息函数
protected:
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif