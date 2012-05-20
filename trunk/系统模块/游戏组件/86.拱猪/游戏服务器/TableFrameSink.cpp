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
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//��Ϸ����
    m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR; 
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	m_bShowCardTime = 0;
	ZeroMemory(m_bScoreCard , sizeof(m_bScoreCard)) ;
	ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;
	m_bTotalShowCardCount = 0 ;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;

	//������Ϣ
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;
	m_wGetGpadeUser = INVALID_CHAIR;

	//�˿���Ϣ
	memset(m_bCardCount , 0 , GAME_PLAYER);	
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
    
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
	m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR;  
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	m_bShowCardTime = 0;
	ZeroMemory(m_bScoreCard , sizeof(m_bScoreCard)) ;
	ZeroMemory(m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;
	m_bTotalShowCardCount = 0 ;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;

	//������Ϣ
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;

	//�˿���Ϣ
	memset(m_bCardCount , 0 , GAME_PLAYER);	
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;


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
	m_pITableFrame->SetGameStatus(GS_WK_SHOWCARD);

	//��Ϸ����
	m_wCurrentUser  = INVALID_CHAIR ;						 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_wShowCardUser[i] = INVALID_CHAIR;  
	ZeroMemory(m_bShowCard , sizeof(m_bShowCard)) ;
	m_bShowCardUserCount = 0;
	memset(m_bShowCardCount , 0 , sizeof(m_bShowCardCount)) ;

	//������Ϣ
	m_wTurnWiner  = INVALID_CHAIR;					 
	m_bTurnCardCount = 0 ;					 
	memset(m_bTurnCardData, 0 , GAME_PLAYER);	
	m_wFirstOutUser = INVALID_CHAIR;

	//�����˿�
	BYTE bRandCard[52];
	m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_bCardCount[i]=HAND_CARD_COUNT;
		CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
		m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i]);
	}
	//�����û�	

	//�����˿�
	CMD_S_SendCard SendCard;
	memset(&SendCard , 0 , sizeof(SendCard)) ;

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		SendCard.wCurrentUser=i;
		CopyMemory(SendCard.bCardData,m_bHandCardData[i],sizeof(SendCard.bCardData));
        m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	//���ȳ�����
	if(INVALID_CHAIR==m_wGetGpadeUser)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
			for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j)
			{
				BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
				bCardColor = bCardColor>>4 ;
				BYTE bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
				if(1==bCardColor && 2==bCardValue) 
				{
					m_wCurrentUser = i ;
					break ;
				}
			}
	}
	else  
		m_wCurrentUser = m_wGetGpadeUser ;

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			m_wGetGpadeUser = INVALID_CHAIR;

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

			//�������
			bool    bShowSpade   = false ,    //������Q
				    bShowHearts  = false ,    //������A
					bShowClubs   = false ,    //��÷��10
					bShowDiamonds= false ;    //������J
			BYTE    bCardColor ;
			BYTE    bCardValue ;

			//�ж�����
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				for(BYTE j=0 ; j<4 ; ++j)
				{
					if(0!=m_bShowCard[i][j])
					{
						bCardColor = m_GameLogic.GetCardColor(m_bShowCard[i][j]) ;
						bCardColor = bCardColor>>4 ;
						bCardValue = m_GameLogic.GetCardValue(m_bShowCard[i][j]) ;
						//�ж��˿�
                        
						//����Q
						if(3==bCardColor && 12==bCardValue)  bShowSpade         = true ;
						//����A
						else if(2==bCardColor && 1==bCardValue)   bShowHearts   = true ;
						//����J
						else if(0==bCardColor && 11==bCardValue)  bShowDiamonds = true ;
                        //÷��10
						else if(1==bCardColor && 10==bCardValue)  bShowClubs    = true ; 
					}
				}
			}

			//�˿˷���
			int  bScore[16]={0,0,0,-10,-10,-10,-10,-10,-10,-20,-30,-40,-50,100,-100,50} ;
			BYTE bIndex ;

			//���ҷ���
			if(true==bShowHearts) for(BYTE i=0 ; i<13 ; ++i) bScore[i] *=2 ;
			//���Ʒ���
			if(true==bShowSpade)    bScore[14] *= 2 ;
			//���Ʒ���
			if(true==bShowDiamonds) bScore[13] *= 2 ;

			bool  bGetClubs    = false  ,     //�ñ�ѹ��
				  bGetHearts   = false  ,     //�ú���A
				  bGetSpade    = false ,      //�ú���Q
				  bGetDiamonds = false ;      //�÷���J

			LONG  lCellScore=m_pGameServiceOption->lCellScore;

			//�������
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				bGetClubs    = false ;
				bGetHearts   = false ;
				bGetSpade    = false ;
				bGetDiamonds = false ;      

				//û�з���
				if(0==m_bScoreCardCount[i]) continue ;

				for(BYTE j=0 ; j<m_bScoreCardCount[i] ; ++j)
				{
					//���ݼ���
					ASSERT(0!=m_bScoreCard[i][j]) ;
					if(0==m_bScoreCard[i][j]) return false ;
					bCardColor = m_GameLogic.GetCardColor(m_bScoreCard[i][j]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(m_bScoreCard[i][j]) ;
					//���ݼ���
					ASSERT(2==bCardColor || (0==bCardColor && 11==bCardValue) || (1==bCardColor && 10==bCardValue) || (3==bCardColor && 12==bCardValue));   

					bIndex = bCardValue-2 ;
					//����Q
					if(3==bCardColor && 12==bCardValue) 
					{
						bGetSpade   = true ;
						bIndex      = 14 ; 
					}
					//����A
					if(2==bCardColor && 1==bCardValue) 
					{
						bGetHearts   = true ;
						bIndex       = 12 ; 
					}
					//����J
					if(0==bCardColor && 11==bCardValue)
					{
						bGetDiamonds = true ;
						bIndex       = 13 ; 
					}
					//÷��10
					if(1==bCardColor && 10==bCardValue)
					{
						bGetClubs    = true ; 
						bIndex       = 15 ; 
					}
					//��÷��10
					if(1!=bCardColor) GameEnd.lGameScore[i]  += bScore[bIndex] ;
				}	

				//�����÷�

				//�����ѹ
				if(1==m_bScoreCardCount[i]      && true==bShowClubs  && true==bGetClubs)  GameEnd.lGameScore[i] = 100 ;
				else if(1==m_bScoreCardCount[i] && false==bShowClubs && true==bGetClubs)  GameEnd.lGameScore[i] = 50  ;
				else if(m_bScoreCardCount[i]>1  && false==bShowClubs && true==bGetClubs)  GameEnd.lGameScore[i] *= 2  ;
				else if(m_bScoreCardCount[i]>1  && true==bShowClubs  && true==bGetClubs)  GameEnd.lGameScore[i] *= 4  ;

				//����÷�
				if((13==m_bScoreCardCount[i]) && (false==bGetClubs) && (false==bGetDiamonds) && (false==bGetSpade))
				{
					if(true==bShowHearts) GameEnd.lGameScore[i] = 400 ;
					else                  GameEnd.lGameScore[i] = 200 ;
				}
				//����÷�
				if(16==m_bScoreCardCount[i])
				{
					//�������
					if(false!=bShowSpade || false!=bShowHearts || false!=bShowDiamonds || false!=bShowClubs) 
					{
						GameEnd.lGameScore[i] = 0 ;
						for(BYTE k=0 ; k<15 ; ++k) GameEnd.lGameScore[i] += abs(bScore[k]) ;
						//��ѹ��
						if(true==bShowClubs) GameEnd.lGameScore[i] *= 4 ;
						else                 GameEnd.lGameScore[i] *= 2 ;
					}
					else//û������ 
						GameEnd.lGameScore[i] = 800 ;
					
				}
			}

			//ƽ�����
			LONG wTempScore[GAME_PLAYER] ;
			memset(wTempScore , 0 , sizeof(wTempScore)) ;
			wTempScore[0] = GameEnd.lGameScore[0]-(GameEnd.lGameScore[1]+GameEnd.lGameScore[2]+GameEnd.lGameScore[3])/3 ;
			wTempScore[1] = GameEnd.lGameScore[1]-(GameEnd.lGameScore[0]+GameEnd.lGameScore[2]+GameEnd.lGameScore[3])/3 ;
			wTempScore[2] = GameEnd.lGameScore[2]-(GameEnd.lGameScore[0]+GameEnd.lGameScore[1]+GameEnd.lGameScore[3])/3 ;
			wTempScore[3] = GameEnd.lGameScore[3]-(GameEnd.lGameScore[0]+GameEnd.lGameScore[1]+GameEnd.lGameScore[2])/3 ;
			
			 //ƽ�⴦��
			if(0!=(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3]))
			{
				if((wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])<0)
				{
					for(BYTE i=0 ; i<GAME_PLAYER ;)
					{
						if(wTempScore[i]<0) 
						{
							wTempScore[i] += 1 ;
							if(0==(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])) break; 
						}
						++i ;
						if((0!=(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])) && i==GAME_PLAYER) i=0 ;
					}
				}
				else
				{
					for(BYTE i=0 ; i<GAME_PLAYER ;)
					{
						if(wTempScore[i]>0) 
						{
							wTempScore[i] -= 1 ;
							if(0==(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])) break; 
						}
						++i ;
						if((0!=(wTempScore[0]+wTempScore[1]+wTempScore[2]+wTempScore[3])) && i==GAME_PLAYER) i=0 ;
					}
				}
			}

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				GameEnd.lGameScore[i] = wTempScore[i] ;
				GameEnd.lGameScore[i] *= lCellScore ;	
			}


			//����˰��
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/10L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*45L/50L;
					}
				}
			}		

			TRACE("��������һ���:\n") ;
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				CString str ;
				str.Format("%ld\n" , GameEnd.lGameScore[i]) ;
				TRACE(str);
			}
			TRACE("���ֽ���\n");
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
			}
//			m_pITableFrame->WriteTableScore(ScoreInfo,m_wPlayerCount,GameEnd.lGameTax);

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			m_wGetGpadeUser = INVALID_CHAIR;

			//�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//ͳ�ƻ���
			GameEnd.lGameScore[wChairID]=-100L*m_pGameServiceOption->lCellScore;

			//����˰��
			if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						GameEnd.lGameTax+=GameEnd.lGameScore[i]/10L;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]*45L/50L;
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
			m_pITableFrame->WriteUserScore(wChairID,ScoreInfo.lScore ,ScoreInfo.lRevenue ,ScoreInfo.ScoreKind );

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	switch (bGameStatus)
	{
	case GS_WK_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_SHOWCARD:	//�������
		{
			CMD_S_StatusShowCard StatusShowCard ;
			memset(&StatusShowCard , 0 , sizeof(StatusShowCard)) ;
			StatusShowCard.wCurrentUser = wChiarID ;
			CopyMemory(StatusShowCard.ShowCard , m_bShowCard , sizeof(m_bShowCard)) ;
			CopyMemory(StatusShowCard.ShowCardUser , m_wShowCardUser , sizeof(m_wShowCardUser)) ;
			CopyMemory(StatusShowCard.bCardData , m_bHandCardData[wChiarID] , sizeof(m_bHandCardData[wChiarID])) ;
			CopyMemory(StatusShowCard.ShowCardCount , m_bShowCardCount , sizeof(m_bShowCardCount)) ;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusShowCard,sizeof(StatusShowCard));
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.wFirstOutUser = m_wFirstOutUser ;

			CopyMemory(StatusPlay.ScoreCard , m_bScoreCard , sizeof(m_bScoreCard)) ;
			CopyMemory(StatusPlay.ShowCard , m_bShowCard , sizeof(m_bShowCard)) ;
			CopyMemory(StatusPlay.ShowCardUser , m_wShowCardUser , sizeof(m_wShowCardUser)) ;
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*sizeof(StatusPlay.bTurnCardData));
			CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.bShowCardCount , m_bShowCardCount , sizeof(m_bShowCardCount)) ;
			CopyMemory(StatusPlay.bScoreCardCount , m_bScoreCardCount , sizeof(m_bScoreCardCount)) ;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_SHOW_CART:       //�������
		{
			CMD_C_ShowCard *pShowCard = (CMD_C_ShowCard*)pDataBuffer ;
			
			//����У��
			ASSERT(wDataSize==sizeof(CMD_C_ShowCard)) ;
			if(wDataSize!=sizeof(CMD_C_ShowCard)) return false ;
            
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();		

			++m_bShowCardTime;

			//û������
			if(0==pShowCard->bShowCardCount) 
			{
				//��Ϸ��ʼ 
				if(GAME_PLAYER==m_bShowCardTime)
				{
					//��������
					if(3==m_bTotalShowCardCount)
					{
						BYTE bCardColor ;
						BYTE bCardValue ;
						bool bHaveShow=false ;

						for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
							for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j) 
							{
								bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
								bCardColor = bCardColor>>4 ;
								bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
								//�ж��˿�
								if((3==bCardColor && 12==bCardValue)  || //����Q
									(2==bCardColor && 1==bCardValue)  || //����A
									(0==bCardColor && 11==bCardValue) || //����J
									(1==bCardColor && 10==bCardValue))   //÷��10
								{
									bHaveShow = false ;
									for(BYTE p=0 ; p<GAME_PLAYER ; ++p)
										for(BYTE c=0 ; c<4 ; ++c)
										{
											if(m_bShowCard[p][c]==m_bHandCardData[i][j]) bHaveShow=true ;
										}
										if(false==bHaveShow)
										{
											m_wShowCardUser[i] = i ; 
											for(BYTE k=0 ; k<4 ; ++k)  if(0==m_bShowCard[i][k]) break ;
											m_bShowCard[i][k] = m_bHandCardData[i][j] ;

											//��������
											CMD_S_ShowCard ShowCard ;
											memset(&ShowCard , 0 , sizeof(ShowCard)) ;
											ShowCard.wShowCardUser = i ;
											ShowCard.bShowCardCount = k+1 ;
											CopyMemory(ShowCard.bShowCard , m_bShowCard[i] , ShowCard.bShowCardCount) ;

											m_bShowCardCount[i] = ShowCard.bShowCardCount ;

											m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
											m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
											break ;
										}
								}
							}
					}

					CMD_S_GameStart GameStart ;
					memset(&GameStart , 0 , sizeof(GameStart)) ;
					GameStart.wCurrentUser = m_wCurrentUser ;
					GameStart.bFirstGame   = (INVALID_CHAIR==m_wGetGpadeUser) ;
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
					m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
				}
				return true ;
			}
			
			//��¼����
			m_wShowCardUser[m_bShowCardUserCount++] = pUserData->wChairID ;
			CopyMemory(m_bShowCard[pUserData->wChairID] , pShowCard->bShowCard , pShowCard->bShowCardCount*sizeof(BYTE)) ;

			return OnUserShowCard(pUserData->wChairID , pShowCard->bShowCard , pShowCard->bShowCardCount) ;
		}
	case SUB_C_OUT_CART:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) 
			{
				return false;
			}

			ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData)));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData))) 
			{
				return false;
			}


			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
		
			return OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,1);
			//////////////////////////////////////////////////////////////////////////

		}
	}

	return true;
}

//�������
bool CTableFrameSink::OnUserShowCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//��������
	CMD_S_ShowCard ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;
	ShowCard.wShowCardUser = wChairID ;
	ShowCard.bShowCardCount = bCardCount ;
	CopyMemory(ShowCard.bShowCard , bCardData , bCardCount) ;

	//��������
	m_wShowCardUser[wChairID] = wChairID ;      
	CopyMemory(m_bShowCard[wChairID] , bCardData , bCardCount) ;
	m_bTotalShowCardCount += bCardCount ;

	m_bShowCardCount[wChairID] = bCardCount ;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
    

	//��Ϸ��ʼ 
	if(GAME_PLAYER==m_bShowCardTime)
	{
		//��������
	    if(3==m_bTotalShowCardCount)
		{
			BYTE bCardColor ;
			BYTE bCardValue ;
			bool bHaveShow=false ;

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
				for(BYTE j=0 ; j<HAND_CARD_COUNT ; ++j) 
				{
                    bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i][j]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i][j]) ;
					//�ж��˿�
					if((3==bCardColor && 12==bCardValue)  || //����Q
						(2==bCardColor && 1==bCardValue)  || //����A
						(0==bCardColor && 11==bCardValue) || //����J
						(1==bCardColor && 10==bCardValue))   //÷��10
					{
						bHaveShow = false ;
						for(BYTE p=0 ; p<GAME_PLAYER ; ++p)
							for(BYTE c=0 ; c<4 ; ++c)
							{
								if(m_bShowCard[p][c]==m_bHandCardData[i][j]) bHaveShow=true ;
							}
						if(false==bHaveShow)
						{
							m_wShowCardUser[i] = i ; 
							for(BYTE k=0 ; k<4 ; ++k)  if(0==m_bShowCard[i][k]) break ;
                            m_bShowCard[i][k] = m_bHandCardData[i][j] ;

							//��������
							CMD_S_ShowCard ShowCard ;
							memset(&ShowCard , 0 , sizeof(ShowCard)) ;
							ShowCard.wShowCardUser = i ;
							ShowCard.bShowCardCount = k+1 ;
							CopyMemory(ShowCard.bShowCard , m_bShowCard[i] , ShowCard.bShowCardCount) ;

							m_bShowCardCount[i] = ShowCard.bShowCardCount ;

							m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
							m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));

							break ;
						}
					}
				}				
		}
		CMD_S_GameStart GameStart ;
		memset(&GameStart , 0 , sizeof(GameStart)) ;
		GameStart.wCurrentUser = m_wCurrentUser ;
		GameStart.bFirstGame   = (INVALID_CHAIR==m_wGetGpadeUser) ;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
	}
    return true ;
}


//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData, BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(&bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;	

	//���ĳ���
	m_bTurnCardCount++ ;
	if(1==m_bTurnCardCount) m_wFirstOutUser = wChairID ;
	m_bTurnCardData[wChairID] = bCardData ;
	if(GAME_PLAYER==m_bTurnCardCount) 
	{
		m_bTurnCardCount = 0 ;

		//������
		WORD wMostUser = m_wFirstOutUser ;
		BYTE bFirstCardColor = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
        bFirstCardColor = bFirstCardColor >> 4 ;
		BYTE bMostCardValue  = m_GameLogic.GetCardValue(m_bTurnCardData[m_wFirstOutUser]) ;
		bool bFoudGpade = false ;

		if(1==bMostCardValue) bMostCardValue += 13 ;
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			BYTE bCurrentCarColor  = m_GameLogic.GetCardColor(m_bTurnCardData[i]) ;
            bCurrentCarColor = bCurrentCarColor >> 4 ;
			BYTE bCurrentCardValue = m_GameLogic.GetCardValue(m_bTurnCardData[i]) ;
			if(1==bCurrentCardValue) bCurrentCardValue += 13 ;
			if(bCurrentCarColor == bFirstCardColor)
				if(bMostCardValue < bCurrentCardValue)
				{
					bMostCardValue = bCurrentCardValue ;
					wMostUser = i ;
				}
			//�õ�����Q
			if(3==bCurrentCarColor && 12==bCurrentCardValue) bFoudGpade=true ;
		}
		//�õ�����Q
		if(true==bFoudGpade) m_wGetGpadeUser = wMostUser ;

		//��ȡ����
		BYTE bScoreCard[GAME_PLAYER] ;
		BYTE bScoreCount=0 ;
        if(true==m_GameLogic.GetScoreCard(m_bTurnCardData , bScoreCard , GAME_PLAYER , bScoreCount))
		{
			CopyMemory(&m_bScoreCard[wMostUser][m_bScoreCardCount[wMostUser]] , bScoreCard , bScoreCount) ;
            m_bScoreCardCount[wMostUser] += bScoreCount ;
		}

        m_wTurnWiner = wMostUser ;
		if (m_bCardCount[wChairID]!=0)
		{
			m_wCurrentUser = wMostUser ;
		}
		else m_wCurrentUser=INVALID_CHAIR;	
		m_wFirstOutUser = wMostUser;	
		memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;
	}	
	else
	{
		m_wTurnWiner=wChairID;
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		if(0==m_bCardCount[m_wCurrentUser])	m_wCurrentUser=INVALID_CHAIR;
	}
	//�л��û�
	
	//��������
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	WORD wSendSize;
	CopyMemory(&OutCard.bCardData,&bCardData,bCardCount*sizeof(BYTE));
	wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);

	//��������

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�жϽ�����Ϸ
	
	if (m_wCurrentUser==INVALID_CHAIR) 
	{
		OnEventGameEnd(wChairID,NULL,GER_NORMAL);
	}

	return true;
}
//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����


//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
