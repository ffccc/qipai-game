#include "StdAfx.h"
#include "dragdropmanager.h"
#include <windowsx.h>	// extensions--for GET_X_LPARAM

const UINT WM_DD_DRAGENTER = RegisterWindowMessage(_T("WM_DD_DRAGENTER"));
const UINT WM_DD_DRAGOVER  = RegisterWindowMessage(_T("WM_DD_DRAGOVER"));
const UINT WM_DD_DRAGDROP  = RegisterWindowMessage(_T("WM_DD_DRAGDROP"));
const UINT WM_DD_DRAGABORT = RegisterWindowMessage(_T("WM_DD_DRAGABORT"));

// 定义一个宏，从鼠标消息中得到光标所在的点
#define GETPOINT(lp) (CPoint(GET_X_LPARAM(lp), GET_Y_LPARAM(lp)))

// 定义单张牌的宽度与高度
#define CARD_WIDTH			36			//扑克宽度
#define CARD_HEIGHT			90			//扑克高度

CDragDropManager::CDragDropManager(void)
{
	m_pMainWnd = NULL;
	m_hwndTracking = NULL;
	m_hCursorSave = NULL;
	m_pDragImage = NULL;
	m_iState = 0;
	SetCursors(LoadCursor(NULL, IDC_ARROW), LoadCursor(NULL, IDC_NO));
	memset(&m_info,0,sizeof(m_info));
	memset(m_hWnd,0,sizeof(m_hWnd));
}

CDragDropManager::~CDragDropManager(void)
{
}

void CDragDropManager::InitDragDropManager(CWnd *pMainWnd,HWND hWnd[9])
{
	m_pMainWnd = pMainWnd;
	CopyMemory(m_hWnd,hWnd,sizeof(m_hWnd));
}

BOOL CDragDropManager::IsDragDropWnd(HWND hWnd)
{
	for(int i=0;i<9;i++)
	{
		if(m_hWnd[i] == hWnd) return true;
	}

	return false;
}
//////////////////
// MFC 的优点之一是它在主窗口中仅通过虚拟 PreTranslateMessage 
//方法便可以过滤所有子窗口消息。这使得 CDragDropMgr 
//可以仅在单一的函数中便可截获发送到任何潜在拖拽源窗口的鼠标消息，
//从而避免了必须子类化每一个窗口。当 CDragDropMgr::PreTranslateMessage 
//看到 WM_LBUTTONDOWN，它便查找该窗口句柄（HWND）以便检查它是否被列入源窗口表。
//如果它是一个源窗口，则进行拖拽初始化，否则忽略该消息。

BOOL CDragDropManager::PreTranslateMessage(MSG* pMsg)
{
	const MSG& msg = *pMsg;

	//用户按下鼠标后，就将源窗口设为了捕获窗口，其后的鼠标与键盘消息都发送到此窗口
	if(IsDragDropWnd(msg.hwnd) && msg.message==WM_LBUTTONDOWN)
	{
 		 return OnLButtonDown(msg);
	}

	//当是拖放窗口，并且处理拖放状态时，才进去处理这两个消息
	if(IsDragDropWnd(msg.hwnd) && m_iState>0)
	{
		if (msg.message==WM_MOUSEMOVE) 
		{
			return OnMouseMove(msg);
		} 
		else if (msg.message==WM_LBUTTONUP) 
		{
			return OnLButtonUp(msg);
		}
		else if (msg.message==WM_KEYDOWN && msg.wParam==VK_ESCAPE) 
		{
			m_pMainWnd->SendMessage(WM_DD_DRAGABORT, 0, NULL);
			SetState(NONE);
			delete m_info.data;
			m_info.data=NULL;
			return 1;
		}
		else if(msg.message==WM_RBUTTONDOWN)
		{
			return true;
		}
	}

	return FALSE;
}

//////////////////
// 处理鼠标按下消息，开始进入捕获状态

BOOL CDragDropManager::OnLButtonDown(const MSG& msg)
{
	//CString str;
	//for(int i=0;i<9;i++)
	//{
	//	str.Format("%s  %d",str,::GetDlgCtrlID(m_hWnd[i]));
	//}
	//str.Format("%s  目前为 %d",str,::GetDlgCtrlID(msg.hwnd));
	//AfxMessageBox(str);

	m_hwndTracking = msg.hwnd;
	m_ptOrg = GETPOINT(msg.lParam);
	SetState(CAPTURED);
	return TRUE;
}

//////////////////
// 处理鼠标移动操作，进入到拖动状态

BOOL CDragDropManager::OnMouseMove(const MSG& msg)
{
	//不是拖放状态的话，直接返回
	if (!m_iState)	return FALSE;

	//得到被跟踪的窗口
	CWnd* pWnd = CWnd::FromHandle(m_hwndTracking);
	CPoint pt = GETPOINT(msg.lParam);
	DRAGDROPINFO& dd = m_info;

	if (IsDragging()) 
	{
		// 已经处理于拖动状态的话，则显示拖动的图像
		pWnd->ClientToScreen(&pt);
		m_pDragImage->DragMove(pt);

		// 判断是否拖动到达不同的窗口，并显示不同的光标
		m_pMainWnd->ScreenToClient(&pt);
		dd.pt = pt;
		dd.hwndTarget = m_pMainWnd->ChildWindowFromPoint(pt)->GetSafeHwnd();
		m_pMainWnd->SendMessage(WM_DD_DRAGOVER, 0, (LPARAM)(void*)&dd);
		SetCursor(dd.hwndTarget && IsDragDropWnd(dd.hwndTarget) ? m_hCursorDrop : m_hCursorNo);
	} 
	else 
	{
		// 还没有进入拖动状态的话，则设置进入到拖动状态
		CPoint delta = pt - m_ptOrg;
		static CPoint jog(GetSystemMetrics(SM_CXDRAG),GetSystemMetrics(SM_CYDRAG));

		if (abs(delta.x)>=jog.x || abs(delta.y)>jog.y) 
		{
			dd.hwndSource = m_hwndTracking;
			dd.pt = m_ptOrg;	// start from ORIGINAL point, not where now
			dd.hwndTarget = NULL;
			dd.data = NULL;

			// Send main window a message: enter drag mode. 
			BOOL bDrag = (BOOL)m_pMainWnd->SendMessage(WM_DD_DRAGENTER,::GetDlgCtrlID(m_hwndTracking), (LPARAM)(void*)&dd);

			if (bDrag && dd.data) 
			{
				SetState(DRAGGING);			 // I am now dragging
				OnMouseMove(msg);
				pWnd->ClientToScreen(&pt);
				CRect rc;
				m_pDragImage = dd.data->CreateDragImage(pWnd, rc);
				m_pDragImage->BeginDrag(0, rc.CenterPoint());
				m_pDragImage->DragEnter(NULL,pt);
			} 
			else 
			{
				SetState(NONE);
			}
		}
	}
	return TRUE;
}

//////////////////
// 处理用户鼠标释放动作：放下被拖的数据后，返回到原始状态
BOOL CDragDropManager::OnLButtonUp(const MSG& msg)
{
	//没有拖动的情况下，什么也不做
	if (!IsDragging()) 
	{
		SetState(NONE); 
		return FALSE;
	}
	//当在目的窗口中时，进行放下操作
	DRAGDROPINFO& dd = m_info;
	if (IsDragDropWnd(dd.hwndTarget) && IsDragDropWnd(dd.hwndSource) && dd.hwndTarget != dd.hwndSource) 
	{
		CPoint pt = GETPOINT(msg.lParam);
		CWnd* pWndSource = CWnd::FromHandle(dd.hwndSource);
		CWnd* pWndTarget = CWnd::FromHandle(dd.hwndTarget);
		pWndSource->ClientToScreen(&pt);
		pWndTarget->ScreenToClient(&pt);
		dd.pt = pt;
		//通知主窗口，执行放下操作
		m_pMainWnd->SendMessage(WM_DD_DRAGDROP,pWndTarget->GetDlgCtrlID(), (LPARAM)(void*)&dd);
	} 
	else 
	{
		//不是目标窗口的话，则放弃拖放操作
		m_pMainWnd->SendMessage(WM_DD_DRAGABORT, 0, 0);
	}

	delete m_info.data;
	m_info.data=NULL;
	SetState(NONE);
	return TRUE;
}

///////////////////////////////////////////////////////////
// 设置拖拉管理器的状态：NONE、CAPTURED 和 DRAGGING
//   1、当用户按下鼠标键，CDragDropMgr 进入 CAPTURED 状态。
//   2、当用户移动鼠标，则进入 DRAGGING 状态。

void CDragDropManager::SetState(UINT iState)
{
	if (iState!=m_iState) 
	{
		//当用户按下鼠标时，则设定被跟踪的窗口为鼠标捕获窗口
		if (iState==CAPTURED)
		{
			::SetCapture(m_hwndTracking);
		} 
		//当用户移动鼠标、保存正前的光标
		else if (iState==DRAGGING) 
		{
			m_hCursorSave = GetCursor();
		}
		//当用户释放鼠标时，拖拉过程结束
		else if (iState==NONE) 
		{
			::ReleaseCapture();				 // 释放被跟踪的窗口
			SetCursor(m_hCursorSave);		 // 还原光标
			if (m_pDragImage) 
			{
				m_pDragImage->EndDrag();	 // 结束被拖拉的图像
				delete m_pDragImage;		
				m_pDragImage=NULL;		
			}
			m_hwndTracking = NULL;          // 清空被跟踪的窗口
		}
		m_iState = iState;
	}
}
/////////////////////////////////////////////////////////
// 创建一个拖拉图像列表

CImageList* CDragDropData::CreateDragImage(CWnd* pWnd, CRect& rc)
{
	const COLORREF BGCOLOR = GetSysColor(COLOR_3DLIGHT);

	// 创建一个与原窗口相兼容的内存DC
	CWindowDC dcWin(pWnd);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dcWin);

	// 使用与原窗口相同的字体
	CFont* pFont = pWnd->GetFont();
	CFont* pOldFont = dcMem.SelectObject(pFont);

	// 得到拖拉显示图片的大小
	// 调用派生类的实现函数，得到拖拉显示图像的大小
	CSize sz = OnGetDragSize(dcMem);
	rc = CRect(CPoint(0,0), sz);

	// 创建图像列表：创建位图，并将位图画入其中
	m_bitmap.CreateCompatibleBitmap(&dcWin, sz.cx, sz.cy);
	CBitmap* pOldBitmap = dcMem.SelectObject(&m_bitmap);
	CBrush brush;
	brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
	dcMem.FillRect(&rc,&brush);
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	// 调用派生类的实现函数，将要显示的图像画入其中
	OnDrawData(dcMem, rc); 
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);

	// 创建图像列表，并将位图画入其中
	CImageList *pil = new CImageList();
	pil->Create(sz.cx, sz.cy, ILC_COLOR24|ILC_MASK, 0, 1); //使用了屏蔽标志时，有透明的效果
	pil->Add(&m_bitmap, BGCOLOR);
	return pil;
}

//////////////////
// 返回拖拉时，显示图像的大小
//
CSize CDragDropData::OnGetDragSize(CDC& dc)
{
	CRect rc(0,0,CARD_WIDTH,CARD_HEIGHT);
	return rc.Size();
}

//////////////////
// 将拖拉时显示的图像画出来
//
void CDragDropData::OnDrawData(CDC& dc, CRect& rc)
{
	if(m_cbCardData!=0)
	{
		//显示不带花的牌
		if(((m_cbCardData&MASK_COLOR)>>4)!=1)
		{
			g_CardResource.m_ImageUserBottom.DrawCardItem(&dc,m_cbCardData,0,0);
		}
		//显示带花的牌
		else
		{
			g_CardResource.m_ImageUserBottom1.DrawCardItem(&dc,m_cbCardData,0,0);
		}
	}
}