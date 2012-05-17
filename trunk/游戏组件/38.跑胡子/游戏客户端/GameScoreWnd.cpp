#include "StdAfx.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScoreWnd::CGameScoreWnd()
{
	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
}

//��������
CGameScoreWnd::~CGameScoreWnd()
{
}

//������Ϣ
int CGameScoreWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return TRUE;
}

//�ػ�����
void CGameScoreWnd::OnPaint() 
{
	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//��ʾ����
	CRect rcDraw;
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		//�û�����
		rcDraw.left=30;
		rcDraw.right=114;
		rcDraw.top=i*22+51;
		rcDraw.bottom=rcDraw.top+12;
		dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		//�û�����
		rcDraw.left=120;
		rcDraw.right=190;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lGameScore[i]);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	}

	//��ʾ˰��
	rcDraw.left=30;
	rcDraw.right=114;
	rcDraw.top=i*22+51;
	rcDraw.bottom=rcDraw.top+12;
	LPCTSTR pszTax=TEXT("����˰");
	dc.DrawText(pszTax,lstrlen(pszTax),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	//��ʾ˰��
	rcDraw.left=120;
	rcDraw.right=190;
	rcDraw.top=i*22+51;
	rcDraw.bottom=rcDraw.top+12;
	dc.DrawText(TEXT("5%"),lstrlen(TEXT("5%")),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

	return;
}

//�滭����
BOOL CGameScoreWnd::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//�����Ϣ
void CGameScoreWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//���û���
void CGameScoreWnd::ResetScore()
{
	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//�滭��ͼ
	if (m_hWnd!=NULL) Invalidate(NULL);
	
	return;
}

//����˰��
void CGameScoreWnd::SetGameTax(LONG lGameTax)
{
	//����˰��
	if (m_lGameTax!=lGameTax)
	{
		//���ñ���
		m_lGameTax=lGameTax;

		//�滭��ͼ
		if (m_hWnd!=NULL) Invalidate(NULL);
	}

	return;
}

//���û���
void CGameScoreWnd::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore)
{
	//���û���
	if (wChairID<CountArray(m_lGameScore))
	{
		//���ñ���
		m_lGameScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));

		//�滭��ͼ
		if (m_hWnd!=NULL) Invalidate(NULL);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
