#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameWnd.h"

//////////////////////////////////////////////////////////////////////////////////

//控制按钮
#define IDC_MIN						100									//最小按钮
#define IDC_MAX						101									//最大按钮
#define IDC_CLOSE					102									//关闭按钮

//控件标识
#define IDC_SKIN_SPLITTER			200									//拆分控件
#define IDC_GAME_CLIENT_VIEW		201									//视图标识

//屏幕位置
#define BORAD_SIZE					3									//边框大小
#define CAPTION_SIZE				32									//标题大小

//控件大小
#define SPLITTER_CX					0									//拆分宽度
#define CAPTION_SIZE				32									//标题大小

//屏幕限制
#define LESS_SCREEN_CY				740									//最小高度
#define LESS_SCREEN_CX				1024								//最小宽度

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameWnd, CFrameWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//自定消息
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CFrameEncircle::CFrameEncircle()
{
}

//析构函数
CFrameEncircle::~CFrameEncircle()
{
}

//绘画处理
bool CFrameEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//加载图标
	CBitImage ImageLogo;
	ImageLogo.LoadFromResource(AfxGetInstanceHandle(),TEXT("GAME_LOGO"));

	//绘画图标
	if (ImageLogo.IsNull()==false)
	{
		//变量定义
		CDC * pDC=CDC::FromHandle(EncircleImage.ImageTL.GetDC());

		//绘画图标
		ImageLogo.TransDrawImage(pDC,10,4,RGB(255,0,255));

		//释放资源
		EncircleImage.ImageTL.ReleaseDC();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameWnd::CGameFrameWnd()
{
	//滚动变量
	m_nScrollXPos=0L;
	m_nScrollYPos=0L;
	m_nScrollXMax=0L;
	m_nScrollYMax=0L;

	//状态变量
	m_bMaxShow=false;
	m_bShowControl=true;
	m_rcNormalSize.SetRect(0,0,0,0);

	//组件接口
	m_pIClientKernel=NULL;
	m_pIGameFrameView=NULL;

	//框架环绕
	tagEncircleResource	EncircleFrame;
	EncircleFrame.pszImageTL=MAKEINTRESOURCE(IDB_FRAME_TL);
	EncircleFrame.pszImageTM=MAKEINTRESOURCE(IDB_FRAME_TM);
	EncircleFrame.pszImageTR=MAKEINTRESOURCE(IDB_FRAME_TR);
	EncircleFrame.pszImageML=MAKEINTRESOURCE(IDB_FRAME_ML);
	EncircleFrame.pszImageMR=MAKEINTRESOURCE(IDB_FRAME_MR);
	EncircleFrame.pszImageBL=MAKEINTRESOURCE(IDB_FRAME_BL);
	EncircleFrame.pszImageBM=MAKEINTRESOURCE(IDB_FRAME_BM);
	EncircleFrame.pszImageBR=MAKEINTRESOURCE(IDB_FRAME_BR);
	m_FrameEncircle.InitEncircleResource(EncircleFrame,GetModuleHandle(GAME_FRAME_DLL_NAME));

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_WND,QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//析构函数
CGameFrameWnd::~CGameFrameWnd()
{
}

//接口查询
VOID * CGameFrameWnd::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameWnd,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameWnd,Guid,dwQueryVer);
	return NULL;
}

//消息解释
BOOL CGameFrameWnd::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CGameFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//功能按钮
	switch (nCommandID)
	{
	case IDC_MIN:				//最小按钮
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_MAX:				//最大按钮
		{
			//窗口控制
			if (m_bMaxShow==true)
			{
				RestoreWindow();
			}
			else
			{
				MaxSizeWindow();
			}

			//更新界面
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return TRUE;
		}
	case IDC_CLOSE:				//关闭按钮
		{
			//获取用户
			ASSERT(m_pIClientKernel!=NULL);
			IClientUserItem * pIClientUserItem=m_pIClientKernel->GetMeUserItem();

			//强退提示
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem->GetUserStatus()==US_PLAYING))
			{
				//提示消息
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("您正在游戏中，强行退出将被扣分，确实要强退吗？"),MB_ICONQUESTION|MB_YESNO,0)!=IDYES)
				{
					return TRUE;
				}
			}

			//投递关闭
			PostMessage(WM_CLOSE,0,0);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//还原窗口
bool CGameFrameWnd::RestoreWindow()
{
	//状态判断
	if (m_bMaxShow==true)
	{
		//设置变量
		m_bMaxShow=false;

		//设置按钮
		m_btMax.SetButtonImage(IDB_BT_MAX,GetModuleHandle(GAME_FRAME_DLL_NAME),false,false);

		//移动窗口
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//最大窗口
bool CGameFrameWnd::MaxSizeWindow()
{
	//状态判断
	if (m_bMaxShow==false)
	{
		//设置变量
		m_bMaxShow=true;

		//默认位置
		GetWindowRect(&m_rcNormalSize);

		//设置按钮
		m_btMax.SetButtonImage(IDB_BT_RESORE,GetModuleHandle(GAME_FRAME_DLL_NAME),false,false);

		//获取位置
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//移动窗口
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//调整控件
VOID CGameFrameWnd::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if ((nWidth==0)||(nHeight==0)) return;
	if ((m_nScrollXMax==0)||(m_nScrollYMax==0)) return;

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//环绕信息
	tagEncircleInfo EncircleInfoFrame;
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);

	//变量定义
	CRect rcSplitter;
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=m_nScrollYMax-EncircleInfoFrame.nBBorder;

	//拆分控件
	if (m_bShowControl==true)
	{
		rcSplitter.left=m_nScrollXMax*75L/100L;
		rcSplitter.right=rcSplitter.left+SPLITTER_CX;
	}
	else
	{
		rcSplitter.left=m_nScrollXMax-EncircleInfoFrame.nRBorder-SPLITTER_CX;
		rcSplitter.right=m_nScrollXMax-EncircleInfoFrame.nRBorder;
	}

	//查询游戏
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	IGameFrameView * pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//游戏视图
	if (pIGameFrameView!=NULL)
	{
		HWND hWndView=pIGameFrameView->GetGameViewHwnd();
		DeferWindowPos(hDwp,hWndView,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcSplitter.left-EncircleInfoFrame.nLBorder,
			m_nScrollYMax-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder,uFlags);
	}

	//控制按钮
	DeferWindowPos(hDwp,m_btMin,NULL,m_nScrollXMax-87,4,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btMax,NULL,m_nScrollXMax-59,4,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,m_nScrollXMax-31,4,0,0,uFlags|SWP_NOSIZE);

	//移动控件
	DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_GameFrameControl,NULL,rcSplitter.right,EncircleInfoFrame.nTBorder,m_nScrollXMax-rcSplitter.right-EncircleInfoFrame.nRBorder,
		m_nScrollYMax-EncircleInfoFrame.nBBorder-EncircleInfoFrame.nTBorder,uFlags);

	//移动结束
	EndDeferWindowPos(hDwp);

	return;
}

//绘画背景
BOOL CGameFrameWnd::OnEraseBkgnd(CDC * pDC)
{
	//获取标题
	CString strTitle;
	GetWindowText(strTitle);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//框架位置
	tagEncircleInfo FrameEncircleInfo;
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);

	//绘画框架
	m_FrameEncircle.DrawEncircleFrame(pDC,rcClient);

	//绘画标题
	if (strTitle.IsEmpty()==false)
	{
		//计算区域
		CRect rcTitle;
		rcTitle.SetRect(35,8,rcClient.Width()-150,FrameEncircleInfo.nTBorder-8);

		//变量定义
		INT nXMove[8]={1,1,1,0,-1,-1,-1,0};
		INT nYMove[8]={-1,0,1,1,1,0,-1,-1};

		//设置环境
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(100,100,100));
		pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//绘画边框
		for (INT i=0;i<CountArray(nXMove);i++)
		{
			//计算位置
			CRect rcTitleFrame;
			rcTitleFrame.top=rcTitle.top+nYMove[i];
			rcTitleFrame.left=rcTitle.left+nXMove[i];
			rcTitleFrame.right=rcTitle.right+nXMove[i];
			rcTitleFrame.bottom=rcTitle.bottom+nYMove[i];

			//绘画字符
			pDC->DrawText(strTitle,&rcTitleFrame,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}

		//绘画文字
		pDC->SetTextColor(RGB(255,255,255));
		pDC->DrawText(strTitle,&rcTitle,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}


	return TRUE;
}

//按键测试
UINT CGameFrameWnd::OnNcHitTest(CPoint Point)
{
	//按钮测试
	if (m_bMaxShow==false)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//转换位置
		CPoint ClientPoint=Point;
		ScreenToClient(&ClientPoint);

		//左面位置
		if (ClientPoint.x<=BORAD_SIZE)
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPLEFT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMLEFT;
			return HTLEFT;
		}

		//右面位置
		if (ClientPoint.x>=(rcClient.Width()-BORAD_SIZE))
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPRIGHT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMRIGHT;
			return HTRIGHT;
		}

		//上下位置
		if (ClientPoint.y<=BORAD_SIZE) return HTTOP;
		if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOM;
	}

	return __super::OnNcHitTest(Point);
}

//位置消息
VOID CGameFrameWnd::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//滚动数据
	m_nScrollXMax=__max(LESS_SCREEN_CX,cx);
	m_nScrollYMax=__max(LESS_SCREEN_CY,cy);
	m_nScrollXPos=__min(0L,m_nScrollXMax-cy);
	m_nScrollYPos=__min(0L,m_nScrollYMax-cy);
	
	//变量定义
	SCROLLINFO ScrollInfoVert;
	SCROLLINFO ScrollInfoHorz;
	ZeroMemory(&ScrollInfoVert,sizeof(ScrollInfoVert));
	ZeroMemory(&ScrollInfoHorz,sizeof(ScrollInfoHorz));

	//设置滚动
	ScrollInfoVert.cbSize=sizeof(ScrollInfoVert);
	ScrollInfoVert.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoVert.nMin=0;
	ScrollInfoVert.nMax=m_nScrollYMax;
	ScrollInfoVert.nPage=cy;
	ScrollInfoVert.nPos=m_nScrollYPos;

	//设置滚动
	ScrollInfoHorz.cbSize=sizeof(ScrollInfoHorz);
	ScrollInfoHorz.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoHorz.nMin=0;
	ScrollInfoHorz.nMax=m_nScrollXMax;
	ScrollInfoHorz.nPage=cx;
	ScrollInfoHorz.nPos=m_nScrollXPos;

	//设置滚动
	//SetScrollInfo(SB_VERT,&ScrollInfoVert,FALSE);
	//SetScrollInfo(SB_HORZ,&ScrollInfoHorz,FALSE);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//位置信息
VOID CGameFrameWnd::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//设置位置
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//建立消息
INT CGameFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置窗口
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//查询接口
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btMax.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MAX);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);

	//设置按钮
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,false,false);
	m_btMax.SetButtonImage(IDB_BT_MAX,hInstance,false,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,TEXT("BT_CLOSE"),hInstance,false,false);

	//控制窗口
	AfxSetResourceHandle(hInstance);
	m_GameFrameControl.Create(IDD_GAME_CONTROL,this);
	m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);

	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("最小化"));
	m_ToolTipCtrl.AddTool(&m_btMax,TEXT("最大化"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("关闭游戏"));

	//设置位置
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//读取位置
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//位置调整
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//移动窗口
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	//创建视图
	ASSERT(m_pIGameFrameView!=NULL);
	if (m_pIGameFrameView!=NULL) m_pIGameFrameView->CreateGameViewWnd(this,IDC_GAME_CLIENT_VIEW);

	//显示窗口
	MaxSizeWindow();

	//设置滚动
	m_SkinScrollBar.InitScroolBar(this);

	return 0L;
}

//鼠标消息
VOID CGameFrameWnd::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//鼠标消息
VOID CGameFrameWnd::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//状态判断
	if (Point.y>CAPTION_SIZE) return;

	//窗口控制
	if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}
	
	return;
}

//设置改变
VOID CGameFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//获取大小
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);
	ClientToScreen(&rcClient);

	//计算位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,0);

	//调整位置
	rcArce.top-=(rcClient.top-rcWindow.top);
	rcArce.left-=(rcClient.left-rcWindow.left);
	rcArce.right+=(rcWindow.right-rcClient.right);
	rcArce.bottom+=(rcWindow.bottom-rcClient.bottom);

	//移动窗口
	SetWindowPos(NULL,rcArce.left,rcArce.top,rcArce.Width(),rcArce.Height(),SWP_NOZORDER|SWP_NOCOPYBITS);

	return;
}

//标题消息
LRESULT	CGameFrameWnd::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//默认调用
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//更新标题
	Invalidate(TRUE);

	return lResult;
}

//////////////////////////////////////////////////////////////////////////////////
