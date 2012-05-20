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
	m_cbManderin = 2;

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

	//�����ؼ�
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_COMBOX_MANDERIN);
	pComboBox->SetCurSel(m_cbManderin-1);

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
	//�����ؼ�
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_COMBOX_MANDERIN);
	m_cbManderin = pComboBox->GetCurSel()+1;
	pComboBox->SetCurSel(m_cbManderin-1);

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
