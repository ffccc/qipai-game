// TransparentCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "../消息定义/GameLogic.h"
#include "TransparentCtrl.h"


// CTransparentCtrl

IMPLEMENT_DYNAMIC(CTransparentCtrl, CStatic)

CTransparentCtrl::CTransparentCtrl()
{
	m_csText = _T("");
	m_iXPos = 0;
}

CTransparentCtrl::~CTransparentCtrl()
{
}

BEGIN_MESSAGE_MAP(CTransparentCtrl, CStatic)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CTransparentCtrl 消息处理程序

HBRUSH CTransparentCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	SetBkMode(pDC->GetSafeHdc(), TRANSPARENT);        //设置背景透明
	HBRUSH hbr =  (HBRUSH)GetStockObject(NULL_BRUSH); //CStatic::OnCtlColor(pDC, pWnd, nCtlColor);//	
	return hbr;
}

// 绘制背景
void CTransparentCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (NULL == GetSafeHwnd())
	{
		return ;
	}
	if (m_PngBack.IsNull())
	{
		if (!m_csText.IsEmpty())
		{
			SetWindowText(m_csText);
		}
		CStatic::OnPaint();
		return ;
	}

	CRect rect;
	GetClientRect(rect);
	m_PngBack.DrawImage(&dc,0,0,rect.Width(), rect.Height(),m_iXPos,0,rect.Width(), rect.Height());

	if (m_csText.IsEmpty())
	{
		GetWindowText(m_csText);
	}

	// Get the font
	CFont Font, *pOldFont;
	Font.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,"宋体"); 
	pOldFont = dc.SelectObject(&Font);

	DWORD dwStyle = GetStyle(), dwText = 0;


	// Map "Static Styles" to "Text Styles"
#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwText |= (dest)
#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwText |= (dest)

	MAP_STYLE(	SS_RIGHT,			DT_RIGHT					);
	MAP_STYLE(	SS_CENTER,			DT_CENTER					);
	MAP_STYLE(	SS_CENTERIMAGE,		DT_VCENTER|DT_SINGLELINE);
	MAP_STYLE(	SS_CENTERIMAGE,		DT_VCENTER);
	MAP_STYLE(	SS_NOPREFIX,		DT_NOPREFIX					);
	MAP_STYLE(	SS_WORDELLIPSIS,	DT_WORD_ELLIPSIS			);
	MAP_STYLE(	SS_ENDELLIPSIS,		DT_END_ELLIPSIS				);
	MAP_STYLE(	SS_PATHELLIPSIS,	DT_PATH_ELLIPSIS			);

	NMAP_STYLE(	SS_LEFTNOWORDWRAP |
		SS_CENTERIMAGE |
		SS_WORDELLIPSIS |
		SS_ENDELLIPSIS |
		SS_PATHELLIPSIS,	DT_WORDBREAK);

	// Set transparent background
	dc.SetBkMode(TRANSPARENT);

	CRect rectTemp = rect;
	int iHeightSingle = dc.DrawText(_T("Test Line"), rectTemp, DT_CALCRECT|DT_EXTERNALLEADING);
	rectTemp = rect;
	int iHeightMuti = dc.DrawText(m_csText, rectTemp, DT_CALCRECT|DT_WORDBREAK|DT_EXTERNALLEADING);
	if (iHeightMuti>iHeightSingle)
	{
		dwText &= ~DT_SINGLELINE;
		rect.top = (rect.Height()-iHeightMuti)/2;
	}
	rect.left += 10;
	dc.DrawText(m_csText, rect, dwText);

	// Select old font
	dc.SelectObject(pOldFont);	
}

BOOL CTransparentCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_TRANSPARENT;

	return CStatic::PreCreateWindow(cs);
}

void CTransparentCtrl::SetWindowText(LPCTSTR lpszString)
{
	if (NULL != lpszString)
	{
		m_csText = lpszString;
	}
	else
	{
		m_csText = _T("");
	}
}

int CTransparentCtrl::GetWindowText(LPTSTR lpszStringBuf, int nMaxCount)
{
	if (m_csText.IsEmpty())
	{
		CStatic::GetWindowText(m_csText);
	}
	if (NULL != lpszStringBuf)
	{
		_sntprintf(lpszStringBuf, nMaxCount, _T("%s"), m_csText.GetBuffer());
		m_csText.ReleaseBuffer();
	}	
	return m_csText.GetLength();
}

void CTransparentCtrl::GetWindowText(CString& rString)
{
	if (m_csText.IsEmpty())
	{
		CStatic::GetWindowText(m_csText);
	}
	rString = m_csText;
	return ;
}

int CTransparentCtrl::GetWindowTextLength()
{
	if (m_csText.IsEmpty())
	{
		CStatic::GetWindowText(m_csText);
	}
	return m_csText.GetLength();
}
BOOL CTransparentCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE; // CStatic::OnEraseBkgnd(pDC);
}
