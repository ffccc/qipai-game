#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\消息定义\GameLogic.h"
#include "TableFrameSink.h"

//const __int64  money[10] = {100L,100L,1000L,10000L,500L,500L,5000L,1000L,5000L,10000L};
//const  __int64 money[7]={100, 1000, 10000,100000, 1000000 ,5000000,10000000};
//////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//扑克变量
protected:
	__int64							m_lMyMaxStake;						//最大下注
	BYTE							m_cbTurnCardCount;					//加注时间累计
	//BYTE							m_cbTurnCardData[MAX_COUNT];		//出牌列表

	__int64							m_Money;							//用户分数
	WORD							m_wCurrentBanker;					//当前庄家
	bool							m_first;							//刚进来
	__int64							m_lApplyBankerCondition;			//坐庄最少分数
	INT nOdds[INDEX_MAX]; //输赢赔率
	bool                            m_ShakeType[3];                     //摇骰子的方式

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口
	BYTE                            cbBankerValue;
	BYTE                            cbPlayerValue;	
	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAndroidUserItemSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool __cdecl InitUserItemSink(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool __cdecl RepositUserItemSink();

	//游戏事件
public:
	//时间消息
	virtual bool __cdecl OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool __cdecl OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//游戏消息
	virtual bool __cdecl OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//场景消息
	virtual bool __cdecl OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void __cdecl OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void __cdecl OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void __cdecl OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void __cdecl OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户段位
	virtual void __cdecl OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//消息处理
protected:
	//切换庄家
	bool OnSubChangeBanker(void * pData, WORD wDataSize);
	//时间状态
	bool OnSubTimerType(void * pData, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(void * pData, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(void * pData, WORD wDataSize);
	//申请做庄
	bool OnSubApplyBanker(void * pData, WORD wDataSize);

	//接收牌数据
	bool OnSubChangeCard(void * pData, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(void * pData, WORD wDataSize);
	int GetNextChair(int chair){ return (chair+GAME_PLAYER)%GAME_PLAYER;};
	int AutoOutCard(bool bOutCard);
public:
	// 机器人加注
	void OnAddJeetton(void);
	// 自己申请坐庄
	void OnAppyBanker(void);
};

//////////////////////////////////////////////////////////////////////////

#endif