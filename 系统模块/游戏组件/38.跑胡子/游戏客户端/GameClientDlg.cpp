#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//游戏定时器
#define IDI_START_GAME				200									//开始定时器
#define IDI_OUT_CARD				201									//出牌定时器
#define IDI_OPERATE_CARD			202									//操作定时器
#define IDI_ASSISTANT_TIME			203									//辅助定时器

//动作定时器
#define IDI_USER_ACTION				400									//动作定时器
#define TIME_USER_ACTION			1000								//动作定时器

//时间间隔
#define TIME_OUT_CARD				30									//出牌时间
#define TIME_START_GAME				60									//开始时间
#define TIME_OPERATE_CARD			30									//操作时间
#define TIME_ASSISTANT_TIME			45									//操作时间

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CHOOSE_CARD,OnChooseCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCardCount,sizeof(m_bUserCardCount));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//辅助变量
	m_bOutCard=false;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//动作变量
	m_wActionCount=0;
	ZeroMemory(m_UserActionArray,sizeof(m_UserActionArray));

	//扑克变量
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

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
	SetWindowText(TEXT("湖南跑胡子  --  Ver：1.0.5.1"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载资源
	g_CardResource.LoadResource();

	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bUserCardCount,sizeof(m_bUserCardCount));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//辅助变量
	m_bOutCard=false;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//动作变量
	m_wActionCount=0;
	ZeroMemory(m_UserActionArray,sizeof(m_UserActionArray));

	//扑克变量
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

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
	case IDI_START_GAME:		//开始游戏
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return false;
			}
			if (nElapse==5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OUT_CARD:			//用户出牌
		{
			//超时判断
			if ((nElapse==0)&&(m_wCurrentUser==GetMeChairID())&&(IsLookonMode()==false))
			{
				//寻找单牌
				for (BYTE i=0;i<CountArray(m_cbCardIndex);i++)
				{
					if (m_cbCardIndex[i]==1)
					{
						OnOutCard(m_GameLogic.SwitchToCardData(i),0L);
						return false;
					}
				}

				//寻找对牌
				for (BYTE i=0;i<CountArray(m_cbCardIndex);i++)
				{
					if (m_cbCardIndex[i]==2)
					{
						OnOutCard(m_GameLogic.SwitchToCardData(i),0L);
						return false;
					}
				}

				return false;
			}

			//播放声音
			if ((nElapse==5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_OPERATE_CARD:		//操作定时器
		{
			//超时判断
			if ((nElapse==0)&&(IsLookonMode()==false))
			{
				OnCardOperate(ACK_NULL,0);
				return false;
			}

			//播放声音
			if ((nElapse==5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_ASSISTANT_TIME:	//辅助定时器
		{
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
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_USER_TI_CARD:	//用户提牌
		{
			return OnSubUserTiCard(pBuffer,wDataSize);
		}
	case SUB_S_USER_PAO_CARD:	//用户跑牌
		{
			return OnSubUserPaoCard(pBuffer,wDataSize);
		}
	case SUB_S_USER_WEI_CARD:	//用户偎牌
		{
			return OnSubUserWeiCard(pBuffer,wDataSize);
		}
	case SUB_S_USER_PENG_CARD:	//用户碰牌
		{
			return OnSubUserPengCard(pBuffer,wDataSize);
		}
	case SUB_S_USER_CHI_CARD:	//用户吃牌
		{
			return OnSubUserChiCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//操作提示
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD_NOTIFY:	//出牌提示
		{
			return OnSubOutCardNotify(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//出牌消息
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发牌消息
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return false;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//游戏信息
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetBankerUser(m_wBankerUser);
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);

			//控件设置
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);

			//设置控件
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}

			//更新界面
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_PLAYING:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//设置变量
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			m_bOutCard=(pStatusPlay->bOutCard==TRUE)?true:false;
			CopyMemory(m_cbCardIndex,pStatusPlay->cbCardIndex,sizeof(m_cbCardIndex));
			CopyMemory(m_bUserCardCount,pStatusPlay->bUserCardCount,sizeof(m_bUserCardCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(m_bWeaveItemCount,pStatusPlay->bWeaveItemCount,sizeof(m_bWeaveItemCount));

			//胡息信息
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//设置胡息
				m_bUserHuXiCount[i]=0;

				//胡息计算
				for (BYTE j=0;j<m_bWeaveItemCount[i];j++) 
				{
					m_bUserHuXiCount[i]+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[i][j]);
				}
			}

			//扑克信息
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(pStatusPlay->cbCardIndex,cbCardData,CountArray(cbCardData));

			//设置界面
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetLeftCardCount(pStatusPlay->bLeftCardCount);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount);

			//出牌界面
			if (m_wOutCardUser!=INVALID_CHAIR) 
			{
				WORD wViewChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wViewChairID,m_cbOutCardData);
			}

			//游戏界面
			for (WORD i=0;i<GAME_PLAYER;i++) 
			{
				//变量定义
				WORD wViewChairID=SwitchViewChairID(i);

				//设置胡息
				m_GameClientView.SetUserHuXiCount(wViewChairID,m_bUserHuXiCount[i]);

				//用户扑克
				if (i!=GetMeChairID())
				{
					WORD wIndex=(wViewChairID==0)?0:1;
					m_GameClientView.m_UserCard[wIndex].SetCardData(pStatusPlay->bUserCardCount[i]);
				}

				//组合界面
				for (BYTE j=0;j<m_bWeaveItemCount[i];j++) 
				{
					m_GameClientView.m_WeaveCard[wViewChairID][j].SetCardData(m_WeaveItemArray[i][j]);
				}
			}

			//动作界面
			if ((pStatusPlay->bUserAction!=ACK_NULL)&&(pStatusPlay->bResponse==FALSE)&&(IsLookonMode()==false))
			{
				ASSERT(m_wCurrentUser==INVALID_CHAIR);
				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				m_GameClientView.m_ControlWnd.SetControlInfo(m_cbOutCardData,pStatusPlay->bUserAction);
			}

			//控件设置
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			m_GameClientView.m_HandCardControl.SetPositively(IsLookonMode()==false);

			//设置时间
			if (m_wCurrentUser!=INVALID_CHAIR) 
			{
				if (m_bOutCard==true) 
				{
					SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
					if ((IsLookonMode()==false)&&(GetMeChairID()==m_wCurrentUser)) m_GameClientView.SetStatusFlag(true,false);
				}
				else if ((GetMeChairID()==m_wCurrentUser)&&(IsLookonMode()==false)) SendData(SUB_C_CONTINUE_CARD);
			}

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//中止动作
bool CGameClientDlg::HangUserAction()
{
	//中止判断
	if (m_wActionCount==0) return true;

	//删除定时器
	KillTimer(IDI_USER_ACTION);

	//设置变量
	m_wActionCount=0;
	ZeroMemory(m_UserActionArray,sizeof(m_UserActionArray));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionPaoCard(WORD wChairID, BYTE cbCardData, BYTE cbRemoveCount)
{
	//明跑判断
	bool bExistWeave=false;
	for (BYTE cbIndex=0;cbIndex<m_bWeaveItemCount[wChairID];cbIndex++)
	{
		//变量定义
		BYTE cbWeaveKind=m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind;
		BYTE cbWeaveCard=m_WeaveItemArray[wChairID][cbIndex].cbCardList[0];

		//明跑判断
		if ((cbCardData==cbWeaveCard)&&((cbWeaveKind==ACK_PENG)||(cbWeaveKind==ACK_WEI))) 
		{
			bExistWeave=true;
			break;
		}
	}

	//扑克数目
	m_bUserCardCount[wChairID]-=cbRemoveCount;

	//扑克数据
	if ((bExistWeave==false)&&(wChairID==GetMeChairID()))
	{
		BYTE cbCardList[3]={cbCardData,cbCardData,cbCardData};
		m_GameLogic.RemoveCard(m_cbCardIndex,cbCardList,cbRemoveCount);
	}

	//设置组合
	if (bExistWeave==false) m_bWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbIndex].cbCardCount=4;
	m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_PAO;
	m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[3]=cbCardData;

	//更新胡息
	UpdateUserHuXiCount(wChairID);

	//删除定时器
	KillGameTimer(IDI_ASSISTANT_TIME);

	//提示动作
	tagActionTip ActionTip;
	ActionTip.wChairID=wChairID;
	ActionTip.cbActionFlags=ACK_PAO;
	ExecuteAction(ACTION_TIP,&ActionTip,sizeof(ActionTip));

	//提牌动作
	tagActionPaoCard ActionPaoCard;
	ActionPaoCard.cbIndex=cbIndex;
	ActionPaoCard.wChairID=wChairID;
	ActionPaoCard.cbRemoveCount=cbRemoveCount;
	ActionPaoCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	ExecuteAction(ACTION_PAO_CARD,&ActionPaoCard,sizeof(ActionPaoCard));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionWeiCard(WORD wChairID, BYTE cbCardData)
{
	//扑克数目
	m_bUserCardCount[wChairID]-=2;

	//扑克数据
	if (wChairID==GetMeChairID())
	{
		BYTE cbCardList[]={cbCardData,cbCardData};
		m_GameLogic.RemoveCard(m_cbCardIndex,cbCardList,CountArray(cbCardList));
	}

	//设置组合
	BYTE cbIndex=m_bWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbIndex].cbCardCount=3;
	m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_WEI;
	m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbCardData;

	//更新胡息
	UpdateUserHuXiCount(wChairID);

	//删除定时器
	KillGameTimer(IDI_ASSISTANT_TIME);

	//提示动作
	tagActionTip ActionTip;
	ActionTip.wChairID=wChairID;
	ActionTip.cbActionFlags=ACK_WEI;
	ExecuteAction(ACTION_TIP,&ActionTip,sizeof(ActionTip));

	//提牌动作
	tagActionWeiCard ActionWeiCard;
	ActionWeiCard.cbIndex=cbIndex;
	ActionWeiCard.wChairID=wChairID;
	ActionWeiCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	ExecuteAction(ACTION_WEI_CARD,&ActionWeiCard,sizeof(ActionWeiCard));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionPengCard(WORD wChairID, BYTE cbCardData)
{
	//扑克数目
	m_bUserCardCount[wChairID]-=2;

	//扑克数据
	if (wChairID==GetMeChairID())
	{
		BYTE cbCardList[2]={cbCardData,cbCardData};
		m_GameLogic.RemoveCard(m_cbCardIndex,cbCardList,CountArray(cbCardList));
	}

	//删除定时器
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_ASSISTANT_TIME);

	//设置界面
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//设置组合
	BYTE cbIndex=m_bWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbIndex].cbCardCount=3;
	m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_PENG;
	m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbCardData;

	//更新胡息
	UpdateUserHuXiCount(wChairID);

	//提示动作
	tagActionTip ActionTip;
	ActionTip.wChairID=wChairID;
	ActionTip.cbActionFlags=ACK_PENG;
	ExecuteAction(ACTION_TIP,&ActionTip,sizeof(ActionTip));

	//提牌动作
	tagActionPengCard ActionPengCard;
	ActionPengCard.cbIndex=cbIndex;
	ActionPengCard.wChairID=wChairID;
	ActionPengCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	ExecuteAction(ACTION_PENG_CARD,&ActionPengCard,sizeof(ActionPengCard));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionChiCard(WORD wChairID, BYTE cbCardData, BYTE cbResultCount, BYTE cbResultData[][3])
{
	//变量定义
	BYTE cbFirstIndex=m_bWeaveItemCount[wChairID];

	//扑克数目
	m_bUserCardCount[wChairID]-=cbResultCount*3-1;

	//设置组合
	for (BYTE k=0;k<cbResultCount;k++)
	{
		BYTE cbIndex=m_bWeaveItemCount[wChairID]++;
		m_WeaveItemArray[wChairID][cbIndex].cbCardCount=3;
		m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_CHI;
		m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
		m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbResultData[k][0];
		m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbResultData[k][1];
		m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbResultData[k][2];
	}

	//删除定时器
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_ASSISTANT_TIME);

	//设置界面
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//更新胡息
	UpdateUserHuXiCount(wChairID);

	//删除扑克
	if (wChairID==GetMeChairID())
	{
		BYTE cbDebarCard=cbCardData;
		for (BYTE k=0;k<cbResultCount;k++)
		{
			for (BYTE l=0;l<3;l++)
			{
				BYTE cbRemoveCard=cbResultData[k][l];
				if (cbRemoveCard==cbDebarCard) cbDebarCard=0;
				else m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbRemoveCard)]--;
			}
		}
	}

	//提示动作
	tagActionTip ActionChi;
	ActionChi.wChairID=wChairID;
	ActionChi.cbActionFlags=ACK_CHI;
	ExecuteAction(ACTION_TIP,&ActionChi,sizeof(ActionChi));

	//吃牌动作
	tagActionChiCard ActionChiCard;
	ActionChiCard.wChairID=wChairID;
	ActionChiCard.cbIndex=cbFirstIndex;
	ActionChiCard.cbActionCard=cbCardData;
	ActionChiCard.cbResultCount=cbResultCount;
	ActionChiCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	CopyMemory(ActionChiCard.cbCardData,cbResultData,sizeof(BYTE)*3*cbResultCount);

	//执行动作
	ExecuteAction(ACTION_CHI_CARD,&ActionChiCard,sizeof(ActionChiCard));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionTiCard(WORD wChairID, BYTE cbCardData, BYTE cbRemoveCount)
{
	//明跑判断
	bool bExistWeave=false;
	for (BYTE cbIndex=0;cbIndex<m_bWeaveItemCount[wChairID];cbIndex++)
	{
		//变量定义
		BYTE cbWeaveKind=m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind;
		BYTE cbWeaveCard=m_WeaveItemArray[wChairID][cbIndex].cbCardList[0];

		//明跑判断
		if ((cbCardData==cbWeaveCard)&&((cbWeaveKind==ACK_PENG)||(cbWeaveKind==ACK_WEI))) 
		{
			bExistWeave=true;
			break;
		}
	}

	//扑克数目
	m_bUserCardCount[wChairID]-=cbRemoveCount;

	//扑克数据
	if (wChairID==GetMeChairID())
	{
		BYTE cbCardList[4]={cbCardData,cbCardData,cbCardData,cbCardData};
		m_GameLogic.RemoveCard(m_cbCardIndex,cbCardList,cbRemoveCount);
	}

	//设置组合
	if (bExistWeave==false) m_bWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbIndex].cbCardCount=4;
	m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind=ACK_TI;
	m_WeaveItemArray[wChairID][cbIndex].cbCenterCard=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[0]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[1]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[2]=cbCardData;
	m_WeaveItemArray[wChairID][cbIndex].cbCardList[3]=cbCardData;

	//更新胡息
	UpdateUserHuXiCount(wChairID);

	//删除定时器
	KillGameTimer(IDI_ASSISTANT_TIME);

	//提示动作
	tagActionTip ActionTip;
	ActionTip.wChairID=wChairID;
	ActionTip.cbActionFlags=ACK_TI;
	ExecuteAction(ACTION_TIP,&ActionTip,sizeof(ActionTip));

	//提牌动作
	tagActionTiCard ActionTiCard;
	ActionTiCard.cbIndex=cbIndex;
	ActionTiCard.wChairID=wChairID;
	ActionTiCard.cbRemoveCount=cbRemoveCount;
	ActionTiCard.bHuXiCount=m_bUserHuXiCount[wChairID];
	ExecuteAction(ACTION_TI_CARD,&ActionTiCard,sizeof(ActionTiCard));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionOutCardNotify(WORD wCurrentUser, BYTE bOutCard)
{
	//删除定时器
	KillGameTimer(IDI_ASSISTANT_TIME);

	//出牌提示
	tagActionOutCardNotify ActionOutCardNotify;
	ActionOutCardNotify.bOutCard=bOutCard;
	ActionOutCardNotify.wCurrentUser=wCurrentUser;
	ExecuteAction(ACTION_OUT_CARD_NOTIFY,&ActionOutCardNotify,sizeof(ActionOutCardNotify));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionOperateNotify(BYTE cbOperateCode, BYTE cbOperateCard, WORD wResumeUser)
{
	//操作提示
	tagActionOperateNotify ActionOperateNotify;
	ActionOperateNotify.wResumeUser=wResumeUser;
	ActionOperateNotify.cbOperateCode=cbOperateCode;
	ActionOperateNotify.cbCurrentCard=cbOperateCard;
	ExecuteAction(ACTION_OPERATE_NOTIFY,&ActionOperateNotify,sizeof(ActionOperateNotify));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionOutCard(WORD wOutCardUser, BYTE cbOutCardData)
{
	//删除扑克
	if ((GetMeChairID()!=wOutCardUser)||(IsLookonMode()==true))
	{
		m_bUserCardCount[wOutCardUser]--;
		if (GetMeChairID()==wOutCardUser) m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData);
	}

	//出牌动作
	tagActionOutCard ActionOutCard;
	ActionOutCard.wOutCardUser=wOutCardUser;
	ActionOutCard.cbOutCardData=cbOutCardData;
	ExecuteAction(ACTION_OUT_CARD,&ActionOutCard,sizeof(ActionOutCard));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteActionSendCard(BYTE cbCardData, WORD wAttachUser)
{
	//删除定时器
	KillGameTimer(IDI_ASSISTANT_TIME);

	//发牌动作
	tagActionSendCard ActionSendCard;
	ActionSendCard.wAttachUser=wAttachUser;
	ActionSendCard.cbSendCardData=cbCardData;
	ExecuteAction(ACTION_SEND_CARD,&ActionSendCard,sizeof(ActionSendCard));

	return true;
}

//执行动作
bool CGameClientDlg::ExecuteAction(BYTE cbActionKind, void * pActionInfo, WORD wDataSize)
{
	//效验状态
	ASSERT(m_wActionCount<CountArray(m_UserActionArray));
	ASSERT(wDataSize<=sizeof(m_UserActionArray[m_wActionCount].cbActionBuffer));
	if (m_wActionCount>=CountArray(m_UserActionArray)) return false;
	if (wDataSize>sizeof(m_UserActionArray[m_wActionCount].cbActionBuffer)) return false;

	//设置提示
	m_UserActionArray[m_wActionCount].cbActionKind=cbActionKind;
	CopyMemory(m_UserActionArray[m_wActionCount++].cbActionBuffer,pActionInfo,wDataSize);

	//设置定时器
	if (m_wActionCount==1) SetTimer(IDI_USER_ACTION,TIME_USER_ACTION,NULL);

	return true;
}

//更新胡息
bool CGameClientDlg::UpdateUserHuXiCount(WORD wChairID)
{
	//胡息计算
	BYTE bUserHuXiCount=0;
	for (BYTE i=0;i<m_bWeaveItemCount[wChairID];i++)
	{
		bUserHuXiCount+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][i]);
	}
	
	//设置胡息
	m_bUserHuXiCount[wChairID]=bUserHuXiCount;

	//自己胡息
	if (wChairID==GetMeChairID())
	{
	}

	return true;
}

//删除扑克
bool CGameClientDlg::RemoveControlCard(WORD wChairID, BYTE cbRemoveCard[], WORD wCardCount)
{
	if (wChairID==GetMeChairID())
	{
		//获取扑克
		BYTE cbHandCardList[MAX_COUNT];
		WORD wHandCardCount=m_GameClientView.m_HandCardControl.GetCardData(cbHandCardList,CountArray(cbHandCardList));

		//删除扑克
		m_GameLogic.RemoveCard(cbHandCardList,(BYTE)(wHandCardCount),cbRemoveCard,(BYTE)wCardCount);

		//设置扑克
		WORD wResultCount=wHandCardCount-wCardCount;
		m_GameClientView.m_HandCardControl.SetCardData(cbHandCardList,wResultCount);
	}
	else
	{
		//设置数目
		WORD wUserIndex=(SwitchViewChairID(wChairID)==2)?1:0;
		WORD wHandCardCount=m_GameClientView.m_UserCard[wUserIndex].GetCardCount();

		//设置扑克
		WORD wResultCount=wHandCardCount-wCardCount;
		m_GameClientView.m_UserCard[wUserIndex].SetCardData(wResultCount);
	}

	return true;
}

//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//设置数据
	m_wResumeUser=INVALID_CHAIR;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	for (WORD i=0;i<GAME_PLAYER;i++) m_bUserCardCount[i]=(m_wBankerUser==i)?21:20;

	//设置数据
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//设置扑克
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//设置界面
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.m_HandCardControl.SetDisplayItem(bPlayerMode);

	//状态设置
	m_GameClientView.SetLeftCardCount(19);
	m_GameClientView.SetUserHuXiCount(INVALID_CHAIR,0);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//旁观界面
	if (bPlayerMode==false)
	{
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	}

	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);

		//用户扑克
		if (wViewChairID!=1)
		{
			WORD wIndex=(wViewChairID==0)?0:1;
			m_GameClientView.m_UserCard[wIndex].SetCardData((i==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1));
		}
		else
		{
			BYTE cbCardCount=(i==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
			m_GameClientView.m_HandCardControl.SetCardData(pGameStart->cbCardData,cbCardCount);
		}

		//旁观界面
		if (bPlayerMode==false)
		{
			WORD wMeChair=GetMeChairID();
			m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(m_WeaveItemArray[wMeChair][0]);
			m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(m_WeaveItemArray[wMeChair][1]);
			m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(m_WeaveItemArray[wMeChair][2]);
			m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(m_WeaveItemArray[wMeChair][3]);
			m_GameClientView.m_WeaveCard[wViewChairID][4].SetCardData(m_WeaveItemArray[wMeChair][4]);
			m_GameClientView.m_WeaveCard[wViewChairID][5].SetCardData(m_WeaveItemArray[wMeChair][5]);
			m_GameClientView.m_WeaveCard[wViewChairID][6].SetCardData(m_WeaveItemArray[wMeChair][6]);
		}
	}

	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	//出牌提示
	if ((m_wCurrentUser==GetMeChairID())&&(IsLookonMode()==false)) 
	{
		m_bOutCard=true;
		m_GameClientView.SetStatusFlag(true,false);
	}

	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	//环境处理
	if (bPlayerMode==true) ActiveGameFrame();
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//用户提牌
bool CGameClientDlg::OnSubUserTiCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserTiCard));
	if (wDataSize!=sizeof(CMD_S_UserTiCard)) return false;

	//变量定义
	CMD_S_UserTiCard * pUserTiCard=(CMD_S_UserTiCard *)pBuffer;

	//执行动作
	ExecuteActionTiCard(pUserTiCard->wActionUser,pUserTiCard->cbActionCard,pUserTiCard->cbRemoveCount);

	return true;
}

//用户跑牌
bool CGameClientDlg::OnSubUserPaoCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserPaoCard));
	if (wDataSize!=sizeof(CMD_S_UserPaoCard)) return false;

	//变量定义
	CMD_S_UserPaoCard * pUserPaoCard=(CMD_S_UserPaoCard *)pBuffer;

	//执行动作
	ExecuteActionPaoCard(pUserPaoCard->wActionUser,pUserPaoCard->cbActionCard,pUserPaoCard->cbRemoveCount);

	return true;
}

//用户偎牌
bool CGameClientDlg::OnSubUserWeiCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserWeiCard));
	if (wDataSize!=sizeof(CMD_S_UserWeiCard)) return false;

	//变量定义
	CMD_S_UserWeiCard * pUserWeiCard=(CMD_S_UserWeiCard *)pBuffer;

	//执行动作
	ExecuteActionWeiCard(pUserWeiCard->wActionUser,pUserWeiCard->cbActionCard);

	return true;
}

//用户碰牌
bool CGameClientDlg::OnSubUserPengCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserPengCard));
	if (wDataSize!=sizeof(CMD_S_UserPengCard)) return false;

	//变量定义
	CMD_S_UserPengCard * pUserPengCard=(CMD_S_UserPengCard *)pBuffer;

	//执行动作
	ExecuteActionPengCard(pUserPengCard->wActionUser,pUserPengCard->cbActionCard);

	return true;
}

//用户吃牌
bool CGameClientDlg::OnSubUserChiCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_UserChiCard));
	if (wDataSize!=sizeof(CMD_S_UserChiCard)) return false;

	//变量定义
	CMD_S_UserChiCard * pUserChiCard=(CMD_S_UserChiCard *)pBuffer;

	//执行动作
	ExecuteActionChiCard(pUserChiCard->wActionUser,pUserChiCard->cbActionCard,pUserChiCard->cbResultCount,pUserChiCard->cbCardData);

	return true;
}

//操作提示
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//执行动作
	ExecuteActionOperateNotify(pOperateNotify->cbOperateCode,pOperateNotify->cbActionCard,pOperateNotify->wResumeUser);

	return true;
}

//出牌提示
bool CGameClientDlg::OnSubOutCardNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OutCardNotify));
	if (wDataSize!=sizeof(CMD_S_OutCardNotify)) return false;

	//变量定义
	CMD_S_OutCardNotify * pOutCardNotify=(CMD_S_OutCardNotify *)pBuffer;

	//执行动作
	ExecuteActionOutCardNotify(pOutCardNotify->wCurrentUser,pOutCardNotify->bOutCard);

	return true;
}

//用户出牌
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//变量定义
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//执行动作
	ExecuteActionOutCard(pOutCard->wOutCardUser,pOutCard->cbOutCardData);

	return true;
}

//发牌命令
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//执行动作
	ExecuteActionSendCard(pSendCard->cbCardData,pSendCard->wAttachUser);

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

	//游戏定时器
	KillTimer(IDI_USER_ACTION);
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_ASSISTANT_TIME);

	//中止动作
	HangUserAction();

	//设置变量
	m_bOutCard=false;
	m_wBankerUser=INVALID_CHAIR;

	//设置界面
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//设置扑克
	BYTE cbPostion=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//设置界面
		WORD wViewChairID=SwitchViewChairID(i);
		if (wViewChairID==0) m_GameClientView.m_UserCard[0].SetCardData(&pGameEnd->bCardData[cbPostion],pGameEnd->bCardCount[i]);
		else if (wViewChairID==2) m_GameClientView.m_UserCard[1].SetCardData(&pGameEnd->bCardData[cbPostion],pGameEnd->bCardCount[i]);

		//设置索引
		cbPostion+=pGameEnd->bCardCount[i];
	}

	//结束显示
	switch (pGameEnd->cbReason)
	{
	case GER_DISMISS:			//游戏解散
	case GER_USER_LEFT:			//用户强退
		{
			//变量定义
			tagScoreViewInfo ScoreViewInfo;
			ZeroMemory(&ScoreViewInfo,sizeof(ScoreViewInfo));

			//用户信息
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				const tagUserData * pUserData=GetUserData(i);

				//设置信息
				ScoreViewInfo.lGameScore[i]=pGameEnd->lGameScore[i];
				lstrcpyn(ScoreViewInfo.szUserName[i],pUserData->szName,CountArray(ScoreViewInfo.szUserName[i]));
			}

			//结束信息
			ScoreViewInfo.cbHuCard=0;
			ScoreViewInfo.lGameTax=pGameEnd->lGameTax;
			ScoreViewInfo.wWinUser=pGameEnd->wWinUser;
			ScoreViewInfo.wProvideUser=pGameEnd->wProvideUser;

			//显示成绩
			m_GameClientView.m_ScoreView.SetScoreViewInfo(ScoreViewInfo,true);

			break;
		}
	case GER_NORMAL:	//常规结束
		{
			if (pGameEnd->wWinUser!=INVALID_CHAIR)
			{
				//变量定义
				tagScoreViewInfo ScoreViewInfo;
				ZeroMemory(&ScoreViewInfo,sizeof(ScoreViewInfo));

				//胡牌信息
				BYTE cbPostion=0;
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//胡牌分析
					if (i==pGameEnd->wWinUser)
					{
						//转换索引
						BYTE cbCardIndex[MAX_INDEX];
						m_GameLogic.SwitchToCardIndex(&pGameEnd->bCardData[cbPostion],pGameEnd->bCardCount[i],cbCardIndex);

						//胡牌信息
						m_GameLogic.GetHuCardInfo(cbCardIndex,pGameEnd->cbHuCard,m_bUserHuXiCount[i],ScoreViewInfo.HuCardInfo);

						//设置组合
						for (BYTE j=0;j<m_bWeaveItemCount[i];j++)
						{
							BYTE cbIndex=ScoreViewInfo.HuCardInfo.cbWeaveCount++;
							ScoreViewInfo.HuCardInfo.WeaveItemArray[cbIndex]=m_WeaveItemArray[i][j];
							ScoreViewInfo.HuCardInfo.cbHuXiCount+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[i][j]);
						}

						break;
					}

					//设置索引
					cbPostion+=pGameEnd->bCardCount[i];
				}

				//用户信息
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//变量定义
					const tagUserData * pUserData=GetUserData(i);

					//设置信息
					ScoreViewInfo.lGameScore[i]=pGameEnd->lGameScore[i];
					lstrcpyn(ScoreViewInfo.szUserName[i],pUserData->szName,CountArray(ScoreViewInfo.szUserName[i]));
				}

				//结束信息
				ScoreViewInfo.lGameTax=pGameEnd->lGameTax;
				ScoreViewInfo.cbHuCard=pGameEnd->cbHuCard;
				ScoreViewInfo.wWinUser=pGameEnd->wWinUser;
				ScoreViewInfo.wProvideUser=pGameEnd->wProvideUser;

				//显示成绩
				m_GameClientView.m_ScoreView.SetScoreViewInfo(ScoreViewInfo,true);
			}
			else m_GameClientView.SetHuangZhuang(true);

			break;
		}
	}

	//扑克控件
	m_GameClientView.m_HandCardControl.SetPositively(false);
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl.SetDisplayItem(true);

	//播放声音
	WORD wMeChairID=GetMeChairID();
	LONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	return true;
}

//时间消息
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	//动作定时器
	if (nIDEvent==IDI_USER_ACTION)
	{
		//删除时间
		if (m_wActionCount==0)
		{
			KillTimer(IDI_USER_ACTION);
			return;
		}

		//动作处理
		switch (m_UserActionArray[0].cbActionKind)
		{
		case ACTION_TIP:				//提示动作
			{
				//变量定义
				tagActionTip * pActionTip=(tagActionTip *)(m_UserActionArray[0].cbActionBuffer);

				//设置界面
				WORD wViewChairID=SwitchViewChairID(pActionTip->wChairID);
				m_GameClientView.SetUserAction(wViewChairID,pActionTip->cbActionFlags);

				break;
			}
		case ACTION_TI_CARD:			//提牌动作
			{
				//变量定义
				tagActionTiCard * pActionTiCard=(tagActionTiCard *)(m_UserActionArray[0].cbActionBuffer);

				//辅助变量
				BYTE cbIndex=pActionTiCard->cbIndex;
				WORD wChairID=pActionTiCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//设置变量
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//组合界面
				m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetCardData(m_WeaveItemArray[wChairID][cbIndex]);
				RemoveControlCard(wChairID,m_WeaveItemArray[wChairID][cbIndex].cbCardList,pActionTiCard->cbRemoveCount);

				//设置界面
				m_GameClientView.SetStatusFlag(false,false);
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionTiCard->bHuXiCount);

				break;
			}
		case ACTION_PAO_CARD:			//跑牌动作
			{
				//变量定义
				tagActionPaoCard * pActionPaoCard=(tagActionPaoCard *)(m_UserActionArray[0].cbActionBuffer);

				//辅助变量
				BYTE cbIndex=pActionPaoCard->cbIndex;
				WORD wChairID=pActionPaoCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//设置变量
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//组合界面
				m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetCardData(m_WeaveItemArray[wChairID][cbIndex]);
				RemoveControlCard(wChairID,m_WeaveItemArray[wChairID][cbIndex].cbCardList,pActionPaoCard->cbRemoveCount);

				//设置界面
				m_GameClientView.SetStatusFlag(false,false);
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionPaoCard->bHuXiCount);

				break;
			}
		case ACTION_WEI_CARD:			//偎牌动作
			{
				//变量定义
				tagActionWeiCard * pActionWeiCard=(tagActionWeiCard *)(m_UserActionArray[0].cbActionBuffer);

				//辅助变量
				BYTE cbIndex=pActionWeiCard->cbIndex;
				WORD wChairID=pActionWeiCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//组合界面
				if(wChairID==GetMeChairID())
					m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetDisplayStyle(true);
				else
					m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetDisplayStyle(false);
				RemoveControlCard(wChairID,m_WeaveItemArray[wChairID][cbIndex].cbCardList,2);
				m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetCardData(m_WeaveItemArray[wChairID][cbIndex]);

				//增加胡息
				BYTE cbUserHuXi=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][cbIndex]);
				m_GameClientView.SetUserHuXiCount(wViewChairID,m_GameClientView.m_bUserHuXiCount[wViewChairID]+cbUserHuXi);

				//出牌信息
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//界面设置
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionWeiCard->bHuXiCount);

				break;
			}
		case ACTION_PENG_CARD:			//碰牌动作
			{
				//变量定义
				tagActionPengCard * pActionPengCard=(tagActionPengCard *)(m_UserActionArray[0].cbActionBuffer);

				//辅助变量
				BYTE cbIndex=pActionPengCard->cbIndex;
				WORD wChairID=pActionPengCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//出牌信息
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//组合界面
				RemoveControlCard(wChairID,m_WeaveItemArray[wChairID][cbIndex].cbCardList,2);
				m_GameClientView.m_WeaveCard[wViewChairID][cbIndex].SetCardData(m_WeaveItemArray[wChairID][cbIndex]);

				//界面设置
				m_GameClientView.SetStatusFlag(false,false);
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionPengCard->bHuXiCount);

				break;
			}
		case ACTION_CHI_CARD:			//用户吃牌
			{
				//变量定义
				tagActionChiCard * pActionChiCard=(tagActionChiCard *)(m_UserActionArray[0].cbActionBuffer);

				//辅助变量
				BYTE cbIndex=pActionChiCard->cbIndex;
				WORD wChairID=pActionChiCard->wChairID;
				WORD wViewChairID=SwitchViewChairID(wChairID);

				//出牌信息
				m_cbOutCardData=0;
				m_wOutCardUser=INVALID_CHAIR;

				//组合界面
				BYTE cbUserHuXi=0;
				for (BYTE i=0;i<pActionChiCard->cbResultCount;i++)
				{
					cbUserHuXi+=m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][cbIndex+i]);
					m_GameClientView.m_WeaveCard[wViewChairID][cbIndex+i].SetCardData(m_WeaveItemArray[wChairID][cbIndex+i]);
				}

				//删除准备
				BYTE cbRemoveCount=0;
				BYTE cbRemoveCard[MAX_COUNT];
				BYTE cbDebarCard=pActionChiCard->cbActionCard;
				for (BYTE j=0;j<pActionChiCard->cbResultCount;j++)
				{
					for (BYTE l=0;l<3;l++)
					{
						BYTE cbCurrentCard=pActionChiCard->cbCardData[j][l];
						if (cbCurrentCard==cbDebarCard) cbDebarCard=0;
						else cbRemoveCard[cbRemoveCount++]=cbCurrentCard;
					}
				}

				//删除扑克
				ASSERT(cbRemoveCount==(pActionChiCard->cbResultCount*3-1));
				RemoveControlCard(wChairID,cbRemoveCard,pActionChiCard->cbResultCount*3-1);

				//界面设置
				m_GameClientView.SetStatusFlag(false,false);
				m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
				m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);
				m_GameClientView.SetUserHuXiCount(wViewChairID,pActionChiCard->bHuXiCount);

				break;
			}
		case ACTION_OUT_CARD:			//用户出牌
			{
				//变量定义
				tagActionOutCard * pActionOutCard=(tagActionOutCard *)(m_UserActionArray[0].cbActionBuffer);

				//设置变量
				m_bOutCard=false;
				m_wCurrentUser=INVALID_CHAIR;

				//扑克收集
				if (m_wOutCardUser!=INVALID_CHAIR)
				{
					WORD wViewChairID=SwitchViewChairID(m_wOutCardUser);
					m_GameClientView.m_DiscardCard[wViewChairID].AddCardItem(m_cbOutCardData);
				}

				//出牌信息
				m_wOutCardUser=pActionOutCard->wOutCardUser;
				m_cbOutCardData=pActionOutCard->cbOutCardData;

				//界面设置
				if ((GetMeChairID()!=m_wOutCardUser)||(IsLookonMode()==true))
				{
					//设置扑克
					BYTE cbRemoveCard[]={m_cbOutCardData};
					RemoveControlCard(pActionOutCard->wOutCardUser,cbRemoveCard,CountArray(cbRemoveCard));

					//设置界面
					KillGameTimer(IDI_OUT_CARD);
					m_GameClientView.SetUserAction(INVALID_CHAIR,0);
					m_GameClientView.SetOutCardInfo(SwitchViewChairID(m_wOutCardUser),m_cbOutCardData);

					//播放声音
					PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
				}

				break;
			}
		case ACTION_SEND_CARD:			//发牌处理
			{
				//变量定义
				tagActionSendCard * pActionSendCard=(tagActionSendCard *)(m_UserActionArray[0].cbActionBuffer);

				//扑克收集
				if (m_wOutCardUser!=INVALID_CHAIR)
				{
					WORD wViewChairID=SwitchViewChairID(m_wOutCardUser);
					m_GameClientView.m_DiscardCard[wViewChairID].AddCardItem(m_cbOutCardData);
				}

				//出牌信息
				m_wOutCardUser=pActionSendCard->wAttachUser;
				m_cbOutCardData=pActionSendCard->cbSendCardData;

				//发牌界面
				WORD wViewChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wViewChairID,m_cbOutCardData);
				m_GameClientView.SetLeftCardCount(m_GameClientView.m_bLeftCardCount-1);

				//播放声音
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

				break;
			}
		case ACTION_OPERATE_NOTIFY:		//操作提示
			{
				//变量定义
				tagActionOperateNotify * pActionOperateNotify=(tagActionOperateNotify *)(m_UserActionArray[0].cbActionBuffer);

				//设置变量
				m_bOutCard=false;
				m_wCurrentUser=INVALID_CHAIR;
				m_wResumeUser=pActionOperateNotify->wResumeUser;

				//用户界面
				if ((IsLookonMode()==false)&&(pActionOperateNotify->cbOperateCode!=ACK_NULL))
				{
					//获取变量
					BYTE cbOperateCode=pActionOperateNotify->cbOperateCode;
					BYTE cbCurrentCard=pActionOperateNotify->cbCurrentCard;

					//设置界面
					ActiveGameFrame();
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					m_GameClientView.m_ControlWnd.SetControlInfo(cbCurrentCard,cbOperateCode);
				}
				else SetGameTimer(m_wResumeUser,IDI_ASSISTANT_TIME,TIME_ASSISTANT_TIME);

				break;
			}
		case ACTION_OUT_CARD_NOTIFY:	//出牌提示
			{
				//变量定义
				tagActionOutCardNotify * pActionOutCardNotify=(tagActionOutCardNotify *)(m_UserActionArray[0].cbActionBuffer);

				//设置变量
				m_bOutCard=false;
				m_wCurrentUser=pActionOutCardNotify->wCurrentUser;

				//设置界面
				if ((pActionOutCardNotify->bOutCard==TRUE)&&(m_wCurrentUser==GetMeChairID())&&(IsLookonMode()==false))
				{
					m_bOutCard=true;
					ActiveGameFrame();
					m_GameClientView.SetStatusFlag(true,false);
				}

				//设置时间
				if (pActionOutCardNotify->bOutCard==FALSE)
				{
					if ((m_wCurrentUser==GetMeChairID())&&(IsLookonMode()==false)) SendData(SUB_C_CONTINUE_CARD);
				}
				else SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

				break;
			}
		}

		//删除动作
		m_wActionCount--;
		if (m_wActionCount>0)
		{
			LONG lMoveSize=m_wActionCount*sizeof(m_UserActionArray[0]);
			MoveMemory(&m_UserActionArray[0],&m_UserActionArray[1],lMoveSize);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//辅助变量
	m_bOutCard=false;
	m_wCurrentUser=INVALID_CHAIR;

	//游戏变量
	ZeroMemory(m_bUserCardCount,sizeof(m_bUserCardCount));
	ZeroMemory(m_bUserHuXiCount,sizeof(m_bUserHuXiCount));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_bWeaveItemCount,sizeof(m_bWeaveItemCount));

	//动作变量
	m_wActionCount=0;
	ZeroMemory(m_UserActionArray,sizeof(m_UserActionArray));

	//扑克变量
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//隐藏控件
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	m_GameClientView.m_ScoreView.ShowWindow(FALSE);
	m_GameClientView.m_ChooseWnd.ShowWindow(FALSE);
	m_GameClientView.m_ControlWnd.ShowWindow(FALSE);

	//设置界面
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetLeftCardCount(0xFF);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.SetUserHuXiCount(INVALID_CHAIR,0xFF);
	m_GameClientView.SetUserAction(INVALID_CHAIR,ACK_NULL);

	//设置扑克
	m_GameClientView.m_UserCard[0].SetCardData(0);
	m_GameClientView.m_UserCard[1].SetCardData(0);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0);

	//设置扑克
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		for (int j=0;j<7;j++) m_GameClientView.m_WeaveCard[i][j].SetCardData(m_WeaveItemArray[i][j]);
	}

	//更新界面
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.UpdateGameView(NULL);

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//出牌操作
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//出牌判断
	if ((IsLookonMode()==true)||(m_bOutCard==false)||(m_wCurrentUser!=GetMeChairID())) return 0;

	//变量定义
	BYTE cbOutCard=(BYTE)wParam;
	WORD wMeChairID=GetMeChairID();

	//出牌效验
	BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOutCard);
	if (m_cbCardIndex[cbCardIndex]==3) return 0;

	//设置变量
	m_bOutCard=false;
	m_wCurrentUser=INVALID_CHAIR;

	//删除扑克
	m_bUserCardCount[wMeChairID]--;
	m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCard);

	//出牌界面
	BYTE cbRemoveCard[]={cbOutCard};
	RemoveControlCard(wMeChairID,cbRemoveCard,CountArray(cbRemoveCard));
	m_GameClientView.SetOutCardInfo(SwitchViewChairID(wMeChairID),cbOutCard);

	//设置界面
	KillGameTimer(IDI_OUT_CARD);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCard;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	return 0;
}

//选牌操作
LRESULT CGameClientDlg::OnChooseCard(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbChiKind=(BYTE)lParam;

	//环境设置
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	KillGameTimer((GetMeChairID()==m_wCurrentUser)?IDI_OUT_CARD:IDI_OPERATE_CARD);

	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbChiKind=cbChiKind;
	OperateCard.cbOperateCode=ACK_CHI;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	//设置定时器
	SetGameTimer(m_wResumeUser,IDI_ASSISTANT_TIME,TIME_ASSISTANT_TIME);

	return 0;
}

//扑克操作
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbOperateCode=(BYTE)(wParam);

	//状态判断
	if ((m_wCurrentUser==GetMeChairID())&&(cbOperateCode==ACK_NULL))
	{
		//环境设置
		m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
		m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

		return 0;
	};

	//吃牌操作
	if (cbOperateCode==ACK_CHI)
	{
		//获取数据
		tagChiCardInfo ChiCardInfo[6];
		BYTE cbWeaveCount=m_GameLogic.GetActionChiCard(m_cbCardIndex,m_cbOutCardData,ChiCardInfo);

		//设置控制
		m_GameClientView.m_ChooseWnd.SetChooseWeave(ChiCardInfo,cbWeaveCount);

		return 0;
	}

	//环境设置
	m_GameClientView.m_ChooseWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	if (cbOperateCode!=ACK_NULL) m_GameClientView.SetStatusFlag(false,true);
	KillGameTimer((GetMeChairID()==m_wCurrentUser)?IDI_OUT_CARD:IDI_OPERATE_CARD);

	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbChiKind=CK_NULL;
	OperateCard.cbOperateCode=cbOperateCode;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	//设置定时器
	SetGameTimer(m_wResumeUser,IDI_ASSISTANT_TIME,TIME_ASSISTANT_TIME);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

