#include "Stdafx.h"
#include "Math.h"
#include "Resource.h"
#include "GoldControl.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define CELL_WIDTH							19							//��Ԫ���
#define LESS_WIDTH							70							//��С���
#define SPACE_WIDTH							12							//��Ϯ���
#define BUTTON_WIDTH						0							//��������
#define CONTROL_HEIGHT						50							//�ؼ��߶�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGoldControl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGoldControl::CGoldControl()
{
	//�������
	m_nWidth=0;
	m_nCellCount=0;
	m_AppendWidth=0;

	//���ñ���
	m_lMaxGold=0L;
	memset(m_lGoldCell,0,sizeof(m_lGoldCell));

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageLeft.SetLoadInfo(IDB_USERMONEY_LEFT,hInstance);
	m_ImageMid.SetLoadInfo(IDB_USERMONEY_MID,hInstance);
	m_ImageRight.SetLoadInfo(IDB_USERMONEY_RIGHT,hInstance);
}

//��������
CGoldControl::~CGoldControl()
{
}

//��ȡ���
LONG CGoldControl::GetGold()
{
	LONG lGold=0L;
	for (LONG i=0;i<CountArray(m_lGoldCell);i++) lGold+=m_lGoldCell[i]*(LONG)pow(10L,i);
	return lGold;
}

//���ý��
void CGoldControl::SetGold(LONG lGold)
{
	//��������
	if (lGold>m_lMaxGold) lGold=m_lMaxGold;
	memset(m_lGoldCell,0,sizeof(m_lGoldCell));

	//���ñ���
	int nIndex=0;
	while (lGold>0L)
	{
		m_lGoldCell[nIndex++]=lGold%10L;
		lGold/=10L;
	}

	//�ػ�����
	Invalidate(FALSE);

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
	memset(m_lGoldCell,0,sizeof(m_lGoldCell));

	//���㵥Ԫ
	m_nCellCount=0;
	while (lMaxGold>0L)
	{
		lMaxGold/=10L;
		m_nCellCount++;
	}
	m_nCellCount=__min(CountArray(m_lGoldCell),__max(m_nCellCount,1));

	//���ý���
	m_AppendWidth=0;
	m_nWidth=(m_nCellCount+(m_nCellCount-1)/3)*CELL_WIDTH+SPACE_WIDTH*6+BUTTON_WIDTH;
	if (m_nWidth<LESS_WIDTH) 
	{
		m_AppendWidth=LESS_WIDTH-m_nWidth;
		m_nWidth=LESS_WIDTH;
	}
	MoveWindow(m_BasicPoint.x-m_nWidth,m_BasicPoint.y-CONTROL_HEIGHT,m_nWidth,CONTROL_HEIGHT);

	//�ػ�����
	Invalidate(FALSE);

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
	return;
}

//�滭���
void CGoldControl::DrawGoldCell(CDC * pDC, int nXPos, int nYPox, LONG lGold)
{
	if (lGold<10L) 
	{
		TCHAR szBuffer[12];
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),lGold);
		pDC->TextOut(nXPos,nYPox,szBuffer,lstrlen(szBuffer));
	}
	else
	{
		LPCTSTR pszBuffer=TEXT("��");
		pDC->TextOut(nXPos,nYPox,pszBuffer,lstrlen(pszBuffer));
	}

	return;
}

//�ػ�����
void CGoldControl::OnPaint()
{
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

	//��������(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));
	CFont GlodFont;
	GlodFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT(""));
	CFont * pOldFont=BackFaceDC.SelectObject(&GlodFont);

	//���� DC
	BackFaceDC.SetBkMode(TRANSPARENT);
	BackFaceDC.SetTextColor(RGB(150,150,150));

	//�滭���
	int nXExcursion=ClientRect.Width()-SPACE_WIDTH*5-BUTTON_WIDTH;
	for (int i=0;i<m_nCellCount;i++)
	{
		//�滭����
		if ((i!=0)&&(i%3)==0)
		{
			nXExcursion-=CELL_WIDTH;
			DrawGoldCell(&BackFaceDC,nXExcursion,0,10);
		}

		//�滭����
		nXExcursion-=CELL_WIDTH;
		DrawGoldCell(&BackFaceDC,nXExcursion,7,m_lGoldCell[i]);
	}

	//������Դ
	BackFaceDC.SelectObject(pOldFont);
	GlodFont.DeleteObject();

	//��ʾ��Ϣ
	TCHAR szBuffer[50];
	sprintf(szBuffer,TEXT("��ࣺ%ld"),m_lMaxGold);
	BackFaceDC.SetTextColor(RGB(200,200,200));
	BackFaceDC.SelectObject(CSkinResourceManager::GetDefaultFont());
	BackFaceDC.TextOut(10,31,szBuffer,lstrlen(szBuffer));

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
	int nHeadWidth=SPACE_WIDTH+m_AppendWidth;
	if ((point.y<=2)||(point.y>=26)) return;
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

	//���ñ���
	m_lGoldCell[iCellPos]=(m_lGoldCell[iCellPos]+1)%10;

	//�ػ�����
	Invalidate(FALSE);

	return;
}

//�Ҽ�������Ϣ
void CGoldControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	//��������
	int nViewCellCount=(m_nCellCount-1)/3+m_nCellCount;

	//λ�ù���
	int nHeadWidth=SPACE_WIDTH+m_AppendWidth;
	if ((point.y<=2)||(point.y>=26)) return;
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

	//���ñ���
	m_lGoldCell[iCellPos]=(m_lGoldCell[iCellPos]+9)%10;

	//�ػ�����
	Invalidate(FALSE);

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
	int nHeadWidth=SPACE_WIDTH+m_AppendWidth;
	int nViewCellCount=(m_nCellCount-1)/3+m_nCellCount;
	if ((point.y>2)&&(point.y<26)&&(point.x>nHeadWidth)&&(point.x<(CELL_WIDTH*nViewCellCount+nHeadWidth)))
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
