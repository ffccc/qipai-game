#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//机器人类
class CAndroidUserItemSink : public IAndroidUserItemSink
{
protected:
	LONGLONG							m_lAreaLimitScore;					//区域限制
	LONGLONG							m_lUserLimitScore;					//区域限制
	LONGLONG							m_lApplyBankerCondition;			//申请条件

	LONGLONG							m_lUserJettonScore[AREA_COUNT+1];	//个人总注
	LONGLONG							m_lAllJettonScore[AREA_COUNT+1];	//全体总注

	LONGLONG							m_lMeMaxScore;						//最大下注
	//庄家信息
protected:
	LONGLONG							m_lBankerScore;						//庄家积分
	LONGLONG							m_lBankerWinScore;					//庄家成绩
	WORD								m_wBankerUser;						//当前庄家
	WORD								m_wBankerTime;						//做庄次数

	//BYTE								m_cbLeftCardCount;					//扑克数目
	bool								m_bEnableSysBanker;					//系统做庄

protected:
	BYTE								m_cbJettonArea;
	WORD								m_wJettonCount;

	bool								m_bMeCurrentBanker;
	bool								m_bApplyingBanker;
	bool								m_bCancelingBanker;
	WORD								m_wRandBankerTime;

	static BOOL							m_bAllowApplyBanker;
	static WORD							m_wMaxBankerTime;
	static INT							m_nMaxJettonRange;

	static CString						m_strConfigFile;
	static BOOL							m_bLoadConfig;

		//控件变量
protected:
	CGameLogic							m_GameLogic;						//游戏逻辑
	IAndroidUserItem *					m_pIAndroidUserItem;				//用户接口

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
	virtual void __cdecl OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {};
	//用户离开
	virtual void __cdecl OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {};
	//用户积分
	virtual void __cdecl OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
	//用户状态
	virtual void __cdecl OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}
	//用户段位
	virtual void __cdecl OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser) {}

	//消息处理
protected:
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//游戏空闲
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请做庄
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//取消做庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//切换庄家
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//游戏记录
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//下注失败
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);
	////扑克牌
	//bool OnSubSendCard(const void * pBuffer, WORD wDataSize);

protected:
	//最大下注
	LONGLONG GetUserMaxJetton();

	static void LoadConfig();
};

//////////////////////////////////////////////////////////////////////////

#endif
