#ifndef DLG_PROXY_CONFIG_HEAD_FILE
#define DLG_PROXY_CONFIG_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//代理设置
class CDlgProxyConfig : public CSkinDialog
{
	//控件变量
protected:
	CSkinButton						m_btOk;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮
	CSkinButton						m_btProxyTest;						//测试按钮
	CSkinComboBox					m_cmProxyType;						//代理类型

	//编辑控件
protected:
	CSkinEdit						m_edPassword;						//用户密码
	CSkinEdit						m_edProxyPort;						//代理端口
	CSkinEdit						m_edProxyServer;					//代理地址
	CSkinEdit						m_edProxyUserName;					//代理用户
	CSkinEdit						m_edProxyPassword;					//代理密码

	//函数定义
public:
	//构造函数
	CDlgProxyConfig();
	//析够函数
	virtual ~CDlgProxyConfig();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//辅助函数
private:
	//获取信息
	bool GetInformation();

	//按钮消息
protected:
	//网络测试
	VOID OnBnClickedProxyTest();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif