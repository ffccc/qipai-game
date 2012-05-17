#ifndef DLG_LOCK_MACHINE_HEAD_FILE
#define DLG_LOCK_MACHINE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//锁机设置
class CDlgLockMachine : public CSkinDialog, public CMissionItem
{
	//变量定义
protected:
	BYTE							m_cbMachine;						//绑定标志
	TCHAR							m_szPassword[LEN_PASSWORD];			//用户密码

	//控件变量
protected:
	CSkinButton						m_btOk;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮
	CPasswordControl				m_PasswordControl;					//用户密码

	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

	//函数定义
public:
	//构造函数
	CDlgLockMachine();
	//析够函数
	virtual ~CDlgLockMachine();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif