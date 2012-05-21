#include "StdAfx.h"
#include "Resource.h"
#include "GameClientDlg.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//常量定义
#define MAX_COUNT					3									//最大牌数

//时间定义
#define IDI_WAITING					99									//停留定时器

//发牌变量
#define SEND_PELS					30									//发牌象素
#define SEND_SPACE					20									//发牌间隔
#define IDI_SEND_CARD				100									//发牌定时器

//回收变量
#define RECLAIM_PELS				30									//收牌象素
#define RECLAIM_SPACE				20									//收牌间隔
#define IDI_RECLAIM_CARD			101									//收牌定时器

//回收筹码
#define SCORE_PELS					30									//收牌象素
#define SCORE_COUNT					10									//收牌间隔
#define IDI_SCORE_CARD				102									//收牌定时器

//筹码按钮
#define IDC_BT_SCORE_1				100									//筹码按钮
#define IDC_BT_SCORE_2				101									//筹码按钮
#define IDC_BT_SCORE_3				102									//筹码按钮
#define IDC_BT_SCORE_4				103									//筹码按钮
#define IDC_BT_SCORE_5				104									//筹码按钮

#define IDC_CONTROL_MAX				110

//功能按钮
#define IDC_BT_START				200									//开始按钮
#define IDC_BT_CLEAN				201									//清除按钮
#define IDC_BT_CHIP_IN				202									//下注按钮
#define IDC_BT_BANKER				203									//庄家按钮
#define IDC_BT_NO_BANKER			204									//放弃按钮

//本身位置
#define MYSELF_CHAIR_ID				3									//自己位置

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)

	//系统消息
	ON_WM_TIMER()
	ON_WM_CREATE()

	//功能按钮
	ON_BN_CLICKED(IDC_BT_START, OnStart)
	ON_BN_CLICKED(IDC_BT_CLEAN, OnClean)
	ON_BN_CLICKED(IDC_BT_BANKER, OnBanker)
	ON_BN_CLICKED(IDC_BT_CHIP_IN, OnChipin)
	ON_BN_CLICKED(IDC_BT_NO_BANKER, OnNoBanker)

	//筹码按钮
	ON_BN_CLICKED(IDC_BT_SCORE_1, OnScore1)
	ON_BN_CLICKED(IDC_BT_SCORE_2, OnScore2)
	ON_BN_CLICKED(IDC_BT_SCORE_3, OnScore3)
	ON_BN_CLICKED(IDC_BT_SCORE_4, OnScore4)
	ON_BN_CLICKED(IDC_BT_SCORE_5, OnScore5)
	ON_BN_CLICKED(IDC_CONTROL_MAX,ControlMax)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientView::CGameClientView(void) : CGameFrameView(true,32)
{
	//等待变量
	ZeroMemory(m_wUserPost,sizeof(m_wUserPost));

	//历史记录
	m_wDrawCount=0;
	m_lGameScore=0L;

	//下注信息
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//界面变量
	m_wMeChairID=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_wChairIndex,sizeof(m_wChairIndex));
	ZeroMemory(m_bBankerScore,sizeof(m_bBankerScore));

	//状态变量
	m_bLongBanker=false;
	m_bTakeScore=false;
	m_bShowType=false;
	m_bWaitBanker=false;
	m_bWaitPlayer=false;
	ZeroMemory(m_szCardType,sizeof(m_szCardType));

	//发牌变量
	m_wSendIndex=0;
	m_wSendCount=0;
	m_SendCardPos.SetPoint(0,0);

	//收牌变量
	m_wReclaimCount=0;
	m_wReclaimIndex=0;
	m_bReclaimIng=false;
	m_ReclaimCardPos.SetPoint(0,0);
	m_ReclaimStartPos.SetPoint(0,0);
	m_ReclaimEndingPos.SetPoint(0,0);

	//位置变量
	ZeroMemory(m_GoldStation,sizeof(m_GoldStation));

	//加载资源
	m_ImageCard.SetLoadInfo(IDB_GAME_CARD,AfxGetInstanceHandle());
	m_ImageCards.SetLoadInfo(IDB_CARD_BACK,AfxGetInstanceHandle());
	m_ImageBanker.SetLoadInfo(IDB_BANKER_FLAG,AfxGetInstanceHandle());
	m_ImageLongBanker.SetLoadInfo(IDB_LONG_BANKER,AfxGetInstanceHandle());
	m_ImageDrawInfo.SetLoadInfo(IDB_DRAW_INFO,AfxGetInstanceHandle());
	m_ImageViewFill.SetLoadInfo(IDB_VIEW_FILL,AfxGetInstanceHandle());
	m_ImageWaitBanker.SetLoadInfo(IDB_WAIT_BANKER,AfxGetInstanceHandle());
	m_ImageWaitPlayer.SetLoadInfo(IDB_WAIT_PLAYER,AfxGetInstanceHandle());
	m_ImageWaitEnter.SetLoadInfo(IDB_WAIT_ENTER,AfxGetInstanceHandle());
	m_ImageWaitContinue.SetLoadInfo(IDB_WAIT_CONTINUE,AfxGetInstanceHandle());
	m_ImageViewBanker.SetLoadInfo(IDB_VIEW_BACK_BANKER,AfxGetInstanceHandle());
	m_ImageViewPlayer.SetLoadInfo(IDB_VIEW_BACK_PLAYER,AfxGetInstanceHandle());
	m_ImageWait.SetLoadInfo(IDB_WAITING_TIPS,AfxGetInstanceHandle());
	m_ImageUserFlag.SetLoadInfo(IDB_USER_FLAG,AfxGetInstanceHandle());

	return;
}

//析构函数
CGameClientView::~CGameClientView(void)
{
}

//开始按钮
void CGameClientView::OnStart()
{
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
	return;
}

//筹码按钮
void CGameClientView::OnScore1()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,0,0);
	return;
}

//筹码按钮
void CGameClientView::OnScore2()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,1,1);
	return;
}

//筹码按钮
void CGameClientView::OnScore3()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,2,2);
	return;
}

//筹码按钮
void CGameClientView::OnScore4()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,3,3);
	return;
}

//筹码按钮
void CGameClientView::OnScore5()
{
	AfxGetMainWnd()->SendMessage(IDM_SCORE,4,4);
	return;
}

//清除按钮
void CGameClientView::OnClean()
{
	AfxGetMainWnd()->SendMessage(IDM_CLEAN,0,0);
	return;
}

//下注按钮
void CGameClientView::OnChipin()
{
	AfxGetMainWnd()->SendMessage(IDM_CHIP_IN,0,0);
	return;
}

//抢庄按钮
void CGameClientView::OnBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_BANKER,1,1);
	return;
}

//放弃按钮
void CGameClientView::OnNoBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_BANKER,0,0);
	return;
}

//时间消息
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//发送处理
	if (nIDEvent==IDI_SEND_CARD)
	{
		//设置索引
		m_wSendIndex++;

		//设置位置
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
		m_SendCardPos.x=m_SendStartPos.x+(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)*m_wSendIndex/m_wSendCount;
		m_SendCardPos.y=m_SendStartPos.y+(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)*m_wSendIndex/m_wSendCount;

		//更新界面
		UpdateGameView(NULL);

		//停止判断
		if (m_wSendIndex>=m_wSendCount)
		{
			//获取扑克
			BYTE cbCardData[MAX_COUNT];
			WORD wChairID=pSendCardItem->wChairID;
			BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

			//设置扑克
			if (cbCardCount<MAX_COUNT)
			{
				cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
			}

			//删除动画
			m_SendCardItemArray.RemoveAt(0);

			//继续动画
			if (m_SendCardItemArray.GetCount()>0)
			{
				//变量定义
				tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

				//设置位置
				m_SendCardPos.x=m_SendStartPos.x;
				m_SendCardPos.y=m_SendStartPos.y;

				//位移计算
				int nXCount=abs(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)/SEND_PELS;
				int nYCount=abs(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)/SEND_PELS;

				//设置次数
				m_wSendIndex=0;
				m_wSendCount=__max(nXCount,nYCount);

				//播放声音
				CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
				pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

				return;
			}
			else 
			{
				//完成处理
				KillTimer(IDI_SEND_CARD);
				AfxGetMainWnd()->PostMessage(IDM_SEND_CARD_FINISH,0,0);

				return;
			}
		}

		return;
	}

	//收起扑克
	if (nIDEvent==IDI_RECLAIM_CARD)
	{
		//设置位置
		if (m_wReclaimIndex<m_wReclaimCount)
		{
			//设置索引
			m_wReclaimIndex++;

			//设置位置
			m_ReclaimCardPos.x=m_ReclaimStartPos.x+(m_ReclaimEndingPos.x-m_ReclaimStartPos.x)*m_wReclaimIndex/m_wReclaimCount;
			m_ReclaimCardPos.y=m_ReclaimStartPos.y+(m_ReclaimEndingPos.y-m_ReclaimStartPos.y)*m_wReclaimIndex/m_wReclaimCount;

			//更新界面
			UpdateGameView(NULL);

			return;
		}

		//起始索引
		WORD wStartIndex=0;
		if (m_wBankerUser==m_wMeChairID) wStartIndex=3;

		//寻找扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//获取索引
			WORD wIndex=(wStartIndex+i)%GAME_PLAYER;

			//获取扑克
			BYTE cbCardData[MAX_COUNT];
			WORD wCardCount=m_CardControl[wIndex].GetCardData(cbCardData,CountArray(cbCardData));

			//回收判断
			if (wCardCount>0)
			{
				//设置扑克
				m_CardControl[wIndex].SetCardData(NULL,0);

				//设置位置
				m_ReclaimCardPos.x=m_SendEndingPos[wIndex].x;
				m_ReclaimCardPos.y=m_SendEndingPos[wIndex].y;
				m_ReclaimStartPos.x=m_SendEndingPos[wIndex].x;
				m_ReclaimStartPos.y=m_SendEndingPos[wIndex].y;

				//位移计算
				int nXCount=abs(m_ReclaimEndingPos.x-m_ReclaimStartPos.x)/RECLAIM_PELS;
				int nYCount=abs(m_ReclaimEndingPos.y-m_ReclaimStartPos.y)/RECLAIM_PELS;

				//设置次数
				m_wReclaimIndex=0;
				m_wReclaimCount=__max(nXCount,nYCount);

				//更新界面
				UpdateGameView(NULL);

				return;
			}
		}

		//完成判断
		if (i==GAME_PLAYER) 
		{
			//设置变量
			m_bReclaimIng=false;

			//删除时间
			KillTimer(IDI_RECLAIM_CARD);

			//更新界面
			UpdateGameView(NULL);

			//发送消息
			AfxGetMainWnd()->PostMessage(IDM_RECLAIM_CARD_FINISH,0,0);
		}

		return;
	}

	//收起筹码
	if (nIDEvent==IDI_SCORE_CARD)
	{
		if (m_wScoreIndex<SCORE_COUNT)
		{
			//设置索引
			m_wScoreIndex++;

			//庄家位置
			WORD wBankerIndex=0;
			if (m_wBankerUser==m_wMeChairID) wBankerIndex=3;
			CPoint ScoreBanker=m_CardControl[wBankerIndex].GetBenchmarkPos();

			//移动位置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				LONG lScore=m_GoldView[i].GetGold();
				if (lScore==0L) continue;

				if (m_bBankerScore[i]==true)
				{
					m_GoldTarget[i].x=(ScoreBanker.x-m_GoldStation[i].x)*m_wScoreIndex/SCORE_COUNT+m_GoldStation[i].x;
					m_GoldTarget[i].y=(ScoreBanker.y-m_GoldStation[i].y)*m_wScoreIndex/SCORE_COUNT+m_GoldStation[i].y;
				}
				else
				{
					CPoint ScorePlayer=m_CardControl[i].GetBenchmarkPos();
					m_GoldTarget[i].x=(ScorePlayer.x-m_GoldStation[i].x)*m_wScoreIndex/SCORE_COUNT+m_GoldStation[i].x;
					m_GoldTarget[i].y=(ScorePlayer.y-m_GoldStation[i].y)*m_wScoreIndex/SCORE_COUNT+m_GoldStation[i].y;
				}
			}

			//更新界面
			UpdateGameView(NULL);
		}
		else
		{
			m_wScoreIndex=0;
			m_bTakeScore=false;
			ZeroMemory(m_bBankerScore,sizeof(m_bBankerScore));
			KillTimer(IDI_SCORE_CARD);

			//设置筹码
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GoldView[i].SetGold(0);
			}

			//显示成绩
			m_ScoreView.ShowScore();

			//更新界面
			UpdateGameView(NULL);
		}

		return;
	}

	//等待开始
	if(IDI_WAITING==nIDEvent)
	{
		m_bDrawTimes++;

		//更新界面
		UpdateGameView(NULL);

		return ;
	}

	__super::OnTimer(nIDEvent);
}

//建立消息
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建控件
	CRect rcCreate(0,0,0,0);
	for (int i=0;i<GAME_PLAYER;i++) m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);

	//开始按钮
	m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false);

	//清除按钮
	m_btClean.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_CLEAN);
	m_btClean.SetButtonImage(IDB_BT_CLEAN,AfxGetInstanceHandle(),false);

	//下注按钮
	m_btChipIn.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_CHIP_IN);
	m_btChipIn.SetButtonImage(IDB_BT_CHIP_IN,AfxGetInstanceHandle(),false);

	//抢庄按钮
	m_btBanker.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_BANKER);
	m_btBanker.SetButtonImage(IDB_BT_BANKER,AfxGetInstanceHandle(),false);

	//放弃按钮
	m_btNoBanker.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_NO_BANKER);
	m_btNoBanker.SetButtonImage(IDB_BT_NO_BANKER,AfxGetInstanceHandle(),false);

	//筹码按钮
	m_btScore1.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_1);
	m_btScore2.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_2);
	m_btScore3.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_3);
	m_btScore4.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_4);
	m_btScore5.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BT_SCORE_5);

	//设置按钮
	m_btScore1.SetButtonImage(IDB_BT_SCORE_1,AfxGetInstanceHandle(),false);
	m_btScore2.SetButtonImage(IDB_BT_SCORE_2,AfxGetInstanceHandle(),false);
	m_btScore3.SetButtonImage(IDB_BT_SCORE_3,AfxGetInstanceHandle(),false);
	m_btScore4.SetButtonImage(IDB_BT_SCORE_4,AfxGetInstanceHandle(),false);
	m_btScore5.SetButtonImage(IDB_BT_SCORE_5,AfxGetInstanceHandle(),false);

	m_btControlMax.Create(TEXT("控制最大"),WS_CHILD|WS_DISABLED,CRect(10,10,70,30),this,IDC_CONTROL_MAX);
	m_btControlMax.ShowWindow(SW_HIDE);

	return 0;
}

//重置界面
void CGameClientView::ResetGameView()
{
	ZeroMemory(m_wUserPost,sizeof(m_wUserPost));

	//下注信息
	m_lMaxScore=0L;
	m_lLessScore=0L;

	//历史记录
	m_wDrawCount=0;
	m_lGameScore=0L;

	//状态变量
	m_bLongBanker=false;
	m_bShowType=false;
	m_bTakeScore=false;
	m_bWaitBanker=false;
	m_bWaitPlayer=false;
	ZeroMemory(m_szCardType,sizeof(m_szCardType));
	ZeroMemory(m_bBankerScore,sizeof(m_bBankerScore));

	//界面变量
	m_wMeChairID=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_wChairIndex,sizeof(m_wChairIndex));

	//发牌变量
	m_wSendIndex=0;
	m_wSendCount=0;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();

	//收牌变量
	m_wReclaimCount=0;
	m_wReclaimIndex=0;
	m_bReclaimIng=false;
	m_ReclaimCardPos.SetPoint(0,0);
	m_ReclaimStartPos.SetPoint(0,0);
	m_ReclaimEndingPos.SetPoint(0,0);

	//设置控件
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GoldView[i].SetGold(0L);
		m_CardControl[i].SetCardData(NULL,0);
	}

	//隐藏控件
	m_btStart.ShowWindow(SW_HIDE);
	m_btClean.ShowWindow(SW_HIDE);
	m_btChipIn.ShowWindow(SW_HIDE);
	m_btBanker.ShowWindow(SW_HIDE);
	m_btNoBanker.ShowWindow(SW_HIDE);

	//禁用控件
	m_btScore1.EnableWindow(FALSE);
	m_btScore2.EnableWindow(FALSE);
	m_btScore3.EnableWindow(FALSE);
	m_btScore4.EnableWindow(FALSE);
	m_btScore5.EnableWindow(FALSE);

	//隐藏窗口
	m_ScoreView.ResetScore();

	//删除时间
	KillTimer(IDI_SEND_CARD);
	KillTimer(IDI_SCORE_CARD);
	KillTimer(IDI_RECLAIM_CARD);

	return;
}

//调整控件
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//移动控件
	if (m_wMeChairID!=m_wBankerUser)
	{
		//用户位置
		m_ptName[0].x=nWidth/2+5;
		m_ptName[0].y=nHeight/2-244;
		m_ptFace[0].x=nWidth/2-m_nXFace;
		m_ptFace[0].y=nHeight/2-248;

		//用户位置
		m_ptName[1].x=nWidth/2+370+m_nXFace;
		m_ptName[1].y=nHeight/2+37;
		m_ptFace[1].x=nWidth/2+365;
		m_ptFace[1].y=nHeight/2+33;

		//用户位置
		m_ptName[2].x=nWidth/2+185+m_nXFace;
		m_ptName[2].y=nHeight/2+146;
		m_ptFace[2].x=nWidth/2+180;
		m_ptFace[2].y=nHeight/2+142;

		//用户位置
		m_ptName[3].x=nWidth/2+5;
		m_ptName[3].y=nHeight/2+177;
		m_ptFace[3].x=nWidth/2-m_nXFace;
		m_ptFace[3].y=nHeight/2+173;

		//用户位置
		m_ptName[4].x=nWidth/2-260+m_nXFace;
		m_ptName[4].y=nHeight/2+146;
		m_ptFace[4].x=nWidth/2-265;
		m_ptFace[4].y=nHeight/2+142;

		//用户位置
		m_ptName[5].x=nWidth/2-445+m_nXFace;
		m_ptName[5].y=nHeight/2+37;
		m_ptFace[5].x=nWidth/2-450;
		m_ptFace[5].y=nHeight/2+33;

		//扑克位置
		m_SendStartPos.SetPoint(nWidth/2+370,nHeight/2-260);
		m_ReclaimEndingPos.SetPoint(nWidth/2-410,nHeight/2-290);

		//发牌位置
		m_SendEndingPos[0].SetPoint(nWidth/2,nHeight/2-158);
		m_SendEndingPos[1].SetPoint(nWidth/2+406,nHeight/2-33);
		m_SendEndingPos[2].SetPoint(nWidth/2+223,nHeight/2+73);
		m_SendEndingPos[3].SetPoint(nWidth/2,nHeight/2+108);
		m_SendEndingPos[4].SetPoint(nWidth/2-223,nHeight/2+73);
		m_SendEndingPos[5].SetPoint(nWidth/2-406,nHeight/2-33);

		//移动准备
		HDWP hDwp=BeginDeferWindowPos(32);
		const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

		//移动筹码
		DeferWindowPos(hDwp,m_btScore1,NULL,nWidth/2-489,nHeight/2+128,0,0,uFlags|SWP_SHOWWINDOW);
		DeferWindowPos(hDwp,m_btScore2,NULL,nWidth/2-420,nHeight/2+170,0,0,uFlags|SWP_SHOWWINDOW);
		DeferWindowPos(hDwp,m_btScore3,NULL,nWidth/2-350,nHeight/2+200,0,0,uFlags|SWP_SHOWWINDOW);
		DeferWindowPos(hDwp,m_btScore4,NULL,nWidth/2-280,nHeight/2+224,0,0,uFlags|SWP_SHOWWINDOW);
		DeferWindowPos(hDwp,m_btScore5,NULL,nWidth/2-210,nHeight/2+240,0,0,uFlags|SWP_SHOWWINDOW);

		//开始按钮
		DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2+160,nHeight/2+130,0,0,uFlags);

		//下注按钮
		DeferWindowPos(hDwp,m_btClean,NULL,nWidth/2+60,nHeight/2+267,0,0,uFlags);
		DeferWindowPos(hDwp,m_btChipIn,NULL,nWidth/2-30,nHeight/2+267,0,0,uFlags);

		//抢庄按钮
		DeferWindowPos(hDwp,m_btBanker,NULL,nWidth/2-30,nHeight/2+267,0,0,uFlags);
		DeferWindowPos(hDwp,m_btNoBanker,NULL,nWidth/2+60,nHeight/2+267,0,0,uFlags);

		//结束移动
		EndDeferWindowPos(hDwp);

		//移动筹码
		m_GoldStation[0].SetPoint(nWidth/2,nHeight/2-158);
		m_GoldStation[1].SetPoint(nWidth/2+315,nHeight/2-150);
		m_GoldStation[2].SetPoint(nWidth/2+170,nHeight/2-58);
		m_GoldStation[3].SetPoint(nWidth/2,nHeight/2-33);
		m_GoldStation[4].SetPoint(nWidth/2-170,nHeight/2-58);
		m_GoldStation[5].SetPoint(nWidth/2-315,nHeight/2-150);

		//移动扑克
		m_CardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-158,enXCenter,enYCenter);
		m_CardControl[1].SetBenchmarkPos(nWidth/2+406,nHeight/2-33,enXCenter,enYCenter);
		m_CardControl[2].SetBenchmarkPos(nWidth/2+223,nHeight/2+73,enXCenter,enYCenter);
		m_CardControl[3].SetBenchmarkPos(nWidth/2,nHeight/2+108,enXCenter,enYCenter);
		m_CardControl[4].SetBenchmarkPos(nWidth/2-223,nHeight/2+73,enXCenter,enYCenter);
		m_CardControl[5].SetBenchmarkPos(nWidth/2-406,nHeight/2-33,enXCenter,enYCenter);
	}
	else
	{
		//用户位置
		m_ptName[0].x=nWidth/2+5;
		m_ptName[0].y=nHeight/2-200;
		m_ptFace[0].x=nWidth/2-m_nXFace;
		m_ptFace[0].y=nHeight/2-204;

		//用户位置
		m_ptName[1].x=nWidth/2+185+m_nXFace;
		m_ptName[1].y=nHeight/2-166;
		m_ptFace[1].x=nWidth/2+180;
		m_ptFace[1].y=nHeight/2-170;

		//用户位置
		m_ptName[2].x=nWidth/2+370+m_nXFace;
		m_ptName[2].y=nHeight/2-61;
		m_ptFace[2].x=nWidth/2+365;
		m_ptFace[2].y=nHeight/2-65;

		//用户位置
		m_ptName[3].x=nWidth/2+5;
		m_ptName[3].y=nHeight/2+219;
		m_ptFace[3].x=nWidth/2-m_nXFace;
		m_ptFace[3].y=nHeight/2+215;

		//用户位置
		m_ptName[4].x=nWidth/2-445+m_nXFace;
		m_ptName[4].y=nHeight/2-61;
		m_ptFace[4].x=nWidth/2-450;
		m_ptFace[4].y=nHeight/2-65;

		//用户位置
		m_ptName[5].x=nWidth/2-260+m_nXFace;
		m_ptName[5].y=nHeight/2-166;
		m_ptFace[5].x=nWidth/2-265;
		m_ptFace[5].y=nHeight/2-170;

		//扑克位置
		m_SendStartPos.SetPoint(nWidth/2-370,nHeight/2+260);
		m_ReclaimEndingPos.SetPoint(nWidth/2+410,nHeight/2+290);

		//发牌位置
		m_SendEndingPos[0].SetPoint(nWidth/2,nHeight/2-108);
		m_SendEndingPos[1].SetPoint(nWidth/2+223,nHeight/2-73);
		m_SendEndingPos[2].SetPoint(nWidth/2+406,nHeight/2+33);
		m_SendEndingPos[3].SetPoint(nWidth/2,nHeight/2+158);
		m_SendEndingPos[4].SetPoint(nWidth/2-406,nHeight/2+33);
		m_SendEndingPos[5].SetPoint(nWidth/2-223,nHeight/2-73);

		//移动准备
		HDWP hDwp=BeginDeferWindowPos(32);
		const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

		//移动筹码
		DeferWindowPos(hDwp,m_btScore1,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);
		DeferWindowPos(hDwp,m_btScore2,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);
		DeferWindowPos(hDwp,m_btScore3,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);
		DeferWindowPos(hDwp,m_btScore4,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);
		DeferWindowPos(hDwp,m_btScore5,NULL,0,0,0,0,uFlags|SWP_HIDEWINDOW);

		//开始按钮
		DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2+160,nHeight/2+130,0,0,uFlags);

		//下注按钮
		DeferWindowPos(hDwp,m_btClean,NULL,nWidth/2-10,nHeight/2+150,0,0,uFlags);
		DeferWindowPos(hDwp,m_btChipIn,NULL,nWidth/2-100,nHeight/2+150,0,0,uFlags);

		//抢庄按钮
		DeferWindowPos(hDwp,m_btBanker,NULL,nWidth/2-30,nHeight/2+267,0,0,uFlags);
		DeferWindowPos(hDwp,m_btNoBanker,NULL,nWidth/2+60,nHeight/2+267,0,0,uFlags);

		//结束移动
		EndDeferWindowPos(hDwp);

		//移动筹码
		m_GoldStation[0].SetPoint(nWidth/2,nHeight/2+40);
		m_GoldStation[1].SetPoint(nWidth/2+170,nHeight/2+61);
		m_GoldStation[2].SetPoint(nWidth/2+315,nHeight/2+150);
		m_GoldStation[3].SetPoint(nWidth/2,nHeight/2+158);
		m_GoldStation[4].SetPoint(nWidth/2-315,nHeight/2+150);
		m_GoldStation[5].SetPoint(nWidth/2-170,nHeight/2+61);

		//移动扑克
		m_CardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-108,enXCenter,enYCenter);
		m_CardControl[1].SetBenchmarkPos(nWidth/2+223,nHeight/2-73,enXCenter,enYCenter);
		m_CardControl[2].SetBenchmarkPos(nWidth/2+406,nHeight/2+33,enXCenter,enYCenter);
		m_CardControl[3].SetBenchmarkPos(nWidth/2,nHeight/2+158,enXCenter,enYCenter);
		m_CardControl[4].SetBenchmarkPos(nWidth/2-406,nHeight/2+33,enXCenter,enYCenter);
		m_CardControl[5].SetBenchmarkPos(nWidth/2-223,nHeight/2-73,enXCenter,enYCenter);
	}

	//调整动画
	SetFlowerControlInfo(0,m_ptFace[0].x-BIG_FACE_WIDTH/2,m_ptFace[0].y);
	SetFlowerControlInfo(1,m_ptFace[1].x-BIG_FACE_WIDTH/2,m_ptFace[1].y);
	SetFlowerControlInfo(2,m_ptFace[2].x-BIG_FACE_WIDTH/2,m_ptFace[2].y);
	SetFlowerControlInfo(3,m_ptFace[3].x-BIG_FACE_WIDTH/2,m_ptFace[3].y);
	SetFlowerControlInfo(4,m_ptFace[4].x,m_ptFace[4].y					);
	SetFlowerControlInfo(5,m_ptFace[5].x-BIG_FACE_WIDTH/2,m_ptFace[5].y);
	return;
}

//绘画界面
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//绘画背景
	DrawViewImage(pDC,m_ImageViewFill,enMode_Spread);

	//绘画背景
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		if (m_wMeChairID==m_wBankerUser) DrawViewImage(pDC,m_ImageViewBanker,enMode_Centent);
		if (m_wMeChairID!=m_wBankerUser) DrawViewImage(pDC,m_ImageViewPlayer,enMode_Centent);
	}

	//总共下注
	LONG lSumScore=0L;
	for (WORD i=0;i<GAME_PLAYER;i++) lSumScore+=m_GoldView[i].GetGold();

	//下注信息
	if (m_lMaxScore!=0L)
	{
		//构造信息
		TCHAR szBuffer[128]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("最大下注：￥%ld"),m_lMaxScore);

		//绘画信息
		if (m_wMeChairID!=m_wBankerUser)
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOut(nWidth/2-350,nHeight/2-335,szBuffer,lstrlen(szBuffer));
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOut(nWidth/2+243,nHeight/2+295,szBuffer,lstrlen(szBuffer));
		}
	}
	if (m_lLessScore!=0L)
	{
		//构造信息
		TCHAR szBuffer[128]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("最小下注：￥%ld"),m_lLessScore);

		//绘画信息
		if (m_wMeChairID!=m_wBankerUser)
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOut(nWidth/2-350,nHeight/2-310,szBuffer,lstrlen(szBuffer));
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->TextOut(nWidth/2+243,nHeight/2+312,szBuffer,lstrlen(szBuffer));
		}
	}
	if ((m_wMeChairID==m_wBankerUser)&&((m_lLessScore!=0L)||(m_lMaxScore!=0L)))
	{
		//构造信息
		TCHAR szBuffer[128]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("总共下注：￥%ld"),lSumScore);

		//绘画信息
		pDC->SetTextColor(RGB(0,0,0));
		pDC->TextOut(nWidth/2+243,nHeight/2+330,szBuffer,lstrlen(szBuffer));
	}

	//绘画筹码
	for (DWORD i=0;i<CountArray(m_GoldView);i++)
	{
		if (m_bTakeScore==false)
		{
			m_GoldView[i].DrawGoldView(pDC,m_GoldStation[i].x,m_GoldStation[i].y,true);
		}
		else
		{
			m_GoldView[i].DrawGoldView(pDC,m_GoldTarget[i].x,m_GoldTarget[i].y,false);
		}
	}

	//庄家标志
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		CImageHandle HandleBanker(&m_ImageBanker);
		CImageHandle HandleLongBanker(&m_ImageLongBanker);
		if (m_wBankerUser==m_wMeChairID) 
		{
			if(!m_bLongBanker)	//常庄
			{
				m_ImageBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2+217);
			}
			else				//长庄
			{
				m_ImageBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2+217);
				//m_ImageLongBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2+217);
			}
		}
		else 
		{
			if(!m_bLongBanker)	//常庄
			{
				m_ImageBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2-245);
			}
			else				//长庄
			{
				m_ImageBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2-245);
				//m_ImageLongBanker.BitBlt(pDC->m_hDC,nWidth/2-110,nHeight/2-245);
			}
		}
	}

	//用户统计
	WORD wUserCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL) wUserCount++;
	}

	//进入标志
	if ((wUserCount==1)&&(m_wBankerUser==m_wMeChairID))
	{
		CImageHandle HandleWaitEnter(&m_ImageWaitEnter);
		m_ImageWaitEnter.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitEnter.GetWidth())/2,nHeight/2-10);
	}

	//等待标志
	if ((m_bWaitBanker==true)&&(wUserCount>1))
	{
		if (m_wBankerUser==m_wMeChairID) 
		{
			if (m_btStart.IsWindowVisible())
			{
				CImageHandle HandleWaitBanker(&m_ImageWaitBanker);
				m_ImageWaitBanker.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitBanker.GetWidth())/2,nHeight/2+80);
			}
			else
			{
				CImageHandle HandleWaitBanker(&m_ImageWaitContinue);
				m_ImageWaitContinue.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitContinue.GetWidth())/2,nHeight/2+80);
			}
		}
		else
		{
			CImageHandle HandleWaitBanker(&m_ImageWaitBanker);
			m_ImageWaitBanker.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitBanker.GetWidth())/2,nHeight/2-120);
		}
	}

	//等待标志
	if ((m_bWaitPlayer==true)&&(wUserCount>1))
	{
		//绘画标志
		CImageHandle HandleWaitPlayer(&m_ImageWaitPlayer);
		if (m_wBankerUser==m_wMeChairID) m_ImageWaitPlayer.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitPlayer.GetWidth())/2,nHeight/2+80);
		else m_ImageWaitPlayer.BitBlt(pDC->m_hDC,(nWidth-m_ImageWaitPlayer.GetWidth())/2,nHeight/2-120);

		//绘画时间
		if (m_wBankerUser==m_wMeChairID)
		{
			WORD wUserTimer=GetUserTimer(m_wBankerUser);
			if (wUserTimer!=0L) DrawUserTimer(pDC,(nWidth+m_ImageWaitPlayer.GetWidth())/2-30,nHeight/2+94,wUserTimer);
		}
	}

	//绘画用户
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		const tagUserData * pUserData=GetUserInfo(i);

		//绘画用户
		if (pUserData!=NULL)
		{
			//获取位置
			WORD wViewFaceID=m_wChairIndex[i];
			pDC->SetTextAlign(TA_LEFT);
			DrawTextString(pDC,pUserData->szName,RGB(255,255,255),RGB(0,0,0),m_ptName[wViewFaceID].x,m_ptName[wViewFaceID].y);

			//用户金币
			TCHAR szBuffer[64]=TEXT("");
			LONG lLeaveScore=pUserData->lScore-m_GoldView[wViewFaceID].GetGold();
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("￥%ld"),lLeaveScore);
			DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptName[wViewFaceID].x,m_ptName[wViewFaceID].y+15);

			//用户头像
			DrawUserFace(pDC,pUserData,m_ptFace[wViewFaceID].x,m_ptFace[wViewFaceID].y,false);

			//玩家时间
			if ((m_wBankerUser!=m_wMeChairID)||(m_bWaitBanker==true))
			{
				WORD wUserTimer=GetUserTimer(i);
				if (wUserTimer!=0L) DrawUserTimer(pDC,m_ptFace[wViewFaceID].x-33+10,m_ptFace[wViewFaceID].y+6,wUserTimer);
			}
		}
	}

	//绘画牌型
	if (m_bShowType==true)
	{
		//创建字体
		CFont TypeFont;
		TypeFont.CreateFont(-14,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("宋体"));

		//设置 DC
		pDC->SelectObject(&TypeFont);
		//pDC->SetTextColor(RGB(250,200,40));

		//绘画信息
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_szCardType[i][0]!=0)
			{
				CPoint Point=m_CardControl[i].GetBenchmarkPos();
				DrawTextString(pDC,m_szCardType[i],RGB(255,255,255),RGB(0,0,0),Point.x+68,Point.y-45);
			}
		}

		//设置 DC
		pDC->SelectObject(CSkinResourceManager::GetDefaultFont());
	}

	//发牌扑克
	if (m_SendCardItemArray.GetCount()>0)
	{
		//变量定义
		CImageHandle HandleCard(&m_ImageCard);
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//获取大小
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//绘画扑克
		int nXPos=m_SendCardPos.x-nItemWidth/2;
		int nYPos=m_SendCardPos.y-nItemHeight/2;
		m_ImageCard.AlphaDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}

	//回收扑克
	if (m_bReclaimIng==true)
	{
		//加载资源
		CImageHandle HandleCard(&m_ImageCards);

		//获取大小
		int nItemWidth=m_ImageCards.GetWidth();
		int nItemHeight=m_ImageCards.GetHeight();

		//绘画扑克
		int nXPos=m_ReclaimCardPos.x-nItemWidth/2;
		int nYPos=m_ReclaimCardPos.y-nItemHeight/2;
		m_ImageCards.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
	}

	//历史成绩
	if (m_wDrawCount!=0)
	{
		//构造信息
		TCHAR szDrawCount[32]=TEXT(""),szGameScore[32]=TEXT("");
		_sntprintf(szDrawCount,CountArray(szDrawCount),TEXT("%ld"),m_wDrawCount);
		_sntprintf(szGameScore,CountArray(szGameScore),TEXT("%ld"),m_lGameScore);

		//绘画背景
		if (m_wMeChairID==m_wBankerUser)
		{
			CImageHandle HandleDrawInfo(&m_ImageDrawInfo);
			m_ImageDrawInfo.AlphaDrawImage(pDC,m_nXBorder,m_nYBorder,RGB(255,0,255));
		}
		else
		{
			CImageHandle HandleDrawInfo(&m_ImageDrawInfo);
			m_ImageDrawInfo.AlphaDrawImage(pDC,nWidth-m_nXBorder-m_ImageDrawInfo.GetWidth(),nHeight-m_nYBorder-m_ImageDrawInfo.GetHeight(),RGB(255,0,255));
		}

		//设置 DC
		pDC->SetTextColor(RGB(10,10,10));
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//绘画信息
		if (m_wMeChairID==m_wBankerUser)
		{
			CRect rcDrawCount(m_nXBorder+10,m_nYBorder+44,m_nXBorder+91,m_nYBorder+66);
			CRect rcGameScore(m_nXBorder+94,m_nYBorder+44,m_nXBorder+188,m_nYBorder+66);
			pDC->DrawText(szDrawCount,lstrlen(szDrawCount),&rcDrawCount,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
			pDC->DrawText(szGameScore,lstrlen(szGameScore),&rcGameScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
		}
		else
		{
			CRect rcDrawCount(nWidth-m_nXBorder-188,nHeight-m_nYBorder-31,nWidth-m_nXBorder-106,nHeight-m_nYBorder-8);
			CRect rcGameScore(nWidth-m_nXBorder-101,nHeight-m_nYBorder-31,nWidth-m_nXBorder-10,nHeight-m_nYBorder-8);
			pDC->Draw3dRect(&rcDrawCount,0,0);
			pDC->Draw3dRect(&rcGameScore,0,0);
			pDC->DrawText(szDrawCount,lstrlen(szDrawCount),&rcDrawCount,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
			pDC->DrawText(szGameScore,lstrlen(szGameScore),&rcGameScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
		}
	}

	return;
}

//获取筹码
LONG CGameClientView::GetUserClipInInfo(WORD wChairID)
{
	//获取筹码
	WORD wChairIndex=m_wChairIndex[wChairID];
	LONG lScore=m_GoldView[wChairIndex].GetGold();

	return lScore;
}

//显示牌型
void CGameClientView::SetCardType(bool bShowType)
{
	//设置变量
	m_bShowType=bShowType;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//等待庄家
void CGameClientView::SetWaitBanker(bool bWaitBanker)
{
	//设置变量
	m_bWaitBanker=bWaitBanker;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//等待闲家
void CGameClientView::SetWaitPlayer(bool bWaitPlayer)
{
	//设置变量
	m_bWaitPlayer=bWaitPlayer;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//下注信息
void CGameClientView::SetChipInInfo(LONG lMaxScore, LONG lLessScore)
{
	//设置变量
	m_lMaxScore=lMaxScore;
	m_lLessScore=lLessScore;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//设置信息
void CGameClientView::SetUserDrawInfo(WORD wDrawCount, LONG lGameScore)
{
	//设置变量
	m_wDrawCount=wDrawCount;
	m_lGameScore=lGameScore;

	//更新界面
	UpdateGameView(NULL);

	return;
}

//设置牌型
void CGameClientView::SetCardTypeInfo(WORD wChairID, LPCTSTR pszCardType)
{
	//设置变量
	lstrcpyn(m_szCardType[wChairID],pszCardType,CountArray(m_szCardType[wChairID]));

	//更新界面
	if (m_bShowType==true) UpdateGameView(NULL);

	return;
}

//设置位置
void CGameClientView::SetUserChairInfo(WORD wBankerUser, WORD wMeChairID)
{
	//设置变量
	m_wMeChairID=wMeChairID;
	m_wBankerUser=wBankerUser;

	//位置更新
	if ((m_wBankerUser!=INVALID_CHAIR)&&(m_wMeChairID!=INVALID_CHAIR))
	{
		if (m_wBankerUser!=m_wMeChairID)
		{
			//默认位置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_wChairIndex[i]=i;
			}

			//实际位置
			WORD wTemp = m_wUserPost[m_wBankerUser];
			m_wUserPost[m_wBankerUser] = m_wUserPost[0];
			m_wUserPost[0] = wTemp;

			//庄家位置
			m_wChairIndex[m_wBankerUser]=0;
			m_wChairIndex[0]=m_wBankerUser;

			//实际位置
			wTemp = m_wUserPost[m_wChairIndex[m_wMeChairID]];
			m_wUserPost[m_wChairIndex[m_wMeChairID]] = m_wUserPost[m_wChairIndex[MYSELF_CHAIR_ID]];
			m_wUserPost[m_wChairIndex[MYSELF_CHAIR_ID]] = wTemp;

			//我的位置
			//WORD wChairID=m_wChairIndex[m_wMeChairID];
			m_wChairIndex[m_wMeChairID]=MYSELF_CHAIR_ID;
			m_wChairIndex[MYSELF_CHAIR_ID]=m_wMeChairID;
		}
		else
		{
			//庄家模式
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_wChairIndex[(i+m_wMeChairID)%GAME_PLAYER]=(i+3)%GAME_PLAYER;
			}
		}
	}

	//调整界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

	//更新界面
	UpdateGameView(NULL);

	return;
}

//设置筹码
void CGameClientView::SetUserClipInInfo(WORD wChairID, LONG lChipInCount)
{
	//设置变量
	if (wChairID!=INVALID_CHAIR)
	{
		WORD wChairIndex=m_wChairIndex[wChairID];
		m_GoldView[wChairIndex].SetGold(lChipInCount);
	}
	else
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_GoldView[i].SetGold(0L);
	}

	//更新界面
	UpdateGameView(NULL);

	return;
}


//设置长庄
void CGameClientView::SetLongBanker(bool bLongBanker)
{
	if(m_bLongBanker!=bLongBanker)
	{
		m_bLongBanker=bLongBanker;

		//更新界面
		UpdateGameView(NULL);
	}

	return ;
}


//物理位置
void CGameClientView::SetUserPost(WORD wPhPost,WORD wViewPost)
{
	//设置变量
	m_wUserPost[wViewPost] = wPhPost;

	return;
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//动画判断
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
}

//收起扑克
void CGameClientView::ReclaimUserCard()
{
	if (m_bReclaimIng==false)
	{
		//设置变量
		m_bReclaimIng=true;

		//设置状态
		m_wReclaimIndex=0;
		m_wReclaimCount=0;
		m_ReclaimCardPos.SetPoint(0,0);

		//设置时间
		SetTimer(IDI_RECLAIM_CARD,RECLAIM_SPACE,NULL);
	}

	return;
}

//收起筹码
void CGameClientView::ReclaimUserScore()
{
	//设置变量
	m_wScoreIndex=0;
	m_bTakeScore=true;

	//设置时间
	SetTimer(IDI_SCORE_CARD,100,NULL);

	return;
}

//完成发牌
void CGameClientView::FinishDispatchCard()
{
	//完成判断
	if (m_SendCardItemArray.GetCount()==0) return;

	//完成动画
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//获取数据
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//获取扑克
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//设置扑克
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//删除处理
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//更新界面
	UpdateGameView(NULL);

	return;
}

//发送扑克
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//效验参数
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//设置变量
	tagSendCardItem SendCardItem;
	SendCardItem.cbCardData=cbCardData;
	SendCardItem.wChairID=m_wChairIndex[wChairID];

	//插入数组
	m_SendCardItemArray.Add(SendCardItem);

	//启动动画
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//设置位置
		m_SendCardPos.x=m_SendStartPos.x;
		m_SendCardPos.y=m_SendStartPos.y;

		//位移计算
		int nXCount=abs(m_SendEndingPos[SendCardItem.wChairID].x-m_SendStartPos.x)/SEND_PELS;
		int nYCount=abs(m_SendEndingPos[SendCardItem.wChairID].y-m_SendStartPos.y)/SEND_PELS;

		//设置次数
		m_wSendIndex=0;
		m_wSendCount=__max(5,__max(nXCount,nYCount));

		//设置定时器
		SetTimer(IDI_SEND_CARD,SEND_SPACE,NULL);

		//播放声音
		CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
		pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}
void CGameClientView::ControlMax()
{
	//如果有此权限
	if((GetUserInfo(m_wMeChairID)->dwUserRight&UR_GAME_CONTROL)!=0)
	{
		CGameClientDlg * pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
		pGameClientDlg->SendData(SUB_C_CONTROL_MAX,NULL,0);
		return;
	}
}
//////////////////////////////////////////////////////////////////////////
