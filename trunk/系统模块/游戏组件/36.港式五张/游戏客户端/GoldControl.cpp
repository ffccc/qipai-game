#include "Stdafx.h"
#include "Math.h"
#include "Resource.h"
#include "GoldControl.h"
#include "GameClientView.h"
#include ".\goldcontrol.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define IDC_ADD_CANCEL					108								//�����ť
#define IDC_ADD_TIMES1					109								//�����ť
#define IDC_ADD_TIMES2					110								//�����ť
#define IDC_ADD_TIMES3					111								//�����ť
#define IDC_ADD_TIMES4					112								//�����ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGoldControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ADD_CANCEL,OnBnCancel)
	ON_BN_CLICKED(IDC_ADD_TIMES1,OnBnAddTimes1)
	ON_BN_CLICKED(IDC_ADD_TIMES2,OnBnAddTimes2)
	ON_BN_CLICKED(IDC_ADD_TIMES3,OnBnAddTimes3)
	ON_BN_CLICKED(IDC_ADD_TIMES4,OnBnAddTimes4)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGoldControl::CGoldControl()
{
	//����
	m_lCellScore = 0L;
	m_lTimes = 0L;
	
	//��׼λ��
	m_BenchmarkPos.SetPoint(0,0);

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_ADD_INFO,hInstance);
	m_ImageNumber.SetLoadInfo(IDB_GOLD_NUMBER,hInstance);
}

//��������
CGoldControl::~CGoldControl()
{
}

//���õ�Ԫע
void CGoldControl::SetCellScore(LONG lCellScore)
{
	m_lCellScore = lCellScore;
	Invalidate(FALSE);
}

//����λ��
void CGoldControl::SetBenchmarkPos(int nXPos, int nYPos)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;

	//��������
	RectifyControl();

	return;
}

//�����ؼ�
void CGoldControl::RectifyControl()
{
	CImageHandle ImageBack(&m_ImageBack);
	MoveWindow(m_BenchmarkPos.x,m_BenchmarkPos.y,m_ImageBack.GetWidth(),m_ImageBack.GetHeight());

	CRect rcButton;
	//�ƶ���ť
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	//��ע��ť
	CRect rtClient;
	GetClientRect(&rtClient);
	INT nXPos = rtClient.Width()/2;
	INT nYPos = 40;
	m_btAddTimes1.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btAddTimes1,NULL,nXPos-rcButton.Width()*2-10*3/2,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes2,NULL,nXPos-rcButton.Width()-10/2,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes3,NULL,nXPos+10/2,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes4,NULL,nXPos+rcButton.Width()+10*3/2,nYPos,0,0,uFlags);

	m_btCancel.GetWindowRect(&rcButton);
	nYPos += 52;
	DeferWindowPos(hDwp,m_btCancel,NULL,nXPos-rcButton.Width()/2,nYPos,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//�ػ�����
void CGoldControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//��������ͼ
	CDC BackFaceDC;
	CBitmap BufferBmp;
	BufferBmp.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height());
	BackFaceDC.CreateCompatibleDC(&dc);
	BackFaceDC.SelectObject(&BufferBmp);

	//������Դ
	CImageHandle ImageHandle1(&m_ImageBack);
	CImageHandle ImageHandle3(&m_ImageNumber);

	//�滭����
	m_ImageBack.BitBlt(BackFaceDC,0,0);

	//�滭��Ԫע
	LONG lCellScore = m_lCellScore;
	BYTE byCell[10],byCellCount = 0;
	do
	{
		byCell[byCellCount++] = (BYTE)(lCellScore-lCellScore/10*10);
		lCellScore /= 10;
	}while( lCellScore > 0L );
	INT nNumberWidth = m_ImageNumber.GetWidth()/10;
	INT nNumberHeight = m_ImageNumber.GetHeight();
	INT nXPos = 70;
	INT nYPos = 9;
	for( BYTE i = 0; i < byCellCount; i++ )
	{
		m_ImageNumber.AlphaDrawImage(&BackFaceDC,nXPos,nYPos,nNumberWidth,nNumberHeight,
			nNumberWidth*byCell[byCellCount-i-1],0,RGB(255,0,255));
		nXPos += nNumberWidth;
	}

	//�滭����
	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&BackFaceDC,0,0,SRCCOPY);

	return;
}

//���ñ���
void CGoldControl::SetAddTimes( LONG lTimes )
{
	m_lTimes = lTimes;
	Invalidate(FALSE);
}

//�����ؼ�
int CGoldControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcCreate(0,0,0,0);
	m_btCancel.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_ADD_CANCEL);
	m_btAddTimes1.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_ADD_TIMES1);
	m_btAddTimes2.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_ADD_TIMES2);
	m_btAddTimes3.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_ADD_TIMES3);
	m_btAddTimes4.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_ADD_TIMES4);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btCancel.SetButtonImage(IDB_BT_CANCEL,hInstance,false);
	m_btAddTimes1.SetButtonImage(IDB_BT_TIMES1,hInstance,false);
	m_btAddTimes2.SetButtonImage(IDB_BT_TIMES2,hInstance,false);
	m_btAddTimes3.SetButtonImage(IDB_BT_TIMES3,hInstance,false);
	m_btAddTimes4.SetButtonImage(IDB_BT_TIMES4,hInstance,false);

	return 0;
}

//ȡ����ť
void CGoldControl::OnBnCancel()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,0,(LPARAM)-1);
	ShowWindow(SW_HIDE);
}

//1����ť
void CGoldControl::OnBnAddTimes1()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,0,(LPARAM)1);
	ShowWindow(SW_HIDE);
}

//2����ť
void CGoldControl::OnBnAddTimes2()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,0,(LPARAM)2);
	ShowWindow(SW_HIDE);
}

//3����ť
void CGoldControl::OnBnAddTimes3()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,0,(LPARAM)3);
	ShowWindow(SW_HIDE);
}

//4����ť
void CGoldControl::OnBnAddTimes4()
{
	AfxGetMainWnd()->SendMessage(IDM_ADD_SCORE,0,(LPARAM)4);
	ShowWindow(SW_HIDE);
}


//////////////////////////////////////////////////////////////////////////
