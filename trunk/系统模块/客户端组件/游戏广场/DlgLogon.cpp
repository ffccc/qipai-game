#include "Stdafx.h"
#include "Resource.h"
#include "DlgLogon.h"
#include "GlobalUnits.h"

#include ".\dlglogon.h"

//��¼��ʽ
#define LOGON_BY_ACCOUNTS				0						//�ʺŵ�¼
#define LOGON_BY_USERID					1						//ID ��¼

//��ɫ����
#define SELECT_COLOR		RGB(37,56,220)						//ѡ����ɫ

//��Ļ����
#define LESS_SCREEN_W					1024					//��С���
#define LESS_SCREEN_H					720						//��С�߶�
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTabCtrlLogon, CTabCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgRegister, CSkinDialogEx)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDlgLogon, CSkinDialogEx)
	ON_EN_CHANGE(IDC_PASSWORD, OnEnChangePassword)
	ON_CBN_SELCHANGE(IDC_USER_ID, OnSelchangeUserID)
	ON_CBN_SELCHANGE(IDC_ACCOUNTS, OnSelchangeAccounts)
	ON_BN_CLICKED(IDC_REGISTER, OnRegisterAccounts)
	ON_BN_CLICKED(IDC_DELETE_RECORD, OnDeleteAccounts)
	ON_BN_CLICKED(IDC_NET_OPTION, OnBnClickedNetOption)
	ON_BN_CLICKED(IDC_PROXY_TEST, OnBnClickedProxyTest)
	ON_NOTIFY(TCN_SELCHANGE, IDC_LOGON_TYPE, OnTcnSelchangeLogonType)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CTabCtrlLogon::CTabCtrlLogon()
{
}

//��������
CTabCtrlLogon::~CTabCtrlLogon()
{
}

//�ػ���Ϣ
void CTabCtrlLogon::OnPaint()
{
	//��������
	CPaintDC dc(this);
	CSkinWndAttribute * pSkinAttribute=CSkinDialog::GetSkinAttribute();

	//�滭����
	CRect ClipRect,ClientRect;
	dc.GetClipBox(&ClipRect);
	dc.FillSolidRect(&ClipRect,pSkinAttribute->m_crBackGround);

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(pSkinAttribute->GetDefaultFont());
	
	//�滭�߿�
	GetClientRect(&ClientRect);
	ClientRect.top+=18;
	dc.Draw3dRect(&ClientRect,pSkinAttribute->m_crInsideBorder,pSkinAttribute->m_crInsideBorder);

	//��ȡ��Ϣ
	TCITEM ItemInfo;
	TCHAR szBuffer[100];
	memset(&ItemInfo,0,sizeof(ItemInfo));
	ItemInfo.mask=TCIF_TEXT|TCIF_IMAGE;
	ItemInfo.pszText=szBuffer;
	ItemInfo.cchTextMax=sizeof(szBuffer);

	//�滭��ť
	CRect rcItem;
	int iCursel=GetCurSel();
	CPen LinePen(PS_SOLID,1,pSkinAttribute->m_crInsideBorder);
	CPen * pOldPen=dc.SelectObject(&LinePen);
	for (int i=0;i<GetItemCount();i++)
	{
		GetItem(i,&ItemInfo);
		GetItemRect(i,&rcItem);
		dc.FillSolidRect(&rcItem,pSkinAttribute->m_crBackGround);
		dc.MoveTo(rcItem.left,rcItem.bottom-1);
		dc.LineTo(rcItem.left,rcItem.top+2);
		dc.LineTo(rcItem.left+2,rcItem.top);
		dc.LineTo(rcItem.right-2,rcItem.top);
		dc.LineTo(rcItem.right,rcItem.top+2);
		dc.LineTo(rcItem.right,rcItem.bottom-1);
		
		if (iCursel!=i)
		{
			rcItem.top+=2;
			dc.MoveTo(rcItem.left,rcItem.bottom-1);
			dc.LineTo(rcItem.right,rcItem.bottom-1);
			dc.SetTextColor(pSkinAttribute->m_crControlTXColor);
			DrawText(dc,szBuffer,lstrlen(szBuffer),&rcItem,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else 
		{
			rcItem.top+=3;
			dc.SetTextColor(SELECT_COLOR);
			DrawText(dc,szBuffer,lstrlen(szBuffer),&rcItem,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
	}

	//������Դ
	dc.SelectObject(pOldPen);
	LinePen.DeleteObject();

	return;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRegister::CDlgRegister() : CSkinDialogEx(IDD_REGISTER)
{
	//��¼��Ϣ
	m_wFaceID=0;
	m_cbGender=0;
	m_szSpreader[0]=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szPasswordBank[0]=0;

	return;
}

//��������
CDlgRegister::~CDlgRegister()
{
}

//�ؼ���
void CDlgRegister::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btLogon);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_USER_FACE, m_FaceSelect);
	DDX_Control(pDX, IDC_REG_WEB, m_LineRegWeb);
	DDX_Control(pDX, IDC_MAIN_PAGE, m_LineMainPage);
	DDX_Control(pDX, IDC_PASSWORD_PROTECT, m_LinePassWord);
}

//��ʼ������
BOOL CDlgRegister::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�ʺ�ע��"));

	//��������
	((CComboBox *)(GetDlgItem(IDC_ACCOUNTS)))->LimitText(NAME_LEN-1);
	((CEdit *)(GetDlgItem(IDC_PASSWORD)))->LimitText(PASS_LEN-1);
	((CEdit *)(GetDlgItem(IDC_PASSWORD2)))->LimitText(PASS_LEN-1);
	m_LineRegWeb.SetHyperLinkUrl(TEXT("http://221.234.25.72"));
	m_LineMainPage.SetHyperLinkUrl(TEXT("http://221.234.25.72"));
	m_LinePassWord.SetHyperLinkUrl(TEXT("http://221.234.25.72"));

	//����ͷ��
	g_GlobalUnits.m_UserFaceRes->FillImageList(m_ImageList);
	m_FaceSelect.SetImageList(&m_ImageList);

	//����ͷ��
	COMBOBOXEXITEM Item;
	ZeroMemory(&Item,sizeof(Item));
	Item.mask=CBEIF_IMAGE|CBEIF_SELECTEDIMAGE;
	for (int i=g_GlobalUnits.m_UserFaceRes->GetFaceCount()-1;i>=0;i--) 
	{
		Item.lParam=i;
		Item.iImage=i;
		Item.iSelectedImage=i;
		m_FaceSelect.InsertItem(&Item);
	}

	//�û��Ա�
	BYTE cbGender[]={/*GENDER_NULL,*/GENDER_BOY,GENDER_GIRL};
	LPCTSTR pszGender[]={/*TEXT("����"),*/TEXT("����"),TEXT("Ů��")};
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(IDC_GENDER);

	//�û��Ա�
	for (int i=0;i<CountArray(cbGender);i++)
	{
		int nItem=pComboBox->InsertString(i,pszGender[i]);
		pComboBox->SetItemData(nItem,cbGender[i]);
	}
	pComboBox->SetCurSel(0);

	//����ͷ��
	m_wFaceID=rand()%g_GlobalUnits.m_UserFaceRes->GetFaceCount();
	m_FaceSelect.SetCurSel(m_wFaceID);

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\Spreader.ini"),szPath);

	//��ȡ�Ƽ���
	TCHAR szSpreader[NAME_LEN]=TEXT("");
	GetPrivateProfileString(TEXT("SpreaderInfo"),TEXT("SpreaderName"),TEXT(""),szSpreader,CountArray(szSpreader),szFileName);

	//��������
	if (szSpreader[0]!=0)
	{
		SetDlgItemText(IDC_SPREADER,szSpreader);
		((CEdit *)GetDlgItem(IDC_SPREADER))->SetReadOnly();
	}

	return TRUE;
}

//ȷ������
void CDlgRegister::OnOK()
{
	CString strBuffer;

	//�û��ʺ�
	GetDlgItemText(IDC_ACCOUNTS,strBuffer);
	strBuffer.TrimLeft();
	strBuffer.TrimRight();
	if (strBuffer.GetLength()<4)
	{
		ShowInformation(TEXT("�ʺ����ֵĳ������Ϊ 4 λ�ַ�������������ע���ʺţ�"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_ACCOUNTS)->SetFocus();
		return;
	}
	lstrcpyn(m_szAccounts,strBuffer,CountArray(m_szAccounts));

	//�û�����
	GetDlgItemText(IDC_PASSWORD,strBuffer);
	if (strBuffer.GetLength()<6)
	{
		ShowInformation(TEXT("��Ϸ���볤�����Ϊ 6 λ�ַ���������������Ϸ���룡"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_PASSWORD)->SetFocus();
		return;
	}

	//ȷ������
	GetDlgItemText(IDC_PASSWORD2,m_szPassword,CountArray(m_szPassword));
	if (lstrcmp(m_szPassword,strBuffer)!=0)
	{
		ShowInformation(TEXT("��Ϸ������ȷ�����벻һ�£�����������ȷ�����룡"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_PASSWORD2)->SetFocus();
		return;
	}

	GetDlgItemText(IDC_BANK_PASSWORD,strBuffer);
	if (strBuffer.GetLength()<6)
	{
		ShowInformation(TEXT("�������볤�����Ϊ 6 λ�ַ���������������Ϸ���룡"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_BANK_PASSWORD)->SetFocus();
		return;
	}

	//ȷ������
	GetDlgItemText(IDC_BANK_PASSWORD2,m_szPasswordBank,CountArray(m_szPasswordBank));
	if (lstrcmp(m_szPasswordBank,strBuffer)!=0)
	{
		ShowInformation(TEXT("����������ȷ�����벻һ�£�����������ȷ�����룡"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_BANK_PASSWORD2)->SetFocus();
		return;
	}

	//ȷ������
	if (lstrcmp(m_szPasswordBank,m_szPassword)==0)
	{
		ShowInformation(TEXT("������������Ϸ���벻������ͬ�������������������룡"),0,MB_ICONQUESTION);
		GetDlgItem(IDC_BANK_PASSWORD)->SetFocus();
		return;
	}

	//�ƹ�Ա��
	GetDlgItemText(IDC_SPREADER,strBuffer);
	strBuffer.TrimLeft();
	strBuffer.TrimRight();
	lstrcpyn(m_szSpreader,strBuffer,CountArray(m_szSpreader));

	//�û��Ա�
	int nCurSel=((CComboBox *)GetDlgItem(IDC_GENDER))->GetCurSel();
	m_cbGender=(BYTE)((CComboBox *)GetDlgItem(IDC_GENDER))->GetItemData(nCurSel);

	//�û�ͷ��
	m_wFaceID=m_FaceSelect.GetCurSel();

	__super::OnOK();
}

//ȡ����Ϣ
void CDlgRegister::OnCancel()
{
	__super::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgLogon::CDlgLogon() : CSkinDialogEx(IDD_LOGON)
{
	//��¼��Ϣ
	m_wFaceID=0;
	m_cbGender=0;
	m_dwUserID=0;
	m_szSpreader[0]=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;
	m_szPasswordBank[0]=0;
	m_LogonMode=LogonMode_Accounts;

	//������Ϣ
	m_bRegister=false;
	m_bChangePassWord=false;

	//λ����Ϣ
	m_nFullWidth=0;
	m_nFullHeight=0;
	m_bNetOption=false;

	return;
}

//��������
CDlgLogon::~CDlgLogon()
{
}

//�ؼ���
void CDlgLogon::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btLogon);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_REGISTER, m_btRegister);
	DDX_Control(pDX, IDC_DELETE_RECORD, m_btDelete);
	DDX_Control(pDX, IDC_LOGON_TYPE, m_TabLogonMode);
	DDX_Control(pDX, IDC_MAIN_PAGE, m_LineMainPage);
	DDX_Control(pDX, IDC_PASSWORD_PROTECT, m_LinePassWord);
	DDX_Control(pDX, IDC_GET_PASSWORD, m_LineGetPassWord);

	DDX_Control(pDX, IDC_NET_OPTION, m_btNetOption);
	DDX_Control(pDX, IDC_PROXY_TEST, m_btProxyTest);	
	DDX_Control(pDX, IDC_PROXY_TYPE, m_cmProxyServerType);
	DDX_Control(pDX, IDC_PROXY_PORT, m_edProxyPort);
	DDX_Control(pDX, IDC_PROXY_SERVER, m_edProxyServer);
	DDX_Control(pDX, IDC_PROXY_USER, m_edProxyUserName);
	DDX_Control(pDX, IDC_PROXY_PASS, m_edProxyPassword);
}

//��ʼ������
BOOL CDlgLogon::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ϸ��¼"));

	//���ؼ�
	m_BrowerAD.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,100,NULL);
	m_BrowerAD.Navigate(TEXT("http://221.234.25.72/AD/LogonAD.asp"),NULL,NULL,NULL,NULL);

	//���λ��
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();
	m_BrowerAD.MoveWindow(nXExcursion+4,nYExcursion+5,307,85);

	//�����ǩ
	m_TabLogonMode.InsertItem(LOGON_BY_ACCOUNTS,TEXT("�ʺŵ�¼"));
	m_TabLogonMode.InsertItem(LOGON_BY_USERID,TEXT("�ɣĵ�¼"));

	//���ÿؼ�
	m_LineMainPage.SetHyperLinkUrl(TEXT("http://221.234.25.72"));
	m_LinePassWord.SetHyperLinkUrl(TEXT("http://221.234.25.72/Passwordprotection.asp"));
	m_LineGetPassWord.SetHyperLinkUrl(TEXT("http://221.234.25.72/FindPassWord.asp"));
	m_LineMainPage.BringWindowToTop();
	m_LinePassWord.BringWindowToTop();
	m_LineGetPassWord.BringWindowToTop();

	//��������
	((CComboBox *)(GetDlgItem(IDC_USER_ID)))->LimitText(11);
	((CComboBox *)(GetDlgItem(IDC_ACCOUNTS)))->LimitText(NAME_LEN-1);
	((CEdit *)(GetDlgItem(IDC_PASSWORD)))->LimitText(PASS_LEN-1);

	//��¼ģʽ
	m_LogonMode=(enLogonMode)AfxGetApp()->GetProfileInt(REG_OPTION_LOGON,TEXT("LogonMode"),LogonMode_Accounts);
	if ((m_LogonMode!=LogonMode_Accounts)&&(m_LogonMode!=LogonMode_UserID)) m_LogonMode=LogonMode_Accounts;
	SetLogonMode(m_LogonMode);

	//��¼����
	if (g_GlobalOption.m_enAcountsRule==enAcountsRule_AccountsAndPass)
		((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_CHECKED);

	//������Ϣ
	LoadLogonServer();
	LoadAccountsInfo();
	LoadProxyServerInfo();

	//���ý���
	UINT uControlID=IDC_ACCOUNTS;
	if (m_LogonMode==LogonMode_UserID) uControlID=IDC_USER_ID;
	if (GetDlgItem(uControlID)->GetWindowTextLength()==0)
	{
		GetDlgItem(uControlID)->SetFocus();
		((CComboBox *)GetDlgItem(uControlID))->SetEditSel(0,-1);
	}
	else 
	{
		GetDlgItem(IDC_PASSWORD)->SetFocus();
		((CEdit *)GetDlgItem(IDC_PASSWORD))->SetSel(0,-1);
	}

	//���λ��
	CRect rcFrame;
	GetWindowRect(&m_rcNormalFrame);
	GetDlgItem(IDC_BORDER)->GetWindowRect(&rcFrame);

	//��¼λ��
	m_nFullWidth=m_rcNormalFrame.Width();
	m_nFullHeight=m_rcNormalFrame.Height();

	//����λ��
	m_rcNormalFrame.bottom=rcFrame.top;
	MoveWindow(&m_rcNormalFrame,FALSE);

	return FALSE;
}

//��Ϣ����
BOOL CDlgLogon::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//ȷ������
void CDlgLogon::OnOK()
{
	//Ч������
	if (CheckLogonInput(true)==false) return;

	//���ñ���
	m_bRegister=false;

	//��¼�㳡
	ShowWindow(SW_HIDE);
	IPlazaViewItem * pIPlazaViewItem=((CGameFrame*)AfxGetMainWnd())->GetPlazaViewItem();
	ASSERT(pIPlazaViewItem!=NULL);
	pIPlazaViewItem->SendConnectMessage();

	return;
}

//ȡ����Ϣ
void CDlgLogon::OnCancel()
{
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

//���͵�¼��
bool CDlgLogon::SendLogonPacket(ITCPSocket * pIClientSocke)
{
	//��������
	TCHAR szPassword[33];
	BYTE cbBuffer[SOCKET_PACKET];
	CMD5Encrypt::EncryptData(m_szPassword,szPassword);

	//��������
	switch (m_LogonMode)
	{
	case LogonMode_Accounts:		//�ʺŵ�¼
		{
			if (m_bRegister==false)
			{
				//��������
				CMD_GP_LogonByAccounts * pLogonByAccounts=(CMD_GP_LogonByAccounts *)cbBuffer;
				memset(pLogonByAccounts,0,sizeof(CMD_GP_LogonByAccounts));
				pLogonByAccounts->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
				CopyMemory(pLogonByAccounts->szPassWord,szPassword,sizeof(pLogonByAccounts->szPassWord));
				lstrcpyn(pLogonByAccounts->szAccounts,m_szAccounts,CountArray(pLogonByAccounts->szAccounts));

				//�������к�
				tagClientSerial ClientSerial;
				g_GlobalUnits.GetClientSerial(ClientSerial);

				//�������ݰ�
				CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_LogonByAccounts),sizeof(cbBuffer)-sizeof(CMD_GP_LogonByAccounts));
				Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
				pIClientSocke->SendData(MDM_GP_LOGON,SUB_GP_LOGON_ACCOUNTS,cbBuffer,sizeof(CMD_GP_LogonByAccounts)+Packet.GetDataSize());
			}
			else
			{
				//��������
				CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;
				memset(pRegisterAccounts,0,sizeof(CMD_GP_RegisterAccounts));
				pRegisterAccounts->wFaceID=m_wFaceID;
				pRegisterAccounts->cbGender=m_cbGender;
				pRegisterAccounts->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
				lstrcpyn(pRegisterAccounts->szPassWord,szPassword,CountArray(pRegisterAccounts->szPassWord));
				lstrcpyn(pRegisterAccounts->szSpreader,m_szSpreader,CountArray(pRegisterAccounts->szSpreader));
				lstrcpyn(pRegisterAccounts->szAccounts,m_szAccounts,CountArray(pRegisterAccounts->szAccounts));

				CMD5Encrypt::EncryptData(m_szPasswordBank,szPassword);
				lstrcpyn(pRegisterAccounts->szPassWordBank,szPassword,CountArray(pRegisterAccounts->szPassWordBank));

				//�������к�
				tagClientSerial ClientSerial;
				g_GlobalUnits.GetClientSerial(ClientSerial);

				//�������ݰ�
				CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_RegisterAccounts),sizeof(cbBuffer)-sizeof(CMD_GP_RegisterAccounts));
				Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
				pIClientSocke->SendData(MDM_GP_LOGON,SUB_GP_REGISTER_ACCOUNTS,cbBuffer,sizeof(CMD_GP_RegisterAccounts)+Packet.GetDataSize());
			}

			break;
		}
	case LogonMode_UserID:			//I D ��¼
		{
			//��������
			CMD_GP_LogonByUserID * pLogonByUserID=(CMD_GP_LogonByUserID *)cbBuffer;
			memset(pLogonByUserID,0,sizeof(CMD_GP_LogonByUserID));
			pLogonByUserID->dwUserID=m_dwUserID;
			pLogonByUserID->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
			CopyMemory(pLogonByUserID->szPassWord,szPassword,sizeof(pLogonByUserID->szPassWord));
			
			//�������к�
			tagClientSerial ClientSerial;
			g_GlobalUnits.GetClientSerial(ClientSerial);

			//�������ݰ�
			CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_LogonByUserID),sizeof(cbBuffer)-sizeof(CMD_GP_LogonByUserID));
			Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
			pIClientSocke->SendData(MDM_GP_LOGON,SUB_GP_LOGON_USERID,cbBuffer,sizeof(CMD_GP_LogonByUserID)+Packet.GetDataSize());

			break;
		}
	}

	return true;
}

//��¼�ɹ�����
bool CDlgLogon::OnLogonSuccess()
{
	//��¼����
	BOOL bRemPassword=(((CButton *)GetDlgItem(IDC_REM_PASSWORD))->GetCheck()==BST_CHECKED);
	AfxGetApp()->WriteProfileInt(REG_OPTION_LOGON,TEXT("LogonMode"),m_LogonMode);
	AfxGetApp()->WriteProfileString(REG_OPTION_LOGON,TEXT("LogonServer"),m_strLogonServer);

	//������Ϣ
	if (bRemPassword==FALSE)
	{
		if (g_GlobalOption.m_enAcountsRule==enAcountsRule_AccountsAndPass)
		{
			g_GlobalOption.m_enAcountsRule=enAcountsRule_Accounts;
		}
	}
	else g_GlobalOption.m_enAcountsRule=enAcountsRule_AccountsAndPass;

	//�û���Ϣ
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	AfxGetApp()->WriteProfileInt(REG_OPTION_LOGON,TEXT("LastUserID"),UserData.dwUserID);

	//д���û���¼
	if (g_GlobalOption.m_enAcountsRule!=enAcountsRule_None)
	{
		//������Ϣ
		TCHAR szBuffer[256];
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%s\\%ld"),REG_USER_INFO,UserData.dwUserID);

		//д����Ϣ
		CRegKey RegUserInfo;
		if (RegUserInfo.Create(HKEY_CURRENT_USER,szBuffer)==ERROR_SUCCESS)
		{
			TCHAR szPassBuffer[256]=TEXT("");
			if (bRemPassword) CXOREncrypt::EncryptData(m_szPassword,szPassBuffer,CountArray(szPassBuffer));
			RegUserInfo.SetKeyValue(TEXT(""),UserData.szAccounts,TEXT("UserAccount"));
			RegUserInfo.SetKeyValue(TEXT(""),UserData.szPassWord,TEXT("UserPassToken"));
			RegUserInfo.SetKeyValue(TEXT(""),szPassBuffer,TEXT("UserPassword"));
			if (UserData.dwGameID!=0L) RegUserInfo.SetDWORDValue(TEXT("GameID"),UserData.dwGameID);
		}
	}
	else
	{
		//������Ϣ
		TCHAR szBuffer[256];
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),UserData.dwUserID);

		//д����Ϣ
		CRegKey RegUserID;
		if (RegUserID.Open(HKEY_CURRENT_USER,REG_USER_INFO)==ERROR_SUCCESS)	RegUserID.RecurseDeleteKey(szBuffer);
	}

	//�رմ���
	DestroyWindow();

	return true;
}

//��ȡ�ʺ�
void CDlgLogon::LoadAccountsInfo()
{
	//��������
	CComboBox * pComBoxAccounts=(CComboBox *)GetDlgItem(IDC_ACCOUNTS);
	CComboBox * pComBoxUserID=(CComboBox *)GetDlgItem(IDC_USER_ID);

	//��ȡ��Ϣ
	CRegKey RegUserInfo;
	RegUserInfo.Open(HKEY_CURRENT_USER,REG_USER_INFO,KEY_READ);
	if (RegUserInfo!=NULL)
	{
		int iInsertItem=CB_ERR;
		LONG lErrorCode=ERROR_SUCCESS;
		DWORD dwType=REG_SZ,dwIndex=0,dwUserDBID=0;
		TCHAR szUserIDBuffer[32]=TEXT(""),szTempBuffer[256]=TEXT("");
		do
		{
			//���ñ���
			dwUserDBID=0;
			szTempBuffer[0]=0;
			szUserIDBuffer[0]=0;

			//��ȡ��Ϸ ID
			DWORD dwBufferSize=sizeof(szUserIDBuffer);
			if (RegUserInfo.EnumKey(dwIndex++,szUserIDBuffer,&dwBufferSize,NULL)!=ERROR_SUCCESS) break;
			dwUserDBID=atol(szUserIDBuffer);
			if (dwUserDBID==0) continue;

			//����������Ϣ
			CRegKey RegUserAccount;
			_snprintf(szTempBuffer,sizeof(szTempBuffer),TEXT("%s\\%ld"),REG_USER_INFO,dwUserDBID);
			RegUserAccount.Open(HKEY_CURRENT_USER,szTempBuffer,KEY_READ);
			if (RegUserAccount!=NULL)
			{
				//��Ϸ�ʺ�
				dwBufferSize=sizeof(szTempBuffer);
				lErrorCode=RegUserAccount.QueryValue(TEXT("UserAccount"),&dwType,szTempBuffer,&dwBufferSize);
				if ((lErrorCode==ERROR_SUCCESS)&&(szTempBuffer[0]!=0)&&(ComboBoxFindString(pComBoxAccounts,szTempBuffer)==LB_ERR))
				{
					iInsertItem=pComBoxAccounts->AddString(szTempBuffer);
					pComBoxAccounts->SetItemData(iInsertItem,dwUserDBID);
				}

				//��Ϸ ID
				DWORD dwGameID=0L;
				dwBufferSize=sizeof(dwGameID);
				if ((RegUserAccount.QueryValue(TEXT("GameID"),&dwType,&dwGameID,&dwBufferSize)==ERROR_SUCCESS)&&(dwGameID!=0L))
				{
					_sntprintf(szTempBuffer,CountArray(szTempBuffer),TEXT("%ld"),dwGameID);
					iInsertItem=pComBoxUserID->AddString(szTempBuffer);
					pComBoxUserID->SetItemData(iInsertItem,dwUserDBID);
				}
			}
		} while (true);
	}

	//����û�
	DWORD dwLastUserID=AfxGetApp()->GetProfileInt(REG_OPTION_LOGON,TEXT("LastUserID"),0L);
	if (dwLastUserID!=0L)
	{
		for (int i=0;i<pComBoxUserID->GetCount();i++)
		{
			DWORD dwComboBoxID=(DWORD)pComBoxUserID->GetItemData(i);
			if ((dwComboBoxID!=0L)&&(dwComboBoxID==dwLastUserID))
			{
				pComBoxUserID->SetCurSel(i);
				UpdateUserComboBox(IDC_USER_ID);
				break;
			}
		}
	}

	//����ѡ��
	if ((pComBoxUserID->GetCurSel()==LB_ERR)&&(pComBoxUserID->GetCount()>0))
	{
		pComBoxUserID->SetCurSel(0);
		UpdateUserComboBox(IDC_USER_ID);
	}

	//����ѡ��
	if ((pComBoxAccounts->GetCurSel()==LB_ERR)&&(pComBoxAccounts->GetCount()>0))
	{
		pComBoxAccounts->SetCurSel(0);
		UpdateUserComboBox(IDC_ACCOUNTS);
	}

	return;
}

//���ط�����
void CDlgLogon::LoadLogonServer()
{
	//��ȡ�����¼������
	CComboBox * pComBoxServer=(CComboBox *)GetDlgItem(IDC_SERVER);
	m_strLogonServer=AfxGetApp()->GetProfileString(REG_OPTION_LOGON,TEXT("LogonServer"),NULL);
	if (m_strLogonServer.IsEmpty()==FALSE)
	{
		pComBoxServer->AddString(m_strLogonServer);
		pComBoxServer->SetWindowText(m_strLogonServer);
	}

	//��ȡ�������б�
	CRegKey RegLogonServer;
	RegLogonServer.Open(HKEY_CURRENT_USER,REG_LOGON_SERVER,KEY_READ);
	if (RegLogonServer!=NULL)
	{
		TCHAR szLogonServer[128]=TEXT("");
		DWORD dwIndex=0,dwBufferSize=sizeof(szLogonServer);
		do
		{
			dwBufferSize=sizeof(szLogonServer);
			if (RegLogonServer.EnumKey(dwIndex++,szLogonServer,&dwBufferSize,NULL)!=ERROR_SUCCESS) break;
			if (szLogonServer[0]!=0)
			{
				if (m_strLogonServer.IsEmpty()) m_strLogonServer=szLogonServer;
				if (ComboBoxFindString(pComBoxServer,szLogonServer)==LB_ERR) pComBoxServer->AddString(szLogonServer);
			}
		} while (true);
	}

	//����ѡ��
	if ((pComBoxServer->GetWindowTextLength()==0)&&(pComBoxServer->GetCount()>0)) pComBoxServer->SetCurSel(0);
	if (pComBoxServer->GetCount()==0)
	{
		pComBoxServer->AddString(TEXT("221.234.25.72"));
		pComBoxServer->SetCurSel(0);
	}

	return;
}

//������Ϣ
void CDlgLogon::LoadProxyServerInfo()
{
	//��������
	LPCTSTR szProxyType[]={TEXT("��ʹ�ô���"),TEXT("HTTP ����"),TEXT("SOCKS 4"),TEXT("SOCKS 5")};
	enProxyServerType ProxyServerType[]={ProxyType_None,ProxyType_Http,ProxyType_Socks4,ProxyType_Socks5};

	//��ȡ��Ϣ
	enProxyServerType CurrentProxyType=g_GlobalOption.m_ProxyServerType;
	const tagProxyServerInfo & ProxyServerInfo=g_GlobalOption.m_ProxyServerInfo;

	//��������
	for (INT i=0;i<CountArray(szProxyType);i++)
	{
		ASSERT(i<CountArray(ProxyServerType));
		INT nItem=m_cmProxyServerType.AddString(szProxyType[i]);
		m_cmProxyServerType.SetItemData(nItem,ProxyServerType[i]);
		if (CurrentProxyType==ProxyServerType[i]) m_cmProxyServerType.SetCurSel(nItem);
	}

	//������Ϣ
	SetDlgItemText(IDC_PROXY_USER,ProxyServerInfo.szUserName);
	SetDlgItemText(IDC_PROXY_PASS,ProxyServerInfo.szPassword);
	SetDlgItemText(IDC_PROXY_SERVER,ProxyServerInfo.szProxyServer);
	if (ProxyServerInfo.wProxyPort!=0) SetDlgItemInt(IDC_PROXY_PORT,ProxyServerInfo.wProxyPort);

	//����ѡ��
	if (m_cmProxyServerType.GetCurSel()==CB_ERR) m_cmProxyServerType.SetCurSel(0);

	return;
}

//��������
int CDlgLogon::ComboBoxFindString(CComboBox * pComboBox, LPCTSTR pszFindString)
{
	CString strBuffer;
	int iItemHandle=LB_ERR,iItemFind=LB_ERR;
	int nStringLen=lstrlen(pszFindString);
	do 
	{
		iItemFind=pComboBox->FindString((iItemHandle==LB_ERR)?0:(iItemHandle+1),pszFindString);
		if ((iItemFind==LB_ERR)||(iItemHandle==iItemFind)||(iItemFind<iItemHandle)) return LB_ERR;
		iItemHandle=iItemFind;
		pComboBox->GetLBText(iItemHandle,strBuffer);
		if (pComboBox->GetLBTextLen(iItemHandle)==nStringLen) return iItemHandle;
	} while (true);
	return LB_ERR;
}

//Ч������
bool CDlgLogon::CheckLogonInput(bool bShowError)
{
	//�������
	UINT uControlID=0;
	CString strBuffer;

	try
	{
		//��Ļ����
		RECT reWindowSize;
		GetDesktopWindow()->GetWindowRect(&reWindowSize);
		INT nHeight =reWindowSize.bottom-reWindowSize.top;
		INT nWidth =reWindowSize.right-reWindowSize.left;
		
		//�ж�����
		if(nWidth<LESS_SCREEN_W || nHeight<LESS_SCREEN_H)
		{
			TCHAR szInfo[255]=TEXT("");
			_sntprintf(szInfo,CountArray(szInfo),TEXT("��Ļ������Ҫ��%d*%d�����ϲſ���������Ϸ!"),LESS_SCREEN_W,LESS_SCREEN_H);
			throw szInfo;
		}

		//��¼������
		GetDlgItemText(IDC_SERVER,m_strLogonServer);
		m_strLogonServer.TrimLeft();
		m_strLogonServer.TrimRight();
		if (m_strLogonServer.IsEmpty())
		{
			uControlID=IDC_SERVER;
			throw TEXT("��¼����������Ϊ�գ�������ѡ����������¼��������");
		}

		//��¼�ʺ�
		switch (m_LogonMode)
		{
		case LogonMode_Accounts:		//�ʺŵ�¼
			{
				GetDlgItemText(IDC_ACCOUNTS,strBuffer);
				strBuffer.TrimLeft();
				strBuffer.TrimRight();
				if (strBuffer.IsEmpty())
				{
					uControlID=IDC_ACCOUNTS;
					throw TEXT("�û���¼�ʺŲ���Ϊ�գ������������¼�ʺţ�");
				}
				lstrcpyn(m_szAccounts,strBuffer,CountArray(m_szAccounts));
				break;
			}
		case LogonMode_UserID:			//ID ��¼
			{
				GetDlgItemText(IDC_USER_ID,strBuffer);
				strBuffer.TrimLeft();
				strBuffer.TrimRight();
				m_dwUserID=atoi(strBuffer);
				if (m_dwUserID==0L)
				{
					uControlID=IDC_USER_ID;
					throw TEXT("�û���¼ ID ����Ϊ�գ������������¼ ID ��");
				}
				break;
			}
		}

		//�û�����
		if (m_bChangePassWord==true)
		{
			m_bChangePassWord=false;
			GetDlgItemText(IDC_PASSWORD,m_szPassword,CountArray(m_szPassword));
		}
		if (m_szPassword[0]==0)
		{
			uControlID=IDC_PASSWORD;
			throw TEXT("��¼���벻��Ϊ�գ������������¼���룡");
		}

		//��������
		CComboBox * pComProxyType=(CComboBox *)GetDlgItem(IDC_PROXY_TYPE);
		enProxyServerType ProxyServerType=(enProxyServerType)pComProxyType->GetItemData(pComProxyType->GetCurSel());

		//������Ϣ
		tagProxyServerInfo ProxyServerInfo;
		ZeroMemory(&ProxyServerInfo,sizeof(ProxyServerInfo));
		ProxyServerInfo.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
		GetDlgItemText(IDC_PROXY_USER,ProxyServerInfo.szUserName,CountArray(ProxyServerInfo.szUserName));
		GetDlgItemText(IDC_PROXY_PASS,ProxyServerInfo.szPassword,CountArray(ProxyServerInfo.szPassword));
		GetDlgItemText(IDC_PROXY_SERVER,ProxyServerInfo.szProxyServer,CountArray(ProxyServerInfo.szProxyServer));

		//Ч�����
		if (ProxyServerType!=ProxyType_None)
		{
			//�����ַ
			if (ProxyServerInfo.szProxyServer[0]==0)
			{
				ShowInformation(TEXT("�����������ַ����Ϊ�գ����������룡"),0,MB_ICONINFORMATION);
				if (m_bNetOption==false) SwitchNetOption(true);
				m_edProxyServer.SetFocus();
				return false;
			}

			//����˿�
			if (ProxyServerInfo.wProxyPort==0)
			{
				ShowInformation(TEXT("���������������˿ںţ����磺1080��"),0,MB_ICONINFORMATION);
				if (m_bNetOption==false) SwitchNetOption(true);
				m_edProxyPort.SetFocus();
				return false;
			}
		}

		//��������
		g_GlobalOption.m_ProxyServerType=ProxyServerType;
		g_GlobalOption.m_ProxyServerInfo=ProxyServerInfo;

		return true;
	}
	catch (LPCTSTR pszError)
	{
		if (bShowError)
		{
			ShowWindow(SW_SHOW);
			BringWindowToTop();
			ShowInformation(pszError,0,MB_ICONQUESTION);
			if (uControlID!=0) GetDlgItem(uControlID)->SetFocus();
		}

	}
	return false;
}

//����ѡ��
void CDlgLogon::UpdateUserComboBox(UINT uComboBoxID)
{
	//��ȡ ID
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(uComboBoxID);
	DWORD dwUserDBID=(DWORD)pComboBox->GetItemData(pComboBox->GetCurSel());

	//������Ϣ
	UpdateUserPassWord(dwUserDBID);
	UINT uComboBoxIDs[]={IDC_ACCOUNTS,IDC_USER_ID};
	for (int i=0;i<CountArray(uComboBoxIDs);i++)
	{
		pComboBox=(CComboBox *)GetDlgItem(uComboBoxIDs[i]);
		for (int j=0;j<pComboBox->GetCount();j++)
		{
			DWORD dwComboBoxID=(DWORD)pComboBox->GetItemData(j);
			if ((dwComboBoxID!=0L)&&(dwComboBoxID==dwUserDBID))
			{
				pComboBox->SetCurSel(j);
				break;
			}
		}
	}

	return;
}

//��������
void CDlgLogon::UpdateUserPassWord(DWORD dwUserDBID)
{
	if (dwUserDBID!=0L)
	{
		//ע����ַ�
		CString strRegKey;
		strRegKey.Format(TEXT("%s\\%ld"),REG_USER_INFO,dwUserDBID);

		//��ע���
		CRegKey RegPassWord;
		RegPassWord.Open(HKEY_CURRENT_USER,strRegKey,KEY_READ);
		if (RegPassWord!=NULL)
		{
			DWORD dwType=REG_SZ;
			TCHAR szPassBuffer[256]=TEXT("");
			DWORD dwBufferLength=sizeof(szPassBuffer);
			if (RegPassWord.QueryValue(TEXT("UserPassword"),&dwType,szPassBuffer,&dwBufferLength)==ERROR_SUCCESS)
			{
				m_szPassword[0]=0;
				CXOREncrypt::CrevasseData(szPassBuffer,m_szPassword,CountArray(m_szPassword));
				if (m_szPassword[0]==0) SetDlgItemText(IDC_PASSWORD,TEXT(""));
				else SetDlgItemText(IDC_PASSWORD,TEXT("**********"));
				m_bChangePassWord=false;
				return;
			}
		}
	}

	//������Ϣ
	m_szPassword[0]=0;
	SetDlgItemText(IDC_PASSWORD,TEXT(""));
	m_bChangePassWord=false;
	
	return;
}

//����ģʽ
void CDlgLogon::SwitchNetOption(bool bNetOption)
{
	//���ñ���
	m_bNetOption=bNetOption;

	//�ؼ�����
	GetDlgItem(IDC_PROXY_TYPE)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_PORT)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_USER)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_PASS)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_TEST)->EnableWindow(m_bNetOption);
	GetDlgItem(IDC_PROXY_SERVER)->EnableWindow(m_bNetOption);

	//�ƶ�λ��
	INT nHeight=m_bNetOption?m_nFullHeight:m_rcNormalFrame.Height();
	SetWindowPos(NULL,0,0,m_nFullWidth,nHeight,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);

	return;
}

//����ģʽ
void CDlgLogon::SetLogonMode(enLogonMode LogonMode)
{
	m_LogonMode=LogonMode;
	switch (m_LogonMode)
	{
	case LogonMode_Accounts:	//�ʺŵ�¼
		{
			m_TabLogonMode.SetCurSel(LOGON_BY_ACCOUNTS);
			SetDlgItemText(IDC_TYPE_NAME,TEXT("��Ϸ�ʺţ�"));
			GetDlgItem(IDC_ACCOUNTS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_USER_ID)->ShowWindow(SW_HIDE);
			break;
		}
	case LogonMode_UserID:		//I D ��¼
		{
			m_TabLogonMode.SetCurSel(LOGON_BY_USERID);
			SetDlgItemText(IDC_TYPE_NAME,TEXT("��Ϸ I D��"));
			GetDlgItem(IDC_ACCOUNTS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_USER_ID)->ShowWindow(SW_SHOW);
			break;
		}
	}
	return;
}

//����ı�
void CDlgLogon::OnEnChangePassword()
{
	m_bChangePassWord=true;
}

//ѡ��ı�
void CDlgLogon::OnSelchangeAccounts()
{
	UpdateUserComboBox(IDC_ACCOUNTS);
	return;
}

//ѡ��ı�
void CDlgLogon::OnSelchangeUserID()
{
	UpdateUserComboBox(IDC_USER_ID);
	return;
}

//���͸ı�
void CDlgLogon::OnTcnSelchangeLogonType(NMHDR * pNMHDR, LRESULT * pResult)
{
	switch (m_TabLogonMode.GetCurSel())
	{
	case LOGON_BY_ACCOUNTS:{ SetLogonMode(LogonMode_Accounts); break; }
	case LOGON_BY_USERID:{ SetLogonMode(LogonMode_UserID); break; }
	default: ASSERT(FALSE);
	}
	return;
}

//ע���ʺ�
void CDlgLogon::OnRegisterAccounts()
{
//#ifndef _DEBUG
//	ShellExecute(NULL,TEXT("open"),TEXT("http://221.234.25.72/service/viewrule.jsp"),NULL,NULL,SW_SHOWDEFAULT);
//	return;
//#endif

	CDlgRegister DlgRegister;
	ShowWindow(SW_HIDE);
	if (DlgRegister.DoModal()!=IDOK) 
	{
		ShowWindow(SW_SHOW);
		return;
	}

	//���ñ���
	m_bRegister=true;
	m_wFaceID=DlgRegister.m_wFaceID;
	m_cbGender=DlgRegister.m_cbGender;
	lstrcpy(m_szSpreader,DlgRegister.m_szSpreader);
	lstrcpy(m_szAccounts,DlgRegister.m_szAccounts);
	lstrcpy(m_szPassword,DlgRegister.m_szPassword);
	lstrcpy(m_szPasswordBank,DlgRegister.m_szPasswordBank);

	//������
	GetDlgItemText(IDC_SERVER,m_strLogonServer);
	if (m_strLogonServer.IsEmpty()) m_strLogonServer=TEXT("221.234.25.72");

	//��¼�㳡
	ShowWindow(SW_HIDE);
	IPlazaViewItem * pIPlazaViewItem=((CGameFrame*)AfxGetMainWnd())->GetPlazaViewItem();
	ASSERT(pIPlazaViewItem!=NULL);
	pIPlazaViewItem->SendConnectMessage();

	return;
}

//ɾ���û�
void CDlgLogon::OnDeleteAccounts() 
{
	//��ȡ����
	UINT uComboBoxID=0;
	switch (m_LogonMode)
	{
	case LogonMode_Accounts: { uComboBoxID=IDC_ACCOUNTS; break;	}
	case LogonMode_UserID: { uComboBoxID=IDC_USER_ID; break; }
	default: ASSERT(FALSE);
	}
	CComboBox * pComboBox=(CComboBox *)GetDlgItem(uComboBoxID);

	//ɾ����Ϣ
	CString strBuffer;
	pComboBox->GetWindowText(strBuffer);
	pComboBox->SetWindowText(TEXT(""));
	if (strBuffer.IsEmpty()) return;
	int iSelectItem=ComboBoxFindString(pComboBox,strBuffer);
	if (iSelectItem!=LB_ERR)
	{
		CRegKey RegUserID;
		CString strUserID;
		DWORD dwUserDBID=(DWORD)pComboBox->GetItemData(iSelectItem);
		strUserID.Format(TEXT("%ld"),dwUserDBID);
		RegUserID.Open(HKEY_CURRENT_USER,REG_USER_INFO);
		if (RegUserID!=NULL) RegUserID.RecurseDeleteKey(strUserID);

		//ɾ��ѡ����Ϣ
		CComboBox * pOtherComboBox=NULL;
		UINT uComboBoxIDs[]={IDC_ACCOUNTS,IDC_USER_ID};
		for (int i=0;i<CountArray(uComboBoxIDs);i++)
		{
			pOtherComboBox=(CComboBox *)GetDlgItem(uComboBoxIDs[i]);
			for (int j=0;j<pOtherComboBox->GetCount();j++)
			{
				DWORD dwUserIDRead=(DWORD)pOtherComboBox->GetItemData(j);
				if (dwUserIDRead==dwUserDBID)
				{
					if (pOtherComboBox->GetCurSel()==j) pOtherComboBox->SetWindowText(TEXT(""));
					pOtherComboBox->DeleteString(j);
					break;
				}
			}
		}
	}

	//����ѡ��
	pComboBox->SetFocus();
	if (pComboBox->GetCount()>0)
	{
		pComboBox->SetCurSel(0);
		DWORD dwUserDBID=(DWORD)pComboBox->GetItemData(0);
		UpdateUserPassWord(dwUserDBID);
		UpdateUserComboBox(uComboBoxID);
	}
	else UpdateUserPassWord(0L);

	return;
}

//��������
void CDlgLogon::OnBnClickedNetOption()
{
	//����ģʽ
	SwitchNetOption(!m_bNetOption);

	return;
}

//�������
void CDlgLogon::OnBnClickedProxyTest()
{
	//��������
	CComboBox * pComProxyType=(CComboBox *)GetDlgItem(IDC_PROXY_TYPE);
	enProxyServerType ProxyServerType=(enProxyServerType)pComProxyType->GetItemData(pComProxyType->GetCurSel());

	//������Ϣ
	tagProxyServerInfo ProxyServerInfo;
	ZeroMemory(&ProxyServerInfo,sizeof(ProxyServerInfo));
	ProxyServerInfo.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,ProxyServerInfo.szUserName,CountArray(ProxyServerInfo.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,ProxyServerInfo.szPassword,CountArray(ProxyServerInfo.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,ProxyServerInfo.szProxyServer,CountArray(ProxyServerInfo.szProxyServer));

	//Ч�����
	if (ProxyServerType!=ProxyType_None)
	{
		//�����ַ
		if (ProxyServerInfo.szProxyServer[0]==0)
		{
			ShowInformation(TEXT("�����������ַ����Ϊ�գ����������룡"),0,MB_ICONINFORMATION);
			if (m_bNetOption==false) SwitchNetOption(true);
			m_edProxyServer.SetFocus();
			return;
		}

		//����˿�
		if (ProxyServerInfo.wProxyPort==0)
		{
			ShowInformation(TEXT("���������������˿ںţ����磺1080��"),0,MB_ICONINFORMATION);
			if (m_bNetOption==false) SwitchNetOption(true);
			m_edProxyPort.SetFocus();
			return;
		}
	}
	else 
	{
		//������ʾ
		ShowInformation(TEXT("�������ô��������������Ϣ��"),0,MB_ICONINFORMATION);
		pComProxyType->SetFocus();
		return;
	}

	//�������
	CNetworkManagerHelper NetworkManagerModule;
	if (NetworkManagerModule.CreateInstance()==false)
	{
		ShowInformation(TEXT("�����������������ʧ�ܣ�����ʧ�ܣ�"),0,MB_ICONINFORMATION);
		return;
	}

	try
	{
		//�������
		DWORD dwSuccess=NetworkManagerModule->ProxyServerTesting(ProxyServerType,ProxyServerInfo);

		//�����ʾ
		if (dwSuccess==CONNECT_SUCCESS)
		{
			ShowInformation(TEXT("�������������������"),0,MB_ICONINFORMATION);
			return;
		}
		else throw TEXT("�޷����Ӵ����������");
	}
	catch (LPCTSTR pszDescribe)
	{
		ShowInformation(pszDescribe,0,MB_ICONINFORMATION);
		return;
	}

	return;
}

//�ػ���Ϣ
void CDlgLogon::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//�滭����
	DrawSkinView(&dc);

	//�滭����
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();
	dc.FillSolidRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,4,m_SkinAttribute.m_crBackFrame);
	dc.FillSolidRect(nXExcursion,ClientRect.Height()-8,ClientRect.Width()-2*nXExcursion,7,m_SkinAttribute.m_crBackFrame);
	dc.FillSolidRect(nXExcursion,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,m_SkinAttribute.m_crBackFrame);
	dc.FillSolidRect(ClientRect.Width()-nXExcursion-12,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,m_SkinAttribute.m_crBackFrame);
	dc.FillSolidRect(nXExcursion,119,ClientRect.Width()-2*nXExcursion,5,m_SkinAttribute.m_crBackFrame);

	//��������
	CPen BorderPen(PS_SOLID,1,m_SkinAttribute.m_crInsideBorder);
	CPen * pOldPen=dc.SelectObject(&BorderPen);
	dc.SelectObject(m_SkinAttribute.m_brBackGround);

	//�滭�ڿ�
	CRect rcDrawRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,ClientRect.Height());
	if (m_bMaxSize==true) rcDrawRect.DeflateRect(3,3,3,2);
	else rcDrawRect.DeflateRect(3,4,2,4);
	rcDrawRect.top=123;
	if(m_bNetOption)rcDrawRect.bottom=368;
	else rcDrawRect.bottom=284;
	dc.RoundRect(&rcDrawRect,CPoint(8,8));

	//�ͷ���Դ
	dc.SelectObject(pOldPen);
	BorderPen.DeleteObject();

	return;
}

//��ȡ����
void CDlgLogon::GetProxyInfo(enProxyServerType &ProxyServerType, tagProxyServerInfo &ProxyServerInfo)
{
	//��������
	CComboBox * pComProxyType=(CComboBox *)GetDlgItem(IDC_PROXY_TYPE);
	ProxyServerType=(enProxyServerType)pComProxyType->GetItemData(pComProxyType->GetCurSel());

	//������Ϣ
	ZeroMemory(&ProxyServerInfo,sizeof(ProxyServerInfo));
	ProxyServerInfo.wProxyPort=GetDlgItemInt(IDC_PROXY_PORT);
	GetDlgItemText(IDC_PROXY_USER,ProxyServerInfo.szUserName,CountArray(ProxyServerInfo.szUserName));
	GetDlgItemText(IDC_PROXY_PASS,ProxyServerInfo.szPassword,CountArray(ProxyServerInfo.szPassword));
	GetDlgItemText(IDC_PROXY_SERVER,ProxyServerInfo.szProxyServer,CountArray(ProxyServerInfo.szProxyServer));

	//Ч�����
	if (ProxyServerType!=ProxyType_None)
	{
		//�����ַ
		if (ProxyServerInfo.szProxyServer[0]==0)
		{
			if (m_bNetOption==false) SwitchNetOption(true);
			m_edProxyServer.SetFocus();
			throw TEXT("�����������ַ����Ϊ�գ����������룡");
		}

		//����˿�
		if (ProxyServerInfo.wProxyPort==0)
		{

			if (m_bNetOption==false) SwitchNetOption(true);
			m_edProxyPort.SetFocus();
			throw TEXT("���������������˿ںţ����磺1080��");
		}
	}
}
//////////////////////////////////////////////////////////////////////////

