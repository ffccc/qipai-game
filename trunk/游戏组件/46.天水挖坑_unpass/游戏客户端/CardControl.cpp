#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

bool					CCardControl::m_bLoad=false;					//���ر�־
CSize					CCardControl::m_CardSize;						//�˿˴�С
CSkinImage				CCardControl::m_ImageCard;						//ͼƬ��Դ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCardControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//���ñ���
	m_bDisplay=false;
	m_bHorizontal=true;
	m_bPositively=false;

	//�������
	m_dwCardHSpace=20;
	m_dwCardVSpace=19;
	m_dwShootAltitude=20;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//���б���
	m_dwCurrentIndex=0xFFFFFFFF;

	//�ڲ�����
	m_pSinkWindow=NULL;

	//��Դ����
	if (m_bLoad==false)
	{
		//������Դ
		m_bLoad=true;
		if (GetSystemMetrics(SM_CXSCREEN)>=1024) m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());
		//else m_ImageCard.SetLoadInfo(IDB_CARD_800,AfxGetInstanceHandle());

		//��ȡ��С
		CImageHandle CardImageHandle(&m_ImageCard);
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;
	}
	m_MoveFlag = false;
	m_BeginPoint.x = 0;
	m_BeginPoint.y = 0;
	m_EndPoint.x = 0;
	m_EndPoint.y = 0;
	m_lastPoint.x = 0;
	m_lastPoint.y = 0;

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//���ô���
void CCardControl::SetSinkWindow(CWnd * pSinkWindow)
{
	//���ñ���
	m_pSinkWindow=pSinkWindow;

	return;
}

//���÷���
void CCardControl::SetDirection(bool bHorizontal)
{
	//״̬�ж�
	if (m_bHorizontal==bHorizontal) return;

	//���ñ���
	m_bHorizontal=bHorizontal;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//�����˿�
void CCardControl::ShootAllCard(bool bShoot)
{
	//��������
	bool bRectify=false;
	tagCardItem * pCardItem=NULL;
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//�����˿�
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//�˿˷���
		if (pCardItem->bShoot!=bShoot)
		{
			bRectify=true;
			pCardItem->bShoot=bShoot;
		}
	}

	//�����ؼ�
	if (bRectify==true) RectifyControl();

	return;
}

//������ʾ
void CCardControl::SetDisplayFlag(bool bDisplay)
{
	//״̬�ж�
	if (m_bDisplay==bDisplay) return;

	//���ñ���
	m_bDisplay=bDisplay;

	//�ػ��ؼ�
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������Ӧ
void CCardControl::SetPositively(bool bPositively)
{
	//���ñ���
	m_bPositively=bPositively;

	return;
}

//���ü��
void CCardControl::SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace, DWORD dwShootAltitude)
{
	//���ñ���
	m_dwCardHSpace=dwCardHSpace;
	m_dwCardVSpace=dwCardVSpace;
	m_dwShootAltitude=dwShootAltitude;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��ȡ��Ŀ
DWORD CCardControl::GetCardCount()
{
	return (DWORD)m_CardDataItem.GetCount();
}

//��ȡ�˿�
DWORD CCardControl::GetShootCard(BYTE bCardData[], DWORD dwMaxCount)
{
	//��������
	DWORD bShootCount=0L;
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//�����˿�
	tagCardItem * pCardItem=NULL;
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//�˿˷���
		if (pCardItem->bShoot==true) bCardData[bShootCount++]=pCardItem->bCardData;
	}

	return bShootCount;
}

//�����˿�
DWORD CCardControl::SetCardData(const BYTE bCardData[], DWORD dwCardCount)
{
	//�����˿�
	m_CardDataItem.SetSize(dwCardCount);
	for (DWORD i=0;i<dwCardCount;i++)
	{
		m_CardDataItem[i].bShoot=false;
		m_CardDataItem[i].bCardData=bCardData[i];
	}

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return dwCardCount;
}

//��ȡ�˿�
DWORD CCardControl::GetCardData(tagCardItem CardItemArray[], DWORD dwMaxCount)
{
	//Ч�����
	ASSERT(dwMaxCount>=(DWORD)m_CardDataItem.GetCount());

	//�����˿�
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	tagCardItem * pCardItemData=m_CardDataItem.GetData();
	CopyMemory(CardItemArray,pCardItemData,dwCardCount*sizeof(tagCardItem));

	return dwCardCount;
}

//�����˿�
DWORD CCardControl::SetCardItem(const tagCardItem CardItemArray[], DWORD dwCardCount)
{
	//�����˿�
	m_CardDataItem.SetSize(dwCardCount);
	CopyMemory(m_CardDataItem.GetData(),CardItemArray,dwCardCount*sizeof(tagCardItem));

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();
 
	return 0;
}

//��������
BYTE CCardControl::HitCardTest(CPoint & Point)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sHitCardTest.log",strTime);

	strTemp.Format("into HitCardTest");
	WriteLog(strFile, strTemp);

//	BYTE							m_bCardCount;					//�˿���Ŀ
	DWORD m_bCardCount=(DWORD)m_CardDataItem.GetCount();

	strTemp.Format("m_bCardCount=%d", m_bCardCount);
	WriteLog(strFile, strTemp);

	if (m_bHorizontal==true)
	{
		//Ѱ�Ұ����˿�
		BYTE bCardIndex=(BYTE)(Point.x/m_dwCardHSpace);
		if (bCardIndex>=m_bCardCount) bCardIndex=m_bCardCount-1;

		strTemp.Format("bCardIndex=%d", bCardIndex);
		WriteLog(strFile, strTemp);

		//�жϰ���λ��
		bool bHitThisCard=true;
		BYTE bCardUp= m_CardDataItem[bCardIndex].bShoot;//m_CardUpArray.GetAt(bCardIndex);

		strTemp.Format("BYTE bCardUp");
		WriteLog(strFile, strTemp);

		if ((bCardUp==TRUE)&&(Point.y>m_dwCardHight)) 
		{
		strTemp.Format("BYTE bCardUp22");
		WriteLog(strFile, strTemp);
	//	m_CardDataItem[bCardIndex].bShoot = !m_CardDataItem[bCardIndex].bShoot;
			bHitThisCard=false;
		}
		if ((bCardUp==FALSE)&&(Point.y<m_dwShootAltitude)) 
		{
		strTemp.Format("BYTE bCardUp33");
		WriteLog(strFile, strTemp);
			bHitThisCard=false;
		}

		strTemp.Format("BYTE bCardUp2");
		WriteLog(strFile, strTemp);


		//Ѱ��ǰ������˿�
		if (bHitThisCard==false)
		{
			BYTE bFindCount=m_dwCardWidth/m_dwCardHSpace+1;
			for (BYTE i=1;i<bFindCount;i++)
			{
				strTemp.Format("i=%d", i);
				WriteLog(strFile, strTemp);
				if (bCardIndex==0) break;
				bCardUp= m_CardDataItem[bCardIndex].bShoot;//m_CardUpArray.GetAt(bCardIndex-i);
				if ((bCardUp==FALSE)&&(Point.y>m_dwCardWidth)) 
				{
					strTemp.Format("(bCardUp==FALSE)&&(Point.y>m_dwCardWidth)");
					WriteLog(strFile, strTemp);
								if (!m_CardDataItem[bCardIndex].bShoot)
			{
						strTemp.Format("true2");
			WriteLog(strFile, strTemp);
			}
			else
			{
						strTemp.Format("false2");
			WriteLog(strFile, strTemp);
			
			}

					m_CardDataItem[bCardIndex].bShoot = !m_CardDataItem[bCardIndex].bShoot;
					return bCardIndex-i;
				}
				if ((bCardUp==TRUE)&&(Point.y<20)) 
				{
					strTemp.Format("(bCardUp==TRUE)&&(Point.y<20)");
					WriteLog(strFile, strTemp);
			if (!m_CardDataItem[bCardIndex].bShoot)
			{
						strTemp.Format("true1");
			WriteLog(strFile, strTemp);
			}
			else
			{
						strTemp.Format("false1");
			WriteLog(strFile, strTemp);
			
			}

					m_CardDataItem[bCardIndex].bShoot = !m_CardDataItem[bCardIndex].bShoot;
					return bCardIndex-i;
				}

			if (!m_CardDataItem[bCardIndex].bShoot)
			{
						strTemp.Format("true3");
			WriteLog(strFile, strTemp);
			}
			else
			{
						strTemp.Format("false3");
			WriteLog(strFile, strTemp);
			
			}

				m_CardDataItem[bCardIndex].bShoot = !m_CardDataItem[bCardIndex].bShoot;
			}//End for

			return 255;
		}//End if
		else
		{
			strTemp.Format("if (bHitThisCard==true");
			WriteLog(strFile, strTemp);

			if (!m_CardDataItem[bCardIndex].bShoot)
			{
						strTemp.Format("true");
			WriteLog(strFile, strTemp);
			}
			else
			{
						strTemp.Format("false");
			WriteLog(strFile, strTemp);
			
			}
			m_CardDataItem[bCardIndex].bShoot = !m_CardDataItem[bCardIndex].bShoot;
		}

		strTemp.Format("BYTE bCardUp3");
		WriteLog(strFile, strTemp);
		return bCardIndex;
	}//End if

	return 255;
}
//���õ����˿�
DWORD CCardControl::SetShootCard(const BYTE bCardDataIndex[], DWORD dwCardCount)
{
	for(DWORD i=0;i<dwCardCount;i++)
	{
		tagCardItem * pCardItem=&m_CardDataItem[bCardDataIndex[i]];
		pCardItem->bShoot=true;
		if (m_hWnd!=NULL) RectifyControl();
	}
	return 0;
}

//����λ��
void CCardControl::RectifyControl()
{
	//��������
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//�����С
	CSize ControlSize;
	if (m_bHorizontal==true)
	{
		ControlSize.cy=m_CardSize.cy+m_dwShootAltitude;
		ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;
	}
	else
	{
		ControlSize.cx=m_CardSize.cx;
		ControlSize.cy=(dwCardCount>0)?(m_CardSize.cy+(dwCardCount-1)*m_dwCardVSpace):0;
	}

	//����λ��
	int nXPos=0;
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
	case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//����λ��
	int nYPos=0;
	switch (m_YCollocateMode)
	{
	case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
	case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	//�ƶ�λ��
	SetWindowPos(NULL,nXPos,nYPos,ControlSize.cx,ControlSize.cy,SWP_NOZORDER);

	//��������
	CRgn CardRegion,SignedRegion;
	tagCardItem * pCardItem=NULL;

	//��������
	CardRegion.CreateRectRgn(0,0,0,0);
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//����λ��
		if (m_bHorizontal==true)
		{
			nXPos=m_dwCardHSpace*i;
			nYPos=(pCardItem->bShoot==false)?m_dwShootAltitude:0;
		}
		else 
		{
			nXPos=0;
			nYPos=m_dwCardVSpace*i;
		}

		//�ϲ�����
		SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+m_CardSize.cx+1,nYPos+m_CardSize.cy+1,2,2);
		CardRegion.CombineRgn(&CardRegion,&SignedRegion,RGN_OR);
		SignedRegion.DeleteObject();
	}

	//��������
	SetWindowRgn(CardRegion,TRUE);
	m_CardRegion.DeleteObject();
	m_CardRegion.Attach(CardRegion.Detach());

	//�ػ�����
	Invalidate(TRUE);

	return;
}

//�����л�
DWORD CCardControl::SwitchCardPoint(const CPoint & MousePoint)
{
	if (m_bHorizontal==true)
	{
		//��������
		DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

		//��ȡ����
		DWORD dwCardIndex=MousePoint.x/m_dwCardHSpace;
		if (dwCardIndex>=dwCardCount) dwCardIndex=(dwCardCount-1);

		//�жϰ���
		bool bCurrentCard=true;
		bool bCardShoot=m_CardDataItem[dwCardIndex].bShoot;
		if ((bCardShoot==true)&&(MousePoint.y>m_CardSize.cy)) bCurrentCard=false;
		if ((bCardShoot==false)&&(MousePoint.y<(INT)m_dwShootAltitude)) bCurrentCard=false;

		//��ǰѰ��
		if (bCurrentCard==false)
		{
			while (dwCardIndex>0)
			{
				dwCardIndex--;
				bCardShoot=m_CardDataItem[dwCardIndex].bShoot;
				if ((bCardShoot==false)&&(MousePoint.y>m_CardSize.cx)) break;
				if ((bCardShoot==true)&&(MousePoint.y<(INT)m_dwShootAltitude)) break;
			}
		}

		return dwCardIndex;
	}

	return DWORD(-1);
}

//�ػ�����
void CCardControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λͼ
	CImageHandle HandleCard(&m_ImageCard);

	//����λͼ
	CBitmap BufferImage;
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&BufferImage);

	//�滭�˿�
	DWORD dwXImagePos,dwYImagePos;
	DWORD dwXScreenPos,dwYScreenPos;
	for (INT i=0;i<m_CardDataItem.GetCount();i++)
	{
		//��ȡ�˿�
		tagCardItem * pCardItem=&m_CardDataItem[i];

		//ͼƬλ��
		if ((m_bDisplay==true)&&(pCardItem->bCardData!=0))
		{
			dwXImagePos=((pCardItem->bCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((pCardItem->bCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			dwXImagePos=m_CardSize.cx*2;
			dwYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		if (m_bHorizontal==true) 
		{
			dwXScreenPos=m_dwCardHSpace*i;
			dwYScreenPos=(pCardItem->bShoot==false)?m_dwShootAltitude:0;
		}
		else
		{
			dwXScreenPos=0;
			dwYScreenPos=m_dwCardVSpace*i;
		}

		//�滭�˿�
		m_ImageCard.BitBlt(BufferDC,dwXScreenPos,dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//������Ϣ
int CCardControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//�����Ϣ
void CCardControl::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//״̬�ж�
	if ((m_bHorizontal==false)||(m_bPositively==false)) return;

	//������Ϣ
	if (m_pSinkWindow!=NULL) m_pSinkWindow->PostMessage(IDM_RIGHT_HIT_CARD,0,0);

	return;
}
// ˫��ѡ��
void CCardControl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/*
	if ((m_bPositively==true)&&(m_bHotHitCard!=255))
	{		
		BYTE bUpCard;		
		for(BYTE i=0;i<m_bCardCount;i++)
		{
			// ƥ���뽹������ͬ��
			if( ((m_CardArray[i]&UG_VALUE_MASK)+1)%15== ((m_CardArray[m_bHotHitCard]&UG_VALUE_MASK)+1)%15 )
			{				
				bUpCard=m_CardUpArray.GetAt(i);
				m_CardUpArray.SetAt(i,(bUpCard==TRUE)?FALSE:TRUE);				
			}
		}		
	//if (m_pHandleWnd!=NULL) m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD);//�Ƿ�ɳ�
	}
	*/
	return;
}
//�����Ϣ
void CCardControl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//״̬�ж�
	if ((m_bHorizontal==false)||(m_bPositively==false)) return;
	if ((m_bPositively==true)&& (m_bHotHitCard!=255))
	{
		m_EndPoint = Point;
		if (m_MoveFlag && m_BeginPoint.x != m_EndPoint.x)
		{//AfxMessageBox("GH");
			MoveSelect();
			m_MoveFlag = false;
			return;
		}

		//�ػ�ʧЧ����
		Invalidate(FALSE);
	}//End if

	m_MoveFlag = false;

	if (m_dwCurrentIndex==0xFFFFFFFF) return;

	//��ȡ����
	DWORD dwCurrentIndex=SwitchCardPoint(Point);
	if (dwCurrentIndex!=m_dwCurrentIndex) return;

	//���ÿؼ�
	tagCardItem * pCardItem=&m_CardDataItem[dwCurrentIndex];
	pCardItem->bShoot=!pCardItem->bShoot;

	//������Ϣ
	if (m_pSinkWindow!=NULL) m_pSinkWindow->PostMessage(IDM_LEFT_HIT_CARD,0,0);

	//�����ؼ�
	RectifyControl();

	return;
}

//�����Ϣ
void CCardControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//״̬�ж�
	if ((m_bHorizontal==false)||(m_bPositively==false)) return;

	//ѡ����
	m_BeginPoint = Point;
	m_MoveFlag = true;
	m_lastPoint = Point;
	//��ȡ����
	m_dwCurrentIndex=SwitchCardPoint(Point);

	return;
}

//�����Ϣ
BOOL CCardControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//���ù��
	if (m_bPositively==true)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,message);
}
// ����϶�ѡ��
void CCardControl::MoveSelect(void)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sMoveSelect.log",strTime);

	strTemp.Format("into MoveSelect");
	WriteLog(strFile, strTemp);

	CPoint now_Point;
	CPoint tempPoint;
	BYTE bHitCard;
	BYTE bUpCard;
	BYTE tempCard;
	int flag = 0;	

	if (m_BeginPoint.x < m_EndPoint.x)
	{
	strTemp.Format("m_BeginPoint.x(%d) < m_EndPoint.x(%d)", m_BeginPoint.x , m_EndPoint.x);
	WriteLog(strFile, strTemp);

		for(int i = m_BeginPoint.x; i <= m_EndPoint.x; i += m_dwCardHSpace)
		{

	strTemp.Format("i=%d", i);
	WriteLog(strFile, strTemp);

			now_Point.x = i;
			now_Point.y = 50;
			bHitCard=HitCardTest(now_Point);

			if (bHitCard<0) //||bHitCard>m_CardUpArray.GetUpperBound())
				return;
			if (flag)
			{
				tempCard = HitCardTest(tempPoint);
				if (tempCard == bHitCard)
					break;
			}		

	strTemp.Format("bUpCard=%d", bUpCard);
	WriteLog(strFile, strTemp);

			//bUpCard=m_CardUpArray.GetAt(bHitCard);
			//m_CardUpArray.SetAt(bHitCard,(bUpCard==TRUE)?FALSE:TRUE);
						
			tempPoint = now_Point;
			flag = 1;
		}//End for

		BYTE bEndCard = HitCardTest(m_EndPoint);
		if (bEndCard<0)//||bEndCard>m_CardUpArray.GetUpperBound())
			return;
		if(bEndCard != bHitCard)
		{
			//bUpCard=m_CardUpArray.GetAt(bEndCard);
			//m_CardUpArray.SetAt(bEndCard,(bUpCard==TRUE)?FALSE:TRUE);
		}
		

		RectifyControl();

	}
	else
		{

		for(int i = m_EndPoint.x; i <= m_BeginPoint.x; i += m_dwCardHSpace)
		{
			now_Point.x = i;
			now_Point.y = 50;				
			bHitCard=HitCardTest(now_Point);
			if (bHitCard<0) //||bHitCard>m_CardUpArray.GetUpperBound())
				return;
			if (flag)
			{
				tempCard = HitCardTest(tempPoint);
				if (tempCard == bHitCard)
					break;
			}
			
			//bUpCard=m_CardUpArray.GetAt(bHitCard);
			//m_CardUpArray.SetAt(bHitCard,(bUpCard==TRUE)?FALSE:TRUE);
			tempPoint = now_Point;
			flag = 1;
		}//End for
		BYTE bEndCard = HitCardTest(m_BeginPoint);
		if (bEndCard<0)//||bEndCard>m_CardUpArray.GetUpperBound())
			return;
		if(bEndCard != bHitCard)
		{
			//bUpCard=m_CardUpArray.GetAt(bEndCard);
			//m_CardUpArray.SetAt(bEndCard,(bUpCard==TRUE)?FALSE:TRUE);
		}
		RectifyControl();
	}//End if
	// ����Ƿ�ɳ���
//Doctor if (m_pHandleWnd!=NULL) m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD);
	//������Ϣ
	if (m_pSinkWindow!=NULL) m_pSinkWindow->PostMessage(IDM_LEFT_HIT_CARD,0,0);
	return;	
}

//////////////////////////////////////////////////////////////////////////
void CCardControl::OnMouseMove(UINT nFlags, CPoint point)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sOnMouseMove.log",strTime);

	strTemp.Format("into OnMouseMove");
//	WriteLog(strFile, strTemp);

	if (m_MoveFlag)
	{
		/*CPaintDC dc(this);
		CPen pen;
		pen.CreatePen(PS_SOLID,2,RGB(255,128,128));
		dc.SelectObject(&pen);
		dc.SelectStockObject(NULL_BRUSH);
		//dc.SetROP2(R2_XORPEN);		
		dc.Rectangle(CRect(m_BeginPoint.x, m_BeginPoint.y, point.x, point.y));*/

		CDC *pDC = GetDC();
		pDC->SetROP2(R2_NOTXORPEN);
		WriteLine(m_BeginPoint, m_lastPoint, pDC);
		WriteLine(m_BeginPoint, point, pDC);
		m_lastPoint = point;
	} 
   else
   {
	   Invalidate(FALSE);//
	 }

}
void CCardControl::WriteLine(CPoint beginPoint, CPoint endPoint, CDC * pDC)
{
	int i, j;
	if (beginPoint.x < endPoint.x)
	{
		for (i = beginPoint.x; i < endPoint.x; i++)
		{
			pDC->SetPixel(i, beginPoint.y, RGB(0,0,255));
			pDC->SetPixel(i, endPoint.y, RGB(0,0,255));
		}
	}
	else if(beginPoint.x > endPoint.x)
	{
		for (i = beginPoint.x; i > endPoint.x; i--)
		{
			pDC->SetPixel(i, beginPoint.y, RGB(0,0,255));
			pDC->SetPixel(i, endPoint.y, RGB(0,0,255));
		}
	}
	
	if (beginPoint.y < endPoint.y)
	{
		for (j = beginPoint.y; j < endPoint.y; j++)
		{
			pDC->SetPixel(beginPoint.x, j, RGB(0,0,255));
			pDC->SetPixel(endPoint.x, j, RGB(0,0,255));
		}
	}
	else if (beginPoint.y > endPoint.y)
	{
		for (j = beginPoint.y; j > endPoint.y; j--)
		{
			pDC->SetPixel(beginPoint.x, j, RGB(0,0,255));
			pDC->SetPixel(endPoint.x, j, RGB(0,0,255));
		}
	}
	return;
}
