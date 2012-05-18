#ifndef PROCESS_MANAGER_HEAD_FILE
#define PROCESS_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////////////

//进程管理
class CProcessManager : public CWnd, public IProcessManager, public IProcessControlSink
{
	//内核变量
protected:
	PROCESS_INFORMATION				m_ProcessInfo;						//进程信息
	IProcessManagerSink *			m_pIProcessManagerSink;				//回调接口

	//通讯变量
protected:
	HWND							m_hProcessControl;					//通讯窗口
	CProcessControlHelper			m_ProcessControlModule;				//进程传输

	//函数定义
public:
	//构造函数
	CProcessManager();
	//析构函数
	virtual ~CProcessManager();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//数据接口
public:
	//进程数据
	virtual bool OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend);

	//控制函数
public:
	//激活进程
	bool ActiveGameProcess();
	//关闭进程
	bool CloseGameProcess(bool bPerformSink);
	//分离进程
	bool DetachGameProcess(bool bPerformSink);
	//启动进程
	bool CreateGameProcess(LPCTSTR pszProcessName);
	//设置接口
	bool SetProcessManagerSink(IUnknownEx * pIUnknownEx);

	//进程数据
public:
	//发送数据
	bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID);
	//发送数据
	bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//执行命令
	bool SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//系统配置
public:
	//等级信息
	bool SendLevelInfo(tagLevelItem LevelItem[], BYTE cbLevelCount);
	//列表信息
	bool SendColumnInfo(tagColumnItem ColumnItem[], BYTE cbColumnCount);
	//道具信息
	bool SendPropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount);
	//房间信息
	bool SendServerInfo(IClientUserItem * pIMyselfUserItem, DWORD dwUserRight, DWORD dwMasterRight, DWORD dwServerRule, WORD wServerType, tagGameServer & GameServer);

	//用户状态
public:
	//用户进入
	bool SendUserItem(IClientUserItem * pIClientUserItem);
	//用户积分
	bool SendUserScore(DWORD dwUserID, tagUserScore & UserScore);
	//用户状态
	bool SendUserStatus(DWORD dwUserID, tagUserStatus & UserStatus);
	//用户头像
	bool SendCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo * pCustomFaceInfo);

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif