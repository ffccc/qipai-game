#include "Stdafx.h"
#include "Resource.h"
#include "SkinListCtrl.h"

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//��̬����
CSkinListAttribute					CSkinHeaderCtrl::m_SkinAttribute;				//�б�����

//Ĭ������
#define LIST_STYTE					LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_UNDERLINEHOT|LVS_EX_FLATSB

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinListAttribute::CSkinListAttribute() 
{
	//��ʼ������
	m_crHeadText=RGB(0,0,0);
}

//��������
CSkinListAttribute::~CSkinListAttribute()
{
}

//��������
bool CSkinListAttribute::UpdateSkinResource(ISkinResource * pISkinResource)
{
	//��ȡ��Ϣ
	tagSkinListCtrlResource SkinListCtrlResource;
	if (pISkinResource!=NULL) pISkinResource->GetResourceInfo(SkinListCtrlResource);

	//��ȡ����
	m_crHeadText=SkinListCtrlResource.crListTX;
	m_ImageHead.SetLoadInfo(SkinListCtrlResource.LoadInfoImageBack);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinHeaderCtrl::CSkinHeaderCtrl()
{
	m_iSortCol=-1;
	m_SortType=Type_NoSort;
	m_pIHeaderCtrlSink=NULL;
}

//��������
CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
}

//�ػ�����
void CSkinHeaderCtrl::OnPaint() 
{
	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//�����Ի�
	HD_ITEM HDItem;
	HDItem.mask=HDI_FORMAT;
	int iItemCount=GetItemCount();
	for (int i=0;i<iItemCount;i++)
	{
		GetItem(i,&HDItem);
		HDItem.fmt|=HDF_OWNERDRAW;
		SetItem(i,&HDItem);
	}

	//��������ͼ
	CBitmap BufferBmp;
	CClientDC ClientDC(this);
	BufferBmp.CreateCompatibleBitmap(&ClientDC,ClientRect.Width(),ClientRect.Height());
	m_BufferDC.CreateCompatibleDC(&ClientDC);
	m_BufferDC.SelectObject(&BufferBmp);
	m_BufferDC.SetBkMode(TRANSPARENT);
	m_BufferDC.SetTextColor(m_SkinAttribute.m_crHeadText);
	m_BufferDC.SelectObject(CSkinResourceManager::m_DefaultFont);

	//����λͼ
	CImageHandle ImageHandle(&m_SkinAttribute.m_ImageHead);
	if (ImageHandle.IsResourceValid())
	{
		ImageHandle->StretchBlt(m_BufferDC,ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height());
		m_BufferDC.Draw3dRect(ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height(),RGB(0,0,0),RGB(0,0,0));
		Default();
	}
	
	//�滭����
	ClientDC.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&m_BufferDC,0,0,SRCCOPY);
	m_BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	
	return;
}

//�滭����
void CSkinHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//��ȡ DC
	CDC * pDC=NULL;
	if (m_BufferDC.GetSafeHdc()==NULL)
	{
		pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_SkinAttribute.m_crHeadText);
		pDC->SelectObject(CSkinResourceManager::m_DefaultFont);
	}
	else pDC=&m_BufferDC;

	//��ȡ��Ϣ
	HDITEM HDItem;
	TCHAR szBuffer[64];
	HDItem.pszText=szBuffer;
	HDItem.mask=HDI_TEXT|HDI_FORMAT;
	HDItem.cchTextMax=sizeof(szBuffer)/sizeof(szBuffer[0]);
	CRect rcItemRect=lpDrawItemStruct->rcItem;
	GetItem(lpDrawItemStruct->itemID,&HDItem);

	//��ȡ״̬
	bool bSelected=(lpDrawItemStruct->itemState==ODS_SELECTED);

	//�滭����
	if (m_SkinAttribute.m_ImageHead.IsNull()) pDC->DrawFrameControl(&rcItemRect,DFC_BUTTON,DFCS_BUTTONPUSH);
	else m_SkinAttribute.m_ImageHead.StretchBlt(pDC->m_hDC,rcItemRect.left,rcItemRect.top,rcItemRect.Width(),rcItemRect.Height());
	if (bSelected) pDC->Draw3dRect(&rcItemRect,RGB(0,0,0),RGB(125,125,125));
	else pDC->Draw3dRect(&rcItemRect,RGB(125,125,125),RGB(0,0,0));
	rcItemRect.DeflateRect(5,0,5,0);

	//�滭��־
	if ((m_iSortCol==lpDrawItemStruct->itemID)&&(m_SortType!=Type_NoSort))
	{
		if (rcItemRect.Width()>=14)
		{
			int nXOff=0,nYPos=(rcItemRect.Height()-9)/2+rcItemRect.top;
			if (bSelected) { nYPos++; nXOff++; }
			CPen * pWhitePen=CPen::FromHandle((HPEN)GetStockObject(WHITE_PEN));
			CPen * pBlackPen=CPen::FromHandle((HPEN)GetStockObject(BLACK_PEN));
			if (m_SortType==Type_Ascend)
			{
				pDC->SelectObject(pWhitePen);
				pDC->MoveTo(rcItemRect.right-5+nXOff,nYPos);
				pDC->LineTo(rcItemRect.right-14+nXOff,nYPos);
				pDC->LineTo(rcItemRect.right-10+nXOff,nYPos+9);
				pDC->SelectObject(pBlackPen);
				pDC->LineTo(rcItemRect.right-5+nXOff,nYPos);
			}
			else
			{
				pDC->SelectObject(pWhitePen);
				pDC->MoveTo(rcItemRect.right-10+nXOff,nYPos);
				pDC->LineTo(rcItemRect.right-14+nXOff,nYPos+9);
				pDC->LineTo(rcItemRect.right-5+nXOff,nYPos+9);
				pDC->SelectObject(pBlackPen);
				pDC->LineTo(rcItemRect.right-10+nXOff,nYPos);
			}
		}
		rcItemRect.right-=19;
	}

	//�滭��������
	UINT uFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER;
	if (HDItem.fmt&HDF_CENTER) uFormat|=DT_CENTER;
	else if (HDItem.fmt&HDF_LEFT) uFormat|=DT_LEFT;
	else if (HDItem.fmt&HDF_RIGHT) uFormat|=DT_RIGHT;
	if (bSelected) rcItemRect.OffsetRect(1,1);
	pDC->DrawText(HDItem.pszText,lstrlen(HDItem.pszText),&rcItemRect,uFormat);

	return;
}

//��������
BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC * pDC) 
{
	return TRUE;
}

//��ȡ����
LPCTSTR CSkinHeaderCtrl::GetItemText(int iCol, TCHAR szBuffer[], int nSize)
{
	ASSERT(szBuffer!=NULL);
	if (iCol<GetItemCount())
	{
		HDITEM HDItem;
		HDItem.pszText=szBuffer;
		HDItem.mask=HDI_TEXT;
		HDItem.cchTextMax=nSize;
		GetItem(iCol,&HDItem);
	}
	else szBuffer[0]=0;
	return szBuffer;
}

//����ת��
int CSkinHeaderCtrl::PointToColIndex(CPoint Point)
{
	CRect rcItemRect;
	int iItemCount=GetItemCount();
	for (int i=0;i<iItemCount;i++)
	{
		GetItemRect(i,&rcItemRect);
		if (rcItemRect.PtInRect(Point)) return i;
	}
	return -1;
}

//���ñ�־
void CSkinHeaderCtrl::SetSortArrow(int iSortCol, enHeaderSortType SortType)
{
	m_iSortCol=iSortCol;
	m_SortType=SortType;
	Invalidate(FALSE);
	return;
}

//��ȡ��־
void CSkinHeaderCtrl::GetSortArrow(int & iSortCol, enHeaderSortType & SortType)
{
	iSortCol=m_iSortCol;
	SortType=m_SortType;
	return;
}

//�����Ϣ
void CSkinHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pIHeaderCtrlSink!=NULL) m_pIHeaderCtrlSink->OnHeaderLButtonDown(this,nFlags,point);
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

//�����Ϣ
void CSkinHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pIHeaderCtrlSink!=NULL) m_pIHeaderCtrlSink->OnHeaderLButtonUp(this,nFlags,point);
	CHeaderCtrl::OnLButtonUp(nFlags, point);
}

//�����Ϣ
void CSkinHeaderCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_pIHeaderCtrlSink!=NULL) m_pIHeaderCtrlSink->OnHeaderRButtonUp(this,nFlags,point);
	CHeaderCtrl::OnRButtonUp(nFlags, point);
}

//�����Ϣ
void CSkinHeaderCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_pIHeaderCtrlSink!=NULL) m_pIHeaderCtrlSink->OnHeaderRButtonDown(this,nFlags,point);
	CHeaderCtrl::OnRButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinListCtrl::CSkinListCtrl()
{
}

//��������
CSkinListCtrl::~CSkinListCtrl()
{
}

//�ؼ���
void CSkinListCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	m_SkinHeadCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
	SetExtendedStyle(LIST_STYTE);
	return;
}

//������Ϣ
int CSkinListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	m_SkinHeadCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
	SetExtendedStyle(LIST_STYTE);
	return 0;
}

//�滭����
BOOL CSkinListCtrl::OnEraseBkgnd(CDC * pDC)
{
	if (m_SkinHeadCtrl.GetSafeHwnd())
	{
		CRect ClientRect,ListHeaderRect;
		GetClientRect(&ClientRect);
		m_SkinHeadCtrl.GetWindowRect(&ListHeaderRect);
		ClientRect.top=ListHeaderRect.Height();
		pDC->FillSolidRect(&ClientRect,GetBkColor());
		return TRUE;
	}
	return __super::OnEraseBkgnd(pDC);
}

//////////////////////////////////////////////////////////////////////////
