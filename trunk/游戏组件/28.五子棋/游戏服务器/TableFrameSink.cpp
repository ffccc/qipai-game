#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define GAME_CLOCK					600										//��Ϸ��ʱ

//////////////////////////////////////////////////////////////////////////
//��̬����

const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_bRestrict=true;
	m_bTradeUser=true;
	m_bDoubleChess=true;

	//ʱ�����
	m_dwLastClock=0L;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));

	//�û�״̬
	m_wBlackUser=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_cbBegStatus,0,sizeof(m_cbBegStatus));
	memset(m_cbPeaceCount,0,sizeof(m_cbPeaceCount));
	memset(m_cbRegretCount,0,sizeof(m_cbRegretCount));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

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

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//ʱ�����
	m_dwLastClock=0L;
	memset(m_wLeftClock,0,sizeof(m_wLeftClock));

	//�û�״̬
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_cbBegStatus,0,sizeof(m_cbBegStatus));
	memset(m_cbPeaceCount,0,sizeof(m_cbPeaceCount));
	memset(m_cbRegretCount,0,sizeof(m_cbRegretCount));

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
	//���ñ���
	m_wCurrentUser=m_wBlackUser;
	m_dwLastClock=(DWORD)time(NULL);
	m_wLeftClock[0]=m_wLeftClock[1]=GAME_CLOCK;

	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//��������
	m_GameLogic.InitGameLogic();

	//���Ϳ�ʼ
	CMD_S_GameStart GameStart;
	GameStart.wGameClock=GAME_CLOCK;
	GameStart.wBlackUser=m_wBlackUser;
	GameStart.cbRestrict=m_bRestrict?TRUE:FALSE;
	GameStart.cbTradeUser=m_bTradeUser?TRUE:FALSE;
	GameStart.cbDoubleChess=m_bDoubleChess?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//ʤ�����
			GameEnd.wWinUser=wChairID;

			//����ֲ�
			LONG lUserScore[2];
			IServerUserItem * pIServerUserItem1=m_pITableFrame->GetServerUserItem(0);
			IServerUserItem * pIServerUserItem2=m_pITableFrame->GetServerUserItem(1);
			lUserScore[0]=pIServerUserItem1->GetUserData()->UserScoreInfo.lScore;
			lUserScore[1]=pIServerUserItem2->GetUserData()->UserScoreInfo.lScore;
			LONG lScoreMargin=(lUserScore[0]>lUserScore[1])?(lUserScore[0]-lUserScore[1]):(lUserScore[1]-lUserScore[0]);

			//�������
			LONG lScore=0;
			enScoreKind ScoreKind;
			if (wChairID!=INVALID_CHAIR)
			{
				//Ӯ�����
				WORD wWinUser=wChairID;
				WORD wLostUser=(wChairID+1)%GAME_PLAYER;
				if (lScoreMargin<100L)
				{
					if (lUserScore[wWinUser]>=lUserScore[wLostUser])
					{
						GameEnd.lUserScore[wWinUser]=10L-lScoreMargin/10L;
						GameEnd.lUserScore[wLostUser]=-GameEnd.lUserScore[wWinUser];
					}
					else
					{
						GameEnd.lUserScore[wWinUser]=10L+lScoreMargin/10L;
						GameEnd.lUserScore[wLostUser]=-GameEnd.lUserScore[wWinUser];
					}
				}
				else if (lScoreMargin<200L)
				{
					GameEnd.lUserScore[wWinUser]=1L;
					GameEnd.lUserScore[wLostUser]=-1L;
				}
				else
				{
					GameEnd.lUserScore[0]=0L;
					GameEnd.lUserScore[1]=0L;
				}

				//д�����
				lScore=GameEnd.lUserScore[wWinUser];
				ScoreKind=enScoreKind_Win;
				m_pITableFrame->WriteUserScore(wWinUser,lScore,0,ScoreKind);

				lScore=GameEnd.lUserScore[wLostUser];
				ScoreKind=enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(wLostUser,lScore,0,ScoreKind);
			}
			else	
			{
				//�������
				if (lScoreMargin<100L)
				{
					GameEnd.lUserScore[0]=(lUserScore[1]-lUserScore[0])/10L;
					GameEnd.lUserScore[1]=-GameEnd.lUserScore[0];
				}
				else if (lScoreMargin<200L)
				{
					GameEnd.lUserScore[0]=(lUserScore[1]-lUserScore[0])?1L:-1L;
					GameEnd.lUserScore[1]=-GameEnd.lUserScore[0];
				}
				else
				{
					GameEnd.lUserScore[0]=0L;
					GameEnd.lUserScore[1]=0L;
				}

				//д�����
				lScore=GameEnd.lUserScore[0];
				ScoreKind=enScoreKind_Draw;
				m_pITableFrame->WriteUserScore((WORD)0,lScore,0,ScoreKind);

				lScore=GameEnd.lUserScore[1];
				ScoreKind=enScoreKind_Draw;
				m_pITableFrame->WriteUserScore((WORD)1,lScore,0,ScoreKind);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�л��û�
			m_wBlackUser=(m_wBlackUser+1)%GAME_PLAYER;

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//���Ļ���
			LONG lScore=0;
			enScoreKind ScoreKind;
			lScore=-30L;
			ScoreKind=enScoreKind_Flee;
			m_pITableFrame->WriteUserScore(wChairID,lScore,0,ScoreKind);

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.wWinUser=INVALID_CHAIR;
			GameEnd.lUserScore[wChairID]=lScore;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�л��û�
			m_wBlackUser=(m_wBlackUser+1)%GAME_PLAYER;

			return true;
		}
	}

	//Ч����
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStation, bool bSendSecret)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
			StatusFree.wBlackUser=m_wBlackUser;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//��������
			WORD wBufferSize=0;
			BYTE cbBuffer[4096];
			for (INT_PTR i=0;i<m_GameLogic.m_ChessManualInfo.GetCount();i++)
			{
				if ((wBufferSize+sizeof(tagChessManual))>sizeof(cbBuffer))
				{
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_CHESS_MANUAL,cbBuffer,wBufferSize);
					wBufferSize=0;
				}
				CopyMemory(cbBuffer+wBufferSize,&m_GameLogic.m_ChessManualInfo[i],sizeof(tagChessManual));
				wBufferSize+=sizeof(tagChessManual);
			}
			if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_CHESS_MANUAL,cbBuffer,wBufferSize);

			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wGameClock=GAME_CLOCK;
			StatusPlay.wBlackUser=m_wBlackUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.cbRestrict=m_bRestrict?TRUE:FALSE;
			StatusPlay.cbTradeUser=m_bTradeUser?TRUE:FALSE;
			StatusPlay.cbDoubleChess=m_bDoubleChess?TRUE:FALSE;
			CopyMemory(StatusPlay.cbBegStatus,m_cbBegStatus,sizeof(m_cbBegStatus));

			//ʱ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_wCurrentUser==i)
				{
					WORD wUserClock=(WORD)((DWORD)time(NULL)-m_dwLastClock);
					if (m_wLeftClock[i]>wUserClock) StatusPlay.wLeftClock[i]=m_wLeftClock[i]-wUserClock;
				}
				else StatusPlay.wLeftClock[i]=m_wLeftClock[i];
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

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
	case SUB_C_PLACE_CHESS:		//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceChess));
			if (wDataSize!=sizeof(CMD_C_PlaceChess)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_PlaceChess * pPlaceChess=(CMD_C_PlaceChess *)pDataBuffer;
			return OnUserPlaceChess(pUserData->wChairID,pPlaceChess->cbXPos,pPlaceChess->cbYPos);
		}
	case SUB_C_REGRET_REQ:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserRegretReq(pUserData->wChairID);
		}
	case SUB_C_REGRET_ANSWER:	//����Ӧ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_RegretAnswer));
			if (wDataSize!=sizeof(CMD_C_RegretAnswer)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_RegretAnswer * pRegretAnswer=(CMD_C_RegretAnswer *)pDataBuffer;
			return OnUserRegretAnswer(pUserData->wChairID,pRegretAnswer->cbApprove);
		}
	case SUB_C_PEACE_REQ:		//�������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserPeaceReq(pUserData->wChairID);
		}
	case SUB_C_PEACE_ANSWER:	//���Ӧ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PeaceAnswer));
			if (wDataSize!=sizeof(CMD_C_PeaceAnswer)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_PeaceAnswer * pPeaceAnswer=(CMD_C_PeaceAnswer *)pDataBuffer;
			return OnUserPeaceAnswer(pUserData->wChairID,pPeaceAnswer->cbApprove);
		}
	case SUB_C_GIVEUP_REQ:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserGiveUp(pUserData->wChairID);
		}
	case SUB_C_TRADE_REQ:		//�Ի�����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserTradeReq(pUserData->wChairID);
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnUserPlaceChess(WORD wChairID, BYTE cbXPos, BYTE cbYPos)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//Ч���û�
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	//��������
	BYTE cbColor=(wChairID==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
	if (m_GameLogic.PlaceChess(cbXPos,cbYPos,cbColor)==0) return false;

	//����ʱ��
	DWORD dwNowClock=(DWORD)time(NULL);
	WORD wUseClock=(WORD)(dwNowClock-m_dwLastClock);
	m_dwLastClock=dwNowClock;
	m_wLeftClock[m_wCurrentUser]-=__min(wUseClock,m_wLeftClock[m_wCurrentUser]);

	//�����ж�
	bool bGameEnd=false;
	WORD wWinUser=INVALID_CHAIR;
	if (m_GameLogic.WinEstimate(cbXPos,cbYPos)==true)
	{
		bGameEnd=true;
		wWinUser=wChairID;
	}
	else if ((m_GameLogic.GetStepCount(BLACK_CHESS)+m_GameLogic.GetStepCount(WHITE_CHESS))==225)
	{
		bGameEnd=true;
		wWinUser=INVALID_CHAIR;
	}
	else m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;

	//��������
	CMD_S_PlaceChess PlaceChess;
	PlaceChess.cbXPos=cbXPos;
	PlaceChess.cbYPos=cbYPos;
	PlaceChess.wPlaceUser=wChairID;
	PlaceChess.wCurrentUser=m_wCurrentUser;
	CopyMemory(PlaceChess.wLeftClock,m_wLeftClock,sizeof(PlaceChess.wLeftClock));

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_CHESS,&PlaceChess,sizeof(PlaceChess));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_CHESS,&PlaceChess,sizeof(PlaceChess));

	//������Ϸ
	if (bGameEnd==true) OnEventGameEnd(wWinUser,NULL,GER_NORMAL);

	return true;
}

//��������
bool CTableFrameSink::OnUserRegretReq(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//״̬�ж�
	if ((m_cbBegStatus[wChairID]&GU_WAIT_REGRET)!=0) return false;
	if (m_GameLogic.GetStepCount((wChairID==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS)==0) return false;

	//�жϴ���
	if (m_cbRegretCount[wChairID]==5)
	{
		//������Ϣ
		CMD_S_RegretFaile RegretFaile;
		RegretFaile.cbFaileReason=FR_COUNT_LIMIT;
		m_pITableFrame->SendTableData(wChairID,SUB_S_REGRET_FAILE,&RegretFaile,sizeof(RegretFaile));

		//ϵͳ��Ϣ
		LPCTSTR pszMessage=TEXT("���Ѿ����� [ ���� ] ���� 5 ���ˣ������ٴλ��塣");
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		m_pITableFrame->SendGameMessage(pIServerUserItem,pszMessage,SMT_INFO);

		return true;
	}

	//���ñ���
	m_cbRegretCount[wChairID]++;
	m_cbBegStatus[wChairID]|=GU_WAIT_REGRET;

	//������Ϣ
	m_pITableFrame->SendTableData((wChairID+1)%GAME_PLAYER,SUB_S_REGRET_REQ);

	return true;
}

//����Ӧ��
bool CTableFrameSink::OnUserRegretAnswer(WORD wChairID, BYTE cbApprove)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//״̬�ж�
	WORD wRegretUser=(wChairID+1)%GAME_PLAYER;
	if ((m_cbBegStatus[wRegretUser]&GU_WAIT_REGRET)==0) return false;

	//���ñ���
	m_cbBegStatus[wRegretUser]&=~GU_WAIT_REGRET;

	//��Ϣ����
	if (cbApprove==FALSE)
	{
		CMD_S_RegretFaile RegretFaile;
		RegretFaile.cbFaileReason=FR_PLAYER_OPPOSE;
		m_pITableFrame->SendTableData(wRegretUser,SUB_S_REGRET_FAILE,&RegretFaile,sizeof(RegretFaile));
		return true;
	}

	//��������
	CMD_S_RegretResult RegretResult;
	memset(&RegretResult,0,sizeof(RegretResult));

	//�������
	WORD wIndex=0;
	BYTE cbRegretColor=(wRegretUser==m_wBlackUser)?BLACK_CHESS:WHITE_CHESS;
	INT_PTR nChessManualCount=m_GameLogic.m_ChessManualInfo.GetCount();
	while (wIndex<nChessManualCount)
	{
		//��ȡ����
		tagChessManual * pChessManual=&m_GameLogic.m_ChessManualInfo[nChessManualCount-wIndex-1];
		if (pChessManual->cbColor==cbRegretColor) 
		{
			RegretResult.wRegretCount++;
			break;
		}
		
		//���ñ���
		wIndex++;
		RegretResult.wRegretCount++;
	}

	//�������
	m_GameLogic.RegretChess(RegretResult.wRegretCount);

	//����ʱ��
	DWORD dwNowClock=(DWORD)time(NULL);
	WORD wUseClock=(WORD)(dwNowClock-m_dwLastClock);
	m_wLeftClock[m_wCurrentUser]-=__min(wUseClock,m_wLeftClock[m_wCurrentUser]);

	//�л��û�
	if (RegretResult.wRegretCount>0) m_wCurrentUser=wRegretUser;

	//��������
	RegretResult.wRegretUser=wRegretUser;
	RegretResult.wCurrentUser=m_wCurrentUser;
	CopyMemory(RegretResult.wLeftClock,m_wLeftClock,sizeof(RegretResult.wLeftClock));

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REGRET_RESULT,&RegretResult,sizeof(RegretResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REGRET_RESULT,&RegretResult,sizeof(RegretResult));

	return true;
}

//�������
bool CTableFrameSink::OnUserPeaceReq(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//�û�״̬
	ASSERT((m_cbBegStatus[wChairID]&GU_WAIT_PEACE)==0);
	if ((m_cbBegStatus[wChairID]&GU_WAIT_PEACE)!=0) return false;

	//��ʹ���
	if (m_cbPeaceCount[wChairID]>=3)
	{
		LPCTSTR pszMessage=TEXT("��� [ ��� ] �����Ѿ����� 3 ���ˣ������ܴ���");
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		m_pITableFrame->SendGameMessage(pIServerUserItem,pszMessage,SMT_INFO);
		return true;
	}

	//���ñ���
	m_cbPeaceCount[wChairID]++;
	m_cbBegStatus[wChairID]|=GU_WAIT_PEACE;

	//������Ϣ
	m_pITableFrame->SendTableData((wChairID+1)%GAME_PLAYER,SUB_S_PEACE_REQ);

	return true;
}

//���Ӧ��
bool CTableFrameSink::OnUserPeaceAnswer(WORD wChairID, BYTE cbApprove)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//�û�״̬
	WORD wPeaceUser=(wChairID+1)%GAME_PLAYER;
	if ((m_cbBegStatus[wPeaceUser]&GU_WAIT_PEACE)==0) return true;

	//���ñ���
	m_cbBegStatus[wPeaceUser]&=~GU_WAIT_PEACE;

	//��Ϣ����
	if (cbApprove==FALSE) m_pITableFrame->SendTableData(wPeaceUser,SUB_S_PEACE_ANSWER);
	else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//������Ϸ
	OnEventGameEnd((wChairID+1)%GAME_PLAYER,NULL,GER_NORMAL);

	return true;
}

//�Ի�����
bool CTableFrameSink::OnUserTradeReq(WORD wChairID)
{
	//��Ϸ״̬
	if (m_pITableFrame->GetGameStatus()!=GS_PLAYING) return true;

	//����ж�
	if (wChairID==m_wBlackUser) return false;
	if (m_wCurrentUser!=wChairID) return false;
	if (m_GameLogic.GetStepCount(BLACK_CHESS)!=2) return false;

	//����ʱ��
	DWORD dwNowClock=(DWORD)time(NULL);
	WORD wUseClock=(WORD)(dwNowClock-m_dwLastClock);
	m_dwLastClock=dwNowClock;
	m_wLeftClock[m_wCurrentUser]-=__min(wUseClock,m_wLeftClock[m_wCurrentUser]);

	//�û�����
	m_wCurrentUser=m_wBlackUser;
	m_wBlackUser=(m_wBlackUser+1)%GAME_PLAYER;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BLACK_TRADE);

	return true;
}

//////////////////////////////////////////////////////////////////////////
