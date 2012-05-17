#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"


#include "math.h"
#include "urlmon.h"
#include ".\gameclientview.h"


extern struct  DrawInfoStruct			g_DrawInfo;

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()	

	//��ť
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_EXERCISE,OnExecise)
	ON_MESSAGE(IDM_QUIT,OnQuit)

	//����
	ON_MESSAGE(IDM_GUIDE,OnGuide)
	ON_MESSAGE(IDM_RESET,OnRealign)
	ON_MESSAGE(IDM_BOM,OnBomb)
	ON_MESSAGE(IDM_STOPIT,OnStopIt)
	ON_MESSAGE(IDM_CLOG,OnClog)
	//
	ON_MESSAGE(IDM_D3D_LINK,OnLink)

	//
	ON_MESSAGE(IDM_CHATCOLOR,OnChatColor)

	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void) : CGameFrameView(true,24),m_MyListView(this)
{	
	m_hButtonCursor = AfxGetApp()->LoadCursor(IDC_MY_HAND);
	::SetCursor(m_hButtonCursor);
	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
	m_MemDC.DeleteDC();
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	CRect CreateRect(0,0,0,0);

	//��������
	m_wndD3D.CreateEx(0,AfxRegisterWndClass(0,NULL,NULL,NULL),"00",
		WS_CHILD|WS_VISIBLE,CRect(0,0,788,768),this,0);
	m_wndD3D.ShowWindow(SW_SHOW);

	m_MyListView.Create(IDD_LISTINFO,this);
	m_MyListView.MoveWindow(789,28,235,768,false);
	m_MyListView.ShowWindow(SW_SHOW);

	SetTimer(0,10,NULL);
	if(!m_GameOption.Create(IDD_OPTION, &m_wndD3D))
	{
		return -1;
	}
	m_GameOption.ShowWindow(SW_HIDE);
	return 0;
}



//���ý���
void CGameClientView::ResetGameView()
{
	
	m_wndD3D.InitChess();
	m_wndD3D.UpdateBackSurface();
	return;
}

//�����ؼ�(onsize())
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��ť�ؼ�
	HDWP hDwp=BeginDeferWindowPos(4);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_MyListView.SetWindowPos(NULL,788,0,0,0,SWP_NOSIZE|SWP_NOZORDER);

	EndDeferWindowPos(hDwp);
	
	
	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	if(pDC == NULL)
		return;

	return;
}

BOOL CGameClientView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CString sClassName;
	::GetClassName(pWnd->GetSafeHwnd(),sClassName.GetBuffer(80),80);
	
	if (sClassName=="Button"  && m_hButtonCursor) {
		CRect rect;
		pWnd->GetWindowRect(rect);
		CPoint point;
		point.x = rect.left+10;
		point.y = rect.top-10;
		OnMouseMove(0,point);
		::SetCursor(m_hButtonCursor);
		return TRUE;
	}
	::SetCursor(m_hButtonCursor);
	return true;
}

void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	//CGameFrameView::OnMouseMove(nFlags, point);
}

void CGameClientView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	CGameFrameView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGameClientView::OnRButtonDown(UINT nFlags, CPoint point)
{

	CGameFrameView::OnRButtonDown(nFlags, point);
}


BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{
	return CGameFrameView::PreTranslateMessage(pMsg);
}

void CGameClientView::OnTimer(UINT nIDEvent)
{
}



// �����û���Ϣ
void CGameClientView::MySetUserInfo(WORD wChairID, tagUserData * pUserData)
{

	m_wndD3D.m_UserData[wChairID] = pUserData;

	if(pUserData == NULL)
	{
		m_wndD3D.UserFlee(wChairID);
	}

	if(wChairID == 2)
	{
		m_MyListView.SetUserDataItem(pUserData);
	}
}




void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
}

// ��ʼ��ť
LRESULT CGameClientView::OnStart(WPARAM wParam, LPARAM lParam)
{
	m_wndD3D.InitChess();
	m_wndD3D.UpdateBackSurface();
	//����׼����Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	m_MyListView.m_btStart.EnableWindow(false);
	return true;
}

LRESULT CGameClientView::OnQuit(WPARAM wParam, LPARAM lParam)
{
	::AfxGetMainWnd()->SendMessage(IDM_QUIT,0,0);
	return true;
}

// ��ϰ
LRESULT CGameClientView::OnExecise(WPARAM wParam, LPARAM lParam)
{
	m_wndD3D.m_ExerciseMode = true;
	m_wndD3D.Execise();
	m_MyListView.m_ImplementCount[PROP_GUIDE] = 3;
	m_MyListView.m_ImplementCount[PROP_RESET] = 3;
	m_MyListView.UpdateImplementState();
	AfxGetMainWnd()->SendMessage(IDM_D3D_ALLMUSIC,IDM_MUSIC_BG,0);
	return true;
}

LRESULT CGameClientView::OnLink(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(AfxGetMainWnd()->m_hWnd,IDM_D3D_LINK,wParam,0);

	if(m_wndD3D.m_ExerciseMode)
	{
		if(m_Logic.ChessEmpty(m_wndD3D.m_Chess[2]) == false)
			m_wndD3D.Execise();
	}
	return true;
}

// �Թ������Ӵ���
void CGameClientView::LinkPro(CPoint first, CPoint end, WORD userid)
{
	m_wndD3D.m_nSelectFirst = first;
	m_wndD3D.m_nSelectSecond = end;
	m_wndD3D.LinkProcess(first,end,userid);
}

// ����������Ӵ���
void CGameClientView::OtherPlayLinkPro(CPoint first, CPoint end, WORD userid)
{
	m_wndD3D.m_Chess[userid][first.y][first.x] = ERROR_ALL;
	m_wndD3D.m_Chess[userid][end.y][end.x] = ERROR_ALL;

	//�������̱���
	m_wndD3D.sExpunctionChess(userid,first);
	m_wndD3D.sExpunctionChess(userid,end);
}

// ������ɫ
LRESULT CGameClientView::OnChatColor(WPARAM wParam, LPARAM lParam)
{
	//������ɫ
	CColorDialog ColorDialog(g_GlobalOption.m_crChatTX,CC_FULLOPEN);
	
	if (ColorDialog.DoModal()==IDOK) 
	{
		g_GlobalOption.m_crChatTX=ColorDialog.GetColor();
	}
	
	return true;
}


// ָ����
LRESULT CGameClientView::OnGuide(WPARAM wParam, LPARAM lParam)
{
	m_wndD3D.Guide();
	return true;
}

// ����
LRESULT CGameClientView::OnRealign(WPARAM wParam, LPARAM lParam)
{
	m_wndD3D.Realign();
	return true;
}

// ը��
LRESULT CGameClientView::OnBomb(WPARAM wParam, LPARAM lParam)
{
	m_wndD3D.AutoLink();
	if(m_wndD3D.m_ExerciseMode)
	{
		if(m_Logic.ChessEmpty(m_wndD3D.m_Chess[2]) == false)
			m_wndD3D.Execise();
	}
	return true;
}

// ����
LRESULT CGameClientView::OnStopIt(WPARAM wParam, LPARAM lParam)
{
	m_wndD3D.m_nMouseState = MOUSE_STOPIT;
	return true;
}

// �ϰ�
LRESULT CGameClientView::OnClog(WPARAM wParam, LPARAM lParam)
{
	m_wndD3D.m_nMouseState = MOUSE_CLOG;
	return true;
}
