// HintInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HintInfo.h"


// CHintInfo �Ի���

IMPLEMENT_DYNAMIC(CHintInfo, CDialog)
CHintInfo::CHintInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CHintInfo::IDD, pParent)
{
}

CHintInfo::~CHintInfo()
{
}

void CHintInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CHintInfo::SetShowInfo(CString &str)
{
	CButton *p= (CButton *)GetDlgItem(IDC_TEXT_INFO);
	p->SetWindowText(str);
	ShowWindow(SW_SHOW);
}


BEGIN_MESSAGE_MAP(CHintInfo, CDialog)
END_MESSAGE_MAP()


// CHintInfo ��Ϣ�������
