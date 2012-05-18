#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#ifdef _DEBUG
	#define KIND_ID						27								    //游戏 I D
#else
	#define KIND_ID						28								    //游戏 I D
#endif

#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("生肖转转乐")					//游戏名字


//游戏状态
#define GS_BACCARAT_FREE            GS_FREE								//空闲
#define GS_BACCARAT_JETTON          GS_PLAYING							//下注
#define GS_BACCARAT_OPENCARD        GS_PLAYING+1						//开奖


//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START            100                                 //开始下注
#define SUB_S_OPEN_CARD			    101									//游戏开始
#define SUB_S_PLACE_JETTON			102									//用户下注
#define SUB_S_GAME_END				103									//游戏结束
#define SUB_S_APPLY_BANKER			104									//申请庄家
#define SUB_S_CHANGE_BANKER			105									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		106									//更新积分
#define SUB_S_CURRENT_BANKER		200
#define SUB_S_SEND_RECORD			107									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		108									//下注失败
#define SUB_S_GAME_SCORE			109									//发送积分
#define SUB_S_FULL					110									//下注已满
#define SUB_S_RESLUT				111									//结果
#define SUB_S_FREE					112


#define	MAX_AREA					14
#define MAX_HISTORY_LENTH			20
#define MAX_HISTORY_DISPLAY			9


//下注时间
#ifdef _DEBUG
#define TIME_PLACE_JETTON			18 									//下注时间
#else
#define TIME_PLACE_JETTON			18									//下注时间
#endif

//结束时间
#ifdef _DEBUG
#define TIME_OPEN_CARD				22									//开奖时间
#else
#define TIME_OPEN_CARD				22									//开奖时间
#endif

//空闲时间
#ifdef _DEBUG
#define TIME_GAME_FREE              6                                   //空闲时间
#else
#define TIME_GAME_FREE              6                                   //空闲时间
#endif


//失败结构
struct CMD_S_PlaceJettonFail
{
	LONGLONG							lJettonArea;						//下注区域
	LONGLONG							lPlaceScore;						//当前下注
	LONGLONG							lMaxLimitScore;						//限制大小
	LONGLONG							lFinishPlaceScore;					//已下注额
};

//更新积分
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//椅子号码
	LONGLONG						lScore;								//玩家积分

	//庄家信息
	WORD							wCurrentBankerChairID;				//当前庄家
	BYTE							cbBankerTime;						//庄家局数
	LONGLONG						lCurrentBankerScore;				//庄家分数
};

struct CMD_S_UserScore
{
	INT64							nScore;
	INT64							nBankerScore;
};

//申请庄家
struct CMD_S_ApplyBanker
{
	CHAR							szAccount[32];					    //申请玩家
	LONGLONG						lScore;								//玩家金币
	bool							bApplyBanker;						//申请标识
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wChairID;							//椅子号码
	BYTE							cbBankerTime;						//庄家局数
	LONGLONG						lBankerScore;						//庄家分数
	LONGLONG						lBankerTreasure;					//庄家金币

	//我的下注
	LONGLONG						lAreaLimitScore;					//区域限制
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//庄家信息
	WORD							wCurrentBankerChairID;				//当前庄家
	BYTE							cbBankerTime;						//庄家局数
	LONGLONG							lCurrentBankerScore;				//庄家分数
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制
	LONGLONG						lBankerTreasure;					//庄家金币
	LONGLONG                        lMeMaxScore;
	INT64							lBankerWin;
};

//游戏状态
struct CMD_S_StatusJetton
{
	//全局信息
	BYTE								cbTimeLeave;						//剩余时间

	WORD								wCurrentBankerChairID;				//当前庄家
	BYTE								cbBankerTime;						//庄家局数
	LONGLONG							lCurrentBankerScore;				//庄家分数
	LONGLONG							lApplyBankerCondition;				//申请条件
	LONGLONG							lAreaLimitScore;					//区域限制
	LONGLONG							lBankerTreasure;					//庄家金币
	INT64								lBankerWin;
};

struct CMD_S_StatusOpenCard
{
	BYTE		cbAnimalPos;
};

//游戏开始
struct CMD_S_OpenCard
{

};

//开始下注
struct CMD_S_GameStart
{
	BYTE                            cbBankerTimes;                        //剩余时间
	WORD							wBanker;
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
	LONGLONG						lMeMaxScore;						//最大下注
	BYTE							cbTimeLeave;						//剩余时间

	//成绩记录
	BYTE							cbWinner;							//胜利玩家
	BYTE							cbKingWinner;						//天王胜利
	BYTE                            cbPlayerPair;                       //闲家对子
	BYTE                            cbBankerPair;                       //庄家对子
	LONGLONG						lBankerTreasure;					//庄家金币

	LONGLONG						lBankerTotalScore;					//庄家成绩
	LONGLONG						lBankerScore;						//庄家成绩
	INT								nBankerTime;						//做庄次数
};

//游戏得分
struct CMD_S_GameScore
{
	//成绩记录
	BYTE							cbWinner;							//胜利玩家
	BYTE							cbKingWinner;						//天王胜利
	BYTE                            cbPlayerPair;                       //闲家对子
	BYTE                            cbBankerPair;                       //庄家对子
	LONGLONG						lMeGameScore;						//我的成绩
	LONGLONG						lMeReturnScore;						//返还注额
	LONGLONG						lBankerScore;						//庄家成绩

};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家

//用户下注
struct CMD_C_PlaceJetton
{	
	BYTE							cbViewJettonArea;					//中间区域
	BYTE							cbAnimal;							//0-13,12-13大小
	LONGLONG						lJettonScore;						//加注数目
};

//申请庄家
struct CMD_C_ApplyBanker
{
	bool							bApplyBanker;						//申请标识
};

//////////////////////////////////////////////////////////////////////////

#endif