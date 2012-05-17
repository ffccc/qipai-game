#include "StdAfx.h"
#include "ScoreView.h"
#include ".\scoreview.h"


#define TIMES				5							//ʱ�����

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	m_lGameScore = 0L;
	m_bStart = false;
	m_bTimes = TIMES;

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_WIN,AfxGetInstanceHandle());
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
	m_lGameScore = 0L;

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth()/3,m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return TRUE;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	if(this->IsWindowVisible()!=TRUE)return;

	CPaintDC dc(this); 
	//SetupRegion(&dc,m_ImageBack,RGB(255,0,255));

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	if (m_lGameScore > 0L)
	{
	//	m_ImageBack.BitBlt(dc.GetSafeHdc(),0,0,m_ImageBack.GetWidth()/3,m_ImageBack.GetHeight(),0,0,SRCCOPY);
		m_ImageBack.AlphaDrawImage(&dc,0,0,m_ImageBack.GetWidth()/3,m_ImageBack.GetHeight(),0,0,RGB(255,0,255));
	}
	else if(m_lGameScore == 0L)
	{
		m_ImageBack.AlphaDrawImage(&dc,0,0,m_ImageBack.GetWidth()/3,m_ImageBack.GetHeight(),m_ImageBack.GetWidth()/3,0,RGB(255,0,255));
	}
	else
	{
		m_ImageBack.AlphaDrawImage(&dc,0,0,m_ImageBack.GetWidth()/3,m_ImageBack.GetHeight(),2*m_ImageBack.GetWidth()/3,0,RGB(255,0,255));
	}

	//��ʾ����
	TCHAR szBuffer[64]=TEXT("");
	CRect rcScore(60,60,60+65,60+19);

	//�û�����
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lGameScore);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	//dc.SetTextColor(RGB(0,0,0));

	//��ʾ����
	TCHAR szBuffe[64]=TEXT("");
	int ileft=52;
	int itop=8;
	CRect rcScor(ileft,itop,ileft+15,itop+15);

	//������Դ
	CFont InfoFont;
	InfoFont.CreateFont(-14,0,0,0,10,0,0,0,4,0,2,1,2,TEXT("����"));

	//���� DC
	dc.SetTextAlign(TA_CENTER|TA_TOP);
	CFont * pOldFont=dc.SelectObject(&InfoFont);

	//�滭��Ϣ
	_snprintf(szBuffe,sizeof(szBuffer),TEXT("%ld"),m_bTimes);
	//dc.Ellipse(&rcScor);
	dc.SetTextColor(RGB(255,255,255));
	dc.TextOut(ileft+132,itop-2,szBuffe);

	//������Դ
	dc.SelectObject(pOldFont);
	InfoFont.DeleteObject();

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

	//���������  ��С��  ��ԭ
	if(GetParent()->IsIconic())
		GetParent()->ShowWindow(SW_RESTORE);

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//��������
	m_lGameScore =0L;

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreView::SetGameTax(LONG lGameTax)
{	
	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID,  LONG lScore)
{
	//���ñ���
	if (wChairID<GAME_PLAYER)
	{
		m_lGameScore=lScore;
		Invalidate(FALSE);
	}

	return;
}

//��ʼʱ��
void CScoreView::SetStartTimes(bool bStart)
{
	//���ñ���
	//ASSERT(bStart);
	m_bStart = bStart;

	if(!bStart)
	{
		m_bTimes = TIMES;
		KillTimer(DLG_IDI_CLOSE);
	}

	return;
}

//��ʾʱ��
void CScoreView::SetShowTimes()
{
	SetTimer(DLG_IDI_CLOSE,1000,NULL);

	return;
}

void CScoreView::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�رյ���ʱ
	if(DLG_IDI_CLOSE==nIDEvent)
	{
		m_bTimes--;
		if(m_bTimes<=0)
		{
			m_bTimes = TIMES;
			KillTimer(DLG_IDI_CLOSE);
			this->ShowWindow(SW_HIDE);
			if(m_bStart)
			{
				m_bStart=false;
				AfxGetMainWnd()->PostMessage(IDM_START_TIMES,0,0);			
			}
		}
		else 
		{
			Invalidate(FALSE);
			UpdateWindow();
		}
	}

	return;
	//CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////