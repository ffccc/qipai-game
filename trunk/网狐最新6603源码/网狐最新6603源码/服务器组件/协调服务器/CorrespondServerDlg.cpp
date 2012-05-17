#include "Stdafx.h"
#include "Resource.h"
#include "CorrespondServerDlg.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCorrespondServerDlg, CDialog)
	ON_WM_QUERYENDSESSION()
	ON_BN_CLICKED(IDC_STOP_SERVICE, OnBnClickedStopService)
	ON_BN_CLICKED(IDC_START_SERVICE, OnBnClickedStartService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCorrespondServerDlg::CCorrespondServerDlg() : CDialog(IDD_DLG_CORRESPOND_SERVER)
{
}

//析构函数
CCorrespondServerDlg::~CCorrespondServerDlg()
{
}

//控件绑定
VOID CCorrespondServerDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACE_MESSAGE, m_TraceServiceControl);
}

//初始化函数
BOOL CCorrespondServerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("协调服务器 -- [ 停止 ]"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//设置组件
	m_ServiceUnits.SetServiceUnitsSink(this);

	return TRUE;
}

//确定消息
VOID CCorrespondServerDlg::OnOK()
{
	return;
}

//取消函数
VOID CCorrespondServerDlg::OnCancel()
{
	//关闭询问
	if (m_ServiceUnits.GetServiceStatus()!=ServiceStatus_Stop)
	{
		LPCTSTR pszQuestion=TEXT("协调服务器正在运行中，您确实要关闭服务器吗？");
		if (AfxMessageBox(pszQuestion,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION)!=IDYES) return;
	}

	//停止服务
	m_ServiceUnits.ConcludeService();

	__super::OnCancel();
}

//消息解释
BOOL CCorrespondServerDlg::PreTranslateMessage(MSG * pMsg)
{
	//按键过滤
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//服务状态
VOID CCorrespondServerDlg::OnServiceUnitsStatus(enServiceStatus ServiceStatus)
{
	//状态设置
	switch (ServiceStatus)
	{
	case ServiceStatus_Stop:	//停止状态
		{
			//设置标题
			SetWindowText(TEXT("协调服务器 -- [ 停止 ]"));

			//设置按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);

			//提示信息
			LPCTSTR pszDescribe=TEXT("服务停止成功");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	case ServiceStatus_Config:	//配置状态
		{
			//设置标题
			SetWindowText(TEXT("协调服务器 -- [ 初始化 ]"));

			//设置按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

			//提示信息
			LPCTSTR pszDescribe=TEXT("正在初始化组件...");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	case ServiceStatus_Service:	//服务状态
		{
			//设置标题
			SetWindowText(TEXT("协调服务器 -- [ 运行 ]"));

			//设置按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

			//提示信息
			LPCTSTR pszDescribe=TEXT("服务启动成功");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	}

	return;
}

//启动服务
VOID CCorrespondServerDlg::OnBnClickedStartService()
{
	//启动服务
	try
	{
		m_ServiceUnits.StartService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//停止服务
VOID CCorrespondServerDlg::OnBnClickedStopService()
{
	//停止服务
	try
	{
		m_ServiceUnits.ConcludeService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//关闭询问
BOOL CCorrespondServerDlg::OnQueryEndSession()
{
	//提示消息
	if (m_ServiceUnits.GetServiceStatus()!=ServiceStatus_Stop)
	{
		CTraceService::TraceString(TEXT("服务正在运行中，系统要求注销回话请求失败"),TraceLevel_Warning);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
