#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////
//��ʱ��
#define				IDI_BANK_TIME							1				//ѡׯ��ʱ��
/////////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));
	//������Ϣ
	m_bAnyFriendEnd=false;
	m_bTurnCardCount=0;
	m_wBankerUser=0;
	m_bWinKind=0;
	m_wPassCount=0;
	m_bResidualCardCount=54;
	memset(m_bCount1,0,sizeof(m_bCount1));
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	ZeroMemory(m_bTempCardData,sizeof(m_bTempCardData));
	//��ֵ���ʼ��
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_UserScoreCard[i].RemoveAll();
		m_UserScoreCard[i].FreeExtra();
	}
	m_bOneTurnScoreCard.RemoveAll();
	m_bOneTurnScoreCard.FreeExtra();
	m_wEndUsers.RemoveAll();
	m_wEndUsers.FreeExtra();
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
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_bAnyFriendEnd=false;
	m_bWinKind=0;
	m_bResidualCardCount=54;
	m_wPassCount=0;
	//������Ϣ
	m_bTurnCardCount=0;
	memset(m_bCount1,0,sizeof(m_bCount1));
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	//��ֵ���ʼ��
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_UserScoreCard[i].RemoveAll();
		m_UserScoreCard[i].FreeExtra();
	}
	m_bOneTurnScoreCard.RemoveAll();
	m_bOneTurnScoreCard.FreeExtra();
	m_wEndUsers.RemoveAll();
	m_wEndUsers.FreeExtra();
	//�˿���Ϣ
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	ZeroMemory(m_bTempCardData,sizeof(m_bTempCardData));
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
//���÷�����ʱ��
void CTableFrameSink::SetGameTimer(WORD wTimerID,UINT nElapse)
{
	nElapse=nElapse*1000;
	/*SetTimer(NULL,wTimerID,nElapse,NULL);
	MSG msg;
	msg.message=NULL;
	PeekMessage(&msg,NULL,NULL,NULL,FALSE);
	while(GetMessage(&msg,NULL,NULL,NULL))
	{
		if(msg.message==WM_TIMER)
		{
					SendCard();	
					break;
		}
	}
	KillTimer(NULL,wTimerID);*/
	Sleep(nElapse);
	SendCard();	
}
void CTableFrameSink::KillGameTimer(WORD wTimerID)
{
	KillTimer(NULL,wTimerID);
	return;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	//���÷ַ��˿�
	CopyMemory(m_bTempCardData,&m_GameLogic.m_bCardListData,sizeof(m_bTempCardData));

	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));
	//������Ϣ
	m_bAnyFriendEnd=false;
	m_bTurnCardCount=0;
	m_wBankerUser=0;
	m_bWinKind=0;
	m_wPassCount=0;
	m_bResidualCardCount=54;
	memset(m_bCount1,0,sizeof(m_bCount1));
	memset(m_bCount2,0,sizeof(m_bCount2));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	//����״̬
	m_pITableFrame->SetGameStatus(GS_RF_BANKER);

	//�����˿�
	BYTE bRandBankCard[GAME_PLAYER];
	m_GameLogic.RandBankCardList(bRandBankCard,CountArray(bRandBankCard));

	//�ַ��˿�
	BYTE bBankCard[GAME_PLAYER];
	ZeroMemory(bBankCard,sizeof(bBankCard));
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(!m_pITableFrame->GetServerUserItem(i))continue;
		CopyMemory(&bBankCard[i],&bRandBankCard[i],sizeof(BYTE));
		//m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}
	//��������
	BYTE wBanker;
	BYTE bData=m_GameLogic.GetCardValue(bBankCard[0]);
	for(BYTE j=0;j<GAME_PLAYER;j++)
	{
		if(!m_pITableFrame->GetServerUserItem(j))continue;
		BYTE bCardData=m_GameLogic.GetCardValue(bBankCard[j]);
		if(bCardData==0x01)
		{
			wBanker=j;
			break;
		}
		if(bData<bCardData)
		{
			bData=bCardData;
			wBanker=j;
		}

	}
	//����ׯ��
	m_wBankerUser=wBanker;
	//���õ�ǰ���
	m_wCurrentUser=wBanker;
	////�趨ÿһ��ׯ��
 //  m_wCurrentUser=(BYTE)(rand()%m_wPlayerCount);
 //  while(!m_pITableFrame->GetServerUserItem(m_wCurrentUser)||!CanBeBanker(m_wCurrentUser))
 //  {
	//srand((unsigned)time(NULL));
	//m_wCurrentUser=(BYTE)(rand()%m_wPlayerCount);
 //  }
 //  m_wFirstPlayer=m_wCurrentUser;

	//�����ж�
//	ASSERT(FALSE);
	//�����˿�
	CMD_S_Bank BankData;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(!m_pITableFrame->GetServerUserItem(i))continue;
		CopyMemory(&BankData.bCardData,bBankCard,sizeof(bBankCard));
		BankData.Moster=wBanker;
		m_pITableFrame->SendTableData(i,SUB_S_BANK,&BankData,sizeof(BankData));
		m_pITableFrame->SendLookonData(i,SUB_S_BANK,&BankData,sizeof(BankData));
	}
	//����ʱ��
	SetGameTimer(IDI_BANK_TIME,5);

	return true;
}
//����
void CTableFrameSink::SendCard()
{
	//������Ϸ״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);
	//ɾ����ʱ��
	KillGameTimer(IDI_BANK_TIME);

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(!m_pITableFrame->GetServerUserItem(i))continue;
	
		//�����˿���
		if(i==m_wBankerUser)
			m_bCardCount[i]=6;
		else
			m_bCardCount[i]=5;
		//��������
		GameStart.wBanker=m_wBankerUser;
		GameStart.wCurrentUser=m_wCurrentUser;
		//�����˿�
		m_GameLogic.RandCardList(GameStart.bCardData,m_bCardCount[i],m_bTempCardData,m_bResidualCardCount);
		CopyMemory(m_bHandCardData[i],&GameStart.bCardData,m_bCardCount[i]);
		m_GameLogic.SortCardList(GameStart.bCardData,m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
		//m_GameLogic.SortCardList(m_bTempCardData,m_bResidualCardCount+m_bCardCount[i]);

		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return ;

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

			//ͳ�ƻ���
			BYTE bPlayerCount=0;
			for(BYTE i=0;i<GAME_PLAYER;i++)
				if(m_pITableFrame->GetServerUserItem(i))
					bPlayerCount++;

			for(BYTE j=0;j<GAME_PLAYER;j++)
			{
				if(!m_pITableFrame->GetServerUserItem(j))continue;
				BYTE bFiveCount=0;
				for(BYTE k=0;k<m_UserScoreCard[j].GetSize();k++)
					if(m_GameLogic.GetCardValue(m_UserScoreCard[j].GetAt(k))==0x05)
						bFiveCount++;
				//�������
				GameEnd.lGameScore[j]=(bFiveCount*5+(m_UserScoreCard[j].GetSize()-bFiveCount)*10)*10-100/bPlayerCount*10;
			}


					
        	//ͳ���Ʒ�
		    LONG lCardScore[GAME_PLAYER];
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				LONG lScoreTimes=1;
				if (m_bCardCount[i]==10) lScoreTimes=4;
				else if (m_bCardCount[i]>=10) lScoreTimes=3;
				else if (m_bCardCount[i]>=8) lScoreTimes=2;
				lCardScore[i]=lScoreTimes*m_bCardCount[i];
			}

			//����˰��
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>1L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]*5/100L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*(100-5)/100;
					}
				}
			}

			//����ͳ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_lAllTurnScore[i]+=GameEnd.lGameScore[i];
				m_lLastTurnScore[i]=GameEnd.lGameScore[i];
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
			GameEnd.lGameScore[wChairID]=-200L*m_pGameServiceOption->lCellScore;

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
	case GS_RF_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

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
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);

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

	case SUB_C_JIU://����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			return true;

		}
	case SUB_C_DISPLAYTEN:
		{
			//��������
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			CMD_C_DisplayTen * pDisplayTen=(CMD_C_DisplayTen *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_DisplayTen)-sizeof(pDisplayTen->bDisplayStatus);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
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


//�ж��ĸ����û���ʤ
int CTableFrameSink::GetSpecialWinKind()
{
	BYTE bSWin=1;
	BYTE bDogFall=0;
	int bCWin=-1;

return 0;

}
//��Ϸ�Ƿ����
bool CTableFrameSink::IsGameOver()
{

	return false;
}
//����
void CTableFrameSink::RecruitCard(WORD wChairID)
{
	BYTE bUserOweCardCount;
	//����
	//��������
	CMD_S_RecruitCard RecruitCard;
	ZeroMemory(&RecruitCard,sizeof(RecruitCard));
	//����һ�ַ�ֵ��
	for(BYTE j=0;j<m_bOneTurnScoreCard.GetSize();j++)
		RecruitCard.bOneTurnScoreCard[j]=m_bOneTurnScoreCard.GetAt(j);
	RecruitCard.bOneTurnScoreCardCount=m_bOneTurnScoreCard.GetSize();
	//����
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		bUserOweCardCount=5-m_bCardCount[i];
		if(!m_pITableFrame->GetServerUserItem(i))continue;
		RecruitCard.wCurrentUser=wChairID;	
		BYTE * bUserOweCardData=new BYTE [bUserOweCardCount];
		ZeroMemory(bUserOweCardData,sizeof(bUserOweCardData));
		m_GameLogic.RandCardList(bUserOweCardData,bUserOweCardCount,m_bTempCardData,m_bResidualCardCount);
		//��
		m_GameLogic.CombinationArray(m_bHandCardData[i],m_bCardCount[i],bUserOweCardData,bUserOweCardCount);
		m_bCardCount[i]+=bUserOweCardCount;
		RecruitCard.bRecruitCount=bUserOweCardCount;
		CopyMemory(&RecruitCard.bRecruitCard,bUserOweCardData,sizeof(BYTE)*bUserOweCardCount);
		m_pITableFrame->SendTableData(i,SUB_S_RECRUIT_CARD,&RecruitCard,sizeof(CMD_S_RecruitCard));

	}
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

	//���ñ���
	m_wPassCount=0;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	////�����ж�
	//bool bMostest=false;
	//if ((bCardCount<=2)&&(bCardData[0]==0x42)) bMostest=true;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);
	//��ֵ��¼
	for(WORD j=0;j<m_bTurnCardCount;j++)
	{
		BYTE m_bTemp1=m_GameLogic.GetCardValue(m_bTurnCardData[j]);
		if(m_bTemp1==0x05||m_bTemp1==0x0A||m_bTemp1==0x0D)
			m_bOneTurnScoreCard.Add(m_bTurnCardData[j]);
	}

	BYTE i=wChairID+1;
	while(!m_bCardCount[(i%m_wPlayerCount)])
	{
		i++;
	}

	//���ñ���
	m_wCurrentUser=i%m_wPlayerCount;
	m_wPassCount=i-wChairID-1;

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
	/*if (bMostest==true) m_bTurnCardCount=0;*/

	


	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return false;
	//���ñ���
	BYTE i=wChairID+1;
	while(!m_bCardCount[(i%m_wPlayerCount)])
	{
		i++;
	}

	//���ñ���
	m_wCurrentUser=i%m_wPlayerCount;
	m_wPassCount+=i-wChairID;

	if(m_wPassCount>=m_wPlayerCount-1)
	{
		ZeroMemory(m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
		m_bTurnCardCount=0;
		if(m_bResidualCardCount)
			RecruitCard(m_wCurrentUser);
	}
	//��һ�����÷�����
	if(!m_bTurnCardCount)
		for(BYTE i=0;i<m_bOneTurnScoreCard.GetSize();i++)
			m_UserScoreCard[m_wCurrentUser].Add(m_bOneTurnScoreCard.GetAt(i));

	//����һ�ַ�ֵ������
	m_bOneTurnScoreCard.RemoveAll();
	m_bOneTurnScoreCard.FreeExtra();

	
	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	//�����ж�
	BYTE bScoreCardCount=0;
	for(BYTE j=0;j<GAME_PLAYER;j++)
	{
		bScoreCardCount+=m_UserScoreCard[j].GetSize();

	}
	if(bScoreCardCount==12)
		OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//////////////////////////////////////////////////////////////////////////
