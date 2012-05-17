#include "StdAfx.h"
#include "Resource.h"
#include "CameraControl.h"
#include "FaceCustomControl.h"

//////////////////////////////////////////////////////////////////////////////////

//控件标识
#define IDC_BT_TURN_LEFT			100									//旋转按钮
#define IDC_BT_TURN_RIGHT			101									//旋转按钮
#define IDC_BT_SHOW_VLINE			102									//控制按钮
#define IDC_BT_LOAD_IMAGE			103									//加载图片
#define IDC_BT_LOAD_CAMERA			104									//加载摄像

//控件标识
#define IDC_IMAGE_EDIT_CTRL			200									//编辑控件

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFaceItemCustomWnd, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()

	//按钮消息
	ON_BN_CLICKED(IDC_BT_TURN_LEFT,OnBnClickedTurnLeft)
	ON_BN_CLICKED(IDC_BT_TURN_RIGHT,OnBnClickedTurnRight)
	ON_BN_CLICKED(IDC_BT_SHOW_VLINE,OnBnClickedShowVLine)
	ON_BN_CLICKED(IDC_BT_LOAD_IMAGE,OnBnClickedLoadImage)
	ON_BN_CLICKED(IDC_BT_LOAD_CAMERA,OnBnClickedLoadCamera)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CFaceItemCustomWnd::CFaceItemCustomWnd()
{
}

//析构函数
CFaceItemCustomWnd::~CFaceItemCustomWnd()
{
}

//加载判断
bool CFaceItemCustomWnd::IsNull()
{
	return m_ImageEditorControl.IsNull();
}

//重置图片
VOID CFaceItemCustomWnd::DestoryImage()
{
	//状态判断
	if (m_ImageEditorControl.IsNull()==false)
	{
		//设置控件
		m_btTurnLeft.ShowWindow(SW_HIDE);
		m_btTurnRight.ShowWindow(SW_HIDE);
		m_btShowVLine.ShowWindow(SW_HIDE);

		//重置图片
		m_ImageEditorControl.DestoryImage();
		m_ImageEditorControl.ShowWindow(SW_HIDE);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	}

	return;
}

//设置接口
VOID CFaceItemCustomWnd::SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent)
{
	//设置接口
	m_ImageEditorControl.SetImageEditorEvent(pIImageEditorEvent);

	return;
}

//获取图片
VOID CFaceItemCustomWnd::DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//获取图片
	m_ImageEditorControl.DrawEditImage(pDC,nXPos,nYPos,nWidth,nHeight);

	return;
}

//绘画消息 
VOID CFaceItemCustomWnd::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载图片
	CPngImage ImageFaceEditBack;
	ImageFaceEditBack.LoadImage(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),TEXT("FACE_EDIT_BACK"));

	//绘画背景
	ImageFaceEditBack.DrawImage(&dc,30,70);

	//获取对象
	ASSERT(CSkinResourceManager::GetInstance()!=NULL);
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();

	//设置环境
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(pSkinResourceManager->GetDefaultFont());

	//绘画文字
	if (m_ImageEditorControl.IsWindowVisible()==FALSE)
	{
		dc.TextOut(60,150,TEXT("您可以选择以下一种方式设置头像:"));
		dc.TextOut(60,180,TEXT("本地照片：选择一张本地图片编辑后上传为头像。"));
		dc.TextOut(60,205,TEXT("自拍头像：通过摄像头拍照编辑后上传为头像。"));
	}

	return;
}

//位置信息
VOID CFaceItemCustomWnd::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//获取大小
	CRect rcLoad;
	m_btLoadImage.GetWindowRect(&rcLoad);

	//加载按钮
	m_btLoadImage.SetWindowPos(NULL,15,8,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btLoadCamera.SetWindowPos(NULL,25+rcLoad.Width(),8,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//获取大小
	CRect rcControl;
	m_btShowVLine.GetWindowRect(&rcControl);

	//控制按钮
	INT nXPos=(cx-rcControl.Width()*3-10)/2;
	m_btTurnLeft.SetWindowPos(NULL,nXPos,cy-27,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btTurnRight.SetWindowPos(NULL,nXPos+5+rcControl.Width(),cy-27,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btShowVLine.SetWindowPos(NULL,nXPos+10+rcControl.Width()*2,cy-27,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//移动控件
	m_ImageEditorControl.SetWindowPos(NULL,8,rcLoad.Height()+18,cx-16,cy-rcLoad.Height()-rcControl.Height()-28,SWP_NOZORDER);

	return;
}

//建立消息
INT CFaceItemCustomWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//变量定义
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=GetModuleHandle(AVATAR_CONTROL_DLL_NAME);

	//创建控件
	m_ImageEditorControl.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_IMAGE_EDIT_CTRL);

	//创建按钮
	m_btTurnLeft.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_TURN_LEFT);
	m_btTurnRight.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_TURN_RIGHT);
	m_btShowVLine.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_SHOW_VLINE);

	//创建按钮
	m_btLoadImage.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_LOAD_IMAGE);
	m_btLoadCamera.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_LOAD_CAMERA);

	//设置位图
	m_btLoadImage.SetButtonImage(IDB_BT_LOAD_IMAGE,hInstance,false,false);
	m_btLoadCamera.SetButtonImage(IDB_BT_LOAD_CAMERA,hInstance,false,false);

	//设置位图
	m_btTurnLeft.SetButtonImage(IDB_BT_FACE_BACK,TEXT("BT_TURN_LEFT"),hInstance,false,false);
	m_btTurnRight.SetButtonImage(IDB_BT_FACE_BACK,TEXT("BT_TURN_RIGHT"),hInstance,false,false);
	m_btShowVLine.SetButtonImage(IDB_BT_FACE_BACK,TEXT("BT_SHOW_VLINE"),hInstance,false,false);

	return 0;
}

//旋转图像
VOID CFaceItemCustomWnd::OnBnClickedTurnLeft()
{
	//选择图片
	m_ImageEditorControl.SetRoundImage(ROUND_TYPE_LEFT);

	return;
}

//旋转图像
VOID CFaceItemCustomWnd::OnBnClickedTurnRight()
{
	//选择图片
	m_ImageEditorControl.SetRoundImage(ROUND_TYPE_RIGHT);

	return;
}

//显示虚线
VOID CFaceItemCustomWnd::OnBnClickedShowVLine()
{
	//设置虚线
	bool bShowVLine=m_ImageEditorControl.GetVLineStatus();
	m_ImageEditorControl.SetVLineStatus((bShowVLine==false));

	return;
}

//加载图像
VOID CFaceItemCustomWnd::OnBnClickedLoadImage()
{
	//变量定义
	CFileDialog FileDialog(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,TEXT("图像文件（*.BMP *.PNG *.JPG）|*.BMP;*.PNG;*JPG||"),this);

	//打开文件
	if (FileDialog.DoModal()==IDOK)
	{
		//加载图片
		if (m_ImageEditorControl.LoadEditImage(FileDialog.GetPathName())==false)
		{
			//设置按钮
			m_btTurnLeft.ShowWindow(SW_HIDE);
			m_btTurnRight.ShowWindow(SW_HIDE);
			m_btShowVLine.ShowWindow(SW_HIDE);

			//设置控件
			m_ImageEditorControl.ShowWindow(SW_HIDE);

			//提示信息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("图片加载失败，请选择其他图片再试！"),MB_ICONERROR,0);
		}
		else
		{
			//设置按钮
			m_btTurnLeft.ShowWindow(SW_SHOW);
			m_btTurnRight.ShowWindow(SW_SHOW);
			m_btShowVLine.ShowWindow(SW_SHOW);

			//设置控件
			m_ImageEditorControl.ShowWindow(SW_SHOW);
		}
	}

	return;
}

//加载图像
VOID CFaceItemCustomWnd::OnBnClickedLoadCamera()
{
	//变量定义
	CCameraControl CameraControl(this);

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(AVATAR_CONTROL_DLL_NAME));

	//捕获图片
	if (CameraControl.DoModal()==IDOK)
	{
		//加载图片
		m_ImageEditorControl.ShowWindow(SW_SHOW);

		//加载图片
		ASSERT(CameraControl.m_ImageResult.IsNull()==false);
		bool bSuccess=m_ImageEditorControl.LoadEditImage(CameraControl.m_ImageResult);

		//设置按钮
		m_btTurnLeft.EnableWindow(bSuccess);
		m_btTurnRight.EnableWindow(bSuccess);
		m_btShowVLine.EnableWindow(bSuccess);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
