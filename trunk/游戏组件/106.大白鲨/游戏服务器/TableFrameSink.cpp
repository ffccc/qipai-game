#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//�궨��
//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//��עʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					20									//����ʱ��

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��
#define TIME_PLACE_JETTON			20									//��עʱ��

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��

#define TIME_GAME_END				22									//����ʱ��
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//��������ת��
BYTE g_cbTurnAreaIndex[][4]={{},
							{7,21},
							{},
							{},
							{3,10,18,25},
							{4,11,17,24},
							{8,16,27},
							{6,15,26},
							{1,12,20},
							{2,13,22},
							{5,23},
							{9,19}};

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	InitializeCriticalSection(&m_csPlaceJetton);
	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_bChangeBanker=false;
	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
	//״̬����
	m_dwJettonTime=0L;
	//ȫ����ע
	m_lALLBigSharkScore=0L;
	m_lALLFeiQinScore=0L;
	m_lALLZouShouScore=0L;
	m_lALLYanZiScore=0L;				
	m_lALLTuZiScore=0L;				
	m_lALLGeZiScore=0L;					
	m_lALLXiongMaoScore=0L;					
	m_lALLKongQueScore=0L;					
	m_lALLHouZiScore=0L;					
	m_lALLLaoYingScore=0L;					
	m_lALLShiZiScore=0L;
	//���Ʊ���
	m_cbWinSideControl=0;
	m_cbMoveEndIndex = 0;
	m_cbResultIndex = 0;
	m_cbEndIndex = 0;
	//������ע
	ZeroMemory(m_lUserBigSharkScore,sizeof(m_lUserBigSharkScore));
	ZeroMemory(m_lUserFeiQinScore,sizeof(m_lUserFeiQinScore));
	ZeroMemory(m_lUserZouShouScore,sizeof(m_lUserZouShouScore));
	ZeroMemory(m_lUserYanZiScore,sizeof(m_lUserYanZiScore));
	ZeroMemory(m_lUserTuZiScore,sizeof(m_lUserTuZiScore));
	ZeroMemory(m_lUserGeZiScore,sizeof(m_lUserGeZiScore));
	ZeroMemory(m_lUserXiongMaoScore,sizeof(m_lUserXiongMaoScore));
	ZeroMemory(m_lUserKongQueScore,sizeof(m_lUserKongQueScore));
	ZeroMemory(m_lUserHouZiScore,sizeof(m_lUserHouZiScore));
	ZeroMemory(m_lUserLaoYingScore,sizeof(m_lUserLaoYingScore));
	ZeroMemory(m_lUserShiZiScore,sizeof(m_lUserShiZiScore));							
	m_cbScoreHistory.RemoveAll();
	m_cbZuoBiIndex = 0;
	m_cbZuoBiCount = 0;
	m_cbLastResultIndex = 0;
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	DeleteCriticalSection(&m_csPlaceJetton);
}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameEvent,Guid,dwQueryVer);
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

	//���ƽӿ�
	m_pITableFrameControl=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ȡ����
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//��ȡ����
	TCHAR szFileName[MAX_PATH];
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\SharkConfig.ini"),szPath);

	//���Ʊ���
	LONG lMaxScore=2147483647;
	m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 100, szFileName);
	m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), lMaxScore, szFileName);
	m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), lMaxScore, szFileName);
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, szFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;
	m_bMaxBankerTime = GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 5, szFileName);

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//ȫ����ע
	m_lALLBigSharkScore=0L;
	m_lALLFeiQinScore=0L;
	m_lALLZouShouScore=0L;
	m_lALLYanZiScore=0L;				
	m_lALLTuZiScore=0L;				
	m_lALLGeZiScore=0L;					
	m_lALLXiongMaoScore=0L;					
	m_lALLKongQueScore=0L;					
	m_lALLHouZiScore=0L;					
	m_lALLLaoYingScore=0L;					
	m_lALLShiZiScore=0L;
	//������ע
	ZeroMemory(m_lUserBigSharkScore,sizeof(m_lUserBigSharkScore));
	ZeroMemory(m_lUserFeiQinScore,sizeof(m_lUserFeiQinScore));
	ZeroMemory(m_lUserZouShouScore,sizeof(m_lUserZouShouScore));
	ZeroMemory(m_lUserYanZiScore,sizeof(m_lUserYanZiScore));
	ZeroMemory(m_lUserTuZiScore,sizeof(m_lUserTuZiScore));
	ZeroMemory(m_lUserGeZiScore,sizeof(m_lUserGeZiScore));
	ZeroMemory(m_lUserXiongMaoScore,sizeof(m_lUserXiongMaoScore));
	ZeroMemory(m_lUserKongQueScore,sizeof(m_lUserKongQueScore));
	ZeroMemory(m_lUserHouZiScore,sizeof(m_lUserHouZiScore));
	ZeroMemory(m_lUserLaoYingScore,sizeof(m_lUserLaoYingScore));
	ZeroMemory(m_lUserShiZiScore,sizeof(m_lUserShiZiScore));

	m_cbMoveEndIndex = 0;
	m_cbResultIndex = 0;
	m_cbEndIndex = 0;
	
	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));
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

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

	//���ñ���
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) 
		GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;

	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}
	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
	case GER_USER_LEFT:		//�û��뿪
		{
			if (cbReason==GER_USER_LEFT)
			{
				//ׯ���ж�
				if (m_wCurrentBanker==wChairID)
				{
					//��ʾ��Ϣ
					TCHAR szTipMsg[128];
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pIServerUserItem->GetAccounts());

					//������Ϣ
					SendGameMessage(INVALID_CHAIR,szTipMsg);	

					//����״̬
					m_pITableFrame->SetGameStatus(GS_GAME_END);
					
					//ֹͣʱ��
					m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);

					//����ʱ��
					m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);
				}
				else
				{
					//��������
					LONG lScore=0;
					LONG lRevenue=0;
					enScoreKind ScoreKind=enScoreKind_Flee;
					
					if (m_pITableFrame->GetGameStatus()==GS_GAME_END)
					{
						//д�����
						//m_pITableFrame->WriteUserScore(pIServerUserItem,m_lUserWinScore[wChairID], m_lUserRevenue[wChairID], ScoreKind);
					}
					else
					{
						//ͳ�Ƴɼ�
						lScore=-( m_lUserYanZiScore[wChairID] + m_lUserTuZiScore[wChairID] + m_lUserGeZiScore[wChairID] + 
							m_lUserXiongMaoScore[wChairID] + m_lUserKongQueScore[wChairID] + m_lUserHouZiScore[wChairID]+
							m_lUserLaoYingScore[wChairID]+m_lUserShiZiScore[wChairID]);

						//�������
						m_lUserYanZiScore[wChairID] = 0;
						m_lUserTuZiScore[wChairID] = 0;
						m_lUserGeZiScore[wChairID] = 0;
						m_lUserXiongMaoScore[wChairID] = 0;
						m_lUserKongQueScore[wChairID] = 0;
						m_lUserHouZiScore[wChairID] = 0;
						m_lUserLaoYingScore[wChairID] = 0;
						m_lUserShiZiScore[wChairID] = 0;
						//д�����
						//m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);
					}
				}
			}

			//ׯ���ж�
			if (cbReason==GER_USER_LEFT && wChairID!=m_wCurrentBanker) return true;

			//��������
			RandGameIndex();

			//�������
			LONG lBankerWinScore=CalculateScore();

			//�����ж�
			//if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker && m_lUserWinScore[m_wCurrentBanker]<0)
			//{
			//	//m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
			//}

			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			
			//ׯ����Ϣ
			GameEnd.nBankerTime = m_wBankerTime;
			GameEnd.lBankerTotallScore=m_lBankerWinScore;
			if (cbReason==GER_USER_LEFT && lBankerWinScore>0) GameEnd.lBankerScore=0;
			else GameEnd.lBankerScore=lBankerWinScore;

			//���ͻ���
			GameEnd.cbTimeLeave=TIME_GAME_END;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserIndex);
				if ( pIServerUserItem == NULL ) continue;
				tagServerUserData *pUserData = pIServerUserItem->GetUserData();

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];
	
				//��������
				GameEnd.cbEndIndex = m_cbEndIndex;
				GameEnd.cbMoveEndIndex = m_cbMoveEndIndex;
				GameEnd.cbResultIndex = m_cbResultIndex;

				//����˰��
				if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
				else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
				else GameEnd.lRevenue=0;
			
				//�����ע��Ϣ
				GameEnd.lUserBigSharkScore = m_lUserBigSharkScore[wUserIndex];
				GameEnd.lUserFeiQinScore = m_lUserFeiQinScore[wUserIndex];
				GameEnd.lUserZouShouScore = m_lUserZouShouScore[wUserIndex];
				GameEnd.lUserYanZiScore = m_lUserYanZiScore[wUserIndex];					
				GameEnd.lUserTuZiScore = m_lUserTuZiScore[wUserIndex];				
				GameEnd.lUserGeZiScore = m_lUserGeZiScore[wUserIndex];					
				GameEnd.lUserXiongMaoScore = m_lUserXiongMaoScore[wUserIndex];				
				GameEnd.lUserKongQueScore = m_lUserKongQueScore[wUserIndex];					
				GameEnd.lUserHouZiScore = m_lUserHouZiScore[wUserIndex];				
				GameEnd.lUserLaoYingScore = m_lUserLaoYingScore[wUserIndex];					
				GameEnd.lUserShiZiScore = m_lUserShiZiScore[wUserIndex];				
	
				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//�л�ׯ��
			//if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker) ChangeBanker(true);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStation, bool bSendSecret)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//���ͼ�¼
			SendGameRecord();

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
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
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
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n��ׯ����Ϊ��%s \n��������Ϊ��%s \n�������Ϊ��%s \n��ׯ����Ϊ��%d"),
				DrawScoreString(m_lApplyBankerCondition),DrawScoreString(m_lAreaLimitScore),DrawScoreString(m_lUserLimitScore),m_bMaxBankerTime);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);



			//����������
			SendApplyUser(pIServerUserItem);
			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord();		

			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//ȫ����ע
			StatusPlay.lALLBigSharkScore=m_lALLBigSharkScore;
			StatusPlay.lALLFeiQinScore=m_lALLFeiQinScore;
			StatusPlay.lALLZouShouScore=m_lALLZouShouScore;
			StatusPlay.lALLYanZiScore=m_lALLYanZiScore;		
			StatusPlay.lALLTuZiScore=m_lALLTuZiScore;		
			StatusPlay.lALLGeZiScore=m_lALLGeZiScore;		
			StatusPlay.lALLXiongMaoScore=m_lALLXiongMaoScore;
			StatusPlay.lALLKongQueScore=m_lALLKongQueScore;	
			StatusPlay.lALLHouZiScore=m_lALLHouZiScore;	
			StatusPlay.lALLLaoYingScore=m_lALLLaoYingScore;	
			StatusPlay.lALLShiZiScore=m_lALLShiZiScore;	

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusPlay.lUserBigSharkScore=m_lUserBigSharkScore[wChiarID];
				StatusPlay.lUserFeiQinScore=m_lUserFeiQinScore[wChiarID];
				StatusPlay.lUserZouShouScore=m_lUserZouShouScore[wChiarID];
				StatusPlay.lUserYanZiScore=m_lUserYanZiScore[wChiarID];			
				StatusPlay.lUserTuZiScore=m_lUserTuZiScore[wChiarID];		
				StatusPlay.lUserGeZiScore=m_lUserGeZiScore[wChiarID];		
				StatusPlay.lUserXiongMaoScore=m_lUserXiongMaoScore[wChiarID];	
				StatusPlay.lUserKongQueScore=m_lUserKongQueScore[wChiarID];	
				StatusPlay.lUserHouZiScore=m_lUserHouZiScore[wChiarID];	
				StatusPlay.lUserLaoYingScore=m_lUserLaoYingScore[wChiarID];		
				StatusPlay.lUserShiZiScore=m_lUserShiZiScore[wChiarID];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
			}
			//�û�״̬
			StatusPlay.cbUserStatus = pIServerUserItem->GetUserStatus();
			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		

			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
				StatusPlay.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
			}	

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusPlay.cbTimeLeave=(BYTE)(TIME_FREE-__min(dwPassTime,TIME_FREE));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//�����ж�
			if (cbGameStation==GS_GAME_END)
			{
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

				//������Ϣ
				StatusPlay.cbEndIndex = m_cbEndIndex;
				StatusPlay.cbMoveEndIndex = m_cbMoveEndIndex;
				StatusPlay.cbResultIndex = m_cbResultIndex;
			}
			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n��ׯ����Ϊ��%s \n��������Ϊ��%s \n�������Ϊ��%s \n��ׯ����Ϊ��%d"),
				DrawScoreString(m_lApplyBankerCondition),DrawScoreString(m_lAreaLimitScore),DrawScoreString(m_lUserLimitScore),m_bMaxBankerTime);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

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
		//	RandGameIndex();
			//��ʼ��Ϸ
			m_pITableFrameControl->StartGame();
			//����ʱ��
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);
			if (m_bChangeBanker)
				SysChangeBanker(true);
			//����״̬
			m_pITableFrame->SetGameStatus(GS_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//����״̬
			m_pITableFrame->SetGameStatus(GS_GAME_END);			

			//������Ϸ
			OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);

			//����ʱ��
			m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);			

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//д�����
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wUserChairID);
				if ( pIServerUserItem == NULL ) continue;
				tagServerUserData *pUserDate = pIServerUserItem->GetUserData();
				if (pUserDate->cbUserStatus!=US_PLAY) continue;
				//ʤ������
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;

				//��ׯ�ж�
				LONG lUserScore=pIServerUserItem->GetUserScore()->lScore;

				//д�����
				if (m_lUserWinScore[wUserChairID]!=0L&&(lUserScore+m_lUserWinScore[wUserChairID])<2000000000) 
					//m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				
				if (wUserChairID!=m_wCurrentBanker && pIServerUserItem->GetUserScore()->lScore<m_lApplyBankerCondition)
				{
					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						if (wUserChairID!=m_ApplyUserArray[i]) continue;

						//ɾ�����
						m_ApplyUserArray.RemoveAt(i);

						//������Ϣ
						CMD_S_CancelBanker CancelBanker;
						ZeroMemory(&CancelBanker,sizeof(CancelBanker));

						//���ñ���
						CancelBanker.wCancelUser=wUserChairID;

						//������Ϣ
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

						//��ʾ��Ϣ
						TCHAR szTipMsg[128];
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%ld��,���Զ���ׯ��"),m_lApplyBankerCondition);
						SendGameMessage(wUserChairID,szTipMsg);

						break;
					}
				}
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//�л�ׯ��
			ChangeBanker(false);
			
			SysChangeBanker(true);
			//������ʷ
			SendGameRecord();

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

			return OnUserApplyBanker(pIServerUserItem);	
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_LOOKON) return true;

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
bool __cdecl CTableFrameSink::OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{
	//�л�ׯ��
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		CancelBanker.wCancelUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;
}
//�û�����
bool __cdecl CTableFrameSink::OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
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
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%ld,��������Ϊ��%ld,�������Ϊ��%ld"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker) ChangeBanker(true);

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser=wChairID;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}

		return true;
	}

	return true;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;

	//��ׯ����
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 5, m_szConfigFileName);

	//ȡ����ǰ
	if (bCancelCurrentBanker)
	{
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
		if (m_pITableFrame->GetGameStatus()==GS_FREE && m_ApplyUserArray.GetCount()!=0)
		{
			m_wCurrentBanker=m_ApplyUserArray[0];
		}

		//���ñ���
		bChangeBanker=true;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		if (m_wCurrentBanker == INVALID_CHAIR)
		{
			m_bChangeBanker = false;//m_bChangeBanker?false:true;
		}
		else
		{
			m_bChangeBanker = true;
		}
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//�����ж�
		if (wBankerTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
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
			if (lBankerScore<m_lApplyBankerCondition)
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%ld)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
			else
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),wBankerTime);

			//������Ϣ
			SendGameMessage(INVALID_CHAIR,szTipMsg);	
		}
	}
	//ϵͳ��ׯ
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
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore()->lScore;
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

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_SHI_ZI)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_SHI_ZI)||(lJettonScore<=0L)) return false;

	//Ч��״̬
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) return true;

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	EnterCriticalSection(&m_csPlaceJetton);

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	LONG lJettonCount=m_lUserBigSharkScore[wChairID]+m_lUserFeiQinScore[wChairID]+m_lUserZouShouScore[wChairID]+
		m_lUserYanZiScore[wChairID]+m_lUserTuZiScore[wChairID]+m_lUserGeZiScore[wChairID]+
		m_lUserXiongMaoScore[wChairID] + m_lUserKongQueScore[wChairID] + m_lUserHouZiScore[wChairID]+
		m_lUserLaoYingScore[wChairID]+m_lUserShiZiScore[wChairID];

	LONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if ( ID_BIG_SHARK == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLBigSharkScore += lJettonScore;
			m_lUserBigSharkScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_FEI_QIN == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLFeiQinScore += lJettonScore;
			m_lUserFeiQinScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_ZOU_SHOU == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLZouShouScore += lJettonScore;
			m_lUserZouShouScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_YAN_ZI == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLYanZiScore += lJettonScore;
			m_lUserYanZiScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_TU_ZI == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLTuZiScore += lJettonScore;
			m_lUserTuZiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_GE_ZI == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLGeZiScore += lJettonScore;
			m_lUserGeZiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_XIONG_MAO == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLXiongMaoScore += lJettonScore;
			m_lUserXiongMaoScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_KONG_QUE == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLKongQueScore += lJettonScore;
			m_lUserKongQueScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HOU_ZI == cbJettonArea )
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLHouZiScore += lJettonScore;
			m_lUserHouZiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if (ID_LAO_YING==cbJettonArea)
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLLaoYingScore+=lJettonScore;
			m_lUserLaoYingScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if (ID_SHI_ZI==cbJettonArea)
	{
		if (GetMaxScore(wChairID,lJettonScore,cbJettonArea) >= 0 )
		{
			//������ע
			m_lALLShiZiScore+=lJettonScore;
			m_lUserShiZiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else
	{
		bPlaceJettonSuccess=false;
	}
	
	LeaveCriticalSection(&m_csPlaceJetton);

	if (bPlaceJettonSuccess)
	{
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
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}
//��������
void CTableFrameSink::RandGameIndex()
{
	srand((unsigned)time(NULL));
	//BYTE cbGaiLv[5];
	//ZeroMemory(cbGaiLv, sizeof(cbGaiLv));;
	//m_cbResultIndex = 0;
	//BYTE cbQuYu = 0;  //����
	//BYTE cbSaveCount = 0; //����
	//LONG lAuto=0;
	////��ȡ����
	//m_cbWinSideControl = GetPrivateProfileInt( TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName );
	//cbGaiLv[0] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("YAN_ZI"), 0, m_szConfigFileName );
	//cbGaiLv[1] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("GE_ZI"), 0, m_szConfigFileName );
	//cbGaiLv[2] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("KONG_QUE"), 0, m_szConfigFileName );
	//cbGaiLv[3] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("LAO_YING"), 0, m_szConfigFileName );
	//cbGaiLv[4] = GetPrivateProfileInt( TEXT("GaiLv"), TEXT("SMALL"), 0, m_szConfigFileName );
	//
	//lAuto = GetPrivateProfileInt( TEXT("Auto"), TEXT("IsAuto"), 0, m_szConfigFileName );
	//cbSaveCount = GetPrivateProfileInt( TEXT("Safe"), TEXT("IsSafe"), 0, m_szConfigFileName ); 

	//if (m_cbWinSideControl!=0 && (cbSaveCount==0||(m_cbZuoBiIndex!=0&&m_cbZuoBiIndex!=m_cbWinSideControl)||m_cbZuoBiCount<cbSaveCount))
	//{
	//	if (m_cbZuoBiIndex==m_cbWinSideControl)
	//		m_cbZuoBiCount++;
	//	else
	//	{
	//		m_cbZuoBiCount = 0;
	//		m_cbZuoBiIndex = m_cbWinSideControl;
	//		m_cbZuoBiCount++;
	//	}
	//	m_cbResultIndex = m_cbWinSideControl;
	//}
	//else
	//{
	//	m_cbZuoBiIndex=0;
	//	LONG cbSunScore = 0;
	//	cbSunScore = m_lALLBigSharkScore+m_lALLFeiQinScore+m_lALLZouShouScore+m_lALLYanZiScore
	//					+m_lALLTuZiScore+m_lALLGeZiScore+m_lALLXiongMaoScore
	//					+m_lALLKongQueScore+m_lALLHouZiScore+m_lALLLaoYingScore+m_lALLShiZiScore;
	//	//������
	//	BYTE cbMultiple[12] = {0, 24, 2, 2, 6, 6, 8, 8, 8, 8, 12, 12};
	//	//�����ע
	//	LONG pUserAllScore[12] = {NULL, m_lALLBigSharkScore,m_lALLFeiQinScore,m_lALLZouShouScore,m_lALLYanZiScore, m_lALLTuZiScore, m_lALLGeZiScore, m_lALLXiongMaoScore, 
	//						  m_lALLKongQueScore, m_lALLHouZiScore,m_lALLLaoYingScore,m_lALLShiZiScore};
	//	//�������
	//	CArrayTemplate<BYTE> cbRelset;
	//	cbRelset.RemoveAll();
	//	//��С��¼
	//	LONG lMinScore = 0L;
	//	if (lAuto!=0&&cbSunScore>=lAuto)//�Զ�����
	//	{
	//		//Ѱ��û����ע������
	//		for (BYTE i=4; i<12; i++)
	//		{
	//			if (pUserAllScore[i]==0)
	//				cbRelset.Add(i);
	//		}
	//		if (cbRelset.GetCount()==0)
	//		{
	//			//Ѱ����С����
	//			for (BYTE i=4; i<12; i++)
	//			{
	//				if (pUserAllScore[i]==0)
	//					continue;
	//				if (lMinScore==0)
	//				{
	//					lMinScore = pUserAllScore[i]*cbMultiple[i];
	//					cbRelset.Add(i);
	//				}
	//				else if (lMinScore==pUserAllScore[i]*cbMultiple[i])
	//				{
	//					cbRelset.Add(i);
	//				}
	//				else if (lMinScore>pUserAllScore[i]*cbMultiple[i])
	//				{
	//					cbRelset.RemoveAll();
	//					cbRelset.Add(i);
	//					lMinScore = pUserAllScore[i]*cbMultiple[i];
	//				}
	//			}
	//			BYTE cbRandIndex = 0;
	//			cbRandIndex = rand()%cbRelset.GetCount();
	//			m_cbResultIndex = cbRelset[cbRandIndex];
	//		}
	//		else
	//		{
	//			BYTE cbRandIndex = 0;
	//			cbRandIndex = rand()%cbRelset.GetCount();
	//			m_cbResultIndex = cbRelset[cbRandIndex];
	//		}
	//	}
	//	else
	//	{
	//		BYTE cbRandIndex[100]; //�������
	//		ZeroMemory(cbRandIndex, sizeof(cbRandIndex));
	//		BYTE cbRand = 0;
	//		for (BYTE i=0; i<4; i++)
	//		{
	//			BYTE j = 0;
	//			while (j<cbGaiLv[i])
	//			{
	//				//srand((unsigned)time(NULL)*10);
	//				cbRand = rand()%50+m_RandGen.NormalRandom(0,50,0,51);
	//				if (cbRandIndex[cbRand]!=0)
	//					continue;
	//				cbRandIndex[cbRand] = i*2+1;
	//				j++;
	//			}
	//		}
	//		BYTE cbResult = m_RandGen.NormalRandom(0,100,0,100);//rand()%50+
	//		if (cbRandIndex[cbResult]!=0)
	//		{
	//			m_cbResultIndex = cbRandIndex[cbResult];
	//		}
	//		else
	//		{	
	//			BYTE cbTemp = rand()%4*2+2;
 //				m_cbResultIndex = cbTemp;
	//		}

	//	}
	//}
	m_cbLastResultIndex = m_cbResultIndex;
	m_cbMoveEndIndex = 28*3+rand()%28;

	BYTE cbAreaIndex=0;
	for (int i=1;i<12;i++)
	{
		for (int j=0;j<4;j++)
		{
			if (g_cbTurnAreaIndex[i][j]==m_cbMoveEndIndex%28 && g_cbTurnAreaIndex[i][j]!=0)
			{
				cbAreaIndex=i;
			}
		}
	}
	//if (m_cbResultIndex>(m_cbMoveEndIndex%8+1))
	//{
	//	m_cbEndIndex = m_cbResultIndex-(m_cbMoveEndIndex%8+1);
	//}
	//else if (m_cbResultIndex<(m_cbMoveEndIndex%8+1))
	//{
	//	m_cbEndIndex = 7-((m_cbMoveEndIndex%8)-m_cbResultIndex);
	//}
	//else
	{
		m_cbEndIndex = 0;
	}
	//��¼��Ϣ
	if (m_cbScoreHistory.GetCount()<MAX_SCORE_HISTORY)
	{
		m_cbScoreHistory.Add(cbAreaIndex-3);
	}
	else
	{
		/*m_cbScoreHistory.RemoveAt(0);*/
		for (BYTE i=0; i<MAX_SCORE_HISTORY-1; i++)
		{
			m_cbScoreHistory[i] = m_cbScoreHistory[i+1];
		}
		m_cbScoreHistory[MAX_SCORE_HISTORY-1] = m_cbResultIndex;//MAX_SCORE_HISTORY
	}
}

//����÷�
LONG CTableFrameSink::CalculateScore()
{
	//��������
	LONG cbRevenue=m_pGameServiceOption->cbRevenue;
	//ׯ������
	LONG lBankerWinScore = 0;
	LONG lUserLostScore[GAME_PLAYER];
	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	BYTE cbAreaIndex=0;
	for (int i=1;i<12;i++)
	{
		for (int j=0;j<4;j++)
		{
			if (g_cbTurnAreaIndex[i][j]==(m_cbMoveEndIndex%28) && g_cbTurnAreaIndex[i][j]!=0)
			{
				cbAreaIndex=i;
			}
		}
	}
	//TCHAR szTemp[64];
	//sprintf(szTemp,"%d %d",cbAreaIndex,m_cbMoveEndIndex);
	//AfxMessageBox(szTemp);
	//������
	BYTE cbMultiple[] = {0, 24, 2, 2, 6, 6, 8, 8, 8, 8, 12, 12};
	//�����ע
	LONG *pUserScore[] = {NULL, m_lUserBigSharkScore,m_lUserFeiQinScore,m_lUserZouShouScore,m_lUserYanZiScore, m_lUserTuZiScore, m_lUserGeZiScore, m_lUserXiongMaoScore, 
		m_lUserKongQueScore, m_lUserHouZiScore,m_lUserLaoYingScore,m_lUserShiZiScore};
	//��ҳɼ�
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;
		for (WORD wAreaIndex=ID_BIG_SHARK; wAreaIndex<=ID_SHI_ZI; wAreaIndex++)
		{
			if (cbAreaIndex==0)
			{
				m_lUserWinScore[i] -= pUserScore[wAreaIndex][i];
			}
			else if (cbAreaIndex==1)
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
			}
			else if (wAreaIndex==cbAreaIndex-3)
			{
				m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
				m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
				//lBankerWinScore -= ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
			}
			else
			{
				m_lUserWinScore[i] -= pUserScore[cbAreaIndex][i];
			}

			if (cbAreaIndex%2==0 && cbAreaIndex>3)
			{
				m_lUserWinScore[i] += ( pUserScore[2][i] * 2 ) ;
				m_lUserReturnScore[i] += pUserScore[2][i] ;
			}
			else
			{
				m_lUserWinScore[i] -= pUserScore[2][i];
			}
			if (cbAreaIndex%2==1 && cbAreaIndex>3)
			{
				m_lUserWinScore[i] += ( pUserScore[3][i] * 2 ) ;
				m_lUserReturnScore[i] += pUserScore[3][i] ;
			}
			else
			{
				m_lUserWinScore[i] -= pUserScore[3][i];
			}

 			//lBankerWinScore += pUserScore[wAreaIndex][i];
		}
		m_lUserWinScore[i] -= m_lUserReturnScore[i];
		//����˰��
		if (0 < m_lUserWinScore[i])
		{
			m_lUserRevenue[i]  = m_lUserWinScore[i]*cbRevenue/100;
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

	}
	//ׯ�ҳɼ�
	//if (m_wCurrentBanker!=INVALID_CHAIR)
	//{
	//	m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

	//	//����˰��
	//	if (0 < m_lUserWinScore[m_wCurrentBanker])
	//	{
	//		
	//		m_lUserRevenue[m_wCurrentBanker]  =m_lUserWinScore[m_wCurrentBanker]*cbRevenue/100;
	//		m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
	//		lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
	//	}		
	//}

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
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
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser=wChairID;

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
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (!pServerUserItem) continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord()
{
	BYTE cbSocreHistroy[MAX_SCORE_HISTORY];
	ZeroMemory(cbSocreHistroy, sizeof(cbSocreHistroy));
	for (BYTE i=0; i<m_cbScoreHistory.GetCount(); i++)
	{
		cbSocreHistroy[i] = m_cbScoreHistory[i];
	}

	CMD_S_ScoreHistory ScoreHistory;
	ZeroMemory(&ScoreHistory, sizeof(ScoreHistory));

	CopyMemory(ScoreHistory.cbScoreHistroy,cbSocreHistroy,sizeof(ScoreHistory.cbScoreHistroy));

	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SCORE_HISTORY, &cbSocreHistroy, sizeof(cbSocreHistroy));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SCORE_HISTORY, &cbSocreHistroy, sizeof(cbSocreHistroy));
}

//�����ע
LONG CTableFrameSink::GetMaxScore(WORD wChairID, LONG lJettonScore,BYTE cbArea)
{
	//�������
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	LONG lAreaScore[]={0,m_lALLBigSharkScore,m_lALLFeiQinScore,m_lALLZouShouScore,
		m_lALLYanZiScore,m_lALLTuZiScore,m_lALLGeZiScore,m_lALLXiongMaoScore,
		m_lALLKongQueScore,m_lALLHouZiScore,m_lALLLaoYingScore,m_lALLShiZiScore};

	//��������
	LONG lAreaLimit = m_lAreaLimitScore - lAreaScore[cbArea];

	LONG lMaxJetton=0L;
	lMaxJetton=min(lUserMaxJetton,lAreaLimit);

	return lMaxJetton;
}

//�����ע
LONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//����ע��
	LONG lNowJetton = 0;
	lNowJetton += m_lUserBigSharkScore[wChairID];
	lNowJetton += m_lUserFeiQinScore[wChairID];
	lNowJetton += m_lUserZouShouScore[wChairID];
	lNowJetton += m_lUserYanZiScore[wChairID];
	lNowJetton += m_lUserTuZiScore[wChairID];
	lNowJetton += m_lUserGeZiScore[wChairID];
	lNowJetton += m_lUserXiongMaoScore[wChairID];
	lNowJetton += m_lUserKongQueScore[wChairID];
	lNowJetton += m_lUserHouZiScore[wChairID];
	lNowJetton += m_lUserLaoYingScore[wChairID];
	lNowJetton += m_lUserShiZiScore[wChairID];

	//��ȡ���
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//�����ע
	LONG lMeMaxScore = min(pIMeServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
	lMeMaxScore -= lNowJetton;
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

void CTableFrameSink::SysChangeBanker(bool bCancelCurrentBanker)
{
	//��ׯ����
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 5, m_szConfigFileName);
	
	if ((m_wBankerTime<wBankerTime&&m_wBankerTime!=0) || !m_bEnableSysBanker || m_wCurrentBanker!=INVALID_CHAIR)
		return;
	
	
	//ϵͳ��ׯ
	if (bCancelCurrentBanker)
	{
		//���ñ���
		//m_bEnableSysBanker = true;
		m_wBankerTime = 0;
		m_bChangeBanker = false;
		//������Ϣ
		CMD_S_ChangeSysBanker ChangeSysBanker;
		ZeroMemory(&ChangeSysBanker,sizeof(ChangeSysBanker));
		ChangeSysBanker.bEnableSysBanker = true;
		ChangeSysBanker.cbBankerTime = m_wBankerTime;

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_SYS_BANKER,&ChangeSysBanker,sizeof(ChangeSysBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_SYS_BANKER,&ChangeSysBanker,sizeof(ChangeSysBanker));

		//��ʾ��Ϣ
		TCHAR szTipMsg[128];
		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[������]��ׯ"));
		//������Ϣ
		SendGameMessage(INVALID_CHAIR,szTipMsg);
	}
	else
	{
		//���ñ���
		//m_bEnableSysBanker = false;
		m_wBankerTime = 0;
		m_bChangeBanker = true;
		//������Ϣ
		CMD_S_ChangeSysBanker ChangeSysBanker;
		ZeroMemory(&ChangeSysBanker,sizeof(ChangeSysBanker));
		ChangeSysBanker.bEnableSysBanker = false;
		ChangeSysBanker.cbBankerTime = m_wBankerTime;

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_SYS_BANKER,&ChangeSysBanker,sizeof(ChangeSysBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_SYS_BANKER,&ChangeSysBanker,sizeof(ChangeSysBanker));

		//��ʾ��Ϣ
		TCHAR szTipMsg[128];
		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ ������]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),wBankerTime);
		//������Ϣ
		SendGameMessage(INVALID_CHAIR,szTipMsg);
	}
}

//�����Ϣ
CString CTableFrameSink::DrawScoreString(LONG lNumber)
{
	CString strNumber=TEXT("");
	strNumber.Format(TEXT("%ld"),lNumber>0?lNumber:-lNumber);
	int p=strNumber.GetLength()-4;
	while(p>0)
	{
		strNumber.Insert(p,",");
		p-=4;
	}
	if(lNumber<0)
	{
		strNumber=TEXT("-")+strNumber;
	}
	return strNumber;
}
//////////////////////////////////////////////////////////////////////////
