#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../消息定义/GameLogic.h"

typedef struct tagPersonInfo
{
	DWORD dwUserID;           // 玩家ID
	int iPercent;             // 胜率
}S_PERSONINFO;
typedef CArrayTemplate<S_PERSONINFO,S_PERSONINFO&> CPensonInfoArray;

//////////////////////////////////////////////////////////////////////////

//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//总下注数
protected:
	__int64							m_lAllScore[COUNT_AZIMUTH];				// 每个区域已下的总注
	__int64                         m_lAllAndroidWinScore;                  // 所有机器人所赢的总注
	//__int64                         m_lMaxAndroidLost;                      // 机器人最多输多少
	bool                            m_bIsAllAndroid;                        // 所有的人都是机器人
	int								m_iAILostLimitPercent;						//机器人总体胜率

	//个人下注
protected:
	__int64							m_lUserScore[COUNT_AZIMUTH][GAME_PLAYER];			//个玩家每个区域已下的总注
protected:
	__int64							m_lAreaLimitScore;						//区域限制
	__int64							m_lUserLimitScore;						//玩家限制
	__int64							m_lApplyBankerCondition;				//申请条件
	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件

	//玩家成绩
protected:
	__int64							m_lUserWinScore[GAME_PLAYER];			//玩家成绩
	__int64							m_lUserReturnScore[GAME_PLAYER];		//返回下注
	__int64							m_lUserRevenue[GAME_PLAYER];			//玩家税收

	//扑克信息
protected:
	E_SICBO_NUMBER					m_enCards[MAX_COUNT_SICBO];             // 骰子点数
	bool                            m_bWinner[COUNT_AZIMUTH];               // 输赢结果
	CPensonInfoArray                m_arPensonInfo;
	INT                             m_iUseWinner;                          // 控制某一区域赢

	//状态变量
protected:
	DWORD							m_dwJettonTime;							//开始时间
	int                             m_iMaxAndroidBanker;                    // 机器人上庄最多玩家
	int                             m_iSameCount;                           // 三个相同开出的间隔
	int                             m_iAndroidRandCount;                    // 控制机器人赢使用的次数过多


	// 庄家信息
protected:
	CArrayTemplate<WORD>			m_wApplyUserArray;						//申请玩家
	WORD							m_wCurrentBanker;						//当前庄家
	WORD							m_wBankerTime;							//做庄次数
	__int64							m_lBankerWinScore;						//累计成绩
	__int64							m_lBankerCurGameScore;					//当前成绩
	bool							m_bEnableSysBanker;						//系统做庄
	bool                            m_bChangeSicboType;                     // 是否已经改变摇骰子的方式
	E_SICBO_TYPE                     m_enSicboType;                         // 摇骰子的方式

	//控制变量
protected:
	BYTE							m_cbWinSideControl;					    // 控制输赢
	int                             m_iNTWinPercent;
	int								m_nSendCardCount;					    // 发送次数
	int                             m_iAndroidNTCount;

	//记录变量
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//游戏记录
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
	CCriticalSection				   m_CriticalSection;				//线程锁定
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
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//用户坐下
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//游戏事件
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 lJettonScore);
	//申请庄家
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//取消申请
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

	//辅助函数
private:
	//发送庄家
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//更换庄家
	bool ChangeBanker(bool bCancelCurrentBanker);
	//发送记录
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//发送消息
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);

	//游戏统计
private:
	//计算得分
    __int64 CalculateScore(void);
	//推断赢家
	void DeduceWinner(bool bWinner[]);

	// 控制输赢
	void CtrlWinner(void);
	// 控制系统坐庄输赢
	void CtrlNTWinner(void);

	// 控制机器人要赢
	void CtrlAndroidWinner(void);

	// 获取每个区域可以下注数
	__int64 GetMaxPlaceJetton(int iArea, WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif
