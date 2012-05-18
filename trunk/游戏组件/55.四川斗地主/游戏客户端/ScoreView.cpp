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
	m_lBaseScore=0L;
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
	m_ImageWinLose.SetLoadInfo(IDB_WIN_LOSE,AfxGetInstanceHandle());
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
	m_lBaseScore=0L;
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//���д���
	CenterWindow(GetParent());

	CImageHandle ImageHandle(&m_ImageBack);
	if (ImageHandle.IsResourceValid())
	{
		//������ʱͼ
		CSkinImage BufferImage;
		int nImageWidth=m_ImageBack.GetWidth();
		int nImageHeight=m_ImageBack.GetHeight();
		BufferImage.Create(nImageWidth,nImageHeight,32);
		ImageHandle->BitBlt(BufferImage.GetDC(),0,0,nImageWidth,nImageHeight,0,0);
		BufferImage.ReleaseDC();

		//��������
		CRgn ImageRgn;
		BufferImage.CreateImageRegion( ImageRgn, RGB( 255, 0, 255 ) );
		if ( (( HRGN )ImageRgn) !=NULL)
		{
			SetWindowPos(NULL,0,0,nImageWidth,nImageHeight,SWP_NOMOVE);
			SetWindowRgn(( HRGN )ImageRgn,TRUE);

			ImageRgn.DeleteObject();

			return true;
		}
	}


	return TRUE;
}

//�ػ�����
void CScoreView::OnPaint() {

	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//�滭��־
	CImageHandle ImageHandleWinLose( &m_ImageWinLose );

	//��ʾ����
	CRect rcDraw;
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		//�û�����
		rcDraw.left=26;
		rcDraw.right=117;
		rcDraw.top=i*28+87;
		rcDraw.bottom=rcDraw.top+28;
		dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		//�û�����
		rcDraw.left=117;
		rcDraw.right=206;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lGameScore[i]);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		//�滭��־
		int nWinLoseWidth = m_ImageWinLose.GetWidth() / 3;
		int nWinLoseHeight = m_ImageWinLose.GetHeight();
		int nDesPosX = ( 296 - 206 - nWinLoseWidth ) / 2 + 200;
		int nDesPoxY = ( 28 - nWinLoseHeight ) / 2 + 79 + i * 29;
		if ( m_lGameScore[ i ] == 0 ) {
			m_ImageWinLose.AlphaDrawImage( &dc, nDesPosX, nDesPoxY, nWinLoseWidth, nWinLoseHeight, 2 * nWinLoseWidth, 0, RGB( 255, 0, 255 ) );
		}
		else if ( 0 < m_lGameScore[ i ] ) {
			m_ImageWinLose.AlphaDrawImage( &dc, nDesPosX, nDesPoxY, nWinLoseWidth, nWinLoseHeight, 0, 0, RGB( 255, 0, 255 ) );
		}
		else {
			m_ImageWinLose.AlphaDrawImage( &dc, nDesPosX, nDesPoxY, nWinLoseWidth, nWinLoseHeight, nWinLoseWidth, 0, RGB( 255, 0, 255 ) );
		}
	}

	//��ʾ˰��
	rcDraw.left=26;
	rcDraw.right=117;
	rcDraw.top=i*28+88;
	rcDraw.bottom=rcDraw.top+28;
	LPCTSTR pszTax=TEXT("����˰");
//	dc.DrawText(pszTax,lstrlen(pszTax),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	//��ʾ˰��
	rcDraw.left=117;
	rcDraw.right=206;
	rcDraw.top=i*28+88;
	rcDraw.bottom=rcDraw.top+28;
	_snprintf(szBuffer,sizeof(szBuffer),"%ld",m_lGameTax);
//	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);	

	//��ʾ��Ԫ
	rcDraw.left=117;
	rcDraw.right=206;
	rcDraw.top=i*28+88;
	rcDraw.bottom=rcDraw.top+28;
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("��Ԫ����:%ld"),m_lBaseScore);
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
	m_lBaseScore = 0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//�滭��ͼ
	if (m_hWnd!=NULL) Invalidate(NULL);

	return;
}

//����˰��
void CScoreView::SetGameTax(LONG lGameTax,LONG lBaseScore)
{
	//���ñ���
	m_lBaseScore = lBaseScore;
	m_lGameTax=lGameTax;

	if (m_hWnd!=NULL) Invalidate(FALSE);
	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore)
{
	//���ñ���
	if (wChairID<CountArray(m_lGameScore))
	{
		m_lGameScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
