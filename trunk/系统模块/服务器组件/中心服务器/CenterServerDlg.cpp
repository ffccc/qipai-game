#include "Stdafx.h"
#include "AtlBase.h"
#include "CenterServerDlg.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSystemOptionDlg, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCenterServerDlg, CDialog)
	ON_BN_CLICKED(IDC_STOP_SERVICE, OnBnClickedStopService)
	ON_BN_CLICKED(IDC_INIT_SERVICE, OnBnClickedInitService)
	ON_BN_CLICKED(IDC_START_SERVICE, OnBnClickedStartService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CSystemOptionDlg::CSystemOptionDlg() : CDialog(IDD_SYSTEM_OPTION)
{
}

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
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_USER))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PASS))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_NAME))->LimitText(31);

	//���ز���
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	//���ÿؼ�
	SetDlgItemInt(IDC_LISTEN_PORT,InitParamter.m_wListenPort,FALSE);
	SetDlgItemInt(IDC_MAX_CONNECT,InitParamter.m_wMaxConnect,FALSE);

	//��Ϣ���ݿ�
	SetDlgItemInt(IDC_SERVER_DATABASE_PORT,InitParamter.m_wServerDataBasePort,FALSE);
	SetDlgItemText(IDC_SERVER_DATABASE_USER,InitParamter.m_szServerDataBaseUser);
	SetDlgItemText(IDC_SERVER_DATABASE_PASS,InitParamter.m_szServerDataBasePass);
	SetDlgItemText(IDC_SERVER_DATABASE_NAME,InitParamter.m_szServerDataBaseName);

	//��Ϣ���ݿ��ַ
	DWORD dwDataBaseIP=inet_addr(InitParamter.m_szServerDataBaseAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(InitParamter.m_szServerDataBaseAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	CIPAddressCtrl * pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_SERVER_DATABASE_IP);
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

	//��Ϣ���ݿ�
	InitParamter.m_wServerDataBasePort=GetDlgItemInt(IDC_SERVER_DATABASE_PORT);
	GetDlgItemText(IDC_SERVER_DATABASE_USER,InitParamter.m_szServerDataBaseUser,sizeof(InitParamter.m_szServerDataBaseUser));
	GetDlgItemText(IDC_SERVER_DATABASE_PASS,InitParamter.m_szServerDataBasePass,sizeof(InitParamter.m_szServerDataBasePass));
	GetDlgItemText(IDC_SERVER_DATABASE_NAME,InitParamter.m_szServerDataBaseName,sizeof(InitParamter.m_szServerDataBaseName));

	//��Ϣ���ݿ��ַ
	DWORD dwDataBaseIP=INADDR_NONE;
	BYTE * pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_SERVER_DATABASE_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szServerDataBaseAddr,sizeof(InitParamter.m_szServerDataBaseAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//��������
	InitParamter.SaveInitParamter(false);

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////

CCenterServerDlg::CCenterServerDlg() : CDialog(IDD_CENTERSERVER_DIALOG)
{
}

CCenterServerDlg::~CCenterServerDlg()
{
}

//�ؼ����໯
void CCenterServerDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICE_EVENT, m_RichEditTrace);
}

//��ʼ������
BOOL CCenterServerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return TRUE;
}

//ȷ����Ϣ
void CCenterServerDlg::OnOK()
{
	return;
}

//ȡ������
void CCenterServerDlg::OnCancel()
{
	//��ȡ״̬
	bool bRuning=m_CenterService.IsService();

	//ѯ���˳�
	if (bRuning==true)
	{
		CString strMessage=TEXT("���ķ��������������У�ȷʵҪ�˳���");
		if (AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)!=IDYES) return;
	}

	//ֹͣ����
	if (bRuning==true) 
	{
		m_CenterService.StopService();
	}

	__super::OnCancel();
}

//��Ϣ����
BOOL CCenterServerDlg::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) 
	{
		return TRUE;
	}
	
	return __super::PreTranslateMessage(pMsg);
}

//��������
void CCenterServerDlg::OnBnClickedStartService()
{
	//��������
	if (m_CenterService.StartService()==true)
	{
		//���ý���
		GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

		//����¼�
		CTraceService::TraceString(TEXT("���ķ��������ɹ�"),TraceLevel_Normal);
	}
	else
	{
		//����¼�
		CTraceService::TraceString(TEXT("���ķ�������ʧ��"),TraceLevel_Exception);
	}

	return;
}

//ֹͣ����
void CCenterServerDlg::OnBnClickedStopService()
{
	//ֹͣ����
	if (m_CenterService.StopService()==true)
	{
		//���ý���
		GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);

		//����¼�
		CTraceService::TraceString(TEXT("���ķ���ֹͣ�ɹ�"),TraceLevel_Normal);
	}
	else
	{
		//����¼�
		CTraceService::TraceString(TEXT("���ķ���ֹͣʧ��"),TraceLevel_Exception);
	}

	return;
}

//ϵͳ����
void CCenterServerDlg::OnBnClickedInitService()
{
	CSystemOptionDlg SystemOptionDlg;
	SystemOptionDlg.DoModal();

	return;
}

//////////////////////////////////////////////////////////////////////////
