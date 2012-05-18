#include "StdAfx.h"
#include "CardControl.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameScoreWnd::CGameScoreWnd()
{
	//设置变量
	ZeroMemory(&m_GameScoreInfo,sizeof(m_GameScoreInfo));
	m_ImageBack.SetLoadInfo(IDB_GAME_SCORE,AfxGetInstanceHandle());
	m_lTax=0;
	return;
}

//析构函数
CGameScoreWnd::~CGameScoreWnd()
{
}

//建立消息
int CGameScoreWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//移动窗口
	CImageHandle BackImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOREPOSITION);

	return TRUE;
}

//重画函数
void CGameScoreWnd::OnPaint()
{
	CPaintDC dc(this);
	
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.FillSolidRect(rcClient,RGB(24,60,104));
	//绘画背景
	CImageHandle BackImageHandle(&m_ImageBack);
	//m_ImageBack.BitBlt(BufferDC,0,0);
	m_ImageBack.AlphaDrawImage(&BufferDC,0,0,RGB(255,0,255));
	//设置 DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(RGB(255,255,255));
	BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());

	//绘画扑克
	if ((m_GameScoreInfo.cbChiHuCard & 0xF0) != 0x10)
	{
		g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_GameScoreInfo.cbChiHuCard,80,280);
	}
	else
	{
		g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_GameScoreInfo.cbChiHuCard,80,280);
	}
	//贴主金
	g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_GameScoreInfo.cbGoldCard,210,280);

	/////////////////////////////////
	//先统计分数为0的用户的个数，可依据其数值而区分胡牌的情况
	BYTE cbCount=0;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_GameScoreInfo.lScore[i]==0)
		{
			cbCount++;
		}
	}
	CFont newFont,*oldFont;
	newFont.CreatePointFont(125,"楷体_GB2312",&BufferDC);
	oldFont=BufferDC.SelectObject(&newFont);
	BufferDC.SetTextColor(RGB(253,228,139));
	BufferDC.SetBkMode(TRANSPARENT);

	CRect rcDraw;
	TCHAR strMessage[32];
	int xPoint[4]={15,115,190,250};
	//打印第一行中的用户名信息
	rcDraw.left=xPoint[0];  rcDraw.right=xPoint[1];
	rcDraw.top=50; 	 rcDraw.bottom=80;
	//游戏正常结束时打第一行中打印胡牌者姓名
	if(cbCount<2)
	{
		lstrcpyn(strMessage,m_GameScoreInfo.szUserName[m_GameScoreInfo.wWinner],CountArray(strMessage));
		BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}
	//有人退出时打印退出者姓名
	else
	{
		BYTE OutID=255;//退出者号
		for(BYTE i=0;i<GAME_PLAYER;i++)
		{
			if(m_GameScoreInfo.lScore[i]!=0)
			{
				lstrcpyn(strMessage,m_GameScoreInfo.szUserName[i],CountArray(strMessage));
				BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
		}
	}
	//打印第一行中的结束信息：退出、自摸、放冲
	rcDraw.left=xPoint[1];  rcDraw.right=xPoint[2];
	switch(cbCount)
	{
	case 0:	
		_snprintf(strMessage,sizeof(strMessage),TEXT("自摸")); break;
	case 1:
		_snprintf(strMessage,sizeof(strMessage),TEXT("胡牌")); break;
	case 2:
		_snprintf(strMessage,sizeof(strMessage),TEXT("退出")); break;
	}
	BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	
	//打印第一行中的点数信息
	rcDraw.left=xPoint[2];  rcDraw.right=xPoint[3];
	sprintf(strMessage,"点数 %d",m_GameScoreInfo.lHuPoint);
	BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//打印三个用户的信息的信息
	int xPoint1[5]={15,115,190,250,365};
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		//打印用户名字
		rcDraw.left=xPoint[0];
		rcDraw.right=xPoint[1];
		rcDraw.top=80+i*25;
		rcDraw.bottom=rcDraw.top+25;
		BufferDC.DrawText(m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//用户积分
		rcDraw.left=xPoint1[1];
		rcDraw.right=xPoint1[2];
		_snprintf(strMessage,sizeof(strMessage),TEXT("%ld"),m_GameScoreInfo.lScore[i]);
		BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		//胡牌家
		if(m_GameScoreInfo.wWinner!=m_GameScoreInfo.wProvideUser && m_GameScoreInfo.wWinner==i)
		{
			rcDraw.left=xPoint1[2];
			rcDraw.right=xPoint1[3];
			_snprintf(strMessage,sizeof(strMessage),TEXT("胡牌"));
			BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		//自摸
		if(m_GameScoreInfo.wWinner==m_GameScoreInfo.wProvideUser && m_GameScoreInfo.wWinner==i)
		{
			rcDraw.left=xPoint1[2];
			rcDraw.right=xPoint1[3];
			_snprintf(strMessage,sizeof(strMessage),TEXT("自摸"));
			BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		//点炮
		if(m_GameScoreInfo.wProvideUser==i && m_GameScoreInfo.wWinner!=i)
		{
			rcDraw.left=xPoint1[2];
			rcDraw.right=xPoint1[3];
			_snprintf(strMessage,sizeof(strMessage),TEXT("放冲"));
			BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		//庄
		if(m_GameScoreInfo.wBankerUser==i)
		{
			rcDraw.left=xPoint1[3];
			rcDraw.right=xPoint1[4];
			_snprintf(strMessage,sizeof(strMessage),TEXT("庄"));
			BufferDC.DrawText(strMessage,lstrlen(strMessage),&rcDraw,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
	}

	if(cbCount<2)
	{
		//胡牌组合
		//定义画最佳组合的起始位置
		WORD xPosition = 50;
		WORD yPosition = 160;

		//绘画八个组合的扑克
		for( BYTE i=0; i<8; i++)
		{
			//根据扑克牌数据，绘画每组扑克里的扑克牌
			for (BYTE j=0; j<m_GameScoreInfo.HuCardWeave.cbCardCount[i]; j++)
			{
				//绘画扑克,普通牌
				if ((m_GameScoreInfo.HuCardWeave.cbCardData[i][j] & 0xF0) != 0x10)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_GameScoreInfo.HuCardWeave.cbCardData[i][j],xPosition,yPosition+j*12);
				}
				//画花牌
				else
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_GameScoreInfo.HuCardWeave.cbCardData[i][j],xPosition,yPosition+j*12);
				}
				//xPosition += 17;
			}
			xPosition += 25;

		}

		//画丫口值 
		for (BYTE i=0; i<2; i++)
		{
			//绘画扑克,普通牌
			if ((m_GameScoreInfo.HuCardWeave.cbYaKou[i] & 0xF0) != 0x10)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(&BufferDC,m_GameScoreInfo.HuCardWeave.cbYaKou[i],xPosition,yPosition+i*12);
			}
			//画花牌
			else
			{
				g_CardResource.m_ImageTableBottom1.DrawCardItem(&BufferDC,m_GameScoreInfo.HuCardWeave.cbYaKou[i],xPosition,yPosition+i*12);
			}
		}
	}
	BufferDC.SelectObject(oldFont);
	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//绘画背景
BOOL CGameScoreWnd::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//鼠标消息
void CGameScoreWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//消息模拟
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//设置积分
void CGameScoreWnd::SetScoreInfo(tagGameScoreInfo &GameScoreInfo)
{
	//设置变量
	m_GameScoreInfo=GameScoreInfo;

	//显示窗口
	Invalidate(NULL);
	ShowWindow(SW_SHOW);

	return;
}

//设置税收
void CGameScoreWnd::SetTax(LONG lTax)
{
	
	if (m_lTax!=lTax)
	{
		m_lTax=lTax;
		Invalidate(NULL);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
