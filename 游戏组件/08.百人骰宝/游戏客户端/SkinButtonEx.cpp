#include "StdAfx.h"
#include "SkinButtonEx.h"

CSkinButtonEx::CSkinButtonEx(void)
{
}

CSkinButtonEx::~CSkinButtonEx(void)
{
}
BEGIN_MESSAGE_MAP(CSkinButtonEx, CSkinButton)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CSkinButtonEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE; //CSkinButton::OnEraseBkgnd(pDC);
}
