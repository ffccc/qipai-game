#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//宏定义250
//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH				101									//转动时间
#define IDI_MOVE_TIMER				102									//开局动画
#define IDI_MOVE_INDEX				103									//动画索引
#define IDI_SELECT_INDEX			104									//选中状态
#define IDI_FLASH_WIN				105									//闪动
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

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//限制信息
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
	//庄家信息
	m_lBankerScore = 0L;
	m_wCurrentBanker = 0L;
	m_wBankerTime = 0;
	m_lTmpBankerWinScore = 0L;
	//个人信息
	m_lMeScore = 0L;
	//个人下注信息

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

	//全体下注
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
	//状态变量
	m_bMeApplyBanker=false;

	//申请上庄信息
	m_ApplyUser.RemoveAll();
	m_cbApplyCount = 0; 
	m_cbApplyIndex = 0;

	ZeroMemory(m_cbScoreHistroy, sizeof(m_cbScoreHistroy));

	//结束信息
	m_lBankerEndScore = 0;
	m_lMeEndScore = 0;
	m_lRevenue = 0;
	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{

	//设置标题
	SetWindowText(TEXT("大白鲨  --  金圭网络"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//初始化声音
	m_DirectSound[0].Create(TEXT("IDC_SND_BK"));			//背景声音
	m_DirectSound[1].Create(TEXT("IDC_SND_ADDCHIP"));		//下注声音
	m_DirectSound[2].Create(TEXT("IDC_SND_GAME_START"));	//开始声音
	m_DirectSound[3].Create(TEXT("IDC_SND_GAMELOST"));		//输声音
	m_DirectSound[4].Create(TEXT("IDC_SND_GAMEWIN"));		//赢声音
	m_DirectSound[5].Create(TEXT("IDC_SND_SELGRID"));		//选择区域声音
	m_DirectSound[6].Create(TEXT("IDC_SND_STARTSHOWCARD"));	//旋转提示
	m_DirectSound[7].Create(TEXT("IDC_BACK_GROUND"));		//下注背景

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

//初始函数
BOOL CGameClientDlg::OnInitDialog()
{
	__super::OnInitDialog();

	SetWindowPos(NULL,200,150,850,736,SWP_NOZORDER);
	CenterWindow();
	ModifyStyle(WS_THICKFRAME, CS_VREDRAW| CS_HREDRAW|DS_MODALFRAME, 0);
	RestoreWindow();

	return TRUE;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//限制信息
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	
	m_cbMoveIndex = 0;
	m_cbMoveEndIndex = 0;
	m_cbResultIndex = 0;
	m_cbEndIndex = 0;
	m_lMeCanUseScore = 0L;
	m_bFlashFlags = false;
	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	//个人信息
	m_lMeScore = 0L;
	//状态变量
	m_bMeApplyBanker=false;
	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//时间消息
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
				//播放声音
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

//网络消息
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_SCORE_HISTORY:	//历史消息
		{
			return OnSubSocreHistory(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_SYS_BANKER://切换庄家
		{
			return OnSubChangeSysBanker(pBuffer, wDataSize);
		}
	}
	return false;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//设置时间
			SetGameTimer(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);
			//SetGameTimer(GetMeChairID(),IDI_FREE,24);
			//玩家信息
			m_lMeMaxScore=pStatusFree->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));
			//庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);

			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//设置状态
			SetGameStatus(GS_FREE);
			//更新控制
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);
			m_DirectSound[0].Play();
			return true;		
		}
	case GS_PLAYING:		//游戏状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//全局下注
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

			//玩家下注
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

			//玩家积分
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//控制信息
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
				//设置成绩
				m_GameClientView.m_ScoreView.SetGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore);
			}
			else
			{
				//播放声音
				//if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			}

			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);
			m_GameClientView.SetGameState(pStatusPlay->cbGameStatus);
			//设置时间
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);
			if (pStatusPlay->cbUserStatus!=US_PLAY)
				return true;
			//更新按钮
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}
	return false;
}
//设置庄家
void CGameClientDlg::SetBankerInfo(WORD wBanker,LONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
}
//更新控制
void CGameClientDlg::UpdateButtonContron()
{
	//置能判断
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) bEnablePlaceJetton=false;
	if (GetGameStatus()!=GS_PLACE_JETTON) bEnablePlaceJetton=false;
	if (m_wCurrentBanker==GetMeChairID()) bEnablePlaceJetton=false;
	if (IsLookonMode()) bEnablePlaceJetton=false;

	//下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算积分
		LONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONG lLeaveScore=m_lMeMaxScore-m_lMeBigSharkScore-m_lMeFeiQinScore-m_lMeZouShouScore-m_lMeYanZiScore-m_lMeTuZiScore-m_lMeGeZiScore-m_lMeXiongMaoScore-m_lMeKongQueScore-
			m_lMeHouZiScore-m_lMeLaoYingScore-m_lMeShiZiScore;

		//设置光标
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

		//控制按钮
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton10000000.EnableWindow((lLeaveScore>=10000000)?TRUE:FALSE);
	}
	else
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);
	}

	//申请滚动按钮
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

//滚动消息
 LRESULT CGameClientDlg::OnScoreLeft(WPARAM wParam, LPARAM lParam)
 {
	return 0;
 }

 //滚动消息
 LRESULT CGameClientDlg::OnScoreRight(WPARAM wParam, LPARAM lParam)
 {
	 return 0;
 }


 //加注消息
 LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
 {
	 //变量定义
	BYTE cbJettonArea=(BYTE)wParam;
	LONG lJettonScore=(LONG)lParam;

	 //庄家判断
	 if ( GetMeChairID() == m_wCurrentBanker ) return true;

	 //状态判断
	 if (GetGameStatus()!=GS_PLAYING)
	 {
		 UpdateButtonContron();
		 return true;
	 }

 	 //设置变量
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

	 //变量定义
	 CMD_C_PlaceJetton PlaceJetton;
	 ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	 //构造变量
	 PlaceJetton.cbJettonArea=cbJettonArea;
	 PlaceJetton.lJettonScore=lJettonScore;

	 //发送消息
	 SendData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	 //更新按钮
	 UpdateButtonContron();

	 //播放声音
	 if (IsEnableSound()) 
	 {
		m_DirectSound[1].Play();
	 }

	 return 0;
 }

 //申请消息
 LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
 {
	 //合法判断
	 tagUserData const *pMeUserData = GetUserData( GetMeChairID() );
	 if (pMeUserData->lScore < m_lApplyBankerCondition) return true;

	 //旁观判断
	 if (IsLookonMode()) return true;

	 //转换变量
	 bool bApplyBanker = wParam ? true:false;

	 //当前判断
	 if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;

	 if (bApplyBanker)
	 {
		 //发送消息
		 SendData(SUB_C_APPLY_BANKER, NULL, 0);

		 m_bMeApplyBanker=true;
	 }
	 else
	 {
		 //发送消息
		 SendData(SUB_C_CANCEL_BANKER, NULL, 0);

		 m_bMeApplyBanker=false;
	 }

	 //设置按钮
	 UpdateButtonContron();

	 return true;
 }

 //定时器消息
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

//开始旋转
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
//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	//停止选中动画
	m_GameClientView.SetSelectIndex(0xff);
	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
	//设置状态
	m_GameClientView.SetGameState(IDI_PLACE_JETTON);
	//设置时间
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//设置状态
	SetGameStatus(GS_PLACE_JETTON);

	//更新控制
	UpdateButtonContron();

	//更新界面
	m_GameClientView.UpdateGameView(NULL);
	//关闭成绩
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}
	//播放声音
 	if(IsEnableSound()) 
 	{
 		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
 		m_DirectSound[7].Play();
 	} 

	return true;
}

//用户加注
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	//加注界面
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
	//选中效果
	m_GameClientView.SetSelectIndex(pPlaceJetton->cbJettonArea-1);
	SetTimer(IDI_SELECT_INDEX,500,NULL);

	//播放声音
	if (IsEnableSound()) 
	{
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			m_DirectSound[1].Play();
		}
	}

	return true;
}

//下注失败
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
		//效验数据
 		ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
 		if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;
 	
 		//消息处理
 		CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

 		//效验参数
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
//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	
	m_cbResultIndex = pGameEnd->cbResultIndex;
	m_cbMoveEndIndex = pGameEnd->cbMoveEndIndex	;
	m_cbEndIndex = pGameEnd->cbEndIndex;
	
	//更新自己下注
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

	//开启转动动画
	m_cbMoveIndex=0;
	m_GameClientView.SetCanMove(true);
	SetTimer(IDI_MOVE_INDEX,50,NULL);
	
	//积分信息
	m_lMeScore = pGameEnd->lUserScore;
	m_lBankerScore = pGameEnd->lBankerScore;

	//税收
	m_lRevenue = pGameEnd->lRevenue;

	//设置时间
	m_GameClientView.SetGameState(GS_GAME_END);
	SetGameTimer(GetMeChairID(),IDI_DISPATCH, pGameEnd->cbTimeLeave);

	//庄家信息
	m_wBankerTime = pGameEnd->nBankerTime;
	m_lTmpBankerWinScore = pGameEnd->lBankerTotallScore;

	//个人成绩
	m_lMeStatisticScore += pGameEnd->lUserScore;
	//设置状态
	SetGameStatus(GS_GAME_END);
	m_GameClientView.SetSelectJettonIndex(0xff);
	//更新成绩
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;
	}

	//更新控件
	UpdateButtonContron();

	//停止声音
	m_DirectSound[0].Stop();
	m_DirectSound[1].Stop();
	m_DirectSound[2].Stop();
	m_DirectSound[3].Stop();
	m_DirectSound[4].Stop();
	m_DirectSound[5].Stop();
	m_DirectSound[6].Stop();
	m_DirectSound[7].Stop();

	//创建成绩
	if (m_GameClientView.m_ScoreView.m_hWnd==NULL)
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE,&m_GameClientView);
	}
	//设置成绩
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

	
	//显示成绩窗体
	CRect scoreRect, gameViewRect ;
	m_GameClientView.GetWindowRect( gameViewRect ) ;
	m_GameClientView.m_ScoreView.GetWindowRect( scoreRect ) ;
	m_GameClientView.m_ScoreView.MoveWindow(gameViewRect.left +(gameViewRect.Width()-scoreRect.Width())/2+30-300, 
		gameViewRect.top+(gameViewRect.Height()-scoreRect.Height())/2+30+285, scoreRect.Width(), scoreRect.Height()) ;
	
	
	return true;
}

//游戏空闲
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//清除定时器
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
	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//设置状态
	SetGameStatus(GS_FREE);

	//清理桌面
	m_cbMoveIndex = 0;
	m_GameClientView.SetCanMove(false);
	m_GameClientView.CleanUserJetton();
	m_GameClientView.SetGameState(GS_FREE);
	//关闭成绩
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}
	for (int nAreaIndex=ID_BIG_SHARK; nAreaIndex<=ID_SHI_ZI; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//更新控件
	UpdateButtonContron();
	m_GameClientView.SetMeHisScore(m_lMeStatisticScore);
	m_DirectSound[0].Play();
	return true;
}

//个人下注
 void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, LONG lJettonCount)
 {
 	//效验参数
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
 	//设置界面
 	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
 }
//申请做庄
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	////效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	//if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;
	//
	////消息处理
	//CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	////获取玩家
	//tagUserData const *pUserData=GetUserData(pApplyBanker->wApplyUser);

	////自己判断
	//if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	////更新控件
	//UpdateButtonContron();

	return true;
}

//取消做庄
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	////效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	//if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	////消息处理
	//CMD_S_ApplyBanker * pCancelyBanker=(CMD_S_ApplyBanker *)pBuffer;

	////获取玩家
	//tagUserData const *pUserData=GetUserData(pCancelyBanker->wApplyUser);

	////删除玩家
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


	////自己判断
	//if (IsLookonMode()==false && GetMeChairID()==pCancelyBanker->wApplyUser) m_bMeApplyBanker=false;

	////更新控件
	//UpdateButtonContron();

	return true;
}

//切换庄家
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	////效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	//if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	////消息处理
	//CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	////显示图片
	////m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	////自己判断
	//if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	//{
	//	m_bMeApplyBanker=false;
	//}
	//else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	//{
	//	m_bMeApplyBanker=true;
	//}

	////庄家信
	//SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	//m_GameClientView.SetBankerScore(0,0);

	////删除玩家
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

	////更新界面
	//UpdateButtonContron();
	//m_GameClientView.UpdateGameView(NULL);

	return true;
}

//切换庄家
bool CGameClientDlg::OnSubChangeSysBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeSysBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeSysBanker)) return false;

	//消息处理
	CMD_S_ChangeSysBanker * pChangeBanker=(CMD_S_ChangeSysBanker *)pBuffer;


	//庄家信
	SetBankerInfo(INVALID_CHAIR,0);
	m_GameClientView.SetBankerScore(pChangeBanker->cbBankerTime,0);
	m_bEnableSysBanker = pChangeBanker->bEnableSysBanker;
	m_wBankerTime = pChangeBanker->cbBankerTime;
	m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

	//更新界面
	UpdateButtonContron();
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//历史记录
bool CGameClientDlg::OnSubSocreHistory(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ScoreHistory));
	if (wDataSize!=sizeof(CMD_S_ScoreHistory)) return false;

	//消息处理
	CMD_S_ScoreHistory * pChangeBanker=(CMD_S_ScoreHistory *)pBuffer;
	CopyMemory(m_cbScoreHistroy, pChangeBanker->cbScoreHistroy, sizeof(m_cbScoreHistroy));

	m_GameClientView.SetScoreHistory(m_cbScoreHistroy);
	return true;
}


//输出成绩
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

//银行消息
LRESULT CGameClientDlg::OnBank(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("请到游戏房间中存取");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
