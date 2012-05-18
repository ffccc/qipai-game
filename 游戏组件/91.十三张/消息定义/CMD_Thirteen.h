#ifndef CMD_LIANGFUTOU_HEAD_FILE
#define CMD_LIANGFUTOU_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义
//公共宏定义
#ifdef VIDEO_GAME
#define KIND_ID							12									//游戏 I D
#else
#define KIND_ID							13									//游戏 I D
#endif
#define GAME_PLAYER						4									//游戏人数
#define GAME_NAME						TEXT("十三张")					    //游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型
#define HAND_CARD_COUNT					13									//扑克数目

//游戏状态
#define GS_WK_FREE				    	GS_FREE								//等待开始
#define GS_WK_SETCHIP			    	GS_PLAYING+1						//设置下注
#define GS_WK_PLAYING			    	GS_PLAYING+2						//游戏进行
#define GS_WK_CALL_BANKER				GS_PLAYING + 3						//叫庄状态

//分段类型
enum enSegmentType
{
	enFront ,																//前墩类型
	enMid ,																	//中墩类型
	enBack,																	//后墩类型
	enAllSeg,																//所有扑克
	enErr,																	//错误类型
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_SEND_CARD					100									//发牌命令
#define SUB_S_SETCHIP					101									//设置下注
#define SUB_S_SETSEGMENT				102									//设置分段
#define SUB_S_SHOW_CARD					103									//玩家摊牌
#define SUB_S_GAME_END					104									//游戏结束
#define SUB_S_COMPARE_CARD				105									//比较扑克
#define SUB_S_SHOW_CARD_TIME			106									//摊牌时间
#define SUB_S_GAME_START				107									//游戏开始
#define SUB_S_START_CALL_BANKER			108									//开始叫庄									
#define SUB_S_CALL_BANKER				109									//玩家叫庄
#define SUB_S_END_ALL_NO_CALL_BANKER	110									//都不叫庄

//////////////////////////////////////////////////////////////////////////

//开始叫庄
struct CMD_S_StartCallBanker
{
	WORD								wCurrentCaller;						//当前玩家
};

//玩家叫庄
struct CMD_S_CallBanker
{
	WORD								wLastCaller ;						//已叫玩家
	bool								bCallBanker ;						//叫庄标识
	WORD								wCurrentCaller;						//当前玩家
} ;

//摊牌时间
struct CMD_S_ShowCardTime
{
	BYTE								bShowCardTime;						//摊牌时间
	WORD								wBanker;							//庄家玩家
	LONG								wMaxChip;							//最高下注
};

//下注状态
struct  CMD_S_StatusSetChip
{
	LONG								nChip[GAME_PLAYER];					//下注大小 
	BYTE								bHandCardData[HAND_CARD_COUNT];		//扑克数据
	WORD								wBanker;							//庄家玩家
	LONG								wMaxChip;							//最高下注
	LONG								wUserToltalChip;					//总共金币

};

//叫庄状态
struct CMD_S_StatusCallBanker
{
	WORD								wCurrentCaller;						//当前玩家
} ;

//发送扑克
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						 //当前玩家
	WORD								wBanker;							 //庄家玩家
	BYTE								bCardData[HAND_CARD_COUNT];			 //手上扑克
	LONG								wMaxChip;							//最高下注
	LONG								wUserToltalChip;					//总共金币
};

//游戏结束
struct CMD_S_GameEnd
{
	LONG								lGameTax;							//游戏税收
	LONG								lGameScore[4];						//游戏积分
	BYTE								bEndMode;							//结束方式
};

//设置下注
struct CMD_S_SetChip
{
	WORD								wCurrentUser;						//下注玩家
	LONG								nChip;								//下注大小
};

//设置分段
struct CMD_S_SetSegment
{
    WORD								wCurrentUser;						//当前玩家
	enSegmentType						SegmentType;						//分段类型
};

//玩家摊牌
struct CMD_S_ShowCard
{
	WORD								wCurrentUser;						//当前玩家
	BYTE								bFrontCard[3];						//前墩扑克
	BYTE								bMidCard[5];						//中墩扑克
	BYTE								bBackCard[5];						//后墩扑克
	bool								bCanSeeShowCard;					//能否看牌
};

//游戏状态
struct CMD_S_StatusPlay
{
	LONG								nChip[GAME_PLAYER];					//下注大小 
	BYTE								bHandCardData[HAND_CARD_COUNT];		//扑克数据
	BYTE								bSegmentCard[GAME_PLAYER][3][5];	//分段扑克
	bool								bFinishSegment[GAME_PLAYER];		//完成分段
	WORD								wBanker;							//庄家玩家
	LONG								wMaxChip;							//最高下注
	LONG								wUserToltalChip;					//总共金币
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_SETCHIP					300									//压注命令
#define SUB_C_SEGCARD					301									//分段命令
#define SUB_C_SHOWCARD					302									//玩家摊牌
#define SUB_C_COMPLETE_COMPARE			303									//完成比较
#define SEB_C_SHOW_CARD_TIME			304									//摊牌时间
#define SUB_C_BAO_PAI					305									//报牌命令
#define SUB_C_CALL_BANKER				306									//玩家叫庄
#define SUB_C_NO_CALL_BANKER			307									//玩家不叫


//摊牌时间
struct CMD_C_ShowCardTime
{
	BYTE								bShowCardTime;						//摊牌时间
	LONG								wMaxChip;							//最高下注
};

//设置压注
struct CMD_C_SetChip
{
	LONG								nChip;								//压注大小
};

//设置分段
struct CMD_C_SegCard
{
	enSegmentType						SegmentType;						//分段类型
};

//分段信息
struct CMD_C_ShowCard
{
	BYTE								bFrontCard[3];						//前墩扑克
	BYTE								bMidCard[5];						//中墩扑克
	BYTE								bBackCard[5];						//后墩扑克
};
//////////////////////////////////////////////////////////////////////////

#endif
