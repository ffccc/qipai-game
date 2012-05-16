#include "Stdafx.h"
#include "AtlBase.h"
#include "LogonServerDlg.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSystemOptionDlg, CDialog)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CLogonServerDlg, CDialog)
	ON_BN_CLICKED(IDC_START_SERVICE, OnBnClickedStartService)
	ON_BN_CLICKED(IDC_STOP_SERVICE, OnBnClickedStopService)
	ON_BN_CLICKED(IDC_INIT_SERVICE, OnBnClickedInitService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSystemOptionDlg::CSystemOptionDlg() : CDialog(IDD_SYSTEM_OPTION)
{
}

//��������
CSystemOptionDlg::~CSystemOptionDlg()
{
}

//�ؼ����໯
void CSystemOptionDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CSystemOptionDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	((CEdit *)GetDlgItem(IDC_LISTEN_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_MAX_CONNECT))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_MAIN_PAGE))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_USER))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_PASS))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_NAME))->LimitText(31);

	//���ز���
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	//���ÿؼ�
	SetDlgItemInt(IDC_LISTEN_PORT,InitParamter.m_wListenPort,FALSE);
	SetDlgItemInt(IDC_MAX_CONNECT,InitParamter.m_wMaxConnect,FALSE);

	//��¼���ݿ�
	SetDlgItemInt(IDC_USER_DATABASE_PORT,InitParamter.m_wUserDataBasePort,FALSE);
	SetDlgItemText(IDC_USER_DATABASE_USER,InitParamter.m_szUserDataBaseUser);
	SetDlgItemText(IDC_USER_DATABASE_PASS,InitParamter.m_szUserDataBasePass);
	SetDlgItemText(IDC_USER_DATABASE_NAME,InitParamter.m_szUserDataBaseName);

	//��¼���ݿ��ַ
	DWORD dwDataBaseIP=inet_addr(InitParamter.m_szUserDataBaseAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(InitParamter.m_szUserDataBaseAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	CIPAddressCtrl * pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_USER_DATABASE_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	//��վ��ַ
	if (InitParamter.m_szMainPage[0]==0) SetDlgItemText(IDC_MAIN_PAGE,szStationPage);
	else SetDlgItemText(IDC_MAIN_PAGE,InitParamter.m_szMainPage);

	//���ķ�����
	dwDataBaseIP=inet_addr(InitParamter.m_szCenterServerAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(InitParamter.m_szCenterServerAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	return TRUE;
}

//ȷ������
void CSystemOptionDlg::OnOK()
{
	//��ȡ����
	CInitParamter InitParamter;
	InitParamter.m_wListenPort=GetDlgItemInt(IDC_LISTEN_PORT);
	InitParamter.m_wMaxConnect=GetDlgItemInt(IDC_MAX_CONNECT);

	//��¼���ݿ� 
	InitParamter.m_wUserDataBasePort=GetDlgItemInt(IDC_USER_DATABASE_PORT);
	GetDlgItemText(IDC_USER_DATABASE_USER,InitParamter.m_szUserDataBaseUser,sizeof(InitParamter.m_szUserDataBaseUser));
	GetDlgItemText(IDC_USER_DATABASE_PASS,InitParamter.m_szUserDataBasePass,sizeof(InitParamter.m_szUserDataBasePass));
	GetDlgItemText(IDC_USER_DATABASE_NAME,InitParamter.m_szUserDataBaseName,sizeof(InitParamter.m_szUserDataBaseName));

	//��¼���ݿ��ַ
	DWORD dwDataBaseIP=INADDR_NONE;
	BYTE * pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_USER_DATABASE_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szUserDataBaseAddr,sizeof(InitParamter.m_szUserDataBaseAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//��վ��ַ
	GetDlgItemText(IDC_MAIN_PAGE,InitParamter.m_szMainPage,sizeof(InitParamter.m_szMainPage));
	if (lstrcmp(InitParamter.m_szMainPage,szStationPage)==0) InitParamter.m_szMainPage[0]=0;

	//���ķ�����
	dwDataBaseIP=INADDR_NONE;
	pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szCenterServerAddr,sizeof(InitParamter.m_szCenterServerAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//��������
	InitParamter.SaveInitParamter(false);

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CLogonServerDlg::CLogonServerDlg() : CDialog(IDD_LOGONSERVER_DIALOG)
{
}

//��������
CLogonServerDlg::~CLogonServerDlg()
{
}

//�ؼ����໯
void CLogonServerDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICE_EVENT, m_RichEditTrace);
}

//��ʼ������
BOOL CLogonServerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return TRUE;
}

//ȷ����Ϣ
void CLogonServerDlg::OnOK()
{
	return;
}

//ȡ������
void CLogonServerDlg::OnCancel()
{
	//��ȡ״̬
	bool bRuning=m_LogonService.IsService();

	//ѯ���˳�
	if (bRuning==true)
	{
		CString strMessage=TEXT("��¼���������������У�ȷʵҪ�˳���");
		int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
		if (iRetCode!=IDYES) return;
	}

	//ֹͣ����
	if (bRuning==true)
	{
		m_LogonService.StopService();
	}

	__super::OnCancel();
}

//��Ϣ����
BOOL CLogonServerDlg::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//��������
void CLogonServerDlg::OnBnClickedStartService()
{
	//��������
	if (m_LogonService.StartService()==true)
	{
		//���ý���
		GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

		//����¼�
		CTraceService::TraceString(TEXT("��¼���������ɹ�"),TraceLevel_Normal);
	}
	else
	{
		//����¼�
		CTraceService::TraceString(TEXT("��¼��������ʧ��"),TraceLevel_Exception);
	}

	return;
}

//ֹͣ����
void CLogonServerDlg::OnBnClickedStopService()
{
	//ֹͣ����
	if (m_LogonService.StopService()==true)
	{
		//���ý���
		GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);

		//����¼�
		CTraceService::TraceString(TEXT("��¼����ֹͣ�ɹ�"),TraceLevel_Normal);
	}
	else
	{
		//����¼�
		CTraceService::TraceString(TEXT("��¼����ֹͣʧ��"),TraceLevel_Exception);
	}

	return;
}

//ϵͳ����
void CLogonServerDlg::OnBnClickedInitService()
{
	CSystemOptionDlg SystemOptionDlg;
	SystemOptionDlg.DoModal();

	return;
}

//////////////////////////////////////////////////////////////////////////

