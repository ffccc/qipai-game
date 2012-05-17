#include "StdAfx.h"
#include <algorithm>

using namespace std;
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define SEND_COUNT					300									//���ʹ���


//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//����ʱ��
#define IDI_FREE					1									//����ʱ��
#ifdef _DEBUG
#define TIME_FREE					3									//����ʱ��
#else
#define TIME_FREE					8									//����ʱ��
#endif

//��עʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��

#ifdef _DEBUG
#define TIME_PLACE_JETTON			10									//��עʱ��
#else
#define TIME_PLACE_JETTON			25									//��עʱ��
#endif

//����ʱ��
#define IDI_GAME_END				3									//����ʱ��

#ifdef _DEBUG
#define TIME_GAME_END				35									//����ʱ��
#else
#define TIME_GAME_END				35									//����ʱ��
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
	m_lScore2=0L;
	m_lScore3=0L;
	m_lScore1=0L;		
	m_lScore6=0L;
	m_lScore5=0L;
	m_lScore4=0L;	
	m_lScore7=0L;
	m_lScore8=0L;

	//���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;

	//������ע
	ZeroMemory(m_lUserScore2,sizeof(m_lUserScore2));
	ZeroMemory(m_lUserScore1,sizeof(m_lUserScore1));
	ZeroMemory(m_lUserScore6,sizeof(m_lUserScore6));
	ZeroMemory(m_lUserScore5,sizeof(m_lUserScore5));
	ZeroMemory(m_lUserScore4,sizeof(m_lUserScore4));
	ZeroMemory(m_lUserScore7,sizeof(m_lUserScore7));
	ZeroMemory(m_lUserScore8,sizeof(m_lUserScore8));
	ZeroMemory(m_lUserScore3,sizeof(m_lUserScore3));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	////�˿���Ϣ
	//ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	//ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

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


	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\ServerIni\\RaceHorseConfig.ini"),szPath);

	//AfxMessageBox(m_szConfigFileName);
	//���Ʊ���

	//m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score"), 10000*10, m_szConfigFileName);
	m_lAreaLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("AreaLimitScore"), 1000000 , m_szConfigFileName);
	m_lUserLimitScore=GetPrivateProfileInt(TEXT("ScoreLimit"), TEXT("UserLimitScore"), 10000000/*lMaxScore*/, m_szConfigFileName);
	BYTE cbEnableSysBanker=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("EnableSysBanker"), 1, m_szConfigFileName);
	m_bEnableSysBanker=cbEnableSysBanker?true:false;

	if (m_pGameServiceOption->lLessScore == 100000)									//������
	{
		m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score1"), 100000*10, m_szConfigFileName);
	}
	else if (m_pGameServiceOption->lLessScore == 1000000)							//�м���
	{
		m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score2"), 1000000*10, m_szConfigFileName);
	}
	else if (m_pGameServiceOption->lLessScore == 10000000)							//�߼���
	{
		m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score3"), 10000000*10, m_szConfigFileName);
	}
	else if (m_pGameServiceOption->lLessScore == 100000000)							//������
	{
		m_lApplyBankerCondition=GetPrivateProfileInt(TEXT("BankerCondition"), TEXT("Score4"), 100000000*10, m_szConfigFileName);
	}else
	{
		m_lApplyBankerCondition = 100000;
	}

	//TCHAR str[100];
	//AfxMessageBox(str);
	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//����ע��
	m_lScore2=0L;
	m_lScore3=0L;
	m_lScore1=0L;		
	m_lScore6=0L;
	m_lScore5=0L;
	m_lScore4=0L;	
	m_lScore7=0L;
	m_lScore8=0L;

	//������ע
	ZeroMemory(m_lUserScore2,sizeof(m_lUserScore2));
	ZeroMemory(m_lUserScore1,sizeof(m_lUserScore1));
	ZeroMemory(m_lUserScore6,sizeof(m_lUserScore6));
	ZeroMemory(m_lUserScore5,sizeof(m_lUserScore5));
	ZeroMemory(m_lUserScore4,sizeof(m_lUserScore4));
	ZeroMemory(m_lUserScore7,sizeof(m_lUserScore7));
	ZeroMemory(m_lUserScore8,sizeof(m_lUserScore8));
	ZeroMemory(m_lUserScore3,sizeof(m_lUserScore3));

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
	{
		pIBankerServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		//m_lAreaLimitScore = pIBankerServerUserItem->GetUserScore()->lGameGold/8;
		//m_lUserLimitScore = pIBankerServerUserItem->GetUserScore()->lGameGold;
	}
	//else
	//{
	//	LONGLONG lBankerCondition=m_pGameServiceOption->lLessScore;
	//	if (lBankerCondition != 0)
	//	{
	//		//m_lApplyBankerCondition = lBankerCondition*10;
	//		m_lAreaLimitScore = m_lApplyBankerCondition;
	//		m_lUserLimitScore = m_lApplyBankerCondition * 8;
	//	}else
	//	{
	//		//m_lApplyBankerCondition = 10000000;
	//		m_lAreaLimitScore = m_lApplyBankerCondition;
	//		m_lUserLimitScore = m_lApplyBankerCondition * 8;
	//	}
	//}

	//������ʾ

	TCHAR szTipMsg[128] = _T("");
	int count = GetPrivateProfileInt(TEXT("GameMsg"),TEXT("GameMsgCount"),0,m_szConfigFileName);
	if (count !=0)
	{
		TCHAR szMsg1[128]=_T(""), szMsg2[128]=_T(""), szMsg3[128]= _T(""),szMsg[128] = _T("");
		GetPrivateProfileString(TEXT("GameMsg"),_T("GameMsg0"),TEXT("��ׯ����Ϊ��"),szMsg1,CountArray(szMsg1),m_szConfigFileName);
		GetPrivateProfileString(TEXT("GameMsg"),_T("GameMsg1"),TEXT("��������Ϊ��"),szMsg2,CountArray(szMsg2),m_szConfigFileName);
		GetPrivateProfileString(TEXT("GameMsg"),_T("GameMsg2"),TEXT("�������Ϊ��"),szMsg3,CountArray(szMsg3),m_szConfigFileName);
		
		sprintf(szMsg,TEXT("\n%s%s\n%s%s\n%s%s"),szMsg1,TEXT("%I64d"),szMsg2,TEXT("%I64d"),szMsg3,TEXT("%I64d"));
		sprintf(szTipMsg,szMsg,m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);
	}
	else
	{
		_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("\n  ��ׯ����Ϊ��%I64d\n  ��������Ϊ��%I64d\n  �������Ϊ��%I64d"),m_lApplyBankerCondition,m_lAreaLimitScore,m_lUserLimitScore);	
	}	

	//���ñ���
	GameStart.cbTimeLeave=TIME_PLACE_JETTON;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=pIBankerServerUserItem->GetUserScore()->lScore;
	GameStart.lAreaLimitScore = m_lAreaLimitScore;

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

		m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
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
					m_pITableFrame->SetGameStatus(GS_FREE);
					//����ʱ��
					m_pITableFrame->SetGameTimer(GS_FREE,TIME_FREE*1000,1,0L);
				}
				else
				{
					//��������
					LONGLONG lScore=0;
					LONGLONG lRevenue=0;
					enScoreKind ScoreKind=enScoreKind_Flee;

					//ͳ�Ƴɼ�
					//lScore=-( m_lUserScore2[wChairID] + m_lUserScore3[wChairID] + m_lUserScore1[wChairID] + 
					//	m_lUserScore5[wChairID] + m_lUserScore4[wChairID] + m_lUserScore6[wChairID]+
					//	m_lUserScore7[wChairID]+m_lUserScore8[wChairID]);

					//д�����
					if (lScore!=0L) m_pITableFrame->WriteUserScore(pIServerUserItem, lScore,lRevenue, ScoreKind);
				}
			}

			//ׯ���ж�
			if (cbReason==GER_USER_LEFT && wChairID!=m_wCurrentBanker) return true;

			
			//�ɷ��˿�
			DispatchTableCard();

			//�������
			LONGLONG lBankerWinScore=CalculateScore();

			//�����ж�
			if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker && m_lUserWinScore[m_wCurrentBanker]<0)
			{
				m_pITableFrame->WriteUserScore(m_wCurrentBanker,m_lUserWinScore[m_wCurrentBanker], m_lUserRevenue[m_wCurrentBanker], enScoreKind_Flee);
			}

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

			CopyMemory(GameEnd.cbHorseSpeed,m_cbHorseSpeed,sizeof(GameEnd.cbHorseSpeed));

			
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

					//��Ϸ��¼
			tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
			GameRecord.cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);

			//�ƶ��±�
			m_nRecordLast = (m_nRecordLast+1) % MAX_S_SCORE_HISTORY;
			if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_S_SCORE_HISTORY;

			
			//�л�ׯ��
			if (cbReason==GER_USER_LEFT && wChairID==m_wCurrentBanker) ChangeBanker(true);

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
			//SendGameRecord(pIServerUserItem);

			LONGLONG lBankerCondition=m_pGameServiceOption->lLessScore;
			if (lBankerCondition != 0)
			{
				m_lApplyBankerCondition = lBankerCondition*10;
			}			

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
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d"),m_lApplyBankerCondition);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_INFO);
						
			//����������
			SendApplyUser(pIServerUserItem);

			return bSuccess;
		}
	case GS_PLACE_JETTON:		//��Ϸ״̬
	case GS_GAME_END:			//����״̬
		{
				
			
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//ȫ����ע
			StatusPlay.lAllScore1=m_lScore1;		
			StatusPlay.lAllScore2=m_lScore2;		
			StatusPlay.lAllScore3=m_lScore3;		
			StatusPlay.lAllScore4=m_lScore4;
			StatusPlay.lAllScore5=m_lScore5;	
			StatusPlay.lAllScore6=m_lScore6;	
			StatusPlay.lAllScore7=m_lScore7;	
			StatusPlay.lAllScore8=m_lScore8;	

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusPlay.lUserScore1=m_lUserScore1[wChiarID];			
				StatusPlay.lUserScore2=m_lUserScore2[wChiarID];		
				StatusPlay.lUserScore3=m_lUserScore3[wChiarID];		
				StatusPlay.lUserScore4=m_lUserScore4[wChiarID];	
				StatusPlay.lUserScore5=m_lUserScore5[wChiarID];	
				StatusPlay.lUserScore6=m_lUserScore6[wChiarID];	
				StatusPlay.lUserScore7=m_lUserScore7[wChiarID];		
				StatusPlay.lUserScore8=m_lUserScore8[wChiarID];

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

				//��Ϣ
				CopyMemory(StatusPlay.cbHorseSpeed,m_cbHorseSpeed,sizeof(StatusPlay.cbHorseSpeed));

				//���ͼ�¼
				SendGameRecord(pIServerUserItem);	
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
				if (m_lUserWinScore[wUserChairID]!=0L) 
					m_pITableFrame->WriteUserScore(wUserChairID,m_lUserWinScore[wUserChairID], m_lUserRevenue[wUserChairID], ScoreKind);

				//��ׯ�ж�
				LONGLONG lUserScore=pIServerUserItem->GetUserScore()->lScore;
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
						_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ����������ׯ����������%I64d��,���Զ���ׯ��"),m_lApplyBankerCondition);
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
	case SUB_C_OPEN_BANK:
		{
			//��ҷ���
			if(m_pGameServiceOption->wServerType == GAME_GENRE_SCORE)
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,"��ֵ���䲻��ʹ�����У�",SMT_INFO);
			}else if(m_pGameServiceOption->wServerType == GAME_GENRE_MATCH)
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,"�������䲻��ʹ�����У�",SMT_INFO);
			}else if(m_pGameServiceOption->wServerType == GAME_GENRE_EDUCATE)
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,"��ϰ���䲻��ʹ�����У�",SMT_INFO);
			}
			
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
		m_pITableFrame->SetGameStatus(GS_FREE);
	}
	

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
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea<=ID_HORSE8)&&(lJettonScore>0L));
	if ((cbJettonArea>ID_HORSE8)||(lJettonScore<=0L)) return false;

	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);
	if (m_pITableFrame->GetGameStatus()!=GS_PLACE_JETTON) return true;

	//ׯ���ж�
	if (m_wCurrentBanker==wChairID) return true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) return true;

	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	LONGLONG lJettonCount=m_lUserScore2[wChairID]+m_lUserScore3[wChairID]+m_lUserScore1[wChairID]+
		m_lUserScore6[wChairID] + m_lUserScore4[wChairID] + m_lUserScore5[wChairID]+
		m_lUserScore7[wChairID]+m_lUserScore8[wChairID];

	LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lScore;

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if ( ID_HORSE1 == cbJettonArea )
	{
		if ( GetMaxHorse1(wChairID) >= lJettonScore )
		{
			//������ע
			m_lScore1 += lJettonScore;
			m_lUserScore1[wChairID] += lJettonScore;			
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE2 == cbJettonArea )
	{
		if ( GetMaxHorse2(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lScore2 += lJettonScore;
			m_lUserScore2[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE3 == cbJettonArea )
	{
		if ( GetMaxHorse3(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lScore3 += lJettonScore;
			m_lUserScore3[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE4 == cbJettonArea )
	{
		if ( GetMaxHorse4(wChairID) >= lJettonScore ) 
		{
			//������ע
			m_lScore4 += lJettonScore;
			m_lUserScore4[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE5 == cbJettonArea )
	{
		if ( GetMaxHorse5(wChairID) >=lJettonScore ) 
		{
			//������ע
			m_lScore5 += lJettonScore;
			m_lUserScore5[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if ( ID_HORSE6 == cbJettonArea )
	{
		if ( GetMaxHorse6(wChairID)>=lJettonScore )
		{
			//������ע
			m_lScore6 += lJettonScore;
			m_lUserScore6[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}	
	else if (ID_HORSE7==cbJettonArea)
	{
		if ( GetMaxHorse7(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lScore7+=lJettonScore;
			m_lUserScore7[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else if (ID_HORSE8==cbJettonArea)
	{
		if ( GetMaxHorse8(wChairID)>=lJettonScore ) 
		{
			//������ע
			m_lScore8+=lJettonScore;
			m_lUserScore8[wChairID] += lJettonScore;
		}
		else bPlaceJettonSuccess=false;
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

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
bool CTableFrameSink::DispatchSpeed()
{

	return true;
}
//�����ٶ�
bool CTableFrameSink::DispatchTableCard()
{

	for (int i=0; i < HORSE_COUNT; i++)
	{
		m_GameLogic.RandNumList(m_cbHorseSpeed[i],HORSE_COUNT);
	}
	

	BYTE cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);
	
////////////////////////////////////////////////////////////////////////////////////////////////////
	//�淶�ٶȣ�ʹ�䱣����һ����Χ��
	BYTE cbCount = m_GameLogic.GetNumListPip(m_cbHorseSpeed[cbWinner-1],HORSE_COUNT);

	if (cbCount > 60)
	{
		BYTE temp = cbCount - 60;
		for (int i=0; i < HORSE_COUNT; i++)
		{
			BYTE temp2 = m_GameLogic.GetNumListPip(m_cbHorseSpeed[i],HORSE_COUNT);
			if (temp2 >= 60)
			{
				m_cbHorseSpeed[i][4] -= temp2 - 60;
			}
		}

	}else if (cbCount < 60)
	{
		BYTE temp = cbCount - 60;
		for (int i=0; i < HORSE_COUNT; i++)
			m_cbHorseSpeed[i][4] += temp;

	}
///////////////////////////////////////////////////////////////////////////////////////////////////
	//��Ϸ����
	BYTE cbTempWinner = 0;

	//��ȡ����
	int   nAccountsCount = 0;
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	//��ȡ����
	TCHAR szFileName[MAX_PATH];
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\ServerIni\\RaceHorseWaiGua.ini"),szPath);
	nAccountsCount = GetPrivateProfileInt( TEXT("WAIGUA"), TEXT("AccountsCount"), 0, szFileName );

	if (nAccountsCount != 0)
	{
		TCHAR szCtrlName[50][MAX_PATH],szTempName[MAX_PATH]=TEXT("");		
		for (WORD i=0;i<nAccountsCount;i++)
		{
			ZeroMemory(szCtrlName[i],sizeof(szCtrlName[i]));
			ZeroMemory(szTempName,sizeof(szTempName));
			sprintf(szTempName,TEXT("p%dname"),i);
			GetPrivateProfileString( TEXT("WAIGUA"), szTempName, "123",szCtrlName[i],sizeof(szCtrlName[i]),szFileName);
		}

		BYTE  cbChairIDCtrl = INVALID_CHAIR;
		TCHAR szNewName[MAX_PATH];//�������˲�Ҫ���ȥ
		ZeroMemory(szNewName,sizeof(szNewName));
		for (WORD i=0;i<nAccountsCount;i++)
		{
			bool bCtrl = false;
			for (WORD wChairID=0;wChairID<GAME_PLAYER;wChairID++)
			{
				//У���Ƿ�������
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
				if (pIServerUserItem == NULL)
				{
					continue;
				}

				if(lstrcmp(pIServerUserItem->GetAccounts(),szCtrlName[i]) == 0)
				{
					CopyMemory(szNewName,szCtrlName[i],sizeof(szCtrlName[i]));
					cbChairIDCtrl = wChairID;
					bCtrl = true;
					break;
				}
			 }
			if (bCtrl)
			{
				break;
			}
		}

		//IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(cbChairIDCtrl);
		//if (pIServerUserItem != NULL)
		//{
		//	m_pITableFrame->SendGameMessage(pIServerUserItem,pIServerUserItem->GetAccounts(),SMT_INFO);
		//}

		//�ж��Ǹ�������ע�ˣ����������ע���������ֻȡǰһ�����򣬺���ĺ���
		//���������Ҳ���ע��
		//�����ׯ�ң���ע���ٵ�����Ӯ

		LONGLONG lScore[] = {m_lUserScore1[cbChairIDCtrl],m_lUserScore2[cbChairIDCtrl],m_lUserScore3[cbChairIDCtrl],m_lUserScore4[cbChairIDCtrl],m_lUserScore5[cbChairIDCtrl],m_lUserScore6[cbChairIDCtrl],m_lUserScore7[cbChairIDCtrl],m_lUserScore8[cbChairIDCtrl]};
		for (BYTE i=0;i<HORSE_COUNT;i++)
		{
			if (lScore[i] > 0)
			{
				cbTempWinner = i+1;
				break;
			}
		}

		//��������Ѿ���ע
		if(cbTempWinner > 0)
		{
			cbWinner = cbTempWinner;
		}
		else if (cbChairIDCtrl == m_wCurrentBanker)
		{
			LONGLONG lScoreAll[] = {m_lScore1, m_lScore2, m_lScore3, m_lScore4, m_lScore5, m_lScore6, m_lScore7, m_lScore8};

			//�ж�һ��������ע��
			for (BYTE i=0;i<HORSE_COUNT;i++)
			{
				if (lScoreAll[i]>0)
				{
					cbTempWinner = 1;
					break;
				}
			}

			//������ע��ȥ�ж��ĸ�������ע����
			if (cbTempWinner == 1)
			{
				LONGLONG lTempScore =lScoreAll[0];
				for (BYTE i=1;i<HORSE_COUNT;i++)
				{					
					if (lTempScore > lScoreAll[i])
					{
						lTempScore = lScoreAll[i];
						cbTempWinner = i+1;
					}
				}
			}
			

			if(cbTempWinner > 0)
			{
				cbWinner = cbTempWinner;
			}
		}	
		
	}	

	//��ȡ����
	m_cbWinSideControl = GetPrivateProfileInt( TEXT("ManagerControl"), TEXT("WinSide"), 0, m_szConfigFileName );

	//������Ӯ
	if ( m_cbWinSideControl != 0 )
	{
		//�ط��ж�
		if ( cbWinner!=m_cbWinSideControl)
		{
			cbWinner = m_cbWinSideControl;
		}
	}

	m_cbHorseSpeed[cbWinner-1][0] = 8;
	m_cbHorseSpeed[cbWinner-1][1] = 7;
	m_cbHorseSpeed[cbWinner-1][2] = 8;
	m_cbHorseSpeed[cbWinner-1][3] = 7;
	m_cbHorseSpeed[cbWinner-1][4] = 8;
	m_cbHorseSpeed[cbWinner-1][5] = 8;
	m_cbHorseSpeed[cbWinner-1][6] = 8;
	m_cbHorseSpeed[cbWinner-1][7] = 8;
////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore()->lScore;
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
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

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
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetAccounts(),m_lApplyBankerCondition);
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
			LONGLONG lMessageCount=GetPrivateProfileInt(TEXT("Message"),TEXT("MessageCount"),0,m_szConfigFileName);
			if (lMessageCount!=0)
			{
				//��ȡ����
				LONGLONG lIndex=rand()%lMessageCount;
				TCHAR szKeyName[32],szMessage1[256],szMessage2[256];				
				_sntprintf(szKeyName,CountArray(szKeyName),TEXT("Item%I64d"),lIndex);
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
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID)
{
	//����ע��
	LONGLONG lNowJetton = 0;
	lNowJetton += m_lUserScore1[wChairID];
	lNowJetton += m_lUserScore2[wChairID];
	lNowJetton += m_lUserScore3[wChairID];
	lNowJetton += m_lUserScore4[wChairID];
	lNowJetton += m_lUserScore5[wChairID];
	lNowJetton += m_lUserScore6[wChairID];
	lNowJetton += m_lUserScore7[wChairID];
	lNowJetton += m_lUserScore8[wChairID];

	//��ȡ���
	IServerUserItem *pIMeServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);

	//�����ע
	LONGLONG lMeMaxScore = min(pIMeServerUserItem->GetUserScore()->lScore, m_lUserLimitScore);
	lMeMaxScore -= lNowJetton;
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxHorse1(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore1;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lScore2+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore7+ m_lScore8;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore1;
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxHorse2(WORD wChairID)
{	
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore2;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore7+ m_lScore8;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore2;
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxHorse3(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore3;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore7+ m_lScore8;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore3;
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxHorse4(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore4;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore5+ m_lScore6+ m_lScore7+ m_lScore8;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore4;
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxHorse5(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore5;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore4+ m_lScore6+ m_lScore7+ m_lScore8;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore5;
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxHorse6(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore6;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore7+ m_lScore8;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore6;
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxHorse7(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore7;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore8;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore7;
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//�����ע
LONGLONG CTableFrameSink::GetMaxHorse8(WORD wChairID)
{
	//�������
	LONGLONG lUserMaxJetton=GetUserMaxJetton(wChairID);

	//��������
	LONGLONG lAreaLimit = m_lAreaLimitScore - m_lScore8;

	//�����ע
	LONGLONG lMaxJetton=0L;

	//ׯ���ж�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��������
		LONGLONG lOtherAreaScore = m_lScore1+ m_lScore2+ m_lScore3+ m_lScore4+ m_lScore5+ m_lScore6+ m_lScore7;

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(m_wCurrentBanker);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore()->lScore;

		//ׯ������
		LONGLONG lMaxPlayerScore = lBankerScore + lOtherAreaScore;
		lMaxPlayerScore -= m_lScore8;
		ASSERT(lMaxPlayerScore>=0);

		//�����ע
		lMaxJetton = min(lMaxPlayerScore/7, lUserMaxJetton);	
		lMaxJetton = min(lMaxJetton, lAreaLimit);
	}
	else
	{
		lMaxJetton=min(lUserMaxJetton,lAreaLimit);
	}

	return lMaxJetton;
}

//����÷�
LONGLONG CTableFrameSink::CalculateScore()
{
	//��������
	LONGLONG cbRevenue=m_pGameServiceOption->wRevenue;

	//�ƶ����
	BYTE cbWinner = m_GameLogic.GetNumBigList(m_cbHorseSpeed,HORSE_COUNT);
	
	////��Ϸ��¼
	//tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	//GameRecord.cbWinner = cbWinner;

	////�ƶ��±�
	//m_nRecordLast = (m_nRecordLast+1) % MAX_S_SCORE_HISTORY;
	//if ( m_nRecordLast == m_nRecordFirst ) m_nRecordFirst = (m_nRecordFirst+1) % MAX_S_SCORE_HISTORY;

	//ׯ������
	LONGLONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�����ע
	LONGLONG *pUserScore[] = {NULL, m_lUserScore1, m_lUserScore2, m_lUserScore3, m_lUserScore4, 
		m_lUserScore5, m_lUserScore6, m_lUserScore7, m_lUserScore8};
	
	//������
	BYTE cbTime = 7;

	//�������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//ׯ���ж�
		if (m_wCurrentBanker==i) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		for ( WORD wAreaIndex = ID_HORSE1; wAreaIndex <= ID_HORSE8; wAreaIndex++ )
		{
			if (wAreaIndex == cbWinner)
			{
				m_lUserWinScore[i] += pUserScore[wAreaIndex][i] * cbTime ;
				m_lUserReturnScore[i] += m_lUserScore1[i] ;
				lBankerWinScore -= pUserScore[wAreaIndex][i] * cbTime ;
			} 
			else
			{
				m_lUserWinScore[i] -= pUserScore[wAreaIndex][i];
				m_lUserReturnScore[i] += 0 ;
				lBankerWinScore += pUserScore[wAreaIndex][i];
			}
			
		}	

		////����˰��
		//if (0 < m_lUserWinScore[i])
		//{
		//	float fRevenuePer=float(cbRevenue/1000.+0.000001);
		//	m_lUserRevenue[i]  = LONGLONG(m_lUserWinScore[i]*fRevenuePer);
		//	m_lUserWinScore[i] -= m_lUserRevenue[i];
		//}

		
		//�ܵķ���
		m_lUserWinScore[i] += lUserLostScore[i];
	}

	
	//ׯ�ҳɼ�
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		////����˰��
		//if (0 < m_lUserWinScore[m_wCurrentBanker])
		//{
		//	float fRevenuePer=float(cbRevenue/1000.+0.000001);
		//	m_lUserRevenue[m_wCurrentBanker]  = LONGLONG(m_lUserWinScore[m_wCurrentBanker]*fRevenuePer);
		//	m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
		//	lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		//}		
	}

	//�ۼƻ���
	m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	m_lBankerCurGameScore=lBankerWinScore;

	return lBankerWinScore;
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

		nIndex = (nIndex+1) % MAX_S_SCORE_HISTORY;
	}
	//if (wBufferSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
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

