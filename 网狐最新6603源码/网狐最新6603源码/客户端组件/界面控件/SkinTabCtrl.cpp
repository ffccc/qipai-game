#include "StdAfx.h"
#include "SkinTabCtrl.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinTabCtrl, CTabCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinTabCtrl::CSkinTabCtrl()
{
	//状态变量
	m_bRenderImage=false;

	//设置变量
	m_crBackGround=RGB(250,250,250);
	m_crControlBorder=RGB(145,168,183);

	return;
}

//析够函数
CSkinTabCtrl::~CSkinTabCtrl()
{
}

//设置渲染
VOID CSkinTabCtrl::SetRenderImage(bool bRenderImage)
{
	//设置变量
	m_bRenderImage=bRenderImage;

	//更新窗口
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//设置颜色
VOID CSkinTabCtrl::SetTabCtrlColor(COLORREF crBackGround, COLORREF crControlBorder)
{
	//设置变量
	m_crBackGround=crBackGround;
	m_crControlBorder=crControlBorder;

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//重画消息
VOID CSkinTabCtrl::OnPaint()
{
	//变量定义
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	CRect rcClip;
	dc.GetClipBox(&rcClip);
	dc.FillSolidRect(&rcClip,m_crBackGround);

	//设置 DC
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	
	//绘画边框
	rcClient.top+=26;
	dc.Draw3dRect(&rcClient,m_crControlBorder,m_crControlBorder);

	//绘画按钮
	CPen LinePen(PS_SOLID,1,m_crControlBorder);
	CPen * pOldPen=dc.SelectObject(&LinePen);

	for (INT i=0;i<GetItemCount();i++)
	{
		//变量定义
		TCITEM ItemInfo;
		ZeroMemory(&ItemInfo,sizeof(ItemInfo));

		//变量定义
		TCHAR szBuffer[100];
		ItemInfo.mask=TCIF_TEXT|TCIF_IMAGE;
		ItemInfo.pszText=szBuffer;
		ItemInfo.cchTextMax=sizeof(szBuffer);

		//获取信息
		CRect rcItem;
		GetItem(i,&ItemInfo);
		GetItemRect(i,&rcItem);

		//填充背景
		dc.FillSolidRect(&rcItem,m_crBackGround);

		//绘画边框
		dc.MoveTo(rcItem.left,rcItem.bottom-1);
		dc.LineTo(rcItem.left,rcItem.top+2);
		dc.LineTo(rcItem.left+2,rcItem.top);
		dc.LineTo(rcItem.right-2,rcItem.top);
		dc.LineTo(rcItem.right,rcItem.top+2);
		dc.LineTo(rcItem.right,rcItem.bottom-1);
		
		//绘画边框
		if (i!=GetCurSel())
		{
			rcItem.top+=2;
			dc.SetTextColor(RGB(0,0,0));
			dc.MoveTo(rcItem.left,rcItem.bottom-1);
			dc.LineTo(rcItem.right,rcItem.bottom-1);
			DrawText(dc,szBuffer,lstrlen(szBuffer),&rcItem,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else 
		{
			rcItem.top+=3;
			dc.SetTextColor(RGB(0,0,0));
			DrawText(dc,szBuffer,lstrlen(szBuffer),&rcItem,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
	}

	//清理资源
	dc.SelectObject(pOldPen);
	LinePen.DeleteObject();

	return;
}

//////////////////////////////////////////////////////////////////////////////////
