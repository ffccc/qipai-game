#include "StdAfx.h"
#include "ScoreView.h"
#include ".\scoreview.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView()
{
	//��������
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));
	ZeroMemory(m_lDrawScore,sizeof(m_lDrawScore));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_SCORE_BACK,AfxGetInstanceHandle());

	return;
}

//��������
CScoreView::~CScoreView()
{
}

//��ʼ������
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetForegroundWindow();
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	CButton* cbBitmapBtn=(CButton*)GetDlgItem(IDC_BUTTON1);
	ASSERT(cbBitmapBtn);
	if(cbBitmapBtn)
	{
	  VERIFY(m_BTbitmap.LoadBitmap(IDB_Cancel));
	  HBITMAP hbmp=(HBITMAP)m_BTbitmap.GetSafeHandle();
	  cbBitmapBtn->SetBitmap(hbmp);
	}

	return FALSE;
}

//�滭����
BOOL CScoreView::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
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
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//��Ϸ�ɼ�
	TCHAR szBuffer[32]=TEXT("");
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("�һ�÷֣�%ld"),(m_lDrawScore[0]==640?m_lDrawScore[0]-200L:m_lDrawScore[0]));
	dc.TextOut(100,40,szBuffer,lstrlen(szBuffer));
	_snprintf(szBuffer,CountArray(szBuffer),TEXT("�Ի�÷֣�%ld"),(m_lDrawScore[1]==640?m_lDrawScore[1]-200L:m_lDrawScore[1]));
	dc.TextOut(100,75,szBuffer,lstrlen(szBuffer));

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
	m_lDrawScore[0]=ScoreViewInfo.lDrawScore[0];
	m_lDrawScore[1]=ScoreViewInfo.lDrawScore[1];

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

void CScoreView::OnBnClickedButton1()
{
	__super::OnOK();
}
