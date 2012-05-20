#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
	m_ImageWinLose.SetLoadInfo(IDB_WINLOSE,AfxGetInstanceHandle());
}

//��������
CScoreView::~CScoreView(void)
{
}

//������Ϣ
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
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
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//_snprintf(m_szUserName[0],sizeof(m_szUserName[0]),TEXT("fghngnjhgjh"));
	//_snprintf(m_szUserName[1],sizeof(m_szUserName[0]),TEXT("fghngnjhgjh"));
	//_snprintf(m_szUserName[2],sizeof(m_szUserName[0]),TEXT("fghnfgfthyghyytugnjhgjh"));
	//_snprintf(m_szUserName[3],sizeof(m_szUserName[0]),TEXT("fghngnjhgjh"));


	//��ʾ����
	CRect rcDraw;
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		//�û�����
		rcDraw.left=30;
		rcDraw.right=114;
		rcDraw.top=i*30+85;
		rcDraw.bottom=rcDraw.top+12;
		dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		//�û�����
		rcDraw.left=120;
		rcDraw.right=190;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lScore[i]);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		CImageHandle HandleWinLose(&m_ImageWinLose);
		INT nWinLoseWidth = m_ImageWinLose.GetWidth()/3;
		INT nWinLostHeight = m_ImageWinLose.GetHeight();
		//ʤ����־
		BYTE index=0;
		if(m_lScore[i]>0) index=0;
		if(m_lScore[i]==0) index=2;
		if(m_lScore[i]<0) index=1;
		m_ImageWinLose.AlphaDrawImage(&dc,242-nWinLoseWidth/3,i*29+90-nWinLostHeight/3,nWinLoseWidth,nWinLostHeight,
				index*nWinLoseWidth,0,RGB(255,0,255));
	}

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
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreView::SetGameTax(LONG lGameTax)
{
	//���ñ���
	//if (m_lGameTax!=lGameTax)
	//{
	//	m_lGameTax=lGameTax;
	//	Invalidate(FALSE);
	//}

	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore)
{
	//���ñ���
	if (wChairID<CountArray(m_lScore))
	{
		m_lScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}

	return;
}
void CScoreView::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	// TODO: Add your message handler code here
	Invalidate(FALSE);
	UpdateWindow();

}
//////////////////////////////////////////////////////////////////////////
