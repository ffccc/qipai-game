#include "StdAfx.h"
#include "ChooseWnd.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChooseWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CChooseWnd::CChooseWnd()
{
	//���ݱ���
	m_bWeaveCount=0;
	ZeroMemory(&m_ChiCardInfo,sizeof(m_ChiCardInfo));

	//���б���
	m_cbCurrentIndex=0xFF;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBackL.SetLoadInfo(IDB_CHOOSE_L,hInstance);
	m_ImageBackM.SetLoadInfo(IDB_CHOOSE_M,hInstance);
	m_ImageBackR.SetLoadInfo(IDB_CHOOSE_R,hInstance);
	m_ImageBackH.SetLoadInfo(IDB_CHOOSE_H,hInstance);

	//������Դ
	CImageHandle HandleBackL(&m_ImageBackL);
	CImageHandle HandleBackM(&m_ImageBackM);
	CImageHandle HandleBackR(&m_ImageBackR);

	//��ȡ��С
	m_SizeCell.cx=m_ImageBackM.GetWidth();
	m_SizeCell.cy=m_ImageBackM.GetHeight();
	m_SizeLeft.cx=m_ImageBackL.GetWidth();
	m_SizeLeft.cy=m_ImageBackL.GetHeight();
	m_SizeRight.cx=m_ImageBackR.GetWidth();
	m_SizeRight.cy=m_ImageBackR.GetHeight();

	return;
}

//��������
CChooseWnd::~CChooseWnd()
{
}

//���û�׼
void CChooseWnd::SetBenchmarkPos(int nXPos, int nYPos)
{
	//���ñ���
	m_ControlPoint.x=nXPos;
	m_ControlPoint.y=nYPos;

	//����λ��
	if (m_hWnd!=NULL)
	{
		int nWidth=m_SizeLeft.cx+m_SizeRight.cx+m_bWeaveCount*m_SizeCell.cx;
		SetWindowPos(NULL,m_ControlPoint.x-nWidth,m_ControlPoint.y-m_SizeCell.cy,nWidth,m_SizeCell.cy,SWP_NOZORDER);
	}

	return;
}

//��������
bool CChooseWnd::SetChooseWeave(tagChiCardInfo ChiCardInfo[6], BYTE cbWeaveCount)
{
	//Ч������
	ASSERT(cbWeaveCount<CountArray(m_ChiCardInfo));
	if (cbWeaveCount>=CountArray(m_ChiCardInfo)) return false;

	//��������
	m_bWeaveCount=cbWeaveCount;
	CopyMemory(&m_ChiCardInfo,ChiCardInfo,sizeof(m_ChiCardInfo[0])*cbWeaveCount);

	//����λ��
	int nWidth=m_SizeLeft.cx+m_SizeRight.cx+m_bWeaveCount*m_SizeCell.cx;
	SetWindowPos(NULL,m_ControlPoint.x-nWidth,m_ControlPoint.y-m_SizeCell.cy,nWidth,m_SizeCell.cy,SWP_NOZORDER|SWP_SHOWWINDOW);

	return true;
}

//�ػ�����
void CChooseWnd::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//������Դ
	CImageHandle HandleBackL(&m_ImageBackL);
	CImageHandle HandleBackM(&m_ImageBackM);
	CImageHandle HandleBackR(&m_ImageBackR);
	CImageHandle HandleBackH(&m_ImageBackH);

	//��������
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���û���
	BufferDC.SelectObject(&BufferImage);

	//�滭����
	m_ImageBackL.BitBlt(BufferDC,0,0);
	m_ImageBackR.BitBlt(BufferDC,rcClient.Width()-m_ImageBackR.GetWidth(),0);
	for (int i=0;i<m_bWeaveCount;i++)
	{
		//�滭����
		int nXPos=m_SizeLeft.cx+m_SizeCell.cx*i;
		if (i==m_cbCurrentIndex) m_ImageBackH.BitBlt(BufferDC,nXPos,0);
		else m_ImageBackM.BitBlt(BufferDC,nXPos,0);

		//�滭�˿�
		g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,m_ChiCardInfo[i].cbCardData[0][0],nXPos+3,10);
		g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,m_ChiCardInfo[i].cbCardData[0][1],nXPos+3,30);
		g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,m_ChiCardInfo[i].cbCardData[0][2],nXPos+3,50);
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferImage.DeleteObject();
	BufferDC.DeleteDC();

	return;
}

//������Ϣ
int CChooseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	return 0;
}

//�����Ϣ
void CChooseWnd::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//״̬�ж�
	if (m_cbCurrentIndex>=m_bWeaveCount) return;

	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_CHOOSE_CARD,m_cbCurrentIndex,m_ChiCardInfo[m_cbCurrentIndex].cbChiKind);

	return;
}

//�����Ϣ
BOOL CChooseWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//��ȡѡ��
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//��ǰ����
	BYTE cbIndex=0xFF;
	if (MousePoint.x>=m_SizeLeft.cx)
	{
		cbIndex=(BYTE)((MousePoint.x-m_SizeLeft.cx)/m_SizeCell.cx);
		if (cbIndex>=m_bWeaveCount) cbIndex=0xFF;
	}

	//��������
	if (cbIndex!=m_cbCurrentIndex)
	{
		//���ñ���
		m_cbCurrentIndex=cbIndex;

		//���´���
		Invalidate(FALSE);
	}

	//���ù��
	if (m_cbCurrentIndex!=0xFF)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,message);
}

//////////////////////////////////////////////////////////////////////////
