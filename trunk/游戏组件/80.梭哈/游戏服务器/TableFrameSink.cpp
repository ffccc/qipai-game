#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

#define IDI_GAME_END				1										//������ʶ

//////////////////////////////////////////////////////////////////////////

//��̬����
__int64				CTableFrameSink::g_lMaxScore=100000000L;				//���仯
__int64				CTableFrameSink::g_lMaxWinScore=100000000L;				//���Ӯ��
__int64				CTableFrameSink::g_lMaxLoseScore=-100000000L;			//������

//�û�����
CBlacklistIDMap		CTableFrameSink::g_BlacklistIDMap;						//�����û�
CScoreHistoryMap	CTableFrameSink::g_ScoreHistoryMap;						//��ʷ��¼

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_cbCardCount,0,sizeof(m_cbCardCount));
	memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
	m_cbGameEndReason = GER_NORMAL;

	//�����Ϣ
	m_lMaxGold=0;
	m_lTurnBasicGold=0;
	m_lBasicGold = 0;
	memset(m_lTableGold,0,sizeof(m_lTableGold));

	//��������
	memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));

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
	//״̬����
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));
	memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
	memset(m_cbCardCount,0,sizeof(m_cbCardCount));
	m_cbGameEndReason = GER_NORMAL;

	//��ұ���
	m_lMaxGold=0L;
	m_lTurnBasicGold=0L;
	m_lBasicGold = 0;
	memset(m_lTableGold,0,sizeof(m_lTableGold));

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
	ASSERT(wChairID<m_wPlayerCount);
	return m_cbPlayStatus[wChairID]?true:false;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//״̬����
	m_wOperaCount=0;
	m_bSendCardCount=0;
	memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));
	memset(m_cbCardCount,0,sizeof(m_cbCardCount));

	//��ұ���
	m_lMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_lTableGold,0,sizeof(m_lTableGold));

	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//��������
	const tagUserScore * pUserScore=NULL;
	IServerUserItem * pIServerUserItem=NULL;

	//������ע
	bool bCalculateGold=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			m_cbPlayStatus[i]=true;
			if (bCalculateGold==false)
			{
				bCalculateGold=true;
				pUserScore=pIServerUserItem->GetUserScore();
				ASSERT(pUserScore->lScore>m_pGameServiceOption->lCellScore);
				m_lMaxGold=pUserScore->lScore;
			}
			else 
			{
				pUserScore=pIServerUserItem->GetUserScore();
				ASSERT(pUserScore->lScore>1L);
				m_lMaxGold=__min(m_lMaxGold,pUserScore->lScore);
			}
		}
	}
	m_lBasicGold = __max(m_lMaxGold/100L,m_pGameServiceOption->lCellScore);

	for( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		if( m_cbPlayStatus[i] )
		{
			m_lTableGold[i*2+1]=m_lBasicGold;
		}
	}

	//�ַ��˿�
	m_bSendCardCount=2;
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_cbCardCount[i]=m_bSendCardCount;
		}
	}

	//���ƴ���
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			//��������
			bool bChangeCard=false;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;

			//�����ж�
			tagScoreHistory * pScoreHistory=NULL;
			if ((g_ScoreHistoryMap.Lookup(pIServerUserItem->GetUserID(),pScoreHistory)==TRUE)&&(pScoreHistory!=NULL))
			{
				if ((g_lMaxScore!=0L)&&(pScoreHistory->lScore>g_lMaxScore)) bChangeCard=true;
				if ((g_lMaxWinScore!=0L)&&(pScoreHistory->lWinScore>g_lMaxWinScore)) bChangeCard=true;
				if ((g_lMaxLoseScore!=0L)&&(pScoreHistory->lLoseScore<g_lMaxLoseScore)) bChangeCard=true;

			}

			//�����û�
			DWORD dwUserID=0L;
			if ((g_BlacklistIDMap.Lookup(pIServerUserItem->GetUserID(),dwUserID)==TRUE)&&(dwUserID!=0L)) bChangeCard=true;

			//�����˿�
			if (bChangeCard==true)
			{
				//��ȡ����
				WORD wLoseIndex=EstimateLoser(0,4);

				//�����˿�
				if (wLoseIndex!=i)
				{
					BYTE cbTempCardData[5];
					CopyMemory(cbTempCardData,m_cbHandCardData[i],5*sizeof(BYTE));
					CopyMemory(m_cbHandCardData[i],m_cbHandCardData[wLoseIndex],5*sizeof(BYTE));
					CopyMemory(m_cbHandCardData[wLoseIndex],cbTempCardData,5*sizeof(BYTE));
				}

				break;
			}
		}
	}

	//�����û�
	m_wCurrentUser=EstimateWinner(1,1);

	//���Ϳ�ʼ
	CMD_S_GameStart GameStart;
	memset(&GameStart,0,sizeof(GameStart));
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.lMaxGold=m_lMaxGold;
	GameStart.lTurnMaxGold=m_lMaxGold/4L;
	GameStart.lTurnBasicGold=m_lTurnBasicGold;
	GameStart.lBasicGold = m_lBasicGold;
	for (WORD i=0;i<m_wPlayerCount;i++) GameStart.bCardData[i]=m_cbHandCardData[i][1];
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		GameStart.bFundusCard=0;
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart.bFundusCard=m_cbHandCardData[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//�������
	case GER_NO_PLAYER:		//û�����
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������ע
			LONG lAllGold=0L;
			for (WORD i=0;i<CountArray(m_lTableGold);i++) lAllGold+=m_lTableGold[i];

			//ͳ����Ϣ
			WORD wWinerUser=EstimateWinner(0,4);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				LONG lScore=0;
				LONG lRevenue=0;
				enScoreKind ScoreKind;

				//������Ϣ
				if ((cbReason==GER_NO_PLAYER)||(m_cbPlayStatus[i]==FALSE)) GameEnd.bUserCard[i]=0;
				else GameEnd.bUserCard[i]=m_cbHandCardData[i][0];

				//������Ϣ
				if (i==wWinerUser)
				{
					lScore=lAllGold-m_lTableGold[i*2]-m_lTableGold[i*2+1];
					GameEnd.lGameGold[i]=lAllGold-m_lTableGold[i*2]-m_lTableGold[i*2+1];
				}
				else
				{
					lScore=-m_lTableGold[i*2]-m_lTableGold[i*2+1];
					GameEnd.lGameGold[i]=-m_lTableGold[i*2]-m_lTableGold[i*2+1];
				}

				//ʤ������
				ScoreKind=(GameEnd.lGameGold[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				//����˰��
				if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					//��˰����
					BYTE cbRevenue=(BYTE)m_pGameServiceOption->wRevenue;

					if (GameEnd.lGameGold[i]>=100L)
					{
						//����˰��
						lRevenue=GameEnd.lGameGold[i]*cbRevenue/100L;

						//�ɼ�����
						GameEnd.lTax+=lRevenue;
						GameEnd.lGameGold[i]=GameEnd.lGameGold[i]-lRevenue;

						//д�ֱ���
						lRevenue=lRevenue;
						lScore=lScore-lRevenue;
					}
				}
				//д�����
				if (m_cbPlayStatus[i]==TRUE)
				{
					m_pITableFrame->WriteUserScore(i,lScore,lRevenue,ScoreKind);
				}

				//�ɼ���¼
				if (m_cbPlayStatus[i]==TRUE)
				{
					//��ȡ�û�
					IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
					if (pIServerUserItem==NULL) continue;
 
					//�����û�
					tagScoreHistory * pScoreHistory=NULL;
					g_ScoreHistoryMap.Lookup(pIServerUserItem->GetUserID(),pScoreHistory);

					//��ȡʱ��
					SYSTEMTIME SystemTime;
					GetLocalTime(&SystemTime);

					//�����û�
					if (pScoreHistory==NULL)
					{
						//��������
						pScoreHistory=new tagScoreHistory;
						g_ScoreHistoryMap[pIServerUserItem->GetUserID()]=pScoreHistory;

						//���ñ���
						pScoreHistory->lScore=0L;
						pScoreHistory->lWinScore=0L;
						pScoreHistory->lLoseScore=0L;
						pScoreHistory->RecordTime=SystemTime;
					}

					//��ԭ�ж�
					if (pScoreHistory->RecordTime.wDay!=SystemTime.wDay)
					{
						pScoreHistory->lScore=0L;
						pScoreHistory->lWinScore=0L;
						pScoreHistory->lLoseScore=0L;
						pScoreHistory->RecordTime=SystemTime;
					}

					//��¼����
					pScoreHistory->lScore+=lScore;
					if (lScore>0L) pScoreHistory->lWinScore+=lScore;
					if (lScore<0L) pScoreHistory->lLoseScore+=lScore;
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
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
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
			StatusFree.dwBasicGold=0;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lMaxGold=m_lMaxGold;
			StatusPlay.lTurnBasicGold=m_lTurnBasicGold;
			StatusPlay.lBasicGold=m_lBasicGold;
			CopyMemory(StatusPlay.bPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));
			CopyMemory(StatusPlay.lTableGold,m_lTableGold,sizeof(m_lTableGold));

			//������ע
			if (m_bSendCardCount<=2) StatusPlay.lTurnMaxGold=m_lMaxGold/4L;
			else if (m_bSendCardCount==3) StatusPlay.lTurnMaxGold=m_lMaxGold/2L;
			else StatusPlay.lTurnMaxGold=m_lMaxGold;

			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					ASSERT(m_pITableFrame->GetServerUserItem(i)!=NULL);
					StatusPlay.bTableCardCount[i]=m_cbCardCount[i];
					if (i==wChiarID) StatusPlay.bTableCardArray[i][0]=m_cbHandCardData[i][0];
					CopyMemory(&StatusPlay.bTableCardArray[i][1],&m_cbHandCardData[i][1],(m_cbCardCount[i]-1)*sizeof(BYTE));
				}
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//Ч����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_GAME_END:	//��Ϸ����
		{
			if (m_pITableFrame->GetGameStatus()==GS_PLAYING)
			{
				OnEventGameEnd(INVALID_CHAIR,NULL,m_cbGameEndReason);
				return true;
			}
		}
	}
	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_ADD_GOLD:	//�û���ע
		{
			//Ч������
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return true;

			//��Ϣ����
			CMD_C_AddGold * pAddGold=(CMD_C_AddGold *)pDataBuffer;
			OnUserAddGold(pUserData->wChairID,pAddGold->lGold,false);

			return true;
		}
	case SUB_C_GIVE_UP:		//�û�����
		{
			//Ч������
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return true;

			//��Ϣ����
			OnUserGiveUp(pUserData->wChairID);

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

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	//״̬�ж�
	ASSERT(m_cbPlayStatus[wChairID]==TRUE);
	if (m_cbPlayStatus[wChairID]==FALSE) return false;

	//��������
	m_cbPlayStatus[wChairID]=FALSE;
	m_cbCardCount[wChairID]=0;

	//���ͷ�����Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wUserChairID=wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//��������
	LONG lScore=0;
	LONG lRevenue=0;
	enScoreKind ScoreKind;

	//���ñ���
	ScoreKind=enScoreKind_Lost;
	lScore=-m_lTableGold[wChairID*2]-m_lTableGold[wChairID*2+1];

	//�û�д��
	m_pITableFrame->WriteUserScore(wChairID,lScore,lRevenue,ScoreKind);

	//�жϽ���
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddGold(wChairID,0L,true);
	}
	else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddGold(WORD wChairID, LONG lGold, bool bCancel)
{
	bool bShowHand = false;
	//��������
	if (bCancel==false)
	{
		//Ч������
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		//������ע
		LONG lMaxGold=0L;
		if (m_bSendCardCount<=2) lMaxGold=m_lMaxGold/4L;
		else lMaxGold=m_lMaxGold;
		
		//����ʣ�µĽ��
		LONG lLeaveGold=lMaxGold-m_lTableGold[wChairID*2+1];
		m_lTableGold[wChairID*2]=__min(lLeaveGold,__max(lGold,m_lTurnBasicGold));

		if( m_lTableGold[wChairID*2]+m_lTableGold[wChairID*2+1] >= m_lMaxGold ) bShowHand = true;
	}

	//��������
	bool bFinishTurn=false;
	WORD wNextUser=INVALID_CHAIR;
	for (WORD i=0;i<m_wPlayerCount-1;i++)
	{
		m_wOperaCount++;
		wNextUser=(m_wCurrentUser+i+1)%m_wPlayerCount;
		if (m_cbPlayStatus[wNextUser]==TRUE) break;
	}
	
	//�ж���ע
	if (m_wOperaCount>=m_wPlayerCount)
	{
		LONG lTableGold=0L;
		WORD i;
		for (i=0;i<m_wPlayerCount;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				lTableGold=m_lTableGold[i*2];
				break;
			}
		}
		while (i<m_wPlayerCount)
		{
			if ((m_cbPlayStatus[i]==TRUE)&&(m_lTableGold[i*2]!=lTableGold)) break;
			i++;
		}
		if (i==m_wPlayerCount) bFinishTurn=true;
	}

	//һ���ж�
	if (bFinishTurn==true)
	{
		//��������
		CMD_S_AddGold GoldResult;
		GoldResult.lCurrentLessGold=0L;
		GoldResult.wLastChairID=wChairID;
		GoldResult.wCurrentUser=INVALID_CHAIR;
		GoldResult.lLastAddGold=m_lTableGold[wChairID*2];
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));

		//�ۼƽ��
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_lTableGold[i*2+1]+=m_lTableGold[i*2];
			m_lTableGold[i*2]=0L;
		}

		//��ʼһ��
		if (m_bSendCardCount<5) 
		{
			//���ñ���
			m_wOperaCount=0;
			m_lTurnBasicGold=0L;
			WORD wLastMostUser = EstimateWinner(1,m_bSendCardCount-1);

			BYTE bySourceCount = m_bSendCardCount;
			if( bShowHand ) m_bSendCardCount = MAX_COUNT;
			else m_bSendCardCount++;

			//�����˿�
			CMD_R_SendCard SendCard;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if (m_cbPlayStatus[i]==FALSE) continue;

				m_cbCardCount[i]=m_bSendCardCount;

				//�ɷ��˿�
				for (BYTE j=0;j<(m_bSendCardCount-bySourceCount);j++)
				{
					ASSERT(j<CountArray(SendCard.bUserCard[i]));
					SendCard.bUserCard[i][j]=m_cbHandCardData[i][bySourceCount+j];
				}
			}

			m_wCurrentUser=EstimateWinner(1,m_bSendCardCount-1);

			//������Ϣ
			SendCard.wCurrentUser=(bShowHand?INVALID_CHAIR:m_wCurrentUser);
			SendCard.wStartChairId = wLastMostUser;
			SendCard.cbSendCardCount = m_bSendCardCount-bySourceCount;
			SendCard.lMaxGold=(m_bSendCardCount>=3)?m_lMaxGold:m_lMaxGold/2L;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

			//������Ϸ
			if ( m_bSendCardCount == 5 && m_lMaxGold <= m_lTableGold[wChairID*2+1] ) 
			{
				m_cbGameEndReason = GER_NORMAL;
				m_pITableFrame->SetGameTimer(IDI_GAME_END,2000,1,0);
			}
		}
		else 
		{
			m_cbGameEndReason = GER_NORMAL;
			m_pITableFrame->SetGameTimer(IDI_GAME_END,2000,1,0);
		}
	}
	else
	{
		//���ñ���
		m_wCurrentUser=wNextUser;
		m_lTurnBasicGold=__max(m_lTableGold[wChairID*2],m_lTurnBasicGold);

		//��������
		CMD_S_AddGold AddGold;
		AddGold.wLastChairID=wChairID;
		AddGold.wCurrentUser=m_wCurrentUser;
		AddGold.lCurrentLessGold=m_lTurnBasicGold;
		AddGold.lLastAddGold=m_lTableGold[wChairID*2];
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
	}

	return true;
}

bool CTableFrameSink::OnUserGetWinner(IServerUserItem * pIServerUserItem)
{
	//��ȡʤ��
	IServerUserItem * pIWinnerUserItem=m_pITableFrame->GetServerUserItem(EstimateWinner(0,4));
	if (pIWinnerUserItem==NULL) return true;

	//������Ϣ
	TCHAR szMessage[128];
	_snprintf(szMessage,CountArray(szMessage),TEXT("���ֵ�ʤ������ǣ�%s"),pIWinnerUserItem->GetAccounts());

	//������Ϣ
	m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_INFO);

	return true;
}

//�ƶ�����
WORD CTableFrameSink::EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	WORD wLoser;
	for (wLoser=0;wLoser<GAME_PLAYER;wLoser++)
	{
		if (m_cbPlayStatus[wLoser]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	for (WORD i=(wLoser+1);i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1)==false) 
		{
			wLoser=i;
		}
	}

	return wLoser;
}

//�ƶ�ʤ��
WORD CTableFrameSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	WORD wWinner;
	for (wWinner=0;wWinner<GAME_PLAYER;wWinner++)
	{
		if (m_cbPlayStatus[wWinner]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	for (WORD i=(wWinner+1);i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=i;
		}
	}

	return wWinner;
}

//////////////////////////////////////////////////////////////////////////
