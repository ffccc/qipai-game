#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						31									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("百人三公")						//游戏名字

//状态定义
#define	GS_PLACE_JETTON				GS_PLAYING							//下注状态
#define	GS_GAME_END					GS_PLAYING+1						//结束状态

//玩家索引
//#define ID_XIAN_JIA					1									//闲家索引
//#define ID_PING_JIA					2									//平家索引
//#define ID_ZHUANG_JIA				3									//庄家索引
//#define ID_XIAN_TIAN_WANG			4									//闲天王
//#define ID_ZHUANG_TIAN_WANG			5									//庄天王
//#define ID_TONG_DIAN_PING			6									//同点平
//#define ID_PLAYER_TWO_PAIR			7									//闲对子
//#define ID_BANKER_TWO_PAIR			8									//庄对子

#define ID_TIAN_MARK				0x01
#define ID_DI_MARK					0x02
#define ID_XUAN_MARK				0x04
#define ID_HUANG_MARK				0x08

//赔率定义
//#define RATE_TWO_PAIR				12									//对子赔率

//记录信息
struct tagServerGameRecord
{
	BYTE							cbWinner;						
//	BYTE							cbMePlaceRegion;
	//bool							bPlayerTwoPair;						//对子标识
	//bool							bBankerTwoPair;						//对子标识
	//BYTE							cbPlayerCount;						//闲家点数
	//BYTE							cbBankerCount;						//庄家点数
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
	LONG							lPlaceScore;						//当前下注
};

//更新积分
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//椅子号码
	DOUBLE							lScore;								//玩家积分

	//庄家信息
	WORD							wCurrentBankerChairID;				//当前庄家
	BYTE							cbBankerTime;						//庄家局数
	DOUBLE							lCurrentBankerScore;				//庄家分数
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
	LONG							lBankerScore;						//庄家银子
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	LONG							lUserMaxScore;							//玩家银子

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONG							lBankerWinScore;					//庄家成绩
	LONG							lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	LONG							lApplyBankerCondition;				//申请条件
	LONG							lAreaLimitScore;					//区域限制
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	//LONG							lAllTieScore;						//买平总注
	//LONG							lAllBankerScore;					//买庄总注
	//LONG							lAllPlayerScore;					//买闲总注
	//LONG							lAllTieSamePointScore;				//平天王注
	//LONG							lAllBankerKingScore;				//庄天王注
	//LONG							lAllPlayerKingScore;				//闲天王注
	//LONG							lAllPlayerTwoPair;					//闲家对子
	//LONG							lAllBankerTwoPair;					//庄家对子
	LONG							lAllTianScore;							//天
	LONG							lAllDiScore;							//地
	LONG							lAllXuanScore;							//玄
	LONG							lAllHuangScore;							//黄


	////玩家下注
	//LONG							lUserTieScore;						//买平总注
	//LONG							lUserBankerScore;					//买庄总注
	//LONG							lUserPlayerScore;					//买闲总注
	//LONG							lUserTieSamePointScore;				//平天王注
	//LONG							lUserBankerKingScore;				//庄天王注
	//LONG							lUserPlayerKingScore;				//闲天王注
	//LONG							lUserPlayerTwoPair;					//闲家对子
	//LONG							lUserBankerTwoPair;					//庄家对子
	LONG							lUserTianScore;							//天
	LONG							lUserDiScore;								//地
	LONG							lUserXuanScore;							//玄
	LONG							lUserHuangScore;							//黄

	//玩家积分
	LONG							lUserMaxScore;						//最大下注							

	//控制信息
	LONG							lApplyBankerCondition;				//申请条件
	LONG							lAreaLimitScore;					//区域限制

	//扑克信息
// 	BYTE							cbCardCount[2];						//扑克数目
	BYTE							cbTableCardArray[6][3];				//桌面扑克

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONG							lBankerWinScore;					//庄家赢分
	LONG							lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//结束信息
	LONG							lEndBankerScore;					//庄家成绩
	LONG							lEndUserScore;						//玩家成绩
	LONG							lEndUserReturnScore;				//返回积分
	LONG							lEndRevenue;						//游戏税收

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
	LONG							lBankerScore;						//庄家银子
	LONG							lUserMaxScore;						//我的银子
	BYTE							cbTimeLeave;						//剩余时间
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	LONG							lJettonScore;						//加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
//	BYTE							cbCardCount[2];						//扑克数目
	BYTE							cbTableCardArray[6][3];				//桌面扑克
 
	//庄家信息
	LONG							lBankerScore;						//庄家成绩
	LONG							lBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	LONG							lUserScore;							//玩家成绩
	LONG							lUserReturnScore;					//返回积分

	//全局信息
	LONG							lRevenue;							//游戏税收
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
	LONG							lJettonScore;						//加注数目
};

//////////////////////////////////////////////////////////////////////////

#endif
