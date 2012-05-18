#include "StdAfx.h"
#include "PlatformEvent.h"
#include "DlgSkinControl.h"

//////////////////////////////////////////////////////////////////////////////////

//窗口定义
#define CONTRL_CX					139									//窗口宽度
#define CONTRL_CY					190									//窗口高度

//子项位置
#define ITEM_ORIGIN_X				15									//原点位置
#define ITEM_ORIGIN_Y				35									//原点位置

//颜色原点
#define COLOR_ORIGIN_X				8									//原点位置
#define COLOR_ORIGIN_Y				125									//原点位置

//空隙位置
#define ITEM_SPACE_CX				10									//空隙位置
#define ITEM_SPACE_CY				10									//空隙位置
#define COLOR_SPACE_CY				16									//空隙位置

//色相范围
#define MIN_COLOR_H					0									//最小色相
#define MAX_COLOR_H					360									//最大色相

//饱和范围
#define MAX_COLOR_S					100									//最大饱和
#define MIN_COLOR_S					-100								//最小饱和

//亮度范围
#define MAX_COLOR_B					20									//最大亮度
#define MIN_COLOR_B					-20									//最小亮度

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSkinControl, CDialog)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgSkinControl::CDlgSkinControl()
{
	//蓝色方案
	m_RenderInfoItem[0].wColorH=DEF_COLOR_H;
	m_RenderInfoItem[0].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[0].dRectifyB=DEF_COLOR_B;

	//普蓝方案
	m_RenderInfoItem[1].wColorH=207;
	m_RenderInfoItem[1].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[1].dRectifyB=DEF_COLOR_S;

	//紫色方案
	m_RenderInfoItem[2].wColorH=279;
	m_RenderInfoItem[2].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[2].dRectifyB=DEF_COLOR_S;

	//绿色方案
	m_RenderInfoItem[3].wColorH=101;
	m_RenderInfoItem[3].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[3].dRectifyB=DEF_COLOR_S;

	//橙色方案
	m_RenderInfoItem[4].wColorH=30;
	m_RenderInfoItem[4].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[4].dRectifyB=DEF_COLOR_S;

	//亮橙方案
	m_RenderInfoItem[5].wColorH=14;
	m_RenderInfoItem[5].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[5].dRectifyB=DEF_COLOR_S;

	//桔红方案
	m_RenderInfoItem[6].wColorH=321;
	m_RenderInfoItem[6].dRectifyS=DEF_COLOR_S;
	m_RenderInfoItem[6].dRectifyB=DEF_COLOR_S;

	//灰色方案
	m_RenderInfoItem[7].wColorH=0;
	m_RenderInfoItem[7].dRectifyS=-1.00;
	m_RenderInfoItem[7].dRectifyB=-0.20;

	//设置变量
	m_wHoverItem=INVALID_WORD;
	m_wHoverColor=INVALID_WORD;
	m_wMouseDownColor=INVALID_WORD;

	//获取大小
	CPngImage ImageSkinItem;
	ImageSkinItem.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_ITEM"));
	m_SizeSkinItem.SetSize(ImageSkinItem.GetWidth()/8,ImageSkinItem.GetHeight()/2);

	//获取大小
	CPngImage ImageColorItem;
	ImageColorItem.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_COLOR"));
	m_SizeColorItem.SetSize(ImageColorItem.GetWidth(),ImageColorItem.GetHeight()/3);

	return;
}

//析构函数
CDlgSkinControl::~CDlgSkinControl()
{
}

//创建函数
BOOL CDlgSkinControl::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_wHoverItem=INVALID_WORD;
	m_wHoverColor=INVALID_WORD;
	m_wMouseDownColor=INVALID_WORD;
	m_CurrentRenderInfo=CSkinRenderManager::GetInstance()->GetSkinRenderInfo();

	//移动窗口
	SetWindowPos(NULL,0,0,CONTRL_CX,CONTRL_CY,SWP_NOZORDER|SWP_NOMOVE);

	return TRUE;
}

//获取选择
WORD CDlgSkinControl::GetCurrentItem(CPoint MousePoint)
{
	//位置搜索
	for (WORD nXPos=0;nXPos<4;nXPos++)
	{
		for (WORD nYPos=0;nYPos<2;nYPos++)
		{
			//位置判断
			if (MousePoint.x<nXPos*(m_SizeSkinItem.cx+ITEM_SPACE_CX)+ITEM_ORIGIN_X) continue;
			if (MousePoint.y<nYPos*(m_SizeSkinItem.cy+ITEM_SPACE_CY)+ITEM_ORIGIN_Y) continue;
			if (MousePoint.x>nXPos*(m_SizeSkinItem.cx+ITEM_SPACE_CX)+ITEM_ORIGIN_X+m_SizeSkinItem.cx) continue;
			if (MousePoint.y>nYPos*(m_SizeSkinItem.cy+ITEM_SPACE_CY)+ITEM_ORIGIN_Y+m_SizeSkinItem.cy) continue;

			return nYPos*4+nXPos;
		}
	}

	return INVALID_WORD;
}

//获取选择
WORD CDlgSkinControl::GetCurrentColor(CPoint MousePoint)
{
	//位置搜索
	for (WORD nIndex=0;nIndex<3;nIndex++)
	{
		//位置判断
		if (MousePoint.x<COLOR_ORIGIN_X) continue;
		if (MousePoint.x>(COLOR_ORIGIN_X+m_SizeColorItem.cx)) continue;
		if (MousePoint.y<(COLOR_ORIGIN_Y+(m_SizeColorItem.cy+COLOR_SPACE_CY)*nIndex)) continue;
		if (MousePoint.y>(COLOR_ORIGIN_Y+(m_SizeColorItem.cy+COLOR_SPACE_CY)*nIndex+m_SizeColorItem.cy)) continue;

		return nIndex;
	}

	return INVALID_WORD;
}

//更新颜色
VOID CDlgSkinControl::SetCurrentColor(WORD wColorIndex, INT nXExcursion)
{
	//设置变量
	switch (wColorIndex)
	{
	case 0:		//色相
		{
			WORD wHRange=MAX_COLOR_H-MIN_COLOR_H;
			m_CurrentRenderInfo.wColorH=(WORD)(nXExcursion*wHRange/m_SizeColorItem.cx+MIN_COLOR_H);
			break;
		}
	case 1:		//饱和
		{
			INT nSRange=MAX_COLOR_S-MIN_COLOR_S;
			m_CurrentRenderInfo.dRectifyS=(nXExcursion*nSRange/m_SizeColorItem.cx+MIN_COLOR_S)/100.0;
			break;
		}
	case 2:		//亮度
		{
			INT nBRange=MAX_COLOR_B-MIN_COLOR_B;
			m_CurrentRenderInfo.dRectifyB=(nXExcursion*nBRange/m_SizeColorItem.cx+MIN_COLOR_B)/100.0;
			break;
		}
	}

	return;
}

//重画消息
VOID CDlgSkinControl::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//变量定义
	CDC * pDC=CDC::FromHandle(ImageBuffer.GetDC());
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//绘画背景
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
	pDC->Draw3dRect(&rcClient,CSkinEdit::m_SkinAttribute.m_crEnableBorad,CSkinEdit::m_SkinAttribute.m_crEnableBorad);

	//加载资源
	CPngImage ImageSkinItem;
	ImageSkinItem.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_ITEM"));

	//绘画子项
	for (WORD nXPos=0;nXPos<4;nXPos++)
	{
		for (WORD nYPos=0;nYPos<2;nYPos++)
		{
			//变量定义
			WORD wIndex=nYPos*4+nXPos;
			WORD wImageIndex=(m_wHoverItem==wIndex)?1:0;
			tagSkinRenderInfo & SkinRenderInfo=m_RenderInfoItem[wIndex];

			//绘画子项
			ImageSkinItem.DrawImage(pDC,nXPos*(m_SizeSkinItem.cx+ITEM_SPACE_CX)+ITEM_ORIGIN_X,
				nYPos*(m_SizeSkinItem.cy+ITEM_SPACE_CY)+ITEM_ORIGIN_Y,m_SizeSkinItem.cx,m_SizeSkinItem.cy,
				(nYPos*4+nXPos)*m_SizeSkinItem.cx,wImageIndex*m_SizeSkinItem.cy);

			//绘画边框
			if ((m_CurrentRenderInfo.wColorH==SkinRenderInfo.wColorH)&&(m_CurrentRenderInfo.dRectifyS==SkinRenderInfo.dRectifyS)
				&&(m_CurrentRenderInfo.dRectifyB==SkinRenderInfo.dRectifyB))
			{
				INT nXDrawPos=nXPos*(m_SizeSkinItem.cx+ITEM_SPACE_CX)+ITEM_ORIGIN_X-2;
				INT nYDrawPos=nYPos*(m_SizeSkinItem.cy+ITEM_SPACE_CY)+ITEM_ORIGIN_Y-2;
				pDC->Draw3dRect(nXDrawPos,nYDrawPos,m_SizeSkinItem.cx+4,m_SizeSkinItem.cy+4,RGB(0,0,0),RGB(0,0,0));
			}
		}
	}

	//加载资源
	CPngImage ImageSkinColor;
	CPngImage ImageSkinButton;
	ImageSkinColor.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_COLOR"));
	ImageSkinButton.LoadImage(AfxGetInstanceHandle(),TEXT("SKIN_BUTTON"));

	//绘画色条
	for (WORD nIndex=0;nIndex<3;nIndex++)
	{
		//绘画色条
		INT nXDrawPos=COLOR_ORIGIN_X;
		INT nYDrawPos=COLOR_ORIGIN_Y+(m_SizeColorItem.cy+COLOR_SPACE_CY)*nIndex;
		ImageSkinColor.DrawImage(pDC,nXDrawPos,nYDrawPos,m_SizeColorItem.cx,m_SizeColorItem.cy,0,m_SizeColorItem.cy*nIndex);

		//绘画滑块
		switch (nIndex)
		{
		case 0:		//色相
			{
				INT nXButtonPos=nXDrawPos+(m_SizeColorItem.cx-6)*m_CurrentRenderInfo.wColorH/(MAX_COLOR_H-MIN_COLOR_H)+3;
				ImageSkinButton.DrawImage(pDC,nXButtonPos-ImageSkinButton.GetWidth()/2,nYDrawPos-5);
				break;
			}
		case 1:		//饱和
			{
				DOUBLE dRectifyS=(m_CurrentRenderInfo.dRectifyS*100-MIN_COLOR_S)/(MAX_COLOR_S-MIN_COLOR_S);
				INT nXButtonPos=(INT)(nXDrawPos+(DOUBLE)(m_SizeColorItem.cx-6)*dRectifyS+3);
				ImageSkinButton.DrawImage(pDC,nXButtonPos-ImageSkinButton.GetWidth()/2,nYDrawPos-5);
				break;
			}
		case 2:		//亮度
			{
				DOUBLE dRectifyB=(m_CurrentRenderInfo.dRectifyB*100-MIN_COLOR_B)/(MAX_COLOR_B-MIN_COLOR_B);
				INT nXButtonPos=(INT)(nXDrawPos+(DOUBLE)(m_SizeColorItem.cx-6)*dRectifyB+3);
				ImageSkinButton.DrawImage(pDC,nXButtonPos-ImageSkinButton.GetWidth()/2,nYDrawPos-5);
				break;
			}
		}
	}

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画字符
	LPCTSTR pszCustom=TEXT("自我定制：");
	LPCTSTR pszRecommend=TEXT("推荐配色：");
	pDC->TextOut(10,102,pszCustom,lstrlen(pszCustom));
	pDC->TextOut(10,14,pszRecommend,lstrlen(pszRecommend));

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pDC,0,0,SRCCOPY);

	//清理资源
	ImageBuffer.ReleaseDC();

	return;
}

//绘画背景
BOOL CDlgSkinControl::OnEraseBkgnd(CDC * pDC)
{
	//更新窗口
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//焦点消息
VOID CDlgSkinControl::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	//注销窗口
	DestroyWindow();

	return;
}

//鼠标消息
VOID CDlgSkinControl::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags,Point);

	//设置颜色
	if (m_wMouseDownColor!=INVALID_WORD)
	{
		//更新颜色
		INT nXExcursion=Point.x-COLOR_ORIGIN_X;
		SetCurrentColor(m_wMouseDownColor,__min(__max(nXExcursion,0),m_SizeColorItem.cx));

		//保存配置
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal!=NULL) pParameterGlobal->m_SkinRenderInfo=m_CurrentRenderInfo;

		//设置资源
		CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
		if (pSkinResourceManager!=NULL) pSkinResourceManager->SetSkinResource(m_CurrentRenderInfo);

		//事件通知
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_SKIN_CONFIG_UPDATE,0L);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
VOID CDlgSkinControl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);

	//取消捕获
	if (m_wMouseDownColor!=INVALID_WORD)
	{
		//取消捕获
		ReleaseCapture();

		//设置变量
		m_wMouseDownColor=INVALID_WORD;
	}

	return;
}

//鼠标消息
VOID CDlgSkinControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//设置资源
	if (m_wHoverColor!=INVALID_WORD)
	{
		//捕获鼠标
		SetCapture();

		//设置变量
		m_wMouseDownColor=m_wHoverColor;

		//更新颜色
		INT nXExcursion=Point.x-COLOR_ORIGIN_X;
		SetCurrentColor(m_wMouseDownColor,__min(__max(nXExcursion,0),m_SizeColorItem.cx));

		//保存配置
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal!=NULL) pParameterGlobal->m_SkinRenderInfo=m_CurrentRenderInfo;

		//设置资源
		CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
		if (pSkinResourceManager!=NULL) pSkinResourceManager->SetSkinResource(m_CurrentRenderInfo);

		//事件通知
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_SKIN_CONFIG_UPDATE,0L);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//设置资源
	if (m_wHoverItem!=INVALID_WORD)
	{
		//设置变量
		m_CurrentRenderInfo=m_RenderInfoItem[m_wHoverItem];

		//保存配置
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal!=NULL) pParameterGlobal->m_SkinRenderInfo=m_CurrentRenderInfo;

		//设置资源
		CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();
		if (pSkinResourceManager!=NULL) pSkinResourceManager->SetSkinResource(m_CurrentRenderInfo);

		//事件通知
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_SKIN_CONFIG_UPDATE,0L);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//光标消息
BOOL CDlgSkinControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//设置选择
	WORD wHoverItem=m_wHoverItem;
	m_wHoverItem=GetCurrentItem(MousePoint);
	m_wHoverColor=GetCurrentColor(MousePoint);

	//更新界面
	if (m_wHoverItem!=wHoverItem)
	{
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//设置光标
	if ((m_wHoverItem!=INVALID_WORD)||(m_wHoverColor!=INVALID_WORD))
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//////////////////////////////////////////////////////////////////////////////////
