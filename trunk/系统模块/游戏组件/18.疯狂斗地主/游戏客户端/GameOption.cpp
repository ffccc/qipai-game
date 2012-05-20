#include "Stdafx.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameOption, CSkinDialogEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameOption::CGameOption() : CSkinDialogEx(IDD_OPTION)
{
	m_bDeasilOrder=true;
	m_bEnableSound=true;
	m_bAllowLookOn=false ;
	m_dwCardHSpace=DEFAULT_PELS;

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

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	if (m_bDeasilOrder==true) ((CButton *)GetDlgItem(IDC_DEASIL_ORDER))->SetCheck(BST_CHECKED);
	if (m_bEnableSound==true) ((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->SetCheck(BST_CHECKED);
	if (m_bAllowLookOn==true) ((CButton *)GetDlgItem(IDC_ALLOW_LOOKON))->SetCheck(BST_CHECKED);

	//�˿�����
	int nItem=0;
	TCHAR szBuffer[32]=TEXT("");
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_CARD_SPACE);
	for (DWORD i=LESS_PELS;i<=MAX_PELS;i++)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%2ld"),i);
		nItem=pComboBox->InsertString((i-LESS_PELS),szBuffer);
		pComboBox->SetItemData(nItem,i);
		if (m_dwCardHSpace==i) pComboBox->SetCurSel(nItem);
	}

	return TRUE;
}

//ȷ����Ϣ
void CGameOption::OnOK()
{
	//��ȡ����
	m_bDeasilOrder=(((CButton *)GetDlgItem(IDC_DEASIL_ORDER))->GetCheck()==BST_CHECKED);
	m_bEnableSound=(((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->GetCheck()==BST_CHECKED);
	m_bAllowLookOn = (((CButton *)GetDlgItem(IDC_ALLOW_LOOKON))->GetCheck()==BST_CHECKED);

	//�˿�����
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_CARD_SPACE);
	m_dwCardHSpace=(DWORD)pComboBox->GetItemData(pComboBox->GetCurSel());

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
