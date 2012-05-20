#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//游戏定时器
#define IDI_OUT_CARD					200								//出牌定时器
#define IDI_MOST_CARD					201								//最大定时器
#define IDI_LAST_TURN					302								//上轮定时器
#define IDI_START_GAME					203								//开始定时器

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_LAST_TURN_CARD,OnMessageLastTurn)
	ON_MESSAGE(IDM_SORT_CARD,OnSortCard)
	ON_MESSAGE(WM_ENABLE_HISTORY,OnMessageEnableHistory)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//游戏变量
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));

	//配置变量
	m_cbSortType=ST_VIEW;
	m_bLastTurn=false;
	m_bDeasilOrder=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//出牌变量
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//辅助变量
	m_bStustee=false;
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;

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
	SetWindowText(TEXT("五狼腿游戏  --  Ver：6.0.1.0"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//读取配置
	m_dwCardHSpace=DEFAULT_PELS;
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	//调整参数
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//配置控件
	m_GameClientView.SetUserOrder(m_bDeasilOrder);
	m_GameClientView.m_HandCardControl[0].SetCardSpace(m_dwCardHSpace,0,0);
	m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace,0,20);

	/*
	BYTE cbTurnCard[]={0x11,0x01, 0x01, 0x21};
	BYTE cbHandCard[]={0x34,0x04,0x17, 0x19,0x19, 0x29, 0x0a, 0x3b,0x3c, 0x2c,\
		0x2c, 0x1c, 0x21, 0x21, 0x01, 0x35, 0x3a, 0x3d
				};
	m_GameLogic.SortCardList(cbTurnCard,CountArray(cbTurnCard),ST_ORDER);
	m_GameLogic.SortCardList(cbHandCard,CountArray(cbHandCard),ST_ORDER);
	m_GameClientView.m_UserCardControl[0].SetCardData(cbTurnCard,CountArray(cbTurnCard));
	m_GameClientView.m_HandCardControl[1].SetCardData(cbHandCard,CountArray(cbHandCard));
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag(true);

	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));
	BYTE cbCardType=m_GameLogic.GetCardType(cbTurnCard,CountArray(cbTurnCard),true);
	//cbCardType = CT_ERROR;
	bool bSuccess=m_GameLogic.SearchOutCard(cbHandCard,CountArray(cbHandCard),cbTurnCard,CountArray(cbTurnCard),cbCardType,OutCardResult);
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
	*/
	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//游戏变量
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));

	//出牌变量
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//辅助变量
	m_cbSortType=ST_VIEW;
	m_bStustee=false;
	m_bLastTurn=false;
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;

	//删除定时
	KillGameTimer(0);
	KillTimer(IDI_LAST_TURN);
	KillTimer(IDI_MOST_CARD);

	//历史数据
	m_HistoryCard.ResetData();

	m_GameClientView.m_HandCardControl[0].SetPositively(false);
	m_GameClientView.m_HandCardControl[1].SetPositively(false);

	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	//构造数据
	CGameOption GameOption;
	GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();
	GameOption.m_bDeasilOrder=m_GameClientView.IsDeasilOrder();

	//配置数据
	if (GameOption.DoModal()==IDOK)
	{
		//获取参数
		m_bDeasilOrder=GameOption.m_bDeasilOrder;
		m_dwCardHSpace=GameOption.m_dwCardHSpace;

		//设置控件
		EnableSound(GameOption.m_bEnableSound);
		m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);
		m_GameClientView.m_HandCardControl[0].SetCardSpace(m_dwCardHSpace,0,0);
		m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace,0,20);

		//保存配置
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);
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
			//超时三次自动托管
			if ((m_wTimeOutCount>=3)&&(m_bStustee==false))OnStusteeControl(0,0);
			//超时判断
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(30))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					if (m_bStustee==false) m_wTimeOutCount++;
					if (!m_bStustee)AutomatismOutCard();
					else 
					{
						if(m_bTurnCardCount==0)AutomatismOutCard();
						OnOutPrompt(0,0);
					}
				}
			}

			//播放声音
			if (m_bHandCardCount[0]<m_bTurnCardCount) return true;
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//开始游戏
		{
			//超时三次自动托管
			if ((m_wTimeOutCount>=3)&&(m_bStustee==false))OnStusteeControl(0,0);
			//超时判断
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(30))))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				{
					if (m_bStustee==false) m_wTimeOutCount++;
					OnStart(0,0);
				}
				return false;
			}
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//显示设置
	bool bDisplay=(IsLookonMode()==false)||(IsAllowLookon()==true);
	m_GameClientView.m_HandCardControl[0].SetDisplayFlag(bDisplay);
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag(bDisplay);

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
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				/*m_GameClientView.m_HandCardControl[0].SetDisplayFlag(true);
				m_GameClientView.m_HandCardControl[1].SetDisplayFlag(true);*/
			}

			//显示设置
			bool bDisplay=(IsLookonMode()==false)||(IsAllowLookon()==true);
			m_GameClientView.m_HandCardControl[0].SetDisplayFlag(bDisplay);
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag(bDisplay);

			//设置时间
			if (IsLookonMode()==false) SetGameTimer(GetMeChairID(),IDI_START_GAME,20);

			return true;
		}
	case GS_RF_PLAYING:		//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//设置变量
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount,true);
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);

			//设置扑克
			WORD wMeChairID=GetMeChairID();
			m_bHandCardCount[0]=pStatusPlay->bCardCount[wMeChairID];
			m_bHandCardCount[1]=pStatusPlay->bCardCount[(wMeChairID+2)%GAME_PLAYER];
			CopyMemory(m_bHandCardData[0],pStatusPlay->cbCardData[0],sizeof(pStatusPlay->cbCardData[0]));
			CopyMemory(m_bHandCardData[1],pStatusPlay->cbCardData[1],sizeof(pStatusPlay->cbCardData[1]));
			m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
			m_GameLogic.SortCardList(m_bHandCardData[1],m_bHandCardCount[1],m_cbSortType);
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
			m_GameClientView.m_HandCardControl[0].SetCardData(m_bHandCardData[1],m_bHandCardCount[1]);

			//设置界面
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
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

			//设置定时器
			WORD wTimeCount=30;
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,wTimeCount);

			//设置扑克
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl[0].SetPositively(true);
				m_GameClientView.m_HandCardControl[1].SetPositively(true);
			}

			//显示设置
			bool bDisplay=(IsLookonMode()==false)||(IsAllowLookon()==true);
			m_GameClientView.m_HandCardControl[0].SetDisplayFlag(bDisplay);
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag(bDisplay);

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
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//历史记录
	m_HistoryCard.ResetData();

	//设置界面
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	for (WORD i=0;i<2;i++)
	{
		m_GameClientView.m_OtherCardControl[i].SetCardData(NULL,0);
	}
	if (IsLookonMode()==true)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.SetUserTaxis(INVALID_CHAIR,0);
		m_GameClientView.SetUserScore(CountArray(m_lGameScore),0);
	}

	//设置扑克
	m_bHandCardCount[0]=CountArray(pGameStart->cbCardData[0]);
	m_bHandCardCount[1]=CountArray(pGameStart->cbCardData[1]);
	CopyMemory(m_bHandCardData[0],pGameStart->cbCardData[0],sizeof(pGameStart->cbCardData[0]));
	CopyMemory(m_bHandCardData[1],pGameStart->cbCardData[1],sizeof(pGameStart->cbCardData[1]));
	m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
	m_GameLogic.SortCardList(m_bHandCardData[1],m_bHandCardCount[1],m_cbSortType);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
	m_GameClientView.m_HandCardControl[0].SetCardData(m_bHandCardData[1],m_bHandCardCount[1]);

	//扑克数目
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_bCardCount[i]=CountArray(pGameStart->cbCardData[0]);
		m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
	}

	//控制设置
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl[1].SetPositively(bPlayerMode);
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag(bPlayerMode);
	m_GameClientView.m_HandCardControl[0].SetDisplayFlag(bPlayerMode);

	//当前玩家
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==GetMeChairID()))
	{
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
	}

	//环境设置
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,30);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//播放声音
void CGameClientDlg::PlaySound(BYTE Type)
{
	switch(Type)
	  {
	    case CT_BOMB:              
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("WoZha"));
						break;
        case CT_DOUBLE_BOMB:           
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("WoZha"));
						break;
        case CT_CRURA_FAKE:       
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("DaZha")); 
						break;
        case CT_KING_LITTET:         
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("DaZha"));
						break;
        case CT_KING_BIG: 
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("DaZha"));
						break;
        case CT_CRURA_PURE: 
			            PlayGameSound(AfxGetInstanceHandle(),TEXT("DaZha"));
						break;
        default :
                 PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
      }
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

	//删除定时器
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//当前扑克
	SwitchToCurrentCard();

	//界面设置
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);

	//出牌设置
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->bCardData,pOutCard->bCardCount);
	}

	//胜利判断
	if (m_bCardCount[pOutCard->wOutCardUser]==0)
	{
		WORD wWinCount=0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_bCardCount[i]==0) wWinCount++;
		}
		m_GameClientView.SetUserTaxis(wOutViewChairID,(BYTE)wWinCount);
	}

	//清理桌面
	if (m_bTurnCardCount==0)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			//保存数据
			m_HistoryCard.OnEventSaveData(i);

			if (i==pOutCard->wOutCardUser) continue;
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//记录出牌
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount,true);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//历史扑克
	m_HistoryCard.OnEventSaveData(pOutCard->wOutCardUser);
	m_HistoryCard.OnEventUserOutCard(pOutCard->wOutCardUser,pOutCard->bCardData,pOutCard->bCardCount);

	//自己扑克
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//删除扑克 
		BYTE bSourceCount=m_bHandCardCount[0];
		m_bHandCardCount[0]-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData[0],bSourceCount);

		//设置界面
		m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
	}

	//对方扑克
	if (pOutCard->wOutCardUser==((GetMeChairID()+2)%GAME_PLAYER))
	{
		//删除扑克 
		BYTE bSourceCount=m_bHandCardCount[1];
		m_bHandCardCount[1]-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData[1],bSourceCount);

		//设置界面
		m_GameLogic.SortCardList(m_bHandCardData[1],m_bHandCardCount[1],m_cbSortType);
		m_GameClientView.m_HandCardControl[0].SetCardData(m_bHandCardData[1],m_bHandCardCount[1]);
	}

	//计算得分
	for (BYTE i=0;i<m_bTurnCardCount;i++)
	{
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==5) m_lGameScore[2]+=5;
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==10) m_lGameScore[2]+=10;
		if ((m_bTurnCardData[i]&LOGIC_MASK_VALUE)==13) m_lGameScore[2]+=10;
	}
	m_GameClientView.SetUserScore(2,m_lGameScore[2]);

	//最大判断
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//累计积分
		WORD wMeChairID=GetMeChairID();
		if ((pOutCard->wOutCardUser==wMeChairID)||(pOutCard->wOutCardUser==((wMeChairID+2)%GAME_PLAYER)))
		{
			m_lGameScore[0]+=m_lGameScore[2];
			m_lGameScore[2]=0L;
			m_GameClientView.SetUserScore(0,m_lGameScore[0]);
			m_GameClientView.SetUserScore(2,m_lGameScore[2]);
		}
		else
		{
			m_lGameScore[1]+=m_lGameScore[2];
			m_lGameScore[2]=0L;
			m_GameClientView.SetUserScore(1,m_lGameScore[1]);
			m_GameClientView.SetUserScore(2,m_lGameScore[2]);
		}

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
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser)) //PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		PlaySound(m_bTurnOutType);

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
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//设置定时器
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		WORD wTimeCount=30;
		SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,wTimeCount);
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

	//当前扑克
	SwitchToCurrentCard();

	//玩家设置
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))
	{
		WORD wViewChairID=SwitchViewChairID(pPassCard->wPassUser);
		m_GameClientView.SetPassFlag(wViewChairID,true);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//历史扑克
	m_HistoryCard.OnEventSaveData(pPassCard->wPassUser);
	m_HistoryCard.OnEventUserPass(pPassCard->wPassUser);

	//一轮判断
	if (pPassCard->bNewTurn==TRUE)
	{
		//累计积分
		WORD wMeChairID=GetMeChairID();
		if ((pPassCard->wCurrentUser==wMeChairID)||(pPassCard->wCurrentUser==((wMeChairID+2)%GAME_PLAYER)))
		{
			m_lGameScore[0]+=m_lGameScore[2];
			m_lGameScore[2]=0L;
			m_GameClientView.SetUserScore(0,m_lGameScore[0]);
			m_GameClientView.SetUserScore(2,m_lGameScore[2]);
		}
		else
		{
			m_lGameScore[1]+=m_lGameScore[2];
			m_lGameScore[2]=0L;
			m_GameClientView.SetUserScore(1,m_lGameScore[1]);
			m_GameClientView.SetUserScore(2,m_lGameScore[2]);
		}

		//设置变量
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
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount>0)?TRUE:FALSE);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//播放声音
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_NOTOUT"));

	//设置时间
	if (m_bTurnCardCount!=0)
	{
		WORD wTimeCount=30;
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,wTimeCount);
	}
	else SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,30);

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

	//删除定时器
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//隐藏控件
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//禁用控件
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btLastTurn.EnableWindow(FALSE);

	//设置积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
	}
	WORD wMeChairID=GetMeChairID();
	tagScoreViewInfo ScoreViewInfo;
	if ((wMeChairID==0)||(wMeChairID==2))
	{
		ScoreViewInfo.lDrawScore[0]=pGameEnd->lDrawScore[0];
		ScoreViewInfo.lDrawScore[1]=pGameEnd->lDrawScore[1];
	}
	else
	{
		ScoreViewInfo.lDrawScore[0]=pGameEnd->lDrawScore[1];
		ScoreViewInfo.lDrawScore[1]=pGameEnd->lDrawScore[0];
	}
	m_GameClientView.m_ScoreView.SetScoreViewInfo(ScoreViewInfo);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//显示扑克
	if (IsLookonMode()==true) 
	{
		m_GameClientView.m_HandCardControl[0].SetDisplayFlag(true);
		m_GameClientView.m_HandCardControl[1].SetDisplayFlag(true);
	}

	//设置扑克
	BYTE bCardPos=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (wViewChairID==0) m_GameClientView.m_HandCardControl[0].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		else if (wViewChairID==1) m_GameClientView.m_OtherCardControl[0].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		else if (wViewChairID==2) m_GameClientView.m_HandCardControl[1].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);
		else if (wViewChairID==3) m_GameClientView.m_OtherCardControl[1].SetCardData(&pGameEnd->bCardData[bCardPos],pGameEnd->bCardCount[i]);

		bCardPos+=pGameEnd->bCardCount[i];
		if (pGameEnd->bCardCount[i]!=0)
		{
			m_GameClientView.SetPassFlag(wViewChairID,false);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}
	}

	//播放声音
	LONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,20);//设置一局游戏结束后 倒记时开始时间
	}

	//成绩输出
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		TCHAR szBuffer[64];
		const tagUserData * pUserData=GetUserData(i);

		//成绩输出
		if (pUserData!=NULL)
		{
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s：%ld"),pUserData->szName,pGameEnd->lGameScore[i]);
			InsertGeneralString(szBuffer,RGB(255,0,255),true);//255,128,0
		}
	}

	return true;
}

//出牌判断
bool CGameClientDlg::VerdictOutCard()
{
	//状态判断
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//获取扑克
	BYTE bCardData[MAX_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(bCardData,CountArray(bCardData));

	//出牌判断
	if (bShootCount>0L)
	{
		//排列扑克
		m_GameLogic.SortCardList(bCardData,bShootCount,ST_ORDER);

		//分析类型
		BYTE bLastType=CT_ERROR;
		if (m_bTurnCardCount>0) bLastType=m_GameLogic.GetCardType(m_bTurnCardData,m_bTurnCardCount,true);
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount,((bLastType==CT_DOUBLE_BOMB)||(bLastType==CT_DOUBLE_LINE)));

		//类型判断
		if (bCardType==CT_ERROR) return false;

		//跟牌判断
		if (m_bTurnCardCount==0) return true;

		//对比扑克
		return m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bShootCount,m_bTurnCardCount);
	}

	return false;
}

//自动出牌
bool CGameClientDlg::AutomatismOutCard()
{
	//先出牌者
	if (m_bTurnCardCount==0)
	{
		//控制界面
		KillGameTimer(IDI_OUT_CARD);
		m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

		//发送数据
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=m_bHandCardData[0][m_bHandCardCount[0]-1];
		SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

		//预先处理
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//预先删除
		BYTE bSourceCount=m_bHandCardCount[0];
		m_bHandCardCount[0]-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData[0],bSourceCount);
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
	}
	else OnPassCard(0,0);

	return true;
}

//定时器消息
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	//上轮
	if (nIDEvent==IDI_LAST_TURN)
	{
		SwitchToCurrentCard();
		return;
	}

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
			if (m_bStustee==false)ActiveGameFrame();
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.EnableWindow(FALSE);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}

		//设置时间
		WORD wTimeCount=20;
		SetGameTimer(wCurrentUser,IDI_OUT_CARD,wTimeCount);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_wTimeOutCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));

	//设置界面
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetUserTaxis(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	m_GameClientView.SetUserScore(CountArray(m_lGameScore),0);

	//隐藏控件
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);//SW_HIDE    SW_SHOW

	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	for (WORD i=0;i<2;i++)
	{
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_OtherCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetPositively(false);
	}

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//出牌消息
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//状态判断
	if ((m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE)||
		(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)) return 0;

	//设置界面
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);
	
	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList(OutCard.bCardData,OutCard.bCardCount,ST_ORDER);
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//预先显示
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	BYTE OutCardType=m_GameLogic.GetCardType(OutCard.bCardData , OutCard.bCardCount ,true);
	PlaySound(OutCardType);
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//预先删除
	BYTE bSourceCount=m_bHandCardCount[0];
	m_bHandCardCount[0]-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData[0],bSourceCount);
	m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);

	return 0;
}

//放弃出牌
LRESULT CGameClientDlg::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//状态判断
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
	m_GameClientView.SetPassFlag(2,true);
	m_GameClientView.m_HandCardControl[1].ShootAllCard(false);
	m_GameClientView.m_UserCardControl[2].SetCardData(NULL,0);

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_NOTOUT"));

	return 0;
}

//出牌提示
LRESULT CGameClientDlg::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//出牌分析
	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	BYTE cbCardData[27];
	CopyMemory(cbCardData,m_bHandCardData[0],sizeof(BYTE)*m_bHandCardCount[0]);
	m_GameLogic.SortCardList(cbCardData,m_bHandCardCount[0],ST_ORDER);
	bool bSuccess=m_GameLogic.SearchOutCard(cbCardData,m_bHandCardCount[0],m_bTurnCardData,m_bTurnCardCount,m_bTurnOutType,OutCardResult);

	//出牌处理
	if ((bSuccess==true)&&(OutCardResult.cbCardCount>0))
	{
		//弹起扑克
		m_GameClientView.m_HandCardControl[1].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//控件控制
		OnLeftHitCard(0,0);
		if(m_bStustee)OnOutCard(0,0);
	}
	else OnPassCard(0,0);

	return 0;
}

//右键扑克
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//出牌状态
	if (m_GameClientView.m_btOutCard.IsWindowVisible())
	{
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);
	}

	return 0;
}

//左键扑克
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//用户出牌
	OnOutCard(0,0);

	return 0;
}

//排列扑克
LRESULT CGameClientDlg::OnSortCard(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_cbSortType=(m_cbSortType==ST_VIEW)?ST_BOMB:ST_VIEW;

	//设置界面
	m_GameLogic.SortCardList(m_bHandCardData[0],m_bHandCardCount[0],m_cbSortType);
	m_GameLogic.SortCardList(m_bHandCardData[1],m_bHandCardCount[1],m_cbSortType);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[0],m_bHandCardCount[0]);
	m_GameClientView.m_HandCardControl[0].SetCardData(m_bHandCardData[1],m_bHandCardCount[1]);

	return 0;
}

//拖管控制
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	m_wTimeOutCount=0;
	//设置状态
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false);
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false);
	}

	return 0;
}

//查看上轮
LRESULT CGameClientDlg::OnMessageLastTurn(WPARAM wParam, LPARAM lParam)
{
	if (m_bLastTurn==false)
	{
		//设置扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//变量定义
			WORD wViewChairID=SwitchViewChairID(i);
			BYTE cbEventFlag=m_HistoryCard.GetHistoryEvent(i);

			//获取扑克
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_HistoryCard.GetHistoryCard(i,cbCardData,CountArray(cbCardData));

			//设置界面
			m_GameClientView.SetPassFlag(wViewChairID,cbEventFlag==AF_PASS);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
		}

		//环境设置
		m_bLastTurn=true;
		SetTimer(IDI_LAST_TURN,3000,NULL);
		m_GameClientView.SetLastTurnCard(true);

		//更新界面
		m_GameClientView.UpdateGameView(NULL);
	}
	else SwitchToCurrentCard();

	return 0;
}

//启用上轮
LRESULT CGameClientDlg::OnMessageEnableHistory(WPARAM wParam, LPARAM lParam)
{
	//启用上轮
	m_GameClientView.m_btLastTurn.EnableWindow(TRUE);

	return 0;
}

//当前扑克
bool CGameClientDlg::SwitchToCurrentCard()
{
	//处理判断
	if (m_bLastTurn==false) return false;

	//环境设置
	m_bLastTurn=false;
	KillTimer(IDI_LAST_TURN);
	m_GameClientView.SetLastTurnCard(false);

	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);
		BYTE cbEventFlag=m_HistoryCard.GetCurrentEvent(i);

		//获取扑克
		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount=m_HistoryCard.GetCurrentCard(i,cbCardData,CountArray(cbCardData));

		//设置界面
		m_GameClientView.SetPassFlag(wViewChairID,cbEventFlag==AF_PASS);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
	}

	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	return true;
}


//////////////////////////////////////////////////////////////////////////

