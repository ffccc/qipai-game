#ifndef CMD_THREEKING_HEAD_FILE
#define CMD_THREEKING_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							9									//游戏 I D
#define GAME_PLAYER						6									//游戏人数
#define GAME_NAME						TEXT("三公游戏")					//游戏名字

//游戏状态
#define GS_TK_FREE						GS_FREE								//空闲状态
#define GS_TK_PLAYING					GS_PLAYING							//开始状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_TIMER_INFO				100									//时间信息
#define SUB_S_SCORE_INFO				101									//下注信息
#define SUB_S_BANKER_USER				102									//庄家用户
#define SUB_S_CHANGE_INFO				103									//抢庄信息

#define SUB_S_GAME_START				200									//游戏开始
#define SUB_S_USER_CHIPIN				201									//用户下注
#define SUB_S_SEND_CARD					202									//发牌消息
#define SUB_S_GAME_END					203									//游戏结束

//游戏状态
struct CMD_S_StatusFree
{
	LONG								lMaxScore;							//最大下注
	LONG								lLessScore;							//最少下注
	WORD								wLeaveTime;							//剩余时间
	WORD								wBankerUser;						//庄家用户

	BOOL								bLongBanker;						//长庄标志
};

//游戏状态
struct CMD_S_StatusPlay
{
	//下注信息
	LONG								lMaxScore;							//最大下注
	LONG								lLessScore;							//最少下注
	LONG								lUserChipIn[GAME_PLAYER];			//下注信息

	//游戏信息
	WORD								wBankerUser;						//庄家用户
	BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态

	//扑克信息
	BYTE								cbSendCard;							//发牌标志
	BYTE								cbNeedFinish;						//发送完成
	BYTE								cbUserCardData[GAME_PLAYER][3];		//用户扑克

	BOOL								bLongBanker;						//长庄标志
};

//时间信息
struct CMD_S_TimerInfo
{
	WORD								wLeaveTime;							//剩余时间
};

//下注信息
struct CMD_S_ScoreInfo
{
	LONG								lMaxScore;							//最大下注
	LONG								lLessScore;							//最少下注
};

//庄家信息
struct CMD_S_BankerUser
{
	BOOL								bLongBanker;						//长庄标志
	LONG								lMaxScore;							//最大下注
	LONG								lLessScore;							//最少下注
	WORD								wBankerUser;						//庄家用户
};

//游戏开始
struct CMD_S_GameStart
{
	LONG								lMaxScore;							//最大下注
	LONG								lLessScore;							//最少下注
	BYTE								cbPlayStatus[GAME_PLAYER];			//用户状态
};

//下注消息
struct CMD_S_UserChipin
{
	WORD								wChairID;							//用户位置
	LONG								lUserChipin;						//加注数目
};

//游戏发牌
struct CMD_S_SendCard
{
	BYTE								cbUserCardData[GAME_PLAYER][3];		//用户扑克
};

//游戏结束
struct CMD_S_GameEnd
{
	LONG								lGameTax[GAME_PLAYER];				//游戏税收
	LONG								lGameScore[GAME_PLAYER];			//游戏得分
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_USER_CHIPIN				1									//用户下注
#define SUB_C_SEND_FINISH				2									//发牌完成
#define SUB_C_CHANGE_INFO				3									//抢庄消息
#define SUB_C_CONTINUE_GAME				4									//继续游戏
#define SUB_C_CONTROL_MAX				5

//抢庄消息
struct CMD_C_ChangeInfo
{
	BYTE								cbChange;							//抢庄标志
};

//下注消息
struct CMD_C_UserChipin
{
	LONG								lScore;								//加注数目
};

//////////////////////////////////////////////////////////////////////////

#endif
