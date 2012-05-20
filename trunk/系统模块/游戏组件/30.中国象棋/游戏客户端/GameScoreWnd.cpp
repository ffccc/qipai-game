#include "StdAfx.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreWnd, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScoreWnd::CGameScoreWnd() : CDialog(IDD_OPTION)
{
	//���ñ���
	memset(&m_GameScoreInfo,0,sizeof(m_GameScoreInfo));
	m_ImageBack.SetLoadInfo(IDB_GAME_SCORE,AfxGetInstanceHandle());

	return;
}

//��������
CGameScoreWnd::~CGameScoreWnd()
{
}

//�ؼ���
void CGameScoreWnd::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLOSE, m_btClose);
}

//��ʼ������
BOOL CGameScoreWnd::OnInitDialog()
{
	__super::OnInitDialog();

	//�ƶ�����
	CImageHandle BackImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOREPOSITION);

	//���ð�ť
	m_btClose.SetButtonImage(IDB_BT_CLOSE,AfxGetInstanceHandle(),false);
	m_btClose.SetWindowPos(NULL,m_ImageBack.GetWidth()-25,10,0,0,SWP_NOSIZE|SWP_NOZORDER);

	return TRUE;
}

//�رհ�ť
void CGameScoreWnd::OnClose()
{
	ShowWindow(SW_HIDE);
	return;
}

//�ػ�����
void CGameScoreWnd::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CImageHandle BackImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//����
	dc.SetTextColor(RGB(10,10,10));
	dc.TextOut(105,9,TEXT("��Ϸ�ɼ���"),10);

	//��������
	CRect rcDraw;
	TCHAR szBuffer[64];
	dc.SetTextColor(RGB(200,200,200));

	//�û��ɼ�
	for (int i=0;i<GAME_PLAYER;i++)
	{
		//�û�����
		rcDraw.left=40;
		rcDraw.top=79+i*33;
		rcDraw.bottom=rcDraw.top+12;
		rcDraw.right=m_ImageBack.GetWidth()/2-5;
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_GameScoreInfo.lScore[i]);
		dc.DrawText(m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		//�û�����
		rcDraw.top=79+i*33;
		rcDraw.bottom=rcDraw.top+12;
		rcDraw.left=m_ImageBack.GetWidth()/2+5;
		rcDraw.right=m_ImageBack.GetWidth()-45;
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}

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
void CGameScoreWnd::SetScoreInfo(tagGameScoreInfo & GameScoreInfo)
{
	//���ñ���
	m_GameScoreInfo=GameScoreInfo;

	//��ʾ����
	Invalidate(NULL);
	ShowWindow(SW_SHOW);

	return;
}

//////////////////////////////////////////////////////////////////////////
