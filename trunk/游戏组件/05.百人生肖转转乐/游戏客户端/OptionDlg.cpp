// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionDlg.h"


// COptionDlg dialog

IMPLEMENT_DYNAMIC(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	m_bAllowGameSound = true;
}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	ON_BN_CLICKED(IDC_ALLOW_SOUND, &COptionDlg::OnBnClickedAllowSound)
END_MESSAGE_MAP()


// COptionDlg message handlers

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CButton*)GetDlgItem(IDC_ALLOW_SOUND))->SetCheck((int)m_bAllowGameSound);
	
	return TRUE;
}

void COptionDlg::OnBnClickedAllowSound()
{
	if (((CButton*)GetDlgItem(IDC_ALLOW_SOUND))->GetCheck())
	{
		m_bAllowGameSound = true;
	}
	else
	{
		m_bAllowGameSound = false;
	}
}
