#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//游戏定时器
#define IDI_OUT_CARD					200								//出牌定时器
#define IDI_START_GAME					201								//开始定时器

//游戏定时器
#define IDI_MOST_CARD					300								//最大定时器
#define IDI_DISPATCH_CARD				301								//发牌定时器


#define TIME_OUT_CARD					30								//定时器时间
#define TIME_START_GAME					30								//定时器时间

#define MAX_TIME_OUT					3								//超时次数

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_SORT_BY_COLOR,OnSortByColor)
	ON_MESSAGE(IDM_SORT_BY_VALUE,OnSortByValue)
	ON_MESSAGE(IDM_STRUSTEE,OnStrustee)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//游戏变量
	m_bOutCard3=TRUE;
	m_bHandCardCount=0;
	m_wBankerUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bStustee = false;
	m_wTimeOutCount = 0;

	//配置变量
	m_dwCardHSpace=DEFAULT_PELS;

	//出牌变量
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//辅助变量
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//辅助变量
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{
	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//读取配置
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);

	//调整参数
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//配置控件
	m_GameClientView.m_HandCardControl[2].SetCardSpace(m_dwCardHSpace,0,20);

	//test
	//BYTE byFirstCard[] = {
	//	0x01,0x03,0x04,0x05,0x08
	//};
	//BYTE bySecondCard[] = {
	//	0x13,0x24,0x15,0x16,0x17
	//};
	//m_GameLogic.SortCardList(byFirstCard,5);
	//m_GameLogic.SortCardList(bySecondCard,5);
	//bool bLarger = m_GameLogic.CompareCard(byFirstCard,bySecondCard,5,5);
	//end test

	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//游戏变量
	m_bOutCard3=TRUE;
	m_bHandCardCount=0;
	m_wBankerUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bStustee = false;
	m_wTimeOutCount = 0;

	//出牌变量
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//辅助变量
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//删除定时
	KillGameTimer(0);
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_DISPATCH_CARD);

	//辅助变量
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	//构造数据
	CGameOption GameOption;
	GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bAllowLookon = IsAllowUserLookon();

	//配置数据
	if (GameOption.DoModal()==IDOK)
	{
		//获取参数
		m_dwCardHSpace=GameOption.m_dwCardHSpace;

		//设置控件
		EnableSound(GameOption.m_bEnableSound);
		AllowUserLookon(0,GameOption.m_bAllowLookon);
		m_GameClientView.m_HandCardControl[2].SetCardSpace(m_dwCardHSpace,0,20);

		//保存配置
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	}

	return;
}

//时间消息
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_OUT_CARD:			//用户出牌
		{
			//超时判断
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_OUT_CARD))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					if( !m_bStustee && ++m_wTimeOutCount>=MAX_TIME_OUT)
					{
						m_wTimeOutCount = 0;
						OnStrustee((WPARAM)TRUE,0);
						//提示信息
						InsertSystemString(TEXT("由于您多次超时，切换为“系统托管”模式"));
					}
					AutomatismOutCard();
				}
				return true;
			}

			//播放声音
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//开始游戏
		{
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_START_GAME))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					if( m_bStustee )
						OnStart(0,0);
					else 
						PostMessage(WM_CLOSE);
				}
				return true;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//旁观设置
	m_GameClientView.m_HandCardControl[2].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

	return;
}

//网络消息
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//用户出牌
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_PASS_CARD:		//放弃出牌
		{
			return OnSubPassCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_STRUSTEE:		//用户托管
		{
			return OnSubStrustee(pBuffer,wDataSize);
		}
	}

	return true;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_RF_FREE:	//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//设置控件
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
			}
			//设置扑克
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);

			//设置托管状态
			for( WORD j = 0; j < GAME_PLAYER; j++ )
				m_GameClientView.SetStrustee(SwitchViewChairID(j),pStatusFree->bStrustee[j]);
			
			//设置时间
			SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);

			return true;
		}
	case GS_RF_PLAYING:		//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//设置变量
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bHandCardCount=pStatusPlay->bCardCount[GetMeChairID()];
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);

			CopyMemory(m_bHandCardData,pStatusPlay->bCardData,sizeof(BYTE)*m_bHandCardCount);

			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,sizeof(BYTE)*pStatusPlay->bTurnCardCount);
			CopyMemory(m_bCardCount,pStatusPlay->bCardCount,sizeof(m_bCardCount));

			//设置界面
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
				m_GameClientView.m_lAllTurnScore[wViewChairID]=pStatusPlay->lAllTurnScore[i];
				m_GameClientView.m_lLastTurnScore[wViewChairID]=pStatusPlay->lLastTurnScore[i];
			}

			//设置扑克
			BYTE cbCardCount[MAX_COUNT];
			ZeroMemory(cbCardCount,sizeof(cbCardCount));

			//设置扑克
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				if (wViewChairID!=2)
				{
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardCount,pStatusPlay->bCardCount[i]);
				}
				else m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);
			}

			//玩家设置
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
			}

			//桌面设置
			if (m_bTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wLastOutUser);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
			}

			//设置托管状态
			for( WORD j = 0; j < GAME_PLAYER; j++ )
				m_GameClientView.SetStrustee(SwitchViewChairID(j),pStatusPlay->bStrustee[j]);
			if( !IsLookonMode() && pStatusPlay->bStrustee[GetMeChairID()] )
			{
				m_bStustee = true;
				OnStrustee(0,0);
			}

			//设置定时器
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			//设置扑克
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl[2].SetPositively(true);
				m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);
			}

			return true;
		}
	}

	return true;
}

//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//设置变量
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_bOutCard3=pGameStart->cbOutCard3;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bCardCount,13,sizeof(m_bCardCount) );

	//设置扑克
	m_bHandCardCount=13;

	CopyMemory(m_bHandCardData,pGameStart->cbCardData,sizeof(BYTE)*m_bHandCardCount);

	//设置界面
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//扑克数目
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetCardCount(i,13);

		if (i!=2) m_GameClientView.m_HandCardControl[i].SetDisplayFlag(false);
	}

	//发送扑克
	DispatchUserCard(m_bHandCardData,m_bHandCardCount);
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount);


	//控制设置
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl[2].SetPositively(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl[2].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

	return true;
}

//用户出牌
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//变量定义
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);

	//效验数据
	ASSERT(wDataSize>=wHeadSize);
	if (wDataSize<wHeadSize) return false;
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

	//停止发牌
	DoncludeDispatchCard();

	//删除定时器
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//界面设置
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);

	//出牌设置
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->bCardData,pOutCard->bCardCount);
	}

	//清理桌面
	if (m_bTurnCardCount==0)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			if (i==pOutCard->wOutCardUser) continue;
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//记录出牌
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//自己扑克
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//删除扑克 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//设置界面
		m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//其他扑克
	if (pOutCard->wOutCardUser!=GetMeChairID())
	{
		BYTE cbCardData[MAX_COUNT];
		ZeroMemory(cbCardData,sizeof(cbCardData));
		m_GameClientView.m_HandCardControl[wOutViewChairID].SetCardData(cbCardData,m_bCardCount[pOutCard->wOutCardUser]);
	}

	//最大判断
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//设置变量
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		m_wMostUser=pOutCard->wCurrentUser;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

		//设置界面
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetPassFlag(wViewChairID,true);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}

		//环境设置
		SetTimer(IDI_MOST_CARD,3000,NULL);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

		return true;
	}

	//播放声音
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser)) PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//玩家设置
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		WORD wViewChairID=SwitchViewChairID(pOutCard->wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//玩家设置
	if ((IsLookonMode()==false)&&(pOutCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
		//自动PASS
		if( m_bTurnCardCount > m_bHandCardCount )
			OnPassCard(0,0);
	}

	//设置定时器
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		if (m_bCardCount[pOutCard->wCurrentUser] <m_bTurnCardCount)
		{
			SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,3);
		}
		else
		{
			SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		}
	}

	return true;
}

//放弃出牌
bool CGameClientDlg::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;

	//删除定时器
	KillGameTimer(IDI_OUT_CARD);

	//玩家设置
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))
	{
		WORD wViewChairID=SwitchViewChairID(pPassCard->wPassUser);
		m_GameClientView.SetPassFlag(wViewChairID,true);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//一轮判断
	if (pPassCard->bNewTurn==TRUE)
	{
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	}

	//设置界面
	WORD wViewChairID=SwitchViewChairID(pPassCard->wCurrentUser);
	m_GameClientView.SetPassFlag(wViewChairID,false);
	m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

	//玩家设置
	if ((IsLookonMode()==false)&&(pPassCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
	}

	//播放声音
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	//设置时间
	if (m_bCardCount[pPassCard->wCurrentUser]<m_bTurnCardCount)
	{
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,3);
	}
	else
	{
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
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

	//设置状态
	SetGameStatus(GS_RF_FREE);

	//停止发牌
	DoncludeDispatchCard();

	//删除定时器
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);
	KillTimer(IDI_DISPATCH_CARD);

	//隐藏控件
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//禁用控件
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//设置积分
	TCHAR szBuffer[128] = TEXT("");
	InsertGeneralString(TEXT("本局结束,成绩统计:"),RGB(255,0,255),true);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
		m_GameClientView.m_lAllTurnScore[SwitchViewChairID(i)]+=pGameEnd->lGameScore[i];
		m_GameClientView.m_lLastTurnScore[SwitchViewChairID(i)]=pGameEnd->lGameScore[i];

		//在聊天框显示成绩
		_snprintf(szBuffer,CountArray(szBuffer),TEXT(" %s: %+ld分"),pUserData->szName,pGameEnd->lGameScore[i]);
		InsertGeneralString(szBuffer,RGB(255,0,255),true);
	}
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//显示扑克
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);

	//设置托管
	if( m_bStustee )
		OnStrustee(0,0);

	//播放声音
	WORD wMeChairID=GetMeChairID();
	DOUBLE lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,30);
	}

	return true;
}

//出牌判断
bool CGameClientDlg::VerdictOutCard()
{
	//状态判断
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//获取扑克
	BYTE bCardData[13];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[2].GetShootCard(bCardData,CountArray(bCardData));

	//强制出大
	WORD wNextUser=(GetMeChairID()+1)%GAME_PLAYER;
	if ((m_bCardCount[wNextUser]==1)&&(bShootCount==1))
	{
		BYTE cbTempCard[MAX_COUNT];
		CopyMemory(cbTempCard,m_bHandCardData,m_bHandCardCount*sizeof(BYTE));
		m_GameLogic.SortCardList(cbTempCard,m_bHandCardCount);
		if (bCardData[0]!=cbTempCard[0]) return false;
	}

	//出牌判断
	if (bShootCount>0L)
	{
		//排列扑克
		m_GameLogic.SortCardList(bCardData,bShootCount);

		//分析类型
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);

		//类型判断
		if (bCardType==CT_ERROR) return false;

		//首出判断
		if ((m_bOutCard3==TRUE)&&(m_wBankerUser==GetMeChairID())&&(m_bCardCount[m_wBankerUser]==13))
		{
			//方三判断
			for (BYTE i=0;i<bShootCount;i++)
			{
				if (bCardData[i]==0x03) break;
			}
			if (i==bShootCount) return false;
		}

		//跟牌判断
		if (m_bTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bShootCount);
	}

	return false;
}

//放弃判断
bool CGameClientDlg::VerdictPassCard()
{
	//首出判断
	if (m_bTurnCardCount==0) return false;

	//跟牌判断
	WORD wNextUser=(GetMeChairID()+1)%GAME_PLAYER;
	if ((m_bCardCount[wNextUser]==1)&&(m_bTurnCardCount==1))
	{
		if (m_GameLogic.CompareCard(m_bTurnCardData[0],m_bHandCardData[0])==true) return false;
	}

	//胜利判断
	if ((m_bTurnCardCount==1)&&(m_bHandCardCount==1))
	{
		if (m_GameLogic.CompareCard(m_bTurnCardData[0],m_bHandCardData[0])==true) return false;
	}

	return true;
}

//自动出牌
bool CGameClientDlg::AutomatismOutCard()
{
	//控制界面
	KillGameTimer(IDI_OUT_CARD);
	//定义变量
	WORD wMeChairId = GetMeChairID();
	//搜索出牌
	tagOutCardResult OutCardResult;
	if( m_GameLogic.StrusteeOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,
		wMeChairId==m_wBankerUser&&m_bHandCardCount==13,m_bCardCount[(wMeChairId+1)%GAME_PLAYER]==1,OutCardResult) )
	{
		//弹起扑克
		m_GameClientView.m_HandCardControl[2].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
		//效验
		ASSERT( VerdictOutCard() );
		m_GameClientView.m_btOutCard.EnableWindow(VerdictOutCard()?TRUE:FALSE);
		OnOutCard(0,0);
	}
	else
	{
		ASSERT( VerdictPassCard() );
		OnPassCard(0,0);
	}

	return true;
}

//停止发牌
bool CGameClientDlg::DoncludeDispatchCard()
{
	//停止发牌
	if (m_cbRemnantCardCount>0)
	{
		//删除时间
		KillTimer(IDI_DISPATCH_CARD);

		//扑克数据
		BYTE cbUserCard[MAX_COUNT];
		ZeroMemory(cbUserCard,sizeof(cbUserCard));

		//设置界面
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=2)
			{
				m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,MAX_COUNT);
			}
			else m_GameClientView.m_HandCardControl[i].SetCardData(m_bHandCardData,m_bHandCardCount); 
		}

		//设置变量
		m_cbRemnantCardCount=0;
		m_cbDispatchCardCount=0;
		ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));
	}

	return true;
}

//派发扑克
bool CGameClientDlg::DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount)
{
	//设置变量
	m_cbDispatchCardCount=0;
	m_cbRemnantCardCount=cbCardCount;
	CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//设置定时器
	SetTimer(IDI_DISPATCH_CARD,200,NULL);

	return true;
}

//定时器消息
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	if ((nIDEvent==IDI_MOST_CARD)&&(m_wMostUser!=INVALID_CHAIR))
	{
		//变量定义
		WORD wCurrentUser=m_wMostUser;
		m_wMostUser=INVALID_CHAIR;

		//删除定时器
		KillTimer(IDI_MOST_CARD);

		//设置界面
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
		for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

		//玩家设置
		if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
		{
			ActiveGameFrame();
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.EnableWindow(FALSE);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
		}

		//设置时间
		SetGameTimer(wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

		return;
	}

	//派发扑克
	if (nIDEvent==IDI_DISPATCH_CARD)
	{
		//派发控制
		if (m_cbRemnantCardCount>0)
		{
			//扑克数据
			BYTE cbUserCard[MAX_COUNT];
			ZeroMemory(cbUserCard,sizeof(cbUserCard));

			//派发扑克
			m_cbRemnantCardCount--;
			m_cbDispatchCardCount++;
			m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount);

			//设置界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (i!=2)
				{
					m_GameClientView.m_HandCardControl[i].SetCardData(cbUserCard,m_cbDispatchCardCount);
				}
				else m_GameClientView.m_HandCardControl[i].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount);
				//设置牌张数
				m_GameClientView.SetCardCount(SwitchViewChairID(i),m_cbDispatchCardCount);
			}

			//播放声音
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
		}

		//中止判断
		if (m_cbRemnantCardCount==0)
		{
			//删除定时器
			KillTimer(IDI_DISPATCH_CARD);

			//扑克数目
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
			}

			//当前玩家
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btPassCard.EnableWindow(FALSE);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			}

			//播放剩余
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

			//设置时间
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_wTimeOutCount=0;
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//设置界面
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	//隐藏控件
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_HandCardControl[i].SetPositively(false);

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//出牌消息
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//状态判断
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return 0;
	if (m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE) return 0;

	//设置界面
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);
	
	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[2].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList(OutCard.bCardData,OutCard.bCardCount);
	SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//预先显示
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//预先删除
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//放弃出牌
LRESULT CGameClientDlg::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//状态判断
	if (m_GameClientView.m_btPassCard.IsWindowVisible()==FALSE) return 0;
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	//设置界面
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//发送数据
	SendData(SUB_C_PASS_CARD);

	//预先显示
	m_GameClientView.m_UserCardControl[2].SetCardData(NULL,0);
	m_GameClientView.SetPassFlag(2,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//出牌提示
LRESULT CGameClientDlg::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//定义变量
	WORD wMeChairId = GetMeChairID();
	//搜索出牌
	tagOutCardResult OutCardResult;
	if( m_GameLogic.StrusteeOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,
		wMeChairId==m_wBankerUser&&m_bHandCardCount==13,m_bCardCount[(wMeChairId+1)%GAME_PLAYER]==1,OutCardResult) )
	{
		//弹起扑克
		m_GameClientView.m_HandCardControl[2].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
		//效验
		ASSERT( VerdictOutCard() );
		m_GameClientView.m_btOutCard.EnableWindow(VerdictOutCard()?TRUE:FALSE);
	}
	else
	{
		ASSERT( VerdictPassCard() );
		OnPassCard(0,0);
	}
	return 0;
}

//花色排序
LRESULT CGameClientDlg::OnSortByColor(WPARAM wParam, LPARAM lParam)
{
	if (m_bHandCardCount>0)
	{
		m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_COLOR);
		m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	return 0;
}

//大小排序
LRESULT CGameClientDlg::OnSortByValue(WPARAM wParam, LPARAM lParam)
{
	if (m_bHandCardCount>0)
	{
		m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_ORDER);
		m_GameClientView.m_HandCardControl[2].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	return 0;
}

//托管
LRESULT CGameClientDlg::OnStrustee(WPARAM wParam, LPARAM lParam)
{
	//设置状态
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStrustee.SetButtonImage(IDB_START_TRUSTEE,AfxGetInstanceHandle(),false);
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStrustee.SetButtonImage(IDB_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
	}
	//提示信息
	if( FALSE == (BOOL)wParam )
	{
		TCHAR szBuffer[32];
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("您%s了托管功能."),m_bStustee?TEXT("选择"):TEXT("取消"));
		InsertSystemString(szBuffer);
	}
	//发送数据
	CMD_C_Strustee Strustee;
	Strustee.bStrustee = m_bStustee;
	SendData(SUB_C_STRUSTEE,&Strustee,sizeof(Strustee));

	return 0;
}

//右键扑克
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
#ifndef	TEST
	//设置控件
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);
#endif

	return 0;
}

//左键扑克
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//用户出牌
	OnOutCard(0,0);

	return 0;
}

//用户托管
bool CGameClientDlg::OnSubStrustee(const void *pBuffer,WORD wDataSize)
{
	//变量定义
	CMD_S_Strustee *pStrustee = (CMD_S_Strustee *)pBuffer;

	//效验数据
	ASSERT( wDataSize == sizeof(CMD_S_Strustee) );
	if( wDataSize != sizeof(CMD_S_Strustee) ) return false;

	//设置托管
	m_GameClientView.SetStrustee(SwitchViewChairID(pStrustee->wChairId),pStrustee->bStrustee);

	//提示信息
	if( IsLookonMode() || GetMeChairID() != pStrustee->wChairId )
	{
		const tagUserData *pUserData = GetUserData(pStrustee->wChairId);
		if( pUserData == NULL ) return true;
		TCHAR szBuffer[128] = TEXT("");
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("玩家[ %s ]%s了托管功能"),pUserData->szName,pStrustee->bStrustee?TEXT("选择"):TEXT("取消"));
		InsertSystemString(szBuffer);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

