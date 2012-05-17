#include "StdAfx.h"
#include "GameLogic.h"

CGameLogic::CGameLogic(void)
{

}

CGameLogic::~CGameLogic(void)
{
}


//重置棋盘和棋子
void CGameLogic::ResetChessBoard(){
	for(BYTE i = 0; i<= MAX_NORMAL_ROADWAY; i++){
		m_ServerTile[i].cbColor = ((GAME_PLAYER -1) + i) %GAME_PLAYER;
		m_ServerTile[i].cbChessCount = 0;
	}
	for(BYTE i = MAX_NORMAL_ROADWAY +1; i<= MAX_ROADWAY; i++){
		m_ServerTile[i].cbColor = (i - MAX_NORMAL_ROADWAY - 1) / UP_SKY_STEP;
		m_ServerTile[i].cbChessCount = 0;
	}
	BYTE cbTemp1, cbTemp2;
	for(BYTE i = MAX_ROADWAY + 1; i<= MAX_ROADWAY_BASE; i++){
		cbTemp1 = (i - MAX_ROADWAY - 1) / GAME_PLAYER;
		cbTemp2 = (i - MAX_ROADWAY - 1) % PLAYER_CHESS_COUNT;
		m_ServerTile[i].cbColor = cbTemp1;
		m_ServerTile[i].cbChessCount = 1;
		m_ServerChess[cbTemp1][cbTemp2].cbColor = cbTemp1;
		m_ServerChess[cbTemp1][cbTemp2].cbTileIndex = i;
		m_ServerChess[cbTemp1][cbTemp2].cbChessID = cbTemp2;
		m_ServerChess[cbTemp1][cbTemp2].cbChessStatus = CHESS_NOT_FINISHED;
	}
	for(BYTE i = MAX_ROADWAY_BASE +1; i<= MAX_VALID_DOWN; i++){
		m_ServerTile[i].cbColor = (i - MAX_ROADWAY_BASE - 1);
		m_ServerTile[i].cbChessCount = 0;
	}
}


//移动棋子
bool CGameLogic::MoveChess(BYTE cbDiceNum){
	BYTE cbCurTileID, cbUpSkySpecial =0;
	tagServerChessItem & apCurChessItem = m_ServerChess[m_CurPlayerID][m_CurChessID];
	//if(apCurChessItem.cbChessStatus == CHESS_FINISHED) return true;
	cbCurTileID = m_ServerChess[m_CurPlayerID][m_CurChessID].cbTileIndex;
	m_ServerTile[cbCurTileID].cbChessCount --;
	if(cbCurTileID >MAX_ROADWAY && cbCurTileID <= MAX_ROADWAY_BASE){
		if(cbDiceNum == MAX_RANDNUM ){
			cbCurTileID = MAX_ROADWAY_BASE + apCurChessItem.cbColor +1;
			cbDiceNum = 0;
		} else return true;
	} else if(cbCurTileID > MAX_ROADWAY_BASE) {
		cbCurTileID = apCurChessItem.cbColor * PER_NORMAL_ROADWAY;
		cbDiceNum--;

	} else {
		if(cbCurTileID ==SPECIALPULL[apCurChessItem.cbColor][1]){
			cbCurTileID = SPECIALTONEXT[apCurChessItem.cbColor][1];
		} else {
			cbCurTileID = cbCurTileID>MAX_NORMAL_ROADWAY?cbCurTileID+1:(cbCurTileID +1) %(MAX_NORMAL_ROADWAY+1);
		}
		cbDiceNum--;

	}
	if(cbCurTileID <=MAX_NORMAL_ROADWAY){
		for(BYTE i= 0; i<= cbDiceNum; i++){
			if((cbCurTileID + i)%(MAX_NORMAL_ROADWAY+1) == SPECIALPULL[apCurChessItem.cbColor][1] ){
				cbUpSkySpecial = SPECIALPULL[apCurChessItem.cbColor][1];
				break;
			}
		}
	}
	if(cbUpSkySpecial){ 
		cbCurTileID = SPECIALTONEXT[apCurChessItem.cbColor][1]+(cbDiceNum -1 - cbUpSkySpecial+cbCurTileID);
		if(cbCurTileID == SPECIALTONEXT[apCurChessItem.cbColor][1] -1) cbCurTileID = SPECIALPULL[apCurChessItem.cbColor][1];
	}
	else {
		if(cbCurTileID <=MAX_NORMAL_ROADWAY){
			cbCurTileID = (cbCurTileID+cbDiceNum) %(MAX_NORMAL_ROADWAY+1);
			if(m_ServerTile[cbCurTileID].cbChessCount > 0){
				BeKilledHandle(cbCurTileID, m_ServerTile[cbCurTileID].cbChessCount, apCurChessItem.cbColor);
			}
			if(cbCurTileID == SPECIALPULL[apCurChessItem.cbColor][0]){
				cbCurTileID =SPECIALTONEXT[apCurChessItem.cbColor][0];
				if(m_ServerTile[cbCurTileID].cbChessCount > 0){
					BeKilledHandle(cbCurTileID, m_ServerTile[cbCurTileID].cbChessCount, apCurChessItem.cbColor);
				}
				if(m_ServerTile[SPECIALTONEXT[apCurChessItem.cbColor][2]].cbChessCount > 0){
					BeKilledHandle(SPECIALTONEXT[apCurChessItem.cbColor][2], m_ServerTile[SPECIALTONEXT[apCurChessItem.cbColor][2]].cbChessCount, apCurChessItem.cbColor);
				}
			}
			if(m_ServerTile[cbCurTileID].cbColor == apCurChessItem.cbColor){
				cbCurTileID = (cbCurTileID+ 4)%(MAX_NORMAL_ROADWAY+1);
				if(m_ServerTile[cbCurTileID].cbChessCount > 0){
					BeKilledHandle(cbCurTileID, m_ServerTile[cbCurTileID].cbChessCount, apCurChessItem.cbColor);
				}
			}
			if(cbCurTileID == SPECIALPULL[apCurChessItem.cbColor][0]){
				cbCurTileID = SPECIALTONEXT[apCurChessItem.cbColor][0];
				if(m_ServerTile[cbCurTileID].cbChessCount > 0){
					BeKilledHandle(cbCurTileID, m_ServerTile[cbCurTileID].cbChessCount, apCurChessItem.cbColor);
				}
				if(m_ServerTile[SPECIALTONEXT[apCurChessItem.cbColor][2]].cbChessCount > 0){
					BeKilledHandle(SPECIALTONEXT[apCurChessItem.cbColor][2], m_ServerTile[SPECIALTONEXT[apCurChessItem.cbColor][2]].cbChessCount, apCurChessItem.cbColor);
				}
			}
		} else if(cbCurTileID <= MAX_ROADWAY){
			if(cbCurTileID+cbDiceNum > SPECIALTONEXT[apCurChessItem.cbColor][1] +UP_SKY_STEP-1){
				cbCurTileID = 2*SPECIALTONEXT[apCurChessItem.cbColor][1] + 2*UP_SKY_STEP-2 - cbDiceNum  -cbCurTileID; 
			} else {
				cbCurTileID = cbCurTileID+cbDiceNum;
			}
		}

	}
	if(cbCurTileID ==SPECIALTONEXT[apCurChessItem.cbColor][1] + UP_SKY_STEP-1){
		apCurChessItem.cbChessStatus = CHESS_FINISHED;
		cbCurTileID = MAX_ROADWAY + apCurChessItem.cbColor * PLAYER_CHESS_COUNT + apCurChessItem.cbChessID +1;
	}
	apCurChessItem.cbTileIndex = cbCurTileID;
	m_ServerTile[cbCurTileID].cbChessCount ++;
	return true;

}

void CGameLogic::BeKilledHandle(BYTE cbTileID, BYTE &cbChessCount, BYTE cbColor){
	BYTE cbSameChessCount = cbChessCount;
	for(BYTE i= 0; i<GAME_PLAYER; i ++){
		for(BYTE j= 0; j < PLAYER_CHESS_COUNT; j++){
			if(m_ServerChess[i][j].cbTileIndex == cbTileID){
				if(m_ServerChess[i][j].cbColor != cbColor){
				m_ServerChess[i][j].cbTileIndex = MAX_ROADWAY + m_ServerChess[i][j].cbColor * PLAYER_CHESS_COUNT + m_ServerChess[i][j].cbChessID +1;
				m_ServerTile[m_ServerChess[i][j].cbTileIndex].cbChessCount ++;
				cbChessCount --;
				if(!cbChessCount) break;
				} else {
					cbSameChessCount--;
					if(!cbSameChessCount) return;
				}
			}
		}
		if(!cbChessCount) break;
	}
	ASSERT(cbChessCount == 0);
}


//重置棋盘和棋子
void CGameLogic::TestResetChessBoard(BYTE cbTestFlag){
	BYTE cbTemp1, cbTemp2;
	switch(cbTestFlag)
	{
	case 1:
		{
			for(BYTE i = 0; i<= MAX_NORMAL_ROADWAY; i++){
				m_ServerTile[i].cbColor = ((GAME_PLAYER -1) + i) %GAME_PLAYER;
				m_ServerTile[i].cbChessCount = 0;
			}
			for(BYTE i = MAX_NORMAL_ROADWAY +1; i<= MAX_ROADWAY; i++){
				cbTemp1 = (i - MAX_NORMAL_ROADWAY - 1) / UP_SKY_STEP;
				cbTemp2 = (i - MAX_NORMAL_ROADWAY - 1) % UP_SKY_STEP;
				m_ServerTile[i].cbColor = cbTemp1;

				if(cbTemp2 < PLAYER_CHESS_COUNT) {
					m_ServerChess[cbTemp1][cbTemp2].cbColor = cbTemp1;
					m_ServerChess[cbTemp1][cbTemp2].cbTileIndex = i;
					m_ServerChess[cbTemp1][cbTemp2].cbChessID = cbTemp2;
					m_ServerChess[cbTemp1][cbTemp2].cbChessStatus = CHESS_NOT_FINISHED;
					m_ServerTile[i].cbChessCount = 1;
				} else m_ServerTile[i].cbChessCount = 0;
			}

			for(BYTE i = MAX_ROADWAY + 1; i<= MAX_ROADWAY_BASE; i++){
				m_ServerTile[i].cbColor = cbTemp1;
				m_ServerTile[i].cbChessCount = 1;
			}
			for(BYTE i = MAX_ROADWAY_BASE +1; i<= MAX_VALID_DOWN; i++){
				m_ServerTile[i].cbColor = (i - MAX_ROADWAY_BASE - 1);
				m_ServerTile[i].cbChessCount = 0;
			}
			break;
		}
	case 2:
		{
			for(BYTE i = 0; i<= MAX_NORMAL_ROADWAY; i++){
				m_ServerTile[i].cbColor = ((GAME_PLAYER -1) + i) %GAME_PLAYER;
				m_ServerTile[i].cbChessCount = 0;
			}
			for(BYTE i = MAX_NORMAL_ROADWAY +1; i<= MAX_ROADWAY; i++){
				cbTemp1 = (i - MAX_NORMAL_ROADWAY - 1) / UP_SKY_STEP;
				cbTemp2 = (i - MAX_NORMAL_ROADWAY - 1) % UP_SKY_STEP;
				m_ServerTile[i].cbColor = cbTemp1;

				if(cbTemp2 == PLAYER_CHESS_COUNT -1) {
					m_ServerChess[cbTemp1][cbTemp2].cbColor = cbTemp1;
					m_ServerChess[cbTemp1][cbTemp2].cbTileIndex = i;
					m_ServerChess[cbTemp1][cbTemp2].cbChessID = cbTemp2;
					m_ServerChess[cbTemp1][cbTemp2].cbChessStatus = CHESS_NOT_FINISHED;
					m_ServerTile[i].cbChessCount = 1;
				} else m_ServerTile[i].cbChessCount = 0;
			}

			for(BYTE i = MAX_ROADWAY + 1; i<= MAX_ROADWAY_BASE; i++){
				cbTemp1 = (i - MAX_ROADWAY - 1) / GAME_PLAYER;
				cbTemp2 = (i - MAX_ROADWAY - 1) % PLAYER_CHESS_COUNT;
				m_ServerTile[i].cbColor = cbTemp1;
				if(cbTemp2 < PLAYER_CHESS_COUNT -1){
					m_ServerTile[i].cbChessCount = 1;
					m_ServerChess[cbTemp1][cbTemp2].cbColor = cbTemp1;
					m_ServerChess[cbTemp1][cbTemp2].cbTileIndex = i;
					m_ServerChess[cbTemp1][cbTemp2].cbChessID = cbTemp2;
					m_ServerChess[cbTemp1][cbTemp2].cbChessStatus = CHESS_FINISHED;
				} else m_ServerTile[i].cbChessCount = 0;
			}
			for(BYTE i = MAX_ROADWAY_BASE +1; i<= MAX_VALID_DOWN; i++){
				m_ServerTile[i].cbColor = (i - MAX_ROADWAY_BASE - 1);
				m_ServerTile[i].cbChessCount = 0;
			}
			break;
		}
	case 3:
		{
			for(BYTE i = 0; i<= MAX_NORMAL_ROADWAY; i++){
				m_ServerTile[i].cbColor = ((GAME_PLAYER -1) + i) %GAME_PLAYER;
				m_ServerTile[i].cbChessCount = 0;
			}
			for(BYTE i = MAX_NORMAL_ROADWAY +1; i<= MAX_ROADWAY; i++){
				cbTemp1 = (i - MAX_NORMAL_ROADWAY - 1) / UP_SKY_STEP;
				cbTemp2 = (i - MAX_NORMAL_ROADWAY - 1) % UP_SKY_STEP;
				m_ServerTile[i].cbColor = cbTemp1;
				m_ServerTile[i].cbChessCount = 0;
			}

			for(BYTE i = MAX_ROADWAY + 1; i<= MAX_ROADWAY_BASE; i++){
				cbTemp1 = (i - MAX_ROADWAY - 1) / GAME_PLAYER;
				cbTemp2 = (i - MAX_ROADWAY - 1) % PLAYER_CHESS_COUNT;
				m_ServerTile[i].cbColor = cbTemp1;
				m_ServerTile[i].cbChessCount = 0;
			}
			for(BYTE i = MAX_ROADWAY_BASE +1; i<= MAX_VALID_DOWN; i++){
				m_ServerTile[i].cbColor = (i - MAX_ROADWAY_BASE - 1);
				m_ServerTile[i].cbChessCount = 0;
			}

			for(BYTE i = 0; i< GAME_PLAYER; i ++){
				switch(i)
				{
				case 0:
					{
						BYTE j;
						for(/*BYTE*/ j= 0; j< PLAYER_CHESS_COUNT -1; j++){
							m_ServerChess[i][j].cbColor = i;
							m_ServerChess[i][j].cbTileIndex = MAX_ROADWAY + i*PLAYER_CHESS_COUNT +j +1;
							m_ServerTile[m_ServerChess[i][j].cbTileIndex].cbChessCount = 1;
							m_ServerChess[i][j].cbChessID = j;
							m_ServerChess[i][j].cbChessStatus = CHESS_NOT_FINISHED;
						}
						j = PLAYER_CHESS_COUNT -1;
						m_ServerChess[i][j].cbColor = i;
						m_ServerChess[i][j].cbTileIndex = 16;
						m_ServerTile[m_ServerChess[i][j].cbTileIndex].cbChessCount = 1;
						m_ServerChess[i][j].cbChessID = j;
						m_ServerChess[i][j].cbChessStatus = CHESS_NOT_FINISHED;
						
						break;
					}
				case 2:
					{
						BYTE aTemp[PLAYER_CHESS_COUNT] = {29, 66, 17, 33};
						for(BYTE j= 0; j< PLAYER_CHESS_COUNT; j++){
							m_ServerChess[i][j].cbColor = i;
							m_ServerChess[i][j].cbTileIndex = aTemp[j];
							m_ServerTile[m_ServerChess[i][j].cbTileIndex].cbChessCount = 1;
							m_ServerChess[i][j].cbChessID = j;
							m_ServerChess[i][j].cbChessStatus = CHESS_NOT_FINISHED;
						}
					break;
					   }
				case 1:{}
				case 3:
					{
						for(BYTE j= 0; j< PLAYER_CHESS_COUNT; j++){
							m_ServerChess[i][j].cbColor = i;
							m_ServerChess[i][j].cbTileIndex = MAX_ROADWAY + i*PLAYER_CHESS_COUNT +j +1;
							m_ServerTile[m_ServerChess[i][j].cbTileIndex].cbChessCount = 1;
							m_ServerChess[i][j].cbChessID = j;
							m_ServerChess[i][j].cbChessStatus = CHESS_NOT_FINISHED;
						}
					 
					break;
					  }
				}
			}
			break;
		}
	}
	/*for( BYTE i = 0; i< GAME_PLAYER; i++){
		for( BYTE j = 0; j< PLAYER_CHESS_COUNT; j++){
			RetailOutput("\nm_ServerChess[%d][%d].cbTileIndex = %d, cbChessStatus = %d, cbColor = %d", i, j, m_ServerChess[i][j].cbTileIndex,  m_ServerChess[i][j].cbChessStatus, m_ServerChess[i][j].cbColor);
		}
	}*/
}

