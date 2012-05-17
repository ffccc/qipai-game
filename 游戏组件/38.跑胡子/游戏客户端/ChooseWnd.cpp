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

//构造函数
CChooseWnd::CChooseWnd()
{
	//数据变量
	m_bWeaveCount=0;
	ZeroMemory(&m_ChiCardInfo,sizeof(m_ChiCardInfo));

	//运行变量
	m_cbCurrentIndex=0xFF;

	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBackL.SetLoadInfo(IDB_CHOOSE_L,hInstance);
	m_ImageBackM.SetLoadInfo(IDB_CHOOSE_M,hInstance);
	m_ImageBackR.SetLoadInfo(IDB_CHOOSE_R,hInstance);
	m_ImageBackH.SetLoadInfo(IDB_CHOOSE_H,hInstance);

	//加载资源
	CImageHandle HandleBackL(&m_ImageBackL);
	CImageHandle HandleBackM(&m_ImageBackM);
	CImageHandle HandleBackR(&m_ImageBackR);

	//获取大小
	m_SizeCell.cx=m_ImageBackM.GetWidth();
	m_SizeCell.cy=m_ImageBackM.GetHeight();
	m_SizeLeft.cx=m_ImageBackL.GetWidth();
	m_SizeLeft.cy=m_ImageBackL.GetHeight();
	m_SizeRight.cx=m_ImageBackR.GetWidth();
	m_SizeRight.cy=m_ImageBackR.GetHeight();

	return;
}

//析构函数
CChooseWnd::~CChooseWnd()
{
}

//设置基准
void CChooseWnd::SetBenchmarkPos(int nXPos, int nYPos)
{
	//设置变量
	m_ControlPoint.x=nXPos;
	m_ControlPoint.y=nYPos;

	//调整位置
	if (m_hWnd!=NULL)
	{
		int nWidth=m_SizeLeft.cx+m_SizeRight.cx+m_bWeaveCount*m_SizeCell.cx;
		SetWindowPos(NULL,m_ControlPoint.x-nWidth,m_ControlPoint.y-m_SizeCell.cy,nWidth,m_SizeCell.cy,SWP_NOZORDER);
	}

	return;
}

//设置数据
bool CChooseWnd::SetChooseWeave(tagChiCardInfo ChiCardInfo[6], BYTE cbWeaveCount)
{
	//效验数据
	ASSERT(cbWeaveCount<CountArray(m_ChiCardInfo));
	if (cbWeaveCount>=CountArray(m_ChiCardInfo)) return false;

	//设置数据
	m_bWeaveCount=cbWeaveCount;
	CopyMemory(&m_ChiCardInfo,ChiCardInfo,sizeof(m_ChiCardInfo[0])*cbWeaveCount);

	//调整位置
	int nWidth=m_SizeLeft.cx+m_SizeRight.cx+m_bWeaveCount*m_SizeCell.cx;
	SetWindowPos(NULL,m_ControlPoint.x-nWidth,m_ControlPoint.y-m_SizeCell.cy,nWidth,m_SizeCell.cy,SWP_NOZORDER|SWP_SHOWWINDOW);

	return true;
}

//重画函数
void CChooseWnd::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载资源
	CImageHandle HandleBackL(&m_ImageBackL);
	CImageHandle HandleBackM(&m_ImageBackM);
	CImageHandle HandleBackR(&m_ImageBackR);
	CImageHandle HandleBackH(&m_ImageBackH);

	//创建缓冲
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//设置缓冲
	BufferDC.SelectObject(&BufferImage);

	//绘画界面
	m_ImageBackL.BitBlt(BufferDC,0,0);
	m_ImageBackR.BitBlt(BufferDC,rcClient.Width()-m_ImageBackR.GetWidth(),0);
	for (int i=0;i<m_bWeaveCount;i++)
	{
		//绘画背景
		int nXPos=m_SizeLeft.cx+m_SizeCell.cx*i;
		if (i==m_cbCurrentIndex) m_ImageBackH.BitBlt(BufferDC,nXPos,0);
		else m_ImageBackM.BitBlt(BufferDC,nXPos,0);

		//绘画扑克
		g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,m_ChiCardInfo[i].cbCardData[0][0],nXPos+3,10);
		g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,m_ChiCardInfo[i].cbCardData[0][1],nXPos+3,30);
		g_CardResource.m_ImageUserBottom.DrawCardItem(&BufferDC,m_ChiCardInfo[i].cbCardData[0][2],nXPos+3,50);
	}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferImage.DeleteObject();
	BufferDC.DeleteDC();

	return;
}

//建立消息
int CChooseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	return 0;
}

//鼠标消息
void CChooseWnd::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//状态判断
	if (m_cbCurrentIndex>=m_bWeaveCount) return;

	//发送消息
	AfxGetMainWnd()->PostMessage(IDM_CHOOSE_CARD,m_cbCurrentIndex,m_ChiCardInfo[m_cbCurrentIndex].cbChiKind);

	return;
}

//光标消息
BOOL CChooseWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//获取选择
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//当前索引
	BYTE cbIndex=0xFF;
	if (MousePoint.x>=m_SizeLeft.cx)
	{
		cbIndex=(BYTE)((MousePoint.x-m_SizeLeft.cx)/m_SizeCell.cx);
		if (cbIndex>=m_bWeaveCount) cbIndex=0xFF;
	}

	//设置索引
	if (cbIndex!=m_cbCurrentIndex)
	{
		//设置变量
		m_cbCurrentIndex=cbIndex;

		//更新窗口
		Invalidate(FALSE);
	}

	//设置光标
	if (m_cbCurrentIndex!=0xFF)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,message);
}

//////////////////////////////////////////////////////////////////////////
