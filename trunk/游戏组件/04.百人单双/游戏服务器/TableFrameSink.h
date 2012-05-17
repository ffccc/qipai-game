#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//历史记录
#define MAX_SCORE_HISTORY			16									//历史个数


//////////////////////////////////////////////////////////////////////////
//玩家信息
struct tagApplyUserInfo
{
	DWORD							dwUserID;							//玩家ID
	WORD							wChairID;							//椅子号码
	RETYPELONG						lUserScore;							//玩家积分
};

typedef CArrayTemplate< tagApplyUserInfo > ApplyUserArrary;

//游戏桌子类
#ifndef __SPECIAL___
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
#else
class CTableFrameSink : public ITableFrameSink, public ITableUserAction,public ITableUserActionEX
#endif
{
	//下注信息
protected:
	RETYPELONG							m_lOddScore;						//买单总注
	RETYPELONG							m_lEvenScore;						//买双总注
	RETYPELONG							m_lAreaLimitScore;					//区域限制
	RETYPELONG							m_lUserLimitScore;

	//下注信息
protected:
	RETYPELONG							m_lUserOddScore[GAME_PLAYER];		//买单总注
	RETYPELONG							m_lUserEvenScore[GAME_PLAYER];		//买双总注
	RETYPELONG							m_UserLeftScore;

	//玩家成绩
protected:
	RETYPELONG							m_lUserWinScore[GAME_PLAYER];		//玩家成绩
	RETYPELONG							m_lUserReturnScore[GAME_PLAYER];	//返回下注
	RETYPELONG							m_lUserRevenue[GAME_PLAYER];		//玩家税收

	//扑克信息
protected:
    BYTE								m_cbDice[2];						//桌面色子
	WORD								m_nSendCardCount;

	//状态变量
protected:
	DWORD								m_dwJettonTime;						//开始时间

	//庄家信息
protected:
	RETYPELONG							m_lApplyBankerCondition;			//申请条件
	ApplyUserArrary						m_ApplyUserArrary;					//申请列表
	tagApplyUserInfo					m_CurrentBanker;					//当前庄家
	BYTE								m_cbBankerTimer;					//做庄次数
	RETYPELONG							m_lBankerWinScore;					//庄家成绩
	TCHAR							    m_szConfigFileName[MAX_PATH];			//配置文件

	//记录变量
protected:
	tagServerGameRecord					m_GameRecordArrary[MAX_SCORE_HISTORY];//游戏记录
	int									m_nRecordFirst;						//开始记录
	int									m_nRecordLast;						//最后记录

	//组件变量
protected:
	ITableFrame							* m_pITableFrame;					//框架接口
	ITableFrameControl					* m_pITableFrameControl;			//框架接口
	const tagGameServiceOption			* m_pGameServiceOption;				//配置参数

	//属性变量
protected:
	static const WORD					m_wPlayerCount;						//游戏人数
	static const enStartMode			m_GameStartMode;					//开始模式
private:
	BOOL							m_IsHightRoom;
	LONGLONG						m_LowerBigBankerCondition;	
		

private:
	struct TAndroidUser 
	{
		bool blAndroidUser;
		int  PlayCount;
		int  BankCount;
		TAndroidUser()
		{
			memset(this,0,sizeof(*this));
		};
	};
	TAndroidUser						m_AndroidUser[GAME_PLAYER];
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

	//请求事件
public:
	//请求同意
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//请求断线
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//请求重入
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//请求坐下
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
	//请求起来
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }

	//动作事件
public:
	//用户断线
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//用户重入
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//用户坐下
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	#ifdef __SPECIAL___
	//银行操作
	virtual bool __cdecl OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem);////

	bool __cdecl IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	#endif
	//游戏事件
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, RETYPELONG lJettonScore);
	//申请庄家
	bool OnUserApplyBanker( tagServerUserData *pUserData, bool bApplyBanker, bool bUserAction=true);
private:
	//获得能够坐庄次数
	WORD GetBankerTime();
	//辅助函数
private:
	//发送扑克
	bool RandDice();
	//盈利判断
	LONGLONG AccountPayoffScore();
	//发送庄家
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//更换庄家
	void ChangeBanker();
	//轮换庄家
	void SendChangeBankerMsg();
	//最大下注
	RETYPELONG GetMaxOddScore(WORD wChairID);	
	//最大下注
	RETYPELONG GetMaxEvenScore(WORD wChairID);
	//计算得分
    void CalculateScore();
	//
	LONGLONG CalculateScoreBanker();
	//推断赢家
	void DeduceWinner(BYTE &cbWinner);

private:
	//处理机器人上庄
	void MakeAndroidUserBanker();
	//随机数
	void RandList(BYTE cbCardBuffer[], BYTE cbBufferCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
