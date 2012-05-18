#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void) : CDialog(IDD_GAME_SCORE)
{
	//��������
	m_lGameTax=0L;
	m_lGameScore=0;

	//������Դ
	m_ImageBackWin.SetLoadInfo(IDB_GAME_END_WIN,AfxGetInstanceHandle());
	m_ImageBackLost.SetLoadInfo(IDB_GAME_END_LOST,AfxGetInstanceHandle());
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
	m_lGameTax=0L;
	m_lGameScore=0;

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandleWin(&m_ImageBackWin);
	SetWindowPos(NULL,0,0,m_ImageBackWin.GetWidth(),m_ImageBackWin.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	CImageHandle ImageHandleLost(&m_ImageBackLost);
	SetWindowPos(NULL,0,0,m_ImageBackLost.GetWidth(),m_ImageBackLost.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//���д���
	CenterWindow(GetParent());

	return TRUE;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());
	//�滭����
	CImageHandle ImageHandleW(&m_ImageBackWin);
	CImageHandle ImageHandleL(&m_ImageBackLost);

	if(m_lGameScore>=0) 
		m_ImageBackWin.BitBlt(dc,0,0);
	else
		m_ImageBackLost.BitBlt(dc,0,0);
	
	//��ʾ����
	CRect rcDraw;
	TCHAR szBuffer[64]=TEXT("");

	//��ʾ˰��
	rcDraw.left=30;
	rcDraw.right=214;
	rcDraw.top=25+51;
	rcDraw.bottom=rcDraw.top+12;
	if(m_lGameScore>=0) _snprintf(szBuffer,sizeof(szBuffer),TEXT("��Ӯ���� %ld ��"),m_lGameScore);
	else _snprintf(szBuffer,sizeof(szBuffer),TEXT("��ʧȥ�� %ld ��"),-m_lGameScore);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	//��ʾ˰��
	rcDraw.left=120;
	rcDraw.right=190;
	rcDraw.top=110+51;
	rcDraw.bottom=rcDraw.top+12;
	_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_lGameTax);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

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
	m_lGameTax=0L;
	m_lGameScore=0;

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreView::SetGameTax(LONG lGameTax)
{
	//���ñ���
	if (m_lGameTax!=lGameTax)
	{
		m_lGameTax=lGameTax;
		Invalidate(FALSE);
	}

	return;
}

//���û���
void CScoreView::SetGameScore(LONG lScore)
{
	//���ñ���
	m_lGameScore=lScore;
	Invalidate(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////
