#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//导入库说明
#pragma comment(lib,"Version")

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_CHANGE_SICBO_TYPE, OnChangeSicboType)
	ON_MESSAGE(IDM_OPEN_BANK, OnOpenBank)
	ON_MESSAGE(IDM_USE_WINNER, OnUseWinner)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//限制信息
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;
	m_lApplyBankerCondition=0L;	

	//个人下注
	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));
	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//状态变量
	m_bMeApplyBanker=false;
	m_bShowBtnSicbo = false;

	m_bJoinInFree=true;

	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{
	//获取信息
	//TCHAR szExeName[_MAX_PATH];
	//_sntprintf(szExeName, sizeof(szExeName), _T("%s.exe"), AfxGetApp()->m_pszExeName);
	//OUTPUT_DEBUG_STRING(szExeName);
	//VS_FIXEDFILEINFO	*pFixedFileInfo=NULL;					//文件信息
	//DWORD dwLangCharset = 0;
	//BYTE byInfoBuffer[4096];
	//memset(byInfoBuffer,0,sizeof(byInfoBuffer));
	TCHAR szVerInfo[100]={_T("骰宝游戏  --  Ver：1.0.0.0")};
	//do 
	//{
	//	//打开文件
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

	//	// 文件版本
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
	//	_sntprintf(szVerInfo, sizeof(szVerInfo), _T("骰宝游戏  --  Ver：%u.%u.%u.%u"), nVer[0],nVer[1],nVer[2],nVer[3]);
	//} while (0);

	// 设置标题
	SetWindowText(szVerInfo);

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载声音
	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));
 
	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//限制信息
	m_lMeMaxScore=0L;			

	//个人下注
	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

	//庄家信息
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;

	//状态变量
	m_bMeApplyBanker=false;
	m_bShowBtnSicbo = false;

	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	return ;
}

//时间消息
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);


		//庄家按钮
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

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//网络消息
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			m_bJoinInFree=true;
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			m_bJoinInFree=true;
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
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
	case SUB_S_GAME_END:		//游戏结束
		{
			m_bJoinInFree=true;
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//游戏记录
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	case SUB_C_CHANGE_SICBO_TYPE:
		{
			return OnSubChangeSicboType(pBuffer,wDataSize);
		}
	default:break;
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	IClientKernel *pIClientKernel = ( IClientKernel * )GetClientKernel( IID_IClientKernel, VER_IClientKernel );
	pServerAttribute  = pIClientKernel->GetServerAttribute();

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

            //玩家信息
			m_lMeMaxScore=pStatusFree->lUserMaxScore;

			m_GameClientView.SetGameMaxScore(m_lMeMaxScore, 0);
			WORD wMeChairID=GetMeChairID();
		
			WORD wSwitchViewChairID=SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wSwitchViewChairID);
			
            //庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			// 直接将庄家金币值赋值给视图
			m_GameClientView.m_lBankerWinScore = pStatusFree->lBankerWinScore;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //控制信息
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//设置状态
			SetGameStatus(GS_FREE);

			m_bJoinInFree=true;
			//更新控制
			UpdateButtonContron(m_bJoinInFree);
			m_GameClientView.UpdateGameView(NULL);
			
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

			// 绘制下注数字			
			for (BYTE i=0; i<COUNT_AZIMUTH; ++i)
			{
				//全局下注
				m_GameClientView.PlaceUserJetton(i,pStatusPlay->lAllScore[i]);

				//玩家下注
				SetMePlaceJetton(i,pStatusPlay->lUserScore[i]);
			}

			//玩家积分
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;
			m_GameClientView.SetGameMaxScore(m_lMeMaxScore, pStatusPlay->lAllMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));
			m_GameClientView.m_enSicboType = (E_SICBO_TYPE)pStatusPlay->bySicboType;
			//控制信息
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//扑克信息
				m_GameClientView.SetCardInfo(pStatusPlay->enCards, pStatusPlay->bWinner);
				m_GameClientView.FinishDispatchCard();
				//设置成绩
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				// 直接将庄家金币值赋值给视图
				m_GameClientView.m_lBankerWinScore = pStatusPlay->lBankerWinScore;
				m_GameClientView.SetCardInfo(NULL, NULL);

				//播放声音
				if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			}

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);

			//设置时间
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			m_bJoinInFree=false;
			//更新按钮
			UpdateButtonContron(m_bJoinInFree);
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}

	return false;
}

//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	ZeroMemory(m_lMeScore, sizeof(m_lMeScore));
	ZeroMemory(m_lAllScore, sizeof(m_lAllScore));

	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//玩家信息
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetGameMaxScore(m_lMeMaxScore, pGameStart->lAllMaxScore);

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//设置状态
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
	//更新控制
	UpdateButtonContron(m_bJoinInFree);

	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	//播放声音
	if (IsEnableSound()) 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
		m_DTSDBackground.Play(0,true);
	}

	return true;
}

//游戏空闲
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//设置状态
	SetGameStatus(GS_FREE);

	//清理桌面
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=0; nAreaIndex<COUNT_AZIMUTH; ++nAreaIndex)
	{
		SetMePlaceJetton(nAreaIndex,0);
	}

	//更新控件
	UpdateButtonContron(m_bJoinInFree);

	//完成发牌
	//m_GameClientView.FinishDispatchCard();

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
	if ((pPlaceJetton->cbJettonArea>=COUNT_AZIMUTH) || (pPlaceJetton->lJettonScore<0))
	{
		return true;
	}

	// 更新玩家下注界面
	m_lAllScore[pPlaceJetton->cbJettonArea] += pPlaceJetton->lJettonScore;

	//加注界面
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

	//更新控件
	UpdateButtonContron(m_bJoinInFree);

	// 播放声音
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

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//取消动画
	for (int i=0; i<COUNT_AZIMUTH; ++i)
	{
		m_GameClientView.SetBombEffect(false,i);
	}

	// 设置时间
	SetGameTimer(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	// 摇骰子
	m_GameClientView.SetCardInfo(pGameEnd->enCards, pGameEnd->bWinner);

	//庄家信息
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//成绩信息
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	// 设置状态
	SetGameStatus(GS_GAME_END);

	//更新成绩
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		tagUserData const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;

		//更新信息
		ApplyUser.lUserScore = pUserData->lScore;
		ApplyUser.strUserName = pUserData->szName;
		m_GameClientView.UpdateApplyBanker(ApplyUser);
	}

	//更新控件
	UpdateButtonContron(m_bJoinInFree);

	//停止声音
	for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//更新控制
void CGameClientDlg::UpdateButtonContron(bool bEnablePlaceJetton)
{
	//置能判断
	//bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) bEnablePlaceJetton=false;
	if (GetGameStatus()!=GS_PLACE_JETTON) bEnablePlaceJetton=false;
	if (m_wCurrentBanker==GetMeChairID())
	{
		bEnablePlaceJetton=false;
		if (!IsLookonMode() && (GetGameStatus() ==GS_PLACE_JETTON)
			&& m_bShowBtnSicbo)  // 已经选择过了则不再显示按钮
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

	//下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算积分
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

		//设置光标
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

		//控制按钮
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
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton500000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
	}

	//庄家按钮
	if (!IsLookonMode())
	{
		//获取信息
		const tagUserData *pMeUserData=GetUserData(GetMeChairID());

		//申请按钮
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->lScore<m_lApplyBankerCondition) bEnableApply=false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//取消按钮
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

		//显示判断
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

//加注消息
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	if(this->IsFreeze())
		return false;

	//变量定义
	BYTE cbJettonArea=(BYTE)wParam;
	__int64 lJettonScore=(__int64)lParam;

	//庄家判断
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	if (IsLookonMode() || lJettonScore<0)
	{
		return true;
	}

	//状态判断
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron(m_bJoinInFree);
		return true;
	}

	//设置变量
	if (cbJettonArea<COUNT_AZIMUTH)
	{
		m_lMeScore[cbJettonArea] += lJettonScore;
		m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lMeScore[cbJettonArea]);
	}

	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	// 构造变量
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//发送消息
	SendData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//更新按钮
	UpdateButtonContron(m_bJoinInFree);

	//播放声音
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

//申请消息
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{

	if(this->IsFreeze())
		return false;
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
	UpdateButtonContron(m_bJoinInFree);

	return true;
}

// 选择摇骰子方式
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
//申请做庄
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//获取玩家
	tagUserData const *pUserData=GetUserData(pApplyBanker->wApplyUser);

	//插入玩家
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName=pUserData->szName;
		ApplyUser.lUserScore=pUserData->lScore;
		m_GameClientView.AddApplyBanker(ApplyUser);
	}

	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;

	//更新控件
	UpdateButtonContron(m_bJoinInFree);

	return true;
}

//取消做庄
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//删除玩家
	tagApplyUser ApplyUser;
	ApplyUser.strUserName=pCancelBanker->szCancelUser;
	ApplyUser.lUserScore=0;
	m_GameClientView.CancelApplyBanker(ApplyUser);

	//自己判断
	const tagUserData *pMeUserData=GetUserData(GetMeChairID());
	if (IsLookonMode()==false && lstrcmp(pMeUserData->szName,pCancelBanker->szCancelUser)==0) m_bMeApplyBanker=false;

	//更新控件
	UpdateButtonContron(m_bJoinInFree);

	return true;
}

//切换庄家
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//显示图片
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//自己判断
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	// 庄家信
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//删除玩家
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

	//更新界面
	UpdateButtonContron(m_bJoinInFree);
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//游戏记录
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//设置记录
	tagClientGameRecord ClientGameRecord;
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);	
		int iCardDot =0;
		for (BYTE i=0; i<MAX_COUNT_SICBO; ++i)
		{
			//OUTPUT_DEBUG_STRING("client 接收到的点数 %d", pServerGameRecord->enCards[i]);
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
// 选择摇骰子方式
bool CGameClientDlg::OnSubChangeSicboType(const void * pBuffer, WORD wDataSize)
{
	//效验数据
		ASSERT(wDataSize==sizeof(CMD_C_ChangeSicboType));
	if (wDataSize!=sizeof(CMD_C_ChangeSicboType)) return false;

	//消息处理
	CMD_C_ChangeSicboType * pChangeSicboType=(CMD_C_ChangeSicboType *)pBuffer;
	if ((pChangeSicboType->enSicboType <enSicboType_Desktop)
		|| (pChangeSicboType->enSicboType > enSicboType_InTheAir))
	{
		return true;
	}
	m_GameClientView.SetSicboType(pChangeSicboType->enSicboType);
	return true;
}
//设置庄家
void CGameClientDlg::SetBankerInfo(WORD wBanker,__int64 lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
}

//个人下注
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, __int64 lJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<COUNT_AZIMUTH);
	if (cbViewIndex>=COUNT_AZIMUTH) return;
	m_lMeScore[cbViewIndex] = lJettonCount;

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}

// 胜负控制
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
			ShowInformation(TEXT("对不起，您的帐户已被冻结，不能进行此操作！\n\n请与管理员联系。"),0,MB_ICONINFORMATION);
			return true;
		}
	}*/
	return false;
}
