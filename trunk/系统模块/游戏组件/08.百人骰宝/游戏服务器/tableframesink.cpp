#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���


//��עʱ��
#define IDI_FREE					1									//����ʱ��
#ifdef _DEBUG
#define TIME_FREE					10									//����ʱ��
#else
#define TIME_FREE					10									//����ʱ��
#endif

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��

#ifdef _DEBUG
#define TIME_PLACE_JETTON			15									//��עʱ��
#else
#define TIME_PLACE_JETTON			20									//��עʱ��
#endif

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��

#ifdef _DEBUG
#define TIME_GAME_END				20									//����ʱ��
#else
#define TIME_GAME_END				20									//����ʱ��
#endif

#define  MONEY_NAME               TEXT("����")                          // ��������

//////////////////////////////////////////////////////////////////////////
#define _GFIX(a) (a>0?a:0)
#define _GABS(a) (a>0?a:-a)


CString FormatNumber(__int64 i64)
{
	CString str;
	str.Format(TEXT("%I64d"),i64>=0?i64:-i64);
	//int p=str.GetLength()-4;
	//while(p>0)
	//{
	//	str.Insert(p,TEXT(","));
	//	p-=4;
	//}
	if(i64<0)
		str=TEXT("-")+str;
	return str;
}
//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////
//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��	
	ZeroMemory(m_lAllScore,sizeof(m_lAllScore));

	//������ע
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));

	//���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;
	m_iNTWinPercent = 0;
	m_arPensonInfo.RemoveAll();
	
	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_enCards,sizeof(m_enCards));
	ZeroMemory(m_bWinner,sizeof(m_bWinner));   // ��Ӯ���

	//״̬����
	m_dwJettonTime=0L;

	//ׯ����Ϣ
	m_wApplyUserArray.RemoveAll();
	m_iMaxAndroidBanker=MAX_ANDROID_BANKER;
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_lAllAndroidWinScore = 0;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\SicBoConfig.ini"),szPath);

	//���Ʊ���
	__int64 lMaxScore=9223372036854775807;
	CString str;

	TCHAR szBuffer[MAX_PATH];
	//m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 100, m_szConfigFileName);
	GetPrivateProfileString(TEXT("BankerCondition"),TEXT("Score"),TEXT("100"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_lApplyBankerCondition=_atoi64(szBuffer);

	//m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), lMaxScore, m_szConfigFileName);
	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("AreaLimitScore"),TEXT("5000000000000000000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_lAreaLimitScore=_atoi64(szBuffer);

	//m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), lMaxScore, m_szConfigFileName);
	GetPrivateProfileString(TEXT("ScoreLimit"),TEXT("UserLimitScore"),TEXT("50000000000000000"),szBuffer,CountArray(szBuffer),m_szConfigFileName);
	m_lUserLimitScore= _atoi64(szBuffer);
	BYTE byCtrlWinner = GetPrivateProfileInt(TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName);
	m_cbWinSideControl=byCtrlWinner;
	m_iNTWinPercent = GetPrivateProfileInt(TEXT("ManagerControl"), TEXT("NTWinPercent"), 0, m_szConfigFileName);
	//m_lMaxAndroidLost = GetPrivateProfileInt(TEXT("ManagerControl"), TEXT("MaxAndroidLost"), 80000000, m_szConfigFileName);
	//m_lMaxAndroidLost = 0-m_lMaxAndroidLost;
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;
	m_bChangeSicboType=false;                      // �Ƿ��Ѿ��ı�ҡ���ӵķ�ʽ
	m_enSicboType=enSicboType_Sicbo;               // ҡ���ӵķ�ʽ
	m_bIsAllAndroid = false;
	m_iSameCount = 0;
	m_iAndroidRandCount = 0;
	m_iAndroidNTCount = -1;
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

	//���ƽӿ�
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	TCHAR szMsg[_MAX_PATH]={"ManagerControl"};
	if (NULL != m_pGameServiceOption)
	{
		_sntprintf(szMsg, sizeof(szMsg), TEXT("ManagerControl%d"), m_pGameServiceOption->wServerID);
	}	
	m_iNTWinPercent = GetPrivateProfileInt(szMsg, TEXT("NTWinPercent"), m_iNTWinPercent, m_szConfigFileName);

	m_iAILostLimitPercent=GetPrivateProfileInt("ScoreLimit","AILostLimitPercent",600,m_szConfigFileName);

	m_iSameCount =  0;
	m_iAndroidRandCount= 0;
	m_iAndroidNTCount = -1;
	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//����ע��	
	ZeroMemory(m_lAllScore,sizeof(m_lAllScore));

	//������ע
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	m_bIsAllAndroid = false;
	m_iAndroidRandCount = 0;
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
	m_iUseWinner=enCardType_Illegal;
	m_iAndroidRandCount=0;

// ���ؿ��Ʊ���
	m_arPensonInfo.RemoveAll();
	//��ȡ����
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	//��ȡ����
	TCHAR szFileName[MAX_PATH];
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\UserList.ini"),szPath);
	int iPersonNum = GetPrivateProfileInt( TEXT("Winner"), TEXT("PersonCount"), 0, szFileName );
	if (iPersonNum>50)  // ������50��
	{
		iPersonNum = 50;
	}

	// ��ֵ̫��������Сһ��
	if (m_lAllAndroidWinScore>9999999999999L)
	{
		m_lAllAndroidWinScore=8000000000000;
	}

	// �������ļ��ļ��ж�ȡ�������
	for (int i=0; i<iPersonNum; ++i)
	{
		TCHAR szKeyName[_MAX_PATH]={0};
		S_PERSONINFO sPensonInfo;
		ZeroMemory(&sPensonInfo, sizeof(sPensonInfo));
		_sntprintf(szKeyName, sizeof(szKeyName), "UserID%d",(i+1));
		// ��ȡ�������
		sPensonInfo.dwUserID = GetPrivateProfileInt( TEXT("Winner"), szKeyName, 0, szFileName );
		if (0 == sPensonInfo.dwUserID)  // Ϊ��ʱ������
		{
			continue;
		}
		// ����ʤ�ʵĶ�ȡ
		_sntprintf(szKeyName, sizeof(szKeyName), "Percent%d",(i+1));
		sPensonInfo.iPercent = GetPrivateProfileInt( TEXT("Winner"), szKeyName, 0, szFileName );
		// ��ʼ����λ��
		m_arPensonInfo.Add(sPensonInfo);
	}
	m_iAILostLimitPercent=GetPrivateProfileInt("ScoreLimit","AILostLimitPercent",600,m_szConfigFileName);

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker)
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
	}
	ZeroMemory(m_bWinner,sizeof(m_bWinner));        // ��Ӯ���
	m_bChangeSicboType=false;                       // �Ƿ��Ѿ��ı�ҡ���ӵķ�ʽ
	m_enSicboType=enSicboType_Sicbo;                // ҡ���ӵķ�ʽ
	
	//���ñ���
	GameStart.cbTimeLeave=(BYTE)TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL)
	{
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;
		GameStart.lAllMaxScore =GameStart.lBankerScore;
		if (m_iAndroidNTCount<0) // ���ׯ�Ҷ���
		{
			BOOL bHaveUser = false;
			for (INT i=0; i<m_wApplyUserArray.GetCount(); ++i)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wApplyUserArray[i]);
				if (NULL != pIServerUserItem)
				{
					if (!pIServerUserItem->IsAndroidUser())
					{
						bHaveUser = true;
						break;
					}
				}
			}
			if (bHaveUser)
			{
				m_iAndroidNTCount = rand()%3 + 1;
			}
			else
			{
				m_iAndroidNTCount = -1;
			}
		}
	}
	else
	{
		GameStart.lAllMaxScore = m_lAreaLimitScore;
	}

    //�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	m_bIsAllAndroid = true;
	bool bAllUser = true;  // ���е��˶������
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->IsAndroidUser())
		{
			bAllUser =false;
		}
		else
		{
			m_bIsAllAndroid = false;
		}

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);

		// ������ׯʱ���������ͨ��ʱ�Ƿ��㹻
		if (pIBankerServerUserItem!=NULL)
		{
			GameStart.lUserMaxScore = min(GameStart.lBankerScore, GameStart.lUserMaxScore);
		}
		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}
	// ���еĶ�����ң������ж��ǻ�������ͬ
	if (bAllUser)
	{
		m_bIsAllAndroid=true;
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
			//�������
			__int64 lBankerWinScore=CalculateScore();

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			GameEnd.lBankerScore=lBankerWinScore;

			// ���ӵ���
			CopyMemory(GameEnd.enCards, m_enCards,sizeof(m_enCards));
			for (int i=0; i<COUNT_AZIMUTH; ++i)
			{
				GameEnd.bWinner[i] = m_bWinner[i];
			}

			//���ͻ���
			GameEnd.cbTimeLeave=TIME_GAME_END;
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];				

				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEFT:		//�û��뿪
		{
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				__int64 lScore=0;
				__int64 lRevenue=0;
				enScoreKind ScoreKind=enScoreKind_Flee;

				//ͳ�Ƴɼ�, ÿ����λ��ע��ֵͳ��
				for (int i=0; i<COUNT_AZIMUTH; ++i)
				{
					lScore += m_lUserScore[i][wChairID];
					//����Ĳ�����������ķ���
					//lScore -= m_lUserScore[i][wChairID] * CGameLogic::m_i64Loss_Percent[i];
					m_lUserScore[i][wChairID]=-m_lUserScore[i][wChairID];
				}

				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
				if(lScore>pIServerUserItem->GetUserData()->UserScoreInfo.lScore)
					lScore=pIServerUserItem->GetUserData()->UserScoreInfo.lScore;

				//д�����
				if (0L != lScore)
				{
					m_pITableFrame->WriteUserScore(pIServerUserItem, -lScore,lRevenue, ScoreKind);
				}

				return true;
			}

			//״̬�ж�
			if (GS_GAME_END != m_pITableFrame->GetGameStatus())
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
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);

				//�������
				CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (m_lBankerWinScore>0) GameEnd.lBankerScore=0;

				// ���ӵ���
				CopyMemory(GameEnd.enCards, m_enCards,sizeof(m_enCards));
				for (int i=0; i<COUNT_AZIMUTH; ++i)
				{
					GameEnd.bWinner[i] = m_bWinner[i];
				}

				//���ͻ���
				GameEnd.cbTimeLeave=TIME_GAME_END;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//���óɼ�
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//��������
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//����˰��
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			if (m_lUserWinScore[m_wCurrentBanker]<0)
			{
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
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
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;

			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIBankerItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIBankerItem->GetUserScore()->lScore;
			}

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore); 
			}

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);
			
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
			CopyMemory(StatusPlay.lAllScore, m_lAllScore, sizeof(m_lAllScore));

			IServerUserItem *pIBankerItem = NULL;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				pIBankerItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIBankerItem->GetUserScore()->lScore;
				StatusPlay.lAllMaxScore = StatusPlay.lBankerScore;
			}
			else
			{
				StatusPlay.lAllMaxScore = m_lAreaLimitScore;
			}

			for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
			{
				StatusPlay.lUserScore[i] = m_lUserScore[i][wChiarID];
			}

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{				
				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
				if (GS_PLACE_JETTON == cbGameStatus)
				{
					// �����Ѿ��¹�����ע
					//���û���
					StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);

					// ������ׯʱ���������ͨ��ʱ�Ƿ��㹻
					if (pIServerUserItem!=NULL)
					{
						StatusPlay.lUserMaxScore = min(StatusPlay.lBankerScore, StatusPlay.lUserMaxScore);
					}
				}
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;	

			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(TIME_PLACE_JETTON-__min(dwPassTime,TIME_PLACE_JETTON));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			
			StatusPlay.bySicboType = (BYTE)m_enSicboType;

			//�����ж�
			if (cbGameStatus==GS_GAME_END)
			{
				StatusPlay.cbTimeLeave=(BYTE)(TIME_GAME_END-__min(dwPassTime,TIME_GAME_END));
				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChiarID];

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChiarID];

				//����˰��
				if (m_lUserRevenue[wChiarID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChiarID];
				else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				// ���ӵ���
				CopyMemory(StatusPlay.enCards, m_enCards,sizeof(m_enCards));

				for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
				{
					StatusPlay.bWinner[i] = m_bWinner[i];
				}
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
	case IDI_FREE:		//����ʱ��
		{
			//��ʼ��Ϸ
			m_pITableFrameControl->StartGame();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);

			//����״̬
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);
			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus()!=GS_GAME_END)
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GS_GAME_END);				
				
				// ������ׯʱ��������Ӯ
				CtrlWinner();

				//������Ϸ
				OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
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
				if ( pIServerUserItem == NULL ) continue;

				//ʤ������
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//д�����
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//��ׯ�ж�
				__int64 lUserScore=pIServerUserItem->GetUserScore()->lScore;
				if (wUserChairID!=m_wCurrentBanker && lUserScore<m_lApplyBankerCondition)
				{
					for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_wApplyUserArray[i]) continue;

						//ɾ�����
						m_wApplyUserArray.RemoveAt(i);

						//������Ϣ
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//���ñ���
						lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));
						CancelBanker.dwUserID = pIServerUserItem->GetUserID();

						//������Ϣ
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//��ʾ��Ϣ
						TCHAR szTipMsg[_MAX_PATH]={0};
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������%s��������ׯ����%s����%I64d��,���Զ���ׯ��"),
							MONEY_NAME, MONEY_NAME, m_lApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			// �ж϶������Ƿ���ׯ
			--m_iAndroidNTCount;
			bool bChangeNT = false;
			if (0 == m_iAndroidNTCount)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				if (NULL != pIServerUserItem)
				{
					if (pIServerUserItem->IsAndroidUser())
					{
						bChangeNT = true;
					}
				}
			}

			//�л�ׯ��
			ChangeBanker(bChangeNT);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE,TIME_FREE*1000,1,0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=TIME_FREE;
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
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;
			if ((0 != wDataSize) && (m_wApplyUserArray.GetCount()>m_iMaxAndroidBanker))
			{
				return true;
			}
			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

			if (0 != wDataSize)				
			{
				if (m_wCurrentBanker == pIServerUserItem->GetChairID())
				{
					return true;
				}
				if (m_wApplyUserArray.GetCount()<=(m_iMaxAndroidBanker+1))
				{
					return true;
				}
			}
			return OnUserCancelBanker(pIServerUserItem);	
		}
	case SUB_C_CHANGE_SICBO_TYPE:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ChangeSicboType));
			if (wDataSize!=sizeof(CMD_C_ChangeSicboType)) return false;

			return true;

			////�û�Ч��
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			//if (pUserData->cbUserStatus!=US_PLAY)
			//{
			//	return true;
			//}

			//// ����ׯ�Ҳ��ܷ�
			//if (pUserData->wChairID != m_wCurrentBanker)
			//{
			//	return true;
			//}

			//// �����ˣ������ٷ�
			//if (m_bChangeSicboType)
			//{
			//	return true;
			//}

			////��Ϣ����
			//CMD_C_ChangeSicboType * pChangeSicboType=(CMD_C_ChangeSicboType *)pDataBuffer;
			//if ((pChangeSicboType->enSicboType <enSicboType_Desktop)
			//	|| (pChangeSicboType->enSicboType > enSicboType_InTheAir))
			//{
			//	return true;
			//}
			//m_bChangeSicboType = true;
			//m_enSicboType=pChangeSicboType->enSicboType;   // ҡ���ӵķ�ʽ
			////������Ϣ
			//m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_C_CHANGE_SICBO_TYPE,(void*)pChangeSicboType, sizeof(CMD_C_ChangeSicboType));
			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_C_CHANGE_SICBO_TYPE, (void*)pChangeSicboType, sizeof(CMD_C_ChangeSicboType));
			//return true;
		}
	case SUB_C_USE_WINNER:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UseWinner));
			if (wDataSize!=sizeof(CMD_C_UseWinner)) return false;
			// ����ע״̬������
			if (GS_PLACE_JETTON != m_pITableFrame->GetGameStatus())
			{
				return true;
			}
			//��Ϣ����
			CMD_C_UseWinner * pUseWinner=(CMD_C_UseWinner *)pDataBuffer;
			if ((pUseWinner->byArea>= COUNT_AZIMUTH)
				|| ((USER_LOSER != pUseWinner->byType)
				&& (USER_WINNER != pUseWinner->byType)))
			{
				return false;
			}

			// ��ǰ�˺Ų��ǿ����˺��򲻴���
			for (int i=0; i<m_arPensonInfo.GetCount(); ++i)
			{
				if (pIServerUserItem->GetUserID() == m_arPensonInfo[i].dwUserID)
				{
					// ����ʤ��
					m_iUseWinner = pUseWinner->byArea;
					break;
				}
			}
			return true;
		}
	default:
		break;
	}
	OUTPUT_DEBUG_STRING(_T("server ���յ���� ������Ϣ"));
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
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�˷�����ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
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
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//ȡ������
		for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_wApplyUserArray[i]) continue;

			//ɾ�����
			m_wApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));
			CancelBanker.dwUserID = pIServerUserItem->GetUserID();

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
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<COUNT_AZIMUTH)&&(lJettonScore>0L));
	if ((cbJettonArea>=COUNT_AZIMUTH)||(lJettonScore<=0L))
	{
		return false;
	}

	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON)
	{
		goto PlaceJettonFail;
	}

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR)
	{
		goto PlaceJettonFail;
	}

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (NULL == pIServerUserItem)
	{
		goto PlaceJettonFail;
	}

	int iArea = cbJettonArea;
	__int64 iMaxUserScore = GetMaxPlaceJetton(iArea, wChairID);
	if (iMaxUserScore < lJettonScore)
	{
		goto PlaceJettonFail;
	}
	//��������
	CMD_S_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.wChairID=wChairID;
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;
	m_lUserScore[cbJettonArea][wChairID] += lJettonScore;
	m_lAllScore[cbJettonArea] += lJettonScore;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	return true;

PlaceJettonFail:

	CMD_S_PlaceJettonFail PlaceJettonFail;
	ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
	PlaceJettonFail.lJettonArea=cbJettonArea;
	PlaceJettonFail.lPlaceScore=lJettonScore;

	//������Ϣ
	m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	__int64 lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
	{
		TCHAR szMsg[_MAX_PATH]={0};
		_sntprintf(szMsg, sizeof(szMsg), TEXT("���%s����������ׯ�ң�����ʧ�ܣ�"), MONEY_NAME);
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,szMsg,SMT_INFO|SMT_EJECT);
		return true;
	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_wApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_wApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,
				TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_INFO|SMT_EJECT);
			return true;
		}
	}

	//������Ϣ 
	m_wApplyUserArray.Add(wApplyUserChairID);

	//�������
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//���ñ���
	ApplyBanker.wApplyUser=wApplyUserChairID;
	ApplyBanker.dwUserID = pIApplyServerUserItem->GetUserID();

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//�л��ж�
	if (m_pITableFrame->GetGameStatus()==GS_FREE && m_wApplyUserArray.GetCount()==1)
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
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,
			TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_wApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_wApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));
			CancelBanker.dwUserID = pIServerUserItem->GetUserID();

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
	m_iMaxAndroidBanker = 1+ rand()%MAX_ANDROID_BANKER;
	
	//��ׯ����
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 10, m_szConfigFileName);

	//ȡ����ǰ
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_wApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) continue;

			//ɾ�����
			m_wApplyUserArray.RemoveAt(i);

			break;
		}

		//����ׯ��
		m_wCurrentBanker=INVALID_CHAIR;

		//�ֻ��ж�
		if (m_pITableFrame->GetGameStatus()==GS_FREE && m_wApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_wApplyUserArray[0];
		}

		//���ñ���
		bChangeBanker=true;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		__int64 lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//�����ж�
		if (wBankerTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
		{
			//�������
			for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
			{
				//��ȡ���
				WORD wChairID=m_wApplyUserArray[i];

				//��������
				if (wChairID!=m_wCurrentBanker) continue;

				//ɾ�����
				m_wApplyUserArray.RemoveAt(i);

				break;
			}

			//����ׯ��
			m_wCurrentBanker=INVALID_CHAIR;

			//�ֻ��ж�
			if (m_wApplyUserArray.GetCount()!=0)
			{
				m_wCurrentBanker=m_wApplyUserArray[0];
			}

			bChangeBanker=true;

			//��ʾ��Ϣ
			TCHAR szTipMsg[128];
			if (lBankerScore<m_lApplyBankerCondition)
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
			else
            	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),wBankerTime);
			
			//������Ϣ
			SendGameMessage(INVALID_CHAIR,szTipMsg);	
		}
	}
	//ϵͳ��ׯ
	else if (m_wCurrentBanker==INVALID_CHAIR && m_wApplyUserArray.GetCount()!=0)
	{
		m_wCurrentBanker=m_wApplyUserArray[0];
		bChangeBanker=true;
	}

	//�л��ж�
	if (bChangeBanker)
	{		
		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			ChangeBanker.dwUserID = pIServerUserItem->GetUserID();
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
				m_iAndroidNTCount = -1;
			}
		}
	}

	return bChangeBanker;
}



//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_wApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_wApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (!pServerUserItem) continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;
		ApplyBanker.dwUserID = pServerUserItem->GetUserID();

		//������Ϣ
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_wApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_wApplyUserArray[i]) continue;

		//ɾ�����
		m_wApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		lstrcpyn(CancelBanker.szCancelUser,pIServerUserItem->GetAccounts(),CountArray(CancelBanker.szCancelUser));
		CancelBanker.dwUserID = pIServerUserItem->GetUserID();

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}

//����÷�
__int64 CTableFrameSink::CalculateScore(void)
{
	//��������
	__int64 cbRevenue=m_pGameServiceOption->wRevenue;
	
	//�ƶ����
	ZeroMemory(&m_bWinner, sizeof(m_bWinner));
	DeduceWinner(m_bWinner);

	// ��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	memcpy(&GameRecord.enCards, m_enCards, sizeof(m_enCards));
	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
	}

	//ׯ������
	__int64 lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	__int64 lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	for (int j=0; j<COUNT_AZIMUTH; ++j)
	{
		lBankerWinScore += m_lAllScore[j];
	}

	//�������
	for (WORD i=0; i<GAME_PLAYER; ++i)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		int j=0;
		for (int j=0; j<COUNT_AZIMUTH; ++j )
		{
			lUserLostScore[i] -= _GFIX(m_lUserScore[j][i]);
			// �������Ƿ�Ӯ��
			if (m_bWinner[j])
			{
				__int64 i64WinScore = 0;
				__int64 i64TmpScore = 0;
				if ((j<enCardType_SicboOne) ||(j>enCardType_SicboSix))
				{
					if(m_lUserScore[j][i]>0)
						i64WinScore = (m_lUserScore[j][i] * CGameLogic::m_i64Loss_Percent[j]);
					else
						i64TmpScore = (-m_lUserScore[j][i]) * CGameLogic::m_i64Loss_Percent[j];
				}
				else
				{
					// ��Ҫ�������
					E_SICBO_NUMBER enSicboNum = (E_SICBO_NUMBER)(j-enCardType_SicboOne+1);
					int iCountNum = m_GameLogic.GetSicboCountByNumber(m_enCards, enSicboNum);
					int iMuti[]={0,2,3,4};
					if(m_lUserScore[j][i]>0)
						i64WinScore = (m_lUserScore[j][i] * iMuti[iCountNum]);
					else
						i64TmpScore = (-m_lUserScore[j][i]) * iMuti[iCountNum];
				}
				m_lUserWinScore[i] += i64WinScore;
				m_lUserReturnScore[i] += (m_lUserScore[j][i]>0)?m_lUserScore[j][i]:0;
				lBankerWinScore -= i64WinScore-i64TmpScore;
			}
			//else
			//{
			//	__int64 i64LoseScore = m_lUserScore[j][i];
			//	lUserLostScore[i] -= i64LoseScore;
				//lBankerWinScore  += i64LoseScore;
			//}
		}

		//����˰��
		if (0 < m_lUserWinScore[i])
		{
			m_lUserRevenue[i]  = (m_lUserWinScore[i]*cbRevenue)/1000L;
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//�ܵķ���
		m_lUserWinScore[i] += lUserLostScore[i];

		if (!m_bIsAllAndroid && (INVALID_CHAIR != m_wCurrentBanker)
			&& pIServerUserItem->IsAndroidUser())
		{
			m_lAllAndroidWinScore += m_lUserWinScore[i];
		}

		OUTPUT_DEBUG_STRING(_T("Server ���������ע %I64d,  ��ǰ���%d��ע %I64d ��Ӯ%I64d"),
			lBankerWinScore, i, -lUserLostScore[i], m_lUserWinScore[i]);
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			m_lUserRevenue[m_wCurrentBanker]  = (m_lUserWinScore[m_wCurrentBanker]*cbRevenue)/1000L;
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (NULL != pIServerUserItem)
		{
			if (!m_bIsAllAndroid && pIServerUserItem->IsAndroidUser())
			{
				m_lAllAndroidWinScore += m_lUserWinScore[m_wCurrentBanker];
			}
		}
	}

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	// ��ǰ����
	m_lBankerCurGameScore = lBankerWinScore;

	return lBankerWinScore;
}

void CTableFrameSink::DeduceWinner(bool bWinner[])
{
	// ��ʼ������
	E_CARD_TYPE enCardResult[COUNT_AZIMUTH];
	int i=0;
	for (i=0; i<COUNT_AZIMUTH; ++i)
	{
		enCardResult[i]=enCardType_Illegal;
	}

	int iTypeCount = 0; // ��������
	m_GameLogic.GetCardType(m_enCards, enCardResult, iTypeCount);
	for (i=0; i<iTypeCount; ++i)
	{
		if (enCardType_Illegal != enCardResult[i])
		{
			bWinner[enCardResult[i]]=true;
		}
	}
}

// ������Ӯ
void CTableFrameSink::CtrlWinner(void)
{
	// �鿴�Ƿ���Ҫ����
	E_CARD_TYPE  enCardType[COUNT_AZIMUTH];
	int iCardTypeCount = 0;
	INT i=0;
	for (i=0; i<COUNT_AZIMUTH; ++i)
	{
		enCardType[i] = enCardType_Illegal;
	}
	do 
	{
		m_enCards[0]=(E_SICBO_NUMBER)(rand()%6+1);
		m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
		m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);		
		int iCountNum = m_enCards[0] + m_enCards[1] + m_enCards[2];
		if (((m_enCards[0] == m_enCards[1]) && (m_enCards[1]==m_enCards[2]))  // ������ͬ
			|| (enCardType_NumberFour==iCountNum) || (enCardType_NumberSeventeen==iCountNum))
		{
			++m_iSameCount;
			if (m_iSameCount < 30)
			{
				Sleep(10);
				continue;
			}
			m_iSameCount = 0;
		}
	} while (false);

	if (enCardType_Illegal == m_iUseWinner)
	{
		// ����ׯ����
		//CtrlNTWinner();

		int iRate=rand()%1000;
		// ���������̫��
		if (m_lAllAndroidWinScore<0 && iRate<=m_iAILostLimitPercent)
		{
			// ������һ��ҪӮǮ
			CtrlAndroidWinner();
		}
		return ;
	}

	m_GameLogic.GetCardType(m_enCards, enCardType, iCardTypeCount);
	for (i=0; i<iCardTypeCount; ++i)
	{
		if (enCardType[i] == m_iUseWinner)
		{
			return ;
		}
	}
	int iCountRand = 0;
	switch (m_iUseWinner)
	{
	case enCardType_Big:
	case enCardType_Small:
	case enCardType_Single:
	case enCardType_Double:
		{
			do 
			{
				m_enCards[0]=(E_SICBO_NUMBER)(rand()%6+1);
				m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
				m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
				m_GameLogic.GetCardType(m_enCards, enCardType, iCardTypeCount);
				for (i=0; i<iCardTypeCount; ++i)
				{
					if (enCardType[i] == m_iUseWinner)
					{
						return ;
					}
				}
				++iCountRand;
				if (iCountRand < 500)  // 500�ζ�û�����
				{
					continue;
				}
			} while (false);
			break;
		}
	case enCardType_NumberFour:
		{
			// 1 2 1 ���
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]= enSicboNum_Two;
			m_enCards[2]= enSicboNum_One;			
			break;
		}
	case enCardType_NumberSeventeen:
		{
			m_enCards[0]= enSicboNum_Six;
			m_enCards[1]= enSicboNum_Five;
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboOne:
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboTwo:
		{
			m_enCards[0]= enSicboNum_Two;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboThree:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]=enSicboNum_Three;
			m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboFour:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]=enSicboNum_Four;
			m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboFive:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]=enSicboNum_Five;
			break;
		}
	case enCardType_SicboSix:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]=enSicboNum_Six;
			break;
		}
	case enCardType_SicboOneAndTwo:  // ����1,2
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Two;
			break;
		}
	case enCardType_SicboOneAndThree:       // ����1,3
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Three;
			break;
		}
	case enCardType_SicboOneAndFour:       // ����1,4
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Four;
			break;
		}
	case enCardType_SicboOneAndFive:        // ����1,5
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]=enSicboNum_Five;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboOneAndSix:         // ����1,6{
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]= enSicboNum_Six;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboTwoAndThree:       // ����2,3
		{
			m_enCards[0]= enSicboNum_Two;
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboTwoAndFour:        // ����2,4
		{
			m_enCards[0]= enSicboNum_Two;
			m_enCards[1]= enSicboNum_Four;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%6+1);
			break;
		}
	case enCardType_SicboTwoAndFive:        // ����2,5
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Two;
			m_enCards[2]= enSicboNum_Five;
			break;
		}
	case enCardType_SicboTwoAndSix:         // ����2,6
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Two;
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboThreeAndFour:      // ����3,4
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= enSicboNum_Four;
			break;
		}
	case enCardType_SicboThreeAndFive:      // ����3,5
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= enSicboNum_Five;
			break;
		}
	case enCardType_SicboThreeAndSix:       // ����3,6
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboFourAndFive:       // ����4,5
		{
			m_enCards[0]= enSicboNum_Five;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Four;
			break;
		}
	case enCardType_SicboFourAndSix:        // ����4,6
		{
			m_enCards[0]= enSicboNum_Six;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Four;
			break;
		}
	case enCardType_SicboFiveAndSix:        // ����5,6
		{
			m_enCards[0]= enSicboNum_Five;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%6+1);
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboDoubleOne:
		{
			m_enCards[0]= enSicboNum_One;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%5+1);
			if (m_enCards[0]==m_enCards[1])
			{
				m_enCards[1] = enSicboNum_Six;
			}
			m_enCards[2]= enSicboNum_One;
			break;
		}
	case enCardType_SicboDoubleTwo:
		{
			m_enCards[0]= enSicboNum_Two;
			m_enCards[1]= (E_SICBO_NUMBER)(rand()%5+1);
			m_enCards[2]= enSicboNum_Two;
			if (m_enCards[0]==m_enCards[1])
			{
				m_enCards[1] = enSicboNum_Six;
			}
			break;
		}
	case enCardType_SicboDoubleThree:
		{
			m_enCards[0]= enSicboNum_Three;
			m_enCards[1]= enSicboNum_Three;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%5+1);
			if (m_enCards[0]==m_enCards[2])
			{
				m_enCards[2] = enSicboNum_Six;
			}
			break;
		}
	case enCardType_SicboDoubleFour:
		{
			m_enCards[0]= enSicboNum_Four;
			m_enCards[1]= enSicboNum_Four;
			m_enCards[2]= (E_SICBO_NUMBER)(rand()%5+1);
			if (m_enCards[0]==m_enCards[2])
			{
				m_enCards[2] = enSicboNum_Six;
			}
			break;
		}
	case enCardType_SicboDoubleFive:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%5+1);
			m_enCards[1]= enSicboNum_Five;
			m_enCards[2]= enSicboNum_Five;
			if (m_enCards[0]==m_enCards[1])
			{
				m_enCards[0] = enSicboNum_Six;
			}
			break;
		}
	case enCardType_SicboDoubleSix:
		{
			m_enCards[0]= (E_SICBO_NUMBER)(rand()%5+1);
			m_enCards[1]= enSicboNum_Six;
			m_enCards[2]= enSicboNum_Six;
			break;
		}
	case enCardType_SicboThreeOne:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_One;
			break;
		}
	case enCardType_SicboThreeTwo:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Two;
			break;
		}
	case enCardType_SicboThreeThree:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Three;
			break;
		}
	case enCardType_SicboThreeFour:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Four;
			break;
		}
	case enCardType_SicboThreeFive:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Five;
			break;
		}
	case enCardType_SicboThreeSix:
		{
			m_enCards[0]=m_enCards[1]=m_enCards[2]=enSicboNum_Six;
			break;
		}
	default: break;
	}
	iCountRand = 3;
	while (iCountRand>0)
	{
		int iIndex = rand()%iCountRand;
		if (iIndex != (iCountRand-1))
		{
			E_SICBO_NUMBER enTemp = m_enCards[iIndex];
			m_enCards[iIndex] = m_enCards[iCountRand-1];
			m_enCards[iCountRand-1] = enTemp;
		}
		--iCountRand;
	}
}

// ����ϵͳ��ׯ��Ӯ
void CTableFrameSink::CtrlNTWinner(void)
{
	// Ŀǰֻ����ϵͳ��ׯʱ����Ӯ���
	if (INVALID_CHAIR != m_wCurrentBanker)
	{
		return ;
	}

	// 85%�Ļ���Ӯ
	if ((0 == m_cbWinSideControl) || (rand()%100 > m_iNTWinPercent))
	{
		return ;
	}

	// ��û��ע�Ͳ��ÿ���ʤ����
	bool bAllZero = true;
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		if (m_lAllScore[i] > 0)
		{
			bAllZero = false;
			break;
		}
	}
	if (bAllZero)
	{
		return ;
	}

	//BYTE byLoseMin = ID_BANKER;
	//__int64 lMinLoseScore=0;

	//// ��Ӯ���
	//bool bWinner[COUNT_AZIMUTH];
	//for (int j=ID_BANKER; j>=ID_EAST; --j)
	//{
	//	BYTE byMaxCard = 0;
	//	BYTE byCount = MAX_COUNT_SICBO;
	//	E_CARD_TYPE enCardType[(COUNT_AZIMUTH+1)];
	//	enCardType[ID_BANKER] = enCardType_SicboThreeAndFive;//m_GameLogic.GetCardType(m_byCards[j], byCount, byMaxCard);
	//	for (BYTE i=ID_EAST; i<ID_BANKER; ++i)
	//	{
	//		BYTE byIndex = (i == j)?ID_BANKER:i;  // �����ׯ�ң���Ӧ����ׯ�ҵ��Ƶ��滻
	//		E_COMPARE_RESULT enCompRet = enCompResult_Less;//m_GameLogic.CompareCards(m_byCards[j], MAX_COUNT_SICBO, m_byCards[byIndex], MAX_COUNT_SICBO);
	//		bWinner[i] = (enCompRet > enCompResult_Less)?false:true;
	//		enCardType[i] = enCardType_SicboThreeAndFive;//m_GameLogic.GetCardType(m_byCards[byIndex], byCount, byMaxCard);
	//	}

	//	// �鿴��Ӯ���
	//	__int64 lBankerWinScore = 0;

	//	//���͵ı���
	//	BYTE cbMultiple[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

	//	//�������
	//	for (WORD i=0; i<GAME_PLAYER; ++i)
	//	{
	//		//ׯ���ж�
	//		if (m_wCurrentBanker==i) continue;

	//		//��ȡ�û�
	//		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
	//		if (pIServerUserItem==NULL) continue;

	//		for ( WORD wAreaIndex = ID_EAST; wAreaIndex <= ID_NORTH; ++wAreaIndex )
	//		{
	//			// �������Ƿ�Ӯ�ˣ���������
	//			if (bWinner[wAreaIndex])
	//			{
	//				__int64 i64WinScore = (m_lUserScore[wAreaIndex][i] * cbMultiple[enCardType[wAreaIndex]]);
	//				lBankerWinScore -= i64WinScore;
	//			}
	//			else
	//			{
	//				__int64 i64LoseScore = (m_lUserScore[wAreaIndex][i] * cbMultiple[enCardType[ID_BANKER]]);
	//				lBankerWinScore  += i64LoseScore;
	//			}
	//		}
	//	}

	//	if ((ID_BANKER == j) && (lBankerWinScore > 0))
	//	{
	//		return ;
	//	}

	//	if (lBankerWinScore > 0)
	//	{
	//		// ��ׯ�ҵ��Ƶ���
	//		BYTE byCardsTemp[MAX_COUNT_SICBO];
	//		memcpy(byCardsTemp, m_byCards[ID_BANKER], MAX_COUNT_SICBO);
	//		memcpy(m_byCards[ID_BANKER],m_byCards[j], MAX_COUNT_SICBO);
	//		memcpy(m_byCards[j],byCardsTemp, MAX_COUNT_SICBO);
	//		return ;
	//	}
	//	if (ID_BANKER == j)
	//	{
	//		lMinLoseScore = lBankerWinScore;
	//	}
	//	else if (lMinLoseScore < lBankerWinScore)
	//	{
	//		lMinLoseScore = lBankerWinScore;
	//		byLoseMin = j;
	//	}
	//}

	//if (ID_BANKER != byLoseMin)
	//{
	//	// ��ׯ�ҵ��Ƶ���
	//	BYTE byCardsTemp[MAX_COUNT_SICBO];
	//	memcpy(byCardsTemp, m_byCards[ID_BANKER], MAX_COUNT_SICBO);
	//	memcpy(m_byCards[ID_BANKER],m_byCards[byLoseMin], MAX_COUNT_SICBO);
	//	memcpy(m_byCards[byLoseMin],byCardsTemp, MAX_COUNT_SICBO);
	//}
}

// ���ƻ�����ҪӮ
void CTableFrameSink::CtrlAndroidWinner(void)
{
	if (INVALID_CHAIR == m_wCurrentBanker)
	{
		return ;
	}

	// ���е����ǻ����˲��ÿ���
	if (m_bIsAllAndroid)
	{
		return;
	}

	// û������ׯ
	IServerUserItem * pIBankerItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
	if (NULL == pIBankerItem)
	{
		return ;
	}

	// ��û��ע�Ͳ��ÿ���ʤ����
	bool bAllZero = true;
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		if (m_lAllScore[i] > 0)
		{
			bAllZero = false;
			break;
		}
	}
	if (bAllZero)
	{
		return ;
	}

	//��������
	__int64 cbRevenue=m_pGameServiceOption->wRevenue;

	//�ƶ����
	bool bWinnerTemp[COUNT_AZIMUTH];
	ZeroMemory(&bWinnerTemp, sizeof(bWinnerTemp));
	DeduceWinner(bWinnerTemp);

	//ׯ������
	__int64 lBankerWinScore = 0;

	//��ҳɼ�
	__int64 lAroidWinerScore=0;
	__int64 lUserWinerScore[GAME_PLAYER];
	__int64 lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserWinerScore, sizeof(lUserWinerScore));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	for (int j=0; j<COUNT_AZIMUTH; ++j)
	{
		lBankerWinScore += m_lAllScore[j];
	}	

	//�������
	for (WORD i=0; i<GAME_PLAYER; ++i)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		int j=0;
		for (int j=0; j<COUNT_AZIMUTH; ++j )
		{
			lUserLostScore[i] -= m_lUserScore[j][i];
			// �������Ƿ�Ӯ��
			if (bWinnerTemp[j])
			{
				__int64 i64WinScore = 0;
				if ((j<enCardType_SicboOne) ||(j>enCardType_SicboSix))
				{
					i64WinScore = (m_lUserScore[j][i] * CGameLogic::m_i64Loss_Percent[j]);
				}
				else
				{
					// ��Ҫ�������
					E_SICBO_NUMBER enSicboNum = (E_SICBO_NUMBER)(j-enCardType_SicboOne+1);
					int iCountNum = m_GameLogic.GetSicboCountByNumber(m_enCards, enSicboNum);
					int iMuti[]={0,2,3,4};
					i64WinScore = (m_lUserScore[j][i] * iMuti[iCountNum]);
				}
				lUserWinerScore[i] += i64WinScore;
				lBankerWinScore -= i64WinScore;
			}
		}

		// ����˰��
		if (0 < lUserWinerScore[i])
		{
			lUserWinerScore[i] -= ((lUserWinerScore[i]*cbRevenue)/1000L);
		}

		//�ܵķ���
		lUserWinerScore[i] += lUserLostScore[i];

		if (!m_bIsAllAndroid && (INVALID_CHAIR != m_wCurrentBanker)
			&& pIServerUserItem->IsAndroidUser())
		{
			lAroidWinerScore += lUserWinerScore[i];
		}
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{

		//����˰��
		if (0 < lBankerWinScore)
		{
			lBankerWinScore -= ((lBankerWinScore*cbRevenue)/1000L);
		}
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		if (NULL != pIServerUserItem)
		{
			if (!m_bIsAllAndroid && pIServerUserItem->IsAndroidUser())
			{
				lAroidWinerScore += lBankerWinScore;
			}
		}
	}

	if ((lAroidWinerScore>0) || (m_iAndroidRandCount>100))
	{
		return ;
	}
	++m_iAndroidRandCount;
	m_enCards[0]=(E_SICBO_NUMBER)(rand()%6+1);
	m_enCards[1]=(E_SICBO_NUMBER)(rand()%6+1);
	m_enCards[2]=(E_SICBO_NUMBER)(rand()%6+1);
	CtrlAndroidWinner();
	return ;
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[SOCKET_BUFFER];
	int nIndex = m_nRecordFirst;
	int nLast = (m_pITableFrame->GetGameStatus()==GS_GAME_END)?((m_nRecordLast+MAX_SCORE_HISTORY-1)%MAX_SCORE_HISTORY):m_nRecordLast;
	while ( nIndex != nLast )
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

// ��ȡÿ�����������ע��
__int64 CTableFrameSink::GetMaxPlaceJetton(int iArea, WORD wChairID)
{
	if ((iArea < 0) || (iArea>=COUNT_AZIMUTH))
	{
		return 0L;
	}

	CSingleLock SingleLockHandle(&m_CriticalSection);
	SingleLockHandle.Lock();
	__int64 i64UserAllScore = 0;    // ����Լ��Ѿ���ע�ķ�
	__int64 i64AllUserAllScore=0;   // ���������ע�� 
	__int64 i64AllLossScore=0;      // ׯ��������
	__int64 i64LoseAreaScore=0;     // ׯ����������ܵ���ע��
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		i64UserAllScore += m_lUserScore[i][wChairID];
		i64AllUserAllScore += m_lAllScore[i];
	}

	for (int i=1; i<=MAX_SICBO_NUMBER; ++i)
	{
		for (int j=i; j<=MAX_SICBO_NUMBER; ++j)
		{
			for (int h=j; h<=MAX_SICBO_NUMBER; ++h)
			{
				E_SICBO_NUMBER enBuffer[]={(E_SICBO_NUMBER)i,(E_SICBO_NUMBER)j,(E_SICBO_NUMBER)h};
				E_CARD_TYPE enCardType[COUNT_AZIMUTH];
				int iCount = 0;
				m_GameLogic.GetCardType(enBuffer,enCardType, iCount);
				__int64 i64AllLossScoreTemp=0;
				__int64 i64LoseAreaScoreTemp=0;
				bool bFind = false;
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enBuffer[l])
					{
						continue;
					}
					if (iArea == enCardType[l])
					{
						bFind = true;
					}
				}
				if (!bFind)
				{
					continue;
				}

				// ��������������Ǯ
				for (int l=0; l<iCount; ++l)
				{
					if (enCardType_Illegal == enCardType[l])
					{
						continue;
					}
					i64LoseAreaScoreTemp += m_lAllScore[enCardType[l]];   // ׯ�����������Ҫ�ѽ�ҿ۳�
					if ((enCardType[l]<enCardType_SicboOne) ||(enCardType[l]>enCardType_SicboSix))
					{
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * m_GameLogic.m_i64Loss_Percent[enCardType[l]]);
					}
					else
					{
						// ��Ҫ�������
						E_SICBO_NUMBER enSicboNum = (E_SICBO_NUMBER)(enCardType[l]-enCardType_SicboOne+1);
						int iCountNum = m_GameLogic.GetSicboCountByNumber(enBuffer, enSicboNum);
						int iMuti[]={0,2,3,4};
						i64AllLossScoreTemp += (m_lAllScore[enCardType[l]] * iMuti[iCountNum]);
					}
				}
				
				if (i64AllLossScoreTemp > i64AllLossScore)
				{
					i64AllLossScore = i64AllLossScoreTemp;
				}

				if (i64LoseAreaScoreTemp>i64LoseAreaScore)
				{
					i64LoseAreaScore = i64LoseAreaScoreTemp;
				}
			}
		}
	}

	// ��ȡ��������
	__int64 lAllMaxScore = 0;
	__int64 lUserMaxScore = 0;
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker)
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
	}

	if (NULL != pIBankerServerUserItem)
	{
		lAllMaxScore = pIBankerServerUserItem->GetUserScore()->lScore;
	}
	else
	{
		lAllMaxScore = m_lAreaLimitScore;
	}


	IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if (NULL == pIServerUserItem)
	{
		return 0L;
	}

	// ���û���
	lUserMaxScore = min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
	// ��ȥ�Ѿ���ע��
	lUserMaxScore -= i64UserAllScore;
	ASSERT(lUserMaxScore>0);
	if (lUserMaxScore<=0)
	{
		return 0L;
	}

	OUTPUT_DEBUG_STRING(_T("Server �����ע %I64d, ���������ע %I64d,  ׯ�ҵ�ǰ�� %I64d  �����ע����%d"),
		lAllMaxScore, i64AllUserAllScore, i64AllLossScore, iArea);

	// ׯ�ҿ������Ǯ
	lAllMaxScore = lAllMaxScore - i64LoseAreaScore + i64AllUserAllScore;
	lAllMaxScore -= i64AllLossScore;
	if (lAllMaxScore<0)
	{
		return 0L;
	}

	if ((iArea<enCardType_SicboOne) ||(iArea>enCardType_SicboSix))
	{
		lAllMaxScore = lAllMaxScore/(m_GameLogic.m_i64Loss_Percent[iArea] -1);
	}
	else
	{
		lAllMaxScore = lAllMaxScore/3; // ���������
	}
	//ASSERT(lAllMaxScore>=0);
	if (lAllMaxScore<0)
	{
		return 0L;
	}
	lUserMaxScore = min(lAllMaxScore, lUserMaxScore);
	return lUserMaxScore;

}
//////////////////////////////////////////////////////////////////////////

