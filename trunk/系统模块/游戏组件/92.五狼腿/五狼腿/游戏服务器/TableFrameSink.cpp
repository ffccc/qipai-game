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
	m_wFirstUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	
	//ʤ����Ϣ
	m_wWinCount=0;
	memset(m_wWindUserList,0,sizeof(m_wWindUserList));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
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
CTableFrameSink::~CTableFrameSink(void)
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
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));

	//ʤ����Ϣ
	m_wWinCount=0;
	memset(m_wWindUserList,0,sizeof(m_wWindUserList));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
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
	BYTE bRandCard[108];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=27;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i],ST_ORDER);
	}

	//�����û�
	if (m_wFirstUser==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			for (BYTE j=0;j<m_bCardCount[i];j++)
			{
				if (m_bHandCardData[i][j]==0x03) 
				{
					m_wCurrentUser=i;
					break;
				}
			}
			if (m_wCurrentUser!=INVALID_CHAIR) break;
		}
	}
	else m_wCurrentUser=m_wFirstUser;

	//�����˿�
	CMD_S_GameStart GameStart;
	GameStart.wCurrentUser=m_wCurrentUser;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�����˿�
		CopyMemory(GameStart.cbCardData[0],m_bHandCardData[i],sizeof(GameStart.cbCardData[0]));
		CopyMemory(GameStart.cbCardData[1],m_bHandCardData[(i+2)%GAME_PLAYER],sizeof(GameStart.cbCardData[1]));

		//��������
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

			//ʤ��ͳ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bCardCount[i]!=0)
				{
					ASSERT(m_wWinCount<GAME_PLAYER);
					m_wWindUserList[m_wWinCount++]=i;
				}
			}

			//���ϵ÷�
			LONG lHandScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				for (BYTE j=0;j<m_bCardCount[i];j++)
				{
					if ((m_bHandCardData[i][j]&LOGIC_MASK_VALUE)==5) lHandScore+=5;
					if ((m_bHandCardData[i][j]&LOGIC_MASK_VALUE)==10) lHandScore+=10;
					if ((m_bHandCardData[i][j]&LOGIC_MASK_VALUE)==13) lHandScore+=10;
				}
			}

			//�ۼƵ÷�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((i%2)!=(m_wWindUserList[3]%2))
				{
					m_lGameScore[i%2]+=lHandScore;
					break;
				}
			}

			//��������   ����ʱ�û����� +-100
			if (((m_wWindUserList[0]%2)==(m_wWindUserList[1]%2))&&(m_lGameScore[m_wWindUserList[0]%2]==200L))
			{
				m_lGameScore[m_wWindUserList[0]%2]=2L*m_lGameScore[m_wWindUserList[0]%2]+200L;				
            }

			//ʤ���÷�
			for (int i=1;i<m_wPlayerCount;i++)
			{
				if ((m_wWindUserList[i]%2)==(m_wWindUserList[0]%2))//�ж�Ӯ�ļ�     �����Ӯ�����ҽ���ѭ��
				{
					WORD wWinner=m_wWindUserList[i];
					m_lGameScore[wWinner%2]+=(40-(i-1)*20);
					m_lGameScore[(wWinner+1)%2]-=(40-(i-1)*20);
					break;
				}
			}
            
			//ͳ�ƻ���
			LONG lCellScore=m_pGameServiceOption->lCellScore;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.lGameScore[i]=lCellScore*((m_lGameScore[i%2]-m_lGameScore[(i+1)%2])/2L);
			}
			for (WORD i=0;i<2;i++)
			{
				GameEnd.lDrawScore[i]=m_lGameScore[i];
			}
            
			//����˰��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (GameEnd.lGameScore[i]<=0) continue;
				if (GameEnd.lGameScore[i]<=20)
				{
					GameEnd.lGameTax=1L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=40)
				{
					GameEnd.lGameTax=2L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=60)
				{
					GameEnd.lGameTax=3L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=80)
				{
					GameEnd.lGameTax=4L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=100)
				{
					GameEnd.lGameTax=5L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=120)
				{
					GameEnd.lGameTax=6L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=140)
				{
					GameEnd.lGameTax=7L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
				else if (GameEnd.lGameScore[i]<=160)
				{
					GameEnd.lGameTax=8L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=180)
				{
					GameEnd.lGameTax=9L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=200)
				{
					GameEnd.lGameTax=10L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=220)
				{
					GameEnd.lGameTax=11L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=240)
				{
					GameEnd.lGameTax=12L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
				}
                else if (GameEnd.lGameScore[i]<=340)
				{
					GameEnd.lGameTax=17L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax;
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
				if (GameEnd.lGameScore[i]==0) ScoreInfo[i].ScoreKind=enScoreKind_Draw;
				else ScoreInfo[i].ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
///			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//���ñ���
			m_wFirstUser=m_wWindUserList[0];

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
			GameEnd.lGameScore[wChairID]=-600L*m_pGameServiceOption->lCellScore;

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
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lGameScore[wChairID];
//			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

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
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);

			//�����˿�
			CopyMemory(StatusPlay.lGameScore,m_lGameScore,sizeof(m_lGameScore));
			CopyMemory(StatusPlay.cbCardData[0],m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.cbCardData[1],m_bHandCardData[(wChiarID+2)%GAME_PLAYER],sizeof(BYTE)*m_bCardCount[(wChiarID+2)%GAME_PLAYER]);

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
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if (bLookonUser==false) m_wFirstUser=INVALID_CHAIR;
	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//��ĿЧ��
	ASSERT(bCardCount<=MAX_COUNT);
	if (bCardCount>MAX_COUNT) return false;

	//�����ж�
	BYTE bLastType=CT_ERROR;
	if (m_bTurnCardCount>0) bLastType=m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount,true);
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount,((bLastType==CT_DOUBLE_BOMB)||(bLastType==CT_DOUBLE_LINE)));
	if (bCardType==CT_ERROR) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bCardCount,m_bTurnCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//����÷�
	for (BYTE i=0;i<m_bTurnCardCount;i++)
	{
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==5) m_lGameScore[2]+=5;
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==10) m_lGameScore[2]+=10;
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==13) m_lGameScore[2]+=10;
	}

	//ʤ���ж�
	if (m_bCardCount[wChairID]==0)
	{
		//�����û�
		m_wWindUserList[m_wWinCount++]=wChairID;

		//�����ж�
		if (m_bCardCount[(wChairID+2)%GAME_PLAYER]==0) m_wCurrentUser=INVALID_CHAIR;
	}

	//�л��û�
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//�����û�
		m_wTurnWiner=wChairID;
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;

		//�û�����
		while (m_bCardCount[m_wCurrentUser]==0) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}

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
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//�ۼƻ���
		m_lGameScore[wChairID%2]+=m_lGameScore[2];
		m_lGameScore[2]=0L;

		//������Ϸ
		OnEventGameEnd(wChairID,NULL,GER_NORMAL);
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//�û�����
	do
	{
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		if (m_wCurrentUser==m_wTurnWiner) 
		{
			m_bTurnCardCount=0;
			break;
		}
	} while (m_bCardCount[m_wCurrentUser]==0);

	//�û�����
	if (m_bCardCount[m_wCurrentUser]==0)
	{
		m_wTurnWiner=(m_wCurrentUser+2)%m_wPlayerCount;
		m_wCurrentUser=(m_wCurrentUser+2)%m_wPlayerCount;
	}

	//����÷�
	if (m_bTurnCardCount==0)
	{
		m_lGameScore[m_wCurrentUser%2]+=m_lGameScore[2];
		m_lGameScore[2]=0L;
	}

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
