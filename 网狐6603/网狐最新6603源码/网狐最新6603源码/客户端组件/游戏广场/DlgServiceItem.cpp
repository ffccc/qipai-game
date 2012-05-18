#include "StdAfx.h"
#include "Resource.h"
#include "DlgServiceItem.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_WEB_PUBLICIZE			100									//浏览控件

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgServiceItem, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgServiceBrowser, CDlgServiceItem)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgServiceItem::CDlgServiceItem(UINT uIDTemplate) : CDialog(uIDTemplate)
{
	//创建资源
	m_cbImageIndex=0;

	return;
}

//析构函数
CDlgServiceItem::~CDlgServiceItem()
{
}

//绘画标题
VOID CDlgServiceItem::DrawItemView(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//创建窗口
VOID CDlgServiceItem::CreateServiceItem(CWnd * pParentWnd, CRect rcCreate)
{
	//创建窗口
	Create(m_lpszTemplateName,pParentWnd);

	//移动窗口
	SetWindowPos(NULL,rcCreate.left,rcCreate.top,rcCreate.Width(),rcCreate.Height(),SWP_NOZORDER|SWP_SHOWWINDOW);

	return;
}

//获取字符
VOID CDlgServiceItem::GetControlItemText(UINT uID, TCHAR szString[], WORD wMaxCount)
{
	//获取字符
	CString strString;
	GetDlgItemText(uID,strString);

	//调整字符
	strString.TrimLeft();
	strString.TrimRight();

	//设置结果
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//绘画背景
BOOL CDlgServiceItem::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//填充背景
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);

	return TRUE;
}

//颜色消息
HBRUSH CDlgServiceItem::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlText);
			return CSkinDialog::m_SkinAttribute.m_brBackGround;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgServiceBrowser::CDlgServiceBrowser() : CDlgServiceItem(IDD_DLG_SERVICE_BROWSER)
{
	//设置变量
	ZeroMemory(m_szItemName,sizeof(m_szItemName));
	ZeroMemory(m_szServiceUrl,sizeof(m_szServiceUrl));

	return;
}

//析构函数
CDlgServiceBrowser::~CDlgServiceBrowser()
{
}

//创建函数
BOOL CDlgServiceBrowser::OnInitDialog()
{
	__super::OnInitDialog();

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建控件
	m_ServicePublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcClient,this,IDC_WEB_PUBLICIZE);

	//连接页面
	if (m_szServiceUrl[0]!=0)
	{
		//构造地址
		TCHAR szBrowseUrl[256]=TEXT("");
		_sntprintf(szBrowseUrl,CountArray(szBrowseUrl),TEXT("%s/%s"),szPlatformLink,m_szServiceUrl);

		//连接地址
		m_ServicePublicize.Navigate(szBrowseUrl,NULL,NULL,NULL,NULL);
	}

	return TRUE;
}

//设置服务
VOID CDlgServiceBrowser::InitService(BYTE cbImageIndex, LPCTSTR pszItemName, LPCTSTR pszServiceUrl)
{
	//设置变量
	m_cbImageIndex=cbImageIndex;
	lstrcpyn(m_szItemName,pszItemName,CountArray(m_szItemName));
	lstrcpyn(m_szServiceUrl,pszServiceUrl,CountArray(m_szServiceUrl));

	return;
}

//位置消息
VOID CDlgServiceBrowser::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//移动控件
	if (m_ServicePublicize.m_hWnd!=NULL)
	{
		m_ServicePublicize.SetWindowPos(NULL,0,0,rcClient.Width(),rcClient.Height(),SWP_NOZORDER);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
