#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						1									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("赛马")						//游戏名字

//状态定义
#define	GS_PLACE_JETTON				GS_PLAYING							//下注状态
#define	GS_GAME_START				GS_PLAYING+1						//比赛状态
#define	GS_GAME_END					GS_PLAYING+2						//结束状态

//玩家索引
#define ID_HORSE1					1									//1号索引
#define ID_HORSE2					2									//2号索引
#define ID_HORSE3					3									//3号索引
#define ID_HORSE4					4									//4号索引
#define ID_HORSE5					5									//5号索引
#define ID_HORSE6					6									//6号索引
#define ID_HORSE7					7									//7号索引
#define ID_HORSE8					8									//8号索引

//赔率定义
#define RATE_TWO_PAIR				8									//赔率

//操作结果
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//服务器历史记录
#define MAX_S_SCORE_HISTORY			16									//历史个数

//记录信息
struct tagServerGameRecord
{
	BYTE							cbWinner;							//赢家
};

//客户端历史记录
#define MAX_C_SCORE_HISTORY			65									//历史个数
//记录信息
struct tagClientGameRecord
{
	enOperateResult					enOperateFlags;						//操作标识
	BYTE							cbWinner;						//闲家点数
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
#define SUB_S_UPDATE_USER_SCORE		109									//更新玩家金币
#define SUB_S_UPDATE_BANKER_SCORE	110									//更新庄家金币


//失败结构
struct CMD_S_PlaceJettonFail
{
	BYTE							lJettonArea;						//下注区域
	LONGLONG							lPlaceScore;						//当前下注
};

//更新积分
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//椅子号码
	LONGLONG							lScore;								//玩家积分

	//庄家信息
	WORD							wCurrentBankerChairID;				//当前庄家
	BYTE							cbBankerTime;						//庄家局数
	LONGLONG							lCurrentBankerScore;				//庄家分数
};

//申请庄家
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//申请玩家
};

//取消申请
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;						//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	LONGLONG							lBankerScore;						//庄家金币
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	LONGLONG							lUserMaxScore;						//玩家金币

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONGLONG							lBankerWinScore;					//庄家成绩
	LONGLONG							lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	LONGLONG							lApplyBankerCondition;				//申请条件
	LONGLONG							lAreaLimitScore;					//区域限制
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注

	LONGLONG							lAllScore1;						//1号总注
	LONGLONG							lAllScore2;						//2号总注
	LONGLONG							lAllScore3;						//3号总注
	LONGLONG							lAllScore4;						//4号总注
	LONGLONG							lAllScore5;						//5号总注
	LONGLONG							lAllScore6;						//6号总注
	LONGLONG							lAllScore7;						//7号总注
	LONGLONG							lAllScore8;						//8号总注

	//玩家下注
	LONGLONG							lUserScore1;						//1号总注
	LONGLONG							lUserScore2;						//2号总注
	LONGLONG							lUserScore3;						//3号总注
	LONGLONG							lUserScore4;						//4号总注
	LONGLONG							lUserScore5;						//5号总注
	LONGLONG							lUserScore6;						//6号总注
	LONGLONG							lUserScore7;						//7号总注
	LONGLONG							lUserScore8;						//8号总注

	//玩家积分
	LONGLONG							lUserMaxScore;						//最大下注							

	//控制信息
	LONGLONG							lApplyBankerCondition;				//申请条件
	LONGLONG							lAreaLimitScore;					//区域限制

	////扑克信息
 //	BYTE							cbCardCount[2];						//扑克数目
	//BYTE							cbTableCardArray[2][3];				//桌面扑克
	//赛马速度
	BYTE							cbHorseSpeed[8][8];

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONGLONG							lBankerWinScore;					//庄家赢分
	LONGLONG							lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//结束信息
	LONGLONG							lEndBankerScore;					//庄家成绩
	LONGLONG							lEndUserScore;						//玩家成绩
	LONGLONG							lEndUserReturnScore;				//返回积分
	LONGLONG							lEndRevenue;						//游戏税收

	//全局信息
	BYTE							cbTimeLeave;						//剩余时间
	BYTE							cbGameStatus;						//游戏状态
};

//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
};

//游戏开始
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//庄家位置
	LONGLONG						lBankerScore;						//庄家金币
	LONGLONG						lUserMaxScore;						//我的金币
	BYTE							cbTimeLeave;						//剩余时间
	LONGLONG						lAreaLimitScore;					//区域限制
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	////扑克信息
	//BYTE							cbCardCount[2];						//扑克数目
	//BYTE							cbTableCardArray[2][3];				//桌面扑克

	//赛马速度
	BYTE							cbHorseSpeed[8][8];
 //
	//庄家信息
	LONGLONG							lBankerScore;						//庄家成绩
	LONGLONG							lBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	LONGLONG							lUserScore;							//玩家成绩
	LONGLONG							lUserReturnScore;					//返回积分

	//全局信息
	LONGLONG							lRevenue;							//游戏税收
};

//更新成绩
struct CMD_S_UpdateUserScore
{
	LONGLONG							lUserScore;							//用户成绩
};

//更新成绩
struct CMD_S_UpdateBankerScore
{
	LONGLONG							lBankerScore;							//用户成绩
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_OPEN_BANK				4									//点击银行
//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
};

//////////////////////////////////////////////////////////////////////////

#endif
