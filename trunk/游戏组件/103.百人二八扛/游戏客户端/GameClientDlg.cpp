#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_PLACE_JETTON, OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_CONTINUE_CARD, OnContinueCard)
	ON_MESSAGE(IDM_AUTO_OPEN_CARD, OnAutoOpenCard)
	ON_MESSAGE(IDM_OPEN_CARD, OnOpenCard)
	ON_MESSAGE(IDM_MANAGE_CONTROL, OnManageControl)
	ON_MESSAGE(IDM_BANK, OnBank)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//������Ϣ
	m_lMeMaxScore = 0L;
	m_lAreaLimitScore = 0L;
	m_lApplyBankerCondition = 0L;

	//������ע
	ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore = 0L;
	m_wCurrentBanker = 0L;
	m_cbLeftCardCount = 0;

	//״̬����
	m_bMeApplyBanker = false;

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
	SetWindowText(TEXT("���˶��˿���Ϸ  --  Ver��6.6.1.0"));


	//����ͼ��
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

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
	m_lMeMaxScore = 0L;

	//������ע
	ZeroMemory(m_lUserJettonScore, sizeof(m_lUserJettonScore));

	//ׯ����Ϣ
	m_lBankerScore = 0L;
	m_wCurrentBanker = 0L;
	m_cbLeftCardCount = 0;

	//״̬����
	m_bMeApplyBanker = false;

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{

	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound = IsEnableSound();
	//��������
	if (GameOption.DoModal() == IDOK)
	{
		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);

	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	if ((nTimerID == IDI_PLACE_JETTON) && (nElapse == 0))
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
//		m_GameClientView.m_btJetton100.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
//		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);


		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
	}

	if (IsEnableSound())
	{
		if (nTimerID == IDI_PLACE_JETTON && nElapse <= 5) PlayGameSound(AfxGetInstanceHandle(), TEXT("TIME_WARIMG"));
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
			return OnSubGameFree(pBuffer, wDataSize);
		}
		case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
		case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer, wDataSize);
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
			return OnSubGameEnd(pBuffer, wDataSize);
		}
		case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{

			return OnSubGameRecord(pBuffer, wDataSize);
		}
		case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer, wDataSize);
		}
		case SUB_S_WIND_RATE_INFO:	//Ӯ����Ϣ
		{
			if (!pBuffer && sizeof(CMD_S_Win_Rate_Info) != wDataSize) return true;

			m_GameClientView.m_dlgWinCtrl.SetWinRate(pBuffer, wDataSize);
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
	ASSERT(FALSE);
	if (GetMeChairID() != INVALID_CHAIR)
	{
		const tagUserData *pUserData = GetUserData(GetMeChairID());
		if (pUserData)
		{
			if (((pUserData->dwUserRight & 0x00008000L) != 0) || ((pUserData->dwUserRight & 0x0110C000L) != 0))
			{
				m_GameClientView.m_btWinRate.ShowWindow(SW_SHOW);
			}
		}
	}

	//������ʾЧ��
	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		m_GameClientView.m_CardControl[i].m_blShowResult = false;
		m_GameClientView.m_CardControl[i].m_blShowLineResult = false;
	}
	switch (cbGameStation)
	{
		case GS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize == sizeof(CMD_S_StatusFree));
			if (wDataSize != sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree = (CMD_S_StatusFree *)pBuffer;

			//����ʱ��
			SetGameTimer(GetMeChairID(), IDI_FREE, pStatusFree->cbTimeLeave);

			//�����Ϣ
			m_lMeMaxScore = pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			const tagUserData *pMeUserData = GetUserData(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->dwUserID);

			//ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser, pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime, pStatusFree->lBankerWinScore);
			m_bEnableSysBanker = pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//������Ϣ
			m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
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
			ASSERT(wDataSize == sizeof(CMD_S_StatusPlay));
			if (wDataSize != sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay = (CMD_S_StatusPlay *)pBuffer;

			//��ע��Ϣ
			for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex, pStatusPlay->lAllJettonScore[nAreaIndex]);
				SetMePlaceJetton(nAreaIndex, pStatusPlay->lUserJettonScore[nAreaIndex]);
			}

			//��һ���
			m_lMeMaxScore = pStatusPlay->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			const tagUserData *pMeUserData = GetUserData(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->dwUserID);

			//������Ϣ
			m_lApplyBankerCondition = pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus == GS_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
				m_GameClientView.FinishDispatchCard();

				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore, pStatusPlay->lEndUserReturnScore, pStatusPlay->lEndBankerScore, pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(NULL);

				for (int i = 0; i < CARD_LOCATION_COUNT; i++)
				{
					m_GameClientView.m_CardControl[i].m_CardItemArray.SetSize(0);
				}

				//��������
				if (IsEnableSound()) m_DTSDBackground.Play(0, true);
				else m_DTSDBackground.Stop();
			}

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser, pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime, pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker = pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			SetGameTimer(GetMeChairID(), pStatusPlay->cbGameStatus == GS_GAME_END ? IDI_DISPATCH_CARD : IDI_PLACE_JETTON, pStatusPlay->cbTimeLeave);


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
	ASSERT(wDataSize == sizeof(CMD_S_GameStart));
	if (wDataSize != sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;

	m_GameClientView.KillCardTime();

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser, pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore = pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//����ʱ��
	SetGameTimer(GetMeChairID(), IDI_PLACE_JETTON, pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	//���¿���
	UpdateButtonContron();

	//������ʾ
	m_GameClientView.SetDispatchCardTip(pGameStart->bContiueCard ? enDispatchCardTip_Continue : enDispatchCardTip_Dispatch);

	//��������
	if (IsEnableSound())
	{
		PlayGameSound(AfxGetInstanceHandle(), TEXT("GAME_START"));
		m_DTSDBackground.Play(0, true);
	}
	else
	{
		m_DTSDBackground.Stop();
	}

	if (m_GameBank.m_hWnd)
		m_GameBank.ShowWindow(SW_HIDE);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_GameFree));
	if (wDataSize != sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree = (CMD_S_GameFree *)pBuffer;

	//����ʱ��
	SetGameTimer(GetMeChairID(), IDI_FREE, pGameFree->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_FREE);

	//��������
	m_GameClientView.SetWinnerSide(false, false, false, false);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex = ID_TIAN_MEN; nAreaIndex <= ID_XUAN_MEN; ++nAreaIndex)
		SetMePlaceJetton(nAreaIndex, 0);

	////�����˿�
	for (int i = 0; i < CountArray(m_GameClientView.m_CardControl); ++i)
	{
		m_GameClientView.m_CardControl[i].SetCardData(0);
		m_GameClientView.m_CardControl[i].SetCardData(NULL, 0);
	}



	//��ɷ���
	m_GameClientView.FinishDispatchCard();
	m_GameClientView.KillCardTime();


	//���³ɼ�
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if (pUserData == NULL) continue;
		tagApplyUser ApplyUser ;

		//������Ϣ
		ApplyUser.lUserScore = pUserData->lScore;
		ApplyUser.strUserName = pUserData->szName;
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}
	if (m_GameClientView.m_bShowRight && m_GameClientView.m_ApplyUser.GetItemCount() > 7)
	{
		m_GameClientView.m_btUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_btUp.EnableWindow(true);
		m_GameClientView.m_btDown.ShowWindow(SW_SHOW);
		m_GameClientView.m_btDown.EnableWindow(true);
	}
	else
	{
		m_GameClientView.m_btUp.ShowWindow(SW_HIDE);
		m_GameClientView.m_btUp.EnableWindow(true);
		m_GameClientView.m_btDown.ShowWindow(SW_HIDE);
		m_GameClientView.m_btDown.EnableWindow(true);

	}
	//���¿ؼ�
	UpdateButtonContron();

	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(true);
	m_GameClientView.UpdateGameView(NULL);



	return true;
}

//�û���ע
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_PlaceJetton));
	if (wDataSize != sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton = (CMD_S_PlaceJetton *)pBuffer;

	if (GetMeChairID() != pPlaceJetton->wChairID || IsLookonMode())
	{
		//��ע����
		m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea, pPlaceJetton->lJettonScore);

		//��������
		if (IsEnableSound())
		{
			if (pPlaceJetton->wChairID != GetMeChairID() || IsLookonMode())
			{
				if (pPlaceJetton->lJettonScore == 5000000) PlayGameSound(AfxGetInstanceHandle(), TEXT("ADD_GOLD_EX"));
				else PlayGameSound(AfxGetInstanceHandle(), TEXT("ADD_GOLD"));
				m_DTSDCheer[rand()%3].Play();
			}
		}
	}

	UpdateButtonContron();

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_GameEnd));
	if (wDataSize != sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;

	m_GameClientView.m_blMoveFinish = false;

	//����ʱ��
	SetGameTimer(GetMeChairID(), IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);

	m_GameClientView.SetFirstShowCard(pGameEnd->bcFirstCard);

	////�����˿�
	for (int i = 0; i < CountArray(m_GameClientView.m_CardControl); ++i)
		m_GameClientView.m_CardControl[i].SetCardData(m_GameClientView.m_cbTableCardArray[i], CARD_COUNT_OF_ONE);

	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		int iType = m_GameClientView.m_GameLogic.GetCardType(m_GameClientView.m_cbTableCardArray[i], CARD_COUNT_OF_ONE);

		m_GameClientView.m_lUserCardType[i] = iType;
	}

// 	//�O�Ó���Ƅ�
// 	m_GameClientView.SetMoveCardTimer();

	//����״̬
	SetGameStatus(GS_GAME_END);

	m_cbLeftCardCount = pGameEnd->cbLeftCardCount;

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);
	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore, pGameEnd->lUserReturnScore, pGameEnd->lBankerScore, pGameEnd->lRevenue);


	{
		m_TempData.a = pGameEnd->lUserScore;
		m_TempData.b = pGameEnd->lUserReturnScore;
		m_TempData.c = pGameEnd->lBankerScore;
		m_TempData.d = pGameEnd->lRevenue;

	}

	for (int i = 0; i < CARD_LOCATION_COUNT; i++)
	{
		m_GameClientView.m_CardControl[i].m_blGameEnd = false;
	}



	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����
	for (int i = 0; i < CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientDlg::UpdateButtonContron()
{
	//�����ж�
	bool bEnablePlaceJetton = true;

	if (m_wCurrentBanker == INVALID_CHAIR)
	{
		bEnablePlaceJetton = true;

	}
	if (GetGameStatus() != GS_PLACE_JETTON)
	{
		bEnablePlaceJetton = false;

	}
	if (m_wCurrentBanker == GetMeChairID())
	{
		bEnablePlaceJetton = false;
	}
	if (IsLookonMode())
	{
		bEnablePlaceJetton = false;
	}
	if (m_bEnableSysBanker == false && m_wCurrentBanker == INVALID_CHAIR)
	{
		bEnablePlaceJetton = false;
	}

	if (GetGameStatus() == GS_GAME_END)
	{
		m_GameClientView.m_btOpenCard.EnableWindow(false);
		m_GameClientView.m_btAutoOpenCard.EnableWindow(false);

	}
	else
	{
		m_GameClientView.m_btOpenCard.EnableWindow(true);
		m_GameClientView.m_btAutoOpenCard.EnableWindow(true);

	}

	//��ע��ť
	if (bEnablePlaceJetton == true)
	{
		//�������
		LONGLONG lCurrentJetton = m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore = m_lMeMaxScore;
		for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; ++nAreaIndex)
			lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//�����ע
		LONG lUserMaxJetton = m_GameClientView.GetUserMaxJetton();

		lLeaveScore = min((lLeaveScore), lUserMaxJetton); //�û����·� �����ֱȽ� ������10��

		//���ù��
		if (lCurrentJetton > lLeaveScore)
		{
// 			if (lLeaveScore >= 5000000L) m_GameClientView.SetCurrentJetton(5000000L);
// 			else
			if (lLeaveScore >= 50000000L) m_GameClientView.SetCurrentJetton(50000000L);
			else if (lLeaveScore >= 10000000L) m_GameClientView.SetCurrentJetton(10000000L);
			else if (lLeaveScore >= 5000000L) m_GameClientView.SetCurrentJetton(5000000L);
			else if (lLeaveScore >= 1000000L) m_GameClientView.SetCurrentJetton(1000000L);
			else if (lLeaveScore >= 100000L) m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore >= 10000L) m_GameClientView.SetCurrentJetton(10000L);
//			else if (lLeaveScore >= 100L) m_GameClientView.SetCurrentJetton(100L);
			else m_GameClientView.SetCurrentJetton(0L);
		}

		//���ư�ť
		int iTimer = 1;
//		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore >= 100*iTimer && lUserMaxJetton >= 100*iTimer) ? TRUE : FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore >= 10000*iTimer && lUserMaxJetton >= 10000*iTimer) ? TRUE : FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore >= 100000*iTimer && lUserMaxJetton >= 100000*iTimer) ? TRUE : FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow((lLeaveScore >= 1000000*iTimer && lUserMaxJetton >= 1000000*iTimer) ? TRUE : FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore >= 5000000*iTimer && lUserMaxJetton >= 5000000*iTimer) ? TRUE : FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow((lLeaveScore >= 10000000*iTimer && lUserMaxJetton >= 10000000*iTimer) ? TRUE : FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore >= 50000000*iTimer && lUserMaxJetton >= 50000000*iTimer) ? TRUE : FALSE);
//		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore >= 5000000*iTimer && lUserMaxJetton >= 5000000*iTimer) ? TRUE : FALSE);
	}
	else
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
//		m_GameClientView.m_btJetton100.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);

		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
//		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
	}

	//ׯ�Ұ�ť
	if (!IsLookonMode())
	{
		//��ȡ��Ϣ
		const tagUserData *pMeUserData = GetUserData(GetMeChairID());

		//���밴ť
		bool bEnableApply = true;
		if (m_wCurrentBanker == GetMeChairID()) bEnableApply = false;
		if (m_bMeApplyBanker) bEnableApply = false;
		if (pMeUserData->lScore < m_lApplyBankerCondition) bEnableApply = false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply ? TRUE : FALSE);

		//ȡ����ť
		bool bEnableCancel = true;
		if (m_wCurrentBanker == GetMeChairID() && GetGameStatus() != GS_FREE) bEnableCancel = false;
		if (m_bMeApplyBanker == false) bEnableCancel = false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel ? TRUE : FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker == GetMeChairID() ? IDB_BT_CANCEL_BANKER : IDB_BT_CANCEL_APPLY, AfxGetInstanceHandle(), false);

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

			if (pMeUserData->lScore < m_lApplyBankerCondition)
			{
				m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);

			}
			else
			{
				m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);


			}

		}

		//���ư�ť
		if (GetMeChairID() == m_wCurrentBanker)
		{
			BOOL bEnableButton = TRUE;
			if (GetGameStatus() != GS_FREE) bEnableButton = FALSE;
			if (m_cbLeftCardCount < 8) bEnableButton = FALSE;

			//m_GameClientView.m_btContinueCard.ShowWindow(SW_SHOW);
			//m_GameClientView.m_btContinueCard.EnableWindow(bEnableButton);
		}
		else
		{
			//m_GameClientView.m_btContinueCard.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		//��ȡ��Ϣ
		const tagUserData *pMeUserData = GetUserData(GetMeChairID());

		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);

		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);

		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
		//m_GameClientView.m_btContinueCard.ShowWindow(SW_HIDE);


	}
	//��ȡ��Ϣ
	const tagUserData *pMeUserData = GetUserData(GetMeChairID());


	//���а�ť
	m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
	//����ж�
	BOOL bEnableStorage = TRUE;
	if (GetGameStatus() != GS_FREE) bEnableStorage = FALSE;
	if (IsLookonMode()) bEnableStorage = FALSE;
	if (m_GameClientView.m_ApplyUser.FindUser(pMeUserData->szName)) bEnableStorage = FALSE;
	if (m_wCurrentBanker == pMeUserData->wChairID) bEnableStorage = FALSE;
	m_GameClientView.m_btBankerStorage.EnableWindow(bEnableStorage);
	m_GameClientView.m_btBankerDraw.EnableWindow(bEnableStorage);

	if (m_wCurrentBanker == pMeUserData->wChairID) m_GameClientView.m_blCanStore = false;
	else  m_GameClientView.m_blCanStore = true;

// 	if (m_GameClientView.m_DlgBank.m_hWnd != NULL)
// 	{
// 		m_GameClientView.m_DlgBank.m_blCanStore = m_GameClientView.m_blCanStore;
// 		m_GameClientView.m_DlgBank.ShowItem();
// 	}

// 	m_GameClientView.m_btBankerStorage.ShowWindow(SW_HIDE);
// 	m_GameClientView.m_btBankerDraw.ShowWindow(SW_HIDE);




	return;
}

//��ע��Ϣ
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea = (BYTE)wParam;
	LONG lJettonScore = (LONG)lParam;

	//�Ϸ��ж�
	ASSERT(cbJettonArea >= ID_TIAN_MEN && cbJettonArea <= ID_XUAN_MEN);
	if (!(cbJettonArea >= ID_TIAN_MEN && cbJettonArea <= ID_XUAN_MEN)) return 0;

	//ׯ���ж�
	if (GetMeChairID() == m_wCurrentBanker) return true;

	//״̬�ж�
	if (GetGameStatus() != GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return true;
	}

	//���ñ���
	m_lUserJettonScore[cbJettonArea] += lJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lUserJettonScore[cbJettonArea]);

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea = cbJettonArea;
	PlaceJetton.lJettonScore = lJettonScore;

	//������Ϣ
	SendData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron();

	//Ԥ����ʾ
	m_GameClientView.PlaceUserJetton(cbJettonArea, lJettonScore);

	//��������
	if (IsEnableSound())
	{
		if (lJettonScore == 5000000) PlayGameSound(AfxGetInstanceHandle(), TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(), TEXT("ADD_GOLD"));
		m_DTSDCheer[rand()%3].Play();
	}

	return 0;
}
//�ֹ�����
LRESULT  CGameClientDlg::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus() == GS_GAME_END)
	{
		InsertGeneralString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"), RGB(255, 0, 255), true);
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = false;
	InsertGeneralString(TEXT("��ϵͳ��ʾ����ѡ�����ֹ�����ģʽ�����ƺ�������ʹ������϶��˿ˣ�"), RGB(255, 0, 255), true);
	//m_GameClientView.m_btOpenCard.EnableWindow(false);
	//m_GameClientView.m_btAutoOpenCard.EnableWindow(true);
	return 1;
}
//�Զ�����
LRESULT  CGameClientDlg::OnAutoOpenCard(WPARAM wParam, LPARAM lParam)
{
	if (GetGameStatus() == GS_GAME_END)
	{
		InsertGeneralString(TEXT("��ϵͳ��ʾ�ݿ���ʱ�䲻���л�����ģʽ��"), RGB(255, 0, 255), true);
		return 1;

	}
	m_GameClientView.m_blAutoOpenCard = true;
	InsertGeneralString(TEXT("��ϵͳ��ʾ����ѡ�����Զ�����ģʽ�����ƺ�ϵͳ���Զ��������ҵ��ƣ�"), RGB(255, 0, 255), true);
	//m_GameClientView.m_btOpenCard.EnableWindow(true);
	//m_GameClientView.m_btAutoOpenCard.EnableWindow(false);
	return 1;
}
//��������
LRESULT CGameClientDlg::OnContinueCard(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	if (GetMeChairID() != m_wCurrentBanker) return 0;
	if (GetGameStatus() != GS_FREE) return 0;
	if (m_cbLeftCardCount < 8) return 0;
	if (IsLookonMode()) return 0;

	//������Ϣ
	SendData(SUB_C_CONTINUE_CARD);

	//���ð�ť
	//m_GameClientView.m_btContinueCard.EnableWindow(FALSE);

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	tagUserData const *pMeUserData = GetUserData(GetMeChairID());
	if (pMeUserData->lScore < m_lApplyBankerCondition) return true;

	//�Թ��ж�
	if (IsLookonMode()) return true;

	//ת������
	bool bApplyBanker = wParam ? true : false;

	//��ǰ�ж�
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;

	if (bApplyBanker)
	{
		//������Ϣ
		SendData(SUB_C_APPLY_BANKER, NULL, 0);

		//m_bMeApplyBanker=true;
	}
	else
	{
		//������Ϣ
		SendData(SUB_C_CANCEL_BANKER, NULL, 0);

		//m_bMeApplyBanker=false;
	}

	//���ð�ť
	UpdateButtonContron();

	return true;
}

//������ׯ
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_ApplyBanker));
	if (wDataSize != sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker = (CMD_S_ApplyBanker *)pBuffer;

	//��ȡ���
	tagUserData const *pUserData = GetUserData(pApplyBanker->wApplyUser);

	//�������
	if (m_wCurrentBanker != pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName = pUserData->szName;
		ApplyUser.lUserScore = pUserData->lScore;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);

		if (m_GameClientView.m_bShowRight && m_GameClientView.m_ApplyUser.GetItemCount() > 7)
		{
			m_GameClientView.m_btUp.ShowWindow(SW_SHOW);
			m_GameClientView.m_btUp.EnableWindow(true);
			m_GameClientView.m_btDown.ShowWindow(SW_SHOW);
			m_GameClientView.m_btDown.EnableWindow(true);
		}
		else
		{
			m_GameClientView.m_btUp.ShowWindow(SW_HIDE);
			m_GameClientView.m_btUp.EnableWindow(true);
			m_GameClientView.m_btDown.ShowWindow(SW_HIDE);
			m_GameClientView.m_btDown.EnableWindow(true);

		}
	}

	//�Լ��ж�
	if (IsLookonMode() == false && GetMeChairID() == pApplyBanker->wApplyUser) m_bMeApplyBanker = true;

	//���¿ؼ�
	UpdateButtonContron();
	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(true);
	m_GameClientView.UpdateGameView(NULL);



	return true;
}

//ȡ����ׯ
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_CancelBanker));
	if (wDataSize != sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker = (CMD_S_CancelBanker *)pBuffer;

	//ɾ�����
	tagApplyUser ApplyUser;
	ApplyUser.strUserName = pCancelBanker->szCancelUser;
	ApplyUser.lUserScore = 0;
	m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(true);

	if (m_GameClientView.m_bShowRight && m_GameClientView.m_ApplyUser.GetItemCount() > 7)
	{
		m_GameClientView.m_btUp.ShowWindow(SW_SHOW);
		m_GameClientView.m_btUp.EnableWindow(true);
		m_GameClientView.m_btDown.ShowWindow(SW_SHOW);
		m_GameClientView.m_btDown.EnableWindow(true);
	}
	else
	{
		m_GameClientView.m_btUp.ShowWindow(SW_HIDE);
		m_GameClientView.m_btUp.EnableWindow(true);
		m_GameClientView.m_btDown.ShowWindow(SW_HIDE);
		m_GameClientView.m_btDown.EnableWindow(true);

	}

	//�Լ��ж�
	const tagUserData *pMeUserData = GetUserData(GetMeChairID());
	if (IsLookonMode() == false && lstrcmp(pMeUserData->szName, pCancelBanker->szCancelUser) == 0) m_bMeApplyBanker = false;

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�л�ׯ��
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_ChangeBanker));
	if (wDataSize != sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	//��ʾͼƬ
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker != pChangeBanker->wBankerUser);

	//�Լ��ж�
	if (m_wCurrentBanker == GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser != GetMeChairID())
	{
		m_bMeApplyBanker = false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser == GetMeChairID())
	{
		m_bMeApplyBanker = true;
	}

	//ׯ����Ϣ
	SetBankerInfo(pChangeBanker->wBankerUser, pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerScore(0, 0);

	//ɾ�����
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		tagUserData const *pBankerUserData = GetUserData(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			ApplyUser.strUserName = pBankerUserData->szName;
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		}
	}

	//���½���
	UpdateButtonContron();
	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(true);
	m_GameClientView.UpdateGameView(NULL);


	return true;
}

//��Ϸ��¼
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize % sizeof(tagServerGameRecord) == 0);
	if (wDataSize % sizeof(tagServerGameRecord) != 0) return false;

	//�������
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord, sizeof(GameRecord));

	//���ü�¼
	WORD wRecordCount = wDataSize / sizeof(tagServerGameRecord);
	for (WORD wIndex = 0; wIndex < wRecordCount; wIndex++)
	{
		tagServerGameRecord * pServerGameRecord = (((tagServerGameRecord *)pBuffer) + wIndex);

		m_GameClientView.SetGameHistory(pServerGameRecord->bWinShunMen, pServerGameRecord->bWinDaoMen, pServerGameRecord->bWinDuiMen);
	}

	return true;
}

//��עʧ��
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize == sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize != sizeof(CMD_S_PlaceJettonFail)) return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail = (CMD_S_PlaceJettonFail *)pBuffer;

	//Ч�����
	BYTE cbViewIndex = pPlaceJettonFail->lJettonArea;
	ASSERT(cbViewIndex <= ID_XUAN_MEN && cbViewIndex >= ID_TIAN_MEN);
	if (!(cbViewIndex <= ID_XUAN_MEN && cbViewIndex >= ID_TIAN_MEN)) return false;

	//�Լ��ж�
	if (GetMeChairID() == pPlaceJettonFail->wPlaceUser && false == IsLookonMode())
	{
		//��ע����
		m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea, -pPlaceJettonFail->lPlaceScore);

		LONG lJettonCount = pPlaceJettonFail->lPlaceScore;
		//�Ϸ�У��
		ASSERT(m_lUserJettonScore[cbViewIndex] >= lJettonCount);
		if (lJettonCount > m_lUserJettonScore[cbViewIndex]) return false;

		//������ע
		m_lUserJettonScore[cbViewIndex] -= lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex, m_lUserJettonScore[cbViewIndex]);
	}

	return true;
}

//����ׯ��
void CGameClientDlg::SetBankerInfo(WORD wBanker, LONGLONG lScore)
{
	m_wCurrentBanker = wBanker;
	m_lBankerScore = lScore;
	tagUserData const *pUserData = m_wCurrentBanker == INVALID_CHAIR ? NULL : GetUserData(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL == pUserData) ? 0 : pUserData->dwUserID;
	m_GameClientView.SetBankerInfo(dwBankerUserID, m_lBankerScore);
}

//������ע
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//�Ϸ��ж�
	ASSERT(cbViewIndex >= ID_TIAN_MEN && cbViewIndex <= ID_XUAN_MEN);
	if (!(cbViewIndex >= ID_TIAN_MEN && cbViewIndex <= ID_XUAN_MEN)) return;

	//���ñ���
	m_lUserJettonScore[cbViewIndex] = lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex, lJettonCount);
}

LRESULT CGameClientDlg::OnManageControl(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

//////////////////////////////////////////////////////////////////////////

bool CGameClientDlg::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	if (SUB_GF_MESSAGE == wSubCmdID)
	{
		CMD_GF_Message * pMessage = (CMD_GF_Message *)pBuffer;
		ASSERT(wDataSize > (sizeof(CMD_GF_Message) - sizeof(pMessage->szContent)));
		if (wDataSize <= (sizeof(CMD_GF_Message) - sizeof(pMessage->szContent))) return false;

		//��Ϣ����
		WORD wHeadSize = sizeof(CMD_GF_Message) - sizeof(pMessage->szContent);
		ASSERT(wDataSize == (wHeadSize + pMessage->wMessageLength*sizeof(TCHAR)));
		if (wDataSize != (wHeadSize + pMessage->wMessageLength*sizeof(TCHAR))) return false;
		pMessage->szContent[pMessage->wMessageLength-1] = 0;



		CString str = pMessage->szContent;
		int iRet = str.Find("��ȡ�ɹ�", 0);
		if (iRet != -1)
		{
			m_GameClientView.ClearBank();

		}
		else
		{
			iRet = str.Find("�洢�ɹ�", 0);
			if (iRet != -1)
			{
				m_GameClientView.ClearBank();

			}

		}

	}

	return false;

}

LRESULT CGameClientDlg::OnBank(WPARAM wParam, LPARAM lParam)
{
	//������¼��
	if (m_GameBank.m_hWnd == NULL)
	{
		m_GameBank.Create(IDD_BACK, this);
	}

	if ((BYTE)wParam == 0)
	{
		m_GameBank.SetButtonSelected(IDC_STORAGE, true);
		m_GameBank.SetButtonSelected(IDC_DRAWOUT, false);
		// 		IClientKernel *pIClientKernel = ( IClientKernel * )GetClientKernel( IID_IClientKernel, VER_IClientKernel );
		// 		pIClientKernel->SendSocketData(MDM_GF_GAME,SUB_GF_BANK_STORAGE);

		//SendData(SUB_GF_BANK_STORAGE, NULL, NULL);
	}
	if ((BYTE)wParam == 1)
	{
		m_GameBank.SetButtonSelected(IDC_DRAWOUT, true);
		m_GameBank.SetButtonSelected(IDC_STORAGE, false);
		//SendData(SUB_GF_BANK_GET, NULL, NULL);
	}
	CRect scoreRect, gameViewRect ;
	m_GameClientView.GetWindowRect(gameViewRect) ;
	m_GameBank.GetWindowRect(scoreRect) ;
	m_GameBank.MoveWindow(gameViewRect.left + (gameViewRect.Width() - scoreRect.Width()) / 2,
	                      gameViewRect.top + (gameViewRect.Height() - scoreRect.Height()) / 2, scoreRect.Width(), scoreRect.Height()) ;
	IClientKernel *pIClientKernel = (IClientKernel *)GetClientKernel(IID_IClientKernel, VER_IClientKernel);
	tagUserData const *pMeUserData = GetUserData(GetMeChairID());
	m_GameBank.SetSendInfo(pIClientKernel, pMeUserData);
	m_GameBank.ShowWindow(SW_SHOW);
	m_GameBank.SetActiveWindow();
	m_GameBank.SetForegroundWindow();

	return 0;
}