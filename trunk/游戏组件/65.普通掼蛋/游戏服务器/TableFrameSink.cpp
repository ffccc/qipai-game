#include "StdAfx.h"
#include "TableFrameSink.h"
#include ".\tableframesink.h"



//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//�궨��
#define SHUANG_XIA					10										//��Ϸ˫��
#define DAN_XIA                     11										//��Ϸ����
#define OTHER						12										//����ĩ��
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBankerUser				=	INVALID_CHAIR;
	m_wCurrentUser				=	INVALID_CHAIR;

	//��ʷ����
	ZeroMemory(m_lAllTurnScore,	sizeof(m_lAllTurnScore));
	ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));

	//������Ϣ
	m_bTurnCardCount			=	0;
	m_wTurnWiner				=	INVALID_CHAIR;
	m_sTurnCardType				=   short(CT_ERROR);
	m_bSeries.bCurrentSeries	=	2;
	m_bSeries.bOtherSeries		=	2;
	m_bSeries.bOurSeries		=   2;
    m_bMark						=   0;
	m_bGameStatus				=	0;
	m_bPayNumber				=	0;
	m_bAnswerCount				=	0;
	m_lLostScore				=	20;
	m_bEnd						=   true;
	ZeroMemory(m_bTurnCardData,	sizeof(m_bTurnCardData));
	ZeroMemory(m_iGameResult,	sizeof(m_iGameResult));
	ZeroMemory(m_bRanKing,		sizeof(m_bRanKing));
	ZeroMemory(m_bTouYou,		sizeof(m_bTouYou));
	ZeroMemory(m_bMoYou,	 	sizeof(m_bMoYou));
	ZeroMemory(m_bPayTribute,	sizeof(m_bPayTribute));
	

	//�˿���Ϣ
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));

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
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
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
	m_wCurrentUser				=	INVALID_CHAIR;
	
	//������Ϣ
	m_bTurnCardCount			=	0;
	m_wTurnWiner				=	INVALID_CHAIR;
	m_sTurnCardType				=   short(CT_ERROR);
	m_bMark						=   0;
	m_bPayNumber				=	0;
	m_lLostScore				=	20;
	m_bAnswerCount				=	0;
	
	//�˿���Ϣ
	ZeroMemory(m_bTurnCardData,	sizeof(m_bTurnCardData));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
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
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	m_GameLogic.m_bSeries	=   m_bSeries.bCurrentSeries|0x20;
	m_bEnd					=	false;
	
	//�����˿�
	BYTE bRandCard[108];
	m_GameLogic.RandCardList(bRandCard,CountArray(bRandCard));

	//�ַ��˿�
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=27;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i] ,m_bCardCount[i]);
	}

	//�����û�
	if(m_wBankerUser==INVALID_CHAIR)
	{
		GameStart.bLiangPai					=	true;
		m_wCurrentUser						=   rand() % m_wPlayerCount;
		BYTE    bIndex						=   rand() % 27;
		GameStart.bLiangCard				=	m_bHandCardData[m_wCurrentUser][bIndex];
		m_wBankerUser						=   m_wCurrentUser;
	}
	else
		m_wCurrentUser						=	m_wBankerUser;

    CMD_S_PayTribute   PayTribute;
	//�жϽ���
	if(m_bGameStatus	==	SHUANG_XIA)
	{
		m_wCurrentUser		=	INVALID_CHAIR;
		BYTE  bFirst		=	m_bRanKing[0];
		BYTE  bSecond		=   (bFirst+1)%m_wPlayerCount;
		BYTE  bThree		=	(bSecond+2)%m_wPlayerCount;

		//ӵ��˫����������
		if(m_bHandCardData[bFirst][0]!=0x42&&m_bHandCardData[(bFirst+2)%m_wPlayerCount][0]!=0x42)
		{
			CopyMemory(PayTribute.bPayType,m_bPayTribute,sizeof(BYTE)*4);
			if(m_bHandCardData[bSecond][0]!=0x42)
				PayTribute.bPayType[bSecond]=0;
			if(m_bHandCardData[bThree][0]!=0x42)
				PayTribute.bPayType[bThree]=0;

			ZeroMemory(m_bPayTribute,sizeof(m_bPayTribute));
			m_wCurrentUser			  =		bFirst;

		}
	}else if(m_bGameStatus == DAN_XIA)
	{
		m_wCurrentUser		=	INVALID_CHAIR;
		BYTE bSecond		=	(m_bRanKing[1]+2)%m_wPlayerCount;

		//ӵ��˫����������
		if(m_bHandCardData[bSecond][0]==0x42&&m_bHandCardData[bSecond][1]==0x42)
		{
			CopyMemory(PayTribute.bPayType,m_bPayTribute,sizeof(BYTE)*4);
			ZeroMemory(m_bPayTribute,sizeof(m_bPayTribute));
			m_wCurrentUser			=      m_bRanKing[0];
		}
	}else if(m_bGameStatus == OTHER)
	{	
		m_wCurrentUser		=	INVALID_CHAIR;
		BYTE bSecond		=	(m_bRanKing[0]+2)%m_wPlayerCount;

		//ӵ��˫����������
		if(m_bHandCardData[bSecond][0]==0x42&&m_bHandCardData[bSecond][1]==0x42)
		{
			CopyMemory(PayTribute.bPayType,m_bPayTribute,sizeof(BYTE)*4);
			ZeroMemory(m_bPayTribute,sizeof(m_bPayTribute));
			m_wCurrentUser			=      m_bRanKing[0];
		}
	}
	
	//�����˿�
	//��Ϣ����
	GameStart.wBankerUser				=	m_wBankerUser;
	GameStart.wCurrentUser				=	m_wCurrentUser;
	GameStart.series					=	m_bSeries;
	CopyMemory(GameStart.m_iGameResult,m_iGameResult,sizeof(m_iGameResult));
	GameStart.RoomType=enRoomType_Normal;
	
	GameStart.wServerType	= m_pGameServiceOption->wServerType;

	for (WORD i=0;i<m_wPlayerCount;i++)
	{  
		CopyMemory(GameStart.cbCardData,m_bHandCardData[i],sizeof(GameStart.cbCardData));
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	//�������
    if(m_wCurrentUser==INVALID_CHAIR)
	{
		CopyMemory(PayTribute.bPayType,m_bPayTribute,sizeof(BYTE)*4);
		PayTribute.wCurrentUser	=	INVALID_CHAIR;
		PayTribute.bPayStatus	=	enJin_Gong;
		PayTribute.wCurrentUser	=	INVALID_CHAIR;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
		CMD_S_MESSAGE message;
		IServerUserItem * pIServerUserItem;
		for(WORD i=0;i<m_wPlayerCount;i++)
		{
			if(PayTribute.bPayType[i]==2)
			{
				pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
				_sntprintf(message.szBuffer,sizeof(message.szBuffer),"�ȴ��û� %-s ����",
					pIServerUserItem->GetAccounts());
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SYSTEM_MESSAGE,&message,sizeof(message));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SYSTEM_MESSAGE,&message,sizeof(message));
			}	
		}
	}
	//������
	else if(m_bGameStatus!=0)
	{
			PayTribute.bPayStatus=enKang_Gong;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
	}
			
	
	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch(cbReason)
	{
	     //��Ϸ��ɢ  
	case GER_DISMISS:
		  {
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
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

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

		    //������Ϸ
		    m_pITableFrame->ConcludeGame();
		   return true;
		  }
	 case GER_NORMAL:
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
            
			//��������
			int  bUpgrading=0,bDemotion1=0,bdown=0,bAdd=0,bDemotion2=0;
			BYTE   bFirst=m_bRanKing[0],bSecond=(m_bRanKing[0]+1)%m_wPlayerCount;
			
			//����ͷ��
			m_bTouYou[bFirst]++;
			
			
			
			//״̬�޸�
			if(m_iGameResult[bFirst]>=0)
				m_iGameResult[bFirst]++;
			else m_iGameResult[bFirst]=1;
			
			if(m_iGameResult[bFirst]>=3)
				bAdd=(m_bTouYou[bFirst]-2);
			
			bUpgrading=bAdd;
			
			m_bPayTribute[bFirst]=1;
			
			//ͳ�ƻ���
			long   lscore;
			 //˫�´���
			if(m_bRanKing[0]==((m_bRanKing[1]+2)%m_wPlayerCount))
			{
                 m_bGameStatus						 =	SHUANG_XIA;
				 lscore=3;
				 bUpgrading+=3;
				 m_iGameResult[(bFirst+2)%m_wPlayerCount]=0;
				 m_bPayTribute[(bFirst+2)%m_wPlayerCount]=1;
				 
				
				 m_bMoYou[bSecond]++;
				 m_bPayTribute[bSecond]=2;
				 if(m_iGameResult[bSecond]<=0)
					 m_iGameResult[bSecond]--;
				 else
					 m_iGameResult[bSecond]=-1;
				 if(m_iGameResult[bSecond]<=-3)bDemotion1+=(-2-m_iGameResult[bSecond]);
				 
				 BYTE i=(bSecond+2)%m_wPlayerCount;
				 m_bMoYou[i]++;
				 m_bPayTribute[i]=2;
				 if(m_iGameResult[i]<=0)
					 m_iGameResult[i]--;
				 else 
					 m_iGameResult[i]=-1;
				 if(m_iGameResult[i]<=-3)bDemotion2+=(-2-m_iGameResult[i]);
				
			}
			//���´���
			else if(m_bRanKing[0]==((m_bRanKing[2]+2)%m_wPlayerCount))
			{
				 m_bGameStatus						=	DAN_XIA; 
				 lscore=2;
				 bUpgrading+=2;
				 m_iGameResult[m_bRanKing[2]]		=	0;
				 m_bPayTribute[m_bRanKing[2]]		=	0;
				 m_iGameResult[m_bRanKing[1]]		=	0;
				 m_bPayTribute[m_bRanKing[1]]		=	0;
				 
                 BYTE i=(m_bRanKing[1]+2)%m_wPlayerCount;
				 m_bMoYou[i]++;
				 m_bPayTribute[i]					=	2;
				 if(m_iGameResult[i]<=0)
					 m_iGameResult[i]--;
				 else 
					 m_iGameResult[i]=-1;
				 if(m_iGameResult[i]<=-3)bDemotion1+=(-2-m_iGameResult[i]);
		    }
			else 
			{
				m_bGameStatus						=   OTHER;
				lscore=1;
				bUpgrading+=1;
				m_iGameResult[m_bRanKing[1]]		=	0;
				m_iGameResult[m_bRanKing[2]]		=	0;
				m_bPayTribute[m_bRanKing[1]]		=	0;
				m_bPayTribute[m_bRanKing[2]]		=	0;
				
				//ĩ�δ���
				BYTE i=(bFirst+2)%m_wPlayerCount;
				m_bMoYou[i]++;
				m_bPayTribute[i]					=	2;
				if(m_iGameResult[i]<=0)
					 m_iGameResult[i]--;
				else 
					 m_iGameResult[i]=-1;
				if(m_iGameResult[i]<=-3)bdown+=(-2-m_iGameResult[i]);
			}
            long lwscore=0;
			if(m_bSeries.bCurrentSeries==14&&m_bGameStatus!=OTHER)
			{
			  if((m_bSeries.bOurSeries==14&&bFirst%2==0)||(m_bSeries.bOtherSeries==14&&bFirst%2==1))
			  {
				 
				if(m_pGameServiceOption->wServerType!=GAME_GENRE_GOLD)
					lwscore=10;
				m_bGameStatus				=	0;
				m_bEnd						=   true;
				ZeroMemory(m_iGameResult,   sizeof(m_iGameResult));
				ZeroMemory(m_bRanKing,		sizeof(m_bRanKing));
				ZeroMemory(m_bTouYou,		sizeof(m_bTouYou));
				ZeroMemory(m_bMoYou,	 	sizeof(m_bMoYou));
				ZeroMemory(m_bPayTribute,	sizeof(m_bPayTribute));
			  }
			}

			if(m_bEnd)
			{
				m_bSeries.bOtherSeries		=2;
				m_bSeries.bCurrentSeries	=2;
				m_bSeries.bOurSeries		=2;
			}
			else
			{
				if((bFirst%2)==0)
				{
					m_bSeries.bOurSeries+=bUpgrading;
					if(m_bSeries.bOurSeries>=14) m_bSeries.bOurSeries=14;
					if(m_bSeries.bOurSeries-2>bdown)
						m_bSeries.bOurSeries-=bdown;
					else
						m_bSeries.bOurSeries=2;
				
					//��A,��˫��
					if(m_bSeries.bOtherSeries==m_bSeries.bCurrentSeries&&m_bSeries.bCurrentSeries==14)
						m_bSeries.bOtherSeries-=3;

					if(m_bSeries.bOtherSeries-2>(bDemotion1+bDemotion2))
						m_bSeries.bOtherSeries-=(bDemotion1+bDemotion2);
					else
						m_bSeries.bOtherSeries=2;
			
					m_bSeries.bCurrentSeries=m_bSeries.bOurSeries;
				}
				else
				{	
					m_bSeries.bOtherSeries+=bUpgrading;
					if(m_bSeries.bOtherSeries>=14) m_bSeries.bOtherSeries=14;
					if(m_bSeries.bOtherSeries-2>bdown)
						m_bSeries.bOtherSeries-=bdown;
					else
						m_bSeries.bOtherSeries=2;
					
					//��A,��˫��
					if(m_bSeries.bOurSeries==m_bSeries.bCurrentSeries&&m_bSeries.bCurrentSeries==14)
						m_bSeries.bOurSeries-=3;

					if(m_bSeries.bOurSeries-2>(bDemotion1+bDemotion2))
						m_bSeries.bOurSeries-=(bDemotion1+bDemotion2);
					else
						m_bSeries.bOurSeries=2;
			
					m_bSeries.bCurrentSeries=m_bSeries.bOtherSeries;
				}
			}
			
			//�������
			lscore=lscore-Pow(2,bdown)+Pow(2,bAdd)+Pow(2,bDemotion1)+Pow(2,bDemotion2);
			GameEnd.lGameScore[bFirst]=lscore;
			GameEnd.lGameScore[bSecond]=-lscore;
			GameEnd.lGameScore[(bFirst+2)%m_wPlayerCount]=lscore;
			GameEnd.lGameScore[(bSecond+2)%m_wPlayerCount]=-lscore;

			for(WORD i=0;i< m_wPlayerCount;i++)
			{
				GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*m_pGameServiceOption->lCellScore;
			}
			GameEnd.lGameScore[bFirst]+=lwscore;
			GameEnd.lGameScore[(bFirst+2)%m_wPlayerCount]+=lwscore;

			//����ͳ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_lAllTurnScore[i]+=GameEnd.lGameScore[i];
				m_lLastTurnScore[i]=GameEnd.lGameScore[i];
			}
		
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
						lRevenue=lScore*m_pGameServiceOption->wRevenue/1000;
					lScore-=lRevenue;
					GameEnd.lGameScore[i]=lScore;
					GameEnd.lGameTax+=lRevenue;
				}
				else if(lScore==0) 	ScoreKind=enScoreKind_Draw;
				else				ScoreKind=enScoreKind_Lost;
				
				m_pITableFrame->WriteUserScore(i,lScore,lRevenue,ScoreKind);
			}
		
			//�л�ׯ��
			if(!m_bEnd)
				m_wBankerUser=m_bRanKing[0];
			else
				m_wBankerUser=INVALID_CHAIR;

			//������Ϸ
			//��ͨ����
			//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				
				m_pITableFrame->ConcludeGame();
		 return true;
		 }
	     //�û�ǿ��
	 case GER_USER_LEFT:
		 {
		 //������Ϸ
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
            
			//���û���
			GameEnd.lGameScore[wChairID]-=m_lLostScore;
			GameEnd.lGameTax			=m_lLostScore;
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			
			if(m_lLostScore!=0)
			{
				//�޸Ļ���
				m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0L,enScoreKind_Flee);
			}
		
			//��Ϸ����
			m_pITableFrame->ConcludeGame();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_END);

			//�л�ר��
			m_wBankerUser=INVALID_CHAIR;
			//�޸ı���
			m_bEnd						=   true;
			m_bSeries.bCurrentSeries	=	2;
			m_bSeries.bOtherSeries		=	2;
			m_bSeries.bOurSeries		=   2;
			m_bGameStatus				=	0;
			ZeroMemory(m_iGameResult,	sizeof(m_iGameResult));
			ZeroMemory(m_bRanKing,		sizeof(m_bRanKing));
			ZeroMemory(m_bTouYou,		sizeof(m_bTouYou));
			ZeroMemory(m_bMoYou,	 	sizeof(m_bMoYou));
			ZeroMemory(m_bPayTribute,	sizeof(m_bPayTribute));

			//��ʷ����
			ZeroMemory(m_lAllTurnScore,	sizeof(m_lAllTurnScore));
			ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));
		
			  
		 return true;
		 }
	}
	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	//Ч����
	//ASSERT(FALSE);
    switch(cbGameStatus)
	{
	case GS_RF_FREE:
		{
		 //�������
		CMD_S_StatusFree StatusFree;
		ZeroMemory(&StatusFree, sizeof(StatusFree));

		//���ñ���
		StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

		//���ͳ���
		return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
		//��Ϸ״̬
	case GS_RF_PLAYING:		
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBankerUser		=	m_wBankerUser;
			StatusPlay.wLastOutUser		=	m_wTurnWiner;
			StatusPlay.wCurrentUser		=	m_wCurrentUser;
			StatusPlay.bTurnCardCount	=	m_bTurnCardCount;
			StatusPlay.lBaseScore		=	m_pGameServiceOption->lCellScore;
			StatusPlay.series			=	m_bSeries;
			CopyMemory(StatusPlay.m_iGameResult,	m_iGameResult,				sizeof(m_iGameResult));
			CopyMemory(StatusPlay.bCardCount,		m_bCardCount,				sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,	m_bTurnCardData,			sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,		m_bHandCardData[wChiarID],	sizeof(BYTE)*m_bCardCount[wChiarID]);

			//��ʷ����
			CopyMemory(StatusPlay.lAllTurnScore,	m_lAllTurnScore,			sizeof(m_lAllTurnScore));
			CopyMemory(StatusPlay.lLastTurnScore,	m_lLastTurnScore,			sizeof(m_lLastTurnScore));
           
			StatusPlay.RoomType=enRoomType_Normal;

			StatusPlay.wServerType	= m_pGameServiceOption->wServerType;
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
	switch(wSubCmdID)
	{
		//�û�����
	case SUB_C_OUT_CARD:		
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
		//�û�����
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserPassCard(pUserData->wChairID);
		}
	case SUB_C_PAY_TRIBUTE:
		{
		    //��������
			CMD_C_PayTribute * pPayTribute=(CMD_C_PayTribute *)pDataBuffer;
			WORD wSize=sizeof(CMD_C_PayTribute);

			//Ч������
			if(wSize!=wDataSize)return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
         
			//��Ϣ����
			return OnPayTribute(pDataBuffer);
		}
	case SUB_C_REQUEST_QUIT:
		{
			//��������
			CMD_S_REQUEST request;
			CopyMemory(&request,pDataBuffer,sizeof(CMD_C_REQUEST));
			m_pITableFrame->SendTableData((request.wChairID+1)%m_wPlayerCount,SUB_S_REQUEST_QUIT,&request,sizeof(CMD_S_REQUEST));
			m_pITableFrame->SendTableData((request.wChairID+3)%m_wPlayerCount,SUB_S_REQUEST_QUIT,&request,sizeof(CMD_S_REQUEST));
			
			return true;
		}
	case SUB_C_ANSWER_QUIT:
		{
			m_bAnswerCount++;
			//��������
			CMD_C_ANSWER *answer=(CMD_C_ANSWER *)pDataBuffer;
			CMD_S_ANSWER sanswer;
			if(m_bAnswerCount==1)
			{
				if(answer->bAnswer)
					m_lLostScore=0;
				else
					m_lLostScore=20;
			}
			else if(m_bAnswerCount==2)
			{
				if(answer->bAnswer&&m_lLostScore==0)
				{
					m_lLostScore=0;
					sanswer.bAnswer=true;
				}
				else
				{
					m_lLostScore=20;
					sanswer.bAnswer=false;
				}
				sanswer.wChairID=answer->wChairID;
				m_pITableFrame->SendTableData(sanswer.wChairID,SUB_S_ANSWER_QUIT,&sanswer,sizeof(CMD_S_ANSWER));
				m_bAnswerCount=0;
			}

			return true;
		}
	case SUB_C_QUIT:
		{
		   	//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			
			OnActionUserStandUp(pUserData->wChairID,pIServerUserItem,false);
			return true;
		}
	case SUB_C_SYSTEM_MESSAGE:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			CMD_C_MESSAGE *pMessage=(CMD_C_MESSAGE *)pDataBuffer;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SYSTEM_MESSAGE,pMessage,wDataSize);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SYSTEM_MESSAGE,pMessage,wDataSize);
			return true;
		}
	case SUB_C_LOOKON:
		{
		   WORD *wChairID=(WORD *)pDataBuffer;
		   bool bLookon=true;
		   //ȡ���Թ��߿���
		   m_pITableFrame->SendLookonData(*wChairID,SUB_S_LOOKON,&bLookon,sizeof(bLookon));
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
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_RF_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;



	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;


	//���Ƽ�¼
	m_bTurnCardCount	=	bCardCount;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);
	m_sTurnCardType		=   m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount);
    
	
	//�л��û�
	if(m_bCardCount[wChairID]==0&&(m_bCardCount[(wChairID+2)%m_wPlayerCount]==0))
	{
		 m_wCurrentUser=INVALID_CHAIR;
	}
	else
	{
		if(m_sTurnCardType!=(((short(17))<<8)|CT_FOUR_KING))
		   m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		while(m_bCardCount[m_wCurrentUser]==0)
		   m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		

	}
   
	if(m_bCardCount[wChairID]==0)
	{
		m_bRanKing[m_bMark]=wChairID;
		m_bMark++;
		m_wTurnWiner=m_wCurrentUser;
		CMD_S_MESSAGE message;
		TCHAR  *szBuffer[3]={"һ","��","��"};
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		_sntprintf(message.szBuffer,sizeof(message.szBuffer),
			"%-s ��%s��������",pIServerUserItem->GetUserData()->szAccounts,szBuffer[m_bMark-1]);
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SYSTEM_MESSAGE,&message,sizeof(message));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SYSTEM_MESSAGE,&message,sizeof(message));
		
	}
	else
		m_wTurnWiner=wChairID;

	
	
	

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
	
	//���ͶԼ��˿�
	if(m_bCardCount[wChairID]==0&&m_wCurrentUser!=INVALID_CHAIR)
	{
		CMD_S_CARD SCard;
		SCard.wFromUser=(wChairID+2)%m_wPlayerCount;
		SCard.bCardCount=m_bCardCount[SCard.wFromUser];
		CopyMemory(SCard.bCardData,m_bHandCardData[SCard.wFromUser],SCard.bCardCount);
		m_pITableFrame->SendTableData(wChairID,SUB_S_CARD,&SCard,sizeof(SCard));
	}
	
	if(m_bCardCount[(wChairID+2)%m_wPlayerCount]==0&&m_wCurrentUser!=INVALID_CHAIR)
	{
		CMD_S_CARD SCard;
		SCard.wFromUser=wChairID;
		SCard.bCardCount=m_bCardCount[SCard.wFromUser];
		CopyMemory(SCard.bCardData,m_bHandCardData[wChairID],SCard.bCardCount);
		m_pITableFrame->SendTableData((wChairID+2)%m_wPlayerCount,SUB_S_CARD,&SCard,sizeof(SCard));
	}

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
	while(m_bCardCount[m_wCurrentUser]==0)
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

//�û�����
bool CTableFrameSink::OnPayTribute(const void * pDataBuffer)
{
	//��ȡ����
	CMD_C_PayTribute *pPayTribute=(CMD_C_PayTribute *)pDataBuffer;
	WORD	wChairID		=	pPayTribute->wUser;
	BYTE    bPayCard		=	pPayTribute->bCard;

	m_bPayNumber++;
	m_bPayCard[wChairID]=bPayCard;
    
	//δȫ������
	if((m_bPayNumber<=m_wPlayerCount&&m_bGameStatus==SHUANG_XIA)||
		(m_bPayNumber<=2&&m_bGameStatus==DAN_XIA)||(m_bPayNumber<=2&&m_bGameStatus==OTHER))
	{
		//��������
		CMD_S_PayTribute PayTribute;
		PayTribute.bPayStatus	=	enNone;
		PayTribute.bCard		=	bPayCard;
		PayTribute.wFromUser	=	wChairID;
		PayTribute.wCurrentUser	=	INVALID_CHAIR;
		PayTribute.wToUser		=	INVALID_CHAIR;
		m_GameLogic.RemoveCard(&bPayCard,1,m_bHandCardData[wChairID],m_bCardCount[wChairID]);
		m_bCardCount[wChairID]--;
		
		CopyMemory(PayTribute.bPayType,m_bPayTribute,sizeof(m_bPayTribute));
		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
	}
	if(pPayTribute->bPayType==enJin_Gong&&((m_bPayNumber>=2&&m_bGameStatus==SHUANG_XIA)
		||(m_bPayNumber>=1&&m_bGameStatus==DAN_XIA)
		||(m_bPayNumber>=1&&m_bGameStatus==OTHER)))
	{   
		//��������
		CMD_S_PayTribute PayTribute;
		PayTribute.bPayStatus	=	enHui_Gong;
		PayTribute.wCurrentUser	=	INVALID_CHAIR;
		CopyMemory(PayTribute.bPayType,m_bPayTribute,sizeof(m_bPayTribute));
		
		//���´���
		if(m_bGameStatus==DAN_XIA)
		{ 
			//��ǰ�û�
			m_wCurrentUser			=   (m_bRanKing[1]+2)%m_wPlayerCount;
	
			//�����˿�
			PayTribute.bCard		=	m_bPayCard[(m_bRanKing[1]+2)%m_wPlayerCount];
			PayTribute.wFromUser	=	(m_bRanKing[1]+2)%m_wPlayerCount;
			PayTribute.wToUser		=   m_bRanKing[0];
			m_pITableFrame->SendTableData(PayTribute.wToUser,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
			m_GameLogic.Add(PayTribute.bCard, m_bHandCardData[PayTribute.wToUser],m_bCardCount[PayTribute.wToUser]);
			m_bCardCount[PayTribute.wToUser]++;
		}
		//ƽ�۴���
		else if(m_bGameStatus==OTHER)
		{
			//��ǰ�û�
			m_wCurrentUser			=   (m_bRanKing[0]+2)%m_wPlayerCount;
	
			//�����˿�
			PayTribute.bCard		=	m_bPayCard[(m_bRanKing[0]+2)%m_wPlayerCount];
			PayTribute.wFromUser	=	(m_bRanKing[0]+2)%m_wPlayerCount;
			PayTribute.wToUser		=   m_bRanKing[0];
			m_pITableFrame->SendTableData(PayTribute.wToUser,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
			m_GameLogic.Add(PayTribute.bCard, m_bHandCardData[PayTribute.wToUser],m_bCardCount[PayTribute.wToUser]);
			m_bCardCount[PayTribute.wToUser]++;
		}
		//˫�´���
		else if(m_bGameStatus==SHUANG_XIA)
		{   
			BYTE bFirst=m_bRanKing[0],bSecond=m_bRanKing[1];
			BYTE bThird=(bFirst+1)%m_wPlayerCount,bFouth=(bSecond+1)%m_wPlayerCount;

			if(m_GameLogic.GetCardLogicValue(m_bPayCard[bThird])>m_GameLogic.GetCardLogicValue(m_bPayCard[bFouth]))
			{
				//��ǰ�û�
				m_wCurrentUser			=   bThird;

				PayTribute.bCard		=	m_bPayCard[bThird];
				PayTribute.wFromUser	=	bThird;
				PayTribute.wToUser		=	bFirst;
				m_pITableFrame->SendTableData(PayTribute.wToUser,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
				m_GameLogic.Add(PayTribute.bCard, m_bHandCardData[PayTribute.wToUser], m_bCardCount[PayTribute.wToUser]);
			    m_bCardCount[PayTribute.wToUser]++;
				
				
				PayTribute.bCard		=	m_bPayCard[bFouth];
				PayTribute.wFromUser	=	bFouth;
				PayTribute.wToUser		=	bSecond;
				m_pITableFrame->SendTableData(PayTribute.wToUser,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
				m_GameLogic.Add(PayTribute.bCard, m_bHandCardData[PayTribute.wToUser], m_bCardCount[PayTribute.wToUser]);
			    m_bCardCount[PayTribute.wToUser]++;
				
			}else
			{
				
				//��ǰ�û�
				m_wCurrentUser			=   bFouth;

				PayTribute.bCard		=	m_bPayCard[bFouth];
				PayTribute.wFromUser	=	bFouth;
				PayTribute.wToUser		=	bFirst;
				m_pITableFrame->SendTableData(PayTribute.wToUser,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
				m_GameLogic.Add(PayTribute.bCard, m_bHandCardData[PayTribute.wToUser], m_bCardCount[PayTribute.wToUser]);
			    m_bCardCount[PayTribute.wToUser]++;
				
				
				PayTribute.bCard		=	m_bPayCard[bThird];
				PayTribute.wFromUser	=	bThird;
				PayTribute.wToUser		=	bSecond;
				m_pITableFrame->SendTableData(PayTribute.wToUser,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
				m_GameLogic.Add(PayTribute.bCard, m_bHandCardData[PayTribute.wToUser], m_bCardCount[PayTribute.wToUser]);
			    m_bCardCount[PayTribute.wToUser]++;

			}
		}
	}
	if(pPayTribute->bPayType==enHui_Gong)
	{
		//��������
		CMD_S_PayTribute PayTribute;
		PayTribute.bPayStatus	=	enGet_Hui_Gong;
		PayTribute.wCurrentUser	=	INVALID_CHAIR;
		CopyMemory(PayTribute.bPayType,m_bPayTribute,sizeof(m_bPayTribute));

		PayTribute.bCard		=	pPayTribute->bCard;
		PayTribute.wFromUser	=	pPayTribute->wUser;
		PayTribute.wToUser		=	pPayTribute->wToUser;
		m_pITableFrame->SendTableData(PayTribute.wToUser,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
		m_GameLogic.Add(PayTribute.bCard, m_bHandCardData[PayTribute.wToUser], m_bCardCount[PayTribute.wToUser]);
		m_bCardCount[PayTribute.wToUser]++;

		//����ϵͳ��Ϣ
		CMD_S_MESSAGE message;
		IServerUserItem *pFrom	=	m_pITableFrame->GetServerUserItem(PayTribute.wFromUser);
		IServerUserItem *pTo	=	m_pITableFrame->GetServerUserItem(PayTribute.wToUser);
		_sntprintf(message.szBuffer,sizeof(message.szBuffer),"�û� %-s ������ %-s",
			pTo->GetAccounts(),pFrom->GetAccounts());
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SYSTEM_MESSAGE,&message,sizeof(message));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SYSTEM_MESSAGE,&message,sizeof(message));

	}
	if((m_bPayNumber==4&&m_bGameStatus==SHUANG_XIA)||
		(m_bPayNumber==2&&m_bGameStatus==DAN_XIA)||m_bPayNumber==2&&m_bGameStatus==OTHER)
	{	
		//��������
		CMD_S_PayTribute PayTribute;
		PayTribute.bPayStatus	=	enNone;
		PayTribute.wCurrentUser	=	m_wCurrentUser;
		CopyMemory(PayTribute.bPayType,m_bPayTribute,sizeof(m_bPayTribute));

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PAY_TRIBUTE,&PayTribute,sizeof(PayTribute));
	}

  return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem *pIServerUserItem,bool bLookonUser)
{
	
	
	if(!bLookonUser)
	{
			
			bool bLookon=false;
			//ȡ���Թ��߿���
			m_pITableFrame->SendLookonData(wChairID,SUB_S_LOOKON,&bLookon,sizeof(bLookon));
		
			//�л�ר��
			m_wBankerUser=INVALID_CHAIR;
			
			//�޸ı���
			m_bEnd						=   true;
			m_bSeries.bCurrentSeries	=	2;
			m_bSeries.bOtherSeries		=	2;
			m_bSeries.bOurSeries		=   2;
			m_bGameStatus				=	0;
			ZeroMemory(m_iGameResult,	sizeof(m_iGameResult));
			ZeroMemory(m_bRanKing,		sizeof(m_bRanKing));
			ZeroMemory(m_bTouYou,		sizeof(m_bTouYou));
			ZeroMemory(m_bMoYou,	 	sizeof(m_bMoYou));
			ZeroMemory(m_bPayTribute,	sizeof(m_bPayTribute));
			//��ʷ����
			ZeroMemory(m_lAllTurnScore,	sizeof(m_lAllTurnScore));
			ZeroMemory(m_lLastTurnScore,sizeof(m_lLastTurnScore));
	}
   return true;
}

//����ָ��
int CTableFrameSink::Pow(int x, int y)
{	
	if(y==0)
		return 0;
	if(y==1)
		return 1;
	int sum=1;
	sum=sum<<(y-1);
	return sum;

}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	
	if(bLookonUser)
	{
		bool bLookon=false;
		//ȡ���Թ��߿���
		m_pITableFrame->SendTableData(wChairID,SUB_S_LOOKON,&bLookon,sizeof(bLookon));
	}

	return true;
}
