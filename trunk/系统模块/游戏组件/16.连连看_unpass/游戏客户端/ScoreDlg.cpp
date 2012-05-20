// ScoreDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ScoreDlg.h"
#include ".\scoredlg.h"


// CScoreDlg �Ի���

IMPLEMENT_DYNAMIC(CScoreDlg, CDialog)
CScoreDlg::CScoreDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScoreDlg::IDD, pParent)
{

	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	memset(m_lPer,0,sizeof(m_lPer));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_END,AfxGetInstanceHandle());
}

CScoreDlg::~CScoreDlg()
{
}

void CScoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CScoreDlg::OnInitDialog()
{
	return TRUE;

	m_isExer = false;

}

BEGIN_MESSAGE_MAP(CScoreDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CScoreDlg ��Ϣ�������

int CScoreDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	memset(m_lPer,0,sizeof(m_lPer));

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

void CScoreDlg::OnPaint()
{
	CPaintDC dc(this); //���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));
	dc.SelectObject(&CSkinAttribute::m_DefaultFont);

	//�滭����
	if(!m_isExer)
	{
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);
	}
	else
	{	
	CImageHandle ImageHandle(&m_ImageBackExer);
	m_ImageBackExer.BitBlt(dc,0,0);
	}

	//��ʾ����
	WORD wStation=0;
	TCHAR szBuffer[64]=TEXT("");
	TCHAR szBuffer2[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			dc.TextOut(50,(int)(wStation*22+78),m_szUserName[i]);
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lScore[i]);
			dc.TextOut(160,(int)(wStation*22+78),szBuffer,lstrlen(szBuffer));
			wStation++;
		}
	}
	return;
}

//���û���
void CScoreDlg::ResetScore()
{
	//��������
	m_lTax=0L;
	memset(m_lScore,0,sizeof(m_lScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	memset(m_lPer,0,sizeof(m_lPer));

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreDlg::SetTax(LONG lTax)
{
	if (m_lTax!=lTax)
	{
		m_lTax=lTax;
		Invalidate(NULL);
	}
	return;
}

//���û���
void CScoreDlg::SetGameScore(WORD wChairID, LPCTSTR pszUserName, float per, int lScore)
{
	if (wChairID<CountArray(m_lScore))
	{
		m_lScore[wChairID]=lScore;
		m_lPer[wChairID] = per;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}
	return;
}

void CScoreDlg::SetCurrMode(bool Exer)
{
	m_isExer = Exer;
}

void CScoreDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CScoreDlg::OnEraseBkgnd(CDC* pDC)
{
	Invalidate(FALSE);
	UpdateWindow();

	return true;
}
