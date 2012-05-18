#include "StdAfx.h"
#include "DrawSiceWnd.h"

//////////////////////////////////////////////////////////////////////////
#define IDI_SICE_POS				203									//�ƶ���ʱ��
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDrawSiceWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDrawSiceWnd::CDrawSiceWnd()
{
	//������Դ
	m_bSicePos=0;
	m_nXPos=0;
	m_nYPos=0;
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_SICE_BACK,hInstance);
	
	return;
}

//��������
CDrawSiceWnd::~CDrawSiceWnd()
{
}

//�ػ�����
void CDrawSiceWnd::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&BufferImage);
	//�滭����
	CImageHandle BackImageHandle(&m_ImageBack);
	CImageHandle SiceImageFirstHandle(&m_ImageDrawSiceFirst);
	CImageHandle SiceImageSecondHandle(&m_ImageDrawSiceSecond);
	CImageHandle SiceImageFirstUserHandle(&m_ImageDrawSiceUserFirst);
	CImageHandle SiceImageSecondUserHandle(&m_ImageDrawSiceUserSecond);

	int nImageWidth=m_ImageBack.GetWidth();
	int nImageHeight=m_ImageBack.GetHeight();
	for (int nXPos=0;nXPos<rcClient.right;nXPos+=nImageWidth)
	{
		for (int nYPos=0;nYPos<rcClient.bottom;nYPos+=nImageHeight)
		{
			m_ImageBack.BitBlt(BufferDC,nXPos,nYPos);
		}
	}
	if(m_bSicePos<9)
	{
		m_ImageDrawSiceFirst.AlphaDrawImage(&BufferDC,30,0,m_ImageDrawSiceFirst.GetWidth()/9,m_ImageDrawSiceFirst.GetHeight(),
			m_bSicePos*m_ImageDrawSiceFirst.GetWidth()/9,0,RGB(0,0,254));
		m_ImageDrawSiceSecond.AlphaDrawImage(&BufferDC,30,0,m_ImageDrawSiceSecond.GetWidth()/9,m_ImageDrawSiceSecond.GetHeight(),
			m_bSicePos*m_ImageDrawSiceSecond.GetWidth()/9,0,RGB(0,0,254));

	}
	else 
	{
		m_ImageDrawSiceUserFirst.AlphaDrawImage(&BufferDC,30,0,m_ImageDrawSiceUserFirst.GetWidth()/9,m_ImageDrawSiceUserFirst.GetHeight(),
			m_bSicePos%9*m_ImageDrawSiceUserFirst.GetWidth()/9,0,RGB(0,0,254));
		m_ImageDrawSiceUserSecond.AlphaDrawImage(&BufferDC,30,0,m_ImageDrawSiceUserSecond.GetWidth()/9,m_ImageDrawSiceUserSecond.GetHeight(),
			m_bSicePos%9*m_ImageDrawSiceUserSecond.GetWidth()/9,0,RGB(0,0,254));

	}
	//�滭����
	
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);
	
	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//������Ϣ
int CDrawSiceWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	CImageHandle BackImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOREPOSITION);
	return 0;
}
//��ʱ����Ϣ
void CDrawSiceWnd::OnTimer(UINT nIDEvent)
{
	if(IDI_SICE_POS==nIDEvent)
	{


		if(m_bSicePos>=17)
		{
			if(m_bSicePos==17)
			{
				KillTimer(IDI_SICE_POS);
				m_bSicePos=0;
				Sleep(500);
				AfxGetMainWnd()->PostMessage(IDM_GAME_START,NULL,NULL);
			}
			m_bSicePos++;
		}
		else
		{
			if(m_bSicePos==8)
			{
				Sleep(500);
				AfxGetMainWnd()->PostMessage(IDM_SICE_TWO,NULL,NULL);
			}
			m_bSicePos++;


		}

	}
	Invalidate();

	__super::OnTimer(nIDEvent);
}
void CDrawSiceWnd::SetSiceInfo(CDC *dc,BYTE bMoveTime,BYTE SiceFirst,BYTE SiceSecond,BYTE SiceUserFirst,BYTE SiceUserSecond)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	ParentDC=dc;
	m_ImageBack.SetLoadInfo(IDB_SICE_BACK,hInstance);
	m_ImageDrawSiceFirst.SetLoadInfo(IDB_DRAW_SICE1+SiceFirst-1,hInstance);
	m_ImageDrawSiceSecond.SetLoadInfo(IDB_DRAW_SICE7+SiceSecond-1,hInstance);
	m_ImageDrawSiceUserFirst.SetLoadInfo(IDB_DRAW_SICE1+SiceUserFirst-1,hInstance);
	m_ImageDrawSiceUserSecond.SetLoadInfo(IDB_DRAW_SICE7+SiceUserSecond-1,hInstance);

	m_bSicePos=0;
	SetTimer(IDI_SICE_POS,bMoveTime,NULL);
	

}
//��׼λ��
void CDrawSiceWnd::SetBenchmarkPos(int nXPos, int nYPos)
{
	m_nXPos=nXPos;
	m_nYPos=nYPos;
}

//////////////////////////////////////////////////////////////////////////
