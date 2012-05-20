#include "StdAfx.h"
#include "GameClient.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bHorizontal=true;
	m_bPositively=false;
	m_bDisplayItem=false;

	//�˿�����
	m_cbCardCount=0;
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
	m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());
	m_ImageCardMask.SetLoadInfo(IDB_CARD_MASK,AfxGetInstanceHandle());
	m_ImageWin.SetLoadInfo(IDB_MAX,AfxGetInstanceHandle());

	//��ȡ��С
	CImageHandle HandleImage(&m_ImageCard);
	m_CardSize.SetSize(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//�����˿�
bool CCardControl::SetCardData(BYTE cbCardCount)
{
	//Ч�����
	ASSERT(cbCardCount<=CountArray(m_CardItemArray));
	if (cbCardCount>CountArray(m_CardItemArray)) return false;

	//���ñ���
	m_cbCardCount=cbCardCount;
	ZeroMemory(m_CardItemArray,sizeof(m_CardItemArray));

	return true;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч�����
	ASSERT(cbCardCount<=CountArray(m_CardItemArray));
	if (cbCardCount>CountArray(m_CardItemArray)) return false;

	//���ñ���
	m_cbCardCount=cbCardCount;

	//�����˿�
	for (WORD i=0;i<cbCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].bEffect = false;
		m_CardItemArray[i].bMy = false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//�����˿�
bool CCardControl::SetShootCard(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	bool bChangeStatus=false;

	//�����˿�
	for (WORD i=0;i<m_cbCardCount;i++) 
	{
		if (m_CardItemArray[i].bShoot==true)
		{
			bChangeStatus=true;
			m_CardItemArray[i].bShoot=false;
		}
	}

	//�����˿�
	for (WORD i=0;i<cbCardCount;i++)
	{
		for (WORD j=0;j<m_cbCardCount;j++)
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

//������Ч
bool CCardControl::SetCardEffect(const BYTE cbCardData[],BYTE cbCardCount)
{
	//��������
	BYTE bTempCount=0;

	//�����˿�
	for (WORD i=0;i<cbCardCount;i++)
	{
		for (WORD j=0;j<m_cbCardCount;j++)
		{
			if ((m_CardItemArray[j].bEffect==false)&&(m_CardItemArray[j].cbCardData==cbCardData[i])) 
			{
				bTempCount++;
				m_CardItemArray[j].bEffect=true;
				break;
			}
		}
	}

	return (bTempCount==cbCardCount);
}

//���ñ�־
bool CCardControl::SetMyCard(const BYTE cbCardData[],BYTE cbCardCount)
{
	//��������
	BYTE bTempCount=0;

	//��־�˿�
	for (WORD i=0;i<cbCardCount;i++)
	{
		for (WORD j=0;j<m_cbCardCount;j++)
		{
			if ((m_CardItemArray[j].bMy==false)&&(m_CardItemArray[j].cbCardData==cbCardData[i])) 
			{
				bTempCount++;
				m_CardItemArray[j].bMy=true;
				break;
			}
		}
	}

	return (bTempCount==cbCardCount);
}

//�����˿�
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], BYTE cbCardCount)
{
	//Ч�����
	ASSERT(cbCardCount<=CountArray(m_CardItemArray));
	if (cbCardCount>CountArray(m_CardItemArray)) return false;

	//�����˿�
	m_cbCardCount=cbCardCount;
	CopyMemory(m_CardItemArray,CardItemArray,cbCardCount*sizeof(tagCardItem));

	return true;
}

//��ȡ�˿�
tagCardItem * CCardControl::GetCardFromIndex(BYTE cbIndex)
{
	return (cbIndex<m_cbCardCount)?&m_CardItemArray[cbIndex]:NULL;
}

//��ȡ�˿�
tagCardItem * CCardControl::GetCardFromPoint(CPoint & MousePoint)
{
	WORD wIndex=SwitchCardPoint(MousePoint);
	return (wIndex!=INVALID_ITEM)?&m_CardItemArray[wIndex]:NULL;
}

//��ȡ�˿�
WORD CCardControl::GetCardData(BYTE cbCardData[], BYTE cbBufferCount)
{
	if(cbBufferCount==0) return 0;
	//Ч�����
	ASSERT(cbBufferCount>=m_cbCardCount);
	if (cbBufferCount<m_cbCardCount) return 0;

	//�����˿�
	for (WORD i=0;i<m_cbCardCount;i++) cbCardData[i]=m_CardItemArray[i].cbCardData;

	return m_cbCardCount;
}

//��ȡ�˿�
WORD CCardControl::GetShootCard(BYTE cbCardData[], BYTE cbBufferCount)
{
	//��������
	WORD wShootCount=0;

	//�����˿�
	for (WORD i=0;i<m_cbCardCount;i++) 
	{
		//Ч�����
		ASSERT(cbBufferCount>wShootCount);
		if (cbBufferCount<=wShootCount) break;

		//�����˿�
		if (m_CardItemArray[i].bShoot==true) cbCardData[wShootCount++]=m_CardItemArray[i].cbCardData;
	}

	return wShootCount;
}

//��ȡ�˿�
WORD CCardControl::GetCardData(tagCardItem CardItemArray[], BYTE cbBufferCount)
{
	//Ч�����
	ASSERT(cbBufferCount>=m_cbCardCount);
	if (cbBufferCount<m_cbCardCount) return 0;

	//�����˿�
	CopyMemory(CardItemArray,m_CardItemArray,sizeof(tagCardItem)*m_cbCardCount);

	return m_cbCardCount;
}

//���þ���
VOID CCardControl::SetCardDistance(UINT nXDistance, UINT nYDistance, UINT nShootDistance)
{
	//���ñ���
	m_nXDistance=nXDistance;
	m_nYDistance=nYDistance;
	m_nShootDistance=nShootDistance;

	return;
}

//��ȡ����
VOID CCardControl::GetCenterPoint(CPoint & CenterPoint)
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
VOID CCardControl::SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	return;
}

//��׼λ��
VOID CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	return;
}

//�滭�˿�
VOID CCardControl::DrawCardControl(CDC * pDC,bool ISbEffect)
{
	//����λͼ
	CImageHandle HandleCard(&m_ImageCard);

	//�˿���ͼ
	CImageHandle ImageCardMaskHandle(&m_ImageCardMask);
	CImageHandle ImageWinHandle(&m_ImageWin);

	//��ͼ��С
	int nCardMaskWidth = m_ImageCardMask.GetWidth();
	int nCardMaskHeight = m_ImageCardMask.GetHeight();

	//��ȡλ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//��������
	INT nXDrawPos=0,nYDrawPos=0;
	INT nXImagePos=0,nYImagePos=0;

	//�滭�˿�
	for (WORD i=0;i<m_cbCardCount;i++)
	{
		//��ȡ�˿�
		bool bShoot=m_CardItemArray[i].bShoot;
		bool bEffect = m_CardItemArray[i].bEffect;
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
			nXImagePos=m_CardSize.cx*2;
			nYImagePos=m_CardSize.cy*4;
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
		if(bEffect&&ISbEffect)
			m_ImageCardMask.AlphaDrawImage(pDC,OriginPoint.x+nXDrawPos-2,OriginPoint.y+nYDrawPos-2,nCardMaskWidth,nCardMaskHeight,0,0,RGB(255,0,255));

		//�˿˱�־
		if(m_CardItemArray[i].bMy)
			m_ImageWin.AlphaDrawImage(pDC,OriginPoint.x+nXDrawPos,OriginPoint.y+m_CardSize.cy+((m_YCollocateMode==enYTop)?(m_CardSize.cy/2-m_ImageWin.GetHeight()):0)-m_ImageWin.GetHeight(),
				m_ImageWin.GetWidth(),m_ImageWin.GetHeight(),0,0,RGB(255,0,255));
	}

	return;
}

//�����Ϣ
bool CCardControl::OnEventSetCursor(CPoint Point)
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
VOID CCardControl::GetControlSize(CSize & ControlSize)
{
	//��ȡ��С
	if (m_bHorizontal==true)
	{
		ControlSize.cy=m_CardSize.cy+m_nShootDistance;
		ControlSize.cx=(m_cbCardCount>0)?(m_CardSize.cx+(m_cbCardCount-1)*m_nXDistance):0;
	}
	else
	{
		ControlSize.cx=m_CardSize.cx;
		ControlSize.cy=(m_cbCardCount>0)?(m_CardSize.cy+(m_cbCardCount-1)*m_nYDistance):0;
	}

	return;
}

//��ȡԭ��
VOID CCardControl::GetOriginPoint(CPoint & OriginPoint)
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
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
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
		if (wCardIndex>=m_cbCardCount) wCardIndex=(m_cbCardCount-1);

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
//////////////////////////////////////////////////////////////////////////
