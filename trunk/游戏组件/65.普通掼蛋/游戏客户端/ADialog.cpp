// ADialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ADialog.h"
#include ".\adialog.h"


// CADialog �Ի���

IMPLEMENT_DYNAMIC(CADialog, CSkinDialogEx)
CADialog::CADialog()
	: CSkinDialogEx(IDD_ANSWER)
	, m_bCause(_T(""))
{
}

CADialog::~CADialog()
{
}

void CADialog::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_bCause);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}


BEGIN_MESSAGE_MAP(CADialog, CSkinDialogEx)
END_MESSAGE_MAP()


// CADialog ��Ϣ�������

BOOL CADialog::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    SetWindowText(m_title);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
