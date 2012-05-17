#ifndef WMD_CHAT_CONTROL_HEAD_FILE
#define WMD_CHAT_CONTROL_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////

//用户数组
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////

//事件接口
interface IChatControlSink
{
	//用户选择
	virtual VOID OnChangeChatTarget(IClientUserItem * pIClientUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//聊天控制
class SHARE_CONTROL_CLASS CWndChatControl : public CWnd
{
	//控件变量
protected:
	CSkinButton						m_btClose;							//关闭按钮
	CSkinButton						m_btControl;						//控制按钮

	//变量定义
protected:
	IClientUserItem *				m_pIClientUserItem;					//目标用户
	IChatControlSink *				m_pIChatControlSink;				//事件接口
	CClientUserItemArray			m_ChatUserItemArray;				//用户数组

	//函数定义
public:
	//构造函数
	CWndChatControl();
	//析构函数
	virtual ~CWndChatControl();

	//重载函数
protected:
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//目标用户
public:
	//获取用户
	DWORD GetChatTargetUserID();
	//获取用户
	IClientUserItem * GetChatTargetUserItem();

	//功能函数
public:
	//删除用户
	VOID DeleteUserItem(IClientUserItem * pIClientUserItem);
	//设置用户
	VOID SetChatTargetUser(IClientUserItem * pIClientUserItem);
	//设置接口
	VOID SetChatControlSink(IChatControlSink * pIChatControlSink);

	//按钮消息
protected:
	//关闭按钮
	VOID OnBnClickedClose();
	//控制按钮
	VOID OnBnClickedControl();

	//消息映射
protected:
	//绘画函数
	VOID OnPaint();
	//销毁消息
	VOID OnDestroy();
	//位置变化
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif