#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgProxyConfig.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgProxyConfig, CSkinDialog)
	ON_BN_CLICKED(IDC_PROXY_TEST, OnBnClickedProxyTest)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgProxyConfig::CDlgProxyConfig() : CSkinDialog(IDD_DLG_PROXY_CONFIG)
{
}

//析够函数
CDlgProxyConfig::~CDlgProxyConfig()
{
}

//控件绑定
VOID CDlgProxyConfig::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//绑定控件
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_PROXY_TEST, m_btProxyTest);
	DDX_Control(pDX, IDC_PROXY_TYPE, m_cmProxyType);

	//代理控件
	DDX_Control(pDX, IDC_PROXY_PORT, m_edProxyPort);
	DDX_Control(pDX, IDC_PROXY_SERVER, m_edProxyServer);
	DDX_Control(pDX, IDC_PROXY_USER, m_edProxyUserName);
	DDX_Control(pDX, IDC_PROXY_PASS, m_edProxyPassword);
}

//创建函数
BOOL CDlgProxyConfig::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("代理服务器设置"));

	//限制输入
	((CEdit *)(GetDlgItem(IDC_PROXY_PORT)))->LimitText(5);
	((CEdit *)(GetDlgItem(IDC_PROXY_USER)))->LimitText(31);
	((CEdit *)(GetDlgItem(IDC_PROXY_PASS)))->LimitText(31);
	((CEdit *)(GetDlgItem(IDC_PROXY_SERVER)))->LimitText(31);

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	BYTE cbProxyType[]={PROXY_NONE,PROXY_HTTP,PROXY_SOCKS4,PROXY_SOCKS5};
	LPCTSTR szProxyType[]={TEXT("不使用代理"),TEXT("HTTP 代理"),TEXT("SOCKS 4"),TEXT("SOCKS 5")};

	//加载类型
	for (INT i=0;i<CountArray(szProxyType);i++)
	{
		INT nItem=m_cmProxyType.AddString(szProxyType[i]);
		m_cmProxyType.SetItemData(nItem,cbProxyType[i]);
		if (pParameterGlobal->m_cbProxyType==cbProxyType[i]) m_cmProxyType.SetCurSel(nItem);
	}

	//设置信息
	SetDlgItemText(IDC_PROXY_USER,pParameterGlobal->m_ProxyServer.szUserName);
	SetDlgItemText(IDC_PROXY_PASS,pParameterGlobal->m_ProxyServer.szPassword);
	SetDlgItemText(IDC_PROXY_SERVER,pParameterGlobal->m_ProxyServer.szProxyServer);
	if (pParameterGlobal->m_ProxyServer.wProxyPort!=0) SetDlgItemInt(IDC_PROXY_PORT,pParameterGlobal->m_ProxyServer.wProxyPort);

	//设置选择
	if (m_cmProxyType.GetCurSel()==CB_ERR) m_cmProxyType.SetCurSel(0);

	return TRUE;
}

//确定函数
VOID CDlgProxyConfig::OnOK()
{
	//获取信息
	if (GetInformation()==false) return;

	__super::OnOK();
}

//获取信息
bool CDlgProxyConfig::GetInformation()
{
	//代理类型
	BYTE cbProxyType=(BYTE)m_cmProxyType.GetItemData(m_cmProxyType.GetCurSel());

	//代理信息
	tagProxyServer ProxyServer;
	ZeroMemory(&ProxyServer,sizeof(ProxyServer));
	ProxyServer.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,ProxyServer.szUserName,CountArray(ProxyServer.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,ProxyServer.szPassword,CountArray(ProxyServer.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,ProxyServer.szProxyServer,CountArray(ProxyServer.szProxyServer));

	//效验代理
	if (cbProxyType!=PROXY_NONE)
	{
		//代理地址
		if (ProxyServer.szProxyServer[0]==0)
		{
			//错误提示
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器地址不能为空，请重新输入！"),MB_ICONERROR,30);

			//设置焦点
			m_edProxyServer.SetFocus();

			return false;
		}

		//代理端口
		if (ProxyServer.wProxyPort==0)
		{
			//错误提示
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("请输入代理服务器端口号，例如：1080！"),MB_ICONERROR,30);

			//设置焦点
			m_edProxyPort.SetFocus();

			return false;
		}
	}

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//保存配置
	pParameterGlobal->m_cbProxyType=cbProxyType;
	pParameterGlobal->m_ProxyServer=ProxyServer;

	return true;
}

//网络测试
VOID CDlgProxyConfig::OnBnClickedProxyTest()
{
	//代理类型
	BYTE cbProxyType=(BYTE)m_cmProxyType.GetItemData(m_cmProxyType.GetCurSel());

	//代理信息
	tagProxyServer ProxyServer;
	ZeroMemory(&ProxyServer,sizeof(ProxyServer));
	ProxyServer.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,ProxyServer.szUserName,CountArray(ProxyServer.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,ProxyServer.szPassword,CountArray(ProxyServer.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,ProxyServer.szProxyServer,CountArray(ProxyServer.szProxyServer));

	//效验代理
	if (cbProxyType!=PROXY_NONE)
	{
		//代理地址
		if (ProxyServer.szProxyServer[0]==0)
		{
			//错误提示
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器地址不能为空，请重新输入！"),MB_ICONERROR,30);

			//设置焦点
			m_edProxyServer.SetFocus();

			return;
		}

		//代理端口
		if (ProxyServer.wProxyPort==0)
		{
			//错误提示
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("请输入代理服务器端口号，例如：1080！"),MB_ICONERROR,30);

			//设置焦点
			m_edProxyPort.SetFocus();

			return;
		}
	}
	else 
	{
		//错误提示
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请先选择代理服务器类型与代理服务器连接信息！"),MB_ICONERROR,30);

		//设置焦点
		m_cmProxyType.SetFocus();

		return;
	}

	//创建组件
	CWHNetworkHelper WHNetworkModule;
	if (WHNetworkModule.CreateInstance()==false)
	{
		ShowInformation(TEXT("网络服务管理组件创建失败，测试失败！"),MB_ICONERROR,30);
		return;
	}

	//代理测试
	switch (WHNetworkModule->ProxyServerTesting(cbProxyType,ProxyServer))
	{
	case CONNECT_SUCCESS:				//连接成功
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器工作正常！"),MB_ICONINFORMATION);
			return;
		}
	case CONNECT_PROXY_USER_INVALID:	//用户错误
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器用户名或者密码错误！"),MB_ICONERROR);
			return;
		}
	default:							//默认处理
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("代理服务器不存在或者连接失败！"),MB_ICONERROR);
			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
