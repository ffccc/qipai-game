#include "StdAfx.h"
#include "FaceItemView.h"
#include "FaceItemControl.h"

//////////////////////////////////////////////////////////////////////////////////
//宏定义

#define COLOR_BACK_GROUND			RGB(229,233,250)					//背景颜色

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFaceItemView, CStatic)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CFaceItemView::CFaceItemView()
{
	//颜色变量
	m_rcBackGround=COLOR_BACK_GROUND;

	//设置变量
	m_wFaceID=INVALID_WORD;
	m_dwCustonID=INVALID_DWORD;
	ZeroMemory(m_dwCustomFace,sizeof(m_dwCustomFace));

	return;
}

//析构函数
CFaceItemView::~CFaceItemView()
{
}

//设置颜色
VOID CFaceItemView::SetControlColor(COLORREF rcBackGround)
{
	if (m_rcBackGround!=rcBackGround)
	{
		//设置变量
		m_rcBackGround=rcBackGround;

		//更新界面
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}

	return;
}

//设置头像
VOID CFaceItemView::SetSystemFace(WORD wFaceID)
{
	if (m_wFaceID!=wFaceID)
	{
		//设置变量
		m_wFaceID=wFaceID;

		//清理数据
		m_dwCustonID=INVALID_DWORD;
		ZeroMemory(m_dwCustomFace,sizeof(m_dwCustomFace));

		//更新界面
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return;
}

//设置头像
VOID CFaceItemView::SetCustomFace(DWORD dwCustonID, DWORD dwCustomFace[FACE_CX*FACE_CY])
{
	if (m_dwCustonID!=dwCustonID)
	{
		//清理头像
		m_wFaceID=INVALID_WORD;

		//设置变量
		m_dwCustonID=dwCustonID;
		CopyMemory(m_dwCustomFace,dwCustomFace,sizeof(m_dwCustomFace));

		//更新界面
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return;
}

//创建消息
INT CFaceItemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(lpCreateStruct);

	//设置窗口
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//重画函数 
VOID CFaceItemView::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcRect;
	GetClientRect(&rcRect);
	
	//获取对象
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//绘画背景
	dc.FillSolidRect(&rcRect,m_rcBackGround);

	//绘画头像
	if ((m_wFaceID!=INVALID_WORD)&&(m_dwCustonID==INVALID_DWORD))
	{
		pIFaceItemControl->DrawFaceNormal(&dc,(rcRect.Width()-FACE_CX)/2,(rcRect.Height()-FACE_CY)/2,m_wFaceID);
	}

	//自定头像
	if ((m_wFaceID==INVALID_WORD)&&(m_dwCustonID!=INVALID_DWORD))
	{
		pIFaceItemControl->DrawFaceNormal(&dc,(rcRect.Width()-FACE_CX)/2,(rcRect.Height()-FACE_CY)/2,m_dwCustomFace);
	}

	//绘画边框
	pIFaceItemControl->DrawFaceItemFrame(&dc,(rcRect.Width()-FACE_CX)/2,(rcRect.Height()-FACE_CY)/2,ITEM_FRAME_HOVER);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
