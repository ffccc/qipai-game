#ifndef DLG_INSURE_COUNTER_HEAD_FILE
#define DLG_INSURE_COUNTER_HEAD_FILE

#pragma once

#include "MissionManager.h"
#include "PasswordControl.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//数据类型
#define DT_PLAZA					0									//大厅数据
#define DT_FRAME					1									//框架数据
#define DT_SERVER					2									//房间数据

//////////////////////////////////////////////////////////////////////////////////

//银行信息
struct tagUserInsureInfo
{
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//////////////////////////////////////////////////////////////////////////////////

//银行接口
interface IInsureCounterAction
{
	//查询事件
	virtual VOID PerformQueryInfo()=NULL;
	//存入事件
	virtual VOID PerformSaveScore(SCORE lSaveScore)=NULL;
	//取出事件
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//保险柜类
class SHARE_CONTROL_CLASS CDlgInsureItem : public CSkinDialog, public IInsureCounterAction
{
	//变量定义
protected:
	bool							m_bSaveMission;						//存款任务
	bool							m_bTakeMission;						//取款任务
	bool							m_bQueryMission;					//查询任务

	//查询变量
protected:
	bool							m_bInitInfo;						//银行信息
	tagUserInsureInfo				m_UserInsureInfo;					//银行信息

	//控件变量
protected:
	CSkinEditEx						m_edScore;							//操作金币
	CSkinButton						m_btQueryInfo;						//查询按钮
	CSkinButton						m_btTakeScore;						//取款按钮
	CSkinButton						m_btSaveScore;						//存款按钮

	//其他控件
protected:
	CSkinTabCtrl					m_TabControl;						//操作选择
	CSkinHyperLink					m_ForgetInsure;						//忘记密码
	CPasswordControl				m_PasswordControl;					//用户密码

	//函数定义
public:
	//构造函数
	CDlgInsureItem();
	//析构函数
	virtual ~CDlgInsureItem();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//确定按钮
	virtual VOID OnOK();

	//窗口函数
protected:
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//事件处理
protected:
	//开始任务
	VOID OnMissionStart();
	//终止任务
	VOID OnMissionConclude();

	//功能函数
protected:
	//查询资料
	VOID QueryUserInsureInfo();
	//获取金币
	SCORE GetUserControlScore();

	//内部函数
protected:
	//绘画数字
	VOID DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos);
	//转换字符
	VOID SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize);
	//转换字符
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);

	//消息函数
protected:
	//输入信息
	VOID OnEnChangeScore();
	//查询按钮
	VOID OnBnClickedQueryInfo();
	//存款按钮
	VOID OnBnClickedSaveScore();
	//取款按钮
	VOID OnBnClickedTakeScore();
	//忘记密码
	VOID OnBnClickedForgetInsure();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//广场银行
class SHARE_CONTROL_CLASS CDlgInsurePlaza : public CDlgInsureItem, public CMissionItem
{
	//变量定义
protected:
	SCORE							m_lScore;							//金币数目
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//银行密码

	//组件变量
protected:
	CMissionManager					m_MissionManager;					//任务管理

	//函数定义
public:
	//构造函数
	CDlgInsurePlaza();
	//析构函数
	virtual ~CDlgInsurePlaza();

	//执行函数
protected:
	//查询事件
	virtual VOID PerformQueryInfo();
	//存入事件
	virtual VOID PerformSaveScore(SCORE lSaveScore);
	//取出事件
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);

	//网络事件
private:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

//房间银行
class SHARE_CONTROL_CLASS CDlgInsureServer : public CDlgInsureItem
{
	//变量定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

	//函数定义
public:
	//构造函数
	CDlgInsureServer();
	//析构函数
	virtual ~CDlgInsureServer();

	//执行函数
protected:
	//查询事件
	virtual VOID PerformQueryInfo();
	//存入事件
	virtual VOID PerformSaveScore(SCORE lSaveScore);
	//取出事件
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);

	//功能函数
public:
	//配置银行
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//银行消息
	bool OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

#endif