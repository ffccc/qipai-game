#ifndef DLG_SERVICE_ITEM_HEAD_FILE
#define DLG_SERVICE_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//服务子项
class CDlgServiceItem : public CDialog
{
	//友元定义
	friend class CDlgService;
	friend class CDlgServiceBar;

	//变量定义
protected:
	BYTE							m_cbImageIndex;						//图像索要

	//函数定义
public:
	//构造函数
	CDlgServiceItem(UINT uIDTemplate);
	//析构函数
	virtual ~CDlgServiceItem();

	//重载函数
protected:
	//确定消息
	virtual VOID OnOK() { return; }
	//取消消息
	virtual VOID OnCancel() { return; }

	//重载函数
public:
	//获取标题
	virtual LPCTSTR GetServiceItemName()=NULL;
	//绘画背景
	virtual VOID DrawItemView(CDC * pDC, INT nWidth, INT nHeight);

	//功能函数
public:
	//创建窗口
	VOID CreateServiceItem(CWnd * pParentWnd, CRect rcCreate);
	//获取字符
	VOID GetControlItemText(UINT uID, TCHAR szString[], WORD wMaxCount);

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//颜色消息
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//网页子项
class CDlgServiceBrowser : public CDlgServiceItem
{
	//变量定义
protected:
	TCHAR							m_szItemName[32];					//子项名字
	TCHAR							m_szServiceUrl[32];					//连接地址

	//控件变量
protected:
	CWebPublicize					m_ServicePublicize;					//浏览控件

	//函数定义
public:
	//构造函数
	CDlgServiceBrowser();
	//析构函数
	virtual ~CDlgServiceBrowser();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//获取标题
	virtual LPCTSTR GetServiceItemName() { return m_szItemName; }

	//功能函数
public:
	//设置服务
	VOID InitService(BYTE cbImageIndex, LPCTSTR pszItemName, LPCTSTR pszServiceUrl);

	//消息函数
protected:
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif