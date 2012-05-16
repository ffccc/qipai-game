#include "Stdafx.h"
#include "Resource.h"
#include "SkinSplitter.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define IDC_SPLITTER_BUTTON		100										//��ť ID

//��̬����
CSkinSplitterAttribute			CSkinSplitter::m_SkinAttribute;			//���������

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinSplitter, CWnd)

BEGIN_MESSAGE_MAP(CSkinSplitter, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_SPLITTER_BUTTON, OnSplitterButton)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinSplitterAttribute::CSkinSplitterAttribute() 
{
	return;
}

//��������
CSkinSplitterAttribute::~CSkinSplitterAttribute() 
{
}

//��������
bool CSkinSplitterAttribute::UpdateSkinResource(ISkinResource * pISkinResource)
{
	//��ȡ��Ϣ
	tagSkinSplitterResource SkinSplitterResource;
	if (pISkinResource!=NULL) pISkinResource->GetResourceInfo(SkinSplitterResource);

	//���ñ���
	m_ImageHor.SetLoadInfo(SkinSplitterResource.LoadInfoHor);
	m_ImageVor.SetLoadInfo(SkinSplitterResource.LoadInfoVor);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinSplitter::CSkinSplitter(enSplitterType SplitterType, ISplitterSink * pISplitterSink)
{
	m_nMaxPos=0;
	m_nLessPos=0;
	m_bEnalbe=true;
	m_bMouseDown=false;
	m_SplitterType=SplitterType;
	m_pISplitterSink=pISplitterSink;
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

//���ò�ַ�Χ
void CSkinSplitter::SetSplitterRange(int nLessPos, int nMaxPos)
{
	m_nMaxPos=nMaxPos;
	m_nLessPos=nLessPos;
	return;
}

//���ò������
void CSkinSplitter::SetSplitterType(enSplitterType SplitterType)
{
	m_SplitterType=SplitterType;
	return;
}

//���ûص��ӿ�
void CSkinSplitter::SetSplitterSink(ISplitterSink * pISplitterSink)
{
	m_pISplitterSink=pISplitterSink;
	return;
}

//��������
void CSkinSplitter::EnableSplitter(bool bEnable)
{
	m_bEnalbe=bEnable;
	return;
}

//���ذ�ť
bool CSkinSplitter::HideSplitterButton()
{
	if (m_SplitterButton.m_hWnd!=NULL) m_SplitterButton.DestroyWindow();
	return true;
}

//��ʾ��ť
bool CSkinSplitter::ShowSplitterButton(UINT uButtonImage, HINSTANCE hInstance)
{
	//������ť
	if (m_SplitterButton.m_hWnd==NULL)
	{
		CRect rcButton(0,0,0,0);
		m_SplitterButton.Create(NULL,WS_CHILD,rcButton,this,IDC_SPLITTER_BUTTON);
	}

	//���ð�ť
	CRect ClientRect,rcButton;
	GetClientRect(&ClientRect);
	m_SplitterButton.SetButtonImage(uButtonImage,hInstance,false);
	m_SplitterButton.GetWindowRect(&rcButton);
	int nXPos=(ClientRect.Width()-rcButton.Width())/2;
	int nYPos=(ClientRect.Height()-rcButton.Height())/2;
	m_SplitterButton.MoveWindow(nXPos,nYPos,rcButton.Width(),rcButton.Height());
	m_SplitterButton.ShowWindow(SW_SHOW);

	return true;
}

//��ť��С
CSize CSkinSplitter::GetButtonSize()
{
	CRect rcButton(0,0,0,0);
	if (m_SplitterButton.m_hWnd!=NULL)
	{
		m_SplitterButton.GetWindowRect(&rcButton);
	}

	return CSize(rcButton.Width(),rcButton.Height());
}

//�ػ���Ϣ
void CSkinSplitter::OnPaint()
{
	CPaintDC dc(this);

	//�滭�߿�
	CRect ClientRect;
	CImageHandle ImageHandle;
	GetClientRect(&ClientRect);
	if (m_SplitterType==HorSpliter)	ImageHandle.AttachResource(&m_SkinAttribute.m_ImageHor);
	else  ImageHandle.AttachResource(&m_SkinAttribute.m_ImageVor);
	if (ImageHandle.IsResourceValid())
	{
		if (m_SplitterType==VorSpliter)
		{
			int nImageHeight=ImageHandle->GetHeight();
			for (int nYPos=0;nYPos<ClientRect.Height();nYPos+=nImageHeight)
			{
				ImageHandle->StretchBlt(dc,0,nYPos,ClientRect.Width(),nImageHeight); 
			}
		}
		else
		{
			int nImageWidth=ImageHandle->GetWidth();
			for (int nXPos=0;nXPos<ClientRect.Width();nXPos+=nImageWidth)
			{
				ImageHandle->StretchBlt(dc,nXPos,0,nImageWidth,ClientRect.Height());
			}
		}
	}
	else 
	{
		dc.FillSolidRect(&ClientRect,RGB(192,192,192));
		dc.Draw3dRect(&ClientRect,RGB(125,125,125),RGB(125,125,125));
	}

	return;
}

//������Ϣ
void CSkinSplitter::OnSplitterButton()
{
	if (m_pISplitterSink!=NULL) m_pISplitterSink->OnSplitterButton(this,&m_SplitterButton);
	return;
}

//�滭����
BOOL CSkinSplitter::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//ʧȥ����
void CSkinSplitter::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	if (m_bMouseDown==true)
	{
		//�ָ�����
		ReleaseCapture();
		m_bMouseDown=false;
		
		//��ȡλ��
		CRect ClientRect;
		GetClientRect(&ClientRect);
		
		//�滭�����
		CDC * pDC=GetDC();
		CBrush * pBrush=CDC::GetHalftoneBrush();
		CBrush * pOldBrush=pDC->SelectObject(pBrush);
		pDC->PatBlt(m_rcSplitter.left,m_rcSplitter.top,m_rcSplitter.Width(),m_rcSplitter.Height(),PATINVERT);
		m_rcSplitter.SetRectEmpty();
		pDC->SelectObject(pOldBrush);
		ReleaseDC(pDC);
	}

	return;
}

//�����Ϣ
void CSkinSplitter::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bMouseDown==true)
	{
		//��ȡλ��
		CRect ClientRect;
		GetClientRect(&ClientRect);

		//Ч��λ��
		CPoint PointCheck(point);
		MapWindowPoints(GetParent(),&PointCheck,1);
		if (m_SplitterType==HorSpliter)
		{
			if ((PointCheck.y>m_nMaxPos)||(PointCheck.y<m_nLessPos))
			{
				PointCheck.y=__min(m_nMaxPos,PointCheck.y);
				PointCheck.y=__max(m_nLessPos,PointCheck.y);
				SetCursor(LoadCursor(NULL,IDC_NO));
			}
			else SetCursor(LoadCursor(NULL,IDC_SIZENS));
		}
		else
		{
			if ((PointCheck.x>m_nMaxPos)||(PointCheck.x<m_nLessPos))
			{
				PointCheck.x=__min(m_nMaxPos,PointCheck.x);
				PointCheck.x=__max(m_nLessPos,PointCheck.x);
				SetCursor(LoadCursor(NULL,IDC_NO));
			}
			else SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		}
		GetParent()->MapWindowPoints(this,&PointCheck,1);
		
		//��������
		CRect NewSpliterRect=ClientRect;
		if (m_SplitterType==HorSpliter)
		{
			NewSpliterRect.top=PointCheck.y-ClientRect.Height()/2;
			NewSpliterRect.bottom=PointCheck.y+ClientRect.Height()/2;
		}
		else
		{
			NewSpliterRect.left=PointCheck.x-ClientRect.Width()/2;
			NewSpliterRect.right=PointCheck.x+ClientRect.Width()/2;
		}
		
		//�滭�����
		if (NewSpliterRect!=m_rcSplitter)
		{
			CDC * pDC=GetDC();
			CBrush * pBrush=CDC::GetHalftoneBrush();
			CBrush * pOldBrush=pDC->SelectObject(pBrush);
			pDC->PatBlt(m_rcSplitter.left,m_rcSplitter.top,m_rcSplitter.Width(),m_rcSplitter.Height(),PATINVERT);
			pDC->PatBlt(NewSpliterRect.left,NewSpliterRect.top,NewSpliterRect.Width(),NewSpliterRect.Height(),PATINVERT);
			m_rcSplitter=NewSpliterRect;
			pDC->SelectObject(pOldBrush);
			ReleaseDC(pDC);
		}
	}
	
	__super::OnMouseMove(nFlags,point);
}

//�����Ϣ
void CSkinSplitter::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bMouseDown==true)
	{
		//�ָ�����
		ReleaseCapture();
		m_bMouseDown=false;

		//��ȡλ��
		CRect ClientRect;
		GetClientRect(&ClientRect);
		MapWindowPoints(GetParent(),&ClientRect);
	
		//�滭�����
		CDC * pDC=GetDC();
		CBrush * pBrush=CDC::GetHalftoneBrush();
		CBrush * pOldBrush=pDC->SelectObject(pBrush);
		pDC->PatBlt(m_rcSplitter.left,m_rcSplitter.top,m_rcSplitter.Width(),m_rcSplitter.Height(),PATINVERT);
		m_rcSplitter.SetRectEmpty();
		pDC->SelectObject(pOldBrush);
		ReleaseDC(pDC);

		//�ƶ�λ��
		CPoint PointCheck(point);
		CRect SplitterRect(ClientRect);
		MapWindowPoints(GetParent(),&PointCheck,1);
		if (m_SplitterType==HorSpliter)
		{
			int nSpace=ClientRect.Height()/2;
			PointCheck.y=__min(m_nMaxPos,PointCheck.y);
			PointCheck.y=__max(m_nLessPos,PointCheck.y);
			SplitterRect.top=PointCheck.y-nSpace;
			SplitterRect.bottom=PointCheck.y+ClientRect.Height()-nSpace;
		}
		else
		{
			int nSpace=ClientRect.Width()/2;
			PointCheck.x=__min(m_nMaxPos,PointCheck.x);
			PointCheck.x=__max(m_nLessPos,PointCheck.x);
			SplitterRect.left=PointCheck.x-nSpace;
			SplitterRect.right=PointCheck.x+ClientRect.Width()-nSpace;
		}
		if (SplitterRect!=ClientRect)
		{
			Invalidate();
			MoveWindow(&SplitterRect);
			if (m_pISplitterSink!=NULL)
			{
				m_pISplitterSink->OnSplitterEvent(this,GetWindowLong(GetSafeHwnd(),GWL_ID),PointCheck.x,PointCheck.y);
			}
		}
	}
	__super::OnLButtonUp(nFlags, point);
}

//�����Ϣ
void CSkinSplitter::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bEnalbe)
	{
		//��ȡλ��
		CRect ClientRect;
		GetClientRect(&ClientRect);

		//���ò���
		SetFocus();
		SetCapture();
		m_bMouseDown=true;

		//�滭�����
		CDC * pDC=GetDC();
		CBrush * pBrush=CDC::GetHalftoneBrush();
		CBrush * pOldBrush=pDC->SelectObject(pBrush);
		m_rcSplitter=ClientRect;
		if (m_SplitterType==HorSpliter)
		{
			m_rcSplitter.top=point.y-ClientRect.Height()/2;
			m_rcSplitter.bottom=point.y+ClientRect.Height()/2;
		}
		else
		{
			m_rcSplitter.left=point.x-ClientRect.Width()/2;
			m_rcSplitter.right=point.x+ClientRect.Width()/2;
		}
		pDC->PatBlt(m_rcSplitter.left,m_rcSplitter.top,m_rcSplitter.Width(),m_rcSplitter.Height(),PATINVERT);
		pDC->SelectObject(pOldBrush);
		ReleaseDC(pDC);
	}
	
	__super::OnLButtonDown(nFlags, point);
}

//λ����Ϣ
void CSkinSplitter::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�ƶ���ť
	if (m_SplitterButton.m_hWnd!=NULL)
	{
		CRect rcButton;
		m_SplitterButton.GetWindowRect(&rcButton);
		int nXPos=(cx-rcButton.Width())/2;
		int nYPos=(cy-rcButton.Height())/2;
		m_SplitterButton.MoveWindow(nXPos,nYPos,rcButton.Width(),rcButton.Height());
		m_SplitterButton.ShowWindow(SW_SHOW);
	}

	return;
}

//�����Ϣ
BOOL CSkinSplitter::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	if (m_bEnalbe)
	{
		//��ȡ��ǰ��
		POINT Point;
		GetCursorPos(&Point);
		
		//�ж��Ƿ�ť
		bool bButtonRgn=false;
		if (m_SplitterButton.m_hWnd!=NULL)
		{
			CRect rcButton;
			m_SplitterButton.GetWindowRect(&rcButton);
			if (rcButton.PtInRect(Point)) bButtonRgn=true;
		}

		//������
		if (bButtonRgn==false)
		{
			GetParent()->ScreenToClient(&Point);
			if (m_SplitterType==VorSpliter) SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			else SetCursor(LoadCursor(NULL,IDC_SIZENS));
			return TRUE;
		}
	}
	return __super::OnSetCursor(pWnd,nHitTest,message);
}

//////////////////////////////////////////////////////////////////////////
