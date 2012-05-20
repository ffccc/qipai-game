#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "resource.h"
#include "ch.h"
//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_HORSE_RUN				301									//����ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	//ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_BANK, OnBank)
	ON_WM_SETTINGCHANGE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	//������ע
	m_lMeScore2=0L;
	m_lMeScore3=0L;
	m_lMeScore1=0L;
	m_lMeScore6=0L;
	m_lMeScore5=0L;
	m_lMeScore4=0L;
	m_lMeScore7=0L;
	m_lMeScore8=0L;

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//״̬����
	m_bMeApplyBanker=false;

	ZeroMemory(&rtMax,sizeof(rtMax));
	
	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}


//��ʼ����

BOOL CGameClientDlg::OnInitDialog()
{
	__super::OnInitDialog();
	
	//SetWindowPos(NULL,200,50,1024,768,SWP_NOZORDER);
	ModifyStyle(WS_THICKFRAME, CS_VREDRAW| CS_HREDRAW|DS_MODALFRAME|DS_CENTER, 0);
	CenterWindow();
	RestoreWindow();

	CRect rtWindow;
	GetWindowRect(&rtWindow);
	ScreenToClient(&rtWindow);
	rtMax.left = rtWindow.right-48;
	rtMax.right = rtMax.left+22;
	rtMax.top = rtWindow.top+2;
	rtMax.bottom = rtMax.top+17;
	//TCHAR str[100];
	//sprintf(str,"%d,%d,%d,%d,\n%d,%d,%d,%d",rtMax.left,rtMax.right,rtMax.top,rtMax.bottom,rtWindow.left,rtWindow.right,rtWindow.top,rtWindow.bottom);
	//MessageBox(str);

	return TRUE;
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("������Ϸ  --  Ver��6.6.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��������
	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));

 
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	m_lMeScore2=0L;
	m_lMeScore3=0L;
	m_lMeScore1=0L;
	m_lMeScore6=0L;
	m_lMeScore5=0L;
	m_lMeScore4=0L;
	m_lMeScore7=0L;
	m_lMeScore8=0L;

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//״̬����
	m_bMeApplyBanker=false;

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	static int num = 0;
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		num = 0;
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);		


		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );

		
	}
	if ((nTimerID==IDI_PLACE_JETTON)&&IsEnableSound()) 
	{
		if (nElapse<=2&&nElapse>1) PlayGameSound(HORSE_READY);
	}


	if (nTimerID == IDI_HORSE_RUN && nElapse > 11 && (nElapse%3) == 0)
	{
		m_GameClientView.SetEveryHorseSpeed(8- (nElapse-11)/3);
	}

	if (nTimerID == IDI_HORSE_RUN)
	{	
		if (nElapse == 34)
		{
			PlayGameSound(RACE_RUN);
		}	
		if (IsEnableSound() && m_GameClientView.GetIsFinishRace())
		{
			
			num ++;
			if (num == 3)
			{
				PlayGameSound(RACE_OVER);
			}						
		}
	}

	return true;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_UPDATE_USER_SCORE:		//���³ɼ�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_UpdateUserScore));
			if (wDataSize!=sizeof(CMD_S_UpdateUserScore)) return false;

			//��Ϣ����
			CMD_S_UpdateUserScore * UpdateUserScore=(CMD_S_UpdateUserScore *)pBuffer;
			m_lMeMaxScore = UpdateUserScore->lUserScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			return true;
		}
	case SUB_S_UPDATE_BANKER_SCORE:	//���³ɼ�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_UpdateBankerScore));
			if (wDataSize!=sizeof(CMD_S_UpdateUserScore)) return false;

			//��Ϣ����
			CMD_S_UpdateBankerScore * UpdateBankerScore=(CMD_S_UpdateBankerScore *)pBuffer;
			m_lBankerScore = UpdateBankerScore->lBankerScore;
			m_GameClientView.SetBankerInfo(m_wCurrentBanker,m_lBankerScore);
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//����ʱ��
			SetGameTimer(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);		

            //�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));
		
            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//����״̬
			SetGameStatus(GS_FREE);

			//���¿���
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_PLAYING:		//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//ȫ����ע
			m_GameClientView.PlaceUserJetton(ID_HORSE1,pStatusPlay->lAllScore1);
			m_GameClientView.PlaceUserJetton(ID_HORSE2,pStatusPlay->lAllScore2);
			m_GameClientView.PlaceUserJetton(ID_HORSE3,pStatusPlay->lAllScore3);
			m_GameClientView.PlaceUserJetton(ID_HORSE4,pStatusPlay->lAllScore4);
			m_GameClientView.PlaceUserJetton(ID_HORSE5,pStatusPlay->lAllScore5);
			m_GameClientView.PlaceUserJetton(ID_HORSE6,pStatusPlay->lAllScore6);
			m_GameClientView.PlaceUserJetton(ID_HORSE7,pStatusPlay->lAllScore7);
			m_GameClientView.PlaceUserJetton(ID_HORSE8,pStatusPlay->lAllScore8);

			//�����ע
			SetMePlaceJetton(ID_HORSE1,pStatusPlay->lUserScore1);
			SetMePlaceJetton(ID_HORSE2,pStatusPlay->lUserScore2);
			SetMePlaceJetton(ID_HORSE3,pStatusPlay->lUserScore3);
			SetMePlaceJetton(ID_HORSE4,pStatusPlay->lUserScore4);
			SetMePlaceJetton(ID_HORSE5,pStatusPlay->lUserScore5);
			SetMePlaceJetton(ID_HORSE6,pStatusPlay->lUserScore6);
			SetMePlaceJetton(ID_HORSE7,pStatusPlay->lUserScore7);
			SetMePlaceJetton(ID_HORSE8,pStatusPlay->lUserScore8);

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetHorseSpeed(pStatusPlay->cbHorseSpeed);
				m_GameClientView.FinishRun();

				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				//m_GameClientView.SetHorseSpeed(NULL);

				//��������
				if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			}

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_HORSE_RUN:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//���°�ť
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	m_lAreaLimitScore = pGameStart->lAreaLimitScore;
	m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);
	//���ÿ���״̬
	m_GameClientView.SetFreeTime();

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	//���¿���
	UpdateButtonContron();

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	if (IsEnableSound()) 
	{
		PlayGameSound(GAME_START);
		m_DTSDBackground.Play(0,true);
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//���ÿ���״̬
	m_GameClientView.SetFreeTime();

	//����״̬
	SetGameStatus(GS_FREE);

	//��������
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_HORSE1; nAreaIndex<=ID_HORSE8; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

	//��������
	if (IsEnableSound()) 
	{
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			if (pPlaceJetton->lJettonScore==5000000 || pPlaceJetton->lJettonScore==10000000) PlayGameSound(ADD_GOLD_EX);
			else PlayGameSound(ADD_GOLD);
			m_DTSDCheer[rand()%3].Play();
		}
	}
    
	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ȡ������
	for (WORD wAreaIndex=ID_HORSE1; wAreaIndex<=ID_HORSE8; ++wAreaIndex) m_GameClientView.SetBombEffect(false,wAreaIndex);

	//�����ע
	//m_GameClientView.CleanUserJetton();

	if (IsEnableSound())
	{
		PlayGameSound(HORSE_GO);
		
	}
	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_HORSE_RUN, pGameEnd->cbTimeLeave);

	////�˿���Ϣ
	m_GameClientView.SetHorseSpeed(pGameEnd->cbHorseSpeed);

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	//����״̬
	SetGameStatus(GS_GAME_END);
	//m_GameBank.ShowWindow(SW_HIDE);
	//���³ɼ�
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;

		//������Ϣ
		ApplyUser.lUserScore = pUserData->lScore;
		ApplyUser.strUserName = pUserData->szName;
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}

	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����
	m_DTSDBackground.Stop();
	m_DTSDCheer[0].Stop();
	m_DTSDCheer[1].Stop();
	m_DTSDCheer[2].Stop();
	//m_DTSDRaceHorse.Stop();

	return true;
}

//���¿���
void CGameClientDlg::UpdateButtonContron()
{
	//�����ж�
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) bEnablePlaceJetton=false;
	if (GetGameStatus()!=GS_PLACE_JETTON) bEnablePlaceJetton=false;
	if (m_wCurrentBanker==GetMeChairID()) bEnablePlaceJetton=false;
	if (IsLookonMode()) bEnablePlaceJetton=false;

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�������
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore-m_lMeScore1-m_lMeScore2-m_lMeScore3-m_lMeScore4-m_lMeScore5-
			m_lMeScore6-m_lMeScore7-m_lMeScore8;

		//���ù��
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=10000000L) m_GameClientView.SetCurrentJetton(10000000L);
			else if (lLeaveScore>=5000000L) m_GameClientView.SetCurrentJetton(5000000L);
			else if (lLeaveScore>=1000000L) m_GameClientView.SetCurrentJetton(1000000L);
			else if (lLeaveScore>=100000L) m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore>=10000L) m_GameClientView.SetCurrentJetton(10000L);
			else if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=100L) m_GameClientView.SetCurrentJetton(100L);
			else m_GameClientView.SetCurrentJetton(0L);
		}

		//���ư�ť
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=5000000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow((lLeaveScore>=10000000)?TRUE:FALSE);		
	}
	else
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);
	}

	//ׯ�Ұ�ť
	if (!IsLookonMode())
	{
		//��ȡ��Ϣ
		const tagUserData *pMeUserData=GetUserData(GetMeChairID());

		//���밴ť
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->lScore<m_lApplyBankerCondition) bEnableApply=false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//ȡ����ť
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,false);

		//��ʾ�ж�
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
	}

	return;
}

//��ע��Ϣ
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=(LONGLONG)lParam;

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	//״̬�ж�
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return true;
	}

	//���ñ���
	switch (cbJettonArea)
	{
		case ID_HORSE1:
			{
				m_lMeScore1 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE1,m_lMeScore1);
				break;
			}
		case ID_HORSE2:
			{
				m_lMeScore2 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE2,m_lMeScore2);
				break;
			}
		case ID_HORSE3:
			{
				m_lMeScore3 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE3,m_lMeScore3);
				break;
			}
		
		case ID_HORSE4:
			{
				m_lMeScore4 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE4,m_lMeScore4);
				break;
			}
		case ID_HORSE5:
			{
				m_lMeScore5 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE5,m_lMeScore5);
				break;
			}
		case ID_HORSE6:
			{
				m_lMeScore6 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE6,m_lMeScore6);
				break;
			}
		case ID_HORSE7:
			{
				m_lMeScore7 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE7,m_lMeScore7);
				break;
			}
		case ID_HORSE8:
			{
				m_lMeScore8 += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HORSE8,m_lMeScore8);
				break;
			}
		}

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//������Ϣ
	SendData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron();

	//��������
	if (IsEnableSound()) 
	{
		if (lJettonScore==5000000 || lJettonScore==10000000) PlayGameSound(ADD_GOLD_EX);
		else PlayGameSound(ADD_GOLD);
		m_DTSDCheer[rand()%3].Play();
	}


	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	tagUserData const *pMeUserData = GetUserData( GetMeChairID() );
	if (pMeUserData->lScore < m_lApplyBankerCondition) return true;

	//�Թ��ж�
	if (IsLookonMode()) return true;

	//ת������
	bool bApplyBanker = wParam ? true:false;

	//��ǰ�ж�
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;

	if (bApplyBanker)
	{
		//������Ϣ
		SendData(SUB_C_APPLY_BANKER, NULL, 0);

		m_bMeApplyBanker=true;
	}
	else
	{
		//������Ϣ
		SendData(SUB_C_CANCEL_BANKER, NULL, 0);

		m_bMeApplyBanker=false;
	}

	//���ð�ť
	UpdateButtonContron();

	return true;
}

//������ׯ
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//��ȡ���
	tagUserData const *pUserData=GetUserData(pApplyBanker->wApplyUser);

	//�������
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName=pUserData->szName;
		ApplyUser.lUserScore=pUserData->lScore;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//ȡ����ׯ
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pCancelyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//��ȡ���
	tagUserData const *pUserData=GetUserData(pCancelyBanker->wApplyUser);

	//ɾ�����
	tagApplyUser ApplyUser;
	ApplyUser.strUserName=pUserData->szName;
	ApplyUser.lUserScore=pUserData->lScore;
	m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pCancelyBanker->wApplyUser) m_bMeApplyBanker=false;

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�л�ׯ��
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//��ʾͼƬ
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//�Լ��ж�
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//ׯ����
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//ɾ�����
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		tagUserData const *pBankerUserData=GetUserData(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			ApplyUser.strUserName = pBankerUserData->szName;
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		}
	}

	//���½���
	UpdateButtonContron();
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��Ϸ��¼
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(enOperateResult_NULL, pServerGameRecord->cbWinner);
	}

	return true;
}

//��עʧ��
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	//Ч�����
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
	ASSERT(cbViewIndex<=ID_HORSE8);
	if (cbViewIndex>ID_HORSE8) return false;

	LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
	if (cbViewIndex==ID_HORSE1)
	{
		m_lMeScore1=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore1);
	}
	else if (cbViewIndex==ID_HORSE2) 
	{
		m_lMeScore2=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore2);
	}
	else if (cbViewIndex==ID_HORSE3) 
	{
		m_lMeScore3=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore3);
	}
	else if (cbViewIndex==ID_HORSE4) 
	{
		m_lMeScore5=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore4);
	}
	else if (cbViewIndex==ID_HORSE5) 
	{
		m_lMeScore4=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore5);
	}
	else if (cbViewIndex==ID_HORSE6) 
	{
		m_lMeScore6=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore6);
	}
	else if (cbViewIndex==ID_HORSE7) 
	{
		m_lMeScore7=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore7);
	}
	else if (cbViewIndex==ID_HORSE8) 
	{
		m_lMeScore8=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore8);
	}
	else {ASSERT(FALSE);return false;}

	return true;
}

//����ׯ��
void CGameClientDlg::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
}

//������ע
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_HORSE8);
	if (cbViewIndex>ID_HORSE8) return;

	if (cbViewIndex==ID_HORSE1) m_lMeScore1=lJettonCount;
	else if (cbViewIndex==ID_HORSE2) m_lMeScore2=lJettonCount;
	else if (cbViewIndex==ID_HORSE3) m_lMeScore3=lJettonCount;
	else if (cbViewIndex==ID_HORSE4) m_lMeScore5=lJettonCount;
	else if (cbViewIndex==ID_HORSE5) m_lMeScore4=lJettonCount;
	else if (cbViewIndex==ID_HORSE6) m_lMeScore6=lJettonCount;
	else if (cbViewIndex==ID_HORSE7) m_lMeScore7=lJettonCount;
	else if (cbViewIndex==ID_HORSE8) m_lMeScore8=lJettonCount;
	else {ASSERT(FALSE);return;}

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}


LRESULT CGameClientDlg::OnBank(WPARAM wParam, LPARAM lParam)
{
	IClientKernel *pIClientKernel = (IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	ASSERT(pIClientKernel!=NULL);
	if (pIClientKernel==NULL) return 0;
	pIClientKernel->ShowBankDialog( 0,0 );
	//������Ϣ
	SendData(SUB_C_OPEN_BANK, NULL, 0);
	return 0;
}

void CGameClientDlg::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	//lpMMI->ptMaxSize.x = 1024;
	//lpMMI->ptMaxSize.y = 740;
	//lpMMI->ptMaxTrackSize.x = 1024;
	//lpMMI->ptMaxTrackSize.y = 740;
	//lpMMI->ptMinTrackSize.x = 1024;
	//lpMMI->ptMinTrackSize.y = 740;

	//lpMMI->ptMaxPosition.x = 300;
	//lpMMI->ptMaxPosition.y = 300;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CGameClientDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect rtMax;
	//GetMaxPos(rtMax);
	if (PtInRect(&rtMax,point))
	{
		return;
	}

	__super::OnLButtonUp(nFlags, point);
}
void CGameClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect rtMax;
	//GetMaxPos(rtMax);
	if (PtInRect(&rtMax,point))
	{
		return;
	}


	__super::OnLButtonDown(nFlags, point);
}

void CGameClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect rtMax;
	//GetMaxPos(rtMax);
	if (PtInRect(&rtMax,point))
	{
		return;
	}


	__super::OnMouseMove(nFlags, point);
}
//////////////////////////////////////////////////////////////////////////
