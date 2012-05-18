// ReDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReDialog.h"
#include ".\redialog.h"


// CReDialog �Ի���

IMPLEMENT_DYNAMIC(CReDialog, CSkinDialogEx)
CReDialog::CReDialog()
	: CSkinDialogEx(IDD_REQUEST)
	, m_bCause(_T(""))
{
}

CReDialog::~CReDialog()
{
}

void CReDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REQUEST, m_bCause);
	DDV_MaxChars(pDX, m_bCause, 50);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}


BEGIN_MESSAGE_MAP(CReDialog, CSkinDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CReDialog ��Ϣ�������

void CReDialog::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnCancel();
}

void CReDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::EndDialog(IDQUIT);
}

BOOL CReDialog::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText("�����˳�");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
