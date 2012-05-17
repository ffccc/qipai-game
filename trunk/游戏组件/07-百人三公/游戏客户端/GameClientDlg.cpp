#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
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
	m_lMeTianScore=0L;						//��
	m_lMeDiScore=0L;						//��
	m_lMeXuanScore=0L;						//��
	m_lMeHuangScore=0L;					//��

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//״̬����
	m_bMeApplyBanker=false;

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
	SetWindowText(TEXT("����������Ϸ  --  Ver��6.6.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��������
	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));

	////�����㷨
	////�����㷨
	//BYTE cbFirst1[3] = {0x2D, 0x3D, 0x0D};
	//BYTE cbNext1[3] = {0x33, 0x23, 0x13};

	//if (m_GameLogic.IsBigger(cbFirst1, cbNext1, 3, 3))
	//	::OutputDebugString("����");
	//else
	//	::OutputDebugString("С��");

	//BYTE cbFirst2[3] = {0x21, 0x3D, 0x0D};
	//BYTE cbNext2[3] = {0x33, 0x24, 0x13};
	//if (m_GameLogic.IsBigger(cbFirst2, cbNext2, 3, 3))
	//	::OutputDebugString("����");
	//else
	//	::OutputDebugString("С��");

	//BYTE cbFirst3[3] = {0x2D, 0x3D, 0x0D};
	//BYTE cbNext3[3] = {0x35, 0x25, 0x15};
	//if (m_GameLogic.IsBigger(cbFirst3, cbNext3, 3, 3))
	//	::OutputDebugString("����");
	//else
	//	::OutputDebugString("С��");


	//BYTE cbFirst4[3] = {0x1D, 0x2a, 0x3d};
	//BYTE cbNext4[3] = {0x1b, 0x34, 0x3d};
	//if (m_GameLogic.IsBigger(cbFirst4, cbNext4, 3, 3))
	//	::OutputDebugString("����");
	//else
	//	::OutputDebugString("С��");
	/*BYTE cbFirst5[3] = {0x22, 0x21, 0x27};
	CString str;
	str.Format("�Ƿ��Ǳ�ʮ��%d", m_GameLogic.IsBiShi(cbFirst5, 3) );
	::OutputDebugString(str);
	str.Format("�Ƿ���ɢ�ƣ�%d", m_GameLogic.IsSanPai(cbFirst5, 3) );
	::OutputDebugString(str);
	str.Format("���ͣ�%d", m_GameLogic.GetCardType(cbFirst5, 3) );
	::OutputDebugString(str);*/

 
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	m_lMeTianScore=0L;						//��
	m_lMeDiScore=0L;						//��
	m_lMeXuanScore=0L;						//��
	m_lMeHuangScore=0L;					//��

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
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton50000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);


		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}

	if (IsEnableSound()) 
	{
		if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
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
			m_GameClientView.PlaceUserJetton(ID_TIAN_MARK,pStatusPlay->lAllTianScore);
			m_GameClientView.PlaceUserJetton(ID_DI_MARK,pStatusPlay->lAllDiScore);
			m_GameClientView.PlaceUserJetton(ID_XUAN_MARK,pStatusPlay->lAllXuanScore);
			m_GameClientView.PlaceUserJetton(ID_HUANG_MARK,pStatusPlay->lAllHuangScore);


			//�����ע
			SetMePlaceJetton(ID_TIAN_MARK,pStatusPlay->lUserTianScore);
			SetMePlaceJetton(ID_DI_MARK,pStatusPlay->lUserDiScore);
			SetMePlaceJetton(ID_XUAN_MARK,pStatusPlay->lUserXuanScore);
			SetMePlaceJetton(ID_HUANG_MARK,pStatusPlay->lUserHuangScore);

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
				m_GameClientView.SetCardInfo(true,pStatusPlay->cbTableCardArray);
				m_GameClientView.FinishDispatchCard();

				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(false,NULL);

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
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

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

	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	//���¿���
	UpdateButtonContron();

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	if (IsEnableSound()) 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
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

	//����״̬
	SetGameStatus(GS_FREE);

	//��������
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.m_bShowResult=false;
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_TIAN_MARK; nAreaIndex<=ID_HUANG_MARK; nAreaIndex*=2) SetMePlaceJetton(nAreaIndex,0);

	//���¿ؼ�
	UpdateButtonContron();

	for(int i=0;i<5;i++)
		m_GameClientView.m_CardControl[i].ReSet();
	m_GameClientView.KillTimer(123456);
	m_GameClientView.m_cbWaitPre=0;
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
			if (pPlaceJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
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
//--ȡ��
	//ȡ������
	//for (WORD wAreaIndex=ID_TIAN_MARK; wAreaIndex<=ID_HUANG_MARK; wAreaIndex*=2) 
	//	m_GameClientView.SetBombEffect(false,wAreaIndex);
	
	m_GameClientView.m_bCanGo=false;
	m_GameClientView.m_bSet=false;
	

	//m_GameClientView.SetAreaFlash(0xFF);


	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(true,pGameEnd->cbTableCardArray);

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	//����״̬
	SetGameStatus(GS_GAME_END);

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
//	m_DTSDCheer[3].Stop();
//	m_DTSDCheer[4].Stop();



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
		LONG lCurrentJetton=m_GameClientView.GetCurrentJetton();

		const tagUserData *pMeUserData=GetUserData(GetMeChairID());
		//LONG lLeaveScore=m_lMeMaxScore-m_lMeTianScore*10-m_lMeDiScore*10-m_lMeXuanScore*10-m_lMeHuangScore*10;
		LONG lLeaveScore1=pMeUserData->lScore-m_lMeTianScore*10-m_lMeDiScore*10-m_lMeXuanScore*10-m_lMeHuangScore*10;
		LONG lLeaveScore2=m_lMeMaxScore-m_lMeTianScore-m_lMeDiScore-m_lMeXuanScore-m_lMeHuangScore;

		//CString str;
		//str.Format("%ld,%ld",lLeaveScore1,lLeaveScore2);
		//AfxMessageBox(str);
		LONG lLeaveScore=min(lLeaveScore1/10,lLeaveScore2);


	/*	CString str;
		str.Format("%ld",lLeaveScore);
		AfxMessageBox(str);*/
		//���ù��

		//if (lCurrentJetton*10>lLeaveScore)
		//{
		//	if (lLeaveScore>=5000000L*10) m_GameClientView.SetCurrentJetton(5000000L);
		//	else if (lLeaveScore>=1000000L*10) m_GameClientView.SetCurrentJetton(1000000L);
		//	else if (lLeaveScore>=100000L*10) m_GameClientView.SetCurrentJetton(100000L);
		//	else if (lLeaveScore>=10000L*10) m_GameClientView.SetCurrentJetton(10000L);
		//	else if (lLeaveScore>=1000L*10) m_GameClientView.SetCurrentJetton(1000L);
		//	else if (lLeaveScore>=100L*10) m_GameClientView.SetCurrentJetton(100L);
		//	else m_GameClientView.SetCurrentJetton(0L);
		//}



		////���ư�ť
		//m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100*10)?TRUE:FALSE);
		//m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000*10)?TRUE:FALSE);
		//m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000*10)?TRUE:FALSE);
		//m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000*10)?TRUE:FALSE);
		//m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000*10)?TRUE:FALSE);		
		//m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=5000000*10)?TRUE:FALSE);

		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=5000000L) m_GameClientView.SetCurrentJetton(5000000L);
			else if (lLeaveScore>=1000000L) m_GameClientView.SetCurrentJetton(1000000L);
			else if (lLeaveScore>=500000L) m_GameClientView.SetCurrentJetton(500000L);
			else if (lLeaveScore>=100000L) m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore>=50000L) m_GameClientView.SetCurrentJetton(50000L);
			else if (lLeaveScore>=10000L) m_GameClientView.SetCurrentJetton(10000L);
			else if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=100L) m_GameClientView.SetCurrentJetton(100L);
			else m_GameClientView.SetCurrentJetton(0L);
		}



		//���ư�ť
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000)?TRUE:FALSE);
		m_GameClientView.m_btJetton50000.EnableWindow((lLeaveScore>=50000)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000)?TRUE:FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow((lLeaveScore>=500000)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=5000000)?TRUE:FALSE);
	}
	else
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton50000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
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
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

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
	LONG lJettonScore=(LONG)lParam;

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
		case ID_TIAN_MARK:
			{
				m_lMeTianScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_TIAN_MARK,m_lMeTianScore);
				break;
			}
		case ID_DI_MARK:
			{
				m_lMeDiScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_DI_MARK,m_lMeDiScore);
				break;
			}
		case ID_XUAN_MARK:
			{
				m_lMeXuanScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_XUAN_MARK,m_lMeXuanScore);
				break;
			}
		case ID_HUANG_MARK:
			{
				m_lMeHuangScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_HUANG_MARK,m_lMeHuangScore);
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
		if (lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
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

	//�������
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(enOperateResult_NULL,pServerGameRecord->cbWinner);
		//m_GameClientView.SetGameHistory(enOperateResult_NULL, pServerGameRecord->cbPlayerCount, pServerGameRecord->cbBankerCount,
		//	pServerGameRecord->cbKingWinner,pServerGameRecord->bPlayerTwoPair,pServerGameRecord->bBankerTwoPair);
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
	ASSERT(cbViewIndex<=ID_HUANG_MARK);
	if (cbViewIndex>ID_HUANG_MARK) return false;

	LONG lJettonCount=pPlaceJettonFail->lPlaceScore;
	if (cbViewIndex==ID_TIAN_MARK)
	{
		m_lMeTianScore=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeTianScore);
	}
	else if (cbViewIndex==ID_DI_MARK) 
	{
		m_lMeDiScore=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeDiScore);
	}
	else if (cbViewIndex==ID_XUAN_MARK) 
	{
		m_lMeXuanScore=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeXuanScore);
	}
	else if (cbViewIndex==ID_HUANG_MARK) 
	{
		m_lMeHuangScore=lJettonCount-lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeHuangScore);
	}
	else {ASSERT(FALSE);return false;}

	return true;
}

//����ׯ��
void CGameClientDlg::SetBankerInfo(WORD wBanker,LONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
}

//������ע
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_HUANG_MARK);
	if (cbViewIndex>ID_HUANG_MARK) return;

	if (cbViewIndex==ID_TIAN_MARK) m_lMeTianScore=lJettonCount;
	else if (cbViewIndex==ID_DI_MARK) m_lMeDiScore=lJettonCount;
	else if (cbViewIndex==ID_XUAN_MARK) m_lMeXuanScore=lJettonCount;
	else if (cbViewIndex==ID_HUANG_MARK) m_lMeHuangScore=lJettonCount;
	else {ASSERT(FALSE);return;}

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//////////////////////////////////////////////////////////////////////////
