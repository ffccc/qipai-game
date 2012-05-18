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
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wChaUser=INVALID_CHAIR;
	m_bgou = false;
	m_bdian = false;
	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	////////////////////////////////////////��ʼ����ӵı���////////////////////////////////////////
	memset(m_bLiOrJue, 0, sizeof(m_bLiOrJue));
	m_cbLiSuperCount = 0;
	memset(m_bNotLiOrJue, 0, sizeof(m_bNotLiOrJue));
	m_cbNotJue = 0;
	m_cbMingCi = 0;
	memset(m_cbMiCi, 0, sizeof(m_cbMiCi));
	m_cbEndType = 0;
	memset(m_cbCiXu, -1, sizeof(m_cbCiXu));
	m_wLiGun = 256;
	m_wJueGun = 256;

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
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wChaUser=INVALID_CHAIR;
	m_bgou = false;
	m_bdian = false;
	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//////////////////////////////��ӱ���������/////////////////////////////
	memset(m_bLiOrJue, 0, sizeof(m_bLiOrJue));
	m_cbLiSuperCount = 0;
	memset(m_bNotLiOrJue, 0, sizeof(m_bNotLiOrJue));
	m_cbNotJue = 0;
	m_cbMingCi = 0;
	memset(m_cbMiCi, 0, sizeof(m_cbMiCi));
	m_cbEndType = 0;
	memset(m_cbCiXu, -1, sizeof(m_cbCiXu));
	m_wLiGun = 256;
	m_wJueGun = 256;

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
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

	BYTE cbSendGoodCard = rand () % GOOD_CARD_PERCENT ;
	BYTE cbAndroidUserCount = 0 ;
	WORD wAndroidUser[ GAME_PLAYER ] = {0};

	WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	{
		//�����˿�
		BYTE bRandCard[ 54 ];
		m_GameLogic.RandCardList( bRandCard,sizeof( BYTE ) * 54 );

		int num = 0;
		//�ַ��˿�
		for ( WORD i = 0; i < GAME_PLAYER; i++ )
		{	
			m_bCardCount[ i ] = 13;		
			CopyMemory( &m_bHandCardData[ i ], &bRandCard[ i * m_bCardCount[ i ] ], sizeof( BYTE ) * m_bCardCount[ i ] );
		}

		CopyMemory( &m_bHandCardData[0][13], &bRandCard[52], sizeof( BYTE ) );m_bCardCount[ 0 ]++;
		CopyMemory( &m_bHandCardData[1][13], &bRandCard[53], sizeof( BYTE ) );m_bCardCount[ 1 ]++;

	}

	///////////////////////////////////////////////////////////
	int index = -1;
	for (int i=0; i<GAME_PLAYER; ++i)
	{
		index = m_GameLogic.FindCard(m_bHandCardData[i], m_bCardCount[i], 0x23);
		if (-1 != index)
		{
			index = i;
			break;
		}
	}
	///////////////////////////////////////////////////////////

	//�����û�
	m_wCurrentUser = index;//��ǰ���=//�׽��û�

	//�����˿�
	CMD_S_SendAllCard SendAllCard ;
	SendAllCard.wCurrentUser = m_wCurrentUser ;

	//�����˿�
	CopyMemory( SendAllCard.bCardData, m_bHandCardData, sizeof( m_bHandCardData ) ) ;
	//�˿˵���Ŀ
	CopyMemory(SendAllCard.cbCardNum,m_bCardCount, sizeof( m_bCardCount ));

	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_pITableFrame->SendTableData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
		m_pITableFrame->SendLookonData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
	}

	//�����˿�
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_GameLogic.SortCardList( m_bHandCardData[ i ], m_bCardCount[ i ], ST_ORDER );//�����˿�
	}

	////������Ϣ
	//CMD_S_GameStart GameStart;
	//GameStart.wCurrentUser=index;

	//m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	//m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

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
			ZeroMemory( &GameEnd, sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				GameEnd.bCardCount[ i ] = m_bCardCount[ i ];//�˿���Ŀ
				CopyMemory( &GameEnd.bCardData[ bCardPos ], m_bHandCardData[ i ], m_bCardCount[ i ] * sizeof( BYTE ) );
				bCardPos += m_bCardCount[ i ];
			}

			//��������
			LONG lCellScore = m_pGameServiceOption->lCellScore;

			WORD Winer = 0;
			for(WORD i = 0; i < m_wPlayerCount; i++ )
			{
				bool a44  = false;
				bool king = false;

				a44  = m_GameLogic.A44(m_bHandCardData[i],m_bCardCount[i]);
				king = m_GameLogic.DoubleKong(m_bHandCardData[i],m_bCardCount[i]);

				if(i == wChairID) continue;

				if((m_bOutCardCount[i] == 0) || a44 || king)
					m_bCardCount[i]*=2;

				//GameEnd.lGameScore[ i ]=-m_bCardCount[i];
				Winer+=m_bCardCount[i];
			}
				
			//GameEnd.lGameScore[wChairID] = Winer;
			CountScore(GameEnd.lGameScore);

			//ͳ�ƻ���
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				LONG lScore=0;
				LONG lRevenue=0;
				enScoreKind ScoreKind;


				//ʤ������
				ScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				//����˰��
				if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						//����˰��
						GameEnd.lGameTax+=(GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L);
						lRevenue = (GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L);

						//���ֵ���
						lScore=lScore-lRevenue ;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue ;
					}
				}
				//�޸ķ���
				m_pITableFrame->WriteUserScore(i,lScore,lRevenue,ScoreKind);

			}



			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

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

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��������
				LONG lScore=0;
				LONG lRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Draw;

				//�������
				if (i==wChairID)
				{
					ScoreKind=enScoreKind_Flee;
					lScore	 =-m_pGameServiceOption->lCellScore*54;
					GameEnd.lGameScore[i]=-/*m_pGameServiceOption->lCellScore*54*/80;
				}
				else if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					//ͳ�ƻ���
					ScoreKind=enScoreKind_Win;
					lScore=(m_pGameServiceOption->lCellScore*54)/3;
					GameEnd.lGameScore[i]=(m_pGameServiceOption->lCellScore*54)/3;

					//����˰��
					if (lScore>=100L)
					{
						//����˰��
						GameEnd.lGameTax+=(GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L);
						lRevenue = (GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenue/100L);

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
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
//			StatusPlay.wLandUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
//			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData,sizeof(BYTE)*sizeof(m_bHandCardData));
			CopyMemory( StatusPlay.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			StatusPlay.gou = m_bgou;
			if(m_bgou) StatusPlay.wCurrentUser=m_wChaUser;
			StatusPlay.dian= m_bdian;
			if(m_bdian) StatusPlay.wCurrentUser=m_wChaUser;
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
	/////////////////////////////////////////��ӵ���Ϣ����///////////////////////////////////////
		//��Ҳ����
	case SUB_C_NOT_JUE_GUN:
		{
			CMD_C_NotLiOrJue *pNotJue;
			pNotJue = (CMD_C_NotLiOrJue *)pDataBuffer;
			m_bNotLiOrJue[pNotJue->wNotLiOrJueChair] = true;

			m_cbNotJue++;
			if (m_cbNotJue == 2)
			{
				CMD_S_GameStart GameStart; 
				//for (BYTE i=0; i<GAME_PLAYER; i++)
				//{
				//	if (m_bLiOrJue[i] == false)
				//	{
				//		m_wCurrentUser = i;
				//		GameStart.wCurrentUser = m_wCurrentUser;
				//		break;
				//	}
				//}
				m_wCurrentUser = m_wLiGun;
				GameStart.wCurrentUser = m_wLiGun;

				for (BYTE j=0; j<GAME_PLAYER; j++)
					m_pITableFrame->SendTableData(j, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));


			}
			return true;
		}
		//������ʱ��ȫ��������
	case SUB_C_LI_GUN_SUPERTIME:
		{
			m_cbLiSuperCount++;
			if (m_cbLiSuperCount == 4)
			{
				//������Ϣ
				CMD_S_GameStart GameStart;
				GameStart.wCurrentUser=m_wCurrentUser;

				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			}

			return true;
		}
		//��ҷ������������ҵ����ĶԼң����������������Ϣ�����Լ��Ƿ�ͬ��
	case SUB_C_LI_GUN:
		{
			CMD_C_LiGun *pLiGun;
			pLiGun = (CMD_C_LiGun *)pDataBuffer;
			
			WORD wLiGunChair = pLiGun->wLiGunUserChairID;

			//�õ����������Լҵ���λ��
			WORD wAnswerChair = ( wLiGunChair + 2) % GAME_PLAYER;
			CMD_S_LiGunRequest LiGunRequest;
			ZeroMemory(&LiGunRequest, sizeof(LiGunRequest));
			LiGunRequest.wRequest = wAnswerChair;

			//������Ϣ
			for (BYTE i=0; i<GAME_PLAYER; i++)
				m_pITableFrame->SendTableData(i, SUB_S_LI_GUN_REQUEST, &LiGunRequest, sizeof(LiGunRequest));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_LI_GUN_REQUEST, &LiGunRequest, sizeof(LiGunRequest));

			return true;

		}
	//�����Ļش�
	case SUB_C_LI_GUN_ANSWER:
		{
			CMD_C_LiGunAnswer *pLiGunAnswer;
			pLiGunAnswer = (CMD_C_LiGunAnswer *)pDataBuffer;
			WORD wChairID = ((pLiGunAnswer->wAgreeChairID)+2)%GAME_PLAYER;
			BYTE cbLiOrJue = pLiGunAnswer->cbLiOrJue;

			//������ͬ�����������߾��
			if (pLiGunAnswer->cbAnswer == 1)
			{
				m_bLiOrJue[pLiGunAnswer->wAgreeChairID] = true;
				CMD_S_CanJueGun CanJueGun;
				ZeroMemory(&CanJueGun, sizeof(CanJueGun));
				BYTE temp = 0;
				//���ͬ����������ʱ����Է�������ҷ��;����Ϣ���������ǵľ����ť
				if (cbLiOrJue == 1)
				{
					m_wLiGun = wChairID;
					//�õ������ҵ�λ��
					for (WORD i=0; i<GAME_PLAYER; i++)
					{
						if (i != wChairID && i != pLiGunAnswer->wAgreeChairID)
						{
							CanJueGun.wCanJueChair[temp] = i;
							temp++;
						}
					}
					for (WORD j=0; j<GAME_PLAYER; j++)
						m_pITableFrame->SendTableData(j, SUB_S_JUE_GUN, &CanJueGun, sizeof(CanJueGun));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_JUE_GUN, &CanJueGun, sizeof(CanJueGun));

				}
				//���ͬ���˾����������Ϸ��ʼ��Ϣ
				if (cbLiOrJue == 2)
				{
					m_wJueGun = wChairID;
					CMD_S_GameStart GameStart;
					ZeroMemory(&GameStart, sizeof(GameStart));
					//for (WORD i=0; i<GAME_PLAYER; i++)
					//{
					//	if (m_bLiOrJue[i] == false)
					//	{
					//		GameStart.wCurrentUser = i;
					//		m_wCurrentUser = i;
					//		break;
					//	}
					//}
					m_wCurrentUser = m_wJueGun;
					GameStart.wCurrentUser = m_wJueGun;
					//CString s;
					//s.Format("%d, %d, %d, %d, %d", m_wCurrentUser, m_bLiOrJue[0], m_bLiOrJue[1], m_bLiOrJue[2], m_bLiOrJue[3]);
					//AfxMessageBox(s);

					for (BYTE j=0; j<GAME_PLAYER; j++)
						m_pITableFrame->SendTableData(j, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));

				}
			}
			//��Ҳ�ͬ����������ô��������ȫ����������������������������ǲ�������
			else
			{
				//��ͬ��������ʱ��
				if (cbLiOrJue == 1)
				{
					//�����Ҳ�ͬ��Է���������ô����������������
					m_bLiOrJue[wChairID] = false;
					m_bLiOrJue[pLiGunAnswer->wAgreeChairID] = false;
					m_bNotLiOrJue[wChairID] = true;
					m_bNotLiOrJue[pLiGunAnswer->wAgreeChairID] = true;

					CMD_S_CanJueGun CanJueGun;
					ZeroMemory(&CanJueGun, sizeof(CanJueGun));
					BYTE temp = 0;

					//�õ�����������ҵ�λ��
					for (WORD i=0; i<GAME_PLAYER; i++)
					{
						if (i != wChairID && i != pLiGunAnswer->wAgreeChairID)
						{
							CanJueGun.wCanJueChair[temp] = i;
							temp++;
						}
					}

					//��������������û���������������߲��������ô�����Ƿ����ܹ���������Ϣ
					if (m_bNotLiOrJue[CanJueGun.wCanJueChair[0]] != true && m_bNotLiOrJue[CanJueGun.wCanJueChair[1]] != true)
					{
						for (WORD j=0; j<GAME_PLAYER; j++)
							m_pITableFrame->SendTableData(j, SUB_S_LI_GUN, &CanJueGun, sizeof(CanJueGun));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_LI_GUN, &CanJueGun, sizeof(CanJueGun));
					}
					//����Ļ���ֱ�ӿ�ʼ��Ϸ
					else
					{
						CMD_S_GameStart GameStart;
						ZeroMemory(&GameStart, sizeof(GameStart));
						for (BYTE i=0; i<GAME_PLAYER; i++)
						{
							if (m_bLiOrJue[i] == false)
							{
								GameStart.wCurrentUser = i;
								m_wCurrentUser = i;
								break;
							}
						}

						for (BYTE j=0; j<GAME_PLAYER; j++)
							m_pITableFrame->SendTableData(j, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
					}
				}

				//��ͬ����,��ʼ��Ϸ
				if (cbLiOrJue == 2)
				{
					CMD_S_GameStart GameStart;
					ZeroMemory(&GameStart, sizeof(GameStart));
					for (BYTE i=0; i<GAME_PLAYER; i++)
					{
						if (m_bLiOrJue[i] == false)
						{
							GameStart.wCurrentUser = i;
							m_wCurrentUser = i;
							break;
						}
					}

					for (BYTE j=0; j<GAME_PLAYER; j++)
						m_pITableFrame->SendTableData(j, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
				}
			}

			return true;
		}
	//������
	case SUB_C_NOT_LI_GUN:
		{
			CMD_C_NotLiOrJue *pNotLiOrJue = (CMD_C_NotLiOrJue *)pDataBuffer;

			m_bNotLiOrJue[pNotLiOrJue->wNotLiOrJueChair] = true;

			BYTE cbCount = 0;
			for (BYTE i=0; i<GAME_PLAYER; i++)
			{
				if (m_bNotLiOrJue[i])
					cbCount++;
			}

			if (cbCount == GAME_PLAYER)
			{
				CMD_S_GameStart GameStart;
				ZeroMemory(&GameStart, sizeof(GameStart));
				GameStart.wCurrentUser = m_wCurrentUser;

				for (WORD j=0; j<GAME_PLAYER; j++)
					m_pITableFrame->SendTableData(j, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &GameStart, sizeof(GameStart));
			}

			return true;

		}
	case SUB_C_JUE_GUN:
		{
			CMD_C_LiGun *pJueGun = (CMD_C_LiGun *)pDataBuffer;
			//m_bLiOrJue[pJueGun->wLiGunUserChairID] = true;

			WORD wLiGunChair = pJueGun->wLiGunUserChairID;

			//�õ����������Լҵ���λ��
			WORD wAnswerChair = ( wLiGunChair + 2) % GAME_PLAYER;
			CMD_S_LiGunRequest LiGunRequest;
			ZeroMemory(&LiGunRequest, sizeof(LiGunRequest));
			LiGunRequest.wRequest = wAnswerChair;

			//������Ϣ
			for (BYTE i=0; i<GAME_PLAYER; i++)
				m_pITableFrame->SendTableData(i, SUB_S_JUE_GUN_REQUEST, &LiGunRequest, sizeof(LiGunRequest));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_JUE_GUN_REQUEST, &LiGunRequest, sizeof(LiGunRequest));

			return true;

		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

			//AfxMessageBox("p");
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:		//��������
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
	case SUB_C_GAME_CHA:		//����
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


			return OnUserChaCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_GAME_NOCHA:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			return OnUserPassChaCard(pUserData->wChairID);
		}
	case SUB_C_GAME_DIAN:		//����
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
			/////////////////////////////////////////////////////

			/////////////////////////////////////////////////////
			return OnUserDianCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount);;
		}
	case SUB_C_GAME_NODIAN:		//��������
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;
			/////////////////////////////////////////////////////

			////////////////////////////////////////////////////
			//��Ϣ����
			return OnUserPassDianCard(pUserData->wChairID);
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
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;//��Ϸ����
	if (wChairID!=m_wCurrentUser) return false;//��ǰ���

	//�����˿�
	m_GameLogic.SortCardList( bCardData, bCardCount, ST_ORDER );//��С����

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);//��ȡ����
	if (bCardType==CT_ERROR) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;//������Ŀ
	else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) return false;//�Ա��˿�

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;//ʣ������

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);//�����б�
/////////////////һ�·�Ϊ����,��������͵�����

	/////////////////////////////////////ȷ���������///////////////////////////////
	if (m_bCardCount[wChairID] == 0)
	{
		m_cbMiCi[m_cbMingCi] = wChairID;
		m_cbMingCi++;
		m_cbCiXu[wChairID] = m_cbMingCi;
	}
	////////////////////////////////////////////////////////////////////////////////
	//�л��û�
	m_wTurnWiner=wChairID;//ʤ�����
	if (m_bCardCount[wChairID]!=0)//�˿���Ŀ
	{
		if (bCardType!=CT_A44_red || bCardType!=CT_A44_red_black)
		{
				m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}
			//////////////////////////////////////////////////////////////////////
		/////////////////////////////////������Ƶ��û�	
		int index = 0;
		if(bCardType == CT_SINGLE)
		{
			for(int i =0; i<GAME_PLAYER; i++)
			{
				if(wChairID == i) continue;
				//////////////////////////////////��ӵĴ���////////////////////////////////
				if (m_bLiOrJue[i]==true) continue;
				//AfxMessageBox("OutCard--1");
				////////////////////////////////////////////////////////////////////////////
				index = m_GameLogic.FindCardNum(m_bHandCardData[i], m_bCardCount[i], bCardData[0]);
				if(index >= 2 && m_bLiOrJue[i]==false)
				{
					m_wChaUser = i;
					m_bgou = true;
					//��������
					CMD_S_OutCard OutCard;//�û�����
					OutCard.bCardCount=bCardCount;
					OutCard.wOutCardUser=wChairID;
					OutCard.wCurrentUser=m_wChaUser;
					CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

					//��������
					WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_CHA,&OutCard,wSendSize);
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_CHA,&OutCard,wSendSize);

					return true;

				}

			}

		}

	}
	//else 
	//	m_wCurrentUser=INVALID_CHAIR;	//��ǰ���=��Ч���Ӻ�
/////////////////////////////////////////////////

	//AfxMessageBox("OutCard--2");
	if (m_bCardCount[m_wCurrentUser]!=0 && m_bLiOrJue[m_wCurrentUser] == false)
	{
		m_wCurrentUser = m_wCurrentUser;
	}
	else
	{
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			m_wCurrentUser = (m_wCurrentUser + 1)%GAME_PLAYER;

			if (m_bLiOrJue[m_wCurrentUser]!=true && m_bCardCount[m_wCurrentUser]!=0)
				break;
		}
	}

	//CString w;
	//w.Format("%d,%d", m_wCurrentUser, m_bLiOrJue[m_wCurrentUser]);
	//AfxMessageBox(w);
	//��������
	CMD_S_OutCard OutCard;//�û�����
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�������
	if (bCardType==CT_A44_red) m_bTurnCardCount=0;//������Ŀ

	//�����ж�
	//if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);
	if ( IsGameOver() != 0) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û���������
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;//��Ϸ����
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;
/////////////////һ�·�Ϊ����,��������͵�����
	//////���ñ���
	////for (BYTE i=0; i<GAME_PLAYER; i++)
	////{
	////	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;//�õ��¸����
	////	if (m_bCardCount[m_wCurrentUser] != 0 && m_bLiOrJue[m_wCurrentUser] == false)
	////		break;
	////	//if (m_bLiOrJue[m_wCurrentUser]==false) break;
	////}
	////if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;//��ǰ���==//ʤ�����
	//�õ���һ�����
	BYTE wTemp;
	wTemp = (m_wCurrentUser+1)%GAME_PLAYER;
	if (wTemp==m_wTurnWiner) m_bTurnCardCount=0;//��ǰ���==//ʤ�����

	if (wTemp == m_wTurnWiner && m_bCardCount[wTemp]==0)
	{
		m_wCurrentUser = (wTemp + 2) % GAME_PLAYER;
		m_bTurnCardCount = 0;
	}
	else
	{
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;//�õ��¸����
			if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;//��ǰ���==//ʤ�����
			if (m_bCardCount[m_wCurrentUser] != 0 && m_bLiOrJue[m_wCurrentUser] == false)
			break;
		}
		
	}
/////////////////////////////////////////////////
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
bool CTableFrameSink::OnUserChaCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;//��Ϸ����
	if (wChairID!=m_wChaUser) return false;//��ǰ���

	//�����˿�
	m_GameLogic.SortCardList( bCardData, bCardCount, ST_ORDER );//��С����

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);//��ȡ����
	if (bCardType==CT_ERROR) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;//������Ŀ
	//else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) return false;//�Ա��˿�

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;//ʣ������

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;//������Ŀ
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);//�����б�
/////////////////һ�·�Ϊ����,��������͵�����
	//�л��û�
	m_wTurnWiner=wChairID;//ʤ�����
	m_bgou = false;
	if (m_bCardCount[wChairID]!=0)//�˿���Ŀ
	{
		//if (bCardType!=CT_A44_red || bCardType!=CT_A44_red_black)
		//	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		/////////////////////////////////������Ƶ��û�	
		int index = -1;
		if(bCardType == CT_DOUBLE)
		{
			for(int i =0; i<GAME_PLAYER; i++)
			{
				if(wChairID == i) continue;//�Լ�����
				if (m_bLiOrJue[i]) continue;

				index = m_GameLogic.FindCardNum(m_bHandCardData[i], m_bCardCount[i], bCardData[0]);
				/////////////////////////////////����������У������һ������//////////////////////////
				if(index >= 1)
				{
					m_wChaUser = i;
					m_bdian = true;
					//��������
					CMD_S_OutCard OutCard;//�û�����
					OutCard.bCardCount=bCardCount;
					OutCard.wOutCardUser=wChairID;
					OutCard.wCurrentUser=m_wChaUser;
					CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

					//��������
					WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_DIAN,&OutCard,wSendSize);
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_DIAN,&OutCard,wSendSize);

					return true;

				}

			}

		}
		if(index == 0)
		{
			//��������
			CMD_S_OutCard OutCard;//�û�����
			OutCard.bCardCount=bCardCount;
			OutCard.wOutCardUser=m_wChaUser;
			OutCard.wCurrentUser=m_wChaUser;
			CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

			//��������
			WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_DIAN,&OutCard,wSendSize);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_DIAN,&OutCard,wSendSize);

			m_wCurrentUser = m_wChaUser;//��ǰ���==//ʤ�����
			 m_bTurnCardCount=0;
			/////////////////////////////////////////////////
			//��������
			CMD_S_PassCard PassCard;
			PassCard.wPassUser=m_wTurnWiner;
			PassCard.wCurrentUser=m_wTurnWiner;
			PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));


			return true;
		
		}
	}
	else 
		m_wCurrentUser=INVALID_CHAIR;	//��ǰ���=��Ч���Ӻ�
/////////////////////////////////////////////////
	//��������
	CMD_S_OutCard OutCard;//�û�����
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�������
	if (bCardType==CT_A44_red) m_bTurnCardCount=0;//������Ŀ

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û���������
bool CTableFrameSink::OnUserPassChaCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;//��Ϸ����
	if ((wChairID!=m_wChaUser)||(m_bTurnCardCount==0)) return false;
/////////////////һ�·�Ϊ����,��������͵�����
	//���ñ���
	//m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;//�õ��¸����
	if (m_bCardCount[m_wCurrentUser] != 0 && m_bLiOrJue[m_wCurrentUser] == false)
	{
		m_wCurrentUser = m_wCurrentUser;
	}
	else 
	{
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			m_wCurrentUser = (m_wCurrentUser + 1)%GAME_PLAYER;
			if (m_bCardCount[m_wCurrentUser] != 0 && m_bLiOrJue[m_wCurrentUser] == false)
				break;
		}
	}
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;
/////////////////////////////////////////////////
	m_bgou = false;
	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_NOCHA,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_NOCHA,&PassCard,sizeof(PassCard));

	return true;
}

//�û�����
bool CTableFrameSink::OnUserDianCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;//��Ϸ����
	if (wChairID!=m_wChaUser) return false;//��ǰ���

	//�����˿�
	m_GameLogic.SortCardList( bCardData, bCardCount, ST_ORDER );//��С����

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);//��ȡ����
	if (bCardType==CT_ERROR) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;//������Ŀ
	//else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) return false;//�Ա��˿�

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;//ʣ������
	m_bdian = false;
	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;//������Ŀ
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);//�����б�

	//�л��û�
	m_wTurnWiner=wChairID;//ʤ�����
	if (m_bCardCount[wChairID]!=0)//�˿���Ŀ
	{
			CMD_S_OutCard OutCard;//�û�����
			OutCard.bCardCount=bCardCount;
			OutCard.wOutCardUser=m_wTurnWiner;
			OutCard.wCurrentUser=m_wTurnWiner;
			CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

			//��������
			WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_DIAN,&OutCard,wSendSize);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_DIAN,&OutCard,wSendSize);


			 m_bTurnCardCount=0;//��ǰ���==//ʤ�����
			/////////////////////////////////////////////////
			//��������
			CMD_S_PassCard PassCard;
			PassCard.wPassUser=m_wTurnWiner;
			PassCard.wCurrentUser=m_wTurnWiner;
			PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

            m_wCurrentUser = m_wTurnWiner;

			return true;
		
		//}
	}
	else 
		m_wCurrentUser=INVALID_CHAIR;	//��ǰ���=��Ч���Ӻ�
/////////////////////////////////////////////////
	//��������
	CMD_S_OutCard OutCard;//�û�����
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�������
	if (bCardType==CT_A44_red) m_bTurnCardCount=0;//������Ŀ

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameEnd(wChairID,NULL,GER_NORMAL);

	return true;
}
//�û���������
bool CTableFrameSink::OnUserPassDianCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;//��Ϸ����
	if ((wChairID!=m_wChaUser)||(m_bTurnCardCount==0)) return false;

	m_wCurrentUser = m_wTurnWiner;//��ǰ���==//ʤ�����
	m_bTurnCardCount=0;

	//////////////////////////////////��ӵļ�����һ��������ҵĴ���/////////////////////////////////////
	if (m_bCardCount[m_wCurrentUser] != 0 && m_bLiOrJue[m_wCurrentUser] == false)
	{
		m_wCurrentUser = m_wCurrentUser;
	}
	else 
	{
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			m_wCurrentUser = (m_wCurrentUser + 1)%GAME_PLAYER;
			if (m_bCardCount[m_wCurrentUser] != 0 && m_bLiOrJue[m_wCurrentUser] == false)
				break;
		}
	}

	/////////////////////////////////////////////////
	//��������
	m_bdian = false;
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=m_wTurnWiner;
	PassCard.wCurrentUser=m_wTurnWiner;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

/////////////////////////////////////////////��ӵĽ�������//////////////////////////////
BYTE CTableFrameSink::IsGameOver()
{
	//////////////���ȷ���û����������������////////////////////////
	//��ǰ����Ϊͬһ����ʱ����
	if ( (m_cbCiXu[0]+m_cbCiXu[2]==3) || (m_cbCiXu[1]+m_cbCiXu[3]==3) )
		m_cbEndType = 1;
	
	//��һ����һ��������ʱ����
	if ( (m_cbCiXu[0]+m_cbCiXu[2]==4) || (m_cbCiXu[1]+m_cbCiXu[3]==4) )
		m_cbEndType = 2;

	//��һ���ڶ���������ʱ����
	if ( (m_cbCiXu[0]+m_cbCiXu[2]==5) || (m_cbCiXu[1]+m_cbCiXu[3]==5) )
		m_cbEndType = 3;

	/////////////������������������ʱ///////////////////////////////
	WORD wNotLiGun = 10;
	WORD wNotJueGun = 10;
	WORD wLiGun = 10;
	WORD wJueGun = 10;

	for (BYTE i=0; i<GAME_PLAYER; i++)
	{
		if (m_bLiOrJue[i]==true && (i==0 || i==2))
			wNotLiGun = i;
		if (m_bLiOrJue[i]==true && (i==1 || i==3))
			wNotJueGun = i;
	}

	if (wNotLiGun != 10) wLiGun = (wNotLiGun + 2)%GAME_PLAYER;
	if (wNotJueGun != 10) wJueGun = (wNotJueGun + 2)%GAME_PLAYER;

	//////////////////�������������о��ʱ
	if (wLiGun != 10 && wJueGun != 10)
	{
		if (m_cbCiXu[0] == wLiGun || m_cbCiXu[0] == wJueGun) m_cbEndType = 4;
	}

	/////////////////�����������һ����
	if (wLiGun != 10 || wJueGun != 10)
	{
		//������Ӯ��
		if (m_cbCiXu[0] == wLiGun || m_cbCiXu[0] == wJueGun) m_cbEndType = 5;
		//����������
		else
		{
			if (wLiGun != 0 && m_cbCiXu[0] != wLiGun && m_cbCiXu[0] != 255)
				m_cbEndType = 6;
			if (wJueGun != 0 && m_cbCiXu[0] != wJueGun && m_cbCiXu[0] != 255)
				m_cbEndType = 6;
		}
	}

	return m_cbEndType;
}


//��ֺ���
void CTableFrameSink::CountScore(LONG lScore[])
{
	switch (m_cbEndType)
	{
	case 1:
		{
			if (m_cbMiCi[0] == 0 || m_cbMiCi[0] == 2)
			{
				lScore[0] = 8;
				lScore[1] = -8;
				lScore[2] = 8;
				lScore[3] = -8;
			}
			else
			{
				lScore[0]=-8;
				lScore[1]=8;
				lScore[2]=-8;
				lScore[3]=8;
			}
			break;
		}
	case 2:
		{
			if (m_cbMiCi[0] == 0 || m_cbMiCi[0] == 2)
			{
				lScore[0]=4;
				lScore[1]=-4;
				lScore[2]=4;
				lScore[3]=-4;
			}
			else
			{
				lScore[0]=-4;
				lScore[1]=4;
				lScore[2]=-4;
				lScore[3]=4;
			}
			break;
		}
	case 3:
		{
			lScore[0]=0;
			lScore[1]=0;
			lScore[2]=0;
			lScore[3]=0;

			break;
		}
	case 4:
		{
			if (m_cbMiCi[0] == 0 || m_cbMiCi[0] == 2)
			{
				lScore[0]=16;
				lScore[1]=-16;
				lScore[2]=16;
				lScore[3]=-16;
			}
			else
			{
				lScore[0]=-16;
				lScore[1]=16;
				lScore[2]=-16;
				lScore[3]=16;
			}
			break;
		}
	case 5:
		{
			if (m_cbMiCi[0] == 0 || m_cbMiCi[0] == 2)
			{
				lScore[0]=16;
				lScore[1]=-16;
				lScore[2]=16;
				lScore[3]=-16;
			}
			else
			{
				lScore[0]=-16;
				lScore[1]=16;
				lScore[2]=-16;
				lScore[3]=16;
			}
			break;
		}
	case 6:
		{
			if (m_cbMiCi[0] == 0 || m_cbMiCi[0] == 2)
			{
				lScore[0]=16;
				lScore[1]=-16;
				lScore[2]=16;
				lScore[3]=-16;
			}
			else
			{
				lScore[0]=-16;
				lScore[1]=16;
				lScore[2]=-16;
				lScore[3]=16;
			}
			break;
		}
	}
}