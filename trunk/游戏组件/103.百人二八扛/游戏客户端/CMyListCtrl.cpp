#include "CMyListCtrl.h"
#include "resource.h"


LRESULT(CALLBACK* OldWinProc)(HWND hwnd, UINT uMsg, WPARAM   wParam, LPARAM lParam);
//LRESULT CALLBACK (*OldWinProc)(HWND hwnd,UINT uMsg,WPARAM   wParam,LPARAM lParam);
struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};
// CListCtrlCl
IMPLEMENT_DYNAMIC(CHeaderCtrlCl, CHeaderCtrl)

CHeaderCtrlCl::CHeaderCtrlCl()
		: m_R(171)
		, m_G(199)
		, m_B(235)
		, m_Gradient(8)
{
	m_Format = "";
	m_Height = 1;
	m_fontHeight = 15;
	m_fontWith = 0;
	m_color = RGB(0, 0, 0);
}

CHeaderCtrlCl::~CHeaderCtrlCl()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlCl, CHeaderCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()



// CHeaderCtrlCl ��Ϣ�������



void CHeaderCtrlCl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CHeaderCtrl::OnPaint()
	int nItem;
	nItem = GetItemCount();//�õ��м�����Ԫ
	for (int i = 0; i < nItem; i ++)
	{
		CRect tRect;
		GetItemRect(i, &tRect);//�õ�Item�ĳߴ�
		int R = m_R, G = m_G, B = m_B;
		CRect nRect(tRect);//�����ߴ絽�µ�������
		nRect.left++;//�����ָ��ߵĵط�
		//�������屳��
		for (int j = tRect.top; j <= tRect.bottom; j++)
		{
			nRect.bottom = nRect.top + 1;
			CBrush _brush;
			_brush.CreateSolidBrush(RGB(R, G, B));//������ˢ
			dc.FillRect(&nRect, &_brush); //��䱳��
			_brush.DeleteObject(); //�ͷŻ�ˢ
			R -= m_Gradient;
			G -= m_Gradient;
			B -= m_Gradient;
			if (R < 0)R = 0;
			if (G < 0)G = 0;
			if (B < 0)B = 0;
			nRect.top = nRect.bottom;
		}
		dc.SetBkMode(TRANSPARENT);
		CFont nFont , * nOldFont;
		//dc.SetTextColor(RGB(250,50,50));
		dc.SetTextColor(m_color);
		nFont.CreateFont(m_fontHeight, m_fontWith, 0, 0, 0, FALSE, FALSE, 0, 0, 0, 0, 0, 0, _TEXT("����"));//��������
		nOldFont = dc.SelectObject(&nFont);

		UINT nFormat = 1;
		if (m_Format[i] == '0')
		{
			nFormat = DT_LEFT;
			tRect.left += 3;
		}
		else if (m_Format[i] == '1')
		{
			nFormat = DT_CENTER;
		}
		else if (m_Format[i] == '2')
		{
			nFormat = DT_RIGHT;
			tRect.right -= 3;
		}
		TEXTMETRIC metric;
		dc.GetTextMetrics(&metric);
		int ofst = 0;
		ofst = tRect.Height() - metric.tmHeight;
		tRect.OffsetRect(0, ofst / 2);

		dc.DrawText(m_HChar[i], &tRect, nFormat);
		dc.SelectObject(nOldFont);
		nFont.DeleteObject(); //�ͷ�����
	}
	//��ͷ��ʣ�ಿ��
	CRect rtRect;
	CRect clientRect;
	GetItemRect(nItem - 1, rtRect);
	GetClientRect(clientRect);
	rtRect.left = rtRect.right + 1;
	rtRect.right = clientRect.right;
	int R = m_R, G = m_G, B = m_B;
	CRect nRect(rtRect);
	//�������屳��
	for (int j = rtRect.top; j <= rtRect.bottom; j++)
	{
		nRect.bottom = nRect.top + 1;
		CBrush _brush;
		_brush.CreateSolidBrush(RGB(R, G, B));//������ˢ
		dc.FillRect(&nRect, &_brush); //��䱳��
		_brush.DeleteObject(); //�ͷŻ�ˢ
		R -= m_Gradient;
		G -= m_Gradient;
		B -= m_Gradient;
		if (R < 0)R = 0;
		if (G < 0)G = 0;
		if (B < 0)B = 0;
		nRect.top = nRect.bottom;
	}
}
LRESULT CHeaderCtrlCl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
	HD_LAYOUT &hdl = *(HD_LAYOUT *) lParam;
	RECT *prc = hdl.prc;
	WINDOWPOS *pwpos = hdl.pwpos;

	//��ͷ�߶�Ϊԭ��1.5�������Ҫ��̬�޸ı�ͷ�߶ȵĻ�����1.5���һ��ȫ�ֱ���
	int nHeight = (int)(pwpos->cy * m_Height);
	pwpos->cy = nHeight;
	prc->top = nHeight;
	return lResult;
}


IMPLEMENT_DYNAMIC(CListCtrlCl, CListCtrl)

CListCtrlCl::CListCtrlCl()
		: m_nRowHeight(0)
		, m_fontHeight(12)
		, m_fontWith(0)
{
	m_color = RGB(0, 0, 0);
}

CListCtrlCl::~CListCtrlCl()
{
}

#define  WM_DRAW WM_USER+1
BEGIN_MESSAGE_MAP(CListCtrlCl, CListCtrl)
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_DRAW, OnDraw)

END_MESSAGE_MAP()



// CListCtrlCl ��Ϣ�������


BOOL CListCtrlCl::OnEraseBkgnd(CDC* pDC)
{
	CClientDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);
	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_APPLY_USER_BACK);
	CBitmap* pOldBitmap = bitmapDC.SelectObject(&bitmap);
	pDC->StretchBlt(clientRect.left, clientRect.top, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, &bitmapDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, SRCCOPY);
	//pDC->BitBlt(clientRect.left,clientRect.top,clientRect.right-clientRect.left,clientRect.bottom-clientRect.top,&bitmapDC,0,0,SRCCOPY);
	bitmapDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	pOldBitmap = NULL;
	return TRUE;
}
void CListCtrlCl::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	ModifyStyle(0, LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}
void CListCtrlCl::OnLButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	SendMessageW(m_viewHandle, WM_LBTUP, nFlags, (LPARAM)&point);
}
void CListCtrlCl::OnPaint()
{
	__super::OnPaint();
	return ;
	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_APPLY_USER_BACK);

	//����λͼ
	CBitmap BufferImage;
	BufferImage.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&bitmap);

	//�滭����
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &BufferDC, 0, 0, SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();



	//OnEraseBkgnd(NULL);


}
LRESULT  CListCtrlCl::OnDraw(WPARAM wParam, LPARAM lParam)
{

	//this->OnPaint();

	this->Invalidate(true);

	return true;

}

LRESULT   CALLBACK NewWinProc(HWND   hwnd,   UINT   uMsg,
                              WPARAM   wParam,   LPARAM   lParam)
{
	if (uMsg == WM_MOUSEWHEEL)
	{
		if ((short)HIWORD(wParam)   >   0)
		{
			//�Ϲ�

			OldWinProc(hwnd,   uMsg,   wParam,   lParam);
			SendMessage(hwnd, WM_DRAW, 0, 0);



		}
		else
		{
			OldWinProc(hwnd,   uMsg,   wParam,   lParam);
			SendMessage(hwnd, WM_DRAW, 0, 0);

			//�¹�                         }
		}
		return   0;
	}
	else
		return   OldWinProc(hwnd,   uMsg,   wParam,   lParam);
}

void CListCtrlCl::Init(CRect windowRect, CWnd* pParent)
{

	OldWinProc   = (WNDPROC)GetWindowLong(this->m_hWnd,   GWL_WNDPROC);
	SetWindowLong(m_hWnd,   GWL_WNDPROC, (LPARAM)NewWinProc);  //WM_MOUSEACTIVATE

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);

	InitializeFlatSB(this->m_hWnd);
	FlatSB_EnableScrollBar(this->m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);

	int nTitleBarHeight = 0;

	if (pParent->GetStyle() & WS_CAPTION)
		nTitleBarHeight = GetSystemMetrics(SM_CYSIZE);


	int nDialogFrameHeight = 0;
	int nDialogFrameWidth = 0;
	if ((pParent->GetStyle() & WS_BORDER))
	{
		nDialogFrameHeight = GetSystemMetrics(SM_CYDLGFRAME);
		nDialogFrameWidth = GetSystemMetrics(SM_CYDLGFRAME);
	}

	if (pParent->GetStyle() & WS_THICKFRAME)
	{
		nDialogFrameHeight += 1;
		nDialogFrameWidth += 1;
	}

}
void CListCtrlCl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{


	// TODO:  ������Ĵ����Ի���ָ����
	TCHAR lpBuffer[256];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ;
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rtClient;
	GetClientRect(&rtClient);
	for (int nCol = 0; GetColumn(nCol, &lvc); nCol++)
	{
		if (nCol > 0)
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol - 1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left;
		}

		CRect rcItem;
		if (!GetSubItemRect(lpDrawItemStruct->itemID, nCol, LVIR_LABEL, rcItem))
			continue;

		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CRect rcTemp;
		rcTemp = rcItem;

		if (nCol == 0)
		{
			rcTemp.left -= 2;
		}

		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			//pDC->FillSolidRect(&rcTemp, GetSysColor(COLOR_HIGHLIGHT)) ;
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
		}
		else
		{
			COLORREF color;
			color = GetBkColor();
			//pDC->FillSolidRect(rcTemp,color);

			if (FindColColor(nCol, color))
			{
				//pDC->FillSolidRect(rcTemp,color);
			}
			if (FindItemColor(nCol, lpDrawItemStruct->itemID, color))
			{
				//pDC->FillSolidRect(rcTemp,color);
			}

			//pDC->SetTextColor(m_color);
		}

		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

		UINT   uFormat    = DT_CENTER ;
		if (m_Header.m_Format[nCol] == '0')
		{
			uFormat = DT_LEFT;
		}
		else if (m_Header.m_Format[nCol] == '1')
		{
			uFormat = DT_CENTER;
		}
		else if (m_Header.m_Format[nCol] == '2')
		{
			uFormat = DT_RIGHT;
		}
		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0, ofst / 2);
		pDC->SetTextColor(m_color);
		COLORREF color;
		if (FindColTextColor(nCol, color))
		{
			pDC->SetTextColor(color);
		}
		if (FindItemTextColor(nCol, lpDrawItemStruct->itemID, color))
		{
			pDC->SetTextColor(color);
		}
		CFont nFont , * nOldFont;
		nFont.CreateFont(m_fontHeight, m_fontWith, 0, 0, 0, FALSE, FALSE, 0, 0, 0, 0, 0, 0, _TEXT("����"));//��������
		nOldFont = pDC->SelectObject(&nFont);


		//rcItem.top = rcItem.top-5;
		//rcItem.bottom = rcItem.bottom-5;


		DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer),
		         &rcItem, uFormat) ;

		pDC->SelectStockObject(SYSTEM_FONT) ;

	}

}

void CListCtrlCl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
void CListCtrlCl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight > 0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}
int CListCtrlCl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_HChar.Add(lpszColumnHeading);
	if (nFormat == LVCFMT_LEFT)
	{
		m_Header.m_Format = m_Header.m_Format + "0";
	}
	else if (nFormat == LVCFMT_CENTER)
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	else if (nFormat == LVCFMT_RIGHT)
	{
		m_Header.m_Format = m_Header.m_Format + "2";
	}
	else
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	return CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}
// Gradient - ����ϵ�������屳����,���ý�����Ϊ0
void CListCtrlCl::SetHeaderBKColor(int R, int G, int B, int Gradient) //���ñ�ͷ����ɫ
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

// ���ñ�ͷ�߶�
void CListCtrlCl::SetHeaderHeight(float Height) //���ñ�ͷ�߶�
{
	m_Header.m_Height = Height;
}
bool CListCtrlCl::FindColColor(int col, COLORREF &color) //��������ɫ
{
	int flag = 0;
	for (POSITION pos = m_ptrListCol.GetHeadPosition(); pos != NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor->nCol == col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1 == flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemColor(int col, int row, COLORREF &color) //������ɫ
{
	int flag = 0;
	for (POSITION pos = m_ptrListItem.GetHeadPosition(); pos != NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor->nCol == col && pColor->nRow == row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1 == flag)
	{
		return true;
	}
	return false;
}
void CListCtrlCl::SetColColor(int col, COLORREF color) //��������ɫ
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_ptrListCol.AddTail(pColor);
}
void CListCtrlCl::SetItemColor(int col, int row, COLORREF color) //���ø�����ɫ
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ptrListItem.AddTail(pColor);
}
void CListCtrlCl::SetRowHeigt(int nHeight) //�����и�
{
	m_nRowHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}
void CListCtrlCl::SetHeaderFontHW(int nHeight, int nWith) //����ͷ�������͸�
{
	m_Header.m_fontHeight = nHeight;
	m_Header.m_fontWith = nWith;
}
void CListCtrlCl::SetHeaderTextColor(COLORREF color) //����ͷ��������ɫ
{
	m_Header.m_color = color;
}
BOOL CListCtrlCl::SetTextColor(COLORREF cr)  //����������ɫ
{
	m_color = cr;
	return TRUE;
}
void CListCtrlCl::SetFontHW(int nHeight, int nWith) //��������ߺͿ�
{
	m_fontHeight = nHeight;
	m_fontWith = nWith;
}
void CListCtrlCl::SetColTextColor(int col, COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_colTextColor.AddTail(pColor);
}
bool CListCtrlCl::FindColTextColor(int col, COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_colTextColor.GetHeadPosition(); pos != NULL;)
	{
		stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
		if (pColor->nCol == col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1 == flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemTextColor(int col, int row, COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_ItemTextColor.GetHeadPosition(); pos != NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nCol == col && pColor->nRow == row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1 == flag)
	{
		return true;
	}
	return false;
}
void CListCtrlCl::SetItemTextColor(int col, int row, COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ItemTextColor.AddTail(pColor);
}

