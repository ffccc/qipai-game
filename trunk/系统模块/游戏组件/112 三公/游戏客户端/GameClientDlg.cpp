#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
//IDB_WAIT_CONTINUE IDB_WAIT_BANKER
//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_BANKER_START			200									//庄家开始
#define IDI_PLAYER_START			201									//玩家开始
#define IDI_CHANGE_BANKER			202									//抢庄时间

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_SCORE,OnScore)
	ON_MESSAGE(IDM_CLEAN,OnClean)
	ON_MESSAGE(IDM_BANKER,OnBanker)
	ON_MESSAGE(IDM_CHIP_IN,OnClipin)
	ON_MESSAGE(IDM_CONTINUE,OnContinue)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_RECLAIM_CARD_FINISH,OnReclaimCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//下注信息
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//历史记录
	m_wDrawCount=0;
	m_lGameScore=0L;

	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//辅助变量
	m_bGameScene=false;
	ZeroMemory(m_lChipInCount,sizeof(m_lChipInCount));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

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

	UINT uIDButton=m_btControl.GetDlgCtrlID();
	PostMessage(WM_COMMAND,uIDButton,0);

	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//下注信息
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//历史记录
	m_wDrawCount=0;
	m_lGameScore=0L;

	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//辅助变量
	m_bGameScene=false;
	ZeroMemory(m_lChipInCount,sizeof(m_lChipInCount));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

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
	switch (nTimerID)
	{
	case IDI_BANKER_START:		//庄家开始
		{
			if ((IsLookonMode()==false)&&(nElapse==0)&&(m_wBankerUser==GetMeChairID()))
			{
				OnStart(0,0);
			}

			return true;
		}
	case IDI_PLAYER_START:		//闲家开始
		{
			if ((IsLookonMode()==false)&&(nElapse==0)&&(m_wBankerUser!=GetMeChairID()))
			{
				WORD wMeChairID=GetMeChairID();
				const tagUserData * pUserData=GetUserData(wMeChairID);
				tagUserData * pMeUserData=(tagUserData *)pUserData;
				pMeUserData->cbUserStatus=US_SIT;
				PostMessage(WM_CLOSE,0,0);
			}

			return true;
		}
	case IDI_CHANGE_BANKER:		//抢庄时间
		{
			if ((IsLookonMode()==false)&&(nElapse==0)&&(m_wBankerUser!=GetMeChairID()))
			{
				OnBanker(0,0);
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
	case SUB_S_TIMER_INFO:		//时间信息
		{
			if (m_bGameScene==false) return true;
			return OnSubTimeInfo(pBuffer,wDataSize);
		}
	case SUB_S_SCORE_INFO:		//下注信息
		{
			if (m_bGameScene==false) return true;
			return OnSubScoreInfo(pBuffer,wDataSize);
		}
	case SUB_S_BANKER_USER:		//庄家信息
		{
			if (m_bGameScene==false) return true;
			return OnSubBankerUser(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_INFO:		//抢庄信息
		{
			if (m_bGameScene==false) return true;
			return OnSubChangeInfo(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			if (m_bGameScene==false) return true;
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_USER_CHIPIN:		//用户下注
		{
			if (m_bGameScene==false) return true;
			return OnSubUserChipin(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发送扑克
		{
			if (m_bGameScene==false) return true;
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			if (m_bGameScene==false) return true;
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return false;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetUserPost(i,SwitchViewChairID(i));
	}

	switch (cbGameStation)
	{
	case GS_FREE:		//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//变量定义
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//辅助变量
			m_bGameScene=true;

			//设置变量
			m_lMaxScore=pStatusFree->lMaxScore;
			m_lLessScore=pStatusFree->lLessScore;
			m_wBankerUser=pStatusFree->wBankerUser;

			//更新位置
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetUserChairInfo(SwitchViewChairID(m_wBankerUser),SwitchViewChairID(wMeChairID));

			//设置界面
			m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);
			m_GameClientView.SetLongBanker((pStatusFree->bLongBanker==TRUE)?true:false);

			//筹码按钮
			if ((IsLookonMode()==false)&&(m_lMaxScore!=0L)) UpdateClipinText(m_lMaxScore);

			//开始设置
			if (pStatusFree->wLeaveTime>0)
			{
				//设置界面
				m_GameClientView.SetWaitBanker(true);

				//设置时间
				ASSERT(m_wBankerUser!=INVALID_CHAIR);
				SetGameTimer(m_wBankerUser,IDI_BANKER_START,pStatusFree->wLeaveTime);
			}
			if((GetUserData(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0)
			{
				m_GameClientView.m_btControlMax.ShowWindow(SW_SHOW);
			}
			return true;
		}
	case GS_TK_PLAYING:	//游戏状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//变量定义
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//辅助变量
			m_bGameScene=true;

			//设置变量
			m_lMaxScore=pStatusPlay->lMaxScore;
			m_lLessScore=pStatusPlay->lLessScore;
			m_wBankerUser=pStatusPlay->wBankerUser;
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));

			//保存用户
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				const tagUserData * pUserData=GetUserData(i);
				if (pUserData!=NULL) lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
			}

			//更新位置
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetUserChairInfo(SwitchViewChairID(m_wBankerUser),SwitchViewChairID(wMeChairID));

			//设置界面
			m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);
			m_GameClientView.SetWaitPlayer((pStatusPlay->cbSendCard==FALSE)?true:false);
			m_GameClientView.SetLongBanker((pStatusPlay->bLongBanker==TRUE)?true:false);

			//设置下注
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (pStatusPlay->lUserChipIn[i]>0L)
				{
					WORD wViewChairID=SwitchViewChairID(i);
					m_GameClientView.SetUserClipInInfo(wViewChairID,pStatusPlay->lUserChipIn[i]);
				}
			}

			//闲家设置
			if (GetMeChairID()!=m_wBankerUser)
			{
				//设置筹码
				UpdateClipinText(m_lMaxScore);

				//设置状态
				if ((pStatusPlay->cbSendCard==FALSE)&&(pStatusPlay->lUserChipIn[GetMeChairID()]==0))
				{
					//更新状态
					UpdateClipinStatus(m_lMaxScore);

					//显示筹码
					m_GameClientView.m_btScore1.ShowWindow(SW_SHOW);
					m_GameClientView.m_btScore2.ShowWindow(SW_SHOW);
					m_GameClientView.m_btScore3.ShowWindow(SW_SHOW);
					m_GameClientView.m_btScore4.ShowWindow(SW_SHOW);
					m_GameClientView.m_btScore5.ShowWindow(SW_SHOW);

					//加注按钮
					m_GameClientView.m_btClean.ShowWindow(SW_SHOW);
					m_GameClientView.m_btChipIn.ShowWindow(SW_SHOW);
					m_GameClientView.m_btClean.EnableWindow(FALSE);
					m_GameClientView.m_btChipIn.EnableWindow(FALSE);

					//设置时间
					SetGameTimer(GetMeChairID(),IDI_PLAYER_START,30);
				}
			}

			//发送扑克
			if (pStatusPlay->cbSendCard==TRUE)
			{
				//派发扑克
				if (pStatusPlay->cbNeedFinish==TRUE)
				{
					//派发扑克
					for (BYTE cbIndex=0;cbIndex<3;cbIndex++)
					{
						for (WORD i=0;i<GAME_PLAYER;i++)
						{
							//用户变量
							WORD wChairID=(m_wBankerUser+i+1)%GAME_PLAYER;

							//发牌处理
							if (m_cbPlayStatus[wChairID]==TRUE)
							{
								//变量定义
								WORD wViewChairID=SwitchViewChairID(wChairID);

								//派发扑克
								BYTE cbCardData=pStatusPlay->cbUserCardData[wChairID][cbIndex];
								if (cbCardData!=0) m_GameClientView.DispatchUserCard(wViewChairID,cbCardData);
							}
						}
					}
				}
				else
				{
					//设置扑克
					for (WORD i=0;i<GAME_PLAYER;i++)
					{
						//用户变量
						WORD wChairID=(m_wBankerUser+i+1)%GAME_PLAYER;

						//发牌处理
						if (m_cbPlayStatus[wChairID]==TRUE)
						{
							WORD wViewChairID=SwitchViewChairID(wChairID);
							WORD wViewChairIndex=m_GameClientView.m_wChairIndex[wViewChairID];
							m_GameClientView.m_CardControl[wViewChairIndex].SetCardData(pStatusPlay->cbUserCardData[wChairID],MAX_COUNT);
						}
					}
				}
			}
			if((GetUserData(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0)
			{
				m_GameClientView.m_btControlMax.ShowWindow(SW_SHOW);
			}

			return true;
		}
	}

	return false;
}

//取消消息
void CGameClientDlg::OnCancel()
{
	if (m_wBankerUser!=GetMeChairID()&&(m_GameClientView.m_btChipIn.IsWindowVisible()))
	{
		WORD wMeChairID=GetMeChairID();
		const tagUserData * pUserData=GetUserData(wMeChairID);
		tagUserData * pMeUserData=(tagUserData *)pUserData;
		pMeUserData->cbUserStatus=US_SIT;
	}

	__super::OnCancel();
}

//时间信息
bool CGameClientDlg::OnSubTimeInfo(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_TimerInfo));
	if (wDataSize!=sizeof(CMD_S_TimerInfo)) return false;

	//消息处理
	CMD_S_TimerInfo * pTimerInfo=(CMD_S_TimerInfo *)pBuffer;

	//设置时间
	if (pTimerInfo->wLeaveTime!=0L)
	{
		//设置时间
		SetGameTimer(m_wBankerUser,IDI_BANKER_START,pTimerInfo->wLeaveTime);

		//开始按钮
		if (IsLookonMode()==false)
		{
			WORD wMeChairID=GetMeChairID();
			if (wMeChairID==m_wBankerUser) m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		}

		//设置界面
		m_GameClientView.SetWaitBanker(true);
	}
	else
	{
		//删除时间
		KillGameTimer(IDI_BANKER_START);

		//开始按钮
		if (IsLookonMode()==false)
		{
			WORD wMeChairID=GetMeChairID();
			if (wMeChairID==m_wBankerUser) m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		}

		//设置界面
		m_GameClientView.SetWaitBanker(false);
	}

	return true;
}

//下注信息
bool CGameClientDlg::OnSubScoreInfo(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ScoreInfo));
	if (wDataSize!=sizeof(CMD_S_ScoreInfo)) return false;

	//消息处理
	CMD_S_ScoreInfo * pScoreInfo=(CMD_S_ScoreInfo *)pBuffer;

	//设置变量
	m_lMaxScore=pScoreInfo->lMaxScore;
	m_lLessScore=pScoreInfo->lLessScore;

	//更新筹码
	UpdateClipinText(m_lMaxScore);

	//设置界面
	m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);

	return true;
}

//庄家信息
bool CGameClientDlg::OnSubBankerUser(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_BankerUser));
	if (wDataSize!=sizeof(CMD_S_BankerUser)) return false;

	//消息处理
	CMD_S_BankerUser * pBankerUser=(CMD_S_BankerUser *)pBuffer;

	//设置变量
	m_lMaxScore=pBankerUser->lMaxScore;
	m_lLessScore=pBankerUser->lLessScore;
	m_wBankerUser=pBankerUser->wBankerUser;

	//设置位置
	WORD wMeChairID=GetMeChairID();
	m_GameClientView.SetUserChairInfo(SwitchViewChairID(m_wBankerUser),SwitchViewChairID(wMeChairID));

	//删除时间
	KillGameTimer(IDI_BANKER_START);

	//换庄设置
	if (GetMeChairID()==m_wBankerUser)
	{
		//删除时间
		KillGameTimer(IDI_CHANGE_BANKER);

		//设置按钮
		m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
		m_GameClientView.m_btNoBanker.ShowWindow(SW_HIDE);
	}

	//设置筹码
	UpdateClipinText(m_lMaxScore);

	//设置界面
	m_GameClientView.SetWaitBanker(false);
	m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);
	m_GameClientView.SetLongBanker((pBankerUser->bLongBanker==TRUE)?true:false);

	//视图筹码
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_GoldView[i].SetGold(0L);

	return true;
}

//抢庄信息
bool CGameClientDlg::OnSubChangeInfo(const void * pBuffer, WORD wDataSize)
{
	//设置时间
	SetGameTimer(GetMeChairID(),IDI_CHANGE_BANKER,30);

	//显示按钮
	m_GameClientView.m_btBanker.ShowWindow(SW_SHOW);
	m_GameClientView.m_btNoBanker.ShowWindow(SW_SHOW);

	//激活窗口
	if (IsLookonMode()==false) ActiveGameFrame();

	return true;
}

//下注信息
bool CGameClientDlg::OnSubUserChipin(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserChipin));
	if (wDataSize!=sizeof(CMD_S_UserChipin)) return false;

	//消息处理
	CMD_S_UserChipin * pUserChipin=(CMD_S_UserChipin *)pBuffer;

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOOSE_SCORE"));

	//设置筹码
	WORD wViewChairID=SwitchViewChairID(pUserChipin->wChairID);
	m_GameClientView.SetUserClipInInfo(wViewChairID,pUserChipin->lUserChipin);

	return true;
}

//发送扑克
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//消息处理
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//设置状态
	m_GameClientView.SetWaitBanker(false);
	m_GameClientView.SetWaitPlayer(false);

	//派发扑克
	for (BYTE cbIndex=0;cbIndex<3;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//用户变量
			WORD wChairID;
			if(GetMeChairID()==m_wBankerUser) 
			{
				wChairID=(m_wBankerUser+i+1)%GAME_PLAYER;
			}
			else 
			{
				WORD wBankerViewID=m_GameClientView.GetBankerViewID();
				wChairID=m_GameClientView.GetUserPost((wBankerViewID+i+1)%GAME_PLAYER);
			}

			//发牌处理
			if ((m_cbPlayStatus[wChairID]==TRUE)&&(GetUserData(wChairID)!=NULL))
			{
				//变量定义
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//派发扑克
				BYTE cbCardData=pSendCard->cbUserCardData[wChairID][cbIndex];
				if (cbCardData!=0) m_GameClientView.DispatchUserCard(wViewChairID,cbCardData);
			}
		}
	}

	//删除时间
	KillGameTimer(0);

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
	m_lMaxScore=pGameStart->lMaxScore;
	m_lLessScore=pGameStart->lLessScore;
	CopyMemory(m_cbPlayStatus,pGameStart->cbPlayStatus,sizeof(m_cbPlayStatus));

	//历史记录
	m_wDrawCount++;
	m_GameClientView.SetUserDrawInfo(m_wDrawCount,m_lGameScore);

	//保存用户
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		if (pUserData!=NULL) lstrcpyn(m_szAccounts[i],pUserData->szName,CountArray(m_szAccounts[i]));
	}

	//设置界面
	KillGameTimer(IDI_BANKER_START);
	m_GameClientView.SetWaitPlayer(true);
	m_GameClientView.SetWaitBanker(false);
	m_GameClientView.SetChipInInfo(m_lMaxScore,m_lLessScore);

	//设置界面
	m_GameClientView.m_ScoreView.ResetScore();

	//闲家设置
	if (m_wBankerUser!=GetMeChairID())
	{
		//更新筹码
		UpdateClipinText(m_lMaxScore);
		UpdateClipinStatus(m_lMaxScore);

		//设置按钮
		m_GameClientView.m_btClean.ShowWindow(SW_SHOW);
		m_GameClientView.m_btChipIn.ShowWindow(SW_SHOW);
		m_GameClientView.m_btClean.EnableWindow(FALSE);
		m_GameClientView.m_btChipIn.EnableWindow(FALSE);
	}

	//设置时间
	SetGameTimer(GetMeChairID(),IDI_PLAYER_START,30);

	//激活框架
	if (IsLookonMode()==false) ActiveGameFrame();

	return true;
}

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//删除时间
	KillGameTimer(IDI_PLAYER_START);

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//结束发牌
	m_GameClientView.FinishDispatchCard();

	//设置成绩
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取索引
		WORD wIndex=(m_wBankerUser+i)%GAME_PLAYER;

		//设置信息
		if (m_cbPlayStatus[wIndex]==TRUE)
		{
			//设置用户
			LPCTSTR pszUserName=m_szAccounts[wIndex];
			if (m_szAccounts[wIndex][0]==0) pszUserName=TEXT("已离开");

			//设置成绩
			m_GameClientView.m_ScoreView.SetGameScore(i,pszUserName,pGameEnd->lGameScore[wIndex],pGameEnd->lGameTax[wIndex]);
		}
	}

	//显示成绩
	if (m_wBankerUser==GetMeChairID())
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE_BANKER,this);
	}
	else
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE_PLAYER,this);
	}

	//设置控件
	m_GameClientView.m_btClean.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipIn.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore1.EnableWindow(FALSE);
	m_GameClientView.m_btScore2.EnableWindow(FALSE);
	m_GameClientView.m_btScore3.EnableWindow(FALSE);
	m_GameClientView.m_btScore4.EnableWindow(FALSE);
	m_GameClientView.m_btScore5.EnableWindow(FALSE);

	//收起筹码
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (pGameEnd->lGameScore[i]>0)
		{
			WORD wViewChairID=SwitchViewChairID(i);
			WORD wIndex=m_GameClientView.m_wChairIndex[wViewChairID];
			m_GameClientView.m_bBankerScore[wIndex]=false;
		}
		else
		{
			WORD wViewChairID=SwitchViewChairID(i);
			WORD wIndex=m_GameClientView.m_wChairIndex[wViewChairID];
			m_GameClientView.m_bBankerScore[wIndex]=true;
		}
	}

	m_GameClientView.ReclaimUserScore();

	//设置牌型
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取扑克
		BYTE cbCardData[MAX_COUNT];
		WORD wCardCount=m_GameClientView.m_CardControl[i].GetCardData(cbCardData,CountArray(cbCardData));

		//设置牌型
		if (wCardCount==MAX_COUNT)
		{
			//变量定义
			TCHAR szCardType[16]=TEXT("");
			BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,MAX_COUNT);

			//构造类型
			switch (cbCardType)
			{
			case CT_VALUES:		//点值类型
				{
					//获取牌点
					BYTE cbValueList=0;
					BYTE cbValueCard=0;
					for (BYTE j=0;j<MAX_COUNT;j++)
					{
						cbValueCard=m_GameLogic.GetCardValue(cbCardData[j]);
						if (cbValueCard<10) cbValueList=(cbValueList+cbValueCard)%10;
					}

					//构造描述
					_snprintf(szCardType,CountArray(szCardType),TEXT("%d 点"),cbValueList);

					break;
				}
			case CT_THREE_JQK:	//三公类型
				{
					//构造描述
					lstrcpyn(szCardType,TEXT("三 公"),CountArray(szCardType));

					break;
				}
			case CT_THREE_KING:	//三条类型
				{
					//构造描述
					lstrcpyn(szCardType,TEXT("三 条"),CountArray(szCardType));

					break;
				}
			}

			//设置类型
			m_GameClientView.SetCardTypeInfo(i,szCardType);
		}
		else 
		{
			//清理牌型
			m_GameClientView.SetCardTypeInfo(i,TEXT(""));
		}
	}

	//设置显示
	m_GameClientView.SetCardType(true);
	m_GameClientView.SetWaitBanker(false);
	m_GameClientView.SetWaitPlayer(false);

	//历史记录
	m_lGameScore+=pGameEnd->lGameScore[GetMeChairID()];
	m_GameClientView.SetUserDrawInfo(m_wDrawCount,m_lGameScore);

	//获取扑克
	BYTE cbCardData[MAX_COUNT];
	WORD wViewBanker=SwitchViewChairID(m_wBankerUser);
	WORD wUserIndex=m_GameClientView.m_wChairIndex[wViewBanker];
	WORD wCardCount=m_GameClientView.m_CardControl[wUserIndex].GetCardData(cbCardData,CountArray(cbCardData));

	//播放结果
	if(wCardCount>0)
	{
		//庄家类型
		BYTE cbBankerValue=0;
		BYTE cbBankerType=m_GameLogic.GetCardType(cbCardData,MAX_COUNT);

		//获取牌点
		if (cbBankerType==CT_VALUES)
		{
			for (BYTE i=0;i<MAX_COUNT;i++)
			{
				BYTE cbValueCard=m_GameLogic.GetCardValue(cbCardData[i]);
				if (cbValueCard<10) cbBankerValue=(cbBankerValue+cbValueCard)%10;
			}
		}

		//播放声音
		if (m_wBankerUser==GetMeChairID())
		{
			if (cbBankerType==CT_VALUES)
			{
				TCHAR szSound[32]=TEXT("");
				_snprintf(szSound,CountArray(szSound),TEXT("POINT_%d"),cbBankerValue);
				PlayGameSound(AfxGetInstanceHandle(),szSound);
			}
			else if (cbBankerType==CT_THREE_KING)
			{
				TCHAR szSound[32]=TEXT("");
				_snprintf(szSound,CountArray(szSound),TEXT("THREE_POINT_%d"),m_GameLogic.GetCardValue(cbCardData[0]));
				PlayGameSound(AfxGetInstanceHandle(),szSound);
			}
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("KING"));
		}
		else
		{
			if (pGameEnd->lGameScore[GetMeChairID()]>0)
			{
				if (cbBankerType==CT_VALUES)
				{
					TCHAR szSound[32]=TEXT("");
					_snprintf(szSound,CountArray(szSound),TEXT("LOSE_POINT_%d"),cbBankerValue);
					PlayGameSound(AfxGetInstanceHandle(),szSound);
				}
				else if (cbBankerType==CT_THREE_KING)
				{
					TCHAR szSound[32]=TEXT("");
					_snprintf(szSound,CountArray(szSound),TEXT("LOSE_THREE_POINT_%d"),m_GameLogic.GetCardValue(cbCardData[0]));
					PlayGameSound(AfxGetInstanceHandle(),szSound);
				}
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("LOSE_KING"));
			}
			else
			{
				if (cbBankerType==CT_VALUES)
				{
					TCHAR szSound[32]=TEXT("");
					_snprintf(szSound,CountArray(szSound),TEXT("WIN_POINT_%d"),cbBankerValue);
					PlayGameSound(AfxGetInstanceHandle(),szSound);
				}
				else if (cbBankerType==CT_THREE_KING)
				{
					TCHAR szSound[32]=TEXT("");
					_snprintf(szSound,CountArray(szSound),TEXT("WIN_THREE_POINT_%d"),m_GameLogic.GetCardValue(cbCardData[0]));
					PlayGameSound(AfxGetInstanceHandle(),szSound);
				}
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("WIN_KING"));
			}
		}
	}

	return true;
}

//更新筹码
void CGameClientDlg::UpdateClipinText(LONG lMaxScore)
{
	//设置筹码
	if (m_lMaxScore<=10L)
	{
		m_lChipInCount[0]=1L;
		m_lChipInCount[1]=2L;
		m_lChipInCount[2]=3L;
		m_lChipInCount[3]=5L;
		m_lChipInCount[4]=10L;
	}
	else if (m_lMaxScore<100L)
	{
		m_lChipInCount[0]=10L;
		m_lChipInCount[1]=20L;
		m_lChipInCount[2]=30L;
		m_lChipInCount[3]=40L;
		m_lChipInCount[4]=50L;
	}
	else if (m_lMaxScore<1000L)
	{
		m_lChipInCount[0]=10L;
		m_lChipInCount[1]=20L;
		m_lChipInCount[2]=30L;
		m_lChipInCount[3]=50L;
		m_lChipInCount[4]=100L;
	}
	else if (m_lMaxScore<10000L)
	{
		m_lChipInCount[0]=100L;
		m_lChipInCount[1]=200L;
		m_lChipInCount[2]=300L;
		m_lChipInCount[3]=500L;
		m_lChipInCount[4]=1000L;
	}
	else
	{
		m_lChipInCount[0]=1000L;
		m_lChipInCount[1]=2000L;
		m_lChipInCount[2]=3000L;
		m_lChipInCount[3]=5000L;
		m_lChipInCount[4]=10000L;
	}

	//构造字符
	TCHAR szScoreText[5][16];
	for (BYTE i=0;i<CountArray(m_lChipInCount);i++)
	{
		_snprintf(szScoreText[i],CountArray(szScoreText[i]),TEXT("%ld"),m_lChipInCount[i]);
	}

	//设置按钮
	m_GameClientView.m_btScore1.SetWindowText(szScoreText[0]);
	m_GameClientView.m_btScore2.SetWindowText(szScoreText[1]);
	m_GameClientView.m_btScore3.SetWindowText(szScoreText[2]);
	m_GameClientView.m_btScore4.SetWindowText(szScoreText[3]);
	m_GameClientView.m_btScore5.SetWindowText(szScoreText[4]);

	return;
}

//更新筹码
void CGameClientDlg::UpdateClipinStatus(LONG lLeaveScore)
{
	//设置按钮
	m_GameClientView.m_btScore1.EnableWindow((lLeaveScore>=m_lChipInCount[0])?TRUE:FALSE);
	m_GameClientView.m_btScore2.EnableWindow((lLeaveScore>=m_lChipInCount[1])?TRUE:FALSE);
	m_GameClientView.m_btScore3.EnableWindow((lLeaveScore>=m_lChipInCount[2])?TRUE:FALSE);
	m_GameClientView.m_btScore4.EnableWindow((lLeaveScore>=m_lChipInCount[3])?TRUE:FALSE);
	m_GameClientView.m_btScore5.EnableWindow((lLeaveScore>=m_lChipInCount[4])?TRUE:FALSE);

	return;
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//设置界面
	KillGameTimer(IDI_BANKER_START);

	//设置界面
	m_GameClientView.SetWaitPlayer(true);
	m_GameClientView.SetWaitBanker(false);

	//设置控件
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_GoldView[i].SetGold(0L);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
	}

	//隐藏控件
	m_GameClientView.m_ScoreView.ResetScore();
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore1.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore2.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore3.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore4.ShowWindow(SW_HIDE);
	m_GameClientView.m_btScore5.ShowWindow(SW_HIDE);

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//加注消息
LRESULT CGameClientDlg::OnScore(WPARAM wParam, LPARAM lParam)
{
	//获取下注
	ASSERT(wParam<CountArray(m_lChipInCount));
	LONG lScore=m_lChipInCount[(BYTE)(wParam)];

	//设置下注
	WORD wMeChairID=GetMeChairID();
	WORD wMeViewChairID=SwitchViewChairID(wMeChairID);
	LONG lMeTableScore=m_GameClientView.GetUserClipInInfo(wMeViewChairID);
	m_GameClientView.SetUserClipInInfo(wMeViewChairID,lMeTableScore+lScore);

	//更新筹码
	ASSERT((m_lMaxScore-lMeTableScore-lScore)>=0L);
	UpdateClipinStatus(m_lMaxScore-lMeTableScore-lScore);

	//加注按钮
	m_GameClientView.m_btClean.EnableWindow(TRUE);
	m_GameClientView.m_btChipIn.EnableWindow((lMeTableScore+lScore)>=m_lLessScore);

	return 0;
}

//清除消息
LRESULT CGameClientDlg::OnClean(WPARAM wParam, LPARAM lParam)
{
	//设置筹码
	m_GameClientView.m_GoldView[3].SetGold(0L);

	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	//设置控件
	UpdateClipinStatus(m_lMaxScore);
	m_GameClientView.m_btClean.EnableWindow(FALSE);
	m_GameClientView.m_btChipIn.EnableWindow(FALSE);

	return 0;
}

//下注消息
LRESULT CGameClientDlg::OnClipin(WPARAM wParam, LPARAM lParam)
{
	//删除时间
	KillGameTimer(IDI_PLAYER_START);

	//筹码控件
	m_GameClientView.m_btScore1.EnableWindow(FALSE);
	m_GameClientView.m_btScore2.EnableWindow(FALSE);
	m_GameClientView.m_btScore3.EnableWindow(FALSE);
	m_GameClientView.m_btScore4.EnableWindow(FALSE);
	m_GameClientView.m_btScore5.EnableWindow(FALSE);

	//设置空间
	m_GameClientView.m_btClean.ShowWindow(SW_HIDE);
	m_GameClientView.m_btChipIn.ShowWindow(SW_HIDE);

	//变量定义
	CMD_C_UserChipin UserChipin;
	ZeroMemory(&UserChipin,sizeof(UserChipin));

	//构造变量
	WORD wViewChairID=SwitchViewChairID(GetMeChairID());
	UserChipin.lScore=m_GameClientView.GetUserClipInInfo(wViewChairID);

	//发送消息
	SendData(SUB_C_USER_CHIPIN,&UserChipin,sizeof(UserChipin));

	return 0;
}

//抢庄消息
LRESULT CGameClientDlg::OnBanker(WPARAM wParam, LPARAM lParam)
{
	//删除时间
	KillGameTimer(IDI_CHANGE_BANKER);

	//隐藏按钮
	m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoBanker.ShowWindow(SW_HIDE);

	//构造变量
	CMD_C_ChangeInfo ChangeInfo;
	ZeroMemory(&ChangeInfo,sizeof(ChangeInfo));

	//设置变量
	ChangeInfo.cbChange=(wParam!=0L)?TRUE:FALSE;

	//发送消息
	SendData(SUB_C_CHANGE_INFO,&ChangeInfo,sizeof(ChangeInfo));

	return 0;
}

//继续消息
LRESULT CGameClientDlg::OnContinue(WPARAM wParam, LPARAM lParam)
{
	//收取扑克
	m_GameClientView.ReclaimUserCard();

	return 0;
}

//发牌完成
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//发送命令
	if (IsLookonMode()==false) SendData(SUB_C_SEND_FINISH);

	return 0;
}

//收牌完成
LRESULT CGameClientDlg::OnReclaimCardFinish(WPARAM wParam, LPARAM lParam)
{
	//设置控件
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_GoldView[i].SetGold(0L);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0L);
	}

	//设置界面
	m_GameClientView.SetCardType(false);
	m_GameClientView.SetWaitBanker(true);

	//发送继续
	if (IsLookonMode()==false)/*OnStart(0,0)*/ SendData(SUB_C_CONTINUE_GAME); 

	return 0;
}

//////////////////////////////////////////////////////////////////////////
