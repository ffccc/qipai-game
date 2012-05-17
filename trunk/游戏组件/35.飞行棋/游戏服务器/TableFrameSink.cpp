#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ



//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	memset(m_bPlayStatus,US_FREE,sizeof(m_bPlayStatus));
	InitVariable();

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//���ò���
	//m_pITableFrame->SetLessTimes(200L);

	return true;
}

//��ʼ������
void CTableFrameSink::InitVariable()
{
	//RetailOutput("\nm_bPlayStatus[0] = %d, [1] = %d, [2] = %d, [3] = %d", m_bPlayStatus[0], m_bPlayStatus[1], m_bPlayStatus[2], m_bPlayStatus[3]);
	
	//RetailOutput("\nm_bPlayStatus[0] = %d, [1] = %d, [2] = %d, [3] = %d", m_bPlayStatus[0], m_bPlayStatus[1], m_bPlayStatus[2], m_bPlayStatus[3]);
	m_wTotalScore =  0;
	//m_GameLogic.TestResetChessBoard(3);
	m_GameLogic.ResetChessBoard();

}


//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	InitVariable();
	//RetailOutput("\nIn RepositTableFrameSink");
	return;
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//���Ϳ�ʼ
	CMD_S_GameStart GameStartData;
	BYTE cbPlayerCount =0;
	for(int i= 0; i< GAME_PLAYER; i++){
		if(m_pITableFrame->GetServerUserItem(i) != NULL){
			cbPlayerCount ++;
			m_bPlayStatus[i] = US_PLAY;
		}
	}

	m_wTotalScore = (cbPlayerCount-1) * 200;
	srand(GetTickCount());
	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);
	//m_GameLogic.ResetChessBoard();
	//m_GameLogic.ResetChessBoard();
	memset(&GameStartData, 0, sizeof(GameStartData));

	m_GameLogic.m_CurPlayerID = 255;
	while(m_GameLogic.m_CurPlayerID == 255){
		GameStartData.wCurrentUser = rand() %GAME_PLAYER;
		if(m_bPlayStatus[GameStartData.wCurrentUser] == US_PLAY){
			m_GameLogic.m_CurPlayerID = GameStartData.wCurrentUser;
		}
	}
	m_GameStatus = GAME_WAIT_DICE;
	memset(m_aAutomatism, NOT_AUTOMATISM, sizeof(m_aAutomatism));
	dwStepTime = GetTickCount();
	//RetailOutput("\nOnEventGameStart m_GameLogic.m_CurPlayerID = %d", m_GameLogic.m_CurPlayerID);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStartData,sizeof(GameStartData));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStartData,sizeof(GameStartData));



	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
	case GER_NO_PLAYER:		//û�����
		{
			//��������
			CMD_S_GameEnd GameEndData;
			memset(&GameEndData,0,sizeof(GameEndData));

			//ʤ�����
			GameEndData.wWinUser=wChairID;

			//д���ж�
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for(int i =0; i<GAME_PLAYER; i++){
				if(m_bPlayStatus[i] == US_PLAY)
				{
					if(i==wChairID)
					{
						ScoreInfo[i].lScore = GameEndData.lUserScore[i] = m_wTotalScore;
						ScoreInfo[i].ScoreKind = enScoreKind_Win;
					} else {
						ScoreInfo[i].lScore = GameEndData.lUserScore[i] = -200;
						ScoreInfo[i].ScoreKind = enScoreKind_Lost;
					}
					m_bPlayStatus[i] = US_SIT;
				}

			}

			/*for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEndData.lUserScore[i]=m_pITableFrame->AccountGameGold(GameEndData.lUserScore[i]);
			}*/
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndData,sizeof(GameEndData));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEndData,sizeof(GameEndData));
//			m_pITableFrame->WriteTableScore(ScoreInfo,GAME_PLAYER,0L);
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//ǿ�˴���
			OnUserGiveUp(wChairID);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_FREE:		//����״̬
		{
			CMD_S_StatusFree StatusFreeData;
			//RetailOutput("\nSendGameScene m_bPlayStatus[0] = %d, [1] = %d, [2] = %d, [3] = %d", m_bPlayStatus[0], m_bPlayStatus[1], m_bPlayStatus[2], m_bPlayStatus[3]);
			memcpy(StatusFreeData.cbPlayStatus, m_bPlayStatus, sizeof(m_bPlayStatus));
			//RetailOutput("\nSendGameScene StatusFreeData.cbPlayStatus[0] = %d, [1] = %d, [2] = %d, [3] = %d", StatusFreeData.cbPlayStatus[0], StatusFreeData.cbPlayStatus[1], StatusFreeData.cbPlayStatus[2], StatusFreeData.cbPlayStatus[3]);
			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFreeData,sizeof(StatusFreeData));
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			CMD_S_StatusPlay StatusPlayData;
			memcpy(StatusPlayData.cbPlayStatus, m_bPlayStatus, sizeof(m_bPlayStatus));
			memcpy(StatusPlayData.aServerChess, m_GameLogic.m_ServerChess, sizeof(m_GameLogic.m_ServerChess));
			if(pIServerUserItem->GetUserStatus() != US_LOOKON)m_aAutomatism[pIServerUserItem->GetChairID()] = NOT_AUTOMATISM;
			memcpy(StatusPlayData.cbAutomatism, m_aAutomatism, sizeof(m_aAutomatism));
			StatusPlayData.wCurrentUser = m_GameLogic.m_CurPlayerID;
			StatusPlayData.cbDiceNum = m_DiceRandNum;
			StatusPlayData.cbGameStatus = m_GameStatus;
			StatusPlayData.lLimitTimeCount = LIMIT_TIME - (GetTickCount() - dwStepTime);
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlayData,sizeof(StatusPlayData));
		}
	}
	//Ч����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

	switch (wSubCmdID)
	{
	case SUB_C_DICE_CAST:		//�ƶ�����
		{

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			return OnUserDiceCast();
		}
	case SUB_C_CHESS_CHOOSE:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ChessChoose));
			if (wDataSize!=sizeof(CMD_C_ChessChoose)) return false;
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			CMD_C_ChessChoose * pChessChoose=(CMD_C_ChessChoose *)pDataBuffer;


			return OnUserChessChoose( pChessChoose->cbChessId);
		}
	case SUB_C_TIME_OUT:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			return OnUserTimeOut(pUserData->wChairID);
		}
	case SUB_C_AUTOMATISM:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_Automatism));
			if (wDataSize!=sizeof(CMD_C_Automatism)) return false;
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			CMD_C_Automatism * pAutomatism =(CMD_C_Automatism *)pDataBuffer;

			CMD_S_UserAutomatism UserAutomatismData;
			UserAutomatismData.wAutoUser = pUserData->wChairID;
			m_aAutomatism[pUserData->wChairID] = UserAutomatismData.cbAutomatism = pAutomatism->Automatism;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_AUTOMATISM,&UserAutomatismData,sizeof(UserAutomatismData));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_AUTOMATISM,&UserAutomatismData,sizeof(UserAutomatismData));
			return true;
		}
	}
	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

bool CTableFrameSink::OnUserDiceCast()
{
	CMD_S_DiceCast DiceCastData;
	DiceCastData.cbDiceNum = 1+rand()%MAX_RANDNUM;
	//DiceCastData.cbDiceNum =DiceCastData.cbDiceNum%2?1:MAX_RANDNUM;
	m_DiceRandNum = DiceCastData.cbDiceNum;
	m_GameStatus = GAME_WAIT_CHOOSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_DICE_CAST,&DiceCastData,sizeof(DiceCastData));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_DICE_CAST,&DiceCastData,sizeof(DiceCastData));
	return true;
}

bool CTableFrameSink::OnUserChessChoose(BYTE cbChessID)
{
	CMD_S_ChessChoose ChessChooseData;
	BYTE i, tempPlayerID;

	m_GameLogic.m_CurChessID = cbChessID;
	//RetailOutput("\nFrom:: m_CurPlayerID = %d, m_CurChessID = %d, CurcbTileIndex = %d, CurChessStatus = %d", m_GameLogic.m_CurPlayerID, m_GameLogic.m_CurChessID, m_GameLogic.m_ServerChess[m_GameLogic.m_CurPlayerID][m_GameLogic.m_CurChessID].cbTileIndex, m_GameLogic.m_ServerChess[m_GameLogic.m_CurPlayerID][m_GameLogic.m_CurChessID].cbChessStatus);
	m_GameLogic.MoveChess(m_DiceRandNum);
	//RetailOutput("\nTo:: m_CurPlayerID = %d, m_CurChessID = %d, CurcbTileIndex = %d, CurChessStatus = %d", m_GameLogic.m_CurPlayerID, m_GameLogic.m_CurChessID, m_GameLogic.m_ServerChess[m_GameLogic.m_CurPlayerID][m_GameLogic.m_CurChessID].cbTileIndex, m_GameLogic.m_ServerChess[m_GameLogic.m_CurPlayerID][m_GameLogic.m_CurChessID].cbChessStatus);
	/*for(i =0; i<= MAX_VALID_DOWN; i+=4){
		RetailOutput("\nm_ServerTile%d = %d, %d = %d, %d = %d, %d = %d", i, m_GameLogic.m_ServerTile[i].cbChessCount, i+1, m_GameLogic.m_ServerTile[i+1].cbChessCount, i+2, m_GameLogic.m_ServerTile[i+2].cbChessCount, i+3, m_GameLogic.m_ServerTile[i+3].cbChessCount);
	}*/

	ChessChooseData.cbChessID = cbChessID;
	

	if(m_DiceRandNum != MAX_RANDNUM){
		tempPlayerID = (m_GameLogic.m_CurPlayerID +1) % GAME_PLAYER;
		while(m_bPlayStatus[tempPlayerID] == US_FREE){
			tempPlayerID = (tempPlayerID +1) % GAME_PLAYER;
		}
		ChessChooseData.wCurrentUser = tempPlayerID;
	} else ChessChooseData.wCurrentUser = m_GameLogic.m_CurPlayerID;

	
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHESS_CHOOSE,&ChessChooseData,sizeof(ChessChooseData));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHESS_CHOOSE,&ChessChooseData,sizeof(ChessChooseData));

	if(CHESS_FINISHED == m_GameLogic.m_ServerChess[m_GameLogic.m_CurPlayerID][cbChessID].cbChessStatus){
		for( i = 0; i< GAME_PLAYER; i++){
			if(m_GameLogic.m_ServerChess[m_GameLogic.m_CurPlayerID][i].cbChessStatus != CHESS_FINISHED)
				break;
		}
		if(i == GAME_PLAYER) return OnEventGameEnd(m_GameLogic.m_CurPlayerID, NULL, GER_NORMAL);
	}

	if(m_DiceRandNum != MAX_RANDNUM) m_GameLogic.m_CurPlayerID = tempPlayerID;
	m_GameStatus = GAME_WAIT_DICE;
	dwStepTime = GetTickCount();
	//RetailOutput("\nm_GameLogic.m_CurPlayerID = %d", m_GameLogic.m_CurPlayerID);

	return true;
}

bool CTableFrameSink::OnUserTimeOut(WORD wChairID)
{
	CMD_S_UserAutomatism UserAutomatismData;
	UserAutomatismData.wAutoUser = wChairID;
	m_aAutomatism[wChairID] = UserAutomatismData.cbAutomatism = ALL_AI_AUTOMATISM;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIME_OUT,&UserAutomatismData,sizeof(UserAutomatismData));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIME_OUT,&UserAutomatismData,sizeof(UserAutomatismData));
	return true;
}



bool CTableFrameSink::OnUserGiveUp(WORD wSeat)
{
	
	ASSERT(m_bPlayStatus[wSeat] !=US_FREE);
	CMD_S_UserLost UserLostData;
	if(m_GameLogic.m_CurPlayerID == wSeat){
		UserLostData.wCurrentUser = (m_GameLogic.m_CurPlayerID +1) % GAME_PLAYER;
		while(m_bPlayStatus[UserLostData.wCurrentUser] == US_FREE){
			UserLostData.wCurrentUser = (UserLostData.wCurrentUser +1) % GAME_PLAYER;
		}
		m_GameLogic.m_CurPlayerID = UserLostData.wCurrentUser;
	} else UserLostData.wCurrentUser = m_GameLogic.m_CurPlayerID;
	UserLostData.wLeftUser = wSeat;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_GIVEUP,&UserLostData,sizeof(UserLostData));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_GIVEUP,&UserLostData,sizeof(UserLostData));


	//RetailOutput("\nOnEventUserStandUp wChairID = %d, m_bPlayStatus[wChairID] = %d",wChairID,m_bPlayStatus[wChairID]);	
	tagScoreInfo ScoreInfo;
	ScoreInfo.lScore =-200;
	ScoreInfo.ScoreKind =enScoreKind_Flee;
//	m_pITableFrame->WriteUserScore(wSeat,ScoreInfo);


	//�жϽ���
	BYTE cbPlayerCount=0, cbSeat;
	for (BYTE i=0;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==US_PLAY && i != wSeat){
			cbPlayerCount++;
			cbSeat = i;
		}
	}
	if(cbPlayerCount <= 1) OnEventGameEnd(cbSeat,NULL,GER_NO_PLAYER);
	else m_bPlayStatus[wSeat] =US_FREE;

	return true;
}





//�û�����
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(m_bPlayStatus[wChairID] == US_PLAY) return true;
	CMD_S_UserLeft UserLeftData;
	UserLeftData.wLeftUser = wChairID;

	
	if(!bLookonUser)m_bPlayStatus[wChairID] =US_FREE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_LEFT,&UserLeftData,sizeof(UserLeftData));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_LEFT,&UserLeftData,sizeof(UserLeftData));

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	CMD_S_UserEnter UserEnterData;
	if(!bLookonUser)m_bPlayStatus[wChairID] =US_SIT;
	UserEnterData.wCurrentUser = wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_ENTER,&UserEnterData,sizeof(UserEnterData));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_ENTER,&UserEnterData,sizeof(UserEnterData));
	//RetailOutput("\nOnEventUserSitDown m_bPlayStatus[0] = %d, [1] = %d, [2] = %d, [3] = %d", m_bPlayStatus[0], m_bPlayStatus[1], m_bPlayStatus[2], m_bPlayStatus[3]);
	return true;
}


//�û�����
bool __cdecl CTableFrameSink::OnEventUserReady(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	CMD_S_UserReady UserReadyData;
	m_bPlayStatus[wChairID] = US_READY;
	UserReadyData.wCurrentUser = wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_READY,&UserReadyData,sizeof(UserReadyData));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_READY,&UserReadyData,sizeof(UserReadyData));
	//RetailOutput("\nOnEventUserReady m_bPlayStatus[0] = %d, [1] = %d, [2] = %d, [3] = %d", m_bPlayStatus[0], m_bPlayStatus[1], m_bPlayStatus[2], m_bPlayStatus[3]);
	return true;
}
//////////////////////////////////////////////////////////////////////////
