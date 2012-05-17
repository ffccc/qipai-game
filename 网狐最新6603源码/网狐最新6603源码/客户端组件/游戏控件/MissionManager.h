#ifndef MISSION_MANAGER_HEAD_FILE
#define MISSION_MANAGER_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//任务回调
interface IMissionSocketSink
{
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode)=NULL;
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason)=NULL;
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//任务连接
class SHARE_CONTROL_CLASS CMissionSocket : public ITCPSocketSink
{
	//状态变量
protected:
	bool							m_bSwitchDns;						//DNS 转换
	bool							m_bTryDefault;						//尝试默认
	WORD							m_wSwitchIndex;						//切换索引

	//连接信息
protected:
	WORD							m_wCurrentPort;						//当前端口
	DWORD							m_dwCurrentServer;					//当前地址
	TCHAR							m_szCustomServer[LEN_SERVER];		//指定地址
	TCHAR							m_szCurrentServer[LEN_SERVER];		//当前地址

	//组件变量
protected:
	CTCPSocketHelper				m_TCPSocketModule;					//网络连接

	//接口变量
protected:
	IMissionSocketSink *			m_pIMissionSocketSink;				//回调接口

	//函数定义
public:
	//构造函数
	CMissionSocket();
	//析构函数
	virtual ~CMissionSocket();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//网络接口
protected:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//状态函数
public:
	//网络状态
	BYTE GetSocketStatus();
	//当前连接
	LPCTSTR GetCurrentServer() { return m_szCurrentServer; }

	//配置函数
public:
	//设置地址
	VOID SetCustomServer(LPCTSTR pszCustomServer);
	//设置接口
	VOID SetMissionSocketSink(IMissionSocketSink * pIMissionSocketSink) { m_pIMissionSocketSink=pIMissionSocketSink; }

	//功能函数
public:
	//中断连接
	bool PerformClose();
	//发起连接
	bool PerformConnect(bool bContinue);
	//发送函数
	bool PerformSendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	bool PerformSendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	//内部函数
private:
	//地址信息
	bool GetServerInfo();
	//切换连接
	bool SwitchServerItem();
};

//////////////////////////////////////////////////////////////////////////////////

//任务子项
class SHARE_CONTROL_CLASS CMissionItem : public IMissionSocketSink
{
	//友元定义
	friend class CMissionManager;

	//变量定义
private:
	bool						m_bActiveStatus;						//任务状态
	CMissionManager *			m_pMissionManager;						//任务管理

	//函数定义
public:
	//构造函数
	CMissionItem();
	//析构函数
	virtual ~CMissionItem();

	//功能函数
public:
	//任务状态
	bool GetActiveStatus() { return m_bActiveStatus; }
	//任务管理
	CMissionManager * GetMissionManager() { return m_pMissionManager; }
};

//////////////////////////////////////////////////////////////////////////////////

//任务调遣
class SHARE_CONTROL_CLASS CMissionManager : public CWnd, public IMissionSocketSink
{
	//组件变量
protected:
	CMissionSocket					m_MissionSocket;					//任务连接

	//任务子项
protected:
	CWHArray<CMissionItem *>		m_MissionItemArray;					//任务子项

	//函数定义
public:
	//构造函数
	CMissionManager();
	//析构函数
	virtual ~CMissionManager();

	//任务接口
protected:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//管理函数
public:
	//设置地址
	VOID SetCustomServer(LPCTSTR pszCustomServer);
	//获取地址
	LPCTSTR GetCurrentServer() { return m_MissionSocket.GetCurrentServer(); }

	//任务管理
public:
	//加入任务
	bool InsertMissionItem(CMissionItem * pMissionItem);
	//删除任务
	bool DeleteMissionItem(CMissionItem * pMissionItem);

	//功能函数
public:
	//激活任务
	bool AvtiveMissionItem(CMissionItem * pMissionItem, bool bContinue);
	//终止任务
	bool ConcludeMissionItem(CMissionItem * pMissionItem, bool bDeferIntermit);

	//功能函数
public:
	//发送函数
	bool SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * const pData, WORD wDataSize);

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif