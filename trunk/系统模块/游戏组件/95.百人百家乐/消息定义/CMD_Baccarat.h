#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						100									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("庄闲平")						//游戏名字

//状态定义
#define	GS_PLACE_JETTON				GS_PLAYING							//下注状态
#define	GS_GAME_END					GS_PLAYING+1						//结束状态

//玩家索引
#define ID_XIAN_JIA					1									//闲家索引
#define ID_PING_JIA					2									//平家索引
#define ID_ZHUANG_JIA				3									//庄家索引
#define ID_XIAN_TIAN_WANG			4									//闲天王
#define ID_ZHUANG_TIAN_WANG			5									//庄天王
#define ID_TONG_DIAN_PING			6									//同点平
#define ID_PLAYER_TWO_PAIR			7									//闲对子
#define ID_BANKER_TWO_PAIR			8									//庄对子

//赔率定义
#define AREA_COUNT					5									//区域数目
#define RATE_TWO_PAIR				12									//对子赔率
#define SERVER_LEN					32									//房间长度

//机器人信息
struct tagRobotInfo
{
	int nChip[6];														//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[6] = {100, 1000, 10000, 100000, 1000000, 5000000};
		int nTmpAreaChance[AREA_COUNT] = {3, 0, 3, 1, 1};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("BaccaratBattleConfig.ini");

		nMaxTime = 2;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//记录信息
struct tagServerGameRecord
{
	BYTE							cbKingWinner;						//天王赢家
	bool							bPlayerTwoPair;						//对子标识
	bool							bBankerTwoPair;						//对子标识
	BYTE							cbPlayerCount;						//闲家点数
	BYTE							cbBankerCount;						//庄家点数
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
#define SUB_S_AMDIN_COMMAND			109									//管理员命令

#define UR_GAME_CONTROL             17907712                            //管理员权限  

//请求回复
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//回复类型
		 #define ACK_SET_WIN_AREA  1
		 #define ACK_PRINT_SYN     2
		 #define ACK_RESET_CONTROL 3
	BYTE cbResult;
	#define CR_ACCEPT  2			//接受
	#define CR_REFUSAL 3			//拒绝
	BYTE cbExtendData[20];			//附加数据
};

//失败结构
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//下注玩家
	BYTE							lJettonArea;						//下注区域
	LONGLONG						lPlaceScore;						//当前下注
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
	TCHAR							szCancelUser[32];					//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	LONGLONG						lBankerScore;						//庄家金币
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	LONGLONG						lUserMaxScore;							//玩家金币

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONGLONG						lBankerWinScore;					//庄家成绩
	LONGLONG						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//房间信息
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	LONGLONG						lAllTieScore;						//买平总注
	LONGLONG						lAllBankerScore;					//买庄总注
	LONGLONG						lAllPlayerScore;					//买闲总注
	LONGLONG						lAllTieSamePointScore;				//平天王注
	LONGLONG						lAllBankerKingScore;				//庄天王注
	LONGLONG						lAllPlayerKingScore;				//闲天王注
	LONGLONG						lAllPlayerTwoPair;					//闲家对子
	LONGLONG						lAllBankerTwoPair;					//庄家对子

	//玩家下注
	LONGLONG						lUserTieScore;						//买平总注
	LONGLONG						lUserBankerScore;					//买庄总注
	LONGLONG						lUserPlayerScore;					//买闲总注
	LONGLONG						lUserTieSamePointScore;				//平天王注
	LONGLONG						lUserBankerKingScore;				//庄天王注
	LONGLONG						lUserPlayerKingScore;				//闲天王注
	LONGLONG						lUserPlayerTwoPair;					//闲家对子
	LONGLONG						lUserBankerTwoPair;					//庄家对子

	//玩家积分
	LONGLONG						lUserMaxScore;						//最大下注							

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//扑克信息
 	BYTE							cbCardCount[2];						//扑克数目
	BYTE							cbTableCardArray[2][3];				//桌面扑克

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONGLONG						lBankerWinScore;					//庄家赢分
	LONGLONG						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//结束信息
	LONGLONG						lEndBankerScore;					//庄家成绩
	LONGLONG						lEndUserScore;						//玩家成绩
	LONGLONG						lEndUserReturnScore;				//返回积分
	LONGLONG						lEndRevenue;						//游戏税收

	//全局信息
	BYTE							cbTimeLeave;						//剩余时间
	BYTE							cbGameStatus;						//游戏状态

	//房间信息
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称
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
	int								nChipRobotCount;					//人数上限 (下注机器人)
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
	BYTE							cbAndroidUser;						//机器标识
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE							cbCardCount[2];						//扑克数目
	BYTE							cbTableCardArray[2][3];				//桌面扑克
 
	//庄家信息
	LONGLONG						lBankerScore;						//庄家成绩
	LONGLONG						lBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	LONGLONG						lUserScore;							//玩家成绩
	LONGLONG						lUserReturnScore;					//返回积分

	//全局信息
	LONGLONG						lRevenue;							//游戏税收
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_AMDIN_COMMAND			4									//管理员命令


struct CMD_C_AdminReq
{
	BYTE cbReqType;
		 #define RQ_SET_WIN_AREA	1
		 #define RQ_RESET_CONTROL	2
		 #define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//附加数据
};


//struct CMD_C_AdminReq
//{
//	BYTE cbReqType;
//		 #define RQ_SETWINAREA   1 
//	BYTE cbExtendData[120];
//};


//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
};

//////////////////////////////////////////////////////////////////////////

#endif
