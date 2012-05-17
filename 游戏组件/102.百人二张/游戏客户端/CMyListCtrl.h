#ifndef __MYLIST__
#define  __MYLIST__
#pragma once

#include "Stdafx.h"
#define WM_LBTUP  WM_USER+1

class CMyListCtrl :public CListCtrl
{
public:
	    HWND	m_viewHandle;
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()
};
#endif
