#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
//////////////////////////////////////////////////////////////////////////
//宏定义

//游戏定时器
#define IDI_OUT_CARD					200								//出牌定时器
#define IDI_START_GAME					201								//开始定时器
#define IDI_WAIT_START					202								//举手定时器
#define IDI_SHOW_CARD                   203                             //亮牌定时器
#define IDI_LAST_CARD_STOP              204                             //停留定时器

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_SHOW_CARD,OnShowCard)	
	ON_MESSAGE(IDM_NOT_SHOW_CARD,OnNotShowCard)
    ON_MESSAGE(IDM_AUTO_OUT_CARD,OnAutoOutCard)
	ON_MESSAGE(IDM_CANCEL_AUTO_OUT_CARD,OnCancelAutoOutCard)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{

	//游戏变量
	ZeroMemory(m_bCardCount , sizeof(m_bCardCount)) ;
	m_bHandCardCount               = 0;	
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
	m_bFirstGame = true ;
	m_bAutoOutCard = false ;
	
	//出牌变量
	memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;					//出牌列表
	m_bTurnCardCount = 0 ;
	m_wLastOutUser   = INVALID_CHAIR ;
	m_wFirstOutUser  = INVALID_CHAIR ;
	m_bFirstOutSpade = true;
	m_bFirstHearts   = true ;  
	m_bFirstClubs    = true ;   
	m_bFirstDiamonds = true ;
	m_bFirstOutCard  = false ;


	//配置变量
	m_bDeasilOrder                 = false ;			        			//出牌顺序
	m_dwCardHSpace                 = DEFAULT_PELS ;	    					//扑克象素
	m_bTimerElapse                 = 30 ;
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
	SetWindowText(TEXT("拱猪游戏"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//读取配置
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	//调整参数

	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//配置控件
	m_GameClientView.SetUserOrder(m_bDeasilOrder);
	m_GameClientView.m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);


	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//游戏变量
	m_bHandCardCount=0;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bFirstGame = true ;
	m_bAutoOutCard = false ;

	//出牌变量
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	m_bTurnCardCount = 0;
	m_wLastOutUser   = INVALID_CHAIR ;
	m_wFirstOutUser  = INVALID_CHAIR ;
	m_bFirstOutSpade = true;
	m_bFirstHearts   = true ;  
	m_bFirstClubs    = true ;   
	m_bFirstDiamonds = true ;
	m_bFirstOutCard  = false ;

	//配置变量
	m_bDeasilOrder                 = false ;			        			//出牌顺序
	m_dwCardHSpace                 = DEFAULT_PELS ;	    					//扑克象素
	m_bTimerElapse                 = 30 ;

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
		m_GameClientView.m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);

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
			//超时判断
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) AutomatismOutCard();
				return false;
			}

			//播放声音
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//开始游戏
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnStart(0,0);
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_SHOW_CARD:        //玩家亮牌
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnNotShowCard(0,0);
				 m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

		}
	case IDI_LAST_CARD_STOP:   //扑克停留
		{
			if(nElapse==0)
			{
				//清除桌面
				for (WORD i=0;i<GAME_PLAYER;i++) 
				{	
					m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
					memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;
				}	
			}
			return true ;
		}
	case IDI_WAIT_START:
		{
			if (nElapse==0)
			{
				if (IsLookonMode()==false) this->PostMessage(WM_CLOSE,0,0);
				return false;
			}
			return true;
		}
	}

	return false;
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
	case SUB_S_SEND_CARD:		//发送扑克
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//用户出牌
		{		
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SHOW_CARD:      //玩家亮牌
		{
			return OnSubShowCard(pBuffer , wDataSize) ;
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
	case GS_WK_FREE:	//空闲状态
		{
			//设置界面
		
			//设置控件
			if (IsLookonMode()==false)
			{
				
				if(FALSE==m_GameClientView.m_btStart.IsWindowVisible())
				{
					m_GameClientView.m_btStart.ShowWindow(TRUE);
					m_GameClientView.m_btStart.SetFocus();
				}
			}
			//设置扑克
			if (IsLookonMode()==false) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

			return true;
		}
	case GS_WK_SHOWCARD:	//玩家亮牌
		{
			//效验数据
			if(wDataSize!=sizeof(CMD_S_StatusShowCard)) return false ;
			CMD_S_StatusShowCard *pSatusShowCard = (CMD_S_StatusShowCard*)pBuffer ;

			
			//设置变量
			m_bHandCardCount=HAND_CARD_COUNT;
			WORD wViewChairID=SwitchViewChairID(pSatusShowCard->wCurrentUser);
			for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=HAND_CARD_COUNT;
			CopyMemory(m_bHandCardData,pSatusShowCard->bCardData,m_bHandCardCount);
			m_GameClientView.SetCardCount(wViewChairID , m_bHandCardCount) ;
			m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount); 

			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);  
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				m_GameClientView.SetCardCount(i , HAND_CARD_COUNT) ;
				wViewChairID = SwitchViewChairID(i) ;
				if(0!=pSatusShowCard->ShowCardCount[i])	m_GameClientView.SetShowCard(wViewChairID , pSatusShowCard->ShowCard[i] , pSatusShowCard->ShowCardCount[i]) ;
			}
	
					//按钮控制
			if ((IsLookonMode()==false)&&(pSatusShowCard->wCurrentUser==GetMeChairID()) && 0==pSatusShowCard->ShowCardCount[pSatusShowCard->wCurrentUser])
			{
				//设置扑克
				m_GameClientView.m_HandCardControl.SetPositively(true) ;

				bool bCanShowCard=false ;
				BYTE bCardColor ;
				BYTE bCardValue ;
				BYTE bCardDataIndex[4] ;
				BYTE dwCardCount=0 ; 
				for(BYTE i=0 ; i<HAND_CARD_COUNT ; ++i)
				{
					bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i]) ;
					//判断扑克
					if((3==bCardColor && 12==bCardValue) || //黑桃Q
						(2==bCardColor && 1==bCardValue)  || //红桃A
						(0==bCardColor && 11==bCardValue) || //方块J
						(1==bCardColor && 10==bCardValue))   //梅花10
					{
						m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
						m_GameClientView.m_btShowCard.EnableWindow(TRUE);

						m_GameClientView.m_btNotShowCard.ShowWindow(SW_SHOW);
						m_GameClientView.m_btNotShowCard.EnableWindow(TRUE);

						m_GameClientView.m_bShowCardHint = true ;

						//设置时间
						SetGameTimer(pSatusShowCard->wCurrentUser,IDI_SHOW_CARD,m_bTimerElapse);

						bCanShowCard = true ;

						//保存亮牌
						bCardDataIndex[dwCardCount++] = i ;
					}
				}

				//不能亮牌
				if(false==bCanShowCard)
				{
					CMD_C_ShowCard  ShowCard ;
					memset(&ShowCard , 0 , sizeof(ShowCard)) ;

					SendData(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard)) ;
				}
				else    //设置亮牌 
					m_GameClientView.m_HandCardControl.SetShootCard(bCardDataIndex , dwCardCount) ;
			}

			return true ;
		}
	case GS_WK_PLAYING:		//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
		
			//设置变量
			m_bHandCardCount=pStatusPlay->bCardCount[pStatusPlay->wCurrentUser];

			CopyMemory(m_bHandCardData,pStatusPlay->bCardData,m_bHandCardCount);			
			//设置界面
	
			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				WORD wViewChairID = SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID , pStatusPlay->bCardCount[i]) ;
				m_bCardCount[i]= pStatusPlay->bCardCount[i] ;
			}
			if(0!=m_bHandCardCount)	
				m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount); 
			m_wLastOutUser = pStatusPlay->wLastOutUser ;
			m_wFirstOutUser= pStatusPlay->wFirstOutUser ;

			for(BYTE i=0 ; i<GAME_PLAYER ; ++i)
			{
				WORD wViewChairID = SwitchViewChairID(i);
				if(0!=pStatusPlay->bScoreCardCount[i])  m_GameClientView.AddScoreCard(wViewChairID , pStatusPlay->ScoreCard[i] , pStatusPlay->bScoreCardCount[i]) ;
				if(0!=pStatusPlay->bShowCardCount[i])   m_GameClientView.SetShowCard(wViewChairID , pStatusPlay->ShowCard[i] , pStatusPlay->bShowCardCount[i]) ;
			}
			CopyMemory(m_bTurnCardData , pStatusPlay->bTurnCardData , sizeof(pStatusPlay->bTurnCardData)) ;
			    
			
			//玩家设置
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);  
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			}

			//设置定时器
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);

			//设置扑克
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);  
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

			if(false==IsLookonMode())
			{
				//设置界面
				m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
				m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;
			}
			return true;
		}
	}	

	return true;
}

//发送扑克
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//设置状态
	SetGameStatus(GS_WK_SHOWCARD);

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	//设置数据
	m_bHandCardCount=CountArray(pSendCard->bCardData);  
	CopyMemory(m_bHandCardData,pSendCard->bCardData,sizeof(pSendCard->bCardData));

	//设置界面
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetCardCount(i,HAND_CARD_COUNT);
		m_bCardCount[i] = HAND_CARD_COUNT ;
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
	
	//设置扑克
	m_GameClientView.m_HandCardControl.SetCardData(pSendCard->bCardData,HAND_CARD_COUNT);

	if (IsLookonMode()==true) 
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.m_HandCardControl.SetDisplayFlag(false);
	}

	//设置按钮
	if ((IsLookonMode()==false)&&(pSendCard->wCurrentUser==GetMeChairID()))
	{
		//设置扑克
		m_GameClientView.m_HandCardControl.SetPositively(true) ;

		bool bCanShowCard=false ;
		BYTE bCardColor ;
		BYTE bCardValue ;
		BYTE bCardDataIndex[4] ;
		BYTE dwCardCount=0 ; 
		for(BYTE i=0 ; i<HAND_CARD_COUNT ; ++i)
		{
            bCardColor = m_GameLogic.GetCardColor(pSendCard->bCardData[i]) ;
			bCardColor = bCardColor>>4 ;
			bCardValue = m_GameLogic.GetCardValue(pSendCard->bCardData[i]) ;
			//判断扑克
			if((3==bCardColor && 12==bCardValue) || //黑桃Q
			   (2==bCardColor && 1==bCardValue)  || //红桃A
			   (0==bCardColor && 11==bCardValue) || //方块J
			   (1==bCardColor && 10==bCardValue))   //梅花10
			{
               m_GameClientView.m_btShowCard.ShowWindow(SW_SHOW);
			   m_GameClientView.m_btShowCard.EnableWindow(TRUE);

			   m_GameClientView.m_btNotShowCard.ShowWindow(SW_SHOW);
               m_GameClientView.m_btNotShowCard.EnableWindow(TRUE);

			   m_GameClientView.m_bShowCardHint = true ;

			   //设置时间
			   SetGameTimer(pSendCard->wCurrentUser,IDI_SHOW_CARD,m_bTimerElapse);

			   bCanShowCard = true ;

			   //保存亮牌
			   bCardDataIndex[dwCardCount++] = i ;
			}
		}

		//不能亮牌
		if(false==bCanShowCard)
		{
			CMD_C_ShowCard  ShowCard ;
			memset(&ShowCard , 0 , sizeof(ShowCard)) ;

			SendData(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard)) ;
		}
		else    //设置亮牌 
			m_GameClientView.m_HandCardControl.SetShootCard(bCardDataIndex , dwCardCount) ;
	}

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//游戏变量
	memset(m_bCardCount , HAND_CARD_COUNT , GAME_PLAYER) ;       	    	
	m_bHandCardCount               = HAND_CARD_COUNT;					                    

	//出牌变量
	memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;					
	m_bTurnCardCount = 0 ;
	m_wLastOutUser   = INVALID_CHAIR ;
	m_wFirstOutUser  = INVALID_CHAIR ;

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	SetGameStatus(GS_WK_PLAYING);
	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	m_bFirstGame   = pGameStart->bFirstGame ;

	//玩家设置
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_HandCardControl.SetPositively(true);
	}

	//设置玩家
	if(false==IsLookonMode() && pGameStart->wCurrentUser==GetMeChairID())
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	}

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//设置定时器
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);

	if(false==IsLookonMode())
	{
		//设置界面
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;
	}
	return true;
}

//用户亮牌
bool CGameClientDlg::OnSubShowCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ShowCard));
	if (wDataSize!=sizeof(CMD_S_ShowCard)) return false;

	CMD_S_ShowCard *pShowCard = (CMD_S_ShowCard*)pBuffer ;

	WORD wChairID = SwitchViewChairID(pShowCard->wShowCardUser) ;

	//数据校验
	ASSERT(wChairID<GAME_PLAYER) ;
	if(wChairID>=GAME_PLAYER) return false ;

	BYTE bCardColor ;
	BYTE bCardValue ;
	for(BYTE i=0 ; i<pShowCard->bShowCardCount ; ++i)
	{
		bCardColor = m_GameLogic.GetCardColor(pShowCard->bShowCard[i]) ;
		bCardColor = bCardColor>>4 ;
		bCardValue = m_GameLogic.GetCardValue(pShowCard->bShowCard[i]) ;

		//数据校验
		ASSERT(0!=pShowCard->bShowCard[i]) ;
		if(0==pShowCard->bShowCard[i]) return false ;
		ASSERT((3==bCardColor && 12==bCardValue) || //黑桃Q
			   (2==bCardColor && 1==bCardValue)  || //红桃A
			   (0==bCardColor && 11==bCardValue) || //方块J
			   (1==bCardColor && 10==bCardValue));  //梅花10

	}
	//保存亮牌
	if(0!=pShowCard->bShowCardCount) m_GameClientView.SetShowCard(wChairID , pShowCard->bShowCard , pShowCard->bShowCardCount) ;

	return true ;
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
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData)));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData))) return false;

	//删除定时器
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_LAST_CARD_STOP) ;

	//清楚桌面
	if(0==m_bTurnCardCount)
	{
		m_wFirstOutUser = pOutCard->wOutCardUser ;

		for (WORD i=0;i<GAME_PLAYER;i++) 
		{	
			m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
			memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;
		}	
		if(GetMeChairID()==pOutCard->wOutCardUser) m_GameClientView.m_UserCardControl[2].SetCardData(&pOutCard->bCardData,pOutCard->bCardCount);
	}

	//界面设置
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	m_bCardCount[pOutCard->wOutCardUser] -= pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);

	//出牌设置
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(&pOutCard->bCardData,pOutCard->bCardCount);
	}

	//保存出牌
	CopyMemory(&m_bTurnCardData[pOutCard->wOutCardUser],&pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);
	m_wLastOutUser = pOutCard->wOutCardUser ;
	m_bTurnCardCount++ ;

	//出牌变量
	if(GAME_PLAYER==m_bTurnCardCount)
	{	//最大出牌
		WORD wMostUser = m_wFirstOutUser ;
		BYTE bFirstCardColor = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
		bFirstCardColor = bFirstCardColor >> 4 ;
		BYTE bMostCardValue  = m_GameLogic.GetCardValue(m_bTurnCardData[m_wFirstOutUser]) ;

		if(1==bMostCardValue) bMostCardValue += 13 ;
        //得分玩家
		for(WORD i=0 ; i<GAME_PLAYER ; ++i)
		{
			BYTE bCurrentCarColor  = m_GameLogic.GetCardColor(m_bTurnCardData[i]) ;
			bCurrentCarColor = bCurrentCarColor >> 4 ;
			BYTE bCurrentCardValue = m_GameLogic.GetCardValue(m_bTurnCardData[i]) ;
			if(1==bCurrentCardValue) bCurrentCardValue += 13 ;
			if(bCurrentCarColor == bFirstCardColor)
				if(bMostCardValue < bCurrentCardValue)
				{
					bMostCardValue = bCurrentCardValue ;
					wMostUser = i ;
				}
		}

		//获取分数
		BYTE bScoreCard[GAME_PLAYER] ;
		BYTE bScoreCount=0 ;
		if(true==m_GameLogic.GetScoreCard(m_bTurnCardData , bScoreCard , GAME_PLAYER , bScoreCount))
		{
			m_GameClientView.AddScoreCard(SwitchViewChairID(wMostUser) , bScoreCard , bScoreCount) ;
		}

		//出牌列表		
		memset(m_bTurnCardData , 0 , sizeof(m_bTurnCardData)) ;					
		m_bTurnCardCount = 0 ;
		m_wLastOutUser   = INVALID_CHAIR ;
		m_wFirstOutUser  = INVALID_CHAIR ;

		SetGameTimer(pOutCard->wCurrentUser,IDI_LAST_CARD_STOP,2);
	}
	    
	//自己扑克
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//删除扑克 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(&pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);
		
		//设置界面
		m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//播放声音

	BYTE bCardColor  = m_GameLogic.GetCardColor(pOutCard->bCardData) ;
	bCardColor = bCardColor >> 4 ;
	BYTE bCardValue = m_GameLogic.GetCardValue(pOutCard->bCardData) ;
	if(3==bCardColor && 12==bCardValue) PlayGameSound(AfxGetInstanceHandle(),TEXT("PIG_CARD"));
	else if (0==bCardColor && 11==bCardValue) PlayGameSound(AfxGetInstanceHandle(),TEXT("SHEEP_CARD"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	
	//玩家设置
	if (pOutCard->wCurrentUser==GetMeChairID()&&IsLookonMode()==false)
	{		
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_HandCardControl.ShootAllCard(false) ;

		//设置定时器
		SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);
	}

	return true;
}
//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
//	InsertSystemString("CGameClientDlg::OnSubGameEnd");
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//删除定时器
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_SHOW_CARD);

	//隐藏控件
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	//禁用控件
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	//设置界面
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btCancelAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btCancelAutoOutCard.EnableWindow(FALSE) ;
	SetGameStatus(GS_WK_FREE);

	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}

	//显示扑克
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

	//设置积分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i]);
	}
	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//设置界面
	if (IsLookonMode()==false)
	{
		if(FALSE==m_GameClientView.m_btStart.IsWindowVisible())
		{
			m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	    	m_GameClientView.m_btStart.UpdateWindow();
		}
		m_GameClientView.m_bSendWaiMsg=true;
		//SetGameTimer(GetMeChairID(),IDI_START_GAME,90);
	}

	//设置变量
	m_GameClientView.AddScoreCard(INVALID_CHAIR , NULL , 0) ;
	m_GameClientView.SetShowCard(INVALID_CHAIR , NULL , 0) ;

	return true;
}
//出牌判断
bool CGameClientDlg::VerdictOutCard(bool bAutoOutCard)
{
	//状态判断
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,HAND_CARD_COUNT);

	//出牌判断
	if (bShootCount==1)
	{
		if(true==m_bFirstGame)
		{
			//首牌判断
			bool bFisrCard=true ;
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
				if(HAND_CARD_COUNT!=m_bCardCount[i]) bFisrCard = false ;

			if(true==bFisrCard)
			{
				BYTE bCardColor = m_GameLogic.GetCardColor(bCardData[0]) ;
				bCardColor = bCardColor>>4 ;
				BYTE bCardValue = m_GameLogic.GetCardValue(bCardData[0]) ;
				if(1==bCardColor && 2==bCardValue) 
				{
					m_bFirstClubs = false ;
					return true ;
				}
				if(false==bAutoOutCard)	MessageBox("第一局的第一张牌必须先出梅花2！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
				m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
				return false ;
			}
		}

		if(INVALID_CHAIR==m_wFirstOutUser) 
		{
			BYTE bCardColor = m_GameLogic.GetCardColor(bCardData[0]) ;
			bCardColor      = bCardColor>>4 ; 
			return VerdictFirstGameColor(bCardColor , bCardData[0] , true) ;
		}
		BYTE bFirstOutColor = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
		bFirstOutColor = bFirstOutColor>>4 ;
		BYTE bCurrentColor = m_GameLogic.GetCardColor(bCardData[0]) ;
		bCurrentColor = bCurrentColor>>4 ;

		//是否垫牌
		if(bFirstOutColor!=bCurrentColor) 
		{
			for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
			{
				bCurrentColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
				bCurrentColor = bCurrentColor >> 4 ;
				if(bFirstOutColor==bCurrentColor)
				{
					if(false==bAutoOutCard)	MessageBox("您必须出和第一张牌相同花色的牌！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
					m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
					return false ;
				}
			}
			return true ;
		}
		else
			return VerdictFirstGameColor(bFirstOutColor , bCardData[0] ,true) ;
		
		
	}

	return false;
}
//第一轮花色
bool  CGameClientDlg::VerdictFirstGameColor(BYTE bFirstOutColor , BYTE bCardData , bool bAutoOutCard)
{
	BYTE bCurrentColor = m_GameLogic.GetCardColor(bCardData) ;
	bCurrentColor = bCurrentColor>>4 ;

	//第一轮花色
	switch(bFirstOutColor)
	{
	case 0:      //方块
		{
			//恢复变量
			if(true==m_bFirstOutCard) m_bFirstDiamonds = true ;

			//第一轮判断
			if(true==m_bFirstDiamonds)
			{
				m_bFirstDiamonds = false ;
				bool bIsShowCard = false ;

				//是否亮牌
				for(BYTE i=0 ; i<m_GameClientView.m_bShowCardCount[2] ; ++i)
					if(bCardData==m_GameClientView.m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
						{
							if(bCardData==m_bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								if(false==bAutoOutCard) MessageBox("亮过的牌不能在该花色首轮出牌时出！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
								m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
								//恢复变量
								m_bFirstDiamonds = true ;
								return false ;
							}
						}

						return true ;
					}
					else
					{
						//恢复变量
						m_bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 1:     //梅花
		{
			//恢复变量
			if(true==m_bFirstOutCard) m_bFirstClubs = true ;

			//第一轮判断
			if(true==m_bFirstClubs)
			{
				m_bFirstClubs = false ;
				bool bIsShowCard = false ;

				//是否亮牌
				for(BYTE i=0 ; i<m_GameClientView.m_bShowCardCount[2] ; ++i)
					if(bCardData==m_GameClientView.m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
						{
							if(bCardData==m_bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								if(false==bAutoOutCard) MessageBox("亮过的牌不能在该花色首轮出牌时出！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
								m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
								//恢复变量
								m_bFirstClubs = true ;
								return false ;
							}
						}

						return true ;
					}
					else
					{
						//恢复变量
						m_bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 2:     //红桃
		{
			//恢复变量
			if(true==m_bFirstOutCard) m_bFirstHearts = true ;

			if(true==m_bFirstHearts)
			{
				m_bFirstHearts = false ;
				bool bIsShowCard = false ;

				//是否亮牌
				for(BYTE i=0 ; i<m_GameClientView.m_bShowCardCount[2] ; ++i)
					if(bCardData==m_GameClientView.m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
						{
							if(bCardData==m_bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								if(false==bAutoOutCard) MessageBox("亮过的牌不能在该花色首轮出牌时出！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
								m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
								//恢复变量
								m_bFirstHearts = true ;
								return false ;
							}
						}
						return true ;
					}
					else
					{
						//恢复变量
						m_bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	case 3:     //黑桃
		{
			//恢复变量
			if(true==m_bFirstOutCard) m_bFirstHearts = true ;

			if(true==m_bFirstOutSpade)
			{
				m_bFirstOutSpade = false ;
				bool bIsShowCard = false ;

				//是否亮牌
				for(BYTE i=0 ; i<m_GameClientView.m_bShowCardCount[2] ; ++i)
					if(bCardData==m_GameClientView.m_bShowCard[2][i]) 
					{
						bIsShowCard = true ;
						break;
					}

					if(true==bIsShowCard)
					{
						for(BYTE i=0 ; i<m_bHandCardCount ; ++i)
						{
							if(bCardData==m_bHandCardData[i]) continue ;
							BYTE bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							if(bCardColor==bCurrentColor) 
							{
								if(false==bAutoOutCard) MessageBox("亮过的牌不能在该花色首轮出牌时出！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
								m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
								//恢复变量
								m_bFirstOutSpade = true ;
								return false ;
							}
						}
						return true ;
					}
					else 
					{
						//恢复变量
						m_bFirstOutCard = true ;
						return true ;
					}
			}		
			else return true ;

			return true ;
		}
	default:    //错误
		return false ;
	}
	return true ;
}

//亮牌判断
bool  CGameClientDlg::VerdictShowCard()
{
	//状态判断
	if (m_GameClientView.m_btShowCard.IsWindowVisible()==FALSE) return false;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,HAND_CARD_COUNT);

	//判断亮牌
	if(bShootCount>=1 && bShootCount<=4)
	{
		BYTE bCardColor ;
		BYTE bCardValue ;

		BYTE bShowCardCount=0;
		for(BYTE i=0 ; i<bShootCount ; ++i)
		{
			bCardColor = m_GameLogic.GetCardColor(bCardData[i]) ;
			bCardColor = bCardColor>>4 ;
			bCardValue = m_GameLogic.GetCardValue(bCardData[i]) ;
			//判断扑克
			if((3==bCardColor && 12==bCardValue)  || //黑桃Q
				(2==bCardColor && 1==bCardValue)  || //红桃A
				(0==bCardColor && 11==bCardValue) || //方块J
				(1==bCardColor && 10==bCardValue))   //梅花10
			{
				++bShowCardCount;
			}
		}
		return bShootCount==bShowCardCount;
	}
	return false ;
}

//定时器消息
void CGameClientDlg::OnTimer(UINT nIDEvent)
{


	__super::OnTimer(nIDEvent);
}
//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//设置变量
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	m_bAutoOutCard = false ;

	//出牌变量
	m_wFirstOutUser  = INVALID_CHAIR ;
	m_wLastOutUser   = INVALID_CHAIR ;
	m_bFirstOutSpade = true;
	m_bFirstHearts   = true ;  
	m_bFirstClubs    = true ;   
	m_bFirstDiamonds = true ;
	m_bFirstOutCard  = false ;

	//配置变量
	m_bDeasilOrder  = false ;			        			
	m_dwCardHSpace  = DEFAULT_PELS ;	    				
	m_bTimerElapse  = 30 ;

	//设置界面
	KillGameTimer(IDI_START_GAME);
	KillGameTimer(IDI_WAIT_START);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);

	//隐藏控件
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btCancelAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btCancelAutoOutCard.EnableWindow(FALSE) ;

	//设置扑克
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	//设置变量
	m_GameClientView.AddScoreCard(INVALID_CHAIR , NULL , 0) ;
	m_GameClientView.SetShowCard(INVALID_CHAIR , NULL , 0) ;

	//清空扑克
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}
//出牌消息
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//状态判断
	if(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return 0;
	if(m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE) return 0;

	//恢复变量
	m_bFirstOutCard = false ;

	//设置界面
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);

	//发送数据
	CMD_C_OutCard OutCard;
	memset(&OutCard , 0 , sizeof(OutCard)) ;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(&OutCard.bCardData,1);

	SendData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//预先显示
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	m_GameClientView.m_UserCardControl[2].SetCardData(&OutCard.bCardData,OutCard.bCardCount);

	//预先删除
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(&OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}
//左键扑克
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//出牌按钮
	if(TRUE==m_GameClientView.m_btOutCard.IsWindowVisible())
	{
		//设置控件
		bool bResult = VerdictOutCard() ;

		m_GameClientView.m_btOutCard.EnableWindow((true==bResult)?TRUE:FALSE); 
	}

	//亮牌按钮
	if(TRUE==m_GameClientView.m_btShowCard.IsWindowVisible())
	{
		//设置控件
		bool bResult = VerdictShowCard() ;

		m_GameClientView.m_btShowCard.EnableWindow((true==bResult)?TRUE:FALSE); 

	}

	return 0;
}

//右键扑克
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//用户出牌
	bool bResult = VerdictOutCard() ;
	if(false==bResult) return 0 ;
	OnOutCard(0,0);

	return 0;
}

LRESULT CGameClientDlg::OnWaitStart(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	KillGameTimer(IDI_START_GAME);
	SetGameTimer(GetMeChairID(),IDI_WAIT_START,10);
	return 0;
}
//玩家不亮
LRESULT CGameClientDlg::OnNotShowCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;
    
	//删除定时器
	KillGameTimer(IDI_SHOW_CARD);

	CMD_C_ShowCard  ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;

	SendData(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard)) ;

	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);

	m_GameClientView.m_btNotShowCard.ShowWindow(SW_HIDE);

	m_GameClientView.m_HandCardControl.ShootAllCard(false) ;

	m_GameClientView.m_bShowCardHint = false ;
	m_GameClientView.UpdateGameView(NULL);

	return 0 ;
}
//玩家亮牌
LRESULT CGameClientDlg::OnShowCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//删除定时器
	KillGameTimer(IDI_SHOW_CARD);

	//获取扑克
	BYTE bCardData[4];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(bCardData,4);

	CMD_C_ShowCard  ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;

	CopyMemory(ShowCard.bShowCard , bCardData , bShootCount) ;
	ShowCard.bShowCardCount = bShootCount ;

    SendData(SUB_C_SHOW_CART , &ShowCard , sizeof(ShowCard)) ;

    m_GameClientView.m_HandCardControl.ShootAllCard(false) ;

	m_GameClientView.m_btShowCard.ShowWindow(SW_HIDE);

	m_GameClientView.m_btNotShowCard.ShowWindow(SW_HIDE);

	m_GameClientView.m_bShowCardHint = false ;
	m_GameClientView.UpdateGameView(NULL);

   return 0 ;
}
//自动代打
LRESULT CGameClientDlg::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	m_bAutoOutCard = !m_bAutoOutCard ; 

	//设置时间
	if(true==m_bAutoOutCard) m_bTimerElapse = 1 ;

	AutomatismOutCard() ;

	//设置界面
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btCancelAutoOutCard.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btCancelAutoOutCard.EnableWindow(TRUE) ;
	return 0 ;
}
//取消代打
LRESULT CGameClientDlg::OnCancelAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	m_bAutoOutCard = !m_bAutoOutCard ;
	if(!m_bAutoOutCard)	m_bTimerElapse = 30 ;

	//设置界面
	m_GameClientView.m_btCancelAutoOutCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btCancelAutoOutCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
	m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;

	return 0 ;
}
//自动出牌
bool CGameClientDlg::AutomatismOutCard()
{
	//设置扑克
	BYTE bCardDataIndex ;
	m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
	for(BYTE i=m_bHandCardCount-1 ; i!=255 ; --i)
	{
		//数据验证
		ASSERT(i>=0 && i<=m_GameClientView.m_HandCardControl.GetCardCount()) ;
        if(i==255 || i>m_GameClientView.m_HandCardControl.GetCardCount()) return false ;
	
        bCardDataIndex = i ;
		m_GameClientView.m_HandCardControl.SetShootCard(&bCardDataIndex , 1) ;	
		//有效判断
		if(true==VerdictOutCard(true)) 
		{
			m_GameClientView.m_btOutCard.EnableWindow(TRUE) ;
			OnOutCard(0 , 0) ;
			return true ;
		}
		m_GameClientView.m_HandCardControl.ShootAllCard(false) ;
	}
	return false ;
}

//////////////////////////////////////////////////////////////////////////
