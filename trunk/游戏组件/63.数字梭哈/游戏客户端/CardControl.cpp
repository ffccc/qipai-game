#include "StdAfx.h"
#include "GameClient.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define INVALID_ITEM				0xFFFF								//��Ч����

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bPositively=false;
	m_bDisplayHead=false;
	m_nXOrgDistance=DEF_X_DISTANCE;
	m_nXDistance = DEF_X_DISTANCE;

	//
	m_bMoving = false;
	m_nYStep = 0;
	m_nYMove = 0;
	m_cbShowIndex = 0xff;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_CardItem,sizeof(m_CardItem));

	//λ�ñ���
	m_YCollocateMode=enYTop;
	m_XCollocateMode=enXLeft;
	m_BenchmarkPos.SetPoint(0,0);

	//������Դ
	m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());

	//��ȡ��С
	CImageHandle HandleImage(&m_ImageCard);
	m_CardSize.SetSize(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//��ȡ�˿�
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wBufferCount)
{
	//Ч�����
	WORD wCardCount = __min(wBufferCount,m_wCardCount);

	//�����˿�
	for( WORD i = 0; i < wCardCount; i++ )
		cbCardData[i] = m_CardItem[i].cbCardData;

	return m_wCardCount;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=CountArray(m_CardItem));
	if (wCardCount>CountArray(m_CardItem)) return false;

	//���ñ���
	m_wCardCount=wCardCount;
	for( WORD i = 0; i < wCardCount; i++ )
	{
		m_CardItem[i].cbCardData = cbCardData[i];
		m_CardItem[i].bDisplay = (i==0)?m_bDisplayHead:true;
	}

	m_bMoving = false;
	m_nXDistance = m_nXOrgDistance;
	m_cbShowIndex = 0xff;

	return true;
}

//��׼λ��
VOID CCardControl::SetBasicStation(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	m_bMoving = false;
	m_nXDistance = m_nXOrgDistance;
	m_cbShowIndex = 0xff;

	return;
}

//�滭�˿�
VOID CCardControl::DrawCardControl(CDC * pDC)
{
	//����λͼ
	CImageHandle HandleCard(&m_ImageCard);

	//��ȡλ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//�滭�˿�
	INT nXImagePos=0,nYImagePos=0;
	BYTE cbCardData = 0;
	if( !m_bMoving || m_nXDistance > 0 )
	{
		for (WORD i=0;i<m_wCardCount;i++)
		{
			//��������
			cbCardData=m_CardItem[i].cbCardData;

			//ͼƬλ��
			if ( m_CardItem[i].bDisplay && cbCardData != 0 && cbCardData != 0xFF )
			{
				nXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
				nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
			}
			else
			{
				nXImagePos=m_CardSize.cx*2;
				nYImagePos=m_CardSize.cy*4;
			}

			//�滭�˿�
			m_ImageCard.AlphaDrawImage(pDC,OriginPoint.x+m_nXDistance*i,OriginPoint.y,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));
		}
	}
	else
	{
		ASSERT( m_bMoving && m_cbShowIndex < m_wCardCount );
		cbCardData = m_CardItem[m_cbShowIndex].cbCardData;
		ASSERT( cbCardData != 0 );
		nXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
		nYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		INT nYMove = __min(m_nYMove,33);
		m_ImageCard.AlphaDrawImage( pDC,OriginPoint.x,OriginPoint.y-nYMove,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255) );
		nXImagePos=m_CardSize.cx*2;
		nYImagePos=m_CardSize.cy*4;
		m_ImageCard.AlphaDrawImage( pDC,OriginPoint.x,OriginPoint.y,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255) );
	}

	return;
}

//�����Ϣ
bool CCardControl::OnEventSetCursor(CPoint Point)
{
	//��괦��
	if (m_bPositively==true&&!m_bMoving)
	{
		//��ȡ����
		WORD wHoverItem=SwitchCardPoint(Point);

		//�����ж�
		if (wHoverItem!=INVALID_ITEM&&!m_CardItem[wHoverItem].bDisplay)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
	}

	return false;
}

//�����Ϣ
bool CCardControl::OnEventLeftMouseDown(CPoint Point)
{
	//��괦��
	if ((m_bPositively==true)&&!m_bMoving)
	{
		//��ȡ����
		WORD wHoverItem=SwitchCardPoint(Point);

		if( wHoverItem != INVALID_ITEM && !m_CardItem[wHoverItem].bDisplay )
		{
			m_cbShowIndex = (BYTE)wHoverItem;
			return true;
		}
	}

	return false;
}

//
bool CCardControl::OnEventLeftMouseUp(CPoint Point)
{
	//��괦��
	if ((m_bPositively==true)&&!m_bMoving)
	{
		//��ȡ����
		WORD wHoverItem=SwitchCardPoint(Point);

		if( wHoverItem != INVALID_ITEM )
		{
			m_cbShowIndex = (BYTE)wHoverItem;
			return true;
		}
	}
	return false;
}

//��ȡ��С
VOID CCardControl::GetControlSize(CSize & ControlSize)
{
	//��ȡ��С
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(m_wCardCount>0)?(m_CardSize.cx+(m_wCardCount-1)*m_nXDistance):0;

	return;
}

//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	if( m_bMoving ) return INVALID_ITEM;
	//��ȡλ��
	CSize ControlSize;
	CPoint OriginPoint;
	GetControlSize(ControlSize);
	GetOriginPoint(OriginPoint);

	//��׼λ��
	INT nXPos=MousePoint.x-OriginPoint.x;
	INT nYPos=MousePoint.y-OriginPoint.y;

	//Խ���ж�
	if ((nXPos<=0)||(nXPos>=ControlSize.cx)) return INVALID_ITEM;
	if ((nYPos<=0)||(nYPos>=ControlSize.cy)) return INVALID_ITEM;

	//��������
	WORD wCardIndex=nXPos/m_nXDistance;
	if (wCardIndex>=m_wCardCount) wCardIndex=(m_wCardCount-1);

	return wCardIndex;
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

//��ȡ��βλ��
CPoint CCardControl::GetTailPos()
{
	CPoint ptTail;

	//�����С
	INT nSpaceWidth = m_wCardCount*m_nXDistance;
	CSize ControlSize;
	GetControlSize(ControlSize);

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ ptTail.x=m_BenchmarkPos.x+nSpaceWidth; break; }
	case enXCenter: { ptTail.x=m_BenchmarkPos.x-ControlSize.cx/2+nSpaceWidth; break; }
	case enXRight:	{ ptTail.x=m_BenchmarkPos.x-ControlSize.cx+nSpaceWidth; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ ptTail.y=m_BenchmarkPos.y; break; }
	case enYCenter: { ptTail.y=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { ptTail.y=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	return ptTail;
}

//
bool CCardControl::BeginMoveCard()
{
	if( m_bMoving ) return false;

	ASSERT( m_cbShowIndex < m_wCardCount );
	if( m_cbShowIndex >= m_wCardCount ) return false;

	m_bMoving = true;
	m_nYStep = MOVE_STEP_Y;
	m_nYMove = 0;

	return true;
}

//
bool CCardControl::PlayMoveCard()
{
	if( !m_bMoving ) return false;

	if( m_nXDistance > 0 )
		m_nXDistance = __max(m_nXDistance-MOVE_STEP_X,0);
	else
	{
		if( m_nYMove >= MAX_MOVE_Y ) m_nYStep = -m_nYStep;
		m_nYMove += m_nYStep;

		if( m_nYMove <= 0 )
		{
			m_bMoving = false;
			m_nXDistance = m_nXOrgDistance;
			m_cbShowIndex = 0xff;
		}
	}

	return true;
}

//
VOID CCardControl::SetXDistance( UINT nXDistance )
{
	m_nXOrgDistance = nXDistance;
}

//
VOID CCardControl::AllowPositively( bool bPositively )
{
	m_bPositively = bPositively;
	m_bMoving = false;
	m_nXDistance = m_nXOrgDistance;
	m_cbShowIndex = 0xff;
}

//
VOID CCardControl::ShowFirstCard( bool bDisplayHead )
{
	m_bDisplayHead = bDisplayHead;
	if( m_wCardCount > 0 )
		m_CardItem[0].bDisplay = bDisplayHead;
	m_bMoving = false;
	m_nXDistance = m_nXOrgDistance;
	m_cbShowIndex = 0xff;
}

//
void CCardControl::SetDispalyCard( WORD wCardIndex,bool bDisplay )
{
	ASSERT( wCardIndex < m_wCardCount );
	if( wCardIndex >= m_wCardCount ) return;

	m_CardItem[wCardIndex].bDisplay = bDisplay;
}


//////////////////////////////////////////////////////////////////////////
