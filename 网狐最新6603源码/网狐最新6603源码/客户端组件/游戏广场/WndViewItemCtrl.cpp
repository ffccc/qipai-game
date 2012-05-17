#include "StdAfx.h"
#include "WndViewItemCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CWndViewItemCtrl * CWndViewItemCtrl::m_pWndViewItemCtrl=NULL;			//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndViewItemCtrl, CWnd)

	//系统消息
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWndViewItemCtrl::CWndViewItemCtrl()
{
	//活动信息
	m_bHovering=false;
	m_wItemDown=INVALID_WORD;
	m_wItemHover=INVALID_WORD;
	m_wItemActive=INVALID_WORD;

	//资源信息
	m_pIViewItemEvent=NULL;
	m_RectViewItem.SetRect(0,0,0,0);

	//设置对象
	ASSERT(m_pWndViewItemCtrl==NULL);
	if (m_pWndViewItemCtrl==NULL) m_pWndViewItemCtrl=this;

	//获取大小
	CImage ItemButtonImage;
	ItemButtonImage.LoadFromResource(AfxGetInstanceHandle(),IDB_ITEM_BUTTON);
	m_SizeButton.SetSize(ItemButtonImage.GetWidth()/3,ItemButtonImage.GetHeight()/2);

	//设置颜色
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	m_LinkBoradHSB=pSkinRenderManager->RGBToHSB(ItemButtonImage.GetPixel(2,m_SizeButton.cy*2-1));

	return;
}

//析构函数
CWndViewItemCtrl::~CWndViewItemCtrl()
{
	//设置对象
	ASSERT(m_pWndViewItemCtrl==this);
	if (m_pWndViewItemCtrl==this) m_pWndViewItemCtrl=NULL;

	return;
}

//获取大小
VOID CWndViewItemCtrl::GetViewItemSize(CSize & ResultSize)
{
	//设置变量
	ResultSize.cy=m_SizeButton.cy;
	ResultSize.cx=m_SizeButton.cx*(m_ViewItemArray.GetCount());
	
	return;
}

//绘画衔接
VOID CWndViewItemCtrl::DrawLinkBoradLine(CWnd * pWnd, CDC * pDC)
{
	//绘画判断
	if (m_wItemActive==INVALID_WORD) return;
	if (m_ViewItemArray[m_wItemActive]->pItemView!=pWnd) return;

	//获取对象
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//绘画衔接
	INT nXPos=m_wItemActive*m_SizeButton.cx+10;
	pDC->FillSolidRect(nXPos,0,m_SizeButton.cx-2,1,pSkinRenderManager->RenderColor(m_LinkBoradHSB.S,m_LinkBoradHSB.B));

	return;
}

//设置接口
VOID CWndViewItemCtrl::SetViewItemEvent(IViewItemEvent * pIViewItemEvent)
{
	//设置接口
	m_pIViewItemEvent=pIViewItemEvent;

	return;
}

//设置位置
VOID CWndViewItemCtrl::SetViewItemPostion(INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//设置变量
	m_RectViewItem.SetRect(nXPos,nYPos,nXPos+nWidth,nYPos+nHeight);

	//调整窗口
	if (m_wItemActive!=INVALID_WORD)
	{
		//获取对象
		ASSERT(m_wItemActive<m_ViewItemArray.GetCount());
		tagViewItemInfo * pViewItemInfo=m_ViewItemArray[m_wItemActive];

		//调整位置
		CWnd * pViewItem=pViewItemInfo->pItemView;
		if (pViewItem!=NULL) pViewItem->SetWindowPos(NULL,nXPos,nYPos,nWidth,nHeight,SWP_NOZORDER|SWP_NOCOPYBITS|SWP_DEFERERASE);
	}

	return;
}

//激活子项
VOID CWndViewItemCtrl::ActiveViewItem(CWnd * pItemView)
{
	//变量定义
	WORD wItemIndex=GetViewItemIndex(pItemView);
	
	//状态判断
	ASSERT(wItemIndex!=INVALID_WORD);
	if (wItemIndex==INVALID_WORD) return;

	//设置状态
	if (m_wItemActive!=wItemIndex)
	{
		//当前对象
		tagViewItemInfo * pCurrentItem=NULL;
		if (m_wItemActive!=INVALID_WORD) pCurrentItem=m_ViewItemArray[m_wItemActive];

		//隐藏窗口
		if (pCurrentItem!=NULL) pCurrentItem->pItemView->ShowWindow(SW_HIDE);

		//激活窗口
		DWORD dwFlags=SWP_NOZORDER|SWP_SHOWWINDOW;
		pItemView->SetWindowPos(NULL,m_RectViewItem.left,m_RectViewItem.top,m_RectViewItem.Width(),m_RectViewItem.Height(),dwFlags);

		//重画窗口
		pItemView->Invalidate();

		//设置状态
		m_wItemActive=wItemIndex;

		//更新窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

		//事件通知
		ASSERT(m_pIViewItemEvent!=NULL);
		if (m_pIViewItemEvent!=NULL) m_pIViewItemEvent->OnViewItemEvent(m_wItemActive,pItemView);

		//更新窗口
		CSize SizeViewItemCtrl;
		GetViewItemSize(SizeViewItemCtrl);
		SetWindowPos(NULL,0,0,SizeViewItemCtrl.cx,SizeViewItemCtrl.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);
	}

	return;
}

//删除子项
VOID CWndViewItemCtrl::RemoveViewItem(CWnd * pItemView)
{
	//效验状态
	ASSERT(GetViewItemIndex(pItemView)!=INVALID_WORD);
	ASSERT(m_ViewItemArray[GetViewItemIndex(pItemView)]!=NULL);

	//获取对象
	WORD wItemIndex=GetViewItemIndex(pItemView);
	tagViewItemInfo * pViewItemInfo=m_ViewItemArray[wItemIndex];

	//删除对象
	SafeDelete(pViewItemInfo);
	m_ViewItemArray.RemoveAt(wItemIndex);

	//当前调整
	if (m_wItemActive==wItemIndex) m_wItemActive=INVALID_WORD;
	if ((m_wItemActive!=INVALID_WORD)&&(m_wItemActive>wItemIndex)) m_wItemActive--;

	//更新窗口
	CSize SizeViewItemCtrl;
	GetViewItemSize(SizeViewItemCtrl);
	SetWindowPos(NULL,0,0,SizeViewItemCtrl.cx,SizeViewItemCtrl.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);

	return;
}

//插入子项
VOID CWndViewItemCtrl::InsertViewItem(CWnd * pItemView, LPCTSTR pszTitleImage)
{
	//存在判断
	if (GetViewItemIndex(pItemView)!=INVALID_WORD)
	{
		ASSERT(FALSE);
		return;
	}

	//创建对象
	tagViewItemInfo * pViewItemInfo=NULL;
	try
	{
		pViewItemInfo=new tagViewItemInfo;
		ZeroMemory(pViewItemInfo,sizeof(tagViewItemInfo));
	}
	catch (...)
	{
		ASSERT(FALSE);
		return;
	}

	//插入对象
	m_ViewItemArray.Add(pViewItemInfo);

	//设置变量
	pViewItemInfo->pItemView=pItemView;
	lstrcpyn(pViewItemInfo->szItemTitle,pszTitleImage,CountArray(pViewItemInfo->szItemTitle));

	//调整位置
	WORD wItemCount=(WORD)m_ViewItemArray.GetCount();
	SetWindowPos(NULL,0,0,m_SizeButton.cx*wItemCount,m_SizeButton.cy,SWP_NOZORDER|SWP_NOMOVE);

	//激活对象
	ActiveViewItem(pItemView);

	return;
}

//对象索引
WORD CWndViewItemCtrl::GetViewItemIndex(CPoint MousePoint)
{
	//对象索引
	WORD wItemIndex=(WORD)(MousePoint.x/m_SizeButton.cx);
	if (wItemIndex>=m_ViewItemArray.GetCount()) wItemIndex=INVALID_WORD;

	return wItemIndex;
}

//对象索引
WORD CWndViewItemCtrl::GetViewItemIndex(CWnd * pItemView)
{
	//搜索对象
	for (INT_PTR i=0;i<m_ViewItemArray.GetCount();i++)
	{
		if (m_ViewItemArray[i]->pItemView==pItemView)
		{
			return (WORD)i;
		}
	}

	return INVALID_WORD;
}

//绘画背景
BOOL CWndViewItemCtrl::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(pDC);
	ImageBuffer.CreateCompatibleBitmap(pDC,rcClient.Width(),rcClient.Height());

	//设置 DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);

	//加载资源
	CBitImage ItemButtonImage;
	ItemButtonImage.LoadFromResource(AfxGetInstanceHandle(),IDB_ITEM_BUTTON);

	//渲染资源
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(ItemButtonImage);

	//绘画子项
	for (INT_PTR i=0;i<m_ViewItemArray.GetCount();i++)
	{
		//变量定义
		CPoint ImagePosion;
		ImagePosion.SetPoint(0,0);

		//获取对象
		ASSERT(m_ViewItemArray[i]!=NULL);
		tagViewItemInfo * pViewItemInfo=m_ViewItemArray[i];

		//竖向位置
		if (m_wItemActive==i) ImagePosion.y=m_SizeButton.cy;

		//横向位置
		if (m_wItemDown==i) ImagePosion.x=m_SizeButton.cx;
		if ((m_wItemHover==i)&&(m_wItemDown!=i)) ImagePosion.x=m_SizeButton.cx*2;

		//绘画按钮
		CPoint PointButton(m_SizeButton.cx*i,0);
		ItemButtonImage.BitBlt(DCBuffer,PointButton.x,PointButton.y,m_SizeButton.cx,m_SizeButton.cy,ImagePosion.x,ImagePosion.y);

		//绘画标题
		if (pViewItemInfo->szItemTitle[0]!=0)
		{
			//设置环境
			DCBuffer.SetTextColor((m_wItemActive==i)?RGB(0,23,40):RGB(0,0,0));
			DCBuffer.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

			//构造位置
			CRect rcItemText;
			rcItemText.top=rcItemText.bottom=PointButton.y+6;
			rcItemText.left=rcItemText.right=PointButton.x+39;

			//输出字体
			INT nItemLen=lstrlen(pViewItemInfo->szItemTitle);
			DCBuffer.DrawText(pViewItemInfo->szItemTitle,nItemLen,&rcItemText,DT_CENTER|DT_VCENTER|DT_NOCLIP);
		}
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//清理资源
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return TRUE;
}

//鼠标消息
VOID CWndViewItemCtrl::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//位置计算
	WORD wItemHover=GetViewItemIndex(Point);

	//盘旋变量
	if (wItemHover!=m_wItemHover)
	{
		//设置变量
		m_wItemHover=wItemHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_UPDATENOW|RDW_INVALIDATE);
	}

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//鼠标消息
VOID CWndViewItemCtrl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//激活子项
	if ((m_wItemHover!=INVALID_WORD)&&(m_wItemHover==m_wItemDown))
	{
		tagViewItemInfo * pViewItemInfo=m_ViewItemArray[m_wItemHover];
		if (pViewItemInfo!=NULL) ActiveViewItem(pViewItemInfo->pItemView);
	}

	//按起处理
	if (m_wItemDown!=INVALID_WORD)
	{
		//释放鼠标
		ReleaseCapture();

		//设置变量
		m_wItemDown=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
VOID CWndViewItemCtrl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置焦点
	SetFocus();

	//按下处理
	if (m_wItemHover!=INVALID_WORD)
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_wItemDown=m_wItemHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标双击
VOID CWndViewItemCtrl::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//关闭子项
	if ((m_wItemHover!=0L)&&(m_wItemHover!=INVALID_WORD))
	{
		tagViewItemInfo * pViewItemInfo=m_ViewItemArray[m_wItemHover];
		if (pViewItemInfo!=NULL) pViewItemInfo->pItemView->PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);
	}

	return;
}

//鼠标消息
LRESULT CWndViewItemCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置状态
	m_bHovering=false;

	//更新界面
	if (m_wItemHover!=INVALID_WORD)
	{
		//设置变量
		m_wItemHover=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//光标消息
BOOL CWndViewItemCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//设置光标
	if (m_wItemHover!=INVALID_WORD)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////////////
