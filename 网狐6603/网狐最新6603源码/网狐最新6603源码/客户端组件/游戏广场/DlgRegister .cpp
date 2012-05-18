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

//构造函数
CDlgRegWizardItem::CDlgRegWizardItem(UINT nIDTemplate) : CDialog(nIDTemplate)
{
}

//析构函数
CDlgRegWizardItem::~CDlgRegWizardItem()
{
}

//初始化函数
BOOL CDlgRegWizardItem::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return TRUE;
}

//创建向导
bool CDlgRegWizardItem::ShowWizardItem(const CRect & rcRect, CWnd * pParentWnd)
{
	//创建窗口
	if (m_hWnd==NULL) 
	{
		Create(m_lpszTemplateName,pParentWnd);
		SetWindowPos(NULL,rcRect.left,rcRect.top,rcRect.Width(),rcRect.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	//显示窗口
	ShowWindow(SW_SHOW);

	return true;
}

//绘画背景
BOOL CDlgRegWizardItem::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画背景
	pDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);

	return TRUE;
}

//背景颜色
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

//确定函数
VOID CDlgRegWizardItem::OnOK() 
{ 
	//投递消息
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDOK,0),0);

	return;
}

//取消消息
VOID CDlgRegWizardItem::OnCancel() 
{ 
	//投递消息
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,0),0);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgRegWizardAccount::CDlgRegWizardAccount() : CDlgRegWizardItem(IDD_REG_WIZARD_ACCOUNT)
{
	//设置变量
	m_wFaceID=0;
	m_szAccounts[0]=0;
	m_szPassword[0]=0;

	return;
}

//析构函数
CDlgRegWizardAccount::~CDlgRegWizardAccount()
{
}

//控件绑定
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

//初始化函数
BOOL CDlgRegWizardAccount::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_wFaceID=rand()%MAX_NORMAL_FACE;

	//限制输入
	m_edAccounts.LimitText(LEN_ACCOUNTS-1);
	m_edPassword.LimitText(LEN_PASSWORD-1);
	m_edPassword2.LimitText(LEN_PASSWORD-1);

	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//设置连接
	TCHAR szBrowseUrl[MAX_PATH]=TEXT("");
	CGlobalStation * pGlobalStation=CGlobalStation::GetInstance();
	m_GameHomePage.SetHyperLinkUrl(pGlobalStation->GetStationHomeLink());
	m_ServiceArticle.SetHyperLinkUrl(pGlobalStation->GetNativeIniLink(LINK_ID_SERVICE_ARTICLE,szBrowseUrl,CountArray(szBrowseUrl)));

	//设置接口
	m_FaceItemView.SetFaceControl(theApp.m_FaceControlModule.GetInterface());

	//设置数据
	m_FaceItemView.SetFaceItem(m_wFaceID);
	SetDlgItemText(IDC_ACCOUNTS,m_szAccounts);
	SetDlgItemText(IDC_PASSWORD,m_szPassword);
	SetDlgItemText(IDC_PASSWORD2,m_szPassword);

	//记住密码
	if (pParameterGlobal->m_AcountsRule==AcountsRule_AccountsAndPass)
	{
		((CButton *)GetDlgItem(IDC_REM_PASSWORD))->SetCheck(BST_CHECKED);
	}

	//设置焦点
	m_edAccounts.SetFocus();

	return TRUE;
}

//保存输入
bool CDlgRegWizardAccount::SaveInputInfo()
{
	//变量定义
	CString strAccount;
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");

	//保存输入
	GetDlgItemText(IDC_ACCOUNTS,strAccount);
	GetDlgItemText(IDC_PASSWORD,m_szPassword,CountArray(m_szPassword));
	GetDlgItemText(IDC_PASSWORD2,szPassword,CountArray(szPassword));

	//效验用户名
	strAccount.TrimLeft();
	strAccount.TrimRight();
	if (strAccount.IsEmpty())
	{
		ShowInformation(TEXT("用户帐号不能为空，请重新输入！"),MB_ICONERROR);
		m_edAccounts.SetFocus();
		return false;
	}
	lstrcpyn(m_szAccounts,strAccount,sizeof(m_szAccounts));

	//效验密码
	if (m_szPassword[0]==0)
	{
		ShowInformation(TEXT("登录密码不能为空，请重新输入！"),MB_ICONERROR);
		m_edPassword.SetFocus();
		return false;
	}
	if (lstrcmp(m_szPassword,szPassword)!=0)
	{
		ShowInformation(TEXT("两次输入的登录密码不相同，请重新输入！"),MB_ICONERROR);
		m_edPassword2.SetFocus();
		return false;
	}

	return true;
}

//头像选择
VOID CDlgRegWizardAccount::OnBnClickedSelectFace()
{
	//创建组件
	if ((m_FaceSelectControl.GetInterface()==NULL)&&(m_FaceSelectControl.CreateInstance()==false))
	{
		ShowInformation(m_FaceSelectControl.GetErrorDescribe(),MB_ICONERROR);
		return;
	}

	//隐藏界面
	GetParent()->ShowWindow(SW_HIDE);

	//选择头像
	m_FaceSelectControl->SetSelectItemCount(MAX_NORMAL_FACE);
	if (m_FaceSelectControl->GetSelectFaceID(m_wFaceID)==true) m_FaceItemView.SetFaceItem(m_wFaceID);

	//显示界面
	GetParent()->ShowWindow(SW_SHOW);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgRegWizardUserInfo::CDlgRegWizardUserInfo() : CDlgRegWizardItem(IDD_REG_WIZARD_USER_INFO)
{
	//设置变量
	m_cbAge=0;
	m_cbGender=0;
	m_szArea[0]=0;
	m_szCity[0]=0;
	m_szProvince[0]=0;

	return;
}

//析构函数
CDlgRegWizardUserInfo::~CDlgRegWizardUserInfo()
{
}

//控件绑定
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

//初始化函数
BOOL CDlgRegWizardUserInfo::OnInitDialog()
{
	__super::OnInitDialog();

	//设置组件
	m_FirendCenter.SetHyperLinkUrl(szFirendCenter);
	m_AreaInfoNote.SetHyperLinkUrl(szAreaInfoNote);

	//性别信息
	BYTE cbGender[]={GENDER_UNKNOW,GENDER_FEMALE,GENDER_MANKIND};
	LPCTSTR szGenderDescribe[]={TEXT("保密"),TEXT("女性"),TEXT("男性")};
	for (INT i=0;i<CountArray(szGenderDescribe);i++)
	{
		ASSERT(i<CountArray(cbGender));
		m_comGender.SetItemData(m_comGender.AddString(szGenderDescribe[i]),cbGender[i]);
	}

	//年龄信息
	BYTE cbAge[]={0,10,20,30,40,60};
	LPCTSTR szAgeDescribe[]={TEXT("保密"),TEXT("15 岁以下"),TEXT("16 - 25 岁"),TEXT("26 - 35 岁"),TEXT("50 岁以上")};
	for (INT i=0;i<CountArray(szAgeDescribe);i++)
	{
		ASSERT(i<CountArray(cbAge));
		m_comAge.SetItemData(m_comAge.AddString(szAgeDescribe[i]),cbAge[i]);
	}

	//加载信息
	LoadProvinceInfo();

	return TRUE;
}

//保存输入
bool CDlgRegWizardUserInfo::SaveInputInfo()
{
	//用户年龄
	INT nCurSel=m_comAge.GetCurSel();
	if (nCurSel==LB_ERR)
	{
		TCHAR szBuffer[16];
		m_comAge.GetWindowText(szBuffer,CountArray(szBuffer));
		m_cbAge=(BYTE)_tstoi(szBuffer);
	}
	else m_cbAge=(BYTE)m_comAge.GetItemData(nCurSel);

	//用户性别
	nCurSel=m_comGender.GetCurSel();
	m_cbGender=(nCurSel!=LB_ERR)?(BYTE)m_comGender.GetItemData(nCurSel):GENDER_UNKNOW;

	//区域信息
	m_comArea.GetWindowText(m_szArea,CountArray(m_szArea));
	m_comCity.GetWindowText(m_szCity,CountArray(m_szCity));
	m_comProvince.GetWindowText(m_szProvince,CountArray(m_szProvince));
	
	return true;
}

//省份信息
VOID CDlgRegWizardUserInfo::LoadProvinceInfo()
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//变量定义
	TCHAR szAreaFile[MAX_PATH],szItem[32],szProvinceRead[CountArray(m_szProvince)];
	_sntprintf(szAreaFile,CountArray(szAreaFile),TEXT("%s\\AreaInfo.INI"),szDirectory);

	//读取资料
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

//城市信息
VOID CDlgRegWizardUserInfo::LoadCityInfo(LPCTSTR pszProvince)
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//变量定义
	TCHAR szAreaFile[MAX_PATH],szItem[20],szCityRead[CountArray(m_szCity)];
	_sntprintf(szAreaFile,CountArray(szAreaFile),TEXT("%s\\AreaInfo.INI"),szDirectory);

	//读取资料
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

//地区信息
VOID CDlgRegWizardUserInfo::LoadAreaInfo(LPCTSTR pszCity)
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//变量定义
	TCHAR szAreaFile[MAX_PATH],szItem[20],szAreaRead[CountArray(m_szArea)];
	_sntprintf(szAreaFile,CountArray(szAreaFile),TEXT("%s\\AreaInfo.INI"),szDirectory);

	//读取资料
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

//选择城市
VOID CDlgRegWizardUserInfo::OnSelchangeCity() 
{
	//获取选择
	INT nIndex=m_comCity.GetCurSel();

	//数据处理
	if (nIndex!=CB_ERR) 
	{
		//获取城市
		CString strCity;
		m_comCity.GetLBText(nIndex,strCity);

		//更新信息
		LoadAreaInfo(strCity);
	}
	
	return;
}

//选择省份
VOID CDlgRegWizardUserInfo::OnSelchangeProvince() 
{
	//获取选择
	INT nIndex=m_comProvince.GetCurSel();

	//数据处理
	if (nIndex!=CB_ERR) 
	{
		//获取省份
		CString strProvince;
		m_comProvince.GetLBText(nIndex,strProvince);

		//更新信息
		LoadCityInfo(strProvince);
		OnSelchangeCity();
	}
	
	return;
}

//绘画函数
VOID CDlgRegWizardUserInfo::OnPaint()
{
	CPaintDC dc(this);

	//绘画背景
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画位图
	CBitImage ImageBreviaryMap;
	ImageBreviaryMap.LoadFromResource(AfxGetInstanceHandle(),IDB_BREVIARY_MAP);
	ImageBreviaryMap.TransDrawImage(&dc,rcClient.Width()-ImageBreviaryMap.GetWidth()-15,35,RGB(255,0,255));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgRegWizard::CDlgRegWizard() : CSkinDialog(IDD_REG_WIZARD)
{
	//设置变量
	m_wActiveIndex=0xFFFF;
	m_pWizardItem[0]=&m_WizardAccount;
	m_pWizardItem[1]=&m_WizardUserInfo;

	return;
}

//析构函数
CDlgRegWizard::~CDlgRegWizard()
{
}

//控件绑定
VOID CDlgRegWizard::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAST, m_btLast);
	DDX_Control(pDX, IDC_NEXT, m_btNext);
	DDX_Control(pDX, IDC_FINISH, m_btFinish);
	DDX_Control(pDX, IDCANCEL, m_btReturn);
}

//初始化函数
BOOL CDlgRegWizard::OnInitDialog()
{
	__super::OnInitDialog();

	//激活向导
	ActiveWizardItem(0);

	return FALSE;
}

//确定函数
VOID CDlgRegWizard::OnOK()
{
	if ((m_wActiveIndex+1)<CountArray(m_pWizardItem)) OnBnClickedNext();
	else OnBnClickedFinish();

	return;
}

//取消函数
VOID CDlgRegWizard::OnCancel()
{
	//隐藏窗口
	ShowWindow(SW_HIDE);

	//显示登录
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowLogon();
	
	return;
}

//激活向导
bool CDlgRegWizard::ActiveWizardItem(WORD wIndex)
{
	//判断状态
	if (m_wActiveIndex==wIndex)
	{
		m_pWizardItem[m_wActiveIndex]->SetFocus();
		return true;
	}

	//保存旧项
	CDlgRegWizardItem * pItemWizard=NULL;
	if (m_wActiveIndex<CountArray(m_pWizardItem))
	{
		pItemWizard=m_pWizardItem[m_wActiveIndex];
		if (m_pWizardItem[m_wActiveIndex]->SaveInputInfo()==false) return false;
	}

	//获取位置
	CRect rcItemRect;
	GetDlgItem(IDC_ITEM_FRAME)->GetWindowRect(rcItemRect);
	ScreenToClient(&rcItemRect);

	//创建新项
	m_wActiveIndex=wIndex;
	m_pWizardItem[m_wActiveIndex]->ShowWizardItem(rcItemRect,this);

	//激活新项
	m_pWizardItem[m_wActiveIndex]->SetFocus();
	if (pItemWizard!=NULL) pItemWizard->ShowWindow(SW_HIDE);

	//更新界面
	m_btLast.EnableWindow((m_wActiveIndex==0)?FALSE:TRUE);
	m_btNext.EnableWindow(((m_wActiveIndex+1)<CountArray(m_pWizardItem))?TRUE:FALSE);
	m_btFinish.SetWindowText((m_btNext.IsWindowEnabled()==FALSE)?TEXT("完 成"):TEXT("立即注册"));

	//构造标题
	TCHAR szTitle[128]=TEXT("");
	LPCTSTR pszWizardTitle=m_pWizardItem[m_wActiveIndex]->GetWizardTitle();
	_sntprintf(szTitle,CountArray(szTitle),TEXT("帐号注册向导 --- [ %s ]"),pszWizardTitle);
	
	//设置标题
	SetWindowText(szTitle);

	return true;
}

//发送登录
WORD CDlgRegWizard::ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize)
{
	//变量定义
	/*TCHAR szPassword[LEN_MD5];
	CWHEncrypt::MD5Encrypt(m_WizardAccount.m_szPassword,szPassword);
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_RegisterAccounts),wBufferSize-sizeof(CMD_GP_RegisterAccounts));

	//变量定义
	CGlobalStation * pGlobalStation=CGlobalStation::GetInstance();
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;

	//基本信息
	CT2CW strPassword(szPassword);
	CT2CW strAccounts(m_WizardAccount.m_szAccounts);
	pRegisterAccounts->wFaceID=m_WizardAccount.m_wFaceID;
	pRegisterAccounts->dwStationID=pGlobalStation->GetStationID();
	pRegisterAccounts->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
	lstrcpynW(pRegisterAccounts->szPassword,strPassword,CountArray(pRegisterAccounts->szPassword));
	lstrcpynW(pRegisterAccounts->szAccounts,strAccounts,CountArray(pRegisterAccounts->szAccounts));

	//变量定义
	DTP_GP_UserInfo UserInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	CT2CW strArea(m_WizardUserInfo.m_szArea);
	CT2CW strCity(m_WizardUserInfo.m_szCity);
	CT2CW strProvince(m_WizardUserInfo.m_szProvince);

	//用户资料
	UserInfo.cbAge=m_WizardUserInfo.m_cbAge;
	UserInfo.cbGender=m_WizardUserInfo.m_cbGender;
	lstrcpynW(UserInfo.szArea,strArea,CountArray(UserInfo.szArea));
	lstrcpynW(UserInfo.szCity,strCity,CountArray(UserInfo.szCity));
	lstrcpynW(UserInfo.szProvince,strProvince,CountArray(UserInfo.szProvince));
	Packet.AddPacket(&UserInfo,sizeof(UserInfo),DTP_GP_USER_INFO);

	//发送数据
	return sizeof(CMD_GP_RegisterAccounts)+Packet.GetDataSize();*/

	return 0;
}

//上一步
VOID CDlgRegWizard::OnBnClickedLast()
{
	//效验参数
	ASSERT(m_wActiveIndex>0);
	ASSERT(m_wActiveIndex<CountArray(m_pWizardItem));

	//切换页面
	if (m_pWizardItem[m_wActiveIndex]->SaveInputInfo()==true) ActiveWizardItem(m_wActiveIndex-1);

	return;
}

//下一步
VOID CDlgRegWizard::OnBnClickedNext()
{
	//效验参数
	ASSERT((m_wActiveIndex+1)<CountArray(m_pWizardItem));

	//切换页面
	if (m_pWizardItem[m_wActiveIndex]->SaveInputInfo()==true) ActiveWizardItem(m_wActiveIndex+1);

	return;
}

//完成按钮
VOID CDlgRegWizard::OnBnClickedFinish()
{
	//保存设置
	if (m_pWizardItem[m_wActiveIndex]->SaveInputInfo()==false) return;

	//隐藏窗口
	ShowWindow(SW_HIDE);

	//执行注册
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(true);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
