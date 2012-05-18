#include "Stdafx.h"
#include "GamePlaza.h"
#include "DlgService.h"
#include "DlgCustomFace.h"
#include "PlatformFrame.h"
#include "DlgLockMachine.h"
#include "WndUserInfoCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//控制按钮
#define IDC_USER_INFO_1				100									//用户按钮
#define IDC_USER_INFO_2				101									//用户按钮
#define IDC_USER_INFO_3				102									//用户按钮
#define IDC_USER_INFO_SET			103									//用户按钮
#define IDC_USER_INFO_EDIT			104									//编辑控件

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndUserInfoCtrl, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//自定消息
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CEditUnderWrite, CSkinEditEx)
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CEditUnderWrite::CEditUnderWrite()
{
	//设置变量
	m_bNeedSend=false;
	m_pParentWnd=NULL;
	ZeroMemory(m_szUnderWrite,sizeof(m_szUnderWrite));

	//设置控件
	m_MissionManager.InsertMissionItem(this);

	return;
}

//析构函数
CEditUnderWrite::~CEditUnderWrite()
{
}

//消息解释
BOOL CEditUnderWrite::PreTranslateMessage(MSG * pMsg)
{
	//取消消息
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		//关闭控件
		CloseUnderWrite();

		return TRUE;
	}

	//确定消息
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//设置焦点
		ASSERT(m_pParentWnd!=NULL);
		if (m_pParentWnd!=NULL) m_pParentWnd->SetFocus();

		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//连接事件
bool CEditUnderWrite::OnEventMissionLink(INT nErrorCode)
{
	//错误判断
	if (nErrorCode!=0L)
	{
		//错误提示
		CInformation Information(m_pParentWnd);
		Information.ShowMessageBox(TEXT("服务器连接失败，个性签名更新失败！"),MB_ICONERROR,30L);

		return true;
	}

	//设置变量
	m_bNeedSend=false;

	//变量定义
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CMD_GP_ModifyUnderWrite ModifyUnderWrite;
	ZeroMemory(&ModifyUnderWrite,sizeof(ModifyUnderWrite));

	//设置数据
	ModifyUnderWrite.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(ModifyUnderWrite.szUnderWrite,m_szUnderWrite,CountArray(ModifyUnderWrite.szUnderWrite));
	lstrcpyn(ModifyUnderWrite.szPassword,pGlobalUserData->szPassword,CountArray(ModifyUnderWrite.szPassword));

	//发送数据
	WORD wHeadSize=sizeof(ModifyUnderWrite)-sizeof(ModifyUnderWrite.szUnderWrite);
	pMissionManager->SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_UNDER_WRITE,&ModifyUnderWrite,wHeadSize+CountStringBuffer(ModifyUnderWrite.szUnderWrite));

	return true;
}

//关闭事件
bool CEditUnderWrite::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//重试任务
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager->AvtiveMissionItem(this,true)==true) return true;

		//设置控件
		CloseUnderWrite();

		//显示提示
		CInformation Information(m_pParentWnd);
		Information.ShowMessageBox(TEXT("由于当前服务器处理繁忙，个性签名修改失败，请稍后再重试！"),MB_ICONERROR);
	}

	return true;
}

//读取事件
bool CEditUnderWrite::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
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

				//关闭连接
				CMissionManager * pMissionManager=GetMissionManager();
				if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

				//设置控件
				CloseUnderWrite();

				//显示消息
				if (pOperateFailure->szDescribeString[0]!=0)
				{
					CInformation Information(m_pParentWnd);
					Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
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
				CMissionManager * pMissionManager=GetMissionManager();
				if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

				//设置信息
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				lstrcpyn(pGlobalUserData->szUnderWrite,m_szUnderWrite,CountArray(pGlobalUserData->szUnderWrite));

				//设置控件
				CloseUnderWrite();

				//发送事件
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

				//显示消息
				if (pOperateSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(m_pParentWnd);
					Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				return true;
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//修改判断
bool CEditUnderWrite::IsModifyStatus()
{
	//状态判断
	if (m_bNeedSend==true) return true;
	if (GetActiveStatus()==true) return true;

	return false;
}

//设置窗口
VOID CEditUnderWrite::SetParentWindow(CWnd * pParentWnd)
{
	//设置变量
	m_pParentWnd=pParentWnd;

	return;
}

//关闭控件
VOID CEditUnderWrite::CloseUnderWrite()
{
	//关闭判断
	if ((m_bNeedSend==false)&&(GetActiveStatus()==false))
	{
		//销毁窗口
		DestroyWindow();
	}
	else
	{
		//隐藏窗口
		ShowWindow(SW_HIDE);
	}

	//更新界面
	ASSERT(m_pParentWnd!=NULL);
	m_pParentWnd->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);

	return;
}

//更新签名
VOID CEditUnderWrite::UpdateUnderWrite()
{
	if (IsWindowVisible()==TRUE)
	{
		//获取信息
		TCHAR szUnderWrite[LEN_UNDER_WRITE]=TEXT("");
		GetWindowText(szUnderWrite,CountArray(szUnderWrite));

		//用户信息
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//更新判断
		bool bActiveMission=false;
		if ((IsModifyStatus()==true)&&(lstrcmp(szUnderWrite,m_szUnderWrite)!=0L)) bActiveMission=true;
		if ((IsModifyStatus()==false)&&(lstrcmp(szUnderWrite,pGlobalUserData->szUnderWrite)!=0L)) bActiveMission=true;

		//更新判断
		if (bActiveMission==true)
		{
			//设置信息
			m_bNeedSend=true;
			lstrcpyn(m_szUnderWrite,szUnderWrite,CountArray(m_szUnderWrite));

			//终止任务
			CMissionManager * pMissionManager=GetMissionManager();
			if (GetActiveStatus()==true) pMissionManager->ConcludeMissionItem(this,false);

			//设置地址
			LPCTSTR pszCurrentServer=pMissionManager->GetCurrentServer();
			if ((pszCurrentServer!=NULL)&&(pszCurrentServer[0]!=0)) pMissionManager->SetCustomServer(pszCurrentServer);

			//激活任务
			pMissionManager->AvtiveMissionItem(this,false);

			//隐藏控件
			ShowWindow(SW_HIDE);

			//更新界面
			ASSERT(m_pParentWnd!=NULL);
			m_pParentWnd->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
		else
		{
			//关闭控件
			CloseUnderWrite();
		}
	}

	return;
}

//消耗消息
VOID CEditUnderWrite::OnDestroy()
{
	__super::OnDestroy();

	//终止任务
	if (GetActiveStatus()==true)
	{
		//变量定义
		CMissionManager * pMissionManager=GetMissionManager();
		if (pMissionManager!=NULL) pMissionManager->ConcludeMissionItem(this,false);

		//更新界面
		ASSERT(m_pParentWnd!=NULL);
		m_pParentWnd->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//失去焦点
VOID CEditUnderWrite::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	//关闭控件
	UpdateUnderWrite();

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CWndUserInfoCtrl::CWndUserInfoCtrl()
{
	//状态标志
	m_bClickFace=false;
	m_bMouseEvent=false;

	//盘旋标志
	m_bHoverFace=false;
	m_bHoverUnderWrite=false;

	//设置区域
	m_rcFaceArea.SetRect(13,14,69,70);

	//设置控件
	m_EditUnderWrite.SetParentWindow(this);

	return;
}

//析构函数
CWndUserInfoCtrl::~CWndUserInfoCtrl()
{
}

//消息过虑
BOOL CWndUserInfoCtrl::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CWndUserInfoCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//功能按钮
	switch (nCommandID)
	{
	case IDC_USER_INFO_1:		//自定头像
		{
			//显示窗口
			CDlgCustomFace DlgCustomFace;
			DlgCustomFace.SetCustomFaceEvent(CPlatformFrame::GetInstance());

			//显示窗口
			DlgCustomFace.DoModal();
						
			return TRUE;
		}
	case IDC_USER_INFO_2:		//锁定机器
		{
			//显示窗口
			CDlgLockMachine DlgLockMachine;
			DlgLockMachine.DoModal();

			return TRUE;
		}
	case IDC_USER_INFO_3:		//魅力兑换
		{
			return TRUE;
		}
	case IDC_USER_INFO_SET:		//个人设置
		{
			//个人设置
			CDlgService DlgService;
			DlgService.DoModal();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//位置消息
VOID CWndUserInfoCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//建立消息
INT CWndUserInfoCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	//加载资源
	CBitImage ImageTitleM;
	ImageTitleM.LoadFromResource(AfxGetInstanceHandle(),IDB_USER_INFO_M);

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btUserInfo1.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_USER_INFO_1);
	m_btUserInfo2.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_USER_INFO_2);
	m_btUserInfo3.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_USER_INFO_3);
	m_btUserInfoSet.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_USER_INFO_SET);

	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btUserInfo1.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_1"),hInstance,true,false);
	m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_2"),hInstance,true,false);
	m_btUserInfo3.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_3"),hInstance,true,false);
	m_btUserInfoSet.SetButtonImage(IDB_BK_USER_INFO_SET,TEXT("BT_USER_INFO_SET"),hInstance,true,false);

	//移动窗口
	CRect rcUserInfo;
	GetWindowRect(&rcUserInfo);
	SetWindowPos(NULL,0,0,rcUserInfo.Width(),ImageTitleM.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btUserInfo1,TEXT("上传头像"));
	m_ToolTipCtrl.AddTool(&m_btUserInfo2,TEXT("锁定本机"));
	m_ToolTipCtrl.AddTool(&m_btUserInfo3,TEXT("魅力兑换"));
	m_ToolTipCtrl.AddTool(&m_btUserInfoSet,TEXT("个人信息设置"));

	return 0;
}

//调整控件
VOID CWndUserInfoCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	//设置区域
	m_rcUnderWrite.SetRect(127,52,nWidth-13,72);

	//变量定义
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER|SWP_NOSIZE;

	//按钮大小
	CRect rcButton;
	m_btUserInfo1.GetWindowRect(&rcButton);

	//间隔定义
	UINT nSpace=14;
	UINT nStartPos=18;

	//调整按钮
	DeferWindowPos(hDwp,m_btUserInfoSet,NULL,69,15,0,0,uFlags);
	DeferWindowPos(hDwp,m_btUserInfo1,NULL,nStartPos,nHeight-rcButton.Height()-9,0,0,uFlags);
	DeferWindowPos(hDwp,m_btUserInfo2,NULL,nStartPos+rcButton.Width()+nSpace,nHeight-rcButton.Height()-9,0,0,uFlags);
	DeferWindowPos(hDwp,m_btUserInfo3,NULL,nStartPos+(rcButton.Width()+nSpace)*2,nHeight-rcButton.Height()-9,0,0,uFlags);

	//结束调整
	EndDeferWindowPos(hDwp);

	return;
}

//绘画背景
BOOL CWndUserInfoCtrl::OnEraseBkgnd(CDC * pDC)
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

	//变量定义
	HINSTANCE hInstance=AfxGetInstanceHandle();
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//加载资源
	CBitImage ImageTitleL;
	CBitImage ImageTitleM;
	CBitImage ImageTitleR;
	ImageTitleL.LoadFromResource(hInstance,IDB_USER_INFO_L);
	ImageTitleM.LoadFromResource(hInstance,IDB_USER_INFO_M);
	ImageTitleR.LoadFromResource(hInstance,IDB_USER_INFO_R);

	//渲染资源
	pSkinRenderManager->RenderImage(ImageTitleL);
	pSkinRenderManager->RenderImage(ImageTitleM);
	pSkinRenderManager->RenderImage(ImageTitleR);

	//绘画背景
	ImageTitleL.BitBlt(BufferDC,0,0);

	//填充中间
	for (INT nXPos=ImageTitleL.GetWidth();nXPos<rcClient.Width();nXPos+=ImageTitleM.GetWidth())
	{
		ImageTitleM.BitBlt(BufferDC,nXPos,0);
	}

	//绘画背景
	ImageTitleR.BitBlt(BufferDC,rcClient.Width()-ImageTitleR.GetWidth(),0);

	//设置环境
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//用户信息
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//绘画用户
	if (pGlobalUserData->dwUserID!=0L)
	{
		//绘画底框
		if ((m_bHoverUnderWrite==true)||((m_EditUnderWrite.m_hWnd!=NULL)&&(m_EditUnderWrite.IsWindowVisible()==TRUE)))
		{
			//加载资源
			CPngImage ImageUnderWriteL;
			CPngImage ImageUnderWriteM;
			CPngImage ImageUnderWriteR;
			ImageUnderWriteL.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_L"));
			ImageUnderWriteM.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_M"));
			ImageUnderWriteR.LoadImage(AfxGetInstanceHandle(),TEXT("UNDER_WRITE_R"));

			//绘画两边
			ImageUnderWriteL.DrawImage(pBufferDC,m_rcUnderWrite.left,m_rcUnderWrite.top);
			ImageUnderWriteR.DrawImage(pBufferDC,m_rcUnderWrite.right-ImageUnderWriteR.GetWidth(),m_rcUnderWrite.top);

			//绘画中间
			INT nXStart=m_rcUnderWrite.left+ImageUnderWriteL.GetWidth();
			INT nXTerminate=m_rcUnderWrite.right-ImageUnderWriteR.GetWidth();
			for (INT nXPos=nXStart;nXPos<nXTerminate;nXPos+=ImageUnderWriteM.GetWidth())
			{
				ImageUnderWriteM.DrawImage(pBufferDC,nXPos,m_rcUnderWrite.top,__min(nXTerminate-nXPos,ImageUnderWriteM.GetWidth()),ImageUnderWriteM.GetHeight(),0,0);
			}
		}

		//绘画边框
		if (m_bHoverFace==true)
		{
			theApp.m_FaceItemControlModule->DrawFaceItemFrame(pBufferDC,17,18,ITEM_FRAME_HOVER);
		}

		//绘画头像
		if (pGlobalUserData->CustomFaceInfo.dwDataSize!=0)
		{
			DWORD * pdwCustomFace=pGlobalUserData->CustomFaceInfo.dwCustomFace;
			theApp.m_FaceItemControlModule->DrawFaceNormal(pBufferDC,16,17,pdwCustomFace);
		}
		else
		{
			theApp.m_FaceItemControlModule->DrawFaceNormal(pBufferDC,16,17,pGlobalUserData->wFaceID);
		}

		//绘画标题
		pBufferDC->SetTextColor(RGB(0,0,0));
		pBufferDC->TextOut(88,16,TEXT("昵 称："));
		pBufferDC->TextOut(88,36,TEXT("奖 牌："));
		pBufferDC->TextOut(88,56,TEXT("签 名："));

		//构造奖牌
		TCHAR szUserMedal[64]=TEXT("");
		_sntprintf(szUserMedal,CountArray(szUserMedal),TEXT("%ld"),pGlobalUserData->dwUserMedal);

		//输出奖牌
		CRect rcUserMedal;
		rcUserMedal.SetRect(130,36,rcClient.Width()-15,48);
		pBufferDC->DrawText(szUserMedal,lstrlen(szUserMedal),&rcUserMedal,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//输出帐号
		CRect rcNickName;
		rcNickName.SetRect(130,16,rcClient.Width()-15,28);
		pBufferDC->DrawText(pGlobalUserData->szNickName,lstrlen(pGlobalUserData->szNickName),&rcNickName,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//变量定义
		LPCTSTR pszUnderWrite=NULL;
		COLORREF crUnderWrite=RGB(0,0,0);

		//签名位置
		CRect rcUnderWrite;
		rcUnderWrite.SetRect(130,56,rcClient.Width()-15,68);

		//常规状态
		if ((m_EditUnderWrite.IsModifyStatus()==false)&&(pGlobalUserData->szUnderWrite[0]!=0))
		{
			crUnderWrite=RGB(0,0,0);
			pszUnderWrite=pGlobalUserData->szUnderWrite;
		}

		//修改状态
		if ((m_EditUnderWrite.IsModifyStatus()==true)&&(m_EditUnderWrite.m_szUnderWrite[0]!=0))
		{
			crUnderWrite=RGB(100,100,100);
			pszUnderWrite=m_EditUnderWrite.m_szUnderWrite;
		}

		//提示状态
		if (pszUnderWrite==NULL)
		{
			crUnderWrite=RGB(150,150,150);
			pszUnderWrite=TEXT("编辑个性签名");
		}

		//输出签名
		pBufferDC->SetTextColor(crUnderWrite);
		pBufferDC->DrawText(pszUnderWrite,lstrlen(pszUnderWrite),&rcUnderWrite,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//鼠标消息
VOID CWndUserInfoCtrl::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonUp(nFlags,MousePoint);

	if ((m_bHoverFace==true)&&(m_bClickFace==true))
	{
		//释放捕获
		ReleaseCapture();

		//设置变量
		m_bClickFace=false;

		//个人信息
		if (m_rcFaceArea.PtInRect(MousePoint)==TRUE)
		{
			CDlgService DlgService;
			DlgService.DoModal();
		}
	}

	return;
}

//鼠标消息
VOID CWndUserInfoCtrl::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	__super::OnLButtonDown(nFlags,MousePoint);

	//设置焦点
	SetFocus();

	//动作处理
	if ((m_bHoverFace==true)&&(m_bClickFace==false))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_bClickFace=true;
	}

	//编辑签名
	if (m_bHoverUnderWrite==true)
	{
		//用户信息
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//创建控件
		if (m_EditUnderWrite.m_hWnd==NULL)
		{
			//创建控件
			m_EditUnderWrite.Create(WS_CHILD|ES_AUTOHSCROLL,CRect(0,0,0,0),this,IDC_USER_INFO_EDIT);

			//设置信息
			m_EditUnderWrite.LimitText(LEN_UNDER_WRITE-1L);
			m_EditUnderWrite.SetWindowText(pGlobalUserData->szUnderWrite);

			//设置控件
			m_EditUnderWrite.SetFont(&CSkinResourceManager::GetInstance()->GetDefaultFont());
		}

		//调整位置
		CRect rcClient;
		GetClientRect(&rcClient);
		m_EditUnderWrite.SetWindowPos(NULL,130,56,rcClient.Width()-145,14,SWP_SHOWWINDOW|SWP_NOZORDER);

		//设置控件
		m_EditUnderWrite.SetFocus();
		m_EditUnderWrite.SetSel(0L,-1L);

		//更新窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//光标消息
BOOL CWndUserInfoCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//变量定义
	bool bMouseEvent=false;
	bool bRedrawWindow=false;

	//盘旋判断
	if ((m_bHoverFace==false)&&(m_rcFaceArea.PtInRect(MousePoint)==TRUE))
	{
		//设置变量
		bMouseEvent=true;
		bRedrawWindow=true;

		//设置变量
		m_bHoverFace=true;
	}

	//离开判断
	if ((m_bHoverFace==true)&&(m_rcFaceArea.PtInRect(MousePoint)==FALSE))
	{
		//设置变量
		bRedrawWindow=true;

		//设置变量
		m_bHoverFace=false;
	}

	//盘旋判断
	if ((m_bHoverUnderWrite==false)&&(m_rcUnderWrite.PtInRect(MousePoint)==TRUE))
	{
		//设置变量
		bRedrawWindow=true;

		//设置变量
		m_bHoverUnderWrite=true;
	}

	//离开判断
	if ((m_bHoverUnderWrite==true)&&(m_rcUnderWrite.PtInRect(MousePoint)==FALSE))
	{
		//设置变量
		bRedrawWindow=true;

		//设置变量
		m_bHoverUnderWrite=false;
	}

	//注册事件
	if ((m_bMouseEvent==false)&&(bMouseEvent==true))
	{
		//设置变量
		m_bMouseEvent=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	//更新界面
	if (bRedrawWindow==true)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//设置光标
	if (m_bHoverFace==true)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return true;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//鼠标消息
LRESULT CWndUserInfoCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_bMouseEvent=false;

	//离开处理
	if (m_bHoverFace==true)
	{
		//设置变量
		m_bMouseEvent=false;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	//离开处理
	if (m_bHoverUnderWrite==true)
	{
		//设置变量
		m_bHoverUnderWrite=false;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return 0;
}

//事件消息
LRESULT CWndUserInfoCtrl::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EVENT_USER_LOGON:			//登录成功
		{
			//更新窗口
			Invalidate();

			//设置按钮
			m_btUserInfo1.EnableWindow(TRUE);
			m_btUserInfo2.EnableWindow(TRUE);
			m_btUserInfo3.EnableWindow(TRUE);
			m_btUserInfoSet.EnableWindow(TRUE);

			//用户信息
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//设置按钮
			if (pGlobalUserData->cbMoorMachine==TRUE)
			{
				HINSTANCE hInstance=AfxGetInstanceHandle();
				m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_4"),hInstance,true,false);
			}

			return 0L;
		}
	case EVENT_USER_LOGOUT:			//注销成功
		{
			//更新窗口
			Invalidate();

			//销毁控件
			m_EditUnderWrite.DestroyWindow();

			//设置按钮
			m_btUserInfo1.EnableWindow(FALSE);
			m_btUserInfo2.EnableWindow(FALSE);
			m_btUserInfo3.EnableWindow(FALSE);
			m_btUserInfoSet.EnableWindow(FALSE);

			//设置按钮
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_2"),hInstance,true,false);

			return 0L;
		}
	case EVENT_USER_INFO_UPDATE:	//资料更新
		{
			//更新窗口
			Invalidate();

			return 0L;
		}
	case EVENT_USER_MOOR_MACHINE:	//绑定机器
		{
			//用户信息
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//设置按钮
			if (pGlobalUserData->cbMoorMachine==FALSE)
			{
				HINSTANCE hInstance=AfxGetInstanceHandle();
				m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_2"),hInstance,true,false);
			}
			else
			{
				HINSTANCE hInstance=AfxGetInstanceHandle();
				m_btUserInfo2.SetButtonImage(IDB_BK_USER_INFO,TEXT("BT_USER_INFO_4"),hInstance,true,false);
			}

			return 0L;
		}
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
