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
	m_ImageBack.BitBlt(BufferDC,0,0);

	if(m_bSicePos<9)
	{
		CImageHandle SiceImageFirstHandle(&m_ImageDrawSiceFirst);
		CImageHandle SiceImageSecondHandle(&m_ImageDrawSiceSecond);
		m_ImageDrawSiceFirst.AlphaDrawImage(&BufferDC,30,0,m_ImageDrawSiceFirst.GetWidth()/9,m_ImageDrawSiceFirst.GetHeight(),
			m_bSicePos*m_ImageDrawSiceFirst.GetWidth()/9,0,RGB(0,0,255));
		m_ImageDrawSiceSecond.AlphaDrawImage(&BufferDC,30,0,m_ImageDrawSiceSecond.GetWidth()/9,m_ImageDrawSiceSecond.GetHeight(),
			m_bSicePos*m_ImageDrawSiceSecond.GetWidth()/9,0,RGB(0,0,255));
	}
	else
	{
		CImageHandle SiceImageFirstUserHandle(&m_ImageDrawSiceUserFirst);
		CImageHandle SiceImageSecondUserHandle(&m_ImageDrawSiceUserSecond);
		m_ImageDrawSiceUserFirst.AlphaDrawImage(&BufferDC,30,0,m_ImageDrawSiceUserFirst.GetWidth()/9,m_ImageDrawSiceUserFirst.GetHeight(),
			m_bSicePos%9*m_ImageDrawSiceUserFirst.GetWidth()/9,0,RGB(0,0,255));
		m_ImageDrawSiceUserSecond.AlphaDrawImage(&BufferDC,30,0,m_ImageDrawSiceUserSecond.GetWidth()/9,m_ImageDrawSiceUserSecond.GetHeight(),
			m_bSicePos%9*m_ImageDrawSiceUserSecond.GetWidth()/9,0,RGB(0,0,255));
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
			KillTimer(IDI_SICE_POS);
			Sleep(1000);
			if( m_pSinkWnd )
				m_pSinkWnd->PostMessage(IDM_SICE_FINISH,NULL,NULL);
		}
		else
		{
			if(m_bSicePos==8)
			{
				Sleep(1000);
				if( m_pSinkWnd )
					m_pSinkWnd->PostMessage(IDM_SICE_TWO,NULL,NULL);
			}
			m_bSicePos++;
		}
		Invalidate();
	}
	
	__super::OnTimer(nIDEvent);
}

//����ɫ����Ϣ
void CDrawSiceWnd::SetSiceInfo(CDC *dc,BYTE bMoveTime,WORD wSice1,WORD wSice2)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	ParentDC=dc;
	m_ImageBack.SetLoadInfo(IDB_SICE_BACK,hInstance);
	BYTE SiceFirst,SiceSecond;
	if( wSice1 > 0 )
	{
		SiceFirst = HIBYTE(wSice1);
		SiceSecond = LOBYTE(wSice1);
		ASSERT( SiceFirst > 0 && SiceFirst < 7 && SiceSecond > 0 && SiceSecond < 7 );
		m_ImageDrawSiceFirst.SetLoadInfo(IDB_DRAW_SICE1+SiceFirst-1,hInstance);
		m_ImageDrawSiceSecond.SetLoadInfo(IDB_DRAW_SICE7+SiceSecond-1,hInstance);
	}
	if( wSice2 > 0 )
	{
		SiceFirst = HIBYTE(wSice2);
		SiceSecond = LOBYTE(wSice2);
		ASSERT( SiceFirst > 0 && SiceFirst < 7 && SiceSecond > 0 && SiceSecond < 7 );
		m_ImageDrawSiceUserFirst.SetLoadInfo(IDB_DRAW_SICE1+SiceFirst-1,hInstance);
		m_ImageDrawSiceUserSecond.SetLoadInfo(IDB_DRAW_SICE7+SiceSecond-1,hInstance);
	}
	m_bSicePos = (wSice1>0)?0:9;
	SetTimer(IDI_SICE_POS,bMoveTime,NULL);
}

//��׼λ��
void CDrawSiceWnd::SetBenchmarkPos(int nXPos, int nYPos)
{
	m_nXPos=nXPos;
	m_nYPos=nYPos;
}

//���ô��ڹ���
void CDrawSiceWnd::SetWindowSink(CWnd *pSinkWnd)
{
	m_pSinkWnd = pSinkWnd;
}


//////////////////////////////////////////////////////////////////////////
