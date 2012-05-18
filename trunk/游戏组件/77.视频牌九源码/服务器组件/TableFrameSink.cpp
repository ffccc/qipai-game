#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define IDI_TIME_GIVE_UP						1							//������ʱ��

//��̬����
const BYTE	CGameLogic::m_bCardArray[52]=									//�˿�����
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
};

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//�����˿�
void CGameLogic::SortCard(BYTE bCardData[], BYTE bCardCount)
{
	//ת����ֵ
	BYTE bLogicVolue[2];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);

	//�������
	bool bSorted=true;
	BYTE bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((bLogicVolue[i]<bLogicVolue[i+1])||
				((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//����λ��
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicVolue[i];
				bLogicVolue[i]=bLogicVolue[i+1];
				bLogicVolue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);
	return;
}

//�ж϶���
BYTE CGameLogic::GetCardType(BYTE bCardList[], BYTE bCardCount)
{
	BYTE Value1=GetCardValue(bCardList[0]);
	BYTE Value2=GetCardValue(bCardList[1]);

	if(Value1==Value2) return CT_DOUBLE;
	if( Value1>10&&Value1!=21&&Value2>10&&Value2!=21 ) return CT_COLOR;
	return CT_SINGLE;
}
BYTE CGameLogic::GetMaxValue(BYTE bCardList[])
{
	BYTE Value1=GetCardValue(bCardList[0]);
	BYTE Value2=GetCardValue(bCardList[1]);
	return (Value1>Value2) ? Value1 : Value2;
}
BYTE CGameLogic::GetMinValue(BYTE bCardList[])
{
	BYTE Value1=GetCardValue(bCardList[0]);
	BYTE Value2=GetCardValue(bCardList[1]);
	return (Value1>Value2) ? Value2 : Value1;
}
BYTE CGameLogic::GetMaxColor(BYTE bCardList[])
{
	BYTE Value1=GetCardValue(bCardList[0]);
	BYTE Value2=GetCardValue(bCardList[1]);
	return (Value1>Value2) ? GetCardColor(bCardList[0]) : GetCardColor(bCardList[1]);
}
//�Ա��˿�
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bCardCount)
{
	//��ȡ����
	BYTE bNextType=GetCardType(bNextList,bCardCount);
	BYTE bFirstType=GetCardType(bFirstList,bCardCount);
	//�����Ƶ����ֵ
	BYTE FirstMaxValue=GetMaxValue(bFirstList);
	BYTE NextMaxValue=GetMaxValue(bNextList);
	//�����Ƶ���Сֵ
	BYTE FirstMinValue=GetMinValue(bFirstList);
	BYTE NextMinValue=GetMinValue(bNextList);
	//����ƵĻ�ɫ
	BYTE FirstColor=GetMaxColor(bFirstList);
	BYTE NextColor=GetMaxColor(bNextList);

	//��ʼ�Ƚ�
	if(bFirstType!=bNextType)
	{
		return (bFirstType>bNextType);
	}
	else
	{
		switch(bFirstType)
		{
			case CT_SINGLE:
				{
					BYTE FirstTotalValue=(GetCardLogicValue(bFirstList[0])+GetCardLogicValue(bFirstList[1]))%10;
					BYTE NextTotalValue=(GetCardLogicValue(bNextList[0])+GetCardLogicValue(bNextList[1]))%10;
					if(FirstTotalValue==NextTotalValue) 
					{
						if(FirstMaxValue!=NextMaxValue) return FirstMaxValue>NextMaxValue;
						if(FirstMinValue!=NextMinValue) return FirstMinValue>NextMinValue;
						else return FirstColor>NextColor;
					}
					else 
					{
						return FirstTotalValue>NextTotalValue;
					}
					break;
				}
			case CT_COLOR:
			case CT_DOUBLE:
				{
					if(FirstMaxValue==NextMaxValue) 
					{
						if(FirstMinValue!=NextMinValue) return FirstMinValue>NextMinValue;
						else return FirstColor>NextColor;
					}
					else 
					{
						return FirstMaxValue>NextMaxValue;
					}
					break;
				}
			default:
				return false;
		}
	}
}
//�����˿�
void CGameLogic::RandCard(BYTE bCardBuffer[], BYTE bBufferCount)
{
	BYTE bSend=0,bStation=0,bCardList[CountArray(m_bCardArray)];
	CopyMemory(bCardList,m_bCardArray,sizeof(m_bCardArray));
	static long int dwRandCount=0L;
	srand((unsigned)time(NULL)+dwRandCount++);
	do
	{
		bStation=rand()%(CountArray(m_bCardArray)-bSend);
		bCardBuffer[bSend++]=bCardList[bStation];
		bCardList[bStation]=bCardList[CountArray(m_bCardArray)-bSend];
	} while (bSend<bBufferCount);

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wOperaCount=0;
	m_bSendCardCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));
	memset(m_bTableCardArray,0,sizeof(m_bTableCardArray));

	//�����Ϣ
	m_lMaxGold=0;
	m_lTurnBasicGold=0;
	memset(m_lTableGold,0,sizeof(m_lTableGold));

	//��������
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));

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
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bTableCardArray,0,sizeof(m_bTableCardArray));
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));

	//��ұ���
	m_lMaxGold=0L;
	m_lTurnBasicGold=0L;
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
	return m_bPlayStatus[wChairID]?true:false;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//״̬����
	m_wOperaCount=0;
	m_bSendCardCount=0;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	memset(m_bTableCardCount,0,sizeof(m_bTableCardCount));

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
			m_bPlayStatus[i]=true;
			m_lTableGold[i*2+1]=m_pGameServiceOption->lCellScore;
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

	//�ַ��˿�
	m_bSendCardCount=4;
	m_GameLogic.RandCard(m_bTableCardArray[0],sizeof(m_bTableCardArray)/sizeof(m_bTableCardArray[0][0]));
	//////////////////////////////////////////////
	//m_bTableCardArray[0][0]=0x07;
	//m_bTableCardArray[0][1]=0x08;
	//m_bTableCardArray[1][0]=0x11;
	//m_bTableCardArray[1][1]=0x14;
	//m_bTableCardArray[2][0]=0x07;
	//m_bTableCardArray[2][1]=0x08;
	//m_bTableCardArray[3][0]=0x11;
	//m_bTableCardArray[3][1]=0x14;
	//////////////////////////////////////////////
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		if (m_bPlayStatus[i]==TRUE)
		{
			m_bTableCardCount[i]=m_bSendCardCount;
		}
	}
	m_wCurrentUser=DeduceWiner(1,1);

	//���Ϳ�ʼ
	CMD_S_GameStart GameStart;
	memset(&GameStart,0,sizeof(GameStart));
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.lMaxGold=m_lMaxGold;
	GameStart.lTurnMaxGold=m_lMaxGold/4L;
	GameStart.lTurnBasicGold=m_lTurnBasicGold;
	for (WORD i=0;i<m_wPlayerCount;i++) GameStart.bCardData[i]=m_bTableCardArray[i][1];
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		GameStart.bFundusCard=0;
		if (m_bPlayStatus[i]==TRUE)
		{
			GameStart.bFundusCard=m_bTableCardArray[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	//���ö�ʱ��
	//m_pITableFrame->SetGameTimer(IDI_TIME_GIVE_UP,35000L,1,m_wCurrentUser);

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
			//�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			memset(&ScoreInfo,0,sizeof(ScoreInfo));
			LONG lTax=0L;

			//������ע
			LONG lAllGold=0L;
			for (WORD i=0;i<CountArray(m_lTableGold);i++) lAllGold+=m_lTableGold[i];

			//ͳ����Ϣ
			WORD wWinerUser=DeduceWiner(0,4);
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//������Ϣ
				if ((cbReason==GER_NO_PLAYER)||(m_bPlayStatus[i]==FALSE)) GameEnd.bUserCard[i]=0;
				else GameEnd.bUserCard[i]=m_bTableCardArray[i][0];

				//������Ϣ
				if (i==wWinerUser)
				{
					GameEnd.lGameGold[i]=lAllGold-m_lTableGold[i*2]-m_lTableGold[i*2+1];
					lTax=(GameEnd.lGameGold[i]>=100L)?GameEnd.lGameGold[i] *m_pGameServiceOption->wRevenue /100L:0L;
					GameEnd.lTax=lTax;
					GameEnd.lGameGold[i]-=lTax;
					ScoreInfo[i].lRevenue=lTax;
				}
				else GameEnd.lGameGold[i]=-m_lTableGold[i*2]-m_lTableGold[i*2+1];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸��û�����

			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bPlayStatus[i]==TRUE)
				{
					ScoreInfo[i].lScore=GameEnd.lGameGold[i];
					ScoreInfo[i].ScoreKind=(GameEnd.lGameGold[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				}
				m_pITableFrame->WriteUserScore(i,ScoreInfo[i].lScore ,ScoreInfo[i].lRevenue ,ScoreInfo[i].ScoreKind);
			}
			//m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lTax);

			//ɾ����ʱ��
			/*if (m_bTimeStation!=255)
			{
				KillTimer(m_bTimeStation);
				m_bTimeStation=255;
			}*/

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
			StatusFree.dwBasicGold=m_pGameServiceOption->lCellScore;

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
			StatusPlay.lBasicGold=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bPlayStatus,m_bPlayStatus,sizeof(m_bPlayStatus));
			CopyMemory(StatusPlay.lTableGold,m_lTableGold,sizeof(m_lTableGold));

			//������ע
			if (m_bSendCardCount<=2) StatusPlay.lTurnMaxGold=m_lMaxGold/4L;
			else if (m_bSendCardCount==3) StatusPlay.lTurnMaxGold=m_lMaxGold/2L;
			else StatusPlay.lTurnMaxGold=m_lMaxGold;

			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_bPlayStatus[i]==TRUE)
				{
					ASSERT(m_pITableFrame->GetServerUserItem(i)!=NULL);
					StatusPlay.bTableCardCount[i]=m_bTableCardCount[i];
					if ((i==wChiarID)&&(bSendSecret==true)) StatusPlay.bTableCardArray[i][0]=m_bTableCardArray[i][0];
					CopyMemory(&StatusPlay.bTableCardArray[i][1],&m_bTableCardArray[i][1],(m_bTableCardCount[i]-1)*sizeof(BYTE));
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
			if (m_bPlayStatus[pUserData->wChairID]==FALSE) return true;

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
			if (m_bPlayStatus[pUserData->wChairID]==FALSE) return true;

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
	ASSERT(m_bPlayStatus[wChairID]==TRUE);
	if (m_bPlayStatus[wChairID]==FALSE) return false;
	//��������
	tagScoreInfo ScoreInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));


	//��������
	m_bPlayStatus[wChairID]=FALSE;
	m_bTableCardCount[wChairID]=0;

	//���ͷ�����Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wUserChairID=wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//д�����ݿ�	
	ScoreInfo.ScoreKind=enScoreKind_Lost;
	ScoreInfo.lScore=-m_lTableGold[wChairID*2]-m_lTableGold[wChairID*2+1];
	m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore,ScoreInfo.lRevenue,enScoreKind_Lost);

	//�жϽ���
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==TRUE) wPlayerCount++;
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
	//��������
	if (bCancel==false)
	{
		//Ч������
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		//������ע
		LONG lMaxGold=0L;
		if (m_bSendCardCount<=2) lMaxGold=m_lMaxGold/4L;
		else if (m_bSendCardCount==3) lMaxGold=m_lMaxGold/2L;
		else lMaxGold=m_lMaxGold;
		
		//����ʣ�µĽ��
		LONG lLeaveGold=lMaxGold-m_lTableGold[wChairID*2+1];
		m_lTableGold[wChairID*2]=__min(lLeaveGold,__max(lGold,m_lTurnBasicGold));
	}

	//��������
	bool bFinishTurn=false;
	WORD wNextUser=INVALID_CHAIR;
	for (WORD i=0;i<m_wPlayerCount-1;i++)
	{
		m_wOperaCount++;
		wNextUser=(m_wCurrentUser+i+1)%m_wPlayerCount;
		if (m_bPlayStatus[wNextUser]==TRUE) break;
	}
	
	//�ж���ע
	if (m_wOperaCount>=m_wPlayerCount)
	{
		LONG lTableGold=0L;
		WORD i=0;
		for (i=0;i<m_wPlayerCount;i++)
		{
			if (m_bPlayStatus[i]==TRUE)
			{
				lTableGold=m_lTableGold[i*2];
				break;
			}
		}
		while (i<m_wPlayerCount)
		{
			if ((m_bPlayStatus[i]==TRUE)&&(m_lTableGold[i*2]!=lTableGold)) break;
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
		if (m_bSendCardCount<2) 
		{
			//���ñ���
			m_wOperaCount=0;
			m_bSendCardCount++;
			m_lTurnBasicGold=0L;
			m_wCurrentUser=DeduceWiner(1,m_bSendCardCount-1);

			//�����˿�
			CMD_R_SendCard SendCard;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if (m_bPlayStatus[i]==TRUE) m_bTableCardCount[i]=m_bSendCardCount;
				SendCard.bUserCard[i]=m_bTableCardArray[i][m_bSendCardCount-1];
			}

			//ɾ���ɶ�ʱ��
			/*if (m_bTimeStation!=255)
			{
			KillTimer(m_bTimeStation);
			m_bTimeStation=255;
			}*/

			//���ö�ʱ��
			/*m_bTimeStation=TIME_PASS+m_wCurrentUser;
			SetTimer(m_bTimeStation,TIME_COUNT);*/

			//������Ϣ
			SendCard.wCurrentUser=m_wCurrentUser;
			SendCard.lMaxGold=(m_bSendCardCount!=3)?m_lMaxGold:m_lMaxGold/2L;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
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

		//ɾ���ɶ�ʱ��
		/*if (m_bTimeStation!=255)
		{
			KillTimer(m_bTimeStation);
			m_bTimeStation=255;
		}

		//���ö�ʱ��
		m_bTimeStation=TIME_PASS+m_wCurrentUser;
		SetTimer(m_bTimeStation,TIME_COUNT);*/
	}

	return true;
}

//�ƶ�ʤ��
WORD CTableFrameSink::DeduceWiner(BYTE bBeginPos, BYTE bEndPos)
{
	//�����˿�
	BYTE bTableCardArray[m_wPlayerCount][5];
	CopyMemory(bTableCardArray,m_bTableCardArray,sizeof(m_bTableCardArray));

	//Ѱ�����
	BYTE bWiner;
	for (bWiner=0;bWiner<m_wPlayerCount;bWiner++)
	{
		if (m_bPlayStatus[bWiner]==TRUE) break;
	}

	//�Ա����
	for (BYTE i=(bWiner+1);i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==false) continue;
		if (m_GameLogic.CompareCard(bTableCardArray[i]+bBeginPos,bTableCardArray[bWiner]+bBeginPos,bEndPos-bBeginPos+1)==true) bWiner=i;
	}

	return bWiner;
}

//////////////////////////////////////////////////////////////////////////
