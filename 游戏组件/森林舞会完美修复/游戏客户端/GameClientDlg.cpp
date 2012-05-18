#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "Path.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
//下注时间
#define IDI_BET_TIME_OVER			1									//下注计时器
#define IDI_BET_TIME_SEND			2									//下注计时器
#define IDI_GAME_END				3									//结束计时器
 
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_CANCEL_FANG,OnFang)
	ON_MESSAGE(IDM_SEND_CARD_CARTOON_FINISH, OnDealAnimationFinish)
	ON_MESSAGE(IDM_INSERT_SYSTEM_STRING, OnInsertSystemString)
	ON_MESSAGE(IDM_PLAY_SOUND, OnPlaySound)
END_MESSAGE_MAP()
CGameClientDlg *pGlobalDlg = NULL;
//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	pGlobalDlg = this;
	//AllocConsole();
	//freopen("CON","wt",stdout);
	//下注信息
	m_lMyMaxStake=0L;

	//历史信息
	m_wMyWinCount = 0;
	m_wMyLossCount = 0;
	m_lMyWinAmountSum=0;
	m_sound=true;
    m_lApplyBankerCondition=0L;
	m_wCurrentBanker=INVALID_CHAIR;
	m_wApplyCountMax=0;

	//扑克信息


	//SetMaxClient(TRUE);
//	m_iGameSerial = KIND_ID;

	m_strSoundPath = g_pPath->StripFileExtension(g_pPath->GetPathExecutable()) + "\\Sound\\";

	m_GameSound.Init(m_hWnd);
	m_GameSound.Load(m_strSoundPath + SOUND_ADD_GOLD, 1, true);
	m_GameSound.Load(m_strSoundPath + SOUND_GAME_END, 1, true);
	m_GameSound.Load(m_strSoundPath + SOUND_PLACE_JETTON, 1, true);

	m_GameSound.Load(m_strSoundPath + SOUND_STOP_JETTON, 1, true);
	m_GameSound.Load(m_strSoundPath + SOUND_WIN, 1, true);
	m_GameSound.Load(m_strSoundPath + SOUND_FREE, 1, true);
	CString temp;

	for (WORD i=0; i<4; i++)
	{
		temp.Format("%splace_jetton_%d.wav", m_strSoundPath.data(), i);
		m_GameSound.Load(temp.GetBuffer(), 1, true);
		m_GameSound.SetLoop(temp.GetBuffer(), true);
	}
	//SetFullWindow(true);
	//SetFullWindowSize(1024,738);
	m_GameSound.SetVolumeByGroup(1, 255);




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
	SetWindowText(TEXT("森林舞会游戏"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	SetMainWnd(this->m_GameClientView.GetSafeHwnd());
	CRect rc(0,0,1024,768);
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	bool bRes = Init(740, 730, rc.Width(), rc.Height(), "H_SLWH", GAME_NAME);
	m_SkinAttribute.m_TitleLeft.SetLoadInfo("H_SLWH\\configs\\UP.bmp",false);
	//m_SkinFrameAttribute.m_imgBorderRight.SetLoadInfo("H_SLWH\\configs\\RIGHT.bmp",false);
	//m_SkinFrameAttribute.m_imgBorderLeft.SetLoadInfo("H_SLWH\\configs\\LEFT.bmp",false);
	//m_SkinFrameAttribute.m_imgBorderBottom.SetLoadInfo("H_SLWH\\configs\\DOWN.bmp",false);

	//SetWindowPosition(enWindowTypeOpenDlg,1,0,0,0,0,false);
	//SetWindowPosition(enWindowTypeOpenDlg,2,0,0,0,0,false);
	//SetWindowPosition(enWindowTypeOpenDlg,3,0,0,0,0,false);
	//SetWindowPosition(enWindowTypeOpenDlg,4,0,0,0,0,false);
	//SetWindowPosition(enWindowTypeOpenDlg,5,0,0,0,0,false);

	return true;
}

bool CGameClientDlg::OnInit()
{
	//创建主框架窗口，游戏信息类
	m_pMainGameWnd = &this->m_GameClientView;
	m_GameClientView.Create(CRect(0,0,740,730), NULL);

	return true;
}


//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//下注信息
	m_lMyMaxStake=0L;

	//历史信息
	m_wMyWinCount = 0;
	m_wMyLossCount = 0;
	m_lMyWinAmountSum=0;

	//庄家信息
	m_lApplyBankerCondition = MAX_SCORE;
	m_wCurrentBanker = INVALID_CHAIR;

	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{

}

//时间消息
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{


			m_GameClientView.UpdateGameView(NULL);
	return true;
}

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{

	for (int i=CountArray(m_GameClientView.m_btnJettons)-1; i>=0; i--)
	{
		m_GameClientView.m_btnJettons[i].EnableWindow(false);
	}
}
//时间状态
bool CGameClientDlg::OnSubTimerType(const void * pBuffer, WORD wDataSize)
{

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_TimerTyper));
	if (wDataSize!=sizeof(CMD_S_TimerTyper)) return false;
	//消息处理
	CMD_S_TimerTyper * pTimerType=(CMD_S_TimerTyper *)pBuffer;
    m_GameClientView.m_timenum=pTimerType->Timernum;
	m_GameClientView.m_timezt=pTimerType->TimerType;
	if (pTimerType->Timernum==TIME_PLACE_JETTON&&pTimerType->TimerType==IDI_BET_TIME_OVER)
	{

		if (pGlobalDlg->IsEnableSound())
		{
			pGlobalDlg->PlayGameSound("H_SLWH\\Sound\\stop_jetton.wav");
		}

		m_GameClientView.m_endsrok2=false;

		//更新上庄控制
		UpdateBankerButtons();

		//更新控制
		UpdateJettonButtons();
        m_cbCardCount=0;


        m_GameClientView.m_lMywin=0;
		m_GameClientView.m_lBankerwin=0;
		m_GameClientView.SetMyMaxScore(m_GameClientView.m_MyGame);

	}

	if (pTimerType->Timernum==0&&pTimerType->TimerType==IDI_BET_TIME_OVER)
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);
		//禁止筹码选择按钮
		m_GameClientView.SetMyMaxScore(0);
	}

	if (pTimerType->Timernum==TIME_GAME_SENDPAI&&pTimerType->TimerType==IDI_BET_TIME_SEND)
	{
		m_GameClientView.Animation(false);
	}

	if (pTimerType->Timernum==0&&pTimerType->TimerType==IDI_BET_TIME_SEND)
	{

		m_GameClientView.Animation(true);
		if (m_GameClientView.m_iSMyWin)
		{
			m_GameClientView.CloseWin();
		}
				
		m_GameClientView.m_bDrawWinArea=false;
		m_GameClientView.KillTimerFLASH();
	
		m_GameClientView.m_endsrok=false;
		m_GameClientView.m_endsrok2=false;
		m_GameClientView.CleanUserJetton();

	}

	m_GameClientView.UpdateGameView(NULL);
	return true;
}
//网络消息
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_PLACE_TIMERTYPE:  //时间状态
		{
			return OnSubTimerType(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnSubApplyBanker( pBuffer, wDataSize );
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnSubChangeBanker( pBuffer, wDataSize );
		}
	case SUB_S_CHANGE_USER_SCORE://更新积分
		{
			return OnSubChangeApplyScore( pBuffer, wDataSize );
		}
	case SUB_S_SUPPERUSER:
		{
			return OnSupperuser(pBuffer, wDataSize);
		}
	case SUB_S_GAME:
		{
			return OnGAME(pBuffer, wDataSize);
		}
	case SUB_S_PLU:
		{
			return OnPLU(pBuffer, wDataSize);
		}
	}
	//错误断言
	ASSERT(FALSE);

	return true;
}
bool CGameClientDlg::OnPLU( const void * pBuffer, WORD wDataSize )
{
	CMD_S_GamePLU *cbPLU;
	cbPLU=( CMD_S_GamePLU *)pBuffer;
    CopyMemory(m_GameClientView.m_showT,cbPLU->cbCardColoct,sizeof(cbPLU->cbCardColoct));
    CopyMemory(m_GameClientView.m_showPL,cbPLU->cbCardCount,sizeof(cbPLU->cbCardCount));


    m_GameClientView.m_showForT=true;

	m_GameClientView.StartHyMovie();
	return true;
}
bool CGameClientDlg::OnGAME( const void * pBuffer, WORD wDataSize )
{
	CMD_S_Game *cbGAME;
	cbGAME=( CMD_S_Game *)pBuffer;

	if (cbGAME->GameBi>0)
	{
		m_GameClientView.m_MyGame=cbGAME->GameBi;
	}

	if (m_GameClientView.m_timezt==IDI_BET_TIME_OVER)
	{
		m_GameClientView.SetMyMaxScore(m_GameClientView.m_MyGame);
	}

    
	return true;
}
bool CGameClientDlg::OnSupperuser( const void * pBuffer, WORD wDataSize )
{
	CMD_S_SUPPERUSER *cbSupper;
	cbSupper=( CMD_S_SUPPERUSER *)pBuffer;

	if (cbSupper->supuser)
	{
		m_GameClientView.m_supper=true;

		for (WORD i=0; i<8; i++)
		{
			m_GameClientView.m_btnsupper[i].EnableWindow(true);
			m_GameClientView.m_btnsupper[i].Show(true);
		}
	}

	return true;
}


//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:			//空闲状态
		{

			//效验数据
 
			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			//庄家变量
			m_lApplyBankerCondition = pStatusFree->lApplyBankerCondition;
			m_wApplyCountMax = pStatusFree->wApplyCountMax;
			m_wCurrentBanker = pStatusFree->wCurrentBankerChairID;


     		//设置位置
			WORD wMyChairID=GetMeChairID();
			m_GameClientView.SetMyChairID(SwitchViewChairID(wMyChairID));
			m_GameClientView.m_GameRecord.SetHistoryResult(pStatusFree->HistoryInfo, pStatusFree->wHistoryCount);
			m_GameClientView.m_MyGame=pStatusFree->lMyGame;
			CopyMemory(m_GameClientView.m_lMyStakes,pStatusFree->lMyStakes,sizeof(pStatusFree->lMyStakes));
			CopyMemory(m_GameClientView.m_lAllStakes,pStatusFree->lAllStakes,sizeof(pStatusFree->lAllStakes));
           m_GameClientView.SetMyMaxScore(0);
			CopyMemory(m_GameClientView.m_showT,pStatusFree->cbCardColoct,sizeof(pStatusFree->cbCardColoct));
			CopyMemory(m_GameClientView.m_showPL,pStatusFree->cbCardCount,sizeof(pStatusFree->cbCardCount));
			m_GameClientView.m_Prizemoney = pStatusFree->cdPrizemoney;
			//庄家信息
			if ( pStatusFree->wCurrentBankerChairID == INVALID_CHAIR )
				m_GameClientView.SetBankerInfo(INVALID_CHAIR, pStatusFree->wBankerCount,0, pStatusFree->lCurrentBankerScore);
			else
				m_GameClientView.SetBankerInfo(SwitchViewChairID( pStatusFree->wCurrentBankerChairID ), pStatusFree->wBankerCount,0, pStatusFree->lCurrentBankerScore );

			return true;
		}
	case GS_PLAYING:		//游戏状态
		{

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//设置位置
			WORD wMyChairID=GetMeChairID();
			m_GameClientView.SetMyChairID(SwitchViewChairID(wMyChairID));
			m_GameClientView.m_GameRecord.SetHistoryResult(pStatusPlay->HistoryInfo, pStatusPlay->wHistoryCount);

			//庄家变量
			m_lApplyBankerCondition = pStatusPlay->lApplyBankerCondition;
			m_wApplyCountMax = pStatusPlay->wApplyCountMax;
			//设置变量
			m_wCurrentBanker = pStatusPlay->wCurrentBankerChairID;

            m_GameClientView.m_MyGame=pStatusPlay->lMyGame;

			m_GameClientView.m_Prizemoney = pStatusPlay->cdPrizemoney;
			CopyMemory(m_GameClientView.m_lMyStakes,pStatusPlay->lMyStakes,sizeof(pStatusPlay->lMyStakes));
			CopyMemory(m_GameClientView.m_lAllStakes,pStatusPlay->lAllStakes,sizeof(pStatusPlay->lAllStakes));
			CopyMemory(m_GameClientView.m_showT,pStatusPlay->cbCardColoct,sizeof(pStatusPlay->cbCardColoct));
			CopyMemory(m_GameClientView.m_showPL,pStatusPlay->cbCardCount,sizeof(pStatusPlay->cbCardCount));
            m_GameClientView.SetMyMaxScore(0);

			//庄家信息
			if ( pStatusPlay->wCurrentBankerChairID == INVALID_CHAIR )
				m_GameClientView.SetBankerInfo(INVALID_CHAIR, pStatusPlay->wBankerCount, 0,pStatusPlay->lCurrentBankerScore);
			else
				m_GameClientView.SetBankerInfo(SwitchViewChairID( pStatusPlay->wCurrentBankerChairID ), pStatusPlay->wBankerCount,0,pStatusPlay->lCurrentBankerScore);
			//更新控制

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
	m_GameClientView.m_Card=pGameStart->cbCard;
	m_GameClientView.g_Tips=pGameStart->cbWinning;
    m_GameClientView.g_Title=true;
	m_GameClientView.cdPrize = pGameStart->cbPrize;

	//派发扑克
	DispatchUserCard(pGameStart->cbCardsite);
	//更新控制

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


	if(IsEnableSound())
	{
 	    PlaySound("H_SLWH\\Sound\\add_gold.wav" ,NULL, SND_FILENAME | SND_ASYNC);//PlaySound("SystemStart",);
	}
	m_GameClientView.m_lAllStakes[pPlaceJetton->cbBetArea] += pPlaceJetton->lBetAmount;

	if	(pPlaceJetton->wChairID == GetMeChairID())
	{
		m_GameClientView.m_lMyStakes[pPlaceJetton->cbBetArea] += pPlaceJetton->lBetAmount;
		m_GameClientView.m_MyGame=pPlaceJetton->lGame;
	}

    m_GameClientView.m_Prizemoney = pPlaceJetton->cdPrizemoney;
    m_GameClientView.SetMyMaxScore(m_GameClientView.m_MyGame);
	m_GameClientView.UpdateGameView(NULL); 	//更新按钮
	return true;
}

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	m_GameClientView.m_wMyWinCount=0;

    for (int i=0;i<INDEX_MAX;i++)
    {
        m_GameClientView.m_wMyWinCount+=pGameEnd->lMyGameScore[i];

    }
    //m_GameClientView.m_Prizemoney =  pGameEnd->lPrizemoney;
    
	return true;
}

void CGameClientDlg::ResetGameData()
{
	//更新上庄控制
	UpdateBankerButtons();
	//更新控制
	//UpdateJettonButtons();
}

//更新控制
void CGameClientDlg::UpdateJettonButtons()
{
	//UpdateGameView(NULL);
	return;
}

//派发扑克
bool CGameClientDlg::DispatchUserCard(BYTE cbPlayerCount)
{

	m_cbCardCount=cbPlayerCount;
    m_GameClientView.m_cbCardCount=cbPlayerCount;
	return true;
}


//发牌动画播放结束
LRESULT CGameClientDlg::OnDealAnimationFinish(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

//定时器消息
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
}

LRESULT CGameClientDlg::OnInsertSystemString(WPARAM wParam, LPARAM lParam)
{
	InsertSystemString((LPCTSTR)wParam);
	return 1;
}

LRESULT CGameClientDlg::OnPlaySound(WPARAM wParam, LPARAM lParam)
{
	//if (IsEnableSound())
	//{
	//	PlayFmodSound(m_strSoundPath +(LPCTSTR)wParam);
	//}

	return 1;
}


//加注消息
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbBetArea=(BYTE)wParam;
	LONG lBetAmount=(LONG)lParam;

	ASSERT(cbBetArea < INDEX_MAX);
	if (cbBetArea >= INDEX_MAX) return 0;

	if (m_GameClientView.m_lMyStakes[cbBetArea]==99)
        return 0;


	//变量定义
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton, sizeof(PlaceJetton));

	//构造变量
	PlaceJetton.cbBetArea=cbBetArea;
	PlaceJetton.lBetAmount=lBetAmount;

	if (m_GameClientView.m_lMyStakes[cbBetArea]+lBetAmount>99)
	{
        PlaceJetton.lBetAmount=99-m_GameClientView.m_lMyStakes[cbBetArea];
	}

	//发送消息
	SendData(SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(PlaceJetton));

	return 0;
}

LRESULT CGameClientDlg::OnFang(WPARAM wParam, LPARAM lParam)
{
//    CGameFrameDlg::OpenSafeBox();
	return true;
}
//自己申请做庄
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{

		//printf("222\n");
	//合法判断
	tagUserData const *pMyUserData = GetUserData( GetMeChairID() );

	if (!pMyUserData)	
		return false;

	bool bApplyBanker = (wParam != 0);

	//当前判断
	if (bApplyBanker)
	{
		if (m_GameClientView.m_MyGame < m_lApplyBankerCondition)
			return true;

		if ( m_wCurrentBanker == GetMeChairID())
			return true;

		if (m_GameClientView.m_AppList.m_AppList.size() >= m_wApplyCountMax)
		{
			CString temp;
			temp.Format(TEXT("超过允许申请的人数 %d"), m_wApplyCountMax);
			InsertSystemString(temp);
			return true;
		}
	}

	CMD_C_ApplyBanker ApplyBanker;

	//赋值变量
	ApplyBanker.bApplyBanker = bApplyBanker;
	ApplyBanker.bAI = false;
	//发送消息
	SendData(SUB_C_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	return true;
}

//其它用户申请做庄
bool CGameClientDlg::OnSubApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pCmdApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//插入玩家
	if ( pCmdApplyBanker->bApplyBanker )
	{
		tagApplyUser ApplyUser;
		ApplyUser.dwUserID = pCmdApplyBanker->dwUserID;
		ApplyUser.lUserScore = pCmdApplyBanker->lGold;
		ApplyUser.strUserName = pCmdApplyBanker->szAccount;
		ApplyUser.stAname = pCmdApplyBanker->szName;
		
		//当前庄家不列表中显示
		if (pCmdApplyBanker->wChairID != m_wCurrentBanker)
		{
			m_GameClientView.m_AppList.AddOrDeletUser(ApplyUser);
		}
		else
		{
			m_GameClientView.m_AppList.AddOrDeletUser(ApplyUser,false);
		}
	}
	else
	{
		tagApplyUser ApplyUser;
		ApplyUser.dwUserID = pCmdApplyBanker->dwUserID;
		ApplyUser.lUserScore = pCmdApplyBanker->lGold;
		ApplyUser.strUserName = pCmdApplyBanker->szAccount;
		ApplyUser.stAname = pCmdApplyBanker->szName;
		m_GameClientView.m_AppList.AddOrDeletUser(ApplyUser,false);
	}

	UpdateBankerButtons();
	


 	return true;
}

void CGameClientDlg::UpdateBankerButtons()
{
	if (IsLookonMode())
		return;

	tagUserData const *pUserData = GetUserData( GetMeChairID());

	if (pUserData)
	{
		if (pUserData->wChairID == m_wCurrentBanker)
		{
            m_GameClientView.m_pSettlement.EnableWindow(false);
		}else
		{
            m_GameClientView.m_pSettlement.EnableWindow(true);
		}
		if (pUserData->wChairID == m_wCurrentBanker || m_GameClientView.m_AppList.FindUser(pUserData->dwUserID) )
		{
			m_GameClientView.m_btnApplyBanker.ShowWindow(FALSE);
			m_GameClientView.m_btnApplyBanker.EnableWindow(FALSE);
			m_GameClientView.m_btnCancelBanker.ShowWindow(TRUE);
			if (m_GameClientView.m_AppList.FindUser(pUserData->dwUserID))
			{
				m_GameClientView.m_btnCancelBanker.EnableWindow(TRUE);
			}
		}
		else
		{
			m_GameClientView.m_btnCancelBanker.ShowWindow(FALSE);
			m_GameClientView.m_btnCancelBanker.EnableWindow(FALSE);
			m_GameClientView.m_btnApplyBanker.ShowWindow(TRUE);

			if (m_GameClientView.m_MyGame>= m_lApplyBankerCondition)
			{
				m_GameClientView.m_btnApplyBanker.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btnApplyBanker.EnableWindow(FALSE);

				static bool bFirst = true;
				if (bFirst)
				{
					//只提醒一次
					m_GameClientView.InsertSystemString(TEXT("申请坐庄的最低分数为 %ld"), m_lApplyBankerCondition);
					bFirst = false;
				}
			}
		}

	}
		return;
}
//切换庄家
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	m_wCurrentBanker = pChangeBanker->wChairID;

	//庄家信息
	if ( pChangeBanker->wChairID == INVALID_CHAIR )
	{
		m_GameClientView.SetBankerInfo(INVALID_CHAIR, pChangeBanker->wBankerCount, pChangeBanker->lBankerWinScore,0);
	}
	else
	{
		m_GameClientView.SetBankerInfo( SwitchViewChairID( pChangeBanker->wChairID ), pChangeBanker->wBankerCount, pChangeBanker->lBankerWinScore ,pChangeBanker->lBankerScore);
	}

	tagUserData const *pUserData = GetUserData(pChangeBanker->wChairID);

	if (pUserData)
	{
		tagApplyUser ApplyUser;
		ApplyUser.dwUserID = pUserData->dwUserID;
		ApplyUser.strUserName = pUserData->szName;
		ApplyUser.stAname = pUserData->szName;
		m_GameClientView.m_AppList.AddOrDeletUser(ApplyUser,false);
	}

	m_lMyMaxStake = pChangeBanker->lMyMaxStake;
	m_GameClientView.m_lBankerMaxLoss = pChangeBanker->lBankerMaxLoss;
	m_GameClientView.SetAllMaxScore(pChangeBanker->lMaxStakes);

	ResetGameData();


	return true;
}

//更新申请庄家列表分数
bool CGameClientDlg::OnSubChangeApplyScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeApplyScore));
	if (wDataSize!=sizeof(CMD_S_ChangeApplyScore)) return false;

	//消息处理
	CMD_S_ChangeApplyScore * pChangeUserScore=(CMD_S_ChangeApplyScore *)pBuffer;

	tagUserData const *pUserData = GetUserData( pChangeUserScore->wChairID );

	if ( pUserData )
	{
		tagApplyUser ApplyUser ;

		ApplyUser.lUserScore = pChangeUserScore->lGold;


		ApplyUser.dwUserID = pUserData->dwUserID;
		ApplyUser.strUserName = pUserData->szName;
		ApplyUser.stAname = pUserData->szName;
		m_GameClientView.m_AppList.UpdateAppList(ApplyUser);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
