#include "StdAfx.h"
#include "PlatformEvent.h"
#include "DlgSkinControl.h"

//////////////////////////////////////////////////////////////////////////////////

//���ڶ���
#define CONTRL_CX					139									//���ڿ��
#define CONTRL_CY					190									//���ڸ߶�

//����λ��
#define ITEM_ORIGIN_X				15									//ԭ��λ��
#define ITEM_ORIGIN_Y				35									//ԭ��λ��

//��ɫԭ��
#define COLOR_ORIGIN_X				8									//ԭ��λ��
#define COLOR_ORIGIN_Y				125									//ԭ��λ��

//��϶λ��
#define ITEM_SPACE_CX				10									//��϶λ��
#define ITEM_SPACE_CY				10									//��϶λ��
#define COLOR_SPACE_CY				16									//��϶λ��

//ɫ�෶Χ
#define MIN_COLOR_H					0									//��Сɫ��
#define MAX_COLOR_H					360									//���ɫ��

//���ͷ�Χ
#define MAX_COLOR_S					100									//��󱥺�
#define MIN_COLOR_S					-100								//��С����

//���ȷ�Χ
#define MAX_COLOR_B					20									//�������
#define MIN_COLOR_B					-20									//��С����

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSkinControl, CDialog)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgSkinControl::CDlgSkinControl()
{
	//��ɫ����
	m_RenderInfoItem[0].wColorH=DEF_COLOR_H;
	m_RenderInfoItem[0].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[0].dRectifyB=DEF_COLOR_B;

	//��������
	m_RenderInfoItem[1].wColorH=207;
	m_RenderInfoItem[1].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[1].dRectifyB=DEF_COLOR_S;

	//��ɫ����
	m_RenderInfoItem[2].wColorH=279;
	m_RenderInfoItem[2].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[2].dRectifyB=DEF_COLOR_S;

	//��ɫ����
	m_RenderInfoItem[3].wColorH=101;
	m_RenderInfoItem[3].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[3].dRectifyB=DEF_COLOR_S;

	//��ɫ����
	m_RenderInfoItem[4].wColorH=30;
	m_RenderInfoItem[4].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[4].dRectifyB=DEF_COLOR_S;

	//���ȷ���
	m_RenderInfoItem[5].wColorH=14;
	m_RenderInfoItem[5].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[5].dRectifyB=DEF_COLOR_S;

	//�ۺ췽��
	m_RenderInfoItem[6].wColorH=321;
	m_RenderInfoItem[6].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[6].dRectifyB=DEF_COLOR_S;

	//��ɫ����
	m_RenderInfoItem[7].wColorH=0;
	m_RenderInfoItem[7].dRectifyS=-1.00;
	m_RenderInfoItem[7].dRectifyB=-0.20;

	//���ñ���
	m_wHoverItem=INVALID_WORD;
	m_wHoverColor=INVALID_WORD;
	m_wMouseDownColor=INVALID_WORD;

	//��ȡ��С
	CPngImage ImageSkinItem;
	ImageSkinItem.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_ITEM"));
	m_SizeSkinItem.SetSize(ImageSkinItem.GetWidth()/8,ImageSkinItem.GetHeight()/2);

	//��ȡ��С
	CPngImage ImageColorItem;
	ImageColorItem.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_COLOR"));
	m_SizeColorItem.SetSize(ImageColorItem.GetWidth(),ImageColorItem.GetHeight()/3);

	return;
}

//��������
CDlgSkinControl::~CDlgSkinControl()
{
}

//��������
BOOL CDlgSkinControl::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	m_wHoverItem=INVALID_WORD;
	m_wHoverColor=INVALID_WORD;
	m_wMouseDownColor=INVALID_WORD;
	m_CurrentRenderInfo=CSkinRenderManager::GetInstance()->GetSkinRenderInfo();

	//�ƶ�����
	SetWindowPos(NULL,0,0,CONTRL_CX,CONTRL_CY,SWP_NOZORDER|SWP_NOMOVE);

	return TRUE;
}

//��ȡѡ��
WORD CDlgSkinControl::GetCurrentItem(CPoint MousePoint)
{
	//λ������
	for (WORD nXPos=0;nXPos<4;nXPos++)
	{
		for (WORD nYPos=0;nYPos<2;nYPos++)
		{
			//λ���ж�
			if (MousePoint.x<nXPos*(m_SizeSkinItem.cx+ITEM_SPACE_CX)+ITEM_ORIGIN_X) continue;
			if (MousePoint.y<nYPos*(m_SizeSkinItem.cy+ITEM_SPACE_CY)+ITEM_ORIGIN_Y) continue;
			if (MousePoint.x>nXPos*(m_SizeSkinItem.cx+ITEM_SPACE_CX)+ITEM_ORIGIN_X+m_SizeSkinItem.cx) continue;
			if (MousePoint.y>nYPos*(m_SizeSkinItem.cy+ITEM_SPACE_CY)+ITEM_ORIGIN_Y+m_SizeSkinItem.cy) continue;

			return nYPos*4+nXPos;
		}
	}

	return INVALID_WORD;
}

//��ȡѡ��
WORD CDlgSkinControl::GetCurrentColor(CPoint MousePoint)
{
	//λ������
	for (WORD nIndex=0;nIndex<3;nIndex++)
	{
		//λ���ж�
		if (MousePoint.x<COLOR_ORIGIN_X) continue;
		if (MousePoint.x>(COLOR_ORIGIN_X+m_SizeColorItem.cx)) continue;
		if (MousePoint.y<(COLOR_ORIGIN_Y+(m_SizeColorItem.cy+COLOR_SPACE_CY)*nIndex)) continue;
		if (MousePoint.y>(COLOR_ORIGIN_Y+(m_SizeColorItem.cy+COLOR_SPACE_CY)*nIndex+m_SizeColorItem.cy)) continue;

		return nIndex;
	}

	return INVALID_WORD;
}

//������ɫ
VOID CDlgSkinControl::SetCurrentColor(WORD wColorIndex, INT nXExcursion)
{
	//���ñ���
	switch (wColorIndex)
	{
	case 0:		//ɫ��
		{
			WORD wHRange=MAX_COLOR_H-MIN_COLOR_H;
			m_CurrentRenderInfo.wColorH=(WORD)(nXExcursion*wHRange/m_SizeColorItem.cx+MIN_COLOR_H);
			break;
		}
	case 1:		//����
		{
			INT nSRange=MAX_COLOR_S-MIN_COLOR_S;
			m_CurrentRenderInfo.dRectifyS=(nXExcursion*nSRange/m_SizeColorItem.cx+MIN_COLOR_S)/100.0;
			break;
		}
	case 2:		//����
		{
			INT nBRange=MAX_COLOR_B-MIN_COLOR_B;
			m_CurrentRenderInfo.dRectifyB=(nXExcursion*nBRange/m_SizeColorItem.cx+MIN_COLOR_B)/100.0;
			break;
		}
	}

	return;
}

//�ػ���Ϣ
VOID CDlgSkinControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//��������
	CDC * pDC=CDC::FromHandle(ImageBuffer.GetDC());
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//�滭����
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
	pDC->Draw3dRect(&rcClient,CSkinEdit::m_SkinAttribute.m_crEnableBorad,CSkinEdit::m_SkinAttribute.m_crEnableBorad);

	//������Դ
	CPngImage ImageSkinItem;
	ImageSkinItem.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_ITEM"));

	//�滭����
	for (WORD nXPos=0;nXPos<4;nXPos++)
	{
		for (WORD nYPos=0;nYPos<2;nYPos++)
		{
			//��������
			WORD wIndex=nYPos*4+nXPos;
			WORD wImageIndex=(m_wHoverItem==wIndex)?1:0;
			tagSkinRenderInfo & SkinRenderInfo=m_RenderInfoItem[wIndex];

			//�滭����
			ImageSkinItem.DrawImage(pDC,nXPos*(m_SizeSkinItem.cx+ITEM_SPACE_CX)+ITEM_ORIGIN_X,
				nYPos*(m_SizeSkinItem.cy+ITEM_SPACE_CY)+ITEM_ORIGIN_Y,m_SizeSkinItem.cx,m_SizeSkinItem.cy,
				(nYPos*4+nXPos)*m_SizeSkinItem.cx,wImageIndex*m_SizeSkinItem.cy);

			//�滭�߿�
			if ((m_CurrentRenderInfo.wColorH==SkinRenderInfo.wColorH)&&(m_CurrentRenderInfo.dRectifyS==SkinRenderInfo.dRectifyS)
				&&(m_CurrentRenderInfo.dRectifyB==SkinRenderInfo.dRectifyB))
			{
				INT nXDrawPos=nXPos*(m_SizeSkinItem.cx+ITEM_SPACE_CX)+ITEM_ORIGIN_X-2;
				INT nYDrawPos=nYPos*(m_SizeSkinItem.cy+ITEM_SPACE_CY)+ITEM_ORIGIN_Y-2;
				pDC->Draw3dRect(nXDrawPos,nYDrawPos,m_SizeSkinItem.cx+4,m_SizeSkinItem.cy+4,RGB(0,0,0),RGB(0,0,0));
			}
		}
	}

	//������Դ
	CPngImage ImageSkinColor;
	CPngImage ImageSkinButton;
	ImageSkinColor.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_COLOR"));
	ImageSkinButton.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_BUTTON"));

	//�滭ɫ��
	for (WORD nIndex=0;nIndex<3;nIndex++)
	{
		//�滭ɫ��
		INT nXDrawPos=COLOR_ORIGIN_X;
		INT nYDrawPos=COLOR_ORIGIN_Y+(m_SizeColorItem.cy+COLOR_SPACE_CY)*nIndex;
		ImageSkinColor.DrawImage(pDC,nXDrawPos,nYDrawPos,m_SizeColorItem.cx,m_SizeColorItem.cy,0,m_SizeColorItem.cy*nIndex);

		//�滭����
		switch (nIndex)
		{
		case 0:		//ɫ��
			{
				INT nXButtonPos=nXDrawPos+(m_SizeColorItem.cx-6)*m_CurrentRenderInfo.wColorH/(MAX_COLOR_H-MIN_COLOR_H)+3;
				ImageSkinButton.DrawImage(pDC,nXButtonPos-ImageSkinButton.GetWidth()/2,nYDrawPos-5);
				break;
			}
		case 1:		//����
			{
				DOUBLE dRectifyS=(m_CurrentRenderInfo.dRectifyS*100-MIN_COLOR_S)/(MAX_COLOR_S-MIN_COLOR_S);
				INT nXButtonPos=(INT)(nXDrawPos+(DOUBLE)(m_SizeColorItem.cx-6)*dRectifyS+3);
				ImageSkinButton.DrawImage(pDC,nXButtonPos-ImageSkinButton.GetWidth()/2,nYDrawPos-5);
				break;
			}
		case 2:		//����
			{
				DOUBLE dRectifyB=(m_CurrentRenderInfo.dRectifyB*100-MIN_COLOR_B)/(MAX_COLOR_B-MIN_COLOR_B);
				INT nXButtonPos=(INT)(nXDrawPos+(DOUBLE)(m_SizeColorItem.cx-6)*dRectifyB+3);
				ImageSkinButton.DrawImage(pDC,nXButtonPos-ImageSkinButton.GetWidth()/2,nYDrawPos-5);
				break;
			}
		}
	}

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭�ַ�
	LPCTSTR pszCustom=TEXT("���Ҷ��ƣ�");
	LPCTSTR pszRecommend=TEXT("�Ƽ���ɫ��");
	pDC->TextOut(10,102,pszCustom,lstrlen(pszCustom));
	pDC->TextOut(10,14,pszRecommend,lstrlen(pszRecommend));

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pDC,0,0,SRCCOPY);

	//������Դ
	ImageBuffer.ReleaseDC();

	return;
}

//�滭����
BOOL CDlgSkinControl::OnEraseBkgnd(CDC * pDC)
{
	//���´���
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//������Ϣ
VOID CDlgSkinControl::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	//ע������
	DestroyWindow();

	return;
}

//�����Ϣ
VOID CDlgSkinControl::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags,Point);

	//������ɫ
	if (m_wMouseDownColor!=INVALID_WORD)
	{
		//������ɫ
		INT nXExcursion=Point.x-COLOR_ORIGIN_X;
		SetCurrentColor(m_wMouseDownColor,__min(__max(nXExcursion,0),m_SizeColorItem.cx));

		//��������
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal!=NULL) pParameterGlobal->m_SkinRenderInfo=m_CurrentRenderInfo;

		//������Դ
		CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
		if (pSkinResourceManager!=NULL) pSkinResourceManager->SetSkinResource(m_CurrentRenderInfo);

		//�¼�֪ͨ
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_SKIN_CONFIG_UPDATE,0L);

		//���½���
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
VOID CDlgSkinControl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//ȡ������
	if (m_wMouseDownColor!=INVALID_WORD)
	{
		//ȡ������
		ReleaseCapture();

		//���ñ���
		m_wMouseDownColor=INVALID_WORD;
	}

	return;
}

//�����Ϣ
VOID CDlgSkinControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//������Դ
	if (m_wHoverColor!=INVALID_WORD)
	{
		//�������
		SetCapture();

		//���ñ���
		m_wMouseDownColor=m_wHoverColor;

		//������ɫ
		INT nXExcursion=Point.x-COLOR_ORIGIN_X;
		SetCurrentColor(m_wMouseDownColor,__min(__max(nXExcursion,0),m_SizeColorItem.cx));

		//��������
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal!=NULL) pParameterGlobal->m_SkinRenderInfo=m_CurrentRenderInfo;

		//������Դ
		CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
		if (pSkinResourceManager!=NULL) pSkinResourceManager->SetSkinResource(m_CurrentRenderInfo);

		//�¼�֪ͨ
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_SKIN_CONFIG_UPDATE,0L);

		//���½���
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//������Դ
	if (m_wHoverItem!=INVALID_WORD)
	{
		//���ñ���
		m_CurrentRenderInfo=m_RenderInfoItem[m_wHoverItem];

		//��������
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal!=NULL) pParameterGlobal->m_SkinRenderInfo=m_CurrentRenderInfo;

		//������Դ
		CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
		if (pSkinResourceManager!=NULL) pSkinResourceManager->SetSkinResource(m_CurrentRenderInfo);

		//�¼�֪ͨ
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_SKIN_CONFIG_UPDATE,0L);

		//���½���
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
BOOL CDlgSkinControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//����ѡ��
	WORD wHoverItem=m_wHoverItem;
	m_wHoverItem=GetCurrentItem(MousePoint);
	m_wHoverColor=GetCurrentColor(MousePoint);

	//���½���
	if (m_wHoverItem!=wHoverItem)
	{
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//���ù��
	if ((m_wHoverItem!=INVALID_WORD)||(m_wHoverColor!=INVALID_WORD))
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////////////
