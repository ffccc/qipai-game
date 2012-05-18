#include "StdAfx.h"
#include "Resource.h"
#include "GameFrameMedalView.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameMedalView, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameMedalView::CGameFrameMedalView()
{
	//设置变量
	m_dwUserMedal=0L;

	return;
}

//析构函数
CGameFrameMedalView::~CGameFrameMedalView()
{
}

//设置奖牌
VOID CGameFrameMedalView::SetUserMedal(DWORD dwUserMedal)
{
	//设置变量
	if (m_dwUserMedal!=dwUserMedal)
	{
		//设置变量
		m_dwUserMedal=dwUserMedal;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}
}

//绘画背景
BOOL CGameFrameMedalView::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置缓冲
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextAlign(TA_LEFT|TA_TOP);
	pBufferDC->SetTextColor(RGB(250,250,250));
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//加载资源
	CImage ImageUserMedalL;
	CImage ImageUserMedalM;
	CImage ImageUserMedalR;
	ImageUserMedalL.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_MEDAL_L);
	ImageUserMedalM.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_MEDAL_M);
	ImageUserMedalR.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_MEDAL_R);

	//变量定义
	INT nXStartPos=ImageUserMedalL.GetWidth();
	INT nXConcludePos=rcClient.Width()-ImageUserMedalR.GetWidth();

	//绘画框架
	ImageUserMedalL.BitBlt(BufferDC,0,0);
	ImageUserMedalR.BitBlt(BufferDC,rcClient.Width()-ImageUserMedalR.GetWidth(),0);

	//绘画中间
	for (INT nXPos=nXStartPos;nXPos<nXConcludePos;nXPos+=ImageUserMedalM.GetWidth())
	{
		INT nCXDraw=__min(ImageUserMedalM.GetWidth(),nXConcludePos-nXPos);
		ImageUserMedalM.BitBlt(BufferDC,nXPos,0,nCXDraw,ImageUserMedalM.GetHeight(),0,0);
	}

	//加载资源
	CPngImage ImageCount;
	CPngImage ImageStringL;
	CPngImage ImageStringR;
	ImageCount.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("MEDAL_COUNT"));
	ImageStringL.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("MEDAL_STRING_L"));
	ImageStringR.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("MEDAL_STRING_R"));

	//获取大小
	CSize SizeCount;
	CSize SizeStringL;
	CSize SizeStringR;
	SizeCount.SetSize(ImageCount.GetWidth()/10L,ImageCount.GetHeight());
	SizeStringL.SetSize(ImageStringL.GetWidth(),ImageStringL.GetHeight());
	SizeStringR.SetSize(ImageStringR.GetWidth(),ImageStringR.GetHeight());

	//构造奖牌
	TCHAR szUserMedal[16]=TEXT("");
	_sntprintf(szUserMedal,CountArray(szUserMedal),TEXT("%ld"),m_dwUserMedal);

	//变量定义
	INT nNumCount=lstrlen(szUserMedal);
	INT nNumStart=(rcClient.Width()-nNumCount*SizeCount.cx)/2+10;

	//绘画字符
	ImageStringL.DrawImage(pBufferDC,nNumStart-SizeStringL.cx-3,4);
	ImageStringR.DrawImage(pBufferDC,nNumStart+nNumCount*SizeCount.cx+3,4);

	//绘画奖牌
	for (INT i=0;i<nNumCount;i++)
	{
		INT nXImage=(szUserMedal[i]-TEXT('0'))*SizeCount.cx;
		ImageCount.DrawImage(pBufferDC,nNumStart+i*SizeCount.cx,7,SizeCount.cx,SizeCount.cy,nXImage,0);
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
