// SaveChessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SaveChessDlg.h"


CSaveChessDlg::CSaveChessDlg(CWnd * pParent)
: CDialog(CSaveChessDlg::IDD,pParent)
{
}

CSaveChessDlg::~CSaveChessDlg()
{
}

void CSaveChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_Ok);
}


BEGIN_MESSAGE_MAP(CSaveChessDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSaveChessDlg ��Ϣ�������
//�������
void CSaveChessDlg::OnBnClickedOk()
{
	KillTimer(1);
	OnOK();
}

//�����沼��
void CSaveChessDlg::OnBnClickedCancel()
{
	KillTimer(1);
	OnCancel();
}

void CSaveChessDlg::OnTimer(UINT nIDEvent)
{
	if(m_iClose++==20)
	{
		KillTimer(1);
		OnCancel();
	}
	CString str;
	str.Format("���������%d",20-m_iClose);
	SetWindowText(str);
	CDialog::OnTimer(nIDEvent);
}

BOOL CSaveChessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_iClose=0;
	SetTimer(1,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
}
