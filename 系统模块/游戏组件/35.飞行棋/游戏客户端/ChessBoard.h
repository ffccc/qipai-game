#ifndef CHESS_BOARD_HEAD_FILE
#define CHESS_BOARD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DxGraphics.h"

//////////////////////////////////////////////////////////////////////////
//宏定义
#define		CHESSMOVE_NOT_ACTIVE		0
#define		CHESSMOVE_NOT_FINISH		1
#define		CHESSMOVE_HAS_FINISHED		2


struct ChessMoveData{
	SHORT shXDirection;
	SHORT shYDirection;
};
// 棋子移动效果
class ChessMove{
public:

	WORD							m_wPlayerID;
	BYTE							m_cbChessID;
	BYTE							m_MoveStatus;				//路径动画状态，0未激活，1激活但未完成，2已完成。
	BYTE							m_CurFrame;					//当前帧号
	BYTE							m_cbRefChessMove;			//棋子移动的定义不是0xFF时就是关联的ChessMove链表数组的下标，在同一个棋子有多个路径的时候使用
	BYTE							m_cbBeRefedCount;			//0表示没有关联，1表示被一个关联，2表示被两个关联，依此类推。
	CArrayTemplate<ChessMoveRoute>	m_ChessMoveRoute;			//Map转换


	ChessMove()
	{	
		m_CurFrame = 0;
	}
	~ChessMove()
	{
		m_ChessMoveRoute.RemoveAll();
	}
	
};
//棋子定位类型
struct tagChessID
{
	BYTE							cbColor;							//棋子颜色
	BYTE							cbChessID;							//棋子标识，飞机0、1、2、3号
};

struct BeEatedTile
{
	BYTE							cbChessCount;
	BYTE							cbTileIndex;
	BYTE							cbRefChessMove;						//对应的开始的ChessMove数组
	BeEatedTile(BYTE lcbChessCount, BYTE lcbTileIndex, BYTE lcbRefChessMove){
		cbChessCount = lcbChessCount;
		cbTileIndex = lcbTileIndex;
		cbRefChessMove = lcbRefChessMove;
	}
	BeEatedTile(){}
};
//////////////////////////////////////////////////////////////////////////

//游戏棋盘
class CChessBoard 
{
	friend class CD3DWnd;
	friend class CGameClientDlg;
	//配置变量


	//状态变量
public:

	cTexture		m_ChessBoardTexture;								//棋盘Texture
	cTexture		m_PlaneTexture;										//棋子表面
	cTexture		m_FinishPlaneTexture;								//成功回航飞机表面
protected:
	CArrayTemplate<tagClientTileItem>		m_ChessBoard;										//服务器棋盘信息
	tagClientChessItem				m_ClientChessItem[GAME_PLAYER][PLAYER_CHESS_COUNT];	//客户端棋子
	WORD							m_CurPlayerID;										//当前玩家
	WORD							m_CurMeChairID;										//进程所对应玩家的座位号
	BYTE							m_CurChessID;										//当前玩家选择的棋子
	BYTE							m_bPlayStatus[GAME_PLAYER];				//玩家状态
	BYTE							m_bViewSeatID[GAME_PLAYER];				//逻辑数据到视图数据的转换。
    struct ChessMoveData m_ChessMoveMap[8];
    USHORT			m_usSpeed;			//动画速度
	CPoint			m_BasePosition[GAME_PLAYER];

	CArrayTemplate<ChessMove,ChessMove &>	m_ChessMove;				//棋子走动容器
	//CGameLogic								m_GameLogic;						//游戏逻辑
	CArrayTemplate<BYTE>					m_CurActiveChess;					//当前玩家的活动棋子
	//函数定义
public:
	//构造函数
	CChessBoard();
	//初始化棋盘
	void InitChessBoard(cGraphics *pGraphics);
	//初始化棋子
	void InitChess();
	//析构函数
	virtual ~CChessBoard();
	//转动了骰子后准备移动棋子，包括计算路径和吃棋判断，吃了棋子后的路径计算。
	//void PrepareMoveChess(BYTE randNum);
	//用作棋子移动的动画显示，在动画结束的时候棋子的最终位置才计算出来。
	void MakeChessMove(DWORD ElapseTime);
	//切换当前用户
	void SwitchCurrentUser(WORD cbPlayerID);
	//切换当前棋子
	void SwitchCurrentChess(BYTE cbChessID);
	//设置进程所对应玩家的座位号
	void SetCurMeChairID(BYTE cbChairID);
	//把棋子画到屏幕
	void BlitChess();
	//产生吃子的路径
	BYTE BeKilledRouteGen(BYTE cbTileID, BYTE &cbChessCount, BYTE cbColor);
	//产生路径
	BOOL ChessRouteGen( BYTE randNum );
	//产生x和y方向的增长率
	void IncrementGen(const tagClientTileItem & origin, const tagClientTileItem & target, float &fXIncrement, float &fYIncrement);
	//产生方向号码
	BYTE DirectionMapNum(const tagClientTileItem & origin, const tagClientTileItem & target);
	//释放Texture;
	void CleanUpTexture();
	void TestChess(BYTE cbTestFlag);
	BOOL InitBackGround(cGraphics *pGraphics);
	//void TestActiveChess();
	//void TestChessMove();
};

//////////////////////////////////////////////////////////////////////////

#endif