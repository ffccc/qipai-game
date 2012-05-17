#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	m_lMeCurGameScore=0L;
	//m_lMeCurGameReturnScore=0L;
	//m_lBankerCurGameScore=0L;

	//������Դ
	//m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
}

//��������
CScoreView::~CScoreView(void)
{
}

//��ʼ������
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	m_lMeCurGameScore=0L;
	//m_lMeCurGameReturnScore=0L;
	//m_lBankerCurGameScore=0L;
	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//���д���
	CenterWindow(GetParent());

	//CImageHandle ImageHandle(&m_ImageBack);
	//if (ImageHandle.IsResourceValid())
	{
		//������ʱͼ
		//CSkinImage BufferImage;
		//int nImageWidth=m_ImageBack.GetWidth();
		//int nImageHeight=m_ImageBack.GetHeight();
		//BufferImage.Create(nImageWidth,nImageHeight,32);
		//ImageHandle->BitBlt(BufferImage.GetDC(),0,0,nImageWidth,nImageHeight,0,0);
		//BufferImage.ReleaseDC();

		//��������
		//CRgn ImageRgn;
		//BufferImage.CreateImageRegion( ImageRgn, RGB( 255, 0, 255 ) );
		//if ( (( HRGN )ImageRgn) !=NULL)
		//{
		//	SetWindowPos(NULL,0,0,nImageWidth,nImageHeight,SWP_NOMOVE);
		//	SetWindowRgn(( HRGN )ImageRgn,TRUE);
		//	ImageRgn.DeleteObject();
		//}
	}
	////����͸��
	//ModifyStyleEx(0,0x00080000);
	//HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	//if(hInst) 
	//{ 
	//	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
	//	MYFUNC fun = NULL;
	//	//ȡ��SetLayeredWindowAttributes��������ָ�� 
	//	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
	//	if(fun)fun(this->GetSafeHwnd(),0,220,2); 
	//	FreeLibrary(hInst); 
	//}

	return TRUE;
}

//�ػ�����
void CScoreView::OnPaint() {

	CPaintDC dc(this); 
	////���� DC
	//dc.SetBkMode(TRANSPARENT);
	//dc.SetTextColor(RGB(250,250,250));
	//dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	//CImageHandle ImageHandle(&m_ImageBack);
	//m_ImageBack.BitBlt(dc,0,0);

	//��ʾ����
	CFont font;
	font.CreatePointFont(180,"����");
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

//�滭����
BOOL CScoreView::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//��������
	//��������
	m_lMeCurGameScore=0L;
	//m_lMeCurGameReturnScore=0L;
	//m_lBankerCurGameScore=0L;
	//�滭��ͼ
	if (m_hWnd!=NULL) Invalidate(NULL);

	return;
}

//����˰��
void CScoreView::SetGameTax(LONG lGameTax)
{
	//���ñ���

	return;
}

//���û���
void CScoreView::SetGameScore(LONG lMeCurGameScore, LONG lMeCurGameReturnScore, LONG lBankerCurGameScore)
{
	//���ñ���
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
	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}
//////////////////////////////////////////////////////////////////////////
