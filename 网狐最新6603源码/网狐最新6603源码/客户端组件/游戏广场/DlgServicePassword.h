#ifndef SERVICE_PASSWORD_HEAD_FILE
#define SERVICE_PASSWORD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgServiceItem.h"

//////////////////////////////////////////////////////////////////////////////////

//修改资料
class CDlgServicePassword : public CDlgServiceItem, public CMissionItem
{
	//变量定义
protected:
	bool							m_bLogonMission;					//修改任务
	CMissionManager					m_MissionManager;					//任务管理

	//密码强度
protected:
	BYTE							m_cbLogonPasswordLevel;				//密码强度
	BYTE							m_cbInsurePasswordLevel;			//密码强度

	//密码资料
protected:
	TCHAR							m_szSrcPassword[LEN_PASSWORD];		//用户密码
	TCHAR							m_szDesPassword[LEN_PASSWORD];		//用户密码

	//控件变量
protected:
	CPasswordControl				m_edLogonPassword1;					//帐号密码
	CPasswordControl				m_edLogonPassword2;					//帐号密码
	CPasswordControl				m_edLogonPassword3;					//帐号密码
	CPasswordControl				m_edInsurePassword1;				//二级密码
	CPasswordControl				m_edInsurePassword2;				//二级密码
	CPasswordControl				m_edInsurePassword3;				//二级密码

	//控件变量
protected:
	CSkinButton						m_btLogonPassword;					//修改按钮
	CSkinButton						m_btInsurePassword;					//修改按钮

	//函数定义
public:
	//构造函数
	CDlgServicePassword();
	//析构函数
	virtual ~CDlgServicePassword();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//重载函数
public:
	//获取标题
	virtual LPCTSTR GetServiceItemName() { return TEXT("修改密码"); }

	//网络事件
private:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//事件处理
protected:
	//开始任务
	VOID OnMissionStart();
	//终止任务
	VOID OnMissionConclude();

	//消息映射
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//密码输入
	VOID OnEnChangeLogonPassword();
	//密码输入
	VOID OnEnChangeInserePassword();
	//密码修改
	VOID OnBnClickedLogonPassword();
	//密码修改
	VOID OnBnClickedInsurePassword();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif