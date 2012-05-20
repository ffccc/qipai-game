#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//扑克变量
protected:
	LONGLONG                            m_lApplyBankerCondition;

	LONGLONG                            m_lMeMaxScore;
	LONGLONG                            m_lMeTieScore;
	LONGLONG                            m_lMeBankerScore;
	LONGLONG                            m_lMePlayerScore;
	INT64								m_lAreaLimitScore;


	int									m_nBetArea;
	LONGLONG							m_nCanBetMoney;
	LONGLONG							m_nCell;
	LONGLONG							m_nMinMoney;
	LONGLONG							m_nMaxMoney;
	LONGLONG							m_nBetMoney;
	LONGLONG							m_nLimitBet;

	int									m_bIsAllowBanker;
	int									m_BankerMinTimes;
	int									m_BankerMaxTimes;

	//庄家信息
	protected:
	WORD                          		m_wCurrentBanker;
	LONGLONG                      		m_lCurrentBankerScore;
	bool                          		m_bMeCurrentBanker;
	bool                          		m_bAllowApplyBanker;
	bool                          		m_bMeApplyBanker;
	int                           		m_nBankerTime;

	//控件变量
protected:
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
	virtual VOID __cdecl Release() { if (IsValid()) delete this; return ; }
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
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//游戏开牌
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnChangeBanker(const void * pBuffer, WORD wDataSize);
	//更新积分
	bool OnChangeUserScore(const void * pBuffer, WORD wDataSize);
	//游戏记录
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//游戏积分
	bool OnSubGameScore(const void * pBuffer, WORD wDataSize);

private:

	//下注区域
	BYTE RandJettonArea();

	bool RandBoolean() { (rand() % 2) ==0 ? true : false; }
};

//////////////////////////////////////////////////////////////////////////

#endif