#ifndef __MYLIST__
#define  __MYLIST__
#pragma once

#include "Stdafx.h"
#include "resource.h"
#define WM_LBTUP  WM_USER+1

class CMyListCtrl :public CListCtrl
{
public:
	    HWND	m_viewHandle;
		BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		void CMyListCtrl::Init(CRect windowRect,CWnd* pParent);
		afx_msg void OnPaint();
		DECLARE_MESSAGE_MAP()
};
#endif
