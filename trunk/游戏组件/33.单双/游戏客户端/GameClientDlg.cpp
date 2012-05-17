#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					100									//����ʱ��
#define IDI_GAME					101									//��Ϸʱ��
#define IDI_END_PLACE_JETTON		102									//��Ϸʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_SHOW_GAME_RESULT		302									//��ʾ���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_UPDATE_CONTROL, OnUpdateControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��ע��Ϣ
	m_lMeMaxScore=0L;
	m_lMeOddScore=0L;
	m_lMeEvenScore=0L;

	m_lAreaLimitScore=0L;	

	//��ʷ��Ϣ
	m_wDrawCount=1;
	m_lMeResultCount=0;

	//״̬����
	m_bPlaying = false;

	//�˿���Ϣ
	ZeroMemory(m_cbDice,sizeof(m_cbDice));

	//ׯ����Ϣ
	m_lApplyBankerCondition = 100000000;
	m_wCurrentBanker = INVALID_CHAIR;
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
	SetWindowText(TEXT("��˫��Ϸ  --  Ver��6.0.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��ע��Ϣ
	m_lMeMaxScore=0L;
	m_lMeOddScore=0L;
	m_lMeEvenScore=0L;
	
	m_lAreaLimitScore=0L;	
    
	//��ʷ��Ϣ
	m_wDrawCount=1;
	m_lMeResultCount=0;

	//״̬����
	m_bPlaying = false;
	m_bMeApplyBanker = false;

	//ׯ����Ϣ
	m_lApplyBankerCondition = 100000000;
	m_wCurrentBanker = INVALID_CHAIR;

	//�˿���Ϣ
	ZeroMemory(m_cbDice,sizeof(m_cbDice));

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
	if ((nTimerID==IDI_GAME)&&(nElapse==0))
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500.EnableWindow(FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);

		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );

		//��������
		if ( IsEnableSound() ) PlayGameSound(AfxGetInstanceHandle(),TEXT("STOP_JETTON"));
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
			return OnUserApplyBanker( pBuffer, wDataSize );
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnChangeBanker( pBuffer, wDataSize );
		}
	case SUB_S_CHANGE_USER_SCORE://���»���
		{
			return OnChangeUserScore( pBuffer, wDataSize );
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_GAME_SCORE:		//��Ϸ����
		{
			return OnSubGameScore(pBuffer, wDataSize);
		}
	case SUB_S_END_PLACE_JETTON://��ע����
		{
			return OnSubEndPlaceJetton(pBuffer, wDataSize);
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

			//ׯ�ұ���
			m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;			

			//����λ��
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));
			m_GameClientView.SetHistoryScore(m_wDrawCount,m_lMeResultCount);

			//�����ע
			m_GameClientView.SetMeMaxScore(pStatusFree->lMeMaxScore);
			m_GameClientView.SetAreaLimitScore( pStatusFree->lAreaLimitScore );

			m_GameClientView.SetMeOddScore(pStatusFree->lMeOddScore);
			m_GameClientView.SetMeEvenScore(pStatusFree->lMeEvenScore);

			m_wCurrentBanker = pStatusFree->wCurrentBankerChairID;

			//���ñ���
			m_lMeMaxScore= pStatusFree->lMeMaxScore ;
			m_lMeOddScore=pStatusFree->lMeOddScore;
			m_lMeEvenScore=pStatusFree->lMeEvenScore;

			//ׯ����Ϣ
			if ( pStatusFree->wCurrentBankerChairID == INVALID_CHAIR )
				m_GameClientView.SetBankerInfo( INVALID_CHAIR, pStatusFree->cbBankerTime, pStatusFree->lCurrentBankerScore );
			else
				m_GameClientView.SetBankerInfo( SwitchViewChairID( pStatusFree->wCurrentBankerChairID ), pStatusFree->cbBankerTime, pStatusFree->lCurrentBankerScore );
			m_GameClientView.SetBankerTreasure(pStatusFree->lBankerTreasure);

			//��ע����
			m_GameClientView.PlaceUserJetton(ID_ODD,pStatusFree->lOddcore);
			m_GameClientView.PlaceUserJetton(ID_EVEN,pStatusFree->lEvenScore);
			m_GameClientView.ShowCloseDicebox(true);

			//���¿���
			UpdateButtonContron();

			//��������
			if ( IsEnableSound() ) PlayGameSound(AfxGetInstanceHandle(),TEXT("PLACE_JETTON"));

			//����ʱ��
			SetGameTimer(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

			return true;
		}
	case GS_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//ׯ�ұ���
			m_lApplyBankerCondition = pStatusPlay->lApplyBankerCondition;

			//���ñ���
			m_lMeMaxScore=pStatusPlay->lMeMaxScore ;
			m_lMeOddScore=pStatusPlay->lMeOddScore;
			m_lMeEvenScore=pStatusPlay->lMeEvenScore;

			//����λ��
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));
			m_GameClientView.SetHistoryScore(m_wDrawCount,m_lMeResultCount);

			//�����ע
			m_GameClientView.SetMeMaxScore(pStatusPlay->lMeMaxScore);	
			m_GameClientView.SetAreaLimitScore( pStatusPlay->lAreaLimitScore );

			m_GameClientView.SetMeOddScore(pStatusPlay->lMeOddScore);
			m_GameClientView.SetMeEvenScore(pStatusPlay->lMeEvenScore);

			//ׯ����Ϣ
			m_wCurrentBanker = pStatusPlay->wCurrentBankerChairID;
			if ( pStatusPlay->wCurrentBankerChairID == INVALID_CHAIR )
				m_GameClientView.SetBankerInfo( INVALID_CHAIR, pStatusPlay->cbBankerTime, pStatusPlay->lCurrentBankerScore );
			else
				m_GameClientView.SetBankerInfo( SwitchViewChairID( pStatusPlay->wCurrentBankerChairID ), pStatusPlay->cbBankerTime, pStatusPlay->lCurrentBankerScore );
			m_GameClientView.SetBankerTreasure(pStatusPlay->lBankerTreasure);


			//��ע����
			m_GameClientView.PlaceUserJetton(ID_ODD,pStatusPlay->lOddcore);
			m_GameClientView.PlaceUserJetton(ID_EVEN,pStatusPlay->lEvenScore);

			//��������
			if ( IsEnableSound() ) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

			//����ɫ��
			m_GameClientView.SetDice(pStatusPlay->cbDice);

			//���ð�ť
			m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
			m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );

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

	//����״̬
	SetGameStatus(GS_WK_GAME);
	SetGameTimer(GetMeChairID(), IDI_GAME, pGameStart->cbGameTime);
	m_GameClientView.SetTimeIndex(1);

	//����״̬
	m_GameClientView.SetCartoon(INDEX_SHAKE_DICEBOX, true);

	//���¿���
	UpdateButtonContron();

	////��������
	//if ( IsEnableSound() ) PlayGameSound(AfxGetInstanceHandle(),TEXT("SHAKE_DICE"));

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

	//��������
	if ( IsEnableSound() ) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

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

	//����״̬
	SetGameStatus(GS_FREE);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_SHOW_GAME_RESULT);

	//���ñ���
	m_lMeOddScore=0L;
	m_lMeEvenScore=0L;

	//���³ɼ�
	for ( WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex )
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;

		//������Ϣ
		ApplyUser.lUserScore = pUserData->lScore;
		ApplyUser.strUserName = pUserData->szName;
		m_GameClientView.m_ApplyUser.UpdateUser( ApplyUser );
	}

	enOperateResult OperateResult = enOperateResult_NULL;
	if ( 0 < m_GameClientView.m_lMeCurGameScore ) OperateResult = enOperateResult_Win;
	else if ( m_GameClientView.m_lMeCurGameScore < 0 ) OperateResult = enOperateResult_Lost;
	else OperateResult = enOperateResult_NULL;

	m_GameClientView.SetGameHistory(OperateResult, m_cbDice[0]+m_cbDice[1]);

	//ׯ�ҽ��
	m_GameClientView.SetBankerTreasure(pGameEnd->lBankerTreasure);

	//״̬����
	m_bPlaying = false;

	//�����ע
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
	m_GameClientView.SetMeOddScore(m_lMeOddScore);
	m_GameClientView.SetMeEvenScore(m_lMeEvenScore);

	//���ý���
	m_GameClientView.CleanUserJetton();
	m_GameClientView.SetWinnerSide(0xFF);

	KillTimer(IDI_SHOW_GAME_RESULT);
	m_GameClientView.SetShowGameFlag(false);

	m_GameClientView.SetHistoryScore(m_wDrawCount,m_lMeResultCount);

	//ׯ����Ϣ
	if ( m_wCurrentBanker != INVALID_CHAIR )
		m_GameClientView.SetBankerInfo(SwitchViewChairID(m_wCurrentBanker), pGameEnd->nBankerTime, pGameEnd->lBankerTotalScore);


	//���¿���
	UpdateButtonContron();

	//��������
	if ( IsEnableSound() ) PlayGameSound(AfxGetInstanceHandle(),TEXT("PLACE_JETTON"));

	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_FREE,pGameEnd->cbTimeLeave);
	m_GameClientView.SetTimeIndex(0);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameScore));
	if (wDataSize!=sizeof(CMD_S_GameScore)) return false;

	//��Ϣ����
	CMD_S_GameScore * pGameScore=(CMD_S_GameScore *)pBuffer;

	//��ʷ�ɼ�
	m_wDrawCount++;
	m_lMeResultCount+=pGameScore->lMeGameScore;

	//���óɼ�
	m_GameClientView.SetGameScore(pGameScore->lMeGameScore, pGameScore->lMeReturnScore, pGameScore->lBankerScore);

	m_GameClientView.SetMeOddScore(pGameScore->lMeOddScore);
	m_GameClientView.SetMeEvenScore(pGameScore->lMeEvenScore);

	//���ñ���
	m_lMeOddScore=pGameScore->lMeOddScore;
	m_lMeEvenScore=pGameScore->lMeEvenScore;

	return true;
}

//��ע����
bool CGameClientDlg::OnSubEndPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(CMD_S_EndPlaceJetton));
	if ( wDataSize != sizeof(CMD_S_EndPlaceJetton)) return false;

	CMD_S_EndPlaceJetton *pEndPlaceJetton = (CMD_S_EndPlaceJetton *)pBuffer;

	//����ɫ��
	m_GameClientView.SetDice(pEndPlaceJetton->cbDice);
	CopyMemory(m_cbDice, pEndPlaceJetton->cbDice, sizeof(m_cbDice));

	//���ö���
	m_GameClientView.ShowCloseDicebox(false);
	m_GameClientView.SetCartoon(INDEX_OPEN_DICEBOX, true);

	//����ʱ��
	SetGameTimer(GetMeChairID(), IDI_END_PLACE_JETTON, pEndPlaceJetton->cbGameTime);
	m_GameClientView.SetTimeIndex(2);

	return true;
}

//���¿���
void CGameClientDlg::UpdateButtonContron()
{
	if ((IsLookonMode()==false)&&(GetGameStatus()==GS_WK_GAME) && m_wCurrentBanker != GetMeChairID() && m_wCurrentBanker != INVALID_CHAIR &&
		m_GameClientView.IsPlaceJettonStatus()==true)
	{
		//�������
		LONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONG lLeaveScore=m_lMeMaxScore-m_lMeOddScore-m_lMeEvenScore;

		//���ù��
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=500000L) m_GameClientView.SetCurrentJetton(500000L);
			else if (lLeaveScore>=100000L) m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore>=50000L) m_GameClientView.SetCurrentJetton(50000L);
			else if (lLeaveScore>=10000L) m_GameClientView.SetCurrentJetton(10000L);
			else if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=500L) m_GameClientView.SetCurrentJetton(500L);
			else if (lLeaveScore>=100L) m_GameClientView.SetCurrentJetton(100L);
			else m_GameClientView.SetCurrentJetton(0L);
		}

		//���ư�ť
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton500.EnableWindow((lLeaveScore>=500)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000)?TRUE:FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow((lLeaveScore>=50000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000)?TRUE:FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow((lLeaveScore>=500000)?TRUE:FALSE);
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
		m_GameClientView.m_btJetton500.EnableWindow(FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
	}

	//���밴ť
	if ( ! IsLookonMode() )
	{
		//״̬�ж�
		if ( GetGameStatus()==GS_WK_FREE ) 
		{
			m_GameClientView.m_btCancelBanker.EnableWindow(TRUE);
			m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);
		}
		else
		{
			m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
			m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		}

		//��ʾ�ж�
		const tagUserData *pMeUserData = GetUserData( GetMeChairID() );
		if ( m_bMeApplyBanker )
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);

			bool bMeBanker = pMeUserData->wChairID == m_wCurrentBanker ? true : false;
			if ( bMeBanker ) m_GameClientView.m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_BANKER,AfxGetInstanceHandle(),false);
			else m_GameClientView.m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

		}
		else if ( m_lApplyBankerCondition <= pMeUserData->lScore )
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		}
	}

	return;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

//��ע��Ϣ
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONG lJettonScore=(LONG)lParam;

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return true;
	if ( m_wCurrentBanker == INVALID_CHAIR ) 
	{
		UpdateButtonContron();
		return true;
	}

	//���ñ���
	switch (cbJettonArea)
	{
		case ID_ODD:
			{
				m_lMeOddScore += lJettonScore;
				m_GameClientView.SetMeOddScore(m_lMeOddScore);
				break;
			}
		case ID_EVEN:
			{
				m_lMeEvenScore += lJettonScore;
				m_GameClientView.SetMeEvenScore(m_lMeEvenScore);
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

	//����״̬
	m_bPlaying = true;

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	tagUserData const *pMeUserData = GetUserData( GetMeChairID() );
	if ( pMeUserData->lScore < m_lApplyBankerCondition ) return true;

	//�Թ��ж�
	if ( IsLookonMode() ) return true;

	//ת������
	bool bApplyBanker = wParam != 0 ? true : false;

	//��ǰ�ж�
	if ( m_wCurrentBanker == GetMeChairID() && bApplyBanker ) return true;

	CMD_C_ApplyBanker ApplyBanker;

	//��ֵ����
	ApplyBanker.bApplyBanker = bApplyBanker;

	//������Ϣ
	SendData( SUB_C_APPLY_BANKER, &ApplyBanker, sizeof( ApplyBanker ) );

	//���ð�ť
	if ( m_wCurrentBanker == GetMeChairID() && !bApplyBanker )
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
	}

	return true;
}

//������Ϣ
LRESULT CGameClientDlg::OnUpdateControl(WPARAM wParam, LPARAM lParam)
{
	//���¿���
	UpdateButtonContron();

	return true;
}

//������ׯ
bool CGameClientDlg::OnUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//�������
	if ( pApplyBanker->bApplyBanker )
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName = pApplyBanker->szAccount;
		ApplyUser.lUserScore = pApplyBanker->lScore;

		//�����ж�
		bool bInsertApplyUser = true;

		if ( m_wCurrentBanker != INVALID_CHAIR )
		{
			tagUserData const *pBankerUserData = GetUserData(m_wCurrentBanker);
			if ( pBankerUserData != NULL && 0==lstrcmp(pBankerUserData->szName, pApplyBanker->szAccount))
				bInsertApplyUser = false;
		}

		if ( bInsertApplyUser == true ) m_GameClientView.m_ApplyUser.InserUser( ApplyUser );

		//������ť
		tagUserData const *pUserData = GetUserData( GetMeChairID() );
		if ( pUserData && !strcmp(pApplyBanker->szAccount, pUserData->szName ) )
		{
			m_bMeApplyBanker = true;
			UpdateButtonContron();
		}
	}
	else
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName = pApplyBanker->szAccount;
		ApplyUser.lUserScore = pApplyBanker->lScore;
		m_GameClientView.m_ApplyUser.DeleteUser( ApplyUser );

		//������ť
		tagUserData const *pUserData = GetUserData( GetMeChairID() );
		if ( pUserData &&  !strcmp(pApplyBanker->szAccount, pUserData->szName ) )
		{
			m_bMeApplyBanker = false;
			
			UpdateButtonContron();
		}
	}

	return true;
}

//�л�ׯ��
bool CGameClientDlg::OnChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//��ʾͼƬ
	m_GameClientView.ShowChangeBanker( true );

	//�Լ��ж�
	if ( m_wCurrentBanker == GetMeChairID() && IsLookonMode() == false && pChangeBanker->wChairID != GetMeChairID() ) 
	{
		m_bMeApplyBanker = false;
	}

	m_wCurrentBanker = pChangeBanker->wChairID;

	//ׯ����Ϣ
	if ( pChangeBanker->wChairID == INVALID_CHAIR )
	{
		m_GameClientView.SetBankerInfo( INVALID_CHAIR, pChangeBanker->cbBankerTime, pChangeBanker->lBankerScore );
	}
	else
	{
		m_GameClientView.SetBankerInfo( SwitchViewChairID( pChangeBanker->wChairID ), pChangeBanker->cbBankerTime, 0 );
	}
	m_GameClientView.SetBankerTreasure(pChangeBanker->lBankerTreasure);

	//ɾ�����
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		tagUserData const *pBankerUserData = GetUserData(m_wCurrentBanker);
		if ( pBankerUserData != NULL )
		{
			tagApplyUser ApplyUser;
			ApplyUser.strUserName = pBankerUserData->szName;
			m_GameClientView.m_ApplyUser.DeleteUser( ApplyUser );
		}
	}
	
	//���½���
	UpdateButtonContron();

	return true;
}

//���»���
bool CGameClientDlg::OnChangeUserScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeUserScore));
	if (wDataSize!=sizeof(CMD_S_ChangeUserScore)) return false;

	//��Ϣ����
	CMD_S_ChangeUserScore * pChangeUserScore=(CMD_S_ChangeUserScore *)pBuffer;

	tagUserData const *pUserData = GetUserData( pChangeUserScore->wChairID );

	if ( pUserData )
	{
		tagApplyUser ApplyUser ;

		ApplyUser.lUserScore = pChangeUserScore->lScore;
		ApplyUser.strUserName = pUserData->szName;
		m_GameClientView.m_ApplyUser.UpdateUser( ApplyUser );
	}

	//ׯ����Ϣ
	if ( m_wCurrentBanker == pChangeUserScore->wCurrentBankerChairID )
	{
		m_GameClientView.SetBankerInfo( SwitchViewChairID( pChangeUserScore->wCurrentBankerChairID ), pChangeUserScore->cbBankerTime, pChangeUserScore->lCurrentBankerScore );

		//���½���
		UpdateButtonContron();
	}

	return true;
}

//��Ϸ��¼
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;
	
	//�������
	tagServerGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(enOperateResult_NULL, pServerGameRecord->cbDicePoint);
	}

	return true;
}

//�ƶ�Ӯ��
void CGameClientDlg::DeduceWinner(BYTE &cbWinner)
{
	bool bEven = (m_cbDice[0] + m_cbDice[1]) % 2 == 0 ? true : false;
	if ( bEven ) cbWinner = ID_EVEN;
	else cbWinner = ID_ODD;
}
//////////////////////////////////////////////////////////////////////////
