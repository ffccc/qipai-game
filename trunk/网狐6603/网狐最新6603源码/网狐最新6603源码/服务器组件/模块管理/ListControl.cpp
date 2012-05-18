#include "StdAfx.h"
#include "ListControl.h"

//////////////////////////////////////////////////////////////////////////////////
//�궨��

//�б�����
#define LIST_STYTE LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_FLATSB

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagSortParameter
{
	bool							bAscendSort;						//�����־
	WORD							wColumnIndex;						//λ������
	CListControl *					pListControl;						//�б�ؼ�
};

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CListHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CListControl, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CListHeaderCtrl::CListHeaderCtrl()
{
}

//��������
CListHeaderCtrl::~CListHeaderCtrl()
{
}

//�ػ�����
VOID CListHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcRect;
	GetClientRect(&rcRect);

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(GetFont());

	//�滭����
	CRect rcItem;
	HDITEM HDItem;
	TCHAR szBuffer[64];
	for (INT i=0;i<GetItemCount();i++)
	{
		//�������
		HDItem.mask=HDI_TEXT;
		HDItem.pszText=szBuffer;
		HDItem.cchTextMax=CountArray(szBuffer);

		//��ȡ��Ϣ
		GetItem(i,&HDItem);
		GetItemRect(i,&rcItem);

		//�滭����
		dc.DrawFrameControl(&rcItem,DFC_BUTTON,DFCS_BUTTONPUSH);

		//�滭����
		rcItem.DeflateRect(3,1,3,1);
		const UINT uFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_CENTER;
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcItem,uFormat);
	}


	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CListControl::CListControl()
{
	//���ñ���
	m_bAscendSort=true;

	return;
}

//��������
CListControl::~CListControl()
{
}

//�ؼ���
VOID CListControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//��������
	SetBkColor(CR_NORMAL_BK);
	SetExtendedStyle(LIST_STYTE);

	//�󶨿ؼ�
	m_ListHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	//�����б�
	InitListControl();

	return;
}

//�滭����
VOID CListControl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//��������
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	//��ȡ����
	INT nItemID=lpDrawItemStruct->itemID;
	INT nColumnCount=m_ListHeaderCtrl.GetItemCount();

	//��ȡ��ɫ
	tagListItemColor ListItemColor;
	GetListItemColor(lpDrawItemStruct->itemData,lpDrawItemStruct->itemState,ListItemColor);

	//�滭����
	CRect rcClipBox;
	pDC->GetClipBox(&rcClipBox);

	//���� DC
	pDC->SetBkColor(ListItemColor.rcBackColor);
	pDC->SetTextColor(ListItemColor.rcTextColor);

	//�滭����
	for (INT i=0;i<nColumnCount;i++)
	{
		//��ȡλ��
		CRect rcSubItem;
		GetSubItemRect(nItemID,i,LVIR_LABEL,rcSubItem);

		//����϶
		if ((i==0)&&(rcItem.left<rcSubItem.left))
		{
			INT nYFill=rcItem.bottom-rcItem.top;
			INT nXFill=rcSubItem.left-rcItem.left;
			pDC->FillSolidRect(rcItem.left,rcItem.top,nXFill,nYFill,ListItemColor.rcBackColor);
		}

		//�滭�ж�
		if (rcSubItem.left>rcClipBox.right) break;
		if (rcSubItem.right<rcClipBox.left) continue;
		if (rcSubItem.left==rcSubItem.right) continue;

		//�滭����
		pDC->FillSolidRect(&rcSubItem,ListItemColor.rcBackColor);

		//��ȡ����
		TCHAR szString[256]=TEXT("");
		GetItemText(nItemID,i,szString,CountArray(szString));

		//��ȡ��ʽ
		HDITEM HDItem;
		HDItem.mask=HDI_FORMAT;
		m_ListHeaderCtrl.GetItem(i,&HDItem);

		//���ø�ʽ
		UINT uFormat=0;
		if (HDItem.fmt&HDF_LEFT) uFormat|=DT_LEFT;
		if (HDItem.fmt&HDF_RIGHT) uFormat|=DT_RIGHT;
		if (HDItem.fmt&HDF_CENTER) uFormat|=DT_CENTER;

		//�滭����
		rcSubItem.left+=5;
		pDC->DrawText(szString,lstrlen(szString),&rcSubItem,uFormat|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭ѡ��
	if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->DrawFocusRect(&rcItem);

	return;
}

//���к���
INT CALLBACK CListControl::SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList)
{
	//��������
    tagSortParameter * pSortParameter=(tagSortParameter *)lParamList;

	//�������
	WORD wColumnIndex=pSortParameter->wColumnIndex;
	CListControl * pListControl=pSortParameter->pListControl;

	//��������
	INT nResult=pListControl->SortListItemData(lParam1,lParam2,wColumnIndex);

	return (pSortParameter->bAscendSort)?nResult:-nResult;
}

//�滭����
BOOL CListControl::OnEraseBkgnd(CDC * pDC)
{
	//�滭����
	if (m_ListHeaderCtrl.m_hWnd!=NULL)
	{
		//��ȡλ��
		CRect rcClient;
		CRect rcHeaderCtrl;
		GetClientRect(&rcClient);
		m_ListHeaderCtrl.GetWindowRect(&rcHeaderCtrl);

		//�滭����
		rcClient.top=rcHeaderCtrl.Height();
		pDC->FillSolidRect(&rcClient,GetBkColor());

		return TRUE;
	}

	return __super::OnEraseBkgnd(pDC);
}

//�����Ϣ
VOID CListControl::OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult) 
{
	//��������
	NM_LISTVIEW * pNMListView=(NM_LISTVIEW *)pNMHDR;

	//��������
	tagSortParameter SortParameter;
	ZeroMemory(&SortParameter,sizeof(SortParameter));

	//���ñ���
	SortParameter.pListControl=this;
	SortParameter.bAscendSort=m_bAscendSort;
	SortParameter.wColumnIndex=pNMListView->iSubItem;

	//����״̬
	m_bAscendSort=!m_bAscendSort;

	//�����б�
	SortItems(SortFunction,(LPARAM)&SortParameter);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
