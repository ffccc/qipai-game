#include "Stdafx.h"
#include "DlgTablePassword.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgTablePassword, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgTablePassword::CDlgTablePassword() : CSkinDialog(IDD_DLG_TABLE_PASSWORD)
{
	//设置变量
	ZeroMemory(m_szString,sizeof(m_szString));
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return;
}

//析构函数
CDlgTablePassword::~CDlgTablePassword()
{
}

//控件绑定
VOID CDlgTablePassword::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//按钮控件
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);

	//其他控件
	DDX_Control(pDX, IDC_STRING, m_stString);
	DDX_Control(pDX, IDC_PASSWORD, m_edPassword);
}

//配置函数
BOOL CDlgTablePassword::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("桌子密码"));

	//设置提示
	m_stString.SetWindowText(m_szString);

	//限制输入
	m_edPassword.LimitText(LEN_PASSWORD-1);

	return FALSE;
}

//确定函数
VOID CDlgTablePassword::OnOK()
{
	//获取密码
	m_edPassword.GetWindowText(m_szPassword,CountArray(m_szPassword));

	//密码判断
	if (m_szPassword[0]==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("游戏桌密码不能为空，请重新输入！"),MB_ICONINFORMATION);

		//设置焦点
		m_edPassword.SetFocus();

		return;
	}

	__super::OnOK();
}

//绘画消息
VOID CDlgTablePassword::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//加载资源
	CPngImage ImageTablePasswd;
	ImageTablePasswd.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_PASSWORD"));

	//绘画图标
	ImageTablePasswd.DrawImage(pDC,25,70);

	return;
}

//设置提示
VOID CDlgTablePassword::SetPromptString(LPCTSTR pszString)
{
	//设置控件
	if (m_stString.m_hWnd!=NULL)
	{
		m_stString.SetWindowText(pszString);
	}

	//设置提示
	lstrcpyn(m_szString,pszString,CountArray(m_szString));

	return;
}

//////////////////////////////////////////////////////////////////////////
