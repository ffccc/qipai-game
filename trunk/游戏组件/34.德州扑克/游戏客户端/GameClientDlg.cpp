#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE			201									//��ע��ʱ��

//ʱ���ʶ
#define TIME_START_GAME				30	/*20*/								//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			30	/*20*/								//������ʱ��

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,		OnStart)
	ON_MESSAGE(IDM_EXIT,		OnExit)
	ON_MESSAGE(IDM_GIVEUP,		OnGiveUp)
	ON_MESSAGE(IDM_ADD_SCORE,	OnAddScore)
	ON_MESSAGE(IDM_MIN_SCORE,	OnMinScore)
	ON_MESSAGE(IDM_MAX_SCORE,	OnMaxScore)
	ON_MESSAGE(IDM_OK_SCORE,	OnOKScore)
	ON_MESSAGE(IDM_CANCEL_SCORE,OnCancelScore)
	ON_MESSAGE(IDM_PASS_CARD,	OnPassCard)
	ON_MESSAGE(IDM_FOLLOW,		OnFollow)
	ON_MESSAGE(IDM_SHOWHAND,	OnShowHand)
	ON_MESSAGE(IDM_START_TIMES,	OnStartTimes)
	ON_MESSAGE(IDM_AUTO_START,	OnAutoStart)
	ON_MESSAGE(IDM_SIT_DOWN,	OnSitDown)
	ON_MESSAGE(IDM_GAME_OVER,	OnGameOver)
	ON_MESSAGE(IDM_SEND_FINISH,	OnSendFinish)
	ON_MESSAGE(IDM_OPEN_CARD,	OnOpenCard)
END_MESSAGE_MAP()

////�����Ϣ
//void TraceMessage(LPCTSTR pszMessage)
//{
//	CFile File;
//	if ((File.Open(TEXT("TraceData.txt"),CFile::modeWrite)==FALSE)&&
//		(File.Open(TEXT("TraceData.txt"),CFile::modeWrite|CFile::modeCreate)==FALSE))
//	{
//		ASSERT(FALSE);
//		return;
//	}
//
//	File.SeekToEnd();
//	File.Write(pszMessage,lstrlen(pszMessage));
//	File.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));
//
//	File.Flush();
//	File.Close();
//
//	return;
//}
//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	//m_wMeChairID = INVALID_CHAIR;
	ZeroMemory(m_bUserName,sizeof(m_bUserName));
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_bOpenCard = false;
	m_bExitTag	= false;

	//��ע��Ϣ
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;
	//ZeroMemory(m_lCurrentScore,sizeof(m_lCurrentScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//�˿˱���
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbOverCardData,sizeof(m_cbOverCardData));

	//״̬����
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bAutoStart = FALSE;
	m_bReset =true;

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("�����˿�  --  Ver��6.0.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//���Դ���
#ifdef _DEBUG
	//m_GameClientView.m_btExit.ShowWindow(5);
#endif 

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	if(!m_bReset)return;

	//��Ϸ����
	m_wDUser = INVALID_CHAIR;
	m_wCurrentUser = INVALID_CHAIR;
	m_bOpenCard = false;
	m_bExitTag	= false;

	//��ע��Ϣ
	m_lAddLessScore = 0L;
	m_lTurnLessScore = 0L;
	m_lBalanceScore = 0L;
	m_lCenterScore = 0L;
	m_lCellScore = 0L;
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//�˿˱���
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbOverCardData,sizeof(m_cbOverCardData));

	//״̬����
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	m_bAutoStart = FALSE;

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	//GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bAllowLookon=IsAllowUserLookon();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//�����˿�
		//m_dwCardHSpace=GameOption.m_dwCardHSpace;
		//for (int i=0;i<GAME_PLAYER;i++) m_GameClientView.m_CardControl[i].SetCardSpace(m_dwCardHSpace);

		//��������
		EnableSound(GameOption.m_bEnableSound);
		AllowUserLookon(0,GameOption.m_bAllowLookon);

		//��������
		//AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch(nTimerID)
	{
	case IDI_START_GAME:
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 					
					PostMessage(WM_CLOSE,0,0);
				return true;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			return true;
		}
	case IDI_USER_ADD_SCORE:
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					//ɾ����ʱ��
					KillGameTimer(IDI_USER_ADD_SCORE);
					OnGiveUp(0,0);
				}
				return true;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			return true;
		}
	}
	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�ж��Թ�
	if(bLookonUser)
	{
		WORD wMeChiarID=GetMeChairID();
		if(IsAllowLookon())
		{
			m_GameClientView.m_CardControl[wMeChiarID].SetCardData(m_cbHandCardData[wMeChiarID],MAX_COUNT);
			m_GameClientView.m_CardControl[wMeChiarID].SetDisplayItem(true);
		}
		else 
		{
			m_GameClientView.m_CardControl[wMeChiarID].SetCardData(NULL,0);
			m_GameClientView.m_CardControl[wMeChiarID].SetDisplayItem(false);
		}
	}

	return ;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//�û���ע
		{
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:			//�û�����
		{
			return OnSubGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_OPEN_CARD:		//�û�����
		{
			return OnSubOpenCard(pBuffer,wDataSize);
		}
	}
	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	//��ʾ�û�
	m_GameClientView.SetShowUserView(TRUE);

	//����λ��
	m_wMeChairID = GetMeChairID();
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetUserPost(i,SwitchViewChairID(i));
		m_GameClientView.SetMePost(m_wMeChairID);
	}

	switch (cbGameStation)
	{
	case GS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//����״̬
			SetGameStatus(GS_FREE);

			//��ע��Ϣ
			m_GameClientView.SetTitleInfo(pStatusFree->lCellMinScore,pStatusFree->lCellMaxScore);

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btExit.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAutoStart.ShowWindow(SW_SHOW);
			}

			//����ʱ��
			SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);

			//�Թ�����
			OnLookonChanged(bLookonOther,NULL,0);

			return true;
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//����״̬
			SetGameStatus(GS_PLAYING);

			//���ñ���
			m_wDUser = pStatusPlay->wDUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;//��ǰ���
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore));//��ע��Ŀ
			CopyMemory(m_lTotalScore,pStatusPlay->lTotalScore,sizeof(m_lTotalScore));//��ע��Ŀ
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));//�û���Ϸ״̬

			//��ע��Ϣ
			m_lAddLessScore = pStatusPlay->lAddLessScore;
			m_lCellScore	= pStatusPlay->lCellScore;
			m_lTurnMaxScore = pStatusPlay->lTurnMaxScore;
			m_lTurnLessScore= pStatusPlay->lTurnLessScore;
			m_lCenterScore	= pStatusPlay->lCenterScore;
			CopyMemory(m_cbHandCardData[GetMeChairID()],pStatusPlay->cbHandCardData,MAX_COUNT);
			CopyMemory(m_cbCenterCardData,pStatusPlay->cbCenterCardData,sizeof(m_cbCenterCardData));

			//�����˿�
			if(!IsLookonMode())
			{
				if(m_cbPlayStatus[GetMeChairID()]==TRUE)
					m_GameClientView.m_CardControl[GetMeChairID()].SetCardData(m_cbHandCardData[GetMeChairID()],MAX_COUNT);
				m_GameClientView.m_btAutoStart.ShowWindow(SW_SHOW);
			}

			//�����˿�
			if(pStatusPlay->cbBalanceCount >0)
			{
				BYTE cbTempCount = pStatusPlay->cbBalanceCount+2;
				m_GameClientView.m_CenterCardControl.SetCardData(m_cbCenterCardData,min(cbTempCount,MAX_CENTERCOUNT));
			}

			//���ý���
			for (WORD i =0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i] == FALSE)continue;
				m_GameClientView.m_SmallCardControl[i].SetCardData(MAX_COUNT);
				m_GameClientView.SetUserTableScore(i,m_lTableScore[i]);
				m_GameClientView.SetTotalScore(i,m_lTotalScore[i]);
			}
			m_GameClientView.SetDFlag(m_wDUser);
			m_GameClientView.SetTitleInfo(m_lCellScore,pStatusPlay->lCellMaxScore);
			m_GameClientView.SetCenterScore(m_lCenterScore);

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			//��ǰ���
			if((IsLookonMode() == false)&&(m_wCurrentUser == GetMeChairID()))
				UpdateScoreControl();

			//����ʱ��
			SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

			//�Թ�����
			OnLookonChanged(bLookonOther,NULL,0);

			//���°�ť
			if(IsLookonMode())
			{
				m_GameClientView.SetMyLookOn(GetMeChairID());

				//���½���
				m_GameClientView.UpdateGameView(NULL);
			}

			return true;
		}
	}
	return false;
}

//�����Ϣ
bool CGameClientDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_GF_MESSAGE:		//ϵͳ��Ϣ
		{
			//Ч�����
			CMD_GF_Message * pMessage=(CMD_GF_Message *)pBuffer;
			ASSERT(wDataSize>(sizeof(CMD_GF_Message)-sizeof(pMessage->szContent)));
			if (wDataSize<=(sizeof(CMD_GF_Message)-sizeof(pMessage->szContent))) return false;

			//�رշ���
			if (pMessage->wMessageType&SMT_CLOSE_GAME) 
			{
				//���ذ�ť
				m_GameClientView.m_btAutoStart.ShowWindow(SW_HIDE);
				m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
				m_GameClientView.m_btExit.ShowWindow(SW_SHOW);

				//ֹͣ��λ
				m_bReset = false;
				m_GameClientView.SetResetTag();
				ZeroMemory(m_bUserName,sizeof(m_bUserName));
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//��������
					const tagUserData * pUserData=GetUserData(i);
					if(pUserData!=NULL)
					{
						//�û�����
						CopyMemory(&m_bUserName[i],pUserData->szName,sizeof(pUserData->szName));

						//�û��ձ�
						WORD wTemp = ((pUserData->cbGender==0)?2:1);
						m_GameClientView.SetUserGender(i,wTemp);
					}
				}

				//��Ϣ��ʾ
				TCHAR szBuffer[1024]=TEXT("");
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n��ϵͳ��Ϣ��"));
				InsertGeneralString(szBuffer,RGB(255,0,0),true);

				//_snprintf(szBuffer,sizeof(szBuffer),TEXT("��Ľ������%ld,���ܼ�����Ϸ��"),m_lCellScore*3);
				InsertGeneralString(pMessage->szContent,RGB(0,0,255),true);

				//��ʾ��
				m_GameClientView.m_Prompt.SetString(pMessage->szContent);

				//���ö�ʱ��
				m_GameClientView.SetNoScoreDlg();

				return true;
			}
		}
	}
	return false;
}

//��ʼ��ť
LRESULT	CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetGameEndEnd();
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//���ý���	
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btExit.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.SetStartTimes(false);
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//��ע����
	m_lCenterScore = 0L;
	m_lCellScore = 0L;

	//���ý���
	for (WORD i = 0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetUserTableScore(i,m_lTableScore[i]);
		m_GameClientView.SetTotalScore(i,0L);
		m_GameClientView.m_SmallCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_CenterCardControl.SetCardData(NULL,0);
	m_GameClientView.SetDFlag(INVALID_CHAIR);
	m_GameClientView.SetCenterScore(0L);

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//�뿪��ť
LRESULT	CGameClientDlg::OnExit(WPARAM wParam, LPARAM lParam)
{
	//��������
	//KillGameTimer(IDI_USER_ADD_SCORE);
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));
#ifdef _DEBUG
	//m_GameClientView.UpdateFrameSize();
	//return 0;
#endif
	PostMessage(WM_CLOSE,0,0);

	return 0;
}

//������ť
LRESULT CGameClientDlg::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//��������
	KillGameTimer(IDI_USER_ADD_SCORE);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	//������Ϣ
	SendData(SUB_C_GIVE_UP);

	HideScoreControl();

	return 0;
}

//��ע��ť
LRESULT CGameClientDlg::OnFollow(WPARAM wParam,LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_USER_ADD_SCORE);

	//��ȡ����
	WORD wMeChairID=GetMeChairID();
	m_lTableScore[wMeChairID] +=m_lTurnLessScore;
	m_lTotalScore[wMeChairID] += m_lTurnLessScore;
	m_GameClientView.SetTotalScore(wMeChairID,m_lTotalScore[wMeChairID]);

	if(m_lTableScore[wMeChairID]!=0L)
		m_GameClientView.DrawMoveAnte(wMeChairID,CGameClientView::AA_BASEFROM_TO_BASEDEST,m_lTurnLessScore);

	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
	HideScoreControl();

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=m_lTurnLessScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//��ע��ť 
LRESULT	CGameClientDlg::OnAddScore(WPARAM wParam, LPARAM lParam)
{
	if (m_GameClientView.m_GoldControl.IsWindowVisible()==FALSE)
	{
		m_GameClientView.m_GoldControl.SetMaxGold((LONG)m_lTurnMaxScore);
		m_GameClientView.m_GoldControl.SetMinGold((LONG)(min(m_lAddLessScore,m_lTurnMaxScore)));
		m_GameClientView.m_GoldControl.SetGold((LONG)(min(m_lAddLessScore,m_lTurnMaxScore)));
		m_GameClientView.m_GoldControl.ShowWindow(SW_SHOW);
	}

	return 0; 
}

//���ټ�ע
LRESULT	CGameClientDlg::OnMinScore(WPARAM wParam, LPARAM lParam)
{	
	//���ٳ���
	m_GameClientView.m_GoldControl.SetGold((LONG)(min(m_lAddLessScore,m_lTurnMaxScore)));

	return 0;
}

//����ע
LRESULT	CGameClientDlg::OnMaxScore(WPARAM wParam, LPARAM lParam)
{	
	//������
	m_GameClientView.m_GoldControl.SetGold((LONG)m_lTurnMaxScore);

	return 0;
}

//ȷ����Ϣ
LRESULT CGameClientDlg::OnOKScore(WPARAM wParam,LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_USER_ADD_SCORE);
	HideScoreControl();

	//��ȡ����
	WORD wMeChairID=GetMeChairID();
	LONG lCurrentScore=m_GameClientView.m_GoldControl.GetGold();
	m_lTableScore[GetMeChairID()] += lCurrentScore;
	m_lTotalScore[GetMeChairID()] += lCurrentScore;
	m_GameClientView.SetTotalScore(wMeChairID,m_lTotalScore[wMeChairID]);

	if(lCurrentScore>0L)
		m_GameClientView.DrawMoveAnte(wMeChairID,CGameClientView::AA_BASEFROM_TO_BASEDEST,lCurrentScore);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//ȡ����Ϣ
LRESULT CGameClientDlg::OnCancelScore(WPARAM wParam,LPARAM lParam)
{
	m_GameClientView.m_GoldControl.ShowWindow(SW_HIDE);

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnPassCard(WPARAM wParam,LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameTimer(IDI_USER_ADD_SCORE);
	HideScoreControl();

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=0L;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	//����Ч��
	PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));

	return 0;
}

//�����Ϣ
LRESULT CGameClientDlg::OnShowHand(WPARAM wParam,LPARAM lParam)
{
	//��������
	ShowHand ShowHandObj;

	//��������
	if (ShowHandObj.DoModal()==IDOK)
	{
		for(BYTE i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData = GetUserData(i);
			if(pUserData!=NULL && pUserData->cbUserStatus==US_PLAY)break;
		}
		if(i<GAME_PLAYER)
		{
			//ɾ����ʱ��
			KillGameTimer(IDI_USER_ADD_SCORE);

			//��ȡ����
			WORD wMeChairID=GetMeChairID();
			m_lTableScore[GetMeChairID()] +=m_lTurnMaxScore;
			m_lTotalScore[GetMeChairID()] += m_lTurnMaxScore;
			m_GameClientView.SetTotalScore(GetMeChairID(),m_lTotalScore[GetMeChairID()]);
			m_GameClientView.SetUserTableScore(GetMeChairID(),m_lTableScore[GetMeChairID()]);

			m_GameClientView.UpdateGameView(NULL);
			HideScoreControl();

			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.lScore=m_lTurnMaxScore;
			SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//����Ч��
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
		}
	}

	return 0;
}

//��ʼ����
LRESULT CGameClientDlg::OnStartTimes(WPARAM wParam,LPARAM lParam)
{
	//���ؿؼ�
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);

	if(m_bAutoStart!=TRUE)
	{
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}
	else //�Զ���ʼ
	{
		OnStart(0,0);
	}

	return 0;
}

//�Զ���ʼ
LRESULT CGameClientDlg::OnAutoStart(WPARAM wParam,LPARAM lParam)
{
	m_bAutoStart = !m_bAutoStart;
	HINSTANCE hInstance=AfxGetInstanceHandle();
	if(m_bAutoStart==TRUE)
	{
		if(m_GameClientView.m_btStart.IsWindowVisible()==TRUE)
		{
			OnStart(0,0);
		}
		m_GameClientView.m_btAutoStart.SetButtonImage(IDB_AUTO_START_TRUE,hInstance,false);
	}
	else
	{
		const tagUserData *pUserData = GetUserData(GetMeChairID());
		if(pUserData->cbUserStatus!=US_READY && pUserData->cbUserStatus!=US_PLAY &&
			m_GameClientView.m_btStart.IsWindowVisible()==FALSE)
		{
			m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
			m_GameClientView.m_btExit.ShowWindow(SW_SHOW);
		}
		m_GameClientView.m_btAutoStart.SetButtonImage(IDB_AUTO_START_FALSE,hInstance,false);
	}

	return 0;
}

//���°�ť
LRESULT CGameClientDlg::OnSitDown(WPARAM wParam,LPARAM lParam)
{
	//״̬����
	//MessageBox("����������Ϸ��,����ʱ���ܼ��롣");
	//if(GetGameStatus()==GS_PLAYING)return 0; 

	//���ؿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_btSitDown[i].ShowWindow(SW_HIDE);
	}

	//ȡ��״̬
	m_GameClientView.SetMyLookOn(INVALID_CHAIR);

	//������Ϸ
	JoinInGameTable(LOWORD(wParam));
	//IClientKernel * pIClientKernel=(IClientKernel*)GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	////��������
	//IPC_JoinInGame JoinInGame;
	//JoinInGame.wTableID=pIClientKernel->GetMeUserInfo()->wTableID;
	//JoinInGame.wChairID=LOWORD(wParam);

	////��������
	//ASSERT(pIClientKernel!=NULL);
	//if(pIClientKernel==NULL) return 0;
	//pIClientKernel->SendProcessData(IPC_MAIN_CONCTROL,IPC_SUB_JOIN_IN_GAME,&JoinInGame,sizeof(JoinInGame));

	return 0;
}

//��Ϸ����
LRESULT CGameClientDlg::OnGameOver(WPARAM wParam,LPARAM lParam)
{
	//����״̬
	SetGameStatus(GS_FREE);

	if(!m_bExitTag)
	{
		if(m_cbPlayStatus[m_wMeChairID] == TRUE)
			m_GameClientView.m_CardControl[m_wMeChairID].SetCardData(m_cbHandCardData[m_wMeChairID],2);
	}
	else
	{
		//ʤ���б�
		UserWinList WinnerList;

		//��ʱ����
		BYTE bTempData[GAME_PLAYER][MAX_CENTERCOUNT];
		CopyMemory(bTempData,m_cbOverCardData,GAME_PLAYER*MAX_CENTERCOUNT);

		//����ʤ����
		m_GameLogic.SelectMaxUser(bTempData,WinnerList,NULL);
		ASSERT(WinnerList.bSameCount>0);

		//�����˿�
		for (WORD i = 0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i] == TRUE) m_GameClientView.m_CardControl[i].SetCardData(m_cbHandCardData[i],2);
			else m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		}

		//��Ч����
		bool wIsMyWin =false ;
		WORD wWinnerID = INVALID_CHAIR;		
		BYTE cbEffectHandCard[MAX_COUNT];
		BYTE cbEffectCenterCardData[MAX_CENTERCOUNT];
		ZeroMemory(cbEffectHandCard,sizeof(cbEffectHandCard));
		ZeroMemory(cbEffectCenterCardData,sizeof(cbEffectCenterCardData));
		BYTE bTempCount1,bTempCount2;

		//����ʤ���˿�
		for (WORD i=0;i<WinnerList.bSameCount;i++)
		{
			wWinnerID=WinnerList.wWinerList[i];
			if(!wIsMyWin && m_wMeChairID==WinnerList.wWinerList[i])
			{
				wIsMyWin = true;
			}

			//�����˿�����
			BYTE bTempCount1=m_GameLogic.GetSameCard(m_cbHandCardData[wWinnerID],bTempData[wWinnerID],MAX_COUNT,MAX_CENTERCOUNT,cbEffectHandCard);
			BYTE bTempCount2=m_GameLogic.GetSameCard(m_cbCenterCardData,bTempData[wWinnerID],MAX_CENTERCOUNT,MAX_CENTERCOUNT,cbEffectCenterCardData);
			ASSERT(bTempCount1+bTempCount2<=MAX_CENTERCOUNT);

			//�����˿���Ч����
			m_GameClientView.m_CardControl[wWinnerID].SetCardEffect(cbEffectHandCard,bTempCount1);
			m_GameClientView.m_CenterCardControl.SetCardEffect(cbEffectCenterCardData,bTempCount2);
		}

		//�Լ��˿�
		if(!wIsMyWin)
		{
			wWinnerID = m_wMeChairID;

			//�Լ��˿�����
			ZeroMemory(cbEffectHandCard,sizeof(cbEffectHandCard));
			ZeroMemory(cbEffectCenterCardData,sizeof(cbEffectCenterCardData));

			//�����˿�����
			bTempCount1=m_GameLogic.GetSameCard(m_cbHandCardData[wWinnerID],bTempData[wWinnerID],MAX_COUNT,MAX_CENTERCOUNT,cbEffectHandCard);
			bTempCount2=m_GameLogic.GetSameCard(m_cbCenterCardData,bTempData[wWinnerID],MAX_CENTERCOUNT,MAX_CENTERCOUNT,cbEffectCenterCardData);
			ASSERT(bTempCount1+bTempCount2<=MAX_CENTERCOUNT);

			//���ñ�־�˿�����
			m_GameClientView.m_CardControl[wWinnerID].SetMyCard(cbEffectHandCard,bTempCount1);
			m_GameClientView.m_CenterCardControl.SetMyCard(cbEffectCenterCardData,bTempCount2);
		}

		//��Ϸ����
		m_GameClientView.SetGameEndStart();
	}

	//Ӯ���
	for (WORD i =0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i] == FALSE) continue;
		if(m_dEndScore[i]>0L)
		{
			m_lCenterScore = m_lCenterScore -m_dEndScore[i]-m_lTotalScore[i];
			m_GameClientView.UpdateWindow();
			m_GameClientView.DrawMoveAnte(i,CGameClientView::AA_CENTER_TO_BASEFROM,m_dEndScore[i]+m_lTotalScore[i]);
			m_GameClientView.SetCenterScore(m_lCenterScore);
		}
		else if(m_dEndScore[i] == 0L)
		{
			m_GameClientView.DrawMoveAnte(i,CGameClientView::AA_CENTER_TO_BASEFROM,m_lTotalScore[i]);
			m_lCenterScore = m_lCenterScore-m_lTotalScore[i];
			m_GameClientView.SetCenterScore(m_lCenterScore);
			m_GameClientView.UpdateWindow();
		}
		m_GameClientView.SetTotalScore(i,0L);
		m_GameClientView.UpdateWindow();
	}

	//��������
	if (IsLookonMode()==false)
	{
		if (m_dEndScore[m_wMeChairID]>=0L) 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	if(m_cbPlayStatus[m_wMeChairID]==TRUE && !IsLookonMode())
	{
		//����λ��
		CRect rcControl;
		m_GameClientView.m_ScoreView.GetWindowRect(&rcControl);
		CRect rcView ;
		m_GameClientView.GetWindowRect( &rcView );
		m_GameClientView.m_ScoreView.MoveWindow(rcView.left+5,rcView.bottom-15-rcControl.Height()*3/2,rcControl.Width(),rcControl.Height()/*nWidth/2-rcControl.Width()/2,nHeight/2+56,0,0,SWP_NOZORDER|SWP_NOSIZE*/);

		m_GameClientView.m_ScoreView.SetGameScore(m_wMeChairID,m_dEndScore[m_wMeChairID]);
		m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
		m_GameClientView.m_ScoreView.SetShowTimes();
	}
	if(!IsLookonMode())
	{
		//���ư�ť
		if(m_bOpenCard)
		{
			m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
		}

		if (m_GameClientView.m_ScoreView.IsWindowVisible()==TRUE)
		{
			m_GameClientView.m_ScoreView.SetStartTimes();
		}
		else if(m_bAutoStart==FALSE)
		{
			SetGameTimer(m_wMeChairID,IDI_START_GAME,TIME_START_GAME);
		}
		else //�Զ���ʼ
		{
			m_GameClientView.m_ScoreView.SetStartTimes();
			m_GameClientView.m_ScoreView.SetShowTimes();
			//OnStart(0,0);
		}
	}

	//״̬����
	KillGameTimer(IDI_USER_ADD_SCORE);

	//��ʼ��ť
	if (!IsLookonMode() && m_bAutoStart==FALSE)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btExit.ShowWindow(SW_SHOW);
	}

	//�ɼ���ʾ�ڼ�ʱ����Ի���
	TCHAR szBuffer[256]=TEXT("");
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n���ֽ���,�ɼ�ͳ��"));
	InsertGeneralString(szBuffer,RGB(0,128,255),true);

	if(m_bReset)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_lTotalScore[i]==0)continue;	
			const tagUserData * pUserData=GetUserData(i);
			//�ɼ����
			if (pUserData!=NULL)
			{
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��%+ld"),/*%s���\n�÷�:%ld*/
					pUserData->szName,m_dEndScore[i]);
				InsertGeneralString(szBuffer,RGB(0,128,255),true);
			}
			else
			{
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("�û����뿪��%+ld"),-m_lTotalScore[i]);/*\n�÷�:%ld*/
				InsertGeneralString(szBuffer,RGB(0,128,255),true);
			}
		}
	}
	else	//������
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_lTotalScore[i]==0)continue;
			//�ɼ����
			if (m_bUserName[i]!=NULL)
			{
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��%+ld"),
					&m_bUserName[i],m_dEndScore[i]);
				InsertGeneralString(szBuffer,RGB(0,128,255),true);
			}
			else
			{
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("�û����뿪��%+ld"),-m_lTotalScore[i]);
				InsertGeneralString(szBuffer,RGB(0,128,255),true);
			}
		}
	}

	//��ֵ����
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lCenterScore = 0L;

	return 0;
}

//���ƽ���
LRESULT CGameClientDlg::OnSendFinish(WPARAM wParam,LPARAM lParam)
{
	//���ƽ���
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}
	if(m_wCurrentUser<GAME_PLAYER)
	{
		SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);
	}

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnOpenCard(WPARAM wParam,LPARAM lParam)
{
	//���ؿؼ�
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);

	//������Ϣ
	SendData(SUB_C_OPEN_CARD,NULL,0);

	return 0;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	m_GameClientView.SetCenterCount();

	//����״̬
	SetGameStatus(GS_PLAYING);

	//�Թ����
	if(IsLookonMode())
	{
		//�������
		m_GameClientView.SetGameEndEnd();
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		m_GameClientView.m_btExit.ShowWindow(SW_HIDE);
		m_GameClientView.m_ScoreView.SetStartTimes(false);
		ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
		ZeroMemory(m_cbCenterCardData,sizeof(m_cbCenterCardData));
		ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
		for (WORD i = 0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.SetUserTableScore(i,m_lTableScore[i]);
			m_GameClientView.SetTotalScore(i,0L);
			m_GameClientView.m_SmallCardControl[i].SetCardData(NULL,0);
			m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		}
		m_GameClientView.m_CenterCardControl.SetCardData(NULL,0);
		m_GameClientView.SetDFlag(INVALID_CHAIR);
		m_GameClientView.SetCenterScore(0L);
		m_lCenterScore = 0L;
		m_lCellScore = 0L;
	}

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//���ñ���
	m_wDUser = pGameStart->wDUser;
	m_wCurrentUser = pGameStart->wCurrentUser;
	m_lAddLessScore = pGameStart->lAddLessScore;
	m_lTurnLessScore = pGameStart->lTurnLessScore;
	m_lTurnMaxScore = pGameStart->lTurnMaxScore;
	m_lCellScore = pGameStart->lCellScore;

	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		const tagUserData * pUserData=GetUserData(i);
		if (pUserData!=NULL) 
		{
			//��Ϸ��Ϣ
			//m_GameClientView.SetUserIdInfo(i,pUserData);
			m_cbPlayStatus[i]=TRUE;
		}
		else 
		{
			//m_GameClientView.SetUserIdInfo(i,NULL);
			m_cbPlayStatus[i]=FALSE;
		}
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//��ע��Ϣ
	m_lTableScore[pGameStart->wDUser] += m_lCellScore;
	m_lTableScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;
	m_lTotalScore[pGameStart->wDUser] =  m_lCellScore;
	m_lTotalScore[pGameStart->wMaxChipInUser] = 2*m_lCellScore;

	//���ý���
	m_GameClientView.SetDFlag(m_wDUser);
	m_GameClientView.DrawMoveAnte(pGameStart->wDUser,CGameClientView::AA_BASEFROM_TO_BASEDEST,m_lTotalScore[pGameStart->wDUser]);
	m_GameClientView.DrawMoveAnte(pGameStart->wMaxChipInUser,CGameClientView::AA_BASEFROM_TO_BASEDEST,m_lTotalScore[pGameStart->wMaxChipInUser]);	
	m_GameClientView.SetTotalScore(pGameStart->wDUser,m_lTotalScore[pGameStart->wDUser]);
	m_GameClientView.SetTotalScore(pGameStart->wMaxChipInUser,m_lTotalScore[pGameStart->wMaxChipInUser]);

	//���Ͱ���
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(m_cbHandCardData));
	for (BYTE j = 0;j<2;j++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				if(GetMeChairID()==i)
				{
					m_GameClientView.DrawMoveCard(i,TO_USERCARD,m_cbHandCardData[i][j]);
				}
				else 
				{
					m_GameClientView.DrawMoveCard(i,TO_USERCARD,0);
				}
			}
		}
	}

	////�ؼ�����
	//if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	//{
	//	ActiveGameFrame();
	//	UpdateScoreControl();
	//}
	//SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

	return true;
}

//��ע��Ϣ
bool CGameClientDlg::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;

	//��ע����
	if ((IsLookonMode()==true)||(pAddScore->wAddScoreUser!=wMeChairID))
	{
		//��ע����
		if(pAddScore->lAddScoreCount>0)
		{
			m_GameClientView.DrawMoveAnte(pAddScore->wAddScoreUser,CGameClientView::AA_BASEFROM_TO_BASEDEST,pAddScore->lAddScoreCount);
			//m_GameClientView.SetUserTableScore(pAddScore->wAddScoreUser,pAddScore->lAddScoreCount+m_lTableScore[pAddScore->wAddScoreUser]);
			m_lTotalScore[pAddScore->wAddScoreUser] += pAddScore->lAddScoreCount;
			m_GameClientView.SetTotalScore(pAddScore->wAddScoreUser,m_lTotalScore[pAddScore->wAddScoreUser]);
			m_lTableScore[pAddScore->wAddScoreUser] +=pAddScore->lAddScoreCount;
		}

		//��������
		if (m_cbPlayStatus[wAddScoreUser]==TRUE)
		{
			//��������
			if (pAddScore->lAddScoreCount==0L) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));
			else if (pAddScore->lAddScoreCount==m_lTurnMaxScore)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
			else if ((pAddScore->lAddScoreCount)==m_lTurnLessScore)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("FOLLOW"));
			else 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		}
	}

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;
	m_lTurnMaxScore = pAddScore->lTurnMaxScore;
	m_lAddLessScore = pAddScore->lAddLessScore;

	//���ƽ���
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	if (m_wCurrentUser==INVALID_CHAIR) 
	{
		KillGameTimer(IDI_USER_ADD_SCORE);
		//һ��ƽ��
		//���Ļ����ۼ�
		for (WORD i =0;i<GAME_PLAYER;i++)
		{			
			m_lCenterScore += m_lTableScore[i];
		}

		//�����ƶ�
		for (WORD i =0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i] == FALSE) continue;
			if(m_lTableScore[i]!=0L)
			{
				//m_GameClientView.SetUserTableScore(i,m_lTableScore[i]);
				m_GameClientView.DrawMoveAnte(i,CGameClientView::AA_BASEDEST_TO_CENTER,m_lTableScore[i]);
				m_GameClientView.SetUserTableScore(i,0L);
			}
		}

		//m_GameClientView.SetCenterScore(m_lCenterScore);

		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
		m_GameClientView.UpdateGameView(NULL);
		m_GameClientView.UpdateWindow();
	}
	else if(m_wCurrentUser<GAME_PLAYER)SetGameTimer(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

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
	m_GameClientView.m_SmallCardControl[pGiveUp->wGiveUpUser].SetCardData(NULL,0);
	m_GameClientView.m_CardControl[pGiveUp->wGiveUpUser].SetCardData(NULL,0);
	m_GameClientView.UpdateWindow();
	m_GameClientView.DrawMoveCard(pGiveUp->wGiveUpUser,TO_GIVEUP_CARD,0);

	//״̬����
	if ((IsLookonMode()==false)&&(pGiveUp->wGiveUpUser==GetMeChairID()))
		SetGameStatus(GS_FREE);

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;

	//��������
	if (wGiveUpUser==GetTimeChairID())
		KillGameTimer(IDI_USER_ADD_SCORE);

	if ((IsLookonMode()==true)||(wGiveUpUser!=GetMeChairID()))
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	//��ʾ����
	if (wGiveUpUser==GetMeChairID())
	{
		if(m_lTableScore[wGiveUpUser]!=0L)
		{
			m_GameClientView.DrawMoveAnte(wGiveUpUser,CGameClientView::AA_BASEDEST_TO_CENTER,m_lTableScore[wGiveUpUser]);
			m_GameClientView.SetUserTableScore(wGiveUpUser,0L);
			m_lTableScore[wGiveUpUser] = 0L;
		}
		if(!IsLookonMode())
		{
			//����λ��
			CRect rcControl;
			m_GameClientView.m_ScoreView.GetWindowRect(&rcControl);
			CRect rcView ;
			m_GameClientView.GetWindowRect( &rcView );
			m_GameClientView.m_ScoreView.MoveWindow(rcView.left+5,rcView.bottom-15-rcControl.Height()*3/2,rcControl.Width(),rcControl.Height());

			m_GameClientView.m_ScoreView.SetGameScore(wGiveUpUser,pGiveUp->lLost);
			m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
			m_GameClientView.m_ScoreView.SetShowTimes();
		}
	}
	else
	{
		if(m_lTableScore[wGiveUpUser]!=0L)
		{
			m_GameClientView.DrawMoveAnte(wGiveUpUser,CGameClientView::AA_BASEDEST_TO_CENTER,m_lTableScore[wGiveUpUser]);
			m_GameClientView.SetUserTableScore(wGiveUpUser,0L);
			m_lTableScore[wGiveUpUser] = 0L;
		}
	}

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//У������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//��ǰ���
	m_wCurrentUser = pSendCard->wCurrentUser;
	CopyMemory(m_cbCenterCardData,pSendCard->cbCenterCardData,sizeof(BYTE)*(pSendCard->cbSendCardCount));

	//���͹���
	if((pSendCard->cbSendCardCount >= 3)&&(pSendCard->cbSendCardCount <= 5)&&(pSendCard->cbPublic==0))
	{
		//���͹��� 
		if((pSendCard->cbSendCardCount == 3))
		{
			for (BYTE j = 0;j<pSendCard->cbSendCardCount;j++)
			{
				m_GameClientView.DrawMoveCard(GAME_PLAYER,TO_CENTER_CARD,pSendCard->cbCenterCardData[j]);
			}
		}
		else if((pSendCard->cbSendCardCount >3))
		{
			BYTE bTemp = pSendCard->cbSendCardCount-1;
			m_GameClientView.DrawMoveCard(GAME_PLAYER,TO_CENTER_CARD,pSendCard->cbCenterCardData[bTemp]);
		}
	}

	if((pSendCard->cbSendCardCount == 5)&&(pSendCard->cbPublic >= 1))
	{
		BYTE bFirstCard = pSendCard->cbPublic ;
		if(bFirstCard==1)bFirstCard = 0;
		else if(bFirstCard==2)bFirstCard = 3;
		else if(bFirstCard==3)bFirstCard = 4;
		for (BYTE j = bFirstCard;j<pSendCard->cbSendCardCount;j++)
		{
			m_GameClientView.DrawMoveCard(GAME_PLAYER,TO_CENTER_CARD,pSendCard->cbCenterCardData[j]);
		}
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	HideScoreControl();

	//���Ʊ�־
	m_bOpenCard = false;

	//�����������м�
	for (WORD i = 0;i<GAME_PLAYER;i++)
	{
		if((m_lTableScore[i]>0)&&(m_cbPlayStatus[i]==TRUE))
		{
			m_GameClientView.DrawMoveAnte(i,CGameClientView::AA_BASEDEST_TO_CENTER,m_lTableScore[i]);			
			m_GameClientView.SetUserTableScore(i,0L);
		}
	}

	//������Ϣ
	CopyMemory(m_dEndScore,pGameEnd->lGameScore,sizeof(m_dEndScore));
	CopyMemory(m_cbHandCardData,pGameEnd->cbCardData,sizeof(m_cbHandCardData));
	CopyMemory(m_cbOverCardData,pGameEnd->cbLastCenterCardData,sizeof(m_cbOverCardData));

	//������Ϣ
	if(pGameEnd->cbTotalEnd == 1)
	{
		m_bExitTag = true;
	}
	else 
	{
		if(m_dEndScore[GetMeChairID()]>0L)
		{
			//���Ʊ�־
			m_bOpenCard = true;
		}
		m_bExitTag = false;
	}

	//��ʱ��ʱ��
	if(m_GameClientView.IsMoveing())
	{
		m_GameClientView.SetOverTimer();
	}
	else OnGameOver(0,0);

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_OpenCard)) return false;
	CMD_S_OpenCard * pOpenCard=(CMD_S_OpenCard *)pBuffer;

	WORD wMeChiarID = GetMeChairID();
	const tagUserData * pUserData=GetUserData(wMeChiarID);
	if(pUserData!=NULL && pUserData->cbUserStatus!=US_READY)
	{
		m_GameClientView.m_CardControl[pOpenCard->wWinUser].SetCardData(m_cbHandCardData[pOpenCard->wWinUser],2);
		m_GameClientView.m_CardControl[pOpenCard->wWinUser].SetDisplayItem(true);
		m_GameClientView.UpdateGameView(NULL);
	}

	return true;
}

//���ؿ���
void CGameClientDlg::HideScoreControl()
{
	m_GameClientView.m_GoldControl.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAdd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);

	return;
}

//���¿���
void CGameClientDlg::UpdateScoreControl()
{
	//��ʾ��ť
	if(m_lTurnLessScore>0L)
	{
		m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btFollow.ShowWindow((m_lTurnLessScore==m_lTurnMaxScore)?SW_HIDE:SW_SHOW);
	}
	else
	{
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
	}

	//Ҫ����
	if(m_lAddLessScore>m_lTurnMaxScore)
	{
		m_GameClientView.m_btAdd.ShowWindow(SW_HIDE);
	}
	else
	{
		m_GameClientView.m_btAdd.ShowWindow(SW_SHOW);
	}

	m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
	m_GameClientView.m_btShowHand.ShowWindow(SW_SHOW);

	return;
}

//////////////////////////////////////////////////////////////////////////
