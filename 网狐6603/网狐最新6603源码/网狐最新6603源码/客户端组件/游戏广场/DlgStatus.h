#ifndef DLG_STATUS_HEAD_FILE
#define DLG_STATUS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//状态接口
interface IStatusViewSink
{
	//取消连接
	virtual VOID OnStatusCancel()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//状态窗口
class CDlgStatus : public CDialog
{
	//状态变量
protected:
	UINT							m_uImageIndex;						//位图位置
	CString							m_strDescribe;						//描述信息

	//接口变量
protected:
	IStatusViewSink *				m_pIStatusViewSink;					//回调接口

	//控件变量
protected:
	CSkinButton						m_btGiveUp;							//取消按钮
	CSkinLayered					m_SkinLayered;						//分层窗口

	//函数定义
public:
	//构造函数
	CDlgStatus();
	//析构函数
	virtual ~CDlgStatus();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();

	//功能函数
public:
	//关闭窗口
	VOID HideStatusWindow();
	//设置消息
	VOID ShowStatusWindow(LPCTSTR pszDescribe);
	//设置接口
	VOID SetStatusViewSink(IStatusViewSink * pIStatusViewSink);

	//消息映射
protected:
	//取消按钮
	VOID OnBnClickedGiveUp();
	//时间消息
	VOID OnTimer(UINT nIDEvent);
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//显示消息
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//位置改变
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif