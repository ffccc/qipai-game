#ifndef GAME_FRAME_SERVIE_HEAD_FILE
#define GAME_FRAME_SERVIE_HEAD_FILE

#pragma once

#include "GameFrameWnd.h"
#include "GameFrameView.h"
#include "GameFrameEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//框架服务
class GAME_FRAME_CLASS CGameFrameService : public IGameFrameService, public IUserEventSink,
	public IClientKernelSink
{
	//界面接口
protected:
	IGameFrameWnd *					m_pIGameFrameWnd;					//框架接口
	IGameFrameControl *				m_pIGameFrameControl;				//控制接口

	//组件接口
protected:
	IClientKernel *					m_pIClientKernel;					//内核接口
	CGameFrameView *				m_pGameFrameView;					//游戏界面
	CGameFrameEngine *				m_pGameFrameEngine;					//框架引擎

	//函数定义
public:
	//构造函数
	CGameFrameService();
	//析构函数
	virtual ~CGameFrameService();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//控制接口
public:
	//重置游戏
	virtual VOID ResetGameClient();
	//关闭游戏
	virtual VOID CloseGameClient();

	//配置接口
public:
	//配置完成
	virtual bool OnSchemeFinish();
	//列表配置
	virtual bool SchemeColumnItem(tagColumnItem ColumnItem[], BYTE cbColumnCount);
	//道具配置
	virtual bool SchemePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount);

	//内核事件
public:
	//游戏消息
	virtual bool OnEventSocket(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//进程消息
	virtual bool OnEventProcess(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//游戏事件
public:
	//旁观消息
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//时钟事件
public:
	//时钟删除
	virtual bool OnEventGameClockKill(WORD wChairID);
	//时钟信息
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//用户事件
public:
	//用户进入
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户离开
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户积分
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户状态
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户头像
	virtual VOID OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//功能函数
public:
	//创建引擎
	bool CGameFrameService::CreateGameFrameService();
};

//////////////////////////////////////////////////////////////////////////////////

#endif