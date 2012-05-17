#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#pragma once

//系统文件
#include "Stdafx.h"
#include "Resource.h"

//控件文件
#include "DlgWhisper.h"
#include "DlgCollocate.h"
#include "DlgSkinControl.h"
#include "ServerListView.h"

//窗口控件
#include "WndViewItemCtrl.h"
#include "WndUserInfoCtrl.h"
#include "PlatformPublicize.h"

//任务文件
#include "MissionList.h"
#include "MissionLogon.h"

//主体窗口
#include "PlazaViewItem.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////

//热键定义
#define IDI_HOT_KEY_BOSS			0x0100								//老板热键
#define IDI_HOT_KEY_WHISPER			0x0200								//私聊热键

//////////////////////////////////////////////////////////////////////////////////

//列表状态
enum enServerListControl
{
	ServerListControl_Hide,			//隐藏列表
	ServerListControl_Show,			//显示列表
	ServerListControl_Turn,			//翻转列表
};

//数组说明
typedef CWHArray<CServerViewItem *>	CServerViewItemArray;				//房间数组

//////////////////////////////////////////////////////////////////////////////////

//列表资源
class CListEncircle : public CEncircleBMP
{
	//函数定义
public:
	//构造函数
	CListEncircle();
	//析构函数
	virtual ~CListEncircle();

	//重载函数
protected:
	//绘画处理
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////////////

//框架资源
class CFrameEncircle : public CEncircleBMP
{
	//函数定义
public:
	//构造函数
	CFrameEncircle();
	//析构函数
	virtual ~CFrameEncircle();

	//重载函数
protected:
	//绘画处理
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////////////

//平台框架
class CPlatformFrame : public CFrameWnd, public ISplitterEvent, public IViewItemEvent, public ICustomFaceEvent
{
	//界面变量
protected:
	bool							m_bMaxShow;							//最大标志
	bool							m_bRectify;							//调整标志
	CRect							m_rcNormalSize;						//正常位置

	//状态变量
protected:
	bool							m_bWhisperIcon;						//私聊图标
	bool							m_bHideGameList;					//隐藏标志
	bool							m_bLockGameList;					//锁定标志

	//控制按钮
public:
	CSkinButton						m_btMin;							//最小按钮
	CSkinButton						m_btMax;							//最大按钮
	CSkinButton						m_btSkin;							//皮肤按钮
	CSkinButton						m_btClose;							//关闭按钮

	//控制按钮
public:
	CSkinButton						m_btControl;						//控制按钮
	CSkinButton						m_btLockList;						//锁定按钮

	//功能按钮
public:
	CSkinButton						m_btPlatform[3];					//平台按钮
	CSkinButton						m_btNavigation[5];					//导航按钮

	//框架控件
protected:
	CPlazaViewItem					m_PlazaViewItem;					//游戏广场
	CWndViewItemCtrl				m_WndViewItemCtrl;					//子项框架
	CWndUserInfoCtrl				m_WndUserInfoCtrl;					//用户信息

	//游戏列表
protected:
	CServerListView					m_ServerListView;					//游戏列表
	CServerListData					m_ServerListData;					//游戏数据

	//控件变量
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CSkinSplitter					m_SkinSplitter;						//拆分控件
	CDlgSkinControl					m_DlgSkinControl;					//界面控制
	CSystemTrayIcon					m_SystemTrayIcon;					//任务图标
	CPlatformPublicize				m_PlatformPublicize;				//浏览控件

	//任务组件
protected:
	CMissionList					m_MissionList;						//列表任务
	CMissionLogon					m_MissionLogon;						//登录任务
	CMissionManager					m_MissionManager;					//任务管理

	//控件数组
protected:
	CServerViewItem *				m_pServerViewItem;					//当前房间
	CDlgWhisperItemArray			m_DlgWhisperItemArray;				//私聊数组
	CServerViewItemArray			m_ServerViewItemArray;				//房间数组

	//资源变量
protected:
	CEncircleBMP					m_ItemEncircle;						//框架资源
	CListEncircle					m_ListEncircle;						//框架资源
	CFrameEncircle					m_FrameEncircle;					//框架资源

	//静态变量
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//框架指针

	//函数定义
public:
	//构造函数
	CPlatformFrame();
	//析构函数
	virtual ~CPlatformFrame();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//事件函数
protected:
	//子项事件
	virtual VOID OnViewItemEvent(WORD wItemIndex, CWnd * pItemView);
	//拆分事件
	virtual VOID OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos);

	//头像接口
protected:
	//下载失败
	virtual VOID OnEventSystemFace(DWORD dwUserID, WORD wFaceID);
	//头像数据
	virtual VOID OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);

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

	//功能函数
public:
	//浏览页面
	bool WebBrowse(LPCTSTR pszURL, bool bAutoFullView);
	//列表控制
	bool ControlServerList(enServerListControl ServerListControl, bool bFocusControl);

	//私聊函数
public:
	//提取私聊
	bool ShowWhisperItem();
	//插入私聊
	bool InsertWhisperItem(CDlgWhisper * pDlgWhisper);
	//删除私聊
	bool RemoveWhisperItem(CDlgWhisper * pDlgWhisper);

	//房间管理
public:
	//激活房间
	bool ActiveServerViewItem(CServerViewItem * pServerViewItem);
	//删除房间
	bool DeleteServerViewItem(CServerViewItem * pServerViewItem);

	//房间函数
public:
	//枚举房间
	CServerViewItem * EmunServerViewItem(WORD wIndex);
	//查找房间
	CServerViewItem * SearchServerViewItem(WORD wServerID);
	//进入房间
	CServerViewItem * EntranceServerItem(CGameKindItem * pGameKindItem);
	//进入房间
	CServerViewItem * EntranceServerItem(CGameServerItem * pGameServerItem);

	//界面函数
protected:
	//还原窗口
	bool RestoreWindow();
	//最大窗口
	bool MaxSizeWindow();

	//按钮消息
protected:
	//平台按钮
	VOID OnBnClickedPlatform1();
	//平台按钮
	VOID OnBnClickedPlatform2();
	//平台按钮
	VOID OnBnClickedPlatform3();

	//消息函数
protected:
	//关闭消息
	VOID OnClose();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//按键测试
	UINT OnNcHitTest(CPoint Point);
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//位置消息
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//改变消息
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	//自定消息
protected:
	//热键消息
	LRESULT OnMessageHotKey(WPARAM wParam, LPARAM lParam);
	//图标消息
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//获取实例
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif