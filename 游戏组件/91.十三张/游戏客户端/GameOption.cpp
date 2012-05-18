#include "Stdafx.h"
#include "GameOption.h"
#include ".\gameoption.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameOption, CSkinDialogEx)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameOption::CGameOption() : CSkinDialogEx(IDD_OPTION)
{
	m_bDeasilOrder=true;
	m_bEnableSound=true;
	m_dwCardHSpace=DEFAULT_PELS;
	m_bCompareTime=1 ;

	return;
}

//析构函数
CGameOption::~CGameOption()
{
}

//控件绑定
void CGameOption::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//初始化函数
BOOL CGameOption::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("游戏配置"));

	//调整参数
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//设置控件
	if (m_bEnableSound==true) ((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->SetCheck(BST_CHECKED);

	//扑克象素
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
	pComboBox=(CComboBox *)GetDlgItem(IDC_COMPARETIME);
	for (DWORD i=1;i<=MAX_TIME;i++)
	{
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%2ld"),i);
		nItem=pComboBox->InsertString(i-1,szBuffer);
		pComboBox->SetItemData(nItem,i);
		if (m_bCompareTime==i) pComboBox->SetCurSel(nItem);
	}

	UpdateData(FALSE) ;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CGameOption::OnBnClickedOk()
{
	//获取变量
	m_bEnableSound=(((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->GetCheck()==BST_CHECKED);

	//扑克象素
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_CARD_SPACE);
	m_dwCardHSpace=(DWORD)pComboBox->GetItemData(pComboBox->GetCurSel());

	//比较时间
	pComboBox=(CComboBox *)GetDlgItem(IDC_COMPARETIME);
	m_bCompareTime=(BYTE)pComboBox->GetItemData(pComboBox->GetCurSel());

	UpdateData(TRUE) ;

	OnOK();
}
