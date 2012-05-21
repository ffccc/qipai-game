#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

bool					CCardControl::m_bLoad=false;					//���ر�־
CSize					CCardControl::m_CardSize;						//�˿˴�С
CSkinImage				CCardControl::m_ImageCard;						//ͼƬ��Դ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCardControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//�������
	m_dwCardHSpace=25;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//��Դ����
	if (m_bLoad==false)
	{
		//������Դ
		m_bLoad=true;
		m_ImageCard.SetLoadInfo(IDB_GAME_CARD,AfxGetInstanceHandle());

		//��ȡ��С
		CImageHandle CardImageHandle(&m_ImageCard);
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;
	}

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//���ü��
void CCardControl::SetCardSpace(DWORD dwCardHSpace)
{
	//���ñ���
	m_dwCardHSpace=dwCardHSpace;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��ȡ��Ŀ
WORD CCardControl::GetCardCount()
{
	return (WORD)m_CardDataItem.GetCount();
}

//��ȡ�˿�
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wMaxCount)
{
	//�����˿�
	WORD wCopyCount=__min((WORD)m_CardDataItem.GetCount(),wMaxCount);
	CopyMemory(cbCardData,m_CardDataItem.GetData(),sizeof(BYTE)*wCopyCount);

	return wCopyCount;
}

//�����˿�
WORD CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//�����˿�
	m_CardDataItem.SetSize(wCardCount);
	for (WORD i=0;i<wCardCount;i++) m_CardDataItem[i]=cbCardData[i];

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return wCardCount;
}

//����λ��
void CCardControl::RectifyControl()
{
	//��������
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//�����С
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

	//����λ��
	int nXPos=0;
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
	case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//����λ��
	int nYPos=0;
	switch (m_YCollocateMode)
	{
	case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
	case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	//�ƶ�λ��
	SetWindowPos(NULL,nXPos,nYPos,ControlSize.cx,ControlSize.cy,SWP_NOZORDER);

	//��������
	CRgn CardRegion,SignedRegion;
	CardRegion.CreateRectRgn(0,0,0,0);

	//����λ��
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//����λ��
		nYPos=0;
		nXPos=m_dwCardHSpace*i;

		//�ϲ�����
		SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+m_CardSize.cx+1,nYPos+m_CardSize.cy+1,6,6);
		CardRegion.CombineRgn(&CardRegion,&SignedRegion,RGN_OR);
		SignedRegion.DeleteObject();
	}

	//��������
	SetWindowRgn(CardRegion,TRUE);
	m_CardRegion.DeleteObject();
	m_CardRegion.Attach(CardRegion.Detach());

	//�ػ�����
	Invalidate(TRUE);

	return;
}

//�ػ�����
void CCardControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λͼ
	CImageHandle HandleCard(&m_ImageCard);

	//����λͼ
	CBitmap BufferImage;
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&BufferImage);

	//�滭�˿�
	DWORD dwXImagePos,dwYImagePos;
	DWORD dwXScreenPos,dwYScreenPos;
	for (INT i=0;i<m_CardDataItem.GetCount();i++)
	{
		//��ȡ�˿�
		BYTE cbCardItem=m_CardDataItem[i];

		//ͼƬλ��
		if (cbCardItem!=0)
		{
			dwXImagePos=((cbCardItem&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((cbCardItem&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			dwXImagePos=m_CardSize.cx*2;
			dwYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		dwYScreenPos=0;
		dwXScreenPos=m_dwCardHSpace*i;

		//�滭�˿�
		m_ImageCard.AlphaDrawImage(&BufferDC,dwXScreenPos,dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos,RGB(255,0,255));
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//������Ϣ
int CCardControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
