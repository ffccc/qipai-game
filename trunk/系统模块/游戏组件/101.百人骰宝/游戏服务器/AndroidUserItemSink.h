#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../消息定义/GameLogic.h"

//////////////////////////////////////////////////////////////////////////
typedef CArrayTemplate<DWORD,DWORD&> CApplyBankerArray;

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	int iRandLimit[7];
	//限制信息
protected:
	__int64							m_lMeMaxScore;						//最大下注
	__int64                         m_lAllMaxScore;                     // 当局可以下注的总注
	__int64							m_lAreaLimitScore;					//区域限制
	__int64							m_lApplyBankerCondition;			//申请条件
	bool                            m_bEnableNote;                      // 是否可以下注
	bool m_bEnableSysBanker;  // 是否可以系统坐庄

	//个人下注
protected:
	__int64							m_lMeScore[COUNT_AZIMUTH];          //每个方位总注
	__int64                         m_lAllScore[COUNT_AZIMUTH];			//每个方位总注
	//庄家信息
protected:
	__int64							m_lBankerScore;						//庄家积分
	WORD							m_wCurrentBanker;					//当前庄家

	CApplyBankerArray               m_arApplyBanker;                    // 申请庄家队列
	int m_iMaxAndroidBanker;                                           // 队列中最多机器人

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

	// 所有玩家都开始了
	virtual void __cdecl OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	// 下注处理
	bool OnPlaceJetton(void);

	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);

	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);

	// 最大下注
	__int64 GetMaxAndroidScore(int iArea);
};

//////////////////////////////////////////////////////////////////////////

#endif
