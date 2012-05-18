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
	//�߼�����
	m_cbPackCount=2;
	m_cbMainValue=0x01;
	m_cbMainColor=COLOR_ERROR;
	m_bNotCall=false;
	m_bIsGanGuoBa=false;
	m_bIsRuanGanGuoBa=false;
	m_bIsYingGanGuoBa=false;

	//������Ϣ
	m_cbValueOrder[0]=0x01;
	m_cbValueOrder[1]=0x01;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//״̬����
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_bKouFlag,sizeof(m_bKouFlag));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//���Ʊ���
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

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
	//�߼�����
	m_cbPackCount=2;
	m_cbMainColor=COLOR_ERROR;
	m_bNotCall=false;
	m_bIsGanGuoBa=false;
	m_bIsRuanGanGuoBa=false;
	m_bIsYingGanGuoBa=false;

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//״̬����
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_bKouFlag,sizeof(m_bKouFlag));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//���Ʊ���
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
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
	m_pITableFrame->SetGameStatus(GS_UG_CALL);

	//���ñ���
	m_cbMainColor=COLOR_ERROR;
	m_bNotCall=false;
	m_bIsGanGuoBa=false;
	m_bIsRuanGanGuoBa=false;
	m_bIsYingGanGuoBa=false;

	//��������
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SetMainValue(m_cbMainValue);
	m_GameLogic.SetPackCount(m_cbPackCount);

	//��������
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.cbPackCount=m_cbPackCount;
	GameStart.cbMainValue=m_cbMainValue;
	GameStart.cbValueOrder[0]=m_cbValueOrder[0];
	GameStart.cbValueOrder[1]=m_cbValueOrder[1];

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//�����˿�
	DispatchUserCard();

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

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

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);


			//�۵ױ���
			if ((wChairID!=m_wBankerUser)&&(((wChairID+2)%m_wPlayerCount)!=m_wBankerUser)&&(m_GameLogic.GetCardLogicColor(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID])==64))
			{
				//�����ж�
				BYTE cbCardType=m_GameLogic.GetCardType(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID]);
				if (cbCardType==CT_ERROR) return false;

				if (cbCardType!=CT_THROW_CARD) 
				{
					switch (m_cbOutCardCount[wChairID])
					{
					case 1:
						GameEnd.wConcealTime=1;
						break;
					case 2:
						GameEnd.wConcealTime=2;
						break;
					case 4:
						GameEnd.wConcealTime=4;
						break;
					case 6:
						GameEnd.wConcealTime=6;
						break;
					case 8:
						GameEnd.wConcealTime=8;
						break;
					default:
						GameEnd.wConcealTime=1;
						break;
					}
				}
				else
				{
					GameEnd.wConcealTime=1;
				}
			}

			//�Ƿ����
			bool bIsKouWang=false;
			//�Ƿ�ٵ�(��С���ٵ�)
			bool bIsKingKou=false;

			//----------------------------------------------------------------------------
			//�ж��Ƿ����
			BYTE bSmallKing=0;  //С���ʹ�����
			BYTE bBigKing=0;

			for(byte i=0;i<m_cbConcealCount;i++)
			{
				if(m_GameLogic.GetCardValue(m_cbConcealCard[i])==14) bSmallKing++;
				if(m_GameLogic.GetCardValue(m_cbConcealCard[i])==15) bBigKing++;
			}
			if(bSmallKing!=0||bBigKing!=0) bIsKouWang=true;
			//----------------------------------------------------------------------------
			//�ж��Ƿ�ٵ�(��С���ٵ�)
			BYTE bSmallKou=0;
			BYTE bBigKou=0;

			if(GameEnd.wConcealTime==1)  //����
			{
				if(m_GameLogic.GetCardValue(m_cbOutCardData[wChairID][0])==14) bSmallKou=1;
				if(m_GameLogic.GetCardValue(m_cbOutCardData[wChairID][0])==15) bBigKou=1;
			}
			if(GameEnd.wConcealTime==2)  //˫��
			{
				if(m_GameLogic.GetCardValue(m_cbOutCardData[wChairID][0])==14) bSmallKou=2;
				if(m_GameLogic.GetCardValue(m_cbOutCardData[wChairID][0])==15) bBigKou=2;
			}
			if(bSmallKou!=0||bBigKou!=0) bIsKingKou=true;
			//----------------------------------------------------------------------------
			LONG lBankerScore=0;
			LONG lLostScore=0;
			BYTE bNum=0;

			if(bIsKouWang) //��������
			{
				//ׯ��ʤ��
				if((wChairID==m_wBankerUser)&&(wChairID==(m_wBankerUser+2)%m_wPlayerCount))
				{
					switch(bSmallKing)
					{
					case 0:
						switch(bBigKing)
						{
						case 1:
							{
								lBankerScore=4;
								lLostScore=-2;
								bNum=2;
								break;
							}
						case 2:
							{
								lBankerScore=8;
								lLostScore=-4;
								bNum=5;
								break;
							}
						}
						break;
					case 1:
						switch(bBigKing)
						{
						case 0:
							{
								lBankerScore=2;
								lLostScore=-1;
								bNum=1;
								break;
							}
						case 1:
							{
								lBankerScore=6;
								lLostScore=-3;
								bNum=4;
								break;
							}
						case 2:
							{
								lBankerScore=14;
								lLostScore=-7;
								bNum=7;
								break;
							}
						}
						break;
					case 2:
						switch(bBigKing)
						{
						case 0:
							{
								lBankerScore=6;
								lLostScore=-3;
								bNum=3;
								break;
							}
						case 1:
							{
								lBankerScore=12;
								lLostScore=-6;
								bNum=6;
								break;
							}
						case 2:
							{
								lBankerScore=16;
								lLostScore=-8;
								bNum=9;
								break;
							}
						}
						break;
					}
					//��������ж�
					if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=11&&m_cbValueOrder[m_wBankerUser%2]<11)	m_cbValueOrder[m_wBankerUser%2]=11;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=12&&m_cbValueOrder[m_wBankerUser%2]==11)	m_cbValueOrder[m_wBankerUser%2]=12;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=13&&m_cbValueOrder[m_wBankerUser%2]==12)	m_cbValueOrder[m_wBankerUser%2]=13;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>13&&m_cbValueOrder[m_wBankerUser%2]==13)	m_cbValueOrder[m_wBankerUser%2]=1;
					else m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1;

					//��ҵ÷�
					GameEnd.lScore[m_wBankerUser]=lBankerScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=lBankerScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=lLostScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=lLostScore*m_pGameServiceOption->lCellScore;
				}
				////����ʤ��
				else
				{
					switch(bSmallKing)
					{
					case 0:
						switch(bBigKing)
						{
						case 1:
							{
								lBankerScore=4;
								lLostScore=-2;
								bNum=2;
								break;
							}
						case 2:
							{
								lBankerScore=8;
								lLostScore=-4;
								bNum=5;
								break;
							}
						}
						break;
					case 1:
						switch(bBigKing)
						{
						case 0:
							{
								lBankerScore=2;
								lLostScore=-1;
								bNum=1;
								break;
							}
						case 1:
							{
								lBankerScore=6;
								lLostScore=-3;
								bNum=4;
								break;
							}
						case 2:
							{
								lBankerScore=14;
								lLostScore=-7;
								bNum=7;
								break;
							}
						}
						break;
					case 2:
						switch(bBigKing)
						{
						case 0:
							{
								lBankerScore=6;
								lLostScore=-3;
								bNum=3;
								break;
							}
						case 1:
							{
								lBankerScore=12;
								lLostScore=-6;
								bNum=6;
								break;
							}
						case 2:
							{
								lBankerScore=16;
								lLostScore=-8;
								bNum=9;
								break;
							}
						}
						break;
					}
					//�����ͷ���
					BYTE bWinScore=0;
					BYTE bLostScore=0;

					//��ҽ����ж� J->A    Q->6   K->J
					if(m_cbValueOrder[m_wBankerUser%2]==11)	
					{
						m_cbValueOrder[m_wBankerUser%2]=1;
						bLostScore=-5;
						bWinScore=10;
					}
					if(m_cbValueOrder[m_wBankerUser%2]==12)	
					{
						m_cbValueOrder[m_wBankerUser%2]=6;
						bLostScore=-6;
						bWinScore=12;
					}
					if(m_cbValueOrder[m_wBankerUser%2]==13)
					{
						m_cbValueOrder[m_wBankerUser%2]=11;
						bLostScore=-7;
						bWinScore=14;
					}

					//���������ж�
					if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=11&&m_cbValueOrder[(m_wBankerUser+1)%2]<11)	m_cbValueOrder[(m_wBankerUser+1)%2]=11;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=12&&m_cbValueOrder[(m_wBankerUser+1)%2]==11)	m_cbValueOrder[(m_wBankerUser+1)%2]=12;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=13&&m_cbValueOrder[(m_wBankerUser+1)%2]==12)	m_cbValueOrder[(m_wBankerUser+1)%2]=13;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>13&&m_cbValueOrder[(m_wBankerUser+1)%2]==13)	m_cbValueOrder[(m_wBankerUser+1)%2]=1;
					else m_cbValueOrder[(m_wBankerUser+1)%2]=(m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1;
				
					//��ҵ÷�
					GameEnd.lScore[m_wBankerUser]=(lLostScore+bLostScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=(lLostScore+bLostScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=(lBankerScore+bWinScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=(lBankerScore+bWinScore)*m_pGameServiceOption->lCellScore;
				}
			}
			//----------------------------------------------------------------------------
			else if (bIsKingKou) //�ٵ׼���
			{
				//����ʤ��
				if((wChairID!=m_wBankerUser)&&(wChairID!=(m_wBankerUser+2)%m_wPlayerCount))
				{
					if(bSmallKou==1) 
					{
						lBankerScore=2;
						lLostScore=-1;
						bNum=1;
					}
					if(bSmallKou==2) 
					{
						lBankerScore=6;
						lLostScore=-3;
						bNum=3;
					}
					if(bBigKou==1) 
					{
						lBankerScore=4;
						lLostScore=-2;
						bNum=2;
					}
					if(bBigKou==2) 
					{
						lBankerScore=8;
						lLostScore=-4;
						bNum=5;
					}
					//�����ͷ���
					BYTE bWinScore=0;
					BYTE bLostScore=0;

					//��ҽ����ж� J->A    Q->6   K->J
					if(m_cbValueOrder[m_wBankerUser%2]==11)	
					{
						m_cbValueOrder[m_wBankerUser%2]=1;
						bLostScore=-5;
						bWinScore=10;
					}
					if(m_cbValueOrder[m_wBankerUser%2]==12)	
					{
						m_cbValueOrder[m_wBankerUser%2]=6;
						bLostScore=-6;
						bWinScore=12;
					}
					if(m_cbValueOrder[m_wBankerUser%2]==13)
					{
						m_cbValueOrder[m_wBankerUser%2]=11;
						bLostScore=-7;
						bWinScore=14;
					}

					//���������ж�
					if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=11&&m_cbValueOrder[(m_wBankerUser+1)%2]<11)	m_cbValueOrder[(m_wBankerUser+1)%2]=11;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=12&&m_cbValueOrder[(m_wBankerUser+1)%2]==11)	m_cbValueOrder[(m_wBankerUser+1)%2]=12;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>=13&&m_cbValueOrder[(m_wBankerUser+1)%2]==12)	m_cbValueOrder[(m_wBankerUser+1)%2]=13;
					else if(((m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1)>13&&m_cbValueOrder[(m_wBankerUser+1)%2]==13)	m_cbValueOrder[(m_wBankerUser+1)%2]=1;
					else m_cbValueOrder[(m_wBankerUser+1)%2]=(m_cbValueOrder[(m_wBankerUser+1)%2]+bNum)%13+1;

					//��ҵ÷�
					GameEnd.lScore[m_wBankerUser]=(lLostScore+bLostScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=(lLostScore+bLostScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=(lBankerScore+bWinScore)*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=(lBankerScore+bWinScore)*m_pGameServiceOption->lCellScore;
				}
				else
				{
					if(bSmallKou==1) 
					{
						lBankerScore=2;
						lLostScore=-1;
						bNum=1;
					}
					if(bSmallKou==2) 
					{
						lBankerScore=6;
						lLostScore=-3;
						bNum=3;
					}
					if(bBigKou==1) 
					{
						lBankerScore=4;
						lLostScore=-2;
						bNum=2;
					}
					if(bBigKou==2) 
					{
						lBankerScore=8;
						lLostScore=-4;
						bNum=5;
					}
					//��������ж�
					if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=11&&m_cbValueOrder[m_wBankerUser%2]<11)	m_cbValueOrder[m_wBankerUser%2]=11;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=12&&m_cbValueOrder[m_wBankerUser%2]==11)	m_cbValueOrder[m_wBankerUser%2]=12;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>=13&&m_cbValueOrder[m_wBankerUser%2]==12)	m_cbValueOrder[m_wBankerUser%2]=13;
					else if(((m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1)>13&&m_cbValueOrder[m_wBankerUser%2]==13)	m_cbValueOrder[m_wBankerUser%2]=1;
					else m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+bNum)%13+1;

					//��ҵ÷�
					GameEnd.lScore[m_wBankerUser]=lBankerScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=lBankerScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=lLostScore*m_pGameServiceOption->lCellScore;
					GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=lLostScore*m_pGameServiceOption->lCellScore;
				}
			}
			//----------------------------------------------------------------------------
			else //��ͨ����
			{
				//ͳ�Ʊ���
				WORD wWholeScore=GameEnd.wGameScore+GameEnd.wConcealTime*GameEnd.wConcealScore;

				//����ͳ��
				if (wWholeScore==0) 
				{
					lBankerScore=3L;
					m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+2)%13+1;
				}
				/*
				else if (wWholeScore<20*m_cbPackCount) 
				{
					lBankerScore=2L;
					m_cbValueOrder[m_wBankerUser%2]=(m_cbValueOrder[m_wBankerUser%2]+1)%13+1;
				}
				*/
				else if (wWholeScore<40*m_cbPackCount)
				{
					lBankerScore=1L;
					m_cbValueOrder[m_wBankerUser%2]=m_cbValueOrder[m_wBankerUser%2]%13+1;
				}
				else
				{
					lBankerScore=-(wWholeScore-40*m_cbPackCount)/(10*m_cbPackCount)-1;
					m_cbValueOrder[(m_wBankerUser+1)%2]=(BYTE)((m_cbValueOrder[(m_wBankerUser+1)%2]-lBankerScore-2)%13+1);
				}
				
				//��ҵ÷�
				GameEnd.lScore[m_wBankerUser]=lBankerScore*m_pGameServiceOption->lCellScore;
				GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=lBankerScore*m_pGameServiceOption->lCellScore;
				GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=-(lBankerScore*m_pGameServiceOption->lCellScore);
				GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=-(lBankerScore*m_pGameServiceOption->lCellScore);

			}

			//----------------------------------------------------------------------------------------------------
			//����˰��
			LONG lGameTax=0L;
			if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lScore[i]>=100L)
					{
						lGameTax+=GameEnd.lScore[i]/10L;
						GameEnd.lScore[i]=GameEnd.lScore[i]*45L/50L;
					}
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				ScoreInfo[i].lScore=GameEnd.lScore[i];
				ScoreInfo[i].ScoreKind=(GameEnd.lScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
			}
//			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,lGameTax);

			//ׯ���л�
			if (GameEnd.lScore[m_wBankerUser]>0L) m_wBankerUser=(m_wBankerUser+2)%m_wPlayerCount;
			else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

			//��������
			m_cbMainValue=m_cbValueOrder[m_wBankerUser%2];

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

			//��ҵ÷�
			GameEnd.lScore[wChairID]=-8*m_pGameServiceOption->lCellScore;
			
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			tagScoreInfo ScoreInfo;
			ScoreInfo.ScoreKind=enScoreKind_Flee;
			ScoreInfo.lScore=GameEnd.lScore[wChairID];
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore ,ScoreInfo.lRevenue ,ScoreInfo.ScoreKind );
			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetAccounts());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			}
			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL) break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			} while (true);

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
	case GS_UG_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_UG_CALL:		//����״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//��Ϸ����
			StatusCall.wBankerUser=m_wBankerUser;
			StatusCall.cbPackCount=m_cbPackCount;
			StatusCall.cbMainValue=m_cbMainValue;
			StatusCall.cbValueOrder[0]=m_cbValueOrder[0];
			StatusCall.cbValueOrder[1]=m_cbValueOrder[1];

			//���Ʊ���
			StatusCall.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusCall.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//������Ϣ
			StatusCall.cbCallCard=m_cbCallCard;
			StatusCall.cbCallCount=m_cbCallCount;
			StatusCall.wCallCardUser=m_wCallCardUser;
			StatusCall.cbComplete=(m_bCallCard[wChiarID]==true)?TRUE:FALSE;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_UG_BACK:		//����״̬
		{
			//��������
			CMD_S_StatusBack StatusBack;
			ZeroMemory(&StatusBack,sizeof(StatusBack));

			//��Ϸ����
			StatusBack.cbPackCount=m_cbPackCount;
			StatusBack.cbMainValue=m_cbMainValue;
			StatusBack.cbMainColor=m_cbMainColor;
			StatusBack.cbValueOrder[0]=m_cbValueOrder[0];
			StatusBack.cbValueOrder[1]=m_cbValueOrder[1];

			//���Ʊ���
			StatusBack.cbCallCard=m_cbCallCard;
			StatusBack.cbCallCount=m_cbCallCount;
			StatusBack.wCallCardUser=m_wCallCardUser;

			//�û�����
			StatusBack.wBankerUser=m_wBankerUser;
			StatusBack.wCurrentUser=m_wCurrentUser;

			//�˿˱���
			StatusBack.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusBack.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//������Ϣ
			if (wChiarID==m_wBankerUser)
			{
				StatusBack.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusBack.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusBack,sizeof(StatusBack));
		}
	case GS_UG_PLAY:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.cbPackCount=m_cbPackCount;
			StatusPlay.cbMainValue=m_cbMainValue;
			StatusPlay.cbMainColor=m_cbMainColor;
			StatusPlay.cbValueOrder[0]=m_cbValueOrder[0];
			StatusPlay.cbValueOrder[1]=m_cbValueOrder[1];

			//���Ʊ���
			StatusPlay.cbCallCard=m_cbCallCard;
			StatusPlay.cbCallCount=m_cbCallCount;
			StatusPlay.wCallCardUser=m_wCallCardUser;

			//�û�����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wFirstOutUser=m_wFirstOutUser;

			//�˿˱���
			StatusPlay.cbCardCount=m_cbHandCardCount[wChiarID];
			CopyMemory(StatusPlay.cbCardData,m_cbHandCardData[wChiarID],sizeof(BYTE)*m_cbHandCardCount[wChiarID]);

			//������Ϣ
			if (wChiarID==m_wBankerUser)
			{
				StatusPlay.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusPlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//���Ʊ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusPlay.cbOutCardCount[i]=m_cbOutCardCount[i];
				CopyMemory(StatusPlay.cbOutCardData[i],m_cbOutCardData[i],sizeof(BYTE)*m_cbOutCardCount[i]);
			}

			//�÷ֱ���
			StatusPlay.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(StatusPlay.cbScoreCardData,m_cbScoreCardData,sizeof(StatusPlay.cbScoreCardData));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_UG_WAIT:	//�ȴ�״̬
		{
			//��������
			CMD_S_StatusWait StatusWait;
			ZeroMemory(&StatusWait,sizeof(StatusWait));

			//�û�����
			StatusWait.wBankerUser=m_wBankerUser;

			//��Ϸ����
			StatusWait.cbPackCount=m_cbPackCount;
			StatusWait.cbMainValue=m_cbMainValue;
			StatusWait.cbValueOrder[0]=m_cbValueOrder[0];
			StatusWait.cbValueOrder[1]=m_cbValueOrder[1];

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusWait,sizeof(StatusWait));
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
	case SUB_C_CALL_CARD:		//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallCard));
			if (wDataSize!=sizeof(CMD_C_CallCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_CallCard * pCallCard=(CMD_C_CallCard *)pDataBuffer;
			return OnUserCallCard(pUserData->wChairID,pCallCard->cbCallCard,pCallCard->cbCallCount);
		}
	case SUB_C_CALL_FINISH:		//�������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserCallFinish(pUserData->wChairID);
		}
	case SUB_C_CONCEAL_CARD:	//�����˿�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ConcealCard));
			if (wDataSize!=sizeof(CMD_C_ConcealCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_ConcealCard * pConcealCard=(CMD_C_ConcealCard *)pDataBuffer;
			return OnUserConcealCard(pUserData->wChairID,pConcealCard->cbConcealCard,pConcealCard->cbConcealCount);
		}
	case SUB_C_KOU_WANG:	//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_KouWang));
			if (wDataSize!=sizeof(CMD_C_KouWang)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_KouWang * pKouWang=(CMD_C_KouWang *)pDataBuffer;
			return OnUserKouWang(pUserData->wChairID,pKouWang->cbConcealCard,pKouWang->cbConcealCount,pKouWang->cbUserCardData,pKouWang->cbUserCardCount);
		}
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
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//���ñ���
	if (bLookonUser==false)
	{
		m_cbMainValue=0x01;
		m_cbValueOrder[0]=0x01;
		m_cbValueOrder[1]=0x01;
		m_wBankerUser=INVALID_CHAIR;
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserCallCard(WORD wChairID, BYTE cbCallCard, BYTE cbCallCount)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//�û�Ч��
	ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//��Ŀͳ��
	BYTE cbHandCardCount=0;
	for (BYTE i=0;i<m_cbHandCardCount[wChairID];i++)
	{
		if (m_cbHandCardData[wChairID][i]==cbCallCard) cbHandCardCount++;
	}

	//Ч������
	ASSERT((cbHandCardCount>=cbCallCount)&&(cbCallCount>0));
	if ((cbHandCardCount<cbCallCount)||(cbCallCount==0)) return false;

	//�����ж�
	if (m_wCallCardUser==wChairID)
	{
		//Ч������
		ASSERT((cbCallCard==m_cbCallCard)&&(cbCallCount>m_cbCallCount));
		if ((cbCallCard!=m_cbCallCard)||(cbCallCount<=m_cbCallCount)) return true;
	}
	else
	{
		//Ч������
		BYTE cbPackCount=m_GameLogic.GetPackCount();
		BYTE cbCardColor=m_GameLogic.GetCardColor(cbCallCard);
		BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);

		//Ч������
		ASSERT((cbCallCount>m_cbCallCount)||((cbCallCount==cbPackCount)&&(cbCardColor>cbCallColor)));
		if ((cbCallCount<=m_cbCallCount)&&((cbCallCount!=cbPackCount)||(cbCardColor<=cbCallColor))) return true;
	}

	//���ñ���
	m_cbCallCard=cbCallCard;
	m_wCallCardUser=wChairID;
	m_cbCallCount=cbCallCount;

	//��������
	CMD_S_CallCard CallCard;
	CallCard.cbCallCard=m_cbCallCard;
	CallCard.cbCallCount=m_cbCallCount;
	CallCard.wCallCardUser=m_wCallCardUser;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));

	return true;
}

//�������
bool CTableFrameSink::OnUserCallFinish(WORD wChairID)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//�û�Ч��
	ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) return false;

	//��������
	m_bCallCard[wChairID]=true;

	//����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bCallCard[i]==false) break;
	}
	if (i!=m_wPlayerCount) return true;

	//ׯ������
	if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wCallCardUser;

	//���·���
	if (m_wBankerUser==INVALID_CHAIR)
	{
		//����״̬
		ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
		
		m_bNotCall=true;
		m_wBankerUser=0;
		m_wCallCardUser=0;
		//�����˿�
		//DispatchUserCard();
		//return true;
	}

	//��ɫ����
	if(m_bNotCall==false)
	{
		if (m_wCallCardUser==INVALID_CHAIR) m_cbMainColor=COLOR_NT;
		else m_cbMainColor=m_GameLogic.GetCardColor(m_cbCallCard);
	}
	else
	{
		BYTE bCardValue=m_GameLogic.GetCardValue(m_cbConcealCard[2]);
		m_cbMainColor=m_GameLogic.GetCardColor(m_cbConcealCard[2]);
		if(bCardValue==14) m_cbMainColor=COLOR_HEI_TAO;
		if(bCardValue==15) m_cbMainColor=COLOR_HONG_TAO;
		if(bCardValue==m_cbMainValue)
		{
			m_cbMainColor=m_GameLogic.GetCardColor(bCardValue);
			//�ɹ��ͳ���
			m_bIsGanGuoBa=true;

			//���ñ���
			WORD wHaveKing=VALUE_ERROR;
			m_bIsYingGanGuoBa=false;
			
			//�ж���Ӳ����
			for(BYTE i=0;i<m_cbConcealCount;i++)
			{
				wHaveKing=m_cbConcealCard[i];
				if(wHaveKing==78||wHaveKing==79) 
				{
					m_bIsYingGanGuoBa=true;
				}
			}
			m_bIsRuanGanGuoBa=(m_bIsYingGanGuoBa==true)?false:true;
		}
	}
	
	//���ñ���
	m_wCurrentUser=m_wBankerUser;
	m_wFirstOutUser=m_wBankerUser;
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_pITableFrame->SetGameStatus(GS_UG_BACK);

	//���õ���
	BYTE cbHandCardCount=m_cbHandCardCount[m_wCurrentUser];
	CopyMemory(&m_cbHandCardData[m_wCurrentUser][cbHandCardCount],m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));
	m_cbHandCardCount[m_wCurrentUser]+=m_cbConcealCount;

	//��������
	CMD_S_SendConceal SendConceal;
	SendConceal.cbMainColor=m_cbMainColor;
	SendConceal.wBankerUser=m_wBankerUser;
	SendConceal.wCurrentUser=m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�������
		if (i==m_wCurrentUser)
		{
			SendConceal.cbConcealCount=m_cbConcealCount;
			CopyMemory(SendConceal.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
		}
		else 
		{
			SendConceal.cbConcealCount=0;
			ZeroMemory(SendConceal.cbConcealCard,sizeof(SendConceal.cbConcealCard));
		}

		//������Ϣ
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
	}

	return true;
}

//�����˿�
bool CTableFrameSink::OnUserConcealCard(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_BACK);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_BACK) return true;

	//Ч������
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbConcealCount==cbConcealCount));
	if ((wChairID!=m_wCurrentUser)||(m_cbConcealCount!=cbConcealCount)) return false;

	//ɾ������
	if (m_GameLogic.RemoveCard(cbConcealCard,cbConcealCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//�����˿�
	m_cbHandCardCount[wChairID]-=cbConcealCount;
	CopyMemory(m_cbConcealCard,cbConcealCard,cbConcealCount*sizeof(BYTE));

	//�˿�����
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		ASSERT(m_cbHandCardCount[i]==m_GameLogic.GetDispatchCount());
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i]);
	}

	//�ɹ���״̬���ؿ���
	if(m_bIsGanGuoBa==false)
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_UG_KOU);

		m_bKouFlag[m_wCurrentUser]=true;
		WORD wNextUser=INVALID_CHAIR;

		//�ж��û�
		for(WORD i=0;i<m_wPlayerCount;i++)
		{
			if(i==INVALID_CHAIR) break;
			WORD wHaveKing=VALUE_ERROR;
			if(m_wCurrentUser!=i)
			{
				m_bKouFlag[i]=true;
				for(WORD j=0;j<m_cbHandCardCount[i];j++)
				{
					wHaveKing=m_cbHandCardData[i][j];
					if(wHaveKing==78||wHaveKing==79) 
					{
						wNextUser=i;
						m_bKouFlag[i]=false;
						break;
					}
				}
			}
		}
		
		//��������
		CMD_S_KouWang KouWang;
		KouWang.wNextUser=wNextUser;
		
		//������Ϣ
		for(i=0;i<m_wPlayerCount;i++)
		{
			//�������
			KouWang.cbConcealCount=m_cbConcealCount;
			CopyMemory(KouWang.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			m_pITableFrame->SendTableData(i,SUB_S_KOU_WANG,&KouWang,sizeof(KouWang));
			m_pITableFrame->SendLookonData(i,SUB_S_KOU_WANG,&KouWang,sizeof(KouWang));
		}
	}
	else
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_UG_PLAY);

		//��������
		CMD_S_GamePlay GamePlay;
		GamePlay.wCurrentUser=m_wCurrentUser;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�������
			GamePlay.cbConcealCount=m_cbConcealCount;
			CopyMemory(GamePlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			
			//������Ϣ
			m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		}
	}
	return true;
}

//�û�����
bool CTableFrameSink::OnUserKouWang(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount,BYTE cbUserCard[], BYTE cbUserCardCount)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_KOU);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_KOU) return true;

	//Ч���û�
	ASSERT((cbUserCardCount<=MAX_COUNT)&&(cbConcealCount<=8));
	if ((cbUserCardCount>MAX_COUNT)||(cbConcealCount>8)) return false;

	//��������
	m_bKouFlag[wChairID]=true;
	CopyMemory(m_cbConcealCard,cbConcealCard,sizeof(BYTE)*cbConcealCount);
	CopyMemory(m_cbHandCardData[wChairID],cbUserCard,sizeof(BYTE)*cbUserCardCount);

	//�Ƿ񶼾�������
	bool bAllKou=true;   

	for(BYTE i=0;i<m_wPlayerCount;i++)
	{
		if(m_bKouFlag[i]==false) 
		{
			bAllKou=false;	
			break;
		}
	}

	//������û����
	if(bAllKou==false)
	{
		//��������
		CMD_S_KouWang KouWang;
		for(BYTE i=0;i<m_wPlayerCount;i++)
		{
			if(m_bKouFlag[i]==false)
			{
				KouWang.wNextUser=i;
				break;
			}
		}

		//������Ϣ
		for(i=0;i<m_wPlayerCount;i++)
		{
			//�������
			KouWang.cbConcealCount=cbConcealCount;
			CopyMemory(KouWang.cbConcealCard,cbConcealCard,sizeof(BYTE)*cbConcealCount);

			m_pITableFrame->SendTableData(i,SUB_S_KOU_WANG,&KouWang,sizeof(KouWang));
			m_pITableFrame->SendLookonData(i,SUB_S_KOU_WANG,&KouWang,sizeof(KouWang));
		}
	}
	else
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_UG_PLAY);

		//��������
		CMD_S_GamePlay GamePlay;
		GamePlay.wCurrentUser=m_wCurrentUser;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�������
			GamePlay.cbConcealCount=m_cbConcealCount;
			CopyMemory(GamePlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			
			//������Ϣ
			m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		}
	}

	return true;
}


//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_PLAY) return true;

	//Ч���û�
	ASSERT((wChairID==m_wCurrentUser)&&(cbCardCount<=MAX_COUNT));
	if ((wChairID!=m_wCurrentUser)||(cbCardCount>MAX_COUNT)) return false;

	//�׳�����
	if (m_wFirstOutUser==wChairID)
	{
		//�����ж�
		BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);
		if (cbCardType==CT_ERROR) return false;
	
		//˦�ƴ���
		if (cbCardType==CT_THROW_CARD)
		{
			//˦���ж�
			tagOutCardResult OutCardResult;
			ZeroMemory(&OutCardResult,sizeof(OutCardResult));
			bool bThrowSuccess=m_GameLogic.EfficacyThrowCard(cbCardData,cbCardCount,wChairID,m_cbHandCardData,m_cbHandCardCount[wChairID],OutCardResult);

			//�������
			if (bThrowSuccess==false)
			{
				//ɾ���˿�
				if (m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
				m_cbHandCardCount[wChairID]-=OutCardResult.cbCardCount;

				//���Ƽ�¼
				m_cbOutCardCount[wChairID]=OutCardResult.cbCardCount;
				CopyMemory(m_cbOutCardData[wChairID],OutCardResult.cbResultCard,OutCardResult.cbCardCount);

				//�л��û�
				m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

				//��������
				CMD_S_ThrowResult ThrowResult;
				ThrowResult.wOutCardUser=wChairID;
				ThrowResult.wCurrentUser=m_wCurrentUser;
				ThrowResult.cbThrowCardCount=cbCardCount;
				ThrowResult.cbResultCardCount=OutCardResult.cbCardCount;
				CopyMemory(&ThrowResult.cbCardDataArray[0],cbCardData,sizeof(BYTE)*cbCardCount);
				CopyMemory(&ThrowResult.cbCardDataArray[cbCardCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);

				//���ͼ���
				BYTE cbCardDataCount=ThrowResult.cbThrowCardCount+ThrowResult.cbResultCardCount;
				WORD wSendSize=sizeof(ThrowResult)-sizeof(ThrowResult.cbCardDataArray)+cbCardDataCount*sizeof(BYTE);

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);

				return true;
			}
			
		}
		
	}
	else
	{
		//��Ŀ�ж�
		ASSERT(cbCardCount==m_cbOutCardCount[m_wFirstOutUser]);
		if (cbCardCount!=m_cbOutCardCount[m_wFirstOutUser]) return false;

		//����Ч��
		if (m_GameLogic.EfficacyOutCard(cbCardData,cbCardCount,m_cbOutCardData[m_wFirstOutUser],
			m_cbOutCardCount[m_wFirstOutUser],m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
	m_cbHandCardCount[wChairID]-=cbCardCount;

	//���Ƽ�¼
	m_cbOutCardCount[wChairID]=cbCardCount;
	CopyMemory(m_cbOutCardData[wChairID],cbCardData,cbCardCount);

	//�л��û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wFirstOutUser) m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//һ�ֽ���
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//�ж�ʤ��
		m_wTurnWinner=m_GameLogic.CompareCardArray(m_cbOutCardData,m_cbOutCardCount[0],m_wFirstOutUser);

		//����÷�
		if ((m_wTurnWinner!=m_wBankerUser)&&((m_wTurnWinner+2)%m_wPlayerCount!=m_wBankerUser))
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				BYTE * pcbScoreCard=&m_cbScoreCardData[m_cbScoreCardCount];
				BYTE cbMaxCount=CountArray(m_cbScoreCardData)-m_cbScoreCardCount;
				m_cbScoreCardCount+=m_GameLogic.GetScoreCard(m_cbOutCardData[i],m_cbOutCardCount[i],pcbScoreCard,cbMaxCount);
			}
		}

		//���ñ���
		if (m_cbHandCardCount[m_wTurnWinner]!=0)
		{
			m_wFirstOutUser=m_wTurnWinner;
			ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
			ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
		}

		//�û��л�
		m_wCurrentUser=(m_cbHandCardCount[m_wTurnWinner]==0)?INVALID_CHAIR:m_wTurnWinner;

		//��������
		CMD_TurnBalance TurnBalance;
		TurnBalance.wTurnWinner=m_wTurnWinner;
		TurnBalance.wCurrentUser=m_wCurrentUser;

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
	}

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(m_wTurnWinner,NULL,GER_NORMAL);

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchUserCard()
{
	//�����˿�
	BYTE cbRandCard[CELL_PACK*2];
	m_GameLogic.RandCardList(cbRandCard,CELL_PACK*m_cbPackCount);

	//�û��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbHandCardCount[i]=m_GameLogic.GetDispatchCount();
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
	}

	//�����˿�
	m_cbConcealCount=CELL_PACK*m_cbPackCount-m_wPlayerCount*m_GameLogic.GetDispatchCount();
	CopyMemory(m_cbConcealCard,&cbRandCard[CELL_PACK*m_cbPackCount-m_cbConcealCount],sizeof(BYTE)*m_cbConcealCount);

	//���Ϳ�ʼ
	CMD_S_SendCard SendCard;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//Ч��״̬
		ASSERT(m_cbHandCardCount[i]<=CountArray(SendCard.cbCardData));
		if (m_cbHandCardCount[i]>CountArray(SendCard.cbCardData)) return false;

		//�����˿�
		SendCard.cbCardCount=m_cbHandCardCount[i];
		CopyMemory(SendCard.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//����ͳ��
		WORD wHeadSize=sizeof(SendCard)-sizeof(SendCard.cbCardData);
		WORD wSendSize=wHeadSize+sizeof(SendCard.cbCardData[0])*SendCard.cbCardCount;

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
