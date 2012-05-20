#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

#define MAX_CARD					5
#define MAX_CARDGROUP				5
#define CONTROL_AREA				4
//////////////////////////////////////////////////////////////////////////
//历史记录
#define MAX_SCORE_HISTORY			16									//历史个数
//////////////////////////////////////////////////////////////////////////

//游戏桌子类
#ifndef  __BANKER___
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
#else
class CTableFrameSink : public ITableFrameSink, public ITableUserAction,public ITableUserActionEX
#endif
{
	//总下注数
protected:
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];		//全体总注
	
	//个人下注
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1][GAME_PLAYER];	//个人总注

	//控制变量
protected:
	bool							m_bRefreshCfg;							//每盘刷新
	TCHAR							m_szRoomName[32];						//配置房间
	TCHAR							m_szConfigFileName[MAX_PATH];			//配置文件
	LONGLONG						m_lAreaLimitScore;						//区域限制
	LONGLONG						m_lUserLimitScore;						//区域限制

	//坐庄控制
	LONGLONG						m_lApplyBankerCondition;				//申请条件
	int								m_nBankerTimeLimit;						//次数限制
	int								m_nBankerTimeAdd;						//增加次数 (金币大于其他玩家时)
	LONGLONG						m_lExtraBankerScore;					//额外条件 (大于此值时可以无视条件额外坐庄)
	int								m_nExtraBankerTime;						//额外次数

	//库存控制
	LONGLONG						m_lStorageStart;						//库存数值
	int								m_nStorageDeduct;						//扣取比例

	//时间控制
	int								m_nFreeTime;							//空闲时间
	int								m_nPlaceJettonTime;						//下注时间
	int								m_nGameEndTime;							//结束时间

	//机器人控制
	int								m_nMaxChipRobot;						//最大数目 (下注机器人)
	int								m_nChipRobotCount;						//人数统计 (下注机器人)
	LONGLONG						m_lRobotAreaLimit;						//区域统计 (机器人)
	LONGLONG						m_lRobotAreaScore[AREA_COUNT+1];		//区域统计 (机器人)

	//玩家成绩
protected:
	LONGLONG						m_lUserWinScore[GAME_PLAYER];			//玩家成绩
	LONGLONG						m_lUserReturnScore[GAME_PLAYER];		//返回下注
	LONGLONG						m_lUserRevenue[GAME_PLAYER];			//玩家税收
	BYTE							m_cbLeftCardCount;						//扑克数目
	bool							m_bContiueCard;							//继续发牌
	BYTE							m_bcFirstPostCard;						//最新發牌點數

	//扑克信息
protected:
	BYTE							m_cbCardCount[4];						//扑克数目
    BYTE							m_cbTableCardArray[5][5];				//桌面扑克
	BYTE							m_cbTableCard[CARD_COUNT];				//桌面扑克

	//状态变量
protected:
	DWORD							m_dwJettonTime;							//开始时间
	bool							m_bExchangeBanker;						//变换庄家
	WORD							m_wAddTime;								//额外坐庄 
	
	//玩家控制						
protected:
	BYTE							m_cbExcuteTimes;								//执行次数
	BYTE							m_cbControlStyle;								//控制方式
	bool							m_bWinArea[CONTROL_AREA];						//赢家区域
	BYTE							m_cbWinAreaCount;
	int								m_nCompareCard[MAX_CARDGROUP][MAX_CARDGROUP];	//对比矩阵

	//庄家信息
protected:
	CArrayTemplate<WORD>			m_ApplyUserArray;						//申请玩家
	WORD							m_wCurrentBanker;						//当前庄家
	WORD							m_wBankerTime;							//做庄次数
	LONGLONG						m_lBankerScore;							//
	LONGLONG						m_lBankerWinScore;						//累计成绩
	LONGLONG						m_lBankerCurGameScore;					//当前成绩
	bool							m_bEnableSysBanker;						//系统做庄

	//控制变量
protected:
	BYTE							m_cbWinSideControl;						//控制输赢
	int								m_nSendCardCount;						//发送次数

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

#ifdef __BANKER___
	//银行操作
	virtual bool __cdecl OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem);
#endif

	//游戏事件
protected:
	//加注事件
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
	//申请庄家
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//取消申请
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
	//管理员命令
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);

	//辅助函数
private:
	//发送扑克
	bool DispatchTableCard();
	//发送庄家
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//更换庄家
	bool ChangeBanker(bool bCancelCurrentBanker);
	//轮换判断
	void TakeTurns();
	//发送记录
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//发送消息
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//读取配置
	void ReadConfigInformation(bool bReadFresh);

	//输赢控制
protected:
	//查找结果
	void GetSuitResult();
	//区域牌型
	bool AreaWinCard(BYTE cbStack[]);
	//计算分配
	bool GetSuitCardCombine(BYTE cbStack[]);
	//庄家输赢
	LONGLONG GetBankerWinScore(int nWinMultiple[]);
	//庄家赢牌
	void BankerWinCard(bool bIsWin,BYTE cbStack[]);
	//输赢处理
protected:
	//输赢处理	
	bool GameCheat();

	//下注计算
private:
	//最大下注
	LONGLONG GetUserMaxJetton(WORD wChairID);

	//游戏统计
private:
	//计算得分
    LONGLONG CalculateScore();
	//推断赢家
    void DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple );
		void AddMoney();
};

//////////////////////////////////////////////////////////////////////////

#endif
