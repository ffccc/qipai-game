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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE; //CSkinButton::OnEraseBkgnd(pDC);
}
