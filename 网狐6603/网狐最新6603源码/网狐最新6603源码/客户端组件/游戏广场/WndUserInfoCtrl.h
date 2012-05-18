#ifndef WND_USER_INFO_CTRL_HEAD_FILE
#define WND_USER_INFO_CTRL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//编辑控件
class CEditUnderWrite : public CSkinEditEx, public CMissionItem
{
	//友元定义
	friend class CWndUserInfoCtrl;

	//变量定义
protected:
	bool							m_bNeedSend;						//请求标志
	TCHAR							m_szUnderWrite[LEN_UNDER_WRITE];	//个性签名

	//变量定义
protected:
	CWnd *							m_pParentWnd;						//窗口对象
	CMissionManager					m_MissionManager;					//任务管理

	//函数定义
public:
	//构造函数
	CEditUnderWrite();
	//析构函数
	virtual ~CEditUnderWrite();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
public:
	//修改判断
	bool IsModifyStatus();
	//设置窗口
	VOID SetParentWindow(CWnd * pParentWnd);

	//内部函数
protected:
	//关闭控件
	VOID CloseUnderWrite();
	//更新签名
	VOID UpdateUnderWrite();

	//消息定义
protected:
	//消耗消息
	VOID OnDestroy();
	//失去焦点
	VOID OnKillFocus(CWnd * pNewWnd);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//用户信息
class CWndUserInfoCtrl : public CWnd
{
	//状态变量
protected:
	bool							m_bClickFace;						//点击标志
	bool							m_bMouseEvent;						//注册标志

	//盘旋标志
protected:
	bool							m_bHoverFace;						//盘旋标志
	bool							m_bHoverUnderWrite;					//盘旋标志

	//区域位置
protected:
	CRect							m_rcFaceArea;						//头像区域
	CRect							m_rcUnderWrite;						//签名区域

	//控件变量
public:
	CSkinButton						m_btUserInfo1;						//用户按钮
	CSkinButton						m_btUserInfo2;						//用户按钮
	CSkinButton						m_btUserInfo3;						//用户按钮
	CSkinButton						m_btUserInfoSet;					//用户按钮
	CEditUnderWrite					m_EditUnderWrite;					//编辑控件

	//组件变量
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件

	//函数定义
public:
	//构造函数
	CWndUserInfoCtrl();
	//析构函数
	virtual ~CWndUserInfoCtrl();

	//重载函数
protected:
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

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
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	//自定消息
protected:
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif