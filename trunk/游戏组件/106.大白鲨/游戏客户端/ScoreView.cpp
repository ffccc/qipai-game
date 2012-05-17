#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CScoreView::CScoreView(void)
{
	//设置数据
	m_lMeCurGameScore=0L;
	//m_lMeCurGameReturnScore=0L;
	//m_lBankerCurGameScore=0L;

	//加载资源
	//m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
}

//析构函数
CScoreView::~CScoreView(void)
{
}

//初始化函数
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//设置数据
	m_lMeCurGameScore=0L;
	//m_lMeCurGameReturnScore=0L;
	//m_lBankerCurGameScore=0L;
	//设置界面
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//居中窗口
	CenterWindow(GetParent());

	//CImageHandle ImageHandle(&m_ImageBack);
	//if (ImageHandle.IsResourceValid())
	{
		//创建临时图
		//CSkinImage BufferImage;
		//int nImageWidth=m_ImageBack.GetWidth();
		//int nImageHeight=m_ImageBack.GetHeight();
		//BufferImage.Create(nImageWidth,nImageHeight,32);
		//ImageHandle->BitBlt(BufferImage.GetDC(),0,0,nImageWidth,nImageHeight,0,0);
		//BufferImage.ReleaseDC();

		//创建区域
		//CRgn ImageRgn;
		//BufferImage.CreateImageRegion( ImageRgn, RGB( 255, 0, 255 ) );
		//if ( (( HRGN )ImageRgn) !=NULL)
		//{
		//	SetWindowPos(NULL,0,0,nImageWidth,nImageHeight,SWP_NOMOVE);
		//	SetWindowRgn(( HRGN )ImageRgn,TRUE);
		//	ImageRgn.DeleteObject();
		//}
	}
	////设置透明
	//ModifyStyleEx(0,0x00080000);
	//HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	//if(hInst) 
	//{ 
	//	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
	//	MYFUNC fun = NULL;
	//	//取得SetLayeredWindowAttributes（）函数指针 
	//	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
	//	if(fun)fun(this->GetSafeHwnd(),0,220,2); 
	//	FreeLibrary(hInst); 
	//}

	return TRUE;
}

//重画函数
void CScoreView::OnPaint() {

	CPaintDC dc(this); 
	////设置 DC
	//dc.SetBkMode(TRANSPARENT);
	//dc.SetTextColor(RGB(250,250,250));
	//dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//绘画背景
	//CImageHandle ImageHandle(&m_ImageBack);
	//m_ImageBack.BitBlt(dc,0,0);

	//显示分数
	CFont font;
	font.CreatePointFont(180,"黑体");
	CFont* pOldFont = dc.SelectObject(&font);
	dc.SelectObject(&font);
	dc.SetTextAlign(TA_CENTER);
	dc.SetTextColor(RGB(255,0,0));
	//TCHAR szBuffer3[1024];
	//_snprintf(szBuffer3,sizeof(szBuffer3),TEXT("%d"),m_lMeCurGameReturnScore);
	//dc.TextOut(328,70,szBuffer3,lstrlen(szBuffer3));
	//DrawNumberStringWithSpace(&dc,m_lMeCurGameReturnScore,328,70);

//  	TCHAR szBuffer3[1024];
//  	_snprintf(szBuffer3,sizeof(szBuffer3),TEXT("----"));
//  	dc.TextOut(328,70,szBuffer3,lstrlen(szBuffer3));


	dc.SetBkColor(RGB(214,214,214));
	if (m_lMeCurGameScore>0)
		dc.SetTextColor(RGB(255,0,0));
	else
		dc.SetTextColor(RGB(255,0,0));
	//TCHAR szBuffer1[1024];
	//_snprintf(szBuffer1,sizeof(szBuffer1),TEXT("%d"),m_lMeCurGameScore);
	//dc.TextOut(177,70,szBuffer1,lstrlen(szBuffer1));
	DrawNumberStringWithSpace(&dc,m_lMeCurGameScore,177,70);

	//if (m_lBankerCurGameScore>0)
	//	dc.SetTextColor(RGB(255,0,0));
	//else
	//	dc.SetTextColor(RGB(255,244,0));
	//TCHAR szBuffer2[1024];
	//_snprintf(szBuffer2,sizeof(szBuffer2),TEXT("%d"),m_lBankerCurGameScore);
	//dc.TextOut(177,102,szBuffer2,lstrlen(szBuffer2));
	//DrawNumberStringWithSpace(&dc,m_lBankerCurGameScore,177,102);
	return;
}

//绘画背景
BOOL CScoreView::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//鼠标消息
void CScoreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//消息模拟
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//重置积分
void CScoreView::ResetScore()
{
	//设置数据
	//设置数据
	m_lMeCurGameScore=0L;
	//m_lMeCurGameReturnScore=0L;
	//m_lBankerCurGameScore=0L;
	//绘画视图
	if (m_hWnd!=NULL) Invalidate(NULL);

	return;
}

//设置税收
void CScoreView::SetGameTax(LONG lGameTax)
{
	//设置变量

	return;
}

//设置积分
void CScoreView::SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore)
{
	//设置变量
	m_lMeCurGameScore=lMeCurGameScore;
	//m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	//m_lBankerCurGameScore=lBankerCurGameScore;
	return;
}

void CScoreView::DrawNumberStringWithSpace(CDC * pDC, LONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT("");
	strNumber.Format(TEXT("%ld"),lNumber>0?lNumber:-lNumber);
	int p=strNumber.GetLength()-4;
	while(p>0)
	{
		strNumber.Insert(p,",");
		p-=4;
	}
	if(lNumber<0)
	{
		strNumber=TEXT("-")+strNumber;
	}
	//输出数字
	pDC->TextOut(nXPos,nYPos,strNumber);
}
//////////////////////////////////////////////////////////////////////////
