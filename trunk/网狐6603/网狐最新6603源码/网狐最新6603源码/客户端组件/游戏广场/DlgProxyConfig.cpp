#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgProxyConfig.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgProxyConfig, CSkinDialog)
	ON_BN_CLICKED(IDC_PROXY_TEST, OnBnClickedProxyTest)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgProxyConfig::CDlgProxyConfig() : CSkinDialog(IDD_DLG_PROXY_CONFIG)
{
}

//��������
CDlgProxyConfig::~CDlgProxyConfig()
{
}

//�ؼ���
VOID CDlgProxyConfig::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//�󶨿ؼ�
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_PROXY_TEST, m_btProxyTest);
	DDX_Control(pDX, IDC_PROXY_TYPE, m_cmProxyType);

	//����ؼ�
	DDX_Control(pDX, IDC_PROXY_PORT, m_edProxyPort);
	DDX_Control(pDX, IDC_PROXY_SERVER, m_edProxyServer);
	DDX_Control(pDX, IDC_PROXY_USER, m_edProxyUserName);
	DDX_Control(pDX, IDC_PROXY_PASS, m_edProxyPassword);
}

//��������
BOOL CDlgProxyConfig::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�������������"));

	//��������
	((CEdit *)(GetDlgItem(IDC_PROXY_PORT)))->LimitText(5);
	((CEdit *)(GetDlgItem(IDC_PROXY_USER)))->LimitText(31);
	((CEdit *)(GetDlgItem(IDC_PROXY_PASS)))->LimitText(31);
	((CEdit *)(GetDlgItem(IDC_PROXY_SERVER)))->LimitText(31);

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	BYTE cbProxyType[]={PROXY_NONE,PROXY_HTTP,PROXY_SOCKS4,PROXY_SOCKS5};
	LPCTSTR szProxyType[]={TEXT("��ʹ�ô���"),TEXT("HTTP ����"),TEXT("SOCKS 4"),TEXT("SOCKS 5")};

	//��������
	for (INT i=0;i<CountArray(szProxyType);i++)
	{
		INT nItem=m_cmProxyType.AddString(szProxyType[i]);
		m_cmProxyType.SetItemData(nItem,cbProxyType[i]);
		if (pParameterGlobal->m_cbProxyType==cbProxyType[i]) m_cmProxyType.SetCurSel(nItem);
	}

	//������Ϣ
	SetDlgItemText(IDC_PROXY_USER,pParameterGlobal->m_ProxyServer.szUserName);
	SetDlgItemText(IDC_PROXY_PASS,pParameterGlobal->m_ProxyServer.szPassword);
	SetDlgItemText(IDC_PROXY_SERVER,pParameterGlobal->m_ProxyServer.szProxyServer);
	if (pParameterGlobal->m_ProxyServer.wProxyPort!=0) SetDlgItemInt(IDC_PROXY_PORT,pParameterGlobal->m_ProxyServer.wProxyPort);

	//����ѡ��
	if (m_cmProxyType.GetCurSel()==CB_ERR) m_cmProxyType.SetCurSel(0);

	return TRUE;
}

//ȷ������
VOID CDlgProxyConfig::OnOK()
{
	//��ȡ��Ϣ
	if (GetInformation()==false) return;

	__super::OnOK();
}

//��ȡ��Ϣ
bool CDlgProxyConfig::GetInformation()
{
	//��������
	BYTE cbProxyType=(BYTE)m_cmProxyType.GetItemData(m_cmProxyType.GetCurSel());

	//������Ϣ
	tagProxyServer ProxyServer;
	ZeroMemory(&ProxyServer,sizeof(ProxyServer));
	ProxyServer.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,ProxyServer.szUserName,CountArray(ProxyServer.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,ProxyServer.szPassword,CountArray(ProxyServer.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,ProxyServer.szProxyServer,CountArray(ProxyServer.szProxyServer));

	//Ч�����
	if (cbProxyType!=PROXY_NONE)
	{
		//�����ַ
		if (ProxyServer.szProxyServer[0]==0)
		{
			//������ʾ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("�����������ַ����Ϊ�գ����������룡"),MB_ICONERROR,30);

			//���ý���
			m_edProxyServer.SetFocus();

			return false;
		}

		//����˿�
		if (ProxyServer.wProxyPort==0)
		{
			//������ʾ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���������������˿ںţ����磺1080��"),MB_ICONERROR,30);

			//���ý���
			m_edProxyPort.SetFocus();

			return false;
		}
	}

	//��������
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	pParameterGlobal->m_cbProxyType=cbProxyType;
	pParameterGlobal->m_ProxyServer=ProxyServer;

	return true;
}

//�������
VOID CDlgProxyConfig::OnBnClickedProxyTest()
{
	//��������
	BYTE cbProxyType=(BYTE)m_cmProxyType.GetItemData(m_cmProxyType.GetCurSel());

	//������Ϣ
	tagProxyServer ProxyServer;
	ZeroMemory(&ProxyServer,sizeof(ProxyServer));
	ProxyServer.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,ProxyServer.szUserName,CountArray(ProxyServer.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,ProxyServer.szPassword,CountArray(ProxyServer.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,ProxyServer.szProxyServer,CountArray(ProxyServer.szProxyServer));

	//Ч�����
	if (cbProxyType!=PROXY_NONE)
	{
		//�����ַ
		if (ProxyServer.szProxyServer[0]==0)
		{
			//������ʾ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("�����������ַ����Ϊ�գ����������룡"),MB_ICONERROR,30);

			//���ý���
			m_edProxyServer.SetFocus();

			return;
		}

		//����˿�
		if (ProxyServer.wProxyPort==0)
		{
			//������ʾ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���������������˿ںţ����磺1080��"),MB_ICONERROR,30);

			//���ý���
			m_edProxyPort.SetFocus();

			return;
		}
	}
	else 
	{
		//������ʾ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("����ѡ������������������������������Ϣ��"),MB_ICONERROR,30);

		//���ý���
		m_cmProxyType.SetFocus();

		return;
	}

	//�������
	CWHNetworkHelper WHNetworkModule;
	if (WHNetworkModule.CreateInstance()==false)
	{
		ShowInformation(TEXT("�����������������ʧ�ܣ�����ʧ�ܣ�"),MB_ICONERROR,30);
		return;
	}

	//�������
	switch (WHNetworkModule->ProxyServerTesting(cbProxyType,ProxyServer))
	{
	case CONNECT_SUCCESS:				//���ӳɹ�
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("�������������������"),MB_ICONINFORMATION);
			return;
		}
	case CONNECT_PROXY_USER_INVALID:	//�û�����
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("����������û��������������"),MB_ICONERROR);
			return;
		}
	default:							//Ĭ�ϴ���
		{
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������ڻ�������ʧ�ܣ�"),MB_ICONERROR);
			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
