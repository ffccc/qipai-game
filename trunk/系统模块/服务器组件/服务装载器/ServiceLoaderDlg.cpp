#include "Stdafx.h"
#include "ServiceLoader.h"
#include "ServiceLoaderDlg.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSystemOptionDlg, CDialog)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CServiceLoaderDlg, CDialog)
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, OnBnClickedStop)
	ON_BN_CLICKED(IDC_CREATE, OnBnClickedCreate)
	ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
	ON_BN_CLICKED(IDC_OPTION, OnBnClickedOption)
	ON_BN_CLICKED(IDC_SYSTEM, OnBnClickedSystem)
	ON_BN_CLICKED(IDC_SAVE_OPTION, OnBnClickedSaveOption)
	ON_WM_QUERYENDSESSION()
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
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_USER))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_PASS))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_NAME))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_USER))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PASS))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_NAME))->LimitText(31);

	//���ز���
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	//��¼���ݿ�
	SetDlgItemInt(IDC_USER_DATABASE_PORT,InitParamter.m_wGameUserDBPort,FALSE);
	SetDlgItemText(IDC_USER_DATABASE_USER,InitParamter.m_szGameUserDBUser);
	SetDlgItemText(IDC_USER_DATABASE_PASS,InitParamter.m_szGameUserDBPass);
	SetDlgItemText(IDC_USER_DATABASE_NAME,InitParamter.m_szGameUserDBName);

	//��Ϣ���ݿ�
	SetDlgItemInt(IDC_SERVER_DATABASE_PORT,InitParamter.m_wServerInfoDBPort,FALSE);
	SetDlgItemText(IDC_SERVER_DATABASE_USER,InitParamter.m_szServerInfoDBUser);
	SetDlgItemText(IDC_SERVER_DATABASE_PASS,InitParamter.m_szServerInfoDBPass);
	SetDlgItemText(IDC_SERVER_DATABASE_NAME,InitParamter.m_szServerInfoDBName);

	//��¼���ݿ��ַ
	DWORD dwDataBaseIP=inet_addr(InitParamter.m_szGameUserDBAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(InitParamter.m_szGameUserDBAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	CIPAddressCtrl * pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_USER_DATABASE_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	//��Ϣ���ݿ��ַ
	dwDataBaseIP=inet_addr(InitParamter.m_szServerInfoDBAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(InitParamter.m_szServerInfoDBAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_SERVER_DATABASE_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

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

	//��¼���ݿ� 
	InitParamter.m_wGameUserDBPort=GetDlgItemInt(IDC_USER_DATABASE_PORT);
	GetDlgItemText(IDC_USER_DATABASE_USER,InitParamter.m_szGameUserDBUser,sizeof(InitParamter.m_szGameUserDBUser));
	GetDlgItemText(IDC_USER_DATABASE_PASS,InitParamter.m_szGameUserDBPass,sizeof(InitParamter.m_szGameUserDBPass));
	GetDlgItemText(IDC_USER_DATABASE_NAME,InitParamter.m_szGameUserDBName,sizeof(InitParamter.m_szGameUserDBName));

	//��¼���ݿ��ַ
	DWORD dwDataBaseIP=INADDR_NONE;
	BYTE * pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_USER_DATABASE_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szGameUserDBAddr,sizeof(InitParamter.m_szGameUserDBAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//��Ϣ���ݿ� 
	InitParamter.m_wServerInfoDBPort=GetDlgItemInt(IDC_SERVER_DATABASE_PORT);
	GetDlgItemText(IDC_SERVER_DATABASE_USER,InitParamter.m_szServerInfoDBUser,sizeof(InitParamter.m_szServerInfoDBUser));
	GetDlgItemText(IDC_SERVER_DATABASE_PASS,InitParamter.m_szServerInfoDBPass,sizeof(InitParamter.m_szServerInfoDBPass));
	GetDlgItemText(IDC_SERVER_DATABASE_NAME,InitParamter.m_szServerInfoDBName,sizeof(InitParamter.m_szServerInfoDBName));

	//��Ϣ���ݿ��ַ
	dwDataBaseIP=INADDR_NONE;
	pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_SERVER_DATABASE_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szServerInfoDBAddr,sizeof(InitParamter.m_szServerInfoDBAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

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
CServiceLoaderDlg::CServiceLoaderDlg() : CDialog(IDD_SERVICELOADER_DIALOG)
{
}

//��������
CServiceLoaderDlg::~CServiceLoaderDlg()
{
}

//�ؼ���
void CServiceLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICE_EVENT, m_RichEditCtrl);
}

//��ʼ������
BOOL CServiceLoaderDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//���ز���
	m_InitParamter.LoadInitParamter();

	//��������
	LPCTSTR pszCmdLine=AfxGetApp()->m_lpCmdLine;
	if (pszCmdLine[0]!=0)
	{
		TCHAR szDescribe[128]=TEXT("");
		bool bSuccess=m_ServiceParameter.LoadParameter(pszCmdLine);
		if (bSuccess==true)
		{
			//���ƽ���
			GetDlgItem(IDC_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_OPTION)->EnableWindow(TRUE);
			GetDlgItem(IDC_SAVE_OPTION)->EnableWindow(TRUE);

			//��ʾ��Ϣ
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����Ϸ������سɹ�"),pszCmdLine);
			ShowErrorMessasge(szDescribe,TraceLevel_Normal);
		}
		else 
		{
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����Ϸ�������ʧ��"),pszCmdLine);
			ShowErrorMessasge(szDescribe,TraceLevel_Exception);
		}
	}

	return TRUE;
}

//��Ϣ����
BOOL CServiceLoaderDlg::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//ȷ����Ϣ
void CServiceLoaderDlg::OnOK()
{
	return;
}

//ȡ������
void CServiceLoaderDlg::OnCancel()
{
	//ֹͣ����
	if (m_GameService.GetInterface()!=NULL)
	{
		//ѯ���˳�
		bool bRuning=m_GameService->IsService();
		if (bRuning==true)
		{
			CString strMessage=TEXT("��Ϸ����������������У�ȷʵҪ�˳���");
			int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
			if (iRetCode!=IDYES) return;
		}

		//ֹͣ����
		if (bRuning==true) m_GameService->StopService();
	}

	__super::OnCancel();
}

//��������
void CServiceLoaderDlg::OnBnClickedStart()
{
	//Ч��״̬
	ASSERT(m_ServiceParameter.IsLoaded()==true);
	if (m_ServiceParameter.IsLoaded()==false) return;

	//���ز���
	m_InitParamter.LoadInitParamter();

	//��������
	tagGameServiceParameter GameServiceParameter;
	memset(&GameServiceParameter,0,sizeof(GameServiceParameter));
	const tagOptionParameter & OptionParameter=m_ServiceParameter.GetOptionParameter();

	//��ȡ����
	const DWORD dwDataBaseAddr=inet_addr(OptionParameter.szDataBaseAddr);
	const tagDBConnectInfo * pDBConnectInfo=GetDBConnectInfo(dwDataBaseAddr);
	if (pDBConnectInfo==NULL)
	{
		ShowErrorMessasge(TEXT("��Ϸ���ݿ���Ϣ�����ڣ���Ϸ�����������ʧ��"),TraceLevel_Exception);
		return;
	}

	//�������
	GameServiceParameter.GameServiceOption=OptionParameter.GameServiceOption;
	GameServiceParameter.dwCenterAddr=inet_addr(m_InitParamter.m_szCenterServerAddr);
	lstrcpyn(GameServiceParameter.szModule,OptionParameter.szModuleName,CountArray(GameServiceParameter.szModule));

	//�û����ݿ�
	GameServiceParameter.GameUserDBInfo.wDataBasePort=m_InitParamter.m_wGameUserDBPort;
	GameServiceParameter.GameUserDBInfo.dwDataBaseAddr=inet_addr(m_InitParamter.m_szGameUserDBAddr);
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBaseUser,m_InitParamter.m_szGameUserDBUser,CountArray(GameServiceParameter.GameUserDBInfo.szDataBaseUser));
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBasePass,m_InitParamter.m_szGameUserDBPass,CountArray(GameServiceParameter.GameUserDBInfo.szDataBasePass));
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBaseName,m_InitParamter.m_szGameUserDBName,CountArray(GameServiceParameter.GameUserDBInfo.szDataBaseName));

	//��Ϸ���ݿ�
	GameServiceParameter.GameScoreDBInfo.dwDataBaseAddr=dwDataBaseAddr;
	GameServiceParameter.GameScoreDBInfo.wDataBasePort=pDBConnectInfo->wDataBasePort;
	lstrcpyn(GameServiceParameter.GameScoreDBInfo.szDataBaseUser,pDBConnectInfo->szDataBaseUser,CountArray(GameServiceParameter.GameScoreDBInfo.szDataBaseUser));
	lstrcpyn(GameServiceParameter.GameScoreDBInfo.szDataBasePass,pDBConnectInfo->szDataBasePass,CountArray(GameServiceParameter.GameScoreDBInfo.szDataBasePass));
	lstrcpyn(GameServiceParameter.GameScoreDBInfo.szDataBaseName,OptionParameter.szDataBaseName,CountArray(GameServiceParameter.GameScoreDBInfo.szDataBaseName));

	//��������
	if ((m_GameService.GetInterface()==NULL)&&(m_GameService.CreateInstance()==false))
	{
		ShowErrorMessasge(TEXT("��Ϸ�����������ʧ��"),TraceLevel_Exception);
		return;
	}

	//��������
	TCHAR szDescribe[128]=TEXT("");
	if (m_GameService->StartService(&GameServiceParameter)==true)
	{
		//�������
		GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_CREATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPTION)->EnableWindow(FALSE);

		//��ʾ��Ϣ
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s - %ld�����������ɹ�"),GameServiceParameter.GameServiceOption.szGameRoomName,GameServiceParameter.GameServiceOption.wServerID);
		ShowErrorMessasge(szDescribe,TraceLevel_Normal);

		//���ñ���
		TCHAR szTitle[128]=TEXT("");
		_snprintf(szTitle,sizeof(szTitle),TEXT("��Ϸ���� [����] - %s"),GameServiceParameter.GameServiceOption.szGameRoomName);
		SetWindowText(szTitle);
	}
	else 
	{
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����������ʧ��"),GameServiceParameter.GameServiceOption.szGameRoomName);
		ShowErrorMessasge(szDescribe,TraceLevel_Exception);
	}

	return;
}

//ֹͣ����
void CServiceLoaderDlg::OnBnClickedStop()
{
	if ((m_GameService.GetInterface()!=NULL)&&(m_GameService->StopService()==true))
	{
		SetWindowText(TEXT("��Ϸ���� - [ֹͣ]"));
		GetDlgItem(IDC_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CREATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPTION)->EnableWindow(TRUE);
		ShowErrorMessasge(TEXT("��Ϸ�������ֹͣ�ɹ�"),TraceLevel_Normal);
	}

	return;
}

//��������
void CServiceLoaderDlg::OnBnClickedCreate()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ��ģ��
	LPCTSTR pszFilter=TEXT("��Ϸ������� ��*.DLL��|*.DLL||");
	CFileDialog DlgOpenParameter(TRUE,NULL,NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY,pszFilter);
	DlgOpenParameter.m_pOFN->lpstrInitialDir=szPath;
	if (DlgOpenParameter.DoModal()!=IDOK) return;

	//��������
	bool bSuccess=m_ServiceParameter.CreateParameter(DlgOpenParameter.GetPathName());
	if (bSuccess==true)	ShowErrorMessasge(TEXT("��Ϸ���䴴���ɹ�"),TraceLevel_Normal);

	//���ƽ���
	GetDlgItem(IDC_START)->EnableWindow(bSuccess?TRUE:FALSE);
	GetDlgItem(IDC_OPTION)->EnableWindow(bSuccess?TRUE:FALSE);
	GetDlgItem(IDC_SAVE_OPTION)->EnableWindow(bSuccess?TRUE:FALSE);

	return;
}

//�򿪷���
void CServiceLoaderDlg::OnBnClickedOpen()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("���������ļ� ��*.ROP��|*.ROP||");
	CFileDialog FileDialog(TRUE,NULL,NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY,pszFilter);
	FileDialog.m_pOFN->lpstrInitialDir=szPath;
	if (FileDialog.DoModal()!=IDOK) return;

	//�򿪷���
	TCHAR szDescribe[128]=TEXT("");
	bool bSuccess=m_ServiceParameter.LoadParameter(FileDialog.GetPathName());
	if (bSuccess==true)
	{
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����Ϸ������سɹ�"),FileDialog.GetFileName());
		ShowErrorMessasge(szDescribe,TraceLevel_Normal);
	}
	else
	{
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����Ϸ�������ʧ��"),FileDialog.GetFileName());
		ShowErrorMessasge(szDescribe,TraceLevel_Exception);
	}

	//���ƽ���
	GetDlgItem(IDC_START)->EnableWindow(bSuccess?TRUE:FALSE);
	GetDlgItem(IDC_OPTION)->EnableWindow(bSuccess?TRUE:FALSE);
	GetDlgItem(IDC_SAVE_OPTION)->EnableWindow(bSuccess?TRUE:FALSE);

	return;
}

//���÷���
void CServiceLoaderDlg::OnBnClickedOption()
{
	m_ServiceParameter.ConfigParameter();
	return;
}

//ϵͳ����
void CServiceLoaderDlg::OnBnClickedSystem()
{
	CSystemOptionDlg SystemOptionDlg;
	SystemOptionDlg.DoModal();

	return;
}

//��������
void CServiceLoaderDlg::OnBnClickedSaveOption()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("���������ļ� ��*.ROP��|*.ROP||");
	CFileDialog DlgSaveParameter(FALSE,TEXT("ROP"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgSaveParameter.m_pOFN->lpstrInitialDir=szPath;
	if (DlgSaveParameter.DoModal()!=IDOK) return;

	//�����ļ�
	if (m_ServiceParameter.SaveParameter(DlgSaveParameter.GetPathName())==false)
	{
		ShowErrorMessasge(TEXT("���������ļ�����ʧ��"),TraceLevel_Exception);
		return;
	}

	return;
}

//��ȡ����
const tagDBConnectInfo * CServiceLoaderDlg::GetDBConnectInfo(DWORD dwDataBaseAddr)
{
	//Ѱ���ִ�
	tagDBConnectInfo * pDBConnectInfo=NULL;
	for (INT_PTR i=0;i<m_DBConnectInfoArray.GetCount();i++)
	{
		pDBConnectInfo=&m_DBConnectInfoArray[i];
		if (pDBConnectInfo->dwDataBaseAddr==dwDataBaseAddr) return pDBConnectInfo;
	}

	//�������ݿ�
	CDataBaseAide ServerInfoAide;
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		ShowErrorMessasge(TEXT("���ݿ��������ʧ�ܣ��޷���ȡ���ݿ���Ϣ"),TraceLevel_Exception);
		return NULL;
	}

	try
	{
		//�������ݿ�
		ServerInfoDB->SetConnectionInfo(m_InitParamter.m_szServerInfoDBAddr,m_InitParamter.m_wServerInfoDBPort,
			m_InitParamter.m_szServerInfoDBName,m_InitParamter.m_szServerInfoDBUser,m_InitParamter.m_szServerInfoDBPass);

		//������
		ServerInfoDB->OpenConnection();
		ServerInfoAide.SetDataBase(ServerInfoDB.GetInterface());

		//ת����ַ
		TCHAR szDataBaseAdr[16]=TEXT("");
		BYTE * pcbDataBaseAdr=(BYTE *)&dwDataBaseAddr;
		_snprintf(szDataBaseAdr,sizeof(szDataBaseAdr),TEXT("%d.%d.%d.%d"),pcbDataBaseAdr[0],pcbDataBaseAdr[1],pcbDataBaseAdr[2],pcbDataBaseAdr[3]);

		//ִ�в�ѯ
		ServerInfoAide.ResetParameter();
		ServerInfoAide.AddParameter(TEXT("@strDataBaseAddr"),szDataBaseAdr);
		LONG lReturnCode=ServerInfoAide.ExecuteProcess(TEXT("GSP_LoadDataBaseInfo"),true);

		//����ж�
		if (lReturnCode!=0L)
		{
			TCHAR szErrorDescribe[256]=TEXT("");
			ServerInfoAide.GetValue_String(TEXT("ErrorDescribe"),szErrorDescribe,CountArray(szErrorDescribe));
			ShowErrorMessasge(szErrorDescribe,TraceLevel_Exception);
			return NULL;
		}

		//��ȡ��Ϣ
		tagDBConnectInfo DBConnectInfo;
		memset(&DBConnectInfo,0,sizeof(DBConnectInfo));
		TCHAR szDBUser[512]=TEXT(""),szDBPass[512]=TEXT(""),szDBAddr[16]=TEXT("");
		ServerInfoAide.GetValue_String(TEXT("DBAddr"),szDBAddr,sizeof(szDBAddr));
		ServerInfoAide.GetValue_String(TEXT("DBUser"),szDBUser,sizeof(szDBUser));
		DBConnectInfo.wDataBasePort=ServerInfoAide.GetValue_WORD(TEXT("DBPort"));
		ServerInfoAide.GetValue_String(TEXT("DBPassword"),szDBPass,sizeof(szDBPass));
		CXOREncrypt::CrevasseData(szDBUser,DBConnectInfo.szDataBaseUser,sizeof(DBConnectInfo.szDataBaseUser));
		CXOREncrypt::CrevasseData(szDBPass,DBConnectInfo.szDataBasePass,sizeof(DBConnectInfo.szDataBasePass));

		//������Ϣ
		DBConnectInfo.dwDataBaseAddr=dwDataBaseAddr;
		INT_PTR nIndex=m_DBConnectInfoArray.Add(DBConnectInfo);
		return &m_DBConnectInfoArray[nIndex];
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return NULL;
}

//��ʾ����
void CServiceLoaderDlg::ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	CTraceService::TraceString(pszString,TraceLevel);
}

//////////////////////////////////////////////////////////////////////////

BOOL CServiceLoaderDlg::OnQueryEndSession()
{
	//ֹͣ����
	if ((m_GameService.GetInterface()!=NULL)&&(m_GameService->IsService()==true))
	{
		CString strMessage=TEXT("��Ϸ����������������У�ϵͳҪ��ע���ػ�����ʧ��");
		ShowErrorMessasge(strMessage,TraceLevel_Warning);

		return FALSE;
	}


	return TRUE;
}
