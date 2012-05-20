#include "StdAfx.h"
#include "CardControl.h"
#include ".\cardcontrol.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardListImage::CCardListImage()
{
	//λ�ñ���
	m_nItemWidth=0;
	m_nItemHeight=0;

	return;
}

//��������
CCardListImage::~CCardListImage()
{
}

//������Դ
bool CCardListImage::LoadResource(UINT uResourceID, int nItemWidth, int nItemHeight)
{
	//������Դ
	m_CardListImage.LoadFromResource(AfxGetInstanceHandle(),uResourceID);

	//���ñ���
	m_nItemWidth=nItemWidth;
	m_nItemHeight=nItemHeight;

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
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor*10+cbValue);
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
	//�û��˿�
	m_ImageUserTop.LoadResource(IDB_CARD_TOP,CARD_WIDTH,CARD_HEIGHT);
	m_ImageUserLeft.LoadResource(IDB_CARD_LEFT,CARD_HEIGHT,CARD_WIDTH);
	m_ImageUserRight.LoadResource(IDB_CARD_RIGHT,CARD_HEIGHT,CARD_WIDTH);
	m_ImageUserBottom.LoadResource(IDB_CARD_BOTTOM,CARD_WIDTH,CARD_HEIGHT);

	//�����˿�
	m_ImageTableTop.LoadResource(IDB_CARD_TABLE_TOP,CARD_TABLE_WIDTH,CARD_TABLE_HEIGHT);
	m_ImageTableLeft.LoadResource(IDB_CARD_TABLE_LEFT,CARD_TABLE_HEIGHT,CARD_TABLE_WIDTH);
	m_ImageTableRight.LoadResource(IDB_CARD_TABLE_RIGHT,CARD_TABLE_HEIGHT,CARD_TABLE_WIDTH);
	m_ImageTableBottom.LoadResource(IDB_CARD_TABLE_BOTTOM,CARD_TABLE_WIDTH,CARD_TABLE_HEIGHT);

	return true;
}

//������Դ
bool CCardResource::DestroyResource()
{
	//�û��˿�
	m_ImageUserTop.DestroyResource();
	m_ImageUserLeft.DestroyResource();
	m_ImageUserRight.DestroyResource();
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
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x-CARD_TABLE_HEIGHT;
				int nYPos=m_ControlPoint.y+i*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_South:	//����
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nYPos=m_ControlPoint.y-CARD_TABLE_HEIGHT;
				int nXPos=m_ControlPoint.x-(i+1)*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_West:	//����
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y-(i+1)*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_North:	//����
		{
			//�滭�˿�
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
CUserCard::CUserCard()
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
CUserCard::~CUserCard()
{
}

//�滭�˿�
void CUserCard::DrawCardControl(CDC * pDC)
{
	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x-CARD_TABLE_HEIGHT;
				int nYPos=m_ControlPoint.y-(i+1)*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos);
			}

			break;
		}
	case Direction_South:	//����
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nYPos=m_ControlPoint.y-CARD_TABLE_HEIGHT;
				int nXPos=m_ControlPoint.x+i*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_West:	//����
		{
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=m_ControlPoint.x;
				int nYPos=m_ControlPoint.y+i*CARD_TABLE_WIDTH;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos);
			}

			break;
		}
	case Direction_North:	//����
		{
			//�滭�˿�
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

//�����˿�
bool CUserCard::SetCardData(WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//�����˿�
	m_wCardCount=wCardCount;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	return true;
}

//�����˿�
bool CUserCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
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
CWeaveCard::CWeaveCard()
{
	//״̬����
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_South;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(&m_cbCardData,sizeof(m_cbCardData));

	m_bDisplayStyle =false;

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

	//�滭�˿�
	switch (m_CardDirection)
	{
	case Direction_East:	//����
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
	case Direction_South:	//����
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
	case Direction_West:	//����
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
	case Direction_North:	//����
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

//�����˿�
bool CWeaveCard::SetCardData(const tagWeaveItem & WeaveItem)
{
	//�����˿�
	switch (WeaveItem.cbWeaveKind)
	{
	case ACK_NULL:	//��
		{
			//�����˿�
			m_wCardCount=0;
			ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

			return true;
		}
	case ACK_TI:	//��
		{
			//�����˿�
			m_wCardCount=4;
			m_cbCardData[0]=WeaveItem.cbCardList[0];
			ZeroMemory(&m_cbCardData[1],sizeof(BYTE)*3);

			return true;
		}
	case ACK_PAO:	//��
		{
			//�����˿�
			m_wCardCount=4;
			CopyMemory(m_cbCardData,WeaveItem.cbCardList,sizeof(BYTE)*4);

			return true;
		}
	case ACK_WEI:	//��
		{
			//�����˿�
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
	case ACK_PENG:	//��
		{
			//�����˿�
			m_wCardCount=3;
			CopyMemory(m_cbCardData,WeaveItem.cbCardList,sizeof(BYTE)*3);

			return true;
		}
	case ACK_CHI:	//��
		{
			//�����˿�
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

// ��ʾ��ʽ

void CWeaveCard::SetDisplayStyle(bool bDisplayStyle)
{
	if(m_bDisplayStyle!=bDisplayStyle)m_bDisplayStyle=bDisplayStyle;
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
	ZeroMemory(&m_CardItemArray,sizeof(m_CardItemArray));

	//��������
	m_ControlPoint.SetPoint(0,0);
	m_ControlSize.cy=CARD_HEIGHT+POS_SHOOT;
	m_ControlSize.cx=CountArray(m_CardItemArray)*CARD_WIDTH;

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
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wMaxCount)
{
	//Ч����Ŀ
	ASSERT(wMaxCount>=m_wCardCount);
	if (wMaxCount<m_wCardCount) return 0;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++) cbCardData[i]=m_CardItemArray[i].cbCardData;

	return m_wCardCount;
}

//��ȡ�˿�
BYTE CCardControl::GetHoverCard()
{
	//��ȡ�˿�
	if (m_wHoverItem==INVALID_ITEM) return 0;
	return m_CardItemArray[m_wHoverItem].cbCardData;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//�����˿�
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].bEnable=true;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//�����˿�
bool CCardControl::SwitchCardItem(WORD wSourceItem, WORD wTargerItem)
{
	//Ч�����
	if (wSourceItem==wTargerItem) return false;
	if ((wSourceItem>=m_wCardCount)||(wTargerItem>=m_wCardCount)) return false;

	//�����˿�
	tagCardItem CardItem=m_CardItemArray[wSourceItem];

	//�ƶ��˿�
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

	//����Ŀ��
	m_wHoverItem=wTargerItem;
	m_CardItemArray[wTargerItem]=CardItem;

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
		m_CardItemArray[i].bEnable=CardItemArray[i].bEnable;
		m_CardItemArray[i].cbCardData=CardItemArray[i].cbCardData;
	}

	return true;
}

//�滭�˿�
void CCardControl::DrawCardControl(CDC * pDC)
{
	//ƫ����ֵ
	int nExcursion=(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH/2;

	//�滭�˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//����λ��
		int nXScreenPos=m_ControlPoint.x+CARD_WIDTH*i+nExcursion;
		int nYScreenPos=m_ControlPoint.y+(((m_CardItemArray[i].bShoot==false)&&(m_wHoverItem!=i))?POS_SHOOT:0);

		//�滭�˿�
		BYTE cbCardData=(m_bDisplayItem==true)?m_CardItemArray[i].cbCardData:0;
		g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos);
	}

	return;
}

//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//ƫ����ֵ
	int nExcursion=(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH/2;

	//��׼λ��
	int nYPos=MousePoint.y-m_ControlPoint.y;
	int nXPos=MousePoint.x-m_ControlPoint.x-nExcursion;

	//��Χ�ж�
	if ((nXPos<0)||(nXPos>m_ControlSize.cx)) return INVALID_ITEM;
	if ((nYPos<POS_SHOOT)||(nYPos>m_ControlSize.cy)) return INVALID_ITEM;

	//��������
	WORD wIndex=nXPos/CARD_WIDTH;
	return (wIndex<m_wCardCount)?wIndex:INVALID_ITEM;
}

//�����Ϣ
bool CCardControl::OnEventSetCursor(CPoint Point, bool & bRePaint)
{
	//��ȡ����
	WORD wHoverItem=SwitchCardPoint(Point);
	bool bShoot=((wHoverItem!=INVALID_ITEM)&&(m_CardItemArray[wHoverItem].bEnable==true));

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
	}

	//���ù��
	if ((wHoverItem!=INVALID_ITEM)&&(m_bPositively==true))
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
	}

	return (wHoverItem!=INVALID_ITEM);
}

//////////////////////////////////////////////////////////////////////////

//��������
CCardResource						g_CardResource;						//�˿���Դ


