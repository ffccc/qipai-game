#include "StdAfx.h"
#include "WebPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//进度定义
#define MAX_LOAD_INDEX				20									//最大索引
#define IDI_LOAD_TIMER				100									//加载时间

//常量定义
#define IDC_WEB_BROWSER				100									//窗口标识

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWebPublicize, CWnd)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CWebPublicize, CWnd)
	ON_EVENT(CWebPublicize, IDC_WEB_BROWSER, 259, OnDocumentCompleteWeb, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CWebPublicize, IDC_WEB_BROWSER, 271, OnNavigateErrorWeb, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CWebPublicize, IDC_WEB_BROWSER, 250, OnBeforeNavigate2Web, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWebPublicize::CWebPublicize()
{
	//设置变量
	m_wIndex=0;
	m_cbStatus=BROWSER_STATUS_LOAD;

	return;
}

//析构函数
CWebPublicize::~CWebPublicize()
{
}

//浏览函数
VOID CWebPublicize::Navigate(LPCTSTR pszURL)
{
	//创建窗口
	if (m_WebBrowser.m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		m_WebBrowser.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_WEB_BROWSER);
	}

	//浏览页面
	m_WebBrowser.Navigate(pszURL,NULL,NULL,NULL,NULL);

	return;
}

//浏览函数
VOID CWebPublicize::Navigate(LPCTSTR pszURL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers)
{
	//创建窗口
	if (m_WebBrowser.m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		m_WebBrowser.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_WEB_BROWSER);
	}

	//浏览页面
	m_WebBrowser.Navigate(pszURL,NULL,NULL,NULL,NULL);

	return;
}

//状态通知
VOID CWebPublicize::OnBrowserStatus(BYTE cbStatus)
{
	switch (cbStatus)
	{
	case BROWSER_STATUS_LOAD:		//加载状态
		{
			SetTimer(IDI_LOAD_TIMER,50,NULL);
			return;
		}
	case BROWSER_STATUS_FINISH:		//完成状态
		{
			KillTimer(IDI_LOAD_TIMER);
			return;
		}
	}

	return;
}

//位置函数
VOID CWebPublicize::GetWebBrowserRect(CRect&rcWebBrowser)
{
	//获取位置
	GetClientRect(&rcWebBrowser);

	return;
}

//绘画界面
VOID CWebPublicize::DrawPublicizeView(CDC * pDC, INT nWidth, INT nHeight, BYTE cbStatus)
{
	//绘画背景
	pDC->FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

	//加载资源
	CPngImage ImageLoadLine1;
	CPngImage ImageLoadLine2;
	ImageLoadLine1.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("WEB_LOAD_LINE_1"));
	ImageLoadLine2.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("WEB_LOAD_LINE_2"));

	//获取大小
	CSize SizeLoadLine1;
	CSize SizeLoadLine2;
	SizeLoadLine1.SetSize(ImageLoadLine1.GetWidth(),ImageLoadLine1.GetHeight());
	SizeLoadLine2.SetSize(ImageLoadLine2.GetWidth(),ImageLoadLine2.GetHeight());

	//绘画界面
	INT nYDrawPos=(nHeight-SizeLoadLine1.cy)/2;
	INT nXDrawPos=(nWidth-SizeLoadLine1.cx-20)/2;
	ImageLoadLine1.DrawImage(pDC,nXDrawPos,nYDrawPos);

	//位置计算
	INT nCompartPos=SizeLoadLine1.cx*m_wIndex/MAX_LOAD_INDEX;
	INT nLoadLineDraw=__min(SizeLoadLine2.cx,SizeLoadLine1.cx-nCompartPos);

	//绘画进度
	INT nYExcursion=(SizeLoadLine1.cy-SizeLoadLine2.cy)/2;
	ImageLoadLine2.DrawImage(pDC,nXDrawPos+nCompartPos,nYDrawPos+nYExcursion,nLoadLineDraw,SizeLoadLine1.cy,0,0);
	ImageLoadLine2.DrawImage(pDC,nXDrawPos,nYDrawPos+nYExcursion,(SizeLoadLine2.cx-nLoadLineDraw),SizeLoadLine1.cy,nLoadLineDraw,0);

	//创建字体
	CFont Font;
	Font.CreateFont(-12,0,0,0,800,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("宋体"));

	//设置设备
	pDC->SelectObject(Font);
	pDC->SetTextColor(RGB(30,30,30));

	//输出字体
	LPCTSTR pszString=TEXT("正在加载页面，请稍后...");
	pDC->TextOut(nXDrawPos,nYDrawPos+20,pszString,lstrlen(pszString));

	//还原设备
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	return;
}

//重画消息
VOID CWebPublicize::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//绘画缓冲
	CDC * pBufferDC=CDC::FromHandle(ImageBuffer.GetDC());
	DrawPublicizeView(pBufferDC,rcClient.Width(),rcClient.Height(),m_cbStatus);

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	//释放资源
	ImageBuffer.ReleaseDC();

	return;
}

//时间消息
VOID CWebPublicize::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);

	//时间处理
	if (nIDEvent==IDI_LOAD_TIMER)
	{
		//设置变量
		m_wIndex=((m_wIndex+1)>=MAX_LOAD_INDEX)?0:(m_wIndex+1);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		return;
	}

	return;
}

//位置消息
VOID CWebPublicize::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//移动窗口
	if (m_WebBrowser.m_hWnd!=NULL)
	{
		if (m_cbStatus==BROWSER_STATUS_FINISH)
		{
			//获取位置
			CRect rcWebBrowser;
			GetWebBrowserRect(rcWebBrowser);

			//移动位置
			m_WebBrowser.SetWindowPos(NULL,rcWebBrowser.left,rcWebBrowser.top,rcWebBrowser.Width(),rcWebBrowser.Height(),SWP_NOZORDER);
		}
		else
		{
			//移动位置
			m_WebBrowser.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER);
		}
	}

	return;
}

//导航完成
VOID CWebPublicize::OnDocumentCompleteWeb(LPDISPATCH pDisp, VARIANT FAR * URL)
{
	//设置变量
	if (m_cbStatus!=BROWSER_STATUS_ERROR)
	{
		//设置变量
		m_cbStatus=BROWSER_STATUS_FINISH;

		//状态通知
		OnBrowserStatus(BROWSER_STATUS_FINISH);

		//移动窗口
		if (m_WebBrowser.m_hWnd!=NULL)
		{
			//获取位置
			CRect rcWebBrowser;
			GetWebBrowserRect(rcWebBrowser);

			//移动位置
			m_WebBrowser.SetWindowPos(NULL,rcWebBrowser.left,rcWebBrowser.top,rcWebBrowser.Width(),rcWebBrowser.Height(),SWP_NOZORDER);
		}
	}

	return;
}

//导航错误
VOID CWebPublicize::OnNavigateErrorWeb(LPDISPATCH pDisp, VARIANT FAR * URL, VARIANT FAR * Frame, VARIANT FAR * StatusCode, BOOL FAR * Cancel)
{
	//设置变量
	m_cbStatus=BROWSER_STATUS_ERROR;

	//状态通知
	OnBrowserStatus(BROWSER_STATUS_ERROR);

	//移动窗口
	if (m_WebBrowser.m_hWnd!=NULL) m_WebBrowser.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER);

	return;
}

//导航开始
VOID CWebPublicize::OnBeforeNavigate2Web(LPDISPATCH pDisp, VARIANT FAR * URL, VARIANT FAR * Flags, VARIANT FAR * TargetFrameName, VARIANT FAR * PostData, VARIANT FAR * Headers, BOOL FAR * Cancel)
{
	//设置变量
	m_cbStatus=BROWSER_STATUS_LOAD;

	//状态通知
	OnBrowserStatus(BROWSER_STATUS_LOAD);

	//移动窗口
	if (m_WebBrowser.m_hWnd!=NULL) m_WebBrowser.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
