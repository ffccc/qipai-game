#include "StdAfx.h"
#include "Resource.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_CLOSE_TIME				100									//退出时间

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CONTINUE, OnContinue)
	ON_BN_CLICKED(IDC_QUIT_GAME, OnQuitGame)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CScoreView::CScoreView() : CDialog(IDD_GAME_SCORE)
{
	//状态变量
	m_uLeaveTime=15;

	//设置数据
	ZeroMemory(m_lRevenue,sizeof(m_lRevenue));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//加载资源
	m_ImageBack.SetLoadInfo(IDB_GAME_SCORE,AfxGetInstanceHandle());
}

//析构函数
CScoreView::~CScoreView()
{
}

//控件绑定
VOID CScoreView::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTINUE, m_btContinue);
	DDX_Control(pDX, IDC_QUIT_GAME, m_btQuitGame);
}

//初始化函数
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//状态变量
	m_uLeaveTime=15;

	//设置界面
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//设置按钮
	m_btContinue.SetTextColor(RGB(255,255,255));
	m_btQuitGame.SetTextColor(RGB(255,255,255));
	m_btContinue.SetButtonImage(IDB_BT_GAME_SCORE,AfxGetInstanceHandle(),false);
	m_btQuitGame.SetButtonImage(IDB_BT_GAME_SCORE,AfxGetInstanceHandle(),false);

	//居中窗口
	CenterWindow(GetParent());

	//移动窗口
	CImageHandle HandleBack(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//移动按钮
	CRect rcButton;
	m_btContinue.GetWindowRect(&rcButton);
	m_btQuitGame.SetWindowPos(NULL,m_ImageBack.GetWidth()/2+15,m_ImageBack.GetHeight()-rcButton.Height()-8,
		0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btContinue.SetWindowPos(NULL,m_ImageBack.GetWidth()/2-rcButton.Width()-15,m_ImageBack.GetHeight()-rcButton.Height()-8,
		0,0,SWP_NOSIZE|SWP_NOZORDER);

	return TRUE;
}

//重画函数
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	dc.FillSolidRect(&rcClient,RGB(255,255,255));

	//设置 DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(10,10,10));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//绘画背景
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//绘画标题
	LPCTSTR pszAccount=TEXT("用户名"),pszScore=TEXT("成绩");
	dc.TextOut(30,21,pszAccount,lstrlen(pszAccount));
	dc.TextOut(140,21,pszScore,lstrlen(pszScore));

	//显示分数
	WORD wStation=0;
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			dc.TextOut(30,(int)(wStation*20+43),m_szUserName[i]);
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lGameScore[i]);
			dc.TextOut(140,(int)(wStation*20+43),szBuffer,lstrlen(szBuffer));
			wStation++;
		}
	}

	return;
}

//继续按钮
void CScoreView::OnContinue()
{
	//隐藏窗口
	DestroyWindow();

	//发送消息
	AfxGetMainWnd()->PostMessage(IDM_CONTINUE,0,0);

	return;
}

//离开按钮
void CScoreView::OnQuitGame()
{
	//隐藏窗口
	DestroyWindow();

	//发送消息
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	return;
}

//时间消息
void CScoreView::OnTimer(UINT nIDEvent)
{
	//时间消息
	if (nIDEvent==IDI_CLOSE_TIME)
	{
		if (m_uLeaveTime>0)
		{
			//构造时间
			TCHAR szBuffer[32]=TEXT("");
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("继续 %d"),m_uLeaveTime--);

			//设置字符
			m_btContinue.SetWindowText(szBuffer);
		}
		else OnQuitGame();
	}

	__super::OnTimer(nIDEvent);
}

//绘画背景
BOOL CScoreView::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//鼠标消息
void CScoreView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//重置积分
void CScoreView::ResetScore()
{
	//设置数据
	ZeroMemory(m_lRevenue,sizeof(m_lRevenue));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//关闭窗口
	if (m_hWnd!=NULL) DestroyWindow();
	
	return;
}

//显示成绩
void CScoreView::ShowScore()
{
	//设置时间
	SetTimer(IDI_CLOSE_TIME,1000,NULL);

	//显示窗口
	ShowWindow(SW_SHOW);

	return;
}

//设置积分
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lGameScore, LONG lRevenue)
{
	if (wChairID<CountArray(m_lGameScore))
	{
		//设置变量
		m_lRevenue[wChairID]=lRevenue;
		m_lGameScore[wChairID]=lGameScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));

		//更新界面
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
