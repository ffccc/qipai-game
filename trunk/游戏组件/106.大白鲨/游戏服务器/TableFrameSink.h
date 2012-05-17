#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "RandomGen.h"
//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableFrameEvent
{
protected:
	CRITICAL_SECTION				m_csPlaceJetton;
	//庄家信息
protected:
	CArrayTemplate<WORD>			m_ApplyUserArray;						//申请玩家
	WORD							m_wCurrentBanker;						//当前庄家
	WORD							m_wBankerTime;							//做庄次数
	LONG							m_lBankerWinScore;						//累计成绩
	LONG							m_lBankerCurGameScore;					//当前成绩
	bool							m_bEnableSysBanker;						//系统做庄
	bool							m_bChangeBanker;						//系统坐庄更换
	BYTE							m_bMaxBankerTime;						//连庄次数
	//控制变量
protected:
	LONG							m_lAreaLimitScore;						//区域限制
	LONG							m_lUserLimitScore;						//区域限制
	LONG							m_lApplyBankerCondition;				//申请条件
	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件
	BYTE							m_cbWinSideControl;						//控制输赢
	BYTE							m_cbMoveEndIndex;						//移动减速索引
	BYTE							m_cbResultIndex;						//结果索引
	BYTE							m_cbLastResultIndex;					//上把索引
	BYTE							m_cbEndIndex;							//停止索引
	//全体下注
protected:
	LONG							m_lALLBigSharkScore;						//自己买燕子总数
	LONG							m_lALLFeiQinScore;						//自己买燕子总数
	LONG							m_lALLZouShouScore;						//自己买燕子总数
	LONG							m_lALLYanZiScore;						//自己买燕子总数
	LONG							m_lALLTuZiScore;						//兔子
	LONG							m_lALLGeZiScore;						//鸽子
	LONG							m_lALLXiongMaoScore;					//熊猫
	LONG							m_lALLKongQueScore;						//孔雀
	LONG							m_lALLHouZiScore;						//猴子
	LONG							m_lALLLaoYingScore;						//老鹰
	LONG							m_lALLShiZiScore;						//狮子
	//个人下注
protected:
	LONG							m_lUserBigSharkScore[GAME_PLAYER];		//大白鲨
	LONG							m_lUserFeiQinScore[GAME_PLAYER];		//飞禽
	LONG							m_lUserZouShouScore[GAME_PLAYER];		//走兽
	LONG							m_lUserYanZiScore[GAME_PLAYER];			//自己买燕子总数
	LONG							m_lUserTuZiScore[GAME_PLAYER];			//兔子
	LONG							m_lUserGeZiScore[GAME_PLAYER];			//鸽子
	LONG							m_lUserXiongMaoScore[GAME_PLAYER];		//熊猫
	LONG							m_lUserKongQueScore[GAME_PLAYER];		//孔雀
	LONG							m_lUserHouZiScore[GAME_PLAYER];			//猴子
	LONG							m_lUserLaoYingScore[GAME_PLAYER];		//老鹰
	LONG							m_lUserShiZiScore[GAME_PLAYER];			//狮子
	//玩家成绩
protected:
	LONG							m_lUserWinScore[GAME_PLAYER];			//玩家成绩
	LONG							m_lUserReturnScore[GAME_PLAYER];		//返回下注
	LONG							m_lUserRevenue[GAME_PLAYER];			//玩家税收
	//状态变量
protected:
	DWORD							m_dwJettonTime;							//开始时间
	//历史记录
protected:
	CArrayTemplate<BYTE>			m_cbScoreHistory;						//历史记录
	//作弊信息
protected:
	BYTE							m_cbZuoBiIndex;							//作弊索引
	BYTE							m_cbZuoBiCount;							//作弊连续次数(同一索引)
	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数
	ITableFrameControl				* m_pITableFrameControl;				//框架接口
	CRandomGen						m_RandGen;						
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
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
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
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore);
	//申请庄家
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//取消申请
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
	//发送庄家
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//游戏开始
	virtual bool __cdecl OnEventGameStart();
	//游戏结束
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStation, bool bSendSecret);
	//辅助函数
public:
	//更换庄家
	bool ChangeBanker(bool bCancelCurrentBanker);
	//系统自动下上坐庄
	void SysChangeBanker(bool bCancelCurrentBanker);
	//索引产生
	void RandGameIndex();
	//发送消息
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//计算得分
	LONG CalculateScore();
	//发送记录
	void SendGameRecord();
	//最大下注
	LONG GetMaxScore(WORD wChairID, LONG lJettonScore,BYTE cbArea);
	//最大下注
	LONG GetUserMaxJetton(WORD wChairID);
	//输出信息
	CString DrawScoreString(LONG lNumber);

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
	virtual bool __cdecl OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//用户重入
	virtual bool __cdecl OnEventUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//用户坐下
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool __cdecl OnEventUserReady(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
};

//////////////////////////////////////////////////////////////////////////

#endif