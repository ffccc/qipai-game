#include "StdAfx.h"
#include "ListControl.h"

//////////////////////////////////////////////////////////////////////////////////
//宏定义

//列表属性
#define LIST_STYTE LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_FLATSB

//////////////////////////////////////////////////////////////////////////////////

//排序信息
struct tagSortParameter
{
	bool							bAscendSort;						//升序标志
	WORD							wColumnIndex;						//位置索引
	CListControl *					pListControl;						//列表控件
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

//构造函数
CListHeaderCtrl::CListHeaderCtrl()
{
}

//析构函数
CListHeaderCtrl::~CListHeaderCtrl()
{
}

//重画函数
VOID CListHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);

	//获取位置
	CRect rcRect;
	GetClientRect(&rcRect);

	//设置 DC
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(GetFont());

	//绘画子项
	CRect rcItem;
	HDITEM HDItem;
	TCHAR szBuffer[64];
	for (INT i=0;i<GetItemCount();i++)
	{
		//构造变量
		HDItem.mask=HDI_TEXT;
		HDItem.pszText=szBuffer;
		HDItem.cchTextMax=CountArray(szBuffer);

		//获取信息
		GetItem(i,&HDItem);
		GetItemRect(i,&rcItem);

		//绘画背景
		dc.DrawFrameControl(&rcItem,DFC_BUTTON,DFCS_BUTTONPUSH);

		//绘画标题
		rcItem.DeflateRect(3,1,3,1);
		const UINT uFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_CENTER;
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcItem,uFormat);
	}


	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CListControl::CListControl()
{
	//设置变量
	m_bAscendSort=true;

	return;
}

//析构函数
CListControl::~CListControl()
{
}

//控件绑定
VOID CListControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//设置属性
	SetBkColor(CR_NORMAL_BK);
	SetExtendedStyle(LIST_STYTE);

	//绑定控件
	m_ListHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	//配置列表
	InitListControl();

	return;
}

//绘画函数
VOID CListControl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//变量定义
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	//获取属性
	INT nItemID=lpDrawItemStruct->itemID;
	INT nColumnCount=m_ListHeaderCtrl.GetItemCount();

	//获取颜色
	tagListItemColor ListItemColor;
	GetListItemColor(lpDrawItemStruct->itemData,lpDrawItemStruct->itemState,ListItemColor);

	//绘画区域
	CRect rcClipBox;
	pDC->GetClipBox(&rcClipBox);

	//设置 DC
	pDC->SetBkColor(ListItemColor.rcBackColor);
	pDC->SetTextColor(ListItemColor.rcTextColor);

	//绘画子项
	for (INT i=0;i<nColumnCount;i++)
	{
		//获取位置
		CRect rcSubItem;
		GetSubItemRect(nItemID,i,LVIR_LABEL,rcSubItem);

		//填充空隙
		if ((i==0)&&(rcItem.left<rcSubItem.left))
		{
			INT nYFill=rcItem.bottom-rcItem.top;
			INT nXFill=rcSubItem.left-rcItem.left;
			pDC->FillSolidRect(rcItem.left,rcItem.top,nXFill,nYFill,ListItemColor.rcBackColor);
		}

		//绘画判断
		if (rcSubItem.left>rcClipBox.right) break;
		if (rcSubItem.right<rcClipBox.left) continue;
		if (rcSubItem.left==rcSubItem.right) continue;

		//绘画背景
		pDC->FillSolidRect(&rcSubItem,ListItemColor.rcBackColor);

		//获取文字
		TCHAR szString[256]=TEXT("");
		GetItemText(nItemID,i,szString,CountArray(szString));

		//获取格式
		HDITEM HDItem;
		HDItem.mask=HDI_FORMAT;
		m_ListHeaderCtrl.GetItem(i,&HDItem);

		//设置格式
		UINT uFormat=0;
		if (HDItem.fmt&HDF_LEFT) uFormat|=DT_LEFT;
		if (HDItem.fmt&HDF_RIGHT) uFormat|=DT_RIGHT;
		if (HDItem.fmt&HDF_CENTER) uFormat|=DT_CENTER;

		//绘画文字
		rcSubItem.left+=5;
		pDC->DrawText(szString,lstrlen(szString),&rcSubItem,uFormat|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//绘画选择
	if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->DrawFocusRect(&rcItem);

	return;
}

//排列函数
INT CALLBACK CListControl::SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList)
{
	//变量定义
    tagSortParameter * pSortParameter=(tagSortParameter *)lParamList;

	//排序变量
	WORD wColumnIndex=pSortParameter->wColumnIndex;
	CListControl * pListControl=pSortParameter->pListControl;

	//子项排序
	INT nResult=pListControl->SortListItemData(lParam1,lParam2,wColumnIndex);

	return (pSortParameter->bAscendSort)?nResult:-nResult;
}

//绘画背景
BOOL CListControl::OnEraseBkgnd(CDC * pDC)
{
	//绘画背景
	if (m_ListHeaderCtrl.m_hWnd!=NULL)
	{
		//获取位置
		CRect rcClient;
		CRect rcHeaderCtrl;
		GetClientRect(&rcClient);
		m_ListHeaderCtrl.GetWindowRect(&rcHeaderCtrl);

		//绘画背景
		rcClient.top=rcHeaderCtrl.Height();
		pDC->FillSolidRect(&rcClient,GetBkColor());

		return TRUE;
	}

	return __super::OnEraseBkgnd(pDC);
}

//点击消息
VOID CListControl::OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult) 
{
	//变量定义
	NM_LISTVIEW * pNMListView=(NM_LISTVIEW *)pNMHDR;

	//变量定义
	tagSortParameter SortParameter;
	ZeroMemory(&SortParameter,sizeof(SortParameter));

	//设置变量
	SortParameter.pListControl=this;
	SortParameter.bAscendSort=m_bAscendSort;
	SortParameter.wColumnIndex=pNMListView->iSubItem;

	//设置状态
	m_bAscendSort=!m_bAscendSort;

	//排列列表
	SortItems(SortFunction,(LPARAM)&SortParameter);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
