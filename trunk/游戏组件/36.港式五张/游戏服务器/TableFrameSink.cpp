#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

#define IDI_GAME_END				1										//������ʶ
#define IDI_PASS_CARD				2										//������ʶ

//////////////////////////////////////////////////////////////////////////

//��̬����
__int64				CTableFrameSink::g_lMaxScore=100000000L;				//���仯
__int64				CTableFrameSink::g_lMaxWinScore=100000000L;				//���Ӯ��
__int64				CTableFrameSink::g_lMaxLoseScore=-100000000L;			//������
CBlacklistIDMap		CTableFrameSink::g_BlacklistIDMap;						//�����û�
CScoreHistoryMap	CTableFrameSink::g_ScoreHistoryMap;						//��ʷ��¼

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wOperaCount=0;
	m_bShowHand=false;
	m_bChaosCard=true;
	m_wCurrentUser=INVALID_CHAIR;
	m_wLastMostUser = INVALID_CHAIR;

	//�û�״̬
	m_lServiceScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//�˿˱���
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;

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
	m_pITableFrame=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrame);
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
	m_wOperaCount=0;
	m_bShowHand=false;
	m_bChaosCard=true;
	m_wCurrentUser=INVALID_CHAIR;
	m_wLastMostUser = INVALID_CHAIR;

	//�û�״̬
	m_lServiceScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//�˿˱���
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;

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
	return (m_cbPlayStatus[wChairID]==TRUE)?true:false;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	//�������
	LONG lBaseScore=0L;

	//�������
	//for (WORD i=0;i<m_wPlayerCount;i++)
	//{
	//	//��ȡ�û�
	//	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
	//	if (pIServerUserItem==NULL) continue;

	//	const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();
	//	if (lBaseScore==0L)
	//	{
	//		lBaseScore=pUserScore->lScore;
	//	}
	//	else lBaseScore=__min(pUserScore->lScore,lBaseScore);
	//}

	////�۳�����
	//if ((lBaseScore>=20L)&&(lBaseScore<10000L))
	//{
	//	m_lServiceScore=10L;
	//}
	//else if ((lBaseScore>=10000L)&&(lBaseScore<100000L))
	//{
	//	m_lServiceScore=30L;
	//}
	//else if ((lBaseScore>=100000L)&&(lBaseScore<300000L))
	//{
	//	m_lServiceScore=50L;
	//}
	//else if ((lBaseScore>=300000L)&&(lBaseScore<600000L))
	//{
	//	m_lServiceScore=80L;
	//}
	//else if ((lBaseScore>=600000L)&&(lBaseScore<1000000L))
	//{
	//	m_lServiceScore=120L;
	//}
	//else if (lBaseScore>=1000000L)
	//{
	//	m_lServiceScore=500L;
	//}

	////�������
	//if (m_lServiceScore>0L)
	//{
	//	for (WORD i=0;i<m_wPlayerCount;i++)
	//	{
	//		//��ȡ�û�
	//		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
	//		if (pIServerUserItem==NULL) continue;

	//		//������Ϣ
	//		tagScoreInfo ScoreInfo;
	//		ScoreInfo.lScore=-m_lServiceScore;
	//		ScoreInfo.ScoreKind=enScoreKind_Service;

	//		//д�����
	//		m_pITableFrame->WriteUserScore(i,ScoreInfo);

	//		//������Ϣ
	//		TCHAR szMess[512]=TEXT("");
	//		_sntprintf(szMess,sizeof(szMess),TEXT("ϵͳ�۳� %ld ����Ϸ�����"),m_lServiceScore);
	//		m_pITableFrame->SendGameMessage(pIServerUserItem,szMess,SMT_INFO);
	//	}
	//}

	//�����ע
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//��ȡ����
		const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();
		ASSERT(pUserScore->lScore>=m_pGameServiceOption->lCellScore);

		//���ñ���
		m_cbPlayStatus[i]=TRUE;
		m_lUserMaxScore[i]=pUserScore->lScore;
		if ((m_lMaxScore==0L)||(pUserScore->lScore<m_lMaxScore)) m_lMaxScore=pUserScore->lScore;
	}

	//��ע����
	m_lTurnMaxScore=m_lMaxScore/2L;
	m_lCellScore=__max(m_lMaxScore/100L,m_pGameServiceOption->lCellScore);
	m_lTurnLessScore=__max(m_lMaxScore/100L,m_pGameServiceOption->lCellScore);

	//�ַ��˿�
	m_cbSendCardCount=2;
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

	//�û�����
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_lTableScore[2*i+1]=m_lCellScore;
			m_cbCardCount[i]=m_cbSendCardCount;
		}
	}

	//�����û�
	m_wOperaCount=0;
	m_wCurrentUser=EstimateWinner(1,1);
	m_wLastMostUser = m_wCurrentUser;

	//��������
	CMD_S_GameStart GameStart;
	GameStart.lMaxScore=m_lMaxScore;
	GameStart.lCellScore=m_lCellScore;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.lTurnMaxScore=m_lTurnMaxScore;
	GameStart.lTurnLessScore=m_lTurnLessScore;

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==FALSE) GameStart.cbCardData[i]=0;
		else GameStart.cbCardData[i]=m_cbHandCardData[i][1];
	}

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		GameStart.cbObscureCard=0;
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart.cbObscureCard=m_cbHandCardData[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		GameStart.cbObscureCard=0;
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	//����ʱ��
	m_pITableFrame->SetGameTimer(IDI_PASS_CARD,40000L,1,m_wCurrentUser);

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
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������ע
			LONG lAllScore=0L;
			for (WORD i=0;i<CountArray(m_lTableScore);i++) lAllScore+=m_lTableScore[i];

			//��������
			WORD wWinerUser=EstimateWinner(0,4);
			LONG lMaxLostScore=m_lTableScore[wWinerUser*2]+m_lTableScore[wWinerUser*2+1];

			//ͳ����Ϣ
			LONG lWinnerScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//������Ϣ
				if ((cbReason==GER_NO_PLAYER)||(m_cbPlayStatus[i]==FALSE)) GameEnd.cbCardData[i]=0;
				else GameEnd.cbCardData[i]=m_cbHandCardData[i][0];

				//������Ϣ
				if (i!=wWinerUser)
				{
					LONG lLostScore=m_lTableScore[i*2]+m_lTableScore[i*2+1];
					if (m_cbPlayStatus[i]==TRUE)
					{
						lWinnerScore+=__min(lLostScore,lMaxLostScore);
						GameEnd.lGameScore[i]=-__min(lLostScore,lMaxLostScore);
					}
					else
					{
						lWinnerScore+=__min(lLostScore,m_lTableScore[wWinerUser*2]+m_lTableScore[wWinerUser*2+1]);
						GameEnd.lGameScore[i]=-lLostScore;
					}
				}
			}

			//ʤ�ߵ÷�
			GameEnd.lGameScore[wWinerUser]=lWinnerScore;
			//��˰
			LONG lRevenue = 0L;
			if( m_pGameServiceOption->wServerType == GAME_GENRE_GOLD )
			{
				if( GameEnd.lGameScore[wWinerUser] >= 100L )
				{
					lRevenue = GameEnd.lGameScore[wWinerUser]*m_pGameServiceOption->cbRevenue/100L;
					GameEnd.lGameScore[wWinerUser] -= lRevenue;
					GameEnd.lGameTax[wWinerUser] = lRevenue;
				}
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if( m_cbPlayStatus[i] == TRUE )
				{
					enScoreKind ScoreKind;
					if( GameEnd.lGameScore[i] == 0 ) ScoreKind = enScoreKind_Draw;
					else if( GameEnd.lGameScore[i] > 0 ) ScoreKind = enScoreKind_Win;
					else ScoreKind = enScoreKind_Lost;
					m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],ScoreKind);
				}
			}
			
			//�ɼ���¼
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					//��ȡ�û�
					IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
					if (pIServerUserItem==NULL) continue;
 
					//�����û�
					tagScoreHistory * pScoreHistory=NULL;
					g_ScoreHistoryMap.Lookup(pIServerUserItem->GetUserID(),pScoreHistory);

					//��ȡʱ��
					SYSTEMTIME SystemTime;
					GetLocalTime(&SystemTime);

					//�����û�
					if (pScoreHistory==NULL)
					{
						//��������
						pScoreHistory=new tagScoreHistory;
						g_ScoreHistoryMap[pIServerUserItem->GetUserID()]=pScoreHistory;

						//���ñ���
						pScoreHistory->lScore=0L;
						pScoreHistory->lWinScore=0L;
						pScoreHistory->lLoseScore=0L;
						pScoreHistory->RecordTime=SystemTime;
					}

					//��ԭ�ж�
					if (pScoreHistory->RecordTime.wDay!=SystemTime.wDay)
					{
						pScoreHistory->lScore=0L;
						pScoreHistory->lWinScore=0L;
						pScoreHistory->lLoseScore=0L;
						pScoreHistory->RecordTime=SystemTime;
					}

					//��¼����
					pScoreHistory->lScore+=GameEnd.lGameScore[i];
					if (GameEnd.lGameScore[i]>0L) pScoreHistory->lWinScore+=GameEnd.lGameScore[i];
					if (GameEnd.lGameScore[i]<0L) pScoreHistory->lLoseScore+=GameEnd.lGameScore[i];
				}
			}

			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_GAME_END);
			m_pITableFrame->KillGameTimer(IDI_PASS_CARD);

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
			m_cbCardCount[wChairID]=0;
			m_cbPlayStatus[wChairID]=FALSE;

			//������Ϣ
			CMD_S_GiveUp GiveUp;
			GiveUp.wGiveUpUser=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

			//������Ϣ
			LONG lScore=-m_lTableScore[wChairID*2]-m_lTableScore[wChairID*2+1];

			//д�����
			m_pITableFrame->WriteUserScore(wChairID,lScore,0,enScoreKind_Flee);

			//���ּ�¼
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
			if (pIServerUserItem!=NULL)
			{
				//�����û�
				tagScoreHistory * pScoreHistory=NULL;
				g_ScoreHistoryMap.Lookup(pIServerUserItem->GetUserID(),pScoreHistory);

				//��ȡʱ��
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime);

				//�����û�
				if (pScoreHistory==NULL)
				{
					//��������
					pScoreHistory=new tagScoreHistory;
					g_ScoreHistoryMap[pIServerUserItem->GetUserID()]=pScoreHistory;

					//���ñ���
					pScoreHistory->lScore=0L;
					pScoreHistory->lWinScore=0L;
					pScoreHistory->lLoseScore=0L;
					pScoreHistory->RecordTime=SystemTime;
				}

				//��ԭ�ж�
				if (pScoreHistory->RecordTime.wDay!=SystemTime.wDay)
				{
					pScoreHistory->lScore=0L;
					pScoreHistory->lWinScore=0L;
					pScoreHistory->lLoseScore=0L;
					pScoreHistory->RecordTime=SystemTime;
				}

				//��¼����
				pScoreHistory->lScore+=lScore;
				pScoreHistory->lLoseScore+=lScore;
			}

			//����ͳ��
			WORD wPlayerCount=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
			}

			//�жϽ���
			if (wPlayerCount>=2)
			{
				if (m_wCurrentUser==wChairID) OnUserAddScore(wChairID,0L,true);
			}
			else
			{
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NO_PLAYER);
			}

			return true;
		}
	case GER_DISMISS:
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_GAME_END);
			m_pITableFrame->KillGameTimer(IDI_PASS_CARD);

			//������Ϸ
			m_pITableFrame->ConcludeGame();

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
	case GS_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��־����
			StatusPlay.bAddScore=m_cbOperaScore[wChiarID];
			StatusPlay.bShowHand=(m_bShowHand==true)?TRUE:FALSE;

			//��ע��Ϣ
			StatusPlay.lMaxScore=m_lMaxScore;
			StatusPlay.lCellScore=m_lCellScore;
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore;
			StatusPlay.lTurnLessScore=m_lTurnLessScore;

			//���ñ���
			StatusPlay.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(m_lTableScore));
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					StatusPlay.cbCardCount[i]=m_cbCardCount[i];
					if ((i==wChiarID)&&(pIServerUserItem->GetUserStatus()!=US_LOOKON)) StatusPlay.cbHandCardData[i][0]=m_cbHandCardData[i][0];
					else StatusPlay.cbHandCardData[i][0]=0;
					CopyMemory(&StatusPlay.cbHandCardData[i][1],&m_cbHandCardData[i][1],(m_cbCardCount[i]-1)*sizeof(BYTE));
				}
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
	switch (wTimerID)
	{
	case IDI_GAME_END:	//��Ϸ����
		{
			if (m_pITableFrame->GetGameStatus()==GS_PLAYING)
			{
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
				return true;
			}
		}
	case IDI_PASS_CARD:	//������ע
		{
			if ((wBindParam==m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentUser);
				
				//�û��ж�
				if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserStatus()!=US_OFFLINE))
				{
					OnUserGiveUp(m_wCurrentUser);
				}
				else m_pITableFrame->SetGameTimer(IDI_PASS_CARD,40000L,1,m_wCurrentUser);

				//test
				//m_pITableFrame->SetGameTimer(IDI_PASS_CARD,40000L,1,m_wCurrentUser);
				return true;
			}
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
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
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore,false);
		}
	case SUB_C_GIVE_UP:				//�û�����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			return OnUserGiveUp(pUserData->wChairID);
		}
	case SUB_C_GET_WINNER:			//��ȡʤ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_GetWinner));
			if (wDataSize!=sizeof(CMD_C_GetWinner)) return false;

			//��������
			CMD_C_GetWinner * pGetWiner=(CMD_C_GetWinner *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			return OnUserGetWinner(pIServerUserItem);
		}
	case SUB_C_GET_OPTION:			//��ȡ��Ϣ
		{
			//�������
			CMD_S_ScoreOption ScoreOption;
			ScoreOption.lMaxScore=g_lMaxScore;
			ScoreOption.lMaxWinScore=g_lMaxWinScore;
			ScoreOption.lMaxLoseScore=g_lMaxLoseScore;
			ScoreOption.lUserCount=(LONG)g_ScoreHistoryMap.GetCount();
			ScoreOption.lBlackCount=(LONG)g_BlacklistIDMap.GetCount();

			//��������
			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SCORE_OPTION,&ScoreOption,sizeof(ScoreOption));

			return true;
		}
	case SUB_C_SET_MAX_SCORE:		//���û���
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ScoreInfo));
			if (wDataSize!=sizeof(CMD_C_ScoreInfo)) return false;

			//���ñ���
			g_lMaxScore=((CMD_C_ScoreInfo *)pDataBuffer)->lScore;
			g_lMaxScore=__max(g_lMaxScore,0L);

			return true;
		}
	case SUB_C_SET_MAX_WIN_SCORE:	//���û���
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ScoreInfo));
			if (wDataSize!=sizeof(CMD_C_ScoreInfo)) return false;

			//���ñ���
			g_lMaxWinScore=((CMD_C_ScoreInfo *)pDataBuffer)->lScore;
			g_lMaxWinScore=__max(g_lMaxWinScore,0L);

			return true;
		}
	case SUB_C_SET_MAX_LOSE_SCORE:	//���û���
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ScoreInfo));
			if (wDataSize!=sizeof(CMD_C_ScoreInfo)) return false;

			//���ñ���
			g_lMaxLoseScore=((CMD_C_ScoreInfo *)pDataBuffer)->lScore;
			g_lMaxLoseScore=__min(g_lMaxLoseScore,0L);

			return true;
		}
	case SUB_C_SET_BLACKLIST:		//�����û�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserIDInfo));
			if (wDataSize!=sizeof(CMD_C_UserIDInfo)) return false;

			//���ñ���
			DWORD dwUserID=((CMD_C_UserIDInfo *)pDataBuffer)->dwUserID;
			g_BlacklistIDMap[dwUserID]=dwUserID;

			return true;
		}
	case SUB_C_REMOVE_BLACKLIST:	//�����û�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserIDInfo));
			if (wDataSize!=sizeof(CMD_C_UserIDInfo)) return false;

			//���ñ���
			g_BlacklistIDMap.RemoveKey(((CMD_C_UserIDInfo *)pDataBuffer)->dwUserID);

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
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (wChairID==m_wCurrentUser)
	{
		m_pITableFrame->KillGameTimer(IDI_PASS_CARD);
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (wChairID==m_wCurrentUser)
	{
		m_pITableFrame->SetGameTimer(IDI_PASS_CARD,40000L,1,m_wCurrentUser);
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	//��������
	m_cbCardCount[wChairID]=0;
	m_cbPlayStatus[wChairID]=FALSE;

	//������Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wGiveUpUser=wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//������Ϣ
	LONG lScore = -m_lTableScore[wChairID*2]-m_lTableScore[wChairID*2+1];

	//д�����
	m_pITableFrame->WriteUserScore(wChairID,lScore,0,enScoreKind_Lost);

	//���ּ�¼
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (pIServerUserItem!=NULL)
	{
		//�����û�
		tagScoreHistory * pScoreHistory=NULL;
		g_ScoreHistoryMap.Lookup(pIServerUserItem->GetUserID(),pScoreHistory);

		//��ȡʱ��
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		//�����û�
		if (pScoreHistory==NULL)
		{
			//��������
			pScoreHistory=new tagScoreHistory;
			g_ScoreHistoryMap[pIServerUserItem->GetUserID()]=pScoreHistory;

			//���ñ���
			pScoreHistory->lScore=0L;
			pScoreHistory->lWinScore=0L;
			pScoreHistory->lLoseScore=0L;
			pScoreHistory->RecordTime=SystemTime;
		}

		//��ԭ�ж�
		if (pScoreHistory->RecordTime.wDay!=SystemTime.wDay)
		{
			pScoreHistory->lScore=0L;
			pScoreHistory->lWinScore=0L;
			pScoreHistory->lLoseScore=0L;
			pScoreHistory->RecordTime=SystemTime;
		}

		//��¼����
		pScoreHistory->lScore+=lScore;
		pScoreHistory->lLoseScore+=lScore;
	}

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//�жϽ���
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddScore(wChairID,0L,true);
	}
	else
	{
		OnEventGameEnd(INVALID_CHAIR,NULL,GER_NO_PLAYER);
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONG lScore, bool bGiveUp)
{
	//��������
	if (bGiveUp==false)
	{
		//״̬Ч��
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		//���Ч��
		ASSERT((lScore>=0)&&(lScore>=m_lTableScore[wChairID*2]));
		ASSERT((lScore+m_lTableScore[wChairID*2+1])<=m_lUserMaxScore[wChairID]);
		if ((lScore<0)||(lScore<m_lTableScore[wChairID*2])) return false;
		if ((lScore+m_lTableScore[wChairID*2+1])>m_lUserMaxScore[wChairID]) return false;

		//��ע����
		LONG lUserMaxScore=0L;
		LONG lUserLessScore=0L;
		if (m_bShowHand==false)
		{
			ASSERT(m_lTurnMaxScore>=m_lTableScore[wChairID*2+1]);
			ASSERT(m_lTurnLessScore>=m_lTableScore[wChairID*2+1]);
			lUserMaxScore=m_lTurnMaxScore-m_lTableScore[wChairID*2+1];
			lUserLessScore=m_lTurnLessScore-m_lTableScore[wChairID*2+1];
		}
		else
		{
			ASSERT(m_lUserMaxScore[wChairID]>=m_lTableScore[wChairID*2+1]);
			lUserMaxScore=m_lUserMaxScore[wChairID]-m_lTableScore[wChairID*2+1];
			lUserLessScore=m_lUserMaxScore[wChairID]-m_lTableScore[wChairID*2+1];
		}

		//����ж�
		bool bUserShowHand=((lScore+m_lTableScore[wChairID*2+1])==m_lUserMaxScore[wChairID]);

		//��עЧ��
		ASSERT((lScore==lUserLessScore)||(lScore>=lUserMaxScore)||(m_cbOperaScore[wChairID]==FALSE));
		if ((lScore>lUserLessScore)&&(lScore<lUserMaxScore)&&(m_cbOperaScore[wChairID]==TRUE)) return false;

		//����ж�
		if ((m_bShowHand==true)||((bUserShowHand==true)&&(m_lMaxScore==m_lTurnMaxScore)))
		{
			//���ý��
			ASSERT((lScore+m_lTableScore[wChairID*2+1])==m_lUserMaxScore[wChairID]);
			m_lTableScore[wChairID*2]=m_lUserMaxScore[wChairID]-m_lTableScore[wChairID*2+1];
		}
		else
		{
			//���ý��
			ASSERT((lScore>=lUserLessScore)&&(lScore<=lUserMaxScore));
			m_lTableScore[wChairID*2]=__min(lUserMaxScore,__max(lScore,lUserLessScore));
		}

		//���ñ���
		m_cbOperaScore[wChairID]=TRUE;
		m_bShowHand=((m_bShowHand==true)||(bUserShowHand==true));
		m_lTurnLessScore=__max(m_lTableScore[wChairID*2]+m_lTableScore[wChairID*2+1],m_lTurnLessScore);
	}

	//�û��л�
	WORD wNextPlayer=INVALID_CHAIR;
	for (WORD i=0;i<m_wPlayerCount-1;i++)
	{
		//���ñ���
		m_wOperaCount++;
		wNextPlayer=(m_wCurrentUser+i+1)%m_wPlayerCount;

		//�����ж�
		if (m_cbPlayStatus[wNextPlayer]==TRUE) break;
	}
	
	//����ж�
	bool bFinishTurn=false;
	if (m_wOperaCount>=m_wPlayerCount)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			LONG lUserScore=m_lTableScore[i*2]+m_lTableScore[i*2+1];
			if ((m_cbPlayStatus[i]==TRUE)&&(lUserScore<m_lTurnLessScore)&&(lUserScore<m_lUserMaxScore[i])) break;
		}
		if (i==m_wPlayerCount) bFinishTurn=true;
	}

	//������ע
	if (bFinishTurn==false)
	{
		//�����û�
		m_wCurrentUser=wNextPlayer;

		//��������
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.wCurrentUser=m_wCurrentUser;
		AddScore.lTurnLessScore=m_lTurnLessScore;
		AddScore.lAddScoreCount=m_lTableScore[wChairID*2];

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));

		//����ʱ��
		m_pITableFrame->SetGameTimer(IDI_PASS_CARD,40000L,1,m_wCurrentUser);

		return true;
	}
	else
	{
		//��������
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.wCurrentUser=INVALID_CHAIR;
		AddScore.lTurnLessScore=m_lTurnLessScore;
		AddScore.lAddScoreCount=m_lTableScore[wChairID*2];

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	}

	//�����ж�
	if (m_cbSendCardCount==MAX_COUNT) 
	{
		OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
		return true;
	}

	//�ۼƽ��
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_lTableScore[i*2+1]+=m_lTableScore[i*2];
		m_lTableScore[i*2]=0L;
	}

	//���ñ���
	m_wOperaCount=0;
	ZeroMemory(m_cbOperaScore,sizeof(m_cbOperaScore));

	//�����˿�
	if (m_bChaosCard==true)
	{
		//�����˿�
		BYTE cbSwitchCard=m_cbHandCardData[0][m_cbSendCardCount];

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount-1;i++)
		{
			m_cbHandCardData[i][m_cbSendCardCount]=m_cbHandCardData[i+1][m_cbSendCardCount];
		}

		//�����˿�
		m_cbHandCardData[m_wPlayerCount-1][m_cbSendCardCount]=cbSwitchCard;
	}

	//�ɷ��˿�
	BYTE cbSourceCount=m_cbSendCardCount;
	if (m_bShowHand==false) m_cbSendCardCount++;
	else m_cbSendCardCount=MAX_COUNT;

	//�ϴ�����û�
	WORD wLastMostUser = m_wLastMostUser;
	//��ǰ�û�
	if (m_bShowHand==false)
	{
		//�û�����
		if( m_cbSendCardCount == MAX_COUNT )
			m_wCurrentUser=EstimateWinner(1,m_cbSendCardCount-1);
		else
			m_wCurrentUser = EstimateWinner(m_cbSendCardCount-1,m_cbSendCardCount-1);
		m_wLastMostUser = m_wCurrentUser;
		
		//��ע����
		m_lTurnMaxScore=(m_cbSendCardCount>=3)?m_lMaxScore:m_lMaxScore/2L;
		m_lTurnLessScore=m_lTableScore[m_wCurrentUser*2]+m_lTableScore[m_wCurrentUser*2+1];
	}
	else m_wCurrentUser=INVALID_CHAIR; 

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=m_wCurrentUser;
	SendCard.wLastMostUser = wLastMostUser;
	SendCard.lTurnMaxScore=m_lTurnMaxScore;
	SendCard.lTurnLessScore=m_lTurnLessScore;
	SendCard.cbSendCardCount=m_cbSendCardCount-cbSourceCount;

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�˿���Ŀ
		if (m_cbPlayStatus[i]==TRUE) m_cbCardCount[i]=m_cbSendCardCount;

		//�ɷ��˿�
		for (BYTE j=0;j<(m_cbSendCardCount-cbSourceCount);j++)
		{
			ASSERT(j<CountArray(SendCard.cbCardData[i]));
			SendCard.cbCardData[i][j]=m_cbHandCardData[i][cbSourceCount+j];
		}
	}

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//��������
	if (m_wCurrentUser==INVALID_CHAIR) 
	{
		m_pITableFrame->KillGameTimer(IDI_PASS_CARD);
		m_pITableFrame->SetGameTimer(IDI_GAME_END,2000,1,0);
	}
	else m_pITableFrame->SetGameTimer(IDI_PASS_CARD,40000L,1,m_wCurrentUser);

	return true;
}

//��ȡʤ��
bool CTableFrameSink::OnUserGetWinner(IServerUserItem * pIServerUserItem)
{
	////��ȡʤ��
	//IServerUserItem * pIWinnerUserItem=m_pITableFrame->GetServerUserItem(EstimateWinner(0,4));
	//if (pIWinnerUserItem==NULL) return true;

	////������Ϣ
	//TCHAR szMessage[128];
	//_snprintf(szMessage,CountArray(szMessage),TEXT("���ֵ�ʤ������ǣ�%s"),pIWinnerUserItem->GetAccounts());

	////���ñ���
	//m_bChaosCard=false;

	////������Ϣ
	//m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_INFO);

	return true;
}

//�ƶ�����
WORD CTableFrameSink::EstimateLoser(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	for (WORD wLoser=0;wLoser<m_wPlayerCount;wLoser++)
	{
		if (m_cbPlayStatus[wLoser]==TRUE) 
		{
			m_GameLogic.SortCardList(cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	for (WORD i=(wLoser+1);i<m_wPlayerCount;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wLoser]+cbStartPos,cbConcludePos-cbStartPos+1)==false)
		{
			wLoser=i;
		}
	}

	return wLoser;
}

//�ƶ�ʤ��
WORD CTableFrameSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	for (WORD wWinner=0;wWinner<m_wPlayerCount;wWinner++)
	{
		if (m_cbPlayStatus[wWinner]==TRUE) 
		{
			//�����˿�
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	for (WORD i=(wWinner+1);i<m_wPlayerCount;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[i]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[i]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=i;
		}
	}

	return wWinner;
}

//////////////////////////////////////////////////////////////////////////
