#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//服务定义

#define KIND_ID						119									//游戏 I D
#define GAME_NAME					TEXT("视频510K")						//游戏名字

//组件属性
#define GAME_PLAYER					4									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(5,1,0)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(5,1,0)				//程序版本

//////////////////////////////////////////////////////////////////////////
//数目定义

#define MAX_COUNT					28									//最大数目
#define FULL_COUNT					108									//全牌数目

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//扑克类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对子类型
#define CT_SINGLE_LINK				3									//单连类型
#define CT_DOUBLE_LINK				4									//对连类型
#define CT_BOMB_3					5									//3炸弹
#define CT_510K_DC					6									//杂510K
#define CT_KING_DC					7									//大小王
#define CT_510K_SC					8									//纯510K
#define CT_KING_ST					9									//纯小王
#define CT_KING_SK					10									//纯大王
#define CT_BOMB_4					11									//4炸类型
#define CT_BOMB_5					12									//5弹类型
#define CT_KING_THREE				13									//3王类型
#define CT_BOMB_6					14									//6炸类型
#define CT_BOMB_7					15									//7炸类型
#define CT_KING_FOUR				16									//4王炸弹
#define CT_BOMB_8					17									//8炸类型

//////////////////////////////////////////////////////////////////////////
//状态定义

#define GAME_SCENE_FREE				GS_FREE							//等待开始
#define GAME_SCENE_PLAY				GS_PLAYING						//游戏进行
#define GAME_SCENE_WAIT				GS_PLAYING+1					//等待开始

//////////////////////////////////////////////////////////////////////////
//命令定义

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				104									//用户出牌
#define SUB_S_PASS_CARD				105									//用户放弃
#define SUB_S_CARD_INFO				106									//扑克信息
#define SUB_S_GAME_END				107									//游戏结束
#define SUB_S_CONTINUE_GAME			108									//继续游戏
#define SUB_S_AUTOMATISM			109									//托管消息

//玩家托管事件
struct CMD_S_UserAutomatism
{
	bool							bAutoStatus[4];						//托管状态
};

//空闲状态
struct CMD_S_StatusFree
{
	//单元积分
	LONG							lCellScore;							//单元积分
	bool							bAutoStatus[4];						//托管状态
};

//游戏状态
struct CMD_S_StatusPlay
{
	//游戏变量
	LONG							lCellScore;							//单元积分
	WORD							wCurrentUser;						//当前玩家
	WORD							wPersistInfo[GAME_PLAYER][2];		//游戏信息
	BYTE							b510KCardRecord[3][8];

	//等级变量
	BYTE							cbMainValue;						//主牌数值
	BYTE							cbValueOrder[GAME_PLAYER];			//等级数值

	//胜利信息
	WORD							wWinCount;							//胜利人数
	WORD							wWinOrder[GAME_PLAYER];				//胜利列表

	//出牌信息
	WORD							wTurnWiner;							//本轮胜者
	BYTE							cbTurnCardType;						//出牌类型
	BYTE							cbTurnCardCount;					//出牌数目
	BYTE							cbTurnCardData[MAX_COUNT];			//出牌数据
	BYTE							cbMagicCardData[MAX_COUNT];			//变幻扑克

	//扑克信息
	BYTE							cbHandCardData[MAX_COUNT];			//手上扑克
	BYTE							cbHandCardCount[GAME_PLAYER];		//扑克数目
	bool							bAutoStatus[4];						//托管状态
};

//等待状态
struct CMD_S_StatusWait
{
	//单元积分
	LONG							lCellScore;							//单元积分

	//游戏变量
	BYTE							cbContinue[GAME_PLAYER];			//继续标志
	WORD							wPersistInfo[GAME_PLAYER][2];		//游戏信息

	//等级变量
	BYTE							cbMainValue;						//主牌数值
	BYTE							cbValueOrder[GAME_PLAYER];			//等级数值
	bool							bAutoStatus[4];						//托管状态
};

//发送扑克
struct CMD_S_GameStart
{
	//游戏信息
	BYTE							cbMainValue;						//主牌数值
	BYTE							cbValueOrder[GAME_PLAYER];			//等级数值
	WORD							wPersistInfo[GAME_PLAYER][2];		//游戏信息

	//扑克信息
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbCardData[MAX_COUNT-1];			//扑克列表
};

//用户出牌
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	int								TurnScore;
	BYTE							b510KCardRecord[3][8];
	BYTE							cbCardData[MAX_COUNT-1];			//扑克列表
};

//放弃出牌
struct CMD_S_PassCard
{
	BYTE							cbTurnOver;							//一轮结束
	WORD				 			wCurrentUser;						//当前玩家
	WORD				 			wPassCardUser;						//放弃玩家
	int								TurnScore;
	int								PlayerScore[GAME_PLAYER];
};

//扑克信息
struct CMD_S_CardInfo
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[MAX_COUNT];				//扑克列表
};

//游戏结束
struct CMD_S_GameEnd
{
	int								TurnScore;
	int								PlayerScore[GAME_PLAYER];

	//游戏成绩
	LONG							lGameScore[GAME_PLAYER];			//游戏积分

	//累计名次
	WORD							wPersistInfo[GAME_PLAYER][2];		//游戏信息

	//扑克信息
	BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//扑克列表
};

//用户继续
struct CMD_S_ContinueGame
{
	WORD							wChairID;							//继续用户
};

//////////////////////////////////////////////////////////////////////////
//命令定义

#define SUB_C_OUT_CARD				1									//用户出牌
#define SUB_C_PASS_CARD				2									//用户放弃
#define SUB_C_CONTINUE_GAME			5									//继续游戏
#define SUB_C_AUTOMATISM			6									//托管消息

//用户托管
struct CMD_C_Automatism
{
	bool							bAutomatism;
};

//用户出牌
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//出牌数目
	BYTE							cbCardData[MAX_COUNT];				//扑克数据
};


//////////////////////////////////////////////////////////////////////////

#endif