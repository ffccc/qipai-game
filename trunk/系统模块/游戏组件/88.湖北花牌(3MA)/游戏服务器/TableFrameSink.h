#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//1、在整个游戏过程中，其值是相关的变量
protected:
	WORD				m_wBankerUser;							//庄家用户--不同局中，要保留其值
	//2、仅决定于整局游戏，而在不同局中都要清空的变量
	//游戏变量
protected:
	BYTE				m_cbLeftCardCount;						//桌面牌堆中所剩余牌的个数--不同局中，必须清空其值
	BYTE				m_cbRepertoryCard[MAX_REPERTORY];		//每局牌的库存牌信息--不同局中，必须清空其值
	
	BYTE				m_cbSendCardCount;						//总发牌次数--不同局中，必须清空其值

	BYTE				m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//用户扑克--不同局中，必须清空其值

	BYTE				m_cbWeaveItemCount[GAME_PLAYER];		//各用户的组合数目--不同局中，必须清空其值
	tagWeaveItem		m_WeaveItemArray[GAME_PLAYER][8];		//各用户的组合信息--不同局中，必须清空其值

	BYTE				m_cbDiscardCount[GAME_PLAYER];			//每个用户丢弃牌的个数--不同局中，必须清空其值
	BYTE				m_cbDiscardCard[GAME_PLAYER][34];		//每个用户丢弃牌的信息--不同局中，必须清空其值

	BYTE                m_cbHuaCardIndex[GAME_PLAYER][5];        //三个用户手中各花牌的个数--不同局中，必须清空其值
	BYTE                m_cbHuaWeaveCardIndex[GAME_PLAYER][5];   //三个用户组合牌中各花牌的个数--不同局中，必须清空其值

	BYTE                m_SiTongCount[GAME_PLAYER];              //每个用户自摸牌时的四藏操作次数--不同局中，必须清空其值
	BYTE                m_SiTongIndex[GAME_PLAYER][8];           //四藏操作时的牌索引

	BYTE                m_WuTongCount[GAME_PLAYER];              //每个用户自摸牌时的五藏操作次数--不同局中，必须清空其值
	BYTE                m_WuTongIndex[GAME_PLAYER][8];           //五藏操作时的牌索引

	bool                m_bGameStart;                            //标记庄家是否可以正式出牌
	bool				b_HuangZhuang;							//是否荒庄--不同局中，必须清空其值

	//3、仅作用于每轮操作过程中的，每轮之后都要清空的变量
protected:
	BYTE			    m_cbChiHuCard;				             //保存能胡哪张牌
	tagHuCardResult		m_HuCardResult[GAME_PLAYER];			 //每次分析用户能否胡牌时，保存其结果

	BYTE               m_bSendCount;                            //每次发牌的张数--每次发牌之前，要确定其值
	BYTE				m_cbSendCardData[2];					//每次发牌的数据

	//运行变量
	WORD				m_wResumeUser;							//当出现跳用户操作牌时，被跳还的第一个用户信息，用于还原用户
	WORD				m_wCurrentUser;							//当前用户

	WORD				m_wProvideUser;							//提供操作牌的用户
	BYTE				m_cbProvideCard;						//当用户能操作牌时，这中保存有能操作的牌

	WORD				m_wOutCardUser;							//出牌用户--每次出牌时都要清空
	BYTE				m_cbOutCardData;						//出牌扑克--每次出牌时都要清空

	//状态变量
	bool				m_bSendStatus;							//发牌状态---?????????

	//用户状态
	BYTE				m_cbUserAction[GAME_PLAYER];			//用户动作--保存各用户的操作提示信息

	BYTE				m_cbOperateCard[GAME_PLAYER];			//操作扑克--保存用户真正执行操作所对应的牌
	BYTE				m_cbPerformAction[GAME_PLAYER];			//执行动作--保存各用户真正执行的操作

	bool				m_bResponse[GAME_PLAYER];				//响应标志--保存各用户是否响应过此操作

	//方便第一轮的藏牌处理过程而设置的变量
	BYTE              m_cbFirstUserAction[GAME_PLAYER+1];     //按顺序保存的四个用户能进行的操作：胡、藏、藏、藏
	WORD              m_cbFirstUser[GAME_PLAYER+1];          //按顺序保存的四个用户：庄家，上家、上上家、庄家
	BYTE              m_cbFirstCurrentIndex;                //当前操作的用户及其操作索引号(0-3)
	BYTE              m_cbFirstCurrentAction;               //当前用户所对应的操作
	WORD              m_cbFirstCurrentUser;

	//4、组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						*m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数

	//5、属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数
	static const enStartMode		m_GameStartMode;						//开始模式

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual void __cdecl Release() { if (IsValid()) delete this; }
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
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//用户事件
public:
	//用户断线
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool __cdecl OnActionUserReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }

	//游戏事件
protected:
	//用户出牌
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//用户操作
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);

	//辅助函数
protected:

	//////////babala 11_15wan

	//分析玩家是否可以藏牌
	BYTE EstimateTongCard(WORD wChairID);

	//发送操作
	bool SendOperateNotify();
	
	//派发扑克
	bool DispatchCardData(WORD wCurrentUser);
	//响应判断
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard);
};

//////////////////////////////////////////////////////////////////////////

#endif