#include "StdAfx.h"
#include "GlobalUnits.h"
#include "DlgServicePassword.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgServicePassword, CDlgServiceItem)
	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_LOGON_PASSWORD2, OnEnChangeLogonPassword)
	ON_EN_CHANGE(IDC_INSURE_PASSWORD2, OnEnChangeInserePassword)
	ON_BN_CLICKED(IDC_MODIFY_LOGON_PASSWORD, OnBnClickedLogonPassword)
	ON_BN_CLICKED(IDC_MODIFY_INSURE_PASSWORD, OnBnClickedInsurePassword)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgServicePassword::CDlgServicePassword() : CDlgServiceItem(IDD_DLG_SERVICE_PASSWORD)
{
	//设置图标
	m_cbImageIndex=1;

	//设置任务
	m_MissionManager.InsertMissionItem(this);

	//设置变量
	m_cbLogonPasswordLevel=PASSWORD_LEVEL_0;
	m_cbInsurePasswordLevel=PASSWORD_LEVEL_0;

	//设置变量
	m_bLogonMission=false;
	ZeroMemory(m_szSrcPassword,sizeof(m_szSrcPassword));
	ZeroMemory(m_szDesPassword,sizeof(m_szDesPassword));

	return;
}

//析构函数
CDlgServicePassword::~CDlgServicePassword()
{
}

//控件绑定
VOID CDlgServicePassword::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//帐号密码
	DDX_Control(pDX, IDC_LOGON_PASSWORD1, m_edLogonPassword1);
	DDX_Control(pDX, IDC_LOGON_PASSWORD2, m_edLogonPassword2);
	DDX_Control(pDX, IDC_LOGON_PASSWORD3, m_edLogonPassword3);

	//二级密码
	DDX_Control(pDX, IDC_INSURE_PASSWORD1, m_edInsurePassword1);
	DDX_Control(pDX, IDC_INSURE_PASSWORD2, m_edInsurePassword2);
	DDX_Control(pDX, IDC_INSURE_PASSWORD3, m_edInsurePassword3);

	//按钮变量
	DDX_Control(pDX, IDC_MODIFY_LOGON_PASSWORD, m_btLogonPassword);
	DDX_Control(pDX, IDC_MODIFY_INSURE_PASSWORD, m_btInsurePassword);
}

//消息解释
BOOL CDlgServicePassword::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//变量定义
		CWnd * pWndFouns=GetFocus();
		TCHAR szPassword[LEN_PASSWORD]=TEXT("");

		//焦点判断
		if (m_edLogonPassword1.IsChild(pWndFouns))
		{
			m_edLogonPassword1.GetUserPassword(szPassword);
			if (szPassword[0]!=0) m_edLogonPassword2.SetFocus();

			return TRUE;
		}

		//焦点判断
		if (m_edLogonPassword2.IsChild(pWndFouns))
		{
			m_edLogonPassword2.GetUserPassword(szPassword);
			if (szPassword[0]!=0) m_edLogonPassword3.SetFocus();

			return TRUE;
		}

		//焦点判断
		if (m_edLogonPassword3.IsChild(pWndFouns))
		{
			m_edLogonPassword3.GetUserPassword(szPassword);
			if (szPassword[0]!=0) OnBnClickedLogonPassword();

			return TRUE;
		}

		//焦点判断
		if (m_edInsurePassword1.IsChild(pWndFouns))
		{
			m_edInsurePassword1.GetUserPassword(szPassword);
			if (szPassword[0]!=0) m_edInsurePassword2.SetFocus();

			return TRUE;
		}

		//焦点判断
		if (m_edInsurePassword2.IsChild(pWndFouns))
		{
			m_edInsurePassword2.GetUserPassword(szPassword);
			if (szPassword[0]!=0) m_edInsurePassword3.SetFocus();

			return TRUE;
		}

		//焦点判断
		if (m_edInsurePassword3.IsChild(pWndFouns))
		{
			m_edInsurePassword3.GetUserPassword(szPassword);
			if (szPassword[0]!=0) OnBnClickedInsurePassword();

			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//连接事件
bool CDlgServicePassword::OnEventMissionLink(INT nErrorCode)
{
	//连接结果
	if (nErrorCode!=0)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//事件处理
		OnMissionConclude();

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("连接服务器超时，密码修改失败！"),MB_OK|MB_ICONSTOP,30);

		return true;
	}

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//登录密码
	if (m_bLogonMission==true)
	{
		//变量定义
		CMD_GP_ModifyLogonPass ModifyLogonPass;
		ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

		//加密密码
		TCHAR szSrcPassword[LEN_PASSWORD]=TEXT("");
		TCHAR szDesPassword[LEN_PASSWORD]=TEXT("");
		CWHEncrypt::MD5Encrypt(m_szSrcPassword,szSrcPassword);
		CWHEncrypt::MD5Encrypt(m_szDesPassword,szDesPassword);

		//构造数据
		ModifyLogonPass.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(ModifyLogonPass.szScrPassword,szSrcPassword,CountArray(ModifyLogonPass.szScrPassword));
		lstrcpyn(ModifyLogonPass.szDesPassword,szDesPassword,CountArray(ModifyLogonPass.szDesPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_LOGON_PASS,&ModifyLogonPass,sizeof(ModifyLogonPass));
	}
	else
	{
		//变量定义
		CMD_GP_ModifyInsurePass ModifyInsurePass;
		ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

		//加密密码
		TCHAR szSrcPassword[LEN_PASSWORD]=TEXT("");
		TCHAR szDesPassword[LEN_PASSWORD]=TEXT("");
		CWHEncrypt::MD5Encrypt(m_szSrcPassword,szSrcPassword);
		CWHEncrypt::MD5Encrypt(m_szDesPassword,szDesPassword);

		//构造数据
		ModifyInsurePass.dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(ModifyInsurePass.szScrPassword,szSrcPassword,CountArray(ModifyInsurePass.szScrPassword));
		lstrcpyn(ModifyInsurePass.szDesPassword,szDesPassword,CountArray(ModifyInsurePass.szDesPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INSURE_PASS,&ModifyInsurePass,sizeof(ModifyInsurePass));
	}

	return true;
}

//关闭事件
bool CDlgServicePassword::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，用户密码修改失败！"),MB_ICONERROR,30);
	}
	
	//事件处理
	OnMissionConclude();

	return true;
}

//读取事件
bool CDlgServicePassword::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_OPERATE_FAILURE:	//操作失败
			{
				//效验参数
				CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

				//失败处理
				OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//显示消息
				if (pOperateFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
				}

				//设置焦点
				if (m_bLogonMission==true)
				{
					m_edLogonPassword1.SetFocus();
				}
				else
				{
					m_edInsurePassword1.SetFocus();
				}

				return true;
			}
		case SUB_GP_OPERATE_SUCCESS:	//操作成功
			{
				//变量定义
				CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)pData;

				//效验数据
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//帐号密码
				if (m_bLogonMission==true)
				{
					//变量定义
					CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
					CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
					tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

					//密码资料
					TCHAR szDesPassword[LEN_PASSWORD]=TEXT("");
					CWHEncrypt::MD5Encrypt(m_szDesPassword,szDesPassword);
					lstrcpyn(pGlobalUserData->szPassword,szDesPassword,CountArray(pGlobalUserData->szPassword));

					//记录密码
					if (pParameterGlobal->m_bRemberPassword==true)
					{
						//构造信息
						TCHAR szKeyUserID[16];
						_sntprintf(szKeyUserID,CountArray(szKeyUserID),TEXT("%s\\%ld"),REG_USER_INFO,pGlobalUserData->dwUserID);

						//上次用户
						CWHRegKey RegUserInfo;
						RegUserInfo.OpenRegKey(REG_USER_INFO,true);

						//用户表项
						CWHRegKey RegUserItem;
						RegUserItem.OpenRegKey(szKeyUserID,true);

						//保存密码
						TCHAR szPassBuffer[MAX_ENCRYPT_LEN]=TEXT("");
						CWHEncrypt::XorEncrypt(m_szDesPassword,szPassBuffer,CountArray(szPassBuffer));

						//帐号信息
						RegUserItem.WriteString(TEXT("UserPassword"),szPassBuffer);
						RegUserItem.WriteString(TEXT("UserPassToken"),pGlobalUserData->szPassword);
					}
				}

				//显示消息
				if (pOperateSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				//成功处理
				OnMissionConclude();
                
				//设置焦点
				if (m_bLogonMission==true)
				{
					m_edLogonPassword1.SetFocus();
				}
				else
				{
					m_edInsurePassword1.SetFocus();
				}

				return true;
			}
		}
	}

	return true;
}

//开始任务
VOID CDlgServicePassword::OnMissionStart()
{
	//功能按钮
	m_btLogonPassword.EnableWindow(FALSE);
	m_btInsurePassword.EnableWindow(FALSE);

	return;
}

//终止任务
VOID CDlgServicePassword::OnMissionConclude()
{
	//功能按钮
	m_btLogonPassword.EnableWindow(TRUE);
	m_btInsurePassword.EnableWindow(TRUE);

	//设置控件
	m_edLogonPassword1.SetUserPassword(TEXT(""));
	m_edLogonPassword2.SetUserPassword(TEXT(""));
	m_edLogonPassword3.SetUserPassword(TEXT(""));

	//设置控件
	m_edInsurePassword1.SetUserPassword(TEXT(""));
	m_edInsurePassword2.SetUserPassword(TEXT(""));
	m_edInsurePassword3.SetUserPassword(TEXT(""));

	return;
}

//绘画背景
BOOL CDlgServicePassword::OnEraseBkgnd(CDC * pDC)
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

	//加载资源
	CPngImage ImagePassword;
	ImagePassword.LoadImage(AfxGetInstanceHandle(),TEXT("PASSWORD_LEVEL"));

	//获取大小
	CSize SizePassword;
	SizePassword.SetSize(ImagePassword.GetWidth()/3,ImagePassword.GetHeight()/2);

	//填充背景
	pBufferDC->FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);

	//绘画等级
	ImagePassword.DrawImage(pBufferDC,230,76,SizePassword.cx*3,SizePassword.cy,0,0);
	ImagePassword.DrawImage(pBufferDC,230,256,SizePassword.cx*3,SizePassword.cy,0,0);

	//绘画叠加
	if (m_cbLogonPasswordLevel>=PASSWORD_LEVEL_1)
	{
		INT nImagePos=(m_cbLogonPasswordLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
		ImagePassword.DrawImage(pBufferDC,230+nImagePos,76,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	}

	//绘画叠加
	if (m_cbInsurePasswordLevel>=PASSWORD_LEVEL_1)
	{
		INT nImagePos=(m_cbInsurePasswordLevel-PASSWORD_LEVEL_1)*SizePassword.cx;
		ImagePassword.DrawImage(pBufferDC,230+nImagePos,256,SizePassword.cx,SizePassword.cy,nImagePos,SizePassword.cy);
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//密码输入
VOID CDlgServicePassword::OnEnChangeLogonPassword()
{
	//获取密码
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	m_edLogonPassword2.GetUserPassword(szPassword);

	//等级判断
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//更新窗口
	if (m_cbLogonPasswordLevel!=cbPasswordLevel)
	{
		//设置变量
		m_cbLogonPasswordLevel=cbPasswordLevel;

		//更新窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//密码输入
VOID CDlgServicePassword::OnEnChangeInserePassword()
{
	//获取密码
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	m_edInsurePassword2.GetUserPassword(szPassword);

	//等级判断
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	BYTE cbPasswordLevel=pUserItemElement->GetPasswordLevel(szPassword);

	//更新窗口
	if (m_cbInsurePasswordLevel!=cbPasswordLevel)
	{
		//设置变量
		m_cbInsurePasswordLevel=cbPasswordLevel;

		//更新窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//密码修改
VOID CDlgServicePassword::OnBnClickedLogonPassword()
{
	//变量定义
	TCHAR szSrcPassword[LEN_PASSWORD];
	TCHAR szDesPassword[2][LEN_PASSWORD];

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//获取信息
	m_edLogonPassword1.GetUserPassword(szSrcPassword);
	m_edLogonPassword2.GetUserPassword(szDesPassword[0]);
	m_edLogonPassword3.GetUserPassword(szDesPassword[1]);

	//加密密码
	TCHAR szEncryptPassword[LEN_PASSWORD]=TEXT("");
	CWHEncrypt::MD5Encrypt(szSrcPassword,szEncryptPassword);

	//密码判断
	if (szSrcPassword[0]==0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("原帐号密码错误，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_edLogonPassword1.SetFocus();

		return;
	}

	//密码判断
	if (lstrcmp(szEncryptPassword,pGlobalUserData->szPassword)!=0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("原帐号密码错误，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_edLogonPassword1.SetFocus();

		return;
	}

	//密码判断
	if (lstrcmp(szDesPassword[0],szDesPassword[1])!=0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("两次输入的新帐号密码不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_edLogonPassword2.SetFocus();

		return;
	}

	//密码判断
	if (pUserItemElement->EfficacyPassword(szDesPassword[0],szDescribe,CountArray(szDescribe))==false)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(szDescribe,MB_ICONERROR,0);

		//设置焦点
		m_edLogonPassword2.SetFocus();

		return;
	}

	//一致判断
	if (lstrcmp(szSrcPassword,szDesPassword[0])==0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您所输入的新密码与原密码一致，请重新输入新密码！"),MB_ICONERROR,0);

		//设置焦点
		m_edLogonPassword2.SetFocus();

		//设置密码
		m_edLogonPassword2.SetUserPassword(TEXT(""));
		m_edLogonPassword3.SetUserPassword(TEXT(""));

		return;
	}

	//设置变量
	m_bLogonMission=true;
	lstrcpyn(m_szSrcPassword,szSrcPassword,CountArray(m_szSrcPassword));
	lstrcpyn(m_szDesPassword,szDesPassword[0],CountArray(m_szDesPassword));

	//事件处理
	OnMissionStart();

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false) 
	{
		OnMissionConclude();
		return;
	}

	return;
}

//密码修改
VOID CDlgServicePassword::OnBnClickedInsurePassword()
{
	//变量定义
	TCHAR szSrcPassword[LEN_PASSWORD];
	TCHAR szDesPassword[2][LEN_PASSWORD];

	//获取信息
	m_edInsurePassword1.GetUserPassword(szSrcPassword);
	m_edInsurePassword2.GetUserPassword(szDesPassword[0]);
	m_edInsurePassword3.GetUserPassword(szDesPassword[1]);

	//密码判断
	if (szSrcPassword[0]==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("原保险柜密码错误，请重新输入！"),MB_ICONERROR,0);

		m_edLogonPassword1.SetFocus();

		return;
	}

	//密码判断
	if (lstrcmp(szDesPassword[0],szDesPassword[1])!=0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("两次输入的新保险柜不一致，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_edInsurePassword2.SetFocus();

		return;
	}

	//变量定义
	TCHAR szDescribe[128]=TEXT("");
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

	//密码判断
	if (pUserItemElement->EfficacyPassword(szDesPassword[0],szDescribe,CountArray(szDescribe))==false)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("新保险柜密码不能为空，请重新输入！"),MB_ICONERROR,0);

		//设置焦点
		m_edInsurePassword2.SetFocus();

		return;
	}

	//一致判断
	if (lstrcmp(szSrcPassword,szDesPassword[0])==0)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您所输入的新密码与原密码一致，请重新输入新密码！"),MB_ICONERROR,0);

		//设置焦点
		m_edInsurePassword2.SetFocus();

		//设置密码
		m_edInsurePassword2.SetUserPassword(TEXT(""));
		m_edInsurePassword3.SetUserPassword(TEXT(""));

		return;
	}

	//设置变量
	m_bLogonMission=false;
	lstrcpyn(m_szSrcPassword,szSrcPassword,CountArray(m_szSrcPassword));
	lstrcpyn(m_szDesPassword,szDesPassword[0],CountArray(m_szDesPassword));

	//事件处理
	OnMissionStart();

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false) 
	{
		OnMissionConclude();
		return;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
