#include "Stdafx.h"
#include "Resource.h"
#include "SkinButton.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_TRANSITION				10									//���ɱ�ʶ
#define TIME_TRANSITION				30									//����ʱ��

//��������
#define MIN_TRANSITION_INDEX		0									//��������
#define MAX_TRANSITION_INDEX		8									//��������

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinButton, CButton)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()

	//�Զ���Ϣ
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinButton::CSkinButton()
{
	//״̬����
	m_bHovering=false;
	m_bTransparent=false;
	m_bRenderImage=false;

	//���ñ���
	m_crButtonText=RGB(0,0,0);
	m_wImageIndex=MIN_TRANSITION_INDEX;

	//������Դ
	m_pszSurface=NULL;
	m_pszResource=NULL;
	m_hResInstance=NULL;

	return;
}

//��������
CSkinButton::~CSkinButton()
{
}

//�󶨺���
VOID CSkinButton::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//���ð�ť
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);

	//�����ؼ�
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//��������
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
	SendMessage(WM_SETFONT,(WPARAM)(HFONT)(pSkinResourceManager->GetDefaultFont()),TRUE);

	return;
}

//ʱ����Ϣ
VOID CSkinButton::OnTimer(UINT_PTR nIDEvent)
{
	//���ɱ���
	if (nIDEvent==IDI_TRANSITION)
	{
		//��������
		if ((m_bHovering==true)&&(m_wImageIndex<MAX_TRANSITION_INDEX))
		{
			m_wImageIndex++;
		}

		//��̬����
		if ((m_bHovering==false)&&(m_wImageIndex>MIN_TRANSITION_INDEX))
		{
			m_wImageIndex--;
		}

		//ɾ��ʱ��
		if ((m_wImageIndex==MIN_TRANSITION_INDEX)||(m_wImageIndex==MAX_TRANSITION_INDEX))
		{
			KillTimer(IDI_TRANSITION);
		}

		//���½���
		Invalidate(FALSE);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//������Ϣ
INT CSkinButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ð�ť
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);

	//�����ؼ�
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//��������
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
	SendMessage(WM_SETFONT,(WPARAM)(HFONT)(pSkinResourceManager->GetDefaultFont()),TRUE);

	return 0;
}

//������Ⱦ
VOID CSkinButton::SetRenderImage(bool bRenderImage)
{
	//���ñ���
	m_bRenderImage=bRenderImage;

	//���´���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//������ɫ
VOID CSkinButton::SetButtonColor(COLORREF crButtonText)
{
	//���ñ���
	m_crButtonText=crButtonText;

	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//����λͼ
bool CSkinButton::SetButtonImage(UINT uBitmapID, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent)
{
	//���ñ���
	m_bRenderImage=bRenderImage;
	m_bTransparent=bTransparent;

	//������Դ
	m_hResInstance=hResInstance;
	m_pszResource=MAKEINTRESOURCE(uBitmapID);

	//�����ؼ�
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//���´���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//����λͼ
bool CSkinButton::SetButtonImage(LPCTSTR pszResource, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent)
{
	//���ñ���
	m_bRenderImage=bRenderImage;
	m_bTransparent=bTransparent;

	//������Դ
	m_pszResource=pszResource;
	m_hResInstance=hResInstance;

	//�����ؼ�
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//���´���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//����λͼ
bool CSkinButton::SetButtonImage(UINT uBitmapID, LPCTSTR pszSurface, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent)
{
	//���ñ���
	m_bRenderImage=bRenderImage;
	m_bTransparent=bTransparent;

	//������Դ
	m_pszSurface=pszSurface;
	m_hResInstance=hResInstance;
	m_pszResource=MAKEINTRESOURCE(uBitmapID);

	//�����ؼ�
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//���´���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//����λͼ
bool CSkinButton::SetButtonImage(LPCTSTR pszResource, LPCTSTR pszSurface, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent)
{
	//������Դ
	m_pszSurface=pszSurface;
	m_pszResource=pszResource;
	m_hResInstance=hResInstance;

	//���ñ���
	m_bRenderImage=bRenderImage;
	m_bTransparent=bTransparent;

	//�����ؼ�
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//���´���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//����λ��
VOID CSkinButton::RectifyControl()
{
	//�����ж�
	if (m_hWnd==NULL) return;

	//����λ��
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		//������Դ
		CBitImage ImageButton;
		ImageButton.LoadFromResource(m_hResInstance,m_pszResource);

		//������С
		SetWindowPos(NULL,0,0,ImageButton.GetWidth()/5,ImageButton.GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

		//͸������
		if (m_bTransparent==true)
		{
			//��������
			CRgn RgnControl;
			ImageButton.CreateImageRegion(RgnControl,RGB(255,0,255));

			//��������
			SetWindowRgn(RgnControl,TRUE);
		}
	}
	else
	{
		//������Դ
		CBitImage ImageButton;
		ImageButton.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_BUTTON);

		//������С
		SetWindowPos(NULL,0,0,ImageButton.GetWidth()/5,ImageButton.GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}

	return;
}

//�滭����
VOID CSkinButton::DrawButtonText(CDC * pDC, UINT uItemState)
{
	//��ȡ����
	CString strText;
	GetWindowText(strText);

	//�滭����
	if (strText.IsEmpty()==FALSE)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//��ť״̬
		bool bDisable=((uItemState&ODS_DISABLED)!=0);
		bool bButtonDown=((uItemState&ODS_SELECTED)!=0);

		//���û���
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor((bDisable==true)?GetSysColor(COLOR_GRAYTEXT):m_crButtonText);

		//�滭����
		rcClient.top+=(bButtonDown==true)?3:1;
		pDC->DrawText(strText,strText.GetLength(),rcClient,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	return;
}

//�����Ϣ
VOID CSkinButton::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//״̬����
	if (m_bHovering==false)
	{
		//���ñ���
		m_bHovering=true;

		//���½���
		Invalidate(FALSE);

		//��������
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		::TrackMouseEvent(&TrackMouseEvent);

		//���ɴ���
		if (m_wImageIndex<MAX_TRANSITION_INDEX)
		{
			SetTimer(IDI_TRANSITION,TIME_TRANSITION,NULL);
		}
	}

	return;
}

//�����Ϣ
LRESULT CSkinButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	//���ñ���
	m_bHovering=false;

	//���ɴ���
	if (m_wImageIndex>MIN_TRANSITION_INDEX)
	{
		SetTimer(IDI_TRANSITION,TIME_TRANSITION,NULL);
	}

	//���½���
	Invalidate(FALSE);

	return 0;
}

//�����Ϣ
BOOL CSkinButton::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//���ù��
	HINSTANCE hResInstance=GetModuleHandle(SKIN_CONTROL_DLL_NAME);
	SetCursor(LoadCursor(hResInstance,MAKEINTRESOURCE(IDC_SKIN_CURSOR)));

	return TRUE;
}

//������Ϣ
VOID CSkinButton::OnNcDestroy()
{
	//״̬����
	m_bHovering=false;
	m_bRenderImage=true;

	//��������
	m_wImageIndex=MIN_TRANSITION_INDEX;

	__super::OnNcDestroy();
}

//�滭����
BOOL CSkinButton::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//�滭����
VOID CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	HDC hDC=lpDrawItemStruct->hDC;
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	//��ť״̬
	bool bDisable=((lpDrawItemStruct->itemState&ODS_DISABLED)!=0);
	bool bButtonDown=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);

	//������Դ
	CBitImage ImageButton;
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		//������Դ
		ImageButton.LoadFromResource(m_hResInstance,m_pszResource);

		//��Ⱦ��Դ
		if (m_bRenderImage==true)
		{
			CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
			if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(ImageButton);
		}

		//�滭��ť
		if (m_pszSurface!=NULL)
		{
			//������Դ
			CPngImage ImageSurface;
			ImageSurface.LoadImage(m_hResInstance,m_pszSurface);

			//�滭����
			CDC * pDCImage=CDC::FromHandle(ImageButton.GetDC());
			ImageSurface.DrawImage(pDCImage,0,0,ImageButton.GetWidth(),ImageButton.GetHeight(),0,0);

			//�ͷ���Դ
			ImageButton.ReleaseDC();
		}
	}
	else
	{
		//������Դ
		ImageButton.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_BUTTON);

		//��Ⱦ��Դ
		CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
		if ((pSkinRenderManager!=NULL)&&(m_bRenderImage==true)) pSkinRenderManager->RenderImage(ImageButton);
	}

	//�滭����
	if (ImageButton.IsNull()==false)
	{
		//��������
		INT nImagePos=0;
		INT nPartWidth=ImageButton.GetWidth()/5;

		//����λ��
		if (bDisable==true) nImagePos=nPartWidth*4;
		if ((bDisable==false)&&(bButtonDown==true)) nImagePos=nPartWidth;
		if ((bDisable==false)&&(bButtonDown==false)&&(m_bHovering==true)) nImagePos=nPartWidth*3;

		//״̬�ж�
		bool bTransitionDraw=false;
		if ((m_bHovering==true)&&(m_wImageIndex<MAX_TRANSITION_INDEX)) bTransitionDraw=true;
		if ((m_bHovering==false)&&(m_wImageIndex>MIN_TRANSITION_INDEX)) bTransitionDraw=true;

		//�滭����
		if ((bDisable==false)&&(bButtonDown==false)&&(bTransitionDraw==true))
		{
			//��������
			CBitImage ImageBuffer;
			ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

			//��ȡ�豸
			CImageDC BufferDC(ImageBuffer);
			CDC * pBufferDC=CDC::FromHandle(BufferDC);

			//�����豸
			pBufferDC->SetBkMode(TRANSPARENT);
			pBufferDC->SelectObject(pDC->GetCurrentFont());

			//�滭��ͼ
			if (m_wImageIndex<MAX_TRANSITION_INDEX)
			{
				if ((rcClient.Width()==nPartWidth)&&(rcClient.Height()==ImageButton.GetHeight()))
				{
					ImageButton.BitBlt(BufferDC,0,0,rcClient.Width(),rcClient.Height(),0,0,SRCCOPY);
				}
				else
				{
					ImageButton.StretchBlt(BufferDC,0,0,rcClient.Width(),rcClient.Height(),0,0,nPartWidth,ImageButton.GetHeight(),SRCCOPY);
				}
			}

			//�滭����
			if (m_wImageIndex>MIN_TRANSITION_INDEX)
			{
				if (m_bTransparent==false)
				{
					ImageButton.AlphaDrawImage(pBufferDC,0,0,rcClient.Width(),rcClient.Height(),
						nPartWidth*3,0,nPartWidth,ImageButton.GetHeight(),m_wImageIndex*255/MAX_TRANSITION_INDEX);
				}
				else
				{
					ImageButton.BlendDrawImage(pBufferDC,0,0,rcClient.Width(),rcClient.Height(),
						nPartWidth*3,0,nPartWidth,ImageButton.GetHeight(),RGB(255,0,255),m_wImageIndex*255/MAX_TRANSITION_INDEX);
				}
			}

			//�滭����
			DrawButtonText(pBufferDC,lpDrawItemStruct->itemState);

			//�滭����
			if (m_bTransparent==false)
			{
				ImageBuffer.BitBlt(hDC,0,0,rcClient.Width(),rcClient.Height(),0,0);
			}
			else
			{
				ImageBuffer.TransDrawImage(pDC,0,0,rcClient.Width(),rcClient.Height(),0,0,RGB(255,0,255));
			}

			return;
		}
		else
		{
			//��ͨģʽ
			if (m_bTransparent==false)
			{
				if ((rcClient.Width()==nPartWidth)&&(rcClient.Height()==ImageButton.GetHeight()))
				{
					ImageButton.BitBlt(hDC,0,0,rcClient.Width(),rcClient.Height(),nImagePos,0,SRCCOPY);
				}
				else
				{
					ImageButton.StretchBlt(hDC,0,0,rcClient.Width(),rcClient.Height(),nImagePos,0,nPartWidth,ImageButton.GetHeight(),SRCCOPY);
				}
			}
			else
			{
				ImageButton.TransDrawImage(pDC,0,0,rcClient.Width(),rcClient.Height(),nImagePos,0,nPartWidth,ImageButton.GetHeight(),RGB(255,0,255));
			}

			//�滭����
			DrawButtonText(pDC,lpDrawItemStruct->itemState);
		}
	}
	else
	{
		//Ĭ�Ͻ���
		if (bDisable==true) pDC->DrawFrameControl(&rcClient,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_INACTIVE);
		if ((bDisable==false)&&(bButtonDown==false)) pDC->DrawFrameControl(&rcClient,DFC_BUTTON,DFCS_BUTTONPUSH);
		if ((bDisable==false)&&(bButtonDown==true)) pDC->DrawFrameControl(&rcClient,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);

		//�滭����
		DrawButtonText(pDC,lpDrawItemStruct->itemState);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////