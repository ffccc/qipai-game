#include "StdAfx.h"
#include "GameClient.h"
#include "SmallCardControl.h"

//////////////////////////////////////////////////////////////////////////

CSkinImage		 CSmallCardControl::m_ImageCard;
//////////////////////////////////////////////////////////////////////////

//���캯��
CSmallCardControl::CSmallCardControl()
{
	//״̬����
	m_bHorizontal=true;
	m_bPositively=false;
	m_bDisplayItem=false;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_CardItemArray,sizeof(m_CardItemArray));

	//�������
	m_nXDistance=DEF_X_DISTANCE;
	m_nYDistance=DEF_Y_DISTANCE;
	m_nShootDistance=DEF_SHOOT_DISTANCE;

	//λ�ñ���
	m_YCollocateMode=enYTop;
	m_XCollocateMode=enXLeft;
	m_BenchmarkPos.SetPoint(0,0);

	//������Դ
	m_ImageCard.SetLoadInfo(IDB_SMALL_CARD,AfxGetInstanceHandle());

	//��ȡ��С
	CImageHandle HandleImage(&m_ImageCard);
	m_CardSize.SetSize(m_ImageCard.GetWidth(),m_ImageCard.GetHeight());

	return;
}

//��������
CSmallCardControl::~CSmallCardControl()
{
}

//�����˿�
bool CSmallCardControl::SetCardData(WORD wCardCount)
{
	//ASSERT(wCardCount>=m_wCardCount);
	//if(m_wCardCount==2 && (wCardCount>=m_wCardCount))return false;

	//Ч�����
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//���ñ���
	m_wCardCount=wCardCount;
	ZeroMemory(m_CardItemArray,sizeof(m_CardItemArray));

	return true;
}

//�����˿�
bool CSmallCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//���ñ���
	m_wCardCount=wCardCount;

	//�����˿�
	for (WORD i=0;i<wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//�����˿�
bool CSmallCardControl::SetShootCard(const BYTE cbCardData[], WORD wCardCount)
{
	//��������
	bool bChangeStatus=false;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++) 
	{
		if (m_CardItemArray[i].bShoot==true)
		{
			bChangeStatus=true;
			m_CardItemArray[i].bShoot=false;
		}
	}

	//�����˿�
	for (WORD i=0;i<wCardCount;i++)
	{
		for (WORD j=0;j<m_wCardCount;j++)
		{
			if ((m_CardItemArray[j].bShoot==false)&&(m_CardItemArray[j].cbCardData==cbCardData[i])) 
			{
				bChangeStatus=true;
				m_CardItemArray[j].bShoot=true;
				break;
			}
		}
	}

	return bChangeStatus;
}

//�����˿�
bool CSmallCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_CardItemArray,CardItemArray,wCardCount*sizeof(tagCardItem));

	return true;
}

//��ȡ�˿�
tagCardItem * CSmallCardControl::GetCardFromIndex(WORD wIndex)
{
	return (wIndex<m_wCardCount)?&m_CardItemArray[wIndex]:NULL;
}

//��ȡ�˿�
tagCardItem * CSmallCardControl::GetCardFromPoint(CPoint & MousePoint)
{
	WORD wIndex=SwitchCardPoint(MousePoint);
	return (wIndex!=INVALID_ITEM)?&m_CardItemArray[wIndex]:NULL;
}

//��ȡ�˿�
WORD CSmallCardControl::GetCardData(BYTE cbCardData[], WORD wBufferCount)
{
	//Ч�����
	ASSERT(wBufferCount>=m_wCardCount);
	if (wBufferCount<m_wCardCount) return 0;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++) cbCardData[i]=m_CardItemArray[i].cbCardData;

	return m_wCardCount;
}

//��ȡ�˿�
WORD CSmallCardControl::GetShootCard(BYTE cbCardData[], WORD wBufferCount)
{
	//��������
	WORD wShootCount=0;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++) 
	{
		//Ч�����
		ASSERT(wBufferCount>wShootCount);
		if (wBufferCount<=wShootCount) break;

		//�����˿�
		if (m_CardItemArray[i].bShoot==true) cbCardData[wShootCount++]=m_CardItemArray[i].cbCardData;
	}

	return wShootCount;
}

//��ȡ�˿�
WORD CSmallCardControl::GetCardData(tagCardItem CardItemArray[], WORD wBufferCount)
{
	//Ч�����
	ASSERT(wBufferCount>=m_wCardCount);
	if (wBufferCount<m_wCardCount) return 0;

	//�����˿�
	CopyMemory(CardItemArray,m_CardItemArray,sizeof(tagCardItem)*m_wCardCount);

	return m_wCardCount;
}

//���þ���
VOID CSmallCardControl::SetCardDistance(UINT nXDistance, UINT nYDistance, UINT nShootDistance)
{
	//���ñ���
	m_nXDistance=nXDistance;
	m_nYDistance=nYDistance;
	m_nShootDistance=nShootDistance;

	return;
}

//��ȡ����
VOID CSmallCardControl::GetCenterPoint(CPoint & CenterPoint)
{
	//��ȡԭ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//��ȡλ��
	CSize ControlSize;
	GetControlSize(ControlSize);

	//��������
	CenterPoint.x=OriginPoint.x+ControlSize.cx/2;
	CenterPoint.y=OriginPoint.y+ControlSize.cy/2;

	return;
}

//��׼λ��
VOID CSmallCardControl::SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	return;
}

//��׼λ��
VOID CSmallCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	return;
}

//�滭�˿�
VOID CSmallCardControl::DrawCardControl(CDC * pDC)
{
	//����λͼ
	CImageHandle HandleCard(&m_ImageCard);

	//��ȡλ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//��������
	INT nXDrawPos=0,nYDrawPos=0;
	INT nXImagePos=0,nYImagePos=0;

	//�滭�˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//��ȡ�˿�
		bool bShoot=m_CardItemArray[i].bShoot;
		BYTE cbCardData=m_CardItemArray[i].cbCardData;

		//��϶����
		if (cbCardData==SPACE_CARD_DATA) continue;

		//ͼƬλ��
		if ((m_bDisplayItem==true)&&(cbCardData!=0))
		{
			if ((cbCardData==0x4E)||(cbCardData==0x4F))
			{
				nXImagePos=((cbCardData&CARD_MASK_VALUE)%14)*m_CardSize.cx;
				nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
			}
			else
			{
				nXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
				nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
			}
		}
		else
		{
			nXImagePos=0;
			nYImagePos=0;
		}

		//��Ļλ��
		if (m_bHorizontal==true)
		{
			nXDrawPos=m_nXDistance*i;
			nYDrawPos=(bShoot==false)?m_nShootDistance:0;
		}
		else
		{
			nXDrawPos=0;
			nYDrawPos=m_nYDistance*i;
		}

		//�滭�˿�
		m_ImageCard.AlphaDrawImage(pDC,OriginPoint.x+nXDrawPos,OriginPoint.y+nYDrawPos,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));
	}

	return;
}

//�����Ϣ
bool CSmallCardControl::OnEventSetCursor(CPoint Point)
{
	//��괦��
	if (m_bPositively==true)
	{
		//��ȡ����
		WORD wHoverItem=SwitchCardPoint(Point);

		//�����ж�
		if (wHoverItem!=INVALID_ITEM)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
	}

	return false;
}

//��ȡ��С
VOID CSmallCardControl::GetControlSize(CSize & ControlSize)
{
	//��ȡ��С
	if (m_bHorizontal==true)
	{
		ControlSize.cy=m_CardSize.cy+m_nShootDistance;
		ControlSize.cx=(m_wCardCount>0)?(m_CardSize.cx+(m_wCardCount-1)*m_nXDistance):0;
	}
	else
	{
		ControlSize.cx=m_CardSize.cx;
		ControlSize.cy=(m_wCardCount>0)?(m_CardSize.cy+(m_wCardCount-1)*m_nYDistance):0;
	}

	return;
}

//��ȡԭ��
VOID CSmallCardControl::GetOriginPoint(CPoint & OriginPoint)
{
	//��ȡλ��
	CSize ControlSize;
	GetControlSize(ControlSize);

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ OriginPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { OriginPoint.x=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ OriginPoint.x=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ OriginPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { OriginPoint.y=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { OriginPoint.y=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	return;
}

//�����л�
WORD CSmallCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//��ȡλ��
	CSize ControlSize;
	CPoint OriginPoint;
	GetControlSize(ControlSize);
	GetOriginPoint(OriginPoint);

	//��׼λ��
	INT nXPos=MousePoint.x-OriginPoint.x;
	INT nYPos=MousePoint.y-OriginPoint.y;

	//����ģʽ
	if (m_bHorizontal==true)
	{
		//Խ���ж�
		if ((nXPos<=0)||(nXPos>=ControlSize.cx)) return INVALID_ITEM;
		if ((nYPos<=0)||(nYPos>=ControlSize.cy)) return INVALID_ITEM;

		//��������
		WORD wCardIndex=nXPos/m_nXDistance;
		if (wCardIndex>=m_wCardCount) wCardIndex=(m_wCardCount-1);

		//�˿�����
		for (WORD i=0;i<=wCardIndex;i++)
		{
			//��������
			WORD wCurrentIndex=wCardIndex-i;

			//�������
			if (nXPos>=(INT)(wCurrentIndex*m_nXDistance+m_CardSize.cx)) break;

			//�������
			bool bShoot=m_CardItemArray[wCurrentIndex].bShoot;
			if ((bShoot==true)&&(nYPos<=m_CardSize.cy)) return wCurrentIndex;
			if ((bShoot==false)&&(nYPos>=(INT)m_nShootDistance)) return wCurrentIndex;
		}
	}

	return INVALID_ITEM;
}

// ��ָ��λ�û���һ����, bCardDataΪ0ʱ���Ʊ���
void CSmallCardControl::DrawOneCard(CDC* pDC, BYTE bCardData,int nX , int nY  )
{
	//��������

	COLORREF clrTrans = RGB( 255, 0, 255 );
	CImageHandle HandleCard(&m_ImageCard);
    
	m_ImageCard.AlphaDrawImage( pDC, nX, nY, SMALL_CARD_WIDTH, SMALL_CARD_HEIGHT
			, 0, 0
			, clrTrans );
	return;

}
//////////////////////////////////////////////////////////////////////////
