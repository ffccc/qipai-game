#include "StdAfx.h"
#include "GameLogic.h"
#include "ScoreView.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CScoreView::CScoreView(void)
{
	//设置变量
	ZeroMemory(&m_ScoreViewInfo,sizeof(m_ScoreViewInfo));

	//加载资源
	m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());

	return;
}

//析构函数
CScoreView::~CScoreView(void)
{
}

//建立消息
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置数据
	ZeroMemory(&m_ScoreViewInfo,sizeof(m_ScoreViewInfo));

	//设置界面
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//移动窗口
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//重画函数
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//设置 DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//绘画背景
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//显示分数
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_ScoreViewInfo.szUserName);i++)
	{
		if (m_ScoreViewInfo.szUserName[i][0]!=0)
		{
			//输出名字
			CRect rcName(29,i*22+250,136,i*22+272);
			dc.DrawText(m_ScoreViewInfo.szUserName[i],lstrlen(m_ScoreViewInfo.szUserName[i]),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//输出成绩
			CRect rcScore(136,i*22+250,249,i*22+272);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_ScoreViewInfo.lGameScore[i]);
			dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//用户状态
			if (m_ScoreViewInfo.wWinUser==i)
			{
				CRect rcStatus(249,i*22+250,315,i*22+272);
				dc.DrawText(TEXT("胡牌"),lstrlen(TEXT("胡牌")),&rcStatus,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
			else if (m_ScoreViewInfo.wProvideUser==i)
			{
				CRect rcStatus(249,i*22+250,315,i*22+272);
				dc.DrawText(TEXT("放炮"),lstrlen(TEXT("放炮")),&rcStatus,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
		}
	}

	//显示税收
	if (m_ScoreViewInfo.lGameTax!=0L)
	{
		//输出名字
		CRect rcName(29,i*22+250,136,i*22+272);
		dc.DrawText(TEXT("税收"),lstrlen(TEXT("税收")),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//输出成绩
		CRect rcScore(136,i*22+250,249,i*22+272);
		_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_ScoreViewInfo.lGameTax);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//胡牌组合
	CWeaveCard WeaveCard;
	CGameLogic GameLogic;
	WeaveCard.SetDirection(Direction_South);
	for (BYTE i=0;i<m_ScoreViewInfo.HuCardInfo.cbWeaveCount;i++)
	{
		//信息收集
		dc.SetTextColor(RGB(10,10,10));
		CRect rcHuXi(33+i*42,195,59+i*42,211);
		CRect rcWeaveKind(33+i*42,61,59+i*42,79);

		//绘画类型
		LPCTSTR pszWeaveKind=NULL;
		switch (m_ScoreViewInfo.HuCardInfo.WeaveItemArray[i].cbWeaveKind)
		{
		case ACK_TI:	{ pszWeaveKind=TEXT("提"); break; }
		case ACK_PAO:	{ pszWeaveKind=TEXT("跑"); break; }
		case ACK_WEI:	{ pszWeaveKind=TEXT("偎"); break; }
		case ACK_CHI:	{ pszWeaveKind=TEXT("吃"); break; }
		case ACK_PENG:	{ pszWeaveKind=TEXT("碰"); break; }
		}
		dc.DrawText(pszWeaveKind,lstrlen(pszWeaveKind),&rcWeaveKind,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//绘画胡息
		_itot(GameLogic.GetWeaveHuXi(m_ScoreViewInfo.HuCardInfo.WeaveItemArray[i]),szBuffer,10);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcHuXi,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//设置组合
		WeaveCard.SetControlPoint(36+i*42,82);
		WeaveCard.SetCardData(m_ScoreViewInfo.HuCardInfo.WeaveItemArray[i]);

		//绘画组合
		WeaveCard.DrawCardControl(&dc);
	}

	//绘画吊牌
	if (m_ScoreViewInfo.HuCardInfo.cbCardEye!=0)
	{
		//信息收集
		dc.SetTextColor(RGB(10,10,10));
		CRect rcHuXi(33+i*42,195,59+i*42,211);
		CRect rcWeaveKind(33+i*42,61,59+i*42,79);

		//绘画类型
		dc.DrawText(TEXT("吊"),lstrlen(TEXT("吊")),&rcWeaveKind,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//绘画组合
		g_CardResource.m_ImageTableBottom.DrawCardItem(&dc,m_ScoreViewInfo.HuCardInfo.cbCardEye,36+i*42,82+CARD_SPACE);
		g_CardResource.m_ImageTableBottom.DrawCardItem(&dc,m_ScoreViewInfo.HuCardInfo.cbCardEye,36+i*42,82+CARD_SPACE*2);
	}

	//总胡息
	CRect rcHuXi(192,30,220,44);
	dc.SetTextColor(RGB(250,250,250));
	_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_ScoreViewInfo.HuCardInfo.cbHuXiCount);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcHuXi,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//重置积分
void CScoreView::ResetScore()
{
	//设置变量
	ZeroMemory(&m_ScoreViewInfo,sizeof(m_ScoreViewInfo));

	//绘画视图
	if (m_hWnd!=NULL) Invalidate(NULL);
	
	return;
}

//设置信息
void CScoreView::SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo, bool bShowWnd)
{
	//设置变量
	m_ScoreViewInfo=ScoreViewInfo;

	//窗口设置
	if (m_hWnd!=NULL)
	{
		Invalidate(NULL);
		ShowWindow((bShowWnd==true)?SW_SHOW:SW_HIDE);
	}
	
	return;
}

//////////////////////////////////////////////////////////////////////////
