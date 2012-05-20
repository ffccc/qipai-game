#include "Stdafx.h"
#include "Resource.h"
#include "ChessCountInfo.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChessCountInfo, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CChessCountInfo::CChessCountInfo() : CDialog(IDD_CHESS_COUNT)
{
	//���ñ���
	m_fBlackCount=0;
	m_fWhiteCount=0;
	m_fCommomCount=0;

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_CHESS_COUNT_INFO,AfxGetInstanceHandle());

	return;
}

//��������
CChessCountInfo::~CChessCountInfo()
{
}

//�ؼ���
void CChessCountInfo::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CChessCountInfo::OnInitDialog()
{
	__super::OnInitDialog();

	//������ˢ
	m_Brush.CreateSolidBrush(RGB(245,238,221));

	//���ñ���
	SetWindowText(TEXT("��Ŀ�����"));

	//�ƶ�����
	CImageHandle BackImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//���ð�ť
	m_btOk.SetButtonImage(IDB_COUNT_BT_ACCEPT,AfxGetInstanceHandle(),false);
	m_btCancel.SetButtonImage(IDB_COUNT_BT_CONTINUE,AfxGetInstanceHandle(),false);

	return TRUE;
}

//ȷ����Ϣ
void CChessCountInfo::OnOK()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_CHESS_COUNT_RESPONSION,1,1);

	//�رմ���
	DestroyWindow();

	return;
}

//ȡ����Ϣ
void CChessCountInfo::OnCancel()
{
	//������Ϣ
	AfxGetMainWnd()->PostMessage(IDM_CHESS_COUNT_RESPONSION,0,0);

	//�رմ���
	DestroyWindow();

	return;
}

//������Ϣ
void CChessCountInfo::SetChessCountInfo(float fBlackCount, float fWhiteCount, float fCommomCount)
{
	//���ñ���
	m_fBlackCount=fBlackCount;
	m_fWhiteCount=fWhiteCount;
	m_fCommomCount=fCommomCount;

	//���½���
	if (GetSafeHwnd()!=NULL) Invalidate(FALSE);

	return;
}

//�ػ�����
void CChessCountInfo::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CImageHandle BackImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(10,10,10));
	dc.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭��Ϣ
	TCHAR szBuffer[64];
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("%0.1f"),m_fBlackCount);
	dc.TextOut(133,57,szBuffer,lstrlen(szBuffer));
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("%0.1f"),m_fWhiteCount);
	dc.TextOut(133,83,szBuffer,lstrlen(szBuffer));
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("%0.1f"),m_fCommomCount);
	dc.TextOut(133,110,szBuffer,lstrlen(szBuffer));

	return;
}

//�滭����
BOOL CChessCountInfo::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//������Ϣ
void CChessCountInfo::OnDestroy()
{
	//������Դ
	m_Brush.DeleteObject();

	__super::OnDestroy();
}

//�����Ϣ
void CChessCountInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//�ؼ���ɫ
HBRUSH CChessCountInfo::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(10,10,10));
			pDC->SetBkColor(RGB(245,238,221));
			return m_Brush;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////
