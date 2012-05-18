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
	ON_MESSAGE(IDM_ADD_GOLD,OnAddGold)
	ON_MESSAGE(IDM_SHOWHAND,OnShowHand)
	ON_MESSAGE(IDM_ADD_SCORE_CONTROL,OnAddGoldControl)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;
	m_lShowHandScore = 0L;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(m_szName,0,sizeof(m_szName));
	ZeroMemory(&m_GameEnd,sizeof(m_GameEnd));

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
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;
	m_lShowHandScore = 0L;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(m_szName,0,sizeof(m_szName));
	ZeroMemory(&m_GameEnd,sizeof(m_GameEnd));

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
				if ((IsLookonMode()==false)&&(wViewChairID==2)) OnGiveUp(1,1);
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

			//���ý���
			m_lGoldShow=0L;
			m_lTurnBasicGold=0L;
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
			CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
			m_wCurrentUser = pStatusPlay->wCurrentUser;
			m_lShowHandScore = pStatusPlay->lShowHandScore;
			m_bShowHand = pStatusPlay->bShowHand?true:false;

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewStation=SwitchViewChairID(i);
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData!=NULL)
				{
					m_lUserScore[i] = pUserData->lScore;
					lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
					if (m_bPlayStatus[i]==TRUE) m_GameClientView.m_CardControl[wViewStation].SetCardData(pStatusPlay->bTableCardArray[i],pStatusPlay->bTableCardCount[i]);
					m_GameClientView.SetUserGold( wViewStation,pUserData->lScore );
				}
				m_GameClientView.SetUserGoldInfo(wViewStation,false,pStatusPlay->lTableGold[2*i]);
				m_GameClientView.SetUserGoldInfo(wViewStation,true,pStatusPlay->lTableGold[2*i+1]);
			}
			m_GameClientView.SetGoldTitleInfo(pStatusPlay->lTurnMaxGold,pStatusPlay->lBasicGold);

			//�ж��Ƿ�ۿ�
			if ((IsLookonMode()==false)||(IsAllowLookon()==true)) m_GameClientView.m_CardControl[2].AllowPositively(true);

			//�ж��Ƿ��Լ���ע
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				UpdateScoreControl();
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
	memset(m_szName,0,sizeof(m_szName));
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lShowHandScore = pGameStart->lShowHandScore;

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;

	//��������
	WORD wMeChairID=GetMeChairID();
	bool bLookonMode=IsLookonMode();
	LONG lBaseGold=pGameStart->lBasicGold;

	//����״̬
	SetGameStatus(GS_PLAYING);
	m_GameClientView.SetGoldTitleInfo(pGameStart->lTurnMaxGold,lBaseGold);

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
			m_lUserScore[i] = pUserData->lScore;
			m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
			m_GameClientView.SetUserGoldInfo(wViewChairID,true,lBaseGold);
			lstrcpyn(m_szName[i],pUserData->szName,CountArray(m_szName[i]));
			m_GameClientView.SetUserGold( wViewChairID,pUserData->lScore );
		}
		else
		{
			m_bPlayStatus[i]=FALSE;
			m_lUserScore[i] = 0L;
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

	//���ö�ʱ��
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
	m_lTurnBasicGold=pAddGold->lCurrentLessGold;

	m_bShowHand = pAddGold->bShowHand;
	if( m_bShowHand && !m_bPlayStatus[pAddGold->wLastChairID] ) m_GameClientView.SetUserShowHand( true );

	if( pAddGold->lLastAddGold > 0L )
		m_GameClientView.SetUserGoldInfo(wViewChairID,false,pAddGold->lLastAddGold);
	if ((IsLookonMode()==false)&&(pAddGold->wCurrentUser==wMeChairID))
	{
		UpdateScoreControl();
	}

	//��������
	SetGameTimer(pAddGold->wCurrentUser,IDI_GIVE_UP,30);

	if ( m_bShowHand && m_bPlayStatus[pAddGold->wLastChairID] ) PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
	else if (m_bPlayStatus[pAddGold->wLastChairID]==TRUE) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	return true;
}

//�û�����
bool CGameClientDlg::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_bPlayStatus[pGiveUp->wUserChairID]=FALSE;
	if (pGiveUp->wUserChairID==GetMeChairID()) SetGameStatus(GS_FREE);

	//���ý���
	BYTE bCard[5]={0,0,0,0,0};
	WORD wViewStation=SwitchViewChairID(pGiveUp->wUserChairID);
	WORD bCount=m_GameClientView.m_CardControl[wViewStation].GetCardCount();
	m_GameClientView.m_CardControl[wViewStation].AllowPositively(false);
	m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(false);
	m_GameClientView.m_CardControl[wViewStation].SetCardData(bCard,bCount);

	//��������
	if( m_lTurnMaxGold != pGiveUp->lTurnMaxGold )
	{
		//��������
		m_lTurnMaxGold = pGiveUp->lTurnMaxGold;

		//���������ע
		m_GameClientView.SetGoldTitleInfo( m_lTurnMaxGold,m_GameClientView.m_lBasicGold );

		if( !IsLookonMode() && m_wCurrentUser==GetMeChairID() && pGiveUp->wUserChairID != m_wCurrentUser )
			UpdateScoreControl();
	}

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
	if( !m_bShowHand )
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//��������
	if( !IsLookonMode() )
	{
		if (pGameEnd->lGameGold[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//����״̬
	SetGameStatus(GS_FREE);

	KillGameTimer(IDI_GIVE_UP);

	//��������
	CopyMemory( &m_GameEnd,pGameEnd,sizeof(m_GameEnd) );

	if( !m_GameClientView.IsDispatchCard() )
		PerformGameEnd();

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
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[i].ShowFirstCard(false);
	}

	//���ÿؼ�
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	HideScoreControl();

	//���ؿؼ�
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//��ע��ť
LRESULT	CGameClientDlg::OnAddGold(WPARAM wParam, LPARAM lParam)
{
	OnFollow(0,0);

	return 0;
}

//������ť
LRESULT CGameClientDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//������Ϸ
	KillGameTimer(IDI_GIVE_UP);
	HideScoreControl();
	m_GameClientView.SetUserGoldInfo(2,false,m_lGoldShow);

	//������Ϣ
	SendData(SUB_C_GIVE_UP);

	return 0;
}

//��ע��ť
LRESULT CGameClientDlg::OnFollow(WPARAM wParam, LPARAM lParam)
{
	//��ȡ���
	LONG lGold=m_GameClientView.m_GoldView[4].GetGold();
	
	LONG lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);

	if( lGold+m_GameClientView.m_GoldView[5].GetGold() < lShowHandScore )
		lGold=__max(lGold,m_lTurnBasicGold);

	//���ñ���
	if ( lShowHandScore == lGold+m_GameClientView.m_GoldView[5].GetGold() ) 
	{
		if( !m_bShowHand )
			m_GameClientView.SetUserShowHand( true );
		m_bShowHand=true;
	}

	//���ý���
	HideScoreControl();
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
	LONG lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);
	LONG lGold=lShowHandScore-m_GameClientView.m_GoldView[5].GetGold();

	m_GameClientView.SetUserGoldInfo(2,false,lGold);
	OnFollow(0,0);

	return 0;
}

//��ע��Ϣ
LRESULT CGameClientDlg::OnAddGoldControl(WPARAM wParam, LPARAM lParam)
{
	//���ü�ע
	if( LONG(wParam) == 0 )
		m_GameClientView.m_GoldView[4].SetGold(0);
	else
		m_GameClientView.m_GoldView[4].SetGold(m_lTurnBasicGold+(LONG)wParam);

	//���ð�ť
	if( LONG(wParam) != 0L )
	{
		m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAddGold.ShowWindow(SW_SHOW);
	}else
	{
		m_GameClientView.m_btFollow.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAddGold.ShowWindow(SW_HIDE);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOOSE_SCORE"));

	return 0;
}

//�������
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	if( m_bPlayStatus[GetMeChairID()] && GetGameStatus() == GS_FREE )
	{
		PerformGameEnd();
		return 0;
	}

	//��ע�ж�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		UpdateScoreControl();
	}

	//���ö�ʱ��
	SetGameTimer(m_wCurrentUser,IDI_GIVE_UP,30);

	return 0;
}

//
void CGameClientDlg::UpdateScoreControl()
{
	WORD wMeChairID = GetMeChairID();

	//��������
	m_lGoldShow=m_GameClientView.m_GoldView[4].GetGold();

	//��������
	WORD wCardCount=m_GameClientView.m_CardControl[2].GetCardData(NULL,0);

	//��ֹ��ť
	LONG lLeaveScore=m_lTurnMaxGold-m_GameClientView.m_GoldView[4].GetGold()-m_GameClientView.m_GoldView[5].GetGold();
	m_GameClientView.m_btShowHand.EnableWindow((wCardCount>=3&&(lLeaveScore>0||m_bShowHand))?TRUE:FALSE);
	lLeaveScore=m_lTurnBasicGold-m_GameClientView.m_GoldView[4].GetGold();
	LONG lShowHandScore = __min(m_lUserScore[wMeChairID],m_lShowHandScore);
	if( !m_bShowHand && 
		( m_lTurnBasicGold == 0 || lLeaveScore>0L&&lShowHandScore>m_lTurnBasicGold+m_GameClientView.m_GoldView[5].GetGold() )
		)
		m_GameClientView.m_btFollow.EnableWindow(TRUE);
	else m_GameClientView.m_btFollow.EnableWindow(FALSE);

	//�Լ���ע
	ActiveGameFrame();
	m_GameClientView.m_btFollow.ShowWindow(SW_SHOW);
	m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
	m_GameClientView.m_btShowHand.ShowWindow(SW_SHOW);

	//��ע����
	lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_GameClientView.m_GoldView[5].GetGold();
	if( lLeaveScore > 0L )
		m_GameClientView.m_GoldControl.SetScoreControl(lLeaveScore,0,0);

	return;
}

//
void CGameClientDlg::HideScoreControl()
{
	m_GameClientView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddGold.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);
}

//
void CGameClientDlg::PerformGameEnd()
{
	CMD_S_GameEnd *pGameEnd = &m_GameEnd;

	//���û���
	CString strTemp ,strEnd = "\n";
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if(pGameEnd->lGameGold[i]>0)
			strTemp.Format("%s: %+ld\n",m_szName[i],pGameEnd->lGameGold[i]);
		else if( m_szName[i][0] != 0 )
			strTemp.Format("%s: %ld\n",m_szName[i],pGameEnd->lGameGold[i]);
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
	m_GameClientView.SetUserShowHand( false );
	HideScoreControl();

	//��Ϸ����
	m_lTurnMaxGold=0L;
	m_lTurnBasicGold=0L;
	memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
	m_wCurrentUser = INVALID_CHAIR;
	m_lShowHandScore = 0L;

	//��������
	m_lGoldShow=0L;
	m_bShowHand=false;
	memset(m_szName,0,sizeof(m_szName));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.SetFocus();
		SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
	}
}

//////////////////////////////////////////////////////////////////////////
