#include "Stdafx.h"
#include "DlgLogon.h"
#include "GamePlaza.h"
#include "MissionLogon.h"
#include "DlgProxyConfig.h"

//////////////////////////////////////////////////////////////////////////////////

//Բ�Ǵ�С
#define ROUND_CX					7									//Բ�ǿ��
#define ROUND_CY					7									//Բ�Ǹ߶�

//��Ļλ��
#define LAYERED_SIZE				5									//�ֲ��С
#define CAPTION_SIZE				35									//�����С

//�ؼ���ʶ
#define IDC_WEB_PUBLICIZE			300									//����ؼ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgLogon, CDialog)

	//ϵͳ��Ϣ
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_REGISTER, OnBnClickedRegister)
	ON_BN_CLICKED(IDC_PROXY_CONFIG, OnBnClickedProxyConfig)
	ON_BN_CLICKED(IDC_DELETE_RECORD, OnBnClickedDeleteRecord)

	//������Ϣ
	ON_BN_CLICKED(IDC_BT_LOGON_LINK_1, OnBnClickedLogonLink1)
	ON_BN_CLICKED(IDC_BT_LOGON_LINK_2, OnBnClickedLogonLink2)
	ON_BN_CLICKED(IDC_BT_LOGON_LINK_3, OnBnClickedLogonLink3)
	ON_BN_CLICKED(IDC_BT_LOGON_LINK_4, OnBnClickedLogonLink4)

	//�ؼ���Ϣ
	ON_CBN_SELCHANGE(IDC_GAME_ID, OnSelchangeGameID)
	ON_CBN_SELCHANGE(IDC_ACCOUNTS, OnSelchangeAccounts)
	ON_CBN_EDITCHANGE(IDC_ACCOUNTS, OnCbnEditchangeAccounts)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONTROL, OnTcnSelchangeLogonMode)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgLogon::CDlgLogon() : CDialog(IDD_DLG_LOGON)
{
	//��¼��Ϣ
	m_dwGameID=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szLogonServer[0]=0;

	//���ñ���
	m_cbRemPassword=FALSE;
	m_cbLogonMode=LOGON_BY_ACCOUNTS;

	//������ˢ
	m_brBrush.CreateSolidBrush(RGB(215,223,228));

	return;
}

//��������
CDlgLogon::~CDlgLogon()
{
	//������Դ
	if (m_brBrush.GetSafeHandle()!=NULL)
	{
		m_brBrush.DeleteObject();
	}

	//ɾ������
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		SafeDelete(m_AccountsInfoArray[i]);
	}

	return;
}

//�ؼ���
VOID CDlgLogon::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//�󶨿ؼ�
	DDX_Control(pDX, IDOK, m_btEnter);
	DDX_Control(pDX, IDC_REGISTER, m_btRegister);

	//�󶨿ؼ�
	DDX_Control(pDX, IDCANCEL, m_btQuit);
	DDX_Control(pDX, IDC_PROXY_CONFIG, m_btOption);
	DDX_Control(pDX, IDC_DELETE_RECORD, m_btDeleteRecord);

	//�����ؼ�
	DDX_Control(pDX, IDC_GAME_ID, m_GameIDControl);
	DDX_Control(pDX, IDC_ACCOUNTS, m_AccountsControl);
	DDX_Control(pDX, IDC_PASSWORD, m_PasswordControl);

	//�����ؼ�
	DDX_Control(pDX, IDC_TAB_CONTROL, m_TabControl);
	DDX_Control(pDX, IDC_LOGON_SERVER, m_ServerControl);

	//���ӿؼ�
	DDX_Control(pDX, IDC_BT_LOGON_LINK_1, m_btLogonLink1);
	DDX_Control(pDX, IDC_BT_LOGON_LINK_2, m_btLogonLink2);
	DDX_Control(pDX, IDC_BT_LOGON_LINK_3, m_btLogonLink3);
	DDX_Control(pDX, IDC_BT_LOGON_LINK_4, m_btLogonLink4);
}

//��������
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�û���¼ -- Version 6603"));
	
	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_LOGON_BACK"));

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//�߿�����
	m_ServerControl.SetDrawBorad(false);
	m_GameIDControl.SetDrawBorad(false);
	m_AccountsControl.SetDrawBorad(false);
	m_PasswordControl.SetDrawBorad(false);

	//��Ⱦ����
	m_ServerControl.SetRenderImage(false);
	m_GameIDControl.SetRenderImage(false);
	m_AccountsControl.SetRenderImage(false);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btQuit.SetButtonImage(IDB_BT_LOGON_QUIT,hInstance,false,false);
	m_btEnter.SetButtonImage(IDB_BT_LOGON,TEXT("BT_LOGON_ENTER"),hInstance,false,false);
	m_btOption.SetButtonImage(IDB_BT_LOGON_ITEM,TEXT("BT_LOGON_ITEM"),hInstance,false,false);
	m_btRegister.SetButtonImage(IDB_BT_LOGON,TEXT("BT_LOGON_REGISTER"),hInstance,false,false);
	m_btDeleteRecord.SetButtonImage(IDB_BT_LOGON_ITEM,TEXT("BT_LOGON_ITEM"),hInstance,false,false);
	m_btLogonLink1.SetButtonImage(IDB_BT_LOGON_LINK,TEXT("BT_LOGON_LINK_1"),hInstance,false,false);
	m_btLogonLink2.SetButtonImage(IDB_BT_LOGON_LINK,TEXT("BT_LOGON_LINK_2"),hInstance,false,false);
	m_btLogonLink3.SetButtonImage(IDB_BT_LOGON_LINK,TEXT("BT_LOGON_LINK_3"),hInstance,false,false);
	m_btLogonLink4.SetButtonImage(IDB_BT_LOGON_LINK,TEXT("BT_LOGON_LINK_4"),hInstance,false,false);

	//���ؼ�
	CRect rcCreate(0,0,0,0);
	m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/LogonLogo.asp"),szPlatformLink);

	//���ù��
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetWindowPos(NULL,LAYERED_SIZE,37,SizeWindow.cx-2*LAYERED_SIZE,84,SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE);

	//�����ǩ
	m_TabControl.SetItemSize(CSize(100,25));
	m_TabControl.InsertItem(LOGON_BY_ACCOUNTS,TEXT(" �ʺŵ�¼ "));
	m_TabControl.InsertItem(LOGON_BY_GAME_ID,TEXT(" I D ��¼ "));

	//���ÿؼ�
	m_TabControl.SetRenderImage(false);
	m_TabControl.SetTabCtrlColor(RGB(215,223,228),RGB(145,168,183));

	//������Ϣ
	LoadAccountsInfo();
	LoadLogonServerInfo();

	//���ý���
	if (m_AccountsControl.GetCurSel()!=LB_ERR)
	{
		//��Ϣ����
		OnSelchangeAccounts();

		//���ý���
		m_PasswordControl.SetFocus();
	}
	else
	{
		//���ý���
		m_AccountsControl.SetFocus();
	}

	//���д���
	CenterWindow(this);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//��Ϣ����
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ȷ������
VOID CDlgLogon::OnOK()
{
	//��ȡ��Ϣ
	if (GetInformation()==false) return;

	//���ش���
	ShowWindow(SW_HIDE);

	//ִ�е�¼
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);

	return;
}

//ȡ����Ϣ
VOID CDlgLogon::OnCancel()
{
	//�رմ���
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);

	return;
}

//��������
WORD CDlgLogon::ConstructLogonPacket(BYTE cbBuffer[], WORD wBufferSize)
{
	//��������
	TCHAR szPassword[LEN_MD5];
	CWHEncrypt::MD5Encrypt(m_szPassword,szPassword);

	//��������
	switch (m_cbLogonMode)
	{
	case LOGON_BY_GAME_ID:			//I D ��¼
		{
			//��������
			CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)cbBuffer;

			//ϵͳ��Ϣ
			pLogonGameID->dwPlazaVersion=VERSION_PLAZA;

			//������ʶ
			CWHService::GetMachineID(pLogonGameID->szMachineID);

			//��¼��Ϣ
			pLogonGameID->dwGameID=m_dwGameID;
			lstrcpyn(pLogonGameID->szPassword,szPassword,CountArray(pLogonGameID->szPassword));

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonGameID->szPassword,CountArray(pGlobalUserData->szPassword));

			return sizeof(CMD_GP_LogonGameID);
		}
	case LOGON_BY_ACCOUNTS:			//�ʺŵ�¼
		{
			//��������
			CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)cbBuffer;

			//ϵͳ��Ϣ
			pLogonAccounts->dwPlazaVersion=VERSION_PLAZA;

			//������ʶ
			CWHService::GetMachineID(pLogonAccounts->szMachineID);

			//��¼��Ϣ
			lstrcpyn(pLogonAccounts->szPassword,szPassword,CountArray(pLogonAccounts->szPassword));
			lstrcpyn(pLogonAccounts->szAccounts,m_szAccounts,CountArray(pLogonAccounts->szAccounts));

			//��������
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
			lstrcpyn(pGlobalUserData->szPassword,pLogonAccounts->szPassword,CountArray(pGlobalUserData->szPassword));

			return sizeof(CMD_GP_LogonAccounts);
		}
	}

	return 0;
}

//�����˺�
VOID CDlgLogon::LoadAccountsInfo()
{
	//��ȡ��Ϣ
	CWHRegKey RegUserInfo;
	if (RegUserInfo.OpenRegKey(REG_USER_INFO,false)==false) return;

	//��������
	DWORD dwRegIndex=0L;
	DWORD dwLastUserID=RegUserInfo.GetValue(TEXT("LastUserID"),0L);

	//��ȡ��Ϣ
	do
	{
		//��������
		tagAccountsInfo AccountsInfo;
		ZeroMemory(&AccountsInfo,sizeof(AccountsInfo));

		//��ȡ����
		TCHAR szKeyItemName[16]=TEXT("");
		if (RegUserInfo.EnumItemString(dwRegIndex++,szKeyItemName,CountArray(szKeyItemName))==false) break;

		//���Ӽ�
		CWHRegKey RegUserItem;
		if (RegUserItem.Attach(RegUserInfo.OpenItemKey(szKeyItemName))==NULL) continue;

		//�ʻ���Ϣ
		AccountsInfo.dwUserID=_tstol(szKeyItemName);
		RegUserItem.GetString(TEXT("GameID"),AccountsInfo.szGameID,CountArray(AccountsInfo.szGameID));
		RegUserItem.GetString(TEXT("UserAccount"),AccountsInfo.szAccounts,CountArray(AccountsInfo.szAccounts));

		//��ȡ����
		TCHAR szPassword[MAX_ENCRYPT_LEN]=TEXT("");
		RegUserItem.GetString(TEXT("UserPassword"),szPassword,CountArray(szPassword));

		//�⿪����
		if (szPassword[0]!=0)
		{
			CWHEncrypt::XorCrevasse(szPassword,AccountsInfo.szPassword,CountArray(AccountsInfo.szPassword));
		}

		//��������
		tagAccountsInfo * pAccountsInfo=new tagAccountsInfo;
		CopyMemory(pAccountsInfo,&AccountsInfo,sizeof(AccountsInfo));

		//��������
		m_AccountsInfoArray.InsertAt(m_AccountsInfoArray.GetCount(),pAccountsInfo);

	} while (true);

	//������Ϣ
	for (INT_PTR i=0;i<m_AccountsInfoArray.GetCount();i++)
	{
		//������Ϣ
		INT nGameIDItem=CB_ERR;
		INT nAccountsItem=CB_ERR;
		tagAccountsInfo * pAccountsInfo=m_AccountsInfoArray[i];
		
		//������Ϣ
		if (pAccountsInfo->szGameID[0]!=0) nGameIDItem=m_GameIDControl.AddString(pAccountsInfo->szGameID);
		if (pAccountsInfo->szAccounts[0]!=0) nAccountsItem=m_AccountsControl.AddString(pAccountsInfo->szAccounts);

		//��������
		if (nGameIDItem!=CB_ERR) m_GameIDControl.SetItemDataPtr(nGameIDItem,pAccountsInfo);
		if (nAccountsItem!=CB_ERR) m_AccountsControl.SetItemDataPtr(nAccountsItem,pAccountsInfo);

		//����ѡ��
		if ((dwLastUserID!=0L)&&(pAccountsInfo->dwUserID==dwLastUserID))
		{
			if (nGameIDItem!=CB_ERR) m_GameIDControl.SetCurSel(nGameIDItem);
			if (nAccountsItem!=CB_ERR) m_AccountsControl.SetCurSel(nAccountsItem);
		}
	}

	//����ѡ��
	if ((m_GameIDControl.GetCount()>0)&&(m_GameIDControl.GetCurSel()==LB_ERR)) m_GameIDControl.SetCurSel(0);
	if ((m_AccountsControl.GetCount()>0)&&(m_AccountsControl.GetCurSel()==LB_ERR)) m_AccountsControl.SetCurSel(0);

	return;
}

//��ַ��Ϣ
VOID CDlgLogon::LoadLogonServerInfo()
{
	//��ȡ��Ŀ
	CGlobalServer GlobalServer;
	WORD wItemCount=GlobalServer.GetItemCount();

	//�ϴε�¼
	TCHAR szLastServer[LEN_SERVER]=TEXT("");
	GlobalServer.GetLastServerName(szLastServer);

	//��ȡ��Ϣ
	for (WORD i=0;i<wItemCount;i++)
	{
		//��ȡ��Ϣ
		TCHAR szServerItem[LEN_SERVER]=TEXT("");
		GlobalServer.GetServerItemName(i,szServerItem);

		//������Ϣ
		if (szServerItem[0]!=0)
		{
			//������Ϣ
			INT nItem=m_ServerControl.AddString(szServerItem);

			//�����¼
			if ((szLastServer[0]!=0)&&(lstrcmp(szLastServer,szServerItem)==0))
			{
				szLastServer[0]=0;
				m_ServerControl.SetCurSel(nItem);
			}
		}
	}

	//�����¼
	if (szLastServer[0]!=0)
	{
		m_ServerControl.SetCurSel(m_ServerControl.AddString(szLastServer));
	}

	//����ѡ��
	if (m_ServerControl.GetCurSel()==LB_ERR)
	{
		if (m_ServerControl.GetCount()==0L)
		{
			m_ServerControl.SetCurSel(m_ServerControl.AddString(szLogonServer));
		}
		else
		{
			m_ServerControl.SetCurSel(0);
		}
	}

	return;
}

//��ȡ��Ϣ
bool CDlgLogon::GetInformation()
{
	//��ȡ��Ϣ
	m_dwGameID=GetDlgItemInt(IDC_GAME_ID,NULL,FALSE);
	GetDlgItemText(IDC_ACCOUNTS,m_szAccounts,CountArray(m_szAccounts));

	//��ȡ��Ϣ
	m_PasswordControl.GetUserPassword(m_szPassword);
	GetDlgItemText(IDC_LOGON_SERVER,m_szLogonServer,CountArray(m_szLogonServer));

	//��ַ�ж�
	if (m_szLogonServer[0]==0L)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��ѡ�񡰵�¼�����������ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		m_ServerControl.SetFocus();

		return false;
	}

	//I D �ж�
	if ((m_cbLogonMode==LOGON_BY_GAME_ID)&&(m_dwGameID==0L))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������� ID ���ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		m_GameIDControl.SetFocus();

		return false;
	}

	//�ʺ��ж�
	if ((m_cbLogonMode==LOGON_BY_ACCOUNTS)&&(m_szAccounts[0]==0L))
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���������ʺź��ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		m_AccountsControl.SetFocus();

		return false;
	}

	//�����ж�
	if (m_szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("��������������ٵ�¼��������������������ָ�ϡ���ȡ������"),MB_ICONERROR,0);

		//��ʾ����
		ShowWindow(SW_SHOW);

		//���ý���
		m_PasswordControl.SetFocus();

		return false;
	}

	//������Ϣ
	m_cbRemPassword=(((CButton *)GetDlgItem(IDC_REM_PASSWORD))->GetCheck()==BST_CHECKED);

	return true;
}

//ѡ��ı�
VOID CDlgLogon::OnSelchangeGameID()
{
	//��ȡѡ��
	INT nCurrentSel=m_GameIDControl.GetCurSel();
	tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)m_GameIDControl.GetItemDataPtr(nCurrentSel);

	//���ÿؼ�
	if ((nCurrentSel!=LB_ERR)&&(pAccountsInfo!=NULL))
	{
		//�û��ʺ�
		m_GameIDControl.SetCurSel(nCurrentSel);

		//��������
		m_PasswordControl.SetUserPassword(pAccountsInfo->szPassword);

		//��ס����
		bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
		((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);
	}
	else
	{
		//��������
		m_PasswordControl.SetUserPassword(TEXT(""));

		//��ס����
		((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_UNCHECKED);
	}

	return;
}

//ѡ��ı�
VOID CDlgLogon::OnSelchangeAccounts()
{
	//��ȡѡ��
	INT nCurrentSel=m_AccountsControl.GetCurSel();
	tagAccountsInfo * pAccountsInfo=(tagAccountsInfo *)m_AccountsControl.GetItemDataPtr(nCurrentSel);

	//���ÿؼ�
	if ((nCurrentSel!=LB_ERR)&&(pAccountsInfo!=NULL))
	{
		//�û��ʺ�
		m_AccountsControl.SetCurSel(nCurrentSel);

		//��������
		m_PasswordControl.SetUserPassword(pAccountsInfo->szPassword);

		//��ס����
		bool bRemPassword=(pAccountsInfo->szPassword[0]!=0);
		((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);
	}
	else
	{
		//��������
		m_PasswordControl.SetUserPassword(TEXT(""));

		//��ס����
		((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_UNCHECKED);
	}

	return;
}

//��������
VOID CDlgLogon::OnBnClickedProxyConfig()
{
	//���ش���
	ShowWindow(SW_HIDE);

	//��������
	CDlgProxyConfig DlgProxyConfig;
	DlgProxyConfig.DoModal();

	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

//�༭�ı�
VOID CDlgLogon::OnCbnEditchangeAccounts()
{
	//��ȫ����
	((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_UNCHECKED);

	//��������
	if (m_PasswordControl.IsFalsityPassword()==true) m_PasswordControl.SetUserPassword(NULL);

	return;
}

//ɾ����¼
VOID CDlgLogon::OnBnClickedDeleteRecord()
{
	//��������
	CAccountsCtrl * pAccountsCtrl=NULL;
	tagAccountsInfo * pAccountsInfo=NULL;

	//��ȡ����
	switch (m_TabControl.GetCurSel())
	{
	case LOGON_BY_GAME_ID: { pAccountsCtrl=&m_GameIDControl; break; }
	case LOGON_BY_ACCOUNTS: { pAccountsCtrl=&m_AccountsControl; break;	}
	default: ASSERT(FALSE);
	}

	//��ȡ��Ϣ
	INT nCurrentSel=pAccountsCtrl->GetCurSel();

	//ɾ������
	if (nCurrentSel!=LB_ERR)
	{
		//��ȡ����
		ASSERT(pAccountsCtrl->GetItemDataPtr(nCurrentSel)!=NULL);
		pAccountsInfo=(tagAccountsInfo *)pAccountsCtrl->GetItemDataPtr(nCurrentSel);

		//�����ʶ
		CString strUserID;
		strUserID.Format(TEXT("%ld"),pAccountsInfo->dwUserID);

		//�û�����
		CWHRegKey RegUserInfo;
		RegUserInfo.OpenRegKey(REG_USER_INFO,false);

		//ɾ������
		RegUserInfo.RecurseDeleteKey(strUserID);

		//ɾ���б�
		for (INT i=0;i<m_GameIDControl.GetCount();i++)
		{
			if (m_GameIDControl.GetItemDataPtr(i)==pAccountsInfo)
			{
				//ɾ���ַ�
				m_GameIDControl.DeleteString(i);
				m_GameIDControl.SetWindowText(TEXT(""));

				//����ѡ��
				if ((m_GameIDControl.GetCurSel()==LB_ERR)&&(m_GameIDControl.GetCount()>0))
				{
					m_GameIDControl.SetCurSel(0);
				}

				break;
			}
		}

		//ɾ���б�
		for (INT i=0;i<m_AccountsControl.GetCount();i++)
		{
			if (m_AccountsControl.GetItemDataPtr(i)==pAccountsInfo)
			{
				//ɾ���ַ�
				m_AccountsControl.DeleteString(i);
				m_AccountsControl.SetWindowText(TEXT(""));

				//����ѡ��
				if ((m_AccountsControl.GetCurSel()==LB_ERR)&&(m_AccountsControl.GetCount()>0))
				{
					m_AccountsControl.SetCurSel(0);
				}

				break;
			}
		}
	}

	//���ý���
	pAccountsCtrl->SetFocus();

	//��ȡ����
	nCurrentSel=pAccountsCtrl->GetCurSel();
	pAccountsInfo=(nCurrentSel!=LB_ERR)?(tagAccountsInfo *)pAccountsCtrl->GetItemDataPtr(nCurrentSel):NULL;

	//��������
	m_PasswordControl.SetUserPassword((pAccountsInfo!=NULL)?pAccountsInfo->szPassword:TEXT(""));

	//��ס����
	bool bRemPassword=(pAccountsInfo!=NULL)?(pAccountsInfo->szPassword[0]!=0):false;
	((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck((bRemPassword==true)?BST_CHECKED:BST_UNCHECKED);

	return;
}

//���͸ı�
VOID CDlgLogon::OnTcnSelchangeLogonMode(NMHDR * pNMHDR, LRESULT * pResult)
{
	switch (m_TabControl.GetCurSel())
	{
	case LOGON_BY_GAME_ID:		//I D ��¼
		{
			//���ñ���
			m_cbLogonMode=LOGON_BY_GAME_ID;

			//���ÿؼ�
			GetDlgItem(IDC_GAME_ID)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ACCOUNTS)->ShowWindow(SW_HIDE);
			SetDlgItemText(IDC_MODE_NAME,TEXT("��Ϸ I D��"));

			//����ѡ��
			m_TabControl.SetCurSel(LOGON_BY_GAME_ID);

			break;
		}
	case LOGON_BY_ACCOUNTS:		//�ʺŵ�¼
		{
			//���ñ���
			m_cbLogonMode=LOGON_BY_ACCOUNTS;

			//���ÿؼ�
			GetDlgItem(IDC_GAME_ID)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ACCOUNTS)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_MODE_NAME,TEXT("��Ϸ�ʺţ�"));

			//����ѡ��
			m_TabControl.SetCurSel(LOGON_BY_ACCOUNTS);

			break;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return;
}

//���Ӱ�ť
VOID CDlgLogon::OnBnClickedLogonLink1()
{
	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink1.asp"),szPlatformLink);

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//���Ӱ�ť
VOID CDlgLogon::OnBnClickedLogonLink2()
{
	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink2.asp"),szPlatformLink);

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//���Ӱ�ť
VOID CDlgLogon::OnBnClickedLogonLink3()
{
	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink3.asp"),szPlatformLink);

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//���Ӱ�ť
VOID CDlgLogon::OnBnClickedLogonLink4()
{
	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink4.asp"),szPlatformLink);

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//ע���ʺ�
VOID CDlgLogon::OnBnClickedRegister()
{
	//���ش���
	ShowWindow(SW_HIDE);

	//��ȡ��ַ
	CString strLogonServer;
	GetDlgItemText(IDC_LOGON_SERVER,strLogonServer);

	//�����ַ
	strLogonServer.TrimLeft();
	strLogonServer.TrimRight();
	lstrcpyn(m_szLogonServer,(strLogonServer.IsEmpty()==true)?szLogonServer:strLogonServer,CountArray(m_szLogonServer));

	//��ʾע��
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowRegister();

	return;
}

//�滭����
BOOL CDlgLogon::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//������Դ
	CPngImage ImageBack;
	CPngImage ImageTitle;
	ImageTitle.LoadImage(AfxGetInstanceHandle(),TEXT("LOGON_TITILE"));
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_LOGON_BACK"));

	//����ͼ��
	CPngImage ImageLogo;
	ImageLogo.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("SKIN_WND_LOGO"));

	//�滭����
	ImageBack.DrawImage(pBufferDC,0,0);
	ImageLogo.DrawImage(pBufferDC,11,6);
	ImageTitle.DrawImage(pBufferDC,40,6);

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//��ʾ��Ϣ
VOID CDlgLogon::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ�ֲ�
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//�����Ϣ
VOID CDlgLogon::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//λ�øı�
VOID CDlgLogon::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//�ƶ��ֲ�
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//�ؼ���ɫ
HBRUSH CDlgLogon::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(10,10,10));
			return m_brBrush;
		}
	}

	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////////////
