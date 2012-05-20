#include "StdAfx.h"
#include "CardControl.h"
#include ".\cardcontrol.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CCardListImage::CCardListImage()
{
	//位置变量
	m_nItemWidth=0;
	m_nItemHeight=0;

	return;
}

//析构函数
CCardListImage::~CCardListImage()
{
}

//加载资源
bool CCardListImage::LoadResource(UINT uResourceID, int nItemWidth, int nItemHeight)
{
	//加载资源
	m_CardListImage.LoadFromResource(AfxGetInstanceHandle(),uResourceID);

	//设置变量
	m_nItemWidth=nItemWidth;
	m_nItemHeight=nItemHeight;

	return true;
}

//释放资源
bool CCardListImage::DestroyResource()
{
	//设置变量
	m_nItemWidth=0;
	m_nItemHeight=0;

	//释放资源
	m_CardListImage.Destroy();

	return true;
}

//获取位置
int CCardListImage::GetImageIndex(BYTE cbCardData)
{
	//背景判断
	if (cbCardData==0) return 0;

	//计算位置
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor*10+cbValue);
}

//绘画扑克
bool CCardListImage::DrawCardItem(CDC * pDestDC, BYTE cbCardData, int xDest, int yDest)
{
	//效验状态
	ASSERT(m_CardListImage.IsNull()==false);
	ASSERT((m_nItemWidth!=0)&&(m_nItemHeight!=0));

	//绘画子项
	int nImageXPos=GetImageIndex(cbCardData)*m_nItemWidth;
	m_CardListImage.AlphaDrawImage(pDestDC,xDest,yDest,m_nItemWidth,m_nItemHeight,nImageXPos,0,RGB(255,0,255));

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CCardResource::CCardResource()
{
}

//析构函数
CCardResource::~CCardResource()
{
}

//加载资源
bool CCardResource::LoadResource()
{
	//用户扑克
	m_ImageUserTop.LoadResource(IDB_CARD_TOP,CARD_WIDTH,CARD_HEIGHT);
	m_ImageUserLeft.LoadResource(IDB_CARD_LEFT,CARD_HEIGHT,CARD_WIDTH);
	m_ImageUserRight.LoadResource(IDB_CARD_RIGHT,CARD_HEIGHT,CARD_WIDTH);
	m_ImageUserBottom.LoadResource(IDB_CARD_BOTTOM,CARD_WIDTH,CARD_HEIGHT);

	//桌面扑克
	m_ImageTableTop.LoadResource(IDB_CARD_TABLE_TOP,CARD_TABLE_WIDTH,CARD_TABLE_HEIGHT);
	m_ImageTableLeft.LoadResource(IDB_CARD_TABLE_LEFT,CARD_TABLE_HEIGHT,CARD_TABLE_WIDTH);
	m_ImageTableRight.LoadResource(IDB_CARD_TABLE_RIGHT,CARD_TABLE_HEIGHT,CARD_TABLE_WIDTH);
	m_ImageTableBottom.LoadResource(IDB_CARD_TABLE_BOTTOM,CARD_TABLE_WIDTH,CARD_TABLE_HEIGHT);

	return true;
}

//消耗资源
bool CCardResource::DestroyResource()
{
	//用户扑克
	m_ImageUserTop.DestroyResource();
	m_ImageUserLeft.DestroyResource();
	m_ImageUserRight.DestroyResource();
	m_ImageUserBottom.DestroyResource();

	//丢弃扑克
	m_ImageTableTop.DestroyResource();
	m_ImageTableLeft.DestroyResource();
	m_ImageTableRight.DestroyResource();
	m_ImageTableBottom.DestroyResource();

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDiscardCard::CDiscardCard()
{
	//扑克数据
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//控制变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//析构函数
CDiscardCard::~CDiscardCard()
{
}

//绘画扑克
void CDiscardCard::DrawCardControl(CDC * pDC)
{
	//绘画控制
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x-CARD_TABLE_HEIGHT;
				int nYPos=m_ControlPoint.y+i*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_South:	//南向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nYPos=m_ControlPoint.y-CARD_TABLE_HEIGHT;
				int nXPos=m_ControlPoint.x-(i+1)*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_West:	//西向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y-(i+1)*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_North:	//北向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nYPos=m_ControlPoint.y;
				int nXPos=m_ControlPoint.x+i*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	}

	return;
}

//增加扑克
bool CDiscardCard::AddCardItem(BYTE cbCardData)
{
	//清理扑克
	if (m_wCardCount>=CountArray(m_cbCardData))
	{
		m_wCardCount--;
		MoveMemory(m_cbCardData,m_cbCardData+1,CountArray(m_cbCardData)-1);
	}

	//设置扑克
	m_cbCardData[m_wCardCount++]=cbCardData;

	return true;
}

//设置扑克
bool CDiscardCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//设置扑克
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CUserCard::CUserCard()
{
	//扑克数据
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//控制变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//析构函数
CUserCard::~CUserCard()
{
}

//绘画扑克
void CUserCard::DrawCardControl(CDC * pDC)
{
	//绘画控制
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x-CARD_TABLE_HEIGHT;
				int nYPos=m_ControlPoint.y-(i+1)*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			}

			break;
		}
	case Direction_South:	//南向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nYPos=m_ControlPoint.y-CARD_TABLE_HEIGHT;
				int nXPos=m_ControlPoint.x+i*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_West:	//西向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y+i*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_North:	//北向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nYPos=m_ControlPoint.y;
				int nXPos=m_ControlPoint.x-(i+1)*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			}

			break;
		}
	}

	return;
}

//设置扑克
bool CUserCard::SetCardData(WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//设置扑克
	m_wCardCount=wCardCount;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	return true;
}

//设置扑克
bool CUserCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//设置扑克
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CWeaveCard::CWeaveCard()
{
	//状态变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_South;

	//扑克数据
	m_wCardCount=0;
	ZeroMemory(&m_cbCardData,sizeof(m_cbCardData));

	m_bDisplayStyle =false;

	return;
}

//析构函数
CWeaveCard::~CWeaveCard()
{
}

//绘画扑克
void CWeaveCard::DrawCardControl(CDC * pDC)
{
	//显示判断
	if (m_wCardCount==0) return;

	//变量定义
	int nXScreenPos=0,nYScreenPos=0;

	//绘画扑克
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			for (WORD i=0;i<CountArray(m_cbCardData);i++)
			{
				if ((m_wCardCount==4)||(i!=0))
				{
					nYScreenPos=m_ControlPoint.y;
					nXScreenPos=m_ControlPoint.x-CARD_SPACE*(4-i)-CARD_TABLE_HEIGHT;
					g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[3-i],nXScreenPos,nYScreenPos);
				}
			}

			break;
		}
	case Direction_South:	//南向
		{
			for (WORD i=0;i<CountArray(m_cbCardData);i++)
			{
				if ((m_wCardCount==4)||(i!=0))
				{
					nXScreenPos=m_ControlPoint.x;
					nYScreenPos=m_ControlPoint.y+CARD_SPACE*i;
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[3-i],nXScreenPos,nYScreenPos);
				}
			}

			break;
		}
	case Direction_West:	//西向
		{
			for (WORD i=0;i<CountArray(m_cbCardData);i++)
			{
				if ((m_wCardCount==4)||(i!=0))
				{
					nYScreenPos=m_ControlPoint.y;
					nXScreenPos=m_ControlPoint.x+CARD_SPACE*(4-i);
					g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[3-i],nXScreenPos,nYScreenPos);
				}
			}

			break;
		}
	case Direction_North:	//北向
		{
			for (WORD i=0;i<CountArray(m_cbCardData);i++)
			{
				if ((m_wCardCount==4)||(i!=0))
				{
					nXScreenPos=m_ControlPoint.x;
					nYScreenPos=m_ControlPoint.y+CARD_SPACE*(3-i)+CARD_TABLE_HEIGHT;
					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[3-i],nXScreenPos,nYScreenPos);
				}
			}

			break;
		}
	}

	return;
}

//设置扑克
bool CWeaveCard::SetCardData(const tagWeaveItem & WeaveItem)
{
	//设置扑克
	switch (WeaveItem.cbWeaveKind)
	{
	case ACK_NULL:	//空
		{
			//设置扑克
			m_wCardCount=0;
			ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

			return true;
		}
	case ACK_TI:	//提
		{
			//设置扑克
			m_wCardCount=4;
			m_cbCardData[0]=WeaveItem.cbCardList[0];
			ZeroMemory(&m_cbCardData[1],sizeof(BYTE)*3);

			return true;
		}
	case ACK_PAO:	//跑
		{
			//设置扑克
			m_wCardCount=4;
			CopyMemory(m_cbCardData,WeaveItem.cbCardList,sizeof(BYTE)*4);

			return true;
		}
	case ACK_WEI:	//偎
		{
			//设置扑克
			m_wCardCount=3;
			if(m_bDisplayStyle)
			{
				m_cbCardData[0]=WeaveItem.cbCardList[0];
				ZeroMemory(&m_cbCardData[1],sizeof(BYTE)*2);
			}
			else
			{
				ZeroMemory(&m_cbCardData[0],sizeof(BYTE)*3);
			}

			return true;
		}
	case ACK_PENG:	//碰
		{
			//设置扑克
			m_wCardCount=3;
			CopyMemory(m_cbCardData,WeaveItem.cbCardList,sizeof(BYTE)*3);

			return true;
		}
	case ACK_CHI:	//吃
		{
			//设置扑克
			m_wCardCount=3;
			CopyMemory(m_cbCardData,WeaveItem.cbCardList,sizeof(BYTE)*3);

			
			if(m_cbCardData[0]!=WeaveItem.cbCenterCard)
			{
				BYTE m_cbTmpCardData=m_cbCardData[0];
				for(BYTE i=1; i<m_wCardCount;i++)
				{
					if(m_cbCardData[i]!=WeaveItem.cbCenterCard)continue;
					m_cbCardData[0]=m_cbCardData[i];
					m_cbCardData[i]=m_cbTmpCardData;
					break;
				
				}
			}

			return true;
		}
	}

	return false;
}

// 显示方式

void CWeaveCard::SetDisplayStyle(bool bDisplayStyle)
{
	if(m_bDisplayStyle!=bDisplayStyle)m_bDisplayStyle=bDisplayStyle;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CCardControl::CCardControl()
{
	//状态变量
	m_bPositively=false;
	m_bDisplayItem=false;

	//位置变量
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//扑克数据
	m_wCardCount=0;
	m_wHoverItem=INVALID_ITEM;
	ZeroMemory(&m_CardItemArray,sizeof(m_CardItemArray));

	//加载设置
	m_ControlPoint.SetPoint(0,0);
	m_ControlSize.cy=CARD_HEIGHT+POS_SHOOT;
	m_ControlSize.cx=CountArray(m_CardItemArray)*CARD_WIDTH;

	return;
}

//析构函数
CCardControl::~CCardControl()
{
}

//基准位置
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//横向位置
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//竖向位置
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//基准位置
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//横向位置
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//竖向位置
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//获取扑克
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wMaxCount)
{
	//效验数目
	ASSERT(wMaxCount>=m_wCardCount);
	if (wMaxCount<m_wCardCount) return 0;

	//拷贝扑克
	for (WORD i=0;i<m_wCardCount;i++) cbCardData[i]=m_CardItemArray[i].cbCardData;

	return m_wCardCount;
}

//获取扑克
BYTE CCardControl::GetHoverCard()
{
	//获取扑克
	if (m_wHoverItem==INVALID_ITEM) return 0;
	return m_CardItemArray[m_wHoverItem].cbCardData;
}

//设置扑克
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//设置扑克
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].bEnable=true;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//交换扑克
bool CCardControl::SwitchCardItem(WORD wSourceItem, WORD wTargerItem)
{
	//效验参数
	if (wSourceItem==wTargerItem) return false;
	if ((wSourceItem>=m_wCardCount)||(wTargerItem>=m_wCardCount)) return false;

	//保存扑克
	tagCardItem CardItem=m_CardItemArray[wSourceItem];

	//移动扑克
	if (wSourceItem>wTargerItem)
	{
		WORD wMoveCount=wSourceItem-wTargerItem;
		MoveMemory(&m_CardItemArray[wTargerItem+1],&m_CardItemArray[wTargerItem],sizeof(tagCardItem)*wMoveCount);
	}
	else
	{
		WORD wMoveCount=wTargerItem-wSourceItem;
		MoveMemory(&m_CardItemArray[wSourceItem],&m_CardItemArray[wSourceItem+1],sizeof(tagCardItem)*wMoveCount);
	}

	//插入目标
	m_wHoverItem=wTargerItem;
	m_CardItemArray[wTargerItem]=CardItem;

	return true;
}

//设置扑克
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return 0;

	//设置扑克
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=CardItemArray[i].bShoot;
		m_CardItemArray[i].bEnable=CardItemArray[i].bEnable;
		m_CardItemArray[i].cbCardData=CardItemArray[i].cbCardData;
	}

	return true;
}

//绘画扑克
void CCardControl::DrawCardControl(CDC * pDC)
{
	//偏移数值
	int nExcursion=(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH/2;

	//绘画扑克
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//计算位置
		int nXScreenPos=m_ControlPoint.x+CARD_WIDTH*i+nExcursion;
		int nYScreenPos=m_ControlPoint.y+(((m_CardItemArray[i].bShoot==false)&&(m_wHoverItem!=i))?POS_SHOOT:0);

		//绘画扑克
		BYTE cbCardData=(m_bDisplayItem==true)?m_CardItemArray[i].cbCardData:0;
		g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
	}

	return;
}

//索引切换
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//偏移数值
	int nExcursion=(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH/2;

	//基准位置
	int nYPos=MousePoint.y-m_ControlPoint.y;
	int nXPos=MousePoint.x-m_ControlPoint.x-nExcursion;

	//范围判断
	if ((nXPos<0)||(nXPos>m_ControlSize.cx)) return INVALID_ITEM;
	if ((nYPos<POS_SHOOT)||(nYPos>m_ControlSize.cy)) return INVALID_ITEM;

	//牌列子项
	WORD wIndex=nXPos/CARD_WIDTH;
	return (wIndex<m_wCardCount)?wIndex:INVALID_ITEM;
}

//光标消息
bool CCardControl::OnEventSetCursor(CPoint Point, bool & bRePaint)
{
	//获取索引
	WORD wHoverItem=SwitchCardPoint(Point);
	bool bShoot=((wHoverItem!=INVALID_ITEM)&&(m_CardItemArray[wHoverItem].bEnable==true));

	//响应判断
	if ((m_bPositively==false)&&(m_wHoverItem!=INVALID_ITEM))
	{
		bRePaint=true;
		m_wHoverItem=INVALID_ITEM;
	}

	//更新判断
	if ((wHoverItem!=m_wHoverItem)&&(m_bPositively==true))
	{
		bRePaint=true;
		m_wHoverItem=wHoverItem;
	}

	//设置光标
	if ((wHoverItem!=INVALID_ITEM)&&(m_bPositively==true))
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
	}

	return (wHoverItem!=INVALID_ITEM);
}

//////////////////////////////////////////////////////////////////////////

//变量声明
CCardResource						g_CardResource;						//扑克资源


