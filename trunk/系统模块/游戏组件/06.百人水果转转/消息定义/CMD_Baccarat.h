#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						32								//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("水果转转")					//游戏名字
#define CLIENT_MODULE_NAME			TEXT("SGDZ_CLIENT.exe")				//客户端模块名称
#define	SERVER_MODULE_NAME			TEXT("SGDZ_SERVER.DLL")				//服务端模块名称

//状态定义
#define	GS_PLACE_JETTON				GS_PLAYING							//下注状态
#define	GS_GAME_END					GS_PLAYING+1						//结束状态

//玩家索引
#define ID_BIG_TIGER			0									//大虎索引
#define ID_SMALL_TIGER			1									//小虎索引
#define ID_BIG_DOG				2									//大狗索引
#define ID_SMALL_DOG			3									//小狗索引
#define ID_BIG_HORSE			4									//大马索引
#define ID_SMALL_HORSE			5									//小马索引
#define ID_BIG_SNAKE			6									//大蛇索引
#define ID_SMALL_SNAKE			7									//小蛇索引


#define	PATH_STEP_NUMBER		32									//转轴格子数
#define JETTON_AREA_COUNT		8									//下注区域数量

//记录信息
struct tagServerGameRecord
{
	__int64 	cbGameTimes;										//第几局
	BYTE		cbRecord;											//范围在ID_BIG_TIGER到ID_SMALL_SNAKE
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				99									//游戏空闲
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_PLACE_JETTON			101									//用户下注
#define SUB_S_GAME_END				102									//游戏结束
#define SUB_S_APPLY_BANKER			103									//申请庄家
#define SUB_S_CHANGE_BANKER			104									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		105									//更新积分
#define SUB_S_SEND_RECORD			106									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		107									//下注失败
#define SUB_S_CANCEL_BANKER			108									//取消申请


//失败结构
struct CMD_S_PlaceJettonFail
{
	BYTE							lJettonArea;						//下注区域
	__int64							iPlaceScore;						//当前下注
};

//更新积分
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//椅子号码
	__int64							iScore;								//玩家积分

	//庄家信息
	WORD							wCurrentBankerChairID;				//当前庄家
	BYTE							cbBankerTime;						//庄家局数
	__int64							iCurrentBankerScore;				//庄家分数
};

//申请庄家
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//申请玩家
};

//取消申请
struct CMD_S_CancelBanker
{
	TCHAR							szCancelUser[32];					//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	__int64							iBankerScore;						//庄家金币
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	__int64							iUserMaxScore;						//玩家金币

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	__int64							iBankerWinScore;					//庄家成绩
	__int64							iBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	__int64							iApplyBankerCondition;				//申请条件
	__int64							iAreaLimitScore[JETTON_AREA_COUNT];	//区域限制
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	__int64								iTotalAreaScore[JETTON_AREA_COUNT];

	//玩家下注
	__int64								iUserAreaScore[JETTON_AREA_COUNT];

	//各区域限注
	__int64								iAreaScoreLimit[JETTON_AREA_COUNT];		

	//玩家积分
	__int64							iUserMaxScore;					//最大下注							

	//控制信息
	__int64							iApplyBankerCondition;			//申请条件

	//扑克信息
 	BYTE							cbCardCount[2];					//扑克数目
	BYTE							cbTableCardArray[2][3];			//桌面扑克

	//庄家信息
	WORD							wBankerUser;					//当前庄家
	WORD							cbBankerTime;					//庄家局数
	__int64							iBankerWinScore;				//庄家赢分
	__int64							iBankerScore;					//庄家分数
	bool							bEnableSysBanker;				//系统做庄

	//结束信息
	__int64							iEndBankerScore;				//庄家成绩
	__int64							iEndUserScore;					//玩家成绩
	__int64							iEndUserReturnScore;			//返回积分
	__int64							iEndRevenue;					//游戏税收

	//全局信息
	BYTE							cbTimeLeave;					//剩余时间
	BYTE							cbGameStatus;					//游戏状态
};

//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
	BYTE							cbGameRecord;						//本次开出的结果
	__int64							iGameTimes;						//当前是游戏启动以来的第几局
};

//游戏开始
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//庄家位置
	__int64							iBankerScore;						//庄家金币
	__int64							iUserMaxScore;						//我的金币
	BYTE							cbTimeLeave;						//剩余时间

	__int64							iAreaLimitScore[JETTON_AREA_COUNT];		//各区域可下分
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	__int64							iJettonScore;						//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE							cbGoalAnimal;						//本次停止的位置
 
	//庄家信息
	__int64							iBankerScore;						//庄家成绩
	__int64							iBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	__int64							iUserScore;							//玩家成绩
	__int64							iUserReturnScore;					//返回积分

	//全局信息
	__int64							iRevenue;							//游戏税收
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请

//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	__int64							iJettonScore;						//加注数目
};

//操作结果
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//记录信息
struct tagClientGameRecord
{
	enOperateResult					enOperateFlags;						//操作标识
	BYTE							cbPlayerCount;						//闲家点数
	BYTE							cbBankerCount;						//庄家点数
	BYTE							cbKingWinner;						//天王赢家
	bool							bPlayerTwoPair;						//对子标识
	bool							bBankerTwoPair;						//对子标识
};

//历史记录
#define MAX_SCORE_HISTORY			1000								//历史个数
#define MAX_SHOW_HISTORY			8									//一次最多显示的历史记录个数

//同时显示申请上庄玩家数量
#define MAX_SCORE_BANKER			4
//////////////////////////////////////////////////////////////////////////

#endif
