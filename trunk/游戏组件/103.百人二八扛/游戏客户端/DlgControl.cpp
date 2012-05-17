// m_DlgControl.cpp : implementation file
//

#include "stdafx.h"
#include "DlgControl.h"
#include ".\dlgcontrol.h"


// CDlgControl dialog

IMPLEMENT_DYNAMIC(CDlgControl, CDialog)
CDlgControl::CDlgControl(CWnd* pParent /*=NULL*/)
		: CDialog(CDlgControl::IDD, pParent)
		, m_bControlBanker(FALSE), m_bControlArea(FALSE)
{
	m_nRadioControlArea = 0;
	m_nRadioControlBanker = 0;
	m_bUserWin = 0;
	m_bUserLost = 1;
	/*m_bControlBanker=false;
	m_bControlArea=false;*/
}

CDlgControl::~CDlgControl()
{
}
void CDlgControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_CONTROL, m_nRadioControlArea);
	DDX_Radio(pDX, IDC_BANKER_LOST, m_nRadioControlBanker);
	DDX_Check(pDX, IDC_CONTROL_BANKER, m_bControlBanker);
	DDX_Check(pDX, IDC_CONTROL_AREA, m_bControlArea);





}


BEGIN_MESSAGE_MAP(CDlgControl, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgControl message handlers

BOOL CDlgControl::OnInitDialog()
{
	CDialog::OnInitDialog();



	//���±���
	UpdateData(FALSE);

	return TRUE;
}

void CDlgControl::OnOK()
{
	//���±���
	UpdateData(TRUE);

	CDialog::OnOK();
}

void CDlgControl::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
