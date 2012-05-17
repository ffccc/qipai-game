#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��������
#define POS_SHOOT					5									//��������
#define POS_SPACE					8									//�ָ����
#define ITEM_COUNT					28									//������Ŀ
#define INVALID_ITEM				0xFFFF								//��Ч����

//�˿˴�С
#define CARD_WIDTH					45									//�˿˿��
#define CARD_HEIGHT					72									//�˿˸߶�

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
bool CCardListImage::LoadResource(LPCTSTR uResourceID, int nViewWidth, int nViewHeight)
{
	//������Դ
	//m_CardListImage.LoadFromResource(AfxGetInstanceHandle(),uResourceID);
	m_CardListImage.LoadImage(AfxGetInstanceHandle(),TEXT(uResourceID));

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
	//m_CardListImage.Destroy();

	return true;
}

//��ȡλ��
int CCardListImage::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) 
		return 0;

	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}

//�滭�˿�
bool CCardListImage::DrawCardItem(CDC * pDestDC, BYTE cbCardData, int xDest, int yDest)
{
	//Ч��״̬
	ASSERT(m_CardListImage.IsNull()==false);
	ASSERT((m_nItemWidth!=0)&&(m_nItemHeight!=0));

	//�滭����
	if(cbCardData<=0x37)
	{
		int nImageXPos=GetImageIndex(cbCardData)*m_nItemWidth;
		//m_CardListImage.AlphaDrawImage(pDestDC,xDest,yDest,m_nItemWidth,m_nItemHeight,nImageXPos,0,RGB(255,0,255));
		m_CardListImage.DrawImage(pDestDC,
									xDest,	//X����
									yDest,	//Y����
									m_CardListImage.GetWidth()/28,				//��
									m_CardListImage.GetHeight(),				//��
									nImageXPos,//��ʼλ��X
									0);
	}
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
	m_ImageUserTop.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_USER_TOP"));
	m_ImageUserBottom.LoadResource("CARD_USER_BOTTOM",CARD_WIDTH,CARD_HEIGHT);

	m_ImageUserLeft.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_USER_LEFT"));
	m_ImageUserRight.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_USER_RIGHT"));

	//�����˿�
	m_ImageTableTop.LoadResource("CARD_TABLE_TOP",31,64);
	m_ImageTableLeft.LoadResource("CARD_TABLE_LEFT",53,35);
	m_ImageTableRight.LoadResource("CARD_TABLE_RIGHT",53,35);
	m_ImageTableBottom.LoadResource("CARD_TABLE_BOTTOM",34,70);

	//���
	m_ImageHeapDoubleLV.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_HEAP_DOUBLE_LV"));
	m_ImageHeapSingleLV.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_HEAP_SINGLE_LV"));

	//�ұ�
	m_ImageHeapDoubleRV.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_HEAP_DOUBLE_RV"));
	m_ImageHeapSingleRV.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_HEAP_SINGLE_RV"));
	
	//�����
	m_ImageHeapDoubleTV.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_HEAP_DOUBLE_TV"));
	m_ImageHeapSingleTV.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_HEAP_SINGLE_TV"));

	//�����
	m_ImageHeapDoubleBV.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_HEAP_DOUBLE_BV"));
	m_ImageHeapSingleBV.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_HEAP_SINGLE_BV"));
	
	//m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("ARROW"));

	return true;
}

//������Դ
bool CCardResource::DestroyResource()
{
	//�û��˿�
	m_ImageUserBottom.DestroyResource();

	//�����˿�
	m_ImageTableTop.DestroyResource();
	m_ImageTableLeft.DestroyResource();
	m_ImageTableRight.DestroyResource();
	m_ImageTableBottom.DestroyResource();

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

//�滭�˿�
void CHeapCard::DrawCardControl(CDC * pDC)
{
	int nOffset=3;	//ƫ����

	switch (m_CardDirection)
	{
	case Direction_East:	//����   ��		��
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				WORD wHeapIndex=m_wMinusHeadCount/2;
				WORD wDoubleHeap=(m_wMinusHeadCount+1)/2;
				WORD wDoubleLast=(m_wFullCount-m_wMinusLastCount)/2;
				WORD wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					nXPos=m_ControlPoint.x + wHeapIndex*nOffset;
					nYPos=m_ControlPoint.y+wHeapIndex*16+13;

					g_CardResource.m_ImageHeapSingleRV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapSingleRV.GetWidth(),				//��
									g_CardResource.m_ImageHeapSingleRV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}

				//�м��˿�
				for (WORD i=wDoubleHeap;i<wFinallyIndex;i++)
				{
					nXPos=m_ControlPoint.x + i*nOffset;
					nYPos=m_ControlPoint.y+i*16;

					g_CardResource.m_ImageHeapDoubleRV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapDoubleRV.GetWidth(),				//��
									g_CardResource.m_ImageHeapDoubleRV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					nXPos=m_ControlPoint.x + wFinallyIndex*nOffset;
					nYPos=m_ControlPoint.y+wFinallyIndex*16+13;

					g_CardResource.m_ImageHeapSingleRV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapSingleRV.GetWidth(),				//��
									g_CardResource.m_ImageHeapSingleRV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}
			}
			
			break;
		}
	case Direction_South:	//����	��
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				WORD wHeapIndex=m_wMinusLastCount/2;
				WORD wDoubleHeap=(m_wMinusLastCount+1)/2;
				WORD wDoubleLast=(m_wFullCount-m_wMinusHeadCount)/2;
				WORD wFinallyIndex=(m_wFullCount-m_wMinusHeadCount)/2;

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					nYPos=m_ControlPoint.y+16;
					nXPos=m_ControlPoint.x+wHeapIndex*34; //42
					g_CardResource.m_ImageHeapSingleBV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapSingleBV.GetWidth(),				//��
									g_CardResource.m_ImageHeapSingleBV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}

				//�м��˿�
				for (WORD i=wDoubleHeap;i<wFinallyIndex;i++)
				{
					nYPos=m_ControlPoint.y;
					nXPos=m_ControlPoint.x+i*34;
					
					g_CardResource.m_ImageHeapDoubleBV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapDoubleBV.GetWidth(),				//��
									g_CardResource.m_ImageHeapDoubleBV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					nYPos=m_ControlPoint.y+16;
					nXPos=m_ControlPoint.x+wFinallyIndex*34;

					g_CardResource.m_ImageHeapSingleBV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapSingleBV.GetWidth(),				//��
									g_CardResource.m_ImageHeapSingleBV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}
			}
			
			break;
		}
	case Direction_West:	//����		��
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				WORD wHeapIndex=m_wMinusLastCount/2;
				WORD wDoubleHeap=(m_wMinusLastCount+1)/2;
				WORD wDoubleLast=(m_wFullCount-m_wMinusHeadCount)/2;
				WORD wFinallyIndex=(m_wFullCount-m_wMinusHeadCount)/2;

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					nXPos=m_ControlPoint.x - wHeapIndex*nOffset;
					nYPos=m_ControlPoint.y+wHeapIndex*16+13;

					g_CardResource.m_ImageHeapSingleLV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapSingleLV.GetWidth(),				//��
									g_CardResource.m_ImageHeapSingleLV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}

				//�м��˿�
				for (WORD i=wDoubleHeap;i<wFinallyIndex;i++)
				{
					nXPos=m_ControlPoint.x - i*nOffset;
					nYPos=m_ControlPoint.y+i*16;
				
					g_CardResource.m_ImageHeapDoubleLV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapDoubleLV.GetWidth(),				//��
									g_CardResource.m_ImageHeapDoubleLV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					nXPos=m_ControlPoint.x - wFinallyIndex*nOffset;
					nYPos=m_ControlPoint.y+wFinallyIndex*16+13;
				
					g_CardResource.m_ImageHeapSingleLV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapSingleLV.GetWidth(),				//��
									g_CardResource.m_ImageHeapSingleLV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}
			}
			
			break;
		}
	case Direction_North:	//����		��
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				WORD wHeapIndex=m_wMinusHeadCount/2;
				WORD wDoubleHeap=(m_wMinusHeadCount+1)/2;
				WORD wDoubleLast=(m_wFullCount-m_wMinusLastCount)/2;
				WORD wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					nYPos=m_ControlPoint.y+15;
					nXPos=m_ControlPoint.x+wHeapIndex*31;

					g_CardResource.m_ImageHeapSingleTV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapSingleTV.GetWidth(),				//��
									g_CardResource.m_ImageHeapSingleTV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}

				//�м��˿�
				for (WORD i=wDoubleHeap;i<wFinallyIndex;i++)
				{
					nYPos=m_ControlPoint.y;
					nXPos=m_ControlPoint.x+i*31;

					g_CardResource.m_ImageHeapDoubleTV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapDoubleTV.GetWidth(),				//��
									g_CardResource.m_ImageHeapDoubleTV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					nYPos=m_ControlPoint.y+15;
					nXPos=m_ControlPoint.x+wFinallyIndex*31;

					g_CardResource.m_ImageHeapSingleTV.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageHeapSingleTV.GetWidth(),				//��
									g_CardResource.m_ImageHeapSingleTV.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}
			}
			
			break;
		}
	}

	return;
}

//�����˿�
bool CHeapCard::SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount)
{
	//���ñ���
	m_wFullCount=wFullCount;
	m_wMinusHeadCount=wMinusHeadCount;
	m_wMinusLastCount=wMinusLastCount;

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CWeaveCard::CWeaveCard()
{
	//״̬����
	m_bDisplayItem=false;
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_South;
	m_cbDirectionCardPos = 1;
	m_bGameEnd = false;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(&m_cbCardData,sizeof(m_cbCardData));

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
	if (m_wCardCount==0) 
		return;

	//��������
	int nXScreenPos=0,nYScreenPos=0;
	int nItemWidth=0,nItemHeight=0,nItemWidthEx=0,nItemHeightEx=0;

	//�滭�˿�
	switch (m_CardDirection)
	{
	case Direction_East:	//����   ��
		{
			//��������
			nItemWidth=g_CardResource.m_ImageTableRight.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableRight.GetViewHeight()-18;
			nItemWidthEx=g_CardResource.m_ImageTableTop.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableTop.GetViewHeight()-10;

			nYScreenPos = m_ControlPoint.y;			
			//if(m_cbDirectionCardPos == 2)
			//{
			//	nXScreenPos=m_ControlPoint.x+nItemWidthEx;
			//	g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			//	nYScreenPos += nItemHeightEx;
			//}
			//else
			{
				nXScreenPos=m_ControlPoint.x+nItemWidthEx*2-nItemWidth;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
				nYScreenPos += nItemHeight;
			}


			//�ڶ��˿�
			nXScreenPos=m_ControlPoint.x+nItemWidthEx*2-nItemWidth + 3;
			g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			//�����˿�
			if (m_wCardCount==4)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos-17);
			}
			nYScreenPos += nItemHeight;

			//��һ�˿�
			//if(m_cbDirectionCardPos == 0)
			//{
			//	nXScreenPos=m_ControlPoint.x+nItemWidthEx + 6;
			//	g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			//}
			//else
			{
				nXScreenPos=m_ControlPoint.x+nItemWidthEx*2-nItemWidth +6;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);

			}

			break;
		}
	case Direction_South:	//����	��
		{
			//��������
			nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableBottom.GetViewHeight();
			nItemWidthEx=g_CardResource.m_ImageTableRight.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableRight.GetViewHeight();

			//�����˿�
			nXScreenPos = m_ControlPoint.x;
			//if(m_cbDirectionCardPos == 2)
			//{
			//	nXScreenPos -= nItemWidthEx;
			//	nYScreenPos=m_ControlPoint.y+nItemHeightEx;
			//	g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			//}
			//else
			{
				nXScreenPos -= nItemWidth;
				nYScreenPos=m_ControlPoint.y+nItemHeightEx*2-nItemHeight;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			}			

			//�ڶ��˿�
			nXScreenPos -= nItemWidth;
			nYScreenPos=m_ControlPoint.y+nItemHeightEx*2-nItemHeight;
			g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			//�����˿�
			if (m_wCardCount==4)
			{
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos-18);
			}


			//��һ�˿�
			//if(m_cbDirectionCardPos == 0)
			//{
			//	nXScreenPos -= nItemWidthEx;
			//	nYScreenPos=m_ControlPoint.y+nItemHeightEx;
			//	g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			//}
			//else
			{
				nXScreenPos -= nItemWidth;
				nYScreenPos=m_ControlPoint.y+nItemHeightEx*2-nItemHeight;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			}


			break;
		}
	case Direction_West:	//����	��
		{
			//��������
			nItemWidth=g_CardResource.m_ImageTableLeft.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableLeft.GetViewHeight()-18;
			nItemWidthEx=g_CardResource.m_ImageTableBottom.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableBottom.GetViewHeight()-10;

			//if(m_cbDirectionCardPos != 1)
			{
				//��һ�˿�
				nYScreenPos=m_ControlPoint.y-nItemHeightEx-nItemHeight*2;
				//if(m_cbDirectionCardPos == 0)
				//{
				//	nXScreenPos=m_ControlPoint.x-nItemWidthEx*2;
				//	g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
				//	nYScreenPos += nItemHeightEx;
				//}
				//else
				{
					nXScreenPos=m_ControlPoint.x-nItemWidthEx*2;
					g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
					nYScreenPos += nItemHeight;
				}
				//�ڶ��˿�
				nXScreenPos=m_ControlPoint.x-nItemWidthEx*2 - 3;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
				//�����˿�
				if (m_wCardCount==4)
				{
					g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos-17);
				}
				nYScreenPos += nItemHeight;

				//�����˿�
				//if(m_cbDirectionCardPos == 2)
				//{
				//	nXScreenPos=m_ControlPoint.x-nItemWidthEx*2 - 6;
				//	g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
				//}
				//else
				{
					nXScreenPos=m_ControlPoint.x-nItemWidthEx*2 - 6;
					g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
				}

			}
			//else
			//{			
			//	//��һ�˿�
			//	nYScreenPos = m_ControlPoint.y-nItemHeight*3;
			//	//nXScreenPos=m_ControlPoint.x-nItemWidth;
			//	nXScreenPos=m_ControlPoint.x-nItemWidthEx*2;
			//	g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			//	//�ڶ��˿�
			//	nYScreenPos += nItemHeight;
			//	g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);
			//	//�����˿�
			//	if (m_wCardCount==4)
			//	{
			//		g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos-12);
			//	}
			//	//�����˿�
			//	nYScreenPos += nItemHeight;
			//	g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			//}

			break;
		}
	case Direction_North:	//����	�ϡ�
		{
			//��������
			nItemWidth=g_CardResource.m_ImageTableTop.GetViewWidth();
			nItemHeight=g_CardResource.m_ImageTableTop.GetViewHeight();
			nItemWidthEx=g_CardResource.m_ImageTableLeft.GetViewWidth();
			nItemHeightEx=g_CardResource.m_ImageTableLeft.GetViewHeight();

			//�����˿�
			nXScreenPos = m_ControlPoint.x;
			//if(m_cbDirectionCardPos == 2)
			//{
			//	nYScreenPos = m_ControlPoint.y-nItemHeight;
			//	g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
			//	nXScreenPos += nItemWidthEx;
			//}
			//else
			{
				nYScreenPos = m_ControlPoint.y-nItemHeight;
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos);
				nXScreenPos += nItemWidth;
			}

			//�ڶ��˿�
			nYScreenPos = m_ControlPoint.y-nItemHeight;
			g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos);			
			//�����˿�
			if (m_wCardCount==4)
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos-17);
			}
			nXScreenPos += nItemWidth;


			//��һ�˿�
			//if(m_cbDirectionCardPos == 0)
			//{
			//	nYScreenPos=m_ControlPoint.y-nItemHeight;			
			//	g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
			//}
			//else
			{
				nYScreenPos=m_ControlPoint.y-nItemHeight;		
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos);
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
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(BYTE)*wCardCount);

	return true;
}

//��ȡ�˿�
BYTE CWeaveCard::GetCardData(WORD wIndex)
{
	ASSERT(wIndex<CountArray(m_cbCardData));
	return ((m_bDisplayItem==true)||(wIndex==3&&m_bGameEnd))?m_cbCardData[wIndex]:0;
}
//�滭�˿�
void CWeaveCard::DrawCardControl(CDC * pDC, int nXPos, int nYPos)
{
	//����λ��
	SetControlPoint(nXPos,nYPos);

	//��ʾ�ж�
	if (m_wCardCount==0) 
		return;

	//��������
	int nXScreenPos=0,nYScreenPos=0;
	int nItemWidth=0,nItemHeight=0,nItemWidthEx=0,nItemHeightEx=0;

	//�滭�˿�
	switch (m_CardDirection)
	{
	case Direction_East:	//����   ��
		{

			//�滭�˿�
			for (WORD i=0;i<3;i++)
			{
				int nXScreenPos=m_ControlPoint.x-g_CardResource.m_ImageTableRight.GetViewWidth();
				int nYScreenPos=m_ControlPoint.y+i*g_CardResource.m_ImageTableRight.GetViewHeight();
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(2-i),nXScreenPos,nYScreenPos);
			}

			//�����˿�
			if (m_wCardCount==4)
			{
				int nXScreenPos=m_ControlPoint.x-g_CardResource.m_ImageTableRight.GetViewWidth();
				int nYScreenPos=m_ControlPoint.y-5+g_CardResource.m_ImageTableRight.GetViewHeight();
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
			}

			break;
	}
	case Direction_South:	//����
		{
			//�滭�˿�
			for (WORD i=0;i<3;i++)
			{
				int nXScreenPos=m_ControlPoint.x+i*g_CardResource.m_ImageTableBottom.GetViewWidth();
				int nYScreenPos=m_ControlPoint.y-g_CardResource.m_ImageTableBottom.GetViewHeight()-5;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(i),nXScreenPos,nYScreenPos);
			}

			//�����˿�
			if (m_wCardCount==4)
			{
				int nXScreenPos=m_ControlPoint.x+g_CardResource.m_ImageTableBottom.GetViewWidth();
				int nYScreenPos=m_ControlPoint.y-g_CardResource.m_ImageTableBottom.GetViewHeight()-5*2;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
			}
			break;

		}
	case Direction_West:	//����
		{
			//�滭�˿�
			for (WORD i=0;i<3;i++)
			{
				int nXScreenPos=m_ControlPoint.x;
				int nYScreenPos=m_ControlPoint.y+i*g_CardResource.m_ImageTableLeft.GetViewHeight();
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(i),nXScreenPos,nYScreenPos);
			}

			//�����˿�
			if (m_wCardCount==4)
			{
				int nXScreenPos=m_ControlPoint.x;
				int nYScreenPos=m_ControlPoint.y+g_CardResource.m_ImageTableLeft.GetViewHeight()-5;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
			}

			break;

		}
	case Direction_North:	//����
		{
			//�滭�˿�
			for (WORD i=0;i<3;i++)
			{
				int nYScreenPos=m_ControlPoint.y;
				int nXScreenPos=m_ControlPoint.x-(i+1)*g_CardResource.m_ImageTableTop.GetViewWidth();
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(2-i),nXScreenPos,nYScreenPos);
			}

			//�����˿�
			if (m_wCardCount==4)
			{
				int nYScreenPos=m_ControlPoint.y-5;
				int nXScreenPos=m_ControlPoint.x-2*g_CardResource.m_ImageTableTop.GetViewWidth();
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos);
			}

			break;

		}
	}

	return;
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
	int nOffset=3;	//ƫ����

	switch (m_CardDirection)
	{
	case Direction_East:	//����   ��	�ұ�
		{
			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x - 5;
				int nYPos=m_ControlPoint.y + 15;
				//g_CardResource.m_ImageUserRight.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
				g_CardResource.m_ImageUserRight.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageUserRight.GetWidth(),				//��
									g_CardResource.m_ImageUserRight.GetHeight(),				//��
									0,//��ʼλ��X
									0);
			}

			//�����˿�
			if (m_wCardCount>0)
			{
				int nXPos=0,nYPos=0;
				for (WORD i=0;i<m_wCardCount;i++)
				{
					nXPos=m_ControlPoint.x + i*nOffset;
					nYPos=m_ControlPoint.y+i*16+40;
					//g_CardResource.m_ImageUserRight.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
					g_CardResource.m_ImageUserRight.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageUserRight.GetWidth(),				//��
									g_CardResource.m_ImageUserRight.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}
			}

			break;
		}
	case Direction_West:	//����	��
		{
			//�����˿�
			if (m_wCardCount>0)
			{
				int nXPos=0,nYPos=0;
				for (WORD i=0;i<m_wCardCount;i++)
				{
					nXPos=m_ControlPoint.x + (m_wCardCount-i-1)*nOffset;
					nYPos=m_ControlPoint.y-(m_wCardCount-i-1)*16-92;
					//g_CardResource.m_ImageUserLeft.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
					g_CardResource.m_ImageUserLeft.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageUserLeft.GetWidth(),				//��
									g_CardResource.m_ImageUserLeft.GetHeight(),				//��
									0,//��ʼλ��X
									0);
				}
			}

			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x - 5;
				int nYPos=m_ControlPoint.y-49 - 18;
				//g_CardResource.m_ImageUserLeft.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
				g_CardResource.m_ImageUserLeft.DrawImage(pDC,
									nXPos,	//X����
									nYPos,	//Y����
									g_CardResource.m_ImageUserLeft.GetWidth(),				//��
									g_CardResource.m_ImageUserLeft.GetHeight(),				//��
									0,//��ʼλ��X
									0);
			}

			break;
		}
	case Direction_North:	//����
		{
			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y;
				g_CardResource.m_ImageUserTop.DrawImage(pDC,nXPos,nYPos);
			}

			//�����˿�
			if (m_wCardCount>0)
			{
				int nXPos=0,nYPos=0;
				for (WORD i=0;i<m_wCardCount;i++)
				{
					nYPos=m_ControlPoint.y;
					nXPos=m_ControlPoint.x+i*31+40;
					g_CardResource.m_ImageUserTop.DrawImage(pDC,nXPos,nYPos);
				}
			}

			break;
		}
	}

	return;
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
	int nOffset=3;	//ƫ����

	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����   ��
		{
			//�滭�˿�
			
			for (WORD i=0;i<m_wCardCount;i++)
			{
				if (i>=8) continue;
				int nXPos=m_ControlPoint.x+(i/8)*50 + (i%8)*nOffset;
				int nYPos=m_ControlPoint.y+(i%8)*18;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			for (WORD i=0;i<m_wCardCount;i++)
			{
				if (i<8) continue; 
				int nXPos=m_ControlPoint.x+(i/8)*50 + (i%8)*nOffset;
				int nYPos=m_ControlPoint.y+(i%8)*18;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}


			break;
		}
	case Direction_South:	//����		��
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x-(i%8)*34;
				int nYPos=m_ControlPoint.y+(i/8)*35;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_West:	//����	��
		{
			//�滭�˿�

			for (WORD i=0;i<m_wCardCount;i++)
			{
				if ((m_wCardCount-i-1)>=8) continue;
				int nXPos=m_ControlPoint.x-((m_wCardCount-1-i)/8)*50 + ((m_wCardCount-1-i)%8)*nOffset;
				int nYPos=m_ControlPoint.y-((m_wCardCount-1-i)%8)*18;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			}

			for (WORD i=0;i<m_wCardCount;i++)
			{
				if ((m_wCardCount-i-1)<8) continue; 
				int nXPos=m_ControlPoint.x-((m_wCardCount-1-i)/8)*50 + ((m_wCardCount-1-i)%8)*nOffset;
				int nYPos=m_ControlPoint.y-((m_wCardCount-1-i)%8)*18;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			}


			//for (WORD i=0;i<m_wCardCount;i++)
			//{
			//	int nXPos=m_ControlPoint.x-((m_wCardCount-1-i)/8)*50 + ((m_wCardCount-1-i)%8)*nOffset;
			//	int nYPos=m_ControlPoint.y-((m_wCardCount-1-i)%8)*20;
			//	g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			//}

			break;
		}
	case Direction_North:	//����		��
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x+((m_wCardCount-1-i)%8)*30;
				int nYPos=m_ControlPoint.y-((m_wCardCount-1-i)/8)*35-14;
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			}

			break;
		}
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
	if (wCardCount>CountArray(m_cbCardData))
		return false;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}
//��ȡλ��
CPoint CDiscardCard::GetLastCardPosition()
{
	switch (m_CardDirection)
	{
	case Direction_East:	//����   ��
		{
			//��������
			int nCellWidth=g_CardResource.m_ImageTableRight.GetViewWidth();
			int nCellHeight=g_CardResource.m_ImageTableRight.GetViewHeight();
			int nXPos=m_ControlPoint.x+((m_wCardCount-1)/8)*50+18 + ((m_wCardCount-1)%8)*2 ;
			int nYPos=m_ControlPoint.y+((m_wCardCount-1)%8)*18+11;


			return CPoint(nXPos,nYPos);
		}
	case Direction_South:	//����	��
		{
			//��������
			int nCellWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
			int nCellHeight=g_CardResource.m_ImageTableBottom.GetViewHeight();
			int nXPos=m_ControlPoint.x-((m_wCardCount-1)%8)*34+14 + 2;
			int nYPos=m_ControlPoint.y+((m_wCardCount-1)/8)*35+16;

			return CPoint(nXPos,nYPos);

		}
	case Direction_West:	//����	��
		{
			//��������
			int nCellWidth=g_CardResource.m_ImageTableLeft.GetViewWidth();
			int nCellHeight=g_CardResource.m_ImageTableLeft.GetViewHeight();
			int nXPos=m_ControlPoint.x-((m_wCardCount-1)/8)*50+17 + ((m_wCardCount-1)%8)*2 + 20;
			int nYPos=m_ControlPoint.y-((m_wCardCount-1)%8)*18+10;
			return CPoint(nXPos,nYPos);

		}
	case Direction_North:	//����	��
		{
			//��������
			int nCellWidth=g_CardResource.m_ImageTableTop.GetViewWidth();
			int nCellHeight=g_CardResource.m_ImageTableTop.GetViewHeight();            
			int nXPos=m_ControlPoint.x+((m_wCardCount-1)%8)*30+14;
			int nYPos=m_ControlPoint.y+((-m_wCardCount+1)/8)*35+17 - 10;
			return CPoint(nXPos,nYPos);
		}
	}
	return CPoint(0,0);

}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableCard::CTableCard()
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
CTableCard::~CTableCard()
{
}

//�滭�˿�
void CTableCard::DrawCardControl(CDC * pDC)
{
	int nOffset=3;	//ƫ����

	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����   ��
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x-33 + i*nOffset;
				int nYPos=m_ControlPoint.y+i*17;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			}

			break;
		}
	case Direction_South:	//����	��
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nYPos=m_ControlPoint.y-36;
				int nXPos=m_ControlPoint.x-(m_wCardCount-i)*34;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_West:	//����	��
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x + (m_wCardCount-i)*nOffset;
				int nYPos=m_ControlPoint.y-(m_wCardCount-i)*17;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_North:	//����	��
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nYPos=m_ControlPoint.y;
				int nXPos=m_ControlPoint.x+i*31;
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			}

			break;
		}
	}

	return;
}

//�����˿�
bool CTableCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData))
		return false;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

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
	if (m_wHoverItem!=INVALID_ITEM)
	{
		if (m_wHoverItem==CountArray(m_CardItemArray))
			return m_CurrentCard.cbCardData;
		return m_CardItemArray[m_wHoverItem].cbCardData;
	}

	return 0;
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
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) 
		return false;

	//��ǰ�˿�
	m_CurrentCard.bShoot=false;
	m_CurrentCard.cbCardData=cbCurrentCard;

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
	if (wCardCount>CountArray(m_CardItemArray))
		return false;

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
		BYTE cbCardData=(m_bDisplayItem==true)?m_CardItemArray[i].cbCardData:0;
		g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
	}

	//��ǰ�˿�
	if (m_CurrentCard.cbCardData!=0)
	{
		//����λ��
		int nXScreenPos=m_ControlPoint.x+m_ControlSize.cx-CARD_WIDTH;
		int nYScreenPos=m_ControlPoint.y+(((m_CurrentCard.bShoot==false)&&(m_wHoverItem!=CountArray(m_CardItemArray)))?POS_SHOOT:0);

		//�滭�˿�
		BYTE cbCardData=(m_bDisplayItem==true)?m_CurrentCard.cbCardData:0;
		g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
	}

	return;
}

//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//��׼λ��
	int nXPos=MousePoint.x-m_ControlPoint.x;
	int nYPos=MousePoint.y-m_ControlPoint.y;

	//��Χ�ж�
	if ((nXPos<0)||(nXPos>m_ControlSize.cx)) 
		return INVALID_ITEM;
	if ((nYPos<POS_SHOOT)||(nYPos>m_ControlSize.cy)) 
		return INVALID_ITEM;

	//��������
	if (nXPos<CARD_WIDTH*CountArray(m_CardItemArray))
	{
		WORD wViewIndex=(WORD)(nXPos/CARD_WIDTH)+m_wCardCount;
		if (wViewIndex>=CountArray(m_CardItemArray))
			return wViewIndex-CountArray(m_CardItemArray);
		return INVALID_ITEM;
	}

	//��ǰ����
	if ((m_CurrentCard.cbCardData!=0)&&(nXPos>=(m_ControlSize.cx-CARD_WIDTH))) 
		return CountArray(m_CardItemArray);

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

//////////////////////////////////////////////////////////////////////////

//��������
CCardResource						g_CardResource;						//�˿���Դ

//////////////////////////////////////////////////////////////////////////
