#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						10001								//游戏 I D
#define GAME_PLAYER					100									//游戏人数
#define GAME_NAME					TEXT("百人二八扛")					//游戏名字

//状态定义
#define	GS_PLACE_JETTON				GS_PLAYING							//下注状态
#define	GS_GAME_END					GS_PLAYING + 1						//结束状态
#define	GS_MOVECARD_END				GS_PLAYING + 2						//结束状态

//区域索引
#define ID_TIAN_MEN					1									//顺门
#define ID_DI_MEN					2									//天门
#define ID_XUAN_MEN					3									//地门

//玩家索引
#define BANKER_INDEX				0									//庄家索引
#define SHUN_MEN_INDEX				1									//顺门索引
#define DUI_MEN_INDEX				2									//天门索引
#define DAO_MEN_INDEX				3									//地门索引

#define AREA_COUNT					3									//区域数目

#define CARD_LOCATION_COUNT			4									//牌位置数
#define CARD_COUNT_OF_ONE			2									//一位牌数


struct CMD_S_AndroidArea
{
	BYTE bWinArea[AREA_COUNT];
	CMD_S_AndroidArea()
	{
		memset(this, 0, sizeof(*this));
	}
};

//赔率定义
#define RATE_TWO_PAIR				1									//对子赔率

//记录信息
struct tagServerGameRecord
{
	bool							bWinShunMen;						//顺门胜利
	bool							bWinDuiMen;							//天门胜利
	bool							bWinDaoMen;							//地门胜利
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
#define SUB_S_ANDROA_AREA			109
#define SUB_S_WIND_RATE_INFO		110									//赢率信息


//失败结构
struct CMD_S_PlaceJettonFail
{
	WORD								wPlaceUser;							//下注玩家
	BYTE								lJettonArea;						//下注区域
	LONGLONG							lPlaceScore;						//当前下注
};

//更新积分
struct CMD_S_ChangeUserScore
{
	WORD								wChairID;							//椅子号码
	LONGLONG							lScore;								//玩家积分

	//庄家信息
	WORD								wCurrentBankerChairID;				//当前庄家
	BYTE								cbBankerTime;						//庄家局数
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
	LONGLONG						lUserMaxScore;						//玩家金币

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONGLONG						lBankerWinScore;					//庄家成绩
	LONGLONG						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	LONGLONG						lAllJettonScore[AREA_COUNT+1];		//全体总注

	//玩家下注
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//个人总注

	//玩家积分
	LONGLONG						lUserMaxScore;						//最大下注

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//扑克信息
	BYTE							cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE];		//桌面扑克

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
	bool							bContiueCard;						//继续发牌
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

	BYTE							bcFirstCard[2];						//骰子点数
	//扑克信息
	BYTE							cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE];				//桌面扑克
	BYTE							cbLeftCardCount;					//扑克数目

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

struct CMD_S_Win_Rate_Info 
{
	DWORD							dwID[10];
	DWORD							dwRate[10];
	LONGLONG						llWinScore[10];
	LONGLONG						llLostScore[10];
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_CONTINUE_CARD			4									//继续发牌
#define SUB_C_GET_WIN_RATE			5									//获取赢率
#define SUB_C_SET_WIN_RATE			6									//设置赢率


//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
};

struct TAndroidUser
{
	bool blAndroidUser;
	int  PlayCount;
	int  BankCount;
	TAndroidUser()
	{
		memset(this, 0, sizeof(*this));
	};
};

struct UserPlaceJetton
{
	WORD dwUserID;
	WORD dwChairID;
	UserPlaceJetton()
	{
		memset(this, 0, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////
inline int  Findstr(char *buf, char findchar, int len)
{
	int i = 0;
	while (i < len)
	{
		if (buf[i++] == findchar)
			return (i - 1);
	}
	return -1;
}

inline char* strGetbuffer(char *buf, int ipos, char *OutBuf, int outLen)
{
	if (ipos < outLen)
		memcpy(OutBuf, buf, ipos);
	else
	{
		memcpy(OutBuf, buf, outLen);
	}
	return  OutBuf;

}

inline void MakeStirngToStirng(char *buf, int len, TCHAR outbuf[][128], int &ioutCount)
{

	int iMax = ioutCount;
	int iPos = 0;
	ioutCount = 0;
	while (1)
	{
		iPos = Findstr(buf, ',', (int)strlen(buf));
		if (iPos == -1)
		{
			char tmp[256] = {0};
			char *strtmp = buf;
			if (ioutCount < iMax - 1)
				memcpy(outbuf[ioutCount++], strtmp, (int)strlen(strtmp));
			break;
		}
		char tmp[256] = {0};
		char *strtmp = strGetbuffer(buf, iPos, tmp, 256);
		if (ioutCount < iMax - 1)
			memcpy(outbuf[ioutCount++], strtmp, (int)strlen(strtmp));
		buf = buf + iPos + 1;
	}
}

#endif
