#include "Stdafx.h"
#include "SkinButtonEx.h"

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinButtonEx, CButton)

BEGIN_MESSAGE_MAP(CSkinButtonEx, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

//���캯��
CSkinButtonEx::CSkinButtonEx()
{
	m_bHovering=false;
	m_byStatusCount=5;
}

//��������
CSkinButtonEx::~CSkinButtonEx()
{
}

//���󸽼ӵ����д���
void CSkinButtonEx::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	return;
}

//������Ϣ
int CSkinButtonEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	return 0;
}

//����λͼ
bool CSkinButtonEx::SetButtonImage(LPCTSTR pszFileName, BYTE byStatusCount, bool bExpandImage)
{
	//Ч�����
	ASSERT(pszFileName);
	if (pszFileName==NULL) return false;

	//����λͼ
	//m_ImageBack.SetLoadInfo(pszFileName);
	m_PngBack.LoadImage(pszFileName);

	//���ñ���
	m_byStatusCount=byStatusCount;

	//����λ��
	if (bExpandImage==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}

//����λͼ
bool CSkinButtonEx::SetButtonImage(UINT uBitmapID, HINSTANCE hInstance, BYTE byStatusCount, bool bExpandImage)
{
	//����λͼ
	ASSERT(uBitmapID!=0);
	if (uBitmapID==0) return false;
	m_ImageBack.SetLoadInfo(uBitmapID,hInstance);

	//���ñ���
	m_byStatusCount=byStatusCount;

	//����λ��
	if (bExpandImage==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}

//Ĭ�ϰ�ť
void CSkinButtonEx::SetDefaultButton(bool bResetIcon, bool bResetColor, bool bResetImage)
{
	//�ָ�Ĭ��
	if (bResetImage) 
	{
		m_ImageBack.RemoveLoadInfo();
	}

	//ˢ�½���
	if (GetSafeHwnd()) Invalidate(FALSE);

	return;
}

//����λ��
bool CSkinButtonEx::FixButtonSize()
{
	if (m_ImageBack.IsSetLoadInfo()&&GetSafeHwnd())
	{
		CImageHandle ImageHandle(&m_ImageBack);
		SetWindowPos(NULL,0,0,ImageHandle->GetWidth()/m_byStatusCount,ImageHandle->GetHeight(),SWP_NOMOVE);
		return true;
	}
	else if (m_PngBack.IsNull()==false&&GetSafeHwnd())
	{
		SetWindowPos(NULL,0,0,m_PngBack.GetWidth()/m_byStatusCount,m_PngBack.GetHeight(),SWP_NOMOVE);
		return true;
	}
	return false;
}

//����ƶ���Ϣ
void CSkinButtonEx::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bHovering==false)
	{
		//ע����Ϣ
		m_bHovering=true;
		Invalidate(FALSE);
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
LRESULT CSkinButtonEx::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering=false;
	Invalidate(FALSE);
	return 0;
}

//��������
BOOL CSkinButtonEx::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//����滭����
void CSkinButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//�������
	CRect ClientRect;
	GetClientRect(&ClientRect);
	bool bDisable=((lpDrawItemStruct->itemState&ODS_DISABLED)!=0);
	bool bButtonDown=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);

	//���� DC
	HDC hDC=lpDrawItemStruct->hDC;
	CDC *pDC=CDC::FromHandle(hDC);

	//��ȡ����
	CString strText;
	GetWindowText(strText);

	//���ر���ͼ
	CImageHandle ImageHandle;
	if (m_ImageBack.IsSetLoadInfo()) ImageHandle.AttachResource(&m_ImageBack);
	if (ImageHandle.IsResourceValid() || m_PngBack.IsNull()==false)
	{
		//����λͼλ��
		int iPartWidth=0,iDrawPos=0;
		if (ImageHandle.IsResourceValid())
            iPartWidth=ImageHandle->GetWidth()/m_byStatusCount;
		else
			iPartWidth=m_PngBack.GetWidth()/m_byStatusCount;

		if (5==m_byStatusCount)
		{
			if (bDisable) iDrawPos=iPartWidth*4;
			else if (bButtonDown) iDrawPos=iPartWidth;
			else if (m_bHovering) iDrawPos=iPartWidth*3;
		}
		else if (4==m_byStatusCount)
		{
			if (bDisable) iDrawPos=iPartWidth*3;
			else if (bButtonDown) iDrawPos=iPartWidth*2;
			else if (m_bHovering) iDrawPos=0;
			else iDrawPos=iPartWidth;
		}
		else if (3==m_byStatusCount)
		{
			if (bButtonDown) iDrawPos=iPartWidth*2;
			else if (m_bHovering) iDrawPos=0;
			else iDrawPos=iPartWidth;
		}

		//�滭����ͼ
		if (ImageHandle.IsResourceValid())
            ImageHandle->BitBlt(hDC,0,0,ClientRect.Width(),ClientRect.Height(),iDrawPos,0,SRCCOPY);
		else
			m_PngBack.DrawImage(pDC,0,0,ClientRect.Width(),ClientRect.Height(),iDrawPos,0);
	}
	else
	{
		//�滭Ĭ�Ͻ���
		CDC * pDC=CDC::FromHandle(hDC);
		pDC->FillSolidRect(&ClientRect,GetSysColor(COLOR_BTNFACE));
		if (bButtonDown) pDC->Draw3dRect(&ClientRect,GetSysColor(COLOR_WINDOWFRAME),GetSysColor(COLOR_3DHILIGHT));
		else pDC->Draw3dRect(&ClientRect,GetSysColor(COLOR_3DHILIGHT),GetSysColor(COLOR_WINDOWFRAME));
	}

	//�滭ͼ��
	if (bButtonDown) ClientRect.top+=2;

	////�滭����
	//ClientRect.top+=1;
	//::SetBkMode(hDC,TRANSPARENT);
	//if(!m_bShowTextFrame)
	//{
	//	if (bDisable) ::SetTextColor(hDC,GetSysColor(COLOR_GRAYTEXT));
	//	else ::SetTextColor(hDC,m_crTextColor);
	//	DrawText(hDC,strText,strText.GetLength(),ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	//}
	////��������
	//else
	//{
	//	CDC * pDC=CDC::FromHandle(hDC);
	//	DrawTextString(pDC,strText,m_crTextColor,m_crTextFrameColor,ClientRect);
	//}

	return;
}
//��������
void CSkinButtonEx::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	////��������
	//int nStringLength=lstrlen(pszString);
	//int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	//int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	////�滭�߿�
	//pDC->SetTextColor(crFrame);
	//CRect rcDraw;
	//for (int i=0;i<CountArray(nXExcursion);i++)
	//{
	//	rcDraw.CopyRect(lpRect);
	//	rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
	//	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	//}

	////�滭����
	//rcDraw.CopyRect(lpRect);
	//pDC->SetTextColor(crText);
	//pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��������
void CSkinButtonEx::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	////��������
	//int nStringLength=lstrlen(pszString);
	//int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	//int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	////�滭�߿�
	//pDC->SetTextColor(crFrame);
	//for (int i=0;i<CountArray(nXExcursion);i++)
	//{
	//	pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	//}

	////�滭����
	//pDC->SetTextColor(crText);
	//pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}
//////////////////////////////////////////////////////////////////////////
