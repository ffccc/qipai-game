#ifndef DLG_SERVER_ITEM_HEAD_FILE
#define DLG_SERVER_ITEM_HEAD_FILE

#pragma once

#include "ModuleManagerHead.h"
#include "ServerInfoManager.h"
#include "ServerListControl.h"

//////////////////////////////////////////////////////////////////////////////////

//主对话框
class MODULE_MANAGER_CLASS CDlgServerItem : public CDialog
{
	//列表变量
protected:
	CServerInfoBuffer				m_ServerInfoBuffer;					//模块信息
	CServerListControl				m_ServerListControl;				//房间列表

	//配置变量
public:
	tagModuleInitParameter			m_ModuleInitParameter;				//配置参数

	//函数定义
public:
	//构造函数
	CDlgServerItem();
	//析构函数
	virtual ~CDlgServerItem();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual VOID OnOK();

	//配置函数
public:
	//打开房间
	bool OpenGameServer();
	//打开房间
	bool OpenGameServer(WORD wServerID);

	//功能函数
protected:
	//加载房间
	bool LoadDBServerItem();
	//更新按钮
	bool UpdateControlStatus();
	//获取参数
	bool GetModuleInitParameter(tagGameServerInfo * pGameServerInfo, bool bAutoMode);

	//按钮消息
protected:
	//加载房间
	VOID OnBnClickedLoadServer();
	//刷新列表
	VOID OnBnClickedReLoadList();
	//删除房间
	VOID OnBnClickedDeleteServer();
	//组件属性
	VOID OnBnClickedServiceAttrib();

	//控件消息
protected:
	//双击列表
	VOID OnNMDblclkServerList(NMHDR * pNMHDR, LRESULT * pResult);
	//选择改变
	VOID OnLvnItemChangedServerList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif