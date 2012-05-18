#include "StdAfx.h"
#include "GameFrameAvatar.h"
#include "GameFrameViewGDI.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameViewGDI, CGameFrameView)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameViewGDI::CGameFrameViewGDI()
{
	//缓冲变量
	m_SizeBackSurface.SetSize(0,0);
	m_RectDirtySurface.SetRect(0,0,0,0);

	return;
}

//析构函数
CGameFrameViewGDI::~CGameFrameViewGDI()
{
}

//重置界面
VOID CGameFrameViewGDI::ResetGameView()
{
	return;
}

//调整控件
VOID CGameFrameViewGDI::RectifyControl(INT nWidth, INT nHeight)
{
	return;
}

//界面更新
VOID CGameFrameViewGDI::InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight)
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

//绘画准备
VOID CGameFrameViewGDI::DrawUserReady(CDC * pDC, INT nXPos, INT nYPos)
{
	//加载资源
	CPngImage ImageUserReady;
	ImageUserReady.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_READY"));

	//获取大小
	CSize SizeUserReady;
	SizeUserReady.SetSize(ImageUserReady.GetWidth(),ImageUserReady.GetHeight());

	//绘画准备
	ImageUserReady.DrawImage(pDC,nXPos-SizeUserReady.cx/2,nYPos-SizeUserReady.cy/2);

	return;
}

//绘画时间
VOID CGameFrameViewGDI::DrawUserClock(CDC * pDC, INT nXPos, INT nYPos, WORD wUserClock)
{
	//绘画时间
	if ((wUserClock>0)&&(wUserClock<100))
	{
		//加载资源
		CPngImage ImageClockItem;
		CPngImage ImageClockBack;
		ImageClockItem.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_CLOCK_ITEM"));
		ImageClockBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_CLOCK_BACK"));

		//获取大小
		CSize SizeClockItem;
		CSize SizeClockBack;
		SizeClockBack.SetSize(ImageClockBack.GetWidth(),ImageClockBack.GetHeight());
		SizeClockItem.SetSize(ImageClockItem.GetWidth()/10,ImageClockItem.GetHeight());

		//绘画背景
		INT nXDrawPos=nXPos-SizeClockBack.cx/2;
		INT nYDrawPos=nYPos-SizeClockBack.cy/2;
		ImageClockBack.DrawImage(pDC,nXDrawPos,nYDrawPos);

		//绘画时间
		WORD nClockItem1=wUserClock/10;
		WORD nClockItem2=wUserClock%10;
		ImageClockItem.DrawImage(pDC,nXDrawPos+13,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem1*SizeClockItem.cx,0);
		ImageClockItem.DrawImage(pDC,nXDrawPos+28,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem2*SizeClockItem.cx,0);
	}

	return;
}

//绘画头像
VOID CGameFrameViewGDI::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//设置头像
	CGameFrameAvatar * pGameFrameAvatar=CGameFrameAvatar::GetInstance();
	if (pGameFrameAvatar!=NULL) pGameFrameAvatar->DrawUserAvatar(pDC,nXPos,nYPos,pIClientUserItem);

	return;
}

//绘画背景
VOID CGameFrameViewGDI::DrawViewImage(CDC * pDC, CBitImage & BitImage, BYTE cbDrawMode)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//无效区域
	CRect rcDirty;
	pDC->GetClipBox(&rcDirty);

	//绘画位图
	switch (cbDrawMode)
	{
	case DRAW_MODE_SPREAD:		//平铺模式
		{
			//加载资源
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//绘画位图
			for (INT nXPos=0;nXPos<rcClient.right;nXPos+=nImageWidth)
			{
				for (INT nYPos=0;nYPos<rcClient.bottom;nYPos+=nImageHeight)
				{
					bool bIntersect=BitImage.IsRectIntersect(rcDirty,nXPos,nYPos,nImageWidth,nImageHeight);
					if (bIntersect==true) BitImage.BitBlt(pDC->m_hDC,nXPos,nYPos);
				}
			}

			return;
		}
	case DRAW_MODE_CENTENT:		//居中模式
		{
			//变量定义
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//位置计算
			INT nXPos=(rcClient.Width()-BitImage.GetWidth())/2;
			INT nYPos=(rcClient.Height()-BitImage.GetHeight())/2;

			//绘画位图
			bool bIntersect=BitImage.IsRectIntersect(rcDirty,nXPos,nYPos,nImageWidth,nImageHeight);
			if (bIntersect==true) BitImage.BitBlt(pDC->m_hDC,nXPos,nYPos);

			return;
		}
	case DRAW_MODE_ELONGGATE:	//拉伸模式
		{
			//变量定义
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//绘画位图
			bool bIntersect=BitImage.IsRectIntersect(rcDirty,0,0,rcClient.Width(),rcClient.Height());
			if (bIntersect==true) BitImage.StretchBlt(pDC->m_hDC,0,0,rcClient.Width(),rcClient.Height(),0,0,nImageWidth,nImageHeight);

			return;
		}
	}

	return;
}

//绘画字符
VOID CGameFrameViewGDI::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect)
{
	//变量定义
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
	INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE;

	//保存格式
	COLORREF rcTextColor=pDC->SetTextColor(crFrame);
	UINT nTextAlign=pDC->SetTextAlign(TA_TOP|TA_LEFT);

	//绘画边框
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//计算位置
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//绘画字符
		pDC->DrawText(pszString,nStringLength,&rcFrame,nDrawFormat);
	}

	//绘画字符
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcRect,nDrawFormat);

	//还原设置
	pDC->SetTextAlign(nTextAlign);
	pDC->SetTextColor(rcTextColor);

	return;
}

//艺术字体
VOID CGameFrameViewGDI::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos)
{
	//变量定义
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//保存设置
	UINT nTextAlign=pDC->GetTextAlign();
	COLORREF rcTextColor=pDC->GetTextColor();

	//绘画边框
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->SetTextColor(crFrame);
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	//还原设置
	pDC->SetTextAlign(nTextAlign);
	pDC->SetTextColor(rcTextColor);

	return;
}

//绘画函数
VOID CGameFrameViewGDI::OnPaint()
{
	CPaintDC dc(this);

	//剪切区域
	CRect rcDirty;
	dc.GetClipBox(&rcDirty);

	//窗口尺寸
	CRect rcClient;
	GetClientRect(&rcClient);

	//调整尺寸
	if ((rcClient.Width()>m_SizeBackSurface.cx)||(rcClient.Height()>m_SizeBackSurface.cy))
	{
		//设置大小
		m_SizeBackSurface.cx=__max(m_SizeBackSurface.cx,rcClient.Width());
		m_SizeBackSurface.cy=__max(m_SizeBackSurface.cy,rcClient.Height());

		//更新区域
		m_RectDirtySurface.SetRect(0,0,rcClient.Width(),rcClient.Height());

		//销毁缓冲
		if (m_ImageBackSurface.IsNull()==false) m_ImageBackSurface.Destroy();

		//创建缓冲
		m_ImageBackSurface.Create(m_SizeBackSurface.cx,m_SizeBackSurface.cy,32);
	}

	//更新判断
	if (m_RectDirtySurface.IsRectEmpty()==FALSE)
	{
		//获取 DC
		HDC hSurfaceDC=m_ImageBackSurface.GetDC();
		CDC * pSurfaceDC=CDC::FromHandle(hSurfaceDC);

		//区域调整
		m_RectDirtySurface.top=__max(m_RectDirtySurface.top,rcClient.top);
		m_RectDirtySurface.left=__max(m_RectDirtySurface.left,rcClient.left);
		m_RectDirtySurface.right=__min(m_RectDirtySurface.right,rcClient.right);
		m_RectDirtySurface.bottom=__min(m_RectDirtySurface.bottom,rcClient.bottom);

		//创建区域
		CRgn RgnDirtySurface;
		RgnDirtySurface.CreateRectRgn(m_RectDirtySurface.left,m_RectDirtySurface.top,m_RectDirtySurface.right,m_RectDirtySurface.bottom);

		//设置 DC
		pSurfaceDC->SetBkMode(TRANSPARENT);
		pSurfaceDC->SelectClipRgn(&RgnDirtySurface);
		pSurfaceDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//绘画界面
		m_RectDirtySurface.SetRect(0,0,0,0);
		DrawGameView(pSurfaceDC,rcClient.Width(),rcClient.Height());

		//更新界面
		dc.BitBlt(rcDirty.left,rcDirty.top,rcDirty.Width(),rcDirty.Height(),pSurfaceDC,rcDirty.left,rcDirty.top,SRCCOPY);

		//清理资源
		RgnDirtySurface.DeleteObject();
		m_ImageBackSurface.ReleaseDC();
	}
	else
	{
		//更新界面
		m_ImageBackSurface.BitBlt(dc,rcDirty.left,rcDirty.top,rcDirty.Width(),rcDirty.Height(),rcDirty.left,rcDirty.top,SRCCOPY);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
