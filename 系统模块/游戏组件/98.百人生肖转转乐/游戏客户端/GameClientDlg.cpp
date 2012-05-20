#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "OptionDlg.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_START_JETTON            100                                 //开牌时间
#define IDI_OPEN_CARD               101                                 //下注时间
#define IDI_GAME_FREE			    102									//下注时间

#define IDI_DISPATCH_CARD			301									//发牌时间
#define IDI_SHOW_GAME_RESULT		302									//显示结果
#define ITI_TIMER_ANIMATE           303                                 //计时器动画
 

//声音标识
#define IDS_SOUND_ADDGOLD           1
#define IDS_SOUND_END               2
#define IDS_SOUND_START_JETTON      3
#define IDS_SOUND_END_JETTON        4
#define IDS_SOUND_SENDCARD          5
#define IDS_SOUND_TIMER             6
#define IDS_SOUND_MAX_JETTON        7
#define IDS_SOUND_WIN               8
#define IDS_SOUND_LOST              9
#define IDS_SOUND_BACKGROUND        10
#define IDS_SOUND_TALK              11
#define IDS_SOUND_ACCLAIM           12

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_PLACE_JETTON, OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//下注信息
	m_lMeMaxScore=0L;

	//状态变量
	m_bAllowGameMusic = true;

	//庄家信息
	m_lApplyBankerCondition = 1000000L;
	m_wCurrentBanker = INVALID_CHAIR;
	m_bMeApplyBanker = false;
	m_LastResult=0xff;

}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{
#ifdef _DEBUG
	SetWindowText(TEXT(""));
#else
	//设置标题
	SetWindowText(TEXT("生肖转转乐"));
#endif
	//设置标题
	//SetWindowText(TEXT("生肖转转乐 --  Ver：6.0.1.0"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
	m_GameClientView.m_btApplyBanker.EnableWindow(true);

	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//下注信息
	m_lMeMaxScore=0L;
	//状态变量
	m_bMeApplyBanker = false;

	//庄家信息
	m_lApplyBankerCondition = 100000000L;
	m_wCurrentBanker = INVALID_CHAIR;

}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
}

//时间消息
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_JETTON:
		{
			if (nElapse==0)
			{
				KillGameTimer(nTimerID);

				//设置光标
				m_GameClientView.SetCurrentJetton(0L);

				//禁止按钮
				m_GameClientView.m_btJetton100.EnableWindow(false);
				m_GameClientView.m_btJetton1K.EnableWindow(false);
				m_GameClientView.m_btJetton1W.EnableWindow(false);
				m_GameClientView.m_btJetton10W.EnableWindow(false);
				m_GameClientView.m_btJetton100W.EnableWindow(false);
				m_GameClientView.m_btJetton1000W.EnableWindow(false);
				m_GameClientView.m_btApplyBanker.EnableWindow(false);
				m_GameClientView.m_btCancelBanker.EnableWindow(false);

				m_GameClientView.KillTimer(ITI_TIMER_ANIMATE);
				
				return false;
			}
			else if (nElapse<=6 && m_wCurrentBanker!=INVALID_CHAIR)
			{
				if (nElapse==6)
				{
					m_GameClientView.SetTimer(ITI_TIMER_ANIMATE, 15, NULL);
				}
			}
			else if (nElapse==17)
			{
			}
			return true;
		}
	case IDI_OPEN_CARD:
		{
			return true;
		}
	case IDI_GAME_FREE:
		{
			return true;
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
	case SUB_S_FREE:
		{
			SetGameStatus(GS_BACCARAT_FREE);

			m_GameClientView.DeleteApplyList();
			m_GameClientView.m_cbTimeCount = TIME_GAME_FREE - 1;
			m_GameClientView.SetTimer(IDT_COUNT_TIME,1000,0);

			if( m_LastResult != 0xff )
			{
				m_GameClientView.m_vecHistory.push_back(m_LastResult);
				if ( m_GameClientView.m_vecHistory.size() > MAX_HISTORY_LENTH )
				{
					std::vector<DWORD>::iterator iter =  m_GameClientView.m_vecHistory.begin() + m_GameClientView.m_vecHistory.size() - MAX_HISTORY_LENTH;
					m_GameClientView.m_vecHistory.erase(m_GameClientView.m_vecHistory.begin(),iter);
					m_GameClientView.m_wCursors = m_GameClientView.m_vecHistory.size() - MAX_HISTORY_DISPLAY - 1;
				}
				if( m_GameClientView.m_vecHistory.size() > MAX_HISTORY_DISPLAY )
				{
					m_GameClientView.m_wCursors++;
				}
			}

			return true;
		}
	case SUB_S_RESLUT:
		{	
			SetGameStatus(GS_BACCARAT_OPENCARD);
			m_GameClientView.DeleteApplyList();
			CMD_S_StatusOpenCard *pOpenCard = (CMD_S_StatusOpenCard *)pBuffer;
			m_GameClientView.StartMovingAnimal(pOpenCard->cbAnimalPos);
			m_LastResult = pOpenCard->cbAnimalPos%12;
			m_GameClientView.m_cbTimeCount = TIME_OPEN_CARD - 1;
			m_GameClientView.SetTimer(IDT_COUNT_TIME,1000,0);
			UpdateButtonContron();
			return true;
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_CURRENT_BANKER:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_GameStart));
			if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

			//消息处理
			CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
		
			const tagUserData *pUserData = GetUserData(m_wCurrentBanker);
			if(pUserData && m_wCurrentBanker != pGameStart->wBanker )
			{
				TCHAR temp[60];
				sprintf( temp,"用户%s已下庄",pUserData->szName );
				InsertSystemString(temp);
			}

			m_wCurrentBanker = pGameStart->wBanker;
			m_GameClientView.SetBankerInfo(pGameStart->wBanker,pGameStart->cbBankerTimes,0);
			m_GameClientView.m_nValidMoney = GetUserData(GetMeChairID())->lGameGold;

			if( IsLookonMode() == false)
			{
				m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
				m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
				m_GameClientView.m_btApplyBanker.EnableWindow(true);

				//状态判断
				if (GetGameStatus()==GS_BACCARAT_FREE || GetGameStatus()==GS_BACCARAT_JETTON) 
				{
					m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
					m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
					m_GameClientView.m_btApplyBanker.EnableWindow(true);
				}
				else
				{
					m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
					m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
					m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
				}

				//显示判断
				const tagUserData *pMeUserData = GetUserData(GetMeChairID());
				if (m_bMeApplyBanker)
				{
					m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
					m_GameClientView.m_btCancelBanker.EnableWindow(true);
					m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);

					bool bMeBanker = pMeUserData->wChairID == m_wCurrentBanker ? true : false;
					if (bMeBanker)
					{
						m_GameClientView.m_btCancelBanker.SetButtonImage( IDR_BT_CANCEL_BANKER,AfxGetInstanceHandle(),FALSE);
					}
					else 
					{
						m_GameClientView.m_btCancelBanker.SetButtonImage(IDR_BT_CANCEL_APPLY,AfxGetInstanceHandle(),FALSE);
					}

					m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
					m_GameClientView.m_btCancelBanker.EnableWindow(TRUE);
				}
				else if (m_lApplyBankerCondition <= pMeUserData->lScore)
				{
					m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
					m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
					m_GameClientView.m_btApplyBanker.EnableWindow(true);
				}
			}
			return  true;
		}
	case SUB_S_OPEN_CARD:       //游戏开牌
		{
			return OnSubOpenCard(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			return OnUserApplyBanker( pBuffer, wDataSize );
		}
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			return OnChangeBanker( pBuffer, wDataSize );
		}
	case SUB_S_CHANGE_USER_SCORE://更新积分
		{
			return OnChangeUserScore( pBuffer, wDataSize );
		}
	case SUB_S_GAME_SCORE:		//游戏积分
		{
			return OnSubGameScore(pBuffer, wDataSize);
		}
	}

	return true;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_BACCARAT_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			SetGameStatus(GS_BACCARAT_FREE);

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_GameClientView.m_wCurrentBankerChairID = m_wCurrentBanker=pStatusFree->wCurrentBankerChairID;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.m_lBankerTotal = pStatusFree->lBankerWin;
			m_GameClientView.m_cbBankerTime = pStatusFree->cbBankerTime;

			if (bLookonOther==false && m_wCurrentBanker==GetMeChairID())
			{
				m_bMeApplyBanker=false;
			}
			UpdateButtonContron();
			m_GameClientView.m_cbTimeCount = TIME_GAME_FREE;
			m_GameClientView.SetTimer(IDT_COUNT_TIME,1000,0);
			m_GameClientView.DeleteApplyList();
			return true;
		}
	case GS_BACCARAT_OPENCARD:
	case GS_BACCARAT_JETTON:		//游戏状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusJetton));
			if (wDataSize!=sizeof(CMD_S_StatusJetton)) return false;

			SetGameStatus(GS_BACCARAT_JETTON);

			//消息处理
			CMD_S_StatusJetton * pStatusJetton=(CMD_S_StatusJetton *)pBuffer;

			//庄家变量
			m_lApplyBankerCondition = pStatusJetton->lApplyBankerCondition;
			m_GameClientView.m_wCurrentBankerChairID = m_wCurrentBanker = pStatusJetton->wCurrentBankerChairID;

			m_lAreaLimitScore=pStatusJetton->lAreaLimitScore;
			m_GameClientView.m_lBankerTotal = pStatusJetton->lBankerWin;
			m_GameClientView.m_cbBankerTime = pStatusJetton->cbBankerTime;

			if (bLookonOther==false && m_wCurrentBanker==GetMeChairID())
			{
				m_bMeApplyBanker=false;
			}

			UpdateButtonContron();
			m_GameClientView.m_cbTimeCount = TIME_GAME_FREE;
			m_GameClientView.SetTimer(IDT_COUNT_TIME,1000,0);
			m_GameClientView.DeleteApplyList();
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
	m_wCurrentBanker = pGameStart->wBanker;
	m_GameClientView.SetBankerInfo(pGameStart->wBanker,pGameStart->cbBankerTimes,0);
	SetGameStatus(GS_BACCARAT_JETTON);
	m_GameClientView.m_nValidMoney = GetUserData(GetMeChairID())->lGameGold;
	//char   string[25];
	//InsertSystemString(itoa(m_wCurrentBanker, string, 10));
	UpdateButtonContron();

	//InsertSystemString("游戏开始，请下注！\n");
	m_GameClientView.m_ImageBackBuf.Destroy();
	m_GameClientView.m_ImageBackBuf.LoadFromResource(AfxGetInstanceHandle(), IDR_VIEW_BACK,TEXT("JPG"),FALSE);
	m_GameClientView.m_bFlashWinner=FALSE;
	m_GameClientView.m_bDisPlayEnd = FALSE;
	m_GameClientView.m_cbTimeCount = TIME_PLACE_JETTON - 1 ;
	m_GameClientView.StartRandAnimal();

	PlayGameSound( AfxGetInstanceHandle() ,"IDC_SND_BK" );

	return true;
}

bool CGameClientDlg::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
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
	m_GameClientView.m_nTotalUserBet[pPlaceJetton->cbJettonArea]+=pPlaceJetton->lJettonScore;
	
	//设置加注界面
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

	if( pPlaceJetton->wChairID == GetMeChairID() )
	{
		m_GameClientView.m_nUserBet[pPlaceJetton->cbJettonArea]+=pPlaceJetton->lJettonScore;
		m_GameClientView.m_nValidMoney -= pPlaceJetton->lJettonScore;		//更新可用分
		UpdateButtonContron();
	}

	//播放声音
	if (pPlaceJetton->wChairID!=GetMeChairID())
	{
		if (pPlaceJetton->lJettonScore >= 1000000L)
		{		
		}
	}

	PlayGameSound( AfxGetInstanceHandle(),"IDC_SND_ADDCHIP");

	return true;
}

//游戏积分
bool CGameClientDlg::OnSubGameScore(const void * pBuffer, WORD wDataSize)
{
	CMD_S_UserScore *pUserScore=(CMD_S_UserScore *)pBuffer; 
	
	//TCHAR	temp[100];
	//sprintf(temp,"用户得分：%I64d,庄家得分:%I64d",pUserScore->nScore,pUserScore->nBankerScore);
	//InsertSystemString(temp);
	m_GameClientView.SetGameScore( pUserScore->nScore , 0 , pUserScore->nBankerScore );

	return true;
}


//更新控制
void CGameClientDlg::UpdateButtonContron()
{
	if (/*!IsLookonMode() &&*/ (GetGameStatus()==GS_BACCARAT_JETTON) &&
		(m_GameClientView.m_wCurrentBankerChairID != INVALID_CHAIR && m_GameClientView.m_wCurrentBankerChairID != GetMeChairID()))
	{	
		m_GameClientView.m_btJetton100.EnableWindow(false);
		m_GameClientView.m_btJetton1K.EnableWindow(false);
		m_GameClientView.m_btJetton1W.EnableWindow(false);
		m_GameClientView.m_btJetton10W.EnableWindow(false);
		m_GameClientView.m_btJetton100W.EnableWindow(false);		
		m_GameClientView.m_btJetton1000W.EnableWindow(false);


		// 80000 , 1000
		if ( m_GameClientView.m_nValidMoney >= 5000000 )
		{
			m_GameClientView.m_btJetton1000W.EnableWindow(true);
		}
		if ( m_GameClientView.m_nValidMoney >= 1000000 )
		{
			m_GameClientView.m_btJetton100W.EnableWindow(true);

			if( m_GameClientView.m_lCurrentJetton > m_GameClientView.m_nValidMoney )
				m_GameClientView.m_lCurrentJetton = 100000;
		}
		if ( m_GameClientView.m_nValidMoney >= 100000 )
		{
			m_GameClientView.m_btJetton10W.EnableWindow(true);

			if(m_GameClientView.m_lCurrentJetton > m_GameClientView.m_nValidMoney )
				m_GameClientView.m_lCurrentJetton = 10000;
		}
		if ( m_GameClientView.m_nValidMoney >= 10000 )
		{
			m_GameClientView.m_btJetton1W.EnableWindow(true);

			if(m_GameClientView.m_lCurrentJetton > m_GameClientView.m_nValidMoney )
				m_GameClientView.m_lCurrentJetton = 1000;
		}
		if ( m_GameClientView.m_nValidMoney >= 1000 )
		{
			m_GameClientView.m_btJetton1K.EnableWindow(true);
			if(m_GameClientView.m_lCurrentJetton > m_GameClientView.m_nValidMoney )
				m_GameClientView.m_lCurrentJetton = 100;
		}
		if ( m_GameClientView.m_nValidMoney >= 100 )
		{
			m_GameClientView.m_btJetton100.EnableWindow(true);
			if(m_GameClientView.m_lCurrentJetton > m_GameClientView.m_nValidMoney )
				m_GameClientView.m_lCurrentJetton = 100;
		}






	}
	else
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		m_GameClientView.m_btJetton100.EnableWindow(false);
		m_GameClientView.m_btJetton1K.EnableWindow(false);
		m_GameClientView.m_btJetton1W.EnableWindow(false);
		m_GameClientView.m_btJetton10W.EnableWindow(false);
		m_GameClientView.m_btJetton100W.EnableWindow(false);
		m_GameClientView.m_btJetton1000W.EnableWindow(false);
	}

	//申请按钮
	if ( ! IsLookonMode() )
	{
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btApplyBanker.EnableWindow(true);

		//状态判断
		if (GetGameStatus()==GS_BACCARAT_FREE || GetGameStatus()==GS_BACCARAT_JETTON) 
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.EnableWindow(true);
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		}

		//显示判断
		const tagUserData *pMeUserData = GetUserData(GetMeChairID());
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btCancelBanker.EnableWindow(true);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);

			bool bMeBanker = pMeUserData->wChairID == m_wCurrentBanker ? true : false;
			if (bMeBanker)
			{
				m_GameClientView.m_btCancelBanker.SetButtonImage( IDR_BT_CANCEL_BANKER,AfxGetInstanceHandle(),FALSE);
			}
			else 
			{
				m_GameClientView.m_btCancelBanker.SetButtonImage(IDR_BT_CANCEL_APPLY,AfxGetInstanceHandle(),FALSE);
			}

			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btCancelBanker.EnableWindow(TRUE);
		}
		else if (m_lApplyBankerCondition <= pMeUserData->lScore)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.EnableWindow(true);
		}
	}
}

//定时器消息
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_DISPATCH_CARD:		//派发扑克
		{
			return;
		}
	case IDI_SHOW_GAME_RESULT:
		{
			m_nShowResultTime--;
			if ( m_nShowResultTime <= 0 )
			{
				KillTimer(IDI_SHOW_GAME_RESULT);
				m_GameClientView.SetShowGameFlag(false);
			}
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//加注消息
LRESULT CGameClientDlg::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//申请消息
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//旁观判断
	if ( IsLookonMode() )
	{
		ASSERT(FALSE);
		return 0;
	}

	//转换变量
	bool bApplyBanker = wParam != 0 ? true : false;

	//合法判断
	tagUserData const *pMeUserData = GetUserData( GetMeChairID() );
	if ( bApplyBanker && pMeUserData->lScore < m_lApplyBankerCondition )
	{
		InsertSystemString(TEXT("您的分数不足以申请做庄的条件！"));
		return 0;
	}

	//当前判断
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) 
	{
		ASSERT(FALSE);
		return 0;
	}

	CMD_C_ApplyBanker ApplyBanker;
	ApplyBanker.bApplyBanker = bApplyBanker;

	//发送消息
	SendData(SUB_C_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//设置按钮
	if ( m_wCurrentBanker == GetMeChairID() && !bApplyBanker )
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(false);
	}

	return 0;
}

//申请做庄
bool CGameClientDlg::OnUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;


	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//插入玩家
	if ( pApplyBanker->bApplyBanker )
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName = pApplyBanker->szAccount;
		ApplyUser.lUserScore = pApplyBanker->lScore;

		TCHAR temp[60];
		sprintf(temp,"用户%s加入上庄队列",pApplyBanker->szAccount);
		InsertSystemString(temp);

		//插入判断
		bool bInsertApplyUser = true;

		if ( m_wCurrentBanker != INVALID_CHAIR )
		{
			tagUserData const *pBankerUserData = GetUserData(m_wCurrentBanker);
			if ( pBankerUserData != NULL && 0==lstrcmp(pBankerUserData->szName, pApplyBanker->szAccount))
				bInsertApplyUser = false;
		}

		if ( bInsertApplyUser == true ) 
		{
			m_GameClientView.m_ApplyUserList.Add(ApplyUser);
		}

		//更换按钮
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

		//TCHAR temp[60];
		//sprintf(temp,"用户%s已下庄",pApplyBanker->szAccount);
		//InsertSystemString(temp);

		//删除申请
		INT_PTR nCount = m_GameClientView.m_ApplyUserList.GetCount();
		for (INT_PTR i = 0; i < nCount; i++)
		{
			tagApplyUser& Found = m_GameClientView.m_ApplyUserList.GetAt(i);
			if (ApplyUser.strUserName==Found.strUserName)
				m_GameClientView.m_ApplyUserList.RemoveAt(i);
		}

		//更换按钮
		tagUserData const *pUserData = GetUserData( GetMeChairID() );
		if ( pUserData &&  !strcmp(pApplyBanker->szAccount, pUserData->szName ) )
		{
			m_bMeApplyBanker = false;
			UpdateButtonContron();
		}
	}

	return true;
}

//切换庄家
bool CGameClientDlg::OnChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;


	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//显示图片
	m_GameClientView.ShowChangeBanker( m_wCurrentBanker != pChangeBanker->wChairID );

	//自己判断
	if ( m_wCurrentBanker == GetMeChairID() /*&& IsLookonMode() == false */&& pChangeBanker->wChairID != GetMeChairID() ) 
	{
		m_bMeApplyBanker = false;
	}

	m_GameClientView.m_wCurrentBankerChairID = m_wCurrentBanker = pChangeBanker->wChairID;

	//删除玩家
	if ( m_wCurrentBanker != INVALID_CHAIR )
	{
		tagUserData const *pBankerUserData = GetUserData(m_wCurrentBanker);
		if ( pBankerUserData != NULL )
		{
			tagApplyUser ApplyUser;
			ApplyUser.strUserName = pBankerUserData->szName;

			INT_PTR nCount = m_GameClientView.m_ApplyUserList.GetCount();
			for (INT_PTR i = 0; i < nCount; i++)
			{
				tagApplyUser& Found = m_GameClientView.m_ApplyUserList.GetAt(i);
				if (ApplyUser.strUserName==Found.strUserName)
				{
					m_GameClientView.m_ApplyUserList.RemoveAt(i);
					break;
				}
			}
		}
	}
	
	//char   string[25];
	//InsertSystemString(itoa(m_wCurrentBanker, string, 10));
	//更新界面
	UpdateButtonContron();

	return true;
}

//更新积分
bool CGameClientDlg::OnChangeUserScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeUserScore));
	if (wDataSize!=sizeof(CMD_S_ChangeUserScore)) return false;

	//消息处理
	CMD_S_ChangeUserScore * pChangeUserScore=(CMD_S_ChangeUserScore *)pBuffer;
	tagUserData const *pUserData = GetUserData( pChangeUserScore->wChairID );

	if ( pUserData )
	{
		tagApplyUser ApplyUser;
		ApplyUser.lUserScore = pChangeUserScore->lScore;
		ApplyUser.strUserName = pUserData->szName;
		//m_GameClientView.m_ApplyUser.UpdateUser( ApplyUser );
	}

	//庄家信息
	if ( m_wCurrentBanker == pChangeUserScore->wCurrentBankerChairID )
	{
		m_GameClientView.SetBankerInfo( ( pChangeUserScore->wCurrentBankerChairID ), pChangeUserScore->cbBankerTime, pChangeUserScore->lCurrentBankerScore );
		//更新界面
		//char   string[25];
		//InsertSystemString(itoa(m_wCurrentBanker, string, 10));
		UpdateButtonContron();
	}

	return true;
}

//游戏记录
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{

	return true;
}
