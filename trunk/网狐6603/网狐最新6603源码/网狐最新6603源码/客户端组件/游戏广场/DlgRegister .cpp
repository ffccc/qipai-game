#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgRegister.h"
#include "MissionLogon.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRegWizardItem, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRegWizardAccount, CDlgRegWizardItem)
	ON_BN_CLICKED(IDC_SELECT_FACE, OnBnClickedSelectFace)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRegWizardUserInfo, CDlgRegWizardItem)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CITY, OnSelchangeCity)
	ON_CBN_SELCHANGE(IDC_PROVINCE, OnSelchangeProvince)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRegWizard, CSkinDialog)
	ON_BN_CLICKED(IDC_LAST, OnBnClickedLast)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_FINISH, OnBnClickedFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRegWizardItem::CDlgRegWizardItem(UINT nIDTemplate) : CDialog(nIDTemplate)
{
}

//��������
CDlgRegWizardItem::~CDlgRegWizardItem()
{
}

//��ʼ������
BOOL CDlgRegWizardItem::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return TRUE;
}

//������
bool CDlgRegWizardItem::ShowWizardItem(const CRect & rcRect, CWnd * pParentWnd)
{
	//��������
	if (m_hWnd==NULL) 
	{
		Create(m_lpszTemplateName,pParentWnd);
		SetWindowPos(NULL,rcRect.left,rcRect.top,rcRect.Width(),rcRect.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	//��ʾ����
	ShowWindow(SW_SHOW);

	return true;
}

//�滭����
BOOL CDlgRegWizardItem::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);

	return TRUE;
}

//������ɫ
HBRUSH CDlgRegWizardItem::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
			return CSkinDialog::m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//ȷ������
VOID CDlgRegWizardItem::OnOK() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDOK,0),0);

	return;
}

//ȡ����Ϣ
VOID CDlgRegWizardItem::OnCancel() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,0),0);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRegWizardAccount::CDlgRegWizardAccount() : CDlgRegWizardItem(IDD_REG_WIZARD_ACCOUNT)
{
	//���ñ���
	m_wFaceID=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;

	return;
}

//��������
CDlgRegWizardAccount::~CDlgRegWizardAccount()
{
}

//�ؼ���
VOID CDlgRegWizardAccount::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCOUNTS, m_edAccounts);
	DDX_Control(pDX, IDC_PASSWORD, m_edPassword);
	DDX_Control(pDX, IDC_PASSWORD2, m_edPassword2);
	DDX_Control(pDX, IDC_FACE_ITEM, m_FaceItemView);
	DDX_Control(pDX, IDC_SELECT_FACE, m_btSelectFace);
	DDX_Control(pDX, IDC_MAIN_STATION, m_GameHomePage);
	DDX_Control(pDX, IDC_SERVICE_ARTICLE, m_ServiceArticle);
}

//��ʼ������
BOOL CDlgRegWizardAccount::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	m_wFaceID=rand()%MAX_NORMAL_FACE;

	//��������
	m_edAccounts.LimitText(LEN_ACCOUNTS-1);
	m_edPassword.LimitText(LEN_PASSWORD-1);
	m_edPassword2.LimitText(LEN_PASSWORD-1);

	//��������
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//��������
	TCHAR szBrowseUrl[MAX_PATH]=TEXT("");
	CGlobalStation * pGlobalStation=CGlobalStation::GetInstance();
	m_GameHomePage.SetHyperLinkUrl(pGlobalStation->GetStationHomeLink());
	m_ServiceArticle.SetHyperLinkUrl(pGlobalStation->GetNativeIniLink(LINK_ID_SERVICE_ARTICLE,szBrowseUrl,CountArray(szBrowseUrl)));

	//���ýӿ�
	m_FaceItemView.SetFaceControl(theApp.m_FaceControlModule.GetInterface());

	//��������
	m_FaceItemView.SetFaceItem(m_wFaceID);
	SetDlgItemText(IDC_ACCOUNTS,m_szAccounts);
	SetDlgItemText(IDC_PASSWORD,m_szPassword);
	SetDlgItemText(IDC_PASSWORD2,m_szPassword);

	//��ס����
	if (pParameterGlobal->m_AcountsRule==AcountsRule_AccountsAndPass)
	{
		((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_CHECKED);
	}

	//���ý���
	m_edAccounts.SetFocus();

	return TRUE;
}

//��������
bool CDlgRegWizardAccount::SaveInputInfo()
{
	//��������
	CString strAccount;
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");

	//��������
	GetDlgItemText(IDC_ACCOUNTS,strAccount);
	GetDlgItemText(IDC_PASSWORD,m_szPassword,CountArray(m_szPassword));
	GetDlgItemText(IDC_PASSWORD2,szPassword,CountArray(szPassword));

	//Ч���û���
	strAccount.TrimLeft();
	strAccount.TrimRight();
	if (strAccount.IsEmpty())
	{
		ShowInformation(TEXT("�û��ʺŲ���Ϊ�գ����������룡"),MB_ICONERROR);
		m_edAccounts.SetFocus();
		return false;
	}
	lstrcpyn(m_szAccounts,strAccount,sizeof(m_szAccounts));

	//Ч������
	if (m_szPassword[0]==0)
	{
		ShowInformation(TEXT("��¼���벻��Ϊ�գ����������룡"),MB_ICONERROR);
		m_edPassword.SetFocus();
		return false;
	}
	if (lstrcmp(m_szPassword,szPassword)!=0)
	{
		ShowInformation(TEXT("��������ĵ�¼���벻��ͬ�����������룡"),MB_ICONERROR);
		m_edPassword2.SetFocus();
		return false;
	}

	return true;
}

//ͷ��ѡ��
VOID CDlgRegWizardAccount::OnBnClickedSelectFace()
{
	//�������
	if ((m_FaceSelectControl.GetInterface()==NULL)&&(m_FaceSelectControl.CreateInstance()==false))
	{
		ShowInformation(m_FaceSelectControl.GetErrorDescribe(),MB_ICONERROR);
		return;
	}

	//���ؽ���
	GetParent()->ShowWindow(SW_HIDE);

	//ѡ��ͷ��
	m_FaceSelectControl->SetSelectItemCount(MAX_NORMAL_FACE);
	if (m_FaceSelectControl->GetSelectFaceID(m_wFaceID)==true) m_FaceItemView.SetFaceItem(m_wFaceID);

	//��ʾ����
	GetParent()->ShowWindow(SW_SHOW);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRegWizardUserInfo::CDlgRegWizardUserInfo() : CDlgRegWizardItem(IDD_REG_WIZARD_USER_INFO)
{
	//���ñ���
	m_cbAge=0;
	m_cbGender=0;
	m_szArea[0]=0;
	m_szCity[0]=0;
	m_szProvince[0]=0;

	return;
}

//��������
CDlgRegWizardUserInfo::~CDlgRegWizardUserInfo()
{
}

//�ؼ���
VOID CDlgRegWizardUserInfo::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AGE, m_comAge);
	DDX_Control(pDX, IDC_AREA, m_comArea);
	DDX_Control(pDX, IDC_CITY, m_comCity);
	DDX_Control(pDX, IDC_GENDER, m_comGender);
	DDX_Control(pDX, IDC_PROVINCE, m_comProvince);
	DDX_Control(pDX, IDC_FIREND_CENTER, m_FirendCenter);
	DDX_Control(pDX, IDC_AREA_INFO_NOTE, m_AreaInfoNote);
}

//��ʼ������
BOOL CDlgRegWizardUserInfo::OnInitDialog()
{
	__super::OnInitDialog();

	//�������
	m_FirendCenter.SetHyperLinkUrl(szFirendCenter);
	m_AreaInfoNote.SetHyperLinkUrl(szAreaInfoNote);

	//�Ա���Ϣ
	BYTE cbGender[]={GENDER_UNKNOW,GENDER_FEMALE,GENDER_MANKIND};
	LPCTSTR szGenderDescribe[]={TEXT("����"),TEXT("Ů��"),TEXT("����")};
	for (INT i=0;i<CountArray(szGenderDescribe);i++)
	{
		ASSERT(i<CountArray(cbGender));
		m_comGender.SetItemData(m_comGender.AddString(szGenderDescribe[i]),cbGender[i]);
	}

	//������Ϣ
	BYTE cbAge[]={0,10,20,30,40,60};
	LPCTSTR szAgeDescribe[]={TEXT("����"),TEXT("15 ������"),TEXT("16 - 25 ��"),TEXT("26 - 35 ��"),TEXT("50 ������")};
	for (INT i=0;i<CountArray(szAgeDescribe);i++)
	{
		ASSERT(i<CountArray(cbAge));
		m_comAge.SetItemData(m_comAge.AddString(szAgeDescribe[i]),cbAge[i]);
	}

	//������Ϣ
	LoadProvinceInfo();

	return TRUE;
}

//��������
bool CDlgRegWizardUserInfo::SaveInputInfo()
{
	//�û�����
	INT nCurSel=m_comAge.GetCurSel();
	if (nCurSel==LB_ERR)
	{
		TCHAR szBuffer[16];
		m_comAge.GetWindowText(szBuffer,CountArray(szBuffer));
		m_cbAge=(BYTE)_tstoi(szBuffer);
	}
	else m_cbAge=(BYTE)m_comAge.GetItemData(nCurSel);

	//�û��Ա�
	nCurSel=m_comGender.GetCurSel();
	m_cbGender=(nCurSel!=LB_ERR)?(BYTE)m_comGender.GetItemData(nCurSel):GENDER_UNKNOW;

	//������Ϣ
	m_comArea.GetWindowText(m_szArea,CountArray(m_szArea));
	m_comCity.GetWindowText(m_szCity,CountArray(m_szCity));
	m_comProvince.GetWindowText(m_szProvince,CountArray(m_szProvince));
	
	return true;
}

//ʡ����Ϣ
VOID CDlgRegWizardUserInfo::LoadProvinceInfo()
{
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//��������
	TCHAR szAreaFile[MAX_PATH],szItem[32],szProvinceRead[CountArray(m_szProvince)];
	_sntprintf(szAreaFile,CountArray(szAreaFile),TEXT("%s\\AreaInfo.INI"),szDirectory);

	//��ȡ����
	m_comProvince.ResetContent();
	UINT nCount=GetPrivateProfileInt(TEXT("AREA_ROOT"),TEXT("COUNT"),0,szAreaFile);
	for (UINT i=0;i<nCount;i++)
	{
		szProvinceRead[0]=0;
		_sntprintf(szItem,CountArray(szItem),TEXT("Item%d"),i);
		GetPrivateProfileString(TEXT("AREA_ROOT"),szItem,TEXT(""),szProvinceRead,sizeof(szProvinceRead),szAreaFile);
		if ((szProvinceRead[0]!=0)&&(m_comProvince.FindString(0,szProvinceRead)==LB_ERR)) m_comProvince.AddString(szProvinceRead);
	}
	
	return;
}

//������Ϣ
VOID CDlgRegWizardUserInfo::LoadCityInfo(LPCTSTR pszProvince)
{
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//��������
	TCHAR szAreaFile[MAX_PATH],szItem[20],szCityRead[CountArray(m_szCity)];
	_sntprintf(szAreaFile,CountArray(szAreaFile),TEXT("%s\\AreaInfo.INI"),szDirectory);

	//��ȡ����
	m_comCity.ResetContent();
	UINT nCount=GetPrivateProfileInt(pszProvince,TEXT("COUNT"),0,szAreaFile);
	for (UINT i=0;i<nCount;i++)
	{
		szCityRead[0]=0;
		_sntprintf(szItem,CountArray(szItem),TEXT("Item%d"),i);
		GetPrivateProfileString(pszProvince,szItem,TEXT(""),szCityRead,sizeof(szCityRead),szAreaFile);
		if ((szCityRead[0]!=0)&&(m_comCity.FindString(0,szCityRead)==LB_ERR)) m_comCity.AddString(szCityRead);
	}

	return;
}

//������Ϣ
VOID CDlgRegWizardUserInfo::LoadAreaInfo(LPCTSTR pszCity)
{
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//��������
	TCHAR szAreaFile[MAX_PATH],szItem[20],szAreaRead[CountArray(m_szArea)];
	_sntprintf(szAreaFile,CountArray(szAreaFile),TEXT("%s\\AreaInfo.INI"),szDirectory);

	//��ȡ����
	m_comArea.ResetContent();
	UINT nCount=GetPrivateProfileInt(pszCity,TEXT("COUNT"),0,szAreaFile);
	for (UINT i=0;i<nCount;i++)
	{
		szAreaRead[0]=0;
		_sntprintf(szItem,CountArray(szItem),TEXT("Item%d"),i);
		GetPrivateProfileString(pszCity,szItem,TEXT(""),szAreaRead,sizeof(szAreaRead),szAreaFile);
		if ((szAreaRead[0]!=0)&&(m_comArea.FindString(0,szAreaRead)==LB_ERR)) m_comArea.AddString(szAreaRead);
	}

	return;
}

//ѡ�����
VOID CDlgRegWizardUserInfo::OnSelchangeCity() 
{
	//��ȡѡ��
	INT nIndex=m_comCity.GetCurSel();

	//���ݴ���
	if (nIndex!=CB_ERR) 
	{
		//��ȡ����
		CString strCity;
		m_comCity.GetLBText(nIndex,strCity);

		//������Ϣ
		LoadAreaInfo(strCity);
	}
	
	return;
}

//ѡ��ʡ��
VOID CDlgRegWizardUserInfo::OnSelchangeProvince() 
{
	//��ȡѡ��
	INT nIndex=m_comProvince.GetCurSel();

	//���ݴ���
	if (nIndex!=CB_ERR) 
	{
		//��ȡʡ��
		CString strProvince;
		m_comProvince.GetLBText(nIndex,strProvince);

		//������Ϣ
		LoadCityInfo(strProvince);
		OnSelchangeCity();
	}
	
	return;
}

//�滭����
VOID CDlgRegWizardUserInfo::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭λͼ
	CBitImage ImageBreviaryMap;
	ImageBreviaryMap.LoadFromResource(AfxGetInstanceHandle(),IDB_BREVIARY_MAP);
	ImageBreviaryMap.TransDrawImage(&dc,rcClient.Width()-ImageBreviaryMap.GetWidth()-15,35,RGB(255,0,255));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgRegWizard::CDlgRegWizard() : CSkinDialog(IDD_REG_WIZARD)
{
	//���ñ���
	m_wActiveIndex=0xFFFF;
	m_pWizardItem[0]=&m_WizardAccount;
	m_pWizardItem[1]=&m_WizardUserInfo;

	return;
}

//��������
CDlgRegWizard::~CDlgRegWizard()
{
}

//�ؼ���
VOID CDlgRegWizard::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAST, m_btLast);
	DDX_Control(pDX, IDC_NEXT, m_btNext);
	DDX_Control(pDX, IDC_FINISH, m_btFinish);
	DDX_Control(pDX, IDCANCEL, m_btReturn);
}

//��ʼ������
BOOL CDlgRegWizard::OnInitDialog()
{
	__super::OnInitDialog();

	//������
	ActiveWizardItem(0);

	return FALSE;
}

//ȷ������
VOID CDlgRegWizard::OnOK()
{
	if ((m_wActiveIndex+1)<CountArray(m_pWizardItem)) OnBnClickedNext();
	else OnBnClickedFinish();

	return;
}

//ȡ������
VOID CDlgRegWizard::OnCancel()
{
	//���ش���
	ShowWindow(SW_HIDE);

	//��ʾ��¼
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowLogon();
	
	return;
}

//������
bool CDlgRegWizard::ActiveWizardItem(WORD wIndex)
{
	//�ж�״̬
	if (m_wActiveIndex==wIndex)
	{
		m_pWizardItem[m_wActiveIndex]->SetFocus();
		return true;
	}

	//�������
	CDlgRegWizardItem * pItemWizard=NULL;
	if (m_wActiveIndex<CountArray(m_pWizardItem))
	{
		pItemWizard=m_pWizardItem[m_wActiveIndex];
		if (m_pWizardItem[m_wActiveIndex]->SaveInputInfo()==false) return false;
	}

	//��ȡλ��
	CRect rcItemRect;
	GetDlgItem(IDC_ITEM_FRAME)->GetWindowRect(rcItemRect);
	ScreenToClient(&rcItemRect);

	//��������
	m_wActiveIndex=wIndex;
	m_pWizardItem[m_wActiveIndex]->ShowWizardItem(rcItemRect,this);

	//��������
	m_pWizardItem[m_wActiveIndex]->SetFocus();
	if (pItemWizard!=NULL) pItemWizard->ShowWindow(SW_HIDE);

	//���½���
	m_btLast.EnableWindow((m_wActiveIndex==0)?FALSE:TRUE);
	m_btNext.EnableWindow(((m_wActiveIndex+1)<CountArray(m_pWizardItem))?TRUE:FALSE);
	m_btFinish.SetWindowText((m_btNext.IsWindowEnabled()==FALSE)?TEXT("�� ��"):TEXT("����ע��"));

	//�������
	TCHAR szTitle[128]=TEXT("");
	LPCTSTR pszWizardTitle=m_pWizardItem[m_wActiveIndex]->GetWizardTitle();
	_sntprintf(szTitle,CountArray(szTitle),TEXT("�ʺ�ע���� --- [ %s ]"),pszWizardTitle);
	
	//���ñ���
	SetWindowText(szTitle);

	return true;
}

//���͵�¼
WORD CDlgRegWizard::ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize)
{
	//��������
	/*TCHAR szPassword[LEN_MD5];
	CWHEncrypt::MD5Encrypt(m_WizardAccount.m_szPassword,szPassword);
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_RegisterAccounts),wBufferSize-sizeof(CMD_GP_RegisterAccounts));

	//��������
	CGlobalStation * pGlobalStation=CGlobalStation::GetInstance();
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;

	//������Ϣ
	CT2CW strPassword(szPassword);
	CT2CW strAccounts(m_WizardAccount.m_szAccounts);
	pRegisterAccounts->wFaceID=m_WizardAccount.m_wFaceID;
	pRegisterAccounts->dwStationID=pGlobalStation->GetStationID();
	pRegisterAccounts->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
	lstrcpynW(pRegisterAccounts->szPassword,strPassword,CountArray(pRegisterAccounts->szPassword));
	lstrcpynW(pRegisterAccounts->szAccounts,strAccounts,CountArray(pRegisterAccounts->szAccounts));

	//��������
	DTP_GP_UserInfo UserInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	CT2CW strArea(m_WizardUserInfo.m_szArea);
	CT2CW strCity(m_WizardUserInfo.m_szCity);
	CT2CW strProvince(m_WizardUserInfo.m_szProvince);

	//�û�����
	UserInfo.cbAge=m_WizardUserInfo.m_cbAge;
	UserInfo.cbGender=m_WizardUserInfo.m_cbGender;
	lstrcpynW(UserInfo.szArea,strArea,CountArray(UserInfo.szArea));
	lstrcpynW(UserInfo.szCity,strCity,CountArray(UserInfo.szCity));
	lstrcpynW(UserInfo.szProvince,strProvince,CountArray(UserInfo.szProvince));
	Packet.AddPacket(&UserInfo,sizeof(UserInfo),DTP_GP_USER_INFO);

	//��������
	return sizeof(CMD_GP_RegisterAccounts)+Packet.GetDataSize();*/

	return 0;
}

//��һ��
VOID CDlgRegWizard::OnBnClickedLast()
{
	//Ч�����
	ASSERT(m_wActiveIndex>0);
	ASSERT(m_wActiveIndex<CountArray(m_pWizardItem));

	//�л�ҳ��
	if (m_pWizardItem[m_wActiveIndex]->SaveInputInfo()==true) ActiveWizardItem(m_wActiveIndex-1);

	return;
}

//��һ��
VOID CDlgRegWizard::OnBnClickedNext()
{
	//Ч�����
	ASSERT((m_wActiveIndex+1)<CountArray(m_pWizardItem));

	//�л�ҳ��
	if (m_pWizardItem[m_wActiveIndex]->SaveInputInfo()==true) ActiveWizardItem(m_wActiveIndex+1);

	return;
}

//��ɰ�ť
VOID CDlgRegWizard::OnBnClickedFinish()
{
	//��������
	if (m_pWizardItem[m_wActiveIndex]->SaveInputInfo()==false) return;

	//���ش���
	ShowWindow(SW_HIDE);

	//ִ��ע��
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(true);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
