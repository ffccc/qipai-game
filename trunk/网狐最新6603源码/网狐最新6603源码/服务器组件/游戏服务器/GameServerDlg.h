#ifndef GAME_SERVER_SERVER_DLG_HEAD_FILE
#define GAME_SERVER_SERVER_DLG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServiceUnits.h"

//////////////////////////////////////////////////////////////////////////////////

//主对话框
class CGameServerDlg : public CDialog, public IServiceUnitsSink
{
	//组件变量
protected:
	CServiceUnits					m_ServiceUnits;						//服务单元
	CTraceServiceControl			m_TraceServiceControl;				//追踪窗口

	//组件变量
protected:
	CModuleDBParameter				m_ModuleDBParameter;				//模块参数

	//配置参数
protected:
	bool							m_bAutoControl;						//自动控制
	bool							m_bOptionSuccess;					//配置标志
	tagModuleInitParameter			m_ModuleInitParameter;				//配置参数

	//函数定义
public:
	//构造函数
	CGameServerDlg();
	//析构函数
	virtual ~CGameServerDlg();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//服务接口
public:
	//服务状态
	virtual VOID OnServiceUnitsStatus(enServiceStatus ServiceStatus);

	//辅助函数
protected:
	//更新图标
	VOID UpdateServerLogo(LPCTSTR pszServerDLL);
	//更新标题
	VOID UpdateServerTitle(enServiceStatus ServiceStatus);
	//更新状态
	VOID UpdateParameterStatus(tagModuleInitParameter & ModuleInitParameter);

	//服务控制
protected:
	//启动房间
	bool StartServerService(WORD wServerID);
	//获取连接
	bool InitDataBaseParameter(tagDataBaseParameter & DataBaseParameter);

	//按钮消息
protected:
	//启动服务
	VOID OnBnClickedStartService();
	//停止服务
	VOID OnBnClickedStopService();
	//打开房间
	VOID OnBnClickedOpenServer();
	//创建房间
	VOID OnBnClickedCreateServer();
	//配置房间
	VOID OnBnClickedOptionServer();

	//消息映射
public:
	//关闭询问
	BOOL OnQueryEndSession();
	//命令处理
	LRESULT OnMessageProcessCmdLine(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif