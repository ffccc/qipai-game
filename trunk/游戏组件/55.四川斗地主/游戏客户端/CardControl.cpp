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
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//���ñ���
	m_bDisplay=false;
	m_bHorizontal=true;
	m_bPositively=false;

	//�������
	m_dwCardHSpace=20;
	m_dwCardVSpace=19;
	m_dwShootAltitude=20;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//���б���
	m_dwCurrentIndex=0xFFFFFFFF;

	//�ڲ�����
	m_pSinkWindow=NULL;

	//��Դ����
	if (m_bLoad==false)
	{
		//������Դ
		m_bLoad=true;
		if (GetSystemMetrics(SM_CXSCREEN)>=1024) m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());
		else m_ImageCard.SetLoadInfo(IDB_CARD_800,AfxGetInstanceHandle());

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

//���ô���
void CCardControl::SetSinkWindow(CWnd * pSinkWindow)
{
	//���ñ���
	m_pSinkWindow=pSinkWindow;

	return;
}

//���÷���
void CCardControl::SetDirection(bool bHorizontal)
{
	//״̬�ж�
	if (m_bHorizontal==bHorizontal) return;

	//���ñ���
	m_bHorizontal=bHorizontal;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//�����˿�
void CCardControl::ShootAllCard(bool bShoot)
{
	//��������
	bool bRectify=false;
	tagCardItem * pCardItem=NULL;
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//�����˿�
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//�˿˷���
		if (pCardItem->bShoot!=bShoot)
		{
			bRectify=true;
			pCardItem->bShoot=bShoot;
		}
	}

	//�����ؼ�
	if (bRectify==true) RectifyControl();

	return;
}

//������ʾ
void CCardControl::SetDisplayFlag(bool bDisplay)
{
	//״̬�ж�
	if (m_bDisplay==bDisplay) return;

	//���ñ���
	m_bDisplay=bDisplay;

	//�ػ��ؼ�
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������Ӧ
void CCardControl::SetPositively(bool bPositively)
{
	//���ñ���
	m_bPositively=bPositively;

	return;
}

//���ü��
void CCardControl::SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace, DWORD dwShootAltitude)
{
	//���ñ���
	m_dwCardHSpace=dwCardHSpace;
	m_dwCardVSpace=dwCardVSpace;
	m_dwShootAltitude=dwShootAltitude;

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
DWORD CCardControl::GetCardCount()
{
	return (DWORD)m_CardDataItem.GetCount();
}

//��ȡ�˿�
DWORD CCardControl::GetShootCard(BYTE bCardData[], DWORD dwMaxCount)
{
	//��������
	DWORD bShootCount=0L;
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//�����˿�
	tagCardItem * pCardItem=NULL;
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//�˿˷���
		if (pCardItem->bShoot==true) bCardData[bShootCount++]=pCardItem->bCardData;
	}

	return bShootCount;
}

//�����˿�
DWORD CCardControl::SetCardData(const BYTE bCardData[], DWORD dwCardCount)
{
	//�����˿�
	m_CardDataItem.SetSize(dwCardCount);
	for (DWORD i=0;i<dwCardCount;i++)
	{
		m_CardDataItem[i].bShoot=false;
		m_CardDataItem[i].bCardData=bCardData[i];
	}

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return dwCardCount;
}

//��ȡ�˿�
DWORD CCardControl::GetCardData(tagCardItem CardItemArray[], DWORD dwMaxCount)
{
	//Ч�����
	ASSERT(dwMaxCount>=(DWORD)m_CardDataItem.GetCount());

	//�����˿�
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	tagCardItem * pCardItemData=m_CardDataItem.GetData();
	CopyMemory(CardItemArray,pCardItemData,dwCardCount*sizeof(tagCardItem));

	return dwCardCount;
}

//�����˿�
DWORD CCardControl::SetCardItem(const tagCardItem CardItemArray[], DWORD dwCardCount)
{
	//�����˿�
	m_CardDataItem.SetSize(dwCardCount);
	CopyMemory(m_CardDataItem.GetData(),CardItemArray,dwCardCount*sizeof(tagCardItem));

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();
 
	return 0;
}

//���õ����˿�
DWORD CCardControl::SetShootCard(const BYTE bCardDataIndex[], DWORD dwCardCount)
{
	//��������
	bool bChangeStatus=false;

	//�����˿�
	for (WORD i=0;i<m_CardDataItem.GetCount();i++) 
	{
		if (m_CardDataItem[i].bShoot==true)
		{
			bChangeStatus=true;
			m_CardDataItem[i].bShoot=false;
		}
	}

	//�����˿�
	for (WORD i=0;i<dwCardCount; ++i)	{
		for (WORD j=0;j<m_CardDataItem.GetCount();j++)
		{
			if ((m_CardDataItem[j].bShoot==false)&&(m_CardDataItem[j].bCardData==bCardDataIndex[i])) 
			{
				bChangeStatus=true;
				m_CardDataItem[j].bShoot=true;
				break;
			}
		}
	}

	//�����ؼ�
	RectifyControl();

	return 0;
}

//����λ��
void CCardControl::RectifyControl()
{
	//��������
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//�����С
	CSize ControlSize;
	if (m_bHorizontal==true)
	{
		ControlSize.cy=m_CardSize.cy+m_dwShootAltitude;
		ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;
	}
	else
	{
		ControlSize.cx=m_CardSize.cx;
		ControlSize.cy=(dwCardCount>0)?(m_CardSize.cy+(dwCardCount-1)*m_dwCardVSpace):0;
	}

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
	tagCardItem * pCardItem=NULL;

	//��������
	CardRegion.CreateRectRgn(0,0,0,0);
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//����λ��
		if (m_bHorizontal==true)
		{
			nXPos=m_dwCardHSpace*i;
			nYPos=(pCardItem->bShoot==false)?m_dwShootAltitude:0;
		}
		else 
		{
			nXPos=0;
			nYPos=m_dwCardVSpace*i;
		}

		//�ϲ�����
		SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+m_CardSize.cx+1,nYPos+m_CardSize.cy+1,2,2);
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

//�����л�
DWORD CCardControl::SwitchCardPoint(const CPoint & MousePoint)
{
	if (m_bHorizontal==true)
	{
		//��������
		DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

		//��ȡ����
		DWORD dwCardIndex=MousePoint.x/m_dwCardHSpace;
		if (dwCardIndex>=dwCardCount) dwCardIndex=(dwCardCount-1);

		//�жϰ���
		bool bCurrentCard=true;
		bool bCardShoot=m_CardDataItem[dwCardIndex].bShoot;
		if ((bCardShoot==true)&&(MousePoint.y>m_CardSize.cy)) bCurrentCard=false;
		if ((bCardShoot==false)&&(MousePoint.y<(INT)m_dwShootAltitude)) bCurrentCard=false;

		//��ǰѰ��
		if (bCurrentCard==false)
		{
			while (dwCardIndex>0)
			{
				dwCardIndex--;
				bCardShoot=m_CardDataItem[dwCardIndex].bShoot;
				if ((bCardShoot==false)&&(MousePoint.y>m_CardSize.cx)) break;
				if ((bCardShoot==true)&&(MousePoint.y<(INT)m_dwShootAltitude)) break;
			}
		}

		return dwCardIndex;
	}

	return DWORD(-1);
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
		tagCardItem * pCardItem=&m_CardDataItem[i];

		//ͼƬλ��
		if ((m_bDisplay==true)&&(pCardItem->bCardData!=0))
		{
			if ((pCardItem->bCardData==0x4E)||(pCardItem->bCardData==0x4F))
			{
				dwXImagePos=((pCardItem->bCardData&CARD_MASK_VALUE)%14)*m_CardSize.cx;
				dwYImagePos=((pCardItem->bCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
			}
			else
			{
				dwXImagePos=((pCardItem->bCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
				dwYImagePos=((pCardItem->bCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
			}
		}
		else
		{
			dwXImagePos=m_CardSize.cx*2;
			dwYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		if (m_bHorizontal==true) 
		{
			dwXScreenPos=m_dwCardHSpace*i;
			dwYScreenPos=(pCardItem->bShoot==false)?m_dwShootAltitude:0;
		}
		else
		{
			dwXScreenPos=0;
			dwYScreenPos=m_dwCardVSpace*i;
		}

		//�滭�˿�
		m_ImageCard.AlphaDrawImage( &BufferDC, dwXScreenPos, dwYScreenPos,m_CardSize.cx, m_CardSize.cy, dwXImagePos, dwYImagePos, 
			RGB( 255, 0, 255 ) );
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

//�����Ϣ
void CCardControl::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//״̬�ж�
	if ((m_bHorizontal==false)||(m_bPositively==false)) return;

	//������Ϣ
	if (m_pSinkWindow!=NULL) m_pSinkWindow->PostMessage(IDM_RIGHT_HIT_CARD,0,0);

	return;
}

//�����Ϣ
void CCardControl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//״̬�ж�
	if (m_dwCurrentIndex==0xFFFFFFFF) return;
	if ((m_bHorizontal==false)||(m_bPositively==false)) return;

	//��ȡ����
	DWORD dwCurrentIndex=SwitchCardPoint(Point);
	if (dwCurrentIndex!=m_dwCurrentIndex) return;

	//���ÿؼ�
	tagCardItem * pCardItem=&m_CardDataItem[dwCurrentIndex];
	pCardItem->bShoot=!pCardItem->bShoot;

	//������Ϣ
	if (m_pSinkWindow!=NULL) m_pSinkWindow->PostMessage(IDM_LEFT_HIT_CARD,0,0);

	//�����ؼ�
	RectifyControl();

	return;
}

//�����Ϣ
void CCardControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//״̬�ж�
	if ((m_bHorizontal==false)||(m_bPositively==false)) return;

	//��ȡ����
	m_dwCurrentIndex=SwitchCardPoint(Point);

	return;
}

//�����Ϣ
BOOL CCardControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//���ù��
	if (m_bPositively==true)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,message);
}

/////////////////////////////////////////////////////////////////////
