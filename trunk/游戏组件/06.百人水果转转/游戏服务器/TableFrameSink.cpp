#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//���м�ʱ��
#ifdef _DEBUG
#define TIME_FREE					10									//����ʱ��
#else
#define TIME_FREE					10									//����ʱ��
#endif

//��עʱ��
#define IDI_PLACE_JETTON			2									//��ע��ʱ��

#ifdef _DEBUG
#define TIME_PLACE_JETTON			40									//��עʱ��
#else
#define TIME_PLACE_JETTON			20									//��עʱ��
#endif

//����ʱ��
#define IDI_GAME_END				3									//������ʱ��

#ifdef _DEBUG
#define TIME_GAME_END				20									//����ʱ��
#else
#define TIME_GAME_END				20									//����ʱ��
#endif

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//��ʼ������������
const int CTableFrameSink::m_iOddsArray[JETTON_AREA_COUNT]={40,4,30,3,20,2,10,1};

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink() : m_iMaxAreaScore(100000000)
{
	//����ע��
	ZeroMemory(m_iTotalJettonScore, sizeof(m_iTotalJettonScore));

	//��������·�
	ZeroMemory(m_iAreaScoreLimit, sizeof(m_iAreaScoreLimit));

	//���Ʊ���
	m_iGoalAnimal = -1;						//ÿ����Ϸ�Ὺ���Ķ���
	m_iGameTimes = 0;


	//������ע
	ZeroMemory(m_iUserAreaScore, sizeof(m_iUserAreaScore));


	//��ҳɼ�	
	ZeroMemory(m_iUserWinScore,sizeof(m_iUserWinScore));
	ZeroMemory(m_iUserReturnScore,sizeof(m_iUserReturnScore));
	ZeroMemory(m_iUserRevenue,sizeof(m_iUserRevenue));

	m_CtrlAccountArray.RemoveAll();

	//״̬����
	m_dwJettonTime=0L;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_iBankerWinScore=0L;		
	m_iBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_nHasWinTimes = 0;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SGDZ_Config.ini"),szPath);

	//���Ʊ���
	__int64 lMaxScore=9223372036854775807;
	CString str;

	//������Ϣ
	TCHAR szBuffer[MAX_PATH];
	GetPrivateProfileString(TEXT("BankerCondition"),TEXT("Score"),TEXT("500000000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_iApplyBankerCondition=_atoi64(szBuffer);

	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("SysLimitScore"),TEXT("9223372036854775807"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_iSysLimitScore=_atoi64(szBuffer);

	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("UserLimitScore"),TEXT("9223372036854775807"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_iUserLimitScore=_atoi64(szBuffer);

	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	BYTE cbCtrlWin = GetPrivateProfileInt(TEXT("ControlWin"), TEXT("Win"), 0, m_szConfigFileName);
	m_bControlWin = cbCtrlWin>0 ? true : false;

	//ׯ��һ����ׯ���پ�
	m_nMaxBankerTimes = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 10, m_szConfigFileName);

	//��ȡ�����˺���Ϣ
	if( m_bControlWin)
	{
		int nAccountNum = GetPrivateProfileInt(TEXT("ControlWin"), TEXT("CtrlAccountNum"), 0, m_szConfigFileName);
		if( nAccountNum > 0 )
		{
			for( int i=0; i<nAccountNum; i++)
			{
				TCHAR szTemp[64]={0};
				ControlAccount CtrlAccount;
				ZeroMemory(&CtrlAccount, sizeof(CtrlAccount));
				sprintf(szTemp, "Account_%d", i);
				CtrlAccount.dwUserID = GetPrivateProfileInt(TEXT("ControlWin"), szTemp, -1, m_szConfigFileName);
				if( CtrlAccount.dwUserID >=0 )
				{
					ZeroMemory(szTemp, sizeof(szTemp));
					sprintf(szTemp, "WinOdds_%d", i);
					CtrlAccount.nOdds = GetPrivateProfileInt(TEXT("ControlWin"), szTemp, 0, m_szConfigFileName);
					m_CtrlAccountArray.Add(CtrlAccount);
				}
			}
		}
	}
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	m_ApplyUserArray.RemoveAll();
	m_CtrlAccountArray.RemoveAll();
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

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//����ע��
	ZeroMemory(m_iTotalJettonScore, sizeof(m_iTotalJettonScore));

	//��������·�
	ZeroMemory(m_iAreaScoreLimit, sizeof(m_iAreaScoreLimit));

	//������ע
	ZeroMemory(m_iUserAreaScore, sizeof(m_iUserAreaScore));

	//��ҳɼ�	
	ZeroMemory(m_iUserWinScore,sizeof(m_iUserWinScore));
	ZeroMemory(m_iUserReturnScore,sizeof(m_iUserReturnScore));
	ZeroMemory(m_iUserRevenue,sizeof(m_iUserRevenue));

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
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	m_iGameTimes++;

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

	//���ñ���
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.iBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;

	//ÿ����Ϸ��ʼʱ�̣�����������ʼ���·�ֵ
	CalculateAreaInitLimite();

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		GameStart.iAreaLimitScore[i] = m_iAreaScoreLimit[i];

    //���͸��Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//���͸���Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) 
			continue;

		//���û���
		GameStart.iUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_iUserLimitScore);
		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//ÿ����Ϸ��ʼʱ�̣�����������ʼ���·�ֵ
bool __cdecl CTableFrameSink::CalculateAreaInitLimite()
{
	//��ʼ������
	__int64 iBankerScore = 0;
	IServerUserItem * pIBankerServerUserItem = NULL;

	//������ׯ�����ׯ����Ϣ
	if( INVALID_CHAIR != m_wCurrentBanker )
	{
		pIBankerServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if( pIBankerServerUserItem == NULL )
			return FALSE;

		//���ׯ�ҵ�ǰ�Ļ���(���)
		iBankerScore = pIBankerServerUserItem->GetUserScore()->lScore;

		//����ׯ�ҽ�����������������ĳ�ʼ�����·�
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
			m_iAreaScoreLimit[i] = __min(iBankerScore/(m_iOddsArray[i]+1), m_iMaxAreaScore);
	}
	else //���������ׯ
	{
		if( m_bEnableSysBanker ) //�������ϵͳ��ׯ
		{
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
			{
				if( i%2 == 0 )
				{
					m_iAreaScoreLimit[i] = __min( m_iSysLimitScore/(m_iOddsArray[i]/m_iOddsArray[i+1]), m_iMaxAreaScore);
				}
				else
				{
					m_iAreaScoreLimit[i] = m_iSysLimitScore;	//����ǵͱ�����������������עΪ�����ļ�ָ�����޶�
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}
//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{
			//�ɷ��˿�
			DispatchTableCard();

			//�������
			__int64 iBankerWinScore=CalculateScore();

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.iBankerTotallScore=m_iBankerWinScore;
			GameEnd.iBankerScore=iBankerWinScore;
			GameEnd.cbGoalAnimal = m_iGoalAnimal;

			//���ͻ���
			GameEnd.cbTimeLeave=TIME_GAME_END;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) 
					continue;

				//���óɼ�
				GameEnd.iUserScore=m_iUserWinScore[wUserIndex];

				//��������
				GameEnd.iUserReturnScore=m_iUserReturnScore[wUserIndex];

				//����˰��
				if (m_iUserRevenue[wUserIndex]>0) 
					GameEnd.iRevenue=m_iUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) 
					GameEnd.iRevenue=m_iUserRevenue[m_wCurrentBanker];
				else 
					GameEnd.iRevenue=0;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEFT:		//�û��뿪
		{
			//�м��뿪�����̽���
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				__int64 iScore=0;
				__int64 iRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Flee;

				//ͳ�Ƴɼ�
				for( int i=0; i<JETTON_AREA_COUNT; i++)
					iScore += m_iUserAreaScore[wChairID][i];
				iScore = -iScore;

				//д�����
				if (iScore!=0L) 
					m_pITableFrame->WriteUserScore(pIServerUserItem, iScore,iRevenue, ScoreKind);

				return true;
			}

			//ׯ���뿪��״̬�ж�
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pIServerUserItem->GetAccounts());

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);

				//����ʱ��
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);

				//�ɷ��˿�
				DispatchTableCard();

				//�������
				CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.iBankerTotallScore=m_iBankerWinScore;
				if (m_iBankerWinScore>0) 
					GameEnd.iBankerScore=0;

				//���ͻ���
				GameEnd.cbTimeLeave=TIME_GAME_END;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) 
						continue;

					//���óɼ�
					GameEnd.iUserScore=m_iUserWinScore[wUserIndex];

					//��������
					GameEnd.iUserReturnScore=m_iUserReturnScore[wUserIndex];

					//����˰��
					if (m_iUserRevenue[wUserIndex]>0) 
						GameEnd.iRevenue=m_iUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) 
						GameEnd.iRevenue=m_iUserRevenue[m_wCurrentBanker];
					else 
						GameEnd.iRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			if (m_iUserWinScore[m_wCurrentBanker]<0)
			{
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_iUserWinScore[m_wCurrentBanker], m_iUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
			}

			//�л�ׯ��
			ChangeBanker(true);

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
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//������Ϣ
			StatusFree.iApplyBankerCondition = m_iApplyBankerCondition;
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				StatusFree.iAreaLimitScore[i] = m_iAreaScoreLimit[i];

			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.iBankerWinScore=m_iBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.iBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.iUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_iUserLimitScore); 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d, �������Ϊ��%I64d"),m_iApplyBankerCondition,m_iUserLimitScore);
			
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
						
			//����������
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//ȫ����ע
			for (int i=0; i<JETTON_AREA_COUNT; i++ )
			{
				StatusPlay.iTotalAreaScore[i] = m_iTotalJettonScore[i];
				StatusPlay.iAreaScoreLimit[i] = m_iAreaScoreLimit[i];
			}

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for(int i=0; i<JETTON_AREA_COUNT; i++)
					StatusPlay.iUserAreaScore[i] = m_iUserAreaScore[wChiarID][i];

				//�����ע
				StatusPlay.iUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_iUserLimitScore);
			}

			//������Ϣ
			StatusPlay.iApplyBankerCondition=m_iApplyBankerCondition;		

			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.iBankerWinScore=m_iBankerWinScore;	
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.iBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				//���óɼ�
				StatusPlay.iEndUserScore=m_iUserWinScore[wChiarID];

				//��������
				StatusPlay.iEndUserReturnScore=m_iUserReturnScore[wChiarID];

				//����˰��
				if (m_iUserRevenue[wChiarID]>0) 
					StatusPlay.iEndRevenue=m_iUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) 
					StatusPlay.iEndRevenue=m_iUserRevenue[m_wCurrentBanker];
				else 
					StatusPlay.iEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.iEndBankerScore=m_iBankerCurGameScore;

			}

			//����������
			SendApplyUser( pIServerUserItem );

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ�����
		{
			//��ʼ��Ϸ
			m_pITableFrameControl->StartGame();

			//������ע��ʱ��
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);

			//������״̬Ϊ��ע�׶�
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ�����
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);			

				//������Ϸ
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);			
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) 
					continue;

				//ʤ������
				enScoreKind ScoreKind=(m_iUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//д�����
				if (m_iUserWinScore[wUserChairID]!=0L) 
					m_pITableFrame->WriteUserScore(wUserChairID,m_iUserWinScore[wUserChairID], m_iUserRevenue[wUserChairID], ScoreKind);

				//��ׯ�����жϡ�һ����Ϸ���������������Ӯ��������Ҫ��������ׯ�����������Ǯ����������ׯ����������
				__int64 iUserScore=pIServerUserItem->GetUserScore()->lScore;
				if (wUserChairID!=m_wCurrentBanker && iUserScore<m_iApplyBankerCondition)
				{
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_ApplyUserArray[i]) continue;

						//��ׯ�Ҷ���ɾ����������ׯ�����
						m_ApplyUserArray.RemoveAt(i);

						//������Ϣ
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//���ñ���
						lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

						//������Ϣ
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//��ʾ��Ϣ
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_iApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�л�ׯ��
			ChangeBanker(false);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=TIME_FREE;
			GameFree.cbGameRecord = m_iGoalAnimal;
			GameFree.iGameTimes = m_iGameTimes;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));

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
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) 
				return false;

			if( pIServerUserItem == NULL )
				return true;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if( pUserData == NULL )
				return true;

			if (pUserData->cbUserStatus!=US_PLAY) 
				return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pDataBuffer;
			if( pPlaceJetton == NULL )
				return FALSE;

			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->iJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if( pUserData == NULL )
				return FALSE;

			if (pUserData->cbUserStatus==US_LOOKON)
				return true;

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if( pUserData == NULL )
				return FALSE;
			if (pUserData->cbUserStatus==US_LOOKON)
				return true;

			return OnUserCancelBanker(pIServerUserItem);	
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
		m_pITableFrame->SetGameStatus(GS_FREE);
	}

	//������ʾ
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�˷�����ׯ����Ϊ��%I64d, �������Ϊ��%I64d"),m_iApplyBankerCondition,m_iUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) 
				continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 iJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_SMALL_SNAKE)&&(iJettonScore>0L));
	if ((cbJettonArea>ID_SMALL_SNAKE)||(iJettonScore<=0L)) 
		return false;

	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) 
		return true;

	//ׯ���жϣ������ׯ�ң���Ӧ����ע
	if (m_wCurrentBanker==wChairID) 
		return true;

	//�����ǰ������ϵͳ��ׯ����û��ׯ�ң��򷵻�
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		return true;

	//ȡ���������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if( pIServerUserItem == NULL )
		return FALSE;

	//���������ڸ�����������ע������ܺ�
	__int64 iJettonCount = 0;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		iJettonCount += m_iUserAreaScore[wChairID][i];


	//�������ҵ�ǰ�ܹ����ж��ٻ��֣���ң�
	__int64 iUserScore = pIServerUserItem->GetUserScore()->lScore;

	//�Ϸ�У�飬�������ע��ͼ�����ע������Ӵ�����Ҿ߱��Ļ��֣��򷵻�
	if (iUserScore < iJettonCount + iJettonScore) 
		return true;

	//�������ע��ͼ�����ע������Ӵ�����ҵ���ע���򷵻�
	if (m_iUserLimitScore < iJettonCount + iJettonScore) 
		return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=false;
	if( cbJettonArea > ID_SMALL_SNAKE || cbJettonArea < ID_BIG_TIGER )
		return true;

	//���ĳ����������ע���ϼ�����ע����С�ڵ��ڸ�����ǰ����ע���ޣ�����в��������򷵻�
	if( m_iTotalJettonScore[cbJettonArea] + iJettonScore <= m_iAreaScoreLimit[cbJettonArea ])
	{
		m_iTotalJettonScore[cbJettonArea] += iJettonScore;
		m_iUserAreaScore[wChairID][cbJettonArea] += iJettonScore;
		m_iAreaScoreLimit[cbJettonArea] -= iJettonScore;
		bPlaceJettonSuccess = true;
	}
	else
	{
		return true;
	}

	//��ע�ɹ�
	if (bPlaceJettonSuccess)
	{
		//��������
		CMD_S_PlaceJetton PlaceJetton;
		ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

		//�������
		PlaceJetton.wChairID=wChairID;
		PlaceJetton.cbJettonArea=cbJettonArea;
		PlaceJetton.iJettonScore=iJettonScore;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	}
	else	//��עʧ��
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.iPlaceScore=iJettonScore;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//�����˿�
bool CTableFrameSink::DispatchTableCard( )
{
	//ϵͳҪ�����ʤ��
	if( m_bControlWin )
	{
		if( m_wCurrentBanker != INVALID_CHAIR )	//��ǰ������ׯ
		{
			//ȡ���������
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			if( pIServerUserItem == NULL )
				return FALSE;
			bool bIsInQueue = false;
			int nWinOdds = 0;

			for( int i=0; i< m_CtrlAccountArray.GetCount(); i++ )
			{
				ControlAccount tempAccount = m_CtrlAccountArray.GetAt(i);
				if( tempAccount.dwUserID == pIServerUserItem->GetUserData()->dwUserID )
				{
					nWinOdds = tempAccount.nOdds;
					bIsInQueue = true;

					break;
				}
			}

			//�����ǰׯ���ڿ���ʤ�ʶ�����
			if( bIsInQueue )
			{
				if( nWinOdds > 0 && nWinOdds <= 100 )
				{
					CtrlDispatchCard(nWinOdds);
				}
				else	//����ʤ�ʣ������洦��
				{
					NormalDispatchCard();
				}
			}
			else	//�����洦��
			{
				NormalDispatchCard();
			}
		}
		else //��ǰ������ׯ
		{
			//����ϵͳ��ׯ
			if( m_bEnableSysBanker )
				CtrlDispatchCard();
		}
	}
	else
	{
		NormalDispatchCard();
	}
	return true;
}

//���淢��
void CTableFrameSink::NormalDispatchCard()
{
	static int nBig=0;	//����Ļ���,10�ο�һ��
	static bool bHasBig=false;	//�����������Ƿ񿪹���
	m_iGoalAnimal = rand()%PATH_STEP_NUMBER;
	if( m_iGoalAnimal%2==0 )	//������ο���
	{
		if( bHasBig )	//��������������Ѿ�������,����������
		{
			m_iGoalAnimal = (m_iGoalAnimal+1)%PATH_STEP_NUMBER;
		}
		else
		{
			bHasBig = true;
		}
	}
	nBig++;
	if( nBig >= 10 )
	{
		bHasBig = false;
		nBig = 0;
	}
}

//���Ʒ���
void CTableFrameSink::CtrlDispatchCard(int nCtrlWinOdds)
{
	//ĳׯ��������ׯ������ȷ��Ӯ�Ĵ���
	int nCanWinTimes = m_nMaxBankerTimes * nCtrlWinOdds/100;

	//��ʣ���پ־͵���ׯ
	int nLeaveTimes = m_nMaxBankerTimes - m_wBankerTime;

	//����Ӯ���پ�
	int nLeaveWinTimes = nCanWinTimes - m_nHasWinTimes;

	CString strMsg;

	if( nLeaveTimes > nLeaveWinTimes )	//���ʣ���������ʣ��Ӯ�֣���ʾ���������һ��
	{
		m_iGoalAnimal = rand()%PATH_STEP_NUMBER;
		__int64 nRet = 0;
		bool bRet = GetOneAreaWinOdds(m_iGoalAnimal%8, nRet);
		if( bRet && nRet > 0 )	//�����������Ľ��Ҳ��Ӯ����Ӧ������
		{
			m_nHasWinTimes ++;
		}
	}
	else	//���ʣ��Ӯ��С�ڵ���ʣ���������Ѱ�Ӯ
	{
		int n = CalculateWinOdds();
		m_iGoalAnimal = n + (rand()%3) * JETTON_AREA_COUNT;
		m_nHasWinTimes ++;
	}
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	if( pIApplyServerUserItem == NULL )
		return true;

	//�Ϸ��ж�
	__int64 lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_iApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_INFO|SMT_EJECT);
			return true;
		}
	}

	//������Ϣ 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//�������
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//���ñ���
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//�л��ж�
	if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GS_FREE)
	{
		//������Ϣ
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) 
			continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) 
			continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//�л�ׯ�� 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return false;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;

	//ȡ����ǰׯ����ׯ�ʸ�
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_ApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) 
				continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//����ׯ��
		m_wCurrentBanker=INVALID_CHAIR;

		//�ֻ��ж�
		if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//���ñ���
		bChangeBanker=true;
	}
	//��ׯ�жϡ���ǰׯ����ʱ���л�����ׯ
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if( pIServerUserItem == NULL )
			return FALSE;

		__int64 iBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//�����жϡ������������ׯ�Ĵ���������ʣ���ֲ���������ׯ��������ǿ���滻��
		if (m_nMaxBankerTimes<=m_wBankerTime || iBankerScore<m_iApplyBankerCondition)
		{
			//�������
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//��ȡ���
				WORD wChairID=m_ApplyUserArray[i];

				//��������
				if (wChairID!=m_wCurrentBanker) continue;

				//ɾ�����
				m_ApplyUserArray.RemoveAt(i);

				break;
			}

			//����ׯ��
			m_wCurrentBanker=INVALID_CHAIR;

			//�ֻ��ж�
			if (m_ApplyUserArray.GetCount()!=0)
			{
				m_wCurrentBanker=m_ApplyUserArray[0];
			}

			bChangeBanker=true;

			//��ʾ��Ϣ
			TCHAR szTipMsg[128];
			if (iBankerScore<m_iApplyBankerCondition)
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),m_iApplyBankerCondition);
			else
            	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),m_nMaxBankerTimes);
			
			//������Ϣ
			SendGameMessage(INVALID_CHAIR,szTipMsg);	
		}
	}
	//�����ǰ��ϵͳ��ׯ������Ի�һ�������������ׯ
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		m_wCurrentBanker=m_ApplyUserArray[0];
		bChangeBanker=true;
	}

	//�л��ж�
	if (bChangeBanker)
	{
		//���ñ���
		m_wBankerTime = 0;
		m_iBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			if( pIServerUserItem == NULL )
				return FALSE;
			ChangeBanker.iBankerScore=pIServerUserItem->GetUserScore()->lScore;
		}
		else
		{
			//���if�����������㣬Ӧ����δ���
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡ��Ϣ
			LONG lMessageCount=GetPrivateProfileInt(TEXT("Message"),TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//��ȡ����
				LONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%ld"),lIndex);
				GetPrivateProfileString(TEXT("Message"),szKeyName,TEXT("��ϲ[ %s ]��ׯ"),szMessage1,CountArray(szMessage1),m_szConfigFileName);

				//��ȡ���
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

				//������Ϣ
				_sntprintf(szMessage2,CountArray(szMessage2),szMessage1,pIServerUserItem->GetAccounts());
				SendGameMessage(INVALID_CHAIR,szMessage2);
			}
		}
	}

	return bChangeBanker;
}



//����������ׯ������б�
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pServerUserItem == NULL) 
			continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) 
			continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ZeroMemory(&ApplyBanker, sizeof(ApplyBanker));
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
	if (wChairID==m_wCurrentBanker) 
		ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) 
			continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//����÷�
__int64 CTableFrameSink::CalculateScore()
{
	//��������
	__int64 cbRevenue=m_pGameServiceOption->wRevenue;

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbRecord = m_iGoalAnimal;
	GameRecord.cbGameTimes = m_iGameTimes;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) 
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//ׯ�ұ��ֳɼ�
	__int64 iBankerWinScore = 0;

	//��ҳɼ�
	__int64 iUserLostScore[GAME_PLAYER];

	//��ҳɼ�
	ZeroMemory(m_iUserWinScore, sizeof(m_iUserWinScore));
	ZeroMemory(m_iUserRevenue, sizeof(m_iUserRevenue));
	ZeroMemory(iUserLostScore, sizeof(iUserLostScore));
	
	//�н�����
	BYTE cbHotArea = m_iGoalAnimal % (JETTON_AREA_COUNT);

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) 
			continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) 
			continue;

		for(int iArea=0; iArea<JETTON_AREA_COUNT; iArea++ )
		{
			//ĳ������ѹ��
			if( cbHotArea == iArea )	
			{
				m_iUserWinScore[i] += m_iUserAreaScore[i][iArea] * m_iOddsArray[iArea];
				m_iUserReturnScore[i] += m_iUserAreaScore[i][iArea];
				iBankerWinScore -= m_iUserAreaScore[i][iArea] * m_iOddsArray[iArea];
			}
			else
			{
				iUserLostScore[i] -= m_iUserAreaScore[i][iArea];
				iBankerWinScore += m_iUserAreaScore[i][iArea];
			}
		}

		//����˰��
		if (0 < m_iUserWinScore[i])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000001);
			m_iUserRevenue[i]  = __int64(m_iUserWinScore[i]*fRevenuePer);
			m_iUserWinScore[i] -= m_iUserRevenue[i];
		}

		//�ܵķ���
		m_iUserWinScore[i] += iUserLostScore[i];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_iUserWinScore[m_wCurrentBanker] = iBankerWinScore;

		//����˰��
		if (0 < m_iUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000001);
			m_iUserRevenue[m_wCurrentBanker]  = __int64(m_iUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_iUserWinScore[m_wCurrentBanker] -= m_iUserRevenue[m_wCurrentBanker];
			iBankerWinScore = m_iUserWinScore[m_wCurrentBanker];
		}		
	}

	//�ۼƻ���
	m_iBankerWinScore += iBankerWinScore;

	//��ǰ����
	m_iBankerCurGameScore=iBankerWinScore;

	return iBankerWinScore;
}

//����ĳ���������Ӯ�����������Ӯ�Ľ����
bool CTableFrameSink::GetOneAreaWinOdds(int nAreaIndex, __int64 & iResult)
{
	iResult = 0;
	if( nAreaIndex > ID_SMALL_SNAKE || nAreaIndex < ID_BIG_TIGER )
		return false;

	__int64 nAllUserScore = 0;
	__int64 nTempScore = 0;

	for( int i=0; i<JETTON_AREA_COUNT; i++)
		nAllUserScore += m_iTotalJettonScore[i];

	for( int j=0; j<GAME_PLAYER; j++)
	{
		if( m_wCurrentBanker == j )
			continue;

		IServerUserItem * pUserItem = m_pITableFrame->GetServerUserItem(j);
		if( pUserItem == NULL )
			continue;

		nTempScore += m_iUserAreaScore[j][nAreaIndex];
	}

	iResult = nAllUserScore - nTempScore * m_iOddsArray[nAreaIndex];
	return true;
}

//ʤ�ʼ��㣬ϵͳ��ׯ������ʱ�á�����ֵΪ��������ʤ�����򣬷�Χ��0-7
int	CTableFrameSink::CalculateWinOdds()
{
	//����ÿ��������Ϊׯ����˵����Ӯȡ�Ľ��
	__int64 nWinScore[JETTON_AREA_COUNT]={0};
	__int64 nTempScore = 0;
	__int64 nAllScore = 0;
	__int64 nSortResult = 0;
	int nRet = ID_SMALL_SNAKE;	//����ֵ

	//���㱾�����µ���ע
	for(int k=0; k<JETTON_AREA_COUNT; k++ )
		nAllScore += m_iTotalJettonScore[k];


	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		//i����������ע���Ա��ʣ���������и�����ׯ����Ӯ���
		GetOneAreaWinOdds(i, nWinScore[i]);
	}

	//ϵͳ��ׯ��Ӯ����ע
	if( m_bEnableSysBanker && m_wCurrentBanker == INVALID_CHAIR )
	{
		for( int t=0; t<JETTON_AREA_COUNT; t++ )
		{

			if( nWinScore[t] > nSortResult )
			{
				nSortResult = nWinScore[t];
				nRet = t;
			}
		}
	}
	else	//�������ׯ�ң���ֻҪӮ�Ϳ�����
	{
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
		{
			if( nWinScore[i] > 0 )
			{
				nRet = i;
				break;;
			}
		}
	}
	
	return nRet;
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
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
	if (wBufferSize>0) 
		m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//������Ϣ
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//��Ϸ���
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO);
		}

		//�Թ����
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_INFO);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO|SMT_EJECT);
	}
}
//////////////////////////////////////////////////////////////////////////

