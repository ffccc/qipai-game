#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\游戏客户端\GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink
{
	//游戏变量
protected:
	WORD							m_wCurrentUser;						//当前玩家
	WORD	                        m_wShowCardUser[GAME_PLAYER];       //亮牌玩家
	BYTE                            m_bShowCard[GAME_PLAYER][4];        //玩家亮牌
	BYTE                            m_bShowCardUserCount;               //玩家数目
	BYTE                            m_bShowCardTime;                    //亮牌次数
	BYTE                            m_bTotalShowCardCount;              //亮牌数目
	BYTE							m_bScoreCard[GAME_PLAYER][16];      //分数扑克
	BYTE                            m_bScoreCardCount[GAME_PLAYER];     //扑克数目
	BYTE							m_bShowCardCount[GAME_PLAYER];		//亮牌数目

	//运行信息
protected:
	WORD							m_wTurnWiner;						//胜利玩家
	WORD                            m_wFirstOutUser;                    //首先出牌
    WORD                            m_wGetGpadeUser;                    //得黑桃Q
	BYTE							m_bTurnCardCount;					//桌面牌数
	BYTE							m_bTurnCardData[GAME_PLAYER];		//桌面出牌

	//扑克信息
protected:
	BYTE							m_bCardCount[GAME_PLAYER];			//扑克数目
	BYTE							m_bHandCardData[GAME_PLAYER][HAND_CARD_COUNT];	//手上扑克

	//组件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	ITableFrame						* m_pITableFrame;					//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;				//配置参数

	//属性变量
protected:
	static const WORD				m_wPlayerCount;						//游戏人数
	static const enStartMode		m_GameStartMode;					//开始模式

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual void __cdecl Release() { }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void __cdecl RepositTableFrameSink();

	//信息接口
public:
	//开始模式
	virtual enStartMode __cdecl GetGameStartMode();
	//游戏状态
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//游戏事件
public:
	//游戏开始
	virtual bool __cdecl OnEventGameStart();
	//游戏结束
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//游戏事件
protected:
	//用户出牌
	bool OnUserOutCard(WORD wChairID, BYTE bCardData, BYTE bCardCount);
	//玩家亮牌
	bool OnUserShowCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	
};

//////////////////////////////////////////////////////////////////////////

#endif