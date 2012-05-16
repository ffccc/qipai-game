#include "Stdafx.h"
#include "SkinFrameWnd.h"

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSkinFrameWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CSkinFrameWnd, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETTINGCHANGE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinFrameWnd::CSkinFrameWnd() : CSkinWndObject(this)
{
}

//��������
CSkinFrameWnd::~CSkinFrameWnd()
{
}

//������Ϣ
int CSkinFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ʼ��
	InitSkinObject();
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//�����Ϣ
void CSkinFrameWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags,point);
	OnLMouseDoubleEvent(point);
	return;
}

//�����Ϣ
void CSkinFrameWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);
	OnLMouseDownEvent(point);
	return;
}

//�����Ϣ
void CSkinFrameWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags,point);
	OnLMouseUpEvent(point); 
	return;
}

//�����Ϣ
void CSkinFrameWnd::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnRButtonDblClk(nFlags,point);
	OnRMouseDoubleEvent(point);
	return;
}

//�����Ϣ
void CSkinFrameWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	__super::OnRButtonDown(nFlags,point);
	OnRMouseDownEvent(point);
	return;
}

//�����Ϣ
void CSkinFrameWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	__super::OnRButtonUp(nFlags,point);
	OnRMouseUpEvent(point);
	return;
}

//�����Ϣ
void CSkinFrameWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags,point);
	OnMouseMoveEvent(point);
	return;
}

//�滭��Ϣ 
void CSkinFrameWnd::OnPaint()
{
	CPaintDC dc(this);
	DrawSkinView(&dc);
	return;
}

//���øı�
void CSkinFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags, lpszSection);

	//������ܴ�С
	CSkinWndObject::OnSettingChange();

	return;
}

//λ�øı�
void CSkinFrameWnd::OnWindowPosChanged(WINDOWPOS * lpwndpos)
{
	__super::OnWindowPosChanged(lpwndpos);

	//����λ��
	CRect rcClient;
	m_cbNowHotButton=255;
	m_cbNowDownButton=255;
	GetClientRect(&rcClient);
	AdjustTitleButton(rcClient.Width(),rcClient.Height());

	//�ػ�����
	DrawSkinView(NULL);

	return;
}

//�滭����
BOOL CSkinFrameWnd::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//�ؼ���ɫ
HBRUSH CSkinFrameWnd::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(m_SkinAttribute.m_crControlTXColor);
			return m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//������Ϣ
LRESULT	CSkinFrameWnd::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);
	DrawSkinView(NULL);
	return lResult;
}

//////////////////////////////////////////////////////////////////////////
