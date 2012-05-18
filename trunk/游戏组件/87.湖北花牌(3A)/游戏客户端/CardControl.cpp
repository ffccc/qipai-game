#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"
//////////////////////////////////////////////////////////////////////////
//�궨��

//��������
#define POS_SHOOT					5									//��������
//#define POS_SPACE					8									//�ָ����
#define POS_SPACE					5									//�ָ����
#define ITEM_COUNT					23									//������Ŀ
const BYTE INVALID_ITEM =  0xFF;								//��Ч����

//�˿˴�С
//#define CARD_WIDTH					35									//�˿˿��
//#define CARD_HEIGHT					50									//�˿˸߶�
//#define CARD_WIDTH					25									//�˿˿��
//#define CARD_HEIGHT					90									//�˿˸߶�

#define CARD_WIDTH					35									//�˿˿��
#define CARD_HEIGHT					90									//�˿˸߶�

//////////////////////////////////////////////////////////////////////////
//���캯��
CCardListImage::CCardListImage()
{
	//λ�ñ���
	m_nItemWidth=0;
	m_nItemHeight=0;
	m_nViewWidth=0;
	m_nViewHeight=0;

	return;
}

//��������
CCardListImage::~CCardListImage()
{
}

//������Դ
bool CCardListImage::LoadResource(UINT uResourceID, int nViewWidth, int nViewHeight)
{
	//������Դ
	m_CardListImage.LoadFromResource(AfxGetInstanceHandle(),uResourceID);

	//���ñ���
	m_nViewWidth=nViewWidth;
	m_nViewHeight=nViewHeight;
	m_nItemHeight=m_CardListImage.GetHeight();
	m_nItemWidth=m_CardListImage.GetWidth()/ITEM_COUNT;

	return true;
}

//�ͷ���Դ
bool CCardListImage::DestroyResource()
{
	//���ñ���
	m_nItemWidth=0;
	m_nItemHeight=0;

	//�ͷ���Դ
	m_CardListImage.Destroy();

	return true;
}

//��ȡλ��
int CCardListImage::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) return 0;

	//����λ��
	//BYTE cbValue=cbCardData&MASK_VALUE;
	//BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	//return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);

	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>0x01)?(10+cbValue):cbValue;
}

//�滭�˿�
bool CCardListImage::DrawCardItem(CDC * pDestDC, BYTE cbCardData, int xDest, int yDest)
{
	//Ч��״̬
	ASSERT(m_CardListImage.IsNull()==false);
	ASSERT((m_nItemWidth!=0)&&(m_nItemHeight!=0));

	//�滭����
	int nImageXPos=GetImageIndex(cbCardData)*m_nItemWidth;
	m_CardListImage.AlphaDrawImage(pDestDC,xDest,yDest,m_nItemWidth,m_nItemHeight,nImageXPos,0,RGB(255,0,255));

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardResource::CCardResource()
{
}

//��������
CCardResource::~CCardResource()
{
}

//������Դ
bool CCardResource::LoadResource()
{
	//��������
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//�û��˿�
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

	//�����˿�
	m_ImageTableTop.LoadResource(IDB_CARD_TABLE_TOP,15,54);
	m_ImageTableLeft.LoadResource(IDB_CARD_TABLE_LEFT,54,15);
	m_ImageTableRight.LoadResource(IDB_CARD_TABLE_RIGHT,54,15);
	m_ImageTableBottom.LoadResource(IDB_CARD_TABLE_BOTTOM,15,54);
	
	return true;
}

//������Դ
bool CCardResource::DestroyResource()
{
	//�û��˿�
	m_ImageUserTop.Destroy();
	m_ImageUserLeft.Destroy();
	m_ImageUserRight.Destroy();
	m_ImageUserBottom.DestroyResource();

	//�����˿�
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


	//�ƶ��˿�
	m_ImageBackH.Destroy();
	m_ImageBackV.Destroy();
	m_ImageHeapSingleV.Destroy();
	m_ImageHeapSingleH.Destroy();
	m_ImageHeapDoubleV.Destroy();
	m_ImageHeapDoubleH.Destroy();

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CHeapCard::CHeapCard()
{
	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	//�˿˱���
	m_wFullCount=0;
	m_wMinusHeadCount=0;
	m_wMinusLastCount=0;

	return;
}

//��������
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
//�滭�˿�--�滭�˿�ʱ��һ����ǽ��Ϊ�����֣�ͷ β �м�
//��Ϊһ����ǽ�п���ͷ���е����ƻ�β�е�����ֻ���м��������������һ�������Ҫ����
void CHeapCard::DrawCardControl(CDC * pDC)
{
	return;
}

//�����˿�
bool CHeapCard::SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount)
{
	//���ñ���
	m_wFullCount=wFullCount;
	m_wMinusHeadCount=wMinusHeadCount;//ͷ����ȱ
	m_wMinusLastCount=wMinusLastCount;//β����ȱ

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CWeaveCard::CWeaveCard()
{
	//״̬����
	m_bAnGang=false;
	m_bDisplayItem=false;
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_South;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	return;
}

//��������
CWeaveCard::~CWeaveCard()
{
}

//�滭�˿�
void CWeaveCard::DrawCardControl(CDC * pDC)
{
	//��ʾ�ж�
	if (m_wCardCount==0) return;

	//��������
	int nXScreenPos=0,nYScreenPos=0;
	int nItemWidth=0,nItemHeight=0,nItemWidthEx=0,nItemHeightEx=0;

	//�滭�˿�
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//��������
			nItemWidth=g_CardResource.m_ImageTableRight.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableRight.GetViewHeight();
			nItemWidthEx=g_CardResource.m_ImageTableTop.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableTop.GetViewHeight();

			//�����˿�
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

			//�ڶ��˿�
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

			//��һ�˿�
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
			//�����˿�
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

			//�����˿�
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
	case Direction_South:	//����
		{
			//��������
			nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableBottom.GetViewHeight();
			nItemWidthEx=g_CardResource.m_ImageTableRight.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableRight.GetViewHeight();
		
			//�����˿�
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
			//�ڶ��˿�
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
			//��һ�˿�
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
			
			//�����˿�
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
				//�����˿�
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
	case Direction_West:	//����
		{
			//��������
			nItemWidth=g_CardResource.m_ImageTableLeft.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableLeft.GetViewHeight();
			nItemWidthEx=g_CardResource.m_ImageTableBottom.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableBottom.GetViewHeight();

			//��һ�˿�
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
			//�ڶ��˿�
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

			//�����˿�
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
			//�����˿�			
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
			//�����˿�
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

//�����˿�
bool CWeaveCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//�����˿�
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

//��ȡ�˿�
BYTE CWeaveCard::GetCardData(WORD wIndex)
{
	ASSERT(wIndex<CountArray(m_cbCardData));

	if(m_bAnGang==true)
		return((m_bDisplayItem==true)&&(wIndex==1))?m_cbCardData[wIndex]:0;
	else
		return (m_bDisplayItem==true)?m_cbCardData[wIndex]:0;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CUserCard::CUserCard()
{
	//�˿�����
	m_wCardCount=0;
	m_bCurrentCard=false;

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//��������
CUserCard::~CUserCard()
{
}

//�滭�˿�
void CUserCard::DrawCardControl(CDC * pDC)
{
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
		//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y+50;
				g_CardResource.m_ImageUserRight.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
			}

			//�����˿�
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
	case Direction_West:	//����
		{
			//�����˿�
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

			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y-67;
				g_CardResource.m_ImageUserLeft.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
			}
			break;
		}
	//case Direction_North:	//����
	//	{
	//		//��ǰ�˿�
	//		if (m_bCurrentCard==true)
	//		{
	//			int nXPos=m_ControlPoint.x;
	//			int nYPos=m_ControlPoint.y+200;
	//			g_CardResource.m_ImageUserTop.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
	//		}

	//		//�����˿�
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

//�����˿�
void CUserCard::AddCardCount(BYTE bCardCount)
{
	m_wCardCount += bCardCount;
}

//�����˿�
bool CUserCard::SetCurrentCard(bool bCurrentCard)
{
	//���ñ���
	m_bCurrentCard=bCurrentCard;

	return true;
}

//�����˿�
bool CUserCard::SetCardData(WORD wCardCount, bool bCurrentCard)
{
	//���ñ���
	m_wCardCount=wCardCount;
	m_bCurrentCard=bCurrentCard;

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDiscardCard::CDiscardCard()
{
	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//��������
CDiscardCard::~CDiscardCard()
{
}

//�滭�˿�
void CDiscardCard::DrawCardControl(CDC * pDC)
{
	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����
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
			////�滭�˿�
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
	case Direction_South:	//����
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

			////�滭�˿�
			//for (WORD i=0;i<m_wCardCount;i++)
			//{
			//	int nXPos=m_ControlPoint.x-(i%10)*25;
			//	int nYPos=m_ControlPoint.y-(i/10)*36;
			//	g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			//}
			//�滭�˿�
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
	case Direction_West:	//����
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

			//�滭�˿�
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
	//case Direction_North:	//����
	//	{
	//		//�滭�˿�
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

//�����˿�
bool CDiscardCard::AddCardItem(BYTE cbCardData)
{
	//�����˿�
	if (m_wCardCount>=CountArray(m_cbCardData))
	{
		m_wCardCount--;
		MoveMemory(m_cbCardData,m_cbCardData+1,CountArray(m_cbCardData)-1);
	}

	//�����˿�
	m_cbCardData[m_wCardCount++]=cbCardData;

	return true;
}

//�����˿�
bool CDiscardCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableCard::CTableCard()
{
	//�˿�����
	m_wCardCount=0;
	m_cbWeaveCount = 0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));
	ZeroMemory(m_cbWeaveCardData,sizeof(m_cbWeaveCardData));
	ZeroMemory(m_cbWeaveCardCount,sizeof(m_cbWeaveCardCount));

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//��������
CTableCard::~CTableCard()
{
}

//�滭�˿�
void CTableCard::DrawCardControl(CDC * pDC)
{
	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//�滭�˿�
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
	case Direction_South:	//����
		{
			//�滭�˿�
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

			////�滭�˿�
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
	case Direction_West:	//����
		{
			//�滭�˿�
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
//�����˿�
bool CTableCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
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

//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bPositively=false;
	m_bDisplayItem=false;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//�˿�����
	m_wCardCount=0;
	m_wHoverItem=INVALID_ITEM;
	ZeroMemory(&m_CurrentCard,sizeof(m_CurrentCard));
	ZeroMemory(&m_CardItemArray,sizeof(m_CardItemArray));

	//��������
	m_ControlPoint.SetPoint(0,0);
	m_ControlSize.cy=CARD_HEIGHT+POS_SHOOT;
	m_ControlSize.cx=(CountArray(m_CardItemArray)+1)*CARD_WIDTH+POS_SPACE;

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//��׼λ��
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//��ȡ�˿�
BYTE CCardControl::GetHoverCard()
{
	//��ȡ�˿�
	if (m_wHoverItem!=INVALID_ITEM)//˵���Ѿ�������˿�
	{
		if (m_wHoverItem==CountArray(m_CardItemArray)) return m_CurrentCard.cbCardData;//������Ǹշ���������
		return m_CardItemArray[m_wHoverItem].cbCardData;//����������е���
	}

	return 0;
}

///11_17
//�����˿�����
void CCardControl::AddCardData(BYTE cbCardData[],BYTE cbCardCount)
{
	//�����˿�
	for (WORD i=0;i<cbCardCount;i++)
	{
		m_CardItemArray[m_wCardCount].bShoot = false;
		m_CardItemArray[m_wCardCount].cbCardData=cbCardData[i];

		m_wCardCount++;
	}
}

//�����˿�
bool CCardControl::SetCurrentCard(BYTE cbCardData)
{
	//���ñ���
	m_CurrentCard.bShoot=false;
	m_CurrentCard.cbCardData=cbCardData;

	return true;
}

//�����˿�
bool CCardControl::SetCurrentCard(tagCardItem CardItem)
{
	//���ñ���
	m_CurrentCard.bShoot=CardItem.bShoot;
	m_CurrentCard.cbCardData=CardItem.cbCardData;

	return true;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_CardItemArray)+1);
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//��ǰ�˿�
	m_CurrentCard.bShoot=false;
	m_CurrentCard.cbCardData=cbCurrentCard;
	//m_MagicCard=cbMagicCard;

	//�����˿�
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//�����˿�
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return 0;

	//�����˿�
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=CardItemArray[i].bShoot;
		m_CardItemArray[i].cbCardData=CardItemArray[i].cbCardData;
	}

	return true;
}

//�滭�˿�
void CCardControl::DrawCardControl(CDC * pDC)
{
	//�滭׼��
	int nXExcursion=m_ControlPoint.x+(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH;

	//�滭�˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//����λ��
		int nXScreenPos=nXExcursion+CARD_WIDTH*i;
		int nYScreenPos=m_ControlPoint.y+(((m_CardItemArray[i].bShoot==false)&&(m_wHoverItem!=i))?POS_SHOOT:0);

		//�滭�˿�
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

	//��ǰ�˿�
	if (m_CurrentCard.cbCardData!=0)
	{
		//����λ��
		int nXScreenPos=m_ControlPoint.x+m_ControlSize.cx-CARD_WIDTH;
		int nYScreenPos=m_ControlPoint.y+(((m_CurrentCard.bShoot==false)&&(m_wHoverItem!=CountArray(m_CardItemArray)))?POS_SHOOT:0);

		//�滭�˿�
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
	//	//����λ��
	//	int nXScreenPos=m_ControlPoint.x-CARD_WIDTH;
	//	int nYScreenPos=m_ControlPoint.y;

	//	//�滭�˿�
	//	BYTE cbCardData=(m_bDisplayItem==true)?m_MagicCard:0;
	//	g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
	//}
	return;
}

//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//��׼λ��
	int nXPos=MousePoint.x-m_ControlPoint.x;
	int nYPos=MousePoint.y-m_ControlPoint.y;

	//��Χ�ж�
	if ((nXPos<0)||(nXPos>m_ControlSize.cx)) return INVALID_ITEM;
	if ((nYPos<POS_SHOOT)||(nYPos>m_ControlSize.cy)) return INVALID_ITEM;

	//��������
	if (nXPos<CARD_WIDTH*CountArray(m_CardItemArray))
	{
		WORD wViewIndex=(WORD)(nXPos/CARD_WIDTH)+m_wCardCount;
		if (wViewIndex>=CountArray(m_CardItemArray)) return wViewIndex-CountArray(m_CardItemArray);
		return INVALID_ITEM;
	}

	//��ǰ����
	if ((m_CurrentCard.cbCardData!=0)&&(nXPos>=(m_ControlSize.cx-CARD_WIDTH))) return CountArray(m_CardItemArray);

	return INVALID_ITEM;
}

//�����Ϣ
bool CCardControl::OnEventSetCursor(CPoint Point, bool & bRePaint)
{
	//��ȡ����
	WORD wHoverItem=SwitchCardPoint(Point);

	//��Ӧ�ж�
	if ((m_bPositively==false)&&(m_wHoverItem!=INVALID_ITEM))
	{
		bRePaint=true;
		m_wHoverItem=INVALID_ITEM;
	}

	//�����ж�
	if ((wHoverItem!=m_wHoverItem)&&(m_bPositively==true))
	{
		bRePaint=true;
		m_wHoverItem=wHoverItem;
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
	}

	return (wHoverItem!=INVALID_ITEM);
}

//////////////////////////////////////////////////////////////////////////

//��������
CCardResource						g_CardResource;						//�˿���Դ

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

//���ݹ�����ڵĵ���õ���Ӧ��������
BYTE CHandCard::GetCurrentCardByPoint(CPoint pt)
{
	//��׼λ��
	int nXPos=pt.x;
	int nYPos=pt.y;

	//��Χ�ж�
	if ((nXPos<0)||(nXPos>36)) 	return INVALID_ITEM;
	if ((nYPos<0)||(nYPos>(m_CardDistance*(m_cbCardCount-1)+90))) return INVALID_ITEM;

	//��������
	if (nYPos<(m_CardDistance*(m_cbCardCount-1)+90))
	{
		BYTE wViewIndex=(BYTE)(nYPos/m_CardDistance);
		if (wViewIndex>=m_cbCardCount) return (BYTE)(m_cbCardCount-1);
		return wViewIndex;
	}

	return INVALID_ITEM;
}

//�����Ƿ�Ϊ������ڵĵ�ǰ����
void CHandCard::SetCurrentCardArea(bool bCurrentCardArea)
{
	m_bCurrentCardArea = bCurrentCardArea;

	//�������ǵ�ǰ����Ļ�����ͬʱ��ѡ�е�������Ϊ��
	if(m_bCurrentCardArea==false)
	{
		m_cbHoverItem=INVALID_ITEM;
	}

	Invalidate(false);
}

//ɾ����ͨ�˿�(һ��)
bool CHandCard::DeleteCardData(BYTE cbCardData)
{
	for (WORD i=0; i<m_cbCardCount; i++)
	{
		if (m_CardItemArray[i].cbCardData == cbCardData)
		{
			m_CardItemArray[i].cbCardData = 0;
			m_CardItemArray[i].bShoot = false;
			m_cbCardCount--;
			
			//����������ƶ�ǰ����
			SortHandCard();

			//�����ؼ�
			RectifyControl();
			return true;
		}
	}
	return false;
}

//ͨ�������±�ɾ���˿�
bool CHandCard::DeleteByArrayIndex(BYTE cbIndex)
{

	ASSERT(cbIndex>=0 && cbIndex<MAXCOUNT);

	if (cbIndex<0 && cbIndex>=MAXCOUNT) return false;
	if (m_CardItemArray[cbIndex].cbCardData == 0) return false;

	m_CardItemArray[cbIndex].cbCardData = 0;
	m_CardItemArray[cbIndex].bShoot = false;

	m_cbCardCount--;

	//����������ƶ�ǰ����
	SortHandCard();
			
	//�����ؼ�
	RectifyControl();
	return true;
}
//11-29
//�õ���������
BYTE CHandCard::SwitchToCardIndex(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;

	//��������֣�ֱ�ӵõ����һλֵ
	if (cbColor == 0 || cbColor==1) return cbValue-1;
	else return  cbValue+9;
}

//�����˿�
void  CHandCard::AddCardData(BYTE cbCardData[],BYTE cbCardCount)
{
	ASSERT((m_cbCardCount + cbCardCount)<=MAXCOUNT);

	for (BYTE i=0; i<cbCardCount; i++)
	{
		m_CardItemArray[m_cbCardCount].cbCardData = cbCardData[i];
		m_CardItemArray[m_cbCardCount++].bShoot = false;
	}

	//�����ؼ�
	RectifyControl();
}
//ֻ����һ��������
void CHandCard::AddCardData(BYTE cbCardData)
{
	ASSERT((m_cbCardCount + 1)<=MAXCOUNT);


	m_CardItemArray[m_cbCardCount].cbCardData = cbCardData;
	m_CardItemArray[m_cbCardCount].bShoot = false;

	m_cbCardCount++;
	

	//�����ؼ�
	RectifyControl();
}

//�����˿�--��������ʾ�����е�������
bool CHandCard::SetCardData(const BYTE cbCardData[], BYTE wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_CardItemArray)+1);
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//�����˿�����
	m_cbCardCount=wCardCount;
	for (WORD i=0;i<m_cbCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	//�����ؼ�
	RectifyControl();

	return true;
}

//��׼λ��
void CHandCard::SetBenchmarkPos(int nXPos, int nYPos)
{
	//λ�ñ���
	m_PointBenchmark.SetPoint(nXPos,nYPos);

	//�����ؼ�
	RectifyControl();

	return;
}

//�����ؼ�--�����Ƶ�������������ʾ
void CHandCard::RectifyControl(bool IsMove)
{
	//Ӧ���Ǹ������õĻ�׼��λ�õ��������λ��

	CRect rcRect;
	rcRect.left=m_PointBenchmark.x;
	rcRect.top=m_PointBenchmark.y;
	rcRect.right=m_PointBenchmark.x+CARD_WIDTH;

	//�����ǰ����û���ƣ����ô�������Ϊһ���Ƶĸߣ���
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

	//���������Ƶ�˳��
	SortHandCardByIndex();

	//�ƶ�����
	MoveWindow(&rcRect);

	Invalidate(FALSE);
	return;
}

void CHandCard::OnPaint()
{
	CPaintDC dc(this); // device context for painting
 
	int nXExcursion=0;
	
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�ڴ滺����
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	BufferDC.SelectObject(&BufferImage);

	int nXScreenPos=0,nYScreenPos=0;
	
	//ֻҪ���Ƹ���Ϊ0��������ʾ������ɫ
	if(m_cbCardCount == 0)
	{
		//��һ���ķ�������
		BufferDC.FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),RGB(15,111,119));

		//���ķ��εı߿�
		CPen newPen,*oldPen;
		newPen.CreatePen(PS_SOLID,2,RGB(60, 60, 60));
		oldPen=BufferDC.SelectObject(&newPen);
		BufferDC.Rectangle(0,0,rcClient.Width(),rcClient.Height());			//��������߿�
		BufferDC.SelectObject(oldPen);

		//���ķ����е�������
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
	//��Ӧ��С�ı���
	if(m_cbCardCount>0)
	{
		BufferDC.FillSolidRect(rcClient,RGB(24,60,104));
	}
	

	BYTE cbCardData =0;
	//�滭�˿�
	if(m_bDisplayItem==true)
	{
		for (WORD i=0;i<m_cbCardCount;i++)
		{
			//�滭�˿�
			cbCardData=m_CardItemArray[i].cbCardData;
			if(cbCardData!=0)
			{
				//��ʾ����������
				if(((cbCardData&MASK_COLOR)>>4)!=1)
				{
					g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,cbCardData,nXScreenPos,nYScreenPos);
				}
				//��ʾ��������
				else
				{
					g_CardResource.m_ImageUserBottom1.DrawCardItem(&BufferDC,cbCardData,nXScreenPos,nYScreenPos);
				}
				nYScreenPos += m_CardDistance;
			}
		}
	}
	
	//�滭�߿�
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

	//������Դ
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

	//��������
	CRect rcItem(0,0,rcClient.Width(),rcClient.Height());
	if (rcItem.PtInRect(MousePoint))
	{
		cbHoverItem=GetCurrentCardByPoint(MousePoint);
	}

	//��������
	if (m_cbHoverItem!=cbHoverItem)
	{
		m_cbHoverItem=cbHoverItem;
		Invalidate(FALSE);
	}

	//�����ж�
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
	////��ǰ����ҵ�����Ϊ0������λ��,�Ӹ�λ�ÿ�ʼ������������Ƶ�ǰ��
	for(BYTE i=0; i<9; i++)
	{
		if (m_CardItemArray[i].cbCardData != 0)
		{
			cbCardData[index++] = m_CardItemArray[i].cbCardData;
		}
	}

	//����������
	for (BYTE i=0; i<9; i++)
	{
		m_CardItemArray[i].cbCardData = cbCardData[i];	
	}

	return true;
}

//�Ҽ���������
void CHandCard::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bCanOutCard == true)
	{
		//fe11-30�������������ݳ�ʼΪ��
		BYTE cbHoverCard=0;
		if (m_cbHoverItem!=INVALID_ITEM)//˵���Ѿ�������˿�
		{
			cbHoverCard=m_CardItemArray[m_cbHoverItem].cbCardData;//����������е���
			m_CardItemArray[m_cbHoverItem].cbCardData = 0;
			m_CardItemArray[m_cbHoverItem].bShoot = false;
			m_cbCardCount--;
			
			//������ǰ�ƶ�
			SortHandCard();

			//�����ؼ�
			RectifyControl();
		}

		//��ȡ�˿�
		if (cbHoverCard!=0) 
		{
			AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);
			return ;
		}
	}

	CWnd::OnRButtonDown(nFlags, point);
}

//���°������������Ƶ�˳��
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

//���Ӵ��ڴ��ڻ״̬ʱ���丸�����ղ��������Ϣ���ʽ���ת����ȥ
void CHandCard::OnMouseMove(UINT nFlags, CPoint point)
{
	m_cbHoverItem = GetCurrentCardByPoint(point);

	ClientToScreen(&point);
	::SendMessage(GetParent()->GetSafeHwnd(),IDM_MOUSE_MOVE,point.x,point.y);

	CWnd::OnMouseMove(nFlags, point);
}
