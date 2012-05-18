#ifndef CMD_UNCOVERPIG_HEAD_FILE
#define CMD_UNCOVERPIG_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							380 								//游戏  ID
#define GAME_PLAYER						4									//游戏人数
#define GAME_NAME						TEXT("拱猪")						//游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型
#define HAND_CARD_COUNT                 13									//扑克数目    

//游戏状态
#define GS_WK_FREE				    	GS_FREE								//等待开始
#define GS_WK_SHOWCARD                  GS_PLAYING+1                        //玩家亮牌
#define GS_WK_PLAYING			    	GS_PLAYING+2						//游戏进行

///////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_SEND_CARD					100									//发牌命令
#define SUB_S_GAME_START				101									//游戏开始
#define SUB_S_OUT_CARD					102									//用户出牌
#define SUB_S_GAME_END					103									//游戏结束
#define SUB_S_SHOW_CARD					104									//玩家亮牌

//游戏状态
struct CMD_S_StatusFree
{
};

//游戏状态
struct CMD_S_StatusShowCard
{
	WORD								wCurrentUser;						//当前玩家
	WORD								ShowCardUser[GAME_PLAYER];	        //亮牌玩家
	BYTE								ShowCard[GAME_PLAYER][4];		    //玩家亮牌
	BYTE								ShowCardCount[GAME_PLAYER];			//亮牌数目
	BYTE								bCardData[HAND_CARD_COUNT];			//手上扑克
};

//游戏状态
struct CMD_S_StatusPlay
{
	WORD								wLastOutUser;						//最后出牌
	WORD								wFirstOutUser;                       //首先出牌
	WORD								wCurrentUser;						//当前玩家
	WORD								ShowCardUser[GAME_PLAYER];	        //亮牌玩家
	BYTE								ShowCard[GAME_PLAYER][4];		    //玩家亮牌
	BYTE								bCardData[HAND_CARD_COUNT];			//手上扑克
	BYTE								bCardCount[GAME_PLAYER];			//扑克数目
	BYTE								bTurnCardData[GAME_PLAYER];			//桌面出牌
	BYTE								ScoreCard[GAME_PLAYER][16];		    //分数扑克
	BYTE								bScoreCardCount[GAME_PLAYER];		//扑克数目
	BYTE								bShowCardCount[GAME_PLAYER];		//亮牌数目
};

//发送扑克
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						 //当前玩家
	BYTE								bCardData[HAND_CARD_COUNT];			 //手上扑克
};

//游戏开始
struct CMD_S_GameStart
{
	WORD								wCurrentUser;						//当前玩家
	bool								bFirstGame;							//首局游戏
};

//用户出牌
struct CMD_S_OutCard
{
	BYTE								bCardCount;						    //出牌数目
	WORD								wCurrentUser;						//当前玩家
	WORD								wOutCardUser;						//出牌玩家
	BYTE								bCardData;					        //出牌扑克
};

//游戏结束
struct CMD_S_GameEnd
{
	LONG								lGameTax;							//游戏税收
	LONG								lGameScore[4];						//游戏积分
};

//亮牌数据
struct CMD_S_ShowCard
{
	WORD								wShowCardUser;                     //亮牌玩家
	BYTE								bShowCard[4];                      //亮牌列表
	BYTE								bShowCardCount;                    //亮牌数目
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CART				   1								  //用户出牌
#define SUB_C_SHOW_CART                2                                  //玩家亮牌

//出牌数据包
struct CMD_C_OutCard
{
	BYTE								bCardCount;						   //出牌数目
	BYTE								bCardData;  					   //扑克列表
};

//亮牌数据包
struct CMD_C_ShowCard
{
	BYTE								bShowCard[4];                      //亮牌列表
	BYTE								bShowCardCount;                    //亮牌数目
};

//////////////////////////////////////////////////////////////////////////

#endif