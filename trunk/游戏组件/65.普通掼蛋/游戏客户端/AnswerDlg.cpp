// AnswerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AnswerDlg.h"
#include ".\answerdlg.h"


// CAnswerDlg �Ի���

IMPLEMENT_DYNAMIC(CAnswerDlg, CSkinDialogEx)
CAnswerDlg::CAnswerDlg()
	: CSkinDialogEx(IDD_DIALOG)
	, m_answer(_T(""))
{
}

CAnswerDlg::~CAnswerDlg()
{
}

void CAnswerDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_answer);
	DDX_Control(pDX, IDOK, m_btOK);
}


BEGIN_MESSAGE_MAP(CAnswerDlg, CSkinDialogEx)
END_MESSAGE_MAP()


// CAnswerDlg ��Ϣ�������

BOOL CAnswerDlg::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText("�Է��Ļظ�");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
