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
	//������Ϣ
	m_wConcealTime=1;

	//��������
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_SCORE_BACK,AfxGetInstanceHandle());

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

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
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

	//������Դ
	CImageHandle HandleBack(&m_ImageBack);
	
	//�滭����
	m_ImageBack.BitBlt(dc,0,0);


	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(250,250,250));
	dc.SelectObject(&CSkinAttribute::m_DefaultFont);

	//��ʾ��Ϣ
	TCHAR szBuffer[128]=TEXT("");
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("��Ϸ���� %d ��"),m_wConcealTime);

	//�滭��ʾ
	CRect rcString(5,129,287,147);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rcString,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//��ʾ����
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			//�������
			CRect rcName(44,i*19+178,146,i*19+195);
			dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//����ɼ�
			CRect rcScore(148,i*19+178,249,i*19+195);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lScore[i]);
			dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
	}

	return;
}

//������Ϣ
void CScoreView::SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo)
{
	//���ñ���
	m_wConcealTime=ScoreViewInfo.wConcealTime;
	
	//ˢ�½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lScore)
{
	if (wChairID<CountArray(m_lScore))
	{
		m_lScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
