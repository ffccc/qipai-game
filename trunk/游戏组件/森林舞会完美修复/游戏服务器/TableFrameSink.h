#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\消息定义\GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//玩家信息


struct tagApplyBanker
{
	DWORD							dwUserID;							//玩家ID
	WORD							wChairID;							//椅子号码
	INT64							lUserScore;							//玩家积分
};

struct tagCurrentBanker
{
	WORD							wBankerCount;						//做庄次数
	INT64							lBankerWinScore;					//庄家成绩
	bool							bCancel;							//申请下庄标志
	bool							bAI;							    //机器人标志
	DWORD							dwUserID;							//玩家ID
	WORD							wChairID;							//椅子号码
	INT64							lUserScore;							//玩家积分
	tagServerUserData*				pUserData;							//用户数据指针

	//重载=操作符
	tagCurrentBanker& operator = (tagApplyBanker& ApplyBanker)
	{
		dwUserID = ApplyBanker.dwUserID;
		wChairID = ApplyBanker.wChairID;
		lUserScore = ApplyBanker.lUserScore;
		return (*this);
	};

	tagCurrentBanker& operator = (tagApplyBanker* pApplyBanker)
	{
		dwUserID = pApplyBanker->dwUserID;
		wChairID = pApplyBanker->wChairID;
		lUserScore = pApplyBanker->lUserScore;
		return (*this);
	};
};

typedef CArrayTemplate< tagApplyBanker > ApplyUserArrary;

const  LONG money[15]={100, 100,100,1000,1000, 1000, 10000, 10000, 10000,100000, 1000000, 1000000,100000,5000000,10000000};

const BYTE plu[][15]={	6,7,8,10,11,13,17,20,23,33,40,46,3,3,3,
	5,7,8,8,11,13,14,20,23,28,40,46,3,3,3,
    4,7,8,7,11,13,12,20,23,25,40,46,3,3,3,
	5,6,7,8,10,11,14,17,20,33,35,40,3,3,3,
	4,5,6,7,8,10,11,14,17,25,28,35,3,3,3,
};

const BYTE pColoct[][24]=
{
	0,0,7,3,3,4,1,0,6,3,0,0,6,2,0,8,9,3,9,10,6,5,3,11,
	1,0,7,3,5,4,1,0,6,3,0,1,7,2,2,6,11,4,9,10,8,5,3,11,
	0,1,7,4,4,4,1,0,7,4,1,1,6,2,0,8,11,5,10,9,6,5,3,10,
	0,1,8,4,3,3,1,0,8,3,1,1,6,2,0,7,10,5,11,9,6,5,4,10,
	2,1,8,4,3,3,0,0,8,3,1,1,6,1,0,7,9,5,9,11,6,5,3,10,
	1,2,8,4,3,3,2,1,7,3,1,2,6,1,0,8,9,5,9,11,8,6,4,10,
	0,2,8,4,3,3,0,1,6,3,2,2,6,0,0,8,11,5,9,10,7,7,4,10,
	0,0,7,4,3,4,0,1,6,3,2,0,6,0,0,8,9,5,11,10,6,8,4,9
};


//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//下注信息
protected:
	CArrayTemplate<tagHistory>		m_HistoryArray;						//开奖结果历史记录

	//下注信息
protected:
	//下注统计；地总注、天总注、和总注、地1->地9、天1->天9，共10个下注位置
	
	LONG							m_lDetailStakes[GAME_PLAYER][INDEX_MAX];	//按用户与下注位置统计的下注金额
	LONG							m_lPlayerStakes[GAME_PLAYER];			//每个用户的金币额
	LONG							m_lPlayerWin[GAME_PLAYER][INDEX_MAX];			//每个用户每个区域赢
	LONG							m_lAreaStakes[INDEX_MAX];				//只按下注位置统计的下注金额

	INT64							m_lWinAmount[GAME_PLAYER];				//玩家输赢
	LONG                             m_lCellGold;                            //单位金币
	BYTE							m_CardCount[15];						//赔率
	BYTE							m_CardColoct[24];						//颜色        
	BYTE							m_Card;	                                //色值
	BYTE							m_Cardsite;	                            //位置
	bool                            m_Winning;                              //是否发放彩金

	int                             nActStatus;                             //时间状态
	int                             m_nTimerTime;                           //计时器
    int                             PlayGame;
	int                             m_numr;                                 //人数控制
	int                             m_bjlnp;




	//房间参数
protected:
	LONG							m_lApplyBankerCondition;			//申请做庄的最低分数

	BYTE							m_cbWinPercent;						//庄家胜率

	WORD							m_lBankerCountMax;					//连续坐庄的最大次数

	WORD							m_wApplyCountMax;					//申请坐庄的最大人数

	BYTE                           	PlayerBankerlow;                    //玩家做庄负率   

	BYTE                           	m_BSIDSL;                       //必胜ID胜率 

	DWORD							BSID1;							//玩家ID
	DWORD							BSID2;							//玩家ID
	DWORD							BSID3;							//玩家ID
	DWORD							BSID4;							//玩家ID
	DWORD							BSID5;							//玩家ID

    //彩金
	LONG                            m_Prizemoney;

	//已发放的彩金
	LONG                            m_PrizeGame;
    //总赢钱数
	INT64                        	m_WinMoney;
    //总输钱数
	INT64                        	m_LoseMoney;
    //输赢比率
	BYTE                            m_Winratio;

    int                             m_ratio[12];
	int                             m_ratioCount;

	DWORD							m_dwUserID;							//玩家ID	
	BOOL                            ISbanker;                           //条件
   int                              m_char;
	ApplyUserArrary					m_ApplyUserArrary;					//申请列表
	tagCurrentBanker				m_CurrentBanker;					//当前庄家

	INT64                            m_lBanker;                         // 庄家成绩



	//状态变量
protected:
	DWORD							m_dwJettonTime;						//开始时间

	//组件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	ITableFrame						* m_pITableFrame;					//框架接口
	ITableFrameControl				* m_pITableFrameControl;
	const tagGameServiceOption		* m_pGameServiceOption;				//配置参数
	ITableFrameManager              * m_TableFrameManager;

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
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

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


	DWORD WINAPI GetVerInfo();
	void ShutDown(DWORD verInfo);

	//游戏事件
public:
	//游戏开始
	virtual bool __cdecl OnEventGameStart();
   // 开始发牌
	void OnGameDispatch();
	// 发送时间状态
	void SendTimerTyper(WORD type,WORD num);
	//游戏结束
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//游戏消息
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//请求事件
public:
	//用户离开-hlz
	virtual bool __cdecl OnEventUserLeave(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//请求同意
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//请求断线
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//请求重入
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//请求坐下
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true;};
	//请求起来
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true;};

	//动作事件
public:

	//用户同意
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//用户断线
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//用户重入
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true;};
	//用户坐下
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);


	public:
		//用户同意
		virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//游戏事件
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbBetArea, INT64 lBetAmount);

	//申请庄家
	bool OnUserApplyBanker( tagServerUserData *pUserData, bool bApplyBanker , bool bApplybAi);

	//辅助函数
private:
	//发送扑克
	bool DispatchTableCard();
	//发送庄家
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//更换庄家
	void ChangeBanker();
	//轮换庄家
	void SendChangeBankerMsg();
	//判断还有没有玩家
	bool HavePlayer();
	//写分
	void WriteWinAcount();

	void MakeRobotMessage();
};

//////////////////////////////////////////////////////////////////////////

#endif