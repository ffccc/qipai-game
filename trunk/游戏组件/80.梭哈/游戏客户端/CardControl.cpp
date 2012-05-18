#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define CARD_SPACE					20								//�˿˼��
#define CARD_WIDTH					70								//�˿˿��
#define CARD_HEIGHT					95								//�˿˸߶�

//��̬����
bool			CCardControl::m_bInit=false;						//��ʼ��־
CSkinImage		CCardControl::m_ImageCard;							//�˿�ͼƬ
CSkinImage		CCardControl::m_ImageBack;							//�˿�ͼƬ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCardControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bShowFirst=false;
	m_bPositively=false;
	m_bCaptureMouse=false;

	//λ�ñ���
	m_XCollocateMode=enXLeft;
	m_YCollocateMode=enYTop;

	//��Դ����
	if (m_bInit==false)
	{
		m_bInit=true;
		m_ImageCard.SetLoadInfo(IDB_CARD,AfxGetInstanceHandle());
		m_ImageBack.SetLoadInfo(IDB_CARD_BACK,AfxGetInstanceHandle());
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
	CImage ImageBuffer;
	CImageHandle HandleCard(&m_ImageCard);
	CImageHandle HandleFundus(&m_ImageBack);
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),16);
	if (ImageBuffer.IsNull()==true) return;

	//�滭�˿�
	bool bDrawCard=false;
	CDC * pBufferDC=CDC::FromHandle(ImageBuffer.GetDC());
	for (int i=0;i<m_CardDataArray.GetCount();i++)
	{
		bDrawCard=((i==0)&&(m_bShowFirst==false))?false:true;
		if ((bDrawCard==true)&&(m_CardDataArray[i]!=0))
		{
			BYTE bCardData=m_CardDataArray[i];
			m_ImageCard.AlphaDrawImage(pBufferDC,i*CARD_SPACE,0,CARD_WIDTH,CARD_HEIGHT,
				GetCardXPos(bCardData),GetCardYPos(bCardData),RGB(255,0,255));
		}
		else
		{
			m_ImageBack.AlphaDrawImage(pBufferDC,i*CARD_SPACE,0,CARD_WIDTH,CARD_HEIGHT,0,0,RGB(255,0,255));
		}
	}
	ImageBuffer.ReleaseDC();
	ImageBuffer.BitBlt(dc,0,0);

	//������Դ
	ImageBuffer.Destroy();

	return;
}

//�����Ϣ
void CCardControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);

	//״̬�ж�
	if (m_bCaptureMouse==false) return;

	//�ͷŲ���
	ReleaseCapture();
	m_bCaptureMouse=false;

	//ˢ�½���
	CRect rcInvalid(0,0,(m_CardDataArray.GetCount()==1)?CARD_WIDTH:CARD_SPACE,CARD_HEIGHT);
	InvalidateRect(rcInvalid,FALSE);

	return;
}

//�����Ϣ
void CCardControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);

	//״̬�ж�
	if ((m_bPositively==false)||(m_bShowFirst==true)) return;
	if ((point.x>CARD_SPACE)||(m_CardDataArray.GetCount()==0)) return;

	//��������
	CClientDC ClientDC(this);
	BYTE bCardData=m_CardDataArray[0];
	CImageHandle HandleCard(&m_ImageCard);
	int nDrawWidth=(m_CardDataArray.GetCount()==1)?CARD_WIDTH:CARD_SPACE;

	//�������
	SetCapture();
	m_bCaptureMouse=true;

	//�滭�˿�
	int nXPos=GetCardXPos(bCardData);
	int nYPos=GetCardYPos(bCardData);
	m_ImageCard.AlphaDrawImage(&ClientDC,0,0,nDrawWidth,CARD_HEIGHT,nXPos,nYPos,RGB(255,0,255));

	return;
}

//�����Ϣ
BOOL CCardControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	if ((m_bPositively==true)&&(m_bShowFirst==false))
	{
		//��ȡ���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//λ���ж�
		if (MousePoint.x<CARD_SPACE)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return TRUE;
		}
	}
	
	return __super::OnSetCursor(pWnd,nHitTest,message);
}

//��ȡ��Ŀ
WORD CCardControl::GetCardCount()
{
	return (WORD)m_CardDataArray.GetCount();
}

//��ʾ�˿�
void CCardControl::ShowFirstCard(bool bShowFirst)
{
	//���ñ���
	m_bShowFirst=bShowFirst;

	//ˢ�½���
	CRect rcInvalid(0,0,(m_CardDataArray.GetCount()==1)?CARD_WIDTH:CARD_SPACE,CARD_HEIGHT);
	InvalidateRect(rcInvalid,FALSE);

	return;
}

//������Ӧ
void CCardControl::AllowPositively(bool bPositively)
{
	m_bPositively=bPositively;
	return;
}

//�����˿�
WORD CCardControl::GetCardData(BYTE bCardBuffer[], WORD wBufferSize)
{
	//��������
	WORD wCopyCount=(WORD)m_CardDataArray.GetCount();
	if (wCopyCount>(wBufferSize/sizeof(BYTE))) wCopyCount=wBufferSize/sizeof(BYTE);
	CopyMemory(bCardBuffer,m_CardDataArray.GetData(),wCopyCount);

	return (WORD)m_CardDataArray.GetCount();
}

//�����˿�
bool CCardControl::SetCardData(const BYTE bCardData[], WORD wCardCount)
{
	//�����˿�
	m_CardDataArray.RemoveAll();
	m_CardDataArray.SetSize(wCardCount);
	CopyMemory(m_CardDataArray.GetData(),bCardData,sizeof(BYTE)*wCardCount);

	//����λ��
	RectifyControl();

	return 0;
}

//���û�׼�㺯��
void CCardControl::SetBasicStation(CPoint & BasicStation, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BasicStation=BasicStation;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//����λ��
	RectifyControl();

	return;
}

//��ȡ�˿���ֵ
BYTE CCardControl::GetCardValue(BYTE bCardData)
{
	BYTE bCardVolue=(bCardData-1)%13;
	if (bCardVolue==0) bCardVolue=14;
	return bCardVolue;
}

//�˿�λ��
WORD CCardControl::GetCardXPos(BYTE bCardData)
{
	BYTE cbCardValue=bCardData&0x0F;
	if (cbCardValue==1) return 6*CARD_WIDTH;
	return (cbCardValue-8)*CARD_WIDTH;
}

//�˿�λ��
WORD CCardControl::GetCardYPos(BYTE bCardData)
{
	return ((bCardData&0xF0)>>4)*CARD_HEIGHT;
}

//��������λ��
void CCardControl::RectifyControl()
{
	//��ȡ��С 
	INT_PTR nCardCount=(int)m_CardDataArray.GetCount();
	CSize CardSize((nCardCount>0)?(CARD_WIDTH+(int)(nCardCount-1)*CARD_SPACE):0,CARD_HEIGHT);

	//����λ��
	CRect rcCardRect;
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ rcCardRect.left=m_BasicStation.x; break; }
	case enXCenter: { rcCardRect.left=m_BasicStation.x-CardSize.cx/2; break; }
	case enXRight:	{ rcCardRect.left=m_BasicStation.x-CardSize.cx; break; }
	}

	switch (m_YCollocateMode)
	{
	case enYTop:	{ rcCardRect.top=m_BasicStation.y; break; }
	case enYCenter: { rcCardRect.top=m_BasicStation.y-CardSize.cy/2; break; }
	case enYBottom: { rcCardRect.top=m_BasicStation.y-CardSize.cy; break; }
	}

	//�ƶ�λ��
	rcCardRect.right=rcCardRect.left+CardSize.cx;
	rcCardRect.bottom=rcCardRect.top+CardSize.cy;
	MoveWindow(&rcCardRect,FALSE);

	//��������
	CRgn AllRgn,SignedRgn;
	AllRgn.CreateRectRgn(0,0,0,0);
	for (int i=0;i<nCardCount;i++)
	{
		SignedRgn.CreateRoundRectRgn(i*CARD_SPACE,0,i*CARD_SPACE+CARD_WIDTH+1,CARD_HEIGHT+1,2,2);
		AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
		SignedRgn.DeleteObject();
	}

	//��������
	SetWindowRgn(AllRgn,TRUE);
	m_CardRgn.DeleteObject();
	m_CardRgn.Attach(AllRgn.Detach());

	//�ػ�����
	Invalidate(FALSE);

	return;
}

//��ȡ��βλ��
CPoint CCardControl::GetTailPos()
{
	CPoint ptTail(0,0);;

	//��������
	DWORD dwCardCount=(DWORD)m_CardDataArray.GetCount();

	//�����С
	CSize ControlSize;
	ControlSize.cy=CARD_HEIGHT;
	ControlSize.cx=(dwCardCount>0)?(CARD_WIDTH+(dwCardCount-1)*CARD_SPACE):0;
	INT nSpaceWidth = dwCardCount*CARD_SPACE;

	//����λ��
	int nXPos=0;
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ ptTail.x=m_BasicStation.x+nSpaceWidth; break; }
	case enXCenter: { ptTail.x=m_BasicStation.x-ControlSize.cx/2+nSpaceWidth; break; }
	case enXRight:	{ ptTail.x=m_BasicStation.x-ControlSize.cx+nSpaceWidth; break; }
	}

	//����λ��
	int nYPos=0;
	switch (m_YCollocateMode)
	{
	case enYTop:	{ ptTail.y=m_BasicStation.y; break; }
	case enYCenter: { ptTail.y=m_BasicStation.y-ControlSize.cy/2; break; }
	case enYBottom: { ptTail.y=m_BasicStation.y-ControlSize.cy; break; }
	}

	return ptTail;
}

//////////////////////////////////////////////////////////////////////////
