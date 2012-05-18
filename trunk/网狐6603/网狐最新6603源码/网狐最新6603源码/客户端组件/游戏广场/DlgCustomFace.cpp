#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomFace.h"

//////////////////////////////////////////////////////////////////////////////////

//编辑模式
#define MM_SOURCE					0									//原有模式
#define MM_CUSTOM					1									//自定模式
#define MM_SYSTEM					2									//系统模式

//类型定义
#define FACE_CUSTOM_EDIT			0									//自定头像
#define FACE_SELECT_ITEM			1									//选择头像

//控件标识
#define IDC_FACE_CUSTOM_WND			100									//自定头像
#define IDC_FACE_SELECT_WND			101									//选择头像

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomFace, CSkinDialog)
	ON_NOTIFY(TCN_SELCHANGE,IDC_TAB_FACE,OnTcnSelAlterFaceMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgCustomFace::CDlgCustomFace() : CSkinDialog(IDD_DLG_COSTOM_FACE)
{
	//设置变量
	m_pICustomFaceEvent=NULL;

	//设置变量
	m_cbMode=MM_SOURCE;
	m_wFaceID=INVALID_WORD;
	ZeroMemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));

	//任务组件
	m_MissionManager.InsertMissionItem(this);

	//设置接口
	m_FaceItemCustomWnd.SetImageEditorEvent(this);
	m_FaceItemSelectWnd.SetFaceItemSelectEvent(this);

	return;
}

//析构函数
CDlgCustomFace::~CDlgCustomFace()
{
}

//图像事件
VOID CDlgCustomFace::OnEventFaceSelect(WORD wFaceID)
{
	//设置变量
	m_wFaceID=wFaceID;

	//设置模式
	m_cbMode=MM_SYSTEM;

	//设置控件
	m_FaceItemCustomWnd.DestoryImage();

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//图像事件
VOID CDlgCustomFace::OnEventImageSelect(bool bLoadImage, bool bOperateing)
{
	//设置变量
	m_cbMode=MM_CUSTOM;

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return;
}

//控件绑定
VOID CDlgCustomFace::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//绑定控件
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_TAB_FACE, m_TabControl);
}

//创建函数
BOOL CDlgCustomFace::OnInitDialog()
{
	__super::OnInitDialog();

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置标题
	SetWindowText(TEXT("更换头像"));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//设置变量
	m_wFaceID=pGlobalUserData->wFaceID;
	CopyMemory(&m_CustomFaceInfo,&pGlobalUserData->CustomFaceInfo,sizeof(m_CustomFaceInfo));

	//设置控件
	m_FaceItemSelectWnd.SetAllowItemFull();

	//设置控件
	m_TabControl.SetItemSize(CSize(150,25));
	m_TabControl.InsertItem(FACE_CUSTOM_EDIT,TEXT(" 自定头像 "));
	m_TabControl.InsertItem(FACE_SELECT_ITEM,TEXT(" 系统头像 "));

	//创建窗口
	CRect rcCreate(0,0,0,0);
	m_FaceItemSelectWnd.Create(NULL,NULL,WS_CHILD,rcCreate,&m_TabControl,IDC_FACE_SELECT_WND);
	m_FaceItemCustomWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,&m_TabControl,IDC_FACE_CUSTOM_WND);

	//调整位置
	CRect rcTabCtrl(10,40,rcClient.Width()-110,rcClient.Height()-45);
	m_FaceItemCustomWnd.SetWindowPos(NULL,1,27,rcTabCtrl.Width()-2,rcTabCtrl.Height()-28,SWP_NOZORDER);
	m_FaceItemSelectWnd.SetWindowPos(NULL,1,27,rcTabCtrl.Width()-2,rcTabCtrl.Height()-28,SWP_NOZORDER);
	m_TabControl.SetWindowPos(NULL,rcTabCtrl.left,rcTabCtrl.top,rcTabCtrl.Width(),rcTabCtrl.Height(),SWP_NOZORDER);

	return TRUE;
}

//绘画函数
VOID CDlgCustomFace::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	__super::OnDrawClientArea(pDC,nWidth,nHeight);

	//获取对象
	ASSERT(CSkinResourceManager::GetInstance()!=NULL);
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();

	//设置环境
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SelectObject(pSkinResourceManager->GetDefaultFont());

	//输出标题
	LPCTSTR pszTitle=TEXT("预览：");
	pDC->TextOut(nWidth-FACE_CX-47,88,pszTitle,lstrlen(pszTitle));

	//绘画边框
	CFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
	pIFaceItemControl->DrawFaceItemFrame(pDC,nWidth-FACE_CX-32,112,ITEM_FRAME_HOVER);

	//绘画头像
	switch (m_cbMode)
	{
	case MM_SOURCE:		//原有模式
		{
			if (m_CustomFaceInfo.dwDataSize!=0L)
			{
				IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
				pIFaceItemControl->DrawFaceNormal(pDC,nWidth-FACE_CX-32,112,m_CustomFaceInfo.dwCustomFace);
			}
			else
			{
				IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
				pIFaceItemControl->DrawFaceNormal(pDC,nWidth-FACE_CX-32,112,m_wFaceID);
			}

			break;
		}
	case MM_SYSTEM:		//系统模式
		{
			IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
			pIFaceItemControl->DrawFaceNormal(pDC,nWidth-FACE_CX-32,112,m_wFaceID);

			break;
		}
	case MM_CUSTOM:		//自定模式
		{
			//自定头像
			ASSERT(m_FaceItemCustomWnd.IsNull()==false);
			m_FaceItemCustomWnd.DrawEditImage(pDC,nWidth-FACE_CX-32,112,FACE_CX,FACE_CY);

			break;
		}
	}

	return;
}

//连接事件
bool CDlgCustomFace::OnEventMissionLink(INT nErrorCode)
{
	//错误判断
	if (nErrorCode==0L)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//发送消息 
		switch (m_cbMode)
		{
		case MM_SYSTEM:		//系统模式
			{
				//变量定义
				CMD_GP_SystemFaceInfo SystemFaceInfo;
				ZeroMemory(&SystemFaceInfo,sizeof(SystemFaceInfo));

				//设置变量
				SystemFaceInfo.wFaceID=m_wFaceID;
				SystemFaceInfo.dwUserID=pGlobalUserData->dwUserID;
				CWHService::GetMachineID(SystemFaceInfo.szMachineID);
				CopyMemory(SystemFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(SystemFaceInfo.szPassword));

				//发送数据
				CMissionManager * pMissionManager=GetMissionManager();
				pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_SYSTEM_FACE_INFO,&SystemFaceInfo,sizeof(SystemFaceInfo));

				break;
			}
		case MM_CUSTOM:		//自定模式
			{
				//变量定义
				CMD_GP_CustomFaceInfo CustomFaceInfo;
				ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

				//设置变量
				CustomFaceInfo.dwUserID=pGlobalUserData->dwUserID;
				CWHService::GetMachineID(CustomFaceInfo.szMachineID);
				CopyMemory(CustomFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(CustomFaceInfo.szPassword));
				CopyMemory(CustomFaceInfo.dwCustomFace,m_CustomFaceInfo.dwCustomFace,sizeof(CustomFaceInfo.dwCustomFace));

				//发送数据
				CMissionManager * pMissionManager=GetMissionManager();
				pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_CUSTOM_FACE_INFO,&CustomFaceInfo,sizeof(CustomFaceInfo));

				break;
			}
		}

		return true;
	}
	else
	{
		//显示控件
		EnableControl(true);

		//显示提示
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("无法连接服务器，头像修改失败，请检查网络是否畅通！"),MB_ICONERROR);
	}

	return true;
}

//关闭事件
bool CDlgCustomFace::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//显示控件
		EnableControl(true);

		//显示提示
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("由于当前服务器处理繁忙，上传自定义头像失败，请稍后再重试！"),MB_ICONERROR);
	}

	return true;
}

//读取事件
bool CDlgCustomFace::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//操作成功
	if ((Command.wMainCmdID==MDM_GP_USER_SERVICE)&&(Command.wSubCmdID==SUB_GP_USER_FACE_INFO))
	{
		//效验参数
		ASSERT(wDataSize==sizeof(CMD_GP_UserFaceInfo));
		if (wDataSize!=sizeof(CMD_GP_UserFaceInfo)) return false;

		//消息处理
		CMD_GP_UserFaceInfo * pUserFaceInfo=(CMD_GP_UserFaceInfo *)pData;

		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//保存数据
		if (pUserFaceInfo->dwCustomID==0)
		{
			//事件通知
			ASSERT(m_pICustomFaceEvent!=NULL);
			if (m_pICustomFaceEvent!=NULL) m_pICustomFaceEvent->OnEventSystemFace(pGlobalUserData->dwUserID,pUserFaceInfo->wFaceID);
		}
		else
		{
			//保存数据
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			pCustomFaceManager->SaveUserCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo.dwCustomFace);

			//事件通知
			ASSERT(m_pICustomFaceEvent!=NULL);
			if (m_pICustomFaceEvent!=NULL) m_pICustomFaceEvent->OnEventCustomFace(pGlobalUserData->dwUserID,pUserFaceInfo->dwCustomID,m_CustomFaceInfo);
		}

		//关闭窗口
		EndDialog(IDOK);

		return true;
	}

	return true;
}

//确定函数
VOID CDlgCustomFace::OnOK()
{
	//连接判断
	bool bConnect=false;

	//系统模式
	if (m_cbMode==MM_SYSTEM)
	{
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		bConnect=(pGlobalUserInfo->GetGlobalUserData()->wFaceID!=m_wFaceID);
	}

	//自定模式
	if (m_cbMode==MM_CUSTOM)
	{
		//创建缓冲
		CImage ImageCustomFace;
		ImageCustomFace.Create(FACE_CX,FACE_CY,32);

		//创建 DC
		CImageDC BufferDC(ImageCustomFace);
		m_FaceItemCustomWnd.DrawEditImage(CDC::FromHandle(BufferDC),0,0,FACE_CX,FACE_CY);

		//获取数据
		INT nImagePitch=ImageCustomFace.GetPitch();
		LPBYTE cbBitCustomFace=(LPBYTE)ImageCustomFace.GetBits();

		//创建区域
		for (INT nYImagePos=0;nYImagePos<FACE_CY;nYImagePos++)
		{
			for (INT nXImagePos=0;nXImagePos<FACE_CX;nXImagePos++)
			{
				//设置颜色
				DWORD dwImageTarget=nYImagePos*nImagePitch+nXImagePos*4L;
				m_CustomFaceInfo.dwCustomFace[nYImagePos*FACE_CX+nXImagePos]=*(COLORREF *)(cbBitCustomFace+dwImageTarget);
			}
		}

		//设置变量
		bConnect=true;
		m_CustomFaceInfo.dwDataSize=sizeof(m_CustomFaceInfo);
	}

	//激活任务
	if (bConnect==true)
	{
		//控件控制
		EnableControl(false);

		//激活任务
		m_MissionManager.AvtiveMissionItem(this,false);

		return;
	}

	__super::OnOK();
}

//设置接口
bool CDlgCustomFace::SetCustomFaceEvent(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent)!=NULL);
		m_pICustomFaceEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ICustomFaceEvent);

		//成功判断
		if (m_pICustomFaceEvent==NULL) return false;
	}
	else m_pICustomFaceEvent=NULL;

	return true;
}

//控件控制
VOID CDlgCustomFace::EnableControl(bool bEnableControl)
{
	//设置控件
	m_btOk.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	m_TabControl.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	m_FaceItemCustomWnd.EnableWindow((bEnableControl==true)?TRUE:FALSE);
	m_FaceItemSelectWnd.EnableWindow((bEnableControl==true)?TRUE:FALSE);

	return;
}

//类型改变
VOID CDlgCustomFace::OnTcnSelAlterFaceMode(NMHDR * pNMHDR, LRESULT * pResult)
{
	switch (m_TabControl.GetCurSel())
	{
	case FACE_CUSTOM_EDIT:		//自定头像
		{
			//显示控件
			m_FaceItemCustomWnd.ShowWindow(SW_SHOW);
			m_FaceItemSelectWnd.ShowWindow(SW_HIDE);

			//设置焦点
			m_FaceItemCustomWnd.SetFocus();
			m_FaceItemCustomWnd.SetActiveWindow();

			break;
		}
	case FACE_SELECT_ITEM:		//系统头像
		{
			//显示控件
			m_FaceItemCustomWnd.ShowWindow(SW_HIDE);
			m_FaceItemSelectWnd.ShowWindow(SW_SHOW);

			//设置焦点
			m_FaceItemSelectWnd.SetFocus();
			m_FaceItemSelectWnd.SetActiveWindow();

			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
