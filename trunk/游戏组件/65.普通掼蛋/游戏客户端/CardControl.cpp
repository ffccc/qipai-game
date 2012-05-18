#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"
#include ".\cardcontrol.h"

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
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//���ñ���
	m_bDisplay=false;
	m_bHorizontal=true;
	m_bPositively=false;
	m_bChange=false;

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
	m_dwSelectIndex=0xFFFFFFFF;

	//�ڲ�����
	m_pSinkWindow=NULL;

	//��Դ����
	if (m_bLoad==false)
	{
		//������Դ
		m_bLoad=true;
		m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());

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
		m_CardDataItem[i].bShoot	=	false;
		m_CardDataItem[i].bCardData	=	bCardData[i];
		m_CardDataItem[i].bSelect	=	false;
		m_CardDataItem[i].bMark		=	false;
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
	for(DWORD i=0;i<dwCardCount;i++)
	{
		
		tagCardItem * pCardItem=&m_CardDataItem[bCardDataIndex[i]];
		pCardItem->bShoot=true;
		if (m_hWnd!=NULL) RectifyControl();
	}
	return 0;
}

//���õ����˿�
DWORD CCardControl::SetShootCardData(const BYTE bCardData [], DWORD dwCardCount)
{
	ASSERT(dwCardCount<=(DWORD)m_CardDataItem.GetCount());
	//���ñ���
	tagCardItem * pCardItem;
	for(DWORD i=0;i<dwCardCount;i++)
	{
		for(DWORD j=0;j<(DWORD)m_CardDataItem.GetCount();j++)
		{
			pCardItem=&m_CardDataItem[j];
			if(pCardItem->bCardData==bCardData[i]&&!pCardItem->bShoot)
			{
				pCardItem->bShoot=true;
				break;
			}
		}
		
	}

	if (m_hWnd!=NULL) RectifyControl();
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
		ControlSize.cy=m_CardSize.cy+m_dwShootAltitude+6;
		ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;
		ControlSize.cx+=6;
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
			nXPos=m_dwCardHSpace*i+3;
			nYPos=(pCardItem->bShoot==false)?m_dwShootAltitude:0;
			nYPos+=3;
		}
		else 
		{
			nXPos=0;
			nYPos=m_dwCardVSpace*i;
		}

		//�ϲ�����
		//�˿˴�С
		CSize     CardSize;
		if(pCardItem->bSelect)
		{
			CardSize.cx	=	m_CardSize.cx+6;
			CardSize.cy	=	m_CardSize.cy+6;
			nYPos-=3;
			nXPos-=3;
		}
		else
		{
            CardSize.cx=m_CardSize.cx;
			CardSize.cy=m_CardSize.cy;
		}
		SignedRegion.CreateRectRgn(nXPos,nYPos,nXPos+CardSize.cx,nYPos+CardSize.cy);
		
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
		
		//��Сλ��
		if(MousePoint.x<=(LONG(m_dwCardHSpace+3)))
			return   0;

		//���λ��
		if(MousePoint.x>(LONG((dwCardCount-1)*m_dwCardHSpace+m_CardSize.cx+6)))
			return  0x0FFFFFFF;
		
		//��ȡ����
		DWORD dwCardIndex=(MousePoint.x-3)/m_dwCardHSpace;
		
		if (dwCardIndex>=dwCardCount) dwCardIndex=(dwCardCount-1);
		
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
			dwXImagePos=((pCardItem->bCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((pCardItem->bCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			dwXImagePos=m_CardSize.cx*2;
			dwYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		if (m_bHorizontal==true) 
		{
			dwXScreenPos=m_dwCardHSpace*i+3;
			dwYScreenPos=(pCardItem->bShoot==false)?m_dwShootAltitude:0;
			dwYScreenPos+=3;
		}
		else
		{
			dwXScreenPos=0;
			dwYScreenPos=m_dwCardVSpace*i;
		}

		//�滭�˿�
		m_ImageCard.BitBlt(BufferDC,dwXScreenPos,dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
        
		//��ѡ���
		if(pCardItem->bSelect)
		{
			CDC cDCBuffer;
			CBitmap bitmap;
			bitmap.LoadBitmap(IDB_SELECT);
			cDCBuffer.CreateCompatibleDC(&dc);
			cDCBuffer.SelectObject(bitmap);
			int xDest,yDest;
			xDest=m_dwSelectIndex*m_dwCardHSpace;
			yDest=m_dwShootAltitude;
			if(pCardItem->bShoot)
				yDest=0;
			BufferDC.TransparentBlt(xDest,yDest,79,104,&cDCBuffer,0,0,79,104,RGB(255,255,255));
		}
        
		//����˿�
		if(pCardItem->bMark)
		{
			CBitmap		bitmap;
			CDC			lBufferDC;
			//װ��λͼ
			bitmap.LoadBitmap(IDB_DIAMOND);
			lBufferDC.CreateCompatibleDC(&dc);
			lBufferDC.SelectObject(bitmap);
	
			int xDest,yDest;
			xDest=i*m_dwCardHSpace+4;
			if(!pCardItem->bShoot)
				yDest=m_CardSize.cy/3+m_dwShootAltitude;
			else
				yDest=m_CardSize.cy/3;
			BufferDC.TransparentBlt(xDest,yDest,15,14,&lBufferDC,0,0,15,14,RGB(255,255,255));
		}
		
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
	

	//�ͷ����
	ReleaseCapture();

	//״̬�ж�
	if (m_dwCurrentIndex==0xFFFFFFFF) return;
	if ((m_bHorizontal==false)||(m_bPositively==false)) return;
    
	
	//���ñ���
	tagCardItem * pCardItem;
	
    m_bChange=false;
	//���ÿؼ�
	if(m_dwSelectIndex!=0xFFFFFFFF)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		if(m_dwSelectIndex<(DWORD)m_CardDataItem.GetCount())
		{
			pCardItem=&m_CardDataItem[m_dwSelectIndex];
			pCardItem->bSelect=false;
			//����λ��
			if(pCardItem->bShoot)
			{
				//�ָ���ֵ
				m_dwSelectIndex=0xFFFFFFFF;
				//�����ؼ�
				RectifyControl();
				return ;
			}

		}
		
		ChangePosition(m_dwSelectIndex);
		m_dwSelectIndex=0xFFFFFFFF;
		//������Ϣ
		if (m_pSinkWindow!=NULL) m_pSinkWindow->SendMessage(IDM_CHANGE_POSITION,0,0);
		return ;
	}
    
	pCardItem=&m_CardDataItem[m_dwCurrentIndex];
	pCardItem->bShoot=!pCardItem->bShoot;
	
	//������Ϣ
	if (m_pSinkWindow!=NULL) m_pSinkWindow->SendMessage(IDM_LEFT_HIT_CARD,0,0);

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
	//�ж��±�
	if(m_dwCurrentIndex>=(DWORD)m_CardDataItem.GetCount())
	{	
		m_dwCurrentIndex=0xFFFFFFFF;
		return ;
	}
	tagCardItem * pCardItem=&m_CardDataItem[m_dwCurrentIndex];
	m_bChange=pCardItem->bShoot;
   

	return;
}

//�����Ϣ
BOOL CCardControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//���ù��
	if (m_bPositively==true)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,message);
}

//////////////////////////////////////////////////////////////////////////
//����ƶ�,�϶��˿�
void CCardControl::OnMouseMove(UINT nFlags, CPoint point)
{

   CWnd::OnMouseMove(nFlags, point);
   
   //״̬�ж�
   if ((m_bHorizontal==false)||(m_bPositively==false)) return;
   
   if(nFlags&MK_LBUTTON&&m_bChange)
   {
	   //���ù��
	   SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SELECT_CUR)));
	   //�������
	   SetCapture();
	
	   if(SwitchCardPoint(point)!=m_dwSelectIndex)
		{
			tagCardItem * pCardItem;
			if(m_dwSelectIndex!=0xFFFFFFFF&&(m_dwSelectIndex<(DWORD)m_CardDataItem.GetCount()))
			{
				pCardItem=&m_CardDataItem[m_dwSelectIndex];
				pCardItem->bSelect=false;
			}
			m_dwSelectIndex=SwitchCardPoint(point);
			if(m_dwSelectIndex<((DWORD)m_CardDataItem.GetCount()))
			{
				pCardItem=&m_CardDataItem[m_dwSelectIndex];
				pCardItem->bSelect=true;
			}
		    //�����ؼ�
			RectifyControl();
		}
   }
}

//�ı��˿�λ��
void CCardControl::ChangePosition(DWORD dwIndex)
{
	//��������
	BYTE  bCardData[PIECES],bShootCardData[PIECES];
	DWORD bShootCount=0L,bNotShootCount=0L;
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	DWORD bPosition;
    
	//�����˿�
	tagCardItem * pCardItem=NULL;
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];
		//��¼�˿�ֵ
		if(i==dwIndex)	bPosition=bNotShootCount;
			
		//�˿˷���
		if (pCardItem->bShoot==true) 
			bShootCardData[bShootCount++]=pCardItem->bCardData;
		else
			bCardData[bNotShootCount++]=pCardItem->bCardData;
	}
    
	//�����˿�
	if(dwIndex==0x0FFFFFFF)
	{
		CopyMemory(&bCardData[bNotShootCount],bShootCardData,bShootCount);
		BYTE bCard=GetMarkCard();
		SetCardData(bCardData,dwCardCount);
		if(bCard!=0)
			MarkCard(bCard,m_bSeries);
	}
	else
	{
		BYTE CardTemp[PIECES];
		CopyMemory(CardTemp,bCardData,bPosition);
		CopyMemory(&CardTemp[bPosition],bShootCardData,bShootCount);
		CopyMemory(&CardTemp[bPosition+bShootCount],&bCardData[bPosition],bNotShootCount-bPosition);
		BYTE bCard=GetMarkCard();
		SetCardData(CardTemp,dwCardCount);
		if(bCard!=0)
			MarkCard(bCard,m_bSeries);
	}

	return ;
}

//��ȡ�ؼ��˿�
DWORD CCardControl::GetCardData(BYTE  bCardData[], BYTE bCardCount)
{
	//Ч�����
	ASSERT(bCardCount>=(DWORD)m_CardDataItem.GetCount());

	//�����˿�
	DWORD  dwCardCount=(DWORD)m_CardDataItem.GetCount();
	tagCardItem *pCardItem;
	for(DWORD i=0;i<dwCardCount;i++)
	{
		pCardItem=&m_CardDataItem[i];
		bCardData[i]=pCardItem->bCardData;
	}


	return dwCardCount;
}


//��־�ɽ����˿�
void CCardControl::MarkCard(BYTE bCard,BYTE bSeries)
{
	//�������
	CGameLogic  lGameLogic;
	tagCardItem *pCardItem;
	
	//��������
	DWORD  dwCardCount=(DWORD)m_CardDataItem.GetCount();
	for(DWORD i=0;i<dwCardCount;i++)
	{
		pCardItem=&m_CardDataItem[i];
		if(lGameLogic.GetCardLogicValue(pCardItem->bCardData)==lGameLogic.GetCardLogicValue(bCard)
			&&pCardItem->bCardData!=bSeries)
		{
			pCardItem->bMark	=	true;
		}
	}

	Invalidate(true);
}

//��ȡ����˿�
BYTE CCardControl::GetMarkCard()
{
	tagCardItem *pCardItem;
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	for(DWORD i=0;i<dwCardCount;i++)
	{
		pCardItem=&m_CardDataItem[i];
		if(pCardItem->bMark)
		{
			return pCardItem->bCardData;
		}
	}

	return 0;
}
