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
	//�������̺�����
	void ResetChessBoard();
	//�ƶ�����
	bool MoveChess(BYTE cbDiceNum);
	//�������ӵ�·��
	void BeKilledHandle(BYTE cbTileID, BYTE &cbChessCount, BYTE cbColor);
	//�������̺����Ӳ���
	void TestResetChessBoard(BYTE cbTestFlag);
};
