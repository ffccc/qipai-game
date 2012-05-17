#ifndef SERVICE_INDIVIDUAL_HEAD_FILE
#define SERVICE_INDIVIDUAL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgServiceItem.h"

//////////////////////////////////////////////////////////////////////////////////

//修改资料
class CDlgServiceIndividual : public CDlgServiceItem, public CMissionItem
{
	//友元定义
	friend class CDlgService;

	//变量定义
protected:
	bool							m_bQueryMission;						//查询任务
	bool							m_bModifyMission;						//修改任务
	CMissionManager					m_MissionManager;						//任务管理

	//帐号资料
protected:
	BYTE							m_cbGender;								//用户性别
	TCHAR							m_szNickName[LEN_NICKNAME];				//游戏昵称
	TCHAR							m_szPassword[LEN_PASSWORD];				//用户密码
	TCHAR							m_szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//详细资料
protected:
	TCHAR							m_szQQ[LEN_QQ];							//Q Q 号码
	TCHAR							m_szEMail[LEN_EMAIL];					//电子邮件
	TCHAR							m_szUserNote[LEN_USER_NOTE];			//用户说明
	TCHAR							m_szSeatPhone[LEN_SEAT_PHONE];			//固定电话
	TCHAR							m_szMobilePhone[LEN_MOBILE_PHONE];		//移动电话
	TCHAR							m_szCompellation[LEN_COMPELLATION];		//真实名字
	TCHAR							m_szDwellingPlace[LEN_DWELLING_PLACE];	//联系地址

	//控件变量
protected:
	CSkinEdit						m_edAccounts;							//用户帐号
	CSkinEdit						m_edNickName;							//用户昵称
	CSkinEdit						m_edPassword;							//修改密码
	CSkinEdit						m_edUnderWrite;							//个性签名

	//控件变量
protected:
	CSkinEdit						m_edQQ;									//Q Q 号码
	CSkinEdit						m_edEMail;								//电子邮件
	CSkinEdit						m_edUserNote;							//用户说明
	CSkinEdit						m_edSeatPhone;							//固定电话
	CSkinEdit						m_edMobilePhone;						//移动电话
	CSkinEdit						m_edCompellation;						//真实名字
	CSkinEdit						m_edDwellingPlace;						//联系地址

	//控件变量
protected:
	CSkinButton						m_btOk;									//修改按钮
	CSkinButton						m_btSelectFace;							//选择按钮

	//组件变量
protected:
	CFaceItemView					m_FaceItemView;							//头像视图
	CFaceSelectControlHelper		m_FaceSelectControl;					//头像选择

	//函数定义
public:
	//构造函数
	CDlgServiceIndividual();
	//析构函数
	virtual ~CDlgServiceIndividual();

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
	//获取标题
	virtual LPCTSTR GetServiceItemName() { return TEXT("用户资料"); }

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

	//加载函数
protected:
	//帐号信息
	VOID LoadAccountsInfo();
	//查询资料
	VOID QueryIndividualInfo();
	//更新资料
	VOID UpdateIndividualInfo();

	//消息映射
protected:
	//头像选择
	VOID OnBnClickedSelectFace();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif