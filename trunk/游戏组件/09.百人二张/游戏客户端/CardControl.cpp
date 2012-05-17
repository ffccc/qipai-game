#include "StdAfx.h"
#include "GameClient.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
CSkinImage					CCardControl::m_ImageCard;						//ͼƬ��Դ

//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bSmallMode=false;
	m_bHorizontal=true;
	m_bPositively=false;
	m_bDisplayItem=false;
	m_bShowCardControl=true;
	m_blGameEnd = false;
	m_blhideOneCard = false;

	//ѡ�����
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;
	m_dwCurrentIndex = INVALID_ITEM;

	//�˿�����
	m_wCardCount=0;

	//λ�ñ���
	m_YCollocateMode=enYTop;
	m_XCollocateMode=enXLeft;
	m_BenchmarkPos.SetPoint(0,0);
	m_MovePoint.SetPoint(0,0);

	//�������
	m_nXDistance=(m_bSmallMode==false)?DEF_X_DISTANCE:DEF_X_DISTANCE_SMALL;
	m_nYDistance=(m_bSmallMode==false)?DEF_Y_DISTANCE:DEF_Y_DISTANCE_SMALL;
	m_nShootDistance=(m_bSmallMode==false)?DEF_SHOOT_DISTANCE:DEF_SHOOT_DISTANCE_SMALL;

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	if (m_ImageCard.IsNull()) m_ImageCard.SetLoadInfo((m_bSmallMode==false)?IDB_GAME_CARD:IDB_GAME_CARD,hResInstance);

	m_ImageOpenCardHandle.SetLoadInfo(IDB_HAND,hResInstance);

	//��ȡ��С
	CImageHandle HandleCard(&m_ImageCard);
	m_CardSize.SetSize(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);

	return;
}

//��������
CCardControl::~CCardControl()
{
	if (m_ImageCard.IsNull()==false) m_ImageCard.Destroy();
}

//�����˿�
bool CCardControl::SetCardData(WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	m_CardItemArray.SetSize(wCardCount);
	//���ñ���
	m_wCardCount=(WORD)m_CardItemArray.GetCount();

	return true;
}

//����ģʽ
bool CCardControl::SetCardMode(bool bSmallMode)
{
	//����ģʽ
	if (m_bSmallMode!=bSmallMode)
	{
		//���ñ���
		m_bSmallMode=bSmallMode;

		//���ü��
		m_nXDistance=(m_bSmallMode==false)?DEF_X_DISTANCE:DEF_X_DISTANCE_SMALL;
		m_nYDistance=(m_bSmallMode==false)?DEF_Y_DISTANCE:DEF_Y_DISTANCE_SMALL;
		m_nShootDistance=(m_bSmallMode==false)?DEF_SHOOT_DISTANCE:DEF_SHOOT_DISTANCE_SMALL;

		//������Դ
		HINSTANCE hResInstance=AfxGetInstanceHandle();
		m_ImageCard.SetLoadInfo((m_bSmallMode==false)?IDB_GAME_CARD:IDB_GAME_CARD,hResInstance);

		//��ȡ��С
		CImageHandle HandleCard(&m_ImageCard);
		m_CardSize.SetSize(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);
	}

	return true;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	if(wCardCount==0)
	{
       m_CardItemArray.SetSize(0);
	}
	m_CardTempItemArray.SetSize(wCardCount);
	//Ч�����
	ASSERT(wCardCount<=CountArray(m_CardTempItemArray));
	if (wCardCount>CountArray(m_CardTempItemArray)) return false;

	//���ñ���
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�˿���Ŀ
	m_wCardCount=0;

	//�����˿�
	for (WORD i=0;i<wCardCount;i++)
	{
		m_CardTempItemArray[i].bShoot=false;
		m_CardTempItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//�����˿�
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//���ñ���
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�����˿�
	m_wCardCount=wCardCount;
	m_CardItemArray.SetSize(wCardCount);
	for (int i = 0;i<wCardCount;i++)
	{
		m_CardItemArray[i]= CardItemArray[i];
	}
	return true;
}

//�����˿�
bool CCardControl::SetShootIndex(WORD wStartIndex, WORD wEndIndex)
{
	//���ñ���
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		if ((i>=wStartIndex)&&(i<=wEndIndex))
		{
			m_CardItemArray[i].bShoot=true;
		}
	}

	return true;
}

//�����˿�
bool CCardControl::SetSelectIndex(WORD wStartIndex, WORD wEndIndex)
{
	//���ñ���
	m_wEndIndex=wEndIndex;
	m_wStartIndex=wStartIndex;

	return true;
}

//�����˿�
bool CCardControl::SetShootCard(const BYTE cbCardData[], WORD wCardCount)
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

//ɾ���˿�
bool CCardControl::RemoveShootItem()
{
	//���ñ���
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�����˿�
	tagCardItem	CardItemArray[MAX_CARD_COUNT];

	WORD wCardCount = (WORD)m_CardItemArray.GetCount();

	m_wCardCount = wCardCount;

	for (int i = 0;i<wCardCount;i++)
	{
		CardItemArray[i]= m_CardItemArray[i];
	}
	//ɾ���˿�
	WORD wRemoveCount=0;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		if (CardItemArray[i].bShoot==true)
		{
			wRemoveCount++;
			CardItemArray[i].cbCardData=0x00;
		}
	}

	//�����˿�
	if (wRemoveCount>0)
	{
		//�����˿�
		WORD wInsertCount=0;
		for (WORD i=0;i<m_wCardCount;i++)
		{
			if (CardItemArray[i].cbCardData!=0x00)
			{
				m_CardItemArray[wInsertCount++]=CardItemArray[i];
			}
		}

		//���ñ���
		m_wCardCount=m_wCardCount-wRemoveCount;
	}

	return true;
}

//��ȡ�˿�
tagCardItem * CCardControl::GetCardFromIndex(WORD wIndex)
{
	return (wIndex<m_wCardCount)?&m_CardItemArray[wIndex]:NULL;
}

//��ȡ�˿�
tagCardItem * CCardControl::GetCardFromPoint(CPoint & MousePoint)
{
	WORD wIndex=SwitchCardPoint(MousePoint);
	return (wIndex!=INVALID_ITEM)?&m_CardItemArray[wIndex]:NULL;
}

//��ȡ�˿�
WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wBufferCount)
{
	//Ч�����
	ASSERT(wBufferCount>=m_wCardCount);
	if (wBufferCount<m_wCardCount) return 0;

	//�����˿�
	for (WORD i=0;i<m_wCardCount;i++) cbCardData[i]=m_CardItemArray[i].cbCardData;

	return m_wCardCount;
}

//��ȡ�˿�
WORD CCardControl::GetShootCard(BYTE cbCardData[], WORD wBufferCount)
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
WORD CCardControl::GetCardData(tagCardItem CardItemArray[], WORD wBufferCount)
{
	//Ч�����
	ASSERT(wBufferCount>=m_wCardCount);
	if (wBufferCount<m_wCardCount) return 0;

	m_wCardCount = (WORD)m_CardItemArray.GetCount();
	for(int i = 0;i<m_wCardCount;i++)
	{
		CardItemArray[i] = m_CardItemArray[i];

	}
	return m_wCardCount;
}

//���þ���
VOID CCardControl::SetCardSpace(UINT nXDistance, UINT nYDistance, UINT nShootDistance)
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
VOID CCardControl::DrawCardControl(CDC * pDC)
{
	//��ʾ�ж�
	if (m_bShowCardControl==false) return;

	//����λͼ
	CPngImage ImageCardMask;
	CImageHandle HandleCard(&m_ImageCard);

	CImageHandle HandleOpenCard(&m_ImageOpenCardHandle);

	//��ȡλ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//��������
	INT nXDrawPos=0,nYDrawPos=0;
	INT nXImagePos=0,nYImagePos=0;

	//������Դ
	CPngImage ImageCardSelect;
	if ((m_wStartIndex<m_wCardCount)&&(m_wEndIndex<m_wCardCount))
	{
		ImageCardSelect.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_SELECT"));
	}
	m_wCardCount=(WORD)m_CardItemArray.GetCount();
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
			nXImagePos=m_CardSize.cx*2;
			nYImagePos=m_CardSize.cy*4;
		}
		if(m_blhideOneCard)
		{
			if(i==0)
			{
				nXImagePos=m_CardSize.cx*2;
				nYImagePos=m_CardSize.cy*4;

			}
		

		}
        m_bHorizontal = true;
		//��Ļλ��
		if (m_bHorizontal==true)
		{
			{
				
				
				if(m_blGameEnd)
				{
					nXDrawPos=m_nXDistance*i;

				}else
				 nXDrawPos = 0;
			}
			
			nYDrawPos=(bShoot==false)?m_nShootDistance:0;
		}
		else
		{
			nXDrawPos=0;
			nYDrawPos=m_nYDistance*i;
		}
		if(m_blGameEnd)
		{

				m_ImageCard.AlphaDrawImage(pDC,OriginPoint.x+nXDrawPos+15,OriginPoint.y+nYDrawPos+5,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));
		}else
		{
				//�滭�˿�
				if(1==i)
				{
					m_ImageCard.AlphaDrawImage(pDC,OriginPoint.x+nXDrawPos+m_MovePoint.x+25,OriginPoint.y+nYDrawPos+m_MovePoint.y+5,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));
					m_ImageOpenCardHandle.AlphaDrawImage(pDC,OriginPoint.x,OriginPoint.y+nYDrawPos-5+5,m_ImageOpenCardHandle.GetWidth(),m_ImageOpenCardHandle.GetHeight(),0,0,RGB(255,0,255));

				
				}
				else
					m_ImageCard.AlphaDrawImage(pDC,OriginPoint.x+nXDrawPos+25,OriginPoint.y+nYDrawPos+5,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));

		}
		//�滭ѡ��
		if ((m_wStartIndex<=i)&&(i<=m_wEndIndex))
		{
			CSize SizeSelectImage(ImageCardSelect.GetWidth(),ImageCardSelect.GetHeight());
			ImageCardSelect.DrawImage(pDC,OriginPoint.x+nXDrawPos,OriginPoint.y+nYDrawPos,SizeSelectImage.cx,SizeSelectImage.cy,0,0);
		}
	}

	return;
}

//�����Ϣ
bool CCardControl::OnEventSetCursor(CPoint Point)
{
	return false;
}

//��ȡ��С
VOID CCardControl::GetControlSize(CSize & ControlSize)
{
	m_wCardCount = (WORD)m_CardItemArray.GetCount();
	//��ȡ��С
	if (m_bHorizontal==true)
	{
		ControlSize.cy=m_CardSize.cy+m_nShootDistance;
		ControlSize.cx=(m_wCardCount>0)?(m_CardSize.cx+(m_wCardCount-1)*m_nXDistance*2):0;
	}
	else
	{
		ControlSize.cx=m_CardSize.cx;
		ControlSize.cy=(m_wCardCount>0)?(m_CardSize.cy+(m_wCardCount-1)*m_nYDistance):0;
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

//�����Ϣ
void CCardControl::OnMouseMove(UINT nFlags, CPoint wndPoint)
{
	
	if(m_blAuto)
		return;
	if(m_dwCurrentIndex==INVALID_ITEM)
	{
		m_MovePoint.SetPoint(0,0);
		m_DownPoint.SetPoint(0,0);
		m_dwCurrentIndex = INVALID_ITEM;
		return ;

	}
	if(m_dwCurrentIndex>=1&&m_dwCurrentIndex !=INVALID_ITEM)
	{
		CSize ControlSize;
		GetControlSize(ControlSize);
		CPoint OriginPoint;
		GetOriginPoint(OriginPoint);

		//��׼λ��
		bool bldecY = false;
		INT nXPos=wndPoint.x-m_DownPoint.x;
		INT nYPos=wndPoint.y-m_DownPoint.y;
		if(nYPos<0)
		{
			nYPos = 0;

		}
		if(nXPos<0)
		{
			nXPos = 0;
		}

		if(nXPos>40)
		{
			nXPos = 40;
			
		}
		if(nYPos>m_CardSize.cy/2)
		{
			nYPos = m_CardSize.cy/2;
		}
		 m_MovePoint.y =  nYPos;
		  m_MovePoint.x = nXPos;
	}
	return;
}
afx_msg void CCardControl::OnLButtonUp(UINT nFlags, CPoint wndPoint)
{
	
	CPoint OriPoint;
	GetOriginPoint(OriPoint);
	int x = wndPoint.x ;
	int y = wndPoint.y;
	CPoint Point(x,y);
	m_MovePoint.SetPoint(0,0);
	m_DownPoint.SetPoint(0,0);
	m_dwCurrentIndex = INVALID_ITEM;
	return ;

}
void CCardControl::OnLButtonDown(UINT nFlags, CPoint wndPoint)
{

	
	CPoint OriPoint;
	GetOriginPoint(OriPoint);
	int x = wndPoint.x ;
	int y = wndPoint.y;
	CPoint Point(x,y);
	m_dwDragMax = DWORD(-1);
	//��ȡ����
	m_dwCurrentIndex=SwitchCardPoint(Point);
	if(m_dwCurrentIndex==INVALID_ITEM&&m_dwCurrentIndex>2)
	{
		printf("");
	}else
	{
		m_DownPoint.SetPoint(wndPoint.x,wndPoint.y);
	}

	return;
}
//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//����ģʽ
	if ((m_bHorizontal==true)&&(m_wCardCount>0))
	{
		//��ȡλ��
		CSize ControlSize;
		CPoint OriginPoint;
		GetControlSize(ControlSize);
		GetOriginPoint(OriginPoint);

		//��׼λ��
		INT nXPos=MousePoint.x-OriginPoint.x;
		INT nYPos=MousePoint.y-OriginPoint.y;

		//Խ���ж�
		if ((nXPos<0)||(nXPos>ControlSize.cx)) return INVALID_ITEM;
		if ((nYPos<0)||(nYPos>ControlSize.cy)) return INVALID_ITEM;

		//��������
		WORD wCardIndex=nXPos/m_nXDistance;
		if (wCardIndex>=m_wCardCount) wCardIndex=(m_wCardCount-1);

		//�˿�����
		for (WORD i=0;i<=wCardIndex;i++)
		{
			//��������
			WORD wCurrentIndex=wCardIndex-i;

			//�������
			if (nXPos>(INT)(wCurrentIndex*m_nXDistance+m_CardSize.cx)) break;

			//�������
			bool bShoot=m_CardItemArray[wCurrentIndex].bShoot;
			if ((bShoot==true)&&(nYPos<=m_CardSize.cy)) return wCurrentIndex;
			if ((bShoot==false)&&(nYPos>=(INT)m_nShootDistance)) return wCurrentIndex;
		}
	}

	return INVALID_ITEM;
}

//�ƶ��˿�
bool CCardControl::MoveCardItem(WORD wTargerItem)
{
	//Ч�����
	ASSERT(wTargerItem<m_wCardCount);
	if (wTargerItem>=m_wCardCount) return false;

	//ͳ���˿�
	WORD wShootCount=0;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		if (m_CardItemArray[i].bShoot==true)
		{
			wShootCount++;
		}
	}

	//�ƶ��˿�
	if ((wShootCount>0)&&(wShootCount<m_wCardCount))
	{
		//��������
		wTargerItem=__min(m_wCardCount-wShootCount,wTargerItem);

		//��������
		WORD wShootIndex=wTargerItem;
		WORD wNormalIndex=(wTargerItem==0)?(wTargerItem+wShootCount):0;

		//��ȡ�˿�
		tagCardItem CardItemTemp[MAX_CARD_COUNT];
		 m_wCardCount = (WORD)m_CardItemArray.GetCount();
		for(int i = 0;i<m_wCardCount;i++)
		{
			CardItemTemp[i] = m_CardItemArray[i];

		}
		//�����˿�
		for (WORD i=0;i<m_wCardCount;i++)
		{
			if (CardItemTemp[i].bShoot==false)
			{
				//�����˿�
				m_CardItemArray[wNormalIndex++]=CardItemTemp[i];

				//��������
				if (wNormalIndex==wTargerItem) wNormalIndex=wTargerItem+wShootCount;
			}
			else
			{
				//�����˿�
				m_CardItemArray[wShootIndex++]=CardItemTemp[i];
			}
		}

		return true;
	}

	return false;
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
	m_CardItemArray[wTargerItem]=CardItem;

	return true;
}
//���������е���ʵ�ֶ���
void CCardControl::OnCopyCard()
{
	WORD wCount = (WORD) m_CardItemArray.GetCount();
	m_blhideOneCard = true;
	if(m_CardTempItemArray.GetCount()>wCount)
	{
		m_blGameEnd = true;
		m_CardItemArray.Add(m_CardTempItemArray[wCount]);

	}

}
//�����˿�
void CCardControl::ShootAllCard(bool bShoot)
{
	if (bShoot==true)
	{
		SetShootIndex(0,m_wCardCount-1);
	}
	else
	{
		for (WORD i=0;i<m_wCardCount;i++)
		{
			m_CardItemArray[i].bShoot=false;
		}
	}
}
//////////////////////////////////////////////////////////////////////////

