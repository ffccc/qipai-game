// DlgWinCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgWinCtrl.h"


// CDlgWinCtrl 对话框

IMPLEMENT_DYNAMIC(CDlgWinCtrl, CDialog)


CDlgWinCtrl::CDlgWinCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWinCtrl::IDD, pParent)
{
	memset(&m_Info, 0, sizeof(m_Info));
}

CDlgWinCtrl::~CDlgWinCtrl()
{
}

void CDlgWinCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Info.dwID[0]);
	DDX_Text(pDX, IDC_EDIT2, m_Info.dwID[1]);
	DDX_Text(pDX, IDC_EDIT3, m_Info.dwID[2]);
	DDX_Text(pDX, IDC_EDIT4, m_Info.dwID[3]);
	DDX_Text(pDX, IDC_EDIT5, m_Info.dwID[4]);
	DDX_Text(pDX, IDC_EDIT6, m_Info.dwID[5]);
	DDX_Text(pDX, IDC_EDIT7, m_Info.dwID[6]);
	DDX_Text(pDX, IDC_EDIT8, m_Info.dwID[7]);
	DDX_Text(pDX, IDC_EDIT9, m_Info.dwID[8]);
	DDX_Text(pDX, IDC_EDIT10, m_Info.dwID[9]);

	DDX_Text(pDX, IDC_EDIT11, m_Info.dwRate[0]);
	DDX_Text(pDX, IDC_EDIT12, m_Info.dwRate[1]);
	DDX_Text(pDX, IDC_EDIT13, m_Info.dwRate[2]);
	DDX_Text(pDX, IDC_EDIT14, m_Info.dwRate[3]);
	DDX_Text(pDX, IDC_EDIT15, m_Info.dwRate[4]);
	DDX_Text(pDX, IDC_EDIT16, m_Info.dwRate[5]);
	DDX_Text(pDX, IDC_EDIT17, m_Info.dwRate[6]);
	DDX_Text(pDX, IDC_EDIT18, m_Info.dwRate[7]);
	DDX_Text(pDX, IDC_EDIT19, m_Info.dwRate[8]);
	DDX_Text(pDX, IDC_EDIT20, m_Info.dwRate[9]);
}

BEGIN_MESSAGE_MAP(CDlgWinCtrl, CDialog)
END_MESSAGE_MAP()


bool CDlgWinCtrl::SetWinRate(const void *pInfo, WORD size)
{
	ASSERT(pInfo != NULL && size == sizeof(CMD_S_Win_Rate_Info));
	if (pInfo == NULL || size != sizeof(CMD_S_Win_Rate_Info)) return false;

	m_Info = *(CMD_S_Win_Rate_Info*)pInfo;

	HWND h = GetSafeHwnd();
	if (h > 0)
		UpdateData(FALSE);

	return true;
}