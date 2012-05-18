

#include "Stdafx.h"
#include "Resource.h"
#include "GameCardEx.h"


BEGIN_MESSAGE_MAP(CGameCardEx, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
//	ON_WM_RBUTTONUP()
ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


//���캯��
CGameCardEx::CGameCardEx()
{
	m_bCardCount=0;
	m_iCardHight=95;
	m_iCardWidth=70;
	m_iCardHSpace=18;
	m_iCardVSpace=18;

	m_iCardHSpaceAdd=m_iCardHSpace;
	m_bAddMaxCnt=0;
	ZeroMemory(m_bAddPosArray,sizeof(m_bAddPosArray));

	m_iCardUpSpace=10;
	m_pHandleWnd=NULL;
	m_bHorzLook=true;
	m_bShowCard=false;
	m_bUseMouse=false;
	m_bHotHitCard=255;
	m_PtBasePoint.SetPoint(0,0);
	m_XShowMode=XShowMode::SWX_MODE_CENTER;
	m_YShowMode=YShowMode::SWY_MODE_CENTER;

	HINSTANCE hInstance = ::AfxGetInstanceHandle();

	m_CardPic[0].LoadFromResource(hInstance,IDB_CARD);
	m_CardPic[1].LoadFromResource(hInstance,IDB_CARD_BK);
}

//��������
CGameCardEx::~CGameCardEx()
{
}

//��ʼ������
bool CGameCardEx::InitGameCard(CWnd * pHandleWnd)
{
	m_pHandleWnd=pHandleWnd;
	return true;
}

//�����˿˲���
void CGameCardEx::SetCardParameter(int iHSpace, int iVSpace, int iUpSpace)
{
	m_iCardHSpace=iHSpace;
	m_iCardVSpace=iVSpace;
	m_iCardUpSpace=iUpSpace;
	m_iCardHSpaceAdd=m_iCardHSpace;
	FixCardControl();
	return;
}

//��ȡ��С����
bool CGameCardEx::GetCardSize(CSize & Size)
{
	if (m_bCardCount>0)
	{
		if (m_bHorzLook==true)
		{
			Size.cx=m_iCardWidth+(m_bCardCount-1)*m_iCardHSpace+m_bAddMaxCnt*m_iCardHSpaceAdd;
			Size.cy=m_iCardHight+m_iCardUpSpace;
		}
		else
		{
			Size.cx=m_iCardWidth;
			Size.cy=m_iCardHight+(m_bCardCount-1)*m_iCardVSpace;
		}
	}
	else
	{
		Size.cx=0;
		Size.cy=0;
	}
	return true;
}

//���û�׼�㺯��
void CGameCardEx::SetBasePoint(CPoint & BasePoint, XShowMode XShowMode, YShowMode YShowMode)
{
	m_XShowMode=XShowMode;
	m_YShowMode=YShowMode;
	m_PtBasePoint=BasePoint;
	FixCardControl();
	return;
}

//������ʾ�˿�
void CGameCardEx::SetShowCard(bool bShow)
{
	m_bShowCard=bShow;
	Invalidate(FALSE);
	return;
}
void CGameCardEx::SetAddParameter(BYTE bAddPos[],BYTE bAddCnt)
{
	ASSERT(bAddCnt<=5);
	m_bAddMaxCnt=bAddCnt;
	for(int i=0;i<m_bAddMaxCnt;++i)
	{
		m_bAddPosArray[i]=bAddPos[i];
	}
}
//����ʹ�����
void CGameCardEx::SetUseMouse(bool bUserMouse)
{
	m_bUseMouse=bUserMouse;
	return;
}

//������ʾ��ʽ
void CGameCardEx::SetLookMode(bool bHorzLook)
{
	m_bHorzLook=bHorzLook;
	return;
}

//�����˿�
bool CGameCardEx::SetCard(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount)
{
	if (bCardCount>m_CardArray.GetSize()) m_CardArray.SetSize(bCardCount);
	if (bCardCount>m_CardUpArray.GetSize()) m_CardUpArray.SetSize(bCardCount);
	if (bCardList!=NULL) CopyMemory(m_CardArray.GetData(),bCardList,sizeof(BYTE)*bCardCount);
	if (bUpList!=NULL) CopyMemory(m_CardUpArray.GetData(),bUpList,sizeof(BYTE)*bCardCount);
	else memset(m_CardUpArray.GetData(),0,sizeof(BYTE)*bCardCount);
	m_bCardCount=bCardCount;
	FixCardControl();
	return true;
}

//��ȡ������˿�
BYTE CGameCardEx::GetUpCard(BYTE bCardList[])
{
	BYTE bUpCount=0;
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		if (m_CardUpArray.GetAt(i)==TRUE)
		{
			if (bCardList!=NULL) bCardList[bUpCount]=m_CardArray.GetAt(i);
			bUpCount++;
		}
	}
	return bUpCount;
}

//��ȡ�˿���Ϣ
BYTE CGameCardEx::GetCard(BYTE bCardList[], BYTE bUpList[])
{
	if (bCardList!=NULL) CopyMemory(bCardList,m_CardArray.GetData(),m_bCardCount*sizeof(BYTE));
	if (bUpList!=NULL) CopyMemory(bUpList,m_CardUpArray.GetData(),m_bCardCount*sizeof(BYTE));
	return m_bCardCount;
}

//��������
BYTE CGameCardEx::HitCardTest(CPoint & Point)
{
	if (m_bHorzLook==true)
	{
		//Ѱ�Ұ����˿�
		BYTE bCardIndex=GetCardIndexPos(Point.x);//(BYTE)(Point.x/m_iCardHSpace);
		if (bCardIndex>=m_bCardCount) bCardIndex=m_bCardCount-1;

		//�жϰ���λ��
		bool bHitThisCard=true;
		BYTE bCardUp=m_CardUpArray.GetAt(bCardIndex);
		if ((bCardUp==TRUE)&&(Point.y>m_iCardHight)) bHitThisCard=false;
		if ((bCardUp==FALSE)&&(Point.y<m_iCardUpSpace)) bHitThisCard=false;

		//Ѱ��ǰ������˿�
		if (bHitThisCard==false)
		{
			BYTE bFindCount=m_iCardWidth/m_iCardHSpace+1;
			for (BYTE i=1;i<bFindCount;i++)
			{
				if (bCardIndex==0) break;
				bCardUp=m_CardUpArray.GetAt(bCardIndex-i);
				if ((bCardUp==FALSE)&&(Point.y>m_iCardWidth)) return bCardIndex-i;
				if ((bCardUp==TRUE)&&(Point.y<20)) return bCardIndex-i;
			}
			return 255;
		}
		return bCardIndex;
	}
	return 255;
}
BYTE CGameCardEx::GetCardIndexPos(int pos)
{
	//���ӵĸ���
	int iAddCnt=-1;
	for(BYTE i=0;i<m_bAddMaxCnt;++i)
	{
		if(pos>(m_bAddPosArray[i]-1)*m_iCardHSpace+i*m_iCardHSpaceAdd&&pos<m_bAddPosArray[i]*m_iCardHSpace+(i+1)*m_iCardHSpaceAdd)
		{
			ASSERT(m_bAddPosArray[i]>0);
			return m_bAddPosArray[i]-1;
		}
		if(pos<m_bAddPosArray[i]*m_iCardHSpace+(i+1)*m_iCardHSpaceAdd)
		{
			iAddCnt=i; 
			break;
		}
	}
	if(iAddCnt==-1) iAddCnt=m_bAddMaxCnt;
	return (BYTE)((pos-iAddCnt*m_iCardHSpaceAdd)/m_iCardHSpace);
}

//ɾ��������˿�
BYTE CGameCardEx::RemoveUpCard()
{
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		if (m_CardUpArray.GetAt(i)==TRUE)
		{
			m_bCardCount--;
			m_CardArray.RemoveAt(i);
			m_CardUpArray.RemoveAt(i);
			i--;
		}
	}
	FixCardControl();
	return m_bCardCount;
}

//��������λ��
void CGameCardEx::FixCardControl()
{
	//��ȡλ��
	CSize Size;
	GetCardSize(Size);

	//����λ��
	CRect CardRect;
	switch (m_XShowMode)
	{
	case SWX_MODE_LEFT: { CardRect.left=m_PtBasePoint.x; break; }
	case SWX_MODE_CENTER: {	CardRect.left=m_PtBasePoint.x-Size.cx/2; break; }
	case SWX_MODE_RIGHT: { CardRect.left=m_PtBasePoint.x-Size.cx; break; }
	}

	switch (m_YShowMode)
	{
	case SWY_MODE_TOP: { CardRect.top=m_PtBasePoint.y; break; }
	case SWY_MODE_CENTER: {	CardRect.top=m_PtBasePoint.y-Size.cy/2;	break; }
	case SWY_MODE_BOTTOM: { CardRect.top=m_PtBasePoint.y-Size.cy; break; }
	}

	//�ƶ�λ��
	CardRect.right=CardRect.left+Size.cx;
	CardRect.bottom=CardRect.top+Size.cy;
	MoveWindow(&CardRect);
	Invalidate(FALSE);

	int iCurAddPos=0;
	int iAddCnt=0;
	//��������
	CRgn AllRgn,SignedRgn;
	AllRgn.CreateRectRgn(0,0,0,0);
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		for(int j=iCurAddPos;j<m_bAddMaxCnt;j++)
		{
			if(i==m_bAddPosArray[j])
			{
				iCurAddPos++;
				iAddCnt++;
				break;
			}
		}
		if (m_bHorzLook==true)
		{
			BYTE bUpCard=m_CardUpArray.GetAt(i);
			SignedRgn.CreateRoundRectRgn(i*m_iCardHSpace+iAddCnt*m_iCardHSpaceAdd,(bUpCard==TRUE)?0:m_iCardUpSpace,
				i*m_iCardHSpace+iAddCnt*m_iCardHSpaceAdd+m_iCardWidth+1,(bUpCard==TRUE)?m_iCardHight+1:m_iCardUpSpace+m_iCardHight+1,2,2);
			AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
			SignedRgn.DeleteObject();
		}
		else 
		{
			SignedRgn.CreateRoundRectRgn(0,i*m_iCardVSpace,m_iCardWidth+1,i*m_iCardVSpace+m_iCardHight+1,2,2);
			AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
			SignedRgn.DeleteObject();
		}
	}

	//��������
	SetWindowRgn(AllRgn,TRUE);
	m_CardViewRgn.DeleteObject();
	m_CardViewRgn.Attach(AllRgn.Detach());

	return;
}

//������Ϣ
int CGameCardEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct)==-1) return -1;
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	return 0;
}

//�ػ�����
void CGameCardEx::OnPaint()
{
	CPaintDC dc(this);
	if (m_bCardCount==0) return;

	//��������
	//˫���廭�˿�
	CDC BufferDC;
	CSize CardSize;
	CBitmap BufferBmp;
	GetCardSize(CardSize);
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,CardSize.cx,CardSize.cy);
	CBitmap * pOldBmp=BufferDC.SelectObject(&BufferBmp);

	//�滭�˿�
	int iXPos=0,iYPos=0;
	
	CSkinImage * pActiveHandle=NULL;

	//��ǰ�Ӽ���λ��
	int iCurAddPos=0;
	int iAddCnt=0;
	for (BYTE i=0;i<m_bCardCount; i++)
	{
		BYTE bCard=m_CardArray.GetAt(i);
		BYTE bUpCard=m_CardUpArray.GetAt(i);

		if ((m_bShowCard==true)&&(m_CardArray.GetAt(i)!=0))
		{
			if ((bCard==0x4E)||(bCard==0x4F))
			{
				iYPos=0;
				iXPos=(bCard-0x4D)*m_iCardWidth;
				pActiveHandle=&m_CardPic[1];
			}
			else
			{
				pActiveHandle=&m_CardPic[0];
				iXPos=GetCardXPos(bCard)*m_iCardWidth;
				iYPos=GetCardYPos(bCard)*m_iCardHight;
			}

		}
		else
		{
			iYPos=0;
			iXPos=0;
			pActiveHandle=&m_CardPic[1];
		}
		for(int j=iCurAddPos;j<m_bAddMaxCnt;j++)
		{
			if(i==m_bAddPosArray[j])
			{
				iCurAddPos++;
				iAddCnt++;
				break;
			}
		}
		if(m_bHorzLook==true) 
		{
			pActiveHandle->BitBlt(BufferDC.m_hDC,
								i*m_iCardHSpace+iAddCnt*m_iCardHSpaceAdd,
									bUpCard?0:m_iCardUpSpace,
								m_iCardWidth,
									m_iCardHight,
									iXPos,
									iYPos);
		}
		else
		{
			pActiveHandle->BitBlt(BufferDC.m_hDC,
									0,
									i*m_iCardVSpace,
									m_iCardWidth,
									m_iCardHight,
									iXPos,
									iYPos);
		}
	}

	//ˢ�½���
	dc.BitBlt(0,0,CardSize.cx,CardSize.cy,&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.SelectObject(pOldBmp);
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();

	return;
}

//�����Ϣ
void CGameCardEx::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if ((m_bHorzLook==true)&&(m_bUseMouse==true)) 
	{
		m_bHotHitCard=HitCardTest(Point);
	}
	return;
}

//�����Ϣ
void CGameCardEx::OnLButtonUp(UINT nFlags, CPoint Point)
{
	if ((m_bUseMouse==true)&&(m_bHotHitCard!=255))
	{
		BYTE bHitCard=HitCardTest(Point);
		if ((bHitCard==255)||(m_bHotHitCard!=bHitCard))
		{
			m_bHotHitCard=255;
			return;
		}
		BYTE bUpCard=m_CardUpArray.GetAt(bHitCard);
		m_CardUpArray.SetAt(bHitCard,(bUpCard==TRUE)?FALSE:TRUE);
		FixCardControl();

		BYTE bAddCnt=0;
		for(int i=0;i<m_bAddMaxCnt;++i)
		{
			if(bHitCard>=m_bAddPosArray[i])
			{
				bAddCnt=i+1;
				break;
			}
		}
		//�ػ�ʧЧ����
		CRect Rect;
		Rect.SetRect(bHitCard*m_iCardHSpace+bAddCnt*m_iCardHSpaceAdd,0,bHitCard*m_iCardHSpace+bAddCnt*m_iCardHSpaceAdd+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
		InvalidateRect(Rect,FALSE);
		//if (m_pHandleWnd!=NULL) m_pHandleWnd->PostMessage(IDM_LEFT_HITCARD);
		AfxGetMainWnd()->SendMessage(IDM_LEFT_HITCARD,bHitCard,!bUpCard);
	}
	if(!m_bUseMouse)
	{
		AfxGetMainWnd()->SendMessage(IDM_REVERT_CARD);
	}
	return;
}

//�����Ϣ
//void CGameCard::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	if (m_bUseMouse==true)
//	{
//		AfxMessageBox("cards right up");
//	//	AfxGetMainWnd()->SendMessage(IDM_RIGHT_HITCARD);
//	}
//	return;
//}

//�����Ϣ
BOOL CGameCardEx::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	if (m_bUseMouse==true)
	{
		HINSTANCE hInstance = ::AfxGetInstanceHandle();

		HCURSOR hCursor = ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_HAND));
		if(hCursor)
		{
			::SetCursor(hCursor);
		}

		return TRUE;
	}
	return __super::OnSetCursor(pWnd,nHitTest,message);
}

void CGameCardEx::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetParent()->SendMessage(WM_RBUTTONUP);
	CWnd::OnRButtonUp(nFlags, point);
}
