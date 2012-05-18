#include "StdAfx.h"
#include "GameClient.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

#define INVALID_ITEM				0xFFFF								//无效子项

//////////////////////////////////////////////////////////////////////////

//构造函数
CCardControl::CCardControl()
{
	//状态变量
	m_bPositively=false;
	m_bDisplayHead=false;
	m_nXOrgDistance=DEF_X_DISTANCE;
	m_nXDistance = DEF_X_DISTANCE;

	//
	m_bMoving = false;
	m_nYStep = 0;
	m_nYMove = 0;
	m_cbShowIndex = 0xff;

	//扑克数据
	m_wCardCount=0;
	ZeroMemory(m_CardItem,sizeof(m_CardItem));

	//位置变量
	m_YCollocateMode=enYTop;
	m_XCollocateMode=enXLeft;
	m_BenchmarkPos.SetPoint(0,0);

	//加载资源
	m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());

	//获取大小
	CImageHandle HandleImage(&m_ImageCard);
	m_CardSize.SetSize(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);

	return;
}

//析构函数
CCardControl::~CCardControl()
{
}

//获取扑克
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wBufferCount)
{
	//效验参数
	WORD wCardCount = __min(wBufferCount,m_wCardCount);

	//拷贝扑克
	for( WORD i = 0; i < wCardCount; i++ )
		cbCardData[i] = m_CardItem[i].cbCardData;

	return m_wCardCount;
}

//设置扑克
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//效验参数
	ASSERT(wCardCount<=CountArray(m_CardItem));
	if (wCardCount>CountArray(m_CardItem)) return false;

	//设置变量
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

//基准位置
VOID CCardControl::SetBasicStation(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	m_bMoving = false;
	m_nXDistance = m_nXOrgDistance;
	m_cbShowIndex = 0xff;

	return;
}

//绘画扑克
VOID CCardControl::DrawCardControl(CDC * pDC)
{
	//加载位图
	CImageHandle HandleCard(&m_ImageCard);

	//获取位置
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//绘画扑克
	INT nXImagePos=0,nYImagePos=0;
	BYTE cbCardData = 0;
	if( !m_bMoving || m_nXDistance > 0 )
	{
		for (WORD i=0;i<m_wCardCount;i++)
		{
			//变量定义
			cbCardData=m_CardItem[i].cbCardData;

			//图片位置
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

			//绘画扑克
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

//光标消息
bool CCardControl::OnEventSetCursor(CPoint Point)
{
	//光标处理
	if (m_bPositively==true&&!m_bMoving)
	{
		//获取索引
		WORD wHoverItem=SwitchCardPoint(Point);

		//更新判断
		if (wHoverItem!=INVALID_ITEM&&!m_CardItem[wHoverItem].bDisplay)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
	}

	return false;
}

//鼠标消息
bool CCardControl::OnEventLeftMouseDown(CPoint Point)
{
	//光标处理
	if ((m_bPositively==true)&&!m_bMoving)
	{
		//获取索引
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
	//光标处理
	if ((m_bPositively==true)&&!m_bMoving)
	{
		//获取索引
		WORD wHoverItem=SwitchCardPoint(Point);

		if( wHoverItem != INVALID_ITEM )
		{
			m_cbShowIndex = (BYTE)wHoverItem;
			return true;
		}
	}
	return false;
}

//获取大小
VOID CCardControl::GetControlSize(CSize & ControlSize)
{
	//获取大小
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(m_wCardCount>0)?(m_CardSize.cx+(m_wCardCount-1)*m_nXDistance):0;

	return;
}

//索引切换
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	if( m_bMoving ) return INVALID_ITEM;
	//获取位置
	CSize ControlSize;
	CPoint OriginPoint;
	GetControlSize(ControlSize);
	GetOriginPoint(OriginPoint);

	//基准位置
	INT nXPos=MousePoint.x-OriginPoint.x;
	INT nYPos=MousePoint.y-OriginPoint.y;

	//越界判断
	if ((nXPos<=0)||(nXPos>=ControlSize.cx)) return INVALID_ITEM;
	if ((nYPos<=0)||(nYPos>=ControlSize.cy)) return INVALID_ITEM;

	//计算索引
	WORD wCardIndex=nXPos/m_nXDistance;
	if (wCardIndex>=m_wCardCount) wCardIndex=(m_wCardCount-1);

	return wCardIndex;
}

//获取原点
VOID CCardControl::GetOriginPoint(CPoint & OriginPoint)
{
	//获取位置
	CSize ControlSize;
	GetControlSize(ControlSize);

	//横向位置
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ OriginPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { OriginPoint.x=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ OriginPoint.x=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//竖向位置
	switch (m_YCollocateMode)
	{
	case enYTop:	{ OriginPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { OriginPoint.y=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { OriginPoint.y=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	return;
}

//获取牌尾位置
CPoint CCardControl::GetTailPos()
{
	CPoint ptTail;

	//计算大小
	INT nSpaceWidth = m_wCardCount*m_nXDistance;
	CSize ControlSize;
	GetControlSize(ControlSize);

	//横向位置
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ ptTail.x=m_BenchmarkPos.x+nSpaceWidth; break; }
	case enXCenter: { ptTail.x=m_BenchmarkPos.x-ControlSize.cx/2+nSpaceWidth; break; }
	case enXRight:	{ ptTail.x=m_BenchmarkPos.x-ControlSize.cx+nSpaceWidth; break; }
	}

	//竖向位置
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
