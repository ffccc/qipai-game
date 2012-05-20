#include "StdAfx.h"
#include "GameClient.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
#include "GameClientView.h"
//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bSmallMode=false;
	m_bHorizontal=true;
	m_bPositively=false;
	m_bDisplayItem=false;
	m_bShowCardControl=true;

	//ѡ�����
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_CardItemArray1,sizeof(m_CardItemArray1));
	ZeroMemory(m_CardItemArray1,sizeof(m_CardItemArray1));

	//λ�ñ���
	m_YCollocateMode=enYTop;
	m_XCollocateMode=enXLeft;
	m_BenchmarkPos.SetPoint(0,0);

	//�������
	m_nXDistance=(m_bSmallMode==false)?DEF_X_DISTANCE:DEF_X_DISTANCE_SMALL;
	m_nYDistance=(m_bSmallMode==false)?DEF_Y_DISTANCE:DEF_Y_DISTANCE_SMALL;
	m_nShootDistance=(m_bSmallMode==false)?DEF_SHOOT_DISTANCE:DEF_SHOOT_DISTANCE_SMALL;

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageCard.SetLoadInfo((m_bSmallMode==false)?IDB_GAME_CARD:IDB_GAME_CARD,hResInstance);

	m_ImageHand.SetLoadInfo(IDB_BMP_HAND,hResInstance);
//	m_ImageNiu.SetLoadInfo(IDB_BMP_NIUNIU,hResInstance);

//	m_cbNiuType=0;
	//��ȡ��С
	CImageHandle HandleCard(&m_ImageCard);
	m_CardSize.SetSize(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);
	
	m_nOffsetSecondCardPos=0;
	m_bIsTuiPaiOver=false;
	m_bTui=false;
	m_bZuoTui=false;
	m_bMoveFirstOver=false;
	m_bIsOver=false;
	m_pWnd=NULL;
	m_SecondCardRect.SetRect(0,0,0,0);
	m_OffsetX=0;
	m_OffsetY=0;
	return;
}

//��������
CCardControl::~CCardControl()
{
}

//�����˿�
bool CCardControl::SetCardData(WORD wCardCount)
{
	//Ч�����
	ASSERT(wCardCount<=5);
	if (wCardCount>5) return false;

	//���ñ���
	m_wCardCount=wCardCount;
	ZeroMemory(m_CardItemArray1,sizeof(m_CardItemArray1));
	ZeroMemory(m_CardItemArray2,sizeof(m_CardItemArray2));

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
	//Ч�����
	ASSERT(wCardCount<=5);
	if (wCardCount>5) return false;
	ZeroMemory(m_CardItemArray1,sizeof(m_CardItemArray1));
	ZeroMemory(m_CardItemArray2,sizeof(m_CardItemArray2));

	//���ñ���
	m_wEndIndex=INVALID_ITEM;
	m_wStartIndex=INVALID_ITEM;

	//�˿���Ŀ
	m_wCardCount=wCardCount;
	m_bIsOver=false;

	//�����˿�
	for (WORD i=0;i<wCardCount;i++)
	{
		if(i<3)
		{
			m_CardItemArray1[i].bShow=true;
			m_CardItemArray1[i].cbCardData=cbCardData[i];
		}
		else
		{
			m_CardItemArray2[i-3].bShow=(i==3)?false:true;
			m_CardItemArray2[i-3].cbCardData=cbCardData[i];
		}
		
	}

	return true;
}

bool CCardControl::SetNiuCardData(const BYTE cbCardData[],WORD wCardCount,bool bHaveNiu)
{
	//Ч�����
	ASSERT(wCardCount==5);
	if (wCardCount != 5) return false;
	ZeroMemory(m_CardItemArray1,sizeof(m_CardItemArray1));
	ZeroMemory(m_CardItemArray2,sizeof(m_CardItemArray2));
	
	m_bIsOver=true;

	//�����˿�
	if(bHaveNiu)
	{
		for (WORD i=0;i<wCardCount;i++)
		{
			if(i<3)
			{
				m_CardItemArray2[i].bShow=true;
				m_CardItemArray2[i].cbCardData=cbCardData[i];
			}
			else
			{
				m_CardItemArray1[i-3].bShow=true;
				m_CardItemArray1[i-3].cbCardData=cbCardData[i];
			}

		}
	}
	else
	{
		for (WORD i=0;i<wCardCount;i++)
		{
			m_CardItemArray2[i].bShow=true;
			m_CardItemArray2[i].cbCardData=cbCardData[i];
		}
	}


	return true;
}

/*
//�����˿�
//bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
//{
//	//Ч�����
//	ASSERT(wCardCount<=CountArray(m_CardItemArray));
//	if (wCardCount>CountArray(m_CardItemArray)) return false;
//
//	//���ñ���
//	m_wEndIndex=INVALID_ITEM;
//	m_wStartIndex=INVALID_ITEM;
//
//	//�����˿�
//	m_wCardCount=wCardCount;
//	CopyMemory(m_CardItemArray,CardItemArray,wCardCount*sizeof(tagCardItem));
//
//	return true;
//}

//�����˿�
//bool CCardControl::SetShootIndex(WORD wStartIndex, WORD wEndIndex)
//{
//	//���ñ���
//	m_wEndIndex=INVALID_ITEM;
//	m_wStartIndex=INVALID_ITEM;
//
//	//�����˿�
//	for (WORD i=0;i<m_wCardCount;i++)
//	{
//		if ((i>=wStartIndex)&&(i<=wEndIndex))
//		{
//			m_CardItemArray[i].bShoot=true;
//		}
//	}
//
//	return true;
//}

//�����˿�
//bool CCardControl::SetSelectIndex(WORD wStartIndex, WORD wEndIndex)
//{
//	//���ñ���
//	m_wEndIndex=wEndIndex;
//	m_wStartIndex=wStartIndex;
//
//	return true;
//}

//�����˿�
//bool CCardControl::SetShootCard(const BYTE cbCardData[], WORD wCardCount)
//{
//	//��������
//	bool bChangeStatus=false;
//
//	//�����˿�
//	for (WORD i=0;i<m_wCardCount;i++) 
//	{
//		if (m_CardItemArray[i].bShoot==true)
//		{
//			bChangeStatus=true;
//			m_CardItemArray[i].bShoot=false;
//		}
//	}
//
//	//�����˿�
//	for (WORD i=0;i<wCardCount;i++)
//	{
//		for (WORD j=0;j<m_wCardCount;j++)
//		{
//			if ((m_CardItemArray[j].bShoot==false)&&(m_CardItemArray[j].cbCardData==cbCardData[i])) 
//			{
//				bChangeStatus=true;
//				m_CardItemArray[j].bShoot=true;
//				break;
//			}
//		}
//	}
//
//	return bChangeStatus;
//}

////ɾ���˿�
//bool CCardControl::RemoveShootItem()
//{
//	//���ñ���
//	m_wEndIndex=INVALID_ITEM;
//	m_wStartIndex=INVALID_ITEM;
//
//	//�����˿�
//	tagCardItem	CardItemArray[MAX_CARD_COUNT];
//	CopyMemory(CardItemArray,m_CardItemArray,sizeof(tagCardItem)*m_wCardCount);
//
//	//ɾ���˿�
//	WORD wRemoveCount=0;
//	for (WORD i=0;i<m_wCardCount;i++)
//	{
//		if (CardItemArray[i].bShoot==true)
//		{
//			wRemoveCount++;
//			CardItemArray[i].cbCardData=0x00;
//		}
//	}
//
//	//�����˿�
//	if (wRemoveCount>0)
//	{
//		//�����˿�
//		WORD wInsertCount=0;
//		for (WORD i=0;i<m_wCardCount;i++)
//		{
//			if (CardItemArray[i].cbCardData!=0x00)
//			{
//				m_CardItemArray[wInsertCount++]=CardItemArray[i];
//			}
//		}
//
//		//���ñ���
//		m_wCardCount=m_wCardCount-wRemoveCount;
//	}
//
//	return true;
//}

//��ȡ�˿�
//tagCardItem * CCardControl::GetCardFromIndex(WORD wIndex)
//{
//	return (wIndex<m_wCardCount)?&m_CardItemArray[wIndex]:NULL;
//}

//��ȡ�˿�
//tagCardItem * CCardControl::GetCardFromPoint(CPoint & MousePoint)
//{
//	WORD wIndex=SwitchCardPoint(MousePoint);
//	return (wIndex!=INVALID_ITEM)?&m_CardItemArray[wIndex]:NULL;
//}

//��ȡ�˿�
//WORD CCardControl::GetCardData(BYTE cbCardData[], WORD wBufferCount)
//{
//	//Ч�����
//	ASSERT(wBufferCount>=m_wCardCount);
//	if (wBufferCount<m_wCardCount) return 0;
//
//	//�����˿�
//	for (WORD i=0;i<m_wCardCount;i++) cbCardData[i]=m_CardItemArray[i].cbCardData;
//
//	return m_wCardCount;
//}

//��ȡ�˿�
//WORD CCardControl::GetShootCard(BYTE cbCardData[], WORD wBufferCount)
//{
//	//��������
//	WORD wShootCount=0;
//
//	//�����˿�
//	for (WORD i=0;i<m_wCardCount;i++) 
//	{
//		//Ч�����
//		ASSERT(wBufferCount>wShootCount);
//		if (wBufferCount<=wShootCount) break;
//
//		//�����˿�
//		if (m_CardItemArray[i].bShoot==true) cbCardData[wShootCount++]=m_CardItemArray[i].cbCardData;
//	}
//
//	return wShootCount;
//}

////��ȡ�˿�
//WORD CCardControl::GetCardData(tagCardItem CardItemArray[], WORD wBufferCount)
//{
//	//Ч�����
//	ASSERT(wBufferCount>=m_wCardCount);
//	if (wBufferCount<m_wCardCount) return 0;
//
//	//�����˿�
//	CopyMemory(CardItemArray,m_CardItemArray,sizeof(tagCardItem)*m_wCardCount);
//
//	return m_wCardCount;
//}

//���þ���
//VOID CCardControl::SetCardSpace(UINT nXDistance, UINT nYDistance, UINT nShootDistance)
//{
//	//���ñ���
//	m_nXDistance=nXDistance;
//	m_nYDistance=nYDistance;
//	m_nShootDistance=nShootDistance;
//
//	return;
//}
*/
void CCardControl::SetParentWnd(CWnd *pWnd)
{
	m_pWnd=pWnd;
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


	CPoint ptt;
	GetOriginPoint(ptt);
	ptt.x+=10;
	ptt.y+=23;
	m_SecondCardRect.SetRect(ptt.x,ptt.y,ptt.x+80,ptt.y+105);
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

	//��ȡλ��
	CPoint OriginPoint;
	GetOriginPoint(OriginPoint);

	//��������
	INT nXDrawPos=0,nYDrawPos=0;
	INT nXImagePos=0,nYImagePos=0;

	//������Դ
	//CPngImage ImageCardSelect;
	//if ((m_wStartIndex<m_wCardCount)&&(m_wEndIndex<m_wCardCount))
	//{
	//	ImageCardSelect.LoadImage(AfxGetInstanceHandle(),TEXT("CARD_SELECT"));
	//}

	for (WORD i=0;i<m_wCardCount;i++)
	{
		//��ȡ�˿�
		BYTE cbCardData=m_CardItemArray1[i].cbCardData;

		//��϶����
		//if (cbCardData==SPACE_CARD_DATA) continue;
		if(!cbCardData)continue;

		//ͼƬλ��
		if (cbCardData!=0)
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
		if(m_bIsOver)
		{
			nXDrawPos=m_nXDistance*i+10;
		}
		else
		{
			nXDrawPos=m_nXDistance*i;
		}
		

		//�滭�˿�
		m_ImageCard.AlphaDrawImage(pDC,OriginPoint.x+nXDrawPos,OriginPoint.y,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));
	


		//�滭ѡ��
		//if ((m_wStartIndex<=i)&&(i<=m_wEndIndex))
		//{
		//	CSize SizeSelectImage(ImageCardSelect.GetWidth(),ImageCardSelect.GetHeight());
		//	ImageCardSelect.DrawImage(pDC,OriginPoint.x+nXDrawPos,OriginPoint.y+nYDrawPos,SizeSelectImage.cx,SizeSelectImage.cy,0,0);
		//}
	}

	for (WORD i=0;i<m_wCardCount;i++)//��
	{
		//��ȡ�˿�
		BYTE cbCardData=m_CardItemArray2[i].cbCardData;
		if(!cbCardData)continue;
		//��϶����
		//if (cbCardData==SPACE_CARD_DATA) continue;

		//ͼƬλ��
		if (cbCardData!=0 && (m_CardItemArray2[i].bShow || m_bTui ||m_bZuoTui))
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
		if(i==1)
		{
			CGameClientView *pWnd=(CGameClientView*)m_pWnd;
			if(!pWnd->m_bAutoModel && m_bMoveFirstOver)
			{
				nXDrawPos=m_nXDistance*i+10+m_OffsetX+m_nOffsetSecondCardPos;
				nYDrawPos=m_OffsetY;
			}
			else
				nXDrawPos=m_nXDistance*i+10+m_nOffsetSecondCardPos;
		}
		else
			nXDrawPos=m_nXDistance*i+10+m_nOffsetFirstCardPos;

		if(m_bIsOver)
		{
			nXDrawPos=nXDrawPos-10;
		}

		//�滭�˿�
		m_ImageCard.AlphaDrawImage(pDC,OriginPoint.x+nXDrawPos,OriginPoint.y+23+7+nYDrawPos,m_CardSize.cx,m_CardSize.cy,nXImagePos,nYImagePos,RGB(255,0,255));
	}

	if(m_bMoveFirstOver)
	{
		CImageHandle ImageHandleHand(&m_ImageHand);
		int posx=0;
		CGameClientView *pWnd=(CGameClientView*)m_pWnd;
		if(pWnd && !pWnd->m_bAutoModel)
		{
			posx-=5;
		}
		
		m_ImageHand.AlphaDrawImage(pDC,OriginPoint.x-16+posx,OriginPoint.y+22+7,m_ImageHand.GetWidth(),m_ImageHand.GetHeight(),0,0,RGB(255,0,255));
	}

	//CImageHandle ImageHandleNiu(&m_ImageNiu);
	//int nType=0;
	//if(m_cbNiuType>9)
	//{
	//	nType=10;
	//}
	//else
	//{
	//	nType=m_cbNiuType;
	//}
	//m_ImageNiu.AlphaDrawImage(pDC, OriginPoint.x + 28, OriginPoint.y + 130, m_ImageNiu.GetWidth()/11, m_ImageNiu.GetHeight(),
	//	m_ImageNiu.GetWidth()/11*nType, 0, RGB(255, 0, 255));
	return;
}
void CCardControl::ReSet()
{
	m_nOffsetSecondCardPos=0;
	m_nOffsetFirstCardPos=0;
	m_bIsTuiPaiOver=false;
	m_bTui=false;
	m_bZuoTui=false;
	m_bMoveFirstOver=false;
}
bool CCardControl::ShouTuiPai()
{
	if(!m_pWnd)return false;

	CGameClientView *pWnd=(CGameClientView*)m_pWnd;

	if(!pWnd->m_bAutoModel)
	{
		m_bMoveFirstOver=true;
		m_nOffsetSecondCardPos=-17;
		m_CardItemArray2[0].bShow=true;
		return false;
	}
	return true;
}
bool CCardControl::TuiPai()
{
	if(!m_bIsTuiPaiOver)
	{
		if(!m_bTui)
		{
			m_nOffsetSecondCardPos-=2;
			if(!m_bMoveFirstOver)
				m_nOffsetFirstCardPos+=1;
			if(m_bZuoTui)
			{
				if(m_nOffsetSecondCardPos<0)
				{
					m_bIsTuiPaiOver=true;
					m_nOffsetSecondCardPos=0;
					m_nOffsetFirstCardPos=0;
					m_bMoveFirstOver=false;
				}	
			}
			else
			{
				if(m_nOffsetSecondCardPos<-10)
				{
					m_bTui=true;
					m_bMoveFirstOver=true;
				}
			}
			
		}
		else
		{
			m_nOffsetSecondCardPos+=2;
			if(m_nOffsetSecondCardPos>30)
			{
				m_bZuoTui=true;
				m_bTui=false;
			}
		}
		

	}
	return m_bIsTuiPaiOver;
}




void CCardControl::OnLButtonDown(CPoint point)
{
	if(m_bMoveFirstOver)
	{
		m_cpPushDown=point;
		m_bPushDown=true;
	}
}
void CCardControl::OnLButtonUp(CPoint point)
{
	if(m_bPushDown)
	{
		m_bPushDown=false;
		m_OffsetX=0;
		m_OffsetY=0;
	}
}
void CCardControl::OnMouseMove(CPoint point)
{
	if(m_bPushDown)
	{
		CPoint pt=point-m_cpPushDown;
		//if(pt.x<0 || pt.y<0 || pt.x>80 || pt.y> 80)
		//	return;
		m_OffsetX=pt.x;
		m_OffsetY=pt.y;
	}
}

bool CCardControl::PtInSecondCardRect(CPoint pt)
{
	return m_SecondCardRect.PtInRect(pt);
}

//�����Ϣ
//bool CCardControl::OnEventSetCursor(CPoint Point)
//{
//	//��괦��
//	//if (m_bPositively==true)
//	//{
//	//	//��ȡ����
//	//	WORD wHoverItem=SwitchCardPoint(Point);
//
//	//	//�����ж�
//	//	if (wHoverItem!=INVALID_ITEM)
//	//	{
//	//		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
//	//		return true;
//	//	}
//	//}
//
//	return false;
//}

//��ȡ��С
VOID CCardControl::GetControlSize(CSize & ControlSize)
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
/*
//�����л�
//WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
//{
//	//����ģʽ
//	if ((m_bHorizontal==true)&&(m_wCardCount>0))
//	{
//		//��ȡλ��
//		CSize ControlSize;
//		CPoint OriginPoint;
//		GetControlSize(ControlSize);
//		GetOriginPoint(OriginPoint);
//
//		//��׼λ��
//		INT nXPos=MousePoint.x-OriginPoint.x;
//		INT nYPos=MousePoint.y-OriginPoint.y;
//
//		//Խ���ж�
//		if ((nXPos<0)||(nXPos>ControlSize.cx)) return INVALID_ITEM;
//		if ((nYPos<0)||(nYPos>ControlSize.cy)) return INVALID_ITEM;
//
//		//��������
//		WORD wCardIndex=nXPos/m_nXDistance;
//		if (wCardIndex>=m_wCardCount) wCardIndex=(m_wCardCount-1);
//
//		//�˿�����
//		for (WORD i=0;i<=wCardIndex;i++)
//		{
//			//��������
//			WORD wCurrentIndex=wCardIndex-i;
//
//			//�������
//			if (nXPos>(INT)(wCurrentIndex*m_nXDistance+m_CardSize.cx)) break;
//
//			//�������
//			bool bShoot=m_CardItemArray[wCurrentIndex].bShoot;
//			if ((bShoot==true)&&(nYPos<=m_CardSize.cy)) return wCurrentIndex;
//			if ((bShoot==false)&&(nYPos>=(INT)m_nShootDistance)) return wCurrentIndex;
//		}
//	}
//
//	return INVALID_ITEM;
//}

//�ƶ��˿�
//bool CCardControl::MoveCardItem(WORD wTargerItem)
//{
//	//Ч�����
//	ASSERT(wTargerItem<m_wCardCount);
//	if (wTargerItem>=m_wCardCount) return false;
//
//	//ͳ���˿�
//	WORD wShootCount=0;
//	for (WORD i=0;i<m_wCardCount;i++)
//	{
//		if (m_CardItemArray[i].bShoot==true)
//		{
//			wShootCount++;
//		}
//	}
//
//	//�ƶ��˿�
//	if ((wShootCount>0)&&(wShootCount<m_wCardCount))
//	{
//		//��������
//		wTargerItem=__min(m_wCardCount-wShootCount,wTargerItem);
//
//		//��������
//		WORD wShootIndex=wTargerItem;
//		WORD wNormalIndex=(wTargerItem==0)?(wTargerItem+wShootCount):0;
//
//		//��ȡ�˿�
//		tagCardItem CardItemTemp[MAX_CARD_COUNT];
//		CopyMemory(CardItemTemp,m_CardItemArray,sizeof(tagCardItem)*m_wCardCount);
//
//		//�����˿�
//		for (WORD i=0;i<m_wCardCount;i++)
//		{
//			if (CardItemTemp[i].bShoot==false)
//			{
//				//�����˿�
//				m_CardItemArray[wNormalIndex++]=CardItemTemp[i];
//
//				//��������
//				if (wNormalIndex==wTargerItem) wNormalIndex=wTargerItem+wShootCount;
//			}
//			else
//			{
//				//�����˿�
//				m_CardItemArray[wShootIndex++]=CardItemTemp[i];
//			}
//		}
//
//		return true;
//	}
//
//	return false;
//}
//�����˿�
//bool CCardControl::SwitchCardItem(WORD wSourceItem, WORD wTargerItem)
//{
//	//Ч�����
//	if (wSourceItem==wTargerItem) return false;
//	if ((wSourceItem>=m_wCardCount)||(wTargerItem>=m_wCardCount)) return false;
//
//	//�����˿�
//	tagCardItem CardItem=m_CardItemArray[wSourceItem];
//
//	//�ƶ��˿�
//	if (wSourceItem>wTargerItem)
//	{
//		WORD wMoveCount=wSourceItem-wTargerItem;
//		MoveMemory(&m_CardItemArray[wTargerItem+1],&m_CardItemArray[wTargerItem],sizeof(tagCardItem)*wMoveCount);
//	}
//	else
//	{
//		WORD wMoveCount=wTargerItem-wSourceItem;
//		MoveMemory(&m_CardItemArray[wSourceItem],&m_CardItemArray[wSourceItem+1],sizeof(tagCardItem)*wMoveCount);
//	}
//
//	//����Ŀ��
//	m_CardItemArray[wTargerItem]=CardItem;
//
//	return true;
//}

//�����˿�
//void CCardControl::ShootAllCard(bool bShoot)
//{
//	if (bShoot==true)
//	{
//		SetShootIndex(0,m_wCardCount-1);
//	}
//	else
//	{
//		for (WORD i=0;i<m_wCardCount;i++)
//		{
//			m_CardItemArray[i].bShoot=false;
//		}
//	}
//}
*/
//////////////////////////////////////////////////////////////////////////

