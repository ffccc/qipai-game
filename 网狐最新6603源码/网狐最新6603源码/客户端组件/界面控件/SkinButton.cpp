#include "Stdafx.h"
#include "Resource.h"
#include "SkinButton.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_TRANSITION				10									//过渡标识
#define TIME_TRANSITION				30									//过渡时间

//常量定义
#define MIN_TRANSITION_INDEX		0									//过渡索引
#define MAX_TRANSITION_INDEX		8									//过渡索引

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinButton, CButton)

	//系统消息
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()

	//自定消息
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinButton::CSkinButton()
{
	//状态变量
	m_bHovering=false;
	m_bTransparent=false;
	m_bRenderImage=false;

	//配置变量
	m_crButtonText=RGB(0,0,0);
	m_wImageIndex=MIN_TRANSITION_INDEX;

	//背景资源
	m_pszSurface=NULL;
	m_pszResource=NULL;
	m_hResInstance=NULL;

	return;
}

//析构函数
CSkinButton::~CSkinButton()
{
}

//绑定函数
VOID CSkinButton::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//设置按钮
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);

	//调整控件
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//设置字体
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
	SendMessage(WM_SETFONT,(WPARAM)(HFONT)(pSkinResourceManager->GetDefaultFont()),TRUE);

	return;
}

//时间消息
VOID CSkinButton::OnTimer(UINT_PTR nIDEvent)
{
	//过渡变量
	if (nIDEvent==IDI_TRANSITION)
	{
		//盘旋过渡
		if ((m_bHovering==true)&&(m_wImageIndex<MAX_TRANSITION_INDEX))
		{
			m_wImageIndex++;
		}

		//常态过渡
		if ((m_bHovering==false)&&(m_wImageIndex>MIN_TRANSITION_INDEX))
		{
			m_wImageIndex--;
		}

		//删除时间
		if ((m_wImageIndex==MIN_TRANSITION_INDEX)||(m_wImageIndex==MAX_TRANSITION_INDEX))
		{
			KillTimer(IDI_TRANSITION);
		}

		//更新界面
		Invalidate(FALSE);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//建立消息
INT CSkinButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置按钮
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);

	//调整控件
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//设置字体
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
	SendMessage(WM_SETFONT,(WPARAM)(HFONT)(pSkinResourceManager->GetDefaultFont()),TRUE);

	return 0;
}

//设置渲染
VOID CSkinButton::SetRenderImage(bool bRenderImage)
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
VOID CSkinButton::SetButtonColor(COLORREF crButtonText)
{
	//设置变量
	m_crButtonText=crButtonText;

	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//加载位图
bool CSkinButton::SetButtonImage(UINT uBitmapID, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent)
{
	//设置变量
	m_bRenderImage=bRenderImage;
	m_bTransparent=bTransparent;

	//设置资源
	m_hResInstance=hResInstance;
	m_pszResource=MAKEINTRESOURCE(uBitmapID);

	//调整控件
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//更新窗口
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//加载位图
bool CSkinButton::SetButtonImage(LPCTSTR pszResource, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent)
{
	//设置变量
	m_bRenderImage=bRenderImage;
	m_bTransparent=bTransparent;

	//设置资源
	m_pszResource=pszResource;
	m_hResInstance=hResInstance;

	//调整控件
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//更新窗口
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//加载位图
bool CSkinButton::SetButtonImage(UINT uBitmapID, LPCTSTR pszSurface, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent)
{
	//设置变量
	m_bRenderImage=bRenderImage;
	m_bTransparent=bTransparent;

	//设置资源
	m_pszSurface=pszSurface;
	m_hResInstance=hResInstance;
	m_pszResource=MAKEINTRESOURCE(uBitmapID);

	//调整控件
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//更新窗口
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//加载位图
bool CSkinButton::SetButtonImage(LPCTSTR pszResource, LPCTSTR pszSurface, HINSTANCE hResInstance, bool bRenderImage, bool bTransparent)
{
	//设置资源
	m_pszSurface=pszSurface;
	m_pszResource=pszResource;
	m_hResInstance=hResInstance;

	//设置变量
	m_bRenderImage=bRenderImage;
	m_bTransparent=bTransparent;

	//调整控件
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		RectifyControl();
	}

	//更新窗口
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return true;
}

//调整位置
VOID CSkinButton::RectifyControl()
{
	//调整判断
	if (m_hWnd==NULL) return;

	//调整位置
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		//加载资源
		CBitImage ImageButton;
		ImageButton.LoadFromResource(m_hResInstance,m_pszResource);

		//调整大小
		SetWindowPos(NULL,0,0,ImageButton.GetWidth()/5,ImageButton.GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

		//透明设置
		if (m_bTransparent==true)
		{
			//创建区域
			CRgn RgnControl;
			ImageButton.CreateImageRegion(RgnControl,RGB(255,0,255));

			//设置区域
			SetWindowRgn(RgnControl,TRUE);
		}
	}
	else
	{
		//加载资源
		CBitImage ImageButton;
		ImageButton.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_BUTTON);

		//调整大小
		SetWindowPos(NULL,0,0,ImageButton.GetWidth()/5,ImageButton.GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}

	return;
}

//绘画文字
VOID CSkinButton::DrawButtonText(CDC * pDC, UINT uItemState)
{
	//获取文字
	CString strText;
	GetWindowText(strText);

	//绘画文字
	if (strText.IsEmpty()==FALSE)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//按钮状态
		bool bDisable=((uItemState&ODS_DISABLED)!=0);
		bool bButtonDown=((uItemState&ODS_SELECTED)!=0);

		//设置环境
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor((bDisable==true)?GetSysColor(COLOR_GRAYTEXT):m_crButtonText);

		//绘画文字
		rcClient.top+=(bButtonDown==true)?3:1;
		pDC->DrawText(strText,strText.GetLength(),rcClient,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	return;
}

//鼠标消息
VOID CSkinButton::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//状态处理
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//更新界面
		Invalidate(FALSE);

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		::TrackMouseEvent(&TrackMouseEvent);

		//过渡处理
		if (m_wImageIndex<MAX_TRANSITION_INDEX)
		{
			SetTimer(IDI_TRANSITION,TIME_TRANSITION,NULL);
		}
	}

	return;
}

//鼠标消息
LRESULT CSkinButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	//设置变量
	m_bHovering=false;

	//过渡处理
	if (m_wImageIndex>MIN_TRANSITION_INDEX)
	{
		SetTimer(IDI_TRANSITION,TIME_TRANSITION,NULL);
	}

	//更新界面
	Invalidate(FALSE);

	return 0;
}

//光标消息
BOOL CSkinButton::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//设置光标
	HINSTANCE hResInstance=GetModuleHandle(SKIN_CONTROL_DLL_NAME);
	SetCursor(LoadCursor(hResInstance,MAKEINTRESOURCE(IDC_SKIN_CURSOR)));

	return TRUE;
}

//销毁消息
VOID CSkinButton::OnNcDestroy()
{
	//状态变量
	m_bHovering=false;
	m_bRenderImage=true;

	//动画变量
	m_wImageIndex=MIN_TRANSITION_INDEX;

	__super::OnNcDestroy();
}

//绘画背景
BOOL CSkinButton::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//绘画函数
VOID CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义
	HDC hDC=lpDrawItemStruct->hDC;
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	//按钮状态
	bool bDisable=((lpDrawItemStruct->itemState&ODS_DISABLED)!=0);
	bool bButtonDown=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);

	//加载资源
	CBitImage ImageButton;
	if ((m_hResInstance!=NULL)&&(m_pszResource!=NULL))
	{
		//加载资源
		ImageButton.LoadFromResource(m_hResInstance,m_pszResource);

		//渲染资源
		if (m_bRenderImage==true)
		{
			CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
			if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(ImageButton);
		}

		//绘画按钮
		if (m_pszSurface!=NULL)
		{
			//加载资源
			CPngImage ImageSurface;
			ImageSurface.LoadImage(m_hResInstance,m_pszSurface);

			//绘画背景
			CDC * pDCImage=CDC::FromHandle(ImageButton.GetDC());
			ImageSurface.DrawImage(pDCImage,0,0,ImageButton.GetWidth(),ImageButton.GetHeight(),0,0);

			//释放资源
			ImageButton.ReleaseDC();
		}
	}
	else
	{
		//加载资源
		ImageButton.LoadFromResource(GetModuleHandle(SKIN_CONTROL_DLL_NAME),IDB_SKIN_BUTTON);

		//渲染资源
		CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
		if ((pSkinRenderManager!=NULL)&&(m_bRenderImage==true)) pSkinRenderManager->RenderImage(ImageButton);
	}

	//绘画界面
	if (ImageButton.IsNull()==false)
	{
		//变量定义
		INT nImagePos=0;
		INT nPartWidth=ImageButton.GetWidth()/5;

		//计算位置
		if (bDisable==true) nImagePos=nPartWidth*4;
		if ((bDisable==false)&&(bButtonDown==true)) nImagePos=nPartWidth;
		if ((bDisable==false)&&(bButtonDown==false)&&(m_bHovering==true)) nImagePos=nPartWidth*3;

		//状态判断
		bool bTransitionDraw=false;
		if ((m_bHovering==true)&&(m_wImageIndex<MAX_TRANSITION_INDEX)) bTransitionDraw=true;
		if ((m_bHovering==false)&&(m_wImageIndex>MIN_TRANSITION_INDEX)) bTransitionDraw=true;

		//绘画背景
		if ((bDisable==false)&&(bButtonDown==false)&&(bTransitionDraw==true))
		{
			//创建缓冲
			CBitImage ImageBuffer;
			ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

			//获取设备
			CImageDC BufferDC(ImageBuffer);
			CDC * pBufferDC=CDC::FromHandle(BufferDC);

			//设置设备
			pBufferDC->SetBkMode(TRANSPARENT);
			pBufferDC->SelectObject(pDC->GetCurrentFont());

			//绘画底图
			if (m_wImageIndex<MAX_TRANSITION_INDEX)
			{
				if ((rcClient.Width()==nPartWidth)&&(rcClient.Height()==ImageButton.GetHeight()))
				{
					ImageButton.BitBlt(BufferDC,0,0,rcClient.Width(),rcClient.Height(),0,0,SRCCOPY);
				}
				else
				{
					ImageButton.StretchBlt(BufferDC,0,0,rcClient.Width(),rcClient.Height(),0,0,nPartWidth,ImageButton.GetHeight(),SRCCOPY);
				}
			}

			//绘画过渡
			if (m_wImageIndex>MIN_TRANSITION_INDEX)
			{
				if (m_bTransparent==false)
				{
					ImageButton.AlphaDrawImage(pBufferDC,0,0,rcClient.Width(),rcClient.Height(),
						nPartWidth*3,0,nPartWidth,ImageButton.GetHeight(),m_wImageIndex*255/MAX_TRANSITION_INDEX);
				}
				else
				{
					ImageButton.BlendDrawImage(pBufferDC,0,0,rcClient.Width(),rcClient.Height(),
						nPartWidth*3,0,nPartWidth,ImageButton.GetHeight(),RGB(255,0,255),m_wImageIndex*255/MAX_TRANSITION_INDEX);
				}
			}

			//绘画文字
			DrawButtonText(pBufferDC,lpDrawItemStruct->itemState);

			//绘画界面
			if (m_bTransparent==false)
			{
				ImageBuffer.BitBlt(hDC,0,0,rcClient.Width(),rcClient.Height(),0,0);
			}
			else
			{
				ImageBuffer.TransDrawImage(pDC,0,0,rcClient.Width(),rcClient.Height(),0,0,RGB(255,0,255));
			}

			return;
		}
		else
		{
			//普通模式
			if (m_bTransparent==false)
			{
				if ((rcClient.Width()==nPartWidth)&&(rcClient.Height()==ImageButton.GetHeight()))
				{
					ImageButton.BitBlt(hDC,0,0,rcClient.Width(),rcClient.Height(),nImagePos,0,SRCCOPY);
				}
				else
				{
					ImageButton.StretchBlt(hDC,0,0,rcClient.Width(),rcClient.Height(),nImagePos,0,nPartWidth,ImageButton.GetHeight(),SRCCOPY);
				}
			}
			else
			{
				ImageButton.TransDrawImage(pDC,0,0,rcClient.Width(),rcClient.Height(),nImagePos,0,nPartWidth,ImageButton.GetHeight(),RGB(255,0,255));
			}

			//绘画文字
			DrawButtonText(pDC,lpDrawItemStruct->itemState);
		}
	}
	else
	{
		//默认界面
		if (bDisable==true) pDC->DrawFrameControl(&rcClient,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_INACTIVE);
		if ((bDisable==false)&&(bButtonDown==false)) pDC->DrawFrameControl(&rcClient,DFC_BUTTON,DFCS_BUTTONPUSH);
		if ((bDisable==false)&&(bButtonDown==true)) pDC->DrawFrameControl(&rcClient,DFC_BUTTON,DFCS_BUTTONPUSH|DFCS_PUSHED);

		//绘画文字
		DrawButtonText(pDC,lpDrawItemStruct->itemState);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////