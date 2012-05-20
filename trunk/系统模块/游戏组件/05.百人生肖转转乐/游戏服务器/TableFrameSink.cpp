#include "StdAfx.h"
#include "TableFrameSink.h"

TCHAR g_szLog[1000];


//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					8									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������


#define IDI_PLACE_JETTON			1									//��עʱ��
#define IDI_OPEN_CARD				2									//����ʱ��
#define IDI_GAME_FREE               3                                   //����ʱ��


//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//״̬����
	m_dwJettonTime=0L;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));

	//ׯ�ұ���
	m_lApplyBankerCondition = 100000000;
	ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
	m_cbBankerTimer=0;
	m_lBankerWinScore = 0;
	m_bCancelBanker=false;
	m_lMaxBankerTimer=0L;

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pITableFrameControl=NULL;


	//��ȡ����
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR ConfigFileName[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
	PathRemoveFileSpec(szPath);
	_snprintf(ConfigFileName,sizeof(ConfigFileName),TEXT("%s\\shengxiaoConfig.ini"),szPath);

	m_lApplyBankerCondition = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 10000000, ConfigFileName);
	m_lAreaLimitScore = ( INT64 )GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("AreaLimitScore"), 0, ConfigFileName);
	m_lMaxBankerTimer =	( INT64 )GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("BankerTimer"), 0, ConfigFileName);

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

	//���ƽӿ�
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//���ò���
	TCHAR   buffer[MAX_PATH];
	if (GetCurrentDirectory(sizeof(buffer), buffer))
	{
		CString strIniFileName;

		CString szDir = buffer;
		if(szDir.Right(1) != "\\")	szDir += "\\";
		szDir += "BaccaratConfig.ini";
		strIniFileName= szDir;
	}

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
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
	memset( m_nUserBet , 0 , sizeof(m_nUserBet) );
	memset( m_ScoreInfo , 0 , sizeof(m_ScoreInfo));
	m_bneedWriteTable = FALSE;

	IServerUserItem *pServerItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
	m_nMaxCanBet = m_CurrentBanker.lUserScore;
	if(pServerItem)
	{
		m_nMaxCanBet = pServerItem->GetUserData()->UserScoreInfo.lGameGold;
	}
	//��ʼ��ע
	CMD_S_GameStart GameStart;
	GameStart.cbBankerTimes = m_cbBankerTimer;
	if( m_CurrentBanker.dwUserID != 0 )
		GameStart.wBanker = m_CurrentBanker.wChairID;
	else
		GameStart.wBanker = INVALID_CHAIR;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	//////////////////////////////////////////////////////////////////////////
	//��־
	
	try
	{
		TCHAR szLog[1000];
		int  lenth = 0;
		IServerUserItem *pServerItem=NULL;

		pServerItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
		if( pServerItem )
		{
			lenth += sprintf( szLog+lenth ,"\nׯ��ID:%d|���:%I64d",pServerItem->GetUserID() , pServerItem->GetUserScore()->lGameGold );
		}

		for ( int i=0; i<GAME_PLAYER ;i++ )
		{		
			pServerItem = m_pITableFrame->GetServerUserItem(i);
			if( pServerItem == NULL ) continue;
			lenth += sprintf(  szLog + lenth , "ID:%d|���:%I64d\n" , pServerItem->GetUserID() , pServerItem->GetUserScore()->lGameGold  );
		}

		SYSTEMTIME st;
		CString strDate;
		GetLocalTime(&st);
		strDate.Format("\\Shengxiao%2d-%2d.txt",st.wMonth,st.wDay);

		TCHAR szPath[MAX_PATH]=TEXT("");
		GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
		PathRemoveFileSpec(szPath);
		strDate=szPath+strDate;

		m_strFileName = strDate;
		FILE *pFile = fopen( m_strFileName , "a+" );
		if( pFile )
		{
			fwrite( ( void *)(&szLog) ,sizeof(TCHAR),lenth ,pFile );
			fclose(pFile);
		}

	}
	catch(...)
	{
		CWnd wnd;
		wnd.MessageBox("д����־�쳣1");
	}
	//////////////////////////////////////////////////////////////////////////
	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			if( m_bneedWriteTable )
			{
				m_bneedWriteTable = FALSE;
				m_pITableFrame->WriteTableScore(m_ScoreInfo,GAME_PLAYER,0);
				//for ( int i=0;i<GAME_PLAYER;i++ )
				//{
				//	if( m_pITableFrame->GetServerUserItem(i) )
				//		m_pITableFrame->WriteUserScore( i , m_ScoreInfo[i].lScore , 0 , m_ScoreInfo[i].ScoreKind );
				//}

				memset( m_ScoreInfo , 0 , sizeof(m_ScoreInfo));
			}
			//������Ϸ
			m_pITableFrame->ConcludeGame();
			return true;
		}
	case GER_USER_LEFT:		//�û��뿪
		{

			if( m_pITableFrame->GetGameStatus() == GS_BACCARAT_JETTON )
			{
				//���ܵ���
				if( m_CurrentBanker.wChairID == wChairID )
				{	
					m_bBankerLeft = true;

					//�ۼ��м����еļƷ�
					INT64 total=0;
					for ( int i=0;i<GAME_PLAYER;i++ )
					{
						for (int j=0;j<MAX_AREA;j++)
						{
							if ( m_nUserBet[i][j] > 0 )
							{
								total+=m_nUserBet[i][j];
							}
						}
					}

					memset( m_nUserBet,0,sizeof(m_nUserBet) );

					IServerUserItem * pServerItem = m_pITableFrame->GetServerUserItem(wChairID);
					if( pServerItem )
					{
						m_pITableFrame->WriteUserScore( wChairID , -total , 0 , enScoreKind_Lost );
						m_bneedWriteTable = FALSE;
						memset( m_ScoreInfo , 0 , sizeof(m_ScoreInfo));

						TCHAR szLog[50];
						int	lenth = sprintf( szLog , "ׯ������ID:%d|�۳�%I64d" ,pServerItem->GetUserID(), -total );

						FILE *pFile = fopen( m_strFileName , "a+" );
						if( pFile )
						{
							fwrite( ( void *)(&szLog) ,sizeof(TCHAR),lenth ,pFile );
							fclose(pFile);
						}

					}
				}
				else
				{
					INT64 total=0;
					for (int i=0;i<MAX_AREA;i++)
					{
						if ( m_nUserBet[wChairID][i] > 0 )
						{
							total += m_nUserBet[wChairID][i];
							m_nUserBet[wChairID][i] = 0;
						}
					}

					m_ScoreInfo[wChairID].lScore = 0;
					m_ScoreInfo[wChairID].lRevenue = 0;
					m_ScoreInfo[wChairID].ScoreKind = enScoreKind_Lost;

					IServerUserItem * pServerItem = m_pITableFrame->GetServerUserItem(wChairID);
					if( pServerItem )
					{
						m_pITableFrame->WriteUserScore( wChairID , -total , 0 , enScoreKind_Lost );
						TCHAR szLog[50];
						int	lenth = sprintf( szLog , "�м�����ID:%d|�۳�%I64d" ,pServerItem->GetUserID(), -total );

						FILE *pFile = fopen( m_strFileName , "a+" );
						if( pFile )
						{
							fwrite( ( void *)(&szLog) ,sizeof(TCHAR),lenth ,pFile );
							fclose(pFile);
						}

					}

					return true;
				}
			}
			
			//��������������Ϸ
			if ( m_pITableFrame->GetGameStatus() == GS_BACCARAT_OPENCARD )
			{
				if( m_bneedWriteTable )
				{
					m_bneedWriteTable = FALSE;
					m_pITableFrame->WriteTableScore(m_ScoreInfo,GAME_PLAYER,0);
					//for ( int i=0;i<GAME_PLAYER;i++ )
					//{
					//	if( m_pITableFrame->GetServerUserItem(i) )
					//		m_pITableFrame->WriteUserScore( i , m_ScoreInfo[i].lScore , 0 , m_ScoreInfo[i].ScoreKind );
					//}

					memset( m_ScoreInfo , 0 , sizeof(m_ScoreInfo));
				}
			}

			IServerUserItem *pServerItem = m_pITableFrame->GetServerUserItem( wChairID );
			tagServerUserData *pServerData = pServerItem->GetUserData();
			if( pServerData )
			{
				OnUserApplyBanker( pServerData , false );
				m_CurrentBanker.dwUserID = 0;
				ChangeBanker();
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
	case GS_BACCARAT_FREE:			//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_GAME_FREE-__min(dwPassTime,TIME_GAME_FREE));
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.lMeMaxScore = pIServerUserItem->GetUserScore()->lScore;

			//ׯ����Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.cbBankerTime = m_cbBankerTimer;
			StatusFree.lBankerWin = m_lBankerWinScore;

			if (m_CurrentBanker.dwUserID != 0) 
			{
				StatusFree.cbBankerTime = m_cbBankerTimer;
				StatusFree.lCurrentBankerScore = m_lBankerWinScore;
				StatusFree.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusFree.lBankerTreasure = m_CurrentBanker.lUserScore;
			}
			else StatusFree.wCurrentBankerChairID = INVALID_CHAIR;
			
			//����������
			SendApplyUser(pIServerUserItem);

			//������Ϸ��¼
			SendGameRecord(pIServerUserItem);

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_BACCARAT_JETTON:		//��ע״̬
	case GS_BACCARAT_OPENCARD:
		{
			//��������
			CMD_S_StatusJetton StatusJetton;
			ZeroMemory(&StatusJetton,sizeof(StatusJetton));

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusJetton.cbTimeLeave=(BYTE)(TIME_PLACE_JETTON-__min(dwPassTime,TIME_PLACE_JETTON));

			StatusJetton.lAreaLimitScore=m_lAreaLimitScore;
			StatusJetton.cbBankerTime = m_cbBankerTimer;
			StatusJetton.lBankerWin = m_lBankerWinScore;

			//ׯ����Ϣ
			StatusJetton.lApplyBankerCondition = m_lApplyBankerCondition;

			//��ע��Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
			//	StatusJetton.lMeMaxScore=pIServerUserItem->GetUserScore()->lScore;
			}

			if (m_CurrentBanker.dwUserID != 0) 
			{
				StatusJetton.cbBankerTime = m_cbBankerTimer;
				StatusJetton.lCurrentBankerScore = m_lBankerWinScore;
				StatusJetton.wCurrentBankerChairID = m_CurrentBanker.wChairID;
				StatusJetton.lBankerTreasure = m_CurrentBanker.lUserScore;
			}
			else StatusJetton.wCurrentBankerChairID=INVALID_CHAIR;

			//����������
			SendApplyUser(pIServerUserItem);

			//������Ϸ��¼
			SendGameRecord(pIServerUserItem);

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusJetton,sizeof(StatusJetton));
		}
	//case GS_BACCARAT_OPENCARD:
	//	{
	//		
	//		return true;
	//	}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
try
{
	switch (wTimerID)
	{
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
			m_pITableFrame->SetGameStatus(GS_BACCARAT_JETTON);

			//��ʼ��Ϸ
			m_bBankerLeft = false;
			m_pITableFrameControl->StartGame();
			m_cbResult = 0xff;
			memset( &m_ScoreInfo,0,sizeof(m_ScoreInfo) );

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_OPEN_CARD,TIME_PLACE_JETTON*1000,1,0L);
			return true;
		}
	case IDI_OPEN_CARD:			//������Ϸ
		{
			//��ȡ����
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR ConfigFileName[MAX_PATH]=TEXT("");
			GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
			PathRemoveFileSpec(szPath);
			_snprintf(ConfigFileName,sizeof(ConfigFileName),TEXT("%s\\shengxiaoConfig.ini"),szPath);
			
			BYTE cbTemp = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Cheat"), 0 , ConfigFileName );
			m_pITableFrame->KillGameTimer(IDI_OPEN_CARD);
			m_pITableFrame->SetGameStatus(GS_BACCARAT_OPENCARD);

			CMD_S_StatusOpenCard OpenCard;
			OpenCard.cbAnimalPos  = rand()%36;		//0-35
			m_cbResult = (OpenCard.cbAnimalPos)%12;	//0-11

			if( cbTemp != 0 && cbTemp>=1 && cbTemp<=12 )
			{
				m_cbResult = OpenCard.cbAnimalPos = cbTemp - 1;
			}

			m_pITableFrame->SendTableData( INVALID_CHAIR , SUB_S_RESLUT , &OpenCard , sizeof(OpenCard));
			m_pITableFrame->SendLookonData( INVALID_CHAIR , SUB_S_RESLUT , &OpenCard , sizeof(OpenCard));

			//������
			memset( &m_ScoreInfo,0,sizeof(m_ScoreInfo) );
		
			try
			{
				if( m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID) && m_CurrentBanker.dwUserID != 0 && m_bBankerLeft == false )
				{
					//////////////////////////////////////////////////////////////////////////
					IServerUserItem *pServerItem = NULL;
					TCHAR szLog[200*GAME_PLAYER];
					int  lenth = 0;
					TCHAR *pszResult[]={"��","С"};
					lenth += sprintf( szLog + lenth ,"�Ʒֱ��ֽ��:%d|%s\n", m_cbResult , pszResult[m_cbResult%2] );
					//////////////////////////////////////////////////////////////////////////

					INT64	total=0;
					for( int i=0 ; i<GAME_PLAYER ;i++ )
					{	
						pServerItem = m_pITableFrame->GetServerUserItem(i);
						if( pServerItem==NULL||m_CurrentBanker.wChairID == i )	continue;

						for ( int j=0;j<MAX_AREA;j++ )
						{
							//����û����ע
							if( m_nUserBet[i][j]<=0 )	continue;

							if ( j==12 || j==13 )
							{
								//��
								if( j%2==m_cbResult%2 )
								{
									m_ScoreInfo[i].lScore += m_nUserBet[i][j];
									m_ScoreInfo[m_CurrentBanker.wChairID].lScore -= m_nUserBet[i][j];

									lenth += sprintf( szLog + lenth , "ID:%d|��СӮ:%I64d\n",
										pServerItem->GetUserID(),m_nUserBet[i][j]);
								}
								else
								{
									m_ScoreInfo[i].lScore -= m_nUserBet[i][j];
									m_ScoreInfo[m_CurrentBanker.wChairID].lScore += m_nUserBet[i][j];

									lenth += sprintf( szLog + lenth , "ID:%d|��С��:%I64d\n",
										pServerItem->GetUserID(),-m_nUserBet[i][j]);
								}

								continue;
							}

							if( j==m_cbResult )
							{
								m_ScoreInfo[i].lScore += m_nUserBet[i][j]*11;
								m_ScoreInfo[m_CurrentBanker.wChairID].lScore -= m_nUserBet[i][j]*11;

								lenth += sprintf( szLog + lenth , "ID:%d|ѹ��Ӯ:%I64d\n",
									pServerItem->GetUserID(),m_nUserBet[i][j]*11);
							}
							else
							{
								m_ScoreInfo[i].lScore -= m_nUserBet[i][j];
								m_ScoreInfo[m_CurrentBanker.wChairID].lScore += m_nUserBet[i][j];

								lenth += sprintf( szLog + lenth , "ID:%d|ûѹ��Ӯ:%I64d\n",
									pServerItem->GetUserID(),-m_nUserBet[i][j]);
							}
						}

						if( m_ScoreInfo[i].lScore>0 )
						{
							m_ScoreInfo[i].ScoreKind = enScoreKind_Win;
						}
						else if (m_ScoreInfo[i].lScore<0)
						{
							m_ScoreInfo[i].ScoreKind = enScoreKind_Lost;
						}
						else
						{
							m_ScoreInfo[i].ScoreKind = enScoreKind_Draw;
						}


						lenth += sprintf( szLog + lenth , "ID:%d|����:%I64d\n",
							pServerItem->GetUserID(),m_ScoreInfo[i].lScore);
					}

					pServerItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);

					lenth += sprintf( szLog + lenth , "ׯ��ID:%d|����:%I64d\n",
						pServerItem->GetUserID(),m_ScoreInfo[m_CurrentBanker.wChairID].lScore);

					m_bneedWriteTable = TRUE;

					CMD_S_UserScore UserScore;
					//
					for (int i=0;i<GAME_PLAYER;i++)
					{
						IServerUserItem *pServerItem = m_pITableFrame->GetServerUserItem(i);
						if( pServerItem == NULL ) continue ;
						UserScore.nScore = m_ScoreInfo[i].lScore;
						UserScore.nBankerScore = m_ScoreInfo[m_CurrentBanker.wChairID].lScore;
						m_pITableFrame->SendTableData( i , SUB_S_GAME_SCORE , &UserScore,sizeof(UserScore) );
					}

					//ׯ�ҳɼ�
					m_lBankerWinScore += m_ScoreInfo[m_CurrentBanker.wChairID].lScore;

					FILE *pFile = fopen( m_strFileName , "a+" );
					if( pFile )
					{
						fwrite( ( void *)(&szLog) ,sizeof(TCHAR),lenth ,pFile );
						fclose(pFile);
					}

				}

			}
			catch(...)
			{
				CWnd wnd;
				wnd.MessageBox("д���쳣2");
			}
			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_GAME_FREE,TIME_OPEN_CARD*1000,1,0L);

			return true;
		}
	case IDI_GAME_FREE:
		{
			m_pITableFrame->KillGameTimer(IDI_GAME_FREE);
			//������Ϸ
			OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

			if( m_CurrentBanker.dwUserID != 0 )
				m_cbBankerTimer++;
			
			m_pITableFrame->SendTableData( INVALID_CHAIR , SUB_S_FREE  );
			m_pITableFrame->SendLookonData( INVALID_CHAIR , SUB_S_FREE );

			//����״̬
			m_pITableFrame->SetGameStatus(GS_BACCARAT_FREE);

			//��ׯ�ж�
			if (m_bCancelBanker && m_CurrentBanker.dwUserID != 0)
			{		
				//��ȡ���
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);

				//���ñ���
				m_cbBankerTimer = 0;
				m_lBankerWinScore=0;
				ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
				m_bCancelBanker=false;

				//������Ϣ
				SendChangeBankerMsg();

				//ɾ��ׯ��
				if (pServerUserItem) OnUserApplyBanker(pServerUserItem->GetUserData(), false);
			}


			//�ֻ�ׯ��
			ChangeBanker();

			//�л��ж�
			if (m_cbBankerTimer == 0)
			{
				//������Ϣ
				SendChangeBankerMsg();
			}

			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_GAME_FREE*1000,1,0L);

			return true;
		}
	}
}
catch(...)
{
	CWnd wnd;
	wnd.MessageBox("�쳣1");
}

	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

try
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{

			if( pIServerUserItem->GetUserStatus() != US_PLAY )	return true;

			if( m_pITableFrame->GetGameStatus() != GS_BACCARAT_JETTON )		return true;


			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton))
			{
				ASSERT(FALSE);
				return false;
			}

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			return OnUserPlaceJetton(pIServerUserItem->GetChairID(),pPlaceJetton->cbViewJettonArea,pPlaceJetton->cbAnimal,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_C_ApplyBanker))
			{
				ASSERT(FALSE);
				return false;
			}

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON)
			{
				ASSERT(FALSE);
				return false;
			}

			//��Ϣ����
			CMD_C_ApplyBanker * pApplyBanker=(CMD_C_ApplyBanker *)pDataBuffer;
			return OnUserApplyBanker(pUserData, pApplyBanker->bApplyBanker);	
		}
	}
}
catch(...)
{
	CWnd wnd;
	wnd.MessageBox("�쳣2");
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
		m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,1000,1,NULL);
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//״̬�ж�
		if (m_pITableFrame->GetGameStatus() != GS_BACCARAT_FREE)
		{
			//��������
			LONG lScore=0;
			LONG lRevenue=0;
			enScoreKind ScoreKind;

			//д�����
			if (lScore!=0L)
			{
				//д�����
				ScoreKind=enScoreKind_Flee;
				m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);
			}
		}

		//�����ж�
		for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[nUserIdx];

			if (ApplyUserInfo.dwUserID == pIServerUserItem->GetUserID())
			{
				//ɾ�����
				m_ApplyUserArrary.RemoveAt(nUserIdx);

				//�������
				CMD_S_ApplyBanker ApplyBanker;
				CopyMemory(ApplyBanker.szAccount, pIServerUserItem->GetAccounts(), sizeof(ApplyBanker.szAccount));
				ApplyBanker.lScore = pIServerUserItem->GetUserScore()->lScore;
				ApplyBanker.bApplyBanker = false;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

			}
		}
	}

	if(pIServerUserItem && pIServerUserItem->GetUserData()->dwUserID == m_CurrentBanker.dwUserID )
	{
		m_CurrentBanker.dwUserID = 0;
		ChangeBanker();
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbViewJettonArea, BYTE cbAnimal , INT64 lJettonScore)
{

	if( m_pITableFrame->GetGameStatus() != GS_BACCARAT_JETTON )	return true;


	if ( cbViewJettonArea>=MAX_AREA || wChairID == INVALID_CHAIR || wChairID >= GAME_PLAYER )
	{
		return false;
	}

	if( m_CurrentBanker.dwUserID == 0 )		return true;
	
	IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem(wChairID);
	if ( !pUserItem )	return false;

	if( pUserItem->GetUserStatus() != US_PLAY )	return true;

	if(	m_nMaxCanBet <= 0 )
	{
		m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
		m_pITableFrame->SetGameStatus(GS_BACCARAT_OPENCARD);
		m_pITableFrame->SetGameTimer(IDI_OPEN_CARD,3*1000,1,0L);
		return true;
	}



	//ׯ�����ܲ�������ע
	if( m_bBankerLeft || m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID) == NULL ) 
	{
		return true;
	}

	if( cbViewJettonArea==12||cbViewJettonArea==13 )
	{
		if ( m_nMaxCanBet - lJettonScore < 0 )
		{
			return true;
		}
		//two
		m_nMaxCanBet = m_nMaxCanBet - lJettonScore;
	}
	else
	{	
		if ( m_nMaxCanBet - 11*lJettonScore < 0 )
		{
			return true;
		}
		//
		m_nMaxCanBet = m_nMaxCanBet - 11*lJettonScore;
	}

	LONGLONG	total=0;
	for ( int i=0;i<MAX_AREA;i++ )
	{
		total += m_nUserBet[wChairID][i];
	}

	//�����Լ����Ƹ�
	if ( pUserItem->GetUserScore()->lGameGold < total + lJettonScore )
	{
		return true;
	}

	m_nUserBet[wChairID][cbViewJettonArea] += lJettonScore;

	CMD_S_PlaceJetton	PlaceJetton;
	PlaceJetton.wChairID = wChairID;
	PlaceJetton.lJettonScore = lJettonScore;
	PlaceJetton.cbJettonArea = cbAnimal;
	
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	return true;
}



//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(tagServerUserData *pUserData, bool bApplyBanker)
{
//	if( m_pITableFrame->GetGameStatus() == GS_BACCARAT_JETTON )	return true;

	//�Ϸ��ж�
	if (bApplyBanker && pUserData->UserScoreInfo.lGameGold < m_lApplyBankerCondition) 
		return true;

	//�������
	if (bApplyBanker)
	{
		//�����ж�
		for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[nUserIdx];
			if (ApplyUserInfo.dwUserID == pUserData->dwUserID) return true;
		}

		//������Ϣ
		tagApplyUserInfo ApplyUserInfo;
		ApplyUserInfo.dwUserID = pUserData->dwUserID;
		ApplyUserInfo.lUserScore = pUserData->UserScoreInfo.lGameGold;
		ApplyUserInfo.wChairID = pUserData->wChairID;

		//�������
		INT_PTR nUserCount = m_ApplyUserArrary.GetCount();
		m_ApplyUserArrary.InsertAt(nUserCount, ApplyUserInfo);

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory(ApplyBanker.szAccount, pUserData->szAccounts, sizeof(ApplyBanker.szAccount));
		ApplyBanker.lScore = pUserData->UserScoreInfo.lGameGold+pUserData->lStorageScore;
		ApplyBanker.bApplyBanker = true;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

		if( m_CurrentBanker.dwUserID == 0 &&  m_pITableFrame->GetGameStatus() == GS_BACCARAT_FREE  )
		{
			ChangeBanker();
		}

		return true;
	}
	else	//ȡ������
	{
		//�����ж�
		for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
		{
			tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[nUserIdx];
			if (ApplyUserInfo.dwUserID == pUserData->dwUserID)
			{
				//��ǰ�ж�
				if (m_CurrentBanker.dwUserID == ApplyUserInfo.dwUserID)
				{
					m_bCancelBanker = true;
					if( m_pITableFrame->GetGameStatus() != GS_BACCARAT_FREE )
						continue;
				}
 
				//ɾ�����
				m_ApplyUserArrary.RemoveAt(nUserIdx);

				//�������
				CMD_S_ApplyBanker ApplyBanker;
				CopyMemory(ApplyBanker.szAccount, pUserData->szAccounts, sizeof(ApplyBanker.szAccount));
				ApplyBanker.lScore = pUserData->UserScoreInfo.lGameGold;
				ApplyBanker.bApplyBanker = false;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

				return true;
			}
		}

		if( m_CurrentBanker.dwUserID == 0 &&  m_pITableFrame->GetGameStatus() == GS_BACCARAT_FREE  )
		{
			ChangeBanker();
		}


		return true;
	}

}

//����ׯ��
void CTableFrameSink::SendApplyUser(IServerUserItem *pRcvServerUserItem)
{
	for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
	{
		tagApplyUserInfo ApplyUserInfo = m_ApplyUserArrary[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(ApplyUserInfo.wChairID);
		if (! pServerUserItem) continue;
		
		if( m_CurrentBanker.dwUserID == pServerUserItem->GetUserData()->dwUserID )	continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		CopyMemory(ApplyBanker.szAccount, pServerUserItem->GetAccounts(), sizeof(ApplyBanker.szAccount));
		ApplyBanker.lScore = pServerUserItem->GetUserScore()->lScore;
		ApplyBanker.bApplyBanker = true;

		//������Ϣ
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//��Ϸ��¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{

}

//����ׯ��
void CTableFrameSink::ChangeBanker()
{
	//��ȡ����
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR ConfigFileName[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szPath,MAX_PATH);
	PathRemoveFileSpec(szPath);
	_snprintf(ConfigFileName,sizeof(ConfigFileName),TEXT("%s\\shengxiaoConfig.ini"),szPath);

	//��ȡ����
	m_lApplyBankerCondition = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 10000000, ConfigFileName);
	m_lAreaLimitScore = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("AreaLimitScore"), 1000000000, ConfigFileName);
	m_lMaxBankerTimer = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("BankerTimer"), 20, ConfigFileName);

	//��ׯ�ж�
	if (m_CurrentBanker.dwUserID != 0)
	{
try
{
		//�ֻ��ж�
		bool bChangeBanker = false;

		IServerUserItem *pBankerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
		if( pBankerUserItem == NULL )
		{	
			for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArrary.GetCount(); ++nUserIdx)
			{
				if ( m_ApplyUserArrary[nUserIdx].dwUserID == m_CurrentBanker.dwUserID )
				{
					m_ApplyUserArrary.RemoveAt(nUserIdx);
					break;
				}
			}

			m_CurrentBanker.dwUserID = 0;
			ChangeBanker();
			return ;
		}
		INT64 lBankerScore = pBankerUserItem->GetUserScore()->lGameGold;

		if( m_lApplyBankerCondition > lBankerScore   )
			bChangeBanker = true;

		//�����ж�
		if (bChangeBanker || m_cbBankerTimer > m_lMaxBankerTimer)
		{
			m_cbBankerTimer = 0;
			m_lBankerWinScore=0;
			m_bCancelBanker = false;

			//�ͻ���ɾ��
			IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_CurrentBanker.wChairID);
			OnUserApplyBanker(pServerUserItem->GetUserData(), false);

			//���¸�ֵ
			ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
			while(! m_ApplyUserArrary.IsEmpty())
			{
				m_CurrentBanker = m_ApplyUserArrary[0];
				//�Ϸ��ж�
				if (m_CurrentBanker.lUserScore < m_lApplyBankerCondition)
				{
					ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));

					//��ȡ���
					IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_ApplyUserArrary[0].wChairID);
					if ( pServerUserItem ) OnUserApplyBanker(pServerUserItem->GetUserData(), false);
				}
				else
					break;
			}
		}
}
catch(...)
{
	CWnd wnd;
	wnd.MessageBox("�쳣3");
}
	}
	else if (0 < m_ApplyUserArrary.GetCount())
	{
		while(!m_ApplyUserArrary.IsEmpty())
		{
			m_CurrentBanker = m_ApplyUserArrary[0];
			//�Ϸ��ж�
			if (m_CurrentBanker.lUserScore < m_lApplyBankerCondition)
			{
				ZeroMemory(&m_CurrentBanker, sizeof(m_CurrentBanker));
				//��ȡ���
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(m_ApplyUserArrary[0].wChairID);
				if (pServerUserItem) 
					OnUserApplyBanker(pServerUserItem->GetUserData(), false);
			}
			else
				break;
		}
		m_cbBankerTimer = 0;
		m_lBankerWinScore=0;
		m_bCancelBanker = false;
	}

	//��ʼ��ע
	CMD_S_GameStart GameStart;
	GameStart.cbBankerTimes = m_cbBankerTimer;
	if( m_CurrentBanker.dwUserID != 0 )
		GameStart.wBanker = m_CurrentBanker.wChairID;
	else
		GameStart.wBanker = INVALID_CHAIR;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CURRENT_BANKER,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CURRENT_BANKER,&GameStart,sizeof(GameStart));
	
} 
//�ֻ�ׯ��
void CTableFrameSink::SendChangeBankerMsg()
{
	CMD_S_ChangeBanker ChangeBanker;

	for (WORD i = 0; i < GAME_PLAYER; ++i)
	{
		IServerUserItem *pSeverUserItem = m_pITableFrame->GetServerUserItem(i);
		if (pSeverUserItem==NULL) continue;

		ZeroMemory(&ChangeBanker, sizeof(ChangeBanker));
		if (m_CurrentBanker.dwUserID != 0)
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
		m_pITableFrame->SendTableData(i, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(i, SUB_S_CHANGE_BANKER, &ChangeBanker, sizeof(ChangeBanker));
	}
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	return true;
}
//////////////////////////////////////////////////////////////////////////

