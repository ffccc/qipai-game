#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���

//��������
#define INDEX_BANKER				0									//ׯ������
#define INDEX_PLAYER1				1									//��
#define INDEX_PLAYER2				2									//��
#define INDEX_PLAYER3				3									//��
#define INDEX_PLAYER4				4									//��


//��עʱ��
#define IDI_FREE					1									//����ʱ��
#ifdef _DEBUG
#define TIME_FREE					4									//����ʱ��
#else
//#define TIME_FREE					30//10									//����ʱ��
//test
#define TIME_FREE					20									//����ʱ��
#endif

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��

#ifdef _DEBUG
#define TIME_PLACE_JETTON			18									//��עʱ��
#else
#define TIME_PLACE_JETTON			18									//��עʱ��
#endif

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��

#ifdef _DEBUG
#define TIME_GAME_END				12									//����ʱ��
#else
#define TIME_GAME_END				10									//����ʱ��
#endif

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//����ע��
	m_lTianScore=0L;							//��
	m_lDiScore=0L;								//��
	m_lXuanScore=0L;							//��
	m_lHuangScore=0L;							//��


	//���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	//������ע
	ZeroMemory(m_lUserTianScore,sizeof(m_lUserTianScore));
	ZeroMemory(m_lUserDiScore,sizeof(m_lUserDiScore));
	ZeroMemory(m_lUserXuanScore,sizeof(m_lUserXuanScore));
	ZeroMemory(m_lUserHuangScore,sizeof(m_lUserHuangScore));


	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	//״̬����
	m_dwJettonTime=0L;

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;


	ZeroMemory(m_cbTimers,sizeof(m_cbTimers));

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\BaiRenSanGongConfig.ini"),szPath);

	//���Ʊ���
	LONG lMaxScore=2147483647;
	m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 100, m_szConfigFileName);
	m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), lMaxScore, m_szConfigFileName);
	m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), lMaxScore, m_szConfigFileName);
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	InitializeCriticalSection(&m_csPlaceJetton);


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
	m_lTianScore=0L;							//��
	m_lDiScore=0L;								//��
	m_lXuanScore=0L;							//��
	m_lHuangScore=0L;							//��

	//������ע
	ZeroMemory(m_lUserTianScore,sizeof(m_lUserTianScore));
	ZeroMemory(m_lUserDiScore,sizeof(m_lUserDiScore));
	ZeroMemory(m_lUserXuanScore,sizeof(m_lUserXuanScore));
	ZeroMemory(m_lUserHuangScore,sizeof(m_lUserHuangScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	ZeroMemory(m_cbTimers,sizeof(m_cbTimers));

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
	if (INVALID_CHAIR!=m_wCurrentBanker) pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);

	//���ñ���
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;

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
			if (cbReason==GER_USER_LEFT  && GS_PLACE_JETTON == m_pITableFrame->GetGameStatus())
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

					//����ʱ��
					m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
					m_pITableFrame->SetGameTimer(IDI_GAME_END,TIME_GAME_END*1000,1,0L);
				}
			}
			//	else
			//	{

			//		//if(GS_PLACE_JETTON == m_pITableFrame->GetGameStatus())
			//		//{

			//		//}

			//		//��������
			//		LONG lScore=0;
			//		LONG lRevenue=0;
			//		enScoreKind ScoreKind=enScoreKind_Flee;

			//		//ͳ�Ƴɼ�
			//		/*lScore=-( m_lUserTieScore[wChairID] + m_lUserBankerScore[wChairID] + m_lUserPlayerScore[wChairID] + 
			//			m_lUserBankerKingScore[wChairID] + m_lUserPlayerKingScore[wChairID] + m_lUserTieSamePointScore[wChairID]+
			//			m_lUserPlayerTwoPair[wChairID]+m_lUserBankerTwoPair[wChairID]);*/
			//		//--ͳ�Ƴɼ�  ���ﻹû������
			//		lScore=-( m_lUserTianScore[wChairID] + m_lUserDiScore[wChairID] + m_lUserXuanScore[wChairID] + 
			//			m_lUserHuangScore[wChairID]);

			//		//д�����
			//		if (lScore!=0L) m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);
			//	}
			//}

			//ׯ���ж�
			//if (cbReason==GER_USER_LEFT && wChairID!=m_wCurrentBanker) return true;
			
			LONG lBankerWinScore=0L;
			if(cbReason!=GER_USER_LEFT || wChairID==m_wCurrentBanker)
			{
				//�ɷ��˿�
				DispatchTableCard();

				/*CString str;
				::OutputDebugString("Server Dispach card:");
				for(BYTE i=0; i<6; i++)
				{
					str.Format("User:%d Card:%x, %x, %x", i, m_cbTableCardArray[i][0], m_cbTableCardArray[i][1], m_cbTableCardArray[i][2]);
					::OutputDebugString(str);
				}*/

				//�������
				lBankerWinScore=CalculateScore();
				/*::OutputDebugString("���ɼ�:");
				str.Format("%d", lBankerWinScore);
				::OutputDebugString(str);*/
			}

			
//////////////////////////

			if(cbReason==GER_USER_LEFT && wChairID!=m_wCurrentBanker)
			{
				//ʤ������
				enScoreKind ScoreKind=(m_lUserWinScore[wChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;
				//д�����
				if (m_lUserWinScore[wChairID]!=0L) m_pITableFrame->WriteUserScore(wChairID,m_lUserWinScore[wChairID], m_lUserRevenue[wChairID], ScoreKind);
				return true;
			}
///////////////////////////
			//�����ж�
			//if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker && m_lUserWinScore[m_wCurrentBanker]<0)
			if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker)
			{
				enScoreKind ScoreKind=(m_lUserWinScore[m_wCurrentBanker]>0L)?enScoreKind_Win:enScoreKind_Lost;
				//m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], ScoreKind);
			}

			//��������
			m_wBankerTime++;

			//if(cbReason!=GER_USER_LEFT)
			{
				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				if (cbReason==GER_USER_LEFT && lBankerWinScore>0) GameEnd.lBankerScore=0;
				else GameEnd.lBankerScore=lBankerWinScore;

				//�˿���Ϣ
				CopyMemory(GameEnd.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));

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

					/*CString str;
					::OutputDebugString("Server Game end:");
					for(BYTE i=0; i<6; i++)
					{
						str.Format("User:%d Card:%x, %x, %x", i, GameEnd.cbTableCardArray[i][0], GameEnd.cbTableCardArray[i][1], GameEnd.cbTableCardArray[i][2]);
						::OutputDebugString(str);
					}*/
					/*str.Format("�ɼ�:%d, %d, %d", GameEnd.lUserScore, GameEnd.lBankerScore, GameEnd.lBankerTotallScore);
					::OutputDebugString(str);*/
					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}

			}

			//�л�ׯ��
			if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker) ChangeBanker(true);

			//��¼���
			//try
			//{
			//	//�����ļ���
			//	TCHAR szPath[MAX_PATH]=TEXT("");
			//	TCHAR szFileName[MAX_PATH];
			//	GetCurrentDirectory(sizeof(szPath),szPath);
			//	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\��Ϸ��¼%ld.txt"),szPath,m_dwRecordCount++/66);
			//	if (m_FileRecord.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
			//	{
			//		TCHAR *szRecord[4]={TEXT(""),TEXT("1\r\n"),TEXT("2\r\n"),TEXT("0\r\n")};

			//		//�ƶ����
			//		BYTE cbWinner,cbKingWinner;
			//		bool bPlayerTwoPair,bBankerTwoPair;
			//		DeduceWinner(cbWinner, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);		
			//		m_FileRecord.SeekToEnd();
			//		m_FileRecord.Write(szRecord[cbWinner],CountStringBuffer(szRecord[cbWinner]));

			//		m_FileRecord.Close();
			//	}
			//}
			//catch(...)
			//{
			//}

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
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%ld,��������Ϊ��%ld,�������Ϊ��%ld"),m_lApplyBankerCondition,
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
			//StatusPlay.lAllTieScore=m_lTieScore;		
			//StatusPlay.lAllBankerScore=m_lBankerScore;		
			//StatusPlay.lAllPlayerScore=m_lPlayerScore;		
			//StatusPlay.lAllTieSamePointScore=m_lTieSamePointScore;
			//StatusPlay.lAllBankerKingScore=m_lBankerKingScore;	
			//StatusPlay.lAllPlayerKingScore=m_lPlayerKingScore;	
			//StatusPlay.lAllPlayerTwoPair=m_lPlayerTwoPair;	
			//StatusPlay.lAllBankerTwoPair=m_lBankerTwoPair;
			StatusPlay.lAllTianScore=m_lTianScore;
			StatusPlay.lAllDiScore=m_lDiScore;
			StatusPlay.lAllXuanScore=m_lXuanScore;
			StatusPlay.lAllHuangScore=m_lHuangScore;

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				//StatusPlay.lUserTieScore=m_lUserTieScore[wChiarID];			
				//StatusPlay.lUserBankerScore=m_lUserBankerScore[wChiarID];		
				//StatusPlay.lUserPlayerScore=m_lUserPlayerScore[wChiarID];		
				//StatusPlay.lUserTieSamePointScore=m_lUserTieSamePointScore[wChiarID];	
				//StatusPlay.lUserBankerKingScore=m_lUserBankerKingScore[wChiarID];	
				//StatusPlay.lUserPlayerKingScore=m_lUserPlayerKingScore[wChiarID];	
				//StatusPlay.lUserPlayerTwoPair=m_lUserPlayerTwoPair[wChiarID];		
				//StatusPlay.lUserBankerTwoPair=m_lUserBankerTwoPair[wChiarID];
				StatusPlay.lUserTianScore=m_lUserTianScore[wChiarID];
				StatusPlay.lUserDiScore=m_lUserDiScore[wChiarID];
				StatusPlay.lUserXuanScore=m_lUserXuanScore[wChiarID];
				StatusPlay.lUserHuangScore=m_lUserHuangScore[wChiarID];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore()->lScore,m_lUserLimitScore);
			}

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
			if (cbGameStatus==GS_GAME_END)
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

				//�˿���Ϣ
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
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
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON,TIME_PLACE_JETTON*1000,1,0L);

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

				//ʤ������
				enScoreKind ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?enScoreKind_Win:enScoreKind_Lost;


				//д�����
				if (m_lUserWinScore[wUserChairID]!=0L) m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//��ׯ�ж�
				LONG lUserScore=pIServerUserItem->GetUserScore()->lScore;
				if (wUserChairID!=m_wCurrentBanker && lUserScore<m_lApplyBankerCondition)
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
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�������������������ׯ������������%ld��,���Զ���ׯ��"),m_lApplyBankerCondition);
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
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%ld,��������Ϊ��%ld,�������Ϊ��%ld"),m_lApplyBankerCondition,
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

//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONG lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_HUANG_MARK)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_HUANG_MARK)||(lJettonScore<=0L)) return false;

	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) return true;

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	EnterCriticalSection(&m_csPlaceJetton);
	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	//LONG lJettonCount=m_lUserTieScore[wChairID]+m_lUserBankerScore[wChairID]+m_lUserPlayerScore[wChairID]+
	//	m_lUserTieSamePointScore[wChairID] + m_lUserPlayerKingScore[wChairID] + m_lUserBankerKingScore[wChairID]+
	//	m_lUserPlayerTwoPair[wChairID]+m_lUserBankerTwoPair[wChairID];

	//--�Ƿ񻹿��Լ�ע
	LONG lJettonCount=m_lUserTianScore[wChairID]+m_lUserDiScore[wChairID]+m_lUserXuanScore[wChairID]+
		m_lUserHuangScore[wChairID];

	LONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//�Ϸ�У��
	if (lUserScore < lJettonCount *10 + lJettonScore*10) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;
//--��ע����

	//�Ϸ���֤
	if ( ID_TIAN_MARK == cbJettonArea )
	{
		if ( GetMaxTianScore(wChairID) >= lJettonScore )
		{
			//������ע
			m_lTianScore += lJettonScore;
			m_lUserTianScore[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_DI_MARK == cbJettonArea )
	{
		if ( GetMaxDiScore(wChairID) >= lJettonScore ) 
		{
			//������ע
			m_lDiScore += lJettonScore;
			m_lUserDiScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_XUAN_MARK == cbJettonArea )
	{
		if ( GetMaxXuanScore(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lXuanScore += lJettonScore;
			m_lUserXuanScore[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HUANG_MARK == cbJettonArea )
	{
		if ( GetMaxHuangScore(wChairID)>=lJettonScore )
		{
			//������ע
			m_lHuangScore += lJettonScore;
			m_lUserHuangScore[wChairID] += lJettonScore;
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

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	srand(time(NULL));
	m_GameLogic.RandCardList(m_cbTableCardArray[0],sizeof(m_cbTableCardArray)/sizeof(m_cbTableCardArray[0][0]));
	//�״η���
	
/*
	//�������
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);

	//�мҲ���
	BYTE cbPlayerThirdCardValue = 0 ;	//�������Ƶ���
	if(cbPlayerTwoCardCount<=5 && cbBankerCount<8)
	{
		//�������
		m_cbCardCount[INDEX_PLAYER]++;
		cbPlayerThirdCardValue = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_PLAYER][2]);
	}
*/

	////��ȡ����
	//m_cbWinSideControl = GetPrivateProfileInt( TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName );

	////������Ӯ
	//if ( m_cbWinSideControl != 0 )
	//{
	//	//�ƶ���Ӯ
	//	BYTE cbWinner;
	//	DeduceWinner(cbWinner);

	//	//�ط��ж�
	//	if ( cbWinner!=m_cbWinSideControl && m_nSendCardCount < SEND_COUNT )
	//	{
	//		m_nSendCardCount++;
	//		DispatchTableCard();
	//	}
	//	else
	//	{
	//		m_nSendCardCount=0;
	//	}

	//}

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
	if (lUserScore<m_lApplyBankerCondition)
	{
		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("������Ӳ���������ׯ�ң�����ʧ�ܣ�"),SMT_INFO|SMT_EJECT);
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
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;
	
	//��ׯ����
	WORD wBankerTime=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Time"), 10, m_szConfigFileName);

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

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
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

//�����ע
LONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//����ע��
	LONG lNowJetton = 0;
	//lNowJetton += m_lUserPlayerScore[wChairID];
	//lNowJetton += m_lUserPlayerKingScore[wChairID];
	//lNowJetton += m_lUserBankerScore[wChairID];
	//lNowJetton += m_lUserTieScore[wChairID];
	//lNowJetton += m_lUserTieSamePointScore[wChairID];
	//lNowJetton += m_lUserBankerKingScore[wChairID];
	//lNowJetton += m_lUserPlayerTwoPair[wChairID];
	//lNowJetton += m_lUserBankerTwoPair[wChairID];
	lNowJetton +=m_lUserTianScore[wChairID];
	lNowJetton +=m_lUserDiScore[wChairID];
	lNowJetton +=m_lUserXuanScore[wChairID];
	lNowJetton +=m_lUserHuangScore[wChairID];

	//��ȡ���
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	LONG lMeMaxScore=pIMeServerUserItem->GetUserScore()->lScore-lNowJetton*10;

	lMeMaxScore=min(lMeMaxScore, m_lUserLimitScore);
	//�����ע
	//LONG lMeMaxScore = min(pIMeServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
	//lMeMaxScore -= lNowJetton*10;
	//ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//�����ע
LONG CTableFrameSink::GetMaxTianScore(WORD wChairID)//��
{
	//�������
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONG lAreaLimit = m_lAreaLimitScore - m_lTianScore;

	//�����ע
	LONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
//		LONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONG lMaxTianScore = lBankerScore;
		lMaxTianScore -=(m_lTianScore*10+m_lDiScore*10+m_lXuanScore*10+m_lHuangScore*10);
		//lMaxTianScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxTianScore>=0);

		//�����ע
		lMaxJetton = min(lMaxTianScore/10, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONG CTableFrameSink::GetMaxDiScore(WORD wChairID)
{	

	//�������
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONG lAreaLimit = m_lAreaLimitScore - m_lDiScore;

	//�����ע
	LONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		//		LONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONG lMaxTianScore = lBankerScore;
		lMaxTianScore -=(m_lTianScore*10+m_lDiScore*10+m_lXuanScore*10+m_lHuangScore*10);
		//lMaxTianScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxTianScore>=0);

		//�����ע
		lMaxJetton = min(lMaxTianScore/10, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONG CTableFrameSink::GetMaxXuanScore(WORD wChairID)
{
	//�������
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONG lAreaLimit = m_lAreaLimitScore - m_lXuanScore;

	//�����ע
	LONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		//		LONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONG lMaxTianScore = lBankerScore;
		lMaxTianScore -=(m_lTianScore*10+m_lDiScore*10+m_lXuanScore*10+m_lHuangScore*10);
		//lMaxTianScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxTianScore>=0);

		//�����ע
		lMaxJetton = min(lMaxTianScore/10, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONG CTableFrameSink::GetMaxHuangScore(WORD wChairID)
{
	//�������
	LONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONG lAreaLimit = m_lAreaLimitScore - m_lHuangScore;

	//�����ע
	LONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		//		LONG lOtherAreaScore = m_lBankerScore+ m_lTieScore+ m_lTieSamePointScore+ m_lBankerKingScore;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONG lMaxTianScore = lBankerScore;
		lMaxTianScore -=(m_lTianScore*10+m_lDiScore*10+m_lXuanScore*10+m_lHuangScore*10);
		//lMaxTianScore -= (m_lPlayerScore+m_lPlayerKingScore*2+(m_lPlayerTwoPair+m_lBankerTwoPair)*RATE_TWO_PAIR);
		ASSERT(lMaxTianScore>=0);

		//�����ע
		lMaxJetton = min(lMaxTianScore/10, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//����÷�
LONG CTableFrameSink::CalculateScore()
{
	//��������
	LONG cbRevenue=m_pGameServiceOption->wRevenue;

	BYTE cbWinner=0;
	//�ƶ����
	DeduceWinner(cbWinner);

	//��Ϸ��¼
	//--
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbWinner=cbWinner;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;

	//��������
	BYTE cbMultiple[5];
	ZeroMemory(&cbMultiple, sizeof(cbMultiple));

	//��������͵ı���
	for(BYTE i=0; i<5; i++)
	{
		cbMultiple[i] = m_GameLogic.GetMultiple(m_cbTableCardArray[i], 3);
	}

	LONG cbTotalScore = 0;
	//���м���ׯ�ұ��ƽ��
	bool bResult[4] = {false, false, false, false};
	for(BYTE i=1; i<5; i++)
	{
		if(m_GameLogic.IsBigger(m_cbTableCardArray[i], m_cbTableCardArray[0], 3, 3))
			bResult[i-1] = true;
		else
			bResult[i-1] = false;
	}

	//����ɼ�
	for(BYTE i=0; i<GAME_PLAYER; i++)
	{
		m_lUserWinScore[i] = 0;

		//��
		if(m_lUserTianScore[i] > 0)
		{
			if(bResult[0])
				m_lUserWinScore[i] += m_lUserTianScore[i]*cbMultiple[1];
			else
				m_lUserWinScore[i] -= m_lUserTianScore[i]*cbMultiple[0];
		}
		//��
		if(m_lUserDiScore[i]>0)
		{
			if(bResult[1])
				m_lUserWinScore[i] += m_lUserDiScore[i]*cbMultiple[2];
			else
				m_lUserWinScore[i] -= m_lUserDiScore[i]*cbMultiple[0];
		}
		//��
		if( m_lUserXuanScore[i] > 0 )
		{
			if(bResult[2])
				m_lUserWinScore[i] += m_lUserXuanScore[i]*cbMultiple[3];
			else
				m_lUserWinScore[i] -= m_lUserXuanScore[i]*cbMultiple[0];
		}
		//��
		if( m_lUserHuangScore[i] > 0 )
		{
			if(bResult[3])
				m_lUserWinScore[i] += m_lUserHuangScore[i]*cbMultiple[4];
			else
				m_lUserWinScore[i] -= m_lUserHuangScore[i]*cbMultiple[0];
		}

		//�ܳɼ�
		cbTotalScore += m_lUserWinScore[i];

		//��˰
		if(m_lUserWinScore[i] > 0)
			m_lUserWinScore[i] = m_lUserWinScore[i]*(1000L-cbRevenue)/1000;
	}

	//ׯ�ҿ�˰
	cbTotalScore = cbTotalScore*(1000L-cbRevenue)/1000;

	return cbTotalScore;

	/*//�����Ƶ�
//	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
//	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	BYTE cbWinner=0;
	//�ƶ����
	DeduceWinner(cbWinner);

	//��Ϸ��¼
	//--
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	//GameRecord.cbBankerCount=cbBankerCount;
	//GameRecord.cbPlayerCount=cbPlayerCount;
	//GameRecord.cbKingWinner=cbKingWinner;
	//GameRecord.bPlayerTwoPair=bPlayerTwoPair;
	//GameRecord.bBankerTwoPair=bBankerTwoPair;
	GameRecord.cbWinner=cbWinner;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;



	//ׯ������
	LONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));


	for(BYTE i=0;i<5;i++)
	{
		m_cbTimers[i]=m_GameLogic.GetTimes(m_cbTableCardArray[i],5);
	}
	

	//�����ע
	//LONG *pUserScore[] = {NULL, m_lUserPlayerScore, m_lUserTieScore, m_lUserBankerScore, m_lUserPlayerKingScore, 
	//	m_lUserBankerKingScore, m_lUserTieSamePointScore};

	//������
	//BYTE cbMultiple[] = {0, 1, 8, 1, 2, 2, 33};

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//for ( WORD wAreaIndex = ID_XIAN_JIA; wAreaIndex <= ID_TONG_DIAN_PING; ++wAreaIndex )
		//{
		//	if ( wAreaIndex == cbWinner || wAreaIndex == cbKingWinner ) 
		//	{
		//		m_lUserWinScore[i] += ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
		//		m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
		//		lBankerWinScore -= ( pUserScore[wAreaIndex][i] * cbMultiple[wAreaIndex] ) ;
		//	}
		//	else if ( cbWinner == ID_PING_JIA )
		//	{
		//		m_lUserReturnScore[i] += pUserScore[wAreaIndex][i] ;
		//	}
		//	else
		//	{
		//		lUserLostScore[i] -= pUserScore[wAreaIndex][i];
		//		lBankerWinScore += pUserScore[wAreaIndex][i];
		//	}
		//}

		////�ж���
		//if (bPlayerTwoPair)
		//{
		//	//���Ӯ��
		//	m_lUserWinScore[i] += (m_lUserPlayerTwoPair[i] * RATE_TWO_PAIR) ;
		//	m_lUserReturnScore[i] += m_lUserPlayerTwoPair[i] ;
		//	lBankerWinScore -= (m_lUserPlayerTwoPair[i] * RATE_TWO_PAIR) ;
		//}
		//else
		//{
		//	//������
		//	lUserLostScore[i] -= m_lUserPlayerTwoPair[i] ;
		//	lBankerWinScore += m_lUserPlayerTwoPair[i] ;
		//}

		//if (bBankerTwoPair)
		//{
		//	//���Ӯ��
		//	m_lUserWinScore[i] += (m_lUserBankerTwoPair[i] * RATE_TWO_PAIR) ;
		//	m_lUserReturnScore[i] += m_lUserBankerTwoPair[i] ;
		//	lBankerWinScore -= (m_lUserBankerTwoPair[i] * RATE_TWO_PAIR) ;
		//}
		//else
		//{
		//	//������
		//	lUserLostScore[i] -= m_lUserBankerTwoPair[i];
		//	lBankerWinScore += m_lUserBankerTwoPair[i];
		//}
//--��û������


		if(ID_TIAN_MARK & cbWinner)
		{
			m_lUserWinScore[i]+=m_lUserTianScore[i]*m_cbTimers[1];
			m_lUserReturnScore[i] += m_lUserTianScore[i] ;
			lBankerWinScore -= m_lUserTianScore[i]*m_cbTimers[1];
		}
		else
		{
			lUserLostScore[i] -=m_lUserTianScore[i]* m_cbTimers[0];
			lBankerWinScore += m_lUserTianScore[i] *m_cbTimers[0];		
		}

		if(ID_DI_MARK & cbWinner)
		{
			m_lUserWinScore[i]+=m_lUserDiScore[i]*m_cbTimers[2];
			m_lUserReturnScore[i] += m_lUserDiScore[i] ;
			lBankerWinScore -= m_lUserDiScore[i]*m_cbTimers[2];
		}
		else
		{
			lUserLostScore[i] -=m_lUserDiScore[i]*m_cbTimers[0] ;
			lBankerWinScore += m_lUserDiScore[i]*m_cbTimers[0] ;
		}

		if(ID_XUAN_MARK & cbWinner)
		{
			m_lUserWinScore[i]+=m_lUserXuanScore[i]*m_cbTimers[3];
			m_lUserReturnScore[i] += m_lUserXuanScore[i] ;
			lBankerWinScore -= m_lUserXuanScore[i]*m_cbTimers[3];
		}
		else
		{
			lUserLostScore[i] -=m_lUserXuanScore[i]*m_cbTimers[0] ;
			lBankerWinScore += m_lUserXuanScore[i]*m_cbTimers[0] ;
		}

		if(ID_HUANG_MARK & cbWinner)
		{
			m_lUserWinScore[i]+=m_lUserHuangScore[i]*m_cbTimers[4] ;
			m_lUserReturnScore[i] += m_lUserHuangScore[i] ;
			lBankerWinScore -= m_lUserHuangScore[i]*m_cbTimers[4] ;
		}
		else
		{
			lUserLostScore[i] -=m_lUserHuangScore[i] *m_cbTimers[0] ;
			lBankerWinScore += m_lUserHuangScore[i] *m_cbTimers[0] ;

		}


		//����˰��
		if (0 < m_lUserWinScore[i])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000001);
			m_lUserRevenue[i]  = LONG(m_lUserWinScore[i]*fRevenuePer);
			m_lUserWinScore[i] -= m_lUserRevenue[i];
		}

		//�ܵķ���
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			float fRevenuePer=float(cbRevenue/1000.+0.000001);
			m_lUserRevenue[m_wCurrentBanker]  = LONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}		
	}

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;*/

	return 1;
}


void CTableFrameSink::DeduceWinner(BYTE &cbWinner)
{
	cbWinner = 0x00;

	if (m_GameLogic.IsBigger(m_cbTableCardArray[1], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x01;
	if (m_GameLogic.IsBigger(m_cbTableCardArray[2], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x02;
	if (m_GameLogic.IsBigger(m_cbTableCardArray[3], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x04;
	if (m_GameLogic.IsBigger(m_cbTableCardArray[4], m_cbTableCardArray[0], 3, 3) )
		cbWinner |= 0x08;
	//�����Ƶ�
//	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
//	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//BOOL bUserOxData[5];
	//ZeroMemory(bUserOxData,sizeof(bUserOxData));
	//for(WORD i=0;i<5;i++)
	//{
	//	bUserOxData[i] = (m_GameLogic.GetCardType(m_cbTableCardArray[i],MAX_COUNT)>0)?TRUE:FALSE;
	//}

	/*for(int i=INDEX_PLAYER1;i<=INDEX_PLAYER4;i++)
	{
		BYTE cbMarkType=0;
		switch(i)
		{
		case 1:
			cbMarkType=ID_TIAN_MARK;
			break;
		case 2:
			cbMarkType=ID_DI_MARK;
			break;
		case 3:
			cbMarkType=ID_XUAN_MARK;
			break;
		case 4:
			cbMarkType=ID_HUANG_MARK;
			break;
		}
		if (m_GameLogic.CompareCard(m_cbTableCardArray[i],m_cbTableCardArray[INDEX_BANKER],MAX_COUNT))
		{

			if(cbMarkType)
			{

				cbWinner|=cbMarkType;
			}
			
		}
		else
		{
			if(cbMarkType)
			{
				cbWinner = (cbWinner & ~cbMarkType);
			}

		}
	}*/
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
//////////////////////////////////////////////////////////////////////////

