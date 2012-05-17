#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��250
//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH				101									//ת��ʱ��
#define IDI_MOVE_TIMER				102									//���ֶ���
#define IDI_MOVE_INDEX				103									//��������
#define IDI_SELECT_INDEX			104									//ѡ��״̬
#define IDI_FLASH_WIN				105									//����
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_SCORE_L, OnScoreLeft)
	ON_MESSAGE(IDM_SCORE_R, OnScoreRight)
	//ON_MESSAGE(IDM_BANKER_T, OnBankerTop)
	//ON_MESSAGE(IDM_BANKER_B, OnBankerBottom)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_BANK, OnBank)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	
	m_cbMoveIndex = 0;
	m_cbMoveEndIndex = 0;
	m_cbResultIndex = 0;
	m_cbEndIndex = 0;
	m_lMeCanUseScore = 0L;
	m_bFlashFlags = false;
	m_lMeStatisticScore = 0L;
	//ׯ����Ϣ
	m_lBankerScore = 0L;
	m_wCurrentBanker = 0L;
	m_wBankerTime = 0;
	m_lTmpBankerWinScore = 0L;
	//������Ϣ
	m_lMeScore = 0L;
	//������ע��Ϣ

	m_lMeBigSharkScore=0L;	
	m_lMeFeiQinScore=0L;	 
	m_lMeZouShouScore=0L;	 
	m_lMeYanZiScore=0L;				
	m_lMeTuZiScore=0L;				
	m_lMeGeZiScore=0L;					
	m_lMeXiongMaoScore=0L;					
	m_lMeKongQueScore=0L;					
	m_lMeHouZiScore=0L;				
	m_lMeLaoYingScore=0L;				
	m_lMeShiZiScore=0L;					

	//ȫ����ע
	m_lALLBigSharkScore=0L;
	m_lALLFeiQinScore=0L;
	m_lALLZouShouScore=0L;
	m_lALLYanZiScore=0L;				
	m_lALLTuZiScore=0L;				
	m_lALLGeZiScore=0L;					
	m_lALLXiongMaoScore=0L;					
	m_lALLKongQueScore=0L;					
	m_lALLHouZiScore=0L;					
	m_lALLLaoYingScore=0L;					
	m_lALLShiZiScore=0L;
	//״̬����
	m_bMeApplyBanker=false;

	//������ׯ��Ϣ
	m_ApplyUser.RemoveAll();
	m_cbApplyCount = 0; 
	m_cbApplyIndex = 0;

	ZeroMemory(m_cbScoreHistroy, sizeof(m_cbScoreHistroy));

	//������Ϣ
	m_lBankerEndScore = 0;
	m_lMeEndScore = 0;
	m_lRevenue = 0;
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
	SetWindowText(TEXT("�����  --  �������"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ʼ������
	m_DirectSound[0].Create(TEXT("IDC_SND_BK"));			//��������
	m_DirectSound[1].Create(TEXT("IDC_SND_ADDCHIP"));		//��ע����
	m_DirectSound[2].Create(TEXT("IDC_SND_GAME_START"));	//��ʼ����
	m_DirectSound[3].Create(TEXT("IDC_SND_GAMELOST"));		//������
	m_DirectSound[4].Create(TEXT("IDC_SND_GAMEWIN"));		//Ӯ����
	m_DirectSound[5].Create(TEXT("IDC_SND_SELGRID"));		//ѡ����������
	m_DirectSound[6].Create(TEXT("IDC_SND_STARTSHOWCARD"));	//��ת��ʾ
	m_DirectSound[7].Create(TEXT("IDC_BACK_GROUND"));		//��ע����

	m_DirectSound[0].EnableSound(TRUE);
	m_DirectSound[1].EnableSound(TRUE);
	m_DirectSound[2].EnableSound(TRUE);
	m_DirectSound[3].EnableSound(TRUE);
	m_DirectSound[4].EnableSound(TRUE);
	m_DirectSound[5].EnableSound(TRUE);
	m_DirectSound[6].EnableSound(TRUE);
	m_DirectSound[7].EnableSound(TRUE);
	return true;
}

//��ʼ����
BOOL CGameClientDlg::OnInitDialog()
{
	__super::OnInitDialog();

	SetWindowPos(NULL,200,150,850,736,SWP_NOZORDER);
	CenterWindow();
	ModifyStyle(WS_THICKFRAME, CS_VREDRAW| CS_HREDRAW|DS_MODALFRAME, 0);
	RestoreWindow();

	return TRUE;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	
	m_cbMoveIndex = 0;
	m_cbMoveEndIndex = 0;
	m_cbResultIndex = 0;
	m_cbEndIndex = 0;
	m_lMeCanUseScore = 0L;
	m_bFlashFlags = false;
	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	//������Ϣ
	m_lMeScore = 0L;
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
	if (nTimerID==IDI_FREE)
	{
		m_GameClientView.SetGameTimerNum(nElapse);
		if (IsEnableSound())
		{
			if (nElapse==1)
				m_DirectSound[2].Play();
		}
		return true;
	}
	if (nTimerID==IDI_PLACE_JETTON)
	{
		m_DirectSound[0].Stop();
		m_GameClientView.SetGameTimerNum(nElapse);
		if (IsEnableSound()&&nElapse==1)
			m_DirectSound[6].Play();
		if (IsEnableSound()&&nElapse<=5&&nElapse>0)
			PlayGameSound(AfxGetInstanceHandle(),TEXT("IDC_SND_WARIMG"));
		return true;
	}
	if (nTimerID==IDI_DISPATCH)
	{
		m_GameClientView.SetGameTimerNum(nElapse);
		if (nElapse<=5)
		{
			m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
			m_bFlashFlags = false;
			m_cbMoveEndIndex = 0;
			m_cbMoveIndex = 0;
			m_cbMoveEndIndex = 0;
			m_cbResultIndex = 0;
			m_GameClientView.SetCanMove(false);
			m_GameClientView.SetMoveIndex(0xff);
			m_GameClientView.SetSelectIndex(0xff);
			m_GameClientView.SetSelectJettonIndex(0xff);
			m_GameClientView.SetBankerScore(m_wBankerTime, m_lTmpBankerWinScore);
			KillTimer(IDI_FLASH_WIN);
			if (IsEnableSound()&&nElapse==5)
			{
				//��������
				if (m_wCurrentBanker==GetMeChairID()&&m_lBankerScore>0)
					m_DirectSound[4].Play();
				else
					m_DirectSound[3].Play();
				if (m_wCurrentBanker!=GetMeChairID()&&m_lMeScore>0)
					m_DirectSound[4].Play();
				else
					m_DirectSound[3].Play();
			}			
		}
		return true;
	}
	return false;
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
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
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
	case SUB_S_SCORE_HISTORY:	//��ʷ��Ϣ
		{
			return OnSubSocreHistory(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_SYS_BANKER://�л�ׯ��
		{
			return OnSubChangeSysBanker(pBuffer, wDataSize);
		}
	}
	return false;
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
			//SetGameTimer(GetMeChairID(),IDI_FREE,24);
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
			m_DirectSound[0].Play();
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
			m_GameClientView.PlaceUserJetton(ID_BIG_SHARK,pStatusPlay->lALLBigSharkScore);
			m_GameClientView.PlaceUserJetton(ID_FEI_QIN,pStatusPlay->lALLFeiQinScore);
			m_GameClientView.PlaceUserJetton(ID_ZOU_SHOU,pStatusPlay->lALLZouShouScore);
			m_GameClientView.PlaceUserJetton(ID_YAN_ZI,pStatusPlay->lALLYanZiScore);
			m_GameClientView.PlaceUserJetton(ID_TU_ZI,pStatusPlay->lALLTuZiScore);
			m_GameClientView.PlaceUserJetton(ID_GE_ZI,pStatusPlay->lALLGeZiScore);
			m_GameClientView.PlaceUserJetton(ID_XIONG_MAO,pStatusPlay->lALLXiongMaoScore);
			m_GameClientView.PlaceUserJetton(ID_KONG_QUE,pStatusPlay->lALLKongQueScore);
			m_GameClientView.PlaceUserJetton(ID_HOU_ZI,pStatusPlay->lALLHouZiScore);
			m_GameClientView.PlaceUserJetton(ID_LAO_YING,pStatusPlay->lALLLaoYingScore);
			m_GameClientView.PlaceUserJetton(ID_SHI_ZI,pStatusPlay->lALLShiZiScore);

			//�����ע
			SetMePlaceJetton(ID_BIG_SHARK,pStatusPlay->lALLBigSharkScore);
			SetMePlaceJetton(ID_FEI_QIN,pStatusPlay->lALLFeiQinScore);
			SetMePlaceJetton(ID_ZOU_SHOU,pStatusPlay->lALLZouShouScore);
			SetMePlaceJetton(ID_YAN_ZI,pStatusPlay->lUserYanZiScore);
			SetMePlaceJetton(ID_TU_ZI,pStatusPlay->lUserTuZiScore);
			SetMePlaceJetton(ID_GE_ZI,pStatusPlay->lUserGeZiScore);
			SetMePlaceJetton(ID_XIONG_MAO,pStatusPlay->lUserXiongMaoScore);
			SetMePlaceJetton(ID_KONG_QUE,pStatusPlay->lUserKongQueScore);
			SetMePlaceJetton(ID_HOU_ZI,pStatusPlay->lUserHouZiScore);
			SetMePlaceJetton(ID_LAO_YING,pStatusPlay->lUserLaoYingScore);
			SetMePlaceJetton(ID_SHI_ZI,pStatusPlay->lUserShiZiScore);

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
				m_cbResultIndex = pStatusPlay->cbResultIndex;
				m_cbMoveEndIndex = pStatusPlay->cbMoveEndIndex;
				m_cbEndIndex = pStatusPlay->cbEndIndex;
				m_cbMoveIndex = m_cbMoveEndIndex+m_cbEndIndex+1;
				SetTimer(IDI_FLASH_WIN, 400, NULL);
				//���óɼ�
				m_GameClientView.m_ScoreView.SetGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore);
			}
			else
			{
				//��������
				//if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			}

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);
			m_GameClientView.SetGameState(pStatusPlay->cbGameStatus);
			//����ʱ��
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);
			if (pStatusPlay->cbUserStatus!=US_PLAY)
				return true;
			//���°�ť
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}
	return false;
}
//����ׯ��
void CGameClientDlg::SetBankerInfo(WORD wBanker,LONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
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
		LONG lLeaveScore=m_lMeMaxScore-m_lMeBigSharkScore-m_lMeFeiQinScore-m_lMeZouShouScore-m_lMeYanZiScore-m_lMeTuZiScore-m_lMeGeZiScore-m_lMeXiongMaoScore-m_lMeKongQueScore-
			m_lMeHouZiScore-m_lMeLaoYingScore-m_lMeShiZiScore;

		//���ù��
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=10000000L) m_GameClientView.SetCurrentJetton(10000000L);
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
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);
	}

	//���������ť
	//if (m_ApplyUser.GetCount()>3 && (m_cbApplyIndex+3)<m_ApplyUser.GetCount())
	//	m_GameClientView.m_btBanker_B.EnableWindow(TRUE);
	//else
	//	m_GameClientView.m_btBanker_B.EnableWindow(FALSE);
	//if (m_cbApplyIndex>0)
	//	m_GameClientView.m_btBanker_T.EnableWindow(TRUE);
	//else
	//	m_GameClientView.m_btBanker_T.EnableWindow(FALSE);
	return;
}

//������Ϣ
 LRESULT CGameClientDlg::OnScoreLeft(WPARAM wParam, LPARAM lParam)
 {
	return 0;
 }

 //������Ϣ
 LRESULT CGameClientDlg::OnScoreRight(WPARAM wParam, LPARAM lParam)
 {
	 return 0;
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
	 if (GetGameStatus()!=GS_PLAYING)
	 {
		 UpdateButtonContron();
		 return true;
	 }

 	 //���ñ���
 	switch (cbJettonArea)
 	 {
	 case ID_BIG_SHARK:
			{
				m_lMeBigSharkScore += lJettonScore;
				m_GameClientView.SetMePlaceJetton(ID_BIG_SHARK,m_lMeBigSharkScore);
				break;
			}
	 case ID_FEI_QIN:
		 {
			 m_lMeFeiQinScore += lJettonScore;
			 m_GameClientView.SetMePlaceJetton(ID_FEI_QIN,m_lMeFeiQinScore);
			 break;
		 }
	 case ID_ZOU_SHOU:
		 {
			 m_lMeZouShouScore += lJettonScore;
			 m_GameClientView.SetMePlaceJetton(ID_ZOU_SHOU,m_lMeZouShouScore);
			 break;
		 }
 	 case ID_YAN_ZI:
 			{
 				m_lMeYanZiScore += lJettonScore;
 				m_GameClientView.SetMePlaceJetton(ID_YAN_ZI,m_lMeYanZiScore);
 				break;
 			}
 	 case ID_TU_ZI:
 			{
 				m_lMeTuZiScore += lJettonScore;
 				m_GameClientView.SetMePlaceJetton(ID_TU_ZI,m_lMeTuZiScore);
 				break;
 			}
 	 case ID_GE_ZI:
 			{
 				m_lMeGeZiScore += lJettonScore;
 				m_GameClientView.SetMePlaceJetton(ID_GE_ZI,m_lMeGeZiScore);
 				break;
 			}
 	 case ID_XIONG_MAO:
 			{
 				m_lMeXiongMaoScore += lJettonScore;
 				m_GameClientView.SetMePlaceJetton(ID_XIONG_MAO,m_lMeXiongMaoScore);
 				break;
 			}
 	 case ID_KONG_QUE:
 			{
 				m_lMeKongQueScore += lJettonScore;
 				m_GameClientView.SetMePlaceJetton(ID_KONG_QUE,m_lMeKongQueScore);
 				break;
 			}
 	 case ID_HOU_ZI:
 			{
 				m_lMeHouZiScore += lJettonScore;
 				m_GameClientView.SetMePlaceJetton(ID_HOU_ZI,m_lMeHouZiScore);
 				break;
 			}
 	 case ID_LAO_YING:
 			{
 				m_lMeLaoYingScore += lJettonScore;
 				m_GameClientView.SetMePlaceJetton(ID_LAO_YING,m_lMeLaoYingScore);
 				break;
 			}
 	 case ID_SHI_ZI:
 			{
 				m_lMeShiZiScore += lJettonScore;
 				m_GameClientView.SetMePlaceJetton(ID_SHI_ZI,m_lMeShiZiScore);
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
		m_DirectSound[1].Play();
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

 //��ʱ����Ϣ
 void CGameClientDlg::OnTimer(UINT nIDEvent)
 {
	if (nIDEvent==IDI_MOVE_INDEX)
	{
		m_GameClientView.SetMoveIndex(m_cbMoveIndex);
		m_cbMoveIndex++;
		StartGameMove();
		if (IsEnableSound())
			m_DirectSound[5].Play();
		//m_GameClientView.UpdateGameView(NULL);
	}
	if (nIDEvent==IDI_SELECT_INDEX)
	{
		m_GameClientView.SetSelectIndex(0xff);
		KillTimer(IDI_SELECT_INDEX);
	}
	if (nIDEvent==IDI_FLASH_WIN)
	{
		if (m_bFlashFlags)
		{
			m_GameClientView.SetCanMove(m_bFlashFlags);
			m_GameClientView.SetMoveIndex(m_cbMoveIndex-1);
			m_GameClientView.SetSelectJettonIndex(m_cbResultIndex);

			m_bFlashFlags = !m_bFlashFlags;
		} 
		else
		{
			m_GameClientView.SetCanMove(m_bFlashFlags);
			m_GameClientView.SetMoveIndex(0xff);
			m_GameClientView.SetSelectJettonIndex(0xff);
			m_bFlashFlags = !m_bFlashFlags;
		}
	}
 }

//��ʼ��ת
void CGameClientDlg::StartGameMove()
{
	if (m_cbMoveIndex==0)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,500,NULL);
	}
	else if (m_cbMoveIndex==1) 
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,400,NULL);
	}
	else if (m_cbMoveIndex==2)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,300,NULL);
	}
	else if (m_cbMoveIndex==5)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,200,NULL);
	}
	else if (m_cbMoveIndex==7)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,100,NULL);
	}
	else if (m_cbMoveIndex==8)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,50,NULL);
	}
	else if (m_cbMoveIndex==m_cbMoveEndIndex-8)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,100,NULL);
	}
	else if (m_cbMoveIndex==m_cbMoveEndIndex-7)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,200,NULL);
	}
	else if (m_cbMoveIndex==m_cbMoveEndIndex-5)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,300,NULL);
	}
	else if (m_cbMoveIndex==m_cbMoveEndIndex-2)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,400,NULL);
	}
	else if (m_cbMoveIndex==m_cbMoveEndIndex-1)
	{
		KillTimer(IDI_MOVE_INDEX);
		SetTimer(IDI_MOVE_INDEX,500,NULL);
	}
	else if (m_cbMoveIndex==(m_cbMoveEndIndex+m_cbEndIndex+1))
	{
		
		//SetTimer(IDI_MOVE_INDEX,3000,NULL);
 		KillTimer(IDI_MOVE_INDEX);
//   		m_cbMoveIndex = 0;
//   		m_cbMoveEndIndex = 0;
//   		m_cbEndIndex = 0;
		m_bFlashFlags = true;
		SetTimer(IDI_FLASH_WIN, 400, NULL);
// 		m_GameClientView.SetMoveIndex(m_cbMoveIndex);
// 		m_GameClientView.SetSelectJettonIndex(m_cbResultIndex);
	}

}
//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	//ֹͣѡ�ж���
	m_GameClientView.SetSelectIndex(0xff);
	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
	//����״̬
	m_GameClientView.SetGameState(IDI_PLACE_JETTON);
	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	//���¿���
	UpdateButtonContron();

	//���½���
	m_GameClientView.UpdateGameView(NULL);
	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}
	//��������
 	if(IsEnableSound()) 
 	{
 		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
 		m_DirectSound[7].Play();
 	} 

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
	//ѡ��Ч��
	m_GameClientView.SetSelectIndex(pPlaceJetton->cbJettonArea-1);
	SetTimer(IDI_SELECT_INDEX,500,NULL);

	//��������
	if (IsEnableSound()) 
	{
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			m_DirectSound[1].Play();
		}
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
 		ASSERT(cbViewIndex<=ID_SHI_ZI);
 		if (cbViewIndex>ID_SHI_ZI) return false;
 	
 		LONG lJettonCount=pPlaceJettonFail->lPlaceScore;
 		if (cbViewIndex==ID_BIG_SHARK)
 		{
 			m_lMeBigSharkScore-=lJettonCount;
 			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeBigSharkScore);
 		}
		else if (cbViewIndex==ID_FEI_QIN)
		{
			m_lMeFeiQinScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeFeiQinScore);
		}
		else if (cbViewIndex==ID_ZOU_SHOU)
		{
			m_lMeZouShouScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeZouShouScore);
		}
		else if (cbViewIndex==ID_YAN_ZI)
		{
			m_lMeYanZiScore-=lJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeYanZiScore);
		}
		else if (cbViewIndex==ID_TU_ZI) 
 		{
 			m_lMeTuZiScore-=lJettonCount;
 			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeTuZiScore);
 		}
 		else if (cbViewIndex==ID_GE_ZI) 
 		{
 			m_lMeGeZiScore-=lJettonCount;
 			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeGeZiScore);
 		}
 		else if (cbViewIndex==ID_XIONG_MAO) 
 		{
 			m_lMeXiongMaoScore-=lJettonCount;
 			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeXiongMaoScore);
 		}
 		else if (cbViewIndex==ID_KONG_QUE) 
 		{
 			m_lMeKongQueScore-=lJettonCount;
 			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeKongQueScore);
 		}
 		else if (cbViewIndex==ID_HOU_ZI) 
 		{
 			m_lMeHouZiScore-=lJettonCount;
 			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeHouZiScore);
 		}
 		else if (cbViewIndex==ID_LAO_YING) 
 		{
 			m_lMeLaoYingScore-=lJettonCount;
 			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeLaoYingScore);
 		}
 		else if (cbViewIndex==ID_SHI_ZI) 
 		{
 			m_lMeShiZiScore-=lJettonCount;
 			m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeShiZiScore);
 		}
 		else {ASSERT(FALSE);return false;}

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
	
	m_cbResultIndex = pGameEnd->cbResultIndex;
	m_cbMoveEndIndex = pGameEnd->cbMoveEndIndex	;
	m_cbEndIndex = pGameEnd->cbEndIndex;
	
	//�����Լ���ע
	SetMePlaceJetton(ID_BIG_SHARK, pGameEnd->lUserBigSharkScore);
	SetMePlaceJetton(ID_FEI_QIN, pGameEnd->lUserFeiQinScore);
	SetMePlaceJetton(ID_ZOU_SHOU, pGameEnd->lUserZouShouScore);
	SetMePlaceJetton(ID_YAN_ZI, pGameEnd->lUserYanZiScore);
	SetMePlaceJetton(ID_TU_ZI,pGameEnd->lUserTuZiScore);
	SetMePlaceJetton(ID_GE_ZI,pGameEnd->lUserGeZiScore);
	SetMePlaceJetton(ID_XIONG_MAO,pGameEnd->lUserXiongMaoScore);
	SetMePlaceJetton(ID_KONG_QUE,pGameEnd->lUserKongQueScore);
	SetMePlaceJetton(ID_HOU_ZI,pGameEnd->lUserHouZiScore);
	SetMePlaceJetton(ID_LAO_YING,pGameEnd->lUserLaoYingScore);
	SetMePlaceJetton(ID_SHI_ZI,pGameEnd->lUserShiZiScore);

	//����ת������
	m_cbMoveIndex=0;
	m_GameClientView.SetCanMove(true);
	SetTimer(IDI_MOVE_INDEX,50,NULL);
	
	//������Ϣ
	m_lMeScore = pGameEnd->lUserScore;
	m_lBankerScore = pGameEnd->lBankerScore;

	//˰��
	m_lRevenue = pGameEnd->lRevenue;

	//����ʱ��
	m_GameClientView.SetGameState(GS_GAME_END);
	SetGameTimer(GetMeChairID(),IDI_DISPATCH, pGameEnd->cbTimeLeave);

	//ׯ����Ϣ
	m_wBankerTime = pGameEnd->nBankerTime;
	m_lTmpBankerWinScore = pGameEnd->lBankerTotallScore;

	//���˳ɼ�
	m_lMeStatisticScore += pGameEnd->lUserScore;
	//����״̬
	SetGameStatus(GS_GAME_END);
	m_GameClientView.SetSelectJettonIndex(0xff);
	//���³ɼ�
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;
	}

	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����
	m_DirectSound[0].Stop();
	m_DirectSound[1].Stop();
	m_DirectSound[2].Stop();
	m_DirectSound[3].Stop();
	m_DirectSound[4].Stop();
	m_DirectSound[5].Stop();
	m_DirectSound[6].Stop();
	m_DirectSound[7].Stop();

	//�����ɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd==NULL)
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE,&m_GameClientView);
	}
	//���óɼ�
	if (GetMeChairID() == m_wCurrentBanker)
	{
		if (pGameEnd->lBankerScore>0)
		{
			m_lMeEndScore = 0;//-(pGameEnd->lBankerScore+m_lRevenue);
			m_lBankerEndScore = pGameEnd->lBankerScore;
		}
		else
		{
			m_lMeEndScore = 0;//-(pGameEnd->lBankerScore-m_lRevenue);
			m_lBankerEndScore = pGameEnd->lBankerScore;
		}

		m_GameClientView.m_ScoreView.SetGameScore(m_lMeEndScore,0,m_lBankerEndScore);
	}
	else
	{
		m_lBankerEndScore = pGameEnd->lBankerScore;
		m_lMeEndScore = pGameEnd->lUserScore;
		m_GameClientView.m_ScoreView.SetGameScore(m_lMeEndScore,pGameEnd->lUserReturnScore,m_lBankerEndScore);
	}

	
	//��ʾ�ɼ�����
	CRect scoreRect, gameViewRect ;
	m_GameClientView.GetWindowRect( gameViewRect ) ;
	m_GameClientView.m_ScoreView.GetWindowRect( scoreRect ) ;
	m_GameClientView.m_ScoreView.MoveWindow(gameViewRect.left +(gameViewRect.Width()-scoreRect.Width())/2+30-300, 
		gameViewRect.top+(gameViewRect.Height()-scoreRect.Height())/2+30+285, scoreRect.Width(), scoreRect.Height()) ;
	
	
	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//�����ʱ��
	KillTimer(IDI_MOVE_INDEX);
	KillTimer(IDI_SELECT_INDEX);
	KillTimer(IDI_FLASH_WIN);
	m_GameClientView.SetMoveIndex(0xff);
	m_GameClientView.SetSelectJettonIndex(0xff);
	m_bFlashFlags = false;
	m_cbMoveEndIndex = 0;
	m_cbMoveIndex = 0;
	m_cbMoveEndIndex = 0;
	m_cbResultIndex = 0;
	m_GameClientView.SetCanMove(false);
	m_GameClientView.SetSelectIndex(0xff);
	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//����ʱ��
	SetGameTimer(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_FREE);

	//��������
	m_cbMoveIndex = 0;
	m_GameClientView.SetCanMove(false);
	m_GameClientView.CleanUserJetton();
	m_GameClientView.SetGameState(GS_FREE);
	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}
	for (int nAreaIndex=ID_BIG_SHARK; nAreaIndex<=ID_SHI_ZI; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//���¿ؼ�
	UpdateButtonContron();
	m_GameClientView.SetMeHisScore(m_lMeStatisticScore);
	m_DirectSound[0].Play();
	return true;
}

//������ע
 void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount)
 {
 	//Ч�����
 	//ASSERT(cbViewIndex<=ID_BANKER_TWO_PAIR);
 	if (cbViewIndex>ID_SHI_ZI) return;
 
	if (cbViewIndex==ID_BIG_SHARK) m_lMeBigSharkScore=lJettonCount;
	else if (cbViewIndex==ID_FEI_QIN) m_lMeFeiQinScore=lJettonCount;
	else if (cbViewIndex==ID_ZOU_SHOU) m_lMeZouShouScore=lJettonCount;
	else if (cbViewIndex==ID_YAN_ZI) m_lMeYanZiScore=lJettonCount;
	else if (cbViewIndex==ID_TU_ZI) m_lMeTuZiScore=lJettonCount;
 	else if (cbViewIndex==ID_GE_ZI) m_lMeGeZiScore=lJettonCount;
 	else if (cbViewIndex==ID_XIONG_MAO) m_lMeXiongMaoScore=lJettonCount;
 	else if (cbViewIndex==ID_KONG_QUE) m_lMeKongQueScore=lJettonCount;
 	else if (cbViewIndex==ID_HOU_ZI) m_lMeHouZiScore=lJettonCount;
 	else if (cbViewIndex==ID_LAO_YING) m_lMeLaoYingScore=lJettonCount;
 	else if (cbViewIndex==ID_SHI_ZI) m_lMeShiZiScore=lJettonCount;
 	else {ASSERT(FALSE);return;}
 	//���ý���
 	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
 }
//������ׯ
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	//if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;
	//
	////��Ϣ����
	//CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	////��ȡ���
	//tagUserData const *pUserData=GetUserData(pApplyBanker->wApplyUser);

	////�Լ��ж�
	//if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	////���¿ؼ�
	//UpdateButtonContron();

	return true;
}

//ȡ����ׯ
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	//if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	////��Ϣ����
	//CMD_S_ApplyBanker * pCancelyBanker=(CMD_S_ApplyBanker *)pBuffer;

	////��ȡ���
	//tagUserData const *pUserData=GetUserData(pCancelyBanker->wApplyUser);

	////ɾ�����
	//tagApplyUser ApplyUser;
	//ApplyUser.strUserName=pUserData->szName;
	//ApplyUser.lUserScore=pUserData->lScore;
	//m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	////m_cbApplyCount--;
	//
	//CString DelApplyUser;
	//DelApplyUser = pUserData->szName;
	//for (BYTE i=0; i<m_ApplyUser.GetCount();i++)
	//{
	//	if (m_ApplyUser[i].strUserName.Compare(DelApplyUser)==0)
	//		m_ApplyUser.RemoveAt(i);
	//}


	////�Լ��ж�
	//if (IsLookonMode()==false && GetMeChairID()==pCancelyBanker->wApplyUser) m_bMeApplyBanker=false;

	////���¿ؼ�
	//UpdateButtonContron();

	return true;
}

//�л�ׯ��
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	//ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	//if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	////��Ϣ����
	//CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	////��ʾͼƬ
	////m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	////�Լ��ж�
	//if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	//{
	//	m_bMeApplyBanker=false;
	//}
	//else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	//{
	//	m_bMeApplyBanker=true;
	//}

	////ׯ����
	//SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	//m_GameClientView.SetBankerScore(0,0);

	////ɾ�����
	//if (m_wCurrentBanker!=INVALID_CHAIR)
	//{
	//	tagUserData const *pBankerUserData=GetUserData(m_wCurrentBanker);
	//	if (pBankerUserData != NULL)
	//	{
	//		tagApplyUser ApplyUser;
	//		ApplyUser.strUserName = pBankerUserData->szName;
	//		m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);

	//		CString DelApplyUser;
	//		DelApplyUser = pBankerUserData->szName;
	//		for (BYTE i=0; i<m_ApplyUser.GetCount();i++)
	//		{
	//			if (m_ApplyUser[i].strUserName.Compare(DelApplyUser)==0)
	//				m_ApplyUser.RemoveAt(i);
	//		}
	//	}
	//}

	////���½���
	//UpdateButtonContron();
	//m_GameClientView.UpdateGameView(NULL);

	return true;
}

//�л�ׯ��
bool CGameClientDlg::OnSubChangeSysBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeSysBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeSysBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeSysBanker * pChangeBanker=(CMD_S_ChangeSysBanker *)pBuffer;


	//ׯ����
	SetBankerInfo(INVALID_CHAIR,0);
	m_GameClientView.SetBankerScore(pChangeBanker->cbBankerTime,0);
	m_bEnableSysBanker = pChangeBanker->bEnableSysBanker;
	m_wBankerTime = pChangeBanker->cbBankerTime;
	m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

	//���½���
	UpdateButtonContron();
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//��ʷ��¼
bool CGameClientDlg::OnSubSocreHistory(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ScoreHistory));
	if (wDataSize!=sizeof(CMD_S_ScoreHistory)) return false;

	//��Ϣ����
	CMD_S_ScoreHistory * pChangeBanker=(CMD_S_ScoreHistory *)pBuffer;
	CopyMemory(m_cbScoreHistroy, pChangeBanker->cbScoreHistroy, sizeof(m_cbScoreHistroy));

	m_GameClientView.SetScoreHistory(m_cbScoreHistroy);
	return true;
}


//����ɼ�
CString CGameClientDlg::DrawScoreString(LONG lNumber)
{
	CString strNumber=TEXT("");
	strNumber.Format(TEXT("%ld"),lNumber>0?lNumber:-lNumber);
	int p=strNumber.GetLength()-4;
	while(p>0)
	{
		strNumber.Insert(p,",");
		p-=4;
	}
	if(lNumber<0)
	{
		strNumber=TEXT("-")+strNumber;
	}
	return strNumber;
}

//������Ϣ
LRESULT CGameClientDlg::OnBank(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("�뵽��Ϸ�����д�ȡ");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
