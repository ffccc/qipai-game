#ifndef DLG_COLLOCATE_HEAD_FILE
#define DLG_COLLOCATE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CCollocateItem;

//数组说明
typedef CWHArray<CCollocateItem *> CCollocateItemArray;

//////////////////////////////////////////////////////////////////////////////////

//选择控制
class CCollocateBar : public CWnd
{
	//活动信息
protected:
	bool							m_bHovering;						//盘旋标志
	WORD							m_wItemDown;						//按下子项
	WORD							m_wItemHover;						//盘旋子项
	WORD							m_wItemActive;						//激活子项

	//子项信息
protected:
	CSize							m_SizeButton;						//按钮大小
	CRect							m_rcItemCreate;						//创建位置
	CCollocateItemArray				m_CollocateItemArray;				//控制数组

	//函数定义
public:
	//构造函数
	CCollocateBar();
	//析构函数
	virtual ~CCollocateBar();

	//重载函数
public:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//参数控制
public:
	//保存参数
	bool SaveParameter();
	//默认参数
	bool DefaultParameter();

	//功能函数
public:
	//设置区域
	bool SetItemCreateRect(CRect rcItemCreate);
	//插入子项
	bool InsertCollocateItem(CCollocateItem * pCollocateItem);
	//激活设置
	bool SetActiveCollocateItem(CCollocateItem * pCollocateItem);

	//内部函数
private:
	//索引切换
	WORD SwitchToButtonIndex(CPoint MousePoint);

	//消息函数
protected:
	//重画消息
	VOID OnPaint();
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标移动
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//光标消息
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//配置子项
class CCollocateItem : public CDialog
{
	//友元定义
	friend class CCollocateBar;
	friend class CDlgCollocate;

	//变量定义
protected:
	BYTE							m_cbImageIndex;						//图像索要

	//函数定义
public:
	//构造函数
	CCollocateItem(UINT uIDTemplate);
	//析构函数
	virtual ~CCollocateItem();

	//重载函数
protected:
	//确定消息
	virtual VOID OnOK() { GetParent()->PostMessage(WM_COMMAND,IDOK,0); }
	//取消消息
	virtual VOID OnCancel() { GetParent()->PostMessage(WM_COMMAND,IDCANCEL,0); }

	//重载函数
public:
	//保存参数
	virtual bool SaveParameter()=NULL;
	//默认参数
	virtual bool DefaultParameter()=NULL;
	//更新控制
	virtual bool UpdateControlStatus()=NULL;

	//功能函数
public:
	//创建窗口
	bool CreateCollocateItem(CWnd * pParentWnd, CRect rcCreate);

	//消息函数
protected:
	//背景消息
	BOOL OnEraseBkgnd(CDC * pDC);
	//颜色消息
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//同桌规则
class CCollocateServer : public CCollocateItem
{
	//胜率限制
public:
	WORD							m_wMinWinRate;						//最低胜率
	bool							m_bLimitWinRate;					//限制胜率

	//逃率限制
public:
	WORD							m_wMaxFleeRate;						//最高逃跑
	bool							m_bLimitFleeRate;					//限制断线

	//积分限制
public:
	LONG							m_lMaxGameScore;					//最高分数 
	LONG							m_lMinGameScore;					//最低分数
	bool							m_bLimitGameScore;					//限制分数

	//其他配置
public:
	bool							m_bLimitDetest;						//限制玩家
	bool							m_bLimitSameIP;						//效验地址

	//携带密码
public:
	bool							m_bTakePassword;					//携带密码
	TCHAR							m_szPassword[LEN_PASSWORD];			//桌子密码

	//编辑控件
protected:
	CSkinEdit						m_edWinRate;						//用户胜率
	CSkinEdit						m_edFleeRate;						//用户逃率
	CSkinEdit						m_edScoreMin;						//最低积分
	CSkinEdit						m_edScoreMax;						//最高积分
	CSkinEdit						m_edPassword;						//桌子密码

	//函数定义
public:
	//构造函数
	CCollocateServer();
	//析构函数
	virtual ~CCollocateServer();

	//重载函数
public:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//配置函数
	virtual BOOL OnInitDialog();

	//重载函数
public:
	//保存参数
	virtual bool SaveParameter();
	//默认参数
	virtual bool DefaultParameter();
	//更新控制
	virtual bool UpdateControlStatus();
};

//////////////////////////////////////////////////////////////////////////////////

//代理配置
class CCollocateProxy : public CCollocateItem
{
	//配置变量
public:
	BYTE							m_cbProxyType;						//代理类型
	tagProxyServer					m_ProxyServer;						//代理信息

	//按钮控件
protected:
	CSkinButton						m_btProxyTest;						//测试按钮
	CSkinComboBox					m_cmProxyType;						//代理类型

	//编辑控件
protected:
	CSkinEdit						m_edProxyPort;						//代理端口
	CSkinEdit						m_edProxyServer;					//代理地址
	CSkinEdit						m_edProxyUserName;					//代理用户
	CSkinEdit						m_edProxyPassword;					//代理密码

	//函数定义
public:
	//构造函数
	CCollocateProxy();
	//析构函数
	virtual ~CCollocateProxy();

	//重载函数
public:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//创建函数
	virtual BOOL OnInitDialog();

	//重载函数
public:
	//保存参数
	virtual bool SaveParameter();
	//默认参数
	virtual bool DefaultParameter();
	//更新控制
	virtual bool UpdateControlStatus();

	//消息函数
public:
	//代理测试
	VOID OnBnClickedProxyTest();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//消息设置
class CCollocateNotify : public CCollocateItem
{
	//配置变量
public:
	WORD							m_wWhisperHotKey;					//私聊热键
	bool							m_bAutoShowWhisper;					//显示消息

	//枚举变量
public:
	BYTE							m_cbInviteMode;						//邀请模式
	BYTE							m_cbMessageMode;					//消息模式

	//函数定义
public:
	//构造函数
	CCollocateNotify();
	//析构函数
	virtual ~CCollocateNotify();

	//重载函数
public:
	//保存参数
	virtual bool SaveParameter();
	//默认参数
	virtual bool DefaultParameter();
	//更新控制
	virtual bool UpdateControlStatus();
};

//////////////////////////////////////////////////////////////////////////////////

//习惯配置
class CCollocateCustom : public CCollocateItem
{
	//配置变量
public:
	bool							m_bSendWhisperByEnter;				//回车发送

	//动作定义
public:
	BYTE							m_cbActionHitAutoJoin;				//自动加入
	BYTE							m_cbActionLeftDoubleList;			//双击列表

	//函数定义
public:
	//构造函数
	CCollocateCustom();
	//析构函数
	virtual ~CCollocateCustom();

	//重载函数
public:
	//保存参数
	virtual bool SaveParameter();
	//默认参数
	virtual bool DefaultParameter();
	//更新控制
	virtual bool UpdateControlStatus();
};

//////////////////////////////////////////////////////////////////////////////////

//系统配置
class CCollocateSystem : public CCollocateItem
{
	//配置变量
public:
	WORD							m_wBossHotKey;						//老板热键

	//配置变量
public:
	bool							m_bAllowSound;						//允许声音
	bool							m_bAutoSitDown;						//自动坐下
	bool							m_bSalienceTable;					//突出桌子
	bool							m_bSaveWhisperChat;					//保存私聊
	bool							m_bNotifyUserInOut;					//进出消息
	bool							m_bNotifyFriendCome;				//好友提示
	bool							m_bFullScreenBrowse;				//全屏浏览

	//函数定义
public:
	//构造函数
	CCollocateSystem();
	//析构函数
	virtual ~CCollocateSystem();

	//重载函数
public:
	//保存参数
	virtual bool SaveParameter();
	//默认参数
	virtual bool DefaultParameter();
	//更新控制
	virtual bool UpdateControlStatus();
};

//////////////////////////////////////////////////////////////////////////////////

//配置窗口
class CDlgCollocate : public CSkinDialog
{
	//配置变量
protected:
	CParameterGame *				m_pParameterGame;					//游戏配置
	CParameterServer *				m_pParameterServer;					//房间配置

	//控件变量
protected:
	CSkinButton						m_btOk;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮
	CSkinButton						m_btDefault;						//默认按钮
	CCollocateBar					m_CollocateBar;						//子项按钮

	//配置子项
public:
	CCollocateProxy					m_CollocateProxy;					//代理配置
	CCollocateSystem				m_CollocateSystem;					//系统配置
	CCollocateCustom				m_CollocateCustom;					//习惯配置
	CCollocateNotify				m_CollocateNotify;					//消息配置
	CCollocateServer				m_CollocateServer;					//房间配置

	//函数定义
public:
	//构造函数
	CDlgCollocate();
	//析构函数
	virtual ~CDlgCollocate();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//功能函数
public:
	//配置参数
	bool InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer);

	//按钮消息
protected:
	//默认按钮
	VOID OnBnClickedDefault();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif