#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "GameOption.h"


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START, OnUserStart)
	ON_MESSAGE(IDM_TIMEOUT, OnUserTimeOut)
	ON_MESSAGE(IDM_DICE_CAST,OnDiceCast)
	ON_MESSAGE(IDM_CHESS_CHOOSE, OnChessChoose)
	ON_MESSAGE(IDM_AUTOMATISM, OnUserAutomatism)
	//选择声音
	ON_MESSAGE(IDM_SOUND_MUSIC, OnPlayMusic)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	
	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{
	//设置标题
	SetWindowText(TEXT("飞行棋游戏  --  Ver：6.0.1.0"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	m_VorSplitter.EnableSplitter(false);

	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	//构造数据
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();

	//配置数据
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
	}

	return;
}

//时间消息
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	return false;
}

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}


//网络消息
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_S_GAME_START:	//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_DICE_CAST:	//游戏开始
		{
			return OnSubDiceCast(pBuffer,wDataSize);
		}
	case SUB_S_CHESS_CHOOSE:	//游戏开始
		{
			return OnSubChessChoose(pBuffer,wDataSize);
		}
	case SUB_S_USER_ENTER:	//游戏开始
		{
			return OnSubUserEnter(pBuffer,wDataSize);
		}
	case SUB_S_USER_LEFT:	//游戏开始
		{
			return OnSubUserLeft(pBuffer,wDataSize);
		}
	case SUB_S_USER_GIVEUP:
		{
			return OnSubUserGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_USER_READY:	//游戏开始
		{
			return OnSubUserReady(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:
		{
			return OnSubGameEnd(pBuffer, wDataSize);
		}
	case SUB_S_AUTOMATISM:
		{
			return OnSubUserAutomatism(pBuffer, wDataSize);
		}
	case SUB_S_TIME_OUT:
		{
			return OnSubUserTimeOut(pBuffer, wDataSize);
		}
	}

	return false;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//空闲状态
		{

			//m_GameClientView.m_GameScoreView.SetWinnerText(TEXT("AAAA"));

			//m_GameClientView.m_GameScoreView.ShowWindow(SW_SHOW);
			m_GameClientView.m_wndD3D.m_ChessBoard.SetCurMeChairID((BYTE)GetMeChairID());
			m_GameClientView.m_wndD3D.InitChessBoard();
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			memcpy(m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus, pStatusFree->cbPlayStatus, sizeof(pStatusFree->cbPlayStatus));
			m_GameClientView.m_wndD3D.m_GameStatus = GAME_WAIT_START;
		}
		const tagUserData *l_pUserData;
		for(int i =0; i < GAME_PLAYER; i++){
			if(m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[i] != US_FREE){
				l_pUserData =GetUserData(i);
				if(l_pUserData != NULL){
					m_GameClientView.m_wndD3D.m_wFaceID[i] =(l_pUserData->wFaceID >60?0:l_pUserData->wFaceID)* 32;
					memcpy(m_GameClientView.m_wndD3D.m_szName[i], l_pUserData->szName, sizeof(l_pUserData->szName));
				}
			}
		}
		return true;
	case GS_PLAYING:	//游戏状态
		{
			//m_GameClientView.m_GameScoreView.ShowWindow(SW_SHOW);
			m_GameClientView.m_wndD3D.m_ChessBoard.SetCurMeChairID((BYTE)GetMeChairID());

			m_GameClientView.m_wndD3D.InitChessBoard();

			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			m_GameClientView.m_wndD3D.m_ChessBoard.SwitchCurrentUser(pStatusPlay->wCurrentUser);
			if(!IsLookonMode() && pStatusPlay->lLimitTimeCount <=0 && m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID == m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID) AfxGetMainWnd()->SendMessage(IDM_TIMEOUT,0,0);
			else {
				m_GameClientView.m_wndD3D.m_lLimitTimeCount = pStatusPlay->lLimitTimeCount;
			}

			m_GameClientView.m_wndD3D.m_DiceRandNum = pStatusPlay->cbDiceNum;
			for(BYTE i =0; i<= MAX_VALID_DOWN; i++){
				m_GameClientView.m_wndD3D.m_ChessBoard.m_ChessBoard.GetAt(i).cbChessCount =0;
			}
			//逻辑数据
			CopyMemory(m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus,pStatusPlay->cbPlayStatus,sizeof(pStatusPlay->cbPlayStatus));
			for(BYTE i =0; i< GAME_PLAYER; i++){
				for(BYTE j= 0; j< PLAYER_CHESS_COUNT; j++){
					m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].cbChessID = pStatusPlay->aServerChess[i][j].cbChessID;
					m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].cbChessStatus = pStatusPlay->aServerChess[i][j].cbChessStatus;
					m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].cbColor = pStatusPlay->aServerChess[i][j].cbColor;
					m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].cbTileIndex = pStatusPlay->aServerChess[i][j].cbTileIndex;
					m_GameClientView.m_wndD3D.m_ChessBoard.m_ChessBoard.GetAt(pStatusPlay->aServerChess[i][j].cbTileIndex).cbChessCount ++;

					m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].fXPos = (float)(m_GameClientView.m_wndD3D.m_ChessBoard.m_ChessBoard[pStatusPlay->aServerChess[i][j].cbTileIndex].usXPos + PLANE_TILE_XOFFSET);
					m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].fYPos = (float)(m_GameClientView.m_wndD3D.m_ChessBoard.m_ChessBoard[pStatusPlay->aServerChess[i][j].cbTileIndex].usYPos + PLANE_TILE_YOFFSET);
					if( pStatusPlay->aServerChess[i][j].cbTileIndex <= MAX_NORMAL_ROADWAY){
						BYTE temp1 = 0, temp2 = 7, temp3 =17, temp4 = 2*(GAME_PLAYER - m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID +2) %8; 
						while(true){
							if(temp1 <= pStatusPlay->aServerChess[i][j].cbTileIndex && temp1 +4 >pStatusPlay->aServerChess[i][j].cbTileIndex){
								m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].usDirection = temp4 *PLANE_W;
							//	if(i==0 && j==3) Debuglog.LogOutput("\ntemp4 = %d", temp4);
								break;
							}
							if(temp2 <= pStatusPlay->aServerChess[i][j].cbTileIndex && temp2 + 6 >pStatusPlay->aServerChess[i][j].cbTileIndex){
								m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].usDirection = temp4 *PLANE_W;
							//	if(i==0 && j==3) Debuglog.LogOutput("\ntemp4 = %d", temp4);
								break;
							} else if(temp3 <= pStatusPlay->aServerChess[i][j].cbTileIndex && temp3 +3 >pStatusPlay->aServerChess[i][j].cbTileIndex){
								m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].usDirection = temp4 *PLANE_W;
							//	if(i==0 && j==3) Debuglog.LogOutput("\ntemp4 = %d", temp4);
								break;
							}
							temp1 +=13;
							temp2 +=13;
							temp3 = (temp3 +13) %(MAX_NORMAL_ROADWAY +1);
							temp4 = (temp4 +2) %8;
							ASSERT(temp2 < MAX_NORMAL_ROADWAY +1);
						}
					} else if(pStatusPlay->aServerChess[i][j].cbTileIndex > MAX_NORMAL_ROADWAY && pStatusPlay->aServerChess[i][j].cbTileIndex <=MAX_ROADWAY)
                        m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].usDirection = 2*(GAME_PLAYER +GAME_PLAYER+pStatusPlay->aServerChess[i][j].cbColor - m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID -2) %8 * PLANE_W;
					else if(pStatusPlay->aServerChess[i][j].cbTileIndex >MAX_ROADWAY && pStatusPlay->aServerChess[i][j].cbTileIndex <=MAX_ROADWAY_BASE)
						m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].usDirection = 2*(GAME_PLAYER+pStatusPlay->aServerChess[i][j].cbColor - m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID -1) %8 * PLANE_W;
					else if(pStatusPlay->aServerChess[i][j].cbTileIndex >MAX_ROADWAY_BASE)
						m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].usDirection = (2*(GAME_PLAYER+pStatusPlay->aServerChess[i][j].cbColor - m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID -1) +1) %8 * PLANE_W;
					//Debuglog.LogOutput("\ni = %d, cbChessID = %d, cbChessStatus = %d, cbColor = %d, cbTileIndex = %d, fXPos = %f, fYPos = %f, usDirection = %d", i, pStatusPlay->aServerChess[i][j].cbChessID, pStatusPlay->aServerChess[i][j].cbChessStatus, pStatusPlay->aServerChess[i][j].cbColor, pStatusPlay->aServerChess[i][j].cbTileIndex, m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].fXPos, m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].fYPos, m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[i][j].usDirection);
				}
			}

			m_GameClientView.m_wndD3D.m_GameStatus = pStatusPlay->cbGameStatus;
			if(IsLookonMode()){
				m_GameClientView.m_wndD3D.m_bIsLookonMode = true;
			} else {
				for(BYTE i =0; i< PLAYER_CHESS_COUNT; i++){
					if(m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID][i].cbTileIndex <=MAX_ROADWAY || m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID][i].cbTileIndex >MAX_ROADWAY_BASE){
						m_GameClientView.m_wndD3D.m_ChessBoard.m_CurActiveChess.Add(i);
					}
				}
			}
			const tagUserData *l_pUserData;
			for(int i =0; i < GAME_PLAYER; i++){
				if(m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[i] != US_FREE){
					l_pUserData =GetUserData(i);
					if(l_pUserData != NULL){
						m_GameClientView.m_wndD3D.m_wFaceID[i] =(l_pUserData->wFaceID >60?0:l_pUserData->wFaceID) * 32;
						memcpy(m_GameClientView.m_wndD3D.m_szName[i], l_pUserData->szName, sizeof(l_pUserData->szName));
					}
				}
			}
			memcpy(m_GameClientView.m_wndD3D.m_aAutomatism, pStatusPlay->cbAutomatism, sizeof(pStatusPlay->cbAutomatism));

			return true;
		}
	}
	return false;
}



//开始按钮
LRESULT CGameClientDlg::OnUserStart(WPARAM wParam, LPARAM lParam)
{
	SendUserReady(NULL,0);
	//m_GameClientView.OnScoreView(0,0);
	
	return true;
}

//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	//m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID = pGameStart->cbCurrentUser;
	m_GameClientView.m_wndD3D.m_DiceCastAnimate.m_cbStatus = NOT_ACTIVE;
	memset(m_GameClientView.m_wndD3D.m_aAutomatism, NOT_AUTOMATISM, sizeof(m_GameClientView.m_wndD3D.m_aAutomatism));
	if(pGameStart->wCurrentUser == GetMeChairID()){
		m_GameClientView.m_wndD3D.m_GameStatus = GAME_WAIT_DICE;
		if(m_GameClientView.m_wndD3D.m_aAutomatism[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID] == ALL_AI_AUTOMATISM) m_GameClientView.m_wndD3D.OnLButtonDown(0, CPoint(610, 610));
	} else {
		m_GameClientView.m_wndD3D.m_GameStatus = GAME_WAIT_OTHER;
	}
	m_GameClientView.m_wndD3D.m_DiceRandNum =0;
	m_GameClientView.m_wndD3D.m_ChessBoard.SwitchCurrentUser(pGameStart->wCurrentUser);
	for(BYTE i =0; i<= MAX_VALID_DOWN; i++){
		m_GameClientView.m_wndD3D.m_ChessBoard.m_ChessBoard.GetAt(i).cbChessCount =0;
	}
	m_GameClientView.m_wndD3D.m_ChessBoard.InitChess();
	m_GameClientView.m_wndD3D.m_AnimatedChessStatus = false;
	m_GameClientView.m_wndD3D.m_AnimatedDiceStatus = false;
	m_GameClientView.m_wndD3D.m_ChessBoard.m_CurActiveChess.RemoveAll();
	m_GameClientView.m_wndD3D.m_ChessBoard.m_ChessMove.RemoveAll();
	//m_GameClientView.m_wndD3D.m_ChessBoard.TestChess(3);

	return true;
}



LRESULT CGameClientDlg::OnDiceCast(WPARAM wParam, LPARAM lParam)
{
//	Debuglog.LogOutput("\nOnLButtonDown OnDiceCast GAME_WAIT_DICE");
	PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_TZBUTTON"));
	SendData(SUB_C_DICE_CAST);
	return true;
}

LRESULT CGameClientDlg::OnChessChoose(WPARAM wParam, LPARAM lParam)
{
	CMD_C_ChessChoose ChessChooseData;
	ChessChooseData.cbChessId = (BYTE)lParam;
//	Debuglog.LogOutput("\nOnLButtonDown OnChessChoose GAME_WAIT_CHOOSE");
	SendData(SUB_C_CHESS_CHOOSE, &ChessChooseData, sizeof(ChessChooseData));
	m_GameClientView.m_wndD3D.m_GameStatus = GAME_CHESS_ANIMATED;
	return true;
}

LRESULT CGameClientDlg::OnUserTimeOut(WPARAM wParam, LPARAM lParam)
{
	if(IsLookonMode()) return true;
	SendData(SUB_C_TIME_OUT);
	m_GameClientView.m_wndD3D.m_lLimitTimeCount = LIMIT_TIME;
	return true;
}

bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	m_GameClientView.m_wndD3D.m_GameStatus = GAME_WAIT_START;

	for(BYTE i=0; i< GAME_PLAYER; i++){
		ASSERT(m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[i] != US_NULL);
		if(m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[i] !=US_FREE)
			m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[i] = US_SIT;
	}

	const tagUserData *pUserData;
	for(BYTE i= 0, j = 0; i< GAME_PLAYER; i++){
		if((pUserData = GetUserData(i)) != NULL){
			if(pUserData->wChairID == pGameEnd->wWinUser)m_GameClientView.m_GameScoreView.SetWinnerText(pUserData->szName);
			m_GameClientView.m_GameScoreView.SetScoreText(j++, pUserData->szName, pGameEnd->lUserScore[i]);
		}
	}
	
	m_GameClientView.m_GameScoreView.Invalidate();
	m_GameClientView.m_GameScoreView.UpdateWindow();
	m_GameClientView.m_GameScoreView.ShowWindow(SW_SHOW);
	return true;
}

bool CGameClientDlg::OnSubDiceCast(const void * pBuffer, WORD wDataSize)
{
	if(IsLookonMode() && !m_GameClientView.m_wndD3D.m_bIsLookonMode) return true;
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_DiceCast));
	if (wDataSize!=sizeof(CMD_S_DiceCast)) return false;
	//消息处理
	CMD_S_DiceCast * pDiceCast=(CMD_S_DiceCast *)pBuffer;

	m_GameClientView.m_wndD3D.m_DiceRandNum = pDiceCast->cbDiceNum;
	//被动触发骰子动画
	if(	m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID != m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID || m_GameClientView.m_wndD3D.m_bIsLookonMode){
		PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_TZBUTTON"));
		m_GameClientView.m_wndD3D.m_DiceCastAnimate.Reset(m_GameClientView.m_wndD3D.m_ChessBoard.m_bViewSeatID[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID] == 1||m_GameClientView.m_wndD3D.m_ChessBoard.m_bViewSeatID[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID] ==2?DICE_LEFT_MOVE:DICE_RIGHT_MOVE);
		m_GameClientView.m_wndD3D.m_AnimatedDiceStatus = true;
	} else if(m_GameClientView.m_wndD3D.m_aAutomatism[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID]!= ALL_AI_AUTOMATISM && m_GameClientView.m_wndD3D.m_ChessBoard.m_CurActiveChess.GetCount() <=1){
		if(m_GameClientView.m_wndD3D.m_DiceRandNum != MAX_RANDNUM)
			m_GameClientView.m_wndD3D.m_aAutomatism[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID] = HALF_AI_AUTOMATISM;
		else {
			BYTE cbNotFinishChessCount =0;
			for(BYTE i =0; i< GAME_PLAYER; i++){
				if(m_GameClientView.m_wndD3D.m_ChessBoard.m_ClientChessItem[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID][i].cbChessStatus ==CHESS_NOT_FINISHED) cbNotFinishChessCount++;
					
			}
			if(cbNotFinishChessCount <=1)
				m_GameClientView.m_wndD3D.m_aAutomatism[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID] = HALF_AI_AUTOMATISM;
		}
	}

//	Debuglog.LogOutput("\n%s OnSubDiceCast m_DiceRandNum = %d", m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID != m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID?"别人":"自己",m_GameClientView.m_wndD3D.m_DiceRandNum);
	if(m_GameClientView.m_wndD3D.m_aAutomatism[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID] == ALL_AI_AUTOMATISM || m_GameClientView.m_wndD3D.m_bIsLookonMode){
		 m_GameClientView.m_wndD3D.m_DiceCastAnimate.m_usRandIndex_W = (pDiceCast->cbDiceNum -1) * 42;
		 m_GameClientView.m_wndD3D.m_DiceCastAnimate.m_cbStatus = NO_ANIMATION;
	} else{
		m_GameClientView.m_wndD3D.m_DiceCastAnimate.m_cbResult = pDiceCast->cbDiceNum;
		m_GameClientView.m_wndD3D.m_DiceCastAnimate.m_cbStatus = FIRST_STAGE;	
	}

//	m_GameClientView.m_wndD3D.m_GameStage = GAME_STAGE_CHOOSE;
	return true;

}
bool CGameClientDlg::OnSubChessChoose(const void * pBuffer, WORD wDataSize)
{
	if(IsLookonMode() && !m_GameClientView.m_wndD3D.m_bIsLookonMode) return true;

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChessChoose));
	if (wDataSize!=sizeof(CMD_S_ChessChoose)) return false;
	//消息处理
	CMD_S_ChessChoose * pChessChoose =(CMD_S_ChessChoose *)pBuffer;

	//Debuglog.LogOutput("\nOnSubChessChoose not change m_CurPlayerID1 = %d, 是%s", m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID, m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID != m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID?"别人":"自己");
	//被动触发飞机移动动画
	if(m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID != m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID || m_GameClientView.m_wndD3D.m_bIsLookonMode){
		m_GameClientView.m_wndD3D.m_ChessBoard.SwitchCurrentChess(pChessChoose->cbChessID);
		m_GameClientView.m_wndD3D.m_ChessBoard.ChessRouteGen(m_GameClientView.m_wndD3D.m_DiceRandNum);
		m_GameClientView.m_wndD3D.m_AnimatedChessStatus = true;
	}
	//Debuglog.LogOutput("\nOnSubChessChoose not change m_CurPlayerID2 = %d, 是%s", m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID, m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID != m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID?"别人":"自己");
	//Debuglog.LogOutput("\nOnSubChessChoose m_DiceRandNum = %d", m_GameClientView.m_wndD3D.m_DiceRandNum);
	m_GameClientView.m_wndD3D.m_ChessBoard.SwitchCurrentUser(pChessChoose->wCurrentUser);
	if(pChessChoose->wCurrentUser != m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID){
		m_GameClientView.m_wndD3D.m_GameStatus = GAME_WAIT_OTHER;
	} else {
		m_GameClientView.m_wndD3D.m_GameStatus = GAME_WAIT_DICE;
	}
	m_GameClientView.m_wndD3D.m_lLimitTimeCount = LIMIT_TIME;
//	Debuglog.LogOutput("\nOnSubChessChoose change m_CurPlayerID = %d, 是%s", m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID, m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID != m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID?"别人":"自己");
//	m_GameClientView.m_wndD3D.m_GameStage = GAME_STAGE_DICE;
	return true;
}

bool CGameClientDlg::OnSubUserLeft(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserLeft));
	if (wDataSize!=sizeof(CMD_S_UserLeft)) return false;
	//消息处理
	CMD_S_UserLeft * pUserLeft=(CMD_S_UserLeft *)pBuffer;

	m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[pUserLeft->wLeftUser] = US_FREE;
	m_GameClientView.m_wndD3D.m_aAutomatism[pUserLeft->wLeftUser] = NOT_AUTOMATISM;
	return true;
}

bool CGameClientDlg::OnSubUserGiveUp(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserLost));
	if (wDataSize!=sizeof(CMD_S_UserLost)) return false;
	//消息处理
	CMD_S_UserLost * pUserLost = (CMD_S_UserLost *)pBuffer;
//	Debuglog.LogOutput("\nOnSubUserGiveUp wLeftUser = %d, wCurrentUser = %d, m_CurPlayerID = %d, m_CurMeChairID = %d", pUserLost->wLeftUser, pUserLost->wCurrentUser, m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID, m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID );
	if(pUserLost->wLeftUser == m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID){
		m_GameClientView.m_wndD3D.m_ChessBoard.m_CurPlayerID = pUserLost->wCurrentUser;
		if(pUserLost->wCurrentUser == m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID){
			m_GameClientView.m_wndD3D.m_GameStatus = GAME_WAIT_DICE;
			if(m_GameClientView.m_wndD3D.m_aAutomatism[m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID] == ALL_AI_AUTOMATISM) m_GameClientView.m_wndD3D.OnLButtonDown(0, CPoint(610, 610));
		} 

	}
	m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[pUserLost->wLeftUser] = US_FREE;
	return true;
}

bool CGameClientDlg::OnSubUserEnter(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserEnter));
	if (wDataSize!=sizeof(CMD_S_UserEnter)) return false;
	//消息处理
	CMD_S_UserEnter * pUserEnter=(CMD_S_UserEnter *)pBuffer;

	m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[pUserEnter->wCurrentUser] = US_SIT;
	const tagUserData *l_pUserData =GetUserData(pUserEnter->wCurrentUser);
	if(l_pUserData != NULL){
		m_GameClientView.m_wndD3D.m_wFaceID[pUserEnter->wCurrentUser] =(l_pUserData->wFaceID >60?0:l_pUserData->wFaceID) * 32;
		memcpy(m_GameClientView.m_wndD3D.m_szName[pUserEnter->wCurrentUser], l_pUserData->szName, sizeof(l_pUserData->szName));
	}
	return true;
}

bool CGameClientDlg::OnSubUserReady(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserReady));
	if (wDataSize!=sizeof(CMD_S_UserReady)) return false;
	//消息处理
	CMD_S_UserReady * pUserReady=(CMD_S_UserReady *)pBuffer;

	m_GameClientView.m_wndD3D.m_ChessBoard.m_bPlayStatus[pUserReady->wCurrentUser] = US_READY;
	return true;
}

bool CGameClientDlg::OnSubUserAutomatism(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserAutomatism));
	if (wDataSize!=sizeof(CMD_S_UserAutomatism)) return false;
	//消息处理
	CMD_S_UserAutomatism * pUserAutomatism =(CMD_S_UserAutomatism *)pBuffer;

	if(m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID != pUserAutomatism->wAutoUser || m_GameClientView.m_wndD3D.m_bIsLookonMode)
		m_GameClientView.m_wndD3D.m_aAutomatism[pUserAutomatism->wAutoUser] =pUserAutomatism->cbAutomatism;
	return true;
}

LRESULT CGameClientDlg::OnUserAutomatism(WPARAM wParam, LPARAM lParam)
{
	CMD_C_Automatism AutomatismData;
	AutomatismData.Automatism = (BYTE)lParam;

	//Debuglog.LogOutput("\nOnLButtonDown OnChessChoose GAME_WAIT_CHOOSE");
	SendData(SUB_C_AUTOMATISM, &AutomatismData, sizeof(AutomatismData));
	return true;

}

bool CGameClientDlg::OnSubUserTimeOut(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserAutomatism));
	if (wDataSize!=sizeof(CMD_S_UserAutomatism)) return false;
	//消息处理
	CMD_S_UserAutomatism * pUserAutomatism =(CMD_S_UserAutomatism *)pBuffer;

	if(m_GameClientView.m_wndD3D.m_ChessBoard.m_CurMeChairID != pUserAutomatism->wAutoUser)
		m_GameClientView.m_wndD3D.m_aAutomatism[pUserAutomatism->wAutoUser] =pUserAutomatism->cbAutomatism;
	else {
		m_GameClientView.m_wndD3D.m_aAutomatism[pUserAutomatism->wAutoUser] =NOT_AUTOMATISM;
		m_GameClientView.m_wndD3D.OnLButtonDown(0, CPoint(650, 600));
	}
	
	return true;
}


//选择声音消息
LRESULT CGameClientDlg::OnPlayMusic(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDM_SOUND_TZBUTTON:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_TZBUTTON"));
		}
		return true;
	case IDM_SOUND_ARRIVE:
		{	
			PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_ARRIVE"));
		}
		return true;
	case IDM_SOUND_CRASH:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_CRASH"));
		}
		return true;
	case IDM_SOUND_LANDOFF:
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("WAVE_LANDOFF"));
		}
		return true;
	}
	
	return true;
}
//////////////////////////////////////////////////////////////////////////
