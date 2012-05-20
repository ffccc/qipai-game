#pragma once
#include "Stdafx.h"

class CSkinButtonEx :
	public CSkinButton
{
public:
	CSkinButtonEx(void);
public:
	~CSkinButtonEx(void);
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
