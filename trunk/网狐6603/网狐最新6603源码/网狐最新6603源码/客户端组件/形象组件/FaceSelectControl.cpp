#include "StdAfx.h"
#include "Resource.h"
#include "FaceSelectControl.h"

//////////////////////////////////////////////////////////////////////////////////
//宏定义

//位置定义
#define TOP_PELS					10									//上高象素
#define LINE_PELS					10									//单行象素
#define FRAME_WIDTH					12									//框架宽度
#define FRAME_HEIGHT				12									//框架高度

//常量定义
#define WHELL_PELS					5									//滚动象素

//颜色定义
#define CR_BACK_FRAME				RGB(250,250,250)					//背景颜色

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFaceItemSelectWnd, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFaceSelectControl, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CFaceItemSelectWnd::CFaceItemSelectWnd()
{
	//滚动信息
	m_nMaxPels=0;
	m_nPagePels=0;
	m_nLineCount=0;
	m_nWidthCount=0;
	m_nCurrentPos=0;
	m_nXExcursion=0;

	//头像变量
	m_wFaceCount=0;
	m_wSelectFace=INVALID_WORD;

	//设置变量
	m_pIFaceItemSelectEvent=NULL;

	return;
}

//析构函数
CFaceItemSelectWnd::~CFaceItemSelectWnd()
{
}

//选择数目
VOID CFaceItemSelectWnd::SetAllowItemFull()
{
	//获取对象
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//设置变量
	m_wFaceCount=pIFaceItemControl->GetFaceCount();
	if (m_wSelectFace!=INVALID_WORD) m_wSelectFace=__min(m_wSelectFace,m_wFaceCount);

	//调整控件
	if (m_hWnd!=NULL)
	{
		CRect rcRect;
		GetClientRect(&rcRect);
		RectifyControl(rcRect.Width(),rcRect.Height());
	}

	return;
}

//选择数目
VOID CFaceItemSelectWnd::SetAllowItemCount(WORD wFaceCount)
{
	//获取对象
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//设置变量
	m_wFaceCount=wFaceCount;
	m_wFaceCount=__min(m_wFaceCount,pIFaceItemControl->GetFaceCount());

	//索引调整
	if (m_wSelectFace!=INVALID_WORD)
	{
		m_wSelectFace=__min(m_wSelectFace,m_wFaceCount);
	}

	//调整控件
	if (m_hWnd!=NULL)
	{
		CRect rcRect;
		GetClientRect(&rcRect);
		RectifyControl(rcRect.Width(),rcRect.Height());
	}

	return;
}

//绘画消息 
VOID CFaceItemSelectWnd::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	CRect rcClip;
	dc.GetClipBox(&rcClip);
	dc.FillSolidRect(&rcClip,CR_BACK_FRAME);

	//计算索引
	INT nXBeginIndex=(rcClip.left-m_nXExcursion)/(FRAME_WIDTH+FACE_CX);
	INT nYBeginIndex=(rcClip.top+m_nCurrentPos-TOP_PELS)/(FRAME_HEIGHT+FACE_CY);

	//调整索引
	nXBeginIndex=__max(0,nXBeginIndex);
	nYBeginIndex=__max(0,nYBeginIndex);

	//绘画头像
	for (INT x=nXBeginIndex;x<m_nWidthCount;x++)
	{
		//坐标判断
		INT nXDrawPos=m_nXExcursion+x*(FRAME_WIDTH+FACE_CX);
		if (nXDrawPos>rcClip.right) break;

		//竖向绘画
		for (INT y=nYBeginIndex;y<m_nLineCount;y++)
		{
			//索引判断
			WORD wIndex=(m_nWidthCount*y+x);
			if (wIndex>=m_wFaceCount) break;

			//坐标判断
			INT nYDrawPos=TOP_PELS+(FRAME_HEIGHT+FACE_CY)*y-m_nCurrentPos;
			if (nYDrawPos>rcClip.bottom) break;

			//绘画头像
			DrawFaceItem(&dc,wIndex,nXDrawPos,nYDrawPos,m_wSelectFace==wIndex);
		}
	}

	return;
}

//位置信息
VOID CFaceItemSelectWnd::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//建立消息
INT CFaceItemSelectWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置滚动
	m_SkinScrollBar.InitScroolBar(this);

	return 0;
}

//鼠标消息
VOID CFaceItemSelectWnd::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//有效判断
	if (m_wSelectFace==INVALID_WORD) return;

	//事件通知
	ASSERT(m_pIFaceItemSelectEvent!=NULL);
	if (m_pIFaceItemSelectEvent!=NULL) m_pIFaceItemSelectEvent->OnEventFaceSelect(m_wSelectFace);

	return;
}

//按键消息
VOID CFaceItemSelectWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar,nRepCnt,nFlags);

	switch (nChar)
	{
	case VK_UP:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,m_nCurrentPos),NULL);
			return;
		}
	case VK_DOWN:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,m_nCurrentPos),NULL);
			return;
		}
	case VK_PRIOR:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEUP,m_nCurrentPos),NULL);
			return;
		}
	case VK_NEXT:
	case VK_SPACE:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEDOWN,m_nCurrentPos),NULL);
			return;
		}
	case VK_HOME:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_TOP,m_nCurrentPos),NULL);
			return;
		}
	case VK_END:
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_BOTTOM,m_nCurrentPos),NULL);
			return;
		}
	}

	return;
}

//光标消息
BOOL CFaceItemSelectWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//点击判断
	WORD wLastFaceID=m_wSelectFace;
	WORD wHoverFaceID=MouseHitTest(MousePoint);

	//相同判断
	if (wHoverFaceID!=m_wSelectFace)
	{
		//变量定义
		CClientDC ClientDC(this);

		//设置变量
		m_wSelectFace=wHoverFaceID;

		//清理选择
		if (wLastFaceID!=INVALID_WORD)
		{
			//计算位置
			INT nXPos=m_nXExcursion+wLastFaceID%m_nWidthCount*(FRAME_WIDTH+FACE_CX);
			INT nYPos=TOP_PELS+wLastFaceID/m_nWidthCount*(FRAME_HEIGHT+FACE_CY)-m_nCurrentPos;

			//绘画头像
			DrawFaceItem(&ClientDC,wLastFaceID,nXPos,nYPos,false);
		}

		//绘画选择
		if (m_wSelectFace!=INVALID_WORD)
		{
			//计算位置
			INT nXPos=m_nXExcursion+m_wSelectFace%m_nWidthCount*(FRAME_WIDTH+FACE_CX);
			INT nYPos=TOP_PELS+m_wSelectFace/m_nWidthCount*(FRAME_HEIGHT+FACE_CY)-m_nCurrentPos;

			//绘画头像
			DrawFaceItem(&ClientDC,m_wSelectFace,nXPos,nYPos,true);
		}
	}

	//设置光标
	if (m_wSelectFace!=INVALID_WORD)
	{
		SetCursor(LoadCursor(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//滚动消息
BOOL CFaceItemSelectWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint Point)
{
	__super::OnMouseWheel(nFlags,zDelta,Point);

	//设置变量
	INT nLastPos=m_nCurrentPos;
	m_nCurrentPos=__max(0,__min(m_nCurrentPos-zDelta/WHELL_PELS,m_nMaxPels-m_nPagePels));

	//设置滚动
	SetScrollPos(SB_VERT,m_nCurrentPos);
	ScrollWindow(0,nLastPos-m_nCurrentPos,NULL,NULL);

	return TRUE;
}

//滚动消息
VOID CFaceItemSelectWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	__super::OnVScroll(nSBCode,nPos,pScrollBar);

	INT nLastPos=m_nCurrentPos;
	switch (nSBCode)
	{
	case SB_TOP:
		{
			m_nCurrentPos=0;
			break;
		}
	case SB_BOTTOM:
		{
			m_nCurrentPos=m_nMaxPels;
			break;
		}
	case SB_LINEUP:
		{
			m_nCurrentPos-=LINE_PELS;
			break;
		}
	case SB_PAGEUP:
		{
			m_nCurrentPos-=m_nPagePels;
			break;
		}
	case SB_LINEDOWN:
		{
			m_nCurrentPos+=LINE_PELS;
			break;
		}
	case SB_PAGEDOWN:
		{
			m_nCurrentPos+=m_nPagePels;
			break;
		}
	case SB_THUMBTRACK:
		{
			m_nCurrentPos=nPos;
			break;
		}
	}
	
	//设置焦点
	SetFocus();

	//调整位置
	m_nCurrentPos=__max(0,__min(m_nCurrentPos,m_nMaxPels-m_nPagePels));

	//设置滚动
	if (nLastPos!=m_nCurrentPos)
	{
		SetScrollPos(SB_VERT,m_nCurrentPos);
		ScrollWindow(0,nLastPos-m_nCurrentPos,NULL,NULL);
	}
	
	return;
}

//按键测试
WORD CFaceItemSelectWnd::MouseHitTest(CPoint MousePoint)
{
	//设置变量
	MousePoint.y+=m_nCurrentPos;

	//范围判断
	if ((MousePoint.y<TOP_PELS)||(MousePoint.x<m_nXExcursion))
	{
		return INVALID_WORD;
	}

	//横向索引
	INT nXIndex=(MousePoint.x-m_nXExcursion)/(FRAME_WIDTH+FACE_CX);
	if (nXIndex>=m_nWidthCount) return INVALID_WORD;

	//竖向索引
	INT nYIndex=(MousePoint.y-TOP_PELS)/(FRAME_HEIGHT+FACE_CY);
	if (nYIndex>=m_nLineCount) return INVALID_WORD;

	//计算索引
	WORD wIndexHit=nYIndex*m_nWidthCount+nXIndex;
	if (wIndexHit>=m_wFaceCount) return INVALID_WORD;

	return wIndexHit;
}

//调整控件
VOID CFaceItemSelectWnd::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if ((nWidth==0)||(nHeight==0)) return;

	//计算行数
	m_nWidthCount=nWidth/(FRAME_WIDTH+FACE_CX);
	m_nLineCount=(m_wFaceCount+m_nWidthCount-1)/m_nWidthCount;

	//页面变量
	m_nPagePels=nHeight;
	m_nMaxPels=TOP_PELS*2+m_nLineCount*(FRAME_HEIGHT+FACE_CY);
	m_nXExcursion=(nWidth-m_nWidthCount*(FRAME_WIDTH+FACE_CX))/2;
	m_nCurrentPos=__max(0,__min(m_nCurrentPos,m_nMaxPels-m_nPagePels));

	//设置滚动
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize=sizeof(SCROLLINFO);
	ScrollInfo.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfo.nMin=0;
	ScrollInfo.nMax=m_nMaxPels;
	ScrollInfo.nPage=m_nPagePels;
	ScrollInfo.nPos=m_nCurrentPos;
	SetScrollInfo(SB_VERT,&ScrollInfo,TRUE);

	return;
}

//绘画头像
VOID CFaceItemSelectWnd::DrawFaceItem(CDC * pDC, WORD wIndex, INT nXPos, INT nYPos, bool bHover)
{
	//获取对象
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//创建缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(FRAME_WIDTH+FACE_CX,FRAME_HEIGHT+FACE_CY,32);

	//绘画背景
	CDC * pBufferDC=CDC::FromHandle(ImageBuffer.GetDC());
	pBufferDC->FillSolidRect(0,0,FRAME_WIDTH+FACE_CX,FRAME_HEIGHT+FACE_CY,CR_BACK_FRAME);

	//绘画框架
	BYTE cbFrameKind=(bHover==true)?ITEM_FRAME_HOVER:ITEM_FRAME_NORMAL;
	pIFaceItemControl->DrawFaceItemFrame(pBufferDC,FRAME_WIDTH/2,FRAME_HEIGHT/2,cbFrameKind);

	//绘画头像
	pIFaceItemControl->DrawFaceNormal(pBufferDC,FRAME_WIDTH/2,FRAME_HEIGHT/2,wIndex);

	//绘画界面
	pDC->BitBlt(nXPos,nYPos,FRAME_WIDTH+FACE_CX,FRAME_HEIGHT+FACE_CY,pBufferDC,0,0,SRCCOPY);

	//清理资源
	ImageBuffer.ReleaseDC();

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CFaceSelectControl::CFaceSelectControl() : CSkinDialog(IDD_DLG_FACE_SELECT)
{
	//设置接口
	m_FaceItemSelectWnd.SetFaceItemSelectEvent(this);

	return;
}

//析构函数
CFaceSelectControl::~CFaceSelectControl()
{
}

//接口查询
VOID * CFaceSelectControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IFaceSelectControl,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IFaceSelectControl,Guid,dwQueryVer);
	return NULL;
}

//选择头像
bool CFaceSelectControl::GetSelectFaceID(WORD & wFaceID)
{
	//设置资源
	AfxSetResourceHandle(GetModuleHandle(AVATAR_CONTROL_DLL_NAME));

	//显示窗口
	if (DoModal()!=IDOK) return false;

	//设置变量
	wFaceID=m_FaceItemSelectWnd.GetSelectFace();

	return true;
}

//图像事件
VOID CFaceSelectControl::OnEventFaceSelect(WORD wFaceID)
{
	//关闭窗口
	EndDialog(IDOK);

	return;
}

//配置函数
BOOL CFaceSelectControl::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("头像选择"));

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建控件
	CRect rcSelectWnd;
	rcSelectWnd.top=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	rcSelectWnd.left=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	rcSelectWnd.right=rcClient.Width()-m_SkinAttribute.m_EncircleInfoFrame.nRBorder;
	rcSelectWnd.bottom=rcClient.Height()-m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	m_FaceItemSelectWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL,rcSelectWnd,this,100);

	//设置焦点
	m_FaceItemSelectWnd.SetFocus();
	m_FaceItemSelectWnd.SetAllowItemFull();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(FaceSelectControl);

//////////////////////////////////////////////////////////////////////////////////
