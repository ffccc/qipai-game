#include "Stdafx.h"
#include "Resource.h"
#include "SkinButton.h"

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinButton, CButton)

BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CRgnButton, CButton)

BEGIN_MESSAGE_MAP(CRgnButton, CButton)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//��̬����
CSkinButtonAttribute				CSkinButton::m_SkinAttribute;			//��ť����

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinButtonAttribute::CSkinButtonAttribute() 
{
	//��ʼ������
	m_crTextColor=RGB(0,0,0);

	return;
}

//��������
CSkinButtonAttribute::~CSkinButtonAttribute() 
{
}

//��������
bool CSkinButtonAttribute::UpdateSkinResource(ISkinResource * pISkinResource)
{
	//��ȡ��Ϣ
	tagSkinButtonResource SkinButtonResource;
	if (pISkinResource!=NULL) pISkinResource->GetResourceInfo(SkinButtonResource);

	//���ñ���
	m_crTextColor=SkinButtonResource.crButtonText;
	m_ImageBack.SetLoadInfo(SkinButtonResource.LoadInfoButton);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinButton::CSkinButton()
{
	m_hIcon=NULL;
	m_bExpand=true;
	m_bHovering=false;
	m_bShowTextFrame=false;
	m_pIMouseEvent=NULL;
	m_crTextColor=m_SkinAttribute.m_crTextColor;
	m_crTextFrameColor=DEF_TEXT_FRAME_COLOR;
}

//��������
CSkinButton::~CSkinButton()
{
}

//���󸽼ӵ����д���
void CSkinButton::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
	return;
}

//������Ϣ
int CSkinButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
	return 0;
}

//������ɫ
bool CSkinButton::SetTextColor(COLORREF crTextColor,COLORREF crTextFrameColor,bool bShowFrame)
{
	m_crTextColor=crTextColor;
	m_bShowTextFrame=bShowFrame;
	m_crTextFrameColor=crTextFrameColor;

	if (GetSafeHwnd()) Invalidate(FALSE);
	return true;
}

//����ͼ��
bool CSkinButton::SetButtonIcon(HICON hIcon)
{
	m_hIcon=hIcon;
	if (GetSafeHwnd()) Invalidate(FALSE);
	return true;
}

//����λͼ
bool CSkinButton::SetButtonImage(LPCTSTR pszFileName, bool bExpandImage)
{
	//Ч�����
	ASSERT(pszFileName);
	if (pszFileName==NULL) return false;

	//����λͼ
	m_bExpand=bExpandImage;
	m_ImageBack.SetLoadInfo(pszFileName);

	//����λ��
	if (m_bExpand==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}

//����λͼ
bool CSkinButton::SetButtonImage(UINT uBitmapID, HINSTANCE hInstance, bool bExpandImage)
{
	//����λͼ
	ASSERT(uBitmapID!=0);
	if (uBitmapID==0) return false;
	m_bExpand=bExpandImage;
	m_ImageBack.SetLoadInfo(uBitmapID,hInstance);

	//����λ��
	if (m_bExpand==false) FixButtonSize();
	if (GetSafeHwnd()) Invalidate(FALSE);

	return true;
}

//��ȡλͼ����
bool CSkinButton::GetButtonLoadInfo(tagImageLoadInfo & LoadInfo)
{
	if (m_ImageBack.IsSetLoadInfo()) return m_ImageBack.GetLoadInfo(LoadInfo);
	return m_SkinAttribute.m_ImageBack.GetLoadInfo(LoadInfo);
}

//Ĭ�ϰ�ť
void CSkinButton::SetDefaultButton(bool bResetIcon, bool bResetColor, bool bResetImage)
{
	//�ָ�Ĭ��
	if (bResetImage) 
	{
		m_bExpand=true;
		m_ImageBack.RemoveLoadInfo();
	}
	if (bResetIcon)	m_hIcon=NULL;
	if (bResetColor)
	{
		m_bShowTextFrame = false;
		m_crTextColor=m_SkinAttribute.m_crTextColor;
		m_crTextFrameColor = DEF_TEXT_FRAME_COLOR;
	}

	//ˢ�½���
	if (GetSafeHwnd()) Invalidate(FALSE);

	return;
}

//����λ��
bool CSkinButton::FixButtonSize()
{
	if (m_ImageBack.IsSetLoadInfo()&&GetSafeHwnd())
	{
		CImageHandle ImageHandle(&m_ImageBack);
		SetWindowPos(NULL,0,0,ImageHandle->GetWidth()/5,ImageHandle->GetHeight(),SWP_NOMOVE);
		return true;
	}
	return false;
}

//����ƶ���Ϣ
void CSkinButton::OnMouseMove(UINT nFlags, CPoint point)
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
LRESULT CSkinButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering=false;
	Invalidate(FALSE);
	return 0;
}

//��������
BOOL CSkinButton::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//����滭����
void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//�������
	CRect ClientRect;
	GetClientRect(&ClientRect);
	bool bDisable=((lpDrawItemStruct->itemState&ODS_DISABLED)!=0);
	bool bButtonDown=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);

	//���� DC
	HDC hDC=lpDrawItemStruct->hDC;

	//��ȡ����
	CString strText;
	GetWindowText(strText);

	//���ر���ͼ
	CImageHandle ImageHandle;
	if (m_ImageBack.IsSetLoadInfo()) ImageHandle.AttachResource(&m_ImageBack);
	else ImageHandle.AttachResource(&m_SkinAttribute.m_ImageBack);
	if (ImageHandle.IsResourceValid())
	{
		//����λͼλ��
		int iPartWidth=ImageHandle->GetWidth()/5,iDrawPos=0;
		if (bDisable) iDrawPos=iPartWidth*4;
		else if (bButtonDown) iDrawPos=iPartWidth;
		else if (m_bHovering) iDrawPos=iPartWidth*3;

		//�滭����ͼ
		if (m_bExpand==false) ImageHandle->BitBlt(hDC,0,0,ClientRect.Width(),ClientRect.Height(),iDrawPos,0,SRCCOPY);
		else ImageHandle->StretchBlt(hDC,0,0,ClientRect.Width(),ClientRect.Height(),iDrawPos,0,iPartWidth,ImageHandle->GetHeight(),SRCCOPY);
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
	if (m_hIcon)
	{
		DrawIconEx(hDC,ClientRect.left+6,ClientRect.top+(ClientRect.Height()-16)/2+1,m_hIcon,16,16,0,NULL,DI_NORMAL);
		ClientRect.left+=22;
	}

	//�滭����
	ClientRect.top+=1;
	::SetBkMode(hDC,TRANSPARENT);
	if(!m_bShowTextFrame)
	{
		if (bDisable) ::SetTextColor(hDC,GetSysColor(COLOR_GRAYTEXT));
		else ::SetTextColor(hDC,m_crTextColor);
		DrawText(hDC,strText,strText.GetLength(),ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}
	//��������
	else
	{
		CDC * pDC=CDC::FromHandle(hDC);
		DrawTextString(pDC,strText,m_crTextColor,m_crTextFrameColor,ClientRect);
	}

	return;
}

//Ĭ�ϻص�����
LRESULT CSkinButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

//��������
void CSkinButton::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��������
void CSkinButton::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CRgnButton::CRgnButton()
{
	m_bHovering=false;
	m_pIMouseEvent=NULL;
	m_crTrans=RGB(255,0,255);
	m_crTextColor=CSkinButton::m_SkinAttribute.m_crTextColor;
}

//��������
CRgnButton::~CRgnButton()
{
}

//���󸽼ӵ����д���
void CRgnButton::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
	return;
}

//������Ϣ
int CRgnButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	SendMessage(WM_SETFONT,(WPARAM)CSkinResourceManager::GetDefaultFont(),TRUE);
	return 0;
}

//����λͼ
bool CRgnButton::LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans)
{
	m_ImageBack.SetLoadInfo(uBitmapID,hInstance);
	return CreateControlRgn(crTrans);
}

//����λͼ
bool CRgnButton::LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans)
{
	m_ImageBack.SetLoadInfo(pszFileName);
	return CreateControlRgn(crTrans);
}

//��������
bool CRgnButton::CreateControlRgn(COLORREF crTrans)
{
	CImageHandle ImageHandle(&m_ImageBack);
	if (ImageHandle.IsResourceValid())
	{
		//������ʱͼ
		CSkinImage BufferImage;
		int nButtonWidth=m_ImageBack.GetWidth()/5;
		int nButtomHeight=m_ImageBack.GetHeight();
		BufferImage.Create(nButtonWidth,nButtomHeight,32);
		ImageHandle->BitBlt(BufferImage.GetDC(),0,0,nButtonWidth,nButtomHeight,0,0);
		BufferImage.ReleaseDC();

		//��������
		CRgn RgnControl;
		BufferImage.CreateImageRegion(RgnControl,crTrans);

		//��������
		if (RgnControl.GetSafeHandle()!=NULL)
		{
			//���ñ���
			m_crTrans=crTrans;

			//���ð�ť
			SetWindowRgn(RgnControl,TRUE);
			SetWindowPos(NULL,0,0,nButtonWidth,nButtomHeight,SWP_NOMOVE|SWP_NOACTIVATE);

			//��������
			m_ButtonRgn.DeleteObject();
			m_ButtonRgn.Attach(RgnControl.Detach());

			return true;
		}
	}

	return false;
}

//����ƶ���Ϣ
void CRgnButton::OnMouseMove(UINT nFlags, CPoint point)
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
LRESULT CRgnButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering=false;
	Invalidate(FALSE);
	return 0;
}

//����滭����
void CRgnButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//�������
	CRect ClientRect;
	GetClientRect(&ClientRect);
	bool bDisable=((lpDrawItemStruct->itemState&ODS_DISABLED)!=0);
	bool bButtonDown=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);

	//���ر���ͼ
	CImageHandle ImageHandle(&m_ImageBack);
	if (ImageHandle.IsResourceValid())
	{
		//����λͼλ��
		int iPartWidth=ImageHandle->GetWidth()/5,iDrawPos=0;
		if (bDisable) iDrawPos=iPartWidth*4;
		else if (bButtonDown) iDrawPos=iPartWidth;
		else if (m_bHovering) iDrawPos=iPartWidth*3;
		else if (lpDrawItemStruct->itemState&ODS_FOCUS) iDrawPos=iPartWidth*2;

		//�滭����ͼ
		CDC * pDesDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		m_ImageBack.AlphaDrawImage(pDesDC,0,0,iPartWidth,m_ImageBack.GetHeight(),iDrawPos,0,m_crTrans);
	}

	//��ȡ����
	CString strText;
	GetWindowText(strText);

	//�滭����
	ClientRect.top+=1;
	::SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
	if (bDisable) ::SetTextColor(lpDrawItemStruct->hDC,GetSysColor(COLOR_GRAYTEXT));
	else ::SetTextColor(lpDrawItemStruct->hDC,m_crTextColor);
	DrawText(lpDrawItemStruct->hDC,strText,strText.GetLength(),ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//Ĭ�ϻص�����
LRESULT CRgnButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

//��������
BOOL CRgnButton::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
