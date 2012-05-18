#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						185									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("森林舞会")						//游戏名字

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_PLACE_JETTON			101									//用户下注
#define SUB_S_GAME_END				102									//游戏结束
#define SUB_S_APPLY_BANKER			103									//申请庄家
#define SUB_S_CHANGE_BANKER			104									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		105									//更新积分
#define SUB_S_BET_TIME				106									//下注时间开始
#define SUB_S_PLACE_TIMERTYPE		108									//时间类型
#define TIME_PLACE_JETTON		    20									//下注时间
#define TIME_GAME_SENDPAI			30									//开牌到下一局开始的时间

#define SUB_S_SUPPERUSER			454									//超级口令
#define SUB_S_GAME			        110									//超级口令
#define SUB_S_PLU			        111									//超级口令
//更新积分
struct CMD_S_Game
{
	LONG							GameBi;							//椅子号码

};
//更新积分
struct CMD_S_SUPPERUSER
{
	bool							supuser;							//椅子号码

};
//更新积分
struct CMD_S_ChangeApplyScore
{
	WORD							wChairID;							//椅子号码
	INT64							lGold;								//玩家积分

	//庄家信息
	WORD							wCurrentBankerChairID;				//当前庄家
	WORD							wBankerCount;						//庄家局数
	INT64							lCurrentBankerScore;				//庄家分数
};

//申请庄家
struct CMD_S_ApplyBanker
{
	WORD							wChairID;							//椅子号码
	CHAR							szAccount[32];					    //申请玩家
	CHAR							szName[32];					        //申请玩家昵称
	DWORD							dwUserID;							//申请玩家
	INT64							lGold;								//玩家金币
	bool							bApplyBanker;						//申请标识
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wChairID;							//椅子号码
	WORD							wBankerCount;						//庄家局数
	LONG							lBankerWinScore;					//庄家分数
	LONG							lBankerMaxLoss;						//庄家最大赔付
	BYTE							cbTimeLeave;						//剩余时间

	LONG                            lBankerScore;

	//我的下注
	INT64							lMyMaxStake;						//最大下注
	INT64							lMaxStakes[INDEX_MAX];				//每个区域的最大下注额
};

//历史记录
struct tagHistory
{
	BYTE							cbPlayerValue;						//点数

	tagHistory()
	{
		cbPlayerValue = 0;


	};

	tagHistory(BYTE acbPlayerValue )
	{
		cbPlayerValue = acbPlayerValue;

	};

};
//时间状态
struct CMD_S_TimerTyper
{
	WORD							TimerType;		
    WORD                            Timernum;
};
//游戏状态
struct CMD_S_StatusFree
{
	LONG							lMyStakes[INDEX_MAX];				//自己的下注信息
	LONG							lAllStakes[INDEX_MAX];				//自己的下注信息
	LONG                            lMyGame;                            //自已的游戏币
	BYTE							cbCardCount[INDEX_MAX];						//赔率
	BYTE							cbCardColoct[24];						//颜色
	LONG                            cdPrizemoney;                       //彩金
	LONG                            lCellGold  ;                        //单位金币   
	WORD							wHistoryCount;
	tagHistory						HistoryInfo[HISTORY_COUNT];			//历史记录
	WORD							wCurrentBankerChairID;				//当前庄家
	WORD							wBankerCount;						//庄家局数
	INT64							lCurrentBankerScore;				//庄家分数
	INT64							lApplyBankerCondition;				//申请坐庄的最低分数
	WORD							wApplyCountMax;						//申请坐庄的最大人数
};

//游戏状态
struct CMD_S_StatusPlay
{
	//下注信息
	LONG                            cdPrizemoney;                       //彩金
	LONG							lMyStakes[INDEX_MAX];				//自己的下注信息
	LONG							lAllStakes[INDEX_MAX];				//自己的下注信息
	LONG                            lMyGame;                            //自已的游戏币
	BYTE							cbCardCount[INDEX_MAX];						//赔率
	BYTE							cbCardColoct[24];						//颜色
	WORD							wCurrentBankerChairID;				//当前庄家
	WORD							wBankerCount;						//庄家局数
	INT64							lCurrentBankerScore;				//庄家分数
	INT64							lApplyBankerCondition;				//申请坐庄的最低分数
	WORD							wApplyCountMax;						//申请坐庄的最大人数

	LONG                            lCellGold  ;                        //单位金币                           
	WORD							wHistoryCount;
	tagHistory						HistoryInfo[HISTORY_COUNT];			//历史记录
};

struct CMD_S_GamePLU
{
	BYTE							cbCardCount[INDEX_MAX];						//赔率
	BYTE							cbCardColoct[24];						//颜色
};

//游戏开始
struct CMD_S_GameStart
{
	BYTE							cbCard;						//扑克数目
	BYTE                            cbCardsite;
	BOOL                            cbWinning;                  //是否发放彩金;
	LONG                            cbPrize;                    //支付的彩金金
};

//游戏结束
struct CMD_S_GameEnd
{
	//成绩记录
	LONG							lMyGameScore[INDEX_MAX];						//我的成绩
	LONG                            lPrizemoney;

};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;						//用户位置
	BYTE							cbBetArea;						//筹码区域
	LONG							lBetAmount;						//加注数目
	LONG							lGame;
	LONG                            cdPrizemoney;//彩金
	LONG							cdAreaStakes[INDEX_MAX];			
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_PlAY_OVER			    88									//申请庄家
#define SUB_C_CHI                   1235           
#define SUB_C_PlAY_TRUE			    44									//是否真
#define SUB_C_FANG                  9                                   //购买游戏币                          
#define SUB_C_Settlement            10                                   //购买游戏币  

struct CMD_C_Settlement
{
	bool							Settlement;						//结算游戏币
};
struct CMD_C_FANG
{
	int							cbFang;						//购买游戏币
};
struct CMD_C_TrueArea
{
	BYTE							cbBetArea;						//区域
};

struct CMD_C_CHI
{
	BYTE							cbBetArea;						//筹码区域
};
//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbBetArea;						//筹码区域
	INT64							lBetAmount;						//加注数目
};

//申请庄家
struct CMD_C_ApplyBanker
{
	bool							bApplyBanker;						//申请标识
	bool                            bAI;
};

//////////////////////////////////////////////////////////////////////////

#endif