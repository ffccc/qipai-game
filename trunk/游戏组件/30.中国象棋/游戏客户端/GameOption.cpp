#include "Stdafx.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameOption, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameOption::CGameOption() : CDialog(IDD_OPTION)
{
	//���ñ���
	m_bEnableSound=true;
	m_bAutoSaveManual=false;

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_GAME_OPTION,AfxGetInstanceHandle());

	return;
}

//��������
CGameOption::~CGameOption()
{
}

//�ؼ���
void CGameOption::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CGameOption::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ϸ����"));

	//������ˢ
	m_Brush.CreateSolidBrush(RGB(245,238,221));

	//�ƶ�����
	CImageHandle BackImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//���ð�ť
	m_btOK.SetButtonImage(IDB_OPTION_BT_OK,AfxGetInstanceHandle(),false);
	m_btCancel.SetButtonImage(IDB_OPTION_BT_CANCEL,AfxGetInstanceHandle(),false);

	//���ÿؼ�
	if (m_bEnableSound==true) ((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->SetCheck(BST_CHECKED);
	((CButton *)GetDlgItem((m_bAutoSaveManual==true)?IDC_AUTO_SAVE_MANUAL:IDC_NO_AUTO_SAVE_MANUAL))->SetCheck(BST_CHECKED);

	return TRUE;
}

//ȷ����Ϣ
void CGameOption::OnOK()
{
	//��ȡ����
	m_bEnableSound=(((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->GetCheck()==BST_CHECKED);
	m_bAutoSaveManual=(((CButton *)GetDlgItem(IDC_AUTO_SAVE_MANUAL))->GetCheck()==BST_CHECKED);

	__super::OnOK();
}

//�ػ�����
void CGameOption::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CImageHandle BackImageHandle(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	return;
}

//�滭����
BOOL CGameOption::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//������Ϣ
void CGameOption::OnDestroy()
{
	//������Դ
	m_Brush.DeleteObject();

	__super::OnDestroy();
}

//�����Ϣ
void CGameOption::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//�ؼ���ɫ
HBRUSH CGameOption::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
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
