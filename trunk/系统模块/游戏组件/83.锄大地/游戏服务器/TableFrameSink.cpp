#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bUserStrustee,sizeof(m_bUserStrustee));

	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
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
	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

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
	//����״̬
	m_pITableFrame->SetGameStatus(GS_RF_PLAYING);

	//�����˿�
	BYTE bRandCard[52];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));
	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=13;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
	}

	//�����û� 
	BYTE bOut3=FALSE;
	if (m_wBankerUser==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			for (BYTE j=0;j<m_bCardCount[i];j++)
			{
				if (m_bHandCardData[i][j]==0x03) 
				{
					bOut3=TRUE;
					m_wBankerUser=i;
					break;
				}
			}
			if (m_wBankerUser!=INVALID_CHAIR) break;
		}
	}

	//�����û�
	m_wCurrentUser=m_wBankerUser;

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}

	//�����˿�
	CMD_S_GameStart GameStart;
	GameStart.cbOutCard3=bOut3;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	IServerUserItem *pUser=NULL;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		pUser = m_pITableFrame->GetServerUserItem(i);
		if( pUser->IsAndroidUser() )
			CopyMemory(GameStart.cbCardData,m_bHandCardData,sizeof(GameStart.cbCardData));
		else
		{
			CopyMemory(GameStart.cbCardData,m_bHandCardData[i],sizeof(BYTE)*m_bCardCount[i]);
		}
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
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

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ͳ���Ʒ�
			LONG lCardScore[GAME_PLAYER];
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				LONG lScoreTimes=1;
				if (m_bCardCount[i]==13) lScoreTimes=4;
				else if (m_bCardCount[i]>=10) lScoreTimes=3;
				else if (m_bCardCount[i]>=8) lScoreTimes=2;
				lCardScore[i]=lScoreTimes*m_bCardCount[i];
			}

			//ͳ�ƻ���
			LONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//������Ϣ
				for (WORD j=0;j<m_wPlayerCount;j++)
				{
					if (j!=i)
					{
						LONG lCellScore=m_pGameServiceOption->lCellScore;
						GameEnd.lGameScore[i]+=(lCardScore[j]-lCardScore[i])*lCellScore;
					}
				}
				
				//��˰
				if( GAME_GENRE_GOLD == m_pGameServiceOption->wServerType )
				{
					if( GameEnd.lGameScore[i] > 5L )
					{
						lGameTax[i] = GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/1000L;
						GameEnd.lGameScore[i] -= lGameTax[i];
					}
				}
			}

			//��ʷ����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_lAllTurnScore[i]+=GameEnd.lGameScore[i];
				m_lLastTurnScore[i]=GameEnd.lGameScore[i];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸ķ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				enScoreKind ScoreKind;
				if( GameEnd.lGameScore[i] == 0 ) ScoreKind = enScoreKind_Draw;
				else if( GameEnd.lGameScore[i] > 0 ) ScoreKind = enScoreKind_Win;
				else ScoreKind = enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lGameTax[i],ScoreKind);
			}

			//�л�ׯ��
			m_wBankerUser=wChairID;

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ͳ�ƻ���
			GameEnd.lGameScore[wChairID] = -200L*m_pGameServiceOption->lCellScore;
			

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,enScoreKind_Flee);

			//�л�ׯ��
			m_wBankerUser=INVALID_CHAIR;

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_RF_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

			//�йܱ�־
			CopyMemory(StatusFree.bStrustee,m_bUserStrustee,sizeof(m_bUserStrustee));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_RF_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			//�йܱ�־
			CopyMemory(StatusPlay.bStrustee,m_bUserStrustee,sizeof(m_bUserStrustee));
			
			if( pIServerUserItem->IsAndroidUser() )
			{
				CopyMemory(StatusPlay.bCardData,m_bHandCardData,sizeof(m_bHandCardData));
			}
			else
			{
				CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			}

			//��ʷ����
			CopyMemory(StatusPlay.lAllTurnScore,m_lAllTurnScore,sizeof(m_lAllTurnScore));
			CopyMemory(StatusPlay.lLastTurnScore,m_lLastTurnScore,sizeof(m_lLastTurnScore));

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
	case SUB_C_OUT_CART:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserPassCard(pUserData->wChairID);
		}
	case SUB_C_STRUSTEE:
		{
			//��������
			CMD_C_Strustee *pStrustee = (CMD_C_Strustee *)pDataBuffer;

			//Ч������
			ASSERT( wDataSize == sizeof(CMD_C_Strustee) );
			if( wDataSize != sizeof(CMD_C_Strustee))  return false;

			//�û�����
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			//��Ϣ����
			return OnUserStrustee(pUserData->wChairID,pStrustee->bStrustee);
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if (bLookonUser==false) 
	{
		m_wBankerUser=INVALID_CHAIR;
		if( m_bUserStrustee[wChairID] == true )
		{
			m_bUserStrustee[wChairID] = false;
			//�йܱ�־
			CMD_S_Strustee Strustee;
			Strustee.bStrustee = m_bUserStrustee[wChairID];
			Strustee.wChairId = wChairID;
			IServerUserItem *pUserItem = NULL;
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				pUserItem = m_pITableFrame->GetServerUserItem(i);
				if( pUserItem == NULL || pUserItem == pIServerUserItem ) continue;
				m_pITableFrame->SendTableData(i,SUB_S_STRUSTEE,&Strustee,sizeof(Strustee));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STRUSTEE,&Strustee,sizeof(Strustee));
		}
	}
	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//�׳��ж�
/*	if ((wChairID==m_wBankerUser)&&(m_bCardCount[wChairID]==13))
	{
		for (BYTE i=0;i<bCardCount;i++)
		{
			if (bCardData[i]==0x03) break;
		}
		if (i==bCardCount) return false;
	}*/

	//ǿ�Ƴ���
	WORD wNextUser=(wChairID+1)%GAME_PLAYER;
//	if ((m_bCardCount[wNextUser]==1)&&(bCardCount==1)&&(bCardData[0]!=m_bHandCardData[wChairID][0])) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	for (BYTE i=0;i<bCardCount;i++)
	{
		BYTE cbCardValue=m_GameLogic.GetCardValue(bCardData[i]);
		if (cbCardValue==0x02) m_bCount2[m_GameLogic.GetCardColor(bCardData[i])>>4]=true;
	}

	//�����ж�
	bool bMostest=false;
	BYTE cbMostValue=m_GameLogic.GetCardValue(bCardData[0]);
	BYTE cbMostColor=m_GameLogic.GetCardColor(bCardData[0]);

	//һ���ж�
	if (cbMostValue==0x02)
	{
		for (BYTE i=(cbMostColor>>4);i<CountArray(m_bCount2);i++)
		{
			if (m_bCount2[i]==false) break;
		}
		if (i==CountArray(m_bCount2)) bMostest=true;
	}

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if (bMostest==false) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�������
	if (bMostest==true) m_bTurnCardCount=0;

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//���ñ���
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;

	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//�û��й�
bool CTableFrameSink::OnUserStrustee(WORD wChairID,bool bStrustee)
{
	//Ч��
	ASSERT( m_bUserStrustee[wChairID] != bStrustee );
	if( m_bUserStrustee[wChairID] == bStrustee ) return true;

	//���ñ���
	m_bUserStrustee[wChairID] = bStrustee;

	//��������
	CMD_S_Strustee Strustee;
	Strustee.wChairId = wChairID;
	Strustee.bStrustee = bStrustee;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STRUSTEE,&Strustee,sizeof(Strustee));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STRUSTEE,&Strustee,sizeof(Strustee));

	return true;
}

//////////////////////////////////////////////////////////////////////////
