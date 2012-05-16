#include "StdAfx.h"
#include "Resource.h"
#include "SkinWndObject.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
CSkinWndAttribute		CSkinWndObject::m_SkinAttribute;				//��������

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinWndAttribute::CSkinWndAttribute() 
{
	//���ñ���
	m_nButtonWidth=0;
	m_nButtonHeigth=0;
	m_nCaptionHeigth=0;

	m_crBackFrame=RGB(0,0,0);
	m_crBackGround=RGB(0,0,0);
	m_crInsideBorder=RGB(0,0,0);
	m_crCaptionTXColor=RGB(0,0,0);	
	m_crControlTXColor=RGB(0,0,0);	

	return;
}

//��������
CSkinWndAttribute::~CSkinWndAttribute()
{
}

//��������
bool CSkinWndAttribute::UpdateSkinResource(ISkinResource * pISkinResource)
{
	//��ȡ��Ϣ
	tagSkinDialogResource SkinDialogResource;
	if (pISkinResource!=NULL) pISkinResource->GetResourceInfo(SkinDialogResource);

	//������Դ
	m_TitleLeft.SetLoadInfo(SkinDialogResource.LoadInfoImageTL);
	m_TitleMid.SetLoadInfo(SkinDialogResource.LoadInfoImageTM);
	m_TitleRight.SetLoadInfo(SkinDialogResource.LoadInfoImageTR);
	m_ButtonMin.SetLoadInfo(SkinDialogResource.LoadInfoButtonMin);
	m_ButtonMax.SetLoadInfo(SkinDialogResource.LoadInfoButtonMax);
	m_ButtonRes.SetLoadInfo(SkinDialogResource.LoadInfoButtonRes);
	m_ButtonClose.SetLoadInfo(SkinDialogResource.LoadInfoButtonClose);

	//��ɫ��Ϣ
	m_crBackGround=SkinDialogResource.crBackGround;
	m_crInsideBorder=SkinDialogResource.crInsideBorder;
	m_crBackFrame=SkinDialogResource.crBackFrame;
	m_crCaptionTXColor=SkinDialogResource.crTitleText;
	m_crControlTXColor=SkinDialogResource.crControlText;

	//��������
	m_brBackFrame.DeleteObject();
	m_brBackGround.DeleteObject();
	m_brBackFrame.CreateSolidBrush(m_crBackFrame);
	m_brBackGround.CreateSolidBrush(m_crBackGround);

	//���ñ���
	CImageHandle TitleHandle(&m_TitleLeft);
	CImageHandle ButtonHandle(&m_ButtonClose);
	if (TitleHandle.IsResourceValid()) m_nCaptionHeigth=TitleHandle->GetHeight();
	else m_nCaptionHeigth=0;
	if (ButtonHandle.IsResourceValid())
	{
		m_nButtonHeigth=ButtonHandle->GetHeight();
		m_nButtonWidth=ButtonHandle->GetWidth()/4;
	}
	else 
	{
		m_nButtonHeigth=0;
		m_nButtonWidth=0;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinWndObject::CSkinWndObject(CWnd * pWndSkin)
{
	m_bActive=false;
	m_bMaxSize=false;
	m_nXExcursionPos=0;
	m_nYExcursionPos=0;
	m_pWndHook=pWndSkin;
	m_cbNowHotButton=255;
	m_cbNowDownButton=255;
	memset(m_cbButtonState,0,sizeof(m_cbButtonState));
}

//��������
CSkinWndObject::~CSkinWndObject(void)
{
}

//��ʼ��
void CSkinWndObject::InitSkinObject()
{
	//���ñ���
	m_bActive=false;
	m_nXExcursionPos=0;
	m_nYExcursionPos=0;
	m_cbNowHotButton=255;
	m_cbNowDownButton=255;
	memset(m_cbButtonState,0,sizeof(m_cbButtonState));

	//��������
	m_pWndHook->ModifyStyle(WS_CAPTION,0,SWP_DRAWFRAME);
	m_pWndHook->GetWindowRect(&m_crNormalSize);
	LONG lWindowStyte=GetWindowLongPtr(m_pWndHook->m_hWnd,GWL_STYLE);
	if (lWindowStyte&WS_SYSMENU)
	{
		m_cbButtonState[BST_CLOSE]=BUT_EXIST|BUT_NORMAL;
		if ((lWindowStyte&WS_MAXIMIZEBOX)||(lWindowStyte&WS_MINIMIZEBOX)) 
		{
			m_cbButtonState[BST_MAX]=BUT_EXIST|BUT_DISABLE;
			m_cbButtonState[BST_MIN]=BUT_EXIST|BUT_DISABLE;
			if (lWindowStyte&WS_MAXIMIZEBOX) m_cbButtonState[BST_MAX]=BUT_EXIST|BUT_NORMAL;
			else m_cbButtonState[BST_MAX]=BUT_EXIST|BUT_DISABLE;
			if (lWindowStyte&WS_MINIMIZEBOX) m_cbButtonState[BST_MIN]=BUT_EXIST|BUT_NORMAL;
			else m_cbButtonState[BST_MIN]=BUT_EXIST|BUT_DISABLE;
		}
	}

	//������ֵ
	CRect ClientRect;
	m_pWndHook->GetClientRect(&ClientRect);
	AdjustTitleButton(ClientRect.Width(),ClientRect.Height());
	m_nYExcursionPos=m_SkinAttribute.m_nCaptionHeigth+1;
	m_nXExcursionPos=1;

	return;
}

//������ť
void CSkinWndObject::AdjustTitleButton(int nCXWindow, int nCYWindow)
{
	for (int i=0;i<3;i++)
	{
		int nYPos=2;
		int nXPos=nCXWindow-m_SkinAttribute.m_nButtonWidth*(i+1)-i-5;
		m_rcButton[i].left=nXPos;
		m_rcButton[i].top=nYPos;
		m_rcButton[i].right=nXPos+m_SkinAttribute.m_nButtonWidth;
		m_rcButton[i].bottom=nYPos+m_SkinAttribute.m_nButtonHeigth;
	}

	return;
}

//�滭����
void CSkinWndObject::DrawSkinView(CDC * pDC)
{
	//��ȡ����
	CRect ClientRect;
	m_pWndHook->GetClientRect(&ClientRect);

	//��������ͼ
	CSkinImage CaptionImage;
	CaptionImage.Create(ClientRect.Width()-2*m_nXExcursionPos,m_SkinAttribute.m_nCaptionHeigth,16);
	if (CaptionImage.IsNull()) return;

	//�滭����
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(NULL);
	BufferDC.SelectObject(CaptionImage);
	DrawCaption(&BufferDC,CaptionImage.GetWidth(),CaptionImage.GetHeight());

	//���� DC
	if (pDC==NULL)
	{
		CClientDC ClientDC(m_pWndHook);
		ClientDC.BitBlt(m_nXExcursionPos,1,CaptionImage.GetWidth(),CaptionImage.GetHeight(),&BufferDC,0,0,SRCCOPY);
		ClientDC.Draw3dRect(&ClientRect,RGB(0,0,0),RGB(0,0,0));
		BufferDC.DeleteDC();
	}
	else
	{
		pDC->BitBlt(m_nXExcursionPos,1,CaptionImage.GetWidth(),CaptionImage.GetHeight(),&BufferDC,0,0,SRCCOPY);
		pDC->Draw3dRect(&ClientRect,RGB(0,0,0),RGB(0,0,0));
	}

	//��ȡ����
	TCHAR strTitle[128];
	GetWindowText(*m_pWndHook,strTitle,CountArray(strTitle));

	//����λ��
	INT nYPos=(m_SkinAttribute.m_nCaptionHeigth-12)/2+2;
	INT nXPos=46;

	//��������ͼ
	CClientDC ClientDC(m_pWndHook);
	ClientDC.SetBkMode(TRANSPARENT);
	ClientDC.SelectObject(m_SkinAttribute.m_DefaultFont);
	ClientDC.SetTextAlign(TA_LEFT);

	//��������
	int nStringLength=lstrlen(strTitle);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	ClientDC.SetTextColor(RGB(0,112,192));
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		ClientDC.TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],strTitle,nStringLength);
	}

	//�滭����m_SkinAttribute.m_crCaptionTXColor
	ClientDC.SetTextColor(RGB(255,255,255));
	ClientDC.TextOut(nXPos,nYPos,strTitle);

	BufferDC.DeleteDC();

	return;
}

//��󴰿�
void CSkinWndObject::MaxSizeWindow()
{
	if (m_bMaxSize==false)
	{
		//����λ��
		CRect rcClient;
		m_pWndHook->GetClientRect(&rcClient);
		m_pWndHook->ClientToScreen(&rcClient);
		m_pWndHook->GetWindowRect(&m_crNormalSize);

		//�ƶ�����
		CRect rcRect;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcRect,SPIF_SENDCHANGE);
		rcRect.top-=(rcClient.top-m_crNormalSize.top);
		rcRect.bottom+=(m_crNormalSize.bottom-rcClient.bottom);
		rcRect.left-=(rcClient.left-m_crNormalSize.left);
		rcRect.right+=(m_crNormalSize.right-rcClient.right);
		m_pWndHook->MoveWindow(&rcRect,FALSE);
		m_pWndHook->Invalidate(FALSE);

		//���ñ���
		m_bMaxSize=true;
	}

	return;
}

//��ԭ����
void CSkinWndObject::RestoreWindow()
{
	if (m_bMaxSize==true)
	{
		//�ƶ�����
		m_pWndHook->MoveWindow(&m_crNormalSize,TRUE);

		//���ñ���
		m_bMaxSize=false;
		DrawSkinView(NULL);
	}
	return;
}

//�滭������
bool CSkinWndObject::DrawCaption(CDC * pDC, int cx, int cy)
{
	//����λͼ
	CImageHandle TitleLeft(&m_SkinAttribute.m_TitleLeft);
	if (m_SkinAttribute.m_TitleLeft.IsNull()) return false;
	CImageHandle TitleMid(&m_SkinAttribute.m_TitleMid);
	if (m_SkinAttribute.m_TitleMid.IsNull()) return false;
	CImageHandle TitleRight(&m_SkinAttribute.m_TitleRight);
	if (m_SkinAttribute.m_TitleRight.IsNull()) return false;

	//�滭����
	int nLeftWidth=TitleLeft->GetWidth();
	int nMidWidth=TitleMid->GetWidth();
	int nRightWidth=TitleRight->GetWidth();
	int nXMinDrawPause=cx-nRightWidth;
	TitleLeft->BitBlt(pDC->m_hDC,0,0);
	for (int i=nLeftWidth;i<nXMinDrawPause;i+=nMidWidth) TitleMid->BitBlt(pDC->m_hDC,i,0);
	TitleRight->BitBlt(pDC->m_hDC,nXMinDrawPause,0);
	
	//�滭����
	CString strTitle;
	CRect rcTitleRect(45,4,cx-100,cy-2);
	m_pWndHook->GetWindowText(strTitle);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_SkinAttribute.m_DefaultFont);
	pDC->SetTextColor(m_SkinAttribute.m_crCaptionTXColor);
	pDC->DrawText(strTitle,&rcTitleRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	//�رհ�ť
	if (m_cbButtonState[BST_CLOSE]&BUT_EXIST)
	{
		int nDrawPos=0;
		CImageHandle ImageHandle(&m_SkinAttribute.m_ButtonClose);
		if (m_cbButtonState[BST_CLOSE]&BUT_DISABLE)
		{
			nDrawPos=ImageHandle->GetWidth()*3/4;
		}
		else if (m_cbNowDownButton==BST_CLOSE)
		{
			nDrawPos=ImageHandle->GetWidth()*2/4;
		}
		else if (m_cbNowHotButton==BST_CLOSE) 
		{
			nDrawPos=ImageHandle->GetWidth()/4;
		}
		ImageHandle->BitBlt(pDC->GetSafeHdc(),m_rcButton[BST_CLOSE].left,m_rcButton[BST_CLOSE].top,
			m_SkinAttribute.m_nButtonWidth,m_SkinAttribute.m_nButtonHeigth,nDrawPos,0);
	}

	//��󻯺ͻ�ԭ��ť
	if (m_cbButtonState[BST_MAX]&BUT_EXIST)
	{
		int nDrawPos=0;
		CImageHandle ImageHandle;
		if (m_bMaxSize==true) ImageHandle.AttachResource(&m_SkinAttribute.m_ButtonRes);
		else ImageHandle.AttachResource(&m_SkinAttribute.m_ButtonMax);
		if (m_cbButtonState[BST_MAX]&BUT_DISABLE)
		{
			nDrawPos=ImageHandle->GetWidth()*3/4;
		}
		if (m_cbNowDownButton==BST_MAX)
		{
			nDrawPos=ImageHandle->GetWidth()*2/4;
		}
		else if (m_cbNowHotButton==BST_MAX)
		{
			nDrawPos=ImageHandle->GetWidth()/4;
		}
		ImageHandle->BitBlt(pDC->GetSafeHdc(),m_rcButton[BST_MAX].left,m_rcButton[BST_MAX].top,
			m_SkinAttribute.m_nButtonWidth,m_SkinAttribute.m_nButtonHeigth,nDrawPos,0);
	}

	//��С����ť
	if (m_cbButtonState[BST_MIN]&BUT_EXIST)	
	{
		int nDrawPos=0;
		CImageHandle ImageHandle(&m_SkinAttribute.m_ButtonMin);
		if (m_cbButtonState[BST_MIN]&BUT_DISABLE)
		{
			nDrawPos=ImageHandle->GetWidth()*3/4;
		}
		if (m_cbNowDownButton==BST_MIN)
		{
			nDrawPos=ImageHandle->GetWidth()*2/4;
		}
		else if (m_cbNowHotButton==BST_MIN) 
		{
			nDrawPos=ImageHandle->GetWidth()/4;
		}
		ImageHandle->BitBlt(pDC->GetSafeHdc(),m_rcButton[BST_MIN].left,m_rcButton[BST_MIN].top,
			m_SkinAttribute.m_nButtonWidth,m_SkinAttribute.m_nButtonHeigth,nDrawPos,0);
	}

	return true;	
}

//���øı��¼�
void CSkinWndObject::OnSettingChange()
{
	//�ƶ�����
	if (m_bMaxSize==true)
	{
		//����λ��
		CRect rcClient;
		m_pWndHook->GetClientRect(&rcClient);
		m_pWndHook->ClientToScreen(&rcClient);
		m_pWndHook->GetWindowRect(&m_crNormalSize);

		//�ƶ�����
		CRect rcRect;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcRect,SPIF_SENDCHANGE);
		rcRect.top-=(rcClient.top-m_crNormalSize.top);
		rcRect.bottom+=(m_crNormalSize.bottom-rcClient.bottom);
		rcRect.left-=(rcClient.left-m_crNormalSize.left);
		rcRect.right+=(m_crNormalSize.right-rcClient.right);
		m_pWndHook->MoveWindow(&rcRect,FALSE);
		m_pWndHook->Invalidate(FALSE);
	}

	return;
}

//����ƶ��¼�
bool CSkinWndObject::OnMouseMoveEvent(CPoint ClientPoint)
{
	//���ù��
	SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));

	//�жϰ���
	if (m_cbNowDownButton!=255) return false;	
	
	//��ť����
	for (int i=0;i<3;i++)
	{
		if ((m_cbButtonState[i]&BUT_EXIST)&&(m_rcButton[i].PtInRect(ClientPoint)))
		{
			if (m_cbNowHotButton==i) return true;
			if (m_cbNowHotButton!=255)
			{
				m_cbButtonState[m_cbNowHotButton]|=BUT_NORMAL;
				m_cbNowHotButton=255;
			}
			if ((m_cbButtonState[i]&BUT_EXIST)&&(!(m_cbButtonState[i]&BUT_DISABLE)))
			{
				m_cbNowHotButton=i;
				m_cbButtonState[m_cbNowHotButton]&=~BUT_NORMAL;
			}
			DrawSkinView(NULL);
			return true;
		}
	}

	//����ɽ���
	if (m_cbNowHotButton!=255)
	{
		m_cbButtonState[m_cbNowHotButton]|=BUT_NORMAL;
		m_cbNowHotButton=255;
		DrawSkinView(NULL);
	}

	return true;
}

//�������¼�
bool CSkinWndObject::OnLMouseDownEvent(CPoint ClientPoint)
{
	//�жϰ���
	if (m_cbNowDownButton!=255) return false;

	//��ť����
	for (int i=0;i<3;i++)
	{
		if ((m_cbButtonState[i]&BUT_EXIST)&&(!(m_cbButtonState[i]&BUT_DISABLE))
			&&(m_rcButton[i].PtInRect(ClientPoint)))
		{
			//��������
			m_pWndHook->SetCapture();
			m_cbNowDownButton=i;
			DrawSkinView(NULL);
			return true;
		}
	}

	//ģ�ⰴ����
	if (m_bMaxSize==false)
	{
		if (ClientPoint.y<GetYExcursionPos())
		{
			m_pWndHook->PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(ClientPoint.x,ClientPoint.y));
			return true;
		}
		return false;
	}

	return true;
}

//�������¼�
bool CSkinWndObject::OnLMouseUpEvent(CPoint ClientPoint)
{
	//��ť����
	if (m_cbNowDownButton!=255)
	{
		ReleaseCapture();
		if ((m_rcButton[m_cbNowDownButton].PtInRect(ClientPoint))
			&&(!(m_cbButtonState[m_cbNowDownButton]&BUT_DISABLE)))
		{
			switch (m_cbNowDownButton)
			{
			case BST_CLOSE:		//�رհ�ť
				{
					m_cbNowDownButton=255;
					m_pWndHook->PostMessage(WM_CLOSE,0,0);
					return true;
				}
			case BST_MAX:		//��ԭ��ť
				{
					m_cbNowDownButton=255;
					if (m_bMaxSize==true) RestoreWindow();
					else MaxSizeWindow();
					return true;
				}
			case BST_MIN:		//��С����ť
				{
					m_cbNowDownButton=255;
					m_pWndHook->ShowWindow(SW_MINIMIZE);
					return true;
				}
			}
		}

		//Ĭ�ϲ���
		m_cbNowDownButton=255;
		DrawSkinView(NULL);
	}

	return true;
}

//�������¼�
bool CSkinWndObject::OnLMouseDoubleEvent(CPoint ClientPoint)
{
	//�жϰ���
	if (m_cbNowDownButton!=255) return false;	

	//��ť����
	for (int i=0;i<3;i++)
	{
		if ((m_cbButtonState[i]&BUT_EXIST)&&(m_rcButton[i].PtInRect(ClientPoint)))
			return true;
	}

	//��󻯴���
	if ((ClientPoint.y<GetYExcursionPos())&&(m_cbButtonState[BST_MAX]&BUT_EXIST)&&
		(!(m_cbButtonState[BST_MAX]&BUT_DISABLE)))
	{
		if (m_bMaxSize==true) RestoreWindow();
		else MaxSizeWindow();
	}
	
	return true;
}

//����Ҽ��¼�
bool CSkinWndObject::OnRMouseDownEvent(CPoint ClientPoint)
{
	return true;
}

//����Ҽ��¼�
bool CSkinWndObject::OnRMouseUpEvent(CPoint ClientPoint)
{
	return true;
}

//����Ҽ��¼�
bool CSkinWndObject::OnRMouseDoubleEvent(CPoint ClientPoint)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
