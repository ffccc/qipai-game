#include "StdAfx.h"
#include "TableFrameSink.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//�궨��
#define	GOOD_CARD_PERCENT				4									//���Ƹ���

#define	MAX_CALL_TIMES					2									//��������

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bMingCard=0;
	m_bBackCatchTag = 0;
	m_bHaveSendCard = 0;
	m_bLookCardTag = 0;
	m_bCurrentState = 0;
	m_wOutBombCount = 0;
	m_wFirstUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bScoreTimes,0,sizeof(m_bScoreTimes));
	memset(m_bCallScoreTimes,0,sizeof(m_bCallScoreTimes));
	memset(m_bCallScoreTag,0,sizeof(m_bCallScoreTag));
	memset(m_bCallBanker,0,sizeof(m_bCallBanker));

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
	m_bMingCard=0;
	m_bBackCatchTag = 0;
	m_bHaveSendCard = 0;
	m_bLookCardTag = 0;
	m_bCurrentState = 0;
	m_wOutBombCount = 0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bScoreTimes,0,sizeof(m_bScoreTimes));
	memset(m_bCallScoreTimes,0,sizeof(m_bCallScoreTimes));
	memset(m_bCallScoreTag,0,sizeof(m_bCallScoreTag));
	memset(m_bCallBanker,0,sizeof(m_bCallBanker));

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
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_SCORE);

	//�˿˱���
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

	//�����ׯ
	/*if(m_wFirstUser==INVALID_CHAIR)*/m_wFirstUser = rand()%m_wPlayerCount;
	m_wCurrentUser = m_wFirstUser;
	m_bMingCard = m_bHandCardData[m_wCurrentUser][(rand()%17)];

	//��������
	CMD_S_CallBanker CallBanker;
	ZeroMemory(&CallBanker,sizeof(CallBanker));
	CallBanker.wCurrentUser = m_wCurrentUser;
	CallBanker.bMingCard = m_bMingCard;

	//��������
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_pITableFrame->SendTableData( i, SUB_S_CHOICE_LOOK, &CallBanker, sizeof( CallBanker ) );
		m_pITableFrame->SendLookonData( i, SUB_S_CHOICE_LOOK, &CallBanker, sizeof( CallBanker ) );
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
			//ASSERT( pIServerUserItem != NULL );
			//ASSERT( wChairID < m_wPlayerCount );

			//��������
			CMD_S_GameEnd GameEnd;
			memset( &GameEnd, 0, sizeof( GameEnd ) );
			CopyMemory(GameEnd.bBackCard,m_bBackCard,sizeof(m_bBackCard));

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
			ZeroMemory( &GameEnd, sizeof( GameEnd ) );
			CopyMemory(GameEnd.bBackCard,m_bBackCard,sizeof(m_bBackCard));

			//ʣ���˿�
			BYTE bCardPos = 0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//��������
			LONG lCellScore=m_pGameServiceOption->lCellScore;
			bool bLandWin =(m_bCardCount[m_wBankerUser]==0)?true:false;

			//�����ж�
			WORD wBombTime=1;
			if(wChairID==m_wBankerUser)
			{
				WORD wUser1=(m_wBankerUser+1)%GAME_PLAYER;
				WORD wUser2=(m_wBankerUser+2)%GAME_PLAYER;
				if((m_bOutCardCount[wUser1]==0)&&(m_bOutCardCount[wUser2]==0))wBombTime*=2;
			}
			else
			{
				if(m_bOutCardCount[m_wBankerUser]==1)wBombTime*=2;
			}

			//ը������
			//wBombTime= __min( wBombTime, 16 );

			//��Ϸ����
			LONG lScore=0;
			LONG lRevenue=0;
			enScoreKind ScoreKind;

			//ͳ�ƻ���
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				lScore=0;
				lRevenue=0;

				//ͳ�ƻ���
				if ( i == m_wBankerUser )
				{
					WORD wUser1=(m_wBankerUser+1)%m_wPlayerCount,wUser2=(m_wBankerUser+2)%m_wPlayerCount;
					lScore=(m_bScoreTimes[wUser1]+m_bScoreTimes[wUser2])*wBombTime*lCellScore*((bLandWin==true)?1:-1);
					GameEnd.lGameScore[i]=lScore;
				}
				else 
				{
					lScore = m_bScoreTimes[i] * wBombTime * lCellScore * ( ( bLandWin == true ) ? -1 : 1 );
					GameEnd.lGameScore[i] = lScore;
				}

				//ʤ������
				ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//����˰��
				if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						//����˰��
						GameEnd.lGameTax+= (GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L);
						lRevenue = (GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L);

						//���ֵ���
						lScore=lScore-lRevenue;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue;
					}
				}
			
				//�޸ķ���
				m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,ScoreKind);
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
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			CopyMemory(GameEnd.bBackCard,m_bBackCard,sizeof(m_bBackCard));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ը������
			//WORD wBombTime = 1;
			//wBombTime=__min(wBombTime,16);

			//��������
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_bScoreTimes[i] = __max(m_bScoreTimes[i],1);
			}

			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��Ϸ����
				LONG lScore=0;
				LONG lRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Draw;

				//�������
				if (i==wChairID)
				{
					ScoreKind=enScoreKind_Flee;
					WORD wUser1=(i+1)%m_wPlayerCount;
					WORD wUser2=(i+2)%m_wPlayerCount;
					lScore=(m_bScoreTimes[wUser1]+m_bScoreTimes[wUser2])*m_pGameServiceOption->lCellScore*(-2);
					GameEnd.lGameScore[i]=lScore;
				}
				else if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					//ͳ�ƻ���
					ScoreKind=enScoreKind_Win;
					lScore=m_bScoreTimes[i]*m_pGameServiceOption->lCellScore*2;
					GameEnd.lGameScore[i]=lScore;

					//����˰��
					if (lScore>=100L)
					{
						//����˰��
						GameEnd.lGameTax+=GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L;
						lRevenue = GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L;

						//���ֵ���
						lScore=lScore-lRevenue ;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue ;
					}
				}

				//д�����
				m_pITableFrame->WriteUserScore(i,lScore,lRevenue,ScoreKind);
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

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
	case GS_WK_SCORE:		//�з�״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//���ñ���
			StatusScore.bBackCatchTag = m_bBackCatchTag;
			StatusScore.wBankerUser = m_wBankerUser;
			StatusScore.wFirstUser = m_wFirstUser;
			StatusScore.bMingCard = m_bMingCard;
			StatusScore.wCurrentUser=m_wCurrentUser;
			StatusScore.bCurrentState = m_bCurrentState;
			StatusScore.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusScore.bCallScoreTimes,m_bCallScoreTimes,sizeof(m_bCallScoreTimes));
			CopyMemory( StatusScore.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			if(m_bBackCatchTag!=FALSE || m_bLookCardTag!=FALSE)
			{
				CopyMemory(StatusScore.bCardData,m_bHandCardData[wChiarID], MAX_COUNT*sizeof(BYTE));
				if(m_bLookCardTag==TRUE)CopyMemory(StatusScore.bBackCard,m_bBackCard, sizeof(m_bBackCard)*sizeof(BYTE));
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			StatusPlay.wFirstUser=m_wFirstUser;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wOutBombCount=m_wOutBombCount;
			StatusPlay.bMingCard=m_bMingCard;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			CopyMemory(StatusPlay.bCallScoreTimes,m_bCallScoreTimes,sizeof(m_bCallScoreTimes));
			CopyMemory(StatusPlay.bScoreTimes,m_bScoreTimes,sizeof(m_bScoreTimes));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory( StatusPlay.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			if(wChiarID==m_wBankerUser || m_bLookCardTag==TRUE)
				CopyMemory(StatusPlay.bBackCard,m_bBackCard,sizeof(m_bBackCard));

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

			//�û�Ч��
			ASSERT(m_wCurrentUser==pUserData->wChairID);
			if(m_wCurrentUser!=pUserData->wChairID)return false;

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

			CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pDataBuffer;

			m_bUserTrustee[ pUserTrustee->wUserChairID ] = pUserTrustee->bTrustee;

			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_TRUSTEE, (void*)pDataBuffer,wDataSize);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_C_TRUSTEE, (void*)pDataBuffer,wDataSize);

			return true;
		}
	}

	return true;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//������Ϣ
bool CTableFrameSink::SendUserInfoData(WORD wLandUser,BYTE bLandScore,WORD wCurrentUser,BYTE bCurrentState,WORD wBankerUser)
{
	//��ǰ״̬
	m_bCurrentState = bCurrentState;

	//��������
	CMD_S_LandScore LandScore;
	ZeroMemory(&LandScore,sizeof(LandScore));

	//ת������
	BYTE bTemp = bLandScore;
	if(bTemp==DOUBLE_SCORE)
	{
		if(m_bCallScoreTimes[wLandUser]==1)bTemp=CALL_SCORE;
		else bTemp=CALL_TWO_SCORE;
	}
	else if(bTemp==0)bTemp=PASS_ACTION;

	//���ò���
	LandScore.bLandUser=wLandUser;
	LandScore.bLandScore=bTemp;
	LandScore.wCurrentUser=wCurrentUser;
	LandScore.bCurrentState=bCurrentState;
	LandScore.wBankerUser=wBankerUser;
	LandScore.bUserCallTimes=m_bCallScoreTimes[wLandUser];

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

	return true;
}

//�����˿�
bool CTableFrameSink::SendUserCard()
{
	//��δ����
	if(m_bHaveSendCard==FALSE)
	{
		//���ñ���
		m_bHaveSendCard = TRUE;

		//�����˿�
		CMD_S_SendAllCard SendAllCard ;
		ZeroMemory(&SendAllCard,sizeof(SendAllCard));

		//�����˿�
		CopyMemory( SendAllCard.bCardData, m_bHandCardData, sizeof( m_bHandCardData ) ) ;

		//�����˿�
		ASSERT(m_bLookCardTag!=m_bBackCatchTag);
		if(m_bLookCardTag==TRUE)CopyMemory(SendAllCard.bBackCardData,m_bBackCard,3);

		for ( WORD i = 0; i < m_wPlayerCount; i++ )
		{
			IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(i);
			if ( pServerUserItem == NULL ) continue;
			if ( pServerUserItem->IsAndroidUser() ) CopyMemory(SendAllCard.bBackCardData,m_bBackCard,3);
			else if ( m_bLookCardTag!=TRUE ) ZeroMemory(SendAllCard.bBackCardData, sizeof(SendAllCard.bBackCardData));

			m_pITableFrame->SendTableData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
			m_pITableFrame->SendLookonData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
		}

		//�����˿�
		for ( WORD i = 0; i < m_wPlayerCount; i++ )
		{
			m_GameLogic.SortCardList( m_bHandCardData[ i ], m_bCardCount[ i ], ST_ORDER );
		}
	}

	return true;
}

//��ʼ����
bool CTableFrameSink::UserStartOutCard()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

	//�����˿�
	if(m_bCardCount[m_wBankerUser]<20)
	{
		//���͵���
		//ASSERT(m_bLookCardTag==FALSE);
		m_bCardCount[m_wBankerUser]=20;
		CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
		m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);
	}

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//������Ϣ
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.bLandScore=0;
	GameStart.wCurrentUser=m_wCurrentUser;

	//�û�����
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_wBankerUser==i)continue;
		WORD wScoreTimes = 0;
		wScoreTimes+=__min(m_bCallScoreTimes[m_wBankerUser],m_bCallScoreTimes[i]);
		wScoreTimes+=m_bCallScoreTimes[i];

		//�мұ���
		GameStart.wUserMultiple[i]=m_wBombTime*((WORD)pow(2,(INT)wScoreTimes));
		m_bScoreTimes[i]=(BYTE)GameStart.wUserMultiple[i];
	}

	//ׯ�ұ���
	WORD wUser1 = (m_wBankerUser+1)%GAME_PLAYER;
	WORD wUser2 = (m_wBankerUser+2)%GAME_PLAYER;
	m_bScoreTimes[m_wBankerUser] = __max(m_bScoreTimes[wUser1],m_bScoreTimes[wUser2]);
	GameStart.wUserMultiple[m_wBankerUser] = m_bScoreTimes[m_wBankerUser];

	//������Ϣ
	for (WORD i=0; i<m_wPlayerCount;i++ )
	{
		if(i==m_wBankerUser && m_bBackCatchTag==TRUE)CopyMemory(GameStart.bBackCard,m_bBackCard,sizeof(m_bBackCard));
		else ZeroMemory(GameStart.bBackCard,sizeof(GameStart.bBackCard));
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//�з��¼�
bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bLandScore)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_SCORE) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�׽�״̬
	if(m_bBackCatchTag==FALSE && m_bLookCardTag==FALSE)
	{
		//Ч�����
		ASSERT(bLandScore==BACK_CATCH || bLandScore==LOOK_CARD);
		if(bLandScore!=BACK_CATCH && bLandScore!=LOOK_CARD)return false;

		//ѡ����
		if(bLandScore==LOOK_CARD)
		{
			//���ñ���
			m_bLookCardTag=TRUE;

			//�����˿�
			SendUserCard();
		}
		//ѡ����ץ
		else
		{
			//���ñ���
			m_bBackCatchTag=TRUE;

			//ȷ��ׯ��
			m_wBankerUser = wChairID;

			//���ַ���
			m_wBombTime*=2;

			//��λ�з�
			m_wCurrentUser=(wChairID+1)%m_wPlayerCount;
		}

		//����ѯ��
		BYTE bCurrentState=((bLandScore==LOOK_CARD)?CALL_BANKER:CALL_SCORE);
		WORD wBankerUser=((bLandScore==LOOK_CARD)?INVALID_CHAIR:m_wBankerUser);
		SendUserInfoData(wChairID,bLandScore,m_wCurrentUser,bCurrentState,wBankerUser);
	}
	//��ׯ״̬
	else if(m_wBankerUser==INVALID_CHAIR)
	{
		//Ч�����
		ASSERT(bLandScore==MING_CATCH || bLandScore==NULL);
		if(bLandScore!=MING_CATCH && bLandScore!=NULL)return false;

		//�û�״̬
		ASSERT(m_bCallBanker[wChairID] == FALSE);
		if(m_bCallBanker[wChairID] != FALSE)return false;

		//���ñ���
		if(bLandScore==MING_CATCH)m_bCallBanker[wChairID] = 2*TRUE;
		else m_bCallBanker[wChairID] = TRUE;

		//�жϴ���
		bool bFullTimes = false;
		for(WORD i=0;i<m_wPlayerCount;i++)if(m_bCallBanker[i] == FALSE)break;
		if(i==m_wPlayerCount)bFullTimes=true;

		//�жϿ�ʼ
		if(bFullTimes || bLandScore==MING_CATCH)
		{
			//ȷ��ׯ��
			if(bLandScore==MING_CATCH)
			{
				m_wBankerUser = wChairID;

				//��ʼ����
				if(bFullTimes)
				{
					SendUserInfoData(wChairID,bLandScore,INVALID_CHAIR,0,m_wBankerUser);
					UserStartOutCard();
				}
				//ѯ�ʽз�
				else 
				{
					//�������
					m_bCardCount[m_wBankerUser]=20;
					CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
					m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

					//��ʼ�з�+�ͻ��˲������
					m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
					BYTE bCurrentState = 0x10+CALL_SCORE;
					SendUserInfoData(wChairID,bLandScore,m_wCurrentUser,bCurrentState,m_wBankerUser);
				}
			}
			//��Ϸ����
			else OnEventGameEnd(INVALID_CHAIR,NULL,GER_DISMISS);

			return true;
		}

		//ѯ����λ
		m_wCurrentUser = (m_wCurrentUser+1)%m_wPlayerCount;
		ASSERT(m_bCallBanker[m_wCurrentUser] != TRUE);

		//����ѯ��
		SendUserInfoData(wChairID,bLandScore,m_wCurrentUser,CALL_BANKER,INVALID_CHAIR);
	}
	//�з�״̬
	else 
	{
		//Ч�����
		ASSERT(m_bCallScoreTimes[wChairID]<MAX_CALL_TIMES);
		ASSERT(bLandScore==DOUBLE_SCORE || bLandScore==0);
		if(m_bCallScoreTimes[wChairID]>=MAX_CALL_TIMES)return false;
		if(bLandScore!=DOUBLE_SCORE && bLandScore!=0)return false;

		//���ñ���
		if(bLandScore==0)m_bCallScoreTag[wChairID] = TRUE;
		else m_bCallScoreTimes[wChairID]++;
		if(m_bCallScoreTimes[wChairID]==MAX_CALL_TIMES)m_bCallScoreTag[wChairID] = TRUE;

		//�жϿ�ʼ
		bool bStartOut=(m_bCallScoreTag[m_wBankerUser]==TRUE)?true:false;
		if(!bStartOut && bLandScore==0 && m_wBankerUser!=wChairID)
		{
			WORD wUser1 = m_wPlayerCount-m_wBankerUser-wChairID;
			if(m_bCallScoreTag[wUser1]==TRUE && m_bCallScoreTimes[wUser1]<MAX_CALL_TIMES)
				bStartOut=true;
			WORD wUser2 = (m_wBankerUser+1)%m_wPlayerCount;
			if(!bStartOut && m_bLookCardTag==TRUE && wUser2==wChairID && m_bCallScoreTimes[wUser2]==0 && m_bCallBanker[wUser1]==TRUE)
				bStartOut=true;
		}

		//��ʼ����
		if(bStartOut)
		{
			if(bLandScore==DOUBLE_SCORE)
			{
				SendUserInfoData(wChairID,bLandScore,INVALID_CHAIR,0,INVALID_CHAIR);
			}

			//�����˿�
			SendUserCard();

			//��ʼ����
			UserStartOutCard();

			return true;
		}

		//��λ���
		WORD wNextUser = INVALID_CHAIR;
		for(WORD i=1;i<m_wPlayerCount;i++)
		{
			wNextUser = (wChairID+i)%m_wPlayerCount;
			if(m_bCallScoreTag[wNextUser]!=TRUE)break;
		}

		//��ǰ�û�
		ASSERT(wNextUser<m_wPlayerCount && m_bCallScoreTimes[wNextUser]<MAX_CALL_TIMES);
		m_wCurrentUser = wNextUser;

		//����ѯ��
		BYTE bCallScoreState=((m_bCallScoreTimes[m_wCurrentUser]==0)?CALL_SCORE:CALL_TWO_SCORE);
		SendUserInfoData(wChairID,bLandScore,m_wCurrentUser,bCallScoreState,INVALID_CHAIR);
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//ը���ж�
	if ((bCardType==CT_BOMB_CARD)||(bCardType==CT_MISSILE_CARD)) 
	{
		for(WORD i=0;i<m_wPlayerCount;i++)m_bScoreTimes[i]*=2;
		m_wOutBombCount++;
	}
	
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

//////////////////////////////////////////////////////////////////////////
