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
	m_lCellScore=1L;
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_lGameScore,0,sizeof(m_lGameScore));

	//������Ϣ
	m_bTurnCardCount=0;
	m_bTurnCardType=CT_ERROR;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	ZeroMemory(m_bAutoPlayer,sizeof(m_bAutoPlayer));
	//�˿���Ϣ
	memset(m_bCardCount,0,sizeof(m_bCardCount));
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

	//��Ԫ����
	m_lCellScore=m_pGameServiceOption->lCellScore;

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//���ñ���
	if (bLookonUser==false)
	{
		m_bAutoPlayer[wChairID]=false;
	}

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��Ԫ����
	m_lCellScore=m_pGameServiceOption->lCellScore;

	//��Ϸ����
	m_cbBombCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_lGameScore,0,sizeof(m_lGameScore));

	//������Ϣ
	m_bTurnCardCount=0;
	m_bTurnCardType=CT_ERROR;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	ZeroMemory(m_bAutoPlayer,sizeof(m_bAutoPlayer));
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

	//�������
	if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
	{
		//��ͻ���
		LONG lLessScore=0L;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem!=NULL) 
			{
				LONG lUserScore=pIServerUserItem->GetUserScore()->lScore;
				if ((lLessScore==0)||(lLessScore>lUserScore)) lLessScore=lUserScore;
			}
		}

		//��Ԫ����
		/*LONG lCellScroe[]={1L,10L,100L,1000L,10000L,100000L};
		for (BYTE i=0;i<CountArray(lCellScroe);i++)
		{
			//�����ж�
			BYTE cbIndex=CountArray(lCellScroe)-i-1;
			if (lCellScroe[cbIndex]<m_lCellScore) break;

			//���û���
			if (lLessScore>=lCellScroe[cbIndex]*100L)
			{
				m_lCellScore=lCellScroe[cbIndex];
				break;
			}
		}*/

		m_lCellScore=lLessScore/100L;
	}

	//�����˿�
	BYTE bRandCard[42];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=17;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}

	//�����û�
	BYTE cbFirstCard[]={0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D};
	for (BYTE cbCardIndex=0;cbCardIndex<CountArray(cbFirstCard);cbCardIndex++)
	{
		for (WORD wUser=0;wUser<m_wPlayerCount;wUser++)
		{
			for (BYTE cbIndex=0;cbIndex<m_bCardCount[wUser];cbIndex++)
			{
				if (m_bHandCardData[wUser][cbIndex]==cbFirstCard[cbCardIndex])
				{
					//�����û�
					m_wBankerUser=wUser;
					m_wCurrentUser=wUser;

					goto SET_CURRENT_USER;
				}
			}
		}
	}

	//�����ж�
	ASSERT(FALSE);
	SET_CURRENT_USER:

	//�����˿�
	CMD_S_GameStart GameStart;
	GameStart.lBaseScore=m_lCellScore;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		CopyMemory(GameStart.cbCardData,m_bHandCardData[i],sizeof(GameStart.cbCardData));
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
			memset(&GameEnd,0,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ը������
			LONG lBombTime=1;
			for (BYTE i=0;i<m_cbBombCount;i++) lBombTime*=2;

			//ͳ�ƻ���
			LONG lGameTimes=lBombTime;
			if(m_bCardCount[(wChairID+1)%GAME_PLAYER]==17) lGameTimes*=3;
			else if(m_bCardCount[(wChairID+1)%GAME_PLAYER]>=11) lGameTimes*=2;

			GameEnd.lGameScore[wChairID]=m_lCellScore*lGameTimes*m_bCardCount[(wChairID+1)%GAME_PLAYER];
			GameEnd.lGameScore[(wChairID+1)%GAME_PLAYER]=-m_lCellScore*lGameTimes*m_bCardCount[(wChairID+1)%GAME_PLAYER];

			////���ֵ���
			//for (WORD i=0;i<m_wPlayerCount;i++)
			//{
			//	if (GameEnd.lGameScore[i]<0L)
			//	{
			//		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			//		if ((-GameEnd.lGameScore[i])>pIServerUserItem->GetUserScore()->lScore)
			//		{
			//			GameEnd.lGameScore[i]=-pIServerUserItem->GetUserScore()->lScore;
			//			GameEnd.lGameScore[(i+1)%GAME_PLAYER]=pIServerUserItem->GetUserScore()->lScore;
			//			break;
			//		}
			//	}
			//}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//д��
			for(int i=0;i<GAME_PLAYER;i++)
			{
				LONG lScore=GameEnd.lGameScore[i];
				LONG lRevenue=0;
				enScoreKind	ScoreKind;
				if(lScore>0)
				{
					ScoreKind=enScoreKind_Win;
					if(m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
						lRevenue=lScore*m_pGameServiceOption->wRevenue/100;
					lScore-=lRevenue;
				}
				else if(lScore==0) 
				{
					ScoreKind=enScoreKind_Draw;
				}
				else
				{
					ScoreKind=enScoreKind_Lost;
				}
				
				m_pITableFrame->WriteUserScore(i,lScore,lRevenue,ScoreKind);
			}

			//������Ϣ
			TCHAR szEndMessage[1024]=TEXT("");
			_sntprintf(szEndMessage,CountArray(szEndMessage),TEXT("���ֽ������ɼ�ͳ�ƣ�\n %s �� %d ��\n %s �� %d ��\n"),
				m_pITableFrame->GetServerUserItem(0)->GetAccounts(),GameEnd.lGameScore[0],
				m_pITableFrame->GetServerUserItem(1)->GetAccounts(),GameEnd.lGameScore[1]);

			//���ͽ�����Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szEndMessage,SMT_INFO);
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

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ͳ�ƻ���
			GameEnd.lGameScore[wChairID]=-50L*m_lCellScore;

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

			////�޸Ļ���
			//tagScoreInfo ScoreInfo;
			//ScoreInfo.ScoreKind=enScoreKind_Flee;
			//ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
			//m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

			//д��
			LONG lScore=GameEnd.lGameScore[wChairID];
			LONG lRevenue=0;
			enScoreKind	ScoreKind=enScoreKind_Flee;
			
			m_pITableFrame->WriteUserScore(wChairID,lScore,lRevenue,ScoreKind);

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
			StatusFree.lBaseScore=m_lCellScore;
			CopyMemory(StatusFree.bAutoStatus,m_bAutoPlayer,sizeof(StatusFree.bAutoStatus));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_RF_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.cbBombCount=m_cbBombCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_lCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.bAutoStatus,m_bAutoPlayer,sizeof(StatusPlay.bAutoStatus));

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
	case SUB_C_AUTOMATISM:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_Automatism));
			if (wDataSize!=sizeof(CMD_C_Automatism)) return false;
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			CMD_C_Automatism * pAutomatism =(CMD_C_Automatism *)pDataBuffer;

			m_bAutoPlayer[pUserData->wChairID]=pAutomatism->bAutomatism;

			CMD_S_UserAutomatism UserAutomatismData;
			CopyMemory(UserAutomatismData.bAutoStatus,m_bAutoPlayer,sizeof(m_bAutoPlayer));
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_AUTOMATISM,&UserAutomatismData,sizeof(UserAutomatismData));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_AUTOMATISM,&UserAutomatismData,sizeof(UserAutomatismData));

			////////////////////////////////////////////////////////////////
			//�����й���Ϣ
			TCHAR szMessage[256]=TEXT("");
			if(pAutomatism->bAutomatism==true) _sntprintf(szMessage,CountArray(szMessage),TEXT("��� [%s] ѡ�����йܹ��ܡ�"),pUserData->szAccounts);
			else _sntprintf(szMessage,CountArray(szMessage),TEXT("��� [%s] ȡ�����йܹ��ܡ�"),pUserData->szAccounts);

			for(int i=0; i<GAME_PLAYER; i++)
			{
				if(i==pUserData->wChairID) continue;
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
				if(pIServerUserItem!=NULL)
				{
					m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_INFO);
				}
			}

			//�й���ҵ���Ϣ
			if(pAutomatism->bAutomatism==true) _sntprintf(szMessage,CountArray(szMessage),TEXT("��ѡ�����йܹ��ܡ�"),pUserData->szAccounts);
			else _sntprintf(szMessage,CountArray(szMessage),TEXT("��ȡ�����йܹ��ܡ�"),pUserData->szAccounts);
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(pUserData->wChairID);
			if(pIServerUserItem!=NULL)
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_INFO);
			}
			////////////////////////////////////////////////////////////////
			return true;
		}
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

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardType=bCardType;
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//ը���ж�
	if (bCardType==CT_BOMB) m_cbBombCount++;

	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
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

//////////////////////////////////////////////////////////////////////////
