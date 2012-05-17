#include "StdAfx.h"
#include "Resource.h"
#include "GameOption.h"
#include "CardControl.h"
#include ".\cardcontrol.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
bool					CCardControl::m_bLoad=false;					//��ʼ��־
CSize					CCardControl::m_CardSize;						//�˿˴�С
CSkinImage				CCardControl::m_ImageCard;						//�˿�ͼƬ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCardControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//���ñ���
	m_bPositively=false;
	m_bDisplayHead=false;
	m_bLookCard=false;
	m_bCompareCard=false;
	m_bCompareBack=false;
	m_wCardColor=INVALID_CHAIR;
	m_dwCardHSpace=DEFAULT_PELS;
	m_wControlID=INVALID_CHAIR;
	m_wMessageID=INVALID_CHAIR;

	//״̬����
	m_bCaptureMouse=false;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;

	//��Դ����
	if (m_bLoad==false)
	{
		//������Դ
		m_bLoad=true;
		m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());

		//��ȡ��С
		CImageHandle CardImageHandle(&m_ImageCard);
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;
	}

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//������Ϣ
int CCardControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
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
	for (INT_PTR i=0;i<m_CardDataArray.GetCount();i++)
	{
		//��������
		BYTE cbCardData=m_CardDataArray[i];
		bool bDrawCard=(m_bDisplayHead==true);

		//ͼƬλ��
		if(m_wCardColor!=INVALID_CHAIR)
		{
			//��������
			if(m_wCardColor==1)
			{
				dwXImagePos=m_CardSize.cx*3;
			}
			//���Ʊ���
			else if(m_wCardColor==2)
			{
				dwXImagePos=m_CardSize.cx*4;
			}
			//���Ƹ���
			else if(m_wCardColor==0)
			{
				dwXImagePos=m_CardSize.cx*2;
			}
			else if(m_wCardColor==3)
			{
				dwXImagePos=m_CardSize.cx*5;
			}
			else if(m_wCardColor==4)
			{
				dwXImagePos=m_CardSize.cx*0;
			}
			else if(m_wCardColor==5)
			{
				dwXImagePos=m_CardSize.cx*1;
			}
			dwYImagePos=m_CardSize.cy*4;
		}
		else if ((bDrawCard==true)&&(cbCardData!=0))
		{
			dwXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			if(!m_bCompareBack)		//����&���Ʊ���
			{
				dwXImagePos=m_CardSize.cx*((m_bLookCard)?6:2);
			}
			else					//���Ʊ���
			{
				dwXImagePos=m_CardSize.cx*5;
			}
			dwYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		dwYScreenPos=0;
		dwXScreenPos=m_dwCardHSpace*i;

		//�滭�˿�
		m_ImageCard.AlphaDrawImage(&BufferDC,dwXScreenPos,dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos,RGB(255,0,255));
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//��ȡ��Ŀ
DWORD CCardControl::GetCardCount()
{
	return (WORD)m_CardDataArray.GetCount();
}

//��ʾ�˿�
void CCardControl::SetDisplayHead(bool bDisplayHead)
{
	//״̬�ж�
	if (m_bDisplayHead==bDisplayHead) return;

	//���ñ���
	m_bDisplayHead=bDisplayHead;

	//�ػ��ؼ�
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//���ü��
void CCardControl::SetCardSpace(DWORD dwCardSpace)
{
	//���ñ���
	m_dwCardHSpace=dwCardSpace;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//������Ӧ
void CCardControl::SetPositively(bool bPositively)
{
	//���ñ���
	m_bPositively=bPositively;

	return;
}

//�����˿�
DWORD CCardControl::GetCardData(BYTE cbCardData[], DWORD dwMaxCount)
{
	//�����˿�
	DWORD dwCopyCount=__min((DWORD)m_CardDataArray.GetCount(),dwMaxCount);
	CopyMemory(cbCardData,m_CardDataArray.GetData(),sizeof(BYTE)*dwCopyCount);

	return dwCopyCount;
}

//�����˿�
DWORD CCardControl::SetCardData(const BYTE cbCardData[], DWORD dwCardCount)
{
	//�����˿�
	m_CardDataArray.SetSize(dwCardCount);
	CopyMemory(m_CardDataArray.GetData(),cbCardData,sizeof(BYTE)*dwCardCount);

	//����λ��
	RectifyControl();

	return dwCardCount;
}

//�����˿�
void CCardControl::SetCardColor(WORD wColor)
{
	m_wCardColor=wColor;

	//�����ؼ�
	RectifyControl();
	return;
}

//�����˿�
void CCardControl::SetLookCard(bool bLook)
{
	m_bLookCard=bLook;

	//�����ؼ�
	RectifyControl();

	//�ػ�����
	//Invalidate(FALSE);
	return;
}

//�����˿�
void CCardControl::SetCompareCard(bool bCompare)
{
	m_bCompareCard=bCompare;

	//�����ؼ�
	RectifyControl();
	return ;
}

//�����˿�
void CCardControl::SetCompareBack(bool bShow)
{
	m_bCompareBack=bShow;

	//�����ؼ�
	RectifyControl();
	return ;
}

//�����˿�
void CCardControl::SetCommandID(WORD ControlID,WORD wMessageID)
{
	m_wMessageID=wMessageID;
	m_wControlID=ControlID;
	return ;
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

//��ȡλ��
CPoint CCardControl::GetCardPos()
{
	//��������
	DWORD dwCardCount=(DWORD)m_CardDataArray.GetCount();

	//�����С
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

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

	CPoint cpTemp;
	cpTemp.x=nXPos;
	cpTemp.y=nYPos;

	return cpTemp;
}

//�����ؼ�
void CCardControl::RectifyControl()
{
	//��������
	DWORD dwCardCount=(DWORD)m_CardDataArray.GetCount();

	//�����С
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

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
	if(nXPos<0 || nYPos<0)
	{
		nXPos=0;nYPos=0;
	}
	SetWindowPos(NULL,nXPos,nYPos,ControlSize.cx,ControlSize.cy,SWP_NOZORDER);

	//��������
	CRgn SignedRegion;
	CRgn CardRegion;
	CardRegion.CreateRectRgn(0,0,0,0);
	for (DWORD i=0;i<dwCardCount;i++)
	{
		SignedRegion.CreateRoundRectRgn(i*m_dwCardHSpace,0,i*m_dwCardHSpace+m_CardSize.cx+1,m_CardSize.cy+1,2,2);
		CardRegion.CombineRgn(&CardRegion,&SignedRegion,RGN_OR);
		SignedRegion.DeleteObject();
	}

	//��������
	SetWindowRgn(CardRegion,TRUE);
	m_CardRegion.DeleteObject();
	m_CardRegion.Attach(CardRegion.Detach());

	//�ػ�����
	Invalidate(FALSE);

	return;
}

//���ù��
BOOL CCardControl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//ѡ��״̬
	if(m_bCompareCard)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		return TRUE;
	}
	
	return __super::OnSetCursor(pWnd,nHitTest,message);
}

//������
void CCardControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_wControlID!=INVALID_CHAIR && m_bCompareCard)
	{
		//�û�ID
		//TCHAR obj[32]=TEXT("");
		//_snprintf(obj,sizeof(obj),TEXT("%d:%ld"),m_wMessageID,m_wControlID);
		//MessageBox(obj);
		AfxGetMainWnd()->SendMessage(m_wMessageID,m_wControlID,m_wControlID);
	}
	return __super::OnLButtonDown(nFlags,point);
}
//////////////////////////////////////////////////////////////////////////

