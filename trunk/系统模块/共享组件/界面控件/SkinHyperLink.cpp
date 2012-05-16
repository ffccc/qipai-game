#include "Stdafx.h"
#include "Resource.h"
#include "SkinHyperLink.h"

//////////////////////////////////////////////////////////////////////////

//��ʾ ID
#define TOOLTIP_ID					100

//��̬����
CSkinHyperLinkAttribute				CSkinHyperLink::m_SkinAttribute;				//����������

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinHyperLink, CStatic)

BEGIN_MESSAGE_MAP(CSkinHyperLink, CStatic)
	ON_WM_PAINT()
	ON_WM_CREATE()
    ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)
	ON_CONTROL_REFLECT(STN_CLICKED, OnStnClicked)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinHyperLinkAttribute::CSkinHyperLinkAttribute() 
{
	//���ñ���
	m_crBackColor=RGB(0,0,0);
	m_crHoverText=RGB(0,0,0);
	m_crNormalText=RGB(0,0,0);
	m_crVisitedText=RGB(0,0,0);
}

//��������
CSkinHyperLinkAttribute::~CSkinHyperLinkAttribute()
{
}

//��������
bool CSkinHyperLinkAttribute::UpdateSkinResource(ISkinResource * pISkinResource)
{
	//��ȡ��Ϣ
	tagSkinHyperLinkResource SkinHyperLinkResource;
	if (pISkinResource!=NULL) pISkinResource->GetResourceInfo(SkinHyperLinkResource);

	//���ñ���
	m_crBackColor=SkinHyperLinkResource.crBackColor;
	m_crHoverText=SkinHyperLinkResource.crHoverText;
	m_crNormalText=SkinHyperLinkResource.crNormalText;
	m_crVisitedText=SkinHyperLinkResource.crVisitedText;

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinHyperLink::CSkinHyperLink()
{
	m_crBackColor=0;
	m_bVisited=false;
	m_bHovering=false;
	m_bUnderline=true;
	m_bAutoAdjust=true;
	m_pIMouseEvent=NULL;
}

//��������
CSkinHyperLink::~CSkinHyperLink()
{
}

//�������ص��ӿ�
void CSkinHyperLink::SetMouseEventSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIMouseEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IMouseEvent);
	ASSERT(m_pIMouseEvent!=NULL);
	return;
}

//��ȡ���ص��ӿ�
void * CSkinHyperLink::GetMouseEventSink(const IID & Guid, DWORD dwQueryVer) 
{ 
	if (m_pIMouseEvent==NULL) return NULL;
	return m_pIMouseEvent->QueryInterface(Guid,dwQueryVer);
}

//��Ϣ����
BOOL CSkinHyperLink::PreTranslateMessage(MSG * pMsg)
{
    m_ToolTip.RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

//�ؼ����໯
void CSkinHyperLink::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//��ʼ��
	InitHyperLink();

	return;
}

//������Ϣ
int CSkinHyperLink::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ʼ��
	InitHyperLink();

	return 0;
}

//�滭��Ϣ
void CSkinHyperLink::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//�滭����
	dc.FillSolidRect(&ClientRect,m_crBackColor);

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	if (m_bHovering) dc.SetTextColor(m_SkinAttribute.m_crHoverText);
	else if (m_bVisited) dc.SetTextColor(m_SkinAttribute.m_crVisitedText);
	else dc.SetTextColor(m_SkinAttribute.m_crNormalText);

	//�滭����
	CString strText;
	GetWindowText(strText);
	LONG dwStyte=GetWindowLong(m_hWnd,GWL_STYLE);
	UINT uFormat=DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	if (dwStyte&SS_RIGHT) uFormat|=DT_RIGHT;
	else if (dwStyte&SS_CENTER) uFormat|=DT_CENTER;
	else uFormat|=DT_LEFT;
	dc.SelectObject(CSkinResourceManager::m_DefaultFont);
	dc.DrawText(strText,&ClientRect,uFormat);

	return;
}

//�����Ϣ
BOOL CSkinHyperLink::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	HCURSOR hCursor=LoadCursor(GetModuleHandle(SKIN_CONTROL_DLL_NAME),MAKEINTRESOURCE(IDC_HAND_CUR));
	::SetCursor(hCursor);
	return TRUE;
}

//�����ؼ�
void CSkinHyperLink::OnStnClicked()
{
	//�ж�״̬
	if ((m_pIMouseEvent!=NULL)||(m_strURL.IsEmpty())) return;

	//����ҳ
	ShellExecute(NULL,TEXT("open"),m_strURL,NULL,NULL,SW_SHOW);

	//�ػ��ؼ�
	if (m_bVisited==false)
	{
		m_bVisited=true;
		Invalidate();
	}

	return;
}

//����ƶ���Ϣ
void CSkinHyperLink::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bHovering==false)
	{
		//ע����Ϣ
		m_bHovering=true;
		Invalidate();
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);
	}

	__super::OnMouseMove(nFlags, point);
}

//����뿪��Ϣ
LRESULT CSkinHyperLink::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering=false;
	Invalidate();
	return 0;
}

//������Ϣ
LRESULT	CSkinHyperLink::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=Default();
	if (m_bAutoAdjust) AdjustHyperLink();
	return lResult;
}

//���÷���
void CSkinHyperLink::SetVisited(bool bVisited)
{
	if (m_bVisited!=bVisited)
	{
		m_bVisited=bVisited;
		if (GetSafeHwnd()) Invalidate();
	}
	return;
}

//�����»���
void CSkinHyperLink::SetUnderline(bool bUnderline)
{
	if (m_bUnderline!=bUnderline)
	{
		m_bUnderline=bUnderline;
		if (GetSafeHwnd()) Invalidate();
	}
	return;
}

//���� URL
void CSkinHyperLink::SetHyperLinkUrl(LPCTSTR pszUrl)
{
	//���ñ���
	m_strURL=pszUrl;

	//������ʾ
	UpdateToolTip();

	return;
}

//������ɫ
void CSkinHyperLink::SetBackGroundColor(COLORREF crColor)
{
	m_crBackColor=crColor;
	if (m_hWnd!=NULL) Invalidate(FALSE);
	return;
}

//���õ���
void CSkinHyperLink::SetAutoAdjust(bool bAutoAdjust)
{
	if (m_bAutoAdjust!=bAutoAdjust)
	{
		m_bAutoAdjust=bAutoAdjust;
		if (m_bAutoAdjust) AdjustHyperLink();
	}
	return;
}

//��ʼ��
void CSkinHyperLink::InitHyperLink()
{
	SetWindowLong(m_hWnd,GWL_STYLE,GetStyle()|SS_NOTIFY);
	if (m_bAutoAdjust) AdjustHyperLink();
	m_crBackColor=m_SkinAttribute.m_crBackColor;
	UpdateToolTip();
	return;
}

//������ʾ
void CSkinHyperLink::UpdateToolTip()
{
	if (GetSafeHwnd())
	{
		if (m_ToolTip.GetSafeHwnd()==NULL) m_ToolTip.Create(this);
		if (m_strURL.IsEmpty()==false)
		{
			CRect ClientRect;
			GetClientRect(&ClientRect);
			m_ToolTip.Activate(TRUE);
			m_ToolTip.AddTool(this,m_strURL,&ClientRect,TOOLTIP_ID);
		}
		else m_ToolTip.Activate(FALSE);
	}
	return;
}

//����λ��
void CSkinHyperLink::AdjustHyperLink()
{
	//��ȡ����
	CString strText;
	GetWindowText(strText);

	//��ȡλ��
	CRect ControlRect;
	GetWindowRect(&ControlRect);
	GetParent()->ScreenToClient(&ControlRect);

	//���㳤��
	CDC * pDC=GetDC();
	pDC->SelectObject(CSkinResourceManager::m_DefaultFont);
	CSize Size=pDC->GetTextExtent(strText);
	ReleaseDC(pDC);

	//�ƶ�λ��
	MoveWindow(ControlRect.left,ControlRect.top,Size.cx,Size.cy);

	return;
}

//Ĭ�ϻص�����
LRESULT CSkinHyperLink::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pIMouseEvent)
	{
		switch (message)
		{
		case WM_MOUSELEAVE:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventMouseLeft(uButtonID,wParam,lParam);
				break;
			}
		case WM_MOUSEMOVE:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventMouseMove(uButtonID,wParam,lParam);
				break;
			}
		case WM_LBUTTONDOWN:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventLButtonDown(uButtonID,wParam,lParam);
				break;
			}
		case WM_LBUTTONUP:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventLButtonUp(uButtonID,wParam,lParam);
				break;
			}
		case WM_RBUTTONDOWN:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventRButtonDown(uButtonID,wParam,lParam);
				break;
			}
		case WM_RBUTTONUP:
			{
				UINT uButtonID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pIMouseEvent->OnEventRButtonUp(uButtonID,wParam,lParam);
				break;
			}
		}
	}
	return __super::DefWindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

