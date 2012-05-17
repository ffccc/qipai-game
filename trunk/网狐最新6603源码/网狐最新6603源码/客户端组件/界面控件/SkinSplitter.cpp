#include "Stdafx.h"
#include "SkinSplitter.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinSplitter, CWnd)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinSplitter::CSkinSplitter()
{
	//״̬����
	m_bMouseDown=false;
	m_rcDrawLine.SetRect(0,0,0,0);

	//��ɫ����
	m_ColorS=0.99;
	m_ColorB=0.42;
	m_crBackColor=CLR_INVALID;

	//���Ա���
	m_nMaxDragPos=0;
	m_nMinDragPos=0;
	m_SplitterType=SplitterType_Vor;

	//�������
	m_pISplitterEvent=NULL;

	return;
}

//��������
CSkinSplitter::~CSkinSplitter()
{
}

//�󶨺���
VOID CSkinSplitter::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//��������
	ModifyStyle(0,SS_NOTIFY|SS_OWNERDRAW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	return;
}

//������Ϣ
BOOL CSkinSplitter::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��Ϣת��
	GetParent()->SendMessage(WM_COMMAND,wParam,lParam);

	return TRUE;
}

//��������
VOID CSkinSplitter::SetSplitterType(enSplitterType SplitterType)
{
	//���ñ���
	m_SplitterType=SplitterType;

	return;
}

//���÷�Χ
VOID CSkinSplitter::SetSplitterRange(INT nMinDragPos, INT nMaxDragPos)
{
	//���ñ���
	m_nMaxDragPos=nMaxDragPos;
	m_nMinDragPos=nMinDragPos;

	return;
}

//���ýӿ�
VOID CSkinSplitter::SetSplitterEvent(ISplitterEvent * pISplitterEvent)
{
	//���ñ���
	m_pISplitterEvent=pISplitterEvent;

	return;
}

//������ɫ
VOID CSkinSplitter::SetSplitterColor(DOUBLE S, DOUBLE B)
{
	//���ñ���
	m_ColorS=S;
	m_ColorB=B;
	m_crBackColor=CLR_INVALID;

	//�ػ�����
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_ERASENOW);

	return;
}

//������ɫ
VOID CSkinSplitter::SetSplitterColor(COLORREF crBackColor)
{
	//���ñ���
	m_ColorS=0.0;
	m_ColorB=0.0;
	m_crBackColor=crBackColor;

	//�ػ�����
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_ERASENOW);

	return;
}

//�ػ���Ϣ
VOID CSkinSplitter::OnPaint()
{
	CPaintDC dc(this);

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	if (m_crBackColor==CLR_INVALID)
	{
		CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
		dc.FillSolidRect(&rcClient,pSkinRenderManager->RenderColor(m_ColorS,m_ColorB));
	}
	else dc.FillSolidRect(&rcClient,m_crBackColor);

	return;
}

//�滭����
BOOL CSkinSplitter::OnEraseBkgnd(CDC * pDC)
{
	//���½���
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//ʧȥ����
VOID CSkinSplitter::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	if (m_bMouseDown==true)
	{
		//��ȡ DC
		CDC * pDC=CDC::FromHandle(::GetDC(NULL));
		pDC->SelectObject(CDC::GetHalftoneBrush());

		//�滭����
		pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

		//�ͷ���Դ
		ReleaseCapture();
		::ReleaseDC(NULL,pDC->m_hDC);

		//���ñ���
		m_bMouseDown=false;
		m_rcDrawLine.SetRectEmpty();
	}

	return;
}

//�����Ϣ
VOID CSkinSplitter::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags,Point);

	if (m_bMouseDown==true)
	{
		//��ȡλ��
		CRect rcWindow;
		GetWindowRect(&rcWindow);

		//Ч��λ��
		CPoint PointParent=Point;
		MapWindowPoints(GetParent(),&PointParent,1);

		//�ؼ�λ��
		CSize SizeControl;
		SizeControl.SetSize(rcWindow.Width(),rcWindow.Height());

		//���ù��
		if (m_SplitterType==SplitterType_Hor)
		{
			if ((PointParent.y>(m_nMaxDragPos-SizeControl.cy/2))||(PointParent.y<(m_nMinDragPos+SizeControl.cy/2)))
			{
				//���ù��
				SetCursor(LoadCursor(NULL,IDC_NO));

				//����λ��
				PointParent.y=__min(m_nMaxDragPos-SizeControl.cy/2,PointParent.y);
				PointParent.y=__max(m_nMinDragPos+SizeControl.cy/2,PointParent.y);
			}
			else SetCursor(LoadCursor(NULL,IDC_SIZENS));
		}
		else
		{
			if ((PointParent.x>(m_nMaxDragPos-SizeControl.cx/2))||(PointParent.x<(m_nMinDragPos+SizeControl.cx/2)))
			{
				//���ù��
				SetCursor(LoadCursor(NULL,IDC_NO));

				//����λ��
				PointParent.x=__min(m_nMaxDragPos-SizeControl.cx/2,PointParent.x);
				PointParent.x=__max(m_nMinDragPos+SizeControl.cx/2,PointParent.x);
			}
			else SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		}

		//ת��λ��
		CPoint PointScreen=PointParent;
		GetParent()->ClientToScreen(&PointScreen);
		
		//��������
		CRect rcCurrentDraw=m_rcDrawLine;
		if (m_SplitterType==SplitterType_Hor)
		{
			rcCurrentDraw.top=PointScreen.y-SizeControl.cy/2;
			rcCurrentDraw.bottom=rcCurrentDraw.top+SizeControl.cy;
		}
		else
		{
			rcCurrentDraw.left=PointScreen.x-SizeControl.cx/2;
			rcCurrentDraw.right=rcCurrentDraw.left+SizeControl.cx;
		}
		
		//�滭����
		if (rcCurrentDraw!=m_rcDrawLine)
		{
			//���� DC
			CDC * pDC=CDC::FromHandle(::GetDC(NULL));
			pDC->SelectObject(CDC::GetHalftoneBrush());

			//�滭���
			pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);
			pDC->PatBlt(rcCurrentDraw.left,rcCurrentDraw.top,rcCurrentDraw.Width(),rcCurrentDraw.Height(),PATINVERT);

			//�ͷ���Դ
			::ReleaseDC(NULL,pDC->m_hDC);

			//���ñ���
			m_rcDrawLine=rcCurrentDraw;
		}
	}

	return;
}

//�����Ϣ
VOID CSkinSplitter::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	if (m_bMouseDown==true)
	{
		//�ָ�״̬
		ReleaseCapture();

		//��ȡ DC
		CDC * pDC=CDC::FromHandle(::GetDC(NULL));
		pDC->SelectObject(CDC::GetHalftoneBrush());

		//�滭����
		pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

		//�ͷ���Դ
		::ReleaseDC(NULL,pDC->m_hDC);

		//��������
		bool bRectify=false;

		//��ȡλ��
		CRect rcControl;
		GetClientRect(&rcControl);
		MapWindowPoints(GetParent(),rcControl);

		//ת������
		CPoint PointParent=Point;
		MapWindowPoints(GetParent(),&PointParent,1);

		//�ؼ�λ��
		CSize SizeControl;
		SizeControl.SetSize(rcControl.Width(),rcControl.Height());

		//����λ��
		if (m_SplitterType==SplitterType_Hor)
		{
			//�ƶ�λ��
			PointParent.y=__min(m_nMaxDragPos-SizeControl.cy/2,PointParent.y);
			PointParent.y=__max(m_nMinDragPos+SizeControl.cy/2,PointParent.y);

			//�ƶ�����
			if (rcControl.top!=(PointParent.y-SizeControl.cy/2))
			{
				bRectify=true;
				SetWindowPos(NULL,rcControl.left,PointParent.y-SizeControl.cy/2,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS);
			}
		}
		else
		{
			//�ƶ�λ��
			PointParent.x=__min(m_nMaxDragPos-SizeControl.cx/2,PointParent.x);
			PointParent.x=__max(m_nMinDragPos+SizeControl.cx/2,PointParent.x);

			//�ƶ�����
			if (rcControl.left!=(PointParent.x-SizeControl.cx/2))
			{
				bRectify=true;
				SetWindowPos(NULL,PointParent.x-SizeControl.cx/2,rcControl.top,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS);
			}
		}

		//����֪ͨ
		if (bRectify==true)
		{
			//����֪ͨ
			if (m_pISplitterEvent!=NULL) 
			{
				UINT uControlID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pISplitterEvent->OnSplitterEvent(uControlID,PointParent.x,PointParent.y);
			}
		}

		//���ñ���
		m_bMouseDown=false;
		m_rcDrawLine.SetRectEmpty();
	}

	return;
}

//�����Ϣ
VOID CSkinSplitter::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//����״̬
	SetFocus();
	SetCapture();

	//��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//���ñ���
	m_bMouseDown=true;
	m_rcDrawLine=rcWindow;

	//���� DC
	CDC * pDC=CDC::FromHandle(::GetDC(NULL));
	pDC->SelectObject(CDC::GetHalftoneBrush());

	//�滭����
	pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

	//�ͷ���Դ
	ReleaseDC(pDC);

	return;
}

//�����Ϣ
BOOL CSkinSplitter::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMmessage)
{
	//��ȡλ��
	CPoint Point;
	GetCursorPos(&Point);

	//���ù��
	if (pWnd==this)
	{
		SetCursor(LoadCursor(NULL,(m_SplitterType==SplitterType_Vor)?IDC_SIZEWE:IDC_SIZENS));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMmessage);
}

//////////////////////////////////////////////////////////////////////////////////
