#include "StdAfx.h"
#include "Resource.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_CLOSE_TIME				100									//�˳�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CONTINUE, OnContinue)
	ON_BN_CLICKED(IDC_QUIT_GAME, OnQuitGame)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView() : CDialog(IDD_GAME_SCORE)
{
	//״̬����
	m_uLeaveTime=15;

	//��������
	ZeroMemory(m_lRevenue,sizeof(m_lRevenue));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_SCORE,AfxGetInstanceHandle());
}

//��������
CScoreView::~CScoreView()
{
}

//�ؼ���
VOID CScoreView::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTINUE, m_btContinue);
	DDX_Control(pDX, IDC_QUIT_GAME, m_btQuitGame);
}

//��ʼ������
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//״̬����
	m_uLeaveTime=15;

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//���ð�ť
	m_btContinue.SetTextColor(RGB(255,255,255));
	m_btQuitGame.SetTextColor(RGB(255,255,255));
	m_btContinue.SetButtonImage(IDB_BT_GAME_SCORE,AfxGetInstanceHandle(),false);
	m_btQuitGame.SetButtonImage(IDB_BT_GAME_SCORE,AfxGetInstanceHandle(),false);

	//���д���
	CenterWindow(GetParent());

	//�ƶ�����
	CImageHandle HandleBack(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//�ƶ���ť
	CRect rcButton;
	m_btContinue.GetWindowRect(&rcButton);
	m_btQuitGame.SetWindowPos(NULL,m_ImageBack.GetWidth()/2+15,m_ImageBack.GetHeight()-rcButton.Height()-8,
		0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btContinue.SetWindowPos(NULL,m_ImageBack.GetWidth()/2-rcButton.Width()-15,m_ImageBack.GetHeight()-rcButton.Height()-8,
		0,0,SWP_NOSIZE|SWP_NOZORDER);

	return TRUE;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	dc.FillSolidRect(&rcClient,RGB(255,255,255));

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(10,10,10));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle ImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//�滭����
	LPCTSTR pszAccount=TEXT("�û���"),pszScore=TEXT("�ɼ�");
	dc.TextOut(30,21,pszAccount,lstrlen(pszAccount));
	dc.TextOut(140,21,pszScore,lstrlen(pszScore));

	//��ʾ����
	WORD wStation=0;
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			dc.TextOut(30,(int)(wStation*20+43),m_szUserName[i]);
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("%ld"),m_lGameScore[i]);
			dc.TextOut(140,(int)(wStation*20+43),szBuffer,lstrlen(szBuffer));
			wStation++;
		}
	}

	return;
}

//������ť
void CScoreView::OnContinue()
{
	//���ش���
	DestroyWindow();

	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_CONTINUE,0,0);

	return;
}

//�뿪��ť
void CScoreView::OnQuitGame()
{
	//���ش���
	DestroyWindow();

	//������Ϣ
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	return;
}

//ʱ����Ϣ
void CScoreView::OnTimer(UINT nIDEvent)
{
	//ʱ����Ϣ
	if (nIDEvent==IDI_CLOSE_TIME)
	{
		if (m_uLeaveTime>0)
		{
			//����ʱ��
			TCHAR szBuffer[32]=TEXT("");
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("���� %d"),m_uLeaveTime--);

			//�����ַ�
			m_btContinue.SetWindowText(szBuffer);
		}
		else OnQuitGame();
	}

	__super::OnTimer(nIDEvent);
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

	//ģ�����
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//��������
	ZeroMemory(m_lRevenue,sizeof(m_lRevenue));
	ZeroMemory(m_lGameScore,sizeof(m_lGameScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//�رմ���
	if (m_hWnd!=NULL) DestroyWindow();
	
	return;
}

//��ʾ�ɼ�
void CScoreView::ShowScore()
{
	//����ʱ��
	SetTimer(IDI_CLOSE_TIME,1000,NULL);

	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONG lGameScore, LONG lRevenue)
{
	if (wChairID<CountArray(m_lGameScore))
	{
		//���ñ���
		m_lRevenue[wChairID]=lRevenue;
		m_lGameScore[wChairID]=lGameScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));

		//���½���
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
