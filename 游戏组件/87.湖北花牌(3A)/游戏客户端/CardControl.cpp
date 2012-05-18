#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"
//////////////////////////////////////////////////////////////////////////
//宏定义

//公共定义
#define POS_SHOOT					5									//弹起象素
//#define POS_SPACE					8									//分隔间隔
#define POS_SPACE					5									//分隔间隔
#define ITEM_COUNT					23									//子项数目
const BYTE INVALID_ITEM =  0xFF;								//无效索引

//扑克大小
//#define CARD_WIDTH					35									//扑克宽度
//#define CARD_HEIGHT					50									//扑克高度
//#define CARD_WIDTH					25									//扑克宽度
//#define CARD_HEIGHT					90									//扑克高度

#define CARD_WIDTH					35									//扑克宽度
#define CARD_HEIGHT					90									//扑克高度

//////////////////////////////////////////////////////////////////////////
//构造函数
CCardListImage::CCardListImage()
{
	//位置变量
	m_nItemWidth=0;
	m_nItemHeight=0;
	m_nViewWidth=0;
	m_nViewHeight=0;

	return;
}

//析构函数
CCardListImage::~CCardListImage()
{
}

//加载资源
bool CCardListImage::LoadResource(UINT uResourceID, int nViewWidth, int nViewHeight)
{
	//加载资源
	m_CardListImage.LoadFromResource(AfxGetInstanceHandle(),uResourceID);

	//设置变量
	m_nViewWidth=nViewWidth;
	m_nViewHeight=nViewHeight;
	m_nItemHeight=m_CardListImage.GetHeight();
	m_nItemWidth=m_CardListImage.GetWidth()/ITEM_COUNT;

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
	//BYTE cbValue=cbCardData&MASK_VALUE;
	//BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	//return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);

	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>0x01)?(10+cbValue):cbValue;
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
	//变量定义
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//用户扑克
	m_ImageUserTop.LoadFromResource(hInstance,IDB_CARD_USER_TOP);
	m_ImageUserLeft.LoadFromResource(hInstance,IDB_CARD_USER_LEFT);
	m_ImageUserRight.LoadFromResource(hInstance,IDB_CARD_USER_RIGHT);
	m_ImageUserBottom.LoadResource(IDB_CARD_USER_BOTTOM,CARD_WIDTH,CARD_HEIGHT);
	
	//----------------------wj---------------------------------
	m_ImageUserBottom1.LoadResource(IDB_CARD_USER_BOTTOM1,CARD_WIDTH,CARD_HEIGHT);
	m_ImageTableTop1.LoadResource(IDB_CARD_TABLE_TOP1,15,54);
	m_ImageTableLeft1.LoadResource(IDB_CARD_TABLE_LEFT1,54,15);
	m_ImageTableRight1.LoadResource(IDB_CARD_TABLE_RIGHT1,54,15);
	m_ImageTableBottom1.LoadResource(IDB_CARD_TABLE_BOTTOM1,15,54);

	//桌子扑克
	m_ImageTableTop.LoadResource(IDB_CARD_TABLE_TOP,15,54);
	m_ImageTableLeft.LoadResource(IDB_CARD_TABLE_LEFT,54,15);
	m_ImageTableRight.LoadResource(IDB_CARD_TABLE_RIGHT,54,15);
	m_ImageTableBottom.LoadResource(IDB_CARD_TABLE_BOTTOM,15,54);
	
	return true;
}

//消耗资源
bool CCardResource::DestroyResource()
{
	//用户扑克
	m_ImageUserTop.Destroy();
	m_ImageUserLeft.Destroy();
	m_ImageUserRight.Destroy();
	m_ImageUserBottom.DestroyResource();

	//桌子扑克
	m_ImageTableTop.DestroyResource();
	m_ImageTableLeft.DestroyResource();
	m_ImageTableRight.DestroyResource();
	m_ImageTableBottom.DestroyResource();

	//--------------------wj---------------------
	m_ImageUserBottom1.DestroyResource();
	m_ImageTableTop1.DestroyResource();
	m_ImageTableLeft1.DestroyResource();
	m_ImageTableRight1.DestroyResource();
	m_ImageTableBottom1.DestroyResource();


	//牌堆扑克
	m_ImageBackH.Destroy();
	m_ImageBackV.Destroy();
	m_ImageHeapSingleV.Destroy();
	m_ImageHeapSingleH.Destroy();
	m_ImageHeapDoubleV.Destroy();
	m_ImageHeapDoubleH.Destroy();

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CHeapCard::CHeapCard()
{
	//控制变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	//扑克变量
	m_wFullCount=0;
	m_wMinusHeadCount=0;
	m_wMinusLastCount=0;

	return;
}

//析构函数
CHeapCard::~CHeapCard()
{
}

void CHeapCard::DrawHeapCard(CDC * pDC,WORD wHeapCard,int nWidth,int nXFace)
{
	if(wHeapCard>0)
	{
		
		int nXPos=0,nYPos=0;
	
		for(int i=0;i<wHeapCard;i++)
		{
			nXPos=nWidth/2-nXFace/2-100;
			nYPos=m_ControlPoint.y+10;

			g_CardResource.m_ImageUserTop.AlphaDrawImage(pDC,nXPos+i*6,nYPos,RGB(255,0,255));			
		}
	}
	else
		return;
}
//绘画扑克--绘画扑克时把一个牌墙分为三部分：头 尾 中间
//因为一个牌墙有可能头号有单张牌或尾有单张牌只有中间的是总是两张牌一起的所以要区分
void CHeapCard::DrawCardControl(CDC * pDC)
{
	return;
}

//设置扑克
bool CHeapCard::SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount)
{
	//设置变量
	m_wFullCount=wFullCount;
	m_wMinusHeadCount=wMinusHeadCount;//头部空缺
	m_wMinusLastCount=wMinusLastCount;//尾部空缺

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CWeaveCard::CWeaveCard()
{
	//状态变量
	m_bAnGang=false;
	m_bDisplayItem=false;
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_South;

	//扑克数据
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

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
	int nItemWidth=0,nItemHeight=0,nItemWidthEx=0,nItemHeightEx=0;

	//绘画扑克
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			//变量定义
			nItemWidth=g_CardResource.m_ImageTableRight.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableRight.GetViewHeight();
			nItemWidthEx=g_CardResource.m_ImageTableTop.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableTop.GetViewHeight();

			//第三扑克
			nYScreenPos=m_ControlPoint.y+nItemHeightEx+nItemHeight*3;
			nXScreenPos=m_ControlPoint.x+nItemWidthEx*2-nItemWidth;
			//g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			if(((GetCardData(2)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableRight1.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			}

			//第二扑克
			nYScreenPos=m_ControlPoint.y+nItemHeightEx+nItemHeight*2;
			nXScreenPos=m_ControlPoint.x+nItemWidthEx*2-nItemWidth;
			//g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			if(((GetCardData(1)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableRight1.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			}

			//第一扑克
			nXScreenPos=m_ControlPoint.x+nItemWidthEx*2-nItemWidth;
			nYScreenPos=m_ControlPoint.y+nItemHeightEx+nItemHeight;
			//g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			if(((GetCardData(0)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableRight1.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			}
			//第四扑克
			if(m_wCardCount==4)
			{
				nYScreenPos=m_ControlPoint.y+nItemWidth+5;
				nXScreenPos=m_ControlPoint.x;
				//g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				if(((GetCardData(3)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableTop1.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
			}

			//第五扑克
			if (m_wCardCount==5)
			{
				nYScreenPos=m_ControlPoint.y+nItemWidth+5;
				nXScreenPos=m_ControlPoint.x;
				//g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				if(((GetCardData(3)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableTop1.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}

				nYScreenPos=m_ControlPoint.y+nItemWidth+5;
				nXScreenPos=m_ControlPoint.x+nItemWidthEx;
				//g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				if(((GetCardData(4)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableTop1.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				}

			}
			break;
		}
	case Direction_South:	//南向
		{
			//变量定义
			nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableBottom.GetViewHeight();
			nItemWidthEx=g_CardResource.m_ImageTableRight.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableRight.GetViewHeight();
		
			//第三扑克
			nXScreenPos=m_ControlPoint.x-nItemWidth*2;
			nYScreenPos=m_ControlPoint.y+95;
			//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			if(((GetCardData(2)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			}
			//第二扑克
			nXScreenPos=m_ControlPoint.x-nItemWidth*3;
			nYScreenPos=m_ControlPoint.y+95;
			//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			if(((GetCardData(1)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			}
			//第一扑克
			nXScreenPos=m_ControlPoint.x-nItemWidth*4;
			nYScreenPos=m_ControlPoint.y+95;
			//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			if(((GetCardData(0)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			}	
			
			//第四扑克
			if (m_wCardCount==4)
			{
				nYScreenPos=m_ControlPoint.y+nItemHeightEx+10+95;
				nXScreenPos=m_ControlPoint.x-nItemWidth*4;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
				if(((GetCardData(3)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableRight1.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
			}
				//第五扑克
			if (m_wCardCount==5)
			{
				nYScreenPos=m_ControlPoint.y+nItemHeightEx+10+95;
				nXScreenPos=m_ControlPoint.x-nItemWidth*4;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
				if(((GetCardData(3)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableRight1.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}

				nYScreenPos=m_ControlPoint.y+nItemHeightEx*2+10+95;
				nXScreenPos=m_ControlPoint.x-nItemWidth*4;
				//g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				if(((GetCardData(4)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableRight1.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				}
			}
			break;
		}
	case Direction_West:	//西向
		{
			//变量定义
			nItemWidth=g_CardResource.m_ImageTableLeft.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableLeft.GetViewHeight();
			nItemWidthEx=g_CardResource.m_ImageTableBottom.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableBottom.GetViewHeight();

			//第一扑克
			nXScreenPos=m_ControlPoint.x-nItemWidthEx*2;
			nYScreenPos=m_ControlPoint.y-nItemHeightEx-nItemHeight*3;
			//g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			if(((GetCardData(0)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableLeft1.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			}
			//第二扑克
			nXScreenPos=m_ControlPoint.x-nItemWidthEx*2;
			nYScreenPos=m_ControlPoint.y-nItemHeightEx-nItemHeight*2;
			//g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			if(((GetCardData(1)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableLeft1.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			}

			//第三扑克
			nYScreenPos=m_ControlPoint.y-nItemHeightEx-nItemHeight*1;
			nXScreenPos=m_ControlPoint.x-nItemWidthEx*2;
			//g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			if(((GetCardData(2)&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageTableLeft1.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			}
			//第四扑克			
			if (m_wCardCount==4)
			{
				nXScreenPos=m_ControlPoint.x-nItemWidthEx;
				nYScreenPos=m_ControlPoint.y-nItemHeightEx-nItemHeight*3;
				//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				if(((GetCardData(3)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
			}
			//第五扑克
			if (m_wCardCount==5)
			{
				nXScreenPos=m_ControlPoint.x-nItemWidthEx;
				nYScreenPos=m_ControlPoint.y-nItemHeightEx-nItemHeight*3;
				//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				if(((GetCardData(3)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
				}

				nXScreenPos=m_ControlPoint.x-nItemWidthEx*2;
				nYScreenPos=m_ControlPoint.y-nItemHeightEx-nItemHeight*3;
				//g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				if(((GetCardData(4)&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				}
				else
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,GetCardData(4),nXScreenPos,nYScreenPos);
				}
			}

			break;
		}
	}

	return;
}

//设置扑克
bool CWeaveCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//设置扑克
	if(cbCardData==NULL || wCardCount==0)
	{
		m_wCardCount=wCardCount;
		ZeroMemory(m_cbCardData,sizeof(m_cbCardData));
	}
	else
	{
		m_wCardCount=wCardCount;
		CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData));
	}
	return true;
}

//获取扑克
BYTE CWeaveCard::GetCardData(WORD wIndex)
{
	ASSERT(wIndex<CountArray(m_cbCardData));

	if(m_bAnGang==true)
		return((m_bDisplayItem==true)&&(wIndex==1))?m_cbCardData[wIndex]:0;
	else
		return (m_bDisplayItem==true)?m_cbCardData[wIndex]:0;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CUserCard::CUserCard()
{
	//扑克数据
	m_wCardCount=0;
	m_bCurrentCard=false;

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
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
		//当前扑克
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y+50;
				g_CardResource.m_ImageUserRight.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
			}

			//正常扑克
			if (m_wCardCount>0)
			{
				int nXPos=0,nYPos=0;
				for (WORD i=0;i<m_wCardCount;i++)
				{
					nXPos=m_ControlPoint.x;
					nYPos=m_ControlPoint.y+i*12+75;
					g_CardResource.m_ImageUserRight.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
				}
			}

			break;
		}
	case Direction_West:	//西向
		{
			//正常扑克
			if (m_wCardCount>0)
			{
				int nXPos=0,nYPos=0;
				for (WORD i=0;i<m_wCardCount;i++)
				{
					nXPos=m_ControlPoint.x;
					nYPos=m_ControlPoint.y-(m_wCardCount-i-1)*12-92;
					g_CardResource.m_ImageUserLeft.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
				}
			}

			//当前扑克
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y-67;
				g_CardResource.m_ImageUserLeft.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
			}
			break;
		}
	//case Direction_North:	//北向
	//	{
	//		//当前扑克
	//		if (m_bCurrentCard==true)
	//		{
	//			int nXPos=m_ControlPoint.x;
	//			int nYPos=m_ControlPoint.y+200;
	//			g_CardResource.m_ImageUserTop.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
	//		}

	//		//正常扑克
	//		if (m_wCardCount>0)
	//		{
	//			int nXPos=0,nYPos=0;
	//			for (WORD i=0;i<m_wCardCount;i++)
	//			{
	//				nYPos=m_ControlPoint.y+200;
	//				nXPos=m_ControlPoint.x+i*25+40;
	//				g_CardResource.m_ImageUserTop.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
	//			}
	//		}

	//		break;
	//	}
	}

	return;
}

//增加扑克
void CUserCard::AddCardCount(BYTE bCardCount)
{
	m_wCardCount += bCardCount;
}

//设置扑克
bool CUserCard::SetCurrentCard(bool bCurrentCard)
{
	//设置变量
	m_bCurrentCard=bCurrentCard;

	return true;
}

//设置扑克
bool CUserCard::SetCardData(WORD wCardCount, bool bCurrentCard)
{
	//设置变量
	m_wCardCount=wCardCount;
	m_bCurrentCard=bCurrentCard;

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
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y+(i%13)*13;
				//g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);	
				if(((m_cbCardData[i]&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);	
				
				}
				else
				{
					g_CardResource.m_ImageTableRight1.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);	
				
				}
			
			}
			////绘画扑克
			//if(m_wCardCount>8)
			//{
			//	if(m_wCardCount>15)
			//	{
			//		for (WORD i=0;i<9;i++)
			//		{
			//			int nXPos=m_ControlPoint.x-(i/9)*33;
			//			int nYPos=m_ControlPoint.y+(i%9)*21;
			//			g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);			
			//		}

			//		for (WORD i=9;i<16;i++)
			//		{
			//			int nXPos=m_ControlPoint.x-(i/9)*33;
			//			int nYPos=m_ControlPoint.y+((i+1)%9)*21;
			//			g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);			
			//		}
			//		for (WORD i=16;i<m_wCardCount;i++)
			//		{
			//			int nXPos=m_ControlPoint.x-((i+2)/9)*33;
			//			int nYPos=m_ControlPoint.y+((i+4)%9)*21;
			//			g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);			
			//		}
			//	}
			//	else
			//	{
			//		for (WORD i=0;i<9;i++)
			//		{
			//			int nXPos=m_ControlPoint.x-(i/9)*33;
			//			int nYPos=m_ControlPoint.y+(i%9)*21;
			//			g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);			
			//		}

			//		for (WORD i=9;i<m_wCardCount;i++)
			//		{
			//			int nXPos=m_ControlPoint.x-(i/9)*33;
			//			int nYPos=m_ControlPoint.y+((i+1)%9)*21;
			//			g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);			
			//		}
			//	}
			//}
			//else if(m_wCardCount<9)
			//{
			//	for (WORD i=0;i<m_wCardCount;i++)
			//	{
			//		int nXPos=m_ControlPoint.x-(i/9)*33;
			//		int nYPos=m_ControlPoint.y+(i%9)*21;
			//		g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);			
			//	}
			//}
			//else 
			//	return;

			break;
		}
	case Direction_South:	//南向
		{
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x-(i%13)*15;
				int nYPos=m_ControlPoint.y;
				if(((m_cbCardData[i]&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
				
				}
				else
				{
					g_CardResource.m_ImageTableBottom1.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
				
				}
			}

			////绘画扑克
			//for (WORD i=0;i<m_wCardCount;i++)
			//{
			//	int nXPos=m_ControlPoint.x-(i%10)*25;
			//	int nYPos=m_ControlPoint.y-(i/10)*36;
			//	g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			//}
			//绘画扑克
			/*if(m_wCardCount>8)
			{
				if(m_wCardCount>15)
				{
					for (WORD i=0;i<9;i++)
					{
						int nXPos=m_ControlPoint.x-(i%9)*25;
						int nYPos=m_ControlPoint.y-(i/9)*26;
						g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
					}

					for (WORD i=9;i<16;i++)
					{
						int nXPos=m_ControlPoint.x-((i+1)%9)*25;
						int nYPos=m_ControlPoint.y-(i/9)*26;
						g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);			
					}
					for (WORD i=16;i<m_wCardCount;i++)
					{
						int nXPos=m_ControlPoint.x-((i+4)%9)*25;
						int nYPos=m_ControlPoint.y-((i+2)/9)*26;
						g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);		
					}
				}
				else
				{
					for (WORD i=0;i<9;i++)
					{
						int nXPos=m_ControlPoint.x-(i%9)*25;
						int nYPos=m_ControlPoint.y-(i/9)*26;
						g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);		
					}

					for (WORD i=9;i<m_wCardCount;i++)
					{
						int nXPos=m_ControlPoint.x-((i+1)%9)*25;
						int nYPos=m_ControlPoint.y-(i/9)*26;
						g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);		
					}
				}
			}
			else if(m_wCardCount<9)
			{
				for (WORD i=0;i<m_wCardCount;i++)
				{
					int nXPos=m_ControlPoint.x-((i+1)%9)*25;
					int nYPos=m_ControlPoint.y-(i/9)*26;
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);		
				}
			}
			else 
				return;*/

			break;
		}
	case Direction_West:	//西向
		{
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y-((m_wCardCount-1-i)%11+1)*13;
				if(((m_cbCardData[m_wCardCount-i-1]&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
				
				}
				else
				{
					g_CardResource.m_ImageTableLeft1.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
				
				}
			}

			//绘画扑克
			//for (WORD i=0;i<m_wCardCount;i++)
			//{
			//	int nXPos=m_ControlPoint.x+((m_wCardCount-1-i)/10+1)*33;
			//	int nYPos=m_ControlPoint.y-((m_wCardCount-1-i)%10+1)*21;//2,1,0
			//	g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			//}
			//if(m_wCardCount>8)
			//{
			//	if(m_wCardCount>15)
			//	{
			//		for (WORD i=0;i<9;i++)
			//		{
			//			int nXPos=m_ControlPoint.x+((9-1-i)/9+1)*33;
			//			int nYPos=m_ControlPoint.y-((9-1-i)%9+1)*21;//9
			//			g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);		
			//		}

			//		for (WORD i=9;i<16;i++)
			//		{
			//			int nXPos=m_ControlPoint.x+((16-i-1)/9+2)*33;
			//			int nYPos=m_ControlPoint.y-((16+1-i-1)%9+1)*21;//8
			//			g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);			
			//		}
			//		for (WORD i=16;i<m_wCardCount;i++)
			//		{
			//			int nXPos=m_ControlPoint.x+((m_wCardCount-i-1)/9+3)*33;
			//			int nYPos=m_ControlPoint.y-((m_wCardCount+2-i-1)%9+1)*21;//7
			//			g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);			
			//		}
			//	}
			//	else
			//	{
			//		for (WORD i=0;i<9;i++)
			//		{
			//			int nXPos=m_ControlPoint.x+((9-1-i)/9+1)*33;
			//			int nYPos=m_ControlPoint.y-((9-1-i)%9+1)*21;//9
			//			g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);		
			//		}

			//		for (WORD i=9;i<m_wCardCount;i++)
			//		{
			//			int nXPos=m_ControlPoint.x+((m_wCardCount-i-1)/9+2)*33;
			//			int nYPos=m_ControlPoint.y-((m_wCardCount+1-i-1)%9+1)*21;//8
			//			g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);			
			//		}
			//	}
			//}
			//else if(m_wCardCount<9)
			//{
			//	for (WORD i=0;i<m_wCardCount;i++)
			//	{
			//		int nXPos=m_ControlPoint.x+((m_wCardCount-1-i)/9+1)*33;
			//		int nYPos=m_ControlPoint.y-((m_wCardCount-1-i)%9+1)*21;
			//		g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);		
			//	}
			//}
			//else 
			//	return;
			break;
		}
	//case Direction_North:	//北向
	//	{
	//		//绘画扑克
	//		/*for (WORD i=0;i<m_wCardCount;i++)
	//		{
	//			int nXPos=m_ControlPoint.x+((m_wCardCount-1-i)%10)*25;
	//			int nYPos=m_ControlPoint.y+((m_wCardCount-1-i)/10+1)*26;
	//			g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
	//		}*/

	//		if(m_wCardCount>8)
	//		{
	//			if(m_wCardCount>15)
	//			{
	//				for (WORD i=0;i<9;i++)
	//				{
	//					int nXPos=m_ControlPoint.x+((9-1-i)%9)*25;
	//					int nYPos=m_ControlPoint.y+((9-1-i)/9+1)*26;
	//					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);		
	//				}
	//				for (WORD i=9;i<16;i++)
	//				{
	//					int nXPos=m_ControlPoint.x+((16+1-i-1)%9)*25;
	//					int nYPos=m_ControlPoint.y+((16-i-1)/9+2)*26;
	//					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);			
	//				}
	//				
	//				for (WORD i=16;i<m_wCardCount;i++)
	//				{
	//					int nXPos=m_ControlPoint.x+((m_wCardCount+2-i-1)%9)*25;
	//					int nYPos=m_ControlPoint.y+((m_wCardCount-i-1)/9+3)*26;
	//					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);		
	//				}
	//			}
	//			else
	//			{
	//			
	//				for (WORD i=0;i<9;i++)
	//				{
	//					int nXPos=m_ControlPoint.x+((9-i-1)%9)*25;
	//					int nYPos=m_ControlPoint.y+((9-i-1)/9+1)*26;
	//					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);		
	//				}
	//			
	//				for (WORD i=9;i<m_wCardCount;i++)
	//				{
	//					int nXPos=m_ControlPoint.x+((m_wCardCount+1-i-1)%9)*25;
	//					int nYPos=m_ControlPoint.y+((m_wCardCount-i-1)/9+2)*26;
	//					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);		
	//				}
	//			}
	//		}
	//		else if(m_wCardCount<9)
	//		{
	//			for (WORD i=0;i<m_wCardCount;i++)
	//			{
	//				int nXPos=m_ControlPoint.x+((m_wCardCount-i-1)%9)*25;
	//				int nYPos=m_ControlPoint.y+((m_wCardCount-i-1)/9+1)*26;
	//				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);		
	//			}
	//		}
	//		else 
	//			return;
	//		break;
	//	}
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
CTableCard::CTableCard()
{
	//扑克数据
	m_wCardCount=0;
	m_cbWeaveCount = 0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));
	ZeroMemory(m_cbWeaveCardData,sizeof(m_cbWeaveCardData));
	ZeroMemory(m_cbWeaveCardCount,sizeof(m_cbWeaveCardCount));

	//控制变量
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//析构函数
CTableCard::~CTableCard()
{
}

//绘画扑克
void CTableCard::DrawCardControl(CDC * pDC)
{
	//绘画控制
	switch (m_CardDirection)
	{
	case Direction_East:	//东向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x-33;
				int nYPos=m_ControlPoint.y+i*15-25;
				//g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
				if(((m_cbCardData[m_wCardCount-i-1]&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
				
				}
				else
				{
					g_CardResource.m_ImageTableRight1.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
				
				}
			}

			break;
		}
	case Direction_South:	//南向
		{
			//绘画扑克
			for (BYTE i=0; i<m_cbWeaveCount; i++)
			{
				int nYPos=m_ControlPoint.y-45;
				int nXPos=m_ControlPoint.x-(m_cbWeaveCount-i)*30;

				for (BYTE j=0; j<m_cbWeaveCardCount[i]; j++)
				{
					//g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
					if(((m_cbWeaveCardData[i][j]&MASK_COLOR)>>4)!=1)
					{
						g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbWeaveCardData[i][j],nXPos,nYPos);
					}
					else
					{
						g_CardResource.m_ImageUserBottom1.DrawCardItem(pDC,m_cbWeaveCardData[i][j],nXPos,nYPos);
					}
					nYPos += m_HandCard.m_CardDistance;
				}
			}

			////绘画扑克
			//for (WORD i=0;i<m_wCardCount;i++)
			//{
			//	int nYPos=m_ControlPoint.y-36;
			//	int nXPos=m_ControlPoint.x-(m_wCardCount-i)*20;
			//	//g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			//	if(((m_cbCardData[i]&MASK_COLOR)>>4)!=1)
			//	{
			//		g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			//
			//	}
			//	else
			//	{
			//		g_CardResource.m_ImageUserBottom1.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			//
			//	}
			//}

			break;
		}
	case Direction_West:	//西向
		{
			//绘画扑克
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y-(m_wCardCount-i)*15;
				//g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
				if(((m_cbCardData[i]&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
				
				}
				else
				{
					g_CardResource.m_ImageTableLeft1.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
				
				}
			}

			break;
		}
	
	}

	return;
}

//babala 11-30
//设置扑克
bool CTableCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	if(cbCardData==NULL || wCardCount==0)
	{
		m_wCardCount=wCardCount;
		ZeroMemory(m_cbCardData,sizeof(m_cbCardData));
	}
	else
	{
		m_wCardCount=wCardCount;
		CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData));
	}

	return true;
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
	ZeroMemory(&m_CurrentCard,sizeof(m_CurrentCard));
	ZeroMemory(&m_CardItemArray,sizeof(m_CardItemArray));

	//加载设置
	m_ControlPoint.SetPoint(0,0);
	m_ControlSize.cy=CARD_HEIGHT+POS_SHOOT;
	m_ControlSize.cx=(CountArray(m_CardItemArray)+1)*CARD_WIDTH+POS_SPACE;

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
BYTE CCardControl::GetHoverCard()
{
	//获取扑克
	if (m_wHoverItem!=INVALID_ITEM)//说明已经点击了扑克
	{
		if (m_wHoverItem==CountArray(m_CardItemArray)) return m_CurrentCard.cbCardData;//点击的是刚发的那张牌
		return m_CardItemArray[m_wHoverItem].cbCardData;//否则就是手中的牌
	}

	return 0;
}

///11_17
//增加扑克数据
void CCardControl::AddCardData(BYTE cbCardData[],BYTE cbCardCount)
{
	//设置扑克
	for (WORD i=0;i<cbCardCount;i++)
	{
		m_CardItemArray[m_wCardCount].bShoot = false;
		m_CardItemArray[m_wCardCount].cbCardData=cbCardData[i];

		m_wCardCount++;
	}
}

//设置扑克
bool CCardControl::SetCurrentCard(BYTE cbCardData)
{
	//设置变量
	m_CurrentCard.bShoot=false;
	m_CurrentCard.cbCardData=cbCardData;

	return true;
}

//设置扑克
bool CCardControl::SetCurrentCard(tagCardItem CardItem)
{
	//设置变量
	m_CurrentCard.bShoot=CardItem.bShoot;
	m_CurrentCard.cbCardData=CardItem.cbCardData;

	return true;
}

//设置扑克
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_CardItemArray)+1);
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//当前扑克
	m_CurrentCard.bShoot=false;
	m_CurrentCard.cbCardData=cbCurrentCard;
	//m_MagicCard=cbMagicCard;

	//设置扑克
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

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
		m_CardItemArray[i].cbCardData=CardItemArray[i].cbCardData;
	}

	return true;
}

//绘画扑克
void CCardControl::DrawCardControl(CDC * pDC)
{
	//绘画准备
	int nXExcursion=m_ControlPoint.x+(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH;

	//绘画扑克
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//计算位置
		int nXScreenPos=nXExcursion+CARD_WIDTH*i;
		int nYScreenPos=m_ControlPoint.y+(((m_CardItemArray[i].bShoot==false)&&(m_wHoverItem!=i))?POS_SHOOT:0);

		//绘画扑克
		if(m_CardItemArray[i].cbCardData!=m_MagicCard)
		{
			BYTE cbCardData=(m_bDisplayItem==true)?m_CardItemArray[i].cbCardData:0;
			//g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
		
			if(((cbCardData&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
			}	
			else
			{
				g_CardResource.m_ImageUserBottom1.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
			}
		}
	
		else
		{
			BYTE cbCardData=(m_bDisplayItem==true)?m_CardItemArray[i].cbCardData:0;
			g_CardResource.m_ImageUserMagicBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
		}
	}

	//当前扑克
	if (m_CurrentCard.cbCardData!=0)
	{
		//计算位置
		int nXScreenPos=m_ControlPoint.x+m_ControlSize.cx-CARD_WIDTH;
		int nYScreenPos=m_ControlPoint.y+(((m_CurrentCard.bShoot==false)&&(m_wHoverItem!=CountArray(m_CardItemArray)))?POS_SHOOT:0);

		//绘画扑克
		if(m_CurrentCard.cbCardData!=m_MagicCard)
		{
			BYTE cbCardData=(m_bDisplayItem==true)?m_CurrentCard.cbCardData:0;
			//g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
			if(((cbCardData&MASK_COLOR)>>4)!=1)
			{
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
			}
			else
			{
				g_CardResource.m_ImageUserBottom1.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
			}	
		}
		else
		{
			BYTE cbCardData=(m_bDisplayItem==true)?m_CurrentCard.cbCardData:0;
			g_CardResource.m_ImageUserMagicBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
		}
	}
	//
	//if (m_MagicCard!=0)
	//{
	//	//计算位置
	//	int nXScreenPos=m_ControlPoint.x-CARD_WIDTH;
	//	int nYScreenPos=m_ControlPoint.y;

	//	//绘画扑克
	//	BYTE cbCardData=(m_bDisplayItem==true)?m_MagicCard:0;
	//	g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
	//}
	return;
}

//索引切换
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//基准位置
	int nXPos=MousePoint.x-m_ControlPoint.x;
	int nYPos=MousePoint.y-m_ControlPoint.y;

	//范围判断
	if ((nXPos<0)||(nXPos>m_ControlSize.cx)) return INVALID_ITEM;
	if ((nYPos<POS_SHOOT)||(nYPos>m_ControlSize.cy)) return INVALID_ITEM;

	//牌列子项
	if (nXPos<CARD_WIDTH*CountArray(m_CardItemArray))
	{
		WORD wViewIndex=(WORD)(nXPos/CARD_WIDTH)+m_wCardCount;
		if (wViewIndex>=CountArray(m_CardItemArray)) return wViewIndex-CountArray(m_CardItemArray);
		return INVALID_ITEM;
	}

	//当前子项
	if ((m_CurrentCard.cbCardData!=0)&&(nXPos>=(m_ControlSize.cx-CARD_WIDTH))) return CountArray(m_CardItemArray);

	return INVALID_ITEM;
}

//光标消息
bool CCardControl::OnEventSetCursor(CPoint Point, bool & bRePaint)
{
	//获取索引
	WORD wHoverItem=SwitchCardPoint(Point);

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
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
	}

	return (wHoverItem!=INVALID_ITEM);
}

//////////////////////////////////////////////////////////////////////////

//变量声明
CCardResource						g_CardResource;						//扑克资源

//////////////////////////////////////////////////////////////////////////

CHandCard::CHandCard(void)
{
	m_ControlSize.cy=CARD_HEIGHT+POS_SHOOT;
	m_ControlSize.cx=(CountArray(m_CardItemArray)+1)*CARD_WIDTH+POS_SPACE;

	ZeroMemory(m_CardItemArray,sizeof(m_CardItemArray));

	m_cbCardCount=0;
	m_cbHoverItem=INVALID_ITEM;

	m_bCanOutCard=false;
	m_bDisplayItem=false;
	m_bCurrentCardArea = false;

	m_CardDistance=20;
}

CHandCard::~CHandCard(void)
{
}

BEGIN_MESSAGE_MAP(CHandCard, CWnd)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//依据光标所在的点而得到相应的牌数据
BYTE CHandCard::GetCurrentCardByPoint(CPoint pt)
{
	//基准位置
	int nXPos=pt.x;
	int nYPos=pt.y;

	//范围判断
	if ((nXPos<0)||(nXPos>36)) 	return INVALID_ITEM;
	if ((nYPos<0)||(nYPos>(m_CardDistance*(m_cbCardCount-1)+90))) return INVALID_ITEM;

	//牌列子项
	if (nYPos<(m_CardDistance*(m_cbCardCount-1)+90))
	{
		BYTE wViewIndex=(BYTE)(nYPos/m_CardDistance);
		if (wViewIndex>=m_cbCardCount) return (BYTE)(m_cbCardCount-1);
		return wViewIndex;
	}

	return INVALID_ITEM;
}

//设置是否为鼠标所在的当前区域
void CHandCard::SetCurrentCardArea(bool bCurrentCardArea)
{
	m_bCurrentCardArea = bCurrentCardArea;

	//当区域不是当前区域的话，则同时将选中的子项设为无
	if(m_bCurrentCardArea==false)
	{
		m_cbHoverItem=INVALID_ITEM;
	}

	Invalidate(false);
}

//删除普通扑克(一张)
bool CHandCard::DeleteCardData(BYTE cbCardData)
{
	for (WORD i=0; i<m_cbCardCount; i++)
	{
		if (m_CardItemArray[i].cbCardData == cbCardData)
		{
			m_CardItemArray[i].cbCardData = 0;
			m_CardItemArray[i].bShoot = false;
			m_cbCardCount--;
			
			//将后面的牌移动前面来
			SortHandCard();

			//调整控件
			RectifyControl();
			return true;
		}
	}
	return false;
}

//通过数组下标删除扑克
bool CHandCard::DeleteByArrayIndex(BYTE cbIndex)
{

	ASSERT(cbIndex>=0 && cbIndex<MAXCOUNT);

	if (cbIndex<0 && cbIndex>=MAXCOUNT) return false;
	if (m_CardItemArray[cbIndex].cbCardData == 0) return false;

	m_CardItemArray[cbIndex].cbCardData = 0;
	m_CardItemArray[cbIndex].bShoot = false;

	m_cbCardCount--;

	//将后面的牌移动前面来
	SortHandCard();
			
	//调整控件
	RectifyControl();
	return true;
}
//11-29
//得到花牌索引
BYTE CHandCard::SwitchToCardIndex(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//如果是数字，直接得到最后一位值
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}

//增加扑克
void  CHandCard::AddCardData(BYTE cbCardData[],BYTE cbCardCount)
{
	ASSERT((m_cbCardCount + cbCardCount)<=MAXCOUNT);

	for (BYTE i=0; i<cbCardCount; i++)
	{
		m_CardItemArray[m_cbCardCount].cbCardData = cbCardData[i];
		m_CardItemArray[m_cbCardCount++].bShoot = false;
	}

	//调整控件
	RectifyControl();
}
//只增加一个牌数据
void CHandCard::AddCardData(BYTE cbCardData)
{
	ASSERT((m_cbCardCount + 1)<=MAXCOUNT);


	m_CardItemArray[m_cbCardCount].cbCardData = cbCardData;
	m_CardItemArray[m_cbCardCount].bShoot = false;

	m_cbCardCount++;
	

	//调整控件
	RectifyControl();
}

//设置扑克--设置这显示区域中的牌数据
bool CHandCard::SetCardData(const BYTE cbCardData[], BYTE wCardCount)
{
	//效验大小
	ASSERT(wCardCount<=CountArray(m_CardItemArray)+1);
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//设置扑克数量
	m_cbCardCount=wCardCount;
	for (WORD i=0;i<m_cbCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	//调整控件
	RectifyControl();

	return true;
}

//基准位置
void CHandCard::SetBenchmarkPos(int nXPos, int nYPos)
{
	//位置变量
	m_PointBenchmark.SetPoint(nXPos,nYPos);

	//调整控件
	RectifyControl();

	return;
}

//调整控件--依据牌的张数而调整显示
void CHandCard::RectifyControl(bool IsMove)
{
	//应该是根据设置的基准点位置调整窗体的位置

	CRect rcRect;
	rcRect.left=m_PointBenchmark.x;
	rcRect.top=m_PointBenchmark.y;
	rcRect.right=m_PointBenchmark.x+CARD_WIDTH;

	//如果当前区域没有牌，设置窗口区域为一张牌的高，宽
	if (m_cbCardCount == 0)
	{
		rcRect.bottom = m_PointBenchmark.y + CARD_HEIGHT;
	}
	else if(m_cbCardCount<=5)
	{
		m_CardDistance=20;
		rcRect.bottom=m_PointBenchmark.y+m_CardDistance*(m_cbCardCount-1)+90;
	}
	else
	{
		m_CardDistance=(25*4)/m_cbCardCount;
		rcRect.bottom=m_PointBenchmark.y+m_CardDistance*(m_cbCardCount-1)+90;
	}

	//重新排列牌的顺序
	SortHandCardByIndex();

	//移动窗口
	MoveWindow(&rcRect);

	Invalidate(FALSE);
	return;
}

void CHandCard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
 
	int nXExcursion=0;
	
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//内存缓冲区
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&BufferImage);

	int nXScreenPos=0,nYScreenPos=0;
	
	//只要是牌个数为0的区域都显示下述颜色
	if(m_cbCardCount == 0)
	{
		//画一个四方形区域
		BufferDC.FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),RGB(15,111,119));

		//画四方形的边框
		CPen newPen,*oldPen;
		newPen.CreatePen(PS_SOLID,2,RGB(60, 60, 60));
		oldPen=BufferDC.SelectObject(&newPen);
		BufferDC.Rectangle(0,0,rcClient.Width(),rcClient.Height());			//绘制区域边框
		BufferDC.SelectObject(oldPen);

		//画四方形中的网格线
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BACK);
		CBrush brush,*pOldBrush;
		brush.CreatePatternBrush(&bitmap);

		pOldBrush = (CBrush*)BufferDC.SelectObject(&brush);
		oldPen = (CPen*)BufferDC.SelectStockObject(NULL_PEN);

		BufferDC.Rectangle(CRect(1,1, rcClient.Width(),rcClient.Height()));

		BufferDC.SelectObject(pOldBrush);
		BufferDC.SelectObject(oldPen);
	}
	//相应大小的背景
	if(m_cbCardCount>0)
	{
		BufferDC.FillSolidRect(rcClient,RGB(24,60,104));
	}
	

	BYTE cbCardData =0;
	//绘画扑克
	if(m_bDisplayItem==true)
	{
		for (WORD i=0;i<m_cbCardCount;i++)
		{
			//绘画扑克
			cbCardData=m_CardItemArray[i].cbCardData;
			if(cbCardData!=0)
			{
				//显示不带花的牌
				if(((cbCardData&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,cbCardData,nXScreenPos,nYScreenPos);
				}
				//显示带花的牌
				else
				{
					g_CardResource.m_ImageUserBottom1.DrawCardItem(&BufferDC,cbCardData,nXScreenPos,nYScreenPos);
				}
				nYScreenPos += m_CardDistance;
			}
		}
	}
	
	//绘画边框
	if(m_cbHoverItem!=INVALID_ITEM && m_bCurrentCardArea)
	{
		CPen myPen,*OldMyPen;
		myPen.CreatePen(PS_SOLID,4,RGB(0,255,64));
		OldMyPen=BufferDC.SelectObject(&myPen);

		if((m_cbHoverItem+1)<m_cbCardCount)
		{
			BufferDC.MoveTo(0,m_cbHoverItem*m_CardDistance);
			BufferDC.LineTo(0,(m_cbHoverItem+1)*m_CardDistance);

			BufferDC.MoveTo(0,m_cbHoverItem*m_CardDistance);
			BufferDC.LineTo(36,m_cbHoverItem*m_CardDistance);

			BufferDC.MoveTo(36,m_cbHoverItem*m_CardDistance);
			BufferDC.LineTo(36,(m_cbHoverItem+1)*m_CardDistance);
		}
		else
		{
			
			BufferDC.MoveTo(0,(m_cbHoverItem)*m_CardDistance);
			BufferDC.LineTo(rcClient.Width(),(m_cbHoverItem)*m_CardDistance);

			BufferDC.LineTo(rcClient.Width(),(m_cbHoverItem)*m_CardDistance+CARD_HEIGHT);

			BufferDC.LineTo(0,(m_cbHoverItem)*m_CardDistance+CARD_HEIGHT);

			BufferDC.LineTo(0,(m_cbHoverItem)*m_CardDistance);
		}

		BufferDC.SelectObject(OldMyPen);	

	}
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();
}


BOOL CHandCard::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CRect rcClient;
	CPoint MousePoint;
	GetClientRect(&rcClient);
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	BYTE cbHoverItem=INVALID_ITEM;

	//计算索引
	CRect rcItem(0,0,rcClient.Width(),rcClient.Height());
	if (rcItem.PtInRect(MousePoint))
	{
		cbHoverItem=GetCurrentCardByPoint(MousePoint);
	}

	//设置索引
	if (m_cbHoverItem!=cbHoverItem)
	{
		m_cbHoverItem=cbHoverItem;
		Invalidate(FALSE);
	}

	//更新判断
	if ((cbHoverItem!=INVALID_ITEM)&&(m_bPositively == true))
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		return true;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

bool CHandCard::SortHandCard()
{
	BYTE index = 0, cbCardData[MAXCOUNT]={0,0,0,0,0,0,0,0,0};
	////从前向后，找到数据为0的索引位置,从该位置开始将后面的数据移到前面
	for(BYTE i=0; i<9; i++)
	{
		if (m_CardItemArray[i].cbCardData != 0)
		{
			cbCardData[index++] = m_CardItemArray[i].cbCardData;
		}
	}

	//拷贝新数据
	for (BYTE i=0; i<9; i++)
	{
		m_CardItemArray[i].cbCardData = cbCardData[i];	
	}

	return true;
}

//右键单击出牌
void CHandCard::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bCanOutCard == true)
	{
		//fe11-30将下述盘旋数据初始为空
		BYTE cbHoverCard=0;
		if (m_cbHoverItem!=INVALID_ITEM)//说明已经点击了扑克
		{
			cbHoverCard=m_CardItemArray[m_cbHoverItem].cbCardData;//否则就是手中的牌
			m_CardItemArray[m_cbHoverItem].cbCardData = 0;
			m_CardItemArray[m_cbHoverItem].bShoot = false;
			m_cbCardCount--;
			
			//将牌向前移动
			SortHandCard();

			//调整控件
			RectifyControl();
		}

		//获取扑克
		if (cbHoverCard!=0) 
		{
			AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);
			return ;
		}
	}

	CWnd::OnRButtonDown(nFlags, point);
}

//重新按索引而排列牌的顺序
void CHandCard::SortHandCardByIndex(void)
{
	for(BYTE i=0;i<m_cbCardCount-1;i++)
	{
		for(BYTE j=i+1;j<m_cbCardCount;j++)
		{
			if(SwitchToCardIndex(m_CardItemArray[i].cbCardData)>SwitchToCardIndex(m_CardItemArray[j].cbCardData))
			{
				BYTE temp=m_CardItemArray[i].cbCardData;
				m_CardItemArray[i].cbCardData=m_CardItemArray[j].cbCardData;
				m_CardItemArray[j].cbCardData=temp;
			}
		}
	}
}

//当子窗口处于活动状态时，其父窗口收不到这个消息，故将其转发过去
void CHandCard::OnMouseMove(UINT nFlags, CPoint point)
{
	m_cbHoverItem = GetCurrentCardByPoint(point);

	ClientToScreen(&point);
	::SendMessage(GetParent()->GetSafeHwnd(),IDM_MOUSE_MOVE,point.x,point.y);

	CWnd::OnMouseMove(nFlags, point);
}
