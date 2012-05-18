#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_pITableFrame=NULL;
	pGameServiceOption=NULL;
	//����A��ʼ��
	m_wBanker=INVALID_CHAIR;
	memset(m_dwUserID,0,sizeof(m_dwUserID));

	m_bLastEndReason =0xff;

	memset(m_bIsCanLook,FALSE,sizeof(m_bIsCanLook));

	InitVariable();
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

void	CTableFrameSink::InitVariable()
{
	
	memset(m_bUserCard,0,sizeof(m_bUserCard));
	memset(m_bUserCardCnt,0,sizeof(m_bUserCardCnt));
	memset(m_bOutCard,0,sizeof(m_bOutCard));
	memset(m_bOutCardCnt,0,sizeof(m_bOutCardCnt));
	memset(m_bScoreCard,0,sizeof(m_bScoreCard));

	memset(m_bCurPublicCard,0,sizeof(m_bCurPublicCard));
	m_bCurPublicCardCnt=0;


	m_wCurOutCard=INVALID_CHAIR;
	m_wPublic=INVALID_CHAIR;

	//���
	m_bTempCnt  =0;
	m_bOutHandCnt=0;				//����������
	memset(m_bLastOutCardCnt,0,sizeof(m_bLastOutCardCnt));
	memset(m_bLastOutCard,0,sizeof(m_bLastOutCard));

	memset(m_bPublicCard,0,sizeof(m_bPublicCard));
	memset(m_bPublicCardCnt,0,sizeof(m_bPublicCardCnt));

	memset(m_bScoreCardCnt,0,sizeof(m_bScoreCardCnt));
	memset(m_nScoreValue,0,sizeof(m_nScoreValue));
	m_wFirstOut=INVALID_CHAIR;

	m_bCurPublicFlower =0xf0;
	m_bFirstCardCnt=0;
	memset(m_bFirstCard,0,sizeof(m_bFirstCard));


	m_wBanker=INVALID_CHAIR;
	//����ǿ�������¿�ʼ
	if(m_bLastEndReason ==GER_USER_LEFT)
	{
		memset(m_dwUserID,0,sizeof(m_dwUserID));
		m_bLastEndReason =0xff;
	}
	//////////////////
	m_bIsHaveScore=FALSE;
	m_bMaxCardCnt=0;
	memset(m_bMaxCard,0,sizeof(m_bMaxCard));
}

//��ʱ�����
WORD CTableFrameSink::GetNextSeat(WORD wSeat)
{
	return (wSeat+3)%GAME_PLAYER;
}
//˳ʱ�뻻ׯ
WORD   CTableFrameSink::GetNextBanker(WORD wBanker)
{
	return (wBanker+1)%GAME_PLAYER;
}

WORD CTableFrameSink::GetWiner(WORD wFirstOut)
{
	BYTE * pUserOutCard[4];
	for (int i=0;i<4;i++) pUserOutCard[i]=m_bOutCard[(wFirstOut+4-i)%4];
	int iWiner=(-1*m_Logic.CompareCard(pUserOutCard,m_bOutCardCnt[0])+4+wFirstOut)%4;
	return iWiner;
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
	pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(pGameServiceOption!=NULL);
	
	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	InitVariable();
	return;
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return enStartMode_FullReady;
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
	m_pITableFrame->SetGameStatus(GS_SEND_CARD);

	//�Ƿ���Ҫ��ԭ
	bool bIsBack=false; 
	//��¼֮
	if(m_dwUserID[0] ==0)
	{
		for(int i=0;i<GAME_PLAYER;i++)
		{
			m_dwUserID[i] = m_pITableFrame->GetServerUserItem(i)->GetUserID();
		}
	}
	else
	{
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_dwUserID[i] != m_pITableFrame->GetServerUserItem(i)->GetUserID())
			{
				bIsBack=true;
				m_bIsCanLook[i] =FALSE;
			}
		}
	}
	//��Ϸ���¿�ʼ
	if(bIsBack)  
	{
		//����A��ʼ��
		m_wBanker=INVALID_CHAIR;
		for(int i=0;i<GAME_PLAYER;i++)
		{
			m_dwUserID[i] = m_pITableFrame->GetServerUserItem(i)->GetUserID();
		}
	}
	//ϴ��
	BYTE bAllCards[TOTAL_CARDS_CNT];
	m_Logic.RandCard(bAllCards,TOTAL_CARDS_CNT);

	int i=0;
	for(i=0;i<GAME_PLAYER;i++)
	{
		m_bUserCardCnt[i] = PLAYER_CARD_CNT;
		memcpy(m_bUserCard[i],&bAllCards[i*PLAYER_CARD_CNT],PLAYER_CARD_CNT);
	}

	ServerBeginSend  begin;
	memset(&begin,0,sizeof(begin));
	begin.bIsResend =FALSE;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		memcpy(begin.bAllCard,m_bUserCard[i],PLAYER_CARD_CNT);
		begin.bIsCanLook = m_bIsCanLook[i];
		//��Ϸ��ʼ��Ϣ
		m_pITableFrame->SendTableData(i,SUB_SC_BEGIN_SEND,&begin,sizeof(ServerBeginSend));
		m_pITableFrame->SendLookonData(i,SUB_SC_BEGIN_SEND,&begin,sizeof(ServerBeginSend));
	}
	//����
	//11��23��2005����Ϊ�ͻ��˷��˿�
	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return GameEnd(wChairID,GER_USER_LEFT);
}

bool	CTableFrameSink::GameEnd(WORD wSeat,BYTE bEndReason)
{
	m_bLastEndReason =bEndReason;
	switch(bEndReason)
	{
	case GER_USER_LEFT:
		{

			ServerGameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.bIsForceExit=TRUE;
			for(int i=0;i<GAME_PLAYER;i++)
			{
				if(i==wSeat)
				{
					GameEnd.iScore[i]=-60*pGameServiceOption->lCellScore;
				}
				else
				{
					GameEnd.iScore[i]=20*pGameServiceOption->lCellScore;
				}
			}
			int iScore[GAME_PLAYER]={0};
			memcpy(iScore,GameEnd.iScore,sizeof(iScore));
			for(int i=0;i<GAME_PLAYER;++i)
			{
				GameEnd.iScore[i] = iScore[i];
			}
			//ȷ����ׯ�ң������ÿͻ�����ʾ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_SC_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_SC_GAME_END,&GameEnd,sizeof(GameEnd));

			tagScoreInfo	ScoreInfo_[GAME_PLAYER];

			for(int i=0;i<GAME_PLAYER;i++)
			{
				ScoreInfo_[i].lScore =iScore[i];
				if(ScoreInfo_[i].lScore<0)
				{
					ScoreInfo_[i].ScoreKind=enScoreKind_Flee;
				}
				else if(ScoreInfo_[i].lScore==0)
				{
					ScoreInfo_[i].ScoreKind=enScoreKind_Draw;
				}
				else
				{
					ScoreInfo_[i].ScoreKind=enScoreKind_Win;
				}

				m_pITableFrame->WriteUserScore(i,ScoreInfo_[i].lScore,0,ScoreInfo_[i].ScoreKind);
			}

			//��ɢ��Ϸ
			m_pITableFrame->ConcludeGame();
			return true;
		}
	case	GER_NORMAL:
		{

			ServerGameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.bIsForceExit=FALSE;
			for(int i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.iScore[i]=(m_nScoreValue[i]-60)*pGameServiceOption->lCellScore;
			}
			int iScore[GAME_PLAYER]={0};
			memcpy(iScore,GameEnd.iScore,sizeof(iScore));
			for(int i=0;i<GAME_PLAYER;++i)
			{
				GameEnd.iScore[i] = iScore[i];
			}
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_SC_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_SC_GAME_END,&GameEnd,sizeof(GameEnd));

			tagScoreInfo	ScoreInfo_[GAME_PLAYER];
			for(int i=0;i<GAME_PLAYER;i++)
			{
				ScoreInfo_[i].lScore=iScore[i];	
				if(ScoreInfo_[i].lScore>0)
				{
					ScoreInfo_[i].ScoreKind=enScoreKind_Win;
				}
				else if(ScoreInfo_[i].lScore==0)
				{
					ScoreInfo_[i].ScoreKind=enScoreKind_Draw;
				}
				else
				{
					ScoreInfo_[i].ScoreKind=enScoreKind_Lost;
				}

				m_pITableFrame->WriteUserScore(i,ScoreInfo_[i].lScore,0,ScoreInfo_[i].ScoreKind);
			}

			m_pITableFrame->ConcludeGame();

			return true;
		}
	default:
		{
			return false;
		}
	}
}
//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	//Ч����
	switch(bGameStatus)
	{
	case GS_FREE:
		{
			return m_pITableFrame->SendGameScene(pIServerUserItem,NULL,0);
		}
	case GS_SEND_CARD:
		{

			GS_SendStruct StatusStruct;
			memset(&StatusStruct,0,sizeof(StatusStruct));
			StatusStruct.wBanker =m_wBanker;
			StatusStruct.bIsCanLook =m_bIsCanLook[wChiarID];
			StatusStruct.wPublic =m_wPublic;
			StatusStruct.bPublicCardFlower=m_bCurPublicFlower;
			StatusStruct.bPublicCardCnt =m_bCurPublicCardCnt;
			memcpy(StatusStruct.bPublicCard,m_bCurPublicCard,m_bCurPublicCardCnt);
	

			StatusStruct.bAllMeCardCnt= m_bUserCardCnt[wChiarID];
			memcpy(StatusStruct.bAllMeCard,m_bUserCard[wChiarID],m_bUserCardCnt[wChiarID]);

			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusStruct,sizeof(StatusStruct));

		}
	case GS_WAIT_PUBLIC:
		{
			GS_WaitStruct StatusStruct;
			memset(&StatusStruct,0,sizeof(StatusStruct));
			StatusStruct.bIsCanLook =m_bIsCanLook[wChiarID];
			StatusStruct.wBanker =m_wBanker;
			StatusStruct.wPublic =m_wPublic;
			StatusStruct.bPublicCardFlower=m_bCurPublicFlower;
			StatusStruct.bPublicCardCnt =m_bCurPublicCardCnt;
			memcpy(StatusStruct.bPublicCard,m_bCurPublicCard,m_bCurPublicCardCnt);
			
			StatusStruct.bMeCardCnt= m_bUserCardCnt[wChiarID];
			memcpy(StatusStruct.bMeCard,m_bUserCard[wChiarID],m_bUserCardCnt[wChiarID]);

			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusStruct,sizeof(StatusStruct));
		}
	case GS_OUT_CARD:
		{
			GS_OutStruct StatusStruct;
			memset(&StatusStruct,0,sizeof(StatusStruct));
			StatusStruct.bIsCanLook =m_bIsCanLook[wChiarID];
			StatusStruct.wBanker =m_wBanker;
			StatusStruct.wThrowSeat =m_wFirstOut;
			StatusStruct.wPublic =m_wPublic;
			StatusStruct.wCurSeat =m_wCurOutCard;

			StatusStruct.bPublicCardCnt=m_bCurPublicCardCnt;
			memcpy(StatusStruct.bPublicCard,m_bCurPublicCard,sizeof(m_bCurPublicCard));

			memcpy(StatusStruct.bScoreCardCnt,m_bScoreCardCnt,sizeof(m_bScoreCardCnt));
			memcpy(StatusStruct.bScoreCard,m_bScoreCard,sizeof(m_bScoreCard));

			memcpy(StatusStruct.nCurScore,m_nScoreValue,sizeof(m_nScoreValue));

			StatusStruct.bPublicCardFlower=m_bCurPublicFlower;
			StatusStruct.bFirstCardCnt =m_bFirstCardCnt;
			memcpy(StatusStruct.bFirstCard,m_bFirstCard,m_bFirstCardCnt);

			
			StatusStruct.bMeCardCnt= m_bUserCardCnt[wChiarID];
			memcpy(StatusStruct.bMeCard,m_bUserCard[wChiarID],m_bUserCardCnt[wChiarID]);

			memcpy(StatusStruct.bOutCardCnt,m_bOutCardCnt,sizeof(m_bOutCardCnt));
			memcpy(StatusStruct.bOutCard,m_bOutCard,sizeof(m_bOutCard));

			memcpy(StatusStruct.bLastOutCardCnt,m_bLastOutCardCnt,sizeof(m_bLastOutCardCnt));
			memcpy(StatusStruct.bLastOutCard,m_bLastOutCard,sizeof(m_bLastOutCard));
			StatusStruct.bOutHandCnt=m_bOutHandCnt;
			StatusStruct.bIsHaveScore=m_bIsHaveScore;
			StatusStruct.bMaxCardCnt= m_bMaxCardCnt;
			memcpy(StatusStruct.bMaxCard,m_bMaxCard,m_bMaxCardCnt);
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusStruct,sizeof(StatusStruct));
		}
	default:
		{
			return false;
		}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

void	CTableFrameSink::ResendCard()
{
	m_bTempCnt  =0;
	m_pITableFrame->SetGameStatus(GS_SEND_CARD);

	if(m_wBanker != INVALID_CHAIR)
	{
		m_wBanker = GetNextBanker(m_wBanker);
		
	}
	//ϴ��
	BYTE bAllCards[TOTAL_CARDS_CNT];
	m_Logic.RandCard(bAllCards,TOTAL_CARDS_CNT);
	int i=0;
	for(i=0;i<GAME_PLAYER;i++)
	{
		m_bUserCardCnt[i] = PLAYER_CARD_CNT;
		memcpy(m_bUserCard[i],&bAllCards[i*PLAYER_CARD_CNT],PLAYER_CARD_CNT);
	}

#ifdef	TEST1
	m_bUserCard[0][0] =0x23;
	m_bUserCard[0][1] =0x23;
	m_bUserCard[0][2] =0x24;
	m_bUserCard[0][3] =0x24;
	m_bUserCard[0][4] =0x41;
	m_bUserCard[0][5] =0x41;

	m_bUserCard[1][0] =0x16;
	m_bUserCard[1][1] =0x16;
	m_bUserCard[1][2] =0x42;
	m_nCurBout =10;
	m_wBanker =0;
#endif


	ServerBeginSend  begin;
	memset(&begin,0,sizeof(begin));
	begin.bIsResend =TRUE;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		memcpy(begin.bAllCard,m_bUserCard[i],PLAYER_CARD_CNT);
		begin.bIsCanLook =m_bIsCanLook[i];
		//��Ϸ��ʼ��Ϣ
		m_pITableFrame->SendTableData(i,SUB_SC_BEGIN_SEND,&begin,sizeof(ServerBeginSend));
		m_pITableFrame->SendLookonData(i,SUB_SC_BEGIN_SEND,&begin,sizeof(ServerBeginSend));
	}

}




//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch(wSubCmdID)
	{

	case SUB_CS_LOOK_INFO:
		{
			if(wDataSize != sizeof(ClientLookInfo)) return false;
			ClientLookInfo *p =(ClientLookInfo *)pDataBuffer;
			WORD wSeat =pIServerUserItem->GetChairID();
			m_bIsCanLook[wSeat]=p->bIsCanLook;
			return true;
		}
	case SUB_NT_CARD:
		{
			if(wDataSize != sizeof(ClientNtStruct)) return false;
			ClientNtStruct *pNT =(ClientNtStruct *)pDataBuffer;
			WORD wSeat =pIServerUserItem->GetChairID();

			return SubNTCard(pNT,wSeat);
		}
	case SUB_CS_SEND_FINISH:
		{
			m_bTempCnt ++;
			//������ҷ���������
			if(m_bTempCnt ==GAME_PLAYER)
			{
				m_pITableFrame->SetGameStatus(GS_WAIT_PUBLIC);
				//�˿�����
				for(int i=0;i<GAME_PLAYER;i++)
				{
					m_Logic.SortCard(m_bUserCard[i],NULL,m_bUserCardCnt[i]);
				}
				m_bTempCnt =0;
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_SC_SEND_FINISH);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_SC_SEND_FINISH);
			}
			return true;

		}
	case SUB_CS_FINISH_ASHIBIT:
		{
			m_bTempCnt ++;
			WORD wSeat =pIServerUserItem->GetChairID();
			//������Ҷ��Ѿ�׼������
			if(m_bTempCnt ==GAME_PLAYER)
			{
				m_bTempCnt =0;
				//û������
				if(m_wPublic ==INVALID_CHAIR)
				{
					//���·���
					ResendCard();
				}
				else 
				{
					m_wBanker =m_wPublic;	
					//ׯ�Ҹ���֮�����°�������ɫ����
					m_Logic.SetNTHuaKind(m_bCurPublicFlower);
					for(int i=0;i<GAME_PLAYER;i++)
					{
						m_Logic.SortCard(m_bUserCard[i],NULL,m_bUserCardCnt[i]);
					}
					m_pITableFrame->SetGameStatus(GS_OUT_CARD);
					BeginOutCard();
				}
			}
			return true;
		}
	case	SUB_OUT_CARD:
		{
			ClientOutCard *pCards =(ClientOutCard *)pDataBuffer;
			return SubOutCard(pCards->wSeat,pCards->bOutCard,pCards->bOutCardCnt);
		}
	default:
		{
			return false;
		}
	}
	return false;
}

bool  CTableFrameSink::SubNTCard(ClientNtStruct *pNT,WORD wSeat)
{
	
	//Ч������
	if(!(pNT->bCardCount>m_bCurPublicCardCnt ||(pNT->bCardCount==m_bCurPublicCardCnt&&m_bCurPublicCardCnt==2&&\
		pNT->bCard[0]>m_bCurPublicCard[0])))
	{
		return true;
	}
	//�趨����
	m_bCurPublicFlower =m_Logic.GetCardHuaKind(pNT->bCard[0],true);
	if(pNT->bCard[0] == 0x4f)
	{
		m_bCurPublicFlower = 0x50;
	}

	//��¼��������

	ServerNtStruct NTCards;
	::memset(&NTCards,0,sizeof(NTCards));
	NTCards.wNTSeat=wSeat;
	NTCards.bCardCount=pNT->bCardCount;
	NTCards.bHuaKind=m_bCurPublicFlower;
	memcpy(NTCards.bCard,pNT->bCard,pNT->bCardCount);

	m_wPublic=wSeat;
	//��¼����ճ����������
	m_bPublicCardCnt[wSeat]=pNT->bCardCount;
	memcpy(m_bPublicCard[wSeat],pNT->bCard,m_bPublicCardCnt[wSeat]);

	m_bCurPublicCardCnt=pNT->bCardCount;
	memcpy(m_bCurPublicCard,pNT->bCard,sizeof(m_bCurPublicCard));

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_NT_CARD,&NTCards,sizeof(NTCards));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_NT_CARD,&NTCards,sizeof(NTCards));

	return true;

}


bool  CTableFrameSink::SubOutCard(WORD wOutSeat,BYTE bOutCards[],BYTE bOutCardsCnt)
{

	m_bOutHandCnt++;
	bool bIsThrowCard =false;
	//����ǵ�һ��������,�ж��Ƿ���˦��
	if(m_bOutHandCnt%GAME_PLAYER ==1)
	{
		m_Logic.SortCard(bOutCards,NULL,bOutCardsCnt);
		BYTE iOutCardKind=m_Logic.GetCardShape(bOutCards,bOutCardsCnt);
		if (iOutCardKind==UG_ERROR_KIND) return FALSE;

		if (iOutCardKind==UG_SHOW_CARD)	 //�ж�˦��
		{

			int iResultCount=0;
			BYTE iResultCard[39];
			BYTE * iCardArray[GAME_PLAYER];
			for (int i=0;i<GAME_PLAYER;i++) iCardArray[i]=m_bUserCard[i];
			if (m_Logic.CanShowCard(bOutCards,bOutCardsCnt,wOutSeat,iCardArray,m_bUserCardCnt[0],iResultCard,iResultCount)==FALSE)
			{
				//˦�Ʋ��ɹ�,�����´������
				bIsThrowCard =true;
				//�������ݰ�
				ServerThrowCard ShowCardFalse;
				ShowCardFalse.wNextSeat =GetNextSeat(wOutSeat);
				ShowCardFalse.iCardCount=bOutCardsCnt;
				ShowCardFalse.iResultCount=iResultCount;
				ShowCardFalse.bDeskStation=wOutSeat;
				::CopyMemory(ShowCardFalse.iCardList,bOutCards,bOutCardsCnt);
				::CopyMemory(&ShowCardFalse.iCardList[bOutCardsCnt],iResultCard,iResultCount);
				//��������
				bOutCardsCnt=iResultCount;
				::CopyMemory(bOutCards,iResultCard,sizeof(BYTE)*iResultCount);
				//������Ϣ
				int iSendSize=sizeof(ShowCardFalse)-sizeof(ShowCardFalse.iCardList)
					+sizeof(BYTE)*(ShowCardFalse.iResultCount+ShowCardFalse.iCardCount);

				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_SC_THROW_CARD,&ShowCardFalse,iSendSize);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_SC_THROW_CARD,&ShowCardFalse,iSendSize);

			}
		}
	}
	//��������һ��
	if(m_bUserCardCnt[wOutSeat] ==bOutCardsCnt)
	{
		//��Ϸ����

		memset(m_bOutCardCnt,bOutCardsCnt,sizeof(m_bOutCardCnt));
		for(int i=0;i<GAME_PLAYER;i++)
		{
			memcpy(m_bOutCard[i],m_bUserCard[i],bOutCardsCnt);
		}

		memset(m_bUserCardCnt,0,sizeof(m_bUserCardCnt));
		memset(m_bUserCard,0,sizeof(m_bUserCard));

		//����ת��ȫ���˿�
		ServerBatchOut BatchOut;
		memset(BatchOut.bOutCardCnt,bOutCardsCnt,sizeof(BatchOut.bOutCardCnt));
		memcpy(BatchOut.bOutCard,m_bOutCard,sizeof(BatchOut.bOutCard));
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_SC_BATCH_OUT,&BatchOut,sizeof(BatchOut));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_SC_BATCH_OUT,&BatchOut,sizeof(BatchOut));

		m_wCurOutCard =GetWiner(wOutSeat);

		SendScoreCard(true);
		//��Ϸ��������
		GameEnd(m_wCurOutCard,GER_NORMAL);

	}
	else
	{
		//��һ��������
		if(m_bOutHandCnt%GAME_PLAYER ==1)
		{
			m_bFirstCardCnt =bOutCardsCnt;
			memcpy(m_bFirstCard,bOutCards,m_bFirstCardCnt);
			//��¼���ȳ�����
			m_wFirstOut =wOutSeat;
			//������һ�ֳ���
			if(m_bOutCardCnt[0])
			{
				memcpy(m_bLastOutCard[m_bOutHandCnt/GAME_PLAYER-1],m_bOutCard,sizeof(m_bOutCard));
				memcpy(m_bLastOutCardCnt[m_bOutHandCnt/GAME_PLAYER-1],m_bOutCardCnt,sizeof(m_bOutCardCnt));
			}
			memset(m_bOutCard,0,sizeof(m_bOutCard));
			memset(m_bOutCardCnt,0,sizeof(m_bOutCardCnt));

		}

		if(m_Logic.RemoveCard(bOutCards,bOutCardsCnt,m_bUserCard[wOutSeat],m_bUserCardCnt[wOutSeat])==0)
		{
			ASSERT(FALSE);
			return false;
		}
		m_bUserCardCnt[wOutSeat] -=bOutCardsCnt;

		//��¼��������
		m_bOutCardCnt[wOutSeat]=bOutCardsCnt;
		memcpy(m_bOutCard[wOutSeat],bOutCards,bOutCardsCnt);

		ServerOutCard OutCard;
		memset(&OutCard,0,sizeof(OutCard));
		OutCard.bOutCardCnt =bOutCardsCnt;
		OutCard.wSeat =wOutSeat;
		memcpy(OutCard.bOutCard,bOutCards,bOutCardsCnt);
		//һ�ֽ���
		if(m_bOutHandCnt%GAME_PLAYER ==0)
		{
			m_bIsHaveScore=FALSE;
			OutCard.bIsLastOut =TRUE;
			WORD wFirstOutSeat =GetNextSeat(wOutSeat);
			OutCard.wNextSeat = GetWiner(wFirstOutSeat);
			//��ԭ�˿�
			m_bFirstCardCnt=0;
			memset(m_bFirstCard,0,sizeof(m_bFirstCard));
			m_bMaxCardCnt=0;
			memset(m_bMaxCard,0,sizeof(m_bMaxCard));
			//���ȳ�����û��
			m_wFirstOut=INVALID_CHAIR;
		}
		else
		{
			if(!m_bIsHaveScore)
			{
				m_bIsHaveScore=m_Logic.IsHaveScore(bOutCards,bOutCardsCnt);
			}
			if(m_Logic.CompareCardEx(bOutCards,bOutCardsCnt,m_bMaxCard,m_bMaxCardCnt))
			{
				m_bMaxCardCnt=bOutCardsCnt;
				memcpy(m_bMaxCard,bOutCards,bOutCardsCnt);
			}
			OutCard.bIsLastOut =FALSE;
			OutCard.wNextSeat=GetNextSeat(wOutSeat);
		}

		m_wCurOutCard =OutCard.wNextSeat;
		if(!bIsThrowCard)
		{
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_OUT_CARD,&OutCard,sizeof(OutCard)-(sizeof(OutCard.bOutCard)-bOutCardsCnt*sizeof(OutCard.bOutCard[0])));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_OUT_CARD,&OutCard,sizeof(OutCard)-(sizeof(OutCard.bOutCard)-bOutCardsCnt*sizeof(OutCard.bOutCard[0])));
		}
		//һ�ֽ���ͳ�Ʒ���(ׯ��һ��Ӯ)
		if(m_bOutHandCnt%GAME_PLAYER ==0)
		{
			SendScoreCard(true);
		}

	}
	return true;

}

void	CTableFrameSink::SendScoreCard(bool bIsLastRound)
{
	BYTE bCards[TOTAL_CARDS_CNT]={0};
	BYTE bCardsCnt=0;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(!bIsLastRound)
		{
			memcpy(&bCards[bCardsCnt],m_bLastOutCard[m_bOutHandCnt/GAME_PLAYER][i],m_bLastOutCardCnt[m_bOutHandCnt/GAME_PLAYER][i]);
			bCardsCnt += m_bLastOutCardCnt[m_bOutHandCnt/GAME_PLAYER][i];
		}
		else
		{
			memcpy(&bCards[bCardsCnt],m_bOutCard[i],m_bOutCardCnt[i]);
			bCardsCnt += m_bOutCardCnt[i];
		}
	}
	BYTE bScoreCards[PLAYER_CARD_CNT]={0};
	BYTE bScoreCardsCnt=0;
	BYTE bScoreValue=0;
	bScoreValue =m_Logic.GetScore(bCards,bCardsCnt,bScoreCards,bScoreCardsCnt);
	if(bScoreValue !=0)
	{
		m_nScoreValue[m_wCurOutCard] += bScoreValue;
		memcpy(&m_bScoreCard[m_wCurOutCard][m_bScoreCardCnt[m_wCurOutCard]],bScoreCards,bScoreCardsCnt);
		m_bScoreCardCnt[m_wCurOutCard] += bScoreCardsCnt;

		ServerGamePoint  ScoreCard;
		memset(&ScoreCard,0,sizeof(ScoreCard));
		ScoreCard.wWiner=m_wCurOutCard;
		ScoreCard.bScoreValue = bScoreValue;
		ScoreCard.bScoreCardCnt=bScoreCardsCnt;
		memcpy(ScoreCard.bScoreCard,bScoreCards,bScoreCardsCnt);
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_SC_GAME_POINT,&ScoreCard,sizeof(ScoreCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_SC_GAME_POINT,&ScoreCard,sizeof(ScoreCard));
	}
}
void	CTableFrameSink::BeginOutCard()
{
	m_wCurOutCard =m_wBanker;
	m_wFirstOut = m_wBanker;
 	m_pITableFrame->SetGameStatus(GS_OUT_CARD);
	ServerBeginOut OutCard;
	OutCard.wFirstOutSeat =m_wBanker;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_SC_BEGIN_OUT,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_SC_BEGIN_OUT,&OutCard,sizeof(OutCard));

#ifdef _DEBUG
	CString str,temp;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		str.Format("��λ%d���˿��� %d:",i,m_bUserCardCnt[i]);
		for(int j=0;j<m_bUserCardCnt[i];j++)
		{
			temp.Format("%x-",m_bUserCard[i][j]);
			str +=temp;
		}
		str += "\n";
		TRACE(str);
	}
#endif

	//��ԭ
	memset(m_bOutCard,0,sizeof(m_bOutCard));
	memset(m_bOutCardCnt,0,sizeof(m_bOutCardCnt));
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
