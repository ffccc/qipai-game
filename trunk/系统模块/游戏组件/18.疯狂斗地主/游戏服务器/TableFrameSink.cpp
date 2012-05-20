#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//�궨��
#define	GOOD_CARD_PERCENT				4									//���Ƹ���

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bLandScore=0;
	m_wFirstUser=rand()%GAME_PLAYER;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

	//������
	m_bBrightCard=false;
	m_bCallScorePhase=CSD_NORMAL;
	ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );
	ZeroMemory( m_bBrightStart, sizeof( m_bBrightStart ) );
	m_cbBrightCardTime = 1;
	m_cbDoubleCount=0;
	ZeroMemory( m_bDoubleUser, sizeof( m_bDoubleUser ) );
	m_bAllowDoubleScore = false;
	ZeroMemory(m_bUserBrightCard, sizeof(m_bUserBrightCard));
	m_cbBrightCardCount=0;

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	memset(m_bBackCard,0,sizeof(m_bBackCard));
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
	m_wBombTime=1;
	m_bLandScore=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

	//������
	m_bBrightCard=false;
	m_bCallScorePhase=CSD_NORMAL;
	ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );
	ZeroMemory( m_bBrightStart, sizeof( m_bBrightStart ) );
	m_cbBrightCardTime = 1;
	m_cbDoubleCount=0;
	ZeroMemory( m_bDoubleUser, sizeof( m_bDoubleUser ) );
	ZeroMemory(m_bUserBrightCard, sizeof(m_bUserBrightCard));
	m_cbBrightCardCount=0;

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	memset(m_bBackCard,0,sizeof(m_bBackCard));
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
	//������
	ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );

	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_SCORE);

	BYTE cbSendGoodCard = rand () % GOOD_CARD_PERCENT ;
	BYTE cbAndroidUserCount = 0 ;
	WORD wAndroidUser[ GAME_PLAYER ] = {0};

	//�����˸���
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( wChairID ) ;
		if ( pServerUserItem->IsAndroidUser() )
		{
			wAndroidUser[ cbAndroidUserCount ] = ( wChairID ) ;
			++cbAndroidUserCount ;
		}
	}

	//���ƴ���
	m_cbBrightCardCount = cbAndroidUserCount;

	WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	if ( 0 < cbAndroidUserCount ) wHaveGoodCardAndroidUser = wAndroidUser[ rand() % cbAndroidUserCount ] ;

	//���ͺ���
	if ( 1 == cbSendGoodCard && wHaveGoodCardAndroidUser != INVALID_CHAIR )
	{
		//�����˿�
		BYTE bRandCard[54];
		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//��ȡ����
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

		//�ַ��˿�
		CopyMemory( m_bHandCardData[ wHaveGoodCardAndroidUser ], cbGoodCard, NORMAL_COUNT ) ;
		m_bCardCount[ wHaveGoodCardAndroidUser ] = 17;
		for ( WORD i = 0, j = 0; i < m_wPlayerCount; i++ )
		{
			if ( i != wHaveGoodCardAndroidUser )
			{
				m_bCardCount[ i ] = 17;
				CopyMemory( &m_bHandCardData[ i ], &bRandCard[ j * 17 ], sizeof( BYTE ) * 17 );
				++j ;
			}
		}
		CopyMemory( m_bBackCard, &bRandCard[ 34 ], sizeof( m_bBackCard ) );
		m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
	}
	else
	{
		//�����˿�
		BYTE bRandCard[ 54 ];

		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

		//�ַ��˿�
		for ( WORD i = 0; i < m_wPlayerCount; i++ )
		{
			m_bCardCount[ i ] = 17;
			CopyMemory( &m_bHandCardData[ i ], &bRandCard[ i * m_bCardCount[ i ] ], sizeof( BYTE ) * m_bCardCount[ i ] );
		}
		CopyMemory( m_bBackCard, &bRandCard[ 51 ], sizeof( m_bBackCard ) );
		m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
	}

	//�����û�
	if ( m_wCurrentUser == INVALID_CHAIR ) m_wCurrentUser = m_wFirstUser;

	//�����˿�
	CMD_S_SendAllCard SendAllCard ;
	SendAllCard.wCurrentUser = m_wCurrentUser ;

	//������Ϣ
	CopyMemory(SendAllCard.bUserBrightCard, m_bUserBrightCard, sizeof(SendAllCard.bUserBrightCard));

	//�����˿�
	CopyMemory( SendAllCard.bCardData, m_bHandCardData, sizeof( m_bHandCardData ) ) ;
	//�����˿�
	CopyMemory( SendAllCard.bBackCardData, m_bBackCard, 3 ) ;
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_pITableFrame->SendTableData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
		m_pITableFrame->SendLookonData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
	}

	//�����˿�
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_GameLogic.SortCardList( m_bHandCardData[ i ], m_bCardCount[ i ], ST_ORDER );
	}

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd( WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason )
{
	switch ( cbReason )
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			ASSERT( pIServerUserItem != NULL );
			ASSERT( wChairID < m_wPlayerCount );

			//��������
			CMD_S_GameEnd GameEnd;
			memset( &GameEnd, 0, sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				GameEnd.bCardCount[ i ] = m_bCardCount[ i ];
				CopyMemory( &GameEnd.bCardData[ bCardPos ], m_bHandCardData[ i ], m_bCardCount[ i ] * sizeof( BYTE ) );
				bCardPos += m_bCardCount[ i ];
			}

			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof( GameEnd ) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof( GameEnd ) );

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

			//��������
			LONG lCellScore=m_pGameServiceOption->lCellScore;
			bool bLandWin=(m_bCardCount[m_wBankerUser]==0)?true:false;

			//��������
			WORD wDrawTimes=(WORD)m_pGameServiceOption->lCellScore;
			for (WORD i=1;i<m_bLandScore;i++) wDrawTimes*=2;

			//���Ʊ���
			if ( m_cbBrightCardTime != 0 ) wDrawTimes *= m_cbBrightCardTime;

			//�����ж�
			if ( wChairID == m_wBankerUser )
			{
				WORD wUser1 = ( m_wBankerUser + 1 ) % GAME_PLAYER;
				WORD wUser2 = ( m_wBankerUser + 2 ) % GAME_PLAYER;
				if ( ( m_bOutCardCount[ wUser1 ] == 0 ) && ( m_bOutCardCount[ wUser2 ] == 0 ) ) wDrawTimes *= 2;
			}
			else
			{
				if ( m_bOutCardCount[ m_wBankerUser ] == 1 ) wDrawTimes *= 2;
			}

			//�������
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i==m_wBankerUser) GameEnd.lGameScore[i]=m_wBombTime*wDrawTimes*((bLandWin==true)?2:-2);
				else GameEnd.lGameScore[i]=m_wBombTime*wDrawTimes*((bLandWin==true)?-1:1);
			}

			//����ƽ��
			if ( m_pGameServiceOption->wServerType&GAME_GENRE_GOLD )
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]<0L)
					{
						IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
						if ((pIServerUserItem->GetUserScore()->lScore+GameEnd.lGameScore[i])<0L)
						{
							//��ҵ���
							if (i==m_wBankerUser)
							{
								for (WORD j=0;j<m_wPlayerCount;j++)
								{
									if (j!=m_wBankerUser) 
									{
										GameEnd.lGameScore[j]=pIServerUserItem->GetUserScore()->lScore/2;
									}
								}
							}
							else
							{
								GameEnd.lGameScore[m_wBankerUser]-=(-GameEnd.lGameScore[i]-pIServerUserItem->GetUserScore()->lScore);
							}

							//�������
							GameEnd.lGameScore[i]=-pIServerUserItem->GetUserScore()->lScore;
						}
					}
				}
			}

			//ͳ�ƻ���
			LONG lScore=0;
			LONG lRevenue=0;
			enScoreKind ScoreKind;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//����˰��
				lRevenue=0;
				if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD && GameEnd.lGameScore[i]>=100L)
				{
					GameEnd.lGameTax+=GameEnd.lGameScore[i]/50L;
					GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*49L/50L;
					lRevenue = GameEnd.lGameScore[i]/50L;
				}
				lScore = GameEnd.lGameScore[i];
				ScoreKind = lScore > 0 ? enScoreKind_Win : enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i, lScore, lRevenue, ScoreKind );
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�л��û�
			m_wFirstUser=wChairID;

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
			WORD wDrawTimes=(WORD)m_pGameServiceOption->lCellScore;
			for (WORD i=1;i<m_bLandScore;i++) wDrawTimes*=2;

			//ը������
			for ( WORD wUserID = 0; wUserID < GAME_PLAYER; ++wUserID )
			{
				//�����˿�
				tagAnalyseResult AnalyseResult ;
				ZeroMemory(&AnalyseResult, sizeof( AnalyseResult ) );
				m_GameLogic.AnalysebCardData(m_bHandCardData[wUserID], m_bCardCount[wUserID], AnalyseResult) ;

				//ը������
				for (WORD i=1;i<AnalyseResult.cbFourCount;i++) wDrawTimes*=2;

				//��ը�ж�
				if ( m_bHandCardData[wUserID][0] == 0x4F && m_bHandCardData[wUserID][1] == 0x4E ) wDrawTimes *= 2;
			}

			//���Ʊ���
			if ( m_cbBrightCardTime != 0 ) wDrawTimes *= m_cbBrightCardTime;

			//�ѳ�ը��
			wDrawTimes *= m_wBombTime;

			//�����ж�
			if ( wDrawTimes < 10 )
			{
				if ( wChairID == m_wBankerUser ) wDrawTimes = 20;
				else wDrawTimes = 10;
			}
			else
			{
				if ( wChairID == m_wBankerUser ) wDrawTimes *= 4;
				else wDrawTimes *= 2;
			}

			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			GameEnd.lGameScore[wChairID] = -wDrawTimes;

			//��ҷ���
			if ( m_pGameServiceOption->wServerType&GAME_GENRE_GOLD )
			{	
				//���ƽ��
				IServerUserItem *pDrawServerUserItem = m_pITableFrame->GetServerUserItem( wChairID );
				if ( pDrawServerUserItem->GetUserScore()->lScore + GameEnd.lGameScore[wChairID] < 0 ) 
					GameEnd.lGameScore[wChairID] = - pDrawServerUserItem->GetUserScore()->lScore;

				//�������
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if ( i != wChairID ) GameEnd.lGameScore[i] = (-GameEnd.lGameScore[wChairID])/2;
				}

				//ͳ�ƻ���
				LONG lScore=0;
				LONG lRevenue=0;
				enScoreKind ScoreKind;
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//����˰��
					lRevenue=0;
					if ( GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/50L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*49L/50L;
						lRevenue = GameEnd.lGameScore[i]/50L;
					}
					lScore = GameEnd.lGameScore[i];
					ScoreKind = lScore > 0 ? enScoreKind_Win : enScoreKind_Lost;
					m_pITableFrame->WriteUserScore(i, lScore, lRevenue, ScoreKind );
				}
			}

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

			//�޸Ļ���
			m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,enScoreKind_Draw);

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
			CopyMemory( StatusFree.bBrightStart, m_bBrightStart, sizeof( StatusFree.bBrightStart ) );

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_SCORE:		//�з�״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//���ñ���
			StatusScore.bLandScore=m_bLandScore;
			StatusScore.wCurrentUser=m_wCurrentUser;
			StatusScore.lBaseScore=m_pGameServiceOption->lCellScore;
			StatusScore.bCallScorePhase=m_bCallScorePhase;
			CopyMemory(StatusScore.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
	//		CopyMemory(StatusScore.bCardData,m_bHandCardData[wChiarID],m_bCardCount[wChiarID]);
			CopyMemory(StatusScore.bCardData,m_bHandCardData, sizeof(m_bHandCardData)*sizeof(BYTE));
			CopyMemory( StatusScore.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			StatusScore.bBrightTime = m_cbBrightCardTime;
			CopyMemory( StatusScore.bUserBrightCard, m_bUserBrightCard, sizeof( StatusScore.bUserBrightCard ) );

			//�����ж�
			if ( m_bCallScorePhase == CSD_BRIGHTCARD && m_wCurrentUser!=wChiarID && m_bUserBrightCard[wChiarID]==false ) m_cbBrightCardCount++;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_WK_DOUBLE_SCORE:	//�ӱ�״̬
		{
			//������Ϣ
			CMD_S_StatusDoubleScore StatusDoubleScore;
			ZeroMemory( &StatusDoubleScore, sizeof( StatusDoubleScore ) );
			StatusDoubleScore.bAllowDouble = m_bAllowDoubleScore;
			CopyMemory(StatusDoubleScore.bBackCard, m_bBackCard, sizeof(m_bBackCard ) );
			CopyMemory(StatusDoubleScore.bCardCount, m_bCardCount, sizeof(m_bCardCount ) );
			CopyMemory(StatusDoubleScore.bCardData, m_bHandCardData, sizeof(StatusDoubleScore.bCardData ) );
			StatusDoubleScore.bLandScore = m_bLandScore;
			CopyMemory(StatusDoubleScore.bUserTrustee, m_bUserTrustee, sizeof(StatusDoubleScore.bUserTrustee ) );	
			StatusDoubleScore.lBaseScore = m_pGameServiceOption->lCellScore;
			StatusDoubleScore.wLandUser = m_wBankerUser;
			CopyMemory(StatusDoubleScore.bDoubleUser, m_bDoubleUser, sizeof(StatusDoubleScore.bDoubleUser ) );	
			CopyMemory( StatusDoubleScore.bUserBrightCard, m_bUserBrightCard, sizeof( StatusDoubleScore.bUserBrightCard ) );


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusDoubleScore,sizeof(StatusDoubleScore));
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBombTime=m_wBombTime;
			StatusPlay.wLandUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.bLandScore=m_bLandScore;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
	//		CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData,sizeof(BYTE)*sizeof(m_bHandCardData));
			CopyMemory( StatusPlay.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			StatusPlay.bBrightTime = m_cbBrightCardTime;
			CopyMemory( StatusPlay.bUserBrightCard, m_bUserBrightCard, sizeof( StatusPlay.bUserBrightCard ) );

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
	case SUB_C_LAND_SCORE:	//�û��з�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_LandScore));
			if (wDataSize!=sizeof(CMD_C_LandScore)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_LandScore * pLandScore=(CMD_C_LandScore *)pDataBuffer;
			return OnUserLandScore(pUserData->wChairID,pLandScore->bLandScore);
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
	case SUB_C_TRUSTEE:			//����й�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserTrustee));
			if (wDataSize != sizeof(CMD_C_UserTrustee)) return false;

			//��������
			CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pDataBuffer;

			//��Ϣ����
			return OnUserTrustee(pUserTrustee->wUserChairID, pUserTrustee->bTrustee);			
		}
	case SUB_C_BRIGHT_START:	//���ƿ�ʼ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			
			//��Ϣ����
			return OnBrightStart( pUserData->wChairID, true );
		}
	case SUB_C_BRIGHT:		//�������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_BrightCard));
			if (wDataSize != sizeof(CMD_C_BrightCard)) return false;			

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			
			//��������
			CMD_C_BrightCard *pBrightCard = ( CMD_C_BrightCard * )pDataBuffer;

			//��Ϣ����
			return OnBrightCard( pUserData->wChairID, pBrightCard->cbBrightCardTime);
		}
	case SUB_C_DOUBLE_SCORE:	//��Ҽӱ�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_DoubleScore));
			if (wDataSize != sizeof(CMD_C_DoubleScore)) return false;			

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			
			//��������
			CMD_C_DoubleScore *pDoubleScore = ( CMD_C_DoubleScore * )pDataBuffer;

			//��Ϣ����
			return OnUserDoubleScore( pUserData->wChairID, pDoubleScore->bDoubleScore);
		}
	}

	return true;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�з��¼�
bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bLandScore)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_SCORE) return true;

	//��������
	bool bContinue=false;
	BYTE cbPreCallScorePhase = m_bCallScorePhase;

	//�зִ���
	if (m_bCallScorePhase==CSD_NORMAL)
	{
		//Ч�����
		ASSERT(((bLandScore<=3)||(bLandScore==255))&&(bLandScore>m_bLandScore));
		if (((bLandScore>3)&&(bLandScore!=255))||(bLandScore<=m_bLandScore)) return false;

		//���ñ���
		if (bLandScore!=255)
		{
		//	m_bLandScore=bLandScore;
			m_bLandScore=1;
			m_wBankerUser=m_wCurrentUser;
		}
		else
		{
			bContinue = true;
			m_bGiveUpBanker[wChairID] = true;

			//�л����
			m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

			//���·���
			if ( m_bGiveUpBanker[m_wCurrentUser] )
			{
				//���·���
				if ( m_wBankerUser == INVALID_CHAIR )
				{
					//������
					m_bBrightCard=false;
					m_bCallScorePhase=CSD_NORMAL;
					ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );

					BYTE cbSendGoodCard = rand() % GOOD_CARD_PERCENT ;
					BYTE cbAndroidUserCount=0 ;
					WORD wAndroidUser[GAME_PLAYER]={0};

					//���ñ���
					ZeroMemory(m_bScoreInfo, sizeof(m_bScoreInfo)) ;

					//�����˸���
					for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
					{
						IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID) ;
						if(pServerUserItem->IsAndroidUser())
						{
							wAndroidUser[cbAndroidUserCount] = (wChairID) ;
							++cbAndroidUserCount ;

						}
					}

					WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
					if(cbAndroidUserCount>0) wHaveGoodCardAndroidUser = wAndroidUser[rand()%cbAndroidUserCount] ;

					//���ͺ���
					if( 0 == cbSendGoodCard && wHaveGoodCardAndroidUser!=INVALID_CHAIR)
					{
						//�����˿�
						BYTE bRandCard[54];
						m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

						BYTE cbGoodCard[NORMAL_COUNT] ;
						m_GameLogic.GetGoodCardData(cbGoodCard) ;

						//��ȡ����
						m_GameLogic.RemoveGoodCardData(cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

						//�ַ��˿�
						CopyMemory(m_bHandCardData[wHaveGoodCardAndroidUser], cbGoodCard, NORMAL_COUNT) ;
						m_bCardCount[wHaveGoodCardAndroidUser]=17;
						for (WORD i=0, j=0;i<m_wPlayerCount;i++)
						{
							if(i!=wHaveGoodCardAndroidUser)
							{
								m_bCardCount[i]=17;
								CopyMemory(&m_bHandCardData[i],&bRandCard[j*17],sizeof(BYTE)*17);
								++j ;
							}
						}
						CopyMemory(m_bBackCard,&bRandCard[34],sizeof(m_bBackCard));
						m_GameLogic.SortCardList(m_bBackCard,3,ST_ORDER);
					}
					else
					{
						//�����˿�
						BYTE bRandCard[54];
						m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

						//�ַ��˿�
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							m_bCardCount[i]=17;
							CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
						}
						CopyMemory(m_bBackCard,&bRandCard[51],sizeof(m_bBackCard));
						m_GameLogic.SortCardList(m_bBackCard,3,ST_ORDER);
					}

					//�����û�
					m_wCurrentUser=m_wFirstUser;

					//�����˿�
					CMD_S_SendAllCard SendAllCard ;
					ZeroMemory(&SendAllCard, sizeof(SendAllCard));
					SendAllCard.wCurrentUser = m_wCurrentUser ;

					//�����˿�
					CopyMemory(SendAllCard.bCardData, m_bHandCardData, sizeof(m_bHandCardData)) ;
					//�����˿�
					CopyMemory(SendAllCard.bBackCardData, m_bBackCard, 3) ;
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
						m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
					}

					//�����˿�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i],ST_ORDER);
					}

					return true;
				}
				else
				{
					//�������
					m_wCurrentUser = m_wBankerUser;

					//�л�״̬
					m_bCallScorePhase=CSD_GAMESTART;

					//���ñ���
					bContinue = false;
				}
			}
		}

		m_bScoreInfo[wChairID]=bLandScore;

		//״̬�л�
		if (m_bLandScore==1)
		{
			bContinue=true;
			m_wFirstUser=wChairID;
			m_bCallScorePhase=CSD_SNATCHLAND;

			//�л����
			m_wCurrentUser = INVALID_CHAIR;
			WORD wUser = (wChairID+1)%GAME_PLAYER;
			while (  wUser != wChairID ) 
			{
				if ( m_bGiveUpBanker[wUser] == false )
				{
					m_wCurrentUser = wUser;
					break;
				}
				wUser = (wUser+1)%GAME_PLAYER ;
			}

			if ( m_wCurrentUser == INVALID_CHAIR )
			{
				m_wCurrentUser = wChairID;
				bContinue = false;
			}
		}
	}
	else if (m_bCallScorePhase==CSD_SNATCHLAND)
	{
		//Ч�����
		ASSERT((bLandScore==4)||(bLandScore==255));
		if ((bLandScore!=4)&&(bLandScore!=255)) return false;

		//���ñ���
		if (bLandScore==4)
		{
			m_bLandScore++;
			m_wBankerUser=wChairID;
		}

		//״̬�л�
		if (m_wFirstUser==wChairID)
		{
			bContinue=true;
			m_wFirstUser=m_wBankerUser;
			m_wCurrentUser = m_wBankerUser;

			//�����ж�
			if ( m_cbBrightCardTime > 2 && m_bUserBrightCard[m_wCurrentUser] )
			{
				m_bCallScorePhase = CSD_GAMESTART;
				bContinue = false;
			}
			else
				m_bCallScorePhase=CSD_BRIGHTCARD;
		}
		else bContinue=true;

		//�л����
		if ( m_bCallScorePhase == CSD_SNATCHLAND )
		{
			m_wCurrentUser = INVALID_CHAIR;
			WORD wUser = (wChairID+1)%GAME_PLAYER;
			while (  wUser != wChairID && wUser != (m_wFirstUser+1)%GAME_PLAYER ) 
			{
				if ( m_bGiveUpBanker[wUser] == false )
				{
					m_wCurrentUser = wUser;
					break;
				}
				wUser = (wUser+1)%GAME_PLAYER ;
			}

			if ( m_wCurrentUser == INVALID_CHAIR || ( m_wFirstUser == m_wBankerUser && m_wFirstUser == m_wCurrentUser ))
			{
				m_wCurrentUser = m_wBankerUser;
				bContinue=true;
				m_wFirstUser=m_wBankerUser;

				//�����ж�
				bool bAllUserBright = true;
				for ( WORD wBrightIdx = 0; wBrightIdx < GAME_PLAYER; ++wBrightIdx )
					if ( ! m_bUserBrightCard[wBrightIdx] ) bAllUserBright = false;

				//�����ж�
				if ( m_cbBrightCardTime > 2 && bAllUserBright )
				{
					m_bCallScorePhase = CSD_GAMESTART;
					bContinue = false;
				}
				else
					m_bCallScorePhase=CSD_BRIGHTCARD;
				}
		}
	}
	else if (m_bCallScorePhase==CSD_BRIGHTCARD)
	{
		////Ч�����
		//ASSERT((bLandScore==5)||(bLandScore==255));
		//if ((bLandScore!=5)&&(bLandScore!=255)) return false;

		////���Ӵ���
		//m_cbBrightCardCount++;
		//if ( m_cbBrightCardCount == GAME_PLAYER )
		//{
		//	bContinue = false;
		//	m_bCallScorePhase = CSD_BRIGHTCARD;
		//}
		//else 
		//{
		//	bContinue = true;
		//	m_bCallScorePhase=CSD_GAMESTART;
		//}

		////���ñ���
		//m_bBrightCard=(bLandScore==5)?true:false;
		//if ( bLandScore == 5 )
		//{
		//	m_bLandScore++;
		//	m_bUserBrightCard[wChairID] = true;
		//}
		bContinue = true;
	}

	//��ʼ�ж�
	if ((bContinue==false)&&(m_bLandScore>=1 || m_wBankerUser != INVALID_CHAIR))
	{
		//�л�״̬
		m_bCallScorePhase = CSD_BRIGHTCARD;
	}


	//����״̬
	if ( m_bCallScorePhase == CSD_BRIGHTCARD )
	{
		//���Ƹ���
		for ( WORD wBrightIdx = 0; wBrightIdx < GAME_PLAYER; ++wBrightIdx ) 
			if ( m_bUserBrightCard[wBrightIdx] ) ++m_cbBrightCardCount;
;
	}

	//��ʼ�ж�
	if ( m_cbBrightCardCount == GAME_PLAYER  && CSD_BRIGHTCARD ==m_bCallScorePhase )
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_WK_DOUBLE_SCORE);

		//��ȡ·��
		CHAR szDirectory[128];
		GetCurrentDirectory(sizeof( szDirectory ), szDirectory );

		//�ļ�����
		CHAR szProfileName[128];
		sprintf(szProfileName, TEXT("%s\\LandCrazy.ini"), szDirectory);

		//��ȡ����
		LONG lDoubleLimit = GetPrivateProfileInt(TEXT("DoubleScore"), TEXT("Score"), 10000, szProfileName );

		bool bAllowDouble = true;
		for ( WORD wUserID = 0; wUserID < GAME_PLAYER; ++wUserID )
		{
			//��ȡ���
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem( wUserID );
			if ( pIServerUserItem == NULL ) continue;

			//�����ж�
			LONG lUserScore = pIServerUserItem->GetUserScore()->lScore;
			if ( lUserScore < lDoubleLimit )
			{
				bAllowDouble = false;
				break;
			}
		}
		m_bAllowDoubleScore = bAllowDouble;

		//���͵���
		m_bCardCount[m_wBankerUser]=20;
		CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
		m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

		//������Ϣ
		CMD_S_DoubleScore DoubleScore;
		ZeroMemory( &DoubleScore, sizeof( DoubleScore ) );
		DoubleScore.bAllowDouble = bAllowDouble;
		DoubleScore.wLandUser = m_wBankerUser;
		DoubleScore.bCurrentScore = m_bLandScore;
		CopyMemory(DoubleScore.bBackCard,m_bBackCard,sizeof(m_bBackCard));

		//������Ϣ
		m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_DOUBLE_SCORE, &DoubleScore, sizeof( DoubleScore ) );

		//���ñ���
		if (m_bLandScore==0) m_bLandScore=1;
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

		return true;
	}

	//������Ϣ
	CMD_S_LandScore LandScore;
	ZeroMemory( &LandScore, sizeof( LandScore ) );

	LandScore.bLandUser=wChairID;
	LandScore.bLandScore=bLandScore;
	LandScore.wCurrentUser=m_wCurrentUser;
	LandScore.bCurrentScore=m_bLandScore;
	LandScore.bCallScorePhase=m_bCallScorePhase;
	LandScore.bPreCallScorePhase = cbPreCallScorePhase;
	CopyMemory(LandScore.bBrightCardUser, m_bUserBrightCard, sizeof( LandScore.bBrightCardUser ) );
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) 
		return false;

	//�����˿�
	m_GameLogic.SortCardList(bCardData,bCardCount,ST_ORDER);

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) 
		return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) 
		return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) 
		return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//ը���ж�
	if ((bCardType==CT_BOMB_CARD)||(bCardType==CT_MISSILE_CARD)) m_wBombTime*=2;
	
	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if (bCardType!=CT_MISSILE_CARD) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
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
	if (bCardType==CT_MISSILE_CARD) m_bTurnCardCount=0;

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
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if ( ! bLookonUser )
	{
		//���ñ���
		m_bBrightStart[wChairID] = false;
		m_bUserBrightCard[wChairID] = false;
		m_cbBrightCardTime=1;
		for ( WORD wBrightUser = 0; wBrightUser < GAME_PLAYER; ++wBrightUser )
			if ( m_bBrightStart[wBrightUser] ) m_cbBrightCardTime = BRIGHT_START_TIME;

		//������Ϣ
		OnBrightStart( wChairID, false );
	}

	return true;
}

//����й�
bool CTableFrameSink::OnUserTrustee(WORD wChairID, bool bTrustee)
{
	m_bUserTrustee[ wChairID ] = bTrustee;

	//��������
	CMD_C_UserTrustee UserTrustee;
	ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
	UserTrustee.wUserChairID = wChairID;
	UserTrustee.bTrustee = bTrustee;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_TRUSTEE, &UserTrustee,sizeof(UserTrustee));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_C_TRUSTEE, &UserTrustee,sizeof(UserTrustee));

	return true;
}

//���ƿ�ʼ
bool CTableFrameSink::OnBrightStart( WORD wChairID , bool bBright )
{
	//���ñ���
	m_bBrightStart[wChairID] = bBright;
	m_bUserBrightCard[wChairID] = bBright;

	if ( bBright ) m_cbBrightCardTime=BRIGHT_START_TIME;

	//�������
	if ( bBright && m_wCurrentUser == INVALID_CHAIR )
	{
		m_wCurrentUser = wChairID;
		m_wBankerUser = wChairID;
	}

	//������Ϣ
	CMD_S_BrightStart BrightStart;
	ZeroMemory( &BrightStart, sizeof( BrightStart ) );
	BrightStart.wBrightUser = wChairID;
	BrightStart.bBright = bBright;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BRIGHT_START,&BrightStart,sizeof(BrightStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BRIGHT_START,&BrightStart,sizeof(BrightStart));
	return true;
}

//�������
bool  CTableFrameSink::OnBrightCard( WORD wChairID, BYTE cbBrightCardTime)
{
	//������Ϣ
	if ( 0 < cbBrightCardTime && cbBrightCardTime != 255 ) 
	{
		CHAR szMsg[128] ;
		sprintf(szMsg, TEXT("[ %s ] ���ѡ�������ƣ����ַ�%d��"), m_pITableFrame->GetServerUserItem( wChairID )->GetAccounts(), cbBrightCardTime==5?1:cbBrightCardTime );

		SendGameMessage( INVALID_CHAIR, szMsg );
	}

	//��������
	if ( m_bCallScorePhase != CSD_BRIGHTCARD )
	{
		ASSERT( cbBrightCardTime == BRIGHT_CARD_TIME );
		if ( cbBrightCardTime != BRIGHT_CARD_TIME ) return true;

		//�����ж�
		m_cbBrightCardTime = m_cbBrightCardTime > cbBrightCardTime ? m_cbBrightCardTime : cbBrightCardTime;

		//���ñ���
		m_bUserBrightCard[wChairID] = cbBrightCardTime > 0 ? true:false;

		//������Ϣ
		CMD_S_BrightCard BrightCard;
		ZeroMemory( &BrightCard, sizeof( BrightCard ) );
		BrightCard.wBrightUser = wChairID;
		BrightCard.cbBrightTime = cbBrightCardTime;
		BrightCard.cbCurrenBrightTime = m_cbBrightCardTime;
		BrightCard.cbCallScorePhase = CSD_NORMAL;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BRIGHT_CARD,&BrightCard,sizeof(BrightCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BRIGHT_CARD,&BrightCard,sizeof(BrightCard));
	}
	else
	{
		//Ч�����
		ASSERT((cbBrightCardTime==5)||(cbBrightCardTime==255));
		if ((cbBrightCardTime!=5)&&(cbBrightCardTime!=255)) return false;

		//�����ж�
		if ( m_bUserBrightCard[wChairID] ) return true;

		//���Ӵ���
		m_cbBrightCardCount++;
		if ( m_cbBrightCardCount == GAME_PLAYER )
		{
			m_bCallScorePhase = CSD_GAMESTART;
		}
		else 
		{
			m_bCallScorePhase = CSD_BRIGHTCARD;
		}

		//���ñ���
		if ( cbBrightCardTime == 5 )
		{
			m_bLandScore++;
			m_bUserBrightCard[wChairID] = true;
		}

		//������Ϣ
		CMD_S_BrightCard BrightCard;
		ZeroMemory( &BrightCard, sizeof( BrightCard ) );
		BrightCard.wBrightUser = wChairID;
		BrightCard.cbBrightTime = cbBrightCardTime;
		BrightCard.cbCurrenBrightTime = m_cbBrightCardTime;
		BrightCard.cbCallScorePhase = CSD_BRIGHTCARD;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BRIGHT_CARD,&BrightCard,sizeof(BrightCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BRIGHT_CARD,&BrightCard,sizeof(BrightCard));

		//�����ж�
		if ( m_bCallScorePhase==CSD_GAMESTART )
		{
			//����״̬
			m_pITableFrame->SetGameStatus(GS_WK_DOUBLE_SCORE);

			//��ȡ·��
			CHAR szDirectory[128];
			GetCurrentDirectory(sizeof( szDirectory ), szDirectory );

			//�ļ�����
			CHAR szProfileName[128];
			sprintf(szProfileName, TEXT("%s\\LandCrazy.ini"), szDirectory);

			//��ȡ����
			LONG lDoubleLimit = GetPrivateProfileInt(TEXT("DoubleScore"), TEXT("Score"), 10000, szProfileName );

			bool bAllowDouble = true;
			for ( WORD wUserID = 0; wUserID < GAME_PLAYER; ++wUserID )
			{
				//��ȡ���
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem( wUserID );
				if ( pIServerUserItem == NULL ) continue;

				//�����ж�
				LONG lUserScore = pIServerUserItem->GetUserScore()->lScore;
				if ( lUserScore < lDoubleLimit )
				{
					bAllowDouble = false;
					break;
				}
			}
			m_bAllowDoubleScore = bAllowDouble;

			//���͵���
			m_bCardCount[m_wBankerUser]=20;
			CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
			m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

			//������Ϣ
			CMD_S_DoubleScore DoubleScore;
			ZeroMemory( &DoubleScore, sizeof( DoubleScore ) );
			DoubleScore.bAllowDouble = bAllowDouble;
			DoubleScore.wLandUser = m_wBankerUser;
			DoubleScore.bCurrentScore = m_bLandScore;
			CopyMemory(DoubleScore.bBackCard,m_bBackCard,sizeof(m_bBackCard));

			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_DOUBLE_SCORE, &DoubleScore, sizeof( DoubleScore ) );

			//���ñ���
			if (m_bLandScore==0) m_bLandScore=1;
			if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

			return true;
		}

	}

	return true;
}

//��Ҽӱ�
bool CTableFrameSink::OnUserDoubleScore( WORD wChairID, bool bDoubleScore)
{
	//�ӱ��ж�
	if ( bDoubleScore ) m_bLandScore++;

	//������Ϣ
	if ( bDoubleScore ) 
	{
		CHAR szMsg[128] ;
		sprintf(szMsg, TEXT("[ %s ] ���ѡ���˷��������ַ�1��"), m_pITableFrame->GetServerUserItem( wChairID )->GetAccounts() );

		SendGameMessage( INVALID_CHAIR, szMsg );
	}

	//���ñ���
	m_bDoubleUser[wChairID] = true;

	//������Ϣ
	CMD_S_UserDouble UserDouble;
	ZeroMemory( &UserDouble, sizeof( UserDouble ) );
	UserDouble.bCurrentScore = m_bLandScore;
	UserDouble.bDoubleScore = bDoubleScore;
	UserDouble.wDoubleUser = wChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_DOUBLE,&UserDouble,sizeof(UserDouble));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_DOUBLE,&UserDouble,sizeof(UserDouble));


	//ͳ�ƴ���
	m_cbDoubleCount++;

	//�����ж�
	if ( m_cbDoubleCount == GAME_PLAYER )
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);	

		//������Ϣ
		m_bTurnCardCount=0;
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//������Ϣ
		CMD_S_GameStart GameStart;
		GameStart.wLandUser=m_wBankerUser;
		GameStart.bLandScore=m_bLandScore;
		GameStart.bBrightCard=m_bBrightCard;
		GameStart.wCurrentUser=m_wCurrentUser;
		CopyMemory(GameStart.bBackCardData, m_bBackCard, 3);
		CopyMemory(GameStart.bUserBrightCard, m_bUserBrightCard, sizeof(GameStart.bUserBrightCard));
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//������Ϣ
void CTableFrameSink::SendGameMessage( WORD wChairID, CHAR szMsg[] )
{
	if ( wChairID == INVALID_CHAIR )
	{
		//��Ϸ���
		for ( WORD wUserIdx = 0; wUserIdx < GAME_PLAYER; ++wUserIdx )
		{
			IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wUserIdx );
			m_pITableFrame->SendGameMessage( pUserItem, szMsg, SMT_INFO );
		}

		//�Թ����
		WORD wLookonIdx = 0;
		IServerUserItem *pLookonUserItem = m_pITableFrame->EnumLookonUserItem(wLookonIdx);
		while( pLookonUserItem != NULL )
		{
			wLookonIdx++;
			m_pITableFrame->SendGameMessage( pLookonUserItem, szMsg, SMT_INFO );
			pLookonUserItem = m_pITableFrame->EnumLookonUserItem(wLookonIdx);
		}
		
	}
	else
	{
		IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wChairID );
		m_pITableFrame->SendGameMessage( pUserItem, szMsg, SMT_INFO );
	}
}
//////////////////////////////////////////////////////////////////////////
