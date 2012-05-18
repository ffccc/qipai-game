#ifndef PLAZA_VIEW_ITEM_HEAD_FILE
#define PLAZA_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//大厅浏览
class CPlazaBrowser : public CHtmlView
{
	//函数定义
public:
	//构造函数
	CPlazaBrowser();
	//析构函数
	virtual ~CPlazaBrowser();

	//重载函数
protected:
	//绑定函数
	virtual VOID PreSubclassWindow();
	//消耗处理
	virtual VOID PostNcDestroy();

	//功能函数
public:
	//创建函数
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, CCreateContext * pContext=NULL);

	//消息函数
public:
	//关闭事件
	VOID OnWindowClosing(BOOL IsChildWindow, BOOL * bCancel);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//游戏广场
class CPlazaViewItem : public CDialog, public IUnknownEx
{
	//状态变量
protected:
	bool							m_bCreateFlag;						//创建标志

	//按钮控件
protected:
	CSkinButton						m_btWebLast;						//网页按钮
	CSkinButton						m_btWebNext;						//网页按钮
	CSkinButton						m_btWebStop;						//网页按钮
	CSkinButton						m_btWebHome;						//网页按钮
	CSkinButton						m_btWebReload;						//网页按钮

	//控件变量
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CPlazaBrowser					m_PlazaBrowser;						//浏览控件

	//资源变量
protected:
	CEncircleBMP					m_BrowserEncircle;					//框架资源

	//控件指针
protected:
	static CPlazaViewItem *			m_pPlazaViewItem;					//广场指针

	//函数定义
public:
	//构造函数
	CPlazaViewItem();
	//析构函数
	virtual ~CPlazaViewItem();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//创建函数
	virtual BOOL OnInitDialog();
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//确定函数
	virtual VOID OnOK() { return; };
	//取消函数
	virtual VOID OnCancel() { return; };

	//功能函数
public:
	//浏览页面
	bool WebBrowse(LPCTSTR pszURL);
	//获取实例
	static CPlazaViewItem * GetInstance() { return m_pPlazaViewItem; }

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif