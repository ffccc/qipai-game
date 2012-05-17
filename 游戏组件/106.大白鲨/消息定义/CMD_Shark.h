#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						500									//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("大白鲨")						//游戏名字

//状态定义
#define	GS_PLACE_JETTON				GS_PLAYING							//下注状态
#define	GS_GAME_END					GS_PLAYING+1						//结束状态

//历史记录
#define MAX_SCORE_HISTORY			50									//历史个数

//玩家索引
#define ID_BIG_SHARK				1									//大白鲨
#define ID_FEI_QIN					2									//飞禽
#define ID_ZOU_SHOU					3									//走兽
#define ID_YAN_ZI					4									//燕子引
#define ID_TU_ZI					5									//兔子引
#define ID_GE_ZI					6									//鸽子引
#define ID_XIONG_MAO				7									//熊猫
#define ID_KONG_QUE					8									//孔雀
#define ID_HOU_ZI					9									//猴子
#define ID_LAO_YING					10									//老鹰
#define ID_SHI_ZI					11									//狮子

//下注区域大小
#define SIZE_X						75									//X轴
#define SIZE_Y						111									//Y轴
#define SIZE_X_2					200									//X轴
#define SIZE_Y_2					55									//Y轴
#define SIZE_X_SHARK				145									//X轴
#define SIZE_Y_SHARK				150									//Y轴

//赔率定义
#define JETTON_AREA_COUNT			11									//区域个数

//筹码种类定义
#define JETTON_COUNT				6									//筹码种类	

//索引位置							
#define SELECT_TOP					0									//上
#define SELECT_BOTTOM				2									//下
#define SELECT_LEFT					3									//左
#define SELECT_RIGHT				1									//由
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
#define SUB_S_SCORE_HISTORY			109									//历史记录
#define SUB_S_CHANGE_SYS_BANKER		110									//切换庄家

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
	LONG							lBankerScore;						//庄家金币
};

//切换庄家
struct CMD_S_ChangeSysBanker
{
	bool							bEnableSysBanker;					//系统做庄
	WORD							cbBankerTime;						//庄家局数
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	LONG							lUserMaxScore;							//玩家金币

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
	//全体下注
	LONG							lALLBigSharkScore;					//自己买燕子总数
	LONG							lALLFeiQinScore;					//自己买燕子总数
	LONG							lALLZouShouScore;					//自己买燕子总数
	LONG							lALLYanZiScore;						//自己买燕子总数
	LONG							lALLTuZiScore;						//兔子
	LONG							lALLGeZiScore;						//鸽子
	LONG							lALLXiongMaoScore;					//熊猫
	LONG							lALLKongQueScore;					//孔雀
	LONG							lALLHouZiScore;						//猴子
	LONG							lALLLaoYingScore;					//老鹰
	LONG							lALLShiZiScore;						//狮子
	//个人下注
	LONG							lUserBigSharkScore;					//自己买大白鲨总数
	LONG							lUserFeiQinScore;					//自己买飞禽总数
	LONG							lUserZouShouScore;					//自己买走兽总数
	LONG							lUserYanZiScore;					//自己买燕子总数
	LONG							lUserTuZiScore;						//兔子
	LONG							lUserGeZiScore;						//鸽子
	LONG							lUserXiongMaoScore;					//熊猫
	LONG							lUserKongQueScore;					//孔雀
	LONG							lUserHouZiScore;					//猴子
	LONG							lUserLaoYingScore;					//老鹰
	LONG							lUserShiZiScore;					//狮子

	//玩家积分
	LONG							lUserMaxScore;						//最大下注							

	//控制信息
	LONG							lApplyBankerCondition;				//申请条件
	LONG							lAreaLimitScore;					//区域限制

	//索引信息
	BYTE							cbMoveEndIndex;						//移动减速索引
	BYTE							cbResultIndex;						//结果索引
	BYTE							cbEndIndex;							//停止索引

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

	//玩家状态
	BYTE							cbUserStatus;						//用户状态
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
	LONG							lBankerScore;						//庄家金币
	LONG							lUserMaxScore;						//我的金币
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

	//索引信息
	BYTE							cbMoveEndIndex;						//移动减速索引
	BYTE							cbResultIndex;						//结果索引
	BYTE							cbEndIndex;							//停止索引
 
	//庄家信息
	LONG							lBankerScore;						//庄家成绩
	LONG							lBankerTotallScore;					//庄家总成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	LONG							lUserScore;							//玩家成绩
	LONG							lUserReturnScore;					//返回积分

	//全局信息
	LONG							lRevenue;							//游戏税收

	//玩家下注信息
	LONG							lUserBigSharkScore;					//自己买大白鲨总数
	LONG							lUserFeiQinScore;					//自己买飞禽总数
	LONG							lUserZouShouScore;					//自己买走兽总数
	LONG							lUserYanZiScore;					//自己买燕子总数
	LONG							lUserTuZiScore;						//兔子
	LONG							lUserGeZiScore;						//鸽子
	LONG							lUserXiongMaoScore;					//熊猫
	LONG							lUserKongQueScore;					//孔雀
	LONG							lUserHouZiScore;					//猴子
	LONG							lUserLaoYingScore;					//老鹰
	LONG							lUserShiZiScore;					//狮子
};

//游戏结束
struct CMD_S_ScoreHistory
{
	BYTE							cbScoreHistroy[MAX_SCORE_HISTORY];					//历史信息
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
