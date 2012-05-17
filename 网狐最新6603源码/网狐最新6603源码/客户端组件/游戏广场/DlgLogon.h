#ifndef DLG_LOGON_HEAD_FILE
#define DLG_LOGON_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServerControl.h"
#include "AccountsControl.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//登录方式
#define LOGON_BY_GAME_ID			1									//I D 登录
#define LOGON_BY_ACCOUNTS			0									//帐号登录

//////////////////////////////////////////////////////////////////////////////////

//账号信息
struct tagAccountsInfo
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szGameID[16];						//游戏标识
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录账号
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};

//数组定义
typedef CWHArray<tagAccountsInfo *>	CAccountsInfoArray;					//账号数组

//////////////////////////////////////////////////////////////////////////////////

//登录窗口
class CDlgLogon : public CDialog
{
	//配置变量
public:
	BYTE							m_cbLogonMode;						//登录模式
	BYTE							m_cbRemPassword;					//记住密码

	//登录信息
public:
	DWORD							m_dwGameID;							//游戏 I D
	TCHAR							m_szAccounts[LEN_ACCOUNTS];			//游戏帐号
	TCHAR							m_szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							m_szLogonServer[LEN_SERVER];		//登录地址

	//控件变量
protected:
	CSkinButton						m_btQuit;							//退出按钮
	CSkinButton						m_btEnter;							//登录按钮
	CSkinButton						m_btOption;							//设置按钮
	CSkinButton						m_btRegister;						//注册按钮
	CSkinButton						m_btDeleteRecord;					//删除按钮

	//帐号控件
protected:
	CAccountsCtrl					m_GameIDControl;					//登录 I D
	CAccountsCtrl					m_AccountsControl;					//登录帐号

	//按钮控件
protected:
	CSkinButton						m_btLogonLink1;						//连接按钮
	CSkinButton						m_btLogonLink2;						//连接按钮
	CSkinButton						m_btLogonLink3;						//连接按钮
	CSkinButton						m_btLogonLink4;						//连接按钮

	//特殊控件
protected:
	CSkinTabCtrl					m_TabControl;						//登录模式
	CServerControl					m_ServerControl;					//登录地址
	CPasswordControl				m_PasswordControl;					//用户密码

	//界面控件
protected:
	CBrush							m_brBrush;							//背景画刷
	CSkinLayered					m_SkinLayered;						//分层窗口
	CPlatformPublicize				m_PlatformPublicize;				//浏览控件

	//变量定义
protected:
	CAccountsInfoArray				m_AccountsInfoArray;				//账号信息

	//函数定义
public:
	//构造函数
	CDlgLogon();
	//析构函数
	virtual ~CDlgLogon();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//功能函数
public:
	//构造数据
	WORD ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize);

	//辅助函数
private:
	//获取信息
	bool GetInformation();
	//加载账号
	VOID LoadAccountsInfo();
	//地址信息
	VOID LoadLogonServerInfo();

	//连接按钮
protected:
	//连接按钮
	VOID OnBnClickedLogonLink1();
	//连接按钮
	VOID OnBnClickedLogonLink2();
	//连接按钮
	VOID OnBnClickedLogonLink3();
	//连接按钮
	VOID OnBnClickedLogonLink4();

	//按钮消息
protected:
	//注册帐号
	VOID OnBnClickedRegister();
	//网络设置
	VOID OnBnClickedProxyConfig();
	//删除记录
	VOID OnBnClickedDeleteRecord();

	//控件函数
protected:
	//选择改变
	VOID OnSelchangeGameID();
	//选择改变
	VOID OnSelchangeAccounts();
	//编辑改变
	VOID OnCbnEditchangeAccounts();
	//类型改变
	VOID OnTcnSelchangeLogonMode(NMHDR * pNMHDR, LRESULT * pResult);

	//系统消息
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//显示消息
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//位置改变
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//控件颜色
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif