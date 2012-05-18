#include "Stdafx.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameOption, CSkinDialogEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameOption::CGameOption() : CSkinDialogEx(IDD_OPTION)
{
	m_bEnableSound=true;
	m_bAllowLookon=true;

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


	if(m_bAllowLookon == true)
	{
		((CButton *)GetDlgItem(IDC_ENABLE_LOOKON))->SetCheck(BST_CHECKED);
	}

	//���ÿؼ�
	if (m_bEnableSound==true) 
		((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->SetCheck(BST_CHECKED);


	return TRUE;
}

//ȷ����Ϣ
void CGameOption::OnOK()
{
	//��ȡ����
	m_bEnableSound=(((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->GetCheck()==BST_CHECKED);
	m_bAllowLookon =(((CButton *)GetDlgItem(IDC_ENABLE_LOOKON))->GetCheck()==BST_CHECKED);

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
