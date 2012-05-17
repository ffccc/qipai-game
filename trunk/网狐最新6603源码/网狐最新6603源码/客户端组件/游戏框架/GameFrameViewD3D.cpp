#include "StdAfx.h"
#include "GameFrameAvatar.h"
#include "GameFrameViewD3D.h"
#include ".\gameframeviewd3d.h"

//////////////////////////////////////////////////////////////////////////////////

//渲染消息
#define WM_D3D_RENDER				WM_USER+300							//渲染消息

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameViewD3D, CGameFrameView)

	//系统消息
	ON_WM_SIZE()
	ON_WM_PAINT()

	//自定消息
	ON_MESSAGE(WM_D3D_RENDER, OnMessageD3DRender)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameViewD3D::CGameFrameViewD3D()
{
	//状态变量
	m_bInitD3D=false;
	m_hRenderThread=NULL;

	//帧数统计
	m_dwDrawBenchmark=0;
	m_dwDrawLastCount=0;
	m_dwDrawCurrentCount=0L;

	//辅助变量
	m_RectDirtySurface.SetRect(0,0,0,0);

	return;
}

//析构函数
CGameFrameViewD3D::~CGameFrameViewD3D()
{
}

//重置界面
VOID CGameFrameViewD3D::ResetGameView()
{
	return;
}

//调整控件
VOID CGameFrameViewD3D::RectifyControl(INT nWidth, INT nHeight)
{
	return;
}

//界面更新
VOID CGameFrameViewD3D::InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//构造位置
	CRect rcInvalid;
	rcInvalid.SetRect(nXPos,nYPos,nXPos+nWidth,nYPos+nHeight);

	//位置调整
	if (rcInvalid.IsRectNull()==TRUE) GetClientRect(&rcInvalid);

	//设置矩形
	if (m_RectDirtySurface.IsRectEmpty()==FALSE)
	{
		//设置变量
		m_RectDirtySurface.top=__min(m_RectDirtySurface.top,rcInvalid.top);
		m_RectDirtySurface.left=__min(m_RectDirtySurface.left,rcInvalid.left);
		m_RectDirtySurface.right=__max(m_RectDirtySurface.right,rcInvalid.right);
		m_RectDirtySurface.bottom=__max(m_RectDirtySurface.bottom,rcInvalid.bottom);
	}
	else
	{
		//设置变量
		m_RectDirtySurface=rcInvalid;
	}

	//刷新窗口
	InvalidateRect(&rcInvalid,FALSE);

	return;
}

//配置设备
VOID CGameFrameViewD3D::OnInitDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//设置变量
	m_bInitD3D=true;

	//获取对象
	ASSERT(CSkinResourceManager::GetInstance()!=NULL);
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();

	//获取字体
	LOGFONT LogFont;
	pSkinResourceManager->GetDefaultFont().GetLogFont(&LogFont);

	//创建对象
	m_D3DFont.CreateFont(LogFont,0L);

	//虚拟设备
	m_VirtualEngine.SetD3DDevice(&m_D3DDevice);

	//设置头像
	CGameFrameAvatar * pGameFrameAvatar=CGameFrameAvatar::GetInstance();
	if (pGameFrameAvatar!=NULL) pGameFrameAvatar->Initialization(pD3DDevice);

	//加载资源
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_D3DTextureReady.LoadImage(pD3DDevice,hInstance,TEXT("USER_READY"),TEXT("PNG"));
	m_D3DTextureMember.LoadImage(pD3DDevice,hInstance,TEXT("MEMBER_FLAG"),TEXT("PNG"));
	m_D3DTextureClockItem.LoadImage(pD3DDevice,hInstance,TEXT("USER_CLOCK_ITEM"),TEXT("PNG"));
	m_D3DTextureClockBack.LoadImage(pD3DDevice,hInstance,TEXT("USER_CLOCK_BACK"),TEXT("PNG"));

	//配置窗口
	InitGameView(pD3DDevice,nWidth,nHeight);

	return;
}

//丢失设备
VOID CGameFrameViewD3D::OnLostDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	return;
}

//重置设备
VOID CGameFrameViewD3D::OnResetDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	return;
}

//渲染窗口
VOID CGameFrameViewD3D::OnRenderWindow(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//动画驱动
	CartoonMovie();

	//绘画窗口
	DrawGameView(pD3DDevice,nWidth,nHeight);

	//虚拟框架
	m_VirtualEngine.DrawWindows(pD3DDevice);

	//设置变量
	m_dwDrawCurrentCount++;

	//累计判断
	if ((GetTickCount()-m_dwDrawBenchmark)>=1000L)
	{
		//设置变量
		m_dwDrawLastCount=m_dwDrawCurrentCount;

		//统计还原
		m_dwDrawCurrentCount=0L;
		m_dwDrawBenchmark=GetTickCount();
	}

	return;
}

//消息解释
BOOL CGameFrameViewD3D::PreTranslateMessage(MSG * pMsg)
{
	//虚拟框架
	if (m_VirtualEngine.PreTranslateMessage(pMsg->message,pMsg->wParam,pMsg->lParam)==true)
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//消息解释
LRESULT CGameFrameViewD3D::DefWindowProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	//虚拟框架
	if (m_VirtualEngine.DefWindowProc(nMessage,wParam,lParam)==true)
	{
		return 0L;
	}

	return __super::DefWindowProc(nMessage,wParam,lParam);
}

//渲染线程
VOID CGameFrameViewD3D::StartRenderThread()
{
	//效验状态
	ASSERT(m_hRenderThread==NULL);
	if (m_hRenderThread!=NULL) return;

	//创建线程
	m_hRenderThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)D3DRenderThread,(LPVOID)(m_hWnd),0L,0L);

	return;
}

//绘画准备
VOID CGameFrameViewD3D::DrawUserReady(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos)
{
	//获取大小
	CSize SizeUserReady;
	SizeUserReady.SetSize(m_D3DTextureReady.GetWidth(),m_D3DTextureReady.GetHeight());

	//绘画准备
	m_D3DTextureReady.DrawImage(pD3DDevice,nXPos-SizeUserReady.cx/2,nYPos-SizeUserReady.cy/2);

	return;
}

//绘画标志
VOID CGameFrameViewD3D::DrawOrderFlag(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, BYTE cbImageIndex)
{
	//获取大小
	CSize SizeMember;
	SizeMember.SetSize(m_D3DTextureMember.GetWidth()/3,m_D3DTextureMember.GetHeight());

	//绘画标志
	m_D3DTextureMember.DrawImage(pD3DDevice,nXPos,nYPos,SizeMember.cx,SizeMember.cy,SizeMember.cx*cbImageIndex,0);

	return;
}

//绘画时间
VOID CGameFrameViewD3D::DrawUserClock(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wUserClock)
{
	//绘画时间
	if ((wUserClock>0)&&(wUserClock<100))
	{
		//获取大小
		CSize SizeClockItem;
		CSize SizeClockBack;
		SizeClockBack.SetSize(m_D3DTextureClockBack.GetWidth(),m_D3DTextureClockBack.GetHeight());
		SizeClockItem.SetSize(m_D3DTextureClockItem.GetWidth()/10,m_D3DTextureClockItem.GetHeight());

		//绘画背景
		INT nXDrawPos=nXPos-SizeClockBack.cx/2;
		INT nYDrawPos=nYPos-SizeClockBack.cy/2;
		m_D3DTextureClockBack.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos);

		//绘画时间
		WORD nClockItem1=wUserClock/10;
		WORD nClockItem2=wUserClock%10;
		m_D3DTextureClockItem.DrawImage(pD3DDevice,nXDrawPos+13,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem1*SizeClockItem.cx,0);
		m_D3DTextureClockItem.DrawImage(pD3DDevice,nXDrawPos+28,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem2*SizeClockItem.cx,0);
	}

	return;
}

//绘画头像
VOID CGameFrameViewD3D::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//设置头像
	CGameFrameAvatar * pGameFrameAvatar=CGameFrameAvatar::GetInstance();
	if (pGameFrameAvatar!=NULL) pGameFrameAvatar->DrawUserAvatar(pD3DDevice,nXPos,nYPos,pIClientUserItem);

	return;
}

//绘画背景
VOID CGameFrameViewD3D::DrawViewImage(CD3DDevice * pD3DDevice, CD3DTexture & D3DTexture, BYTE cbDrawMode)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画位图
	switch (cbDrawMode)
	{
	case DRAW_MODE_SPREAD:		//平铺模式
		{
			//获取大小
			CSize SizeTexture;
			SizeTexture.SetSize(D3DTexture.GetWidth(),D3DTexture.GetHeight());

			//绘画位图
			for (INT nXPos=0;nXPos<rcClient.right;nXPos+=SizeTexture.cx)
			{
				for (INT nYPos=0;nYPos<rcClient.bottom;nYPos+=SizeTexture.cy)
				{
					D3DTexture.DrawImage(pD3DDevice,nXPos,nYPos);
				}
			}

			return;
		}
	case DRAW_MODE_CENTENT:		//居中模式
		{
			//获取大小
			CSize SizeTexture;
			SizeTexture.SetSize(D3DTexture.GetWidth(),D3DTexture.GetHeight());

			//位置计算
			INT nXPos=(rcClient.Width()-SizeTexture.cx)/2;
			INT nYPos=(rcClient.Height()-SizeTexture.cy)/2;

			//绘画位图
			D3DTexture.DrawImage(pD3DDevice,nXPos,nYPos);

			return;
		}
	case DRAW_MODE_ELONGGATE:	//拉伸模式
		{
			//获取大小
			CSize SizeTexture;
			SizeTexture.SetSize(D3DTexture.GetWidth(),D3DTexture.GetHeight());

			//绘画位图
			D3DTexture.DrawImage(pD3DDevice,0,0,rcClient.Width(),rcClient.Height(),0,0,SizeTexture.cx,SizeTexture.cy);

			return;
		}
	}

	return;
}

//绘画数字
VOID CGameFrameViewD3D::DrawNumberString(CD3DDevice * pD3DDevice, CD3DTexture & D3DTexture, LONG lNumber, INT nXPos, INT nYPos)
{
	//效验状态
	ASSERT(D3DTexture.IsNull()==false);
	if (D3DTexture.IsNull()==true) return;

	//获取大小
	CSize SizeNumber;
	SizeNumber.SetSize(D3DTexture.GetWidth()/10,D3DTexture.GetHeight());

	//变量定义
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;

	//计算数目
	do
	{
		lNumberCount++;
		lNumberTemp/=10L;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-SizeNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeNumber.cx/2-SizeNumber.cx;

	//绘画桌号
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=lNumber%10L;
		D3DTexture.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeNumber.cx,SizeNumber.cy,lCellNumber*SizeNumber.cx,0);

		//设置变量
		lNumber/=10;
		nXDrawPos-=SizeNumber.cx;
	};

	return;
}

//绘画背景
VOID CGameFrameViewD3D::DrawViewImage(CD3DDevice * pD3DDevice, CD3DSprite & D3DSprite, BYTE cbDrawMode)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画位图
	switch (cbDrawMode)
	{
	case DRAW_MODE_SPREAD:		//平铺模式
		{
			//获取大小
			CSize SizeTexture;
			SizeTexture.SetSize(D3DSprite.GetWidth(),D3DSprite.GetHeight());

			//绘画位图
			for (INT nXPos=0;nXPos<rcClient.right;nXPos+=SizeTexture.cx)
			{
				for (INT nYPos=0;nYPos<rcClient.bottom;nYPos+=SizeTexture.cy)
				{
					D3DSprite.DrawImage(pD3DDevice,nXPos,nYPos);
				}
			}

			return;
		}
	case DRAW_MODE_CENTENT:		//居中模式
		{
			//获取大小
			CSize SizeTexture;
			SizeTexture.SetSize(D3DSprite.GetWidth(),D3DSprite.GetHeight());

			//位置计算
			INT nXPos=(rcClient.Width()-SizeTexture.cx)/2;
			INT nYPos=(rcClient.Height()-SizeTexture.cy)/2;

			//绘画位图
			D3DSprite.DrawImage(pD3DDevice,nXPos,nYPos);

			return;
		}
	case DRAW_MODE_ELONGGATE:	//拉伸模式
		{
			//获取大小
			CSize SizeTexture;
			SizeTexture.SetSize(D3DSprite.GetWidth(),D3DSprite.GetHeight());

			//绘画位图
			D3DSprite.DrawImage(pD3DDevice,0,0,rcClient.Width(),rcClient.Height(),0,0,SizeTexture.cx,SizeTexture.cy);

			return;
		}
	}

	return;
}

//绘画数字
VOID CGameFrameViewD3D::DrawNumberString(CD3DDevice * pD3DDevice, CD3DSprite & D3DSprite, LONG lNumber, INT nXPos, INT nYPos)
{
	//效验状态
	ASSERT(D3DSprite.IsNull()==false);
	if (D3DSprite.IsNull()==true) return;

	//获取大小
	CSize SizeNumber;
	SizeNumber.SetSize(D3DSprite.GetWidth()/10,D3DSprite.GetHeight());

	//变量定义
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;

	//计算数目
	do
	{
		lNumberCount++;
		lNumberTemp/=10L;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-SizeNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeNumber.cx/2-SizeNumber.cx;

	//绘画桌号
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=lNumber%10L;
		D3DSprite.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,SizeNumber.cx,SizeNumber.cy,lCellNumber*SizeNumber.cx,0);

		//设置变量
		lNumber/=10;
		nXDrawPos-=SizeNumber.cx;
	};

	return;
}

//绘画字符
VOID CGameFrameViewD3D::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw, UINT nFormat, D3DCOLOR crText, D3DCOLOR crFrame)
{
	//变量定义
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//计算位置
		CRect rcFrame;
		rcFrame.top=rcDraw.top+nYExcursion[i];
		rcFrame.left=rcDraw.left+nXExcursion[i];
		rcFrame.right=rcDraw.right+nXExcursion[i];
		rcFrame.bottom=rcDraw.bottom+nYExcursion[i];

		//绘画字符
		m_D3DFont.DrawText(pD3DDevice,pszString,&rcFrame,nFormat,crFrame);
	}

	//绘画字符
	m_D3DFont.DrawText(pD3DDevice,pszString,&rcDraw,nFormat,crText);

	return;
}

//绘画字符
VOID CGameFrameViewD3D::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT nFormat, D3DCOLOR crText, D3DCOLOR crFrame)
{
	//变量定义
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		m_D3DFont.DrawText(pD3DDevice,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i],nFormat,crFrame);
	}

	//绘画字符
	m_D3DFont.DrawText(pD3DDevice,pszString,nXPos,nYPos,nFormat,crText);

	return;
}

//输出字体
VOID CGameFrameViewD3D::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw,UINT uFormat, D3DCOLOR D3DColor)
{
	//输出字体
	m_D3DFont.DrawText(pD3DDevice,pszString,rcDraw,uFormat,D3DColor);

	return;
}

//输出字体
VOID CGameFrameViewD3D::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT uFormat, D3DCOLOR D3DColor)
{
	//输出字体
	m_D3DFont.DrawText(pD3DDevice,pszString,nXPos,nYPos,uFormat,D3DColor);

	return;
}

//绘画函数
VOID CGameFrameViewD3D::OnPaint()
{
	CPaintDC dc(this);

	//渲染设备
	if (m_bInitD3D==true) m_D3DDevice.RenderD3DDevice();

	return;
}

//位置变化
VOID CGameFrameViewD3D::OnSize(UINT nType, INT cx, INT cy)
{
	//重置设备
	if (m_bInitD3D==false)
	{
		if ((cx>=0L)&&(cy>0L))
		{
			//配置环境
			if (m_D3DDirect.CreateD3DDirect()==false)
			{
				ASSERT(FALSE);
				return;
			}

			//创建设备
			if (m_D3DDevice.CreateD3DDevice(m_hWnd,this)==false)
			{
				ASSERT(FALSE);
				return;
			}
		}
	}
	else
	{
		//重置设备
		m_D3DDevice.ResetD3DDevice();
	}

	__super::OnSize(nType, cx, cy);
}

//渲染消息
LRESULT CGameFrameViewD3D::OnMessageD3DRender(WPARAM wParam, LPARAM lParam)
{
	//渲染设备
	if (m_bInitD3D==true)
	{
		m_D3DDevice.RenderD3DDevice();
	}

	return 0L;
}

//渲染线程
VOID CGameFrameViewD3D::D3DRenderThread(LPVOID pThreadData)
{
	//效验参数
	ASSERT(pThreadData!=NULL);
	if (pThreadData==NULL) return;

	//渲染循环
	while (TRUE)
	{
		//渲染等待
		Sleep(10);

		//发送消息
		::SendMessage((HWND)pThreadData,WM_D3D_RENDER,0L,0L);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
