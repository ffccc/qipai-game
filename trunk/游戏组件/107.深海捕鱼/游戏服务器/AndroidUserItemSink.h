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
	BYTE                            m_cbScene;
	int								m_nFishGold;						//渔币
	LONG						m_lCellScore;						//倍数
	BYTE							m_CannonType;						//子弹类型
	BYTE							m_SwapCannonType;					//要切换的子弹类型
	float							m_fLastFireRote;					//上一次的子弹角度
	
    //WORD                            m_wSceneSendFishCount;

    Role_Net_Object                m_RoleObjects[GAME_PLAYER];			//角色记录
    //Fish_Net_Object                m_FishObjects[MAX_FISH_OBJECT];

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
	virtual void __cdecl Release() { return ; }
	//virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }//改
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

//	//消息处理
//protected:
//
//	//游戏开始
//	bool OnSubStart(const void * pBuffer, WORD wDataSize);

	//内部函数
private:
	/// 购买子弹
	void BuyBullet();
	/// 切换子弹
	void ChangeCannon();
	/// 切换子弹
	void ChangeCannonOnce();
	/// 发射子弹
	void Fire();
};

//////////////////////////////////////////////////////////////////////////

#endif