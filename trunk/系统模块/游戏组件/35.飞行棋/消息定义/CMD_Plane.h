#ifndef CMD_FLIGHTCHESS_HEAD_FILE
#define CMD_FLIGHTCHESS_HEAD_FILE

#include "..\..\..\公共文件\GlobalDef.h"
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							401								//游戏 I D
#define GAME_PLAYER						4									//游戏人数
#define GAME_NAME						TEXT("飞行棋游戏")					//游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型

//棋盘格数量
#define 	CHESSCOUNT_W				21
#define		CHESSCOUNT_H				20

//棋子颜色
#define RED								0									//红色(棋子)
#define YELLOW							1									//黄色(棋子)
#define GREEN							2									//绿色(棋子)
#define BLUE							3									//蓝色(棋子)

#define PLAYER_CHESS_COUNT				4

#define MAX_NORMAL_ROADWAY		51
#define PER_NORMAL_ROADWAY		(MAX_NORMAL_ROADWAY+1)/GAME_PLAYER
#define MAX_ROADWAY				75
#define MAX_ROADWAY_BASE		75 + GAME_PLAYER * PLAYER_CHESS_COUNT
#define MAX_VALID_DOWN			MAX_ROADWAY_BASE + GAME_PLAYER
#define MAX_RANDNUM				6
#define UP_SKY_STEP			    6

#define N						255
#define NONE_TILE				N

#define CHESS_NOT_FINISHED      1
#define CHESS_FINISHED			2

#define NOT_AUTOMATISM				0
#define HALF_AI_AUTOMATISM			1
#define ALL_AI_AUTOMATISM			2

#define LIMIT_TIME					20999



#define GAME_NOT_ACTIVE			0
#define GAME_WAIT_START			1
#define GAME_WAIT_DICE			2
#define GAME_DICE_ANIMATED		3
#define GAME_WAIT_CHOOSE		4
#define GAME_CHESS_ANIMATED		5

//#define GAME_WAIT_DICE_OTHER		3
//#define GAME_DICE_ANIMATED_OTHER	6
//#define GAME_WAIT_CHOOSE_OTHER		4
//#define GAME_CHESS_ANIMATED_OTHER	8
#define GAME_WAIT_OTHER				6
#define GAME_READY_TO_START			7



//棋盘地图
const BYTE FLIGHTCHESS_MAP[CHESSCOUNT_H][CHESSCOUNT_W] = 	{	
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,203,N,N,N,N,N,N,	
		N,N,202,N,N,N,N,33,34,35,36,37,38,39,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,32,N,N,70,N,N,40,N,N,N,N,N,N,N,		
		N,N,N,N,N,N,N,31,N,N,71,N,N,41,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,30,228,229,72,230,231,42,N,N,N,N,N,N,N,	
		N,N,N,26,27,28,29,212,N,N,73,N,N,213,43,44,45,46,N,N,N,
		N,N,N,25,N,N,227,N,N,N,74,N,N,N,232,N,N,47,N,N,N,	
		N,N,N,24,N,N,226,N,N,N,75,N,N,N,233,N,N,48,N,N,N,
		N,N,N,23,64,65,66,67,68,69,N,57,56,55,54,53,52,49,N,N,N,	
		N,N,N,22,N,N,225,N,N,N,63,N,N,N,234,N,N,50,N,N,N,
		N,N,N,21,N,N,224,N,N,N,62,N,N,N,235,N,N,51,N,N,N,	
		N,N,N,20,19,18,17,211,N,N,61,N,N,210,3,2,1,0,N,N,N,
		N,N,N,N,N,N,N,16,223,222,60,221,220,4,N,200,N,N,N,N,N,	
		N,N,N,201,N,N,N,15,N,N,59,N,N,5,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,14,N,N,58,N,N,6,N,N,N,N,N,N,N,		
		N,N,N,N,N,N,N,13,12,11,10,9,8,7,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,	
		N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N		
	};
//定义特殊点，specialPull[0],specialPull[1]分别与specialDown[0],specialDown[1]相对应

const BYTE SPECIALPULL[GAME_PLAYER][2]={{17,49}, {30,10},{43,23},{4,36}};//特殊点
const BYTE SPECIALTONEXT[GAME_PLAYER][3]={{29,52,66},{42,58,72},{3,64,54},{16,70,60}};  //特殊点的落点和经过点


//#define STATUS_WAIT			0
//#define STATUS_FLYUP		1
//#define	STATUS_MOVE			2
//#define STATUS_TOSKY		3
//#define STATUS_COMPLETE		4
//棋子定义
struct tagServerChessItem
{
	BYTE							cbTileIndex;						//棋子在棋盘的位置
	BYTE							cbColor;							//棋子颜色
	BYTE							cbChessID;							//棋子标识，飞机0、1、2、3号
	BYTE							cbChessStatus;						//棋子状态，0为等待起飞，1为起飞，2为走，3为升天，4为完成回基地。
};

//棋盘格子定义
struct tagServerTileItem
{
	BYTE							cbColor;							//棋盘格子颜色
	BYTE							cbChessCount;						//棋盘的棋子数
};

//////////////////////////////////////////////////////////////////////////
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_DICE_CAST				101									//游戏开始
#define SUB_S_CHESS_CHOOSE			102									//游戏开始
#define SUB_S_USER_ENTER			103									//用户进入
#define SUB_S_USER_LEFT				104									//用户离开（游戏未开始）
#define SUB_S_USER_GIVEUP			105									//用户离开（游戏开始）
#define SUB_S_USER_READY			106									//用户准备
#define SUB_S_GAME_END				107
#define SUB_S_TIME_OUT				108
#define SUB_S_AUTOMATISM			109

#define SUB_C_DICE_CAST				1									//移动棋子
#define SUB_C_CHESS_CHOOSE			2
#define SUB_C_TIME_OUT				3									//走棋超时
#define SUB_C_AUTOMATISM			4
//////////////////////////////////////////////////////////////////////////
//服务器命令结构
//游戏开始
struct CMD_S_StatusFree
{
	BYTE							cbPlayStatus[GAME_PLAYER];				//开始出棋的玩家
};

struct CMD_S_StatusPlay
{
	WORD							wCurrentUser;							//当前玩家
	long							lLimitTimeCount;
	BYTE							cbPlayStatus[GAME_PLAYER];				//开始出棋的玩家
	BYTE							cbAutomatism[GAME_PLAYER];				//电脑自动托管
	BYTE							cbDiceNum;								//抛出的骰子number
	BYTE							cbGameStatus;
	tagServerChessItem				aServerChess[GAME_PLAYER][PLAYER_CHESS_COUNT];
};

struct CMD_S_GameStart
{
	WORD							wCurrentUser;							//开始出棋的玩家
};

struct CMD_S_DiceCast
{
	BYTE							cbDiceNum;								//抛出的骰子number
};

struct CMD_S_ChessChoose
{
	WORD							wCurrentUser;							//出棋的当前玩家
	BYTE							cbChessID;
};

//游戏结束
struct CMD_S_GameEnd
{
	WORD							wWinUser;							//胜利玩家
	long							lUserScore[GAME_PLAYER];			//用户积分
};

//离开游戏（未开始）
struct CMD_S_UserLeft
{
	WORD							wLeftUser;							//轮到出棋的玩家
};


//离开游戏（已开始）
struct CMD_S_UserLost
{
	WORD							wLeftUser;								//离开的玩家
	WORD							wCurrentUser;							//轮到出棋的玩家
};


//进入游戏
struct CMD_S_UserEnter
{
	WORD							wCurrentUser;							//开始出棋的玩家
};

//玩家准备
struct CMD_S_UserReady
{
	WORD							wCurrentUser;							//开始出棋的玩家
};

//玩家超时
/*struct CMD_S_UserTimeOut
{
	BYTE							cbTimeOutUser;							//超时的玩家
};*/

//玩家托管事件
struct CMD_S_UserAutomatism
{
	WORD							wAutoUser;
	BYTE							cbAutomatism;						//发生托管事件的玩家
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

struct CMD_C_ChessChoose
{
	BYTE							cbChessId;
};

struct CMD_C_Automatism
{
	BYTE							Automatism;
};
//////////////////////////////////////////////////////////////////////////

#endif