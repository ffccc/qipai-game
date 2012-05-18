#ifndef GAME_FRAME_ENGINE_HEAD_FILE
#define GAME_FRAME_ENGINE_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//驱动引擎
class GAME_FRAME_CLASS CGameFrameEngine : public CWnd, public IGameFrameEngine
{
	//友元定义
	friend class CGameFrameService;

	//组件接口
protected:
	IClientKernel *					m_pIClientKernel;					//内核接口
	IStringMessage *				m_pIStringMessage;					//信息接口

	//静态变量
protected:
	static CGameFrameEngine *		m_pGameFrameEngine;					//引擎接口

	//函数定义
public:
	//构造函数
	CGameFrameEngine();
	//析构函数
	virtual ~CGameFrameEngine();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//系统事件
public:
	//创建函数
	virtual bool OnInitGameEngine()=NULL;
	//重置函数
	virtual bool OnResetGameEngine()=NULL;

	//时钟事件
public:
	//时钟删除
	virtual bool OnEventGameClockKill(WORD wChairID)=NULL;
	//时钟信息
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)=NULL;

	//游戏事件
public:
	//旁观消息
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize)=NULL;
	//游戏消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//场景消息
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)=NULL;

	//模式函数
public:
	//单机模式
	bool IsSingleMode();
	//旁观用户
	bool IsLookonMode();
	//允许旁观
	bool IsAllowLookon();

	//状态函数
public:
	//获取状态
	BYTE GetGameStatus();
	//设置状态
	VOID SetGameStatus(BYTE cbGameStatus);

	//功能函数
public:
	//获取自己
	WORD GetMeChairID();
	//获取自己
	IClientUserItem * GetMeUserItem();
	//游戏用户
	IClientUserItem * GetTableUserItem(WORD wChariID);
	//游戏用户
	IClientUserItem * SearchTableUserItem(DWORD dwUserID);

	//发送函数
public:
	//发送函数
	bool SendSocketData(WORD wSubCmdID);
	//发送函数
	bool SendSocketData(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//功能接口
public:
	//发送准备
	bool SendUserReady(VOID * pData, WORD wDataSize);
	//发送表情
	bool SendUserExpression(DWORD dwTargetUserID, WORD wItemIndex);
	//发送聊天
	bool SendUserChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor);

	//时间函数
public:
	//时钟标识
	UINT GetClockID();
	//时钟位置
	WORD GetClockChairID();
	//删除时钟
	VOID KillGameClock(WORD wClockID);
	//设置时钟
	VOID SetGameClock(WORD wChairID, WORD wClockID, UINT nElapse);

	//辅助函数
public:
	//激活框架
	VOID ActiveGameFrame();
	//切换椅子
	WORD SwitchViewChairID(WORD wChairID);

	//声音函数
public:
	//播放声音
	bool PlayGameSound(LPCTSTR pszSoundName);
	//播放声音
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//内部函数
private:
	//重置引擎
	bool ResetGameFrameEngine();
	//创建引擎
	bool CreateGameFrameEngine();

	//静态函数
public:
	//获取实例
	static CGameFrameEngine * GetInstance() { return m_pGameFrameEngine; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif