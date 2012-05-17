#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

#define	STOCK_TAX						0									//�ؿۺ���

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{						
	//��Ϸ����	
	m_lStockScore = 0;
	m_lExitScore=0;	
	//m_wOperaCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wFisrtCallUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//�û�״̬
	//ZeroMemory(m_wWinCount,sizeof(m_wWinCount));		
	//ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));m_bCallStatus
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++)m_bOxCard[i]=0xff;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ--��������Ϣ�汾
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
	m_lExitScore=0;	
	m_wCurrentUser=INVALID_CHAIR;
	//m_wOperaCount=0;

	//�û�״̬
	//ZeroMemory(m_wWinCount,sizeof(m_wWinCount));		
	//ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bPlayStatus,sizeof(m_bPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++)m_bOxCard[i]=0xff;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

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
	ASSERT(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE);
	if(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE)return true;
	return false;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

	//�û�״̬
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem *pIServerUser=m_pITableFrame->GetServerUserItem(i);

		if(pIServerUser==NULL)
		{
			m_bPlayStatus[i]=FALSE;
		}
		else
		{
			m_bPlayStatus[i]=TRUE;
		}
	}

	//�׾����ʼ��
	if(m_wFisrtCallUser==INVALID_CHAIR)
	{
		m_wFisrtCallUser=rand()%m_wPlayerCount;
	}
	else
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	//ʼ���û�
	while(m_bPlayStatus[m_wFisrtCallUser]!=TRUE)
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	//��ǰ�û�
	m_wCurrentUser=m_wFisrtCallUser;

	//���ñ���
	CMD_S_CallBanker CallBanker;
	CallBanker.wCallBanker=m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			WORD m_wWinTimes[GAME_PLAYER],m_wWinCount[GAME_PLAYER];
			ZeroMemory(m_wWinCount,sizeof(m_wWinCount));
			ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

			//�����˿�
			BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
			CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

			//ׯ�ұ���
			ASSERT(m_bOxCard[m_wBankerUser]<2);
			if(m_bOxCard[m_wBankerUser]==TRUE)
				m_wWinTimes[m_wBankerUser]=m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser],MAX_COUNT);
			else m_wWinTimes[m_wBankerUser]=1;

			//�Ա����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;

				ASSERT(m_bOxCard[i]<2);

				//�Ա��˿�
				if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,m_bOxCard[i],m_bOxCard[m_wBankerUser])) 
				{
					m_wWinCount[i]++;
					//��ȡ����
					if(m_bOxCard[i]==TRUE)
						m_wWinTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
					else m_wWinTimes[i]=1;
				}
				else
				{
					m_wWinCount[m_wBankerUser]++;
				}
			}

			//ͳ�Ƶ÷�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;

				WORD j=i;
				if(m_wWinCount[j]>0)	//�м�ʤ��
				{
					GameEnd.lGameScore[j]=m_lTableScore[j]*m_wWinTimes[j];
					GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[j];
					m_lTableScore[j]=0;
				}
				else					//ׯ��ʤ��
				{
					GameEnd.lGameScore[j]=(-1)*m_lTableScore[j]*m_wWinTimes[m_wBankerUser];
					GameEnd.lGameScore[m_wBankerUser]+=(-1)*GameEnd.lGameScore[j];
					m_lTableScore[j]=0;
				}
			}

			//�м�ǿ�˷���	
			GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

			//�뿪�û�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//��˰����
			WORD cbRevenue=m_pGameServiceOption->wRevenue;

			//����˰��
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(GameEnd.lGameScore[i]>0L)
				{
					GameEnd.lGameTax[i]=GameEnd.lGameScore[i]*cbRevenue/1000L;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
				}
			}

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				enScoreKind nScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
				m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],nScoreKind);
			}

			//���ͳ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(i);
				if ((pIServerUserIte==NULL)||(pIServerUserIte->IsAndroidUser()==false)) continue;

				//����ۼ�
				m_lStockScore+=GameEnd.lGameScore[i];
			}

			//������
			m_lStockScore=m_lStockScore-m_lStockScore*STOCK_TAX/100;

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount && m_bPlayStatus[wChairID]==TRUE);

			//����״̬
			m_bPlayStatus[wChairID]=FALSE;

			//�������
			CMD_S_PlayerExit PlayerExit;
			PlayerExit.wPlayerID=wChairID;

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID || m_bPlayStatus[i]==FALSE)continue;
				m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

			WORD m_wWinTimes[GAME_PLAYER];
			if (m_pITableFrame->GetGameStatus()>GS_TK_CALL)
			{
				if (wChairID==m_wBankerUser)	//ׯ��ǿ��
				{
					//�������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));
					ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

					BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
					CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

					//�÷ֱ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_wWinTimes[i]=(m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING)?(1):(m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT));
					}

					//ͳ�Ƶ÷� ���»�û��
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						GameEnd.lGameScore[i]=m_lTableScore[i]*m_wWinTimes[i];
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[i];
						m_lTableScore[i]=0;
					}

					//�м�ǿ�˷��� 
					GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

					//�뿪�û�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
					}

					//��˰����
					WORD cbRevenue=m_pGameServiceOption->wRevenue;

					//����˰��
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(GameEnd.lGameScore[i]>0L)
						{
							GameEnd.lGameTax[i]=GameEnd.lGameScore[i]*cbRevenue/1000L;
							GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
						}
					}

					//������Ϣ
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//�޸Ļ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_bPlayStatus[i]==FALSE && i!=m_wBankerUser)continue;
						enScoreKind nScoreKind=(GameEnd.lGameScore[i]>0L)?enScoreKind_Win:enScoreKind_Lost;
						m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],nScoreKind);
					}

					//��ȡ�û�
					IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(m_wBankerUser);
					
					//����ۼ�
					if ((pIServerUserIte!=NULL)&&(pIServerUserIte->IsAndroidUser()!=false)) 
						m_lStockScore+=GameEnd.lGameScore[m_wBankerUser];
	
					//������
					m_lStockScore=m_lStockScore-m_lStockScore*STOCK_TAX/100;

					//������Ϸ
					m_pITableFrame->ConcludeGame();
				}
				else						//�м�ǿ��
				{
					//�Ѿ���ע
					if (m_lTableScore[wChairID]>0L)
					{
						ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

						//�û��˿�
						BYTE cbUserCardData[MAX_COUNT];
						CopyMemory(cbUserCardData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);

						//�û�����
						m_wWinTimes[m_wBankerUser]=(m_pITableFrame->GetGameStatus()==GS_TK_SCORE)?(1):(m_GameLogic.GetTimes(cbUserCardData,MAX_COUNT));

						//�޸Ļ���
						LONG lScore=-m_lTableScore[wChairID]*m_wWinTimes[m_wBankerUser];
						m_lExitScore+=(-1*lScore);
						m_lTableScore[wChairID]=(-1*lScore);
						m_pITableFrame->WriteUserScore(wChairID,lScore,0,enScoreKind_Lost);

						//��ȡ�û�
						IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(wChairID);
						
						//����ۼ�
						if ((pIServerUserIte!=NULL)&&(pIServerUserIte->IsAndroidUser()!=false)) 
							m_lStockScore+=lScore;		

					}

					//�������
					WORD wUserCount=0;
					for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE)wUserCount++;

					//������Ϸ
					if(wUserCount==1)
					{
						//�������
						CMD_S_GameEnd GameEnd;
						ZeroMemory(&GameEnd,sizeof(GameEnd));
						ASSERT(m_lExitScore>=0L); 

						//��˰����
						WORD cbRevenue=m_pGameServiceOption->wRevenue;

						//ͳ�Ƶ÷�
						GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;
						GameEnd.lGameTax[m_wBankerUser]=GameEnd.lGameScore[m_wBankerUser]*cbRevenue/1000L;
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameTax[m_wBankerUser];

						//�뿪�û�
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
						}

						//������Ϣ
						m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

						for (WORD Zero=0;Zero<m_wPlayerCount;Zero++)if(m_lTableScore[Zero]!=0)break;
						if(Zero!=m_wPlayerCount)
						{
							//�޸Ļ���
							LONG lRevenue = GameEnd.lGameTax[m_wBankerUser];
							LONG lScore=GameEnd.lGameScore[m_wBankerUser];
							m_pITableFrame->WriteUserScore(m_wBankerUser,lScore,lRevenue,enScoreKind_Win);

							//��ȡ�û�
							IServerUserItem * pIServerUserIte=m_pITableFrame->GetServerUserItem(wChairID);
							
							//����ۼ�
							if ((pIServerUserIte!=NULL)&&(pIServerUserIte->IsAndroidUser()!=false)) 
								m_lStockScore+=lScore;
						}

						//������
						m_lStockScore=m_lStockScore-m_lStockScore*STOCK_TAX/100;

						//������Ϸ
						m_pITableFrame->ConcludeGame();		
					}
					else if	(m_pITableFrame->GetGameStatus()==GS_TK_SCORE && m_lTableScore[wChairID]==0L)
					{
						OnUserAddScore(wChairID,0);
					}
					else if (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && m_bOxCard[wChairID]==0xff)
					{
						OnUserOpenCard(wChairID,0);
					}
				}
			}
			else 
			{
				//�������
				WORD wUserCount=0;
				for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE)wUserCount++;

				//������Ϸ
				if(wUserCount==1)
				{
					//�������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//������Ϣ
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_bPlayStatus[i]==FALSE)continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//������Ϸ
					m_pITableFrame->ConcludeGame();			
				}
				else if(m_wCurrentUser==wChairID)OnUserCallBanker(wChairID,0);
			}

			return true;
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=0L;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_CALL:	//��ׯ״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//���ñ���
			StatusCall.wCallBanker=m_wCurrentUser;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//��ע��Ϣ
			StatusScore.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusScore.wBankerUser=m_wBankerUser;

			//���û���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				StatusScore.lTableScore[i]=m_lTableScore[i];
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//������Ϣ
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusPlay.wBankerUser=m_wBankerUser;
			CopyMemory(StatusPlay.bOxCard,m_bOxCard,sizeof(StatusPlay.bOxCard));

			//�����˿�
			for (WORD i=0;i< m_wPlayerCount;i++)
			{
				if(m_bPlayStatus[i]==FALSE)continue;
				WORD j= i;
				StatusPlay.lTableScore[j]=m_lTableScore[j];
				CopyMemory(StatusPlay.cbHandCardData[j],m_cbHandCardData[j],MAX_COUNT);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}
	//Ч�����
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
	case SUB_C_CALL_BANKER:			//�û���ׯ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallBanker));
			if (wDataSize!=sizeof(CMD_C_CallBanker)) return false;

			//��������
			CMD_C_CallBanker * pCallBanker=(CMD_C_CallBanker *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return false;

			//��Ϣ����
			return OnUserCallBanker(pUserData->wChairID,pCallBanker->bBanker);
		}
	case SUB_C_ADD_SCORE:			//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return false;

			//��Ϣ����
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
		}
	case SUB_C_OPEN_CARD:			//�û�̯��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_OxCard));
			if (wDataSize!=sizeof(CMD_C_OxCard)) return false;

			//��������
			CMD_C_OxCard * pOxCard=(CMD_C_OxCard *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_bPlayStatus[pUserData->wChairID]!=FALSE);
			if(m_bPlayStatus[pUserData->wChairID]==FALSE)return false;

			//��Ϣ����
			return OnUserOpenCard(pUserData->wChairID,pOxCard->bOX);
		}
	}

	return false;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//��ׯ�¼�
bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE bBanker)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_CALL) return false;

	//���ñ���
	m_bCallStatus[wChairID]=TRUE;

	//��ׯ����
	WORD wCallUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bPlayStatus[i]==TRUE && m_bCallStatus[i]==TRUE) wCallUserCount++;
		else if(m_bPlayStatus[i]!=TRUE) wCallUserCount++;
	}

	//��ע��ʼ
	if(bBanker==TRUE || wCallUserCount==m_wPlayerCount)
	{
		//ʼ���û�
		m_wBankerUser=wChairID;

		//�������һ����ׯ�û�ǿ�����
		while(m_bPlayStatus[m_wBankerUser]==FALSE)
		{
			m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;
		}

		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//�������
		WORD wUserCount=0;
		for (WORD i=0;i<m_wPlayerCount;i++)if(m_bPlayStatus[i]==TRUE )wUserCount++;

		//�����ע
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE || i==m_wBankerUser)continue;

			//��ȡ�û�
			pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//��ȡ����
			const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();

			//��Ҽ���
			ASSERT(pUserScore->lScore>=m_pGameServiceOption->lCellScore);

			//��ע���� �ͻ�Ҫ��
			m_lTurnMaxScore[i]=__min(lBankerScore/(wUserCount-1)/5,pUserScore->lScore/5);
		}

		//���ñ���
		//m_wOperaCount=0;
		CMD_S_GameStart GameStart;
		GameStart.wBankerUser=m_wBankerUser;
		GameStart.lTurnMaxScore=0;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE)continue;
			GameStart.lTurnMaxScore=m_lTurnMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	else		 //�û���ׯ
	{
		//�����¸����
		do{
			m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}while(m_bPlayStatus[m_wCurrentUser]==FALSE);

		//���ñ���
		CMD_S_CallBanker CallBanker;
		CallBanker.wCallBanker=m_wCurrentUser;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]!=TRUE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONG lScore)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_SCORE) return false;

	//���Ч��
	if(m_bPlayStatus[wChairID]==TRUE)
	{
		ASSERT(lScore>0 && lScore<=m_lTurnMaxScore[wChairID]);
		if (lScore<=0 || lScore>m_lTurnMaxScore[wChairID]) return false;
	}
	else //û��ע���ǿ��
	{
		ASSERT(lScore==0);
		if (lScore!=0) return false;
	}

	if(lScore>0L)
	{
		//��ע���
		m_lTableScore[wChairID]=lScore;

		//��������
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.lAddScoreCount=m_lTableScore[wChairID];

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	}

	//��ע����
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_lTableScore[i]>0L && m_bPlayStatus[i]==TRUE)bUserCount++;
		else if(m_bPlayStatus[i]==FALSE || i==m_wBankerUser)bUserCount++;
	}

	//�м�ȫ��
	if(bUserCount==m_wPlayerCount)
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

		//��������
		CMD_S_SendCard SendCard;
		ZeroMemory(SendCard.cbCardData,sizeof(SendCard.cbCardData));

		//��ȡ�˿�
		BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
		m_GameLogic.RandCardList(bTempArray,sizeof(bTempArray));
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;

			//�ɷ��˿�
			CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
			m_GameLogic.SortCardList(m_cbHandCardData[i],MAX_COUNT);
		}

		//�����˿�
		AnalyseCard();

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;

			//�ɷ��˿�
			CopyMemory(SendCard.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);
		}

		//��������
		for (WORD i=0;i< m_wPlayerCount;i++)
		{
			if(m_bPlayStatus[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	return true;
}

//̯���¼�
bool CTableFrameSink::OnUserOpenCard(WORD wChairID, BYTE bOx)
{
	//״̬Ч��
	ASSERT (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return false;

	//Ч������
	ASSERT(bOx==FALSE || bOx==TRUE);
	if(bOx!=FALSE && bOx!=TRUE)return false;

	//Ч������
	if(bOx)
	{
		ASSERT(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0);
		if(!(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0))return false;
	}

	//ţţ����
	m_bOxCard[wChairID] = bOx;

	//̯������
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bOxCard[i]<2 && m_bPlayStatus[i]==TRUE)bUserCount++;
		else if(m_bPlayStatus[i]==FALSE)bUserCount++;
	}

	 //�������
	CMD_S_Open_Card OpenCard;
	ZeroMemory(&OpenCard,sizeof(OpenCard));

	//���ñ���
	OpenCard.bOpen=bOx;
	OpenCard.wPlayerID=wChairID;

	//��������
	for (WORD i=0;i< m_wPlayerCount;i++)
	{
		if(m_bPlayStatus[i]==FALSE)continue;
		m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

	//������Ϸ
	if(bUserCount == m_wPlayerCount)
	{
		return OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	return true;
}

//�˿˷���
void CTableFrameSink::AnalyseCard()
{
	//��������
	bool bIsAiBanker = false;
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			ASSERT(m_bPlayStatus[i]==TRUE);
			if(pIServerUserItem->IsAndroidUser()) 
			{
				wAiCount++ ;
				if(!bIsAiBanker && i==m_wBankerUser)bIsAiBanker = true;
			}
			wPlayerCount++; 
		}
	}

	//ȫ������
	if(wPlayerCount == wAiCount || wAiCount==0)return;

	//�˿˱���
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

	//ţţ����
	BOOL bUserOxData[GAME_PLAYER];
	ZeroMemory(bUserOxData,sizeof(bUserOxData));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bPlayStatus[i]!=TRUE)continue;
		bUserOxData[i] = (m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
	}

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
	}

	//��������
	LONG lAndroidScore=0;

	//��������
	BYTE cbCardTimes[GAME_PLAYER];
	ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

	//���ұ���
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_bPlayStatus[i]==TRUE)
		{
			cbCardTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
		}
	}

	//����ׯ��
	if(bIsAiBanker)
	{
		//�Ա��˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�û�����
			if ((i==m_wBankerUser)||(m_bPlayStatus[i]==FALSE)) continue;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//��������
			if ((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser())) continue;

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,bUserOxData[i],bUserOxData[m_wBankerUser])==true)
			{
				lAndroidScore-=cbCardTimes[i]*m_lTableScore[i];
			}
			else
			{
				lAndroidScore+=cbCardTimes[m_wBankerUser]*m_lTableScore[i];
			}
		}
	}
	else//�û�ׯ��
	{
		//�Ա��˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);

			//�û�����
			if ((i==m_wBankerUser)||(pIServerUserItem==NULL)||!(pIServerUserItem->IsAndroidUser())) continue;

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,bUserOxData[i],bUserOxData[m_wBankerUser])==true)
			{
				lAndroidScore+=cbCardTimes[i]*m_lTableScore[i];
			}
			else
			{
				lAndroidScore-=cbCardTimes[m_wBankerUser]*m_lTableScore[i];
			}
		}
	}

	//����ж�
	if ((m_lStockScore+lAndroidScore)<0)
	{
		//��������
		WORD wWinUser=INVALID_CHAIR;

		//�ж����
		bool bCardMax = (bIsAiBanker)?true:false;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�û�����
			if (m_bPlayStatus[i]==FALSE) continue;

			//�����û�
			if (wWinUser==INVALID_CHAIR)
			{
				wWinUser=i;
				continue;
			}

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],m_cbHandCardData[wWinUser],MAX_COUNT,bUserOxData[i],bUserOxData[wWinUser])==bCardMax)
			{
				wWinUser=i;
			}
		}

		//��������
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[wWinUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wWinUser],cbTempData,MAX_COUNT);
	}

	return;
}


//////////////////////////////////////////////////////////////////////////
