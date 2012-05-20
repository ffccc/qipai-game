#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME					200								//��ʼ��ʱ��
#define IDI_GIVE_UP						201								//������ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_FOLLOW,OnFollow)
	ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)
	ON_MESSAGE(IDM_SHOWHAND,OnShowHand)
	ON_MESSAGE(IDM_ADD_GOLD,OnAddGold)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	m_lBasicGold = 0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	m_bMeAddGold=false;
	m_bAddGold = false;
	memset(m_szName,0,sizeof(m_szName));

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	m_lBasicGold = 0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;


	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	m_bMeAddGold=false;
	m_bAddGold = false;
	memset(m_szName,0,sizeof(m_szName));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bAllowLookon=IsAllowUserLookon();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
		AllowUserLookon(0,GameOption.m_bAllowLookon);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//������Ϸ��ʱ��
		{
			if (nElapse==0)
			{
				PostMessage(WM_CLOSE,0,0);
				return false;
			}
			if (nElapse<=10) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_GIVE_UP:			//������ʱ��
		{
			WORD wViewChairID=SwitchViewChairID(wChairID);
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wViewChairID==2)) OnGiveUp(0,0);
				return false;
			}
			if ((nElapse<=10)&&(wViewChairID==2)&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}
	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�ؼ�����
	m_GameClientView.m_CardControl[2].AllowPositively(IsAllowLookon());

	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_GOLD:	//�û���ע
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubAddGold(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			m_GameClientView.FinishDispatchCard();
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_bShowHand=false;
			m_bMeAddGold=false;

			//���ý���
			m_lGoldShow=0L;
			m_lTurnBasicGold=0L;
			m_lBasicGold = pStatusFree->dwBasicGold;
			m_GameClientView.SetGoldTitleInfo(0,pStatusFree->dwBasicGold);

			//�������
			if (IsLookonMode()==false)
			{
				//���ð�ť
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();

				//����ʱ��
				SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
			}

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//���ñ���
			m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
			m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
			m_lBasicGold = pStatusPlay->lBasicGold;
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			m_wCurrentUser = pStatusPlay->wCurrentUser;
//			m_bShowHand = pStatusPlay->bShowHand;

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewStation=SwitchViewChairID(i);
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData!=NULL)
				{
					lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
					if (m_bPlayStatus[i]==TRUE) m_GameClientView.m_CardControl[wViewStation].SetCardData(pStatusPlay->bTableCardArray[i],pStatusPlay->bTableCardCount[i]);
				}
				m_GameClientView.SetUserGoldInfo(wViewStation,false,pStatusPlay->lTableGold[2*i]);
				m_GameClientView.SetUserGoldInfo(wViewStation,true,pStatusPlay->lTableGold[2*i+1]);

				//�ж��Ƿ����
				if (i==GetMeChairID())
				{
					m_bShowHand=((pStatusPlay->lMaxGold-pStatusPlay->lTableGold[2*i+1]-pStatusPlay->lTableGold[2*i])<=0L);
				}
			}
			m_GameClientView.SetGoldTitleInfo(pStatusPlay->lMaxGold,pStatusPlay->lBasicGold);

			//�ж��Ƿ�ۿ�
			if ((IsLookonMode()==false)||(IsAllowLookon()==true)) m_GameClientView.m_CardControl[2].AllowPositively(true);

			//�ж��Ƿ��Լ���ע
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_bMeAddGold=true;
				BYTE wCardCount=pStatusPlay->bTableCardCount[GetMeChairID()];

				LONG lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_GameClientView.m_GoldView[5].GetGold();

				//��ʾ��ť
				m_GameClientView.ShowScoreControl( true );

				//��ֹ��ť
				m_GameClientView.m_btShowHand.EnableWindow((m_bShowHand==true)||(wCardCount>=3&&lLeaveScore>0)?TRUE:FALSE);
				m_GameClientView.m_btFollow.EnableWindow(((m_bShowHand==false)||(lLeaveScore>=0L)||(m_lTurnBasicGold==0L))?TRUE:FALSE);

				//��ע��ť
				lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_GameClientView.m_GoldView[5].GetGold();
				m_GameClientView.m_btAddTimes1.EnableWindow( (lLeaveScore>=m_lBasicGold)?TRUE:FALSE );
				m_GameClientView.m_btAddTimes2.EnableWindow( (lLeaveScore>=2*m_lBasicGold)?TRUE:FALSE );
				m_GameClientView.m_btAddTimes3.EnableWindow( (lLeaveScore>=3*m_lBasicGold)?TRUE:FALSE );
			}
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_GIVE_UP,30);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//��Ϸ����
	m_lTurnMaxGold=pGameStart->lTurnMaxGold;
	m_lTurnBasicGold=pGameStart->lTurnBasicGold;
	m_lBasicGold = pGameStart->lBasicGold;
	memset(m_szName,0,sizeof(m_szName));
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = pGameStart->wCurrentUser;

	//�û�����
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		const tagUserData * pUserData=GetUserData(i);
		if( pUserData == NULL ) 
		{
			m_szAccounts[i][0] = 0;
			continue;
		}
		lstrcpy( m_szAccounts[i],pUserData->szName );
	}

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	m_bMeAddGold=false;

	//��������
	WORD wMeChairID=GetMeChairID();
	bool bLookonMode=IsLookonMode();
	LONG lBaseGold=m_lBasicGold;
	m_GameClientView.SetGoldTitleInfo( pGameStart->lMaxGold,m_lBasicGold );

	//����״̬
	SetGameStatus(GS_PLAYING);

	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		const tagUserData * pUserData=GetUserData(i);

		//���ý���
		if (pUserData!=NULL)
		{
			m_bPlayStatus[i]=TRUE;
			m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
			m_GameClientView.SetUserGoldInfo(wViewChairID,true,lBaseGold);
			m_GameClientView.m_GoldView[i*2].SetGoldIndex(1);
			lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
		}
		else
		{
			m_bPlayStatus[i]=FALSE;
			m_GameClientView.SetUserGoldInfo(wViewChairID,true,0L);
			m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
			
		}

		//���ÿؼ�
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[wViewChairID].ShowFirstCard(false);
	}
	if (bLookonMode==false) 
	{
		ActiveGameFrame();
		m_GameClientView.m_CardControl[2].AllowPositively(true);
	}

	//�Թۿ���
	if ((bLookonMode==true)&&(IsAllowLookon()==true))
	{
		m_GameClientView.m_CardControl[2].AllowPositively(true);
	}

	//���ÿ���
	if ((bLookonMode==false)&&(pGameStart->wCurrentUser==wMeChairID))
	{
		m_bMeAddGold=true;
	}

	//�ɷ��˿�
	for (BYTE cbIndex=0;cbIndex<2;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_bPlayStatus[i]==TRUE)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardData[2]={0,pGameStart->bCardData[i]};

				//�ɷ��˿�
				cbCardData[0]=(GetMeChairID()==i)?pGameStart->bFundusCard:0;
				m_GameClientView.DispatchUserCard(wViewChairID,cbCardData[cbIndex]);
			}
		}
	}

	//����
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddGold)) return false;
	CMD_S_AddGold * pAddGold=(CMD_S_AddGold *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pAddGold->wLastChairID);
	m_wCurrentUser = pAddGold->wCurrentUser;

	//��������
	m_lTurnBasicGold=__max(pAddGold->lLastAddGold,pAddGold->lCurrentLessGold);
	if( m_lTurnBasicGold >= m_lTurnMaxGold-m_GameClientView.m_GoldView[5].GetGold() )
		m_bShowHand = true;
	m_GameClientView.SetUserGoldInfo(wViewChairID,false,pAddGold->lLastAddGold);
	if ((IsLookonMode()==false)&&(pAddGold->wCurrentUser==wMeChairID))
	{
		//��������
		m_bMeAddGold=true;
		m_lGoldShow=m_GameClientView.m_GoldView[4].GetGold();

		//��������
		WORD wCardCount=m_GameClientView.m_CardControl[2].GetCardData(NULL,0);
		LONG lGold = m_GameClientView.m_GoldView[5].GetGold();
		LONG lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_GameClientView.m_GoldView[5].GetGold();

		//��ʾ��ť
		m_GameClientView.ShowScoreControl( true );

		//��ֹ��ť
		m_GameClientView.m_btShowHand.EnableWindow((m_bShowHand||(wCardCount>=3&&lLeaveScore>0))?TRUE:FALSE);
		m_GameClientView.m_btFollow.EnableWindow((m_bShowHand==false&&((lLeaveScore>=0L)||(m_lTurnBasicGold==0L)))?TRUE:FALSE);

		ActiveGameFrame();

		//��ע��ť
		m_GameClientView.m_btAddTimes1.EnableWindow( (lLeaveScore>=m_lBasicGold&&m_bAddGold==false)?TRUE:FALSE );
		m_GameClientView.m_btAddTimes2.EnableWindow( (lLeaveScore>=2*m_lBasicGold&&m_bAddGold==false)?TRUE:FALSE );
		m_GameClientView.m_btAddTimes3.EnableWindow( (lLeaveScore>=3*m_lBasicGold&&m_bAddGold==false)?TRUE:FALSE );
	}

	//��������
	SetGameTimer(pAddGold->wCurrentUser,IDI_GIVE_UP,30);
	LONG lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_GameClientView.m_GoldView[5].GetGold();
	WORD wCardCount=m_GameClientView.m_CardControl[2].GetCardData(NULL,0);
	if ( m_bShowHand && m_bPlayStatus[pAddGold->wLastChairID]) PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
	else if (m_bPlayStatus[pAddGold->wLastChairID]==TRUE) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	if( m_bShowHand && pAddGold->wLastChairID != wMeChairID && m_bPlayStatus[pAddGold->wLastChairID] )
		m_GameClientView.m_GoldView[wViewChairID*2].SetGoldIndex(2);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_bPlayStatus[pGiveUp->wUserChairID]=false;
	if (pGiveUp->wUserChairID==GetMeChairID()) SetGameStatus(GS_FREE);

	//���ý���
	BYTE bCard[5]={0,0,0,0,0};
	WORD wViewStation=SwitchViewChairID(pGiveUp->wUserChairID);
	WORD bCount=m_GameClientView.m_CardControl[wViewStation].GetCardCount();
	m_GameClientView.m_CardControl[wViewStation].AllowPositively(false);
	m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(false);
	m_GameClientView.m_CardControl[wViewStation].SetCardData(bCard,bCount);

	//��������
	if (pGiveUp->wUserChairID==GetTimeChairID()) KillGameTimer(IDI_GIVE_UP);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_R_SendCard)) return false;
	CMD_R_SendCard * pSendCard=(CMD_R_SendCard *)pBuffer;

	//���ñ���
	m_lGoldShow=0L;
	m_lTurnBasicGold=0L;
	m_lTurnMaxGold=pSendCard->lMaxGold;
	m_wCurrentUser = pSendCard->wCurrentUser;
	m_bAddGold = false;

	//���½���
	LONG lUserTableGold=0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡλ��
		WORD wViewChairID=SwitchViewChairID(i);

		//���ý��
		lUserTableGold=m_GameClientView.m_GoldView[wViewChairID*2].GetGold();
		lUserTableGold+=m_GameClientView.m_GoldView[wViewChairID*2+1].GetGold();
		m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
		m_GameClientView.SetUserGoldInfo(wViewChairID,true,lUserTableGold);
	}

	//��ע�ж�
	if ((IsLookonMode()==false)&&(pSendCard->wCurrentUser==GetMeChairID()))
	{
		m_bMeAddGold=true;
	}

	//�ɷ��˿�,���ϴ������ҿ�ʼ����
	WORD wLastMostUser = pSendCard->wStartChairId;
	ASSERT( wLastMostUser != INVALID_CHAIR );
	for (BYTE i=0;i<pSendCard->cbSendCardCount;i++)
	{
		for (WORD j=0;j<GAME_PLAYER;j++)
		{
			WORD wChairId = (wLastMostUser+j)%GAME_PLAYER;
			if (m_bPlayStatus[wChairId]==TRUE&&pSendCard->bUserCard[wChairId][i]!=0)
			{
				WORD wViewChairID=SwitchViewChairID(wChairId);
				m_GameClientView.DispatchUserCard(wViewChairID,pSendCard->bUserCard[wChairId][i]);
			}
		}
	}

	//��������
// 	if( !m_bShowHand )
// 		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
// 	else 
//		PlayGameSound(AfxGetInstanceHandle(), TEXT("GET_CARD"));

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//��������
	if (IsLookonMode()==false)
	{
		if (pGameEnd->lGameGold[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���û���
	CString strTemp ,strEnd = "\n";
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if(pGameEnd->lGameGold[i]>0)
			strTemp.Format("%s: %+ld\n",m_szAccounts[i],pGameEnd->lGameGold[i]);
		else if( m_szAccounts[i][0] != 0 )
			strTemp.Format("%s: %ld\n",m_szAccounts[i],pGameEnd->lGameGold[i]);
		else strTemp.Empty();
		strEnd += strTemp;
	}
	//��Ϣ����
	InsertSystemString((LPCTSTR)strEnd);

	//��Ϸ��������
	BYTE bCardData[5];
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (pGameEnd->lGameGold[i]!=0L)
		{
			if (m_szName[i][0]==0) m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),pGameEnd->lGameGold[i]);
			else m_GameClientView.m_ScoreView.SetGameScore(i,m_szName[i],pGameEnd->lGameGold[i]);
			if (pGameEnd->bUserCard[i]!=0)
			{
				WORD wViewStation=SwitchViewChairID(i);
				WORD wCardCount=m_GameClientView.m_CardControl[wViewStation].GetCardData(bCardData,CountArray(bCardData));
				bCardData[0]=pGameEnd->bUserCard[i];
				m_GameClientView.m_CardControl[wViewStation].SetCardData(bCardData,wCardCount);
				m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(true);
			}
		}
	}
	m_GameClientView.m_ScoreView.SetTax(pGameEnd->lTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//���ÿؼ�
	KillGameTimer(IDI_GIVE_UP);
	//��ʾ��ť
	m_GameClientView.ShowScoreControl( false );

	//����״̬
	SetGameStatus(GS_FREE);

	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	m_bMeAddGold=false;
	m_bAddGold = false;
	memset(m_szName,0,sizeof(m_szName));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.SetFocus();
		SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
	}

	return true;
}

//��ʼ��ť
LRESULT	CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ��ʱ��
	KillGameTimer(IDI_START_GAME);

	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_GoldView[i*2].SetGold(0L);
		m_GameClientView.m_GoldView[i*2+1].SetGold(0L);
		m_GameClientView.m_GoldView[i*2].SetGoldIndex(1);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[i].ShowFirstCard(false);
	}

	//���ÿؼ�
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	//��ʾ��ť
	m_GameClientView.ShowScoreControl( false );


	//���ؿؼ�
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//������ť
LRESULT CGameClientDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//������Ϸ
	m_bMeAddGold=false;
	KillGameTimer(IDI_GIVE_UP);
	//��ʾ��ť
	m_GameClientView.ShowScoreControl( false );

	m_GameClientView.SetUserGoldInfo(2,false,m_lGoldShow);

	//������Ϣ
	SendData(SUB_C_GIVE_UP);

	return 0;
}

//��ע��ť
LRESULT CGameClientDlg::OnFollow(WPARAM wParam, LPARAM lParam)
{
	//��ȡ���
	LONG lGold;
	if( (BOOL)wParam )
	{
		const tagUserData * pUserData=GetUserData(GetMeChairID());
		ASSERT( pUserData != NULL );
		lGold = pUserData->lScore-m_GameClientView.m_GoldView[5].GetGold();
	}else
	{
		lGold = m_GameClientView.m_GoldView[4].GetGold();
		lGold = __max(lGold,m_lTurnBasicGold);
	}

	//���ñ���
	m_bMeAddGold=false;
	m_bAddGold = true;

	//���ý���
	if( (BOOL)wParam )
	{
		m_GameClientView.m_GoldView[4].SetGoldIndex(2);
	}

	//��ʾ��ť
	m_GameClientView.ShowScoreControl( false );

	m_GameClientView.SetUserGoldInfo(2,false,lGold);

	//ɾ����ʱ��
	KillGameTimer(IDI_GIVE_UP);
	
	//��������
	CMD_C_AddGold AddPoint;
	AddPoint.lGold=lGold;
	SendData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

	return 0;
}

//�����ť
LRESULT CGameClientDlg::OnShowHand(WPARAM wParam, LPARAM lParam)
{
	LONG lGold=m_lTurnMaxGold-m_GameClientView.m_GoldView[5].GetGold();
	LONG dwMoney4=m_GameClientView.m_GoldView[4].GetGold();

	//
	m_bAddGold = true;

	if (m_bMeAddGold==true)
	{
		m_GameClientView.SetUserGoldInfo(2,false,lGold);
		OnFollow(TRUE,0);
	}

	m_GameClientView.m_GoldView[4].SetGoldIndex(2);

	return 0;
}

//��ע��Ϣ
LRESULT CGameClientDlg::OnAddGold(WPARAM wParam, LPARAM lParam)
{
	//���ӳ���
	if (m_lTurnBasicGold>m_GameClientView.m_GoldView[4].GetGold())
	{
		m_GameClientView.m_GoldView[4].SetGold(m_lTurnBasicGold);
	}

	//���ü�ע
	LONG lScore4=m_GameClientView.m_GoldView[4].GetGold();
	m_GameClientView.m_GoldView[4].SetGold(lScore4+m_lBasicGold*(LONG)wParam);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOOSE_SCORE"));

	//
	OnFollow(0,0);

	return 0;
}

//�������
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//��ע�ж�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();

		LONG lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_GameClientView.m_GoldView[5].GetGold();

		//��ʾ��ť
		m_GameClientView.ShowScoreControl( true );

		//��ֹ��ť
		WORD wCardCount=m_GameClientView.m_CardControl[2].GetCardData(NULL,0);
		m_GameClientView.m_btShowHand.EnableWindow(((m_bShowHand==true)||(wCardCount>=3)&&(m_bShowHand==false))?TRUE:FALSE);
		m_GameClientView.m_btFollow.EnableWindow(((m_bShowHand==false)||(lLeaveScore>=0L)||(m_lTurnBasicGold==0L))?TRUE:FALSE);

		//��ע��ť
		m_GameClientView.m_btAddTimes1.EnableWindow( (lLeaveScore>=m_lBasicGold)?TRUE:FALSE );
		m_GameClientView.m_btAddTimes2.EnableWindow( (lLeaveScore>=2*m_lBasicGold)?TRUE:FALSE );
		m_GameClientView.m_btAddTimes3.EnableWindow( (lLeaveScore>=3*m_lBasicGold)?TRUE:FALSE );
	}

	//���ö�ʱ��
	SetGameTimer(m_wCurrentUser,IDI_GIVE_UP,30);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
