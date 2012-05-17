#include "Stdafx.h"
#include "Resource.h"
#include "CameraControl.h"

//////////////////////////////////////////////////////////////////////////////////

#define IDC_CAMERA_WND				100									//窗口标识

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCameraControl, CSkinDialog)
	ON_BN_CLICKED(IDC_BT_REPEAT,OnBnClickedRepeat)
	ON_BN_CLICKED(IDC_BT_CAMERA_DICT,OnBnClickedCameraDict)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CCameraControl::CCameraControl(CWnd * pParent) : CSkinDialog(IDD_DLG_CAMERA, pParent)
{
	//设置变量
	m_bInit=false;
	m_hWndCamera=NULL;

	//设置变量
	ZeroMemory(&m_CapStatus,sizeof(m_CapStatus));
	ZeroMemory(&m_CapTureparms,sizeof(m_CapTureparms));
	ZeroMemory(&m_CapDrivercaps,sizeof(m_CapDrivercaps));

	return;
}

//析构函数
CCameraControl::~CCameraControl()
{
}

//控件绑定
VOID CCameraControl::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	//按钮控件
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDC_BT_REPEAT, m_btRepeat);
	DDX_Control(pDX, IDC_BT_CAMERA_DICT, m_btCameraDict);

	//其他控件
	DDX_Control(pDX, IDC_STRING_CONTROL, m_StringControl);
	DDX_Control(pDX, IDC_CAMERA_CONTROL, m_CameraControl);
}

//消息解释
BOOL CCameraControl::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CCameraControl::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("视频拍摄"));

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//获取大小
	CRect rcClient;
	m_CameraControl.GetClientRect(&rcClient);

	//创建窗口
	DWORD dwStyle=WS_CHILD|WS_BORDER;
	m_hWndCamera=capCreateCaptureWindow(NULL,dwStyle,0,0,rcClient.Width(),rcClient.Height(),m_CameraControl,IDC_CAMERA_WND);

	//连接设备
	if (capDriverConnect(m_hWndCamera,0)==TRUE)
	{
		//获取性能
		capDriverGetCaps(m_hWndCamera,&m_CapDrivercaps,sizeof(m_CapDrivercaps));

		//结果判断
		if (m_CapDrivercaps.fCaptureInitialized==TRUE)
		{
			//设置变量
			m_bInit=true;

			//设置属性
			capPreview(m_hWndCamera,TRUE);
			capPreviewRate(m_hWndCamera,30);

			//显示窗口
			::ShowWindow(m_hWndCamera,SW_SHOW);

			//获取状态
			capGetStatus(m_hWndCamera,&m_CapStatus,sizeof(m_CapStatus));
		}
	}

	//设置控件
	if (m_bInit==true)
	{
		m_btCameraDict.EnableWindow(TRUE);
		m_StringControl.SetWindowText(TEXT("摄像头初始化成功"));
	}
	else
	{
		//设置信息
		m_StringControl.SetWindowText(TEXT("没有安装摄像头或者摄像头初始化失败"));
	}

	return TRUE; 
}

//重拍按钮
VOID CCameraControl::OnBnClickedRepeat()
{
	//设置窗口
	capPreview(m_hWndCamera,TRUE);

	//设置按钮
	m_btOk.EnableWindow(FALSE);
	m_btRepeat.EnableWindow(FALSE);
	m_btCameraDict.EnableWindow(TRUE);

	return;
}

//拍照按钮
VOID CCameraControl::OnBnClickedCameraDict()
{
	//截获图像
	capGrabFrame(m_hWndCamera);
	capEditCopy(m_hWndCamera);

	//拷贝图像
	if (OpenClipboard()==FALSE)
	{
		ASSERT(FALSE);
		return;
	}

	//销毁图像
	if (m_ImageResult.IsNull()==false)
	{
		m_ImageResult.Destroy();
	}

	//拷贝数据
	HBITMAP hImageResult=(HBITMAP)CopyImage(GetClipboardData(CF_BITMAP),IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

	//结果处理
	if (hImageResult!=NULL)
	{
		//附加数据
		m_ImageResult.Attach(hImageResult);

		//清空剪切
		EmptyClipboard();
		CloseClipboard();

		//设置按钮
		m_btOk.EnableWindow(TRUE);
		m_btRepeat.EnableWindow(TRUE);
		m_btCameraDict.EnableWindow(FALSE);

		return;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
