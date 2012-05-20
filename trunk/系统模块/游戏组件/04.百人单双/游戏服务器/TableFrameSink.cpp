#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					8									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					6									//����ʱ��

//��Ϸʱ��
#define IDI_GAME					2									//��Ϸʱ��
#define TIME_GAME					20									//��Ϸʱ��

//����ʱ��
#define IDI_GAME_END				12									//����ʱ��
#define TIME_GAME_END				12									//����ʱ��

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink():m_pGameServiceOption(NULL)
{
	//��ע��Ϣ
	m_lOddScore=0L;
	m_lEvenScore=0L;
	m_nSendCardCount = 0;
	
	//״̬����
	m_dwJettonTime=0L;

	//��ע��Ϣ
	ZeroMemory(m_lUserOddScore,sizeof(m_lUserOddScore));
	ZeroMemory(m_lUserEvenScore,sizeof(m_lUserEvenScore));

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbDice,sizeof(m_cbDice));

	//ׯ�ұ���
	m_lApplyBankerCondition = 100000000;
	ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );
	m_cbBankerTimer=0;
	m_lBankerWinScore = 0;
	m_UserLeftScore=0;
	
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pITableFrameControl=NULL;
	m_pGameServiceOption = NULL;

	//��Ϸ��¼
	ZeroMemory(m_GameRecordArrary, sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;	
	m_nRecordLast=0;	

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
#ifdef __SPECIAL___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
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

	//���ƽӿ�
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//��ȡ����
	bool blGetConfig = true;
	if(m_pGameServiceOption !=0)
	{
		blGetConfig = false;

	}
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	if(blGetConfig==false)
	{
		return true;
	}

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\OddEvenBattleConfig.ini"),szPath);

	LONGLONG lMaxScore=2147483647;
	TCHAR OutBuf[255];


	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("BankerCondition%d"),m_pGameServiceOption->wServerID);
	GetPrivateProfileString(Session,TEXT("Score"),"1000",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lApplyBankerCondition);

	BYTE cbIsHightRoom=GetPrivateProfileInt(Session, TEXT("IsHightRoom"), 1, m_szConfigFileName);
	m_IsHightRoom=cbIsHightRoom?true:false;

	if(m_IsHightRoom)
	{
		memset(OutBuf,0,255);
		GetPrivateProfileString(Session, TEXT("LowerBigBankerCondition"),"1500000000",OutBuf,255,m_szConfigFileName);
		_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_LowerBigBankerCondition);

	}else
	{
		memset(OutBuf,0,255);
		GetPrivateProfileString(Session, TEXT("LowerBigBankerCondition"),"500000000",OutBuf,255,m_szConfigFileName);
		_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_LowerBigBankerCondition);
	}

	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("ScoreLimit%d"),m_pGameServiceOption->wServerID);
	GetPrivateProfileString(Session,TEXT("AreaLimitScore"),"2147483647",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lAreaLimitScore);

	memset(OutBuf,0,255);
	GetPrivateProfileString(Session,TEXT("UserLimitScore"),"2147483647",OutBuf,255,m_szConfigFileName);
	_snscanf(OutBuf,strlen(OutBuf),"%I64d",&m_lUserLimitScore);

	return true;

}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��ע��Ϣ
	m_lOddScore=0L;
	m_lEvenScore=0L;
	m_UserLeftScore=0;

	//��ע��Ϣ
	ZeroMemory(m_lUserOddScore,sizeof(m_lUserOddScore));
	ZeroMemory(m_lUserEvenScore,sizeof(m_lUserEvenScore));

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbDice,sizeof(m_cbDice));

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
	m_pITableFrame->SetGameStatus(GS_WK_GAME);

	
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	GameStart.cbGameTime = TIME_GAME;
	GameStart.wCurBankerID = m_CurrentBanker.wChairID;
	GameStart.wServerID = m_pGameServiceOption->wServerID;


	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//�ֻ�ׯ��
	ChangeBanker();

	//�л��ж�
	if ( m_cbBankerTimer == 0 )
	{
		//������Ϣ
		SendChangeBankerMsg();
	}

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			GameEnd.nBankerTime = m_cbBankerTimer;

			GameEnd.lBankerTotalScore = m_lBankerWinScore;

			//�ƶ����
			DeduceWinner(GameEnd.cbWinner);			

			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;

				//ʤ������
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//д�����
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], (LONG)m_lUserRevenue[wUserChairID], ScoreKind);

				//ׯ���ж�
				if ( m_CurrentBanker.dwUserID == pIServerUserItem->GetUserID() ) m_CurrentBanker.lUserScore = pIServerUserItem->GetUserScore()->lScore;
			}

			//���ͻ���
			GameEnd.cbTimeLeave=TIME_FREE;	
			if ( m_CurrentBanker.dwUserID != 0 ) GameEnd.lBankerTreasure = m_CurrentBanker.lUserScore;
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//������Ϣ					
				GameEnd.lMeMaxScore=pIServerUserItem->GetUserScore()->lScore;
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//ׯ���ж�
			if ( m_CurrentBanker.dwUserID != 0 )
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
				if (pIServerUserItem) 
				{
					//�����ж�
					if ( pIServerUserItem->GetUserScore()->lScore < m_lApplyBankerCondition )
					{
						//�����ׯ
						OnUserApplyBanker( pIServerUserItem->GetUserData(), false, false );
						//��ʾ��Ϣ
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
						m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
					}
				}
			}

				//���»���
			for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
			{
				tagApplyUserInfo &ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(ApplyUserInfo.wChairID);
				if ( pIServerUserItem == NULL ) continue;
				ApplyUserInfo.lUserScore = pIServerUserItem->GetUserScore()->lScore;
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�ƶ��±�
			m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
			if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;


			return true;
		}
	case GER_USER_LEFT:		//�û��뿪
		{
			//ʤ������
			enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

			//д�����
			if (m_lUserWinScore[wChairID]!=0L) m_pITableFrame->WriteUserScore(wChairID,m_lUserWinScore[wChairID], (LONG)m_lUserRevenue[wChairID], ScoreKind);
			m_lUserWinScore[wChairID] = 0;

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
	case GS_FREE:			//����״̬
		{
			//���ͼ�¼
			WORD wBufferSize=0;
			BYTE cbBuffer[SOCKET_BUFFER];
			int nIndex = m_nRecordFirst;
			while ( nIndex != m_nRecordLast )
			{
				if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
				{
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
					wBufferSize=0;
				}
				CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
				wBufferSize+=sizeof(tagServerGameRecord);

				nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
			}
			if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//��ע��Ϣ
			StatusFree.lOddcore=m_lOddScore;
			StatusFree.lEvenScore=m_lEvenScore;

			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.lUserLimitScore = m_lUserLimitScore;

			//ׯ����Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;

			//��ע��Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lMeOddScore=m_lUserOddScore[wChiarID];
				StatusFree.lMeEvenScore=m_lUserEvenScore[wChiarID];
				StatusFree.lMeMaxScore=pIServerUserItem->GetUserScore()->lScore;
			}
	
			if ( m_CurrentBanker.dwUserID != 0 ) 
			{
				StatusFree.cbBankerTime = m_cbBankerTimer;
				StatusFree.lCurrentBankerScore = m_lBankerWinScore;
				StatusFree.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusFree.lBankerTreasure = m_CurrentBanker.lUserScore;
			}
			else StatusFree.wCurrentBankerChairID = INVALID_CHAIR;
			

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
			//����������
			SendApplyUser(  pIServerUserItem );

			return bSuccess;
		}
	case GS_PLAYING:		//��Ϸ״̬
	case GS_WK_JETTON:	
	case GS_WK_END:	
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��ע��Ϣ
			StatusPlay.lOddcore=m_lOddScore;
			StatusPlay.lEvenScore=m_lEvenScore;

			StatusPlay.lAreaLimitScore = m_lAreaLimitScore;
			StatusPlay.lUserLimitScore = m_lUserLimitScore;

			//ׯ����Ϣ
			StatusPlay.lApplyBankerCondition = m_lApplyBankerCondition;

			//��ע��Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusPlay.lMeOddScore=m_lUserOddScore[wChiarID];
				StatusPlay.lMeEvenScore=m_lUserEvenScore[wChiarID];
				StatusPlay.lMeMaxScore=pIServerUserItem->GetUserScore()->lScore;
			}

			if ( m_CurrentBanker.dwUserID != 0 ) 
			{
				StatusPlay.cbBankerTime = m_cbBankerTimer;
				StatusPlay.lCurrentBankerScore = m_lBankerWinScore;
				StatusPlay.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusPlay.lBankerTreasure = m_CurrentBanker.lUserScore;
			}
			else StatusPlay.wCurrentBankerChairID = INVALID_CHAIR;

			//ɫ����Ϣ
			CopyMemory(StatusPlay.cbDice,m_cbDice,sizeof(m_cbDice));

			//����������
			SendApplyUser( pIServerUserItem );

			//���ͳ���
			m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			return true;
		}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{

			RandDice();

			MakeAndroidUserBanker();

			//��ʼ��Ϸ
			m_pITableFrameControl->StartGame();

			//����ʱ��
			m_pITableFrame->SetGameTimer(IDI_GAME,TIME_GAME*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_WK_GAME);

			return true;
		}
	case IDI_GAME:
		{
			//����ʱ��
			m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_WK_END);

			CMD_S_EndPlaceJetton EndPlaceJetton;
			ZeroMemory(&EndPlaceJetton, sizeof(EndPlaceJetton));
			EndPlaceJetton.cbGameTime = TIME_GAME_END;
			CopyMemory(EndPlaceJetton.cbDice, m_cbDice, sizeof(EndPlaceJetton.cbDice));

			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_END_PLACE_JETTON,&EndPlaceJetton,sizeof(EndPlaceJetton));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_END_PLACE_JETTON,&EndPlaceJetton,sizeof(EndPlaceJetton));

			//�������
			CalculateScore();

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//������Ϸ
			OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_WK_FREE);

			//ׯ����Ϣ
			if ( m_CurrentBanker.dwUserID != 0 )
			{
				CMD_S_ChangeUserScore ChangeUserScore;
				ZeroMemory( &ChangeUserScore, sizeof( ChangeUserScore ) );
				ChangeUserScore.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				ChangeUserScore.lCurrentBankerScore = m_lBankerWinScore;
				ChangeUserScore.cbBankerTime = m_cbBankerTimer;
				ChangeUserScore.lScore = m_CurrentBanker.lUserScore;
				ChangeUserScore.wChairID = m_CurrentBanker.wChairID;
				m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHANGE_USER_SCORE,&ChangeUserScore,sizeof(ChangeUserScore));
				m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHANGE_USER_SCORE,&ChangeUserScore,sizeof(ChangeUserScore));
			}

			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ApplyBanker));
			if (wDataSize!=sizeof(CMD_C_ApplyBanker)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			//if (pUserData->cbUserStatus!=US_SIT) return true;

			//��Ϣ����
			CMD_C_ApplyBanker * pApplyBanker=(CMD_C_ApplyBanker *)pDataBuffer;

			return OnUserApplyBanker( pUserData, pApplyBanker->bApplyBanker );	
		}

case SUB_C_MANAGE_CONTROL:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ManageControl));
			if (wDataSize!=sizeof(CMD_C_ManageControl)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if((pUserData->dwUserRight&UR_GAME_CONTROL)==0) return false;

			//����ת��
			CMD_C_ManageControl *pManageControl=(CMD_C_ManageControl*)pDataBuffer;
			//
			//�������

			TCHAR Session[255];
			memset(Session,0,255);
			if(m_pGameServiceOption)
			_snprintf(Session,sizeof(Session),TEXT("ManagerControl%d"),m_pGameServiceOption->wServerID);
			else
			{
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,128,"m_pGameServiceOption ==NULL SUB_C_MANAGE_CONTROL");
				
			}

			if (pManageControl->bControlBanker)
			{
				WritePrivateProfileString(Session, TEXT("BankerWin"), pManageControl->nControlBanker?TEXT("1"):TEXT("0"), m_szConfigFileName);
			}
			else
			{
				WritePrivateProfileString(Session, TEXT("BankerWin"), TEXT("9"), m_szConfigFileName);
			}

			if (pManageControl->bControlArea)
			{
				TCHAR szWindSide[16];
				_sntprintf(szWindSide,CountArray(szWindSide),TEXT("%d"),pManageControl->nControlArea+1);
				WritePrivateProfileString(Session, TEXT("WinSide"), szWindSide,m_szConfigFileName);
			}
			else
			{
				WritePrivateProfileString(Session, TEXT("WinSide"), TEXT("9"),m_szConfigFileName);
			}

			//������Ϣ
			m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("���óɹ�"),SMT_INFO|SMT_EJECT);

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
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,NULL);
		m_pITableFrame->SetGameStatus(GS_WK_FREE);
	}
	//������ʾ
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//״̬�ж�
		if ( m_pITableFrame->GetGameStatus() != GS_PLAYING )
		{
			//��������
			RETYPELONG lScore=0;
			RETYPELONG lRevenue=0;
			enScoreKind ScoreKind;

			//�������
			m_lUserOddScore[wChairID] = 0;
			m_lUserEvenScore[wChairID] = 0;

			//д�����
			if (lScore!=0L)
			{
				//д�����
				ScoreKind=enScoreKind_Flee;
				m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,(LONG)lRevenue, ScoreKind);
			}
		}

		//�����ж�
		for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

			if ( ApplyUserInfo.dwUserID == pIServerUserItem->GetUserID() )
			{

				//�������
				CMD_S_ApplyBanker ApplyBanker;
				CopyMemory( ApplyBanker.szAccount, pIServerUserItem->GetAccounts(), sizeof( ApplyBanker.szAccount ) );
				ApplyBanker.lScore = pIServerUserItem->GetUserScore()->lScore;
				ApplyBanker.bApplyBanker = false;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );

				//��ǰ�ж�
				if ( ApplyUserInfo.dwUserID == m_CurrentBanker.dwUserID )
				{
					//���ñ���
					ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );
					m_cbBankerTimer=0;
					m_lBankerWinScore=0;

					//������Ϣ
					SendChangeBankerMsg();
				}
				
				//ɾ�����
				m_ApplyUserArrary.RemoveAt( nUserIdx );
			}
		}
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, RETYPELONG lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_EVEN)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_EVEN)||(lJettonScore<=0L)) return false;



	if (m_pITableFrame->GetGameStatus()!=GS_WK_GAME)
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		return true;
	}


	//ׯ���ж�
	if ( m_CurrentBanker.dwUserID != 0 && m_CurrentBanker.wChairID == wChairID ) return true;
	if ( m_CurrentBanker.dwUserID == 0 ) return true;


	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	RETYPELONG lJettonCount=m_lUserOddScore[wChairID]+m_lUserEvenScore[wChairID];

	RETYPELONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	if ( lUserScore < lJettonCount + lJettonScore ) return true;

	//�Ϸ���֤
	if ( ID_ODD == cbJettonArea )
	{
		if ( GetMaxOddScore(wChairID) < lJettonScore ) return true;

		//������ע
		m_lOddScore+= lJettonScore;
		m_lUserOddScore[wChairID] += lJettonScore;
	}
	else if ( ID_EVEN == cbJettonArea )
	{
		if ( GetMaxEvenScore(wChairID) <lJettonScore ) return true;

		//������ע
		m_lEvenScore+= lJettonScore;
		m_lUserEvenScore[wChairID] += lJettonScore;
	}
	else
	{
		ASSERT(FALSE);
		return true;
	}

	//��������
	CMD_S_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.wChairID=wChairID;
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	return true;
}

//�����˿�
bool CTableFrameSink::RandDice()
{
	//����ɫ��
	m_cbDice[0] = rand() % 6 + 1;
	m_cbDice[1] = rand() % 6 + 1;


	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("ManagerControl%d"),m_pGameServiceOption->wServerID);


	int m_cbWinSideControl=GetPrivateProfileInt(Session, TEXT("WinSide"), 9, m_szConfigFileName );

	int nBankerWin=GetPrivateProfileInt(Session, TEXT("BankerWin"), 9, m_szConfigFileName );

	if (0==nBankerWin || 1==nBankerWin)
	{

		if (m_nSendCardCount < SEND_COUNT)
		{
			//ׯ�ҵ÷�
			LONGLONG lBankerWinScore=CalculateScoreBanker();

			if (0==nBankerWin && lBankerWinScore>0)
			{
				m_nSendCardCount++;
				RandDice();
			}
			else if (1==nBankerWin && lBankerWinScore<0)
			{
				m_nSendCardCount++;
				RandDice();

			}else
			{
				m_nSendCardCount=0;

			}
		}else
		{
			m_nSendCardCount = 0;

		}
	}else
	{
		if(m_cbWinSideControl==1)
		{

			BYTE Tmp1[]={2,4,6};
			BYTE Tmp2[]={1,3,5};
			if(m_cbDice[0]%2==1)
			{
				int index = (rand()%3);
				m_cbDice[1]= Tmp1[index];
			}else
			{
				int index = (rand()%3);
				m_cbDice[1]= Tmp2[index];
			}
	
		}else
		{
			if(m_cbWinSideControl==2)
			{
				BYTE Tmp1[]={2,4,6};
				BYTE Tmp2[]={1,3,5};

				if(m_cbDice[0]%2==1)
				{
					int index = (rand()%3);
					m_cbDice[1]= Tmp2[index];

				}else
				{
					int index = (rand()%3);
					m_cbDice[1]= Tmp1[index];

				}

			}
		}
	}

	return true;
}

//ӯ���ж�
LONGLONG CTableFrameSink::AccountPayoffScore()
{
	return 0;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker( tagServerUserData *pUserData, bool bApplyBanker, bool bUserAction )
{
	//״̬�ж�
//	if ( bUserAction == true && m_pITableFrame->GetGameStatus() != GS_WK_FREE ) return true;

	//�Ϸ��ж�
	if ( bApplyBanker && pUserData->UserScoreInfo.lScore < m_lApplyBankerCondition ) return true;

	//�������
	if ( bApplyBanker )
	{
		//�����ж�
		for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];
			if ( ApplyUserInfo.dwUserID == pUserData->dwUserID ) return true;
		}

		//������Ϣ
		tagApplyUserInfo ApplyUserInfo;
		ApplyUserInfo.dwUserID = pUserData->dwUserID;
		ApplyUserInfo.lUserScore = pUserData->UserScoreInfo.lScore;
		ApplyUserInfo.wChairID = pUserData->wChairID;

		//�������
		INT_PTR nUserCount = m_ApplyUserArrary.GetCount();
		m_ApplyUserArrary.InsertAt( nUserCount, ApplyUserInfo );

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory( ApplyBanker.szAccount, pUserData->szAccounts, sizeof( ApplyBanker.szAccount ) );
		ApplyBanker.lScore = pUserData->UserScoreInfo.lScore+pUserData->lStorageScore;
		ApplyBanker.bApplyBanker = true;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );

		return true;
	}
	else	//ȡ������
	{
		//�����ж�
		for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

			if ( ApplyUserInfo.dwUserID == pUserData->dwUserID )
			{
				//ɾ�����
				m_ApplyUserArrary.RemoveAt( nUserIdx );

				//��ǰ�ж�
				if ( m_CurrentBanker.dwUserID == ApplyUserInfo.dwUserID )
				{
					m_CurrentBanker.dwUserID = 0;
					ChangeBanker();
					SendChangeBankerMsg();
				}

				//�������
				CMD_S_ApplyBanker ApplyBanker;
				CopyMemory( ApplyBanker.szAccount, pUserData->szAccounts, sizeof( ApplyBanker.szAccount ) );
				ApplyBanker.lScore = pUserData->UserScoreInfo.lScore;
				ApplyBanker.bApplyBanker = false;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );


				return true;
			}
		}

		return true;
	}

	return true;

}
void CTableFrameSink::RandList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{

	//����׼��
	BYTE *cbCardData = new BYTE[cbBufferCount];
	CopyMemory(cbCardData,cbCardBuffer,cbBufferCount);

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	delete []cbCardData;
	cbCardData = NULL;

	return;
}
//�����������ׯ
void CTableFrameSink::MakeAndroidUserBanker()
{

	BYTE  bcAndroidUser[GAME_PLAYER];
	int   bcOutAppUserArrayCount = 0;
	int   inInAppUserArrayCount = 0;
	int i = 0;

	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("BankerCondition%d"),m_pGameServiceOption->wServerID);



	for (WORD wCharidid=0; wCharidid<GAME_PLAYER; ++wCharidid)
	{

		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wCharidid);

		if(!pServerUserItem)
		{
			memset(&m_AndroidUser[wCharidid],0,sizeof(TAndroidUser));
			continue;
		}

		if(pServerUserItem&&pServerUserItem->IsAndroidUser())
		{
			m_AndroidUser[wCharidid].blAndroidUser = true;

		}else
		{
			m_AndroidUser[wCharidid].blAndroidUser = false;
			memset(&m_AndroidUser[wCharidid],0,sizeof(TAndroidUser));
			continue;

		}
		if (m_CurrentBanker.wChairID !=wCharidid )
		{
			if(m_AndroidUser[wCharidid].blAndroidUser==true)
			{
				m_AndroidUser[wCharidid].PlayCount++;
				m_AndroidUser[wCharidid].BankCount=0;

			}
			bool blInArray = false;
			for (INT nUserIdx=0; nUserIdx<m_ApplyUserArrary.GetCount(); ++nUserIdx)
			{
				if(wCharidid == m_ApplyUserArrary[nUserIdx].wChairID)
				{
					inInAppUserArrayCount++;
					blInArray = true;
					break;

				}
			}

			int	m_nAndroidUserPlayerCount=GetPrivateProfileInt(Session, TEXT("AndroidUserPlayerCount"), 2, m_szConfigFileName);

			if(blInArray==false&&m_AndroidUser[wCharidid].PlayCount>m_nAndroidUserPlayerCount&&m_pITableFrame->GetServerUserItem(wCharidid)->GetUserScore()->lScore>m_lApplyBankerCondition
				)
			{
				bcAndroidUser[i++] = (BYTE)wCharidid;
				bcOutAppUserArrayCount++;
			}

		}else
		{

			if(m_AndroidUser[wCharidid].blAndroidUser==true)
			{
				m_AndroidUser[wCharidid].PlayCount=0;
				m_AndroidUser[wCharidid].BankCount++;

			}
			int	m_nAndroidUserBankerCount=GetPrivateProfileInt(Session, TEXT("AndroidUserBankerCount"), 1, m_szConfigFileName);
			if(m_AndroidUser[wCharidid].BankCount>m_nAndroidUserBankerCount)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wCharidid);
				if(pServerUserItem)
					OnUserApplyBanker(pServerUserItem->GetUserData(),false,true);
				m_AndroidUser[wCharidid].BankCount = 0;

			}

		}

	}
	WORD wMinApplyBankerCount=GetPrivateProfileInt(Session, TEXT("MinApplyBankerCount"), 2, m_szConfigFileName);

	if(inInAppUserArrayCount<wMinApplyBankerCount&&bcOutAppUserArrayCount>0)
	{
		RandList(bcAndroidUser,bcOutAppUserArrayCount);

		if(bcOutAppUserArrayCount<wMinApplyBankerCount-inInAppUserArrayCount)
		{ 
			for (int i = 0;i<bcOutAppUserArrayCount;i++)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(bcAndroidUser[i]);

				if(pServerUserItem)
					OnUserApplyBanker(pServerUserItem->GetUserData(),true,true);
			}


		}else
		{
			for (int i = 0;i<wMinApplyBankerCount-inInAppUserArrayCount;i++)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(bcAndroidUser[i]);

				if(pServerUserItem)
					OnUserApplyBanker(pServerUserItem->GetUserData(),true,true);
			}

		}

	}

}
//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for ( INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx )
	{
		tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[ nUserIdx ];

		//��ȡ���
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( ApplyUserInfo.wChairID );
		if ( ! pServerUserItem ) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory( ApplyBanker.szAccount, pServerUserItem->GetAccounts(), sizeof( ApplyBanker.szAccount ) );
		ApplyBanker.lScore = pServerUserItem->GetUserScore()->lScore;
		ApplyBanker.bApplyBanker = true;

		//������Ϣ
		m_pITableFrame->SendUserData( pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );
	}
}
//����ܹ���ׯ����
WORD CTableFrameSink::GetBankerTime()
{


	//��ȡ����
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	TCHAR szFileName[MAX_PATH];
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\OddEvenBattleConfig.ini"),szPath);
	TCHAR Session[255];
	memset(Session,0,255);
	_snprintf(Session,sizeof(Session),TEXT("BankerCondition%d"),m_pGameServiceOption->wServerID);

	WORD wBankerTime=GetPrivateProfileInt(Session, TEXT("Time"), 20, szPath);

	if(m_CurrentBanker.wChairID==INVALID_CHAIR)
	{
		return wBankerTime;

	}



	if(m_cbBankerTimer>=wBankerTime)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);

		if(pIServerUserItem==NULL)
		{
			return wBankerTime;
		}

		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;
		LONGLONG ScoreCondition = 0;

	
		ScoreCondition= m_LowerBigBankerCondition;
		

		if(lBankerScore>=ScoreCondition)
		{

			WORD wBankerTimeAdd=GetPrivateProfileInt(Session, TEXT("TimeAdd"), 10, szFileName);
			return wBankerTimeAdd+wBankerTime;

		}else
		{
			for (WORD i=0; i<m_ApplyUserArrary.GetCount(); ++i)
			{
				//��ȡ���
				tagApplyUserInfo wChairID=m_ApplyUserArrary[i];

				//��������
				if(wChairID.wChairID==m_CurrentBanker.wChairID)continue;

				IServerUserItem *pbIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID.wChairID);

				if(pbIServerUserItem)
				{
					LONGLONG lScore=pbIServerUserItem->GetUserScore()->lScore;

					if(lBankerScore<lScore)
					{
						return 0;
					}
				}

				break;
			}

		}	 
	}

	return wBankerTime;

}
//����ׯ��
void CTableFrameSink::ChangeBanker()
{

	try
	{
			
			//��������
			m_cbBankerTimer++;
			//��ȡ����
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetCurrentDirectory(sizeof(szPath),szPath);

			//��ȡ����
			TCHAR szFileName[MAX_PATH];
			_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\OddEvenBattleConfig.ini"),szPath);

			TCHAR Session[255];
			memset(Session,0,255);
			_snprintf(Session,sizeof(Session),TEXT("BankerCondition"));


			WORD wBankerTime = GetBankerTime();
			
			LONGLONG lBigScore = 0; 

			for(int i = 0;i<m_ApplyUserArrary.GetCount();i++)
			{
				tagApplyUserInfo wChairID=m_ApplyUserArrary[i];

				if(wChairID.wChairID==m_CurrentBanker.wChairID)
				{
					continue;
				}
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID.wChairID);
				if(pIServerUserItem)
				{
					if(pIServerUserItem->GetUserScore()->lScore>lBigScore)
					{
						lBigScore = pIServerUserItem->GetUserScore()->lScore;

					}	
				}else
				{
					continue ;
				}

			}
			//��ׯ�ж�
			if ( m_CurrentBanker.dwUserID != 0 )
			{
				//�ֻ��ж�
				IServerUserItem *pBankerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
				if(pBankerUserItem)
				{

					//�����ж�
					if ( (wBankerTime <= m_cbBankerTimer) || (m_CurrentBanker.lUserScore < m_lApplyBankerCondition))
					{
						m_cbBankerTimer = 0;
						m_lBankerWinScore=0;

						//�ͻ���ɾ��
						IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_CurrentBanker.wChairID );
						OnUserApplyBanker( pServerUserItem->GetUserData(), false, false ) ;

						//���¸�ֵ
						ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );

						while( ! m_ApplyUserArrary.IsEmpty() )
						{
							m_CurrentBanker = m_ApplyUserArrary[ 0 ];
							//�Ϸ��ж�
							if ( m_CurrentBanker.lUserScore < m_lApplyBankerCondition )
							{
								ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );

								//��ȡ���
								IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_ApplyUserArrary[ 0 ].wChairID );
								if (  pServerUserItem  ) OnUserApplyBanker( pServerUserItem->GetUserData(), false, false );
							}
							else
								break;
						}
					}

				}else if ( 0 < m_ApplyUserArrary.GetCount() )
				{

					while( ! m_ApplyUserArrary.IsEmpty() )
					{
						m_CurrentBanker = m_ApplyUserArrary[ 0 ];
						//�Ϸ��ж�
						if ( m_CurrentBanker.lUserScore < m_lApplyBankerCondition )
						{
							ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );
							//��ȡ���
							IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_ApplyUserArrary[ 0 ].wChairID );
							if (  pServerUserItem  ) OnUserApplyBanker( pServerUserItem->GetUserData(), false, false );
						}
						else
							break;
					}
					m_cbBankerTimer = 0;
					m_lBankerWinScore=0;
				}
				
			}
			else if ( 0 < m_ApplyUserArrary.GetCount() )
			{
				while( ! m_ApplyUserArrary.IsEmpty() )
				{
					m_CurrentBanker = m_ApplyUserArrary[ 0 ];
					//�Ϸ��ж�
					if ( m_CurrentBanker.lUserScore < m_lApplyBankerCondition )
					{
						ZeroMemory( &m_CurrentBanker, sizeof( m_CurrentBanker ) );
						//��ȡ���
						IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem( m_ApplyUserArrary[ 0 ].wChairID );
						if (  pServerUserItem  ) OnUserApplyBanker( pServerUserItem->GetUserData(), false, false );
					}
					else
						break;
				}
				m_cbBankerTimer = 0;
				m_lBankerWinScore=0;
			}
	}
	catch (...)
	{
	}
}

//�ֻ�ׯ��
void CTableFrameSink::SendChangeBankerMsg()
{
	CMD_S_ChangeBanker ChangeBanker;

	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		IServerUserItem *pSeverUserItem = m_pITableFrame->GetServerUserItem( wChairID );
		if ( ! pSeverUserItem ) continue;

		ZeroMemory( &ChangeBanker, sizeof( ChangeBanker ) );
		if ( m_CurrentBanker.dwUserID != 0 )
		{
			ChangeBanker.wChairID = m_CurrentBanker.wChairID;
			ChangeBanker.lBankerTreasure = m_CurrentBanker.lUserScore;
		}
		else
		{
			ChangeBanker.wChairID = INVALID_CHAIR;
		}
		ChangeBanker.lBankerScore = m_CurrentBanker.lUserScore;
		ChangeBanker.cbBankerTime = m_cbBankerTimer;

		//�ҵ���ע
		m_pITableFrame->SendTableData( wChairID, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof( ChangeBanker ) );
		m_pITableFrame->SendLookonData( wChairID, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof( ChangeBanker ) );
	}

	if(m_CurrentBanker.dwUserID == 0 || m_CurrentBanker.wChairID>=GAME_PLAYER)return;
	TCHAR Session[255]={0};
	_snprintf(Session,sizeof(Session),TEXT("Message%d"),m_pGameServiceOption->wServerID);

	//��ȡ��Ϣ
	LONGLONG lMessageCount=GetPrivateProfileInt(Session,TEXT("MessageCount"),0,m_szConfigFileName);
	if (lMessageCount!=0)
	{
		//��ȡ����
		LONGLONG lIndex=rand()%lMessageCount;
		TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
		_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%ld"),lIndex);
		GetPrivateProfileString(Session,szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

		//��ȡ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
		if(pIServerUserItem==NULL)return;

		//������Ϣ
		_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetAccounts());

		//��Ϸ���
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage2,SMT_INFO);
		}

		//�Թ����
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,szMessage2,SMT_INFO);

		}while(true);
	}
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	return true;
}

//�����ע
RETYPELONG CTableFrameSink::GetMaxOddScore(WORD wChairID)
{
	//��������
	RETYPELONG lOtherAreaScore = m_lEvenScore;

	IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//�����ע
	RETYPELONG lMaxOddScore = m_CurrentBanker.lUserScore+lOtherAreaScore;
	lMaxOddScore -= (m_lOddScore);

	//����ע��
	RETYPELONG lNowJetton = 0;
	lNowJetton += m_lUserOddScore[wChairID];
	lNowJetton += m_lUserEvenScore[wChairID];

	//�ҵ���ע
	
	RETYPELONG lMeLessScore  = min(pServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
	 lMeLessScore =  lMeLessScore  - lNowJetton;
	ASSERT(lMeLessScore >= 0 );
	lMeLessScore = max(lMeLessScore, 0);

	//�����ע
	RETYPELONG lMaxJetton = min(lMaxOddScore, lMeLessScore);
	RETYPELONG lAreaLimit = m_lAreaLimitScore - m_lOddScore;
	lMaxJetton = min(lMaxJetton, lAreaLimit);

	return lMaxJetton;
}
//�����ע
RETYPELONG CTableFrameSink::GetMaxEvenScore(WORD wChairID)
{
	//��������
	RETYPELONG lOtherAreaScore = m_lOddScore ;

	IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//�����ע
	RETYPELONG lMaxEvenScore = lOtherAreaScore + m_CurrentBanker.lUserScore;
	lMaxEvenScore -= (m_lEvenScore);

	//����ע��
	RETYPELONG lNowJetton = 0;
	lNowJetton += m_lUserOddScore[wChairID];
	lNowJetton += m_lUserEvenScore[wChairID];

	//�ҵ���ע
	RETYPELONG lMeLessScore = min(m_lUserLimitScore,pServerUserItem->GetUserScore()->lScore);
	lMeLessScore  = lMeLessScore - lNowJetton;
	ASSERT(lMeLessScore >= 0 );
	lMeLessScore = max(lMeLessScore, 0);

	//�����ע
	RETYPELONG lMaxJetton = min(lMaxEvenScore, lMeLessScore);
	RETYPELONG lAreaLimit = m_lAreaLimitScore - m_lEvenScore;
	lMaxJetton = min(lMaxJetton, lAreaLimit);

	

	return lMaxJetton;
}

//����÷�
void CTableFrameSink::CalculateScore()
{
	//������Ϣ
	CMD_S_GameScore GameScore;
	ZeroMemory(&GameScore,sizeof(GameScore));

	//��������
	RETYPELONG cbRevenue=m_pGameServiceOption->wRevenue;

	//�ƶ����
	DeduceWinner(GameScore.cbWinner);

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbDicePoint = m_cbDice[0] + m_cbDice[1];
	GameRecord.wWinner = GameScore.cbWinner;


	//ׯ������
	RETYPELONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	RETYPELONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�����ע
	RETYPELONG *pUserScore[] = {NULL, m_lUserOddScore, m_lUserEvenScore};

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//ׯ���ж�
		if ( m_CurrentBanker.dwUserID != 0 && m_CurrentBanker.wChairID == i ) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		for ( WORD wAreaIndex = ID_ODD; wAreaIndex <= ID_EVEN; ++wAreaIndex )
		{
			if ( wAreaIndex == GameScore.cbWinner ) 
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][i] ) ;
			}
			else
			{
				lUserLostScore[i] -= pUserScore[wAreaIndex][i] ;
				lBankerWinScore += pUserScore[wAreaIndex][i] ;
			}
		}

		//����˰��
		if ( 100 <= m_lUserWinScore[i] )
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[i]  = RETYPELONG(m_lUserWinScore[i]*fRevenuePer);
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//�ܵķ���
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	//ׯ�ҳɼ�
	if ( m_CurrentBanker.dwUserID != 0 )
	{
		WORD wBankerChairID = m_CurrentBanker.wChairID;
		m_lUserWinScore[wBankerChairID] = lBankerWinScore;

		//����˰��
		if ( 100 <= m_lUserWinScore[wBankerChairID] )
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[wBankerChairID]  = RETYPELONG(m_lUserWinScore[wBankerChairID]*fRevenuePer);
			m_lUserWinScore[wBankerChairID] -= m_lUserRevenue[wBankerChairID];
			lBankerWinScore = m_lUserWinScore[wBankerChairID];
		}
		IServerUserItem *pBankerUserItem = m_pITableFrame->GetServerUserItem(wBankerChairID);

		//�ۼƻ���
		m_lBankerWinScore += lBankerWinScore;
	}

	//��ע����
	GameScore.lDrawOddScore=m_lOddScore;
	GameScore.lDrawEvenScore=m_lEvenScore;

	GameScore.lBankerScore = lBankerWinScore;

	//���ͻ���
	for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
		if ( pIServerUserItem == NULL ) continue;

		//�ҵ���ע
		GameScore.lMeOddScore=m_lUserOddScore[wUserIndex];
		GameScore.lMeEvenScore=m_lUserEvenScore[wUserIndex];

		//������Ϣ
		GameScore.lMeGameScore=m_lUserWinScore[wUserIndex];
		GameScore.lMeReturnScore = m_lUserReturnScore[wUserIndex];
		m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_SCORE,&GameScore,sizeof(GameScore));
		m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_SCORE,&GameScore,sizeof(GameScore));
	}
	return ;
}
//����÷�
LONGLONG CTableFrameSink::CalculateScoreBanker()
{


	RETYPELONG							m_lUserWinScore[GAME_PLAYER];		//��ҳɼ�
	RETYPELONG							m_lUserReturnScore[GAME_PLAYER];	//������ע
	RETYPELONG							m_lUserRevenue[GAME_PLAYER];		//

	//������Ϣ
	CMD_S_GameScore GameScore;
	ZeroMemory(&GameScore,sizeof(GameScore));

	//��������
	RETYPELONG cbRevenue=m_pGameServiceOption->wRevenue;

	//�ƶ����
	DeduceWinner(GameScore.cbWinner);

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbDicePoint = m_cbDice[0] + m_cbDice[1];
	GameRecord.wWinner = GameScore.cbWinner;

	//ׯ������
	RETYPELONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	RETYPELONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�����ע
	RETYPELONG *pUserScore[] = {NULL, m_lUserOddScore, m_lUserEvenScore};

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//ׯ���ж�
		if ( m_CurrentBanker.dwUserID != 0 && m_CurrentBanker.wChairID == i ) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		for ( WORD wAreaIndex = ID_ODD; wAreaIndex <= ID_EVEN; ++wAreaIndex )
		{
			if ( wAreaIndex == GameScore.cbWinner ) 
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
				lBankerWinScore -= ( pUserScore[wAreaIndex][i] ) ;
			}
			else
			{
				lUserLostScore[i] -= pUserScore[wAreaIndex][i] ;
				lBankerWinScore += pUserScore[wAreaIndex][i] ;
			}
		}

		//����˰��
		if ( 100 <= m_lUserWinScore[i] )
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[i]  = RETYPELONG(m_lUserWinScore[i]*fRevenuePer);
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//�ܵķ���
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	//ׯ�ҳɼ�
	if ( m_CurrentBanker.dwUserID != 0 )
	{
		WORD wBankerChairID = m_CurrentBanker.wChairID;
		m_lUserWinScore[wBankerChairID] = lBankerWinScore;

		//����˰��
		if ( 100 <= m_lUserWinScore[wBankerChairID] )
		{
			float fRevenuePer=float(cbRevenue/1000.);
			m_lUserRevenue[wBankerChairID]  = RETYPELONG(m_lUserWinScore[wBankerChairID]*fRevenuePer);
			m_lUserWinScore[wBankerChairID] -= m_lUserRevenue[wBankerChairID];
			lBankerWinScore = m_lUserWinScore[wBankerChairID];
		}
		IServerUserItem *pBankerUserItem = m_pITableFrame->GetServerUserItem(wBankerChairID);


	}

	return  lBankerWinScore;
}
//�ƶ�Ӯ��
void CTableFrameSink::DeduceWinner(BYTE &cbWinner)
{
	//���㵥˫
	bool bEven = ((m_cbDice[0] + m_cbDice[1]) % 2 == 0 ) ? true : false;

	cbWinner = bEven ? ID_EVEN : ID_ODD; 
}
//////////////////////////////////////////////////////////////////////////
//���в���
#ifdef __SPECIAL___
bool __cdecl CTableFrameSink::OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem->GetUserStatus()!=US_LOOKON && pIServerUserItem->GetUserScore()->lScore < m_lApplyBankerCondition)
	{
		//ׯ���ж�
		OnActionUserStandUp(pIServerUserItem->GetChairID(), pIServerUserItem, false);
	}
	return true;
}
bool __cdecl CTableFrameSink::IsAllowBankAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	/*if(wChairID == m_CurrentBanker.dwUserID)
		return false;
	else */
		return true;

}
#endif
