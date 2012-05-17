#include "StdAfx.h"
#include "Resource.h"
#include "DlgBrowser.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_BROWSER					100									//浏览控件

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgBrowser, CSkinDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgBrowser::CDlgBrowser() : CSkinDialog(IDD_BROWSER)
{
	//设置变量
	m_SizeWindow.SetSize(0,0);
	
	return;
}

//析构函数
CDlgBrowser::~CDlgBrowser()
{
}

//消息解释
BOOL CDlgBrowser::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgBrowser::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(m_strWindowTitle);

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_WebPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_BROWSER);

	//调整大小
	if ((m_SizeWindow.cx>0)&&(m_SizeWindow.cy>0))
	{
		SetWindowPos(NULL,0,0,m_SizeWindow.cx,m_SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE);
	}

	//居中窗口
	CenterWindow();

	//调整控件
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//构造地址
	TCHAR szBrowser[256]=TEXT("");
	_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/%s"),szPlatformLink,m_strBrowserURL);

	//连接页面
	m_WebPublicize.Navigate(szBrowser,NULL,NULL,NULL,NULL);

	return FALSE;
}

//确定函数
VOID CDlgBrowser::OnOK()
{
	return;
}

//调整控件
VOID CDlgBrowser::RectifyControl(INT nWidth, INT nHeight)
{
	//调整位置
	if (m_WebPublicize.m_hWnd!=NULL)
	{
		//框架位置
		INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
		INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
		INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
		INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

		//移动位置
		m_WebPublicize.SetWindowPos(NULL,nLBorder,nTBorder,nWidth-nRBorder-nLBorder,nHeight-nBBorder-nTBorder,SWP_NOZORDER);
	}

	return;
}

//创建窗口
VOID CDlgBrowser::CreateBrowserControl(LPCTSTR pszTitle, LPCTSTR pszBrowserURL, CSize SizeWindow)
{
	//设置变量
	m_SizeWindow=SizeWindow;
	m_strWindowTitle=pszTitle;
	m_strBrowserURL=pszBrowserURL;

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(SHARE_CONTROL_DLL_NAME));

	//显示窗口
	DoModal();

	return;
}

//位置消息
VOID CDlgBrowser::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
