#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//�����˵��
#pragma comment(lib,"Version")

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_CHANGE_SICBO_TYPE, OnChangeSicboType)
	ON_MESSAGE(IDM_OPEN_BANK, OnOpenBank)
	ON_MESSAGE(IDM_USE_WINNER, OnUseWinner)
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
	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));
	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//״̬����
	m_bMeApplyBanker=false;
	m_bShowBtnSicbo = false;

	m_bJoinInFree=true;

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//��ȡ��Ϣ
	//TCHAR szExeName[_MAX_PATH];
	//_sntprintf(szExeName, sizeof(szExeName), _T("%s.exe"), AfxGetApp()->m_pszExeName);
	//OUTPUT_DEBUG_STRING(szExeName);
	//VS_FIXEDFILEINFO	*pFixedFileInfo=NULL;					//�ļ���Ϣ
	//DWORD dwLangCharset = 0;
	//BYTE byInfoBuffer[4096];
	//memset(byInfoBuffer,0,sizeof(byInfoBuffer));
	TCHAR szVerInfo[100]={_T("������Ϸ  --  Ver��1.0.0.0")};
	//do 
	//{
	//	//���ļ�
	//	DWORD dwFileHandle=NULL;
	//	DWORD dwBufferSize=::GetFileVersionInfoSize(szExeName,&dwFileHandle);
	//	if ((dwBufferSize==0)||(dwBufferSize>sizeof(byInfoBuffer)))
	//	{
	//		break;
	//	}
	//	if (!::GetFileVersionInfo(szExeName,dwFileHandle,sizeof(byInfoBuffer),byInfoBuffer))
	//	{
	//		break;
	//	}

	//	// �ļ��汾
	//	UINT uQuerySize=0;
	//	if (!::VerQueryValue(byInfoBuffer,TEXT("\\"),(void * *)&pFixedFileInfo,&uQuerySize))
	//	{
	//		break;
	//	}
	//	if ((uQuerySize!=sizeof(VS_FIXEDFILEINFO))||(NULL==pFixedFileInfo))
	//	{
	//		break;
	//	}
	//	WORD   nVer[4];   
	//	nVer[0]   =   HIWORD(pFixedFileInfo->dwProductVersionMS);
	//	nVer[1]   =   LOWORD(pFixedFileInfo->dwProductVersionMS);
	//	nVer[2]   =   HIWORD(pFixedFileInfo->dwProductVersionLS);
	//	nVer[3]   =   LOWORD(pFixedFileInfo->dwProductVersionLS);
	//	_sntprintf(szVerInfo, sizeof(szVerInfo), _T("������Ϸ  --  Ver��%u.%u.%u.%u"), nVer[0],nVer[1],nVer[2],nVer[3]);
	//} while (0);

	// ���ñ���
	SetWindowText(szVerInfo);

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
	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//״̬����
	m_bMeApplyBanker=false;
	m_bShowBtnSicbo = false;

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	return ;
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
		if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
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
			m_bJoinInFree=true;
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			m_bJoinInFree=true;
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
			m_bJoinInFree=true;
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
	case SUB_C_CHANGE_SICBO_TYPE:
		{
			return OnSubChangeSicboType(pBuffer,wDataSize);
		}
	default:break;
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	IClientKernel *pIClientKernel = ( IClientKernel * )GetClientKernel( IID_IClientKernel, VER_IClientKernel );
	pServerAttribute  = pIClientKernel->GetServerAttribute();

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

			m_GameClientView.SetGameMaxScore(m_lMeMaxScore, 0);
			WORD wMeChairID=GetMeChairID();
		
			WORD wSwitchViewChairID=SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wSwitchViewChairID);
			
            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			// ֱ�ӽ�ׯ�ҽ��ֵ��ֵ����ͼ
			m_GameClientView.m_lBankerWinScore = pStatusFree->lBankerWinScore;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//����״̬
			SetGameStatus(GS_FREE);

			m_bJoinInFree=true;
			//���¿���
			UpdateButtonContron(m_bJoinInFree);
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

			// ������ע����			
			for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
			{
				//ȫ����ע
				m_GameClientView.PlaceUserJetton(i,pStatusPlay->lAllScore[i]);

				//�����ע
				SetMePlaceJetton(i,pStatusPlay->lUserScore[i]);
			}

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;
			m_GameClientView.SetGameMaxScore(m_lMeMaxScore, pStatusPlay->lAllMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));
			m_GameClientView.m_enSicboType = (E_SICBO_TYPE)pStatusPlay->bySicboType;
			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetCardInfo(pStatusPlay->enCards, pStatusPlay->bWinner);
				m_GameClientView.FinishDispatchCard();
				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				// ֱ�ӽ�ׯ�ҽ��ֵ��ֵ����ͼ
				m_GameClientView.m_lBankerWinScore = pStatusPlay->lBankerWinScore;
				m_GameClientView.SetCardInfo(NULL, NULL);

				//��������
				if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			}

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			m_bJoinInFree=false;
			//���°�ť
			UpdateButtonContron(m_bJoinInFree);
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

	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetGameMaxScore(m_lMeMaxScore, pGameStart->lAllMaxScore);

	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	if ((GetMeChairID() == m_wCurrentBanker) && !IsLookonMode())
	{
		m_bShowBtnSicbo = true;
	}
	else
	{
		m_bShowBtnSicbo = false;
	}
	m_GameClientView.SetSicboType(enSicboType_Sicbo);
	//���¿���
	UpdateButtonContron(m_bJoinInFree);

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
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=0; nAreaIndex<COUNT_AZIMUTH; ++nAreaIndex)
	{
		SetMePlaceJetton(nAreaIndex,0);
	}

	//���¿ؼ�
	UpdateButtonContron(m_bJoinInFree);

	//��ɷ���
	//m_GameClientView.FinishDispatchCard();

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
	if ((pPlaceJetton->cbJettonArea>=COUNT_AZIMUTH) || (pPlaceJetton->lJettonScore<0))
	{
		return true;
	}

	// ���������ע����
	m_lAllScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

	//���¿ؼ�
	UpdateButtonContron(m_bJoinInFree);

	// ��������
	if (IsEnableSound())
	{
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			if (pPlaceJetton->lJettonScore==5000000)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
			}
			else
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
			}
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
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		m_GameClientView.SetBombEffect(false,i);
	}

	// ����ʱ��
	SetGameTimer(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	// ҡ����
	m_GameClientView.SetCardInfo(pGameEnd->enCards, pGameEnd->bWinner);

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	// ����״̬
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
		m_GameClientView.UpdateApplyBanker(ApplyUser);
	}

	//���¿ؼ�
	UpdateButtonContron(m_bJoinInFree);

	//ֹͣ����
	for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientDlg::UpdateButtonContron(bool bEnablePlaceJetton)
{
	//�����ж�
	//bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) bEnablePlaceJetton=false;
	if (GetGameStatus()!=GS_PLACE_JETTON) bEnablePlaceJetton=false;
	if (m_wCurrentBanker==GetMeChairID())
	{
		bEnablePlaceJetton=false;
		if (!IsLookonMode() && (GetGameStatus() ==GS_PLACE_JETTON)
			&& m_bShowBtnSicbo)  // �Ѿ�ѡ�����������ʾ��ť
		{
			//m_GameClientView.m_btSicboDesktop.EnableWindow(TRUE);
			//m_GameClientView.m_btSicboInMidair.EnableWindow(TRUE);
			//m_GameClientView.m_btSicboInTheAir.EnableWindow(TRUE);
			//m_GameClientView.m_btSicboDesktop.ShowWindow(SW_SHOW);
			//m_GameClientView.m_btSicboInMidair.ShowWindow(SW_SHOW);
			//m_GameClientView.m_btSicboInTheAir.ShowWindow(SW_SHOW);
		}
		else
		{
			m_GameClientView.m_btSicboDesktop.ShowWindow(SW_HIDE);
			m_GameClientView.m_btSicboInMidair.ShowWindow(SW_HIDE);
			m_GameClientView.m_btSicboInTheAir.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_GameClientView.m_btSicboDesktop.ShowWindow(SW_HIDE);
		m_GameClientView.m_btSicboInMidair.ShowWindow(SW_HIDE);
		m_GameClientView.m_btSicboInTheAir.ShowWindow(SW_HIDE);
	}

	if (IsLookonMode()) bEnablePlaceJetton=false;

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�������
		__int64 lCurrentJetton=m_GameClientView.GetCurrentJetton();
		__int64 lLeaveScore= 0L;
		for (int i=0;i<COUNT_AZIMUTH; ++i)
		{
			__int64 iLeaveScoreTemp = m_GameClientView.GetMaxPlayerScore(i);
			if (iLeaveScoreTemp > lLeaveScore)
			{
				lLeaveScore = iLeaveScoreTemp;
			}
		}

		//���ù��
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=5000000L)      m_GameClientView.SetCurrentJetton(5000000L);		
			else if (lLeaveScore>=1000000L)	m_GameClientView.SetCurrentJetton(1000000L);
			else if (lLeaveScore>=500000L)  m_GameClientView.SetCurrentJetton(500000L);
			else if (lLeaveScore>=100000L)  m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore>=10000L)   m_GameClientView.SetCurrentJetton(10000L);
			else if (lLeaveScore>=1000L)    m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=100L)     m_GameClientView.SetCurrentJetton(100L);
			else m_GameClientView.SetCurrentJetton(0L);
		}

		//���ư�ť
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000)?TRUE:FALSE);
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
	if(this->IsFreeze())
		return false;

	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	__int64 lJettonScore=(__int64)lParam;

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	if (IsLookonMode() || lJettonScore<0)
	{
		return true;
	}

	//״̬�ж�
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron(m_bJoinInFree);
		return true;
	}

	//���ñ���
	if (cbJettonArea<COUNT_AZIMUTH)
	{
		m_lMeScore[cbJettonArea] += lJettonScore;
		m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lMeScore[cbJettonArea]);
	}

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	// �������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//������Ϣ
	SendData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron(m_bJoinInFree);

	//��������
	if (IsEnableSound()) 
	{
		if (lJettonScore==5000000)
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		}
		else
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		}
		m_DTSDCheer[rand()%3].Play();
	}

	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{

	if(this->IsFreeze())
		return false;
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
	UpdateButtonContron(m_bJoinInFree);

	return true;
}

// ѡ��ҡ���ӷ�ʽ
LRESULT CGameClientDlg::OnChangeSicboType(WPARAM wParam, LPARAM lParam)
{
	if (!m_bShowBtnSicbo || IsLookonMode() || (m_wCurrentBanker != GetMeChairID()))
	{
		return 0;
	}
	CMD_C_ChangeSicboType cmdChangeSicboType;
	ZeroMemory(&cmdChangeSicboType, sizeof(cmdChangeSicboType));
	cmdChangeSicboType.enSicboType = (E_SICBO_TYPE)wParam;
	m_bShowBtnSicbo = false;
	m_GameClientView.m_btSicboDesktop.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSicboInMidair.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSicboInTheAir.ShowWindow(SW_HIDE);
	SendData(SUB_C_CHANGE_SICBO_TYPE, &cmdChangeSicboType, sizeof(cmdChangeSicboType));
	return 0;
}
LRESULT CGameClientDlg::OnOpenBank(WPARAM wParam, LPARAM lParam)
{
	IClientKernel *pIClientKernel = (IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	ASSERT(pIClientKernel!=NULL);
	if (pIClientKernel==NULL) return 0;
	pIClientKernel->ShowBankDialog( 0,0 );
	return 0;
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
		m_GameClientView.AddApplyBanker(ApplyUser);
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonContron(m_bJoinInFree);

	return true;
}

//ȡ����ׯ
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//ɾ�����
	tagApplyUser ApplyUser;
	ApplyUser.strUserName=pCancelBanker->szCancelUser;
	ApplyUser.lUserScore=0;
	m_GameClientView.CancelApplyBanker(ApplyUser);

	//�Լ��ж�
	const tagUserData *pMeUserData=GetUserData(GetMeChairID());
	if (IsLookonMode()==false && lstrcmp(pMeUserData->szName,pCancelBanker->szCancelUser)==0) m_bMeApplyBanker=false;

	//���¿ؼ�
	UpdateButtonContron(m_bJoinInFree);

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

	// ׯ����
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
			m_GameClientView.CancelApplyBanker(ApplyUser);
		}
	}

	//���½���
	UpdateButtonContron(m_bJoinInFree);
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
	tagClientGameRecord ClientGameRecord;
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);	
		int iCardDot =0;
		for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
		{
			//OUTPUT_DEBUG_STRING("client ���յ��ĵ��� %d", pServerGameRecord->enCards[i]);
			ClientGameRecord.iCards[i] = pServerGameRecord->enCards[i];
			iCardDot += ClientGameRecord.iCards[i];
		}
		if (iCardDot>enCardType_NumberTen)
		{
			ClientGameRecord.bFlags = true;
		}
		else
		{
			ClientGameRecord.bFlags = false;
		}
		m_GameClientView.SetGameHistory(ClientGameRecord);
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
	ASSERT(cbViewIndex<COUNT_AZIMUTH);
	if (cbViewIndex>=COUNT_AZIMUTH) return false;

	__int64 lJettonCount=pPlaceJettonFail->lPlaceScore;
	m_lMeScore[cbViewIndex] -= lJettonCount;
	if (m_lMeScore[cbViewIndex] < 0)
	{
		m_lMeScore[cbViewIndex] = 0;
	}
	m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeScore[cbViewIndex]);
	UpdateButtonContron(m_bJoinInFree);
	return true;
}
// ѡ��ҡ���ӷ�ʽ
bool CGameClientDlg::OnSubChangeSicboType(const void * pBuffer, WORD wDataSize)
{
	//Ч������
		ASSERT(wDataSize==sizeof(CMD_C_ChangeSicboType));
	if (wDataSize!=sizeof(CMD_C_ChangeSicboType)) return false;

	//��Ϣ����
	CMD_C_ChangeSicboType * pChangeSicboType=(CMD_C_ChangeSicboType *)pBuffer;
	if ((pChangeSicboType->enSicboType <enSicboType_Desktop)
		|| (pChangeSicboType->enSicboType > enSicboType_InTheAir))
	{
		return true;
	}
	m_GameClientView.SetSicboType(pChangeSicboType->enSicboType);
	return true;
}
//����ׯ��
void CGameClientDlg::SetBankerInfo(WORD wBanker,__int64 lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
}

//������ע
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<COUNT_AZIMUTH);
	if (cbViewIndex>=COUNT_AZIMUTH) return;
	m_lMeScore[cbViewIndex] = lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}

// ʤ������
LRESULT CGameClientDlg::OnUseWinner(WPARAM wParam, LPARAM lParam)
{
	if (GS_PLACE_JETTON != GetGameStatus())
	{
		return 0;
	}

	BYTE byArea = (BYTE)wParam;
	BYTE byType = (BYTE)lParam;
	if ((byArea<0) || (byArea>=COUNT_AZIMUTH)
		|| ((USER_LOSER!= byType)&&(USER_WINNER != lParam)))
	{
		return 0;
	}
	
	CMD_C_UseWinner sUseWinner;
	sUseWinner.byArea =byArea;
	sUseWinner.byType = byType;
	SendData(SUB_C_USE_WINNER, (void *)&sUseWinner, sizeof(sUseWinner));
	return 0;
}
//////////////////////////////////////////////////////////////////////////
bool CGameClientDlg::IsFreeze(void)
{
	/*if(pServerAttribute->wGameGenre==GAME_GENRE_GOLD)
	{
		tagUserData const *pMeUserData = GetUserData( GetMeChairID());
		if(pMeUserData->bFreeze)
		{
			ShowInformation(TEXT("�Բ��������ʻ��ѱ����ᣬ���ܽ��д˲�����\n\n�������Ա��ϵ��"),0,MB_ICONINFORMATION);
			return true;
		}
	}*/
	return false;
}
