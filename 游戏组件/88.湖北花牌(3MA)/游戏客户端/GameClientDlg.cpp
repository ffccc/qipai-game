#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include "gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////

//游戏定时器
#define IDI_START_GAME				200									//开始定时器
#define IDI_OPERATE_CARD			202									//操作定时器
#define IDI_HD					    203								    //海底定时器

//游戏定时器
#define TIME_START_GAME				25									//开始定时器
#define TIME_HEAR_STATUS			10									//出牌定时器
#define TIME_OPERATE_CARD			25									//操作定时器
#define TIME_HD						20									//海底定时器

const BYTE INVALID_CARDINDEX  =  0xFF;			//无效牌索引

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)

	ON_MESSAGE(IDM_TUOGUAN,OnTuoGuan)
	ON_MESSAGE(IDM_DISTUOGUAN,OnDisTuoGuan)

	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_SEND_ONE_CARD_FINISH,OnSendOneCardFinish)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//游戏变量
	m_cbSex = 0;              //默认的是女生
	m_IsTuoGuan=false;
	m_cbSendCardFinish=false;

	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	b_IsHD=false;

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	m_SiTongCount =0;
	m_WuTongCount =0;
	
	FillMemory(m_WuTongIndex,sizeof(m_WuTongIndex),INVALID_CARDINDEX);
	FillMemory(m_SiTongIndex,sizeof(m_SiTongIndex),INVALID_CARDINDEX);

	ZeroMemory(m_cbHuaCardIndex,sizeof(m_cbHuaCardIndex));
	ZeroMemory(m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));

	m_bGameStart = false;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
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
	SetWindowText(TEXT("湖北花牌三精动画版"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载资源
	g_CardResource.LoadResource();
	//得到用户配置的播放声音
	CWinApp * pApp=AfxGetApp();
	m_wPlayNativeSound=pApp->GetProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),1);

	return true;
}

//重置框架--本函数目前没有调用到--!!!!!!!!!!!!!!!!!1
void CGameClientDlg::ResetGameFrame()
{
	m_IsTuoGuan=false;

	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	//m_wHDCurrentUser=INVALID_CHAIR;
	b_IsHD=false;

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	///////////////babala11_12wan
	m_SiTongCount =0;
	m_WuTongCount =0;
	for (BYTE i=0; i<8; i++)
	{
		m_WuTongIndex[i] = INVALID_CARDINDEX;
		m_SiTongIndex[i] = INVALID_CARDINDEX;
	}

	ZeroMemory(m_cbHuaCardIndex,sizeof(m_cbHuaCardIndex));

	//-----11_16xia
	ZeroMemory(m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));

	m_bGameStart = false;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	//构造数据
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();

	//得到用户配置的播放声音
	CWinApp * pApp=AfxGetApp();
	GameOption.m_bPlayNativeSound=pApp->GetProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),1);

	
	//配置数据
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
	}
	m_wPlayNativeSound=GameOption.m_bPlayNativeSound;

	return;
}

//时间消息
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//开始游戏
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return true;
			}
			if (nElapse<=5)
			{
				if(IsEnableSound()==true)
				{
					CString szSoundName=_T("");
					szSoundName="Sound\\";
					szSoundName+="GAME_WARN.wav";
					PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
				}
			}
			DisableOutCard();
			return true;
		}
	case IDI_OPERATE_CARD:		//操作定时器
		{
			//托管判断
			if((nElapse<=TIME_OPERATE_CARD-2) && m_IsTuoGuan && IsLookonMode()==false)
			{
				//获取位置
				WORD wMeChairID=GetMeChairID();

				//动作处理
				if ((wChairID==wMeChairID) && (m_cbSendCardFinish==true))
				{
					//修改为当最后一个区域牌数为0张时，向后寻找第一个非0张区域的最下面一张牌
					if (m_wCurrentUser == wMeChairID)
					{
						BYTE cbCardData = 0;
						for(int i = 8; i >= 0; i--)
						{
							//如果当前区域为找到的第一个
							if(m_GameClientView.m_HandCard[i].m_cbCardCount != 0)
							{
								//得到出牌数据
								cbCardData=m_GameClientView.m_HandCard[i].m_CardItemArray[m_GameClientView.m_HandCard[i].m_cbCardCount-1].cbCardData;				
								m_GameClientView.m_HandCard[i].DeleteByArrayIndex(m_GameClientView.m_HandCard[i].m_cbCardCount-1);
								break;
							}
						}

						OnOutCard(cbCardData,cbCardData);
					}
					else 
					{
						OnCardOperate(WIK_NULL,0);
					}
				}
				//禁止用户出牌操作
				DisableOutCard();
				return true;
			}
			//超时判断
			if ((nElapse==0)&&(IsLookonMode()==false))
			{
				//获取位置
				WORD wMeChairID=GetMeChairID();

				//动作处理
				if ((wChairID==wMeChairID) && (m_cbSendCardFinish==true))
				{
					//当是自已摸牌操作超时的情况下，出最后的一张牌
					if (m_wCurrentUser == wMeChairID)
					{
						//找到最后的一张牌
						BYTE cbCardData = 0;
						for(int i = 8; i >= 0; i--)
						{
							//如果当前区域为找到的第一个
							if(m_GameClientView.m_HandCard[i].m_cbCardCount != 0)
							{
								//得到出牌数据
								cbCardData=m_GameClientView.m_HandCard[i].m_CardItemArray[m_GameClientView.m_HandCard[i].m_cbCardCount-1].cbCardData;
								m_GameClientView.m_HandCard[i].DeleteByArrayIndex(m_GameClientView.m_HandCard[i].m_cbCardCount-1);

								//当前已经得到要出的牌，所以跳出该循环
								break;
							}
						}
						//将最后一张牌打出去
						OnOutCard(cbCardData,cbCardData);
					}
					//如果是别人出牌而操作时，则向服务器发送放弃消息
					else 
					{
						OnCardOperate(WIK_NULL,0);
					}
				}

				//禁止用户出牌操作
				DisableOutCard();
				return true;
			}

			//播放声音
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				if(IsEnableSound()==true)
				{
					CString szSoundName=_T("");
					szSoundName="Sound\\";
					szSoundName+="GAME_WARN.wav";
					PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
				}
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
	case SUB_S_DECIDE_BANKER:	//定庄
		{
			return OnSubGameDecideBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//用户出牌
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发牌消息
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//操作提示
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//操作结果
		{
			return OnSubOperateResult(pBuffer,wDataSize);
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
	case GS_MJ_FREE:	//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//设置数据
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);

			//设置控件
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			//更新界面
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_MJ_PLAY:	//游戏状态
		{
//////////////////////////////////////////////////////////////////////
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//辅助变量
			WORD wViewChairID[GAME_PLAYER]={0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);
//----------恢复相关的变量值------------------------------//
			//恢重庄家
			m_wBankerUser=pStatusPlay->wBankerUser;
			
			//得到用户配置的播放声音
			CWinApp * pApp=AfxGetApp();
			m_wPlayNativeSound=pApp->GetProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),1);

			//恢复各用户手中的牌
			CopyMemory(m_bCardCount,pStatusPlay->cbUserCardCount,sizeof(m_bCardCount));

			//恢复用户四藏的数据
			m_SiTongCount = pStatusPlay->bSiTongCount;
			CopyMemory(m_SiTongIndex,pStatusPlay->bSiTongIndex,sizeof(m_SiTongIndex));

			//恢复用户五藏的数据
			m_WuTongCount = pStatusPlay->bWuTongCount;
			CopyMemory(m_WuTongIndex,pStatusPlay->bWuTongIndex,sizeof(m_WuTongIndex));

			//恢复各用户已组合牌的数据
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			//恢复玩家自己的组合牌中花牌的个数
			CopyMemory(m_cbHuaWeaveCardIndex,pStatusPlay->cbHuaWeaveCardIndex[GetMeChairID()],sizeof(m_cbHuaWeaveCardIndex));
			
			//恢复自己用户手中牌的信息
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);
			//恢复自己用户手中花牌数据
			m_GameLogic.GetHuaCard(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbHuaCardIndex);

			//恢复各用户已出牌的信息
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//游戏是否牌开始状态
			m_bGameStart = pStatusPlay->bGameStart;

			//恢复当前用户的信息
			m_wCurrentUser=pStatusPlay->wCurrentUser;

			//恢复当前出牌的用户及其牌信息
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;

			//恢复当前用户的操作提示及操作牌信息
			m_cbActionMask=pStatusPlay->cbActionMask;
			m_cbActionCard=pStatusPlay->cbActionCard;

			//界面设置
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);
//---------------------------堆牌区数据的恢复--------------------------------------//
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
			//得到堆牌区中头部牌的位置
		//	m_GameClientView.m_leaveHeapCardData=(WORD)m_cbLeftCardCount;
			//m_GameClientView.m_LHeapCard.SetCardData(m_cbLeftCardCount,0,110);
			//得到堆牌区中尾部牌的位置
			BYTE wMinusLastCount=0;
			//用户招牌时要从尾部补牌
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				for(BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					if(m_WeaveItemArray[i][j].cbWeaveKind==WIK_SAN_ZHAO)
					{
						wMinusLastCount++;
					}
					else if(m_WeaveItemArray[i][j].cbWeaveKind==WIK_SI_ZHAO)
					{
						wMinusLastCount+=2;
					}
				}
			}
			//用户藏牌时，也要从尾部补牌
			wMinusLastCount+=pStatusPlay->bSiTongAllCount;
			wMinusLastCount+=pStatusPlay->bWuTongAllCount;

			m_GameClientView.m_wMinusLastCount=wMinusLastCount==0?0:wMinusLastCount-1;

			//得到堆牌区中头部牌的位置
			m_GameClientView.m_wMinusHeadCount=pStatusPlay->cbLeftCardCount+m_GameClientView.m_wMinusLastCount+1;
			m_GameClientView.m_LHeapCard.SetCardData(m_GameClientView.m_wMinusHeadCount,m_GameClientView.m_wMinusLastCount,110);


//--------------------左右两家用户手中牌的恢复------------------------------//
			for (BYTE i=0; i<GAME_PLAYER; i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				//下家用户不用户恢复手中牌
				if(wViewChairID==1)   continue;

				//是当前用户的话，是有一张牌单独显示出来的
				m_bCardCount[i]=pStatusPlay->cbUserCardCount[i];
				BYTE cbCardCount=m_wCurrentUser==i?m_bCardCount[i]-1:m_bCardCount[i];
				m_GameClientView.m_UserCard[wViewChairID].SetCardData(cbCardCount,(m_wCurrentUser==i));
			}
//--------------------三家用户组合区数据的恢复----------------------//			
			for (WORD i=0; i<GAME_PLAYER; i++)
			{
				//初始化八种组合
				for (WORD j=0; j<8; j++)
				{
					m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
					m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
				}
			}			
			//设置组合扑克数据
			BYTE cbWeaveCard[5];
			//组合花牌数据要备份，在使用过程中进行了修改
			BYTE cbHuaWeaveCardIndex[3][5];
			CopyMemory(cbHuaWeaveCardIndex,pStatusPlay->cbHuaWeaveCardIndex,sizeof(cbHuaWeaveCardIndex));

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;

					BYTE index = m_GameLogic.SwitchToCardIndex(cbCenterCard);
					//如果是经牌，传入花牌的个数
					if (index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
					{
						BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard,cbHuaWeaveCardIndex[i][index/2]);
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					}
					else
					{
						BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard,0);
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					}
				}
			}
//---------------------三家用户已出牌区的恢复----------------------------//
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
			}
//--------------------正在出牌的显示恢复-------------------------//
			//有出牌用户的话，显示其出牌的信息
			if (m_wOutCardUser!=INVALID_CHAIR)
			{
				WORD wOutChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wOutChairID,m_cbOutCardData);
			}
//-------------------重进入游戏后，其托管自动取消-----------------//
			m_IsTuoGuan=false;
			if(IsLookonMode()==false)
			{
				m_GameClientView.m_btTuoGuan.ShowWindow(SW_SHOW);
				m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_HIDE);
			}
//--------------------用户自己手中各分区牌的恢复--------------------//
			//设置用户九个区中的数据
			StartSendAfterFace();

			//如果是用户那么禁止其出牌
			DisableOutCard();

//-------------还原打牌的流程过程-------------------------------------//
			//表明还处在于第一轮的藏牌阶段
			if(m_bGameStart==false)
			{
				m_cbFirstCurrentAction=pStatusPlay->cbFirstCurrentAction;
				m_cbFirstCurrentUser=pStatusPlay->wFirstCurrentUser;

				//当首操作用户为庄用户，并且其操作为WIK_NULL时，表明无用户可操作，正常启动游戏
				if((m_cbFirstCurrentUser==m_wBankerUser) && (m_cbFirstCurrentAction==WIK_NULL))
				{
					m_bGameStart = true;
					m_wCurrentUser=m_wBankerUser;
				}
				else
				{
					m_bGameStart = false;
					m_wCurrentUser=INVALID_CHAIR;
				}

				//定有人能操作牌，并且是自己的话，则要显示操作提示
				if ((m_bGameStart==false) && (m_cbFirstCurrentUser==GetMeChairID()))
				{
					//当前用户是庄用户并且能胡牌的话，则先胡牌
					if((GetMeChairID()==m_wBankerUser) && (m_cbFirstCurrentAction&WIK_CHI_HU)!=0)
					{
						BYTE cbHuScore = pStatusPlay->cbHuScore;
						BYTE cbGoldCard = m_GameLogic.SwitchToCardData(pStatusPlay->cbGoldCard);

						tagHuCardWeave HuCardweave;
						ZeroMemory(&HuCardweave,sizeof(HuCardweave));
						GetHuCardWeave(pStatusPlay->cbCenterCard,pStatusPlay->cbWeaveKind,pStatusPlay->cbYaKou,HuCardweave);

						m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,cbHuScore,cbGoldCard);
					}
					// 当用户有藏牌操作时，则显示用户藏牌提示
					else if((m_cbFirstCurrentAction&WIK_SI_TONG)!=0 || (m_cbFirstCurrentAction&WIK_WU_TONG)!=0)
					{
						ShowTongOperateControl(m_cbFirstCurrentAction,0);
					}

					//显示胡牌定时器
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}
		
				//定有人能操作牌，但不是自己的话，则要显示等待的定时器信息
				if((m_bGameStart==false) && (m_cbFirstCurrentUser!=GetMeChairID()))
				{
					SetGameTimer(m_cbFirstCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

					m_GameClientView.SetStatusFlag(false,true);
				}

				//环境处理
				if(IsEnableSound()==true)
				{
					CString szSoundName=_T("");
					szSoundName="Sound\\";
					szSoundName+="GAME_START.wav";
					PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
				}

				//当没有用户藏牌时，启动庄家出牌、定时器及显示请选择要出的牌几个字
				if (m_bGameStart == true && m_wCurrentUser == m_wBankerUser) 
				{
					//如果自己是当前用户那么提示请出牌
					if(GetMeChairID()==m_wBankerUser)
					{
						if(IsLookonMode()==false) EnableOutCard();

						m_GameClientView.SetStatusFlag(true,false);
					}
					//如果自己不是当前用户那么提示稍等
					else
					{
						m_GameClientView.SetStatusFlag(false,true);
					}

					SetGameTimer(m_wBankerUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}

				return true;
			}
			else
			{
				//此时表明是已有用户出了牌,应该有用户对此牌进行操作，但还没有操作之前的情况
				if(m_wCurrentUser==INVALID_CHAIR)
				{
					//并且用户自己也有操作时，则进行显示操作提示
					if((IsLookonMode() == false) && m_cbActionMask!=WIK_NULL)
					{
						//激活框架
						ActiveGameFrame();

						//如果有胡牌操作和其它操作的情况下，先显示胡牌最佳组合
						if((m_cbActionMask & WIK_CHI_HU) != 0)
						{
							//屏蔽掉胡牌后，设置界面
							m_cbActionMask = (m_cbActionMask & 0x7F);

							BYTE hScore = pStatusPlay->cbHuScore;
							BYTE GoldCard = m_GameLogic.SwitchToCardData(pStatusPlay->cbGoldCard);

							tagHuCardWeave HuCardweave;
							ZeroMemory(&HuCardweave,sizeof(HuCardweave));
							GetHuCardWeave(pStatusPlay->cbCenterCard,pStatusPlay->cbWeaveKind,pStatusPlay->cbYaKou,HuCardweave);
							
							m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,hScore,GoldCard);
						}
						//如果没有胡牌操作的情况--主要是针对三招与四招的情况
						else 
						{
							m_GameClientView.m_ControlWnd.SetDuiZhaoOperateCardData(m_cbActionCard,m_cbActionMask,m_cbHuaCardIndex);
							
							m_cbActionMask=0;
							m_cbActionCard=0;
						}
						//启动操作定时器
						SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);

						//更新界面
						m_GameClientView.UpdateGameView(NULL);

						return true;
					}
				}
				//当前用户是用户自己的情况
				else if((m_wCurrentUser==GetMeChairID()) && (IsLookonMode() == false))
				{
					//启动用户出牌
					EnableOutCard();

					//当前用户有操作情况时，表明用户可能胡牌或藏牌
					if(m_cbActionMask!=WIK_NULL)
					{
						//当有胡牌操作时，先进行胡牌操作，后进行藏牌操作
						if ((m_cbActionMask&WIK_CHI_HU)!=0)
						{
							//屏蔽掉胡牌后，设置界面
							m_cbActionMask = (m_cbActionMask & 0x7F);

							tagHuCardWeave HuCardweave;
							ZeroMemory(&HuCardweave,sizeof(HuCardweave));

							BYTE hScore = pStatusPlay->cbHuScore;
							BYTE GoldCard = m_GameLogic.SwitchToCardData(pStatusPlay->cbGoldCard);
							GetHuCardWeave(pStatusPlay->cbCenterCard,pStatusPlay->cbWeaveKind,pStatusPlay->cbYaKou,HuCardweave);
							m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,hScore,GoldCard);	
						}
						//判断除了胡之外是否还有其他操作
						else
						{
							ShowTongOperateControl(m_cbActionMask,0);
							m_cbActionMask = 0;
						}
					}
					//否则是用户摸牌后无操作或操作之后，该用户出牌了
					else
					{
						//提示用户出牌
						m_GameClientView.SetStatusFlag(true,false);
					}
					//启动操作定时器
					SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					//更新界面
					m_GameClientView.UpdateGameView(NULL);

					return true;
				}
				//只有其它人操作时，设置对方的定时器
				else if(m_wCurrentUser!=INVALID_CHAIR && m_wCurrentUser!=GetMeChairID())
				{
					//启动操作定时器
					SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					//提示用户请等待
					m_GameClientView.SetStatusFlag(false,true);
					//更新界面
					m_GameClientView.UpdateGameView(NULL);

					return true;
				}

				return true;
			}
		}

	}
	return false;
}


bool CGameClientDlg::OnSubGameDecideBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_DecideBanker));
	if (wDataSize!=sizeof(CMD_S_DecideBanker)) return false;

	//变量定义
	CMD_S_DecideBanker * pDecideBandker=(CMD_S_DecideBanker *)pBuffer;

	//得到定庄数据结构中玩家的视图ID号
	WORD wOperateViewID = SwitchViewChairID(pDecideBandker->bUserID);

	m_GameClientView.m_bDecideBanker = true;
	m_GameClientView.m_DecideBankerCard[wOperateViewID]=pDecideBandker->bCardData;

	SetTimer(1,1000,NULL);
	//m_GameClientView.m_DecideBankerCard[pDecideBandker->bUserID]=pDecideBandker->bCardData;

	return true;
}

//将发到用户手中的牌按一定的规则分到不同的区中
void CGameClientDlg::StartSendAfterFace()
{
	//设置变量--设置用户操作的九个区
	for(BYTE k=0;k<9;k++)
	{
		m_GameClientView.m_HandCard[k].SetPositively(true);
		m_GameClientView.m_HandCard[k].SetCardData(NULL,0);

		m_GameClientView.m_HandCard[k].ShowWindow(SW_SHOW);
		m_GameClientView.m_HandCard[k].m_bDisplayItem=true;
	}

	BYTE cbCardAreaCount=0;         //保存找到的分区数
	BYTE cbCardAreaIndex[9][9];     //保存每个分区中牌的索引
	BYTE cbCardAreaCardCount[9];    //保存每个分区中牌的个数
	ZeroMemory(cbCardAreaIndex,sizeof(cbCardAreaIndex));
	ZeroMemory(cbCardAreaCardCount,sizeof(cbCardAreaCardCount));

	//将用户手中牌备份到临时操作变量中
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,m_cbCardIndex,sizeof(cbCardIndexTemp));

	//1、找出具有3张或以上的牌，将其单独放入到一个区中
	for(BYTE cbIndex=0;cbIndex<MAX_INDEX;cbIndex++)
	{
		if(cbCardIndexTemp[cbIndex]>=3)
		{
			//将找到的组合牌的索引放到数组中
			for(BYTE cbCount=0;cbCount<cbCardIndexTemp[cbIndex];cbCount++)
			{
				cbCardAreaIndex[cbCardAreaCount][cbCount]=cbIndex;
			}
			//保存此分区中牌的个数
			cbCardAreaCardCount[cbCardAreaCount]=cbCardIndexTemp[cbIndex];
			//将此种牌的个数设为0
			cbCardIndexTemp[cbIndex]=0;
			//确定的分区数加1
			cbCardAreaCount++;
		}
	}

	//2、分别查找上大人、可知礼、孔乙已、化三千、八九子、七十土
	//六种情况，达3张或以上的，也单独放到一个区中
	//定义此6种组合所对应的索引值
	BYTE cbTargetCardIndex[6][3]={{12,13,14},{18,19,20},{10,11,0},
								{2,16,17},{7,8,21},{6,9,15}};
	//逐一扫描此6种情况，只要此种组合中具有3张以上的，就单独成立分区
	for(BYTE i=0;i<6;i++)
	{
		//得到此种组合中牌的总个数
		BYTE cbCount=cbCardIndexTemp[cbTargetCardIndex[i][0]]+cbCardIndexTemp[cbTargetCardIndex[i][1]]+cbCardIndexTemp[cbTargetCardIndex[i][2]];
		if(cbCount>=3)
		{
			BYTE cbTemp=0;  //记录此分区中牌的个数
			//逐一查看这组合三张牌的情况
			for(BYTE s=0;s<3;s++)
			{
				//将此张牌的索引加入到分区数组中
				for(BYTE t=0;t<cbCardIndexTemp[cbTargetCardIndex[i][s]];t++)
				{
					cbCardAreaIndex[cbCardAreaCount][cbTemp++]=cbTargetCardIndex[i][s];
				}
				//将此张牌所对应的个数设为0
				cbCardIndexTemp[cbTargetCardIndex[i][s]]=0;
			}
			//记录分区中牌的个数和分区数
			cbCardAreaCardCount[cbCardAreaCount]=cbTemp;
			cbCardAreaCount++;
		}
	}

	//3、将单牌，以5张一区存放
	BYTE cbTemp=0;   //记录此分区中牌的个数
	for(BYTE cbIndex=0;cbIndex<MAX_INDEX;cbIndex++)
	{
		if(cbCardIndexTemp[cbIndex]>0)
		{
			//将此张牌的索引加入到分区数组中
			for(BYTE cbCount=0;cbCount<cbCardIndexTemp[cbIndex];cbCount++)
			{
				cbCardAreaIndex[cbCardAreaCount][cbTemp++]=cbIndex;
			}
			//将此张牌所对应的个数设为0
			cbCardIndexTemp[cbIndex]=0;
			
			//当此分区中个数达到5时，新增加一个分区
			if(cbTemp>=5)
			{
				cbCardAreaCardCount[cbCardAreaCount]=cbTemp;
				cbCardAreaCount++;
				cbTemp=0;
			}
		}
	}
	//当循环结束时，要将最后的几张牌也放到一个分区中
	if(cbTemp>0)
	{
		cbCardAreaCardCount[cbCardAreaCount]=cbTemp;
		cbCardAreaCount++;
	}
	ASSERT(cbCardAreaCount<=9);

	//将分区后的数据显示出来
	BYTE cbCardData[9];
	BYTE cbHuaCardIndexTemp[5];
	CopyMemory(cbHuaCardIndexTemp,m_cbHuaCardIndex,sizeof(cbHuaCardIndexTemp));
	for(BYTE cbCount=0;cbCount<cbCardAreaCount;cbCount++)
	{
		ZeroMemory(cbCardData,sizeof(cbCardData));
		m_GameLogic.SwitchToCardData(cbCardAreaIndex[cbCount],cbCardAreaCardCount[cbCount],cbCardData,cbHuaCardIndexTemp);
		m_GameClientView.m_HandCard[cbCount].SetCardData(cbCardData,cbCardAreaCardCount[cbCount]);
	}

	////旁观界面
	//if (bPlayerMode==false)
	//{
	//	m_GameClientView.m_TableCard[1].SetCardData(NULL,0);
	//	m_GameClientView.m_DiscardCard[1].SetCardData(NULL,0);
	//	//将8个组合区都清空
	//	for(BYTE cbCount=0;cbCount<8;cbCount++)
	//	{
	//		m_GameClientView.m_WeaveCard[1][cbCount].SetCardData(NULL,0);
	//	}
	//}
}

//当客户端收到服务器端到来的游戏开始消息时，会调用到此函数
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	//变量定义
	CMD_S_GameStart *pGameStart=(CMD_S_GameStart *)pBuffer;
	//设置状态
	SetGameStatus(GS_MJ_PLAY);

	//设置相关的变量
	
	m_wBankerUser=pGameStart->wBankerUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*25-1;

	if((pGameStart->wCurrentUser==m_wBankerUser) && (pGameStart->cbCurrentAction==WIK_NULL))
	{
		m_bGameStart = true;
		m_wCurrentUser=m_wBankerUser;
	}
	else
	{
		m_bGameStart = false;
		m_wCurrentUser=INVALID_CHAIR;
	}

	//禁止用户点击右键出牌
	DisableOutCard();
	if(IsLookonMode()==true)
	{
		//扑克设置
		for (BYTE i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			
			for(BYTE j=0;j<8;j++)
			{
				m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
			}
		}
	}
	m_GameClientView.m_IsStartSendCard=true;
	//m_GameClientView.m_leaveHeapCardData=m_cbLeftCardCount;
	m_GameClientView.m_LHeapCard.SetCardData(34,0,34);
	m_GameClientView.m_wMinusHeadCount=34;
	m_GameClientView.m_wMinusLastCount=0;

	//设置三家的扑克牌数
	for (BYTE i=0; i<GAME_PLAYER; i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);
		if (i == m_wBankerUser) 
		{
			m_bCardCount[i] = 26;
		}
		else
		{
			m_bCardCount[i] = 25;
		}
	}

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;

	//设置扑克
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?26:25;

	//得到花牌索引
	BYTE count = m_GameLogic.GetHuaCard(pGameStart->cbCardData,cbCardCount,m_cbHuaCardIndex);

	//将服务器传过来的数据，保存到当前玩家手中牌索引数组中
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//设置界面
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

	if(IsLookonMode()==false)
	{
		m_GameClientView.m_btTuoGuan.ShowWindow(SW_SHOW);
		m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_HIDE);
	}

	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//旁观模式
	if (bPlayerMode==false)
	{
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetHD(false);
		//设置出牌和等待标志
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
		m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
	}

	//初始化八种组合
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		for (WORD j=0; j<8; j++)
		{
			//组合界面
			m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
		}
	}

	//设置用户九个区中的数据
	StartSendAfterFace();

	//设置另两家用户的手中牌
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);
		//等于1的为视图中的当前用户
		if (wViewChairID!=1)
		{
			m_GameClientView.m_UserCard[wViewChairID].SetCardData(CountArray(pGameStart->cbCardData),(i==m_wBankerUser));
		}
	}
	m_cbFirstCurrentAction=pGameStart->cbCurrentAction;
	m_cbFirstCurrentUser=pGameStart->wCurrentUser;
	//当前操作用户是自己，并且有操作的话，则显示操作提示对话框
	if ((m_bGameStart==false) && (pGameStart->wCurrentUser==GetMeChairID()) && IsLookonMode()==false)
	{
		//当前用户是庄用户并且能胡牌的话，则先胡牌
		if((GetMeChairID()==m_wBankerUser) && (pGameStart->cbCurrentAction&WIK_CHI_HU)!=0)
		{
			m_cbActionCard=0;
			m_cbActionMask=pGameStart->cbCurrentAction;

			tagHuCardWeave HuCardweave;
			ZeroMemory(&HuCardweave,sizeof(HuCardweave));

			BYTE hScore = pGameStart->cbHuScore;
			BYTE GoldCard = m_GameLogic.SwitchToCardData(pGameStart->cbGoldCard);
			
			GetHuCardWeave(pGameStart->cbCenterCard,pGameStart->cbWeaveKind,pGameStart->cbYaKou,HuCardweave);
			m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,hScore,GoldCard);
	
			//显示胡牌定时器
			SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
		// 
		else if((pGameStart->cbCurrentAction&WIK_SI_TONG)!=0 || (pGameStart->cbCurrentAction&WIK_WU_TONG)!=0)
		{
			m_cbActionCard=0;
			m_cbActionMask=pGameStart->cbCurrentAction;

			ShowTongOperateControl(m_cbActionMask,0);
				
			//显示藏牌定时器
			SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
	}

	//定有人能操作牌，但不是自己的话，则要显示等待的定时器信息
	if((m_bGameStart==false) && (pGameStart->wCurrentUser!=GetMeChairID()))
	{
		SetGameTimer(pGameStart->wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

		m_GameClientView.SetStatusFlag(false,true);
	}
	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	//激活框架
	ActiveGameFrame();

	//环境处理
	if(IsEnableSound()==true)
	{
		CString szSoundName=_T("");
		szSoundName="Sound\\";
		szSoundName+="GAME_START.wav";
		PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);

	}
	
	//第一轮没有人藏牌，则提示当前用户出牌
	if ((bPlayerMode==true)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		WORD wMeChairID=GetMeChairID();

		//启动有人请牌请稍后提示
		if (m_wCurrentUser==wMeChairID)
		{
			m_GameClientView.SetStatusFlag(true,false);
		}
		else
		{
			m_GameClientView.SetStatusFlag(false,true);
		}
	}

	//当没有用户藏牌时，启动庄家出牌及定时器
	if (m_bGameStart == true) 
	{
		if(GetMeChairID()==m_wBankerUser && IsLookonMode()==false) EnableOutCard();

		SetGameTimer(m_wBankerUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}
	return true;
}

bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//消息处理
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//变量定义
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;

	//出牌用户的牌数减１
	m_bCardCount[m_wOutCardUser] -=1;

	//其他用户
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//环境设置
		KillGameTimer(IDI_OPERATE_CARD); 
		if(IsEnableSound()==true)
		{
			m_cbSex = GetUserData(m_wOutCardUser)->cbGender;
			PlayCardSound(pOutCard->cbOutCardData);
		}
		//出牌界面
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//设置观看玩家的扑克
		if (GetMeChairID()==m_wOutCardUser)
		{
			
			////删除扑克
			//BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);
			
			//从九个操作区域中将其删除
			for(BYTE AreaIndex=0;AreaIndex<9;AreaIndex++)
			{
				//逐一查找每一个分区中的每一张牌
				bool bDelete=false;
				for(BYTE cbCardIndex=0;cbCardIndex<m_GameClientView.m_HandCard[AreaIndex].m_cbCardCount;cbCardIndex++)
				{
					if(m_GameClientView.m_HandCard[AreaIndex].m_CardItemArray[cbCardIndex].cbCardData == pOutCard->cbOutCardData)
					{
						m_GameClientView.m_HandCard[AreaIndex].DeleteByArrayIndex(cbCardIndex);
						bDelete=true;
						break;  //结束本分区的查找过程
					}
				}
				if(bDelete==true) break;  //提前结束本张牌的删除过程
			}
		}
		else
		{
			//WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wOutViewChairID].SetCurrentCard(false);
		}
	}

	//如果是旁观用户那么禁止其出牌
	if((IsLookonMode()==true))
	{
		//AfxMessageBox("我是旁观用户，被禁止出牌了");
		DisableOutCard();
	}
	m_GameClientView.SetStatusFlag(false,false);
	return true;
}

//发牌消息
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{

	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//如果是旁观用户那么禁止其出牌
	if((IsLookonMode()==true))
	{
		DisableOutCard();
	} 

	//第一轮藏牌过程中的发牌，一定是藏牌
	if(m_bGameStart==false)
	{
		m_bTongCard=true;
	}
	//正常游戏开始后,但当前用户无指定人时,则一定是用户自摸牌的到来----只要有人出了牌，则会清空当前用户的
	else if(m_bGameStart==true && m_wCurrentUser==INVALID_CHAIR)
	{
		m_bTongCard=false;
	}
	//正常游戏开始后,但当前用户有确定的用户时,则一定是用户藏牌之后的补牌的到来
	else if(m_bGameStart==true && m_wCurrentUser!=INVALID_CHAIR)
	{
		m_bTongCard=true;
	}

	//丢弃扑克--设置上个用户出牌的信息
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//丢弃扑克
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);
		
		//设置变量
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//将与发送牌一起过来的数据保存起来
	m_cbSendCardFinish=false;
	m_wCurrentUser=pSendCard->wCurrentUser;

	//是海底的情况时，都是自摸牌
	b_IsHD=pSendCard->bIsHD;
	if(b_IsHD==true)
	{
		m_bTongCard=false;
	}

	m_cbSendCount=pSendCard->cbSendCount;
	CopyMemory(m_cbSendCardData,pSendCard->cbCardData,sizeof(m_cbSendCardData));

	m_cbActionMask=pSendCard->cbActionMask;

	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);

	//当前操作用户是自己，并且能胡牌的话，还要保存胡牌的数据
	if ((m_wCurrentUser==wMeChairID) && ((m_cbActionMask&WIK_CHI_HU)!=0))
	{
		m_cbHuScore = pSendCard->cbHuScore;
		m_cbGoldCard = m_GameLogic.SwitchToCardData(pSendCard->cbGoldCard);

		CopyMemory(m_cbYaKou,pSendCard->cbYaKou,sizeof(m_cbYaKou));
		CopyMemory(m_cbCenterCard,pSendCard->cbCenterCard,sizeof(m_cbCenterCard));
		CopyMemory(m_cbWeaveKind,pSendCard->cbWeaveKind,sizeof(m_cbWeaveKind));
	}


//---------------以下的代码是起动发牌的动画显示-----------------------
	if(IsLookonMode()==false)
	{
		CPoint ptStartPoint;
		CPoint ptTargetPoint[GAME_PLAYER];

		WORD nWidth=761;
		WORD nHeight=705;

		CRect rect;
		m_GameClientView.GetClientRect(&rect);

		ptTargetPoint[1].x=rect.Width()/2;
		ptTargetPoint[1].y=rect.Height()-200;
		ptTargetPoint[0].x=rect.Width()-200;
		ptTargetPoint[0].y=rect.Height()/2-80;
		ptTargetPoint[2].x=100;
		ptTargetPoint[2].y=rect.Height()/2-80;

		//启动发牌动画
		if(m_bTongCard==true)   //藏牌而发的牌,都是从尾部拿的
		{
			for(WORD i=0;i<pSendCard->cbSendCount;i++)
			{
				WORD wMinusLastCount=m_GameClientView.m_wMinusLastCount;
				ptStartPoint.x=0+32+wMinusLastCount*6+198;			
				ptStartPoint.y=0+30+54;

				if(m_wCurrentUser==wMeChairID)
				{
					//ptTargetPoint[1].x=600;
					//ptTargetPoint[1].y=570;
					m_GameClientView.DispatchUserCard(1,ptStartPoint,ptTargetPoint[1],pSendCard->cbCardData[i],false);
				}
				else
				{
					//if(wViewChairID==0)
					//{
					//	BYTE cbCardCount=m_bCardCount[wMeChairID]-1;

					//	ptTargetPoint[0].x=nWidth-172+24;
					//	ptTargetPoint[0].y=nHeight/2-310+75+cbCardCount*12;
					//}
					//if(wViewChairID==2)
					//{
					//	BYTE cbCardCount=m_bCardCount[wMeChairID]-1;

					//	ptTargetPoint[2].x=100+90-65-24;
					//	ptTargetPoint[2].y=nHeight/2+160-cbCardCount*12-92;
					//}
					m_GameClientView.DispatchUserCard(wViewChairID,ptStartPoint,ptTargetPoint[wViewChairID],0,false);
				}
			}
		}
		else                 //自摸而发的牌,都是从头部拿的
		{
			WORD wMinusHeadCount=m_GameClientView.m_wMinusHeadCount;
			ptStartPoint.x=0+32+54+wMinusHeadCount*6;			
			ptStartPoint.y=0+30+54;

			if(m_wCurrentUser==wMeChairID)
			{
				//ptTargetPoint[1].x=600;
				//ptTargetPoint[1].y=570;
				m_GameClientView.DispatchUserCard(1,ptStartPoint,ptTargetPoint[1],pSendCard->cbCardData[0],true);
			}
			else
			{
				//if(wViewChairID==0)
				//{
				//	BYTE cbCardCount=m_bCardCount[wMeChairID]-1;

				//	ptTargetPoint[0].x=nWidth-172+24;
				//	ptTargetPoint[0].y=nHeight/2-310+75+cbCardCount*12;
				//}
				//if(wViewChairID==2)
				//{
				//	BYTE cbCardCount=m_bCardCount[wMeChairID]-1;

				//	ptTargetPoint[2].x=100+90-65-24;
				//	ptTargetPoint[2].y=nHeight/2+160-cbCardCount*12-92;
				//}
				m_GameClientView.DispatchUserCard(wViewChairID,ptStartPoint,ptTargetPoint[wViewChairID],0,true);
			}
		}
	}


	return true;
/////////////////////////////////////////////////////////////////////////////
}

//操作提示
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//AfxMessageBox("CGameClientDlg::OnSubOperateNotify");
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//仅针对别人打出的牌，服务器分析不同用户对此牌的需求信息，并且分别发送给相应的用户
	//能到此处的话，说明此用户能对此牌进行胡、三招或四招的操作。
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		m_cbSendCardFinish=true;

		if(m_IsTuoGuan)
		{
			OnCardOperate(WIK_NULL,0);
			return true;
		}

		//操作牌记为引起操作的牌
		m_cbActionMask=pOperateNotify->cbActionMask;
		m_cbActionCard=pOperateNotify->cbActionCard;

		//如果有胡牌操作和其它操作的情况下，先显示胡牌最佳组合
		if ((m_cbActionMask & WIK_CHI_HU) != 0)
		{
			//屏蔽掉胡牌后，设置界面
			m_cbActionMask = (m_cbActionMask & 0x7F);

			BYTE hScore = pOperateNotify->cbHuScore;
			BYTE GoldCard = m_GameLogic.SwitchToCardData(pOperateNotify->cbGoldCard);

			tagHuCardWeave HuCardweave;
			ZeroMemory(&HuCardweave,sizeof(HuCardweave));
			GetHuCardWeave(pOperateNotify->cbCenterCard,pOperateNotify->cbWeaveKind,pOperateNotify->cbYaKou,HuCardweave);
			
			m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,hScore,GoldCard);
		}
		//如果没有胡牌操作的情况--主要是针对三招与四招的情况
		else 
		{
			m_GameClientView.m_ControlWnd.SetDuiZhaoOperateCardData(m_cbActionCard,m_cbActionMask,m_cbHuaCardIndex);
			
			m_cbActionMask=0;
			m_cbActionCard=0;
		}
		ActiveGameFrame();

		SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		m_GameClientView.SetStatusFlag(false,false);
	}

	//如果是旁观用户那么禁止其出牌
	if((IsLookonMode()==true))
	{
		//AfxMessageBox("我是旁观用户，被禁止出牌了");
		DisableOutCard();
	}
	m_GameClientView.UpdateGameView(NULL);
	return true;
}

//操作结果--服务器返回的操作结果的处理
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//变量定义
	BYTE cbPublicCard=TRUE;   //标记是否全部正面显示
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);

	//出牌变量
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}
		//如果是旁观用户那么禁止其出牌
	if((IsLookonMode()==true))
	{
		//AfxMessageBox("我是旁观用户，被禁止出牌了");
		DisableOutCard();
	}

	//进行藏牌处理
	if ((pOperateResult->cbOperateCode & WIK_SI_TONG) != 0 || (pOperateResult->cbOperateCode & WIK_WU_TONG)!=0 || (pOperateResult->cbOperateCode & WIK_TONG_GIVE_UP)!=0 )
	{
		//1.保存四统的牌--只有可能是用户手中有四张的情况
		if ((pOperateResult->cbOperateCode & WIK_SI_TONG) != 0)
		{
			//只有是当前操作用户，才能保存自己的四藏情况
			if(GetMeChairID()==wOperateUser)
			{
				m_SiTongIndex[m_SiTongCount++] = m_GameLogic.SwitchToCardIndex(cbOperateCard);
			}
			WORD ViewChairID=SwitchViewChairID(wOperateUser);
			m_GameClientView.m_cbTongCount[ViewChairID]++;
		}
		//保存五统的牌--有两种情况：一是用户手中五张牌的藏操作；二是三招后组合牌的五藏操作
		else if ((pOperateResult->cbOperateCode & WIK_WU_TONG) != 0)
		{
			//得到引起操作牌的索引
			BYTE cbOperateindex = m_GameLogic.SwitchToCardIndex(cbOperateCard);

			//1、判断是否是组合的五藏--将五藏当作四招来处理
			BYTE cbWeaveIndex = 0xFF;
			//对操作用户的所有操作进行分析判断是否是组合藏牌
			for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
			{
				BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
				BYTE cbCenterCardIndex = m_GameLogic.SwitchToCardIndex(m_WeaveItemArray[wOperateUser][i].cbCenterCard );
				//用户手中只有1张牌，但组合牌中有过三招的话，可能实现将五藏当作四招来处理
				if ((m_cbCardIndex[cbOperateindex]==1)&&(cbOperateindex == cbCenterCardIndex) && (cbWeaveKind==WIK_SAN_ZHAO))
				{
					cbWeaveIndex=i;
					//修改此组合的相关信息
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=WIK_SI_ZHAO;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
					
					break;
				}
			}

			//如果用户手中只有一张牌，并且组合牌中有四张与其相同的牌
			if (cbWeaveIndex != 0xFF && m_cbCardIndex[cbOperateindex] == 1)
			{
				//对应玩家的牌数减1
				m_bCardCount[wOperateUser] -= 1;

				//如果是一，三，五，七，九
				BYTE bHuaCard = (cbOperateindex == 0 || cbOperateindex == 2 || cbOperateindex == 4 || cbOperateindex == 6 || cbOperateindex == 8)?2:0;
				
				//修改组合界面的显示情况
				BYTE cbTongCard[5]={0,0,0,0,0},cbTongKind=WIK_SI_ZHAO;
				//得到操作的牌，返回操作的扑克张数
				BYTE cbTongCardCount=m_GameLogic.GetWeaveCard(cbTongKind,cbOperateCard,cbTongCard,bHuaCard);
				//重新设置出牌用户的组合牌
				m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbTongCard,cbTongCardCount);	
				//保存藏牌的次数
				WORD ViewChairID=SwitchViewChairID(wOperateUser);
				if(GetMeChairID()==wOperateUser)
				{
					m_GameClientView.m_cbTongCount[ViewChairID]++;
				}
				//删除扑克
				if (GetMeChairID()==wOperateUser)
				{
					//如果是一，三，五，七，九，将对应位置的花牌数置为0
					if (cbOperateindex == 0 || cbOperateindex == 2 || cbOperateindex == 4 || cbOperateindex == 6 || cbOperateindex == 8)
					{
						m_cbHuaWeaveCardIndex[cbOperateindex/2] = 2;
						m_cbHuaCardIndex[cbOperateindex/2] = 0;
					}
					m_cbCardIndex[cbOperateindex] = 0;

					bool bDelete = false;
					//从手中牌区域找到该牌所在的区域，并且从中删除
					for (BYTE i=0; i<9; i++)
					{
						//扫描每个子区域的牌数据
						for (BYTE j=0; j<m_GameClientView.m_HandCard[i].m_cbCardCount; j++)
						{
							if (m_GameLogic.SwitchToCardIndex(m_GameClientView.m_HandCard[i].m_CardItemArray[j].cbCardData) == cbOperateindex)
							{
								m_GameClientView.m_HandCard[i].DeleteCardData(m_GameClientView.m_HandCard[i].m_CardItemArray[j].cbCardData);
								bDelete = true;
								break;
							}
						}
						if(bDelete == true) break;
					}
				}
			}
			//是用户手中的牌实现五藏操作
			else
			{
				BYTE index=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				if(GetMeChairID()==wOperateUser)
				{
					m_WuTongIndex[m_WuTongCount++] = index;

					//下步修改，将此张牌调整到藏牌区中
				}
				bool IsSiTong=false;
				WORD ViewChairID=SwitchViewChairID(wOperateUser);

				for(BYTE j=0;j<m_SiTongCount;j++)
				{
					if(m_SiTongIndex[j]==index)
					{
						//
						m_GameClientView.m_cbTongCount[ViewChairID]++;
						IsSiTong=true;

					}
				}
				if(IsSiTong==false)
				{
					m_GameClientView.m_cbTongCount[ViewChairID]+=2;
				}

			}
		}

		//如果是第一圈统牌什么也不做，只是显示统牌标记通知其他玩家
		if (m_bGameStart == false)
		{
			KillGameTimer(IDI_OPERATE_CARD);
			//统牌完成,启动庄家定时器
			if ((pOperateResult->wCurrentUser==m_wBankerUser) && (pOperateResult->cbCurrentAction==WIK_NULL))
			{
				m_bGameStart = true;
				m_wCurrentUser=m_wBankerUser;
				if(GetMeChairID()==m_wBankerUser) 
				{
					//启动庄家出牌操作
					EnableOutCard();
					//提示庄家开始出牌
					m_GameClientView.SetStatusFlag(true,false);
				}

				//隐藏操作提示窗口
				m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

				//对于庄家用户来说，也可能启动了胡牌提示框
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

				//显示藏牌标志
				m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
				
				//更新界面
				m_GameClientView.UpdateGameView(NULL);

				//环境设置
				if(IsEnableSound()==true)
				{
					m_cbSex = GetUserData(pOperateResult->wOperateUser)->cbGender;
					PlayActionSound(pOperateResult->cbOperateCode);
				}
			
					SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

				return true;
			}

			//第一轮的统牌没完成,服务器发送回来操作时,只显示用户的动作 ，播放声音，
			m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);

			//对于本操作用户来说，当进行了四藏或五藏操作而拿到牌后，还可能进行藏牌的操作
			if(((pOperateResult->cbCurrentAction & WIK_SI_TONG) != 0 || (pOperateResult->cbCurrentAction & WIK_WU_TONG)!=0) && IsLookonMode()==false)
			{
				//当用户是藏牌藏操作时，放到发牌处进行了处理，此处仅是处理切换用户而进行的藏牌处理
				if(pOperateResult->wCurrentUser!=pOperateResult->wOperateUser)
				{
					if(GetMeChairID()==pOperateResult->wCurrentUser)
					{
						m_cbActionMask=pOperateResult->cbCurrentAction;

						m_GameClientView.SetStatusFlag(false,false);
						ShowTongOperateControl(m_cbActionMask,0);
						
						SetGameTimer(pOperateResult->wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					}
					else
					{
						m_GameClientView.SetStatusFlag(false,true);
						SetGameTimer(pOperateResult->wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					}
				}
			}
			//更新界面
			m_GameClientView.UpdateGameView(NULL);

			//环境设置
			if(IsEnableSound()==true)
			{
				m_cbSex = GetUserData(pOperateResult->wOperateUser)->cbGender;
				PlayActionSound(pOperateResult->cbOperateCode);
			}
			return true;
		}
		
		if ((pOperateResult->cbOperateCode & WIK_TONG_GIVE_UP)!=0)  return true;
	}
	//对牌，招牌操作
	else if (pOperateResult->cbOperateCode != WIK_NULL)
	{
		//设置变量
		m_wCurrentUser=pOperateResult->wOperateUser;

		//设置组合牌
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//组合牌界面
		BYTE cbWeaveCard[5]={0,0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard,pOperateResult->cbHuaCount);

		//对应玩家的牌数减少
		m_bCardCount[wOperateUser] -= cbWeaveCardCount-1;

		//设置出牌用户的组合牌
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);

		//删除扑克
		if (GetMeChairID()==wOperateUser)
		{/////////////////////////////今天改的4.13///////////////////
			if((IsLookonMode()==false) && (pOperateResult->cbOperateCode&WIK_DUI)!=0)	EnableOutCard();
			//从当前玩家的临时组合牌中删除一张别人提供的牌，再从自己手中牌索引数组中删除别外的两张
			BYTE bDelet = 0;
			//1、删除组合牌中，别人打过来的牌数据
			for (BYTE i=0; i<cbWeaveCardCount; i++)
			{
				if (cbWeaveCard[i] == cbOperateCard)
				{
					bDelet++;
					cbWeaveCard[i] = 0;
					//将后面的牌向前移
					for (BYTE j=i; j<cbWeaveCardCount-1; j++)
					{
						cbWeaveCard[j] = cbWeaveCard[j+1];
					}
					//将最后一置为0
					cbWeaveCard[j] = 0;
					break;
				}
			}
			//2、从自己的手中牌索引数组中删除组合牌中自己手中的牌
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);

			//3、如果传过来的花牌数不为0，从当前玩家手中花牌数减少，加到组合花牌中
			if (pOperateResult->cbHuaCount != 0)
			{
				BYTE index = m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard);
				m_cbHuaWeaveCardIndex[index/2] += pOperateResult->cbHuaCount;
				
				//在实现三招与四招时，是尽量用自己手中的花牌去招别人的牌
				m_cbHuaCardIndex[index/2] = 0;
			}

			//4、判断所执行的操作是否是拆了四藏的牌
			if ((pOperateResult->cbOperateCode & WIK_SI_ZHAO) != 0)
			{
				for (BYTE i=0; i<8;i++)
				{
					if (m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard) == m_SiTongIndex[i])
					{
					//	m_SiTongIndex[i] = INVALID_CARDINDEX;
						m_SiTongIndex[i] =(BYTE)(-1);
						m_SiTongCount--;
						break;
					}
				}
			}
			//5、从操作区域中删除组合的牌
		    
			////每次循环，删除其中的一张牌
			for(BYTE cbDeletIndex=0;cbDeletIndex<(cbWeaveCardCount-1);cbDeletIndex++)
			{
				//得到此张要删除的牌
				BYTE cbDeleteCardData=cbWeaveCard[cbDeletIndex];
				//从九个操作区域中将其删除
				for(BYTE AreaIndex=0;AreaIndex<9;AreaIndex++)
				{
					//逐一查找每一个分区中的每一张牌
					bool bDelete=false;
					for(BYTE cbCardIndex=0;cbCardIndex<m_GameClientView.m_HandCard[AreaIndex].m_cbCardCount;cbCardIndex++)
					{
						if(m_GameClientView.m_HandCard[AreaIndex].m_CardItemArray[cbCardIndex].cbCardData == cbDeleteCardData)
						{
							m_GameClientView.m_HandCard[AreaIndex].DeleteByArrayIndex(cbCardIndex);
							bDelete=true;
							break;  //结束本分区的查找过程
						}
					}
					if(bDelete==true) break;  //提前结束本张牌的删除过程
				}
			}
		}
		else
		{
			m_GameClientView.m_UserCard[wOperateViewID].SetCardData(m_bCardCount[wOperateUser]-1,true);
		}
	}

	//如果是当前玩家的操作返回时，才隐藏操作提示
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	//如果操作用户是自己那么提示请出牌

	if(m_wCurrentUser==GetMeChairID())
	{
		m_GameClientView.SetStatusFlag(true,false);
	}
	//如果自己不是操作用户那么提示请等待
	else
	{
		m_GameClientView.SetStatusFlag(false,true);
	}
	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	//环境设置
	if(IsEnableSound()==true)
	{
		m_cbSex = GetUserData(pOperateResult->wOperateUser)->cbGender;
		PlayActionSound(pOperateResult->cbOperateCode);
	}

	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR )
	{
		//设置时间
		SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return true;
}



bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	char msg[60];
	memset(msg,0,sizeof(msg));

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	
	//设置游戏状态
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//删除定时器
	KillGameTimer(IDI_OPERATE_CARD);
	if(IsLookonMode()==false)
	{
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}
	
	m_IsTuoGuan=false;
	if(IsLookonMode()==false)
	{
		m_GameClientView.m_btTuoGuan.ShowWindow(SW_HIDE);
		m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_HIDE);
	}
	ZeroMemory(m_GameClientView.m_cbTongCount,sizeof(m_GameClientView.m_cbTongCount));
	
	DisableOutCard();

	//设置控件
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
	m_GameClientView.SetHD(false);

	tagGameScoreInfo GameScoreInfo;
	memset(&GameScoreInfo,0,sizeof(GameScoreInfo));

	GameScoreInfo.lHuPoint = pGameEnd->lHuPoint;
	GameScoreInfo.cbGoldCard = pGameEnd->bGoldCard;

	m_GameClientView.m_GameScoreWnd.SetTax(pGameEnd->lTax);

	//babala 11-30该置最好将玩家自摸的牌传到客户端
	//结束设置,有人放炮的胡牌，得到所有信息，设置显示分数的窗体
	if (pGameEnd->cbChiHuCard!=0 && pGameEnd->bIsQuit==false)
	{
		//PlayActionSound(WIK_CHI_HU);
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
		GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		GameScoreInfo.cbGoldCard=pGameEnd->bGoldCard;
		GameScoreInfo.wWinner=(BYTE)pGameEnd->wWinner;

		InsertShtickString( TEXT( " 本局结束，成绩统计：" ), RGB( 255, 0, 0 ), true );

		CString strScore;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->szName,CountArray(GameScoreInfo.szUserName[i]));
			strScore.Format( TEXT( " %s ：%ld" ), pUserData->szName,pGameEnd->lGameScore[i]);
			InsertShtickString( strScore, RGB( 255, 0, 0 ), true );
		}
		//拷贝胡牌组合
		ZeroMemory(&GameScoreInfo.HuCardWeave,sizeof(GameScoreInfo.HuCardWeave));
		GetHuCardWeave(pGameEnd->cbCenterCard,pGameEnd->cbWeaveKind,pGameEnd->cbYaKou,pGameEnd->cbHuaWeaveCount,pGameEnd->cbHuaHandCount,m_cbWeaveCount[pGameEnd->wWinner],GameScoreInfo.HuCardWeave);

		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);
	}
	
	//荒庄的时候，清除牌墙，显示荒庄图片
	if ((pGameEnd->cbChiHuCard==0)&&(pGameEnd->wChiHuUserCount==0)&&(pGameEnd->bIsQuit==false))
	{
		m_GameClientView.m_wMinusHeadCount=m_GameClientView.m_wMinusLastCount+1;
		m_GameClientView.SetHuangZhuang(true);
		m_GameClientView.SetStatusFlag(false,false);
	}
	//强退
	else
	{
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
		GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		GameScoreInfo.cbGoldCard=pGameEnd->bGoldCard;
		GameScoreInfo.wWinner=(BYTE)pGameEnd->wWinner;
		GameScoreInfo.lHuPoint = pGameEnd->lHuPoint;

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];

			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->szName,CountArray(GameScoreInfo.szUserName[i]));
		}

		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);
	}

	//用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		//设置赢家胡牌标志
		if (pGameEnd->wWinner == i)
		{
			m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		}

		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//设置扑克
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);

	//播放声音
	LONG lScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lScore>0L) 
	{
		m_cbSex = GetUserData(GetMeChairID())->cbGender;
		//播放男生
		if (m_cbSex == 1)
		{
			if(IsEnableSound()==true)
			{
				CString szSoundName=_T("");
				szSoundName="Sound\\boy\\";
				szSoundName+="满.wav";
				PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
			}
		}
		else  
		{
			if(IsEnableSound()==true)
			{
				CString szSoundName=_T("");
				szSoundName="Sound\\girl\\";
				szSoundName+="满.wav";
				PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
			}
		}
	}
	else if (lScore<0L) 
	{
		if(IsEnableSound()==true)
		{
			CString szSoundName=_T("");
			szSoundName="Sound\\";
			szSoundName+="GAME_LOST.wav";
			PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
		}
	}
	else
	{
		if(IsEnableSound()==true)
		{
			CString szSoundName=_T("");
			szSoundName="Sound\\";
			szSoundName+="GAME_END.wav";
			PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
		}
	}


	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	}

	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	return true;
}


//----------------------------------------------------------------------------------------
//播放声音
void CGameClientDlg::PlayCardSound(BYTE cbCardData)
{
	
	//变量定义
	CString szSoundName;

	if(m_wPlayNativeSound==1)
	{
		if(m_cbSex == 0)
		{
			szSoundName="Sound\\地方话\\girl\\";
		}
		else if(m_cbSex == 1)
		{
			szSoundName="Sound\\地方话\\boy\\";
		}
		else if(m_cbSex == 2)
		{
			szSoundName="Sound\\地方话\\girl\\";
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		if(m_cbSex == 0)
		{
			szSoundName="Sound\\普通话\\girl\\";
		}
		else if(m_cbSex == 1)
		{
			szSoundName="Sound\\普通话\\boy\\";
		}
		else if(m_cbSex == 2)
		{
			szSoundName="Sound\\普通话\\girl\\";
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	BYTE cbCardIndex = m_GameLogic.SwitchToCardIndex(cbCardData);
	//0--9
	if (cbCardIndex >=0 && cbCardIndex<10)
	{
		switch(cbCardIndex)
		{
		case 0:
			{
				szSoundName+="一.wav";
				break;
			}
		case 1:
			{
				szSoundName+="二.wav";
				break;
			}
		case 2:
			{
				szSoundName+="三.wav";
				break;
			}
		case 3:
			{
				szSoundName+="四.wav";
				break;
			}
		case 4:
			{
				szSoundName+="五.wav";
				break;
			}
		case 5:
			{
				szSoundName+="六.wav";
				break;
			}
		case 6:
			{
				szSoundName+="七.wav";
				break;
			}
		case 7:
			{
				szSoundName+="八.wav";
				break;
			}
		case 8:
			{
				szSoundName+="九.wav";
				break;
			}
		case 9:
			{
				szSoundName+="十.wav";
				break;
			}
		}
	}
	else
	{
		switch(cbCardIndex)
		{
		case 10:
			{
				szSoundName+="孔.wav";

				break;
			}
		case 11:
			{
				szSoundName+="几.wav";
				break;
			}
		case 12:
			{
				szSoundName+="上.wav";
				break;
			}
		case 13:
			{
				szSoundName+="大.wav";
				break;
			}
		case 14:
			{
				szSoundName+="人.wav";
				break;
			}
		case 15:
			{
				szSoundName+="土.wav";
				break;
			}
		case 16:
			{
				szSoundName+="化.wav";
				break;
			}
		case 17:
			{
				szSoundName+="千.wav";
				break;
			}
		case 18:
			{
				szSoundName+="可.wav";
				break;
			}
		case 19:
			{
				szSoundName+="知.wav";
				break;
			}
		case 20:
			{
				szSoundName+="礼.wav";
				break;
			}
		case 21:
			{
				szSoundName+="子.wav";
				break;
			}
		}
	}

	PlaySound(szSoundName,NULL,SND_FILENAME|SND_ASYNC);

	return;
}
//播放声音
void CGameClientDlg::PlayActionSound(BYTE cbAction)
{
	
	CString szSoundName=_T("");
	if(m_wPlayNativeSound==1)
	{
		if(m_cbSex == 0)
		{
			szSoundName="Sound\\地方话\\girl\\";
		}
		else if(m_cbSex == 1)
		{
			szSoundName="Sound\\地方话\\boy\\";
		}
		else if(m_cbSex == 2)
		{
			szSoundName="Sound\\地方话\\girl\\";
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		
		if(m_cbSex == 0)
		{
			szSoundName="Sound\\普通话\\girl\\";
		}
		else if(m_cbSex == 1)
		{
			szSoundName="Sound\\普通话\\boy\\";
		}
		else if(m_cbSex == 2)
		{
			szSoundName="Sound\\普通话\\girl\\";
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	switch (cbAction)
	{
	case WIK_NULL:		//取消
		{
			break;
		}
	case WIK_DUI:		//对牌
		{
			szSoundName+="对.WAV";
			break;
		}
	case WIK_SAN_ZHAO:		//三招，四招
		{
			szSoundName+="开招.WAV";
			break;
		}
	case WIK_SI_ZHAO:
		{
			szSoundName+="团圆.WAV";
			break;
		}
	case WIK_SI_TONG:
	case WIK_WU_TONG:
		{
			szSoundName+="藏.WAV";
			break;
		}
	case WIK_CHI_HU:	//吃胡
		{
			szSoundName+="满.WAV";
			break;
		}
	}

	PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);

	return;
}

//显示控制
void CGameClientDlg::ShowTongOperateControl(BYTE cbUserAction, BYTE cbActionCard)
{
	//分析玩家的藏牌情况
	tagSiTongResult SiTongResult;
	tagWuTongResult WuTongResult;

	ZeroMemory(&SiTongResult,sizeof(SiTongResult));
	ZeroMemory(&WuTongResult,sizeof(WuTongResult));

	WORD wMeChairID=GetMeChairID();

	//从玩家牌索引数组中除去藏的牌，后判断对牌和招牌
	BYTE bTempIndex[MAX_INDEX];
	CopyMemory(bTempIndex,m_cbCardIndex,sizeof(bTempIndex));

	//删除已五藏的牌索引之后，再分析是否还有五藏的情况
	for (BYTE j=0; j<8; j++)
	{
		BYTE index = m_WuTongIndex[j];
		if (index != INVALID_CARDINDEX)
		{
			bTempIndex[index] = 0;
		}
	}
	m_GameLogic.AnalyseWuTong(bTempIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],WuTongResult);

	//再删除已四藏的牌索引之后，再分析是否还有四藏的情况
	for (BYTE j=0; j<8; j++)
	{
		BYTE index = m_SiTongIndex[j];
		if (index != INVALID_CARDINDEX)
		{
			bTempIndex[index] = 0;
		}
	}
	m_GameLogic.AnalyseSiTong(bTempIndex,SiTongResult);

	if((IsLookonMode()==false) && (SiTongResult.cbCardCount>0 || WuTongResult.cbCardCount>0))
	{
		//为了进入托管理而设置的
		m_cbSendCardFinish=true;

		m_GameClientView.m_ControlWnd.SetTongOperateCardData(cbActionCard,cbUserAction,SiTongResult,WuTongResult,m_cbHuaCardIndex);
	}
}

//根据所给的中心牌索引，操作类型，得出最佳组合牌数据，保存到胡牌组合中
bool CGameClientDlg::GetHuCardWeave(BYTE cbCardIndex[],BYTE cbWeaveKind[],BYTE cbYaKou[],BYTE cbHuaWeaveCount[],BYTE cbHuaHandCount[],BYTE cbWeaveCount,tagHuCardWeave &HuCardWeave)
{	
	BYTE cbCount=0;
	for (BYTE i=0; i<cbWeaveCount; i++)
	{
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		//如果是1,3,5,7,9,结合组合花牌进行分析
		if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6 ||cbCardIndex[i] == 8 )
		{
			count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,cbHuaWeaveCount[cbCardIndex[i]/2]);
		}
		else
		{
			count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);
		}
		//拷贝得到的结果
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	//断言组合牌个数不大于8
	ASSERT(i<= 8);
	
	//建立手中花牌临时索引
	BYTE cbHuaCardIndex[5] = {0,0,0,0,0};
	CopyMemory(cbHuaCardIndex,cbHuaHandCount,sizeof(cbHuaCardIndex));
	//标志癞子是否是放在组合牌中
	//(癞子摆放的原则是首先放在对牌,藏牌的组合中,如果能放在那里那么标志置为真.如果没有放在前面这种组合中那么就再放在其他类型的牌中,如果还没放那么就放在丫口中)
	bool LZUse=false; 

	//得到手中牌的组合
	for (; i<8; i++)
	{
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		if(cbWeaveKind[i]==WIK_QI_TA) continue;
		//1.WIK_QI_TA情况
		if (cbWeaveKind[i] != WIK_QI_TA)
		{
			//如果是1,3,5,7,9,结合组合花牌进行分析
			if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6 ||cbCardIndex[i] == 8 )
			{
				count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,cbHuaCardIndex[cbCardIndex[i]/2]);
				BYTE num=0;
				switch(cbWeaveKind[i])
				{
				case WIK_WU_TONG:
					{
						num=5;
						break;
					}
				case WIK_SI_TONG:
					{
						num=4;
						break;
					}
				case WIK_DUI:
					{
						num=3;
						break;
					}
				}

				cbHuaCardIndex[cbCardIndex[i]/2] = 0;
			}
			else
			{
				count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);
			}

		}
		
		//拷贝得到的结果
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	for(BYTE i=0;i<8;i++)
	{
		if(cbWeaveKind[i]!=WIK_QI_TA) continue;
		
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);

		//花牌校正
		//中心牌是:一，三，五,七(可能含有两张花牌)
		if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6)
		{
			//判断中心牌是:一（三），三（五），五（七），七（九）的花牌是否为0
			if (cbHuaCardIndex[cbCardIndex[i]/2] != 0)
			{
				//校正第0位花牌
				cbCardData[0] = cbCardData[0]|0x10;
				cbHuaCardIndex[cbCardIndex[i]/2] -= 1;
			}
			//校正第2位花牌
			if (cbHuaCardIndex[cbCardIndex[i]/2+1] != 0)
			{
				cbCardData[2] = cbCardData[2]|0x10;
				cbHuaCardIndex[cbCardIndex[i]/2+1] -= 1;
			}
		}
		//中心牌是 二，四，六，八(只可能含有一张花牌),判断花牌三，五，七,九是否为0
		if (cbCardIndex[i] == 1 || cbCardIndex[i] == 3||cbCardIndex[i] == 5 || cbCardIndex[i] == 7)
		{
			//校正中间的花牌数据
			if (cbHuaCardIndex[(cbCardIndex[i]+1)/2] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[(cbCardIndex[i]+1)/2] -= 1;
			}
		}
		//孔乙已：孔(10)
		if (cbCardIndex[i] == 10)
		{
			//判断是否有花乙
			if (cbHuaCardIndex[0] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[0] -= 1;
			}
		}
        //七十土:土(15)
		if (cbCardIndex[i] == 15)
		{
			//判断是否有花七
			if (cbHuaCardIndex[3] != 0)
			{
				cbCardData[0] = cbCardData[0]|0x10;
				cbHuaCardIndex[3] -= 1;
			}
		}
		//化三千:化(16)
		if (cbCardIndex[i] == 16)
		{
			//判断三是否有花三
			if (cbHuaCardIndex[1] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[1] -= 1;
			}
		}
		//八九子:子(21)
		if (cbCardIndex[i] == 21)
		{
			//判断是否有花九
			if (cbHuaCardIndex[4] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[4] -= 1;
			}
		}

		//拷贝得到的结果
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	////保存丫口数据
	for (BYTE i=0; i<2; i++)
	{
		//1,3,5,7,9，转换
		if (cbYaKou[i] == 0 ||cbYaKou[i] == 2 ||cbYaKou[i] == 4 ||cbYaKou[i] == 6 ||cbYaKou[i] == 8 )
		{
			//根据花牌个数进行转换
			if (cbHuaCardIndex[cbYaKou[i]/2] != 0)
			{
				HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i])|0x10;
				cbHuaCardIndex[cbYaKou[i]/2]--;
			}
			//没有花牌
			else
			{
				HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i]);
			}

		}
		//非1，3，5，7，9，转换
		else
		{
			HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i]);
		}

	}
	return true;
}




//根据所给的中心牌索引，操作类型，得出最佳组合牌数据，保存到胡牌组合中
bool CGameClientDlg::GetHuCardWeave(BYTE cbCardIndex[],BYTE cbWeaveKind[],BYTE cbYaKou[],tagHuCardWeave &HuCardWeave)
{
	//根据当前玩家操作类型,操作中心牌,组合花牌,得到组合牌数据
	WORD wMeChairID=GetMeChairID();
	BYTE cbCount=0;
	for (BYTE i=0; i<m_cbWeaveCount[wMeChairID]; i++)
	{
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		//如果是1,3,5,7,9,结合组合花牌进行分析
		if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6 ||cbCardIndex[i] == 8 )
		{
			count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,m_cbHuaWeaveCardIndex[cbCardIndex[i]/2]);
		}
		else
		{
			count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);
		}
		//拷贝得到的结果
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	//断言组合牌个数不大于8
	ASSERT(i<= 8);
	
	//建立手中花牌临时索引
	BYTE cbHuaCardIndex[5] = {0,0,0,0,0};
	CopyMemory(cbHuaCardIndex,m_cbHuaCardIndex,sizeof(cbHuaCardIndex));
	//标志癞子是否是放在组合牌中
	//(癞子摆放的原则是首先放在对牌,藏牌的组合中,如果能放在那里那么标志置为真.如果没有放在前面这种组合中那么就再放在其他类型的牌中,如果还没放那么就放在丫口中)
	bool LZUse=false; 

	//得到手中牌的组合
	for (; i<8; i++)
	{
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		if (cbWeaveKind[i] == WIK_QI_TA) continue;
		//1.非WIK_QI_TA情况
		if (cbWeaveKind[i] != WIK_QI_TA)
		{
			//如果是1,3,5,7,9,结合组合花牌进行分析
			if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6 ||cbCardIndex[i] == 8 )
			{
				count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,cbHuaCardIndex[cbCardIndex[i]/2]);
				BYTE num=0;
				switch(cbWeaveKind[i])
				{
				case WIK_WU_TONG:
					{
						num=5;
						break;
					}
				case WIK_SI_TONG:
					{
						num=4;
						break;
					}
				case WIK_DUI:
					{
						num=3;
						break;
					}
				}

				cbHuaCardIndex[cbCardIndex[i]/2] = 0;
			}
			else
			{
				count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);
			}

		}
		//拷贝得到的结果
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}

	for(BYTE j=0;j<8;j++)
	{
		
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		if (cbWeaveKind[j] != WIK_QI_TA) continue;
		count = m_GameLogic.GetHuCardWeave(cbWeaveKind[j],cbCardIndex[j],cbCardData,0);

		//花牌校正
		//中心牌是:一，三，五,七(可能含有两张花牌)
		if (cbCardIndex[j] == 0 ||cbCardIndex[j] == 2 ||cbCardIndex[j] == 4 ||cbCardIndex[j] == 6)
		{
			//判断中心牌是:一（三），三（五），五（七），七（九）的花牌是否为0
			if (cbHuaCardIndex[cbCardIndex[j]/2] != 0)
			{
				//校正第0位花牌
				cbCardData[0] = cbCardData[0]|0x10;
				cbHuaCardIndex[cbCardIndex[j]/2] -= 1;
			}
			//校正第2位花牌
			if (cbHuaCardIndex[cbCardIndex[j]/2+1] != 0)
			{
				cbCardData[2] = cbCardData[2]|0x10;
				cbHuaCardIndex[cbCardIndex[j]/2+1] -= 1;
			}
		}
		//中心牌是 二，四，六，八(只可能含有一张花牌),判断花牌三，五，七,九是否为0
		if (cbCardIndex[j] == 1 || cbCardIndex[j] == 3||cbCardIndex[j] == 5 || cbCardIndex[j] == 7)
		{
			//校正中间的花牌数据
			if (cbHuaCardIndex[(cbCardIndex[j]+1)/2] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[(cbCardIndex[j]+1)/2] -= 1;
			}
		}
		//孔乙已：孔(10)
		if (cbCardIndex[j] == 10)
		{
			//判断是否有花乙
			if (cbHuaCardIndex[0] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[0] -= 1;
			}
		}
        //七十土:土(15)
		if (cbCardIndex[j] == 15)
		{
			//判断是否有花七
			if (cbHuaCardIndex[3] != 0)
			{
				cbCardData[0] = cbCardData[0]|0x10;
				cbHuaCardIndex[3] -= 1;
			}
		}
		//化三千:化(16)
		if (cbCardIndex[j] == 16)
		{
			//判断三是否有花三
			if (cbHuaCardIndex[1] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[1] -= 1;
			}
		}
		//八九子:子(21)
		if (cbCardIndex[j] == 21)
		{
			//判断是否有花九
			if (cbHuaCardIndex[4] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[4] -= 1;
			}
		}
		
		//拷贝得到的结果
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	////保存丫口数据
	for (BYTE i=0; i<2; i++)
	{
		//1,3,5,7,9，转换
		if (cbYaKou[i] == 0 ||cbYaKou[i] == 2 ||cbYaKou[i] == 4 ||cbYaKou[i] == 6 ||cbYaKou[i] == 8 )
		{
			//根据花牌个数进行转换
			if (cbHuaCardIndex[cbYaKou[i]/2] != 0)
			{
				HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i])|0x10;
				cbHuaCardIndex[cbYaKou[i]/2]--;
			}
			//没有花牌
			else
			{
				HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i]);
			}

		}
		//非1，3，5，7，9，转换
		else
		{
			HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i]);
		}

	}
	return true;
}


//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//---------------以下清空是窗口中的显示控件信息----------------------//

	//环境设置
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	for (BYTE i=0; i<9; i++)
	{
		m_GameClientView.m_HandCard[i].SetCardData(NULL,0);
	}

	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

	m_IsTuoGuan=false;
	m_cbSendCardFinish=false;

	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	m_GameClientView.m_IsStartSendCard=false;

	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	//设置界面
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetHD(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_wMinusHeadCount=1;
	m_GameClientView.m_wMinusLastCount=0;

	//扑克设置
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);

	ZeroMemory(m_GameClientView.m_cbTongCount,sizeof(m_GameClientView.m_cbTongCount));
	//堆立扑克
	m_GameClientView.m_leaveHeapCardData=0;

	//扑克设置
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		
		for(BYTE j=0;j<8;j++)
		{
			m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
		}
	}

	//---------------以下清空是游戏中的变量信息----------------------//

	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	m_SiTongCount =0;
	m_WuTongCount =0;
	for (BYTE i=0; i<8; i++)
	{
		m_WuTongIndex[i] = INVALID_CARDINDEX;
		m_SiTongIndex[i] = INVALID_CARDINDEX;
	}

	//清空记录花牌的情况
	ZeroMemory(m_cbHuaCardIndex,sizeof(m_cbHuaCardIndex));
	ZeroMemory(m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));

	//清空记录组合牌的情况
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	m_bGameStart = false;

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	b_IsHD=false;

	//出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}


//取消托管按钮
LRESULT CGameClientDlg::OnDisTuoGuan(WPARAM wParam, LPARAM lParam)
{
    //出牌判断
	//if (m_bGameStart==true && (m_wCurrentUser!=GetMeChairID())) return 0;

	m_GameClientView.m_btTuoGuan.ShowWindow(SW_SHOW);
	m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_HIDE);

	m_IsTuoGuan=false;

	return 0;
}

//托管按钮
LRESULT CGameClientDlg::OnTuoGuan(WPARAM wParam, LPARAM lParam)
{	

	//出牌判断
	m_GameClientView.m_btTuoGuan.ShowWindow(SW_HIDE);
	m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_SHOW);
	m_IsTuoGuan=true;

	
	return 0;
}

//出牌操作
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//出牌判断
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return 0;

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;

	m_cbActionMask = 0;
	m_cbActionCard = 0;

	BYTE cbOutCardIndex=m_GameLogic.SwitchToCardIndex(cbOutCardData);

	//如果打出去的是花牌，花牌索引位置的数量-1
	if ( (cbOutCardData&MASK_COLOR) == 0x10 )
	{
		//断言该花牌不为0
		ASSERT(m_cbHuaCardIndex[cbOutCardIndex/2] != 0);
		if (m_cbHuaCardIndex[cbOutCardIndex/2] == 0)
		{
			//AfxMessageBox("喔哦！不好意思，花牌出错了！");
			return 0;
		}

		//花牌索引位置减1
		m_cbHuaCardIndex[cbOutCardIndex/2]--;
	}

	//手中牌索引位置减1
	m_cbCardIndex[cbOutCardIndex]--;

	//禁止用户出牌操作
	DisableOutCard();

	KillGameTimer(IDI_OPERATE_CARD);

	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(1,cbOutCardData);

	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

	m_GameClientView.UpdateGameView(NULL);

	if(IsEnableSound()==true)
	{
		m_cbSex = GetUserData(GetMeChairID())->cbGender;
		//播放声音
		PlayCardSound(cbOutCardData);
	}
	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
	
	return 0;
}

//扑克操作
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard=(BYTE)(lParam);

	//是第一轮的藏牌过程中用户点击弃，将操作转换成放弃藏牌操作再发到服务器端
	if ((cbOperateCode==WIK_NULL) && m_bGameStart == false )
	{
		cbOperateCode=WIK_TONG_GIVE_UP;
	}

	//用户点击弃牌操作的处理
	if (cbOperateCode==WIK_NULL)
	{
		//这里处理用户自己摸牌的情况--可能同时有胡牌与藏牌的情况--用户点击了胡牌的弃或藏牌的弃
		if(m_wCurrentUser==GetMeChairID())
		{
			//如果玩家点击了弃之后，操作码不为空，说明是点击了胡牌对话框上的弃，显示其他操作
			if (m_cbActionMask != 0 && IsLookonMode()==false)
			{
				//隐藏胡牌对话框
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

				ShowTongOperateControl(m_cbActionMask,0);
				m_cbActionMask = 0;
				
				return 0;
			}
			else
			{
				m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
			}
			
		}
		//这里处理其它用户出牌的情况--可能同时有胡牌与对招牌的情况--用户点击了胡牌的弃或对招牌的弃
		else 
		{
			//如果玩家点击了弃之后，操作码不为空，说明是点击了胡牌对话框上的弃，显示其他操作
			if (m_cbActionMask != 0 && IsLookonMode()==false)
			{
				//隐藏胡牌对话框
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

				m_GameClientView.m_ControlWnd.SetDuiZhaoOperateCardData(m_cbActionCard,m_cbActionMask,m_cbHuaCardIndex);
				
				m_cbActionMask=0;
				m_cbActionCard=0;

				return 0;//继续显示操作提示
			}
			else
			{
				m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
			}
		}
	}

	//环境设置
	KillGameTimer(IDI_OPERATE_CARD);
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

bool CGameClientDlg::SetWoindowData(BYTE cbCardData[],BYTE cbDataCount)
{
	if (m_wCurrentUser != GetMeChairID())
	{
		//AfxMessageBox("啊！错误发牌啦！不是轮到给我发牌也发了一张牌给我~!!@");
	}
	
	BYTE addCount=0;
	for (int i=8; i>=0; i--)
	{
		///////////////???????????需要修改的地方，将9换成每个区域的牌个数
		if (m_GameClientView.m_HandCard[i].m_cbCardCount<9 && addCount<cbDataCount)
		{
			m_GameClientView.m_HandCard[i].AddCardData(cbCardData[addCount]);
			addCount++;
			
			//为的是下回还要接着判断该区域	
			i=i+1;
		}			
	}

	return true;
}

bool CGameClientDlg::AddUserHandCard(BYTE cbCardData,BYTE cbDataCount)
{
	//if (m_wCurrentUser != GetMeChairID())
	//{
	//	//AfxMessageBox("啊！错误发牌啦！不是轮到给我发牌也发了一张牌给我~!!@");
	//}
	
	for (int i=8; i>=0; i--)
	{
		///////////////???????????需要修改的地方，将9换成每个区域的牌个数
		if (m_GameClientView.m_HandCard[i].m_cbCardCount<9)
		{
			m_GameClientView.m_HandCard[i].AddCardData(cbCardData);
			
			//为的是下回还要接着判断该区域	
			break;
		}			
	}

	//if (addCount != cbDataCount)
	//{
	//	CString sz;
	//	sz.Format("实际增加的牌个数为:%d,服务器发送过来的牌个数为：%d",addCount,cbDataCount);
	//	//AfxMessageBox(sz);
	//}

	//if (m_GameClientView.m_HandCard[8].m_cbCardCount != 0)
	//{
	//	CString sz;
	//	sz.Format("注意啊！第九区有牌啦！牌数为:%d",m_GameClientView.m_HandCard[8].m_wCardCount);
	//	//AfxMessageBox(sz);
	//}
	return true;
}

//设置放牌窗口是响应鼠标事件
void CGameClientDlg::EnableOutCard()
{

	for(BYTE i=0;i<9;i++)
	{
		m_GameClientView.m_HandCard[i].m_bCanOutCard=true;
	}
	
}
//设置放牌窗口不响应鼠标事件
void CGameClientDlg::DisableOutCard()
{
	for(BYTE i=0;i<9;i++)
	{
		m_GameClientView.m_HandCard[i].m_bCanOutCard=false;
	}
}
//发送牌动画结束
LRESULT CGameClientDlg::OnSendOneCardFinish(WPARAM wParam, LPARAM lParam)
{
	BYTE cbCardData=(BYTE)lParam;
	WORD wViewChairID=(WORD)wParam;

	if(m_bGameStart==false)
	{
		if(wViewChairID==1)
		{
			//将牌加入到相应的分区中
			AddUserHandCard(cbCardData,1);
		}
	}
	//正常游戏开始之后的藏牌发牌的动画或用户自摸牌而发牌的动画
	else if(m_bGameStart==true)
	{
		if(wViewChairID==1)
		{
			//将牌加入到相应的分区中
			AddUserHandCard(cbCardData,1);
		}
	}
	if(IsEnableSound()==true)
	{
		CString szSoundName=_T("");
		szSoundName="Sound\\";
		szSoundName+="SEND_CARD.wav";
		PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
	}
	return 0;
}

//发送牌动画结束
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//如果是旁观用户那么禁止其出牌
	if(IsLookonMode()==true)
	{
		DisableOutCard();
	}

	//第一轮的藏牌的发牌动画的结束
	if(m_bGameStart==false)
	{
		//在第一轮的藏牌过程中，将牌发给谁，谁在处理过程中就是当前用户--处理完了之后，会将其清空的
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_bCardCount[m_wCurrentUser] += m_cbSendCount;

		//左右两边的用户,仅设置其牌数和启动定时器
		if (wViewChairID!=1)
		{
			m_GameClientView.m_UserCard[wViewChairID].AddCardCount(m_cbSendCount);

			if(m_cbActionMask!=WIK_NULL)
			{
				SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
			}
		}
		//当前玩家
		else
		{
			//根据发牌的张数进行保存
			for (BYTE i=0; i<m_cbSendCount; i++)
			{
				//保存发的两张牌到索引中
				m_cbCardIndex[m_GameLogic.SwitchToCardIndex(m_cbSendCardData[i])]++;
			}

			//保存花牌到花牌索引数组中
			m_GameLogic.SaveHuaCard(m_cbSendCardData,m_cbSendCount,m_cbHuaCardIndex);
			////将牌加入到相应的分区中
			//SetWoindowData(m_cbSendCardData,m_cbSendCount);

//还有藏牌的话，则继续显示藏牌
//----特别强调说明，本来是要在此显示用户能否进行藏牌操作的。
//----但对于第一轮的藏处理来说，要先进行处理之后，才能再继续显示操作提示框。
//----故现将藏操作移动了操作结果处理的位置进行显示


			//如果有胡牌操作，显示胡牌最佳组合
			if ((GetMeChairID()==m_wBankerUser) && (m_cbActionMask & WIK_CHI_HU)!=0)
			{
				tagHuCardWeave HuCardweave;
				ZeroMemory(&HuCardweave,sizeof(HuCardweave));
			
				GetHuCardWeave(m_cbCenterCard,m_cbWeaveKind,m_cbYaKou,HuCardweave);
				m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,m_cbHuScore,m_cbGoldCard);

				//显示胡牌定时器
				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
			}
			//判断除了胡之外是否还有其他操作
			else if((m_cbActionMask & WIK_SI_TONG)!=0 || (m_cbActionMask & WIK_WU_TONG)!=0)
			{
				ShowTongOperateControl(m_cbActionMask,0);
				m_cbActionMask = 0;

				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
			}
		}

		//处理完了之后又将当前用户还原为空--表明是第一轮的藏牌
		m_wCurrentUser =INVALID_CHAIR;
		
		//更新界面
		m_GameClientView.UpdateGameView(NULL);

		m_cbSendCardFinish=true;
		return true;
	}
	//正常游戏开始之后的藏牌发牌的动画或用户自摸牌而发牌的动画
	else if(m_bGameStart==true)
	{

		//设置视图界面的显示，如果是当前玩家，将扑克保存到玩家手中牌索引数组中，否则，直接设置玩家当前牌可用
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_bCardCount[m_wCurrentUser] += m_cbSendCount;

		//如果是当前用户
		if (m_wCurrentUser == GetMeChairID())
		{
			//根据发牌的张数进行保存
			for (BYTE i=0; i<m_cbSendCount; i++)
			{
				//保存发的两张牌到索引中
				m_cbCardIndex[m_GameLogic.SwitchToCardIndex(m_cbSendCardData[i])]++;
			}
			//保存花牌到花牌索引数组中
			m_GameLogic.SaveHuaCard(m_cbSendCardData,m_cbSendCount,m_cbHuaCardIndex);
			////将牌加入到相应的分区中
			//SetWoindowData(m_cbSendCardData,m_cbSendCount);
		}
		//不是当前用户
		else
		{
//对于左右两边用户手中牌处理的说明：！！！！！！！！！！！
//当为当前用户时，其有一张牌为当前牌，此时总牌数是少1张的
//只要在拿牌、出牌和操作牌时能保持牌的数目一致性就可以了。
			if (m_cbSendCount == 2 )
			{
				m_GameClientView.m_UserCard[wViewChairID].AddCardCount(1);
			}
			if(b_IsHD==false)
			{
				m_GameClientView.m_UserCard[wViewChairID].SetCurrentCard(true);
			}
		}

		//当前用户
		if (m_wCurrentUser==GetMeChairID())
		{
			//激活框架
			ActiveGameFrame();

			//允许用户出牌操作
			EnableOutCard();

			//动作处理
			if (m_cbActionMask!=WIK_NULL)
			{
				//此处设计的思路为：
/*
//       1、当发牌到用户手中时，如果同时有胡牌操作与藏牌操作的话，先显示胡牌操作对话框。
//   只有当用户放弃胡牌操作时，再显示藏牌操作对话框。
//       2、在显示胡牌操作对话框时，要屏蔽m_cbActionMask中的胡牌操作码，以便用户选择
//   放弃胡牌操作时，在OnCardOperat函数中再次显示藏牌操作对话框。
//       3、当仅有藏牌操作时，显示藏牌对话框之后，也是将m_cbActionMask清空。
*/
				if ((m_cbActionMask&WIK_CHI_HU)!=0)
				{
					//屏蔽掉胡牌后，设置界面
					m_cbActionMask = (m_cbActionMask & 0x7F);

					tagHuCardWeave HuCardweave;
					ZeroMemory(&HuCardweave,sizeof(HuCardweave));
				
					GetHuCardWeave(m_cbCenterCard,m_cbWeaveKind,m_cbYaKou,HuCardweave);
					m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,m_cbHuScore,m_cbGoldCard);

					//显示胡牌定时器
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}
				//判断除了胡之外是否还有其他操作
				else 
				{
					ShowTongOperateControl(m_cbActionMask,0);
					m_cbActionMask = 0;

					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}
			}
			///应该自己出牌了，给出提示
			else
			{
				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);

				//出牌提示
				m_GameClientView.SetStatusFlag(true,false);
			}
		}

		//当当前用户有操作时，也要启动其它用户的定时器
		if((m_cbActionMask!=WIK_NULL) && (GetMeChairID()!=m_wCurrentUser))
		{
			SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}

		//更新界面
		m_GameClientView.UpdateGameView(NULL);

		m_cbSendCardFinish=true;

		return true;
	}

	m_cbSendCardFinish=true;
	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CGameFrameDlg::OnTimer(nIDEvent);
}

BOOL CGameClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CGameFrameDlg::PreTranslateMessage(pMsg);
}

//设置游戏开始之后的界面
void CGameClientDlg::GameStartedFace(void)
{

}
