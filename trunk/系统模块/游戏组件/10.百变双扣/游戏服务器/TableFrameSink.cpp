#include "StdAfx.h"
#include "TableFrameSink.h"
#include "cmath"
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
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));
	ZeroMemory(m_cbGongxian,sizeof(m_cbGongxian));

	//������Ϣ
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//�˿���Ϣ
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));


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
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//ʤ����Ϣ
	m_wWinCount=0;
	ZeroMemory(m_wWinOrder,sizeof(m_wWinOrder));
	ZeroMemory(m_cbGongxian,sizeof(m_cbGongxian));

	//������Ϣ
	m_cbTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//�˿���Ϣ
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	srand((unsigned)time(NULL));
	BYTE cbRandCard[FULL_COUNT];
	m_GameLogic.RandCardList(cbRandCard,CountArray(cbRandCard));

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHandCardCount[i]=MAX_COUNT;
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
	}
    
	//�����û�
	if(m_wBankerUser == INVALID_CHAIR)
		m_wBankerUser = rand()%4;
	m_wTurnWiner=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//�������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//���ñ���
	GameStart.wCurrentUser=m_wCurrentUser;
    GameStart.lCellScore=m_pGameServiceOption->lCellScore;

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�����˿�
		ASSERT(CountArray(GameStart.cbCardData)>=m_cbHandCardCount[i]);
		CopyMemory(GameStart.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);
		CopyMemory(GameStart.cbPartnerCardData,m_cbHandCardData[(i+2)%GAME_PLAYER],sizeof(BYTE)*m_cbHandCardCount[(i+2)%GAME_PLAYER]);

		GameStart.bTrustee[i]=m_bTrustee[i];
		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i],ST_ORDER);
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

			//�û��˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameEnd.cbCardData[i],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));
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
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

			//ʤ����Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (m_cbHandCardCount[i]!=0)
				{
					m_wWinOrder[m_wWinCount++]=i;
				}
			}

			//ʣ���˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameEnd.cbCardData[i],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));
			}

			//���ñ���
			LONG lTotalTimes[4];
			LONG lMaxTimes[4];
			
			//���㹱�׷ֺ������
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				GetTotalTimes(m_cbGongxian,i,lTotalTimes[i],lMaxTimes[i]);
			}
            
			
			if (m_wWinOrder[0]==(m_wWinOrder[2]+2)%GAME_PLAYER) //�����ж�
			{
				//ȡӮ�������ı���
				LONG lBothMaxTimes=max(lMaxTimes[m_wWinOrder[0]],lMaxTimes[m_wWinOrder[2]]);
	
				//��Ϸ����
				ScoreInfo[m_wWinOrder[0]].lScore = 4*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[0]].ScoreKind = enScoreKind_Win;
				ScoreInfo[m_wWinOrder[2]].lScore = 4*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[2]].ScoreKind = enScoreKind_Win;
				ScoreInfo[m_wWinOrder[1]].lScore= -4*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[1]].ScoreKind = enScoreKind_Lost;
				ScoreInfo[m_wWinOrder[3]].lScore= -4*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[3]].ScoreKind = enScoreKind_Lost;
				
				//�л�ׯ��
				m_wBankerUser=m_wWinOrder[0];

			}			
			else if (m_wWinOrder[0]==(m_wWinOrder[1]+2)%GAME_PLAYER)//˫���ж�
			{
				//ȡӮ�������ı���
				LONG lBothMaxTimes=max(lMaxTimes[m_wWinOrder[0]],lMaxTimes[m_wWinOrder[1]]);

				//��Ϸ����
				ScoreInfo[m_wWinOrder[0]].lScore = 6*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[0]].ScoreKind = enScoreKind_Win;
				ScoreInfo[m_wWinOrder[1]].lScore = 6*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[1]].ScoreKind = enScoreKind_Win;
				ScoreInfo[m_wWinOrder[2]].lScore = -6*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[2]].ScoreKind = enScoreKind_Lost;
				ScoreInfo[m_wWinOrder[3]].lScore = -6*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[3]].ScoreKind = enScoreKind_Lost;
			
				//�л�ׯ��
				m_wBankerUser=rand()%4;
			}			
			else //ƽ���ж�
			{
				//ȡӮ�������ı���
				LONG lBothMaxTimes=max(lMaxTimes[m_wWinOrder[0]],lMaxTimes[m_wWinOrder[3]]);

				//��Ϸ����
				ScoreInfo[m_wWinOrder[0]].lScore =2*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[0]].ScoreKind = enScoreKind_Draw;
				ScoreInfo[m_wWinOrder[2]].lScore = -2*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[2]].ScoreKind = enScoreKind_Draw;
				ScoreInfo[m_wWinOrder[1]].lScore = -2*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[1]].ScoreKind = enScoreKind_Draw;
				ScoreInfo[m_wWinOrder[3]].lScore = 2*m_pGameServiceOption->lCellScore*lBothMaxTimes;
				ScoreInfo[m_wWinOrder[3]].ScoreKind = enScoreKind_Draw;

				//�л�ׯ��
				m_wBankerUser=rand()%4;
			}
			//���㹱�׷�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (WORD j=0;j<GAME_PLAYER;j++)
				{
					if(i==j) continue;
					if(lTotalTimes[i]==0) continue;
					ScoreInfo[i].lScore +=m_pGameServiceOption->lCellScore*lTotalTimes[i];
					ScoreInfo[j].lScore -=m_pGameServiceOption->lCellScore*lTotalTimes[i];
				}
			}
			//��Ϸ����
			for (WORD i =0;i<GAME_PLAYER;i++)
			{
				GameEnd.lGameScore[i] = ScoreInfo[i].lScore;
			}

			//����˰��
			if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
			{
				//��˰����
				BYTE cbRevenue=m_pGameServiceOption->wRevenue;

				//����˰��
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>0L)
					{
						//����˰��
						LONG lRevenue=GameEnd.lGameScore[i]*cbRevenue/100L;

						//���ֵ���
						GameEnd.lGameTax+=lRevenue;
						ScoreInfo[i].lRevenue=lRevenue;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue;
					}
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//д�����
				m_pITableFrame->WriteUserScore(i,ScoreInfo[i].lScore,ScoreInfo[i].lRevenue,ScoreInfo[i].ScoreKind);
			}
			//������Ϸ
			m_pITableFrame->ConcludeGame();


			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//�û��˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_cbHandCardCount[i];
				CopyMemory(&GameEnd.cbCardData[i],m_cbHandCardData[i],m_cbHandCardCount[i]*sizeof(BYTE));
			}
			//���ñ���
			LONG lTotalTimes[4];
			LONG lMaxTimes[4];
			
			//���㹱�׷ֺ������
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				GetTotalTimes(m_cbGongxian,i,lTotalTimes[i],lMaxTimes[i]);
			}

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�����Լ�
				if(i==wChairID) continue;
				GameEnd.lGameScore[wChairID] -= lTotalTimes[i]*m_pGameServiceOption->lCellScore;

			}
			if(GameEnd.lGameScore[wChairID]==0)
			{
				GameEnd.lGameScore[wChairID] -=m_pGameServiceOption->lCellScore*2*6 ;
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//��������
			tagScoreInfo ScoreInfo;
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

			//�޸Ļ���
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,0,ScoreInfo.ScoreKind);


			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�л�ׯ��
			m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_RF_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
            
            
			//��Ϸ����
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_RF_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
		
			StatusPlay.wBankerUser=m_wBankerUser;

			//ʤ����Ϣ
			StatusPlay.wWinCount=m_wWinCount;
			CopyMemory(StatusPlay.wWinOrder,m_wWinOrder,sizeof(StatusPlay.wWinOrder));
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//������Ϣ
			StatusPlay.wTurnWiner=m_wTurnWiner;
			StatusPlay.cbTurnCardCount=m_cbTurnCardCount;
			CopyMemory(StatusPlay.cbTurnCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));
			CopyMemory(StatusPlay.cbMagicCardData,m_cbMagicCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿���Ϣ
			CopyMemory(StatusPlay.cbHandCardCount,m_cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(StatusPlay.cbHandCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*m_cbHandCardCount[wChairID]);
			CopyMemory(StatusPlay.cbPartnerCardData,m_cbHandCardData[(wChairID+2)%GAME_PLAYER],sizeof(BYTE)*m_cbHandCardCount[(wChairID+2)%GAME_PLAYER]);

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
	case SUB_C_OUT_CARD:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0]))) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData,pOutCard->cbCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserPassCard(pUserData->wChairID);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			m_bTrustee[pUserData->wChairID]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

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
//�û�����
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	{
		m_wBankerUser=pIServerUserItem->GetChairID();
	}


	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if (bLookonUser==false) m_wBankerUser=INVALID_CHAIR;


	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;


	//����˿�
	BYTE cbMagicCardData[MAX_COUNT];
	m_GameLogic.MagicCardData(bCardData,bCardCount,cbMagicCardData);

	//�����ж�
	BYTE cbStarLevel;
	BYTE bCardType=m_GameLogic.GetCardType(cbMagicCardData,bCardCount,cbStarLevel);
	if (bCardType==CT_ERROR) return false;

	if(bCardType>=CT_BOMB)
	{
		m_cbGongxian[wChairID][cbStarLevel-4]++;
	}

	//�����ж�
	if (m_cbTurnCardCount!=0)
	{
		//�Ա��˿�
		if (m_GameLogic.CompareCard(m_cbMagicCardData,cbMagicCardData,m_cbTurnCardCount,bCardCount)==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_cbTurnCardCount=bCardCount;
	m_cbHandCardCount[wChairID]-=bCardCount;
	CopyMemory(m_cbTurnCardData,bCardData,sizeof(BYTE)*bCardCount);
	CopyMemory(m_cbMagicCardData,cbMagicCardData,sizeof(BYTE)*bCardCount);

	//ʤ���ж�
	if (m_cbHandCardCount[wChairID]==0) 
	{
		//�����û�
		m_wWinOrder[m_wWinCount++]=wChairID;

		//�û�ͳ��
		if (m_cbHandCardCount[(wChairID+2)%GAME_PLAYER]==0) 
			m_wCurrentUser=INVALID_CHAIR;
	}

	//�л��û�
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//�����û�
		m_wTurnWiner=wChairID;
		m_wCurrentUser=(wChairID+1)%GAME_PLAYER;

		//�û�����
		while (m_cbHandCardCount[m_wCurrentUser]==0) 
			m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	}

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=bCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,m_cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

	//��������
	WORD wHeadSize=sizeof(OutCard)-sizeof(OutCard.cbCardData);
	WORD wSendSize=wHeadSize+OutCard.cbCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_cbTurnCardCount==0)) return false;

	//�û��л�
	do
	{
		//�����û�
		m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;

		//����ж�
		if (m_wCurrentUser==m_wTurnWiner)
		{
			//���ñ���
			m_cbTurnCardCount=0;

			//�ӷ紦��
			if (m_cbHandCardCount[m_wTurnWiner]==0)
			{
				m_wCurrentUser=(m_wTurnWiner+2)%GAME_PLAYER;
			}
			break;
		}

		//��������
		if (m_cbHandCardCount[m_wCurrentUser]!=0) break;

	} while(true);

	//������Ϣ
	CMD_S_PassCard PassCard;
	PassCard.wPassCardUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.cbTurnOver=(m_cbTurnCardCount==0)?TRUE:FALSE;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//���㹱��
VOID CTableFrameSink::GetTotalTimes(BYTE cbGongxian[GAME_PLAYER][15],WORD wChairID,LONG &lTotalTimes,LONG &lMaxTimes)
{
	BYTE cbTempGongxian[15];
	CopyMemory(cbTempGongxian,cbGongxian[wChairID],sizeof(cbTempGongxian));
	//3�����Ǽ����� 3*4 ��5
	if(cbTempGongxian[0]>=3)
	{
		ASSERT((0+cbTempGongxian[0]-1)<15);
		cbTempGongxian[0+cbTempGongxian[0]-2]++;
		cbTempGongxian[0]=0;
	}
	//2�����Ǽ����� 
	if(cbTempGongxian[1]>=2)
	{
		ASSERT((1+cbTempGongxian[1]-1)<15);
		cbTempGongxian[1+cbTempGongxian[1]-1]++;
		cbTempGongxian[1]=0;
	}
	//2�����Ǽ�����
	if(cbTempGongxian[2]>=2)
	{		
		ASSERT((2+cbTempGongxian[2]-1)<15);
		cbTempGongxian[2+cbTempGongxian[2]-1]++;
		cbTempGongxian[2]=0;
	}
	//2��7�Ǽ�����	
	if(cbTempGongxian[3]>=2)
	{
		ASSERT((3+cbTempGongxian[3]-1)<15);
		cbTempGongxian[3+cbTempGongxian[3]-1]++;
		cbTempGongxian[3]=0;
	}
	//2��8�Ǽ�����	
	if(cbTempGongxian[4]>=2)
	{
		ASSERT((4+cbTempGongxian[4]-1)<15);
		cbTempGongxian[4+cbTempGongxian[4]-1]++;
		cbTempGongxian[4]=0;
	}
	//2��9�Ǽ�����	
	if(cbTempGongxian[5]>=2)
	{
		ASSERT((5+cbTempGongxian[5]-1)<15);
		cbTempGongxian[5+cbTempGongxian[5]-1]++;
		cbTempGongxian[5]=0;
	}
	//2��10�Ǽ�����	
	if(cbTempGongxian[6]>=2)
	{
		ASSERT((6+cbTempGongxian[6]-1)<15);
		cbTempGongxian[6+cbTempGongxian[6]-1]++;
		cbTempGongxian[6]=0;
	}
	//2��11�Ǽ�����	
	if(cbTempGongxian[7]>=2)
	{
		ASSERT((7+cbTempGongxian[7]-1)<15);
		cbTempGongxian[7+cbTempGongxian[7]-1]++;
		cbTempGongxian[7]=0;
	}
	//2��12�Ǽ�����	
	if(cbTempGongxian[8]>=2)
	{
		ASSERT((8+cbTempGongxian[8]-1)<15);
		cbTempGongxian[8+cbTempGongxian[8]-1]++;
		cbTempGongxian[8]=0;
	}
	//2��13�Ǽ�����	
	if(cbTempGongxian[9]>=2)
	{
		ASSERT((9+cbTempGongxian[9]-1)<15);
		cbTempGongxian[9+cbTempGongxian[9]-1]++;
		cbTempGongxian[9]=0;
	}
	//2��14�Ǽ�����	
	if(cbTempGongxian[10]>=2)
	{
		ASSERT((10+cbTempGongxian[10]-1)<15);
		cbTempGongxian[10+cbTempGongxian[10]-1]++;
		cbTempGongxian[10]=0;
	}
	//2��15�Ǽ�����	
	if(cbTempGongxian[11]>=2)
	{
		ASSERT((11+cbTempGongxian[11]-1)<15);
		cbTempGongxian[11+cbTempGongxian[11]-1]++;
		cbTempGongxian[11]=0;
	}
	//2��16�Ǽ�����	
	if(cbTempGongxian[12]>=2)
	{
		ASSERT((12+cbTempGongxian[12]-1)<15);
		cbTempGongxian[12+cbTempGongxian[12]-1]++;
		cbTempGongxian[12]=0;
	}
	//2��17�Ǽ�����	
	if(cbTempGongxian[13]==2)
	{
		ASSERT((13+cbTempGongxian[13]-1)<15);
		cbTempGongxian[13+cbTempGongxian[13]-1]++;
		cbTempGongxian[13]=0;
	}
	//��ʵ�������ȫ������һ��ѭ��д  Ϊ�����˿��ø���� �˶�����β���if
	//�㹱�׵�ʱ�򷵻����б��� ���ۼ�
	//��6����ʼ��

	//�ӵ�����
	lTotalTimes=0L;
	for(INT i=2;i<15;i++)
	{
		//���˹���Ϊ0
		if(cbTempGongxian[i]==0) continue;
		//�ۼ�
		LONG lTempTotalTimes = pow(2,i);
		if(lTempTotalTimes>lTotalTimes)
			lTotalTimes=lTempTotalTimes;
	}

	//����� ��5������
	lMaxTimes=1L;
	for(INT i=14;i>=1;i--)
	{
		//���˹���Ϊ0
		if(cbTempGongxian[i]==0) continue;
		//�ۼ�
		 LONG lTempMaxTimes=pow(2,i);
		 if(lTempMaxTimes>lMaxTimes)
			 lMaxTimes=lTempMaxTimes;
		 if(lMaxTimes>=16)
			 lMaxTimes=16;
	}

	//����Ҫ�Ӹ������� 
	CopyMemory(cbTempGongxian,cbGongxian[wChairID],sizeof(cbTempGongxian));
	INT lMinGongxian=0;
	for(INT i=0;i<15;i++)
	{
		//�����޹���
		if(cbTempGongxian[i]==0) continue;
		lMinGongxian=i;
		break;
	}
	for (INT i=14;i>lMinGongxian;i--)
	{
		if(cbTempGongxian[i]==0) continue;
		ASSERT(cbTempGongxian[lMinGongxian]!=0);
		cbTempGongxian[lMinGongxian]+=cbTempGongxian[i];
	}

	//�����4��
	if((lMinGongxian==0)&&(cbTempGongxian[lMinGongxian]>2))
	{
		LONG lTempTotalTimes=pow(2,cbTempGongxian[lMinGongxian]-2);
		if(lTempTotalTimes>lMaxTimes)
		{
			lMaxTimes=lTempTotalTimes;
			if(lMaxTimes>=16)
				lMaxTimes=16;
		}

	}
	if(lMinGongxian==1)
	{
		LONG lTempTotalTimes=pow(2,cbTempGongxian[lMinGongxian]-1);
		if(lTempTotalTimes>lMaxTimes)
		{
			lMaxTimes=lTempTotalTimes;
			if(lTempTotalTimes>=16)
				lMaxTimes=16;
		}

	}
	if(lMinGongxian>1)
	{
		LONG lTempTotalTimes=pow(2,cbTempGongxian[lMinGongxian]-1);
		if(lTempTotalTimes>lMaxTimes)
		{
			lMaxTimes=lTempTotalTimes;
			if(lMaxTimes>=16)
				lMaxTimes=16;
		}
		if(lTempTotalTimes>lTotalTimes)
		{
			lTotalTimes=lTempTotalTimes;
		}
	}
	
}
//////////////////////////////////////////////////////////////////////////
