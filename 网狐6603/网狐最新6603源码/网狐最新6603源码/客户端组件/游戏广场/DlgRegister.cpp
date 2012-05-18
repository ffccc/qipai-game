#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgRegister.h"
#include "MissionLogon.h"

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					7									//圆角宽度
#define ROUND_CY					7									//圆角高度

//屏幕位置
#define LAYERED_SIZE				5									//分层大小
#define CAPTION_SIZE				35									//标题大小

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgRegister, CDialog)

	//系统消息
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()

	//按钮消息
	ON_BN_CLICKED(IDC_QUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDC_SELECT_FACE, OnBnClickedSelectFace)

	//特殊消息
	ON_EN_CHANGE(IDC_LOGON_PASS1, OnEnChangeLogonPass)
	ON_EN_CHANGE(IDC_INSURE_PASS1, OnEnChangeInsurePass)
	ON_STN_CLICKED(IDC_CHANGE_VALIDATE, OnBnClickedChangeValidate)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgRegister::CDlgRegister() : CDialog(IDD_DLG_REGISTER)
{
	//设置变量
	m_wFaceID=0;
	m_cbGender=0;
	m_szAccounts[0]=0;
	m_szNickName[0]=0;
	m_szSpreader[0]=0;
	m_szLogonPass[0]=0;
	m_szInsurePass[0]=0;
	m_szPassPortID[0]=0;
	m_szCompellation[0]=0;

	//设置变量
	m_cbRemPassword=FALSE;
	m_cbLogonPassLevel=PASSWORD_LEVEL_0;
	m_cbInsurePassLevel=PASSWORD_LEVEL_0;

	//创建画刷
	m_brBrush.CreateSolidBrush(RGB(215,223,228));

	return;
}

//析构函数
CDlgRegister::~CDlgRegister()
{
}

//控件绑定
VOID CDlgRegister::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//按钮控件
	DDX_Control(pDX, IDOK, m_btEnter);
	DDX_Control(pDX, IDC_QUIT, m_btQuit);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_SELECT_FACE, m_btSelectFace);

	//编辑控件
	DDX_Control(pDX, IDC_ACCOUNTS, m_edAccounts);
	DDX_Control(pDX, IDC_SPREADER, m_edSpreader);
	DDX_Control(pDX, IDC_NICKNAME, m_edNickName);
	DDX_Control(pDX, IDC_PASSPORT_ID, m_edPassPortID);
	DDX_Control(pDX, IDC_LOGON_PASS1, m_edLogonPass1);
	DDX_Control(pDX, IDC_LOGON_PASS2, m_edLogonPass2);
	DDX_Control(pDX, IDC_INSURE_PASS1, m_edInsurePass1);
	DDX_Control(pDX, IDC_INSURE_PASS2, m_edInsurePass2);
	DDX_Control(pDX, IDC_COMPELLATION, m_edCompellation);
	DDX_Control(pDX, IDC_VALIDATE_CODE, m_edValidateCode);

	//其他控件
	DDX_Control(pDX, IDC_FACE_ITEM, m_FaceItemView);
	DDX_Control(pDX, IDC_CHANGE_VALIDATE, m_ChangeValidate);
	DDX_Control(pDX, IDC_VALIDATE_CODE_WND, m_WndValidateCode);
}

//消息解释
BOOL CDlgRegister::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->message==WM_CHAR)
	{
		//密码过滤
		if ((m_edLogonPass1.m_hWnd==pMsg->hwnd)&&(pMsg->wParam>=0x80)) return TRUE;
		if ((m_edLogonPass2.m_hWnd==pMsg->hwnd)&&(pMsg->wParam>=0x80)) return TRUE;
		if ((m_edInsurePass1.m_hWnd==pMsg->hwnd)&&(pMsg->wParam>=0x80)) return TRUE;
		if ((m_edInsurePass2.m_hWnd==pMsg->hwnd)&&(pMsg->wParam>=0x80)) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgRegister::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("用户注册"));

	//变量定义
	ASSERT(CFaceItemControl::GetInstance()!=NULL);
	IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_REGISTER_BACK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//验证控件
	m_WndValidateCode.RandValidateCode();
	m_edValidateCode.LimitText(VALIDATE_COUNT);
	m_ChangeValidate.SetBackGroundColor(RGB(215,223,238));

	//限制输入
	m_edAccounts.LimitText(LEN_ACCOUNTS-1);
	m_edNickName.LimitText(LEN_NICKNAME-1);
	m_edSpreader.LimitText(LEN_ACCOUNTS-1);
	m_edLogonPass1.LimitText(LEN_PASSWORD-1);
	m_edLogonPass2.LimitText(LEN_PASSWORD-1);
	m_edInsurePass1.LimitText(LEN_PASSWORD-1);
	m_edInsurePass2.LimitText(LEN_PASSWORD-1);
	m_edPassPortID.LimitText(LEN_PASS_PORT_ID-1);
	m_edCompellation.LimitText(LEN_COMPELLATION-1);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btCancel.SetButtonImage(IDB_BT_REGISTER_CLOSE,hInstance,false,false);
	m_btQuit.SetButtonImage(IDB_BT_REGISTER,TEXT("BT_REGISTER_QUIT"),hInstance,false,false);
	m_btEnter.SetButtonImage(IDB_BT_REGISTER,TEXT("BT_REGISTER_ENTER"),hInstance,false,false);

	//获取头像
	m_wFaceID=rand()%pIFaceItemControl->GetFaceCount();

	//设置头像
	m_FaceItemView.SetSystemFace(m_wFaceID);

	//设置性别
	((CButton *)GetDlgItem(IDC_GENDER_MANKIND))->SetCheck(BST_CHECKED);

	//居中窗口
	CenterWindow(this);

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=LAYERED_SIZE;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(LAYERED_SIZE,LAYERED_SIZE,SizeWindow.cx-LAYERED_SIZE+1,SizeWindow.cy-LAYERED_SIZE+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return FALSE;
}

//确定函数
VOID CDlgRegister::OnOK()
{
	//获取信息
	if (GetInformation()==false) return;

	//隐藏窗口
	ShowWindow(SW_HIDE);

	//执行注册
	CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
	if (pMissionLogon!=NULL) pMissionLogon->PerformLogonMission(m_cbRemPassword==TRUE);

	return;
}

//取消函数
VOID CDlgRegister::OnCancel()
{
	//隐藏窗口
	ShowWindow(SW_HIDE);

	//显示登录
	ASSERT(CMissionLogon::GetInstance()!=NULL);
	if (CMissionLogon::GetInstance()!=NULL) CMissionLogon::GetInstance()->ShowLogon();
	
	return;
}

//发送登录
WORD CDlgRegister::ConstructRegisterPacket(BYTE cbBuffer[], WORD wBufferSize)
{
	//变量定义
	TCHAR szLogonPass[LEN_MD5];
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(m_szLogonPass,szLogonPass);
	CWHEncrypt::MD5Encrypt(m_szInsurePass,szInsurePass);

	//变量定义
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)cbBuffer;

	//系统信息
	pRegisterAccounts->dwPlazaVersion=VERSION_PLAZA;

	//机器标识
	CWHService::GetMachineID(pRegisterAccounts->szMachineID);

	//基本信息
	pRegisterAccounts->wFaceID=m_wFaceID;
	pRegisterAccounts->cbGender=m_cbGender;
	lstrcpyn(pRegisterAccounts->szAccounts,m_szAccounts,CountArray(pRegisterAccounts->szAccounts));
	lstrcpyn(pRegisterAccounts->szNickName,m_szNickName,CountArray(pRegisterAccounts->szNickName));
	lstrcpyn(pRegisterAccounts->szSpreader,m_szSpreader,CountArray(pRegisterAccounts->szSpreader));
	lstrcpyn(pRegisterAccounts->szLogonPass,szLogonPass,CountArray(pRegisterAccounts->szLogonPass));
	lstrcpyn(pRegisterAccounts->szInsurePass,szInsurePass,CountArray(pRegisterAccounts->szInsurePass));
	lstrcpyn(pRegisterAccounts->szPassPortID,m_szPassPortID,CountArray(pRegisterAccounts->szPassPortID));
	lstrcpyn(pRegisterAccounts->szCompellation,m_szCompellation,CountArray(pRegisterAccounts->szCompellation));

	//保存密码
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	lstrcpyn(pGlobalUserData->szPassword,pRegisterAccounts->szLogonPass,CountArray(pGlobalUserData->szPassword));

	//发送数据
	return sizeof(CMD_GP_RegisterAccounts);
}

//获取信息
bool CDlgRegister::GetInformation()
{
	//验证信息
	TCHAR szValidateCode[VALIDATE_COUNT+1]=TEXT("");
	GetDlgItemText(IDC_VALIDATE_CODE,szValidateCode,CountArray(szValidateCode));

	//密码信息
	TCHAR szLogonPass2[LEN_PASSWORD]=TEXT("");
	TCHAR szInsurePass2[LEN_PASSWORD]=TEXT("");
	GetDlgItemText(IDC_LOGON_PASS2,szLogonPass2,CountArray(szLogonPass2));
	GetDlgItemText(IDC_LOGON_PASS1,m_szLogonPass,CountArray(m_szLogonPass));
	GetDlgItemText(IDC_INSURE_PASS2,szInsurePass2,CountArray(szInsurePass2));
	GetDlgItemText(IDC_INSURE_PASS1,m_szInsurePass,CountArray(m_szInsurePass));

	//获取字符
	GetControlString(IDC_ACCOUNTS,m_szAccounts,CountArray(m_szAccounts));
	GetControlString(IDC_NICKNAME,m_szNickName,CountArray(m_szNickName));
	GetControlString(IDC_SPREADER,m_szSpreader,CountArray(m_szSpreader));
	GetControlString(IDC_SPREADER,m_szSpreader,CountArray(m_szSpreader));
	GetControlString(IDC_PASSPORT_ID,m_szPassPortID,CountArray(m_szPassPortID));
	GetControlString(IDC_COMPELLATION,m_szCompellation,CountArray(m_szCompellation));

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//帐号判断
	if (pUserItemElement->EfficacyAccounts(m_szAccounts,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		m_edAccounts.SetFocus();

		return false;
	}

	//昵称判断
	if (pUserItemElement->EfficacyNickName(m_szNickName,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		m_edNickName.SetFocus();

		return false;
	}

	//密码判断
	if (pUserItemElement->EfficacyPassword(m_szLogonPass,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		m_edLogonPass1.SetFocus();

		return false;
	}

	//密码判断
	if (lstrcmp(m_szLogonPass,szLogonPass2)!=0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您两次输入的帐号密码不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_edLogonPass1.SetFocus();

		return false;
	}

	//密码判断
	if (pUserItemElement->EfficacyPassword(m_szInsurePass,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		m_edInsurePass1.SetFocus();

		return false;
	}

	//密码判断
	if (lstrcmp(m_szInsurePass,szInsurePass2)!=0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您两次输入的保险柜密码不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_edInsurePass1.SetFocus();

		return false;
	}

	//身份证号
	if (pUserItemElement->EfficacyPassPortID(m_szPassPortID,szDescribe,CountArray(szDescribe))==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		m_edPassPortID.SetFocus();

		return false;
	}

	//效验判断
	if (m_WndValidateCode.CheckValidateCode(szValidateCode)==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("效验码效验验证失败，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_edValidateCode.SetFocus();

		return false;
	}

	//用户性别
	if (((CButton *)GetDlgItem(IDC_GENDER_FEMALE))->GetCheck()==BST_CHECKED) m_cbGender=GENDER_FEMALE;
	if (((CButton *)GetDlgItem(IDC_GENDER_MANKIND))->GetCheck()==BST_CHECKED) m_cbGender=GENDER_MANKIND;

	//配置信息
	m_cbRemPassword=(((CButton *)GetDlgItem(IDC_REM_PASSWORD))->GetCheck()==BST_CHECKED);

	return true;
}

//获取字符
VOID CDlgRegister::GetControlString(UINT uControlID, TCHAR szString[], WORD wMaxCount)
{
	//获取字符
	CString strString;
	GetDlgItemText(uControlID,strString);

	//去掉空格
	strString.TrimLeft();
	strString.TrimRight();

	//拷贝字符
	lstrcpyn(szString,strString,wMaxCount);

	return;
}

//关闭按钮
VOID CDlgRegister::OnBnClickedQuit()
{
	//关闭窗口
	OnCancel();

	return;
}

//密码输入
VOID CDlgRegister::OnEnChangeLogonPass()
{
	//获取密码
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	GetDlgItemText(IDC_LOGON_PASS1,szPassword,CountArray(szPassword));

	//等级判断
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//更新窗口
	if (m_cbLogonPassLevel!=cbPasswordLevel)
	{
		//设置变量
		m_cbLogonPassLevel=cbPasswordLevel;

		//更新窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//密码输入
VOID CDlgRegister::OnEnChangeInsurePass()
{
	//获取密码
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	GetDlgItemText(IDC_INSURE_PASS1,szPassword,CountArray(szPassword));

	//等级判断
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//更新窗口
	if (m_cbInsurePassLevel!=cbPasswordLevel)
	{
		//设置变量
		m_cbInsurePassLevel=cbPasswordLevel;

		//更新窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//头像选择
VOID CDlgRegister::OnBnClickedSelectFace()
{
	//创建组件
	if ((m_FaceSelectControl.GetInterface()==NULL)&&(m_FaceSelectControl.CreateInstance()==false))
	{
		CInformation Information(this);
		Information.ShowMessageBox(m_FaceSelectControl.GetErrorDescribe(),MB_ICONERROR);
		return;
	}

	//隐藏界面
	ShowWindow(SW_HIDE);

	//设置控件
	m_FaceSelectControl->SetAllowItemFull();

	//选择头像
	if (m_FaceSelectControl->GetSelectFaceID(m_wFaceID)==true)
	{
		m_FaceItemView.SetSystemFace(m_wFaceID);
	}

	//显示界面
	ShowWindow(SW_SHOW);

	return;
}

//更换验证
VOID CDlgRegister::OnBnClickedChangeValidate()
{
	//更换验证
	m_WndValidateCode.RandValidateCode();

	return;
}

//绘画背景
BOOL CDlgRegister::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置缓冲
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//加载资源
	CPngImage ImageBack;
	CPngImage ImageTitle;
	ImageTitle.LoadImage(AfxGetInstanceHandle(),TEXT("REGISTER_TITLE"));
	ImageBack.LoadImage(AfxGetInstanceHandle(),TEXT("DLG_REGISTER_BACK"));

	//加载图标
	CPngImage ImageLogo;
	ImageLogo.LoadImage(GetModuleHandle(SKIN_CONTROL_DLL_NAME),TEXT("SKIN_WND_LOGO"));

	//绘画背景
	ImageBack.DrawImage(pBufferDC,0,0);
	ImageLogo.DrawImage(pBufferDC,11,6);
	ImageTitle.DrawImage(pBufferDC,40,6);

	//加载资源
	CPngImage ImagePassword;
	ImagePassword.LoadImage(AfxGetInstanceHandle(),TEXT("PASSWORD_LEVEL"));

	//获取大小
	CSize SizePassword;
	SizePassword.SetSize(ImagePassword.GetWidth()/3,ImagePassword.GetHeight()/2);

	//绘画等级
	ImagePassword.DrawImage(pBufferDC,249,258,SizePassword.cx*3,SizePassword.cy,0,0);
	ImagePassword.DrawImage(pBufferDC,249,311,SizePassword.cx*3,SizePassword.cy,0,0);

	//绘画叠加
	if (m_cbLogonPassLevel>=PASSWORD_LEVEL_1)
	{
		INT nImagePos=(m_cbLogonPassLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
		ImagePassword.DrawImage(pBufferDC,249+nImagePos,258,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	}

	//绘画叠加
	if (m_cbInsurePassLevel>=PASSWORD_LEVEL_1)
	{
		INT nImagePos=(m_cbInsurePassLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
		ImagePassword.DrawImage(pBufferDC,249+nImagePos,311,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//显示消息
VOID CDlgRegister::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//显示分层
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

//鼠标消息
VOID CDlgRegister::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟标题
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//位置改变
VOID CDlgRegister::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
	__super::OnWindowPosChanging(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}

	return;
}

//控件颜色
HBRUSH CDlgRegister::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
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
