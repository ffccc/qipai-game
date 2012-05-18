#include "StdAfx.h"
#include "WndViewItemCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CWndViewItemCtrl * CWndViewItemCtrl::m_pWndViewItemCtrl=NULL;			//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndViewItemCtrl, CWnd)

	//ϵͳ��Ϣ
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWndViewItemCtrl::CWndViewItemCtrl()
{
	//���Ϣ
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//��Դ��Ϣ
	m_pIViewItemEvent=NULL;
	m_RectViewItem.SetRect(0,0,0,0);

	//���ö���
	ASSERT(m_pWndViewItemCtrl==NULL);
	if (m_pWndViewItemCtrl==NULL) m_pWndViewItemCtrl=this;

	//��ȡ��С
	CImage ItemButtonImage;
	ItemButtonImage.LoadFromResource(AfxGetInstanceHandle(),IDB_ITEM_BUTTON);
	m_SizeButton.SetSize(ItemButtonImage.GetWidth()/3,ItemButtonImage.GetHeight()/2);

	//������ɫ
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	m_LinkBoradHSB=pSkinRenderManager->RGBToHSB(ItemButtonImage.GetPixel(2,m_SizeButton.cy*2-1));

	return;
}

//��������
CWndViewItemCtrl::~CWndViewItemCtrl()
{
	//���ö���
	ASSERT(m_pWndViewItemCtrl==this);
	if (m_pWndViewItemCtrl==this) m_pWndViewItemCtrl=NULL;

	return;
}

//��ȡ��С
VOID CWndViewItemCtrl::GetViewItemSize(CSize & ResultSize)
{
	//���ñ���
	ResultSize.cy=m_SizeButton.cy;
	ResultSize.cx=m_SizeButton.cx*(m_ViewItemArray.GetCount());
	
	return;
}

//�滭�ν�
VOID CWndViewItemCtrl::DrawLinkBoradLine(CWnd * pWnd, CDC * pDC)
{
	//�滭�ж�
	if (m_wItemActive==INVALID_WORD) return;
	if (m_ViewItemArray[m_wItemActive]->pItemView!=pWnd) return;

	//��ȡ����
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//�滭�ν�
	INT nXPos=m_wItemActive*m_SizeButton.cx+10;
	pDC->FillSolidRect(nXPos,0,m_SizeButton.cx-2,1,pSkinRenderManager->RenderColor(m_LinkBoradHSB.S,m_LinkBoradHSB.B));

	return;
}

//���ýӿ�
VOID CWndViewItemCtrl::SetViewItemEvent(IViewItemEvent * pIViewItemEvent)
{
	//���ýӿ�
	m_pIViewItemEvent=pIViewItemEvent;

	return;
}

//����λ��
VOID CWndViewItemCtrl::SetViewItemPostion(INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//���ñ���
	m_RectViewItem.SetRect(nXPos,nYPos,nXPos+nWidth,nYPos+nHeight);

	//��������
	if (m_wItemActive!=INVALID_WORD)
	{
		//��ȡ����
		ASSERT(m_wItemActive<m_ViewItemArray.GetCount());
		tagViewItemInfo * pViewItemInfo=m_ViewItemArray[m_wItemActive];

		//����λ��
		CWnd * pViewItem=pViewItemInfo->pItemView;
		if (pViewItem!=NULL) pViewItem->SetWindowPos(NULL,nXPos,nYPos,nWidth,nHeight,SWP_NOZORDER|SWP_NOCOPYBITS|SWP_DEFERERASE);
	}

	return;
}

//��������
VOID CWndViewItemCtrl::ActiveViewItem(CWnd * pItemView)
{
	//��������
	WORD wItemIndex=GetViewItemIndex(pItemView);
	
	//״̬�ж�
	ASSERT(wItemIndex!=INVALID_WORD);
	if (wItemIndex==INVALID_WORD) return;

	//����״̬
	if (m_wItemActive!=wItemIndex)
	{
		//��ǰ����
		tagViewItemInfo * pCurrentItem=NULL;
		if (m_wItemActive!=INVALID_WORD) pCurrentItem=m_ViewItemArray[m_wItemActive];

		//���ش���
		if (pCurrentItem!=NULL) pCurrentItem->pItemView->ShowWindow(SW_HIDE);

		//�����
		DWORD dwFlags=SWP_NOZORDER|SWP_SHOWWINDOW;
		pItemView->SetWindowPos(NULL,m_RectViewItem.left,m_RectViewItem.top,m_RectViewItem.Width(),m_RectViewItem.Height(),dwFlags);

		//�ػ�����
		pItemView->Invalidate();

		//����״̬
		m_wItemActive=wItemIndex;

		//���´���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

		//�¼�֪ͨ
		ASSERT(m_pIViewItemEvent!=NULL);
		if (m_pIViewItemEvent!=NULL) m_pIViewItemEvent->OnViewItemEvent(m_wItemActive,pItemView);

		//���´���
		CSize SizeViewItemCtrl;
		GetViewItemSize(SizeViewItemCtrl);
		SetWindowPos(NULL,0,0,SizeViewItemCtrl.cx,SizeViewItemCtrl.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);
	}

	return;
}

//ɾ������
VOID CWndViewItemCtrl::RemoveViewItem(CWnd * pItemView)
{
	//Ч��״̬
	ASSERT(GetViewItemIndex(pItemView)!=INVALID_WORD);
	ASSERT(m_ViewItemArray[GetViewItemIndex(pItemView)]!=NULL);

	//��ȡ����
	WORD wItemIndex=GetViewItemIndex(pItemView);
	tagViewItemInfo * pViewItemInfo=m_ViewItemArray[wItemIndex];

	//ɾ������
	SafeDelete(pViewItemInfo);
	m_ViewItemArray.RemoveAt(wItemIndex);

	//��ǰ����
	if (m_wItemActive==wItemIndex) m_wItemActive=INVALID_WORD;
	if ((m_wItemActive!=INVALID_WORD)&&(m_wItemActive>wItemIndex)) m_wItemActive--;

	//���´���
	CSize SizeViewItemCtrl;
	GetViewItemSize(SizeViewItemCtrl);
	SetWindowPos(NULL,0,0,SizeViewItemCtrl.cx,SizeViewItemCtrl.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);

	return;
}

//��������
VOID CWndViewItemCtrl::InsertViewItem(CWnd * pItemView, LPCTSTR pszTitleImage)
{
	//�����ж�
	if (GetViewItemIndex(pItemView)!=INVALID_WORD)
	{
		ASSERT(FALSE);
		return;
	}

	//��������
	tagViewItemInfo * pViewItemInfo=NULL;
	try
	{
		pViewItemInfo=new tagViewItemInfo;
		ZeroMemory(pViewItemInfo,sizeof(tagViewItemInfo));
	}
	catch (...)
	{
		ASSERT(FALSE);
		return;
	}

	//�������
	m_ViewItemArray.Add(pViewItemInfo);

	//���ñ���
	pViewItemInfo->pItemView=pItemView;
	lstrcpyn(pViewItemInfo->szItemTitle,pszTitleImage,CountArray(pViewItemInfo->szItemTitle));

	//����λ��
	WORD wItemCount=(WORD)m_ViewItemArray.GetCount();
	SetWindowPos(NULL,0,0,m_SizeButton.cx*wItemCount,m_SizeButton.cy,SWP_NOZORDER|SWP_NOMOVE);

	//�������
	ActiveViewItem(pItemView);

	return;
}

//��������
WORD CWndViewItemCtrl::GetViewItemIndex(CPoint MousePoint)
{
	//��������
	WORD wItemIndex=(WORD)(MousePoint.x/m_SizeButton.cx);
	if (wItemIndex>=m_ViewItemArray.GetCount()) wItemIndex=INVALID_WORD;

	return wItemIndex;
}

//��������
WORD CWndViewItemCtrl::GetViewItemIndex(CWnd * pItemView)
{
	//��������
	for (INT_PTR i=0;i<m_ViewItemArray.GetCount();i++)
	{
		if (m_ViewItemArray[i]->pItemView==pItemView)
		{
			return (WORD)i;
		}
	}

	return INVALID_WORD;
}

//�滭����
BOOL CWndViewItemCtrl::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(pDC);
	ImageBuffer.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());

	//���� DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);

	//������Դ
	CBitImage ItemButtonImage;
	ItemButtonImage.LoadFromResource(AfxGetInstanceHandle(),IDB_ITEM_BUTTON);

	//��Ⱦ��Դ
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(ItemButtonImage);

	//�滭����
	for (INT_PTR i=0;i<m_ViewItemArray.GetCount();i++)
	{
		//��������
		CPoint ImagePosion;
		ImagePosion.SetPoint(0,0);

		//��ȡ����
		ASSERT(m_ViewItemArray[i]!=NULL);
		tagViewItemInfo * pViewItemInfo=m_ViewItemArray[i];

		//����λ��
		if (m_wItemActive==i) ImagePosion.y=m_SizeButton.cy;

		//����λ��
		if (m_wItemDown==i) ImagePosion.x=m_SizeButton.cx;
		if ((m_wItemHover==i)&&(m_wItemDown!=i)) ImagePosion.x=m_SizeButton.cx*2;

		//�滭��ť
		CPoint PointButton(m_SizeButton.cx*i,0);
		ItemButtonImage.BitBlt(DCBuffer,PointButton.x,PointButton.y,m_SizeButton.cx,m_SizeButton.cy,ImagePosion.x,ImagePosion.y);

		//�滭����
		if (pViewItemInfo->szItemTitle[0]!=0)
		{
			//���û���
			DCBuffer.SetTextColor((m_wItemActive==i)?RGB(0,23,40):RGB(0,0,0));
			DCBuffer.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

			//����λ��
			CRect rcItemText;
			rcItemText.top=rcItemText.bottom=PointButton.y+6;
			rcItemText.left=rcItemText.right=PointButton.x+39;

			//�������
			INT nItemLen=lstrlen(pViewItemInfo->szItemTitle);
			DCBuffer.DrawText(pViewItemInfo->szItemTitle,nItemLen,&rcItemText,DT_CENTER|DT_VCENTER|DT_NOCLIP);
		}
	}

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return TRUE;
}

//�����Ϣ
VOID CWndViewItemCtrl::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//����λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//λ�ü���
	WORD wItemHover=GetViewItemIndex(Point);

	//��������
	if (wItemHover!=m_wItemHover)
	{
		//���ñ���
		m_wItemHover=wItemHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_UPDATENOW|RDW_INVALIDATE);
	}

	//�����ж�
	if (m_bHovering==false)
	{
		//���ñ���
		m_bHovering=true;

		//��������
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//ע����Ϣ
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//ע���¼�
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//�����Ϣ
VOID CWndViewItemCtrl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//��������
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		tagViewItemInfo * pViewItemInfo=m_ViewItemArray[m_wItemHover];
		if (pViewItemInfo!=NULL) ActiveViewItem(pViewItemInfo->pItemView);
	}

	//������
	if (m_wItemDown!=INVALID_WORD)
	{
		//�ͷ����
		ReleaseCapture();

		//���ñ���
		m_wItemDown=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
VOID CWndViewItemCtrl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//���ý���
	SetFocus();

	//���´���
	if (m_wItemHover!=INVALID_WORD)
	{
		//����˻�
		SetCapture();

		//���ñ���
		m_wItemDown=m_wItemHover;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//���˫��
VOID CWndViewItemCtrl::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//�ر�����
	if ((m_wItemHover!=0L)&&(m_wItemHover!=INVALID_WORD))
	{
		tagViewItemInfo * pViewItemInfo=m_ViewItemArray[m_wItemHover];
		if (pViewItemInfo!=NULL) pViewItemInfo->pItemView->PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
	}

	return;
}

//�����Ϣ
LRESULT CWndViewItemCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	m_bHovering=false;

	//���½���
	if (m_wItemHover!=INVALID_WORD)
	{
		//���ñ���
		m_wItemHover=INVALID_WORD;

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//�����Ϣ
BOOL CWndViewItemCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//���ù��
	if (m_wItemHover!=INVALID_WORD)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////////////
