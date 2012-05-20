#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//消息定义
#define IDM_CARD_OPERATE			(WM_USER+300)						//扑克操作

//////////////////////////////////////////////////////////////////////////

//控制窗口
class CControlWnd : public CWnd
{
	//变量定义
protected:
	BYTE							m_cbCurrentCard;					//当前扑克

	//控件变量
protected:
	CSkinButton						m_btChi;							//吃牌按钮
	CSkinButton						m_btPeng;							//碰牌按钮
	CSkinButton						m_btChiHu;							//吃胡按钮
	CSkinButton						m_btGiveUp;							//放弃按钮

	//资源变量
protected:
	CSkinImage						m_ImageButtonBack;					//资源图片

	//函数定义
public:
	//构造函数
	CControlWnd();
	//析构函数
	virtual ~CControlWnd();

	//功能函数
public:
	//设置扑克
	void SetControlInfo(BYTE cbCurrentCard, BYTE cbOperateCode);

	//消息映射
protected:
	//重画函数
	afx_msg void OnPaint();
	//吃牌按钮
	afx_msg void OnChi();
	//碰牌按钮
	afx_msg void OnPeng();
	//吃胡按钮
	afx_msg void OnChiHu();
	//放弃按钮
	afx_msg void OnGiveUp();
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif