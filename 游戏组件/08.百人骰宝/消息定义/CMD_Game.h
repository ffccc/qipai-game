// CMD_Game.h  消息定义

#pragma once

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						214								// 游戏 I D
#define GAME_PLAYER					100									// 游戏人数
#define GAME_NAME					TEXT("骰宝")				        // 游戏名字

//状态定义
#define	GS_PLACE_JETTON				GS_PLAYING							//下注状态
#define	GS_GAME_END					GS_PLAYING+1						//结束状态

//玩家索引
#define COUNT_AZIMUTH               52                                  // 52个区域
#define MAX_MULTIPLE                8                                   // 最大倍数

#define USER_WINNER                 0x01                                // 赢
#define USER_LOSER                  0xFF                                // 输
#define MAX_ANDROID_BANKER          3                                   // 队列中机器人的个数

//////////////////////////////////////////////////////////////////////////
//历史记录
#define MAX_SCORE_HISTORY			30									//历史个数
#define SHOW_SCORE_HISTORY          16                                  // 显示记录数

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

#define MAX_COUNT_SICBO             0x03            // 一组3个骰子
#define MAX_SICBO_NUMBER            0x06            // 最大6点

// 骰子
enum E_SICBO_NUMBER
{
	enSicboNum_One=(int)1,          // 骰子点数1
	enSicboNum_Two,                 // 骰子点数2
	enSicboNum_Three,               // 骰子点数3
	enSicboNum_Four,                // 骰子点数4
	enSicboNum_Five,                // 骰子点数5
	enSicboNum_Six                  // 骰子点数6
};

// 牌型
enum E_CARD_TYPE
{
	enCardType_Illegal=(int)-1,        // 非法
	enCardType_Big,                    // 大
	enCardType_Small,                  // 小
	enCardType_Single,                 // 单
	enCardType_Double,                 // 双		

	enCardType_NumberFour=(int)4,      // 3个骰子点数和为4
	enCardType_NumberFive,             // 3个骰子点数和为5
	enCardType_NumberSix,              // 3个骰子点数和为6
	enCardType_NumberSeven,            // 3个骰子点数和为7
	enCardType_NumberEight,            // 3个骰子点数和为8
	enCardType_NumberNine,             // 3个骰子点数和为9
	enCardType_NumberTen,              // 3个骰子点数和为10
	enCardType_NumberEleven,           // 3个骰子点数和为11
	enCardType_NumberTwelve,           // 3个骰子点数和为12
	enCardType_NumberThirteen,         // 3个骰子点数和为13
	enCardType_NumberFourteen,         // 3个骰子点数和为14
	enCardType_NumberFifteen,          // 3个骰子点数和为15
	enCardType_NumberSixteen,          // 3个骰子点数和为16
	enCardType_NumberSeventeen,        // 3个骰子点数和为17

	enCardType_SicboOne,               // 骰点1
	enCardType_SicboTwo,               // 骰点2
	enCardType_SicboThree,             // 骰点3
	enCardType_SicboFour,              // 骰点4
	enCardType_SicboFive,              // 骰点5
	enCardType_SicboSix,               // 骰点6

	enCardType_SicboOneAndTwo,         // 骰点1,2
	enCardType_SicboOneAndThree,       // 骰点1,3
	enCardType_SicboOneAndFour,        // 骰点1,4
	enCardType_SicboOneAndFive,        // 骰点1,5
	enCardType_SicboOneAndSix,         // 骰点1,6
	enCardType_SicboTwoAndThree,       // 骰点2,3
	enCardType_SicboTwoAndFour,        // 骰点2,4
	enCardType_SicboTwoAndFive,        // 骰点2,5
	enCardType_SicboTwoAndSix,         // 骰点2,6
	enCardType_SicboThreeAndFour,      // 骰点3,4
	enCardType_SicboThreeAndFive,      // 骰点3,5
	enCardType_SicboThreeAndSix,       // 骰点3,6
	enCardType_SicboFourAndFive,       // 骰点4,5
	enCardType_SicboFourAndSix,        // 骰点4,6
	enCardType_SicboFiveAndSix,        // 骰点5,6

	enCardType_SicboDoubleOne,         // 骰点2个1
	enCardType_SicboDoubleTwo,         // 骰点2个2
	enCardType_SicboDoubleThree,       // 骰点2个3
	enCardType_SicboDoubleFour,        // 骰点2个4
	enCardType_SicboDoubleFive,        // 骰点2个5
	enCardType_SicboDoubleSix,         // 骰点2个6

	enCardType_SicboThreeOne,          // 骰点3个1
	enCardType_SicboThreeTwo,          // 骰点3个2
	enCardType_SicboThreeThree,        // 骰点3个3
	enCardType_SicboThreeFour,         // 骰点3个4
	enCardType_SicboThreeFive,         // 骰点3个5
	enCardType_SicboThreeSix,          // 骰点3个6

	enCardType_SicboThreeSame          // 骰点3个相同点数
};


// 记录信息
struct tagServerGameRecord
{
	// 骰宝游戏只记录骰子点数
	E_SICBO_NUMBER enCards[MAX_COUNT_SICBO];
};

//失败结构
struct CMD_S_PlaceJettonFail
{
	BYTE							lJettonArea;						//下注区域
	__int64							lPlaceScore;						//当前下注
};

// 更新积分
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							// 椅子号码
	__int64							lScore;								// 玩家积分

	//庄家信息
	WORD							wCurrentBankerChairID;				// 当前庄家
	BYTE							cbBankerTime;						// 庄家局数
	__int64							lCurrentBankerScore;				// 庄家分数
};

//申请庄家
struct CMD_S_ApplyBanker
{
	DWORD                           dwUserID;                           // 玩家ID
	WORD							wApplyUser;							// 申请玩家
};

//取消申请
struct CMD_S_CancelBanker
{
	DWORD                           dwUserID;                           // 玩家ID
	TCHAR							szCancelUser[32];					//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	DWORD                           dwUserID;                           // 玩家ID
	WORD							wBankerUser;						//当庄玩家
	__int64							lBankerScore;						//庄家金币
};

// 游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	__int64							lUserMaxScore;						//玩家金币

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	__int64							lBankerWinScore;					//庄家成绩
	__int64							lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	__int64							lApplyBankerCondition;				//申请条件
	__int64							lAreaLimitScore;					//区域限制
};

// 游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	__int64							lAllScore[COUNT_AZIMUTH];			// 东,南西北总注

	//玩家下注
	__int64							lUserScore[COUNT_AZIMUTH];			// 玩家买东,南西北总注

	//玩家积分
	__int64							lUserMaxScore;						// 最大下注
	__int64							lAllMaxScore;						// 最大下注

	//控制信息
	__int64							lApplyBankerCondition;				//申请条件
	__int64							lAreaLimitScore;					//区域限制

	//扑克信息
 	E_SICBO_NUMBER					enCards[MAX_COUNT_SICBO];		    // 骰子点数
	bool                            bWinner[COUNT_AZIMUTH];
	BYTE							bySicboIndex;				        // 当前摇的序号
	BYTE                            bySicboType;                        // 摇骰子类型

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	__int64							lBankerWinScore;					//庄家赢分
	__int64							lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//结束信息
	__int64							lEndBankerScore;					//庄家成绩
	__int64							lEndUserScore;						//玩家成绩
	__int64							lEndUserReturnScore;				//返回积分
	__int64							lEndRevenue;						//游戏税收

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
	__int64							lBankerScore;						//庄家金币
	__int64							lUserMaxScore;						//我的金币
	__int64							lAllMaxScore;						//当局最多下注金币
	BYTE							cbTimeLeave;						//剩余时间
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	__int64							lJettonScore;						//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	E_SICBO_NUMBER					enCards[MAX_COUNT_SICBO];		    // 骰子点数
	bool                            bWinner[COUNT_AZIMUTH];              // 输赢结果

	//庄家信息
	__int64							lBankerScore;						//庄家成绩
	__int64							lBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	__int64							lUserScore;							//玩家成绩
	__int64							lUserReturnScore;					//返回积分

	//全局信息
	__int64							lRevenue;							//游戏税收
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_CHANGE_SICBO_TYPE     4                                   // 庄家选择摇骰子方式
#define SUB_C_USE_WINNER            5                                   // 控制胜负

#define  SICBOTYPE_COUNT            5   // 摇骰子类型总数
enum E_SICBO_TYPE
{
	enSicboType_Normal=0x00,             // 普通的摇骰子
	enSicboType_Desktop,                 // 桌面摇骰子
	enSicboType_InMidair,                // 半空摇骰子
	enSicboType_InTheAir,                // 空中摇骰子
	enSicboType_Sicbo                    // 骰宝游戏的摇骰子方式
};

//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	__int64							lJettonScore;						//加注数目
};

struct CMD_C_ChangeSicboType
{
	E_SICBO_TYPE enSicboType;
};

struct CMD_C_UseWinner
{
	BYTE              byArea;  // 区域
	BYTE              byType;  // 类型
};

//////////////////////////////////////////////////////////////////////////