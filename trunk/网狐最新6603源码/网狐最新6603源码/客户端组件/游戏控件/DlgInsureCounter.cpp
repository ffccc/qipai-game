#include "StdAfx.h"
#include "Resource.h"
#include "Information.h"
#include "DlgInsureCounter.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgInsureItem, CSkinDialog)
	ON_EN_CHANGE(IDC_SCORE, OnEnChangeScore)
	ON_BN_CLICKED(IDC_BT_QUERY_INFO, OnBnClickedQueryInfo)
	ON_BN_CLICKED(IDC_BT_SAVE_SCORE, OnBnClickedSaveScore)
	ON_BN_CLICKED(IDC_BT_TAKE_SCORE, OnBnClickedTakeScore)
	ON_STN_CLICKED(IDC_FORGET_INSURE, OnBnClickedForgetInsure)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsureItem::CDlgInsureItem() : CSkinDialog(IDD_INSURE_CONUTER)
{
	//服务管理
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=false;

	//设置变量
	m_bInitInfo=false;
	ZeroMemory(&m_UserInsureInfo,sizeof(m_UserInsureInfo));

	return;
}

//析构函数
CDlgInsureItem::~CDlgInsureItem()
{
}

//控件绑定
VOID CDlgInsureItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//控件变量
	DDX_Control(pDX, IDC_SCORE, m_edScore);
	DDX_Control(pDX, IDC_TAB_CTRL, m_TabControl);
	DDX_Control(pDX, IDC_PASSWORD, m_PasswordControl);
	DDX_Control(pDX, IDC_FORGET_INSURE, m_ForgetInsure);

	//按钮控件
	DDX_Control(pDX, IDC_BT_QUERY_INFO, m_btQueryInfo);
	DDX_Control(pDX, IDC_BT_TAKE_SCORE, m_btTakeScore);
	DDX_Control(pDX, IDC_BT_SAVE_SCORE, m_btSaveScore);
}

//消息解释
BOOL CDlgInsureItem::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgInsureItem::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("保险柜"));

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//设置控件
	m_edScore.LimitText(16L);
	m_TabControl.SetItemSize(CSize(100,25));

	//加入标签
	m_TabControl.InsertItem(0,TEXT(" 存款与取款 "));
	//m_TabControl.InsertItem(1,TEXT(" 赠送玩家游戏币 "));

	//设置按钮
	HINSTANCE hResInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	m_btQueryInfo.SetButtonImage(IDB_BT_QUERY_INFO,hResInstance,false,false);
	m_btSaveScore.SetButtonImage(IDB_BT_INSURE_SCORE,TEXT("BT_SAVE_SCORE"),hResInstance,false,false);
	m_btTakeScore.SetButtonImage(IDB_BT_INSURE_SCORE,TEXT("BT_TAKE_SCORE"),hResInstance,false,false);

	//查询信息
	OnBnClickedQueryInfo();

	return FALSE;
}

//确定按钮
VOID CDlgInsureItem::OnOK()
{
	//提取按钮
	OnBnClickedTakeScore();

	return;
}

//绘画消息
VOID CDlgInsureItem::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//获取位置
	CRect rcScore;
	CRect rcTabControl;
	m_edScore.GetWindowRect(&rcScore);
	m_TabControl.GetWindowRect(&rcTabControl);

	//转换位置
	ScreenToClient(&rcScore);
	ScreenToClient(&rcTabControl);

	//绘画线条
	INT nLBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;
	pDC->FillSolidRect(nLBorder,rcTabControl.bottom-1L,nWidth-nLBorder-nRBorder,1,RGB(145,168,183));

	//绘画输入
	COLORREF crScoreBorad=CSkinEdit::m_SkinAttribute.m_crEnableBorad;
	pDC->Draw3dRect(rcScore.left-6,rcScore.top-5,rcScore.Width()+12,24L,crScoreBorad,crScoreBorad);

	//加载资源
	CPngImage ImageInsureInfo;
	ImageInsureInfo.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INFO_FRAME"));

	//绘画信息
	INT nYPosInfo=75L;
	INT nXPosInfo=(nWidth-ImageInsureInfo.GetWidth())/2L;
	ImageInsureInfo.DrawImage(pDC,nXPosInfo,nYPosInfo,ImageInsureInfo.GetWidth(),ImageInsureInfo.GetHeight(),0,0);

	//银行信息
	if (m_bInitInfo==true)
	{
		//加载资源
		CPngImage ImageInsureIcon;
		ImageInsureIcon.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("INSURE_ICON"));

		//绘画标志
		ImageInsureIcon.DrawImage(pDC,nXPosInfo+10,nYPosInfo+17);

		//绘画数字
		DrawNumberString(pDC,m_UserInsureInfo.lUserScore,nXPosInfo+136,nYPosInfo+18L);
		DrawNumberString(pDC,m_UserInsureInfo.lUserInsure,nXPosInfo+136,nYPosInfo+53L);

		//绘画提示
		LPCTSTR pszScoreString=TEXT("游戏币：");
		LPCTSTR pszInsureString=TEXT("保险柜：");
		pDC->TextOut(nXPosInfo+83,nYPosInfo+20L,pszScoreString,lstrlen(pszScoreString));
		pDC->TextOut(nXPosInfo+83,nYPosInfo+55L,pszInsureString,lstrlen(pszInsureString));
	}
	else
	{
	}

	//构造提示
	TCHAR szString[128]=TEXT("");
	_sntprintf(szString,CountArray(szString),TEXT("温馨提示：存入游戏币免手续费，取出与赠送游戏币将扣除 %ld%% 的手续费"),100*INSURE_REVENUE_RATIO);

	//温馨提示
	pDC->SetTextColor(RGB(50,50,50));
	pDC->TextOut(18,nHeight-28,szString,lstrlen(szString));

	return;
}

//开始任务
VOID CDlgInsureItem::OnMissionStart()
{
	return;
}

//终止任务
VOID CDlgInsureItem::OnMissionConclude()
{
	return;
}

//查询资料
VOID CDlgInsureItem::QueryUserInsureInfo()
{
	//设置变量
	m_bSaveMission=false;
	m_bTakeMission=false;
	m_bQueryMission=true;

	//事件处理
	OnMissionStart();

	//执行查询
	PerformQueryInfo();

	return;
}

//获取金币
SCORE CDlgInsureItem::GetUserControlScore()
{
	//变量定义
	WORD wIndex=0;
	SCORE lScore=0L;

	//获取数值
	TCHAR szBuffer[32]=TEXT("");
	GetDlgItemText(IDC_SCORE,szBuffer,CountArray(szBuffer));

	//构造数值
	while (szBuffer[wIndex]!=0)
	{
		//变量定义
		WORD wCurrentIndex=wIndex++;

		//构造数值
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lScore=lScore*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lScore;
}

//绘画数字
VOID CDlgInsureItem::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
{
	//转换逗号
	TCHAR szControl[128]=TEXT("");
	SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

	//变量定义
	INT nXDrawPos=nXPos;
	INT nScoreLength=lstrlen(szControl);

	//绘画判断
	if (nScoreLength>0L)
	{
		//加载资源
		CPngImage ImageNumber;
		ImageNumber.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("SCORE_NUMBER"));

		//获取大小
		CSize SizeNumber;
		SizeNumber.SetSize(ImageNumber.GetWidth()/12L,ImageNumber.GetHeight());

		//绘画数字
		for (INT i=0;i<nScoreLength;i++)
		{
			//绘画逗号
			if (szControl[i]==TEXT(','))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*10L,0L);
			}

			//绘画点号
			if (szControl[i]==TEXT('.'))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*11L,0L);
			}

			//绘画数字
			if (szControl[i]>=TEXT('0')&&szControl[i]<=TEXT('9'))
			{
				ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*(szControl[i]-TEXT('0')),0L);
			}

			//设置位置
			nXDrawPos+=SizeNumber.cx;
		}
	}

	return;
}

//转换字符
VOID CDlgInsureItem::SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize)
{
	//变量定义
	LPCTSTR pszNumber[]={TEXT("零"),TEXT("壹"),TEXT("贰"),TEXT("叁"),TEXT("肆"),TEXT("伍"),TEXT("陆"),TEXT("柒"),TEXT("捌"),TEXT("玖")};
	LPCTSTR pszWeiName[]={TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万"),TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("亿"),TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万")};

	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	bool bNeedFill=false;
	bool bNeedZero=false;
	UINT uTargetIndex=0,uSourceIndex=0;

	//字符长度
	UINT uItemLength=lstrlen(pszNumber[0]);
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//转换操作
	for (UINT i=0;i<uSwitchLength;i++)
	{
		//变量定义
		WORD wNumberIndex=szSwitchScore[i]-TEXT('0');

		//补零操作
		if ((bNeedZero==true)&&(wNumberIndex!=0L))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszNumber[0],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//拷贝数字
		if (wNumberIndex!=0L)
		{
			_tcsncat(pszBuffer,pszNumber[wNumberIndex],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//拷贝位名
		if ((wNumberIndex!=0L)&&((uSwitchLength-i)>=2))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//补零判断
		if ((bNeedZero==false)&&(wNumberIndex==0))
		{
			bNeedZero=true;
		}

		//补位判断
		if ((bNeedFill==false)&&(wNumberIndex!=0))
		{
			bNeedFill=true;
		}

		//填补位名
		if (((uSwitchLength-i)==5)||((uSwitchLength-i)==9))
		{
			//拷贝位名
			if ((bNeedFill==true)&&(wNumberIndex==0L))
			{
				_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
			}

			//设置变量
			bNeedZero=false;
			bNeedFill=false;
		}
	}

	return;
}

//转换字符
VOID CDlgInsureItem::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//转换字符
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//拷贝字符
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//插入逗号
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//结束字符
	pszBuffer[wTargetIndex++]=0;

	return;
}

//输入信息
VOID CDlgInsureItem::OnEnChangeScore()
{
	//变量定义
	SCORE lScore=GetUserControlScore();

	//设置界面
	if (lScore>0L)
	{
		//转换数值
		TCHAR szString[128]=TEXT("");
		SwitchScoreString(lScore,szString,CountArray(szString));

		//转换逗号
		TCHAR szControl[128]=TEXT("");
		SwitchScoreFormat(lScore,3L,szControl,CountArray(szControl));

		//设置控件
		SetDlgItemText(IDC_SCORE,szControl);
		SetDlgItemText(IDC_SCORE_STRING,szString);

		//设置光标
		INT nScoreLength=m_edScore.GetWindowTextLength();
		m_edScore.SetSel(nScoreLength,nScoreLength,FALSE);
	}
	else
	{
		//设置控件
		SetDlgItemText(IDC_SCORE,TEXT(""));
		SetDlgItemText(IDC_SCORE_STRING,TEXT(""));
	}

	return;
}

//查询按钮
VOID CDlgInsureItem::OnBnClickedQueryInfo()
{
	//设置焦点
	m_edScore.SetFocus();

	//查询信息
	QueryUserInsureInfo();

	return;
}

//存款按钮
VOID CDlgInsureItem::OnBnClickedSaveScore()
{
	//获取变量
	SCORE lSaveScore=GetUserControlScore();

	//数据判断
	if (lSaveScore<=0L)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("存入的游戏币数量不能为空，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_edScore.SetFocus();

		return;
	}

	//数目判断
	if (lSaveScore>m_UserInsureInfo.lUserScore)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您所携带游戏币的数目余额不足，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_edScore.SetFocus();

		return;
	}

	//事件处理
	OnMissionStart();

	//设置变量
	m_bSaveMission=true;
	m_bTakeMission=false;
	m_bQueryMission=false;

	//执行存款
	PerformSaveScore(lSaveScore);

	return;
}

//取款按钮
VOID CDlgInsureItem::OnBnClickedTakeScore()
{
	//获取变量
	SCORE lTakeScore=GetUserControlScore();

	//获取密码
	TCHAR szPassword[LEN_PASSWORD];
	m_PasswordControl.GetUserPassword(szPassword);

	//加密密码
	TCHAR szInsurePass[LEN_MD5];
	CWHEncrypt::MD5Encrypt(szPassword,szInsurePass);

	//数据判断
	if (lTakeScore<=0L)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("取出的游戏币数量不能为空，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_edScore.SetFocus();

		return;
	}

	//数目判断
	if (lTakeScore>m_UserInsureInfo.lUserInsure)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您保险柜游戏币的数目余额不足，请重新输入游戏币数量！"),MB_ICONERROR,30);

		//设置焦点
		m_edScore.SetFocus();

		return;
	}

	//密码判断
	if (szPassword[0]==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("保险柜密码不能为空，请重新输入保险柜密码！"),MB_ICONERROR,30);

		//设置焦点
		m_PasswordControl.SetFocus();

		return;
	}

	//事件处理
	OnMissionStart();

	//设置变量
	m_bTakeMission=true;
	m_bSaveMission=false;
	m_bQueryMission=false;

	//事件通知
	PerformTakeScore(lTakeScore,szInsurePass);

	return;
}

//忘记密码
VOID CDlgInsureItem::OnBnClickedForgetInsure()
{
	//构造地址
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/LogonLink4.asp"),szPlatformLink);

	//打开页面
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsurePlaza::CDlgInsurePlaza()
{
	//设置任务
	m_MissionManager.InsertMissionItem(this);

	//设置变量
	m_lScore=0L;
	ZeroMemory(m_szInsurePass,sizeof(m_szInsurePass));

	return;
}

//析构函数
CDlgInsurePlaza::~CDlgInsurePlaza()
{
}

//查询事件
VOID CDlgInsurePlaza::PerformQueryInfo()
{
	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//存入事件
VOID CDlgInsurePlaza::PerformSaveScore(SCORE lSaveScore)
{
	//设置变量
	m_lScore=lSaveScore;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//取出事件
VOID CDlgInsurePlaza::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//设置变量
	m_lScore=lTakeScore;
	lstrcpyn(m_szInsurePass,pszInsurePass,CountArray(m_szInsurePass));

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//连接事件
bool CDlgInsurePlaza::OnEventMissionLink(INT nErrorCode)
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

		//错误提示
		if (m_bSaveMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，游戏币存入失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bTakeMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，游戏币取出失败，请稍后再次重试！"),MB_OK|MB_ICONSTOP,30);
		}

		//错误提示
		if (m_bQueryMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("连接服务器超时，保险柜资料获取失败！"),MB_OK|MB_ICONSTOP,30);
		}

		return true;
	}

	//存入任务
	if (m_bSaveMission==true)
	{
		//变量定义
		CMD_GP_UserSaveScore UserSaveScore;
		ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		UserSaveScore.lSaveScore=m_lScore;
		UserSaveScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineID(UserSaveScore.szMachineID);

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_SAVE_SCORE,&UserSaveScore,sizeof(UserSaveScore));

		return true;
	}

	//取出任务
	if (m_bTakeMission==true)
	{
		//变量定义
		CMD_GP_UserTakeScore UserTakeScore;
		ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//设置变量
		UserTakeScore.lTakeScore=m_lScore;
		UserTakeScore.dwUserID=pGlobalUserData->dwUserID;
		CWHService::GetMachineID(UserTakeScore.szMachineID);
		lstrcpyn(UserTakeScore.szPassword,m_szInsurePass,CountArray(UserTakeScore.szPassword));

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_USER_TAKE_SCORE,&UserTakeScore,sizeof(UserTakeScore));

		return true;
	}

	//查询任务
	if (m_bQueryMission==true)
	{
		//变量定义
		CMD_GP_QueryInsureInfo QueryInsureInfo;
		ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

		//设置变量
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		QueryInsureInfo.dwUserID=pGlobalUserInfo->GetGlobalUserData()->dwUserID;

		//发送数据
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
	}

	return true;
}

//关闭事件
bool CDlgInsurePlaza::OnEventMissionShut(BYTE cbShutReason)
{
	//提示消息
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//存入任务
		if (m_bSaveMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，游戏币存入失败！"),MB_ICONERROR,30);
		}

		//取出任务
		if (m_bTakeMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，游戏币取出失败！"),MB_ICONERROR,30);
		}

		//查询任务
		if (m_bQueryMission==true)
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("与服务器的连接异常断开了，保险柜资料获取失败！"),MB_ICONERROR,30);
		}
	}

	//事件处理
	OnMissionConclude();

	return true;
}

//读取事件
bool CDlgInsurePlaza::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_USER_INSURE_INFO:		//银行资料
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GP_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GP_UserInsureInfo)) return false;

				//变量定义
				CMD_GP_UserInsureInfo * pUserInsureInfo=(CMD_GP_UserInsureInfo *)pData;

				//设置变量
				m_bInitInfo=true;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//更新界面
				RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				return true;
			}
		case SUB_GP_USER_INSURE_SUCCESS:	//银行成功
			{
				//效验参数
				CMD_GP_UserInsureSuccess * pUserInsureSuccess=(CMD_GP_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//设置变量
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//输入控件
				m_PasswordControl.SetFocus();
				m_PasswordControl.SetUserPassword(TEXT(""));

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//更新界面
				RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				//显示消息
				if (pUserInsureSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				return true;
			}
		case SUB_GP_USER_INSURE_FAILURE:	//银行失败
			{
				//效验参数
				CMD_GP_UserInsureFailure * pUserInsureFailure=(CMD_GP_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//失败处理
				OnMissionConclude();

				//关闭连接
				m_MissionManager.ConcludeMissionItem(this,false);

				//输入控件
				m_PasswordControl.SetFocus();
				m_PasswordControl.SetUserPassword(TEXT(""));

				//显示消息
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgInsureServer::CDlgInsureServer()
{
	//设置变量
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;

	return;
}

//析构函数
CDlgInsureServer::~CDlgInsureServer()
{
}

//查询事件
VOID CDlgInsureServer::PerformQueryInfo()
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO);

	return;
}

//存入事件
VOID CDlgInsureServer::PerformSaveScore(SCORE lSaveScore)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	ZeroMemory(&SaveScoreRequest,sizeof(SaveScoreRequest));

	//设置变量
	SaveScoreRequest.lSaveScore=lSaveScore;

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//取出事件
VOID CDlgInsureServer::PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)
{
	//效验参数
	ASSERT(m_pITCPSocket!=NULL);
	if (m_pITCPSocket==NULL) return;

	//变量定义
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//设置变量
	TakeScoreRequest.lTakeScore=lTakeScore;
	lstrcpyn(TakeScoreRequest.szInsurePass,pszInsurePass,CountArray(TakeScoreRequest.szInsurePass));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	return;
}

//配置银行
VOID CDlgInsureServer::Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem)
{
	//设置变量
	m_pITCPSocket=pITCPSocket;
	m_pIMySelfUserItem=pIMySelfUserItem;

	return;
}

//银行消息
bool CDlgInsureServer::OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//消息判断
	if (wMainCmdID==MDM_GR_INSURE)
	{
		switch (wSubCmdID)
		{
		case SUB_GR_USER_INSURE_INFO:		//银行资料
			{
				//效验参数
				ASSERT(wDataSize==sizeof(CMD_GR_S_UserInsureInfo));
				if (wDataSize<sizeof(CMD_GR_S_UserInsureInfo)) return false;

				//变量定义
				CMD_GR_S_UserInsureInfo * pUserInsureInfo=(CMD_GR_S_UserInsureInfo *)pData;

				//设置变量
				m_bInitInfo=true;
				m_UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
				m_UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;

				//更新界面
				RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				return true;
			}
		case SUB_GR_USER_INSURE_SUCCESS:	//银行成功
			{
				//效验参数
				CMD_GR_S_UserInsureSuccess * pUserInsureSuccess=(CMD_GR_S_UserInsureSuccess *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

				//设置变量
				m_UserInsureInfo.lUserScore=pUserInsureSuccess->lUserScore;
				m_UserInsureInfo.lUserInsure=pUserInsureSuccess->lUserInsure;

				//输入控件
				m_PasswordControl.SetFocus();
				m_PasswordControl.SetUserPassword(TEXT(""));

				//更新界面
				RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

				//显示消息
				if (pUserInsureSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				return true;
			}
		case SUB_GR_USER_INSURE_FAILURE:	//银行失败
			{
				//效验参数
				CMD_GR_S_UserInsureFailure * pUserInsureFailure=(CMD_GR_S_UserInsureFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

				//失败处理
				OnMissionConclude();

				//输入控件
				if (m_bInitInfo==true)
				{
					m_PasswordControl.SetFocus();
					m_PasswordControl.SetUserPassword(TEXT(""));
				}

				//显示消息
				if (pUserInsureFailure->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
				}

				return true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
