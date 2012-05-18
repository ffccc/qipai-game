#include "StdAfx.h"
#include "dragdropmanager.h"
#include <windowsx.h>	// extensions--for GET_X_LPARAM

const UINT WM_DD_DRAGENTER = RegisterWindowMessage(_T("WM_DD_DRAGENTER"));
const UINT WM_DD_DRAGOVER  = RegisterWindowMessage(_T("WM_DD_DRAGOVER"));
const UINT WM_DD_DRAGDROP  = RegisterWindowMessage(_T("WM_DD_DRAGDROP"));
const UINT WM_DD_DRAGABORT = RegisterWindowMessage(_T("WM_DD_DRAGABORT"));

// ����һ���꣬�������Ϣ�еõ�������ڵĵ�
#define GETPOINT(lp) (CPoint(GET_X_LPARAM(lp), GET_Y_LPARAM(lp)))

// ���嵥���ƵĿ����߶�
#define CARD_WIDTH			36			//�˿˿��
#define CARD_HEIGHT			90			//�˿˸߶�

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
// MFC ���ŵ�֮һ�������������н�ͨ������ PreTranslateMessage 
//��������Թ��������Ӵ�����Ϣ����ʹ�� CDragDropMgr 
//���Խ��ڵ�һ�ĺ����б�ɽػ��͵��κ�Ǳ����קԴ���ڵ������Ϣ��
//�Ӷ������˱������໯ÿһ�����ڡ��� CDragDropMgr::PreTranslateMessage 
//���� WM_LBUTTONDOWN��������Ҹô��ھ����HWND���Ա������Ƿ�����Դ���ڱ�
//�������һ��Դ���ڣ��������ק��ʼ����������Ը���Ϣ��

BOOL CDragDropManager::PreTranslateMessage(MSG* pMsg)
{
	const MSG& msg = *pMsg;

	//�û��������󣬾ͽ�Դ������Ϊ�˲��񴰿ڣ���������������Ϣ�����͵��˴���
	if(IsDragDropWnd(msg.hwnd) && msg.message==WM_LBUTTONDOWN)
	{
 		 return OnLButtonDown(msg);
	}

	//�����ϷŴ��ڣ����Ҵ����Ϸ�״̬ʱ���Ž�ȥ������������Ϣ
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
// ������갴����Ϣ����ʼ���벶��״̬

BOOL CDragDropManager::OnLButtonDown(const MSG& msg)
{
	//CString str;
	//for(int i=0;i<9;i++)
	//{
	//	str.Format("%s  %d",str,::GetDlgCtrlID(m_hWnd[i]));
	//}
	//str.Format("%s  ĿǰΪ %d",str,::GetDlgCtrlID(msg.hwnd));
	//AfxMessageBox(str);

	m_hwndTracking = msg.hwnd;
	m_ptOrg = GETPOINT(msg.lParam);
	SetState(CAPTURED);
	return TRUE;
}

//////////////////
// ��������ƶ����������뵽�϶�״̬

BOOL CDragDropManager::OnMouseMove(const MSG& msg)
{
	//�����Ϸ�״̬�Ļ���ֱ�ӷ���
	if (!m_iState)	return FALSE;

	//�õ������ٵĴ���
	CWnd* pWnd = CWnd::FromHandle(m_hwndTracking);
	CPoint pt = GETPOINT(msg.lParam);
	DRAGDROPINFO& dd = m_info;

	if (IsDragging()) 
	{
		// �Ѿ��������϶�״̬�Ļ�������ʾ�϶���ͼ��
		pWnd->ClientToScreen(&pt);
		m_pDragImage->DragMove(pt);

		// �ж��Ƿ��϶����ﲻͬ�Ĵ��ڣ�����ʾ��ͬ�Ĺ��
		m_pMainWnd->ScreenToClient(&pt);
		dd.pt = pt;
		dd.hwndTarget = m_pMainWnd->ChildWindowFromPoint(pt)->GetSafeHwnd();
		m_pMainWnd->SendMessage(WM_DD_DRAGOVER, 0, (LPARAM)(void*)&dd);
		SetCursor(dd.hwndTarget && IsDragDropWnd(dd.hwndTarget) ? m_hCursorDrop : m_hCursorNo);
	} 
	else 
	{
		// ��û�н����϶�״̬�Ļ��������ý��뵽�϶�״̬
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
// �����û�����ͷŶ��������±��ϵ����ݺ󣬷��ص�ԭʼ״̬
BOOL CDragDropManager::OnLButtonUp(const MSG& msg)
{
	//û���϶�������£�ʲôҲ����
	if (!IsDragging()) 
	{
		SetState(NONE); 
		return FALSE;
	}
	//����Ŀ�Ĵ�����ʱ�����з��²���
	DRAGDROPINFO& dd = m_info;
	if (IsDragDropWnd(dd.hwndTarget) && IsDragDropWnd(dd.hwndSource) && dd.hwndTarget != dd.hwndSource) 
	{
		CPoint pt = GETPOINT(msg.lParam);
		CWnd* pWndSource = CWnd::FromHandle(dd.hwndSource);
		CWnd* pWndTarget = CWnd::FromHandle(dd.hwndTarget);
		pWndSource->ClientToScreen(&pt);
		pWndTarget->ScreenToClient(&pt);
		dd.pt = pt;
		//֪ͨ�����ڣ�ִ�з��²���
		m_pMainWnd->SendMessage(WM_DD_DRAGDROP,pWndTarget->GetDlgCtrlID(), (LPARAM)(void*)&dd);
	} 
	else 
	{
		//����Ŀ�괰�ڵĻ���������ϷŲ���
		m_pMainWnd->SendMessage(WM_DD_DRAGABORT, 0, 0);
	}

	delete m_info.data;
	m_info.data=NULL;
	SetState(NONE);
	return TRUE;
}

///////////////////////////////////////////////////////////
// ����������������״̬��NONE��CAPTURED �� DRAGGING
//   1�����û�����������CDragDropMgr ���� CAPTURED ״̬��
//   2�����û��ƶ���꣬����� DRAGGING ״̬��

void CDragDropManager::SetState(UINT iState)
{
	if (iState!=m_iState) 
	{
		//���û��������ʱ�����趨�����ٵĴ���Ϊ��겶�񴰿�
		if (iState==CAPTURED)
		{
			::SetCapture(m_hwndTracking);
		} 
		//���û��ƶ���ꡢ������ǰ�Ĺ��
		else if (iState==DRAGGING) 
		{
			m_hCursorSave = GetCursor();
		}
		//���û��ͷ����ʱ���������̽���
		else if (iState==NONE) 
		{
			::ReleaseCapture();				 // �ͷű����ٵĴ���
			SetCursor(m_hCursorSave);		 // ��ԭ���
			if (m_pDragImage) 
			{
				m_pDragImage->EndDrag();	 // ������������ͼ��
				delete m_pDragImage;		
				m_pDragImage=NULL;		
			}
			m_hwndTracking = NULL;          // ��ձ����ٵĴ���
		}
		m_iState = iState;
	}
}
/////////////////////////////////////////////////////////
// ����һ������ͼ���б�

CImageList* CDragDropData::CreateDragImage(CWnd* pWnd, CRect& rc)
{
	const COLORREF BGCOLOR = GetSysColor(COLOR_3DLIGHT);

	// ����һ����ԭ��������ݵ��ڴ�DC
	CWindowDC dcWin(pWnd);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dcWin);

	// ʹ����ԭ������ͬ������
	CFont* pFont = pWnd->GetFont();
	CFont* pOldFont = dcMem.SelectObject(pFont);

	// �õ�������ʾͼƬ�Ĵ�С
	// �����������ʵ�ֺ������õ�������ʾͼ��Ĵ�С
	CSize sz = OnGetDragSize(dcMem);
	rc = CRect(CPoint(0,0), sz);

	// ����ͼ���б�����λͼ������λͼ��������
	m_bitmap.CreateCompatibleBitmap(&dcWin, sz.cx, sz.cy);
	CBitmap* pOldBitmap = dcMem.SelectObject(&m_bitmap);
	CBrush brush;
	brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
	dcMem.FillRect(&rc,&brush);
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	// �����������ʵ�ֺ�������Ҫ��ʾ��ͼ��������
	OnDrawData(dcMem, rc); 
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);

	// ����ͼ���б�����λͼ��������
	CImageList *pil = new CImageList();
	pil->Create(sz.cx, sz.cy, ILC_COLOR24|ILC_MASK, 0, 1); //ʹ�������α�־ʱ����͸����Ч��
	pil->Add(&m_bitmap, BGCOLOR);
	return pil;
}

//////////////////
// ��������ʱ����ʾͼ��Ĵ�С
//
CSize CDragDropData::OnGetDragSize(CDC& dc)
{
	CRect rc(0,0,CARD_WIDTH,CARD_HEIGHT);
	return rc.Size();
}

//////////////////
// ������ʱ��ʾ��ͼ�񻭳���
//
void CDragDropData::OnDrawData(CDC& dc, CRect& rc)
{
	if(m_cbCardData!=0)
	{
		//��ʾ����������
		if(((m_cbCardData&MASK_COLOR)>>4)!=1)
		{
			g_CardResource.m_ImageUserBottom.DrawCardItem(&dc,m_cbCardData,0,0);
		}
		//��ʾ��������
		else
		{
			g_CardResource.m_ImageUserBottom1.DrawCardItem(&dc,m_cbCardData,0,0);
		}
	}
}