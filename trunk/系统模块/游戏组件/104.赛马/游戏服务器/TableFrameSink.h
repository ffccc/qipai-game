#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////



//马的数量
#define HORSE_COUNT					8									//马的数目
//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//总下注数
protected:
	LONGLONG							m_lScore2;							//买平总注
	LONGLONG							m_lScore3;							//买庄总注
	LONGLONG							m_lScore1;							//买闲总注
	LONGLONG							m_lScore6;							//平天王注
	LONGLONG							m_lScore5;							//庄天王注
	LONGLONG							m_lScore4;							//闲天王注
	LONGLONG							m_lScore7;							//闲家对子
	LONGLONG							m_lScore8;							//庄家对子

	//个人下注
protected:
	LONGLONG							m_lUserScore2[GAME_PLAYER];			//买平总注
	LONGLONG							m_lUserScore3[GAME_PLAYER];			//买庄总注
	LONGLONG							m_lUserScore1[GAME_PLAYER];			//买闲总注
	LONGLONG							m_lUserScore6[GAME_PLAYER];			//平天王注
	LONGLONG							m_lUserScore5[GAME_PLAYER];			//庄天王注
	LONGLONG							m_lUserScore4[GAME_PLAYER];			//闲天王注
	LONGLONG							m_lUserScore7[GAME_PLAYER];			//闲家对子
	LONGLONG							m_lUserScore8[GAME_PLAYER];			//庄家对子

	//控制变量
protected:
	LONGLONG							m_lAreaLimitScore;						//区域限制
	LONGLONG							m_lUserLimitScore;						//区域限制
	LONGLONG							m_lApplyBankerCondition;				//申请条件
	TCHAR								m_szConfigFileName[MAX_PATH];			//配置文件

	//玩家成绩
protected:
	LONGLONG							m_lUserWinScore[GAME_PLAYER];			//玩家成绩
	LONGLONG							m_lUserReturnScore[GAME_PLAYER];		//返回下注
	LONGLONG							m_lUserRevenue[GAME_PLAYER];			//玩家税收

	//扑克信息
protected:
	//BYTE							m_cbCardCount[2];						//扑克数目
 //   BYTE							m_cbTableCardArray[2][3];				//桌面扑克
	BYTE							m_cbHorseSpeed[HORSE_COUNT][HORSE_COUNT];

	//状态变量
protected:
	DWORD							m_dwJettonTime;							//开始时间

	//庄家信息
protected:
	CArrayTemplate<WORD>			m_ApplyUserArray;						//申请玩家
	WORD							m_wCurrentBanker;						//当前庄家
	WORD							m_wBankerTime;							//做庄次数
	LONGLONG						m_lBankerWinScore;						//累计成绩
	LONGLONG						m_lBankerCurGameScore;					//当前成绩
	bool							m_bEnableSysBanker;						//系统做庄

	//控制变量
protected:
	BYTE							m_cbWinSideControl;						//控制输赢
	int								m_nSendCardCount;						//发送次数

	//记录变量
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_S_SCORE_HISTORY];//游戏记录
	int								m_nRecordFirst;							//开始记录
	int								m_nRecordLast;							//最后记录
	CFile							m_FileRecord;							//记录结果
	DWORD							m_dwRecordCount;						//记录数目

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	ITableFrameControl				* m_pITableFrameControl;				//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数

	//属性变量
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

	//动作事件
public:
	//用户断线
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//用户重入
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return false; }
	//用户坐下
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//游戏事件
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
	//申请庄家
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//取消申请
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

	//辅助函数
private:

	//发送扑克
	bool DispatchSpeed();
	//发送扑克
	bool DispatchTableCard();
	//发送庄家
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//更换庄家
	bool ChangeBanker(bool bCancelCurrentBanker);
	//发送记录
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//发送消息
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);

	//下注计算
private:
	//最大下注
	LONGLONG GetUserMaxJetton(WORD wChairID);
	//最大下注
	LONGLONG GetMaxHorse1(WORD wChairID);
	//最大下注
	LONGLONG GetMaxHorse2(WORD wChairID);
	//最大下注
	LONGLONG GetMaxHorse3(WORD wChairID);
	//最大下注
	LONGLONG GetMaxHorse4(WORD wChairID);
	//最大下注
	LONGLONG GetMaxHorse5(WORD wChairID);
	//最大下注
	LONGLONG GetMaxHorse6(WORD wChairID);
	//最大下注
	LONGLONG GetMaxHorse7(WORD wChairID);
	//最大下注
	LONGLONG GetMaxHorse8(WORD wChairID);

	//游戏统计
private:
	//计算得分
    LONGLONG CalculateScore();
};

//////////////////////////////////////////////////////////////////////////

#endif
