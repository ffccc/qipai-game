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
//	m_wBombTime=1;
	//m_bLandScore=0;
	//m_wFirstUser=0;
	//m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
//	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//�˿���Ϣ
//	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	
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
	//��Ϸ����
	//m_wBombTime=1;
	//m_bLandScore=0;
	//m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	//memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//�˿���Ϣ
	//memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
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
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

	//�����˿�
	BYTE bRandCard[48];
	m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=16;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}

	//Ѱ�Һ���3,����2
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		for(int j=0;j<m_bCardCount[i];j++)
		{
			if(m_bHandCardData[i][j]==0x23)
			{
				m_wBankerUser=i;
			}
		}
	}
	m_wCurrentUser=m_wBankerUser;

	//�����˿�
	CMD_S_GameStart GameStart;
	GameStart.wCurrentUser=m_wCurrentUser;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		CopyMemory(GameStart.bCardData,m_bHandCardData[i],sizeof(GameStart.bCardData));
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
	case GER_NORMAL:		//�������
		{
			//�������
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

			//��������
			LONG lCellScore=m_pGameServiceOption->lCellScore;
			//bool bLandWin=(m_bCardCount[m_wBankerUser]==0)?true:false;

			////�����ж�
			//if (wChairID==m_wBankerUser)
			//{
			//	WORD wUser1=(m_wBankerUser+1)%GAME_PLAYER;
			//	WORD wUser2=(m_wBankerUser+2)%GAME_PLAYER;
			//	if ((m_bOutCardCount[wUser1]==0)&&(m_bOutCardCount[wUser2]==0)) m_wBombTime*=2;
			//}
			//else
			//{
			//	if (m_bOutCardCount[m_wBankerUser]==1) m_wBombTime*=2;
			//}

			//����
			BYTE times=1;
			//������������Ǻ���3
			if( m_bTurnCardData[0]==0x23)
				times=2;

			WORD wWiner;
			LONG lWinSore=0;
			//ͳ�ƻ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bCardCount[i]==0)
					wWiner=i;
				GameEnd.lGameScore[i]=-m_bCardCount[i]*times*lCellScore;
				lWinSore-=GameEnd.lGameScore[i];
				//if (i==m_wBankerUser) GameEnd.lGameScore[i]=m_wBombTime*m_bLandScore*lCellScore*((bLandWin==true)?2:-2);
				//else GameEnd.lGameScore[i]=m_wBombTime*m_bLandScore*lCellScore*((bLandWin==true)?-1:1);
			}
			GameEnd.lGameScore[wWiner]=lWinSore;

			//����˰��
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/50L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*49L/50L;
					}
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i,ScoreInfo[i].lScore,ScoreInfo[i].lRevenue,ScoreInfo[i].ScoreKind);
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//�л��û�
//			m_wFirstUser=wChairID;

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			LONG lCellScore=m_pGameServiceOption->lCellScore;
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			
			//����ǿ��
			//GameEnd.lGameScore[wChairID]=-2*m_wBombTime*m_bLandScore*lCellScore;
			//for (WORD i=0;i<m_wPlayerCount;i++)
			//{
			//	if(i==wChairID)
			//	{
			//		GameEnd.lGameScore[i]=-2*m_wBombTime*m_bLandScore*lCellScore;
			//	}
			//	else if( i==m_wBankerUser)
			//	{
			//		GameEnd.lGameScore[i]=2*m_wBombTime*m_bLandScore*lCellScore;
			//	}
			//	else
			//	{
			//		if( wChairID==m_wBankerUser)
			//			GameEnd.lGameScore[i]=m_wBombTime*m_bLandScore*lCellScore;
			//		else
			//			GameEnd.lGameScore[i]=0;
			//	}
			//}
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID)	
					GameEnd.lGameScore[wChairID]=3*m_bCardCount[wChairID];
				else
					GameEnd.lGameScore[i]=0;
			}
		
			//����˰��
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/50L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*49L/50L;
					}
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			////ʣ���˿�
			//BYTE bCardPos=0;
			//for (WORD i=0;i<m_wPlayerCount;i++)
			//{
			//	GameEnd.bCardCount[i]=m_bCardCount[i];
			//	CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
			//	bCardPos+=m_bCardCount[i];
			//}

			////������Ϣ
			//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			//m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Draw;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,ScoreInfo.lRevenue,ScoreInfo.ScoreKind);

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
	case GS_WK_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	//case GS_WK_SCORE:		//�з�״̬
	//	{
	//		//��������
	//		CMD_S_StatusScore StatusScore;
	//		memset(&StatusScore,0,sizeof(StatusScore));

	//		//���ñ���
	//		StatusScore.bLandScore=m_bLandScore;
	//		StatusScore.wCurrentUser=m_wCurrentUser;
	//		StatusScore.lBaseScore=m_pGameServiceOption->lCellScore;
	//		CopyMemory(StatusScore.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
	//		CopyMemory(StatusScore.bCardData,m_bHandCardData[wChiarID],m_bCardCount[wChiarID]);

	//		//���ͳ���
	//		return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
	//	}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			////���ñ���
			//StatusPlay.wBombTime=m_wBombTime;
			//StatusPlay.wLandUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
		//	StatusPlay.bLandScore=m_bLandScore;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
//			CopyMemory(StatusPlay.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);

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
	//case SUB_C_LAND_SCORE:	//�û��з�
	//	{
	//		//Ч������
	//		ASSERT(wDataSize==sizeof(CMD_C_LandScore));
	//		if (wDataSize!=sizeof(CMD_C_LandScore)) return false;

	//		//�û�Ч��
	//		tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	//		if (pUserData->cbUserStatus!=US_PLAY) return true;

	//		//��Ϣ����
	//		CMD_C_LandScore * pLandScore=(CMD_C_LandScore *)pDataBuffer;
	//		return OnUserLandScore(pUserData->wChairID,pLandScore->bLandScore);
	//	}
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
	}

	return true;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

////�з��¼�
//bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bLandScore)
//{
//	//Ч��״̬
//	if (m_pITableFrame->GetGameStatus()!=GS_WK_SCORE) return true;
//	if (wChairID!=m_wCurrentUser) return false;
//
//	//Ч�����
//	if (((bLandScore>3)&&(bLandScore!=255))||(bLandScore<=m_bLandScore)) return false;
//
//	//���˽��ط�
//	if( bLandScore==255&&(m_wFirstUser==(wChairID+1)%m_wPlayerCount))
//	{
//		//�����˿�
//		BYTE bRandCard[54];
//		m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));
//
//		//�ַ��˿�
//		for (WORD i=0;i<m_wPlayerCount;i++)
//		{
//			m_bCardCount[i]=17;
//			CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
//			m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
//		}
//		CopyMemory(m_bBackCard,&bRandCard[51],sizeof(m_bBackCard));
//		m_GameLogic.SortCardList(m_bBackCard,3);
//
//		srand( (unsigned)time( NULL ) );
//		m_wFirstUser=rand()%3;
//		//�����û�
//		m_wCurrentUser=m_wFirstUser;
//
//		//�����˿�
//		CMD_S_SendCard SendCard;
//		SendCard.wCurrentUser=m_wCurrentUser;
//		for (WORD i=0;i<m_wPlayerCount;i++)
//		{
//			CopyMemory(SendCard.bCardData,m_bHandCardData[i],sizeof(SendCard.bCardData));
//			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
//			m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
//		}
//		return true;
//	}
//	//���ñ���
//	if (bLandScore!=255)
//	{
//		m_bLandScore=bLandScore;
//		m_wBankerUser=m_wCurrentUser;
//	}
//	m_bScoreInfo[wChairID]=bLandScore;
//	//��ʼ�ж�
//	if ((m_bLandScore==3)||(m_wFirstUser==(wChairID+1)%m_wPlayerCount))
//	{
//		//���ñ���
//		if (m_bLandScore==0) m_bLandScore=1;
//		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;
//
//		//����״̬
//		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
//
//		//���͵���
//		m_bCardCount[m_wBankerUser]=20;
//		CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
//		m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser]);
//
//		//������Ϣ
//		m_bTurnCardCount=0;
//		m_wTurnWiner=m_wBankerUser;
//		m_wCurrentUser=m_wBankerUser;
//
//		//������Ϣ
//		CMD_S_GameStart GameStart;
//		GameStart.wLandUser=m_wBankerUser;
//		GameStart.bLandScore=m_bLandScore;
//		GameStart.wCurrentUser=m_wCurrentUser;
//		CopyMemory(GameStart.bBackCard,m_bBackCard,sizeof(m_bBackCard));
//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
//
//		return true;
//	}
//
//	//�����û�
//	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;
//
//	//������Ϣ
//	CMD_S_LandScore LandScore;
//	LandScore.bLandUser=wChairID;
//	LandScore.bLandScore=bLandScore;
//	LandScore.wCurrentUser=m_wCurrentUser;
//	LandScore.bCurrentScore=m_bLandScore;
//	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
//	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
//
//	return true;
//}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_INVALID) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else 
	{
		//������
		//�ذ�һ
		if(m_bCardCount[wChairID]>1)
		{
			if(m_bCardCount[wChairID]-bCardCount<1)
				return false;
			//���һ�Ų���Ϊ2
			else if(m_bCardCount[wChairID]-bCardCount==2  || m_bCardCount[wChairID]-bCardCount==1)
			{
				if(m_GameLogic.IsCardTwoOwner(m_bHandCardData[wChairID],m_bCardCount[wChairID]))
				{
					if(!m_GameLogic.IsCardTwoOwner(bCardData,bCardCount))
						return false;
				}
			}
		}
		if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	//����һ
	if( m_bCardCount[wChairID]==1 )
	{
		CMD_S_EightOne EightOne;
		EightOne.wEightOneUser=wChairID;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_EIGHT_ONE,&EightOne,sizeof(EightOne));
	}
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//ը���ж�
//	if ((bCardType==CT_BOMB_CARD)||(bCardType==CT_MISSILE_CARD)) m_wBombTime*=2;
	
	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
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

	////�������
	//if (bCardType==CT_MISSILE_CARD) m_bTurnCardCount=0;

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//���ñ���
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_bCardCount[m_wCurrentUser]==2)
	{
		if (m_bHandCardData[m_wCurrentUser][0]==0x22 || m_bHandCardData[m_wCurrentUser][1]==0x22 )
		{
			m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}
	}
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

//////////////////////////////////////////////////////////////////////////
