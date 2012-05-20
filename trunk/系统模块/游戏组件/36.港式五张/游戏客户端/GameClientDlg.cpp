#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE			201									//��ע��ʱ��

//ʱ���ʶ
#define TIME_START_GAME				30									//��ʼ��ʱ��
//test
#define TIME_USER_ADD_SCORE			30									//������ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_NO_ADD,OnFollow)
	ON_MESSAGE(IDM_FOLLOW,OnFollow)
	ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)
	ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
	ON_MESSAGE(IDM_SHOW_HAND,OnShowHand)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bAddScore=false;
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

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

	//��ȡ����
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	for (int i=0;i<GAME_PLAYER;i++) m_GameClientView.m_CardControl[i].SetCardSpace(m_dwCardHSpace);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bAddScore=false;
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bAllowLookon=IsAllowUserLookon();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//�����˿�
		m_dwCardHSpace=GameOption.m_dwCardHSpace;
		for (int i=0;i<GAME_PLAYER;i++) m_GameClientView.m_CardControl[i].SetCardSpace(m_dwCardHSpace);

		//��������
		EnableSound(GameOption.m_bEnableSound);
		AllowUserLookon(0,GameOption.m_bAllowLookon);

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��ʱ��
		{
			//��ֹ�ж�
			if (nElapse==0)
			{
				PostMessage(WM_CLOSE,0,0);
				return false;
			}

			//����֪ͨ
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("START_WARN"));

			return true;
		}
	case IDI_USER_ADD_SCORE:	//��ע��ʱ��
		{
			//��ȡλ��
			WORD wViewChairID=SwitchViewChairID(wChairID);

			//��ֹ�ж�
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wViewChairID==2)) OnGiveUp(1,1);
				return false;
			}

			//����֪ͨ
			if ((nElapse<=3)&&(wViewChairID==2)&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_END"));
				return true;
			}

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	return;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
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

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}

			//����ʱ��
			if (IsLookonMode()==false) SetGameTimer(GetMeChairID(),IDI_START_GAME,30);

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//��ע��Ϣ
			m_lMaxScore=pStatusPlay->lMaxScore;
			m_lCellScore=pStatusPlay->lCellScore;
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			m_lTurnLessScore=pStatusPlay->lTurnLessScore;
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore));

			//״̬����
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_bAddScore=(pStatusPlay->bAddScore==TRUE)?true:false;
			m_bShowHand=(pStatusPlay->bShowHand==TRUE)?true:false;
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));

			//�ʺ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData!=NULL) lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
			}

			//���ý���
			LONG lTableScore=0L;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//����λ��
				WORD wViewChairID=SwitchViewChairID(i);

				//�����˿�
				if (m_cbPlayStatus[i]==TRUE) 
				{
					BYTE cbCardCount=pStatusPlay->cbCardCount[i];
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(pStatusPlay->cbHandCardData[i],cbCardCount);
				}
				lTableScore += m_lTableScore[2*i+1];
				//��������
				m_GameClientView.m_PlayerJeton[wViewChairID].SetScore(m_lTableScore[2*i]);
				//������ע
				m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[2*i]+m_lTableScore[2*i+1]);
			}
			m_GameClientView.m_PlayerJeton[GAME_PLAYER].SetScore(lTableScore);

			//
			m_GameClientView.SetCellScore(m_lCellScore);

			//�������
			if (IsLookonMode()==false) 
			{
				//��������
				m_GameClientView.m_CardControl[2].SetPositively(true);
				if (m_wCurrentUser==GetMeChairID()) UpdateScoreControl();
			}

			//���ö�ʱ��
			SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

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

	//����״̬
	SetGameStatus(GS_PLAYING);

	//��ע����
	m_lMaxScore=pGameStart->lMaxScore;
	m_lCellScore=pGameStart->lCellScore;
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	m_lTurnLessScore=pGameStart->lTurnLessScore;

	//״̬����
	m_bAddScore=false;
	m_wCurrentUser=pGameStart->wCurrentUser;

	//���ñ���
	LONG lTableScore=0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserData(i);
		if (pUserData==NULL) continue;

		//��Ϸ��Ϣ
		m_cbPlayStatus[i]=TRUE;
		lTableScore+=m_lCellScore;
		m_lTableScore[2*i+1]=m_lCellScore;

		//�û�����
		lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
	}

	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);
	m_GameClientView.SetCellScore(m_lCellScore);

	//���ý���
	lTableScore = 0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//�����˿�
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(NULL,0); 
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(false);
		lTableScore += m_lTableScore[2*i+1];
		//���ó���
		m_GameClientView.m_PlayerJeton[wViewChairID].SetScore(m_lTableScore[2*i]);
		m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[i*2]+m_lTableScore[i*2+1]);
	}
	m_GameClientView.m_PlayerJeton[GAME_PLAYER].SetScore(lTableScore);

	//�ɷ��˿�
	for (BYTE cbIndex=0;cbIndex<2;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardData[2]={0,pGameStart->cbCardData[i]};

				//�ɷ��˿�
				cbCardData[0]=(GetMeChairID()==i)?pGameStart->cbObscureCard:0;
				m_GameClientView.DispatchUserCard(wViewChairID,cbCardData[cbIndex]);
			}
		}
	}

	//�������
	if (IsLookonMode()==false) 
	{
		//��������
		m_GameClientView.m_CardControl[2].SetPositively(true);
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//������
	if (IsLookonMode()==false) ActiveGameFrame();

//	//��ȡʤ��
//#ifdef SUPPER_VER
//	CMD_C_GetWinner GetWinner;
//	GetWinner.dwUserID=GetUserData(GetMeChairID())->dwUserID;
//	SendData(SUB_C_GET_WINNER,&GetWinner,sizeof(GetWinner));
//#endif

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=SwitchViewChairID(wAddScoreUser);

	//����ж�
	if (m_bShowHand==false)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserData(wAddScoreUser);
		if (pUserData!=NULL) m_bShowHand=(pUserData->lScore==(m_lTableScore[wAddScoreUser*2+1]+pAddScore->lAddScoreCount));
	}

	//��ע����
	if ((IsLookonMode()==true)||(pAddScore->wAddScoreUser!=wMeChairID))
	{
		//��ע����
		m_GameClientView.m_PlayerJeton[wViewChairID].SetScore(pAddScore->lAddScoreCount);

		//��������
		if (m_cbPlayStatus[wAddScoreUser]==TRUE)
		{
			//��ȡ����
			const tagUserData * pUserData=(tagUserData *)GetUserData(wAddScoreUser);
			LONG lTableScore=m_lTableScore[wAddScoreUser*2+1];
			LONG lTurnAddScore=m_lTableScore[wAddScoreUser*2];
			LONG lShowHandScore=pUserData->lScore-lTableScore;

			//��������
			if ((pAddScore->lAddScoreCount-lTurnAddScore)==0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));
			else if (pAddScore->lAddScoreCount==lShowHandScore) PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
			else if ((pAddScore->lAddScoreCount+lTableScore)==m_lTurnLessScore) PlayGameSound(AfxGetInstanceHandle(),TEXT("FOLLOW"));
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		}
	}

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;
	m_lTableScore[wAddScoreUser*2]=pAddScore->lAddScoreCount;

	//���ó���
	m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[wAddScoreUser*2+1]+m_lTableScore[wAddScoreUser*2]);
	m_GameClientView.UpdateGameView(NULL);

	//���ƽ���
	if ((IsLookonMode()==false)&&(pAddScore->wCurrentUser==wMeChairID))
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	if (m_wCurrentUser==INVALID_CHAIR) KillGameTimer(IDI_USER_ADD_SCORE);
	else SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

	return true;
}

//�û�����
bool CGameClientDlg::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;
	BYTE cbCardData[MAX_COUNT]={0,0,0,0,0};
	WORD wViewChairID=SwitchViewChairID(wGiveUpUser);
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardCount();

	//�����˿�
	m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);
	m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(false);
	m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);

	//״̬����
	if ((IsLookonMode()==false)&&(pGiveUp->wGiveUpUser==GetMeChairID())) 
	{
		HideScoreControl();
		SetGameStatus(GS_FREE);
	}

	//��������
	if (wGiveUpUser==GetTimeChairID()) KillGameTimer(IDI_USER_ADD_SCORE);
	if ((IsLookonMode()==true)||(wGiveUpUser!=GetMeChairID())) PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	m_bAddScore=false;
	m_lTurnLessScore=0L;
	m_wCurrentUser=pSendCard->wCurrentUser;
	m_lTurnMaxScore=pSendCard->lTurnMaxScore;
	m_lTurnLessScore=pSendCard->lTurnLessScore;

	//���ó���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_PlayerJeton[wViewChairID].SetScore(0L);
		m_GameClientView.m_PlayerJeton[GAME_PLAYER].AddScore(m_lTableScore[i*2]);
	}

	//�ۼƽ��
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_lTableScore[i*2+1]+=m_lTableScore[i*2];
		m_lTableScore[i*2]=0L;
	}

	//�ɷ��˿�,���ϴ������ҿ�ʼ����
	WORD wLastMostUser = pSendCard->wLastMostUser;
	ASSERT( wLastMostUser != INVALID_CHAIR );
	for (BYTE i=0;i<pSendCard->cbSendCardCount;i++)
	{
		for (WORD j=0;j<GAME_PLAYER;j++)
		{
			WORD wChairId = (wLastMostUser+j)%GAME_PLAYER;
			if (m_cbPlayStatus[wChairId]==TRUE)
			{
				WORD wViewChairID=SwitchViewChairID(wChairId);
				m_GameClientView.DispatchUserCard(wViewChairID,pSendCard->cbCardData[wChairId][i]);
			}
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//״̬����
	HideScoreControl();
	SetGameStatus(GS_FREE);
	KillGameTimer(IDI_USER_ADD_SCORE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

	//��������
	if (IsLookonMode()==false)
	{
		if (pGameEnd->lGameScore[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//��������
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (pGameEnd->lGameScore[i]!=0L)
		{
			if (m_szAccounts[i][0]==0) m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),pGameEnd->lGameScore[i]);
			else m_GameClientView.m_ScoreView.SetGameScore(i,m_szAccounts[i],pGameEnd->lGameScore[i]);
			//��˰
			m_GameClientView.m_ScoreView.SetGameTax(i,pGameEnd->lGameTax[i]);
			if (pGameEnd->cbCardData[i]!=0)
			{
				//��ȡ�˿�
				BYTE cbCardData[5];
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

				//�����˿�
				cbCardData[0]=pGameEnd->cbCardData[i];
				m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(true);
				m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
			}
		}
	}
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//���û���
	TCHAR szBuffer[128] = TEXT("");
	InsertGeneralString(TEXT("\n���ֽ���,�ɼ�ͳ��:"),RGB(255,0,255),true);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if( pGameEnd->lGameScore[i] != 0L && m_szAccounts[i][0]!=0 )
		{
			//���������ʾ�ɼ�
			_snprintf(szBuffer,CountArray(szBuffer),TEXT(" %s: %+ld��"),m_szAccounts[i],pGameEnd->lGameScore[i]);
			InsertGeneralString(szBuffer,RGB(255,0,255),true);
		}
	}

	//��ע��Ϣ
	m_lMaxScore=0L;
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bAddScore=false;
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));


	//��ʼ��ť
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	return true;
}

//���ؿ���
void CGameClientDlg::HideScoreControl()
{
	//���ư�ť
	m_GameClientView.m_btAddScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoAdd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);
	m_GameClientView.m_GoldControl.ShowWindow(SW_HIDE);
	return;
}

//���¿���
void CGameClientDlg::UpdateScoreControl()
{
	//��������
	WORD wMeChairID=GetMeChairID();
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[2].GetCardCount();
	LONG lDrawAddScore=m_lTableScore[wMeChairID*2]+m_lTableScore[wMeChairID*2+1];

	//������
	const tagUserData * pUserData=GetUserData(wMeChairID);
	LONG lMaxLeaveScore=pUserData->lScore-lDrawAddScore;
	LONG lUserLessScore=__min(lMaxLeaveScore,__max(m_lTurnLessScore-lDrawAddScore,0L));

	//��ע��ť
	LONG lLeaveScore=__max(m_lTurnMaxScore-lDrawAddScore,0L);
	m_GameClientView.m_btAddScore.ShowWindow(SW_SHOW);
	m_GameClientView.m_btAddScore.EnableWindow(((m_bShowHand==false)&&(m_bAddScore==false)&&(lLeaveScore>0))?TRUE:FALSE);

	//��ʾ��ť
	if( lUserLessScore == 0L )
		m_GameClientView.m_btNoAdd.ShowWindow(SW_SHOW);
	else
		m_GameClientView.m_btFollow.ShowWindow(SW_SHOW);
	m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
	m_GameClientView.m_btShowHand.ShowWindow(SW_SHOW);

	//���ư�ť
	m_GameClientView.m_btNoAdd.EnableWindow((lUserLessScore==0L)?TRUE:FALSE);
	m_GameClientView.m_btShowHand.EnableWindow((lMaxLeaveScore>0L)&&(cbCardCount>=3)?TRUE:FALSE);
	m_GameClientView.m_btFollow.EnableWindow(((m_bShowHand==false)&&(lUserLessScore>0L))?TRUE:FALSE);

	return;
}

//��ʼ��ť
LRESULT	CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_START_GAME);

	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_PlayerJeton[i].SetScore(0L);
	}
	m_GameClientView.m_PlayerJeton[GAME_PLAYER].SetScore(0L);

	//���ý���
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//������ť
LRESULT CGameClientDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//��������
	m_bAddScore=true;
	HideScoreControl();

	//��������
	KillGameTimer(IDI_USER_ADD_SCORE);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	//������Ϣ
	SendData(SUB_C_GIVE_UP);

	return 0;
}

//��ע��ť
LRESULT CGameClientDlg::OnFollow(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	WORD wMeChairID=GetMeChairID();
	LONG lCurrentScore=m_lTurnLessScore-m_lTableScore[wMeChairID*2+1];

	//��ע����
	m_bAddScore=true;
	HideScoreControl();
	m_GameClientView.m_PlayerJeton[2].SetScore(lCurrentScore);

	//��������
	KillGameTimer(IDI_USER_ADD_SCORE);
	PlayGameSound(AfxGetInstanceHandle(),0L==lCurrentScore?TEXT("NO_ADD"):TEXT("FOLLOW"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//��ע��ť 
LRESULT	CGameClientDlg::OnAddScore(WPARAM wParam, LPARAM lParam)
{
	//ȡ����ť
	if( lParam == -1 )
	{
		UpdateScoreControl();
		return 0;
	}
	else if( lParam == 0 )
	{
		//���ӻ��ע
		return OnFollow(0,0);
	}

	//��ȡ����
	WORD wMeChairID=GetMeChairID();
	LONG lCurrentScore=m_lTurnLessScore-m_lTableScore[wMeChairID*2+1]+m_lCellScore*(lParam);

	//��ע����
	m_bAddScore=true;
	HideScoreControl();
	m_GameClientView.m_PlayerJeton[2].SetScore(lCurrentScore);

	//��������
	KillGameTimer(IDI_USER_ADD_SCORE);
	m_GameClientView.UpdateGameView(NULL);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//�����ť
LRESULT CGameClientDlg::OnShowHand(WPARAM wParam, LPARAM lParam)
{
	//��������
	WORD wMeChairID=GetMeChairID();
	const tagUserData * pUserData=NULL;

	//��ȡ�û�
	if (wMeChairID!=INVALID_CHAIR)
	{
		pUserData=GetUserData(GetMeChairID());
		if (pUserData==NULL) return 0;
	}

	//��֤
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[2].GetCardCount();
	if( cbCardCount < 3 || m_wCurrentUser!=wMeChairID ) return 0;

	//���ͳ��
	LONG lTurnAddScore=m_lTableScore[wMeChairID*2];
	LONG lCurrentScore=pUserData->lScore-m_lTableScore[wMeChairID*2+1];
	LONG lDrawAddScore=m_lTableScore[wMeChairID*2]+m_lTableScore[wMeChairID*2+1];

	//��ע����
	m_bAddScore=true;
	HideScoreControl();
	m_GameClientView.m_PlayerJeton[2].SetScore(lCurrentScore);

	//��������
	KillGameTimer(TIME_USER_ADD_SCORE);
	m_GameClientView.UpdateGameView(NULL);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//�������
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_wCurrentUser==INVALID_CHAIR) return 0;

	//��ע״̬
	if ((IsLookonMode()==false)&&(GetMeChairID()==m_wCurrentUser))
	{
		//��������
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
