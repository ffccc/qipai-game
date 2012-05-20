#ifndef CLIENT_HEAD_FILE
#define CLIENT_HEAD_FILE




//棋盘格子定义
struct tagClientTileItem
{
	USHORT							usXPos;								//棋盘格子坐标
	USHORT							usYPos;								//棋盘格子坐标
	BYTE							cbColor;							//棋盘格子颜色
	BYTE							cbChessCount;						//棋盘的棋子数
};

//棋子定义
struct tagClientChessItem
{
	BYTE							cbTileIndex;						//棋子在棋盘的位置
	BYTE							cbColor;							//棋子颜色
	BYTE							cbChessID;							//棋子标识，飞机0、1、2、3号
	BYTE							cbChessStatus;						//棋子状态，0为等待起飞，1为起飞，2为走，3为升天，4为完成回基地。
	USHORT							usDirection;						//飞机方向
	float							fXPos;								//棋盘格子坐标
	float							fYPos;								//棋盘格子坐标
};

#define CHESSMOVE_NOT_DEFINE		0xFF								//没有定义
#define CHESSMOVE_TO_FINISH			0xFE								//飞机完成，0到0xFD为CHESSMOVE被吃的棋子数
//棋盘格子定义
struct ChessMoveRoute
{
	BYTE							cbMoveMapNum;						//棋子移动ID
	BYTE							cbChessMoveFlag;					//棋子移动标志
	BYTE							cbRefChessMove;						//关联的链表数组的下标，在吃子的时候有用
	USHORT							usXPos;								//路径格子坐标
	USHORT							usYPos;								//路径格子坐标
	float							fXIncrement;
	float							fYIncrement;
};



#define NULL_TILE					0
#define NORMAL_TILE					1
#define	ROUND_TILE					2
#define SKY_TILE					3
#define BASE_TILE					4
#define VOLPLANE_TILE				5

#define TILE_W						34
#define TILE_H						34

#define PLANE_W						43
#define PLANE_H						43

#define PLANE_TILE_XOFFSET			-4	
#define PLANE_TILE_YOFFSET			-4

#define			IDM_SOUND_MUSIC						WM_USER+161			//声音消息
#define			IDM_SOUND_ARRIVE					0					//飞机完成旅程声音	
#define			IDM_SOUND_CRASH						1					//飞机被吃声音	
#define			IDM_SOUND_LANDOFF					2					//飞机起飞声音
#define			IDM_SOUND_TZBUTTON					3					//按动骰子声音

const DWORD aSeatColor[GAME_PLAYER] = {0xFFFF0000, 0xFFFFFF00, 0xFF00FF00, 0xFF0000FF};
#endif