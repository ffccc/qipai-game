#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgEnquire.h"
#include "DlgSearchUser.h"
#include "PlatformFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//圆角大小
#define ROUND_CX					4									//圆角宽度
#define ROUND_CY					4									//圆角高度

//屏幕位置
#define BORAD_SIZE					5									//边框大小
#define CAPTION_SIZE				79									//标题大小

//屏幕限制
#define LESS_LIST_CX				285									//列表宽度
#define SPLITTER_WIDTH				8									//拆分宽度

//屏幕限制
#define LESS_SCREEN_CY				740									//最小高度
#define LESS_SCREEN_CX				1024								//最小宽度

//////////////////////////////////////////////////////////////////////////////////

//控制按钮
#define IDC_MIN						100									//最小按钮
#define IDC_MAX						101									//最大按钮
#define IDC_SKIN					102									//皮肤按钮
#define IDC_CLOSE					103									//关闭按钮

//控制按钮
#define IDC_CONTROL					110									//控制按钮
#define IDC_LOCK_LIST				111									//锁定按钮

//控件标识
#define IDC_SERVER_LIST				300									//游戏列表
#define IDC_WEB_PUBLICIZE			301									//浏览控件
#define IDC_SKIN_SPLITTER			302									//拆分控件
#define IDC_SYSTEM_TRAY_ICON		303									//任务图标

//控件标识
#define IDC_VIEW_ITEM_CTRL			310									//子项框架
#define IDC_USER_INFO_CTRL			311									//用户信息

//范围按钮
#define IDC_PLATFORM				400									//平台按钮
#define IDC_NAVIGATION				500									//导航按钮

//////////////////////////////////////////////////////////////////////////////////

//房间数目
#define MAX_SERVER_COUNT			5									//房间数目

//时间标识
#define IDI_FLASH_TRAY_ICON			10									//闪动图标

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CPlatformFrame * CPlatformFrame::m_pPlatformFrame=NULL;					//框架指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPlatformFrame, CFrameWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//按钮消息
	ON_BN_CLICKED((IDC_PLATFORM+0), OnBnClickedPlatform1)
	ON_BN_CLICKED((IDC_PLATFORM+1), OnBnClickedPlatform2)
	ON_BN_CLICKED((IDC_PLATFORM+2), OnBnClickedPlatform3)

	//自定消息
	ON_MESSAGE(WM_HOTKEY, OnMessageHotKey)
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CListEncircle::CListEncircle()
{
}

//析构函数
CListEncircle::~CListEncircle()
{
}

//绘画处理
bool CListEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//变量定义
	CDC * pDCL=CDC::FromHandle(EncircleImage.ImageTL.GetDC());
	CDC * pDCM=CDC::FromHandle(EncircleImage.ImageTM.GetDC());
	CDC * pDCR=CDC::FromHandle(EncircleImage.ImageTR.GetDC());

	//加载资源
	CPngImage ListTitleL;
	CPngImage ListTitleM;
	CPngImage ListTitleR;
	ListTitleL.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_L"));
	ListTitleM.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_M"));
	ListTitleR.LoadImage(AfxGetInstanceHandle(),TEXT("LIST_TITLE_R"));

	//构造资源
	ListTitleL.DrawImage(pDCL,0,0);
	ListTitleM.DrawImage(pDCM,0,0);
	ListTitleR.DrawImage(pDCR,0,0);

	//释放资源
	EncircleImage.ImageTL.ReleaseDC();
	EncircleImage.ImageTM.ReleaseDC();
	EncircleImage.ImageTR.ReleaseDC();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CFrameEncircle::CFrameEncircle()
{
}

//析构函数
CFrameEncircle::~CFrameEncircle()
{
}

//绘画处理
bool CFrameEncircle::PreDrawEncircleImage(tagEncircleBMP & EncircleImage)
{
	//变量定义
	CDC * pDC=CDC::FromHandle(EncircleImage.ImageTL.GetDC());

	//加载资源
	CPngImage StationLogo;
	StationLogo.LoadImage(AfxGetInstanceHandle(),TEXT("PLATFORM_LOGO"));

	//构造标志
	StationLogo.DrawImage(pDC,10,5);
	EncircleImage.ImageTL.ReleaseDC();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlatformFrame::CPlatformFrame()
{
	//界面变量
	m_bMaxShow=false;
	m_bRectify=false;
	m_rcNormalSize.SetRect(0,0,0,0);

	//状态变量
	m_bWhisperIcon=false;
	m_bHideGameList=false;
	m_bLockGameList=false;
	m_bAutoMenuEnable=FALSE;

	//房间变量
	m_pServerViewItem=NULL;

	//任务组件
	m_MissionManager.InsertMissionItem(&m_MissionList);
	m_MissionManager.InsertMissionItem(&m_MissionLogon);

	//平台变量
	ASSERT(m_pPlatformFrame==NULL);
	if (m_pPlatformFrame==NULL) m_pPlatformFrame=this;

	//设置接口
	CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
	pCustomFaceManager->SetCustomFaceEvent(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	//子项资源
	tagEncircleResource	EncircleItem;
	EncircleItem.pszImageTL=MAKEINTRESOURCE(IDB_ITEM_FRAME_TL);
	EncircleItem.pszImageTM=MAKEINTRESOURCE(IDB_ITEM_FRAME_TM);
	EncircleItem.pszImageTR=MAKEINTRESOURCE(IDB_ITEM_FRAME_TR);
	EncircleItem.pszImageML=MAKEINTRESOURCE(IDB_ITEM_FRAME_ML);
	EncircleItem.pszImageMR=MAKEINTRESOURCE(IDB_ITEM_FRAME_MR);
	EncircleItem.pszImageBL=MAKEINTRESOURCE(IDB_ITEM_FRAME_BL);
	EncircleItem.pszImageBM=MAKEINTRESOURCE(IDB_ITEM_FRAME_BM);
	EncircleItem.pszImageBR=MAKEINTRESOURCE(IDB_ITEM_FRAME_BR);
	m_ItemEncircle.InitEncircleResource(EncircleItem,AfxGetInstanceHandle());

	//列表资源
	tagEncircleResource	EncircleList;
	EncircleList.pszImageTL=MAKEINTRESOURCE(IDB_SERVER_LIST_TL);
	EncircleList.pszImageTM=MAKEINTRESOURCE(IDB_SERVER_LIST_TM);
	EncircleList.pszImageTR=MAKEINTRESOURCE(IDB_SERVER_LIST_TR);
	EncircleList.pszImageML=MAKEINTRESOURCE(IDB_SERVER_LIST_ML);
	EncircleList.pszImageMR=MAKEINTRESOURCE(IDB_SERVER_LIST_MR);
	EncircleList.pszImageBL=MAKEINTRESOURCE(IDB_SERVER_LIST_BL);
	EncircleList.pszImageBM=MAKEINTRESOURCE(IDB_SERVER_LIST_BM);
	EncircleList.pszImageBR=MAKEINTRESOURCE(IDB_SERVER_LIST_BR);
	m_ListEncircle.InitEncircleResource(EncircleList,AfxGetInstanceHandle());

	//框架环绕
	tagEncircleResource	EncircleFrame;
	EncircleFrame.pszImageTL=MAKEINTRESOURCE(IDB_FRAME_TL);
	EncircleFrame.pszImageTM=MAKEINTRESOURCE(IDB_FRAME_TM);
	EncircleFrame.pszImageTR=MAKEINTRESOURCE(IDB_FRAME_TR);
	EncircleFrame.pszImageML=MAKEINTRESOURCE(IDB_FRAME_ML);
	EncircleFrame.pszImageMR=MAKEINTRESOURCE(IDB_FRAME_MR);
	EncircleFrame.pszImageBL=MAKEINTRESOURCE(IDB_FRAME_BL);
	EncircleFrame.pszImageBM=MAKEINTRESOURCE(IDB_FRAME_BM);
	EncircleFrame.pszImageBR=MAKEINTRESOURCE(IDB_FRAME_BR);
	m_FrameEncircle.InitEncircleResource(EncircleFrame,AfxGetInstanceHandle());

	return;
}

//析构函数
CPlatformFrame::~CPlatformFrame()
{
	//平台变量
	ASSERT(m_pPlatformFrame==this);
	if (m_pPlatformFrame==this) m_pPlatformFrame=NULL;

	return;
}

//接口查询
VOID * CPlatformFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ICustomFaceEvent,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ICustomFaceEvent,Guid,dwQueryVer);
	return NULL;
}

//子项事件
VOID CPlatformFrame::OnViewItemEvent(WORD wItemIndex, CWnd * pItemView)
{
	//房间控制
	bool bShowList=true;
	bool bPlazaView=(m_PlazaViewItem.m_hWnd==pItemView->GetSafeHwnd());

	//房间判断
	if (bPlazaView==false) bShowList=m_bLockGameList;

	//列表控制
	ControlServerList((bShowList==true)?ServerListControl_Show:ServerListControl_Hide,false);

	return;
}

//拆分事件
VOID CPlatformFrame::OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//显示控件
	if (m_bHideGameList==true)
	{
		//设置变量
		m_bRectify=true;
		m_bHideGameList=false;
	
		//设置按钮
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_btControl.SetButtonImage(IDB_BK_GAME_LIST,(m_bHideGameList==true)?TEXT("BT_GAME_LIST_SHOW"):TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);
	}

	//调整控件
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//下载失败
VOID CPlatformFrame::OnEventSystemFace(DWORD dwUserID, WORD wFaceID)
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//自我数据
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//设置变量
		pGlobalUserData->wFaceID=wFaceID;

		//清理数据
		pGlobalUserData->dwCustomID=0L;
		ZeroMemory(&pGlobalUserData->CustomFaceInfo,sizeof(pGlobalUserData->CustomFaceInfo));

		//发送事件
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	return;
}

//头像数据
VOID CPlatformFrame::OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//自我数据
	if (pGlobalUserData->dwUserID==dwUserID)
	{
		//设置变量
		pGlobalUserData->dwCustomID=dwCustomID;
		CopyMemory(&pGlobalUserData->CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

		//发送事件
		CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
		if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
	}

	//当前房间
	if (m_pServerViewItem!=NULL)
	{
		m_pServerViewItem->UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	}

	//更新房间
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		m_ServerViewItemArray[i]->UpdateUserCustomFace(dwUserID,dwCustomID,CustomFaceInfo);
	}

	return;
}

//消息过虑
BOOL CPlatformFrame::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CPlatformFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//功能按钮
	switch (nCommandID)
	{
	case IDC_MIN:				//最小按钮
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_MAX:				//最大按钮
		{
			//窗口控制
			if (m_bMaxShow==true)
			{
				RestoreWindow();
			}
			else
			{
				MaxSizeWindow();
			}

			//更新界面
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return TRUE;
		}
	case IDC_CLOSE:				//关闭按钮
		{
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		}
	case IDC_SKIN:				//界面控制
		{
			//创建窗口
			if (m_DlgSkinControl.m_hWnd==NULL)
			{
				m_DlgSkinControl.Create(IDD_SKIN_CONTROL,this);
			}

			//获取位置
			CRect rcButton;
			CRect rcControl;
			m_btSkin.GetWindowRect(&rcButton);
			m_DlgSkinControl.GetWindowRect(&rcControl);

			//显示窗口
			UINT uFlags=SWP_NOZORDER|SWP_NOSIZE|SWP_SHOWWINDOW;
			m_DlgSkinControl.SetWindowPos(NULL,rcButton.right-rcControl.Width(),rcButton.bottom,0,0,uFlags);

			return TRUE;
		}
	case IDC_CONTROL:			//控制按钮
		{
			//控制列表
			ControlServerList(ServerListControl_Turn,true);

			return TRUE;
		}
	case IDC_LOCK_LIST:			//锁定列表
		{
			//设置变量
			m_bLockGameList=!m_bLockGameList;

			//设置按钮
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_btLockList.SetButtonImage(IDB_BK_LIST,(m_bLockGameList==true)?TEXT("BT_UNLOCK_LIST"):TEXT("BT_LOCK_LIST"),hInstance,true,false);

			return TRUE;
		}
	case IDC_NAVIGATION+0:
	case IDC_NAVIGATION+1:
	case IDC_NAVIGATION+2:
	case IDC_NAVIGATION+3:
	case IDC_NAVIGATION+4:		//导航按钮
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//构造地址
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Navigation%ld.asp"),szPlatformLink,(nCommandID-IDC_NAVIGATION)+1);

			//连接地址
			WebBrowse(szNavigation,true);

			return TRUE;
		}
	}

	//菜单命令
	switch (nCommandID)
	{
	case IDM_USER_LOGON:		//用户登录
		{
			m_MissionLogon.ShowLogon();

			return TRUE;
		}
	case IDM_SWITCH_ACCOUNTS:	//切换帐号
		{
			//切换询问
			if ((m_ServerViewItemArray.GetCount()>0L)||(m_pServerViewItem!=NULL))
			{
				CInformation Information(this);
				if (Information.ShowMessageBox(TEXT("所有的游戏房间即将关闭，确实要“切换帐号”吗？ "),MB_YESNO)!=IDYES) return TRUE;
			}

			//全局设置
			CGlobalUnits::GetInstance()->DeleteUserCookie();
			CGlobalUserInfo::GetInstance()->ResetUserInfoData();

			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_LOGOUT,0L);

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//调整控件
VOID CPlatformFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if ((nWidth==0)||(nHeight==0)) return;

	//拆分位置
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//环绕信息
	tagEncircleInfo EncircleInfoList;
	tagEncircleInfo EncircleInfoItem;
	tagEncircleInfo EncircleInfoFrame;
	m_ListEncircle.GetEncircleInfo(EncircleInfoList);
	m_ItemEncircle.GetEncircleInfo(EncircleInfoItem);
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);

	//获取位置
	ScreenToClient(&rcSplitter);

	//拆分控件
	if (m_bHideGameList==false)
	{
		//拆分范围
		INT nLessPos=nWidth*28/100,nMaxPos=nWidth/2;
		if (nLessPos>=LESS_LIST_CX) nLessPos=LESS_LIST_CX;

		//调整拆分
		if (m_bRectify==false)
		{
			m_bRectify=true;
			rcSplitter.left=nLessPos;
			rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
		}

		//拆分范围
		m_SkinSplitter.SetSplitterRange(nLessPos,nMaxPos);

		//调整位置
		if ((nWidth!=0)&&(nHeight!=0))
		{
			if (rcSplitter.left<nLessPos)
			{
				rcSplitter.left=nLessPos;
				rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
			}
			if (rcSplitter.right>nMaxPos)
			{
				rcSplitter.right=nMaxPos;
				rcSplitter.left=rcSplitter.right-SPLITTER_WIDTH;
			}
		}
	}
	else 
	{
		//拆分位置
		rcSplitter.left=EncircleInfoFrame.nLBorder;
		rcSplitter.right=rcSplitter.left+SPLITTER_WIDTH;
	}

	//位置调整
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=nHeight-EncircleInfoFrame.nBBorder;

	//控件位置
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//控制按钮
	DeferWindowPos(hDwp,m_btMin,NULL,nWidth-94,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btMax,NULL,nWidth-66,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSkin,NULL,nWidth-122,0,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-38,0,0,0,uFlags|SWP_NOSIZE);

	//导航按钮
	CRect rcNavigation;
	m_btNavigation[0].GetWindowRect(&rcNavigation);

	//导航按钮
	INT nNavigationArea=rcNavigation.Width()*CountArray(m_btNavigation);
	INT nSpace=__min(__max((nWidth-nNavigationArea-700)/(CountArray(m_btNavigation)+1),0),36);

	//导航按钮
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		INT nIndex=CountArray(m_btNavigation)-i;
		INT nXExcursion=nWidth-(rcNavigation.Width()+nSpace)*nIndex-nSpace-100;
		DeferWindowPos(hDwp,m_btNavigation[i],NULL,nXExcursion,8,0,0,uFlags|SWP_NOSIZE);
	}

	//平台按钮
	CRect rcPlatform;
	m_btPlatform[0].GetWindowRect(&rcPlatform);

	//平台按钮
	INT nYPlatform=EncircleInfoFrame.nTBorder+2L;
	DeferWindowPos(hDwp,m_btPlatform[0],NULL,nWidth-(rcPlatform.Width()+7)*3-EncircleInfoFrame.nRBorder-10,nYPlatform,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btPlatform[1],NULL,nWidth-(rcPlatform.Width()+7)*2-EncircleInfoFrame.nRBorder-10,nYPlatform,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btPlatform[2],NULL,nWidth-(rcPlatform.Width()+7)*1-EncircleInfoFrame.nRBorder-10,nYPlatform,0,0,uFlags|SWP_NOSIZE);

	//锁定按钮
	CRect rcLockList;
	m_btLockList.GetWindowRect(&rcLockList);
	DeferWindowPos(hDwp,m_btLockList,NULL,rcSplitter.left-rcLockList.Width()-13,EncircleInfoFrame.nTBorder+rcUserInfoCtrl.Height()+4,
		rcLockList.Width(),rcLockList.Height(),uFlags);

	//页面位置
	m_WndViewItemCtrl.SetViewItemPostion(rcSplitter.right+EncircleInfoItem.nLBorder,EncircleInfoFrame.nTBorder+EncircleInfoItem.nTBorder,
		nWidth-EncircleInfoFrame.nRBorder-EncircleInfoItem.nRBorder-EncircleInfoItem.nLBorder-rcSplitter.right,
		nHeight-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder-EncircleInfoItem.nTBorder-EncircleInfoItem.nBBorder);

	//子项框架
	CSize SizeItemFrame;
	m_WndViewItemCtrl.GetViewItemSize(SizeItemFrame);
	DeferWindowPos(hDwp,m_WndViewItemCtrl,NULL,rcSplitter.right+10,EncircleInfoFrame.nTBorder+EncircleInfoItem.nTBorder-SizeItemFrame.cy,
		SizeItemFrame.cx,SizeItemFrame.cy,uFlags);

	//其他控件
	DeferWindowPos(hDwp,m_PlatformPublicize,NULL,320,7,270,65,uFlags);
	DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_WndUserInfoCtrl,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcSplitter.left-EncircleInfoFrame.nLBorder,rcUserInfoCtrl.Height(),uFlags);
	DeferWindowPos(hDwp,m_ServerListView,NULL,EncircleInfoFrame.nLBorder+EncircleInfoList.nLBorder,EncircleInfoFrame.nTBorder+EncircleInfoList.nTBorder+rcUserInfoCtrl.Height(),
		rcSplitter.left-EncircleInfoList.nLBorder-EncircleInfoList.nRBorder-EncircleInfoFrame.nLBorder,nHeight-EncircleInfoList.nTBorder-EncircleInfoList.nBBorder
		-EncircleInfoFrame.nTBorder-rcUserInfoCtrl.Height()-EncircleInfoFrame.nBBorder,uFlags);

	//结束调整
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//控制按钮
	CRect rcControl;
	m_btControl.GetClientRect(&rcControl);
	m_btControl.SetWindowPos(NULL,(rcSplitter.Width()-rcControl.Width())/2,(rcSplitter.Height()-rcControl.Height())/2,0,0,uFlags|SWP_NOSIZE);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//浏览页面
bool CPlatformFrame::WebBrowse(LPCTSTR pszURL, bool bAutoFullView)
{
	//浏览页面
	CPlazaViewItem::GetInstance()->WebBrowse(pszURL);

	//激活大厅
	m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);

	//界面控制
	if (bAutoFullView==true)
	{
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
		if (pParameterGlobal->m_bFullScreenBrowse==true) ControlServerList(ServerListControl_Hide,false);
	}

	return true;
}

//列表控制
bool CPlatformFrame::ControlServerList(enServerListControl ServerListControl, bool bFocusControl)
{
	//位置控制
	bool bHideGameList=false;
	switch (ServerListControl)
	{
	case ServerListControl_Hide: { bHideGameList=true; break; }
	case ServerListControl_Show: { bHideGameList=false; break; }
	case ServerListControl_Turn: { bHideGameList=!m_bHideGameList; break; }
	}

	//控制过滤
	if ((bHideGameList==true)&&(bFocusControl==false)&&(m_bLockGameList==true))
	{
		return true;
	}

	//调整判断
	if (bHideGameList!=m_bHideGameList)
	{
		//设置变量
		m_bRectify=false;
		m_bHideGameList=bHideGameList;

		//设置按钮
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_btControl.SetButtonImage(IDB_BK_GAME_LIST,(m_bHideGameList==true)?TEXT("BT_GAME_LIST_SHOW"):TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);

		//调整界面
		CRect rcClient;
		GetClientRect(&rcClient);
		RectifyControl(rcClient.Width(),rcClient.Height());
	}

	return true;
}

//提取私聊
bool CPlatformFrame::ShowWhisperItem()
{
	//提取消息
	if (m_DlgWhisperItemArray.GetCount()>0L)
	{
		//获取子项
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[0];

		//显示窗口
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//置顶窗口
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();
		pDlgWhisper->SetForegroundWindow();

		//删除子项
		m_DlgWhisperItemArray.RemoveAt(0L);

		//删除图标
		if (m_DlgWhisperItemArray.GetCount()==0L)
		{
			//设置变量
			m_bWhisperIcon=false;

			//删除时间
			KillTimer(IDI_FLASH_TRAY_ICON);

			//设置图标
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
		}

		return true;
	}

	return false;
}

//插入私聊
bool CPlatformFrame::InsertWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//参数效验
	ASSERT((pDlgWhisper!=NULL)&&(pDlgWhisper->m_hWnd!=NULL));
	if ((pDlgWhisper==NULL)||(pDlgWhisper->m_hWnd==NULL)) return false;

	//状态判断
	if (pDlgWhisper->IsWindowVisible()) return false;

	//存在搜索
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper) return false;
	}

	//插入私聊
	m_DlgWhisperItemArray.Add(pDlgWhisper);

	//设置图标
	if (m_DlgWhisperItemArray.GetCount()==1L)
	{
		//设置变量
		m_bWhisperIcon=true;

		//设置时间
		SetTimer(IDI_FLASH_TRAY_ICON,500,NULL);

		//设置图标
		m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WHISPER)));
	}

	return true;
}

//删除私聊
bool CPlatformFrame::RemoveWhisperItem(CDlgWhisper * pDlgWhisper)
{
	//存在搜索
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		if (m_DlgWhisperItemArray[i]==pDlgWhisper)
		{
			//删除子项
			m_DlgWhisperItemArray.RemoveAt(i);

			//删除图标
			if (m_DlgWhisperItemArray.GetCount()==0L)
			{
				//设置变量
				m_bWhisperIcon=false;

				//删除时间
				KillTimer(IDI_FLASH_TRAY_ICON);

				//设置图标
				m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
			}

			return true;
		}
	}

	return false;
}

//激活房间
bool CPlatformFrame::ActiveServerViewItem(CServerViewItem * pServerViewItem)
{
	//插入判断
	if (m_pServerViewItem==pServerViewItem)
	{
		//设置变量
		m_pServerViewItem=NULL;

		//插入房间
		m_ServerViewItemArray.Add(pServerViewItem);

		//插入标签
		LPCTSTR pszKindName(pServerViewItem->GetKindName());
		m_WndViewItemCtrl.InsertViewItem(pServerViewItem,pszKindName);
	}

	//激活房间
	m_WndViewItemCtrl.ActiveViewItem(pServerViewItem);

	return true;
}

//删除房间
bool CPlatformFrame::DeleteServerViewItem(CServerViewItem * pServerViewItem)
{
	//当前房间
	if ((m_pServerViewItem!=NULL)&&(m_pServerViewItem==pServerViewItem))
	{
		//设置变量
		m_pServerViewItem=NULL;

		//销毁房间
		pServerViewItem->DestroyWindow();

		return true;
	}

	//搜索房间
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		if (m_ServerViewItemArray[i]==pServerViewItem)
		{
			//销毁房间
			pServerViewItem->DestroyWindow();
			m_ServerViewItemArray.RemoveAt(i);

			//删除房间
			m_WndViewItemCtrl.RemoveViewItem(pServerViewItem);
			m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);

			//搜索通知
			CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
			if (pDlgSearchUser!=NULL) pDlgSearchUser->OnViewItemDelete(pServerViewItem);

			return true;
		}
	}

	return false;
}

//枚举房间
CServerViewItem * CPlatformFrame::EmunServerViewItem(WORD wIndex)
{
	//索引判断
	if (wIndex<m_ServerViewItemArray.GetCount())
	{
		return m_ServerViewItemArray[wIndex];
	}

	return NULL;
}

//查找房间
CServerViewItem * CPlatformFrame::SearchServerViewItem(WORD wServerID)
{
	//查找房间
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];
		if (pServerViewItem->GetServerID()==wServerID) return pServerViewItem;
	}

	return NULL;
}

//进入房间
CServerViewItem * CPlatformFrame::EntranceServerItem(CGameKindItem * pGameKindItem)
{
	return NULL;
}

//进入房间
CServerViewItem * CPlatformFrame::EntranceServerItem(CGameServerItem * pGameServerItem)
{
	//效验参数
	ASSERT(pGameServerItem!=NULL);
	if (pGameServerItem==NULL) return NULL;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//登录判断
	if (pGlobalUserData->dwUserID==0L) return NULL;

	//变量定义
	tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
	tagGameKind * pGameKind=&pGameServerItem->m_pGameKindItem->m_GameKind;

	//查找房间
	WORD wServerID=pGameServer->wServerID;
	CServerViewItem * pServerViewItem=SearchServerViewItem(wServerID);

	//激活房间
	if (pServerViewItem!=NULL)
	{
		ActiveServerViewItem(pServerViewItem);
		return pServerViewItem;
	}

	//当前判断
	if (m_pServerViewItem!=NULL)
	{
		//相同判断
		if (wServerID==m_pServerViewItem->GetServerID()) return pServerViewItem;

		//提示消息
		TCHAR szBuffer[256]=TEXT("");
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("正在进入 [ %s ] 游戏房间中，请稍后等待片刻..."),m_pServerViewItem->GetServerName());

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(szBuffer,MB_OK|MB_ICONWARNING,30);

		return m_pServerViewItem;
	}

	//会员房间
	if (m_ServerViewItemArray.GetCount()>=MAX_SERVER_COUNT)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("您进入的游戏房间太多了，请关闭部分后再次尝试！"),MB_ICONQUESTION,60);

		return NULL;
	}

	//安装判断
	if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
	{
		//获取版本
		CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
		CWHService::GetModuleVersion(pGameKind->szProcessName,pGameKindItem->m_dwProcessVersion);

		//下载游戏
		if (pGameServerItem->m_pGameKindItem->m_dwProcessVersion==0L)
		{
			CGlobalUnits * pCGlobalUnits=CGlobalUnits::GetInstance();
			pCGlobalUnits->DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,wServerID);

			return NULL;
		}

		//更新列表
		m_ServerListView.OnGameItemUpdate(pGameKindItem);
	}

	//创建房间
	try
	{
		//创建房间
		m_pServerViewItem=new CServerViewItem;
		m_pServerViewItem->Create(IDD_GAME_SERVER,this);

		//配置房间
		m_pServerViewItem->InitServerViewItem(pGameServerItem);
	}
	catch (LPCTSTR pszString)
	{
		//销毁房间
		if (m_pServerViewItem!=NULL)
		{
			if (m_pServerViewItem->m_hWnd!=NULL)
			{
				m_pServerViewItem->DestroyWindow();
			}
			else
			{
				SafeDelete(m_pServerViewItem);
			}
		}

		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(pGameServer->szServerName,pszString,MB_ICONERROR);

		return NULL;
	}

	return m_pServerViewItem;
}

//还原窗口
bool CPlatformFrame::RestoreWindow()
{
	//状态判断
	if (m_bMaxShow==true)
	{
		//设置变量
		m_bMaxShow=false;
		m_bRectify=false;

		//设置按钮
		m_btMax.SetButtonImage(IDB_BT_MAX,AfxGetInstanceHandle(),true,false);

		//移动窗口
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//最大窗口
bool CPlatformFrame::MaxSizeWindow()
{
	//状态判断
	if (m_bMaxShow==false)
	{
		//设置变量
		m_bMaxShow=true;
		m_bRectify=false;

		//默认位置
		GetWindowRect(&m_rcNormalSize);

		//设置按钮
		m_btMax.SetButtonImage(IDB_BT_RESORE,AfxGetInstanceHandle(),true,false);

		//获取位置
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//移动窗口
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//关闭消息
VOID CPlatformFrame::OnClose()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//关闭提示
	if ((pGlobalUserData->dwUserID!=0L)&&(m_MissionLogon.GetActiveStatus()==false))
	{
		//显示窗口
		CDlgEnquire DlgEnquire;
		INT_PTR nResult=DlgEnquire.DoModal();

		//命令处理
		switch (nResult)
		{
		case IDCANCEL:				//取消动作
			{
				return;
			}
		case IDC_CLOSE_SERVER:		//退出房间
			{
				//关闭房间
				ASSERT(m_WndViewItemCtrl.GetActiveItem()>0);
				WORD wActiveItem=m_WndViewItemCtrl.GetActiveItem();
				
				//关闭房间
				if (wActiveItem>=1)
				{
					ASSERT((wActiveItem-1)<m_ServerViewItemArray.GetCount());
					DeleteServerViewItem(m_ServerViewItemArray[wActiveItem-1]);
				}

				return;
			}
		case IDC_SWITCH_ACCOUNTS:	//切换帐号
			{
				//投递消息
				PostMessage(WM_COMMAND,IDM_SWITCH_ACCOUNTS,0);

				return;
			}
		}
	}

	//注销热键
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS);
	UnregisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER);

	__super::OnClose();
}

//绘画背景
BOOL CPlatformFrame::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//框架位置
	tagEncircleInfo ItemEncircleInfo;
	tagEncircleInfo FrameEncircleInfo;
	m_ItemEncircle.GetEncircleInfo(ItemEncircleInfo);
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);

	//拆分位置
	CRect rcSplitter;
	m_SkinSplitter.GetWindowRect(&rcSplitter);

	//广告位置
	CRect rcWebPublicize;
	m_PlatformPublicize.GetWindowRect(&rcWebPublicize);

	//位置转换
	ScreenToClient(&rcSplitter);
	ScreenToClient(&rcWebPublicize);

	//控件位置
	CRect rcUserInfoCtrl;
	m_WndUserInfoCtrl.GetWindowRect(&rcUserInfoCtrl);

	//游戏列表
	if (m_bHideGameList==false)
	{
		//列表位置
		CRect rcList;
		rcList.right=rcSplitter.left;
		rcList.left=FrameEncircleInfo.nLBorder;
		rcList.top=FrameEncircleInfo.nTBorder+rcUserInfoCtrl.Height();
		rcList.bottom=rcClient.Height()-FrameEncircleInfo.nBBorder;

		//建立缓冲
		CImage ImageBuffer;
		ImageBuffer.Create(rcList.Width(),rcList.Height(),32);

		//创建 DC
		CDC BufferDC;
		BufferDC.CreateCompatibleDC(pDC);
		BufferDC.SelectObject(ImageBuffer);

		//设置环境
		BufferDC.SetBkMode(TRANSPARENT);
		BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//绘画列表
		CRect rcBuffer(0,0,rcList.Width(),rcList.Height());
		m_ListEncircle.DrawEncircleFrame(&BufferDC,rcBuffer);

		//构造字符
		TCHAR szOnLineInfo[32]=TEXT("");
		_sntprintf(szOnLineInfo,CountArray(szOnLineInfo),TEXT("同时在线人数：%ld"),3000000);

		//绘画信息
		BufferDC.SetTextColor(RGB(0,0,0));
		//BufferDC.TextOut(10,7,szOnLineInfo,lstrlen(szOnLineInfo));

		//绘画界面
		pDC->BitBlt(rcList.left,rcList.top,rcList.Width(),rcList.Height(),&BufferDC,0,0,SRCCOPY);

		//清理资源
		BufferDC.DeleteDC();
		ImageBuffer.Destroy();
	}

	//子项框架
	CRect rcItem;
	rcItem.left=rcSplitter.right;
	rcItem.top=FrameEncircleInfo.nTBorder;
	rcItem.right=rcClient.Width()-FrameEncircleInfo.nRBorder;
	rcItem.bottom=rcClient.Height()-FrameEncircleInfo.nBBorder;

	//绘画框架
	m_ItemEncircle.DrawEncircleFrame(pDC,rcItem);
	m_FrameEncircle.DrawEncircleFrame(pDC,rcClient);

	return TRUE;
}

//按键测试
UINT CPlatformFrame::OnNcHitTest(CPoint Point)
{
	//按钮测试
	if (m_bMaxShow==false)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//转换位置
		CPoint ClientPoint=Point;
		ScreenToClient(&ClientPoint);

		//左面位置
		if (ClientPoint.x<=BORAD_SIZE)
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPLEFT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMLEFT;
			return HTLEFT;
		}

		//右面位置
		if (ClientPoint.x>=(rcClient.Width()-BORAD_SIZE))
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPRIGHT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMRIGHT;
			return HTRIGHT;
		}

		//上下位置
		if (ClientPoint.y<=BORAD_SIZE) return HTTOP;
		if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOM;
	}

	return __super::OnNcHitTest(Point);
}

//时间消息
VOID CPlatformFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_FLASH_TRAY_ICON:	//任务图标
		{
			//设置变量
			m_bWhisperIcon=!m_bWhisperIcon;

			//设置图标
			UINT uIconID=(m_bWhisperIcon==true)?IDI_WHISPER:IDI_NULL;
			m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(uIconID)));

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//位置消息
VOID CPlatformFrame::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	//创建区域
	CRgn WindowRgn;
	WindowRgn.CreateRoundRectRgn(0,0,cx+1,cy+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(WindowRgn,TRUE);

	return;
}

//位置信息
VOID CPlatformFrame::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//设置位置
	lpMMI->ptMinTrackSize.x=__min(LESS_SCREEN_CX,rcArce.Width());
	lpMMI->ptMinTrackSize.y=__min(LESS_SCREEN_CY,rcArce.Height());

	return;
}

//建立消息
INT CPlatformFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置窗口
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//创建广场
	CRect rcCreate(0,0,0,0);
	m_PlazaViewItem.Create(IDD_GAME_PLAZA,this);

	//创建控件
	m_ServerListView.Create(WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SERVER_LIST);
	m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);

	//窗口控件
	m_WndViewItemCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_VIEW_ITEM_CTRL);
	m_WndUserInfoCtrl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_USER_INFO_CTRL);
	m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//创建按钮
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btMax.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MAX);
	m_btSkin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);
	m_btControl.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CONTROL);
	m_btLockList.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_LOCK_LIST);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,true,false);
	m_btMax.SetButtonImage(IDB_BT_MAX,hInstance,true,false);
	m_btSkin.SetButtonImage(IDB_BT_SKIN,hInstance,true,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,TEXT("BT_CLOSE"),hInstance,true,false);
	m_btLockList.SetButtonImage(IDB_BK_LIST,TEXT("BT_LOCK_LIST"),hInstance,true,false);

	//控制按钮
	m_btControl.SetParent(&m_SkinSplitter);
	m_btControl.SetButtonImage(IDB_BK_GAME_LIST,TEXT("BT_GAME_LIST_HIDE"),hInstance,true,false);

	//平台按钮
	for (INT i=0;i<CountArray(m_btPlatform);i++)
	{
		m_btPlatform[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_PLATFORM+i);
	}

	//导航按钮
	for (INT i=0;i<CountArray(m_btNavigation);i++)
	{
		m_btNavigation[i].Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_NAVIGATION+i);
	}

	//平台按钮
	m_btPlatform[0].SetButtonImage(IDB_BK_PLATFORM_1,TEXT("BT_PLATFORM_1"),hInstance,true,false);
	m_btPlatform[1].SetButtonImage(IDB_BK_PLATFORM_2,TEXT("BT_PLATFORM_2"),hInstance,true,false);
	m_btPlatform[2].SetButtonImage(IDB_BK_PLATFORM_3,TEXT("BT_PLATFORM_3"),hInstance,true,false);

	//导航按钮
	m_btNavigation[0].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_1"),hInstance,true,false);
	m_btNavigation[1].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_2"),hInstance,true,false);
	m_btNavigation[2].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_3"),hInstance,true,false);
	m_btNavigation[3].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_4"),hInstance,true,false);
	m_btNavigation[4].SetButtonImage(IDB_BK_NAVIGATION,TEXT("BT_NAVIGATION_5"),hInstance,true,false);

	//拆分设置
	m_SkinSplitter.SetSplitterEvent(this);
	m_SkinSplitter.SetSplitterColor(0.67,0.95);
	m_SkinSplitter.SetSplitterType(SplitterType_Vor);

	//游戏列表
	m_ServerListView.InitServerTreeView();
	CServerListData::GetInstance()->SetServerListDataSink(&m_ServerListView);

	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("最小化"));
	m_ToolTipCtrl.AddTool(&m_btMax,TEXT("最大化"));
	m_ToolTipCtrl.AddTool(&m_btSkin,TEXT("界面换肤"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("关闭游戏"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[0],TEXT("官方首页"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[1],TEXT("上传头像"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[2],TEXT("帐号充值"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[3],TEXT("道具商城"));
	m_ToolTipCtrl.AddTool(&m_btNavigation[4],TEXT("会员用户"));
	m_ToolTipCtrl.AddTool(&m_btControl,TEXT("隐藏（显示）游戏列表"));

	//系统控制
	m_WndViewItemCtrl.SetViewItemEvent(this);
	m_WndViewItemCtrl.InsertViewItem(&m_PlazaViewItem,TEXT("游戏广场"));

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//设置位置
	CSize SizeRestrict;
	SizeRestrict.SetSize(LESS_SCREEN_CX,LESS_SCREEN_CY);

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//读取位置
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//位置调整
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//移动窗口
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/Platform.asp"),szPlatformLink);

	//连接广告
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetBoradColor(RGB(0,0,0));

	//显示窗口
	MaxSizeWindow();

	//登录系统
	PostMessage(WM_COMMAND,IDM_USER_LOGON,0);

	//注册热键
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_BOSS,pParameterGlobal->m_wBossHotKey);
	CWHService::RegisterHotKey(m_hWnd,IDI_HOT_KEY_WHISPER,pParameterGlobal->m_wWhisperHotKey);

	//设置托盘
	m_SystemTrayIcon.InitTrayIcon(m_hWnd,IDC_SYSTEM_TRAY_ICON);
	m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)),szProduct);

	return 0;
}

//鼠标消息
VOID CPlatformFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//模拟按标题
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//鼠标消息
VOID CPlatformFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//状态判断
	if (Point.y>CAPTION_SIZE) return;

	//窗口控制
	if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}
	
	return;
}

//设置改变
VOID CPlatformFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//获取大小
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);
	ClientToScreen(&rcClient);

	//计算位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,0);

	//调整位置
	rcArce.top-=(rcClient.top-rcWindow.top);
	rcArce.left-=(rcClient.left-rcWindow.left);
	rcArce.right+=(rcWindow.right-rcClient.right);
	rcArce.bottom+=(rcWindow.bottom-rcClient.bottom);

	//移动窗口
	SetWindowPos(NULL,rcArce.left,rcArce.top,rcArce.Width(),rcArce.Height(),SWP_NOZORDER|SWP_NOCOPYBITS);

	return;
}

//平台按钮
VOID CPlatformFrame::OnBnClickedPlatform1()
{
	//查找用户
	CDlgSearchUser DlgSearchUser;
	DlgSearchUser.DoModal();

	return;
}

//平台按钮
VOID CPlatformFrame::OnBnClickedPlatform2()
{
	//变量定义
	bool bGoldServer=false;
	CServerViewItem * pServerViewGold=NULL;

	//查找房间
	for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
	{
		//获取房间
		ASSERT(m_ServerViewItemArray[i]!=NULL);
		CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];

		//房间类型
		WORD wServerType=pServerViewItem->GetServerType();
		enServiceStatus ServiceStatus=pServerViewItem->GetServiceStatus();

		//房间判断
		if (((wServerType&GAME_GENRE_GOLD)!=0)&&(ServiceStatus==ServiceStatus_ServiceIng))
		{
			//设置变量
			bGoldServer=true;
			pServerViewGold=pServerViewItem;

			break;
		}
	}

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(SHARE_CONTROL_DLL_NAME));

	//银行中心
	if (bGoldServer==false)
	{
		//广场银行
		CDlgInsurePlaza DlgInsurePlaza;
		INT_PTR nResult=DlgInsurePlaza.DoModal();
	}
	else
	{
		//房间银行
		ASSERT(pServerViewGold!=NULL);
		pServerViewGold->ShowInsureView();
	}

	return;
}

//平台按钮
VOID CPlatformFrame::OnBnClickedPlatform3()
{
	//变量定义
	CParameterGame * pParameterGame=NULL;
	CParameterServer * pParameterServer=NULL;

	//房间变量
	WORD wActiveItem=m_WndViewItemCtrl.GetActiveItem();
	tagViewItemInfo * pViewItemInfo=m_WndViewItemCtrl.GetViewItemInfo(wActiveItem);

	//房间搜索
	if ((wActiveItem>=1)&&(pViewItemInfo!=NULL))
	{
		//获取窗口
		CWnd * pItemView=pViewItemInfo->pItemView;
		CServerViewItem * pServerViewItem=(CServerViewItem *)pItemView;

		//设置变量
		if (pServerViewItem!=NULL)
		{
			pParameterGame=pServerViewItem->GetParameterGame();
			pParameterServer=pServerViewItem->GetParameterServer();
		}

	}

	//构造变量
	CDlgCollocate DlgCollocate;
	DlgCollocate.InitCollocate(pParameterGame,pParameterServer);

	//显示配置
	if (DlgCollocate.DoModal()==IDOK)
	{
		//发送规则
		for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
		{
			//获取窗口
			CServerViewItem * pServerViewItem=m_ServerViewItemArray[i];
			if (pServerViewItem->GetServiceStatus()==ServiceStatus_ServiceIng) pServerViewItem->SendUserRulePacket();
		}
	}

	return;
}

//热键消息
LRESULT CPlatformFrame::OnMessageHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDI_HOT_KEY_BOSS:		//老板热键
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//登录判断
			if (pGlobalUserData->dwUserID!=0L)
			{
				//变量定义
				bool bBossCome=(IsWindowVisible()==FALSE)?false:true;

				//隐藏窗口
				if (bBossCome==false)
				{
					//还原窗口
					ShowWindow(SW_RESTORE);
					ShowWindow(SW_SHOW);

					//置顶窗口
					SetActiveWindow();
					BringWindowToTop();
					SetForegroundWindow();

					//显示图标
					m_SystemTrayIcon.ShowTrayIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)));
				}
				else
				{
					//隐藏窗口
					ShowWindow(SW_MINIMIZE);
					ShowWindow(SW_HIDE);

					//隐藏图标
					m_SystemTrayIcon.HideTrayIcon();
				}

				//发送消息
				for (INT_PTR i=0;i<m_ServerViewItemArray.GetCount();i++)
				{
					ASSERT(m_ServerViewItemArray[i]!=NULL);
					m_ServerViewItemArray[i]->NotifyBossCome(bBossCome);
				}
			}

			return 0;
		}
	case IDI_HOT_KEY_WHISPER:	//私聊热键
		{
			//提取消息
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}

			return 0;
		}
	}

	return 0;
}

//图标消息
LRESULT CPlatformFrame::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//鼠标单击
	case WM_LBUTTONDBLCLK:		//鼠标双击
		{
			//提取消息
			if (m_DlgWhisperItemArray.GetCount()>0L)
			{
				ShowWhisperItem();
				return 1;
			}

			return 1;
		}
	/*case WM_RBUTTONDOWN:		//鼠标单击
	case WM_RBUTTONDBLCLK:		//鼠标双击
		{
			//创建菜单
			CSkinMenu Menu;
			Menu.CreateMenu();

			//构造菜单
			Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("显示界面"));
			Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("退出广场"));
			Menu.AppendSeparator();
			Menu.AppendMenu(IDM_NULL_COMMAND,TEXT("关于..."));

			//显示菜单
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

			return 1;
		}*/
	}

	return 0;
}

//事件消息
LRESULT CPlatformFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录完成
		{
			//显示窗口
			ShowWindow(SW_SHOW);
			SetForegroundWindow();

			//游戏列表
			ControlServerList(ServerListControl_Show,false);

			//用户甜饼
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->WriteUserCookie();

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//注销成功
		{
			//激活广场
			m_WndViewItemCtrl.ActiveViewItem(&m_PlazaViewItem);

			//关闭房间
			if (m_pServerViewItem!=NULL)
			{
				DeleteServerViewItem(m_pServerViewItem);
			}

			//关闭房间
			while (m_ServerViewItemArray.GetCount()>0)
			{
				DeleteServerViewItem(m_ServerViewItemArray[0]);
			}

			//隐藏窗口
			ShowWindow(SW_HIDE);

			//显示登录
			m_MissionLogon.ShowLogon();

			return 0L;
		}
	case EVENT_DOWN_LOAD_FINISH:	//下载完成
		{
			//获取参数
			WORD wKindID=LOWORD(lParam);
			WORD wServerID=HIWORD(lParam);

			//更新列表
			CServerListData * pServerListData=CServerListData::GetInstance();
			if (pServerListData!=NULL) pServerListData->OnEventDownLoadFinish(wKindID);

			//进入房间
			if ((wKindID!=0)&&(wServerID!=0))
			{
				CGameServerItem * pGameServerItem=m_ServerListData.SearchGameServer(wServerID);
				if (pGameServerItem!=NULL) EntranceServerItem(pGameServerItem);
			}

			return 0L;
		}
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
