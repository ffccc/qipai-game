#ifndef CLIENT_KERNEL_HEAD_FILE
#define CLIENT_KERNEL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "TimerPulseEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef CWHArray<IUserEventSink *,IUserEventSink *> CUserEventSinkArray;

//////////////////////////////////////////////////////////////////////////////////

//内核框架
class CClientKernel : public CWnd, public IClientKernel, public IProcessControlSink, public IUserManagerSink
{
	//状态变量
protected:
	bool							m_bService;							//服务状态
	tagUserAttribute				m_UserAttribute;					//用户属性

	//时间信息
protected:
	WORD							m_wClockID;							//时钟标识
	UINT							m_nElapseCount;						//时钟计数
	WORD							m_wClockChairID;					//时钟位置

	//游戏状态
protected:
	bool							m_bAllowLookon;						//允许旁观
	BYTE							m_cbGameStatus;						//游戏状态
	IClientUserItem	*				m_pIMySelfUserItem;					//自己信息

	//游戏属性
protected:
	tagGameAttribute				m_GameAttribute;					//游戏属性
	tagServerAttribute				m_ServerAttribute;					//房间属性

	//接口变量
protected:
	IStringMessage *				m_pIStringMessage;					//信息接口
	IClientKernelSink *				m_pIClientKernelSink;				//内核接口

	//内核变量
protected:
	HWND							m_hProcessWnd;						//通讯窗口
	CUserEventSinkArray				m_UserEventSinkArray;				//用户事件

	//组件变量
protected:
	CTimerPulseEngine				m_TimerPulseEngine;					//时间引擎
	CProcessControlHelper			m_ProcessControlModule;				//进程传输
	CGameUserManagerHelper			m_GameUserManagerModule;			//用户管理

	//函数定义
public:
	//构造函数
	CClientKernel();
	//析构函数
	virtual ~CClientKernel();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//内核配置
	virtual bool InitClientKernel(LPCTSTR lpszComLine);
	//设置接口
	virtual bool SetUserEventSink(IUnknownEx * pIUnknownEx);
	//设置接口
	virtual bool SetStringMessage(IUnknownEx * pIUnknownEx);
	//设置接口
	virtual bool SetClientKernelSink(IUnknownEx * pIUnknownEx);
	//游戏属性
	virtual bool SetGameAttribute(WORD wKindID, WORD wPlayerCount, DWORD dwClientVersion, HICON hGameIcon, LPCTSTR pszGameName);

	//连接接口
public:
	//连接大厅
	virtual bool CreateConnect();
	//中断连接
	virtual bool IntermitConnect();

	//属性接口
public:
	//用户属性
	virtual tagUserAttribute * GetUserAttribute() { return &m_UserAttribute; }
	//游戏属性
	virtual tagGameAttribute * GetGameAttribute() { return &m_GameAttribute; }
	//房间属性
	virtual tagServerAttribute * GetServerAttribute() { return &m_ServerAttribute; }

	//模式接口
public:
	//单机模式
	virtual bool IsSingleMode();
	//旁观用户
	virtual bool IsLookonMode();
	//允许旁观
	virtual bool IsAllowLookon();
	//服务状态
	virtual bool IsServiceStatus();

	//状态接口
public:
	//获取状态
	virtual BYTE GetGameStatus() { return m_cbGameStatus; };
	//设置状态
	virtual VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; };

	//用户接口
public:
	//自己位置
	virtual WORD GetMeChairID();
	//自己位置
	virtual IClientUserItem * GetMeUserItem();
	//游戏用户
	virtual IClientUserItem * GetTableUserItem(WORD wChariID);
	//查找用户
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID);
	//查找用户
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID);
	//旁观用户
	virtual IClientUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//网络接口
public:
	//发送函数
	virtual bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//进程接口
public:
	//发送数据
	virtual bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID);
	//发送数据
	virtual bool SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//执行命令
	virtual bool SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//功能接口
public:
	//发送准备
	virtual bool SendUserReady(VOID * pData, WORD wDataSize);
	//旁观控制
	virtual bool SendUserLookon(DWORD dwUserID, bool bAllowLookon);
	//发送表情
	virtual bool SendUserExpression(DWORD dwTargetUserID, WORD wItemIndex);
	//发送聊天
	virtual bool SendUserChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);

	//辅助接口
public:
	//激活框架
	virtual VOID ActiveGameFrame();
	//切换椅子
	virtual WORD SwitchViewChairID(WORD wChairID);

	//时钟接口
public:
	//时钟标识
	virtual UINT GetClockID();
	//时钟位置
	virtual WORD GetClockChairID();
	//删除时钟
	virtual VOID KillGameClock(WORD wClockID);
	//设置时钟
	virtual VOID SetGameClock(WORD wChairID, WORD wClockID, UINT nElapse);

	//用户管理
public:
	//用户激活
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//用户删除
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);

	//数据接口
public:
	//进程数据
	virtual bool OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend);

	//数据处理
protected:
	//网络消息
	bool OnProcessMainSocket(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//控制信息
	bool OnProcessMainControl(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//配置消息
	bool OnProcessMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//用户消息
	bool OnProcessMainUserInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//用户聊天
	bool OnSocketSubUserChat(VOID * pData, WORD wDataSize);
	//用户表情
	bool OnSocketSubExpression(VOID * pData, WORD wDataSize);
	//游戏状态
	bool OnSocketSubGameStatus(VOID * pData, WORD wDataSize);
	//游戏场景
	bool OnSocketSubGameScene(VOID * pData, WORD wDataSize);
	//旁观状态
	bool OnSocketSubLookonStatus(VOID * pData, WORD wDataSize);
	//系统消息
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//动作消息
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);

	//内部函数
protected:
	//设置标题
	VOID SetGameFrameTitle();

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif