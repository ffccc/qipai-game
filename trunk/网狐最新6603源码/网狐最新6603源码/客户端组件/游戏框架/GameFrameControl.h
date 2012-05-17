#ifndef GAME_FRAME_CONTROL_HEAD_FILE
#define GAME_FRAME_CONTROL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameUserView.h"
#include "GameFrameMedalView.h"

//////////////////////////////////////////////////////////////////////////////////

//用户选择
#define MAX_HISTOY_USER				8									//最大数目

//用户数组
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//控制框架
class GAME_FRAME_CLASS CGameFrameControl : public CDialog, public IGameFrameControl, public IUserEventSink,
	public IChatControlSink, public IExpressionSink, public IGamePropertySink
{
	//界面变量
protected:
	bool							m_bCreateFlag;						//创建标志

	//聊天变量
protected:
	DWORD							m_dwChatTime;						//聊天时间
	CString							m_strChatString;					//发送信息

	//数组变量
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//用户数组

	//控件变量
protected:
	CSkinEditEx						m_ChatEdit;							//聊天输入
	CSkinButton						m_btSendChat;						//发送聊天
	CSkinButton						m_btChatShort;						//聊天短语
	CRichEditMessage				m_ChatMessage;						//聊天信息

	//聊天按钮
protected:
	CSkinButton						m_btChatButton1;					//聊天按钮
	CSkinButton						m_btChatButton2;					//聊天按钮
	CSkinButton						m_btChatButton3;					//聊天按钮
	CSkinButton						m_btChatButton4;					//聊天按钮

	//控件变量
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CWndChatControl					m_ChatControl;						//聊天控制
	CUserListControl				m_UserListControl;					//用户列表
	CGamePropertyCtrl				m_GamePropertyCtrl;					//游戏道具
	CGameFrameUserView				m_GameFrameUserView;				//用户信息
	CGameFrameMedalView				m_GameFrameMedalView;				//奖牌信息

	//控件变量
protected:
	CExpressionControl *			m_pExpressionControl;				//表情窗口

	//接口变量
protected:
	IClientKernel *					m_pIClientKernel;					//内核接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

	//资源变量
protected:
	CEncircleBMP					m_ChatEncircle;						//聊天资源
	CEncircleBMP					m_UserListEncircle;					//用户资源

	//函数定义
public:
	//构造函数
	CGameFrameControl();
	//析构函数
	virtual ~CGameFrameControl();

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
	//配置函数
	virtual BOOL OnInitDialog();
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//接口函数
protected:
	//重置控件
	virtual VOID ResetControl();
	//设置广告
	virtual VOID SetGameBillInfo(WORD wKindID, WORD wServerID);
	//设置用户
	virtual VOID SetUserViewInfo(IClientUserItem * pIClientUserItem);
	//列表配置
	virtual VOID SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount);
	//道具配置
	virtual VOID SetPropertyDescribe(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount);

	//道具接口
protected:
	//购买道具
	virtual bool OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);

	//用户菜单
protected:
	//显示菜单
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem);
	//显示菜单
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount);

	//用户事件
public:
	//用户进入
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户离开
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户积分
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户状态
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户头像
	virtual VOID OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//控件事件
protected:
	//用户选择
	virtual VOID OnChangeChatTarget(IClientUserItem * pIClientUserItem);
	//表情事件
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//界面函数
private:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//调整界面
	VOID RectifyControl(CSkinRenderManager * pSkinRenderManager);

	//内部函数
protected:
	//聊天效验
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);

	//聊天命令
protected:
	//更多颜色
	VOID OnSelectMoreColor();
	//选择颜色
	VOID OnSelectChatColor(UINT nCommandID);
	//选择短语
	VOID OnSelectChatShort(UINT nCommandID);

	//消息函数
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//右键列表
	VOID OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//选择改变
	VOID OnLvnItemchangedUserList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif