#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\游戏客户端\GameLogic.h"
#include "..\消息定义\CMD_Thirteen.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//组件变量
protected:
	CGameLogic							m_GameLogic;										//游戏逻辑
	ITableFrame							* m_pITableFrame;									//框架接口
	const tagGameServiceOption			* m_pGameServiceOption;								//配置参数
	bool								m_bBaoPai[GAME_PLAYER];								//玩家包牌

	//属性变量
protected:
	static const WORD					m_wPlayerCount;										//游戏人数
	static const enStartMode			m_GameStartMode;									//开始模式

	//游戏变量
protected:
	LONG								m_nChip[GAME_PLAYER];								//下注大小
	LONG								m_lMaxChip;											//最高下注
	BYTE								m_bUserCardData[GAME_PLAYER][HAND_CARD_COUNT];		//玩家扑克
	BYTE								m_bSegmentCard[GAME_PLAYER][3][5];					//分段扑克
	bool								m_bFinishSegment[GAME_PLAYER];						//完成分段
	WORD								m_wFirstCallBankerUser ;							//叫庄玩家
	WORD								m_wBanker;											//庄家玩家
	WORD								m_wCurrentCaller ;									//当前叫庄
	BYTE								m_bShowCardCount;									//摊牌数目
	BYTE								m_bCompleteCompareCount ;							//完成比较
	BYTE								m_bShowCardTime;									//摊牌时间
	BYTE								m_bSetChipCount;									//下注次数

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID __cdecl Release() { }
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

	//消息处理
protected:
	//设置下注
	bool OnSetChip(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//设置分段
	bool OnSetSegment(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//玩家摊牌
	bool OnShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//摊牌时间
	bool OnShowCardTime(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;





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

	//////////////////////////////////////////////////////////////////////////
	//请求事件
public:
	//请求同意
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//请求断线
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//请求重入
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//请求坐下
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true ;}
	//请求起来
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true ;}

	//动作事件
public:
		//用户断线
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//用户重入
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//用户坐下
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){return true ;}
};

//////////////////////////////////////////////////////////////////////////

#endif
