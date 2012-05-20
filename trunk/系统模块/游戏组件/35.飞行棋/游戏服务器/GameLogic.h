#pragma once

#include "StdAfx.h"




class CGameLogic
{
	friend class CTableFrameSink;
protected:
	tagServerChessItem  m_ServerChess[GAME_PLAYER][PLAYER_CHESS_COUNT];
	tagServerTileItem	m_ServerTile[MAX_VALID_DOWN +1];
	WORD m_CurPlayerID;
	BYTE m_CurChessID;
public:
	CGameLogic(void);
	~CGameLogic(void);
	//重置棋盘和棋子
	void ResetChessBoard();
	//移动棋子
	bool MoveChess(BYTE cbDiceNum);
	//产生吃子的路径
	void BeKilledHandle(BYTE cbTileID, BYTE &cbChessCount, BYTE cbColor);
	//重置棋盘和棋子测试
	void TestResetChessBoard(BYTE cbTestFlag);
};
