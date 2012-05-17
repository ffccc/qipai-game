#include "Stdafx.h"
#include "Math.h"
#include "Resource.h"
#include "GoldControl.h"
#include ".\goldcontrol.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define CELL_WIDTH							23							//��Ԫ���
#define LESS_WIDTH							197							//��С���
#define CONTROL_HEIGHT						75							//�ؼ��߶�
#define LEFT_WIDTH							65							//��߿��
#define RIGHT_WIDTH							65							//�ұ߿��	
#define IMAGE_HEIGHT						39							//ͼƬ�߶�
#define IMAGE_WIDTH							23							//ͼƬ���

//��ť��ʶ
#define  IDC_MAXSCORE						101							//�����ע
#define  IDC_MINSCORE						102							//��С��ע
#define  IDC_OK								103							//ȷ����Ŧ
#define  IDC_CANCEL							104							//ȡ����Ŧ	

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGoldControl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_MINSCORE,OnMinScore)
	ON_BN_CLICKED(IDC_MAXSCORE,OnMaxScore)
	ON_BN_CLICKED(IDC_OK,OnOKScore)
	ON_BN_CLICKED(IDC_CANCEL,OnCancelScore)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGoldControl::CGoldControl()
{
	//�������
	m_nWidth=0;
	m_nCellCount=0;

	//���ñ���
	m_lMaxGold = 0L;
	m_lMinGold = 0L;
	ZeroMemory(m_lGoldCell,sizeof(m_lGoldCell));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageLeft.SetLoadInfo(IDB_LEFT,hInstance);
	m_ImageMid.SetLoadInfo(IDB_MID,hInstance);
	m_ImageRight.SetLoadInfo(IDB_RIGHT,hInstance);
	m_ImageNumber.SetLoadInfo(IDB_NUMBER,hInstance);
}

//��������
CGoldControl::~CGoldControl()
{
}

//��ȡ���
LONG CGoldControl::GetGold()
{
	LONG lGold=0L;
	for (LONG i=0;i<CountArray(m_lGoldCell);i++) 
		lGold+=m_lGoldCell[i]*(LONG)pow(10L,i);
	return lGold;
}

//���ý��
void CGoldControl::SetGold(LONG lGold)
{
	//��������
	if (lGold>m_lMaxGold) lGold=m_lMaxGold;
	ZeroMemory(m_lGoldCell,sizeof(m_lGoldCell));

	//���ñ���
	int nIndex=0;
	while (lGold>0L)
	{
		m_lGoldCell[nIndex++]=lGold%10L;
		lGold/=10L;
	}
	//��������
	RectifyControl();
	return;
}

//�����û���С��ע
void CGoldControl::SetMinGold(LONG lMinGold)
{
	//У��ı�
	if (lMinGold>m_lMaxGold) return;
	if((lMinGold == m_lMinGold)||(lMinGold <0L)) return;
	ZeroMemory(m_lGoldCell,sizeof(m_lGoldCell));

	//���ñ���
	m_lMinGold = lMinGold;
	//���ñ���
	int nIndex=0;
	while (lMinGold>0L)
	{
		m_lGoldCell[nIndex++]=lMinGold%10L;
		lMinGold/=10L;
	}
	//��������
	RectifyControl();

	return;
}

//�����û������ע��
void CGoldControl::SetMaxGold(LONG lMaxGold)
{
	//Ч��ı�
	if (m_lMaxGold==lMaxGold) return;

	//���ñ���
	m_lMaxGold=lMaxGold;
	if (m_lMaxGold>9999999L) m_lMaxGold=9999999L;
	ZeroMemory(m_lGoldCell,sizeof(m_lGoldCell));

	//���㵥Ԫ
	m_nCellCount=0;
	while (lMaxGold>0L)
	{
		lMaxGold/=10L;
		m_nCellCount++;
	}
	m_nCellCount=__min(CountArray(m_lGoldCell),__max(m_nCellCount,1));

	//���ý���
	m_nWidth=(m_nCellCount+(m_nCellCount-1)/3)*CELL_WIDTH+LEFT_WIDTH+RIGHT_WIDTH;

	MoveWindow(m_BasicPoint.x-m_nWidth,m_BasicPoint.y-CONTROL_HEIGHT,m_nWidth,CONTROL_HEIGHT);
	return;
}


//����λ��
void CGoldControl::SetBasicPoint(int nXPos, int nYPos)
{
	//���ñ���
	m_BasicPoint.x=nXPos;
	m_BasicPoint.y=nYPos;

	//��������
	RectifyControl();

	return;
}

//�����ؼ�
void CGoldControl::RectifyControl()
{
	MoveWindow(m_BasicPoint.x-m_nWidth,m_BasicPoint.y-CONTROL_HEIGHT,m_nWidth,CONTROL_HEIGHT);

	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//���ť
	m_btMaxScore.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btMaxScore,NULL,6,7,0,0,uFlags);

	//��С��ť
	DeferWindowPos(hDwp,m_btMinScore,NULL,6,CONTROL_HEIGHT-rcButton.Height()-9,0,0,uFlags);

	//ȷ����Ŧ
	DeferWindowPos(hDwp,m_btOK,NULL,m_nWidth-rcButton.Width()-6,7,0,0,uFlags);

	//��ע��ť
	DeferWindowPos(hDwp,m_btCancel,NULL, m_nWidth-rcButton.Width()-6,CONTROL_HEIGHT-rcButton.Height()-9,0,0,uFlags);


	//�����ƶ�
	EndDeferWindowPos(hDwp);
	return;
}

//�滭���
void CGoldControl::DrawGoldCell(CDC * pDC, int nXPos, int nYPox, LONG lGold)
{
	CImageHandle ImageNumberHandle(&m_ImageNumber);
	if (lGold<10L) 
	{

		m_ImageNumber.BitBlt(pDC->m_hDC,nXPos,nYPox,IMAGE_WIDTH,IMAGE_HEIGHT,lGold*IMAGE_WIDTH,0,SRCCOPY);
	}
	else
	{
		m_ImageNumber.BitBlt(pDC->m_hDC,nXPos,nYPox,IMAGE_WIDTH,IMAGE_HEIGHT,10*IMAGE_WIDTH,0,SRCCOPY);
	}

	return;
}

//�ػ�����
void CGoldControl::OnPaint()
{
	if(GetGold()>=m_lMinGold)m_btOK.EnableWindow(TRUE);
	else m_btOK.EnableWindow(FALSE);

	CPaintDC dc(this);

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//��������ͼ
	CDC BackFaceDC;
	CBitmap BufferBmp;
	BufferBmp.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height());
	BackFaceDC.CreateCompatibleDC(&dc);
	BackFaceDC.SelectObject(&BufferBmp);

	//������Դ
	CImageHandle ImageMidHandle(&m_ImageMid);
	CImageHandle ImageLeftHandle(&m_ImageLeft);
	CImageHandle ImageRightHandle(&m_ImageRight);

	//�滭����
	m_ImageLeft.BitBlt(BackFaceDC,0,0);
	m_ImageMid.StretchBlt(BackFaceDC,m_ImageLeft.GetWidth(),0,ClientRect.Width()-m_ImageLeft.GetWidth()-m_ImageRight.GetWidth(),m_ImageMid.GetHeight());
	m_ImageRight.BitBlt(BackFaceDC,ClientRect.Width()-m_ImageRight.GetWidth(),0);

	//�滭���
	int nXExcursion=ClientRect.Width()-RIGHT_WIDTH;
	for (int i=0;i<m_nCellCount;i++)
	{
		//�滭����
		if ((i!=0)&&(i%3)==0)
		{
			nXExcursion-=IMAGE_WIDTH;
			DrawGoldCell(&BackFaceDC,nXExcursion,12,10);
		}

		//�滭����
		nXExcursion-=IMAGE_WIDTH;
		DrawGoldCell(&BackFaceDC,nXExcursion,12,m_lGoldCell[i]);
	}

	//�滭����
	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&BackFaceDC,0,0,SRCCOPY);

	return;
}

//���������Ϣ
void CGoldControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	//��������
	int nViewCellCount=(m_nCellCount-1)/3+m_nCellCount;

	//λ�ù���
	int nHeadWidth=LEFT_WIDTH;
	if ((point.y<=5)||(point.y>=65)) return;
	if ((point.x<=nHeadWidth)||(point.x>=(CELL_WIDTH*nViewCellCount+nHeadWidth))) return;

	//��ť����
	int iCellPos=(nViewCellCount-(point.x-nHeadWidth)/CELL_WIDTH)-1;
	if ((iCellPos==3)||(iCellPos==7)) return;
	if (iCellPos>3) iCellPos=iCellPos-(iCellPos-1)/3;

	//��������
	ASSERT((iCellPos>=0)&&(iCellPos<CountArray(m_lGoldCell)));
	if (m_lGoldCell[iCellPos]!=9L)
	{
		LONG lAddGold=(LONG)pow(10L,iCellPos);
		if ((GetGold()+lAddGold)>m_lMaxGold) return;
	}
	//if (m_lGoldCell[iCellPos]==9L)
	//{
	//	LONG lAddGold=(LONG)pow(10L,iCellPos)*9;
	//	if((GetGold()-lAddGold <m_lMinGold)) return;
	//}

	//���ñ���
	m_lGoldCell[iCellPos]=(m_lGoldCell[iCellPos]+1)%10;
	int iCellPosTemp=(nViewCellCount-(point.x-nHeadWidth)/CELL_WIDTH)-1;

	if(iCellPosTemp>3)
		InvalidateRect(CRect(m_nWidth-(iCellPosTemp+1)*IMAGE_WIDTH -RIGHT_WIDTH,5,m_nWidth-iCellPosTemp*IMAGE_WIDTH-RIGHT_WIDTH,12+IMAGE_HEIGHT));
	else
		InvalidateRect(CRect(m_nWidth-(iCellPos+1)*IMAGE_WIDTH -RIGHT_WIDTH,5,m_nWidth-iCellPos*IMAGE_WIDTH-RIGHT_WIDTH,12+IMAGE_HEIGHT));
	
	return;
}

//�Ҽ�������Ϣ
void CGoldControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	//��������
	int nViewCellCount=(m_nCellCount-1)/3+m_nCellCount;

	//λ�ù���
	int nHeadWidth=LEFT_WIDTH;
	if ((point.y<=5)||(point.y>=65)) return;
	if ((point.x<=nHeadWidth)||(point.x>=(CELL_WIDTH*nViewCellCount+nHeadWidth))) return;

	//��ť����
	int iCellPos=(nViewCellCount-(point.x-nHeadWidth)/CELL_WIDTH)-1;
	if ((iCellPos==3)||(iCellPos==7)) return;
	if (iCellPos>3) iCellPos=iCellPos-(iCellPos-1)/3;

	//��������
	ASSERT((iCellPos>=0)&&(iCellPos<CountArray(m_lGoldCell)));
	if (m_lGoldCell[iCellPos]==0L)
	{
		LONG lAddGold=9L*(LONG)pow(10L,iCellPos);
		if ((GetGold()+lAddGold)>m_lMaxGold) return;
	}
	//if (m_lGoldCell[iCellPos]!=0L)
	//{
	//	LONG lAddGold=(LONG)pow(10L,iCellPos);
	//	if ((GetGold()-lAddGold)<m_lMinGold) return;

	//}

	//���ñ���
	m_lGoldCell[iCellPos]=(m_lGoldCell[iCellPos]+9)%10;

	int iCellPosTemp =(nViewCellCount-(point.x-nHeadWidth)/CELL_WIDTH)-1;

	if(iCellPosTemp>3)
		InvalidateRect(CRect(m_nWidth-(iCellPosTemp+1)*IMAGE_WIDTH -RIGHT_WIDTH,5,m_nWidth-iCellPosTemp*IMAGE_WIDTH-RIGHT_WIDTH,12+IMAGE_HEIGHT));
	else
		InvalidateRect(CRect(m_nWidth-(iCellPos+1)*IMAGE_WIDTH -RIGHT_WIDTH,5,m_nWidth-iCellPos*IMAGE_WIDTH-RIGHT_WIDTH,12+IMAGE_HEIGHT));

	return;
}

//���ù��
BOOL CGoldControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//��ȡ���
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	//���ù��
	int nHeadWidth=LEFT_WIDTH;
	int nViewCellCount=(m_nCellCount-1)/3+m_nCellCount;
	if ((point.y>5)&&(point.y<65)&&(point.x>nHeadWidth)&&(point.x<(CELL_WIDTH*nViewCellCount+nHeadWidth)))
	{
		int iCellPos=(m_nCellCount-(point.x-nHeadWidth)/CELL_WIDTH);
		if ((iCellPos!=3)&&(iCellPos!=7))
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return TRUE;
		}
	}

	return __super::OnSetCursor(pWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////

int CGoldControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_btMaxScore.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MAXSCORE);
	m_btMinScore.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MINSCORE);
	m_btOK.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_OK);
	m_btCancel.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CANCEL);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btMaxScore.SetButtonImage(IDB_BT_MAX,hInstance,false);
	m_btMinScore.SetButtonImage(IDB_BT_MIN,hInstance,false);
	m_btOK.SetButtonImage(IDB_BT_OK,hInstance,false);
	m_btCancel.SetButtonImage(IDB_BT_CANCEL,hInstance,false);
	return 0;
}

//��С��ע
void CGoldControl::OnMinScore()
{
	SetGold(m_lMinGold);
	Invalidate(false);
	AfxGetMainWnd()->SendMessage(IDM_MIN_SCORE,WPARAM(m_lMinGold),LPARAM(m_lMinGold));
	return ;
}

//�����ע
void CGoldControl::OnMaxScore()
{
	SetGold(m_lMaxGold);
	Invalidate(false);
	AfxGetMainWnd()->SendMessage(IDM_MAX_SCORE,WPARAM(m_lMaxGold),LPARAM(m_lMaxGold));
	return ;
}

//ȷ����Ϣ
void CGoldControl::OnOKScore()
{
	LONG lGold = GetGold();
	AfxGetMainWnd()->SendMessage(IDM_OK_SCORE,WPARAM(lGold),LPARAM(lGold));

	return;
}

//ȡ����Ϣ
void CGoldControl::OnCancelScore()
{
	AfxGetMainWnd()->SendMessage(IDM_CANCEL_SCORE,0,0);

	return;
}

void CGoldControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//��Ϣģ��
	//PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	__super::OnLButtonDown(nFlags, point);
}
