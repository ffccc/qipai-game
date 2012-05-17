#include "CMyListCtrl.h"


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

 void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	SendMessageW(m_viewHandle,WM_LBTUP,nFlags,(LPARAM)&point);
}
