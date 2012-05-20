#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView()
{
	//��������
	ZeroMemory(m_lGameTax,sizeof(m_lGameTax));
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_SCORE_VIEW,AfxGetInstanceHandle());
	m_ImageWinLose.SetLoadInfo(IDB_WIN_LOSE,AfxGetInstanceHandle());

	return;
}

//��������
CScoreView::~CScoreView()
{
}

//������Ϣ
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	ZeroMemory(m_lGameTax,sizeof(m_lGameTax));
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle HandleBack(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//��������
	CRect rt;
	GetClientRect(&rt);
	CRgn WndRgn;
	WndRgn.CreateRoundRectRgn(rt.left,rt.top,rt.right,rt.bottom,10,10);
	SetWindowRgn(WndRgn,TRUE);
	WndRgn.DeleteObject();

	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//��ʾ����
	WORD wStation=0;
	TCHAR szBuffer[64]=TEXT("");
	CImageHandle HandleWinLose(&m_ImageWinLose);
	INT nWinLoseWidth = m_ImageWinLose.GetWidth()/3;
	INT nWinLostHeight = m_ImageWinLose.GetHeight();
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			//�������
			CRect rcName(28,wStation*29+79,117,(wStation+1)*29+79);
			dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//����ɼ�
			CRect rcScore(117,wStation*29+79,182,(wStation+1)*29+79);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lScore[i]);
			dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//ʤ����־
			BYTE byIndex = 0;
			if( m_lScore[i] == 0 ) byIndex = 2;
			else if( m_lScore[i] < 0 ) byIndex = 1;
			m_ImageWinLose.AlphaDrawImage(&dc,182+(253-182)/2-nWinLoseWidth/2,wStation*29+79+29/2-nWinLostHeight/2,nWinLoseWidth,nWinLostHeight,
				byIndex*nWinLoseWidth,0,RGB(255,0,255));

			//�������
			CRect rcService(253,wStation*29+79,321,(wStation+1)*29+79);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lGameTax[i]);
			dc.DrawText(szBuffer,lstrlen(szBuffer),&rcService,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//���ñ���
			wStation++;
		}
	}

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//��������
	ZeroMemory(m_lGameTax,sizeof(m_lGameTax));
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//�滭��ͼ
	if (m_hWnd!=NULL) Invalidate(FALSE);
	
	return;
}

//����˰��
void CScoreView::SetGameTax(WORD wChairID,LONG lGameTax)
{
	if( wChairID == INVALID_CHAIR )
	{
		ZeroMemory(m_lGameTax,sizeof(m_lGameTax));
		//�ػ�����
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}
	ASSERT( wChairID >= 0 && wChairID < GAME_PLAYER );
	if ( m_lGameTax[wChairID] != lGameTax )
	{
		//���ñ���
		m_lGameTax[wChairID] = lGameTax;

		//�ػ�����
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}
	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore)
{
	if (wChairID<CountArray(m_lScore))
	{
		//���ñ���
		m_lScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));

		//�ػ�����
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
