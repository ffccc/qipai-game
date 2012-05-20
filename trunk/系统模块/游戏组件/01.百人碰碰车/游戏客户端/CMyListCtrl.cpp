#include "CMyListCtrl.h"


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_LBUTTONUP()
	//ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CMyListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CClientDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);

	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_APPLY_USER_BACK);
	CBitmap* pOldBitmap = bitmapDC.SelectObject(&bitmap);
	pDC->BitBlt(clientRect.left,clientRect.top,clientRect.right-clientRect.left,clientRect.bottom-clientRect.top,&bitmapDC,0,0,SRCCOPY);
	bitmapDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	pOldBitmap = NULL;
	return TRUE;
}
 void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	SendMessageW(m_viewHandle,WM_LBTUP,nFlags,(LPARAM)&point);
}
void CMyListCtrl::OnPaint()
{
	

}
void CMyListCtrl::Init(CRect windowRect,CWnd* pParent)
{
	
	//another way to hide scrollbars
	

		//return ;
	InitializeFlatSB(this->m_hWnd);
	FlatSB_EnableScrollBar(this->m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);


	
	//GetSystemMetrics to find out height difference of the
	//titlebar so we can position scrollbars properly without
	//having to worry about windows appearances affecting it

	/*CWnd* pParent = GetParent();

	CRect windowRect;
	GetWindowRect(&windowRect);*/

	//CRect windowRect;
	//GetWindowRect(&windowRect);

	int nTitleBarHeight = 0;

	if(pParent->GetStyle() & WS_CAPTION)
		nTitleBarHeight = GetSystemMetrics(SM_CYSIZE);


	int nDialogFrameHeight = 0;
	int nDialogFrameWidth = 0;
	if((pParent->GetStyle() & WS_BORDER))
	{
		nDialogFrameHeight = GetSystemMetrics(SM_CYDLGFRAME);
		nDialogFrameWidth = GetSystemMetrics(SM_CYDLGFRAME);
	}

	if(pParent->GetStyle() & WS_THICKFRAME)
	{
		nDialogFrameHeight+=1;
		nDialogFrameWidth+=1;
	}


	//Create scrollbars at runtime
	//m_SkinVerticleScrollbar.Create(NULL, WS_CHILD|SS_LEFT|SS_NOTIFY|WS_VISIBLE|WS_GROUP,CRect(windowRect.right-nDialogFrameWidth,windowRect.top-nTitleBarHeight-nDialogFrameHeight-1,windowRect.right+12-nDialogFrameWidth,windowRect.bottom+11-nTitleBarHeight-nDialogFrameHeight), pParent);
	//m_SkinVerticleScrollbar.pList = this;
}
