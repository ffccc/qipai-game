#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//限制信息
protected:
	LONGLONG							m_lMeMaxScore;						//最大下注
	LONGLONG							m_lAreaLimitScore;					//区域限制
	LONGLONG							m_lApplyBankerCondition;			//申请条件

	//下注信息
protected:
	//全局下注
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];		//全体总注
	//玩家下注
	LONGLONG						m_lMeUserJettonScore[AREA_COUNT+1];		//个人总注
	//扑克信息
	BYTE							m_cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE];				//桌面扑克
	BYTE							m_cbLeftCardCount;						//扑克数目

	//成绩报告
	LONGLONG						m_lEndBankerScore;					//庄家成绩
	LONGLONG						m_lEndUserScore;					//玩家成绩
	LONGLONG						m_lEndUserReturnScore;				//返回积分
	LONGLONG						m_lEndRevenue;						//游戏税收

	//成绩报告
	LONGLONG						m_lMeStatisticScore;				//玩家成绩

	//当庄信息
protected:
	INT								m_nMeBankerCount;					//当庄次数
	INT								m_nMePlayerCount;					//闲家次数

	//庄家信息
protected:
	LONGLONG						m_lBankerScore;						//庄家积分
	WORD							m_wCurrentBanker;					//当前庄家
	WORD							m_cbBankerTime;						//庄家局数
	LONGLONG						m_lBankerWinScore;					//庄家成绩

	//申请列表
protected:
	//static CArrayTemplate<WORD>		m_AppyBankerAdroidUserArray;		//申请玩家
	int								m_AppyBankerAdroidUserCount;		//申请数量
	//控制变量
protected:
	bool							m_bEnableAndroidUserBanker;			//允许做庄
	bool							m_bEnableSysBanker;					//允许系统做庄
	bool							m_bAppyBanker;						//是否申请中

	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;				//用户接口

	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID __cdecl Release() { }
	//是否有效
	virtual bool __cdecl IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CAndroidUserItemSink)) ? true : false;
	}
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

	//辅助函数
private:
	//随机下注
	void AndroidUserPlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//申请做庄
	void RandomApplyBanker();

	//游戏消息
private:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户下注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//下注失败
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	//游戏空闲
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//取消申请
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//申请坐庄失败
	//bool OnSubUserApplyFail(const void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif
