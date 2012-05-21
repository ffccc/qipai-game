#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//ʱ�䶨��
#define TIME_BANKER_START			30										//ׯ�ҿ�ʼ
#define TIME_PLAYER_START			30										//��ҿ�ʼ

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{	
	//״̬����
	m_cbSendCard=FALSE;
	m_cbWaitStart=FALSE;
	m_cbWaitChange=FALSE;
	m_cbWaitContinue=FALSE;
	m_wMaxChairID=INVALID_CHAIR;

	//��Ϸ����
	m_lExitScore = 0;
	m_bOffLineGameEnd = false;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_lUserChipIn,sizeof(m_lUserChipIn));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbUserCardData,sizeof(m_cbUserCardData));

	//״̬����
	ZeroMemory(m_cbUserFinish,sizeof(m_cbUserFinish));
	ZeroMemory(m_cbUserBanker,sizeof(m_cbUserBanker));
	ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));

	//��������
	m_lMaxScore=0L;
	m_lLessScore=0L;
	m_dwBankerStartTime=0L;
	m_wCurrentUser=INVALID_CHAIR;

	//AI����
	m_dStockScore=0;

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
	//��������
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//����״̬
	m_lExitScore = 0;
	m_cbSendCard=FALSE;
	ZeroMemory(m_cbUserFinish,sizeof(m_cbUserFinish));

	//��Ϸ����
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_lUserChipIn,sizeof(m_lUserChipIn));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbUserCardData,sizeof(m_cbUserCardData));

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
	m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

	//�û�����
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_pITableFrame->GetServerUserItem(i)!=NULL)
		{
			m_cbPlayStatus[i]=TRUE;
			m_cbUserFinish[i]=FALSE;
		}
		else
		{
			m_cbUserFinish[i]=TRUE;
			m_cbPlayStatus[i]=FALSE;
		}
	}

	//�ַ��˿�
	m_GameLogic.RandCardList(m_cbUserCardData[0],sizeof(m_cbUserCardData)/sizeof(m_cbUserCardData[0][0]));
	//����������
	if (m_wMaxChairID != INVALID_CHAIR)
	{
		//�˿˷���
		for (WORD wUserIndex=0;wUserIndex<m_wPlayerCount;wUserIndex++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wUserIndex);
			if (pIServerUserItem==NULL) continue;
			if (wUserIndex == m_wMaxChairID) continue;

			//�Ա��˿�
			if (m_GameLogic.CompareCard(m_cbUserCardData[wUserIndex],m_cbUserCardData[m_wMaxChairID],MAX_COUNT)==true)
			{
				//��������
				BYTE cbTempData[MAX_COUNT];
				CopyMemory(cbTempData,m_cbUserCardData[wUserIndex],MAX_COUNT);
				CopyMemory(m_cbUserCardData[wUserIndex],m_cbUserCardData[m_wMaxChairID],MAX_COUNT);
				CopyMemory(m_cbUserCardData[m_wMaxChairID],cbTempData,MAX_COUNT);
			}
		}
	}

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��������
		GameStart.lLessScore=m_lLessScore;
		GameStart.lMaxScore=GetUserMaxChipIn(i);
		CopyMemory(GameStart.cbPlayStatus,m_cbPlayStatus,sizeof(GameStart.cbPlayStatus));

		//������Ϣ
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	m_wMaxChairID = INVALID_CHAIR;

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			BYTE cbCardTimes[GAME_PLAYER];
			ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

			//״̬����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					m_GameLogic.SortCardList(m_cbUserCardData[i],MAX_COUNT);
					cbCardTimes[i]=m_GameLogic.GetCardTimes(m_cbUserCardData[i],MAX_COUNT);
				}
			}

			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//�Ա��˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�û�����
				if (i==m_wBankerUser) continue;
				if (m_cbPlayStatus[i]==FALSE) continue;

				//�Ա��˿�
				if (m_GameLogic.CompareCard(m_cbUserCardData[i],m_cbUserCardData[m_wBankerUser],MAX_COUNT)==true) 
				{
					m_lGameScore[i]+=cbCardTimes[i]*m_lUserChipIn[i];
					m_lGameScore[m_wBankerUser]-=cbCardTimes[i]*m_lUserChipIn[i];
				}
				else
				{
					m_lGameScore[i]-=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
					m_lGameScore[m_wBankerUser]+=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
				}
			}

			//ǿ�˷���
			m_lGameScore[m_wBankerUser]+=m_lExitScore;
			m_lExitScore = 0;

			//��˰����
			LONG lRevenueSum=0L;
			BYTE cbRevenue=m_pGameServiceOption->wRevenue;

			//���û���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_lGameScore[i]>=100L)
				{
					lRevenueSum+=m_lGameScore[i]*cbRevenue/100L;
					GameEnd.lGameTax[i]=m_lGameScore[i]*cbRevenue/100L;
					GameEnd.lGameScore[i]=m_lGameScore[i]-m_lGameScore[i]*cbRevenue/100L;
				}
				else GameEnd.lGameScore[i]=m_lGameScore[i];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					//�������
					enScoreKind ScoreKind=(GameEnd.lGameScore[i]>=0L)?enScoreKind_Win:enScoreKind_Lost;

					//д�����
					m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],ScoreKind);
				}
			}

			//ׯ�ҵ���
			BYTE cbCardType=m_GameLogic.GetCardType(m_cbUserCardData[m_wBankerUser],MAX_COUNT);
			BYTE cbCardValue=m_GameLogic.GetCardValue(m_cbUserCardData[m_wBankerUser],MAX_COUNT);

			//��������
			IServerUserItem * pUserInfo=m_pITableFrame->GetServerUserItem(m_wBankerUser);
			tagServerUserData * pServerUserData=pUserInfo->GetUserData();

			//��ׯ�ж�
			//if ((pUserInfo->IsAndroidUser()==false)&&((pServerUserData->dwUserRight&UR_LONG_BANKER)==0))
			{
				//��ׯ�ж�
				if ((cbCardType!=CT_VALUES)||(cbCardValue!=9))
				{
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if ((i!=m_wBankerUser)&&(m_cbPlayStatus[i]==TRUE))
						{
							//��ȡ����
							BYTE cbCardType=m_GameLogic.GetCardType(m_cbUserCardData[i],MAX_COUNT);
							BYTE cbCardValue=m_GameLogic.GetCardValue(m_cbUserCardData[i],MAX_COUNT);

							//��ׯ�ж�
							if ((IsSuitBankerUser(i)==true)&&(cbCardType==CT_VALUES)&&(cbCardValue==9))
							{
								m_cbWaitChange=TRUE;
								m_cbUserBanker[i]=TRUE;
							}
						}
					}
				}
			}

			//���ͳ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
				if ((pIServerUserItem==NULL)||(pIServerUserItem->IsAndroidUser()==false)) continue;

				//����ۼ�
				m_dStockScore+=GameEnd.lGameScore[i];
			}

			//������
			if(m_dStockScore>=100)m_dStockScore=m_dStockScore/100*95;

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//ׯ���ж�
			if ((m_wBankerUser!=INVALID_CHAIR)&&(IsSuitBankerUser(m_wBankerUser)==false))
			{
				//����ׯ��
				m_wBankerUser=INVALID_CHAIR;

				//����ׯ��
				SearceBankerUser();
			}

			//��ע����
			if (m_wBankerUser!=INVALID_CHAIR)
			{
				//��ȡׯ��
				IServerUserItem * pIBankerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);

				//��������
				if (pIBankerUserItem!=NULL)
				{
					m_lLessScore=m_pGameServiceOption->lCellScore;
					m_lMaxScore=(LONG)(pIBankerUserItem->GetUserScore()->lScore)/37L;
				}

				//�ȴ�����
				m_cbWaitContinue=TRUE;
				ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));

				//��ׯ����
				if (m_cbWaitChange==TRUE)
				{
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if (m_cbUserBanker[i]==TRUE)
						{
							m_wCurrentUser=i;
							m_pITableFrame->SendTableData(i,SUB_S_CHANGE_INFO);
							break;
						}
					}
				}
			}

			return true;
		}
	case GER_USER_LEFT:		//�û��뿪
		{
			//ׯ���뿪
			if (wChairID==m_wBankerUser)
			{
				//�������
				LONG lWasteCount=0L;

				//��ע״̬
				if (m_cbSendCard==FALSE)
				{
					//���û���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						m_lGameScore[i]+=m_lUserChipIn[i];
						m_lGameScore[m_wBankerUser]-=m_lUserChipIn[i];
					}
				}

				//����״̬
				if (m_cbSendCard==TRUE)
				{
					//��������
					BYTE cbCardTimes[GAME_PLAYER];
					ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

					//���ͷ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if ((m_cbPlayStatus[i]==TRUE)||(i==wChairID))
						{
							m_GameLogic.SortCardList(m_cbUserCardData[i],MAX_COUNT);
							cbCardTimes[i]=m_GameLogic.GetCardTimes(m_cbUserCardData[i],MAX_COUNT);
						}
					}

					//���û���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//�û�����
						if (i==m_wBankerUser) continue;
						if (m_cbPlayStatus[i]==FALSE) continue;

						//�Ա��˿�
						if (m_GameLogic.CompareCard(m_cbUserCardData[i],m_cbUserCardData[m_wBankerUser],MAX_COUNT)==true)
						{
							m_lGameScore[i]+=cbCardTimes[i]*m_lUserChipIn[i];
							m_lGameScore[m_wBankerUser]-=cbCardTimes[i]*m_lUserChipIn[i];
						}
						else
						{
							lWasteCount+=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
							m_lGameScore[i]-=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
						}
					}
				}

				//ǿ�˷���
				m_lGameScore[m_wBankerUser]+=m_lExitScore;
				m_lExitScore =0;

				//��������
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//��˰����
				LONG lRevenueSum=0L;
				BYTE cbRevenue=m_pGameServiceOption->wRevenue;

				//���û���
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (m_lGameScore[i]>=100L)
					{
						lRevenueSum+=m_lGameScore[i]*cbRevenue/100L;
						GameEnd.lGameTax[i]=m_lGameScore[i]*cbRevenue/100L;
						GameEnd.lGameScore[i]=m_lGameScore[i]-m_lGameScore[i]*cbRevenue/100L;
					}
					else GameEnd.lGameScore[i]=m_lGameScore[i];
				}

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//�޸Ļ���
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (m_cbPlayStatus[i]==TRUE)
					{
						//�������
						LONG lRevenue=GameEnd.lGameTax[i];
						enScoreKind ScoreKind=(i==wChairID)?enScoreKind_Flee:enScoreKind_Win;

						//��ļ���
						if (i==wChairID) lRevenue+=lWasteCount;

						//д�����
						m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,ScoreKind);
					}
				}

				//������Ϸ
				m_pITableFrame->ConcludeGame();

				//�ȴ�����
				m_cbWaitContinue=TRUE;
				ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));
			}

			//�м��뿪
			if (wChairID!=m_wBankerUser)
			{
				//�������
				//LONG lWasteCount=0L;

				//����״̬
				m_cbPlayStatus[wChairID]=FALSE;

				//��ע״̬
				if (m_cbSendCard==FALSE)
				{
					//lWasteCount+=m_lUserChipIn[wChairID];
					m_lGameScore[wChairID]-=m_lUserChipIn[wChairID];
				}

				//����״̬
				if (m_cbSendCard==TRUE)
				{
					//��ȡ����
					m_GameLogic.SortCardList(m_cbUserCardData[m_wBankerUser],MAX_COUNT);
					BYTE cbCardTimes=m_GameLogic.GetCardTimes(m_cbUserCardData[m_wBankerUser],MAX_COUNT);

					//���û���
					//lWasteCount+=m_lUserChipIn[wChairID]*cbCardTimes;
					m_lGameScore[wChairID]-=m_lUserChipIn[wChairID]*cbCardTimes;
				}

				//ׯ������
				m_lExitScore+=(LONG)(m_lGameScore[wChairID]*(-1));

				//д�����
				m_pITableFrame->WriteUserScore(wChairID,m_lGameScore[wChairID],0,enScoreKind_Flee);

				//�����ж�
				if(m_cbSendCard==TRUE)
				{
					//�����ж�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if ((m_cbPlayStatus[i]==TRUE)&&(m_cbUserFinish[i]==FALSE)) break;
					}

					//��������
					if (i==m_wPlayerCount) 
					{
						//���ñ���
						m_cbSendCard=FALSE;

						//������Ϸ
						OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

						return true;
					}
				}

				//�����ж�
				if ((GetTableUserCount()-1)==1)
				{
					//��������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//���ñ���
					GameEnd.lGameScore[wChairID]=m_lGameScore[wChairID];

					//ׯ�ҵ÷�
					if(m_lExitScore>0)
					{
						//����÷�
						BYTE cbRevenue=m_pGameServiceOption->wRevenue;
						if (m_lExitScore>=100L)
						{
							GameEnd.lGameTax[m_wBankerUser] = m_lExitScore*cbRevenue/100L;
							GameEnd.lGameScore[m_wBankerUser] = m_lExitScore-GameEnd.lGameTax[m_wBankerUser];
						}

						//д�����
						m_pITableFrame->WriteUserScore(m_wBankerUser,GameEnd.lGameScore[m_wBankerUser],GameEnd.lGameTax[m_wBankerUser],enScoreKind_Win);
						m_lExitScore = 0;
					}

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//������Ϸ
					m_pITableFrame->ConcludeGame();

					//��ע����
					if (m_wBankerUser!=INVALID_CHAIR)
					{
						//��ȡׯ��
						IServerUserItem * pIBankerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);

						//��������
						if (pIBankerUserItem!=NULL)
						{
							m_lLessScore=m_pGameServiceOption->lCellScore;
							m_lMaxScore=(LONG)(pIBankerUserItem->GetUserScore()->lScore)/37L;
						}
					}

					//�ȴ�����
					m_cbWaitContinue=TRUE;
					ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));

					return true;
				}

				//�����ж�
				if (m_cbSendCard==FALSE)
				{
					//�����ж�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//ׯ�ҹ���
						if (i==m_wBankerUser) continue;

						//��ҹ���
						if ((m_cbPlayStatus[i]==TRUE)&&(m_lUserChipIn[i]==0L)) break;
					}

					//�����˿�
					if (i==m_wPlayerCount)
					{
						//�˿˷���
						if(ChangeCardData()==false)AnalyseCard();

						//���ñ���
						m_cbSendCard=TRUE;

						//�������
						CMD_S_SendCard SendCard;
						ZeroMemory(&SendCard,sizeof(SendCard));

						//�����˿�
						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							if (m_cbPlayStatus[j]==TRUE)
							{
								CopyMemory(SendCard.cbUserCardData[j],m_cbUserCardData[j],sizeof(SendCard.cbUserCardData[i]));
							}
						}

						//������Ϣ
						m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
					}
				}
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
	case GS_TK_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//��Ϸ��Ϣ
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lLessScore=m_lLessScore;
			StatusFree.lMaxScore=GetUserMaxChipIn(wChiarID);

			//��������
			IServerUserItem * pUserInfo=m_pITableFrame->GetServerUserItem(m_wBankerUser);
			tagServerUserData * pServerUserData=pUserInfo->GetUserData();

			//��ׯ�ж�
			if ((pUserInfo->IsAndroidUser()==true))
			{
				StatusFree.bLongBanker=TRUE;
			}

			//��ʼʱ��
			if ((m_cbWaitStart==TRUE)&&(m_dwBankerStartTime!=0L))
			{
				DWORD dwPassTime=(DWORD)time(NULL)-m_dwBankerStartTime;
				StatusFree.wLeaveTime=(WORD)(__max(TIME_BANKER_START,dwPassTime)-dwPassTime);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//ׯ��AI
			IServerUserItem *pUserInfo = m_pITableFrame->GetServerUserItem(m_wBankerUser);
			if(pUserInfo->IsAndroidUser())StatusPlay.bLongBanker=TRUE;

			//��ע��Ϣ
			StatusPlay.lLessScore=m_lLessScore;
			StatusPlay.lMaxScore=GetUserMaxChipIn(wChiarID);
			CopyMemory(StatusPlay.lUserChipIn,m_lUserChipIn,sizeof(StatusPlay.lUserChipIn));

			//��Ϸ��Ϣ
			StatusPlay.wBankerUser=m_wBankerUser;
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

			//�˿���Ϣ
			if (m_cbSendCard==TRUE)
			{
				StatusPlay.cbSendCard=TRUE;
				StatusPlay.cbNeedFinish=(m_cbUserFinish[wChiarID]==FALSE)?TRUE:FALSE;
				CopyMemory(StatusPlay.cbUserCardData,m_cbUserCardData,sizeof(StatusPlay.cbUserCardData));
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

//��Ϸ��Ϣ
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_CONTROL_MAX:
		{
			//��������й���ԱȨ�� �򷵻ش���
			if((pIServerUserItem->GetUserData()->dwUserRight&UR_GAME_CONTROL)==0)
			{
				return false;
			}
			m_wMaxChairID = pIServerUserItem->GetChairID();
			m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("���Ƴɹ�"),SMT_INFO|SMT_EJECT);

			return true;
		}
	case SUB_C_CHANGE_INFO:		//��ׯ��Ϣ
		{
			//��Ϸ״̬
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_FREE);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_FREE) return true;

			//�û�״̬
			ASSERT(pIServerUserItem->GetUserStatus()==US_SIT);
			if (pIServerUserItem->GetUserStatus()!=US_SIT) return true;

			//�û��ж�
			ASSERT(m_wCurrentUser==pIServerUserItem->GetChairID());
			if (m_wCurrentUser!=pIServerUserItem->GetChairID()) return true;

			//��ׯ״̬
			ASSERT((m_cbWaitChange==TRUE)&&(m_cbUserBanker[pIServerUserItem->GetChairID()]==TRUE));
			if ((m_cbWaitChange==FALSE)||(m_cbUserBanker[pIServerUserItem->GetChairID()]==FALSE)) return true;

			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ChangeInfo));
			if (wDataSize!=sizeof(CMD_C_ChangeInfo)) return false;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			CMD_C_ChangeInfo * pChangeInfo=(CMD_C_ChangeInfo *)pDataBuffer;

			//���ñ���
			m_cbUserBanker[wChairID]=FALSE;

			//����ׯ��
			if (pChangeInfo->cbChange==FALSE)
			{
				//Ѱ���û�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//��ȡλ��
					WORD wNextChairID=(m_wCurrentUser+i+1)%GAME_PLAYER;

					//��ׯ�ж�
					if (m_cbUserBanker[wNextChairID]==TRUE)
					{
						m_wCurrentUser=wNextChairID;
						m_pITableFrame->SendTableData(wNextChairID,SUB_S_CHANGE_INFO);
						break;
					}
				}

				//����ж�
				if (i!=m_wPlayerCount) return true;
			}
			else
			{
				//����ׯ��
				SetBankerUser(wChairID);
			}

			//���ñ���
			m_cbWaitChange=FALSE;
			m_wCurrentUser=INVALID_CHAIR;
			ZeroMemory(m_cbUserBanker,sizeof(m_cbUserBanker));

			//״̬�л�
			if ((m_cbWaitStart==FALSE)&&(m_cbWaitContinue==FALSE))
			{
				//���ñ���
				m_cbWaitStart=TRUE;
				m_dwBankerStartTime=(DWORD)time(NULL);

				//�������
				CMD_S_TimerInfo TimerInfo;
				TimerInfo.wLeaveTime=TIME_BANKER_START;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			}

			return true;
		}
	case SUB_C_USER_CHIPIN:		//�û���ע
		{
			//��Ϸ״̬
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;

			//�û�״̬
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAY);
			if (pIServerUserItem->GetUserStatus()!=US_PLAY) return true;

			//��ע״̬
			ASSERT(m_lUserChipIn[pIServerUserItem->GetChairID()]==0L);
			if (m_lUserChipIn[pIServerUserItem->GetChairID()]!=0L) return true;

			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserChipin));
			if (wDataSize!=sizeof(CMD_C_UserChipin)) return false;

			//��Ϣ����
			WORD wChairID=pIServerUserItem->GetChairID();
			CMD_C_UserChipin * pUserChipin=(CMD_C_UserChipin *)pDataBuffer;

			//��������
			LONG lUserChipin=pUserChipin->lScore;
			LONG lUserMaxScore=GetUserMaxChipIn(wChairID);

			//��ע��Ŀ
			ASSERT((lUserChipin>=m_lLessScore)&&(lUserChipin<=lUserMaxScore));
			if ((lUserChipin<m_lLessScore)||(lUserChipin>lUserMaxScore)) return false;

			//���ñ���
			m_lUserChipIn[wChairID]=lUserChipin;

			//��������
			CMD_S_UserChipin UserChipin;
			ZeroMemory(&UserChipin,sizeof(UserChipin));

			//�������
			UserChipin.wChairID=wChairID;
			UserChipin.lUserChipin=lUserChipin;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_CHIPIN,&UserChipin,sizeof(UserChipin));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_CHIPIN,&UserChipin,sizeof(UserChipin));

			//��ע����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//ׯ�ҹ���
				if (i==m_wBankerUser) continue;

				//��ҹ���
				if ((m_cbPlayStatus[i]==TRUE)&&(m_lUserChipIn[i]==0L)) break;
			}

			//�����˿�
			if (i==m_wPlayerCount)
			{
				//�˿˷���
				if(ChangeCardData()==false)AnalyseCard();

				//���ñ���
				m_cbSendCard=TRUE;
				ZeroMemory(m_cbUserFinish,sizeof(m_cbUserFinish));

				//�������
				CMD_S_SendCard SendCard;
				ZeroMemory(&SendCard,sizeof(SendCard));

				//�����˿�
				for (WORD j=0;j<m_wPlayerCount;j++)
				{
					if (m_cbPlayStatus[j]==TRUE)
					{
						CopyMemory(SendCard.cbUserCardData[j],m_cbUserCardData[j],sizeof(SendCard.cbUserCardData[i]));
					}
				}

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
			}

			return true;
		}
	case SUB_C_SEND_FINISH:		//�������
		{
			//��Ϸ״̬
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;

			//�û�״̬
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAY);
			if (pIServerUserItem->GetUserStatus()!=US_PLAY) return true;

			//����״̬
			ASSERT((m_cbSendCard==TRUE)&&(m_cbUserFinish[pIServerUserItem->GetChairID()]==FALSE));
			if ((m_cbSendCard==FALSE)||(m_cbUserFinish[pIServerUserItem->GetChairID()]==TRUE)) return true;

			//���ñ���
			m_cbUserFinish[pIServerUserItem->GetChairID()]=TRUE;

			//�����ж�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_cbPlayStatus[i]==TRUE)&&(m_cbUserFinish[i]==FALSE)) break;
			}

			//��������
			if (i==m_wPlayerCount) 
			{
				//���ñ���
				m_cbSendCard=FALSE;

				//������Ϸ
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
			}

			return true;
		}
	case SUB_C_CONTINUE_GAME:	//������Ϸ
		{
			//��Ϸ״̬
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_FREE);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_FREE) return true;

			//�û�״̬
			ASSERT(pIServerUserItem->GetUserStatus()==US_SIT);
			if (pIServerUserItem->GetUserStatus()!=US_SIT) return true;

			//�ȴ�״̬
			ASSERT((m_cbWaitContinue==TRUE)&&(m_cbUserContinue[pIServerUserItem->GetChairID()]==FALSE));
			if ((m_cbWaitContinue==FALSE)||(m_cbUserContinue[pIServerUserItem->GetChairID()]==TRUE)) return true;

			//���ñ���
			m_cbUserContinue[pIServerUserItem->GetChairID()]=TRUE;

			//����ж�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_cbUserContinue[i]==FALSE)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
				{
					return true;
				}
			}

			//���ñ���
			m_cbWaitContinue=FALSE;

			//��ʼ�ж�
			if ((m_cbWaitChange==FALSE)&&(GetTableUserCount()>=2))
			{
				//Ч��״̬
				ASSERT(m_cbWaitStart==FALSE);

				//���ñ���
				m_cbWaitStart=TRUE;
				m_dwBankerStartTime=(DWORD)time(NULL);

				//��������
				CMD_S_TimerInfo TimerInfo;
				CMD_S_ScoreInfo ScoreInfo;
				ZeroMemory(&TimerInfo,sizeof(TimerInfo));
				ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

				//�������
				TimerInfo.wLeaveTime=TIME_BANKER_START;

				//��ע��Ϣ
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//�������
					ScoreInfo.lLessScore=m_lLessScore;
					ScoreInfo.lMaxScore=GetUserMaxChipIn(i);

					//������Ϣ
					m_pITableFrame->SendTableData(i,SUB_S_SCORE_INFO,&ScoreInfo,sizeof(ScoreInfo));
					m_pITableFrame->SendLookonData(i,SUB_S_SCORE_INFO,&ScoreInfo,sizeof(ScoreInfo));
				}

				//ʱ����Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			}

			return true;
		}
	}

	return false;
}

//�����Ϣ
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//��ʼ��Ϣ
	if (wSubCmdID==SUB_GF_USER_READY)
	{
		//Ч������
		ASSERT(wDataSize==0);
		if (wDataSize!=0) throw TEXT("���ݴ���");

		//״̬Ч��
		ASSERT(m_cbWaitStart==TRUE);
		if (m_cbWaitStart==FALSE) return false;

		//�û�Ч��
		ASSERT(m_wBankerUser==pIServerUserItem->GetChairID());
		if (m_wBankerUser!=pIServerUserItem->GetChairID()) return false;

		//״̬Ч��
		ASSERT(pIServerUserItem->GetUserStatus()==US_SIT);
		if (pIServerUserItem->GetUserStatus()!=US_SIT) return false;

		//���ñ���
		m_cbWaitStart=FALSE;
		m_dwBankerStartTime=0L;

		//��ʼ��Ϸ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=m_wBankerUser)
			{
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pIServerUserItem!=NULL) pIServerUserItem->GetUserData()->cbUserStatus=US_READY;
			}
		}

		return false;
	}

	return false;
}

////��������
//bool __cdecl CTableFrameSink::OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon)
//{
//	//��Ҵ���
//	if (bReqLookon==false)
//	{
//		//ׯ���ж�
//		if (m_wBankerUser==INVALID_CHAIR)
//		{
//			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
//			if (pUserData->UserScoreInfo.lScore<(m_pGameServiceOption->lRestrictScore)) return false;
//		}
//	}
//
//	return true;
//}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if(m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && (m_lUserChipIn[wChairID]>0L || m_wBankerUser==wChairID) && !m_bOffLineGameEnd)m_bOffLineGameEnd = true;

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	if(m_bOffLineGameEnd)
	{
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			IServerUserItem * pIUserItem=m_pITableFrame->GetServerUserItem(i);
			if(pIUserItem==NULL)continue;
			if(pIUserItem->GetUserData()->cbUserStatus==US_OFFLINE)break;
		}

		if(i==GAME_PLAYER)m_bOffLineGameEnd = false;
	}
	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��Ҵ���
	if (bLookonUser==false)
	{
		//����ׯ��
		if (m_wBankerUser==INVALID_CHAIR)
		{
			//Ч��״̬
			ASSERT(m_cbWaitStart==FALSE);
			ASSERT(m_cbWaitChange==FALSE);
			ASSERT(m_cbWaitContinue==FALSE);//?

			//����ׯ��
			SetBankerUser(wChairID);

			return true;
		}

		//״̬����
		if (m_cbWaitContinue==TRUE) m_cbUserContinue[wChairID]=TRUE;

		//״̬�л�
		if ((m_cbWaitStart==FALSE)&&(m_cbWaitContinue==FALSE)&&(m_cbWaitChange==FALSE))
		{
			//���ñ���
			m_cbWaitStart=TRUE;
			m_dwBankerStartTime=(DWORD)time(NULL);

			//�������
			CMD_S_TimerInfo TimerInfo;
			TimerInfo.wLeaveTime=TIME_BANKER_START;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
		}
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��Ҵ���
	if (bLookonUser==false)
	{
		//ׯ���뿪
		if (wChairID==m_wBankerUser)
		{
			//���ñ���
			m_lMaxScore=0L;
			m_lLessScore=0L;
			m_wBankerUser=INVALID_CHAIR;

			//����ׯ��
			if (SearceBankerUser()==false) 
			{
				m_cbWaitContinue=FALSE;
				return true;
			}

			//�ȴ�����
			if (m_cbWaitStart==TRUE)
			{
				m_cbWaitStart=FALSE;
				m_dwBankerStartTime=0L;
			}

			//��ׯ״̬
			if ((m_cbWaitChange==TRUE)&&(m_wBankerUser!=INVALID_CHAIR))
			{
				//���ñ���
				m_cbUserBanker[m_wBankerUser]=FALSE;

				//����ж�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if (m_cbUserBanker[i]==TRUE) break;
				}

				//��ɴ���
				if (i==m_wPlayerCount)
				{
					//���ñ���
					m_cbWaitChange=FALSE;
					m_wCurrentUser=INVALID_CHAIR;
					ZeroMemory(m_cbUserBanker,sizeof(m_cbUserBanker));
				}
			}
		}

		//��ׯ״̬
		if (m_cbWaitChange==TRUE)
		{
			//���ñ���
			m_cbUserBanker[wChairID]=FALSE;

			//��ǰ�û�
			if (wChairID==m_wCurrentUser)
			{
				//Ѱ���û�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//��ȡλ��
					WORD wNextChairID=(m_wCurrentUser+i+1)%GAME_PLAYER;

					//��ׯ�ж�
					if (m_cbUserBanker[wNextChairID]==TRUE)
					{
						m_wCurrentUser=wNextChairID;
						m_pITableFrame->SendTableData(m_wCurrentUser,SUB_S_CHANGE_INFO);
						break;
					}
				}

				//����ж�
				if (i!=m_wPlayerCount) return true;
			}

			//���ñ���
			m_cbWaitChange=FALSE;
			m_wCurrentUser=INVALID_CHAIR;
			ZeroMemory(m_cbUserBanker,sizeof(m_cbUserBanker));
		}

		//�ȴ�����
		if (m_cbWaitContinue==TRUE)
		{
			//�û�����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_cbUserContinue[i]==FALSE)&&(m_pITableFrame->GetServerUserItem(i))!=NULL)
				{
					return true;
				}
			}

			//����ж�
			m_cbWaitContinue=FALSE;
			ZeroMemory(m_cbUserContinue,sizeof(m_cbUserContinue));
		}

		//ֹͣʱ��
		WORD wAddCount = ((m_pITableFrame->GetServerUserItem(wChairID)!=NULL)?0:1);
		if ((m_cbWaitStart==TRUE)&&((GetTableUserCount()+wAddCount)<=1))
		{
			//���ñ���
			m_cbWaitStart=FALSE;
			m_dwBankerStartTime=0L;

			//�������
			CMD_S_TimerInfo TimerInfo;
			ZeroMemory(&TimerInfo,sizeof(TimerInfo));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));

			return true;
		}

		//��ȡ����
		WORD wUserCount=GetTableUserCount()+wAddCount;
		BYTE cbGameStatus=m_pITableFrame->GetGameStatus();

		//��ʼʱ��
		if ((m_cbWaitStart==FALSE)&&(m_cbWaitContinue==FALSE)&&(m_cbWaitChange==FALSE)&&(cbGameStatus==GS_FREE)&&(wUserCount>=2))
		{
			//��Ϸ����
			if(m_bOffLineGameEnd)
			{
				m_bOffLineGameEnd = false;
				return true;
			}

			//���ñ���
			m_cbWaitStart=TRUE;
			m_dwBankerStartTime=(DWORD)time(NULL);

			//�������
			CMD_S_TimerInfo TimerInfo;
			ZeroMemory(&TimerInfo,sizeof(TimerInfo));

			//�������
			TimerInfo.wLeaveTime=TIME_BANKER_START;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TIMER_INFO,&TimerInfo,sizeof(TimerInfo));

			return true;
		}
	}

	return true;
}

//����ׯ��
void CTableFrameSink::SetBankerUser(WORD wBankerUser)
{
	//��ͬ�ж�
	ASSERT(m_wBankerUser!=wBankerUser);
	if (m_wBankerUser==wBankerUser) return;

	//��Ϸ����
	m_lMaxScore=0L;
	m_lLessScore=0L;
	m_wBankerUser=wBankerUser;

	//�����ж�
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//�û���Ŀ
		IServerUserItem * pIBankerUserItem=m_pITableFrame->GetServerUserItem(m_wBankerUser);
		tagServerUserData * pServerUserData=pIBankerUserItem->GetUserData();

		//��������
		m_lLessScore=m_pGameServiceOption->lCellScore;
		m_lMaxScore=(LONG)(pIBankerUserItem->GetUserScore()->lScore)/37L;

		//�������
		CMD_S_BankerUser BankerUser;
		ZeroMemory(&BankerUser,sizeof(BankerUser));

		//��ׯ�ж�
		if ((pIBankerUserItem->IsAndroidUser()==true))
		{
			BankerUser.bLongBanker=TRUE;
		}

		//������Ϣ
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�������
			BankerUser.lLessScore=m_lLessScore;
			BankerUser.wBankerUser=m_wBankerUser;
			BankerUser.lMaxScore=GetUserMaxChipIn(i);

			//������Ϣ
			m_pITableFrame->SendTableData(i,SUB_S_BANKER_USER,&BankerUser,sizeof(BankerUser));
			m_pITableFrame->SendLookonData(i,SUB_S_BANKER_USER,&BankerUser,sizeof(BankerUser));
		}
	}

	return;
}

//ׯ�Ҽ��
bool CTableFrameSink::IsSuitBankerUser(WORD wChairID)
{
	//��ȡ�û�
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (pIServerUserItem==NULL) return false;

	//���ּ��
	tagServerUserData * pServerUserData=pIServerUserItem->GetUserData();
	if (pServerUserData->UserScoreInfo.lScore<(m_pGameServiceOption->lRestrictScore)) return false;

	return true;
}

//Ѱ��ׯ��
bool CTableFrameSink::SearceBankerUser()
{
	//Ѱ��ׯ��
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (IsSuitBankerUser(i)==true)
		{
			SetBankerUser(i);
			return true;
		}
	}

	//�����û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//������Ϣ
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("����û��������㹻�����ö��ׯ�ң���Ϸ��ɢ��"),SMT_CLOSE_GAME|SMT_EJECT);
		}
		else
		{
			ITableFrameManager * pITableFrameManager=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrame,ITableFrameManager);
			pITableFrameManager->PerformStandUpAction(pIServerUserItem);
		}
	}

	return false;
}

//�˿˷���
void CTableFrameSink::AnalyseCard()
{
	//�˿˷���
	for (WORD wUserIndex=0;wUserIndex<m_wPlayerCount;wUserIndex++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wUserIndex);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsAndroidUser()==false)) continue;

		//��������
		LONG lAndroidScore=0;

		//��������
		BYTE cbCardTimes[GAME_PLAYER];
		ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

		//�˿˱���
		BYTE cbUserCardData[GAME_PLAYER][3];
		CopyMemory(cbUserCardData,m_cbUserCardData,sizeof(m_cbUserCardData));

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
		}

		//���ұ���
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				cbCardTimes[i]=m_GameLogic.GetCardTimes(cbUserCardData[i],MAX_COUNT);
			}
		}

		//�Ա��˿�
		if (wUserIndex==m_wBankerUser)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�û�����
				if ((i==m_wBankerUser)||(m_cbPlayStatus[i]==FALSE)) continue;

				//�Ա��˿�
				if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT)==true)
				{
					lAndroidScore-=cbCardTimes[i]*m_lUserChipIn[i];
				}
				else
				{
					lAndroidScore+=cbCardTimes[m_wBankerUser]*m_lUserChipIn[i];
				}
			}
		}
		else
		{
			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[m_wBankerUser],cbUserCardData[wUserIndex],MAX_COUNT)==true)
			{
				lAndroidScore-=cbCardTimes[m_wBankerUser]*m_lUserChipIn[wUserIndex];
			}
			else
			{
				lAndroidScore+=cbCardTimes[wUserIndex]*m_lUserChipIn[wUserIndex];
			}
		}

		//����ж�
		if ((m_dStockScore+lAndroidScore)<0.0)
		{
			//��������
			WORD wWinUser=INVALID_CHAIR;

			//����û�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�û�����
				if (m_cbPlayStatus[i]==FALSE) continue;

				//�����û�
				if (wWinUser==INVALID_CHAIR)
				{
					wWinUser=i;
					continue;
				}

				//�Ա��˿�
				if (m_GameLogic.CompareCard(cbUserCardData[i],m_cbUserCardData[wWinUser],MAX_COUNT)==true)
				{
					wWinUser=i;
				}
			}

			//��������
			BYTE cbTempData[MAX_COUNT];
			CopyMemory(cbTempData,m_cbUserCardData[wUserIndex],MAX_COUNT);
			CopyMemory(m_cbUserCardData[wUserIndex],m_cbUserCardData[wWinUser],MAX_COUNT);
			CopyMemory(m_cbUserCardData[wWinUser],cbTempData,MAX_COUNT);
		}

		break;
	}

	return;
}

//�û���Ŀ
WORD CTableFrameSink::GetTableUserCount()
{
	//��������
	WORD wUserCount=0;

	//�û�ͳ��
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_pITableFrame->GetServerUserItem(i)!=NULL) 
		{
			wUserCount++;
		}
	}

	return wUserCount;
}

//�����Ŀ
LONG CTableFrameSink::GetUserMaxChipIn(WORD wChairID)
{
	//��ȡ�û�
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

	//�û��ж�
	if (pIServerUserItem==NULL) return 0L;

	return __min(m_lMaxScore,(LONG)(pIServerUserItem->GetUserScore()->lScore)/9);
}

//��������
bool CTableFrameSink::ChangeCardData()
{
	////�û�����
	//BYTE bUserCount =0;
	//for(WORD i=0;i<GAME_PLAYER;i++) if(m_cbPlayStatus[i]==TRUE) bUserCount++;

	////�����û�
	//for(WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	//��������
	//	INT nType = m_pITableFrame->GetUserType(i);
	//	if(nType==enUserType_MustLostUser || nType==enUserType_MustWinUser)break;
	//}
	//if(i==GAME_PLAYER)return false;

	////�û���Ϣ
	//INT nUserWinTag[GAME_PLAYER];
	//ZeroMemory(nUserWinTag,sizeof(nUserWinTag));

	////ȷ����Ӯ
	//WORD wWinUser = INVALID_CHAIR,wLoseUser = INVALID_CHAIR;
	//WORD wWinCount = 0,wLoseCount = 0;
	//for(WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	if(m_cbPlayStatus[i]!=TRUE)continue;
	//	nUserWinTag[i] = m_pITableFrame->GetUserType(i);
	//	if(nUserWinTag[i] == enUserType_MustWinUser)
	//	{
	//		if(wWinUser == INVALID_CHAIR)wWinUser = i;
	//		wWinCount++;
	//	}
	//	else if(nUserWinTag[i] == enUserType_MustLostUser)
	//	{
	//		if(wLoseUser == INVALID_CHAIR)wLoseUser = i;
	//		wLoseCount++;
	//	}
	//}
	//if(wWinCount==bUserCount || wLoseCount==bUserCount)return true;

	////��������
	//bool bReturnValue = false;
	//if(wWinUser < GAME_PLAYER)
	//{
	//	for(WORD i=0;i<GAME_PLAYER;i++)
	//	{
	//		if(m_cbPlayStatus[i]!=TRUE || i==wWinUser)continue;
	//		if(m_GameLogic.CompareCard(m_cbUserCardData[i],m_cbUserCardData[wWinUser],MAX_COUNT)==TRUE)
	//		{
	//			BYTE bTempData[MAX_COUNT];
	//			CopyMemory(bTempData,m_cbUserCardData[i],sizeof(bTempData));
	//			CopyMemory(m_cbUserCardData[i],m_cbUserCardData[wWinUser],sizeof(bTempData));
	//			CopyMemory(m_cbUserCardData[wWinUser],bTempData,sizeof(bTempData));
	//		}
	//	}
	//	bReturnValue = true;
	//}

	////��������
	//if(wLoseUser < GAME_PLAYER)
	//{
	//	for(WORD i=0;i<GAME_PLAYER;i++)
	//	{
	//		if(m_cbPlayStatus[i]!=TRUE || i==wLoseUser)continue;
	//		if(m_GameLogic.CompareCard(m_cbUserCardData[i],m_cbUserCardData[wLoseUser],MAX_COUNT)==FALSE)
	//		{
	//			BYTE bTempData[MAX_COUNT];
	//			CopyMemory(bTempData,m_cbUserCardData[i],sizeof(bTempData));
	//			CopyMemory(m_cbUserCardData[i],m_cbUserCardData[wLoseUser],sizeof(bTempData));
	//			CopyMemory(m_cbUserCardData[wLoseUser],bTempData,sizeof(bTempData));
	//		}
	//	}
	//	bReturnValue = true;
	//}

	return false;
}

//////////////////////////////////////////////////////////////////////////
