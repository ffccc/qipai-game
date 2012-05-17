#include "StdAfx.h"
#include "Resource.h"
#include "WndChatControl.h"

//////////////////////////////////////////////////////////////////////////

//按钮标识
#define IDC_CHAT_CLOSE				100									//关闭按钮
#define IDC_CHAT_CONTROL			101									//控制按钮

//用户选择
#define MAX_HISTOY_USER				8									//最大数目
#define IDM_CANCEL_CHAT_USER		(WM_USER+100)						//历史用户
#define IDM_HISTORY_CHAT_USER		(WM_USER+101)						//历史用户

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndChatControl, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()

	//按钮消息
	ON_BN_CLICKED(IDC_CHAT_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_CHAT_CONTROL, OnBnClickedControl)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CWndChatControl::CWndChatControl()
{
	//设置变量
	m_pIClientUserItem=NULL;
	m_pIChatControlSink=NULL;

	return;
}

//析构函数
CWndChatControl::~CWndChatControl()
{
}

//命令函数
BOOL CWndChatControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//取消聊天
	if (LOWORD(wParam)==IDM_CANCEL_CHAT_USER)
	{
		SetChatTargetUser(NULL);
		return TRUE;
	}

	//历史用户
	if ((LOWORD(wParam)>=IDM_HISTORY_CHAT_USER)&&(LOWORD(wParam)<(IDM_HISTORY_CHAT_USER+MAX_HISTOY_USER)))
	{
		//选择用户
		INT_PTR nIndex=LOWORD(wParam)-(IDM_HISTORY_CHAT_USER+MAX_HISTOY_USER);

		//选择用户
		ASSERT(nIndex<m_ChatUserItemArray.GetCount());
		if (nIndex<m_ChatUserItemArray.GetCount()) SetChatTargetUser(m_ChatUserItemArray[nIndex]);

		return TRUE;
	}

	return __super::OnCommand(wParam,lParam);
}

//获取用户
DWORD CWndChatControl::GetChatTargetUserID()
{
	if (m_pIClientUserItem!=NULL)
	{
		return m_pIClientUserItem->GetUserID();
	}

	return 0L;
}

//获取用户
IClientUserItem * CWndChatControl::GetChatTargetUserItem()
{
	return m_pIClientUserItem;
}

//删除用户
VOID CWndChatControl::DeleteUserItem(IClientUserItem * pIClientUserItem)
{
	//删除用户
	for (INT_PTR i=0;i<m_ChatUserItemArray.GetCount();i++)
	{
		//获取用户
		IClientUserItem * pIChatUserItem=m_ChatUserItemArray[i];
		if (pIChatUserItem->GetUserID()==pIClientUserItem->GetUserID())
		{
			m_ChatUserItemArray.RemoveAt(i);
			break;
		}
	}

	//当前用户
	if (m_pIClientUserItem==pIClientUserItem)
	{
		//设置变量
		m_pIClientUserItem=NULL;

		//事件通知
		ASSERT(m_pIChatControlSink!=NULL);
		m_pIChatControlSink->OnChangeChatTarget(m_pIClientUserItem);

		//更新界面
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
		}
	}

	return;
}

//设置用户
VOID CWndChatControl::SetChatTargetUser(IClientUserItem * pIClientUserItem)
{
	//设置变量
	m_pIClientUserItem=pIClientUserItem;

	//历史用户
	if (pIClientUserItem!=NULL)
	{
		//删除用户
		for (INT_PTR i=0;i<m_ChatUserItemArray.GetCount();i++)
		{
			//获取用户
			IClientUserItem * pIChatUserItem=m_ChatUserItemArray[i];
			if (pIChatUserItem->GetUserID()==pIClientUserItem->GetUserID())
			{
				m_ChatUserItemArray.RemoveAt(i);
			}
		}

		//插入用户
		m_ChatUserItemArray.Add(pIClientUserItem);

		//清理多余
		if (m_ChatUserItemArray.GetCount()>MAX_HISTOY_USER)
		{
			m_ChatUserItemArray.RemoveAt(0);
		}
	}

	//事件通知
	ASSERT(m_pIChatControlSink!=NULL);
	m_pIChatControlSink->OnChangeChatTarget(m_pIClientUserItem);

	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//设置接口
VOID CWndChatControl::SetChatControlSink(IChatControlSink * pIChatControlSink)
{
	//设置变量
	m_pIChatControlSink=pIChatControlSink;

	return;
}

//创建消息
INT CWndChatControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//创建按钮
	CRect rcButton(0,0,0,0);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcButton,this,IDC_CHAT_CLOSE);
	m_btControl.Create(NULL,WS_CHILD|WS_VISIBLE,rcButton,this,IDC_CHAT_CONTROL);

	//设置按钮
	HINSTANCE hInstance=GetModuleHandle(SHARE_CONTROL_DLL_NAME);
	m_btClose.SetButtonImage(IDB_BT_CHAT_CLOSE,hInstance,true,false);
	m_btControl.SetButtonImage(IDB_BT_CHAT_HISTORY,hInstance,true,false);

	//调整位置
	CBitImage ImageBackGroud;
	ImageBackGroud.LoadFromResource(hInstance,IDB_CHAT_CONTROL_BACK);
	SetWindowPos(NULL,0,0,rcWindow.Width(),ImageBackGroud.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//绘画函数
VOID CWndChatControl::OnPaint()
{
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//加载资源
	CBitImage ImageBackGroud;
	ImageBackGroud.LoadFromResource(GetModuleHandle(SHARE_CONTROL_DLL_NAME),IDB_CHAT_CONTROL_BACK);

	//渲染背景
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(ImageBackGroud);

	//绘画背景
	for (INT nXPos=0;nXPos<rcClient.Width();nXPos+=ImageBackGroud.GetWidth())
	{
		ImageBackGroud.BitBlt(dc,nXPos,0);
	}

	//绘画信息
	if (m_pIClientUserItem!=NULL)
	{
		//构造位置
		CRect rcTargetUser;
		rcTargetUser.top=rcClient.top+6;
		rcTargetUser.left=rcClient.left+22;
		rcTargetUser.bottom=rcClient.bottom;
		rcTargetUser.right=rcClient.right-40;

		//设置 DC
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(10,10,10));
		dc.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//绘画信息
		TCHAR szString[64]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("目标用户“%s”"),m_pIClientUserItem->GetNickName());
		dc.DrawText(szString,lstrlen(szString),&rcTargetUser,DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
	}

	return;
}

//销毁消息
VOID CWndChatControl::OnDestroy()
{
	__super::OnDestroy();

	//设置变量
	m_pIClientUserItem=NULL;

	return;
}

//位置变化
VOID CWndChatControl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//控制按钮
	if (m_btControl.m_hWnd!=NULL)
	{
		CRect rcButton;
		m_btControl.GetWindowRect(&rcButton);
		m_btControl.SetWindowPos(NULL,3,cy-rcButton.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
	}

	//关闭按钮
	if (m_btClose.m_hWnd!=NULL)
	{
		CRect rcButton;
		m_btClose.GetWindowRect(&rcButton);
		m_btClose.SetWindowPos(NULL,cx-rcButton.Width()-3,cy-rcButton.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
	}

	return;
}

//关闭按钮
VOID CWndChatControl::OnBnClickedClose()
{
	//取消聊天
	ASSERT(m_pIChatControlSink!=NULL);
	if (m_pIChatControlSink!=NULL) m_pIChatControlSink->OnChangeChatTarget(NULL);

	return;
}

//控制按钮
VOID CWndChatControl::OnBnClickedControl()
{
	//效验状态
	ASSERT((m_pIClientUserItem!=NULL)&&(m_ChatUserItemArray.GetCount()>0));
	if ((m_pIClientUserItem==NULL)||(m_ChatUserItemArray.GetCount()==0)) return;

	//构造菜单
	CSkinMenu ChatUserMenu;
	ChatUserMenu.CreateMenu();

	//构造菜单
	if (m_ChatUserItemArray.GetCount()>1)
	{
		for (INT_PTR i=0;i<m_ChatUserItemArray.GetCount();i++)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=m_ChatUserItemArray[i];
			if (pIClientUserItem->GetUserID()==m_pIClientUserItem->GetUserID()) continue;

			//构造菜单
			TCHAR szMenuString[64]=TEXT("");
			_sntprintf(szMenuString,CountArray(szMenuString),TEXT("与 [ %s ] 交谈"),pIClientUserItem->GetNickName());

			//插入菜单
			ChatUserMenu.AppendMenu(IDM_HISTORY_CHAT_USER+(UINT)i,szMenuString);
		}
	}
	else ChatUserMenu.AppendMenu(0,TEXT("没有历史聊天对象"),MF_GRAYED);

	//取消聊天
	if (m_pIClientUserItem!=NULL)
	{
		//构造菜单
		TCHAR szMenuString[64]=TEXT("");
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("取消与 [ %s ] 交谈"),m_pIClientUserItem->GetNickName());

		//插入菜单
		ChatUserMenu.AppendSeparator();
		ChatUserMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}
	
	//弹出菜单
	ChatUserMenu.TrackPopupMenu(this);
	
	return;
}

//////////////////////////////////////////////////////////////////////////
