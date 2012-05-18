#include "Stdafx.h"
#include "GameOption.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameOption, CSkinDialogEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameOption::CGameOption() : CSkinDialogEx(IDD_OPTION)
{
	m_bEnableSound=true;
	m_bPlayNativeSound=1;

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
	//DDX_Control(pDX, IDOK, m_btOK);
	//DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//初始化函数
BOOL CGameOption::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("游戏配置"));

	//设置控件
	if (m_bEnableSound==true) ((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->SetCheck(BST_CHECKED);
	if(m_bPlayNativeSound==1)	((CButton *)GetDlgItem(IDC_PLAY_NATIVE_SOUND))->SetCheck(BST_CHECKED);

	return TRUE;
}

//确定消息
void CGameOption::OnOK()
{
	//获取变量
	m_bEnableSound=(((CButton *)GetDlgItem(IDC_ENABLE_SOUND))->GetCheck()==BST_CHECKED);
//保存用户配置的声音信息
	//地方话
	if((((CButton *)GetDlgItem(IDC_PLAY_NATIVE_SOUND))->GetCheck()==BST_CHECKED))
	{
		m_bPlayNativeSound=1;
		CWinApp * pApp=AfxGetApp();
		pApp->WriteProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),1);
	}
	else	//普通话
	{
		m_bPlayNativeSound=0;
		CWinApp * pApp=AfxGetApp();
		pApp->WriteProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),0);
	}
	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
