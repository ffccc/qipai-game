#include "StdAfx.h"
#include "TimerControl.h"
#include "Struct.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTimerControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerControl::CTimerControl()
{
	//���ñ���
	m_bDisplay=false;

	//λ�ñ���
	m_BenchmarkPos.SetPoint(0,0);

	//���б���
	m_dwCurrentIndex=0xFFFFFFFF;

	//�ڲ�����
	m_pSinkWindow=NULL;

	m_nTimer=-1;

	m_bNotify=FALSE;

	m_bPause=FALSE;

	m_bDes=FALSE;
	return;
}

//��������
CTimerControl::~CTimerControl()
{
}

void CTimerControl::SetBitmap(UINT uIdBack,UINT uIdArrow)
{
	m_ImageBack.SetLoadInfo(uIdBack,AfxGetInstanceHandle());
	m_ImageArrow.SetLoadInfo(uIdArrow,AfxGetInstanceHandle());
	CImageHandle HandleBack(&m_ImageBack);
	m_Size.cx=m_ImageBack.GetWidth();
	m_Size.cy=m_ImageBack.GetHeight();
}

//���ô���
void CTimerControl::SetSinkWindow(CWnd * pSinkWindow)
{
	//���ñ���
	m_pSinkWindow=pSinkWindow;
}

//���õ���ʱ��,directionΪ����,secordΪ����,bNotify�ǵ�ʱ�Ƿ�֪ͨ
void CTimerControl::SetTimerUp(int direction,int secord,BOOL bNotify)
{
	m_dwCurrentIndex=direction;
	m_nTimer=secord*2;
	m_bNotify=bNotify;
	SetPause(false);
	ShowTimer(true);
}

void CTimerControl::ShowTimer(bool bDisplay)
{
	//״̬�ж�
	if (m_bDisplay==bDisplay) return;

	//���ñ���
	m_bDisplay=bDisplay;

	if (m_bDisplay==false)
		m_nTimer=-1;

	//�ػ��ؼ�
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//��׼λ��
void CTimerControl::SetBenchmarkPos(int nXPos, int nYPos)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();
}

//����λ��
void CTimerControl::RectifyControl()
{
	int nXPos=0;
	int nYPos=0;
	nXPos=m_BenchmarkPos.x-m_Size.cx/2;
	nYPos=m_BenchmarkPos.y-m_Size.cy/2;
	//�ƶ�λ��
	SetWindowPos(NULL,nXPos,nYPos,m_Size.cx,m_Size.cy,SWP_NOZORDER);

	//�ػ�����
	Invalidate(TRUE);
	return;
}

//�ػ�����
void CTimerControl::OnPaint()
{
	CPaintDC dc(this);
	SetupRgn( &dc, m_ImageBack, RGB( 255, 0, 255) );
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λͼ
	CImageHandle HandleBack(&m_ImageBack);
	CImageHandle HandleArrow(&m_ImageArrow);

	//����λͼ
	CBitmap BufferImage;
	if (!BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height())) return;

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&BufferImage);

	//m_ImageBack.BitBlt(BufferDC,0,0);
	m_ImageBack.AlphaDrawImage( &BufferDC, 0, 0, COLORTRANS );
	switch(m_dwCurrentIndex)
	{
		case 0:
			m_ImageArrow.AlphaDrawImage(&BufferDC,rcClient.Width()/2-m_ImageArrow.GetWidth()/4/2,0,16,16,0,0,RGB(255,0,255));
			break;
		case 3:
			if (m_bDes)
				m_ImageArrow.AlphaDrawImage(&BufferDC,rcClient.Width()-m_ImageArrow.GetWidth()/4,rcClient.Height()/2-m_ImageArrow.GetHeight()/2,16,16,48,0,RGB(255,0,255));
			else
				m_ImageArrow.AlphaDrawImage(&BufferDC,0,rcClient.Height()/2-m_ImageArrow.GetHeight()/2,16,16,16,0,RGB(255,0,255));
			break;
		case 2:
			m_ImageArrow.AlphaDrawImage(&BufferDC,rcClient.Width()/2-m_ImageArrow.GetWidth()/4/2,rcClient.Height()-m_ImageArrow.GetHeight(),16,16,32,0,RGB(255,0,255));
			break;
		case 1:
			if (m_bDes)
				m_ImageArrow.AlphaDrawImage(&BufferDC,0,rcClient.Height()/2-m_ImageArrow.GetHeight()/2,16,16,16,0,RGB(255,0,255));
			else
				m_ImageArrow.AlphaDrawImage(&BufferDC,rcClient.Width()-m_ImageArrow.GetWidth()/4,rcClient.Height()/2-m_ImageArrow.GetHeight()/2,16,16,48,0,RGB(255,0,255));
			break;
	}

	BufferDC.SetTextColor(RGB(220,220,220));
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextAlign(TA_CENTER);
	CString str;
	if (m_nTimer!=-1)
		str.Format("%02d",m_nTimer/2);
	else
		str="00";
	BufferDC.TextOut(rcClient.Width()/2+2,rcClient.Height()/2-5,str);

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//������Ϣ
int CTimerControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	SetTimer(1245,500,NULL);
	return 0;
}

void CTimerControl::OnTimer(UINT_PTR nIDEvent)
{
	if (m_nTimer!=-1 && m_bPause==FALSE)
	{
		m_nTimer--;
		Invalidate();
		if (m_nTimer==-1 && m_bNotify && m_pSinkWindow)
			m_pSinkWindow->SendMessage(IDM_TIMER_OUT,0,0);
	}
}

void CTimerControl::SetPause(BOOL bPause)
{
	m_bPause=bPause;
	//m_nTimer= 0;
}

void CTimerControl::SetDesOrder(bool bDes)
{
	m_bDes=bDes;
}

//͸������
void CTimerControl::SetupRgn(CDC *pDC,CSkinImage bImage,COLORREF TransColor){	//bImage.GetWidth()/5;
	tagImageLoadInfo ImageIDB;
	bImage.GetLoadInfo(ImageIDB);
	CBitmap cBitmap;
	cBitmap.LoadBitmap(ImageIDB.uResourceID);//���BITMAP��5�����ģ����Ի�������������
	CDC memDC;
	//�����봫��DC���ݵ���ʱDC
	memDC.CreateCompatibleDC(pDC);

	CBitmap *pOldMemBmp=NULL;
	//��λͼѡ����ʱDC
	pOldMemBmp=memDC.SelectObject(&cBitmap);

	CRgn wndRgn;
	//�����ܵĴ������򣬳�ʼregionΪ0
	wndRgn.CreateRectRgn(0,0,0,0);


	BITMAP bit;   
	cBitmap.GetBitmap (&bit);//ȡ��λͼ����������Ҫ�õ�λͼ�ĳ��Ϳ�     

	int y;
	for(y=0;y<=bit.bmHeight  ;y++)
	{
		CRgn rgnTemp; //������ʱregion

		int iX = 0;
		do
		{
			//����͸��ɫ�ҵ���һ����͸��ɫ�ĵ�.
			while (iX <= bit.bmWidth  && memDC.GetPixel(iX, y) == TransColor)
				iX++;

			//��ס�����ʼ��
			int iLeftX = iX;

			//Ѱ���¸�͸��ɫ�ĵ�
			while (iX <= bit.bmWidth  && memDC.GetPixel(iX, y) != TransColor)
				++iX;

			//����һ������������ص���Ϊ1���ص���ʱ��region��
			rgnTemp.CreateRectRgn(iLeftX, y, iX, y+1);

			//�ϲ�����"region".
			wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_OR);

			//ɾ����ʱ"region",�����´δ���ʱ�ͳ���
			rgnTemp.DeleteObject();
		}while(iX <bit.bmWidth );
		iX = 0;
	}
	if(pOldMemBmp)
		memDC.SelectObject(pOldMemBmp);
	CWnd * pWnd = pDC->GetWindow();
	pWnd->SetWindowRgn(wndRgn,TRUE);
	pWnd->SetForegroundWindow();   } 