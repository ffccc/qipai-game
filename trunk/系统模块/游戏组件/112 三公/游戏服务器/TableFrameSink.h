#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//游戏变量
protected:
	LONG							m_lExitScore;							//退出注数
	bool							m_bOffLineGameEnd;						//断线结束
	WORD							m_wBankerUser;							//庄家用户
	LONG							m_lGameScore[GAME_PLAYER];				//积分信息
	LONG							m_lUserChipIn[GAME_PLAYER];				//下注信息
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//游戏状态
	BYTE							m_cbUserCardData[GAME_PLAYER][3];		//用户扑克

	WORD							m_wMaxChairID;

	//状态变量
protected:
	BYTE							m_cbSendCard;							//发牌标志
	BYTE							m_cbWaitStart;							//等待开始
	BYTE							m_cbWaitChange;							//等待换庄
	BYTE							m_cbWaitContinue;						//等待继续

	//状态变量
protected:
	BYTE							m_cbUserFinish[GAME_PLAYER];			//完成标志
	BYTE							m_cbUserBanker[GAME_PLAYER];			//换庄标志
	BYTE							m_cbUserContinue[GAME_PLAYER];			//继续标志

	//辅助变量
protected:
	LONG							m_lMaxScore;							//最大下注
	LONG							m_lLessScore;							//最少下注
	WORD							m_wCurrentUser;							//当前用户
	DWORD							m_dwBankerStartTime;					//开始时间

	//AI变量
protected:
	LONG							m_dStockScore;							//总输赢分

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
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

	//请求事件
public:
	//请求坐下
	//virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon);

	//动作事件
public:
	//用户同意
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//用户断线
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//用户重入
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//用户坐下
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//功能函数
protected:
	//扑克分析
	void AnalyseCard();
	//设置庄家
	void SetBankerUser(WORD wBankerUser);
	//庄家检查
	bool IsSuitBankerUser(WORD wChairID);
	//寻找庄家
	bool SearceBankerUser();

	//辅助函数
protected:
	//用户数目
	WORD GetTableUserCount();
	//最大数目
	LONG GetUserMaxChipIn(WORD wChairID);
	//交换数据
	bool ChangeCardData();
};

//////////////////////////////////////////////////////////////////////////

#endif
