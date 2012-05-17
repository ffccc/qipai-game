#include "StdAfx.h"
#include "GamePlaza.h"
#include "ServerViewItem.h"

//数据控件
#include "GlobalUnits.h"
#include "PlatformEvent.h"

//界面控件
#include "PlatformFrame.h"
#include "WndViewItemCtrl.h"

//对话框类
#include "DlgSearchUser.h"
#include "DlgServerManager.h"
#include "DlgTablePassword.h"

//////////////////////////////////////////////////////////////////////////////////

//常量定义
#define PROPERTY_CY					89									//游戏道具
#define PUBLICIZE_CY				80									//游戏广告
#define INCISE_SCALE				47/100								//界面分割

//常量定义
#define SPLITTER_CY					3									//拆分高度
#define SPLITTER_CX					8									//拆分宽度
#define TABLE_BUTTON_SX				5									//按钮空隙
#define	TABLE_BUTTON_CX				330									//按钮区域

//控件标识
#define IDC_CHAT_CONTROL			100									//聊天控制
#define IDC_SERVER_TABLE			101									//桌子控件
#define IDC_GAME_PROPERTY			102									//游戏道具
#define IDC_WEB_PUBLICIZE			103									//浏览控件

//////////////////////////////////////////////////////////////////////////////////

//颜色选择
#define MAX_CHAT_COLOR				16									//最大数目
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//选择颜色

//快捷短语
#define MAX_SHORT_COUNT				16									//最大数目
#define IDM_SELECT_CHAT_SHORT		(WM_USER+300)						//选择短语

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServerViewItem, CDialog)

	//系统消息
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()

	//控件消息
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST_CONTROL, OnNMRclickUserList)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST_CONTROL, OnNMDblclkUserList)

	//聊天命令
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_SHORT, (IDM_SELECT_CHAT_SHORT+MAX_SHORT_COUNT), OnSelectChatShort)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerViewItem::CServerViewItem()
{
	//界面变量
	m_bCreateFlag=false;
	m_bRectifyHor=false;
	m_bRectifyVor=false;
	m_bHideUserList=false;

	//辅助变量
	m_dwChatTime=0L;
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;

	//用户属性
	m_dwUserRight=0L;
	m_dwMasterRight=0L;
	m_pIMySelfUserItem=NULL;

	//房间属性
	m_wServerType=0L;
	m_dwServerRule=0L;
	m_ServiceStatus=ServiceStatus_Unknow;

	//配置参数
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;

	//控件变量
	m_pDlgSearchTable=NULL;
	m_pDlgInsureServer=NULL;
	m_pDlgServerManager=NULL;
	m_pExpressionControl=NULL;

	//设置组件
	m_DlgStatus.SetStatusViewSink(this);

	//配置信息
	m_cbColumnCount=0;
	m_cbPropertyCount=0;
	ZeroMemory(m_ColumnItem,sizeof(m_ColumnItem));
	ZeroMemory(m_PropertyInfo,sizeof(m_PropertyInfo));

	//房间属性
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//聊天资源
	tagEncircleResource	EncircleChat;
	EncircleChat.pszImageTL=MAKEINTRESOURCE(IDB_CHAT_TL);
	EncircleChat.pszImageTM=MAKEINTRESOURCE(IDB_CHAT_TM);
	EncircleChat.pszImageTR=MAKEINTRESOURCE(IDB_CHAT_TR);
	EncircleChat.pszImageML=MAKEINTRESOURCE(IDB_CHAT_ML);
	EncircleChat.pszImageMR=MAKEINTRESOURCE(IDB_CHAT_MR);
	EncircleChat.pszImageBL=MAKEINTRESOURCE(IDB_CHAT_BL);
	EncircleChat.pszImageBM=MAKEINTRESOURCE(IDB_CHAT_BM);
	EncircleChat.pszImageBR=MAKEINTRESOURCE(IDB_CHAT_BR);
	m_ChatEncircle.InitEncircleResource(EncircleChat,AfxGetInstanceHandle());

	//桌子资源
	tagEncircleResource	EncircleTable;
	EncircleTable.pszImageTL=MAKEINTRESOURCE(IDB_TABLE_TL);
	EncircleTable.pszImageTM=MAKEINTRESOURCE(IDB_TABLE_TM);
	EncircleTable.pszImageTR=MAKEINTRESOURCE(IDB_TABLE_TR);
	EncircleTable.pszImageML=MAKEINTRESOURCE(IDB_TABLE_ML);
	EncircleTable.pszImageMR=MAKEINTRESOURCE(IDB_TABLE_MR);
	EncircleTable.pszImageBL=MAKEINTRESOURCE(IDB_TABLE_BL);
	EncircleTable.pszImageBM=MAKEINTRESOURCE(IDB_TABLE_BM);
	EncircleTable.pszImageBR=MAKEINTRESOURCE(IDB_TABLE_BR);
	m_TableEncircle.InitEncircleResource(EncircleTable,AfxGetInstanceHandle());

	//用户资源
	tagEncircleResource	EncircleUserList;
	EncircleUserList.pszImageTL=MAKEINTRESOURCE(IDB_USER_LIST_TL);
	EncircleUserList.pszImageTM=MAKEINTRESOURCE(IDB_USER_LIST_TM);
	EncircleUserList.pszImageTR=MAKEINTRESOURCE(IDB_USER_LIST_TR);
	EncircleUserList.pszImageML=MAKEINTRESOURCE(IDB_USER_LIST_ML);
	EncircleUserList.pszImageMR=MAKEINTRESOURCE(IDB_USER_LIST_MR);
	EncircleUserList.pszImageBL=MAKEINTRESOURCE(IDB_USER_LIST_BL);
	EncircleUserList.pszImageBM=MAKEINTRESOURCE(IDB_USER_LIST_BM);
	EncircleUserList.pszImageBR=MAKEINTRESOURCE(IDB_USER_LIST_BR);
	m_UserListEncircle.InitEncircleResource(EncircleUserList,AfxGetInstanceHandle());

	return;
}

//析构函数
CServerViewItem::~CServerViewItem()
{
	//删除对象
	SafeDelete(m_pDlgSearchTable);
	SafeDelete(m_pExpressionControl);

	//删除私聊
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//获取子项
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];

		//删除私聊
		CPlatformFrame::GetInstance()->RemoveWhisperItem(pDlgWhisper);

		//关闭窗口
		pDlgWhisper->DestroyWindow();

		//清理内存
		SafeDelete(pDlgWhisper);
	}

	return;
}

//接口查询
VOID * CServerViewItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGamePropertySink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableViewFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerViewItem,Guid,dwQueryVer);
	return NULL;
}

//控件绑定
VOID CServerViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//聊天控件
	DDX_Control(pDX, IDC_CHAT_EDIT, m_ChatEdit);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);

	//聊天按钮
	DDX_Control(pDX, IDC_BT_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_BT_CHAT_SHORT, m_btChatShort);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_1, m_btChatButton1);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_2, m_btChatButton2);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_3, m_btChatButton3);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_4, m_btChatButton4);

	//桌子按钮
	DDX_Control(pDX, IDC_BT_QUIT_SERVER, m_btQuitServer);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_1, m_btTableButton1);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_2, m_btTableButton2);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_3, m_btTableButton3);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_4, m_btTableButton4);
	DDX_Control(pDX, IDC_BT_TABLE_BUTTON_5, m_btTableButton5);

	//常规控件
	DDX_Control(pDX, IDC_VIEW_CONTROL, m_btControl);
	DDX_Control(pDX, IDC_VOR_SPLITER, m_VorSplitter);
	DDX_Control(pDX, IDC_HOR_SPLITER, m_HorSplitter);
	DDX_Control(pDX, IDC_USER_LIST_CONTROL, m_UserListControl);

	return;
}

//创建函数
BOOL CServerViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	//变量定义
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//创建控件
	m_TableViewFrame.CreateTableFrame(this,IDC_SERVER_TABLE,QUERY_ME_INTERFACE(IUnknownEx));
	m_GamePropertyCtrl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_GAME_PROPERTY);
	m_PlatformPublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	//拆分控件
	m_VorSplitter.SetSplitterEvent(this);
	m_HorSplitter.SetSplitterEvent(this);
	m_VorSplitter.SetSplitterColor(0.67,0.95);
	m_HorSplitter.SetSplitterColor(0.67,0.95);
	m_VorSplitter.SetSplitterType(SplitterType_Vor);
	m_HorSplitter.SetSplitterType(SplitterType_Hor);

	//设置列表
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	m_UserListControl.SetMySelfUserID(pGlobalUserInfo->GetGlobalUserData()->dwUserID);

	//聊天控件
	m_ChatEdit.SetLimitText(LEN_USER_CHAT-1);
	m_ChatEdit.SetEnableColor(RGB(0,0,0),RGB(255,255,255),RGB(255,255,255));

	//聊天控件
	m_ChatControl.SetChatControlSink(this);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(255,255,255));

	//桌子按钮
	m_btQuitServer.SetButtonImage(IDB_BT_QUIT_SERVER,TEXT("BT_QUIT_SERVER"),hInstance,true,false);
	m_btTableButton1.SetButtonImage(IDB_BT_TABLE_BUTTON_1,TEXT("BT_TABLE_BUTTON_1"),hInstance,true,false);
	m_btTableButton2.SetButtonImage(IDB_BT_TABLE_BUTTON_2,TEXT("BT_TABLE_BUTTON_2"),hInstance,true,false);
	m_btTableButton3.SetButtonImage(IDB_BT_TABLE_BUTTON_3,TEXT("BT_TABLE_BUTTON_3"),hInstance,true,false);
	m_btTableButton4.SetButtonImage(IDB_BT_TABLE_BUTTON_4,TEXT("BT_TABLE_BUTTON_4"),hInstance,true,false);
	m_btTableButton5.SetButtonImage(IDB_BT_TABLE_BUTTON_5,TEXT("BT_TABLE_BUTTON_5"),hInstance,true,false);

	//聊天按钮
	m_btChatButton1.SetButtonImage(IDB_BT_CHAT_BUTTON_1,TEXT("BT_CHAT_BUTTON_1"),hInstance,true,false);
	m_btChatButton2.SetButtonImage(IDB_BT_CHAT_BUTTON_2,TEXT("BT_CHAT_BUTTON_2"),hInstance,true,false);
	m_btChatButton3.SetButtonImage(IDB_BT_CHAT_BUTTON_3,TEXT("BT_CHAT_BUTTON_3"),hInstance,true,false);
	m_btChatButton4.SetButtonImage(IDB_BT_CHAT_BUTTON_4,TEXT("BT_CHAT_BUTTON_4"),hInstance,true,false);

	//聊天按钮
	m_btSendChat.SetButtonImage(IDB_BT_SEND_CHAT,hInstance,true,false);
	m_btChatShort.SetButtonImage(IDB_BT_CHAT_SHORT,hInstance,true,false);

	//控制按钮
	m_btControl.SetParent(&m_VorSplitter);
	m_btControl.SetButtonImage(IDB_BK_USER_LIST,TEXT("BT_USER_LIST_HIDE"),hInstance,true,false);

	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//创建组件
	if (m_GameLevelParserModule.CreateInstance()==false) throw TEXT("等级解释组件创建失败！");
	if (m_PlazaUserManagerModule.CreateInstance()==false) throw TEXT("用户管理组件创建失败！");

	//设置组件
	m_ProcessManager.SetProcessManagerSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_UserListControl.SetGameLevelParser(m_GameLevelParserModule.GetInterface());

	//配置组件
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	CUserInformation * pUserInformation=CUserInformation::GetInstance();
	if (m_PlazaUserManagerModule->SetUserManagerSink(pIUnknownEx)==false) throw TEXT("用户管理组件配置失败！");
	if (m_PlazaUserManagerModule->SetUserInformation(pUserInformation)==false) throw TEXT("用户管理组件配置失败！");

	//设置变量
	m_bCreateFlag=true;

	//设置界面
	InitServerViewCtrl(CSkinRenderManager::GetInstance());

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	return TRUE;
}

//消息过虑
BOOL CServerViewItem::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	//按键消息
	if (pMsg->message==WM_KEYDOWN)
	{
		//取消按钮
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}

		//回车按钮
		if (pMsg->wParam==VK_RETURN)
		{
			PostMessage(WM_COMMAND,IDC_BT_SEND_CHAT,0);
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//命令函数
BOOL CServerViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//按钮命令
	switch (nCommandID)
	{
	case IDC_VIEW_CONTROL:			//控制按钮
		{
			//设置变量
			m_bHideUserList=!m_bHideUserList;

			//设置按钮
			HINSTANCE hInstance=AfxGetInstanceHandle();
			m_btControl.SetButtonImage(IDB_BK_USER_LIST,(m_bHideUserList==true)?TEXT("BT_USER_LIST_SHOW"):TEXT("BT_USER_LIST_HIDE"),hInstance,true,false);

			//调整界面
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			return TRUE;
		}
	case IDC_BT_QUIT_SERVER:		//退出房间
		{
			//发送命令
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0L);

			return TRUE;
		}
	case IDC_BT_SEND_CHAT:			//发送聊天
		{
			//发送聊天
			if (m_ServiceStatus==ServiceStatus_ServiceIng)
			{
				//获取信息
				CString strMessage;
				m_ChatEdit.GetWindowText(strMessage);

				//格式消息
				strMessage.TrimLeft();
				strMessage.TrimRight();

				//发送信息
				if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
				{
					CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
					SendUserChatPacket(m_ChatControl.GetChatTargetUserID(),strMessage,pParameterGlobal->m_crChatTX);
				}

				//设置焦点
				m_ChatEdit.SetFocus();
				m_ChatEdit.SetWindowText(TEXT(""));
			}

			return TRUE;
		}
	case IDC_BT_CHAT_SHORT:			//聊天短语
		{
			//构造菜单
			CSkinMenu ChatShortMenu;
			ChatShortMenu.CreateMenu();

			//工作目录
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

			//构造路径
			TCHAR szFileName[MAX_PATH]=TEXT("");
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PhraseInfo.ini"),szDirectory);

			//变量定义
			BYTE cbIndex=1;
			bool bSuccess=false;
			TCHAR szItemName[16]=TEXT(""),szShortcut[LEN_USER_CHAT]=TEXT("");

			//读取信息
			while (true)
			{
				//读取信息
				_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),cbIndex);
				GetPrivateProfileString(TEXT("ServerChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),szFileName);

				//结果判断
				if (szShortcut[0]!=0)
				{
					//设置变量
					cbIndex++;
					bSuccess=true;

					//加入菜单
					ChatShortMenu.AppendMenu(IDM_SELECT_CHAT_SHORT+cbIndex-1,szShortcut);
				}

				//结束判断
				if ((szShortcut[0]==0)||(cbIndex>=MAX_SHORT_COUNT))
				{
					break;
				}
			}

			//弹出菜单
			if (bSuccess==true)
			{
				CRect rcButton;
				m_btChatShort.GetWindowRect(&rcButton);
				ChatShortMenu.TrackPopupMenu(rcButton.right,rcButton.top,this);
			}

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_1:		//表情按钮
		{
			//创建表情
			CRect rcButton;
			m_btChatButton1.GetWindowRect(&rcButton);

			//创建窗口
			if (m_pExpressionControl==NULL)
			{
				m_pExpressionControl=new CExpressionControl;
			}

			//显示窗口
			m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_2:		//选择颜色
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//变量定义
			INT nWidth=72,nHeight=16;
			COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
				RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

			//构造颜色
			CImage ImageColor[CountArray(crColor)];
			for (INT i=0;i<CountArray(ImageColor);i++)
			{
				//创建位图
				ImageColor[i].Create(nWidth,nHeight,16);

				//获取设备
				CImageDC BufferDC(ImageColor[i]);
				CDC * pDC=CDC::FromHandle(BufferDC);

				//绘画位图
				pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
				pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
				pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
			}

			//创建菜单
			CSkinMenu MenuColor;
			MenuColor.CreateMenu();

			//构造菜单
			MenuColor.AppendMenu(IDM_MORE_COLOR,TEXT("选择其他颜色"));

			//颜色选项
			MenuColor.AppendSeparator();
			for (INT i=0;i<CountArray(ImageColor);i++) 
			{
				MenuColor.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
				if (pParameterGlobal->m_crChatTX==crColor[i]) MenuColor.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
			}

			//弹出菜单
			CRect rcButton;
			m_btChatButton2.GetWindowRect(&rcButton);
			MenuColor.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_3:		//屏蔽按钮
		{
			//创建菜单
			CSkinMenu MenuMessage;
			MenuMessage.CreateMenu();

			//构造菜单
			MenuMessage.AppendMenu(IDM_MESSAGE_SHOW_ALL,TEXT("显示所有信息"));
			MenuMessage.AppendMenu(IDM_MESSAGE_HIDE_DETEST,TEXT("屏蔽厌恶信息"));
			MenuMessage.AppendMenu(IDM_MESSAGE_ONLY_FRIEND,TEXT("只显示好友信息"));

			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//设置菜单
			BYTE cbMessageMode=pParameterGlobal->m_cbMessageMode;
			if (cbMessageMode==MESSAGE_MODE_ALL) MenuMessage.CheckMenuItem(IDM_MESSAGE_SHOW_ALL,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_DETEST) MenuMessage.CheckMenuItem(IDM_MESSAGE_HIDE_DETEST,MF_BYCOMMAND|MF_CHECKED);
			if (cbMessageMode==MESSAGE_MODE_FRIEND) MenuMessage.CheckMenuItem(IDM_MESSAGE_ONLY_FRIEND,MF_BYCOMMAND|MF_CHECKED);

			//弹出菜单
			CRect rcButton;
			m_btChatButton3.GetWindowRect(&rcButton);
			MenuMessage.TrackPopupMenu(rcButton.left,rcButton.top,this);

			return TRUE;
		}
	case IDC_BT_CHAT_BUTTON_4:		//清屏按钮
		{
			//设置焦点
			m_ChatEdit.SetFocus();

			//聊天清屏
			m_ChatMessage.CleanScreen();

			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_1:		//加入按钮
		{
			//动作过虑
			if (m_ServiceStatus!=ServiceStatus_ServiceIng) return TRUE;
			if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)) return TRUE;

			//状态判断
			if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
			{
				CInformation Information(this);
				Information.ShowMessageBox(TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),MB_ICONINFORMATION,30L);
				return TRUE;
			}

			//随机座位
			if (CServerRule::IsAvertCheatMode(m_dwServerRule)==true)
			{
				PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
				return TRUE;
			}

			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//执行动作
			switch (pParameterGlobal->m_cbActionHitAutoJoin)
			{
			case ACTION_SEARCH_TABLE:		//查找位置
				{
					//变量定义
					tagSearchTable SearchTable;
					ZeroMemory(&SearchTable,sizeof(SearchTable));

					//搜索条件
					SearchTable.bNotFull=true;
					SearchTable.bOneNull=true;
					SearchTable.bAllNull=false;
					SearchTable.bFilterPass=true;
					SearchTable.bTwoNull=(m_TableViewFrame.GetChairCount()!=2);

					//搜索结果
					SearchTable.wResultTableID=INVALID_TABLE;
					SearchTable.wResultChairID=INVALID_CHAIR;
					SearchTable.wStartTableID=m_wFindTableID+1;

					//搜索桌子
					SearchGameTable(SearchTable);
					m_wFindTableID=SearchTable.wResultTableID;

					//搜索桌子
					if (m_wFindTableID==INVALID_CHAIR)
					{
						//搜索条件
						SearchTable.bAllNull=true;
						SearchTable.bNotFull=true;
						SearchTable.bOneNull=true;
						SearchTable.bFilterPass=true;
						SearchTable.bTwoNull=(m_TableViewFrame.GetChairCount()!=2);

						//搜索结果
						SearchTable.wResultTableID=INVALID_TABLE;
						SearchTable.wResultChairID=INVALID_CHAIR;
						SearchTable.wStartTableID=m_wFindTableID+1;

						//搜索桌子
						SearchGameTable(SearchTable);
						m_wFindTableID=SearchTable.wResultTableID;
					}

					//结果判断
					if (m_wFindTableID!=INVALID_CHAIR)
					{
						//效验数据
						ASSERT(SearchTable.wResultTableID!=INVALID_TABLE);
						ASSERT(SearchTable.wResultChairID!=INVALID_CHAIR);

						//设置数据
						WORD wChairID=SearchTable.wResultChairID;
						m_TableViewFrame.VisibleTable(m_wFindTableID);
						m_TableViewFrame.FlashGameChair(m_wFindTableID,wChairID);

						//自动坐下
						CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
						if (pParameterGlobal->m_bAutoSitDown==true) PerformSitDownAction(m_wFindTableID,wChairID,true);
					}
					else
					{
						//提示消息
						CInformation Information(this);
						Information.ShowMessageBox(TEXT("抱歉，现在暂时没有可以加入的游戏桌，请稍后再次尝试！"),MB_ICONINFORMATION,30L);
					}

					break;
				}
			case ACTION_SHOW_SEARCH_DLG:	//查找桌子
				{
					//发送命令
					PostMessage(WM_COMMAND,IDC_BT_TABLE_BUTTON_2,0L);

					break;
				}
			}

			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_2:		//查找桌子
		{
			//创建对象
			if (m_pDlgSearchTable==NULL)
			{
				//创建对象
				try
				{
					m_pDlgSearchTable=new CDlgSearchTable;
					if (m_pDlgSearchTable==NULL) return TRUE;
				}
				catch (...)
				{
					ASSERT(FALSE);
					return TRUE;
				}

				//设置对象
				m_pDlgSearchTable->SetServerViewItem(this);
				m_pDlgSearchTable->SetTableViewFrame(&m_TableViewFrame);
			}

			//创建窗口
			if (m_pDlgSearchTable->m_hWnd==NULL)
			{
				m_pDlgSearchTable->Create(IDD_DLG_SEARCH_TABLE,this);
			}

			//显示窗口
			m_pDlgSearchTable->ShowWindow(SW_SHOW);
			m_pDlgSearchTable->SetForegroundWindow();

			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_3:		//锁定按钮
		{
			//变量定义
			CDlgTablePassword DlgTablePassword;
			DlgTablePassword.SetPromptString(TEXT("桌子密码"));

			//显示窗口
			if (DlgTablePassword.DoModal()==IDOK)
			{
				//设置密码
				m_pParameterServer->m_bTakePassword=true;
				lstrcpyn(m_pParameterServer->m_szPassword,DlgTablePassword.m_szPassword,CountArray(m_pParameterServer->m_szPassword));

				//发送规则
				SendUserRulePacket();
			}

			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_4:		//游戏设置
		{
			return TRUE;
		}
	case IDC_BT_TABLE_BUTTON_5:		//更多按钮
		{
			//构造菜单
			CSkinMenu FunctionMenu;
			FunctionMenu.CreateMenu();

			//更多菜单
			FunctionMenu.AppendMenu(IDM_GAME_RULE,TEXT("规则介绍"));
			FunctionMenu.AppendMenu(IDM_GAME_CHART,TEXT("游戏排行榜"));

			//插入菜单
			if (m_pIMySelfUserItem->GetMasterOrder()>0)
			{
				//插入分割
				FunctionMenu.AppendSeparator();

				//插入菜单
				FunctionMenu.AppendMenu(IDM_MANAGER_USER,TEXT("游戏玩家管理..."),MF_DISABLED);
				FunctionMenu.AppendMenu(IDM_MANAGER_SERVER,TEXT("游戏房间管理..."),MF_DISABLED);
				FunctionMenu.AppendMenu(IDM_MANAGER_MESSAGE,TEXT("系统消息管理..."),MF_DISABLED);

				//设置菜单
				if (CMasterRight::CanManagerServer(m_dwMasterRight)==true)
				{
					FunctionMenu.EnableMenuItem(IDM_MANAGER_SERVER,MF_ENABLED);
				}

				//设置菜单
				if (CMasterRight::CanIssueMessage(m_dwMasterRight)==true)
				{
					FunctionMenu.EnableMenuItem(IDM_MANAGER_MESSAGE,MF_ENABLED);
				}
			}

			//弹出菜单
			CRect rcButton;
			m_btTableButton5.GetWindowRect(&rcButton);
			FunctionMenu.TrackPopupMenu(rcButton.left,rcButton.bottom,this);

			return TRUE;
		}
	}

	//菜单命令
	switch (nCommandID)
	{
	case IDM_GAME_RULE:				//游戏规则
		{
			//构造地址
			TCHAR szBrowser[MAX_PATH]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/GameRule.asp?GameID=%ld&KindID=%ld"),szPlatformLink,m_GameKind.wGameID,
				m_GameServer.wKindID);

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return TRUE;
		}
	case IDM_GAME_CHART:			//游戏排行
		{
			//变量定义
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			//构造地址
			TCHAR szBrowser[256]=TEXT("");
			_sntprintf(szBrowser,CountArray(szBrowser),TEXT("%s/GameChart.asp?GameID=%ld&KindID=%ld&ServerID=%ld&UserID=%ld"),szPlatformLink,
				m_GameKind.wGameID,m_GameServer.wKindID,m_GameServer.wServerID,pGlobalUserData->dwUserID);

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szBrowser,NULL,NULL,SW_NORMAL);

			return TRUE;
		}
	case IDM_DELETE_SERVER_ITEM:	//关闭房间
		{
			//中断连接
			if (m_TCPSocketModule.GetInterface()!=NULL)
			{
				m_TCPSocketModule->CloseSocket();
				m_ServiceStatus=ServiceStatus_NetworkDown;
			}

			//关闭房间
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->DeleteServerViewItem(this);

			return TRUE;
		}
	case IDM_CREATE_ISER_WISPER:	//发送私聊
		{
			//创建私聊
			if (m_ServiceStatus==ServiceStatus_ServiceIng)
			{
				WORD wUserCount=(WORD)m_MenuUserItemArray.GetCount();
				WhisperConversation(m_MenuUserItemArray.GetData(),wUserCount);
			}

			return TRUE;
		}
	case IDM_COPY_USER_NICKNAME:	//拷贝昵称
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//拷贝字符
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			CWHService::SetClipboardString(pIClientUserItem->GetNickName());

			//设置字符
			m_ChatEdit.Paste();
			m_ChatEdit.SetFocus();

			return TRUE;
		}
	case IDM_SEE_USER_LOCATION:		//查看位置
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//隐藏信息
			bool bHideUserInfo=CServerRule::IsAvertCheatMode(m_dwServerRule);
			if ((bHideUserInfo==true)&&(m_pIMySelfUserItem->GetMasterOrder()==0)) return FALSE;

			//获取用户
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];

			//获取属性
			WORD wTableID=pIClientUserItem->GetTableID();
			WORD wChairID=pIClientUserItem->GetChairID();
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

			//定位位置
			if (wTableID!=INVALID_TABLE)
			{
				//设置可视
				ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
				CWndViewItemCtrl::GetInstance()->ActiveViewItem(this);

				//设置可视
				m_TableViewFrame.VisibleTable(wTableID);

				//闪动椅子
				if (cbUserStatus==US_LOOKON)
				{
					m_TableViewFrame.FlashGameTable(wTableID);
				}
				else
				{
					m_TableViewFrame.FlashGameChair(wTableID,wChairID);
				}
			}

			//变量定义
			LVFINDINFO LVFindInfo;
			ZeroMemory(&LVFindInfo,sizeof(LVFindInfo));

			//设置变量
			LVFindInfo.flags=LVFI_PARAM;
			LVFindInfo.lParam=(LPARAM)pIClientUserItem;

			//定位列表
			INT nItem=m_UserListControl.FindItem(&LVFindInfo);

			//设置列表
			if (nItem!=-1L)
			{
				m_UserListControl.EnsureVisible(nItem,FALSE);
				m_UserListControl.SetItemState(nItem,LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);
			}

			return TRUE;
		}
	case IDM_SET_CHAT_USER:			//交谈对象
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//设置聊天
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			if (pIClientUserItem!=NULL) m_ChatControl.SetChatTargetUser(pIClientUserItem);

			return TRUE;
		}
	case IDM_CANCEL_CHAT_USER:		//取消交谈
		{
			//设置聊天
			m_ChatControl.SetChatTargetUser(NULL);

			return TRUE;
		}
	case IDM_SET_FRIEND_USER:		//设为好友
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//变量定义
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_FRIEND)?CP_NORMAL:CP_FRIEND;

			//变量定义
			ASSERT(CUserInformation::GetInstance()!=NULL);
			CUserInformation * pUserInformation=CUserInformation::GetInstance();

			//设置关系
			pIClientUserItem->SetUserCompanion(cbDestCompanion);
			pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);

			//更新列表
			m_UserListControl.UpdateDataItem(pIClientUserItem);

			return TRUE;
		}
	case IDM_SET_DETEST_USER:		//设为厌恶
		{
			//效验状态
			ASSERT(m_MenuUserItemArray.GetCount()>0);
			if (m_MenuUserItemArray.GetCount()==0) return TRUE;

			//变量定义
			IClientUserItem * pIClientUserItem=m_MenuUserItemArray[0];
			BYTE cbSourCompanion=pIClientUserItem->GetUserCompanion();
			BYTE cbDestCompanion=(cbSourCompanion==CP_DETEST)?CP_NORMAL:CP_DETEST;

			//变量定义
			ASSERT(CUserInformation::GetInstance()!=NULL);
			CUserInformation * pUserInformation=CUserInformation::GetInstance();

			//设置关系
			pIClientUserItem->SetUserCompanion(cbDestCompanion);
			pUserInformation->InsertCompanionInfo(pIClientUserItem,cbDestCompanion);

			//更新列表
			m_UserListControl.UpdateDataItem(pIClientUserItem);

			return TRUE;
		}
	case IDM_MESSAGE_SHOW_ALL:		//显示所有
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_ALL;

			return TRUE;
		}
	case IDM_MESSAGE_HIDE_DETEST:	//屏蔽厌恶
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_DETEST;

			return TRUE;
		}
	case IDM_MESSAGE_ONLY_FRIEND:	//显示好友
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal::GetInstance()->m_cbMessageMode=MESSAGE_MODE_FRIEND;

			return TRUE;
		}
	case IDM_MANAGER_SERVER:		//房间管理
		{
			//变量定义
			CDlgServerManager DlgServerManager;
			DlgServerManager.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);

			//设置变量
			m_pDlgServerManager=&DlgServerManager;

			//显示窗口
			DlgServerManager.DoModal();

			//设置变量
			m_pDlgServerManager=NULL;

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//鼠标双击
VOID CServerViewItem::OnDButtonHitTable(WORD wTableID)
{
	return;
}

//左键按下
VOID CServerViewItem::OnLButtonHitTable(WORD wTableID, WORD wChairID)
{
	//动作过虑
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return;
	if ((wTableID==m_wReqTableID)&&(wChairID==m_wReqChairID)) return;

	//获取用户
	IClientUserItem * pTableUserItem=NULL;
	if (wChairID!=INDEX_ENTER_CHAIR) pTableUserItem=m_TableViewFrame.GetClientUserItem(wTableID,wChairID);

	//随机座位
	if (CServerRule::IsAvertCheatMode(m_dwServerRule)==true)
	{
		PerformSitDownAction(INVALID_TABLE,INVALID_CHAIR,false);
		return;
	}

	//消息处理
	if (pTableUserItem!=NULL)
	{
		if (pTableUserItem==m_pIMySelfUserItem)
		{
			//状态过滤
			if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//起立动作
			PerformStandUpAction(wTableID,wChairID);
		}
		else
		{
			//状态过滤
			WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
			WORD wMeChairID=m_pIMySelfUserItem->GetChairID();
			if ((wMeTableID==wTableID)&&(wMeChairID==wChairID)) return;
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) return;

			//旁观动作
			PerformLookonAction(wTableID,wChairID);
		}
	}
	else
	{
		//激活界面
		if (wChairID==INDEX_ENTER_CHAIR)
		{
			//变量定义
			WORD wUserTableID=m_pIMySelfUserItem->GetTableID();
			BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

			//激活界面
			if ((wTableID==wUserTableID)&&(cbUserStatus!=US_LOOKON))
			{
				m_ProcessManager.SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
				return;
			}
		}

		//坐下动作
		PerformSitDownAction(wTableID,wChairID,true);
	}

	return;
}

//右键按下
VOID CServerViewItem::OnRButtonHitTable(WORD wTableID, WORD wChairID)
{
	//点击位置
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//获取用户
		IClientUserItem * pIClientUserItem=m_TableViewFrame.GetClientUserItem(wTableID,wChairID);

		//用户菜单
		if (pIClientUserItem!=NULL)
		{
			TrackUserItemMenu(pIClientUserItem);
		}

		return;
	}

	return;
}

//连接事件
bool CServerViewItem::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//错误提示
	if (nErrorCode!=0L)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("很抱歉，游戏房间连接失败，请稍后再试或留意网站公告！"),MB_ICONSTOP);

		return true;
	}

	//发送登录
	SendLogonPacket();

	//设置状态
	SetServiceStatus(ServiceStatus_Validate);

	return true;
}

//关闭事件
bool CServerViewItem::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//关闭状态
	m_DlgStatus.HideStatusWindow();

	//关闭游戏
	m_ProcessManager.CloseGameProcess(false);

	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if (m_ServiceStatus==ServiceStatus_ServiceIng)
		{
			//提示消息
			m_ChatMessage.InsertSystemString(TEXT("与游戏服务器的连接中断了"));

			//关闭提示
			CInformation Information(this);
			INT_PTR nResult=Information.ShowMessageBox(m_GameServer.szServerName,TEXT("与游戏服务器的连接已经中断，是否关闭当前游戏房间？"),
				MB_ICONERROR|MB_YESNO|MB_DEFBUTTON2,0L);

			//关闭房间
			if (nResult==IDYES)
			{
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM);
			}
		}
		else
		{
			//关闭提示
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,TEXT("由于意外的原因，房间连接失败，请稍后重新尝试！"),MB_ICONERROR);

			//关闭房间
			PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
		}
	}

	//设置状态
	m_ServiceStatus=ServiceStatus_NetworkDown;

	return true;
}

//读取事件
bool CServerViewItem::OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:		//登录消息
		{
			return OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_CONFIG:		//配置信息
		{
			return OnSocketMainConfig(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_USER:		//用户信息
		{
			return OnSocketMainUser(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_STATUS:		//状态信息
		{
			return OnSocketMainStatus(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_INSURE:		//银行消息
		{
			return OnSocketMainInsure(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_MANAGE:		//管理消息
		{
			return OnSocketMainManager(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_CM_SYSTEM:		//系统消息
		{
			return OnSocketMainSystem(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_GF_GAME:		//游戏消息
	case MDM_GF_FRAME:		//框架消息
		{
			return OnSocketMainGameFrame(Command.wMainCmdID,Command.wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//购买道具
bool CServerViewItem::OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	//效验参数
	ASSERT((pszNickName!=NULL)&&(wItemCount>0));
	if ((pszNickName==NULL)||(wItemCount==0)) return false;

	//变量定义
	ASSERT(CGamePropertyManager::GetInstance()!=NULL);
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();

	//查找道具
	ASSERT(pGamePropertyManager->GetPropertyItem(wPropertyIndex)!=NULL);
	CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->GetPropertyItem(wPropertyIndex);

	//查找用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByNickName(pszNickName);

	//用户判断
	if (pIClientUserItem==NULL)
	{
		return false;
	}

	//变量定义
	CMD_GR_C_BuyProperty BuyProperty;
	ZeroMemory(&BuyProperty,sizeof(BuyProperty));

	//设置变量
	BuyProperty.wItemCount=wItemCount;
	BuyProperty.wPropertyIndex=wPropertyIndex;
	BuyProperty.dwTargetUserID=pIClientUserItem->GetUserID();

	//发送消息
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_BUY_PROPERTY,&BuyProperty,sizeof(BuyProperty));

	return true;
}

//进程错误
bool CServerViewItem::OnGameProcessError()
{
	return true;
}

//进程关闭
bool CServerViewItem::OnGameProcessClose(DWORD dwExitCode)
{
	//效验状态
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//变量定义
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();
	BYTE cbUserStatus=m_pIMySelfUserItem->GetUserStatus();

	//退出游戏
	if ((wTableID!=INVALID_TABLE)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		SendStandUpPacket(wTableID,wChairID,TRUE);
	}

	return true;
}

//进程启动
bool CServerViewItem::OnGameProcessCreate(bool bAlreadyExist)
{
	//变量定义
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	WORD wChairID=m_pIMySelfUserItem->GetChairID();

	//房间配置
	if (bAlreadyExist==false)
	{
		//获取等级
		tagLevelItem LevelItem[64];
		BYTE cbLevelCount=(BYTE)m_GameLevelParserModule->GetGameLevelItem(LevelItem,CountArray(LevelItem));

		//发送配置
		m_ProcessManager.SendLevelInfo(LevelItem,cbLevelCount);
		m_ProcessManager.SendColumnInfo(m_ColumnItem,m_cbColumnCount);
		m_ProcessManager.SendPropertyInfo(m_PropertyInfo,m_cbPropertyCount);
	}

	//房间信息
	m_ProcessManager.SendServerInfo(m_pIMySelfUserItem,m_dwUserRight,m_dwMasterRight,m_dwServerRule,m_wServerType,m_GameServer);

	//发送用户
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		//变量定义
		WORD wEnumIndex=0;
		IPlazaUserManager * pIPlazaUserManager=m_PlazaUserManagerModule.GetInterface();

		//发送玩家
		while (true)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()==US_LOOKON) continue;

			//发送用户
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};

		//旁观用户
		wEnumIndex=0;
		while (true)
		{
			//获取用户
			IClientUserItem * pIClientUserItem=pIPlazaUserManager->EnumUserItem(wEnumIndex++);
			if (pIClientUserItem==NULL) break;

			//发送判断
			if (pIClientUserItem->GetTableID()!=wTableID) continue;
			if (pIClientUserItem->GetUserStatus()!=US_LOOKON) continue;

			//发送用户
			m_ProcessManager.SendUserItem(pIClientUserItem);
		};
	}

	//配置完成
	m_ProcessManager.SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_CONFIG_FINISH);

	return true;
}

//进程数据
bool CServerViewItem::OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//网络消息
	if ((wMainCmdID==IPC_CMD_GF_SOCKET)&&(wSubCmdID==IPC_SUB_GF_SOCKET_SEND))
	{
		//效验数据
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//变量定义
		IPC_GF_SocketSend * pSocketPackage=(IPC_GF_SocketSend *)pData;
		WORD wPacketSize=wDataSize-(sizeof(IPC_GF_SocketSend)-sizeof(pSocketPackage->cbBuffer));

		//处理数据
		if (wPacketSize==0)
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID);
		}
		else 
		{
			TCP_Command * pCommand=&pSocketPackage->CommandInfo;
			m_TCPSocketModule->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wPacketSize);
		}

		return true;
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//用户激活
VOID CServerViewItem::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//判断自己
	if (m_pIMySelfUserItem==NULL)
	{
		m_pIMySelfUserItem=pIClientUserItem;
	}

	//插入列表
	m_UserListControl.InsertDataItem(pIClientUserItem);

	//设置桌子
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		WORD wTableID=pIClientUserItem->GetTableID();
		WORD wChairID=pIClientUserItem->GetChairID();
		m_TableViewFrame.SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	//提示信息
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if ((pParameterGlobal->m_bNotifyUserInOut==true)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
	}

	return;
}

//用户删除
VOID CServerViewItem::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//变量定义
	WORD wLastTableID=pIClientUserItem->GetTableID();
	WORD wLastChairID=pIClientUserItem->GetChairID();
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//变量定义
	ASSERT(CParameterGlobal::GetInstance());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//离开处理
	if ((wLastTableID!=INVALID_TABLE)&&(wLastChairID!=INVALID_CHAIR))
	{
		//获取用户
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_TableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,NULL);

		//离开通知
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.cbUserStatus=US_NULL;
			UserStatus.wTableID=INVALID_TABLE;
			UserStatus.wChairID=INVALID_CHAIR;
			m_ProcessManager.SendUserStatus(pIClientUserItem->GetUserID(),UserStatus);
		}
	}

	//聊天对象
	m_ChatControl.DeleteUserItem(pIClientUserItem);

	//删除列表
	m_UserListControl.DeleteDataItem(pIClientUserItem);

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemDelete(pIClientUserItem,this);

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserLeave(pIClientUserItem);
	}

	//菜单对象
	for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
	{
		//获取用户
		IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
		if (pIChatUserItem->GetUserID()==dwLeaveUserID) m_MenuUserItemArray.RemoveAt(i);
	}

	//提示信息
	if ((pParameterGlobal->m_bNotifyUserInOut==true)&&(m_ServiceStatus==ServiceStatus_ServiceIng))
	{
		m_ChatMessage.InsertUserLeave(pIClientUserItem->GetNickName());
	}

	return;
}

//用户更新
VOID CServerViewItem::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//列表更新
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//更新桌子
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->cbUserStatus!=US_LOOKON))
	{
		m_TableViewFrame.UpdateTableView(pUserInfo->wTableID);
	}

	//更新游戏
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(m_pIMySelfUserItem->GetTableID()==pUserInfo->wTableID))
	{
		m_ProcessManager.SendCustomFace(pUserInfo->dwUserID,pUserInfo->dwCustomID,pCustomFaceInfo);
	}

	return;
}

//用户更新
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();

	//列表更新
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//游戏通知
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		//变量定义
		tagUserScore UserScore;
		ZeroMemory(&UserScore,sizeof(UserScore));

		//设置变量
		UserScore.lScore=pUserInfo->lScore;
		UserScore.lGrade=pUserInfo->lGrade;
		UserScore.lInsure=pUserInfo->lInsure;
		UserScore.dwWinCount=pUserInfo->dwWinCount;
		UserScore.dwLostCount=pUserInfo->dwLostCount;
		UserScore.dwDrawCount=pUserInfo->dwDrawCount;
		UserScore.dwFleeCount=pUserInfo->dwFleeCount;
		UserScore.dwUserMedal=pUserInfo->dwUserMedal;
		UserScore.dwExperience=pUserInfo->dwExperience;
		UserScore.dwLoveLiness=pUserInfo->dwLoveLiness;

		//发送数据
		m_ProcessManager.SendUserScore(pUserInfo->dwUserID,UserScore);
	}

	return;
}

//用户更新
VOID CServerViewItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	WORD wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	WORD wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	BYTE cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;

	//列表更新
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	//查找窗口
	CDlgSearchUser * pDlgSearchUser=CDlgSearchUser::GetInstance();
	if (pDlgSearchUser!=NULL) pDlgSearchUser->OnUserItemUpdate(pIClientUserItem,this);

	//私聊窗口
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if (pDlgWhisper->m_hWnd!=NULL) pDlgWhisper->OnEventUserStatus(pIClientUserItem);
	}

	//桌子离开
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem) m_TableViewFrame.SetClientUserItem(wLastTableID,wLastChairID,NULL);
	}

	//桌子加入
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		m_TableViewFrame.SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}

	//桌子状态
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		m_TableViewFrame.UpdateTableView(wNowTableID);
	}

	//离开通知
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
		if ((pIClientUserItem==m_pIMySelfUserItem)||(wLastTableID==pMeUserInfo->wTableID))
		{
			tagUserStatus UserStatus;
			UserStatus.wTableID=wNowTableID;
			UserStatus.wChairID=wNowChairID;
			UserStatus.cbUserStatus=cbNowStatus;
			m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);
		}

		return;
	}

	//加入处理
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//游戏通知
		if ((m_pIMySelfUserItem!=pIClientUserItem)&&(pMeUserInfo->wTableID==wNowTableID))
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_ProcessManager.SendUserItem(pIClientUserItem);
		}

		//自己判断
		if (m_pIMySelfUserItem==pIClientUserItem)
		{
			//设置变量
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;

			//启动进程
			m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);
		}

		return;
	}

	//状态改变
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		//游戏通知
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;
		m_ProcessManager.SendUserStatus(pUserInfo->dwUserID,UserStatus);

		return;
	}

	return;
}

//取消连接
VOID CServerViewItem::OnStatusCancel()
{
	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return;
}

//拆分事件
VOID CServerViewItem::OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//显示控件
	if (m_bHideUserList==true)
	{
		//设置变量
		m_bRectifyHor=true;
		m_bHideUserList=false;
	
		//设置按钮
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_btControl.SetButtonImage(IDB_BK_USER_LIST,(m_bHideUserList==true)?TEXT("BT_USER_LIST_SHOW"):TEXT("BT_USER_LIST_HIDE"),hInstance,true,false);
	}

	//调整控件
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//用户选择
VOID CServerViewItem::OnChangeChatTarget(IClientUserItem * pIClientUserItem)
{
	//设置变量
	if (pIClientUserItem!=NULL)
	{
		//设置界面
		if (m_ChatControl.m_hWnd==NULL)
		{
			//创建窗口
			CRect rcCreate(0,0,0,0);
			m_ChatControl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CHAT_CONTROL);

			//调整窗口
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}
	else
	{
		//设置界面
		if (m_ChatControl.m_hWnd!=NULL)
		{
			//销毁窗口
			m_ChatControl.DestroyWindow();

			//调整界面
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());
		}
	}

	//设置焦点
	m_ChatEdit.SetFocus();

	return;
}

//表情事件
VOID CServerViewItem::OnExpressionSelect(CExpression * pUserExpression, tagExpressionInfo * pExpressionInfo)
{
	//发送表情
	if (m_ServiceStatus==ServiceStatus_ServiceIng)
	{
		//设置焦点
		m_ChatEdit.SetFocus();

		//发送表情
		if (EfficacyUserChat(NULL,pUserExpression->GetIndex())==true)
		{
			SendExpressionPacket(m_ChatControl.GetChatTargetUserID(),pUserExpression->GetIndex());
		}
	}

	return;
}

//显示菜单
VOID CServerViewItem::TrackUserItemMenu(IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//判断状态
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if (CServerRule::IsAvertCheatMode(m_dwServerRule)==true) return;

	//设置变量
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.Add(pIClientUserItem);

	//构造菜单
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//变量定义
	TCHAR szMenuString[256]=TEXT("");
	bool bMeUserItem=(pIClientUserItem==m_pIMySelfUserItem);

	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//变量定义
	LPCTSTR pszUserNote=pIClientUserItem->GetUserNoteInfo();
	CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
	IGameLevelParser * pGameLevelParser=m_GameLevelParserModule.GetInterface();

	//用户信息
	WORD wTableID=pUserInfo->wTableID;
	WORD wChairID=pUserInfo->wChairID;
	BYTE cbUserStatus=pUserInfo->cbUserStatus;

	//玩家信息
	WORD wMeTableID=pMeUserInfo->wTableID;
	WORD wMeChiarID=pMeUserInfo->wChairID;
	BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

	//插入名片
	CImage ImageUserCard;
	pUserItemElement->ConstructNameCard(pIClientUserItem,pGameLevelParser,ImageUserCard);
	if (ImageUserCard.IsNull()==false) UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,ImageUserCard,MF_GRAYED);

	//插入分割
	UserInfoMenu.AppendSeparator();

	//常规菜单
	if (bMeUserItem==false)
	{
		UserInfoMenu.AppendMenu(IDM_CREATE_ISER_WISPER,TEXT("发送私聊消息 ..."));
		UserInfoMenu.SetDefaultItem(IDM_CREATE_ISER_WISPER,MF_BYCOMMAND);
	}
	UserInfoMenu.AppendMenu(IDM_COPY_USER_NICKNAME,TEXT("复制昵称"));

	//设置交谈
	if ((bMeUserItem==false)&&(pIChatTargetItem!=m_pIMySelfUserItem)&&(pIChatTargetItem!=pIClientUserItem))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("与 [ %s ] 交谈"),pIClientUserItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_SET_CHAT_USER,szMenuString);
	}

	//取消交谈
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("取消与 [ %s ] 交谈"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//操作菜单
	if ((bMeUserItem==false)&&(cbMeUserStatus!=US_PLAYING))
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//旁观游戏
		bool bEnableMenu=((wTableID!=INVALID_TABLE)&&((cbUserStatus==US_PLAYING)||(cbUserStatus==US_OFFLINE)));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("旁观 [ %s ] 游戏"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_LOOKON_USER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);

		//一起游戏
		bEnableMenu=((wTableID!=INVALID_TABLE)&&((cbUserStatus==US_SIT)||(cbUserStatus==US_READY)));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("坐到 [ %s ] 的游戏桌"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_PLAY_GAME_TOGETHER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);

		//邀请游戏
		bEnableMenu=((wMeTableID!=INVALID_TABLE)&&(cbMeUserStatus!=US_LOOKON)&&(cbUserStatus!=US_PLAYING)&&(cbUserStatus!=US_OFFLINE));
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("邀请 [ %s ] 一起玩游戏"),pUserInfo->szNickName);
		UserInfoMenu.AppendMenu(IDM_INVITE_USER,szMenuString,(bEnableMenu==false)?MF_GRAYED:0);
	}

	//好友管理
	if (bMeUserItem==false)
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//加入菜单
		BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
		UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("设置为好友"),(cbCompanion==CP_FRIEND)?MF_CHECKED:0);
		UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("设置为厌恶"),(cbCompanion==CP_DETEST)?MF_CHECKED:0);
	}

	//管理菜单
	if ((bMeUserItem==false)&&(m_pIMySelfUserItem->GetMasterOrder()!=0))
	{
		//插入分割
		/*UserInfoMenu.AppendSeparator();

		//管理菜单
		UserInfoMenu.AppendMenu(IDM_KILL_USER,TEXT("踢用户下线..."),(CMasterRight::CanKillUser(m_dwMasterRight)==false)?MF_GRAYED:0);
		
		//玩家管理
		UINT nMenuFlags=(CMasterRight::CanLimitUserChat(m_dwMasterRight)==false)?MF_GRAYED:0;
		UserInfoMenu.AppendSeparator();
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_ROOM_CHAT,TEXT("禁止该用户大厅聊天"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_ROOM_CHAT,TEXT("允许该用户大厅聊天"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_GAME_CHAT,TEXT("禁止该用户游戏聊天"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_GAME_CHAT,TEXT("允许该用户游戏聊天"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_LIMIT_USER_WHISP_CHAT,TEXT("禁止该用户私聊"),nMenuFlags);
		UserInfoMenu.AppendMenu(IDM_ALLOW_USER_WHISP_CHAT,TEXT("允许该用户私聊"),nMenuFlags);*/
	}

	//插入分割
	UserInfoMenu.AppendSeparator();

	//查看位置
	UserInfoMenu.AppendMenu(IDM_SEE_USER_LOCATION,(bMeUserItem==true)?TEXT("查看我的位置"):TEXT("查看用户位置"));
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) UserInfoMenu.EnableMenuItem(IDM_SEE_USER_LOCATION,MF_BYCOMMAND|MF_GRAYED);

	//功能菜单
	UserInfoMenu.AppendMenu(IDM_SHOW_USER_INFO,TEXT("用户详细资料 ..."),MF_GRAYED);
	UserInfoMenu.AppendMenu(IDM_USER_INFO_MANAGER,TEXT("用户信息管理 ..."),MF_GRAYED);
	UserInfoMenu.AppendMenu(IDM_SEARCH_ONLINE_USER,TEXT("查找在线用户 ..."));

	//插入分割
	UserInfoMenu.AppendSeparator();

	//玩家位置
	if (wTableID!=INVALID_TABLE)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("正在 [ %d ] 号游戏桌"),wTableID+1);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//积分信息
	DWORD dwPlayCount=pIClientUserItem->GetUserPlayCount();
	FLOAT fUserFleeRate=pIClientUserItem->GetUserFleeRate();
	_sntprintf(szMenuString,CountArray(szMenuString),TEXT("成绩：") SCORE_STRING TEXT(" 经验值：%ld 总局数：%ld 逃跑率：%5.2f%%"),
		pUserInfo->lScore,pUserInfo->dwExperience,dwPlayCount,fUserFleeRate);
	UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

	//社团信息
	if (pUserInfo->szGroupName[0]!=0)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("游戏社团：%s"),pUserInfo->szGroupName);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//备注信息
	if ((pszUserNote!=NULL)&&(pszUserNote[0]!=0))
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("备注信息：%s"),pszUserNote);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
	}

	//弹出菜单
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//显示菜单
VOID CServerViewItem::TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wUserCount)
{
	//效验参数
	ASSERT((pIClientUserItem!=NULL)&&(wUserCount>0));
	if ((pIClientUserItem==NULL)||(wUserCount==0)) return;

	//判断状态
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return;
	if (CServerRule::IsAvertCheatMode(m_dwServerRule)==true) return;

	//设置变量
	m_MenuUserItemArray.RemoveAll();
	m_MenuUserItemArray.SetSize(wUserCount);
	CopyMemory(m_MenuUserItemArray.GetData(),pIClientUserItem,wUserCount*sizeof(IClientUserItem *));

	//构造菜单
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//变量定义
	TCHAR szMenuString[256]=TEXT("");
	tagUserInfo * pMeUserInfo=m_pIMySelfUserItem->GetUserInfo();
	IClientUserItem * pIChatTargetItem=m_ChatControl.GetChatTargetUserItem();

	//玩家信息
	WORD wMeTableID=pMeUserInfo->wTableID;
	WORD wMeChiarID=pMeUserInfo->wChairID;
	BYTE cbMeUserStatus=pMeUserInfo->cbUserStatus;

	//发送私聊
	UserInfoMenu.AppendMenu(IDM_CREATE_ISER_WISPER,TEXT("发送群聊消息 ..."),MF_GRAYED);
	UserInfoMenu.SetDefaultItem(IDM_CREATE_ISER_WISPER,MF_BYCOMMAND);

	//聊天对象
	if (pIChatTargetItem!=NULL)
	{
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("取消与 [ %s ] 交谈"),pIChatTargetItem->GetNickName());
		UserInfoMenu.AppendMenu(IDM_CANCEL_CHAT_USER,szMenuString);
	}

	//插入分割
	UserInfoMenu.AppendSeparator();

	//加入菜单
	UserInfoMenu.AppendMenu(IDM_SET_FRIEND_USER,TEXT("设置为好友"));
	UserInfoMenu.AppendMenu(IDM_SET_DETEST_USER,TEXT("设置为厌恶"));

	//插入分割
	UserInfoMenu.AppendSeparator();

	//功能菜单
	UserInfoMenu.AppendMenu(IDM_USER_INFO_MANAGER,TEXT("用户信息管理 ..."));
	UserInfoMenu.AppendMenu(IDM_SEARCH_ONLINE_USER,TEXT("查找在线用户 ..."));

	//弹出菜单
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//显示银行
VOID CServerViewItem::ShowInsureView()
{
	//效验参数
	ASSERT(m_pIMySelfUserItem!=NULL);
	ASSERT(m_TCPSocketModule.GetInterface()!=NULL);

	//变量定义
	CDlgInsureServer DlgInsureServer;
	DlgInsureServer.Initialization(m_TCPSocketModule.GetInterface(),m_pIMySelfUserItem);
	
	//设置变量
	m_pDlgInsureServer=&DlgInsureServer;

	//显示窗口
	DlgInsureServer.DoModal();

	//设置变量
	m_pDlgInsureServer=NULL;

	return;
}

//老板事件
VOID CServerViewItem::NotifyBossCome(bool bBossCome)
{
	return;
}

//查找桌子
VOID CServerViewItem::SearchGameTable(tagSearchTable & SearchTable)
{
	//变量定义
	WORD wNullCount=0;
	WORD wChairUser=m_TableViewFrame.GetChairCount();
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();

	//搜索桌子
	for (WORD i=0;i<m_TableViewFrame.GetTableCount();i++)
	{
		//设置索引
		SearchTable.wResultTableID=(SearchTable.wStartTableID+i)%m_TableViewFrame.GetTableCount();

		//搜索处理
		if (wMeTableID!=SearchTable.wResultTableID)
		{
			//获取桌子
			ITableView * pITableView=m_TableViewFrame.GetTableViewItem(SearchTable.wResultTableID);

			//搜索过虑
			if (pITableView->GetPlayFlag()==true) continue;
			if ((SearchTable.bFilterPass==true)&&(pITableView->GetLockerFlag()==true)) continue;

			//寻找空位置
			wNullCount=pITableView->GetNullChairCount(SearchTable.wResultChairID);

			//判断数目
			if (wNullCount>0)
			{
				//效验规则
				CString strDescribe;
				if (EfficacyTableRule(SearchTable.wResultTableID,SearchTable.wResultChairID,false,strDescribe)==false)
				{
					continue;
				}
		
				//数目判断
				if ((SearchTable.bOneNull==true)&&(wNullCount==1)) return;
				if ((SearchTable.bTwoNull==true)&&(wNullCount==2)) return;
				if ((SearchTable.bNotFull==true)&&(wNullCount<wChairUser)) return;
				if ((SearchTable.bAllNull==true)&&(wNullCount==wChairUser)) return;
			}
		}
	}

	//设置数据
	SearchTable.wResultTableID=INVALID_TABLE;
	SearchTable.wResultChairID=INVALID_CHAIR;

	return;
}

//更新头像
VOID CServerViewItem::UpdateUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)
{
	//查找用户
	ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule.GetInterface()->SearchUserByUserID(dwUserID);

	//更新用户
	if (pIClientUserItem!=NULL)
	{
		ASSERT(m_PlazaUserManagerModule.GetInterface()!=NULL);
		m_PlazaUserManagerModule->UpdateUserCustomFace(pIClientUserItem,dwCustomID,CustomFaceInfo);
	}

	return;
}

//用户处理
bool CServerViewItem::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_ENTER:			//用户进入
		{
			return OnSocketSubUserEnter(pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//用户积分
		{
			return OnSocketSubUserScore(pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//用户状态
		{
			return OnSocketSubUserStatus(pData,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:	//请求失败
		{
			return OnSocketSubRequestFailure(pData,wDataSize);
		}
	case SUB_GR_USER_CHAT:			//用户聊天
		{
			return OnSocketSubUserChat(pData,wDataSize);
		}
	case SUB_GR_USER_EXPRESSION:	//用户表情
		{
			return OnSocketSubExpression(pData,wDataSize);
		}
	case SUB_GR_WISPER_CHAT:		//用户私聊
		{
			return OnSocketSubWisperUserChat(pData,wDataSize);
		}
	case SUB_GR_WISPER_EXPRESSION:	//私聊表情
		{
			return OnSocketSubWisperExpression(pData,wDataSize);
		}
	}

	return true;
}

//登录处理
bool CServerViewItem::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:	//登录成功
		{
			return OnSocketSubLogonSuccess(pData,wDataSize);
		}
	case SUB_GR_LOGON_FAILURE:	//登录失败
		{
			return OnSocketSubLogonFailure(pData,wDataSize);
		}
	case SUB_GR_LOGON_FINISH:	//登录完成
		{
			return OnSocketSubLogonFinish(pData,wDataSize);
		}
	case SUB_GR_UPDATE_NOTIFY:	//更新提示
		{
			return OnSocketSubUpdateNotify(pData,wDataSize);
		}
	}

	return true;
}

//配置处理
bool CServerViewItem::OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:		//列表配置
		{
			//变量定义
			CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

			//效验参数
			ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=wDataSize) return false;

			//保存信息
			m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
			CopyMemory(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

			//设置列表
			m_UserListControl.SetColumnDescribe(pConfigColumn->ColumnItem,pConfigColumn->cbColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_SERVER:		//房间配置
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_GR_ConfigServer));
			if (wDataSize<sizeof(CMD_GR_ConfigServer)) return false;

			//消息处理
			CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)pData;

			//房间属性
			m_wServerType=pConfigServer->wServerType;
			m_dwServerRule=pConfigServer->dwServerRule;

			//构造参数
			WORD wTableCount=pConfigServer->wTableCount;
			WORD wChairCount=pConfigServer->wChairCount;
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);

			//资源目录
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//创建桌子
			bool bSuccess=m_TableViewFrame.ConfigTableFrame(wTableCount,wChairCount,m_dwServerRule,szResDirectory);

			//错误处理
			if (bSuccess==false)
			{
				//隐藏提示
				m_DlgStatus.HideStatusWindow();

				//关闭连接
				m_TCPSocketModule->CloseSocket();

				//提示信息
				CInformation Information(AfxGetMainWnd());
				INT nResult=Information.ShowMessageBox(TEXT("游戏资源文件加载失败或者格式错误，是否现在进行修复？"),MB_ICONERROR|MB_YESNO);

				//下载游戏
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//关闭房间
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	case SUB_GR_CONFIG_PROPERTY:	//道具配置
		{
			//变量定义
			CMD_GR_ConfigProperty * pConfigProperty=(CMD_GR_ConfigProperty *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ConfigProperty)-sizeof(pConfigProperty->PropertyInfo);

			//效验参数
			ASSERT((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))==wDataSize);
			if ((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))!=wDataSize) return false;

			//保存信息
			m_cbPropertyCount=__min(pConfigProperty->cbPropertyCount,CountArray(m_PropertyInfo));
			CopyMemory(m_PropertyInfo,pConfigProperty->PropertyInfo,sizeof(tagPropertyInfo)*m_cbPropertyCount);

			//设置道具
			m_GamePropertyCtrl.SetGamePropertyInfo(m_PropertyInfo,m_cbPropertyCount,PT_ISSUE_AREA_SERVER);

			return true;
		}
	case SUB_GR_CONFIG_FINISH:		//配置完成
		{
			//资源目录
			TCHAR szResDirectory[LEN_KIND]=TEXT("");
			GetGameResDirectory(szResDirectory,CountArray(szResDirectory));

			//游戏等级
			ASSERT(m_GameLevelParserModule.GetInterface()!=NULL);
			bool bSuccess=m_GameLevelParserModule->LoadGameLevelItem(m_GameKind.szKindName,szResDirectory,m_wServerType);

			//错误处理
			if (bSuccess==false)
			{
				//关闭连接
				m_TCPSocketModule->CloseSocket();

				//提示信息
				INT nResult=ShowInformation(TEXT("游戏等级配置读取失败或者格式错误，是否现在进行修复？"),MB_ICONERROR|MB_YESNO);

				//下载游戏
				if (nResult==IDYES)
				{
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wGameID,m_GameServer.wServerID);
				}

				//关闭房间
				PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);
			}

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//状态处理
bool CServerViewItem::OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//桌子信息
		{
			//变量定义
			CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)pData;
			WORD wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatusArray);

			//效验数据
			ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))==wDataSize);
			if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0]))!=wDataSize) return false;

			//消息处理
			for (WORD i=0;i<pTableInfo->wTableCount;i++)
			{
				BYTE cbTableLock=pTableInfo->TableStatusArray[i].cbTableLock;
				BYTE cbPlayStatus=pTableInfo->TableStatusArray[i].cbPlayStatus;
				m_TableViewFrame.SetTableStatus(i,(cbPlayStatus==TRUE),(cbTableLock==TRUE));
			}

			return true;
		}
	case SUB_GR_TABLE_STATUS:	//桌子状态
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GR_TableStatus));
			if (wDataSize!=sizeof(CMD_GR_TableStatus)) return false;

			//变量定义
			CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)pData;

			//设置桌子
			BYTE cbTableLock=pTableStatus->TableStatus.cbTableLock;
			BYTE cbPlayStatus=pTableStatus->TableStatus.cbPlayStatus;
			m_TableViewFrame.SetTableStatus(pTableStatus->wTableID,(cbPlayStatus==TRUE),(cbTableLock==TRUE));

			return true;
		}
	}

	return true;
}

//银行处理
bool CServerViewItem::OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(m_pDlgInsureServer!=NULL);
	if (m_pDlgInsureServer==NULL) return true;

	//消息处理
	if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE,wSubCmdID,pData,wDataSize)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//管理处理
bool CServerViewItem::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GR_OPTION_CURRENT:	//当前配置
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GR_OptionCurrent));
			if (wDataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			//消息处理
			if (m_pDlgServerManager!=NULL)
			{
				CMD_GR_OptionCurrent * pOptionCurrent=(CMD_GR_OptionCurrent *)pData;
				m_pDlgServerManager->SetServerOptionInfo(pOptionCurrent->ServerOptionInfo,pOptionCurrent->dwRuleMask);
			}

			return true;
		}
	}

	return true;
}

//系统处理
bool CServerViewItem::OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CM_SYSTEM_MESSAGE:	//系统消息
		{
			return OnSocketSubSystemMessage(pData,wDataSize);
		}
	case SUB_CM_ACTION_MESSAGE:	//动作消息
		{
			return OnSocketSubActionMessage(pData,wDataSize);
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//游戏处理
bool CServerViewItem::OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;

	//构造数据
	IPC_GF_SocketRecv SocketRecv;
	SocketRecv.CommandInfo.wSubCmdID=wSubCmdID;
	SocketRecv.CommandInfo.wMainCmdID=wMainCmdID;

	//叠加数据
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketRecv.cbBuffer,pData,wDataSize);
	}

	//发送数据
	WORD wSendSize=sizeof(SocketRecv)-sizeof(SocketRecv.cbBuffer)+wDataSize;
	m_ProcessManager.SendProcessData(IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_RECV,&SocketRecv,wSendSize);

	return true;
}

//用户进入
bool CServerViewItem::OnSocketSubUserEnter(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead)) return false;

	//变量定义
	tagUserInfo UserInfo;
	tagCustomFaceInfo CustomFaceInfo;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

	//消息处理
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bHideUserInfo=CServerRule::IsAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserInfoHead->dwUserID;
	bool bMasterUserOrder=((pUserInfoHead->cbMasterOrder>0)||((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetMasterOrder()>0)));

	//读取信息
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		//用户属性
		UserInfo.wFaceID=pUserInfoHead->wFaceID;
		UserInfo.dwGameID=pUserInfoHead->dwGameID;
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
		UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

		//用户状态
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//用户属性
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

		//用户积分
		UserInfo.lScore=pUserInfoHead->lScore;
		UserInfo.lGrade=pUserInfoHead->lGrade;
		UserInfo.lInsure=pUserInfoHead->lInsure;
		UserInfo.dwWinCount=pUserInfoHead->dwWinCount;
		UserInfo.dwLostCount=pUserInfoHead->dwLostCount;
		UserInfo.dwDrawCount=pUserInfoHead->dwDrawCount;
		UserInfo.dwFleeCount=pUserInfoHead->dwFleeCount;
		UserInfo.dwUserMedal=pUserInfoHead->dwUserMedal;
		UserInfo.dwExperience=pUserInfoHead->dwExperience;
		UserInfo.dwLoveLiness=pUserInfoHead->dwLoveLiness;

		//变量定义
		VOID * pDataBuffer=NULL;
		tagDataDescribe DataDescribe;
		CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));

		//扩展信息
		while (true)
		{
			pDataBuffer=RecvPacket.GetData(DataDescribe);
			if (DataDescribe.wDataDescribe==DTP_NULL) break;
			switch (DataDescribe.wDataDescribe)
			{
			case DTP_GR_NICK_NAME:		//用户昵称
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szNickName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szNickName))
					{
						CopyMemory(&UserInfo.szNickName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szNickName[CountArray(UserInfo.szNickName)-1]=0;
					}
					break;
				}
			case DTP_GR_GROUP_NAME:		//用户社团
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szGroupName))
					{
						CopyMemory(&UserInfo.szGroupName,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szGroupName[CountArray(UserInfo.szGroupName)-1]=0;
					}
					break;
				}
			case DTP_GR_UNDER_WRITE:	//个性签名
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite));
					if (DataDescribe.wDataSize<=sizeof(UserInfo.szUnderWrite))
					{
						CopyMemory(UserInfo.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
						UserInfo.szUnderWrite[CountArray(UserInfo.szUnderWrite)-1]=0;
					}
					break;
				}
			}
		}

		//自定头像
		if (pUserInfoHead->dwCustomID!=0L)
		{
			//加载头像
			CCustomFaceManager * pCustomFaceManager=CCustomFaceManager::GetInstance();
			bool bSuccess=pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID,CustomFaceInfo);

			//下载头像
			if (bSuccess==false)
			{
				pCustomFaceManager->LoadUserCustomFace(pUserInfoHead->dwUserID,pUserInfoHead->dwCustomID);
			}
		}
	}
	else
	{
		//用户信息
		UserInfo.dwUserID=pUserInfoHead->dwUserID;
		lstrcpyn(UserInfo.szNickName,TEXT("游戏玩家"),CountArray(UserInfo.szNickName));

		//用户状态
		UserInfo.wTableID=pUserInfoHead->wTableID;
		UserInfo.wChairID=pUserInfoHead->wChairID;
		UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

		//用户属性
		UserInfo.cbGender=pUserInfoHead->cbGender;
		UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	}

	//激活用户
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(UserInfo.dwUserID);
	if (pIClientUserItem==NULL) pIClientUserItem=m_PlazaUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo);

	return true;
}

//用户积分
bool CServerViewItem::OnSocketSubUserScore(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore)) return false;

	//寻找用户
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bHideUserInfo=CServerRule::IsAvertCheatMode(m_dwServerRule);
	bool bMySelfUserItem=pGlobalUserData->dwUserID==pUserScore->dwUserID;
	bool bMasterUserOrder=((pIClientUserItem->GetMasterOrder()>0)||(m_pIMySelfUserItem->GetMasterOrder()>0));

	//全局信息
	if (bMySelfUserItem==true)
	{
		//设置奖牌
		DWORD dwLastMedal=pIClientUserItem->GetUserMedal();
		DWORD dwCurrentMedal=pUserScore->UserScore.dwUserMedal;

		//设置奖牌
		if (dwCurrentMedal>dwLastMedal)
		{
			//设置变量
			pGlobalUserData->dwUserMedal+=(dwCurrentMedal-dwLastMedal);

			//事件通知
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
		}
	}

	//更新用户
	if ((bHideUserInfo==false)||(bMySelfUserItem==true)||(bMasterUserOrder==true))
	{
		m_PlazaUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);
	}

	return true;
}

//用户状态
bool CServerViewItem::OnSocketSubUserStatus(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus)) return false;

	//寻找用户
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

	//用户判断
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return true;

	//设置状态
	if (pUserStatus->UserStatus.cbUserStatus==US_NULL) 
	{
		//删除用户
		m_PlazaUserManagerModule->DeleteUserItem(pIClientUserItem);
	}
	else
	{
		//更新用户
		m_PlazaUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);
	}

	return true;
}

//登录完成
bool CServerViewItem::OnSocketSubLogonFinish(VOID * pData, WORD wDataSize)
{
	//关闭提示
	m_DlgStatus.HideStatusWindow();

	//设置状态
	SetServiceStatus(ServiceStatus_ServiceIng);

	//激活房间
	CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
	if (pPlatformFrame!=NULL) pPlatformFrame->ActiveServerViewItem(this);

	//查找按钮
	if (CServerRule::IsAvertCheatMode(m_dwServerRule)==false)
	{
		m_btTableButton2.EnableWindow(TRUE);
	}

	//锁桌判断
	if (CServerRule::IsForfendLockTable(m_dwServerRule)==false)
	{
		//设置按钮
		m_btTableButton3.EnableWindow(TRUE);
	}

	//规则判断
	if (CServerRule::IsForfendGameRule(m_dwServerRule)==false)
	{
		//发送规则
		SendUserRulePacket();

		//设置按钮
		m_btTableButton4.EnableWindow(TRUE);
	}

	//重入判断
	if ((m_pIMySelfUserItem!=NULL)&&(m_pIMySelfUserItem->GetTableID()!=INVALID_TABLE))
	{
		//启动进程	
		bool bSuccess=m_ProcessManager.CreateGameProcess(m_GameKind.szProcessName);

		//错误处理
		if (bSuccess==false)
		{
			//中断连接
			m_TCPSocketModule->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetworkDown;
		}
	}

	return true;
}

//登录成功
bool CServerViewItem::OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_LogonSuccess));
	if (wDataSize!=sizeof(CMD_GR_LogonSuccess)) return false;

	//消息处理
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)pData;

	//玩家属性
	m_dwUserRight=pLogonSuccess->dwUserRight;
	m_dwMasterRight=pLogonSuccess->dwMasterRight;

	//设置状态
	SetServiceStatus(ServiceStatus_RecvInfo);

	//关闭提示
	m_DlgStatus.ShowStatusWindow(TEXT("正在读取房间信息..."));

	//构造地址
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/GameRoom.asp?KindID=%ld&ServerID=%ld"),szPlatformLink,m_GameKind.wKindID,m_GameServer.wServerID);

	//连接广告
	m_PlatformPublicize.Navigate(szBillUrl);
	m_PlatformPublicize.SetBoradColor(RGB(0,0,0));

	return true;
}

//登录失败
bool CServerViewItem::OnSocketSubLogonFailure(VOID * pData, WORD wDataSize)
{
	//效验参数
	CMD_GR_LogonFailure * pLogonFailure=(CMD_GR_LogonFailure *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString)));
	if (wDataSize<(sizeof(CMD_GR_LogonFailure)-sizeof(pLogonFailure->szDescribeString))) return false;

	//关闭提示
	m_DlgStatus.HideStatusWindow();

	//关闭处理
	m_TCPSocketModule->CloseSocket();

	//设置状态
	SetServiceStatus(ServiceStatus_NetworkDown);

	//显示消息
	LPCTSTR pszDescribeString=pLogonFailure->szDescribeString;
	if (lstrlen(pszDescribeString)>0) ShowInformation(pszDescribeString,MB_ICONERROR,60);

	//关闭房间
	PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//更新提示
bool CServerViewItem::OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UpdateNotify));
	if (wDataSize!=sizeof(CMD_GR_UpdateNotify)) return false;

	//变量定义
	CMD_GR_UpdateNotify * pUpdateNotify=(CMD_GR_UpdateNotify *)pData;

	//大厅更新
	if (pUpdateNotify->cbMustUpdatePlaza==TRUE)
	{
		//关闭处理
		m_DlgStatus.HideStatusWindow();
		m_TCPSocketModule->CloseSocket();

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//下载大厅
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		if (pGlobalUnits!=NULL) pGlobalUnits->DownLoadClient(TEXT("游戏广场"),0,0);

		return true;
	}

	//游戏更新
	if ((pUpdateNotify->cbMustUpdateClient==TRUE)||(pUpdateNotify->cbAdviceUpdateClient==TRUE))
	{
		//关闭处理
		if (pUpdateNotify->cbMustUpdateClient==TRUE)
		{
			m_DlgStatus.HideStatusWindow();
			m_TCPSocketModule->CloseSocket();
		}

		//构造提示
		if (pUpdateNotify->cbMustUpdateClient==FALSE)
		{
			//构造提示
			TCHAR szDescribe[512]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("“%s”已经更新了，我们强烈建议您进行更新，现在进行更新吗？"),m_GameKind.szKindName);

			//提示消息
			CInformation Information(this);
			if (Information.ShowMessageBox(szDescribe,MB_ICONINFORMATION|MB_YESNO,0)!=IDYES) return true;
		}

		//关闭房间
		PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

		//更新提示
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->DownLoadClient(m_GameKind.szKindName,m_GameKind.wKindID,m_GameServer.wServerID);
	}

	return true;
}

//请求失败
bool CServerViewItem::OnSocketSubRequestFailure(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_RequestFailure * pRequestFailure=(CMD_GR_RequestFailure *)pData;

	//效验参数
	ASSERT(wDataSize>(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString)));
	if (wDataSize<=(sizeof(CMD_GR_RequestFailure)-sizeof(pRequestFailure->szDescribeString))) return false;

	//变量定义
	IClientUserItem * pITableUserItem=NULL;
	WORD wMeTableID=m_pIMySelfUserItem->GetTableID();
	WORD wMeChairID=m_pIMySelfUserItem->GetChairID();

	//界面还原
	if (m_wReqTableID!=INVALID_TABLE)
	{
		IClientUserItem * pITableUserItem=m_TableViewFrame.GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pITableUserItem==m_pIMySelfUserItem) m_TableViewFrame.SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//设置变量
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;

	//提示消息
	if (pRequestFailure->szDescribeString[0]!=0)
	{
		CInformation Information;
		Information.ShowMessageBox(pRequestFailure->szDescribeString,MB_ICONINFORMATION,30L);
	}

	return true;
}

//用户聊天
bool CServerViewItem::OnSocketSubUserChat(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_S_UserChat * pUserChat=(CMD_GR_S_UserChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//显示消息
	if (pUserChat->dwTargetUserID!=0L)
	{
		//获取用户
		IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;

		//显示消息
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
		m_ChatMessage.InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}
	else 
	{
		//显示消息
		LPCTSTR pszSendUser=pISendUserItem->GetNickName();
		m_ChatMessage.InsertUserChat(pszSendUser,pUserChat->szChatString,pUserChat->dwChatColor);
	}

	return true;
}

//用户表情
bool CServerViewItem::OnSocketSubExpression(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_S_UserExpression)) return false;

	//变量定义
	CMD_GR_S_UserExpression * pUserExpression=(CMD_GR_S_UserExpression *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//公聊消息
	if (pUserExpression->dwTargetUserID==0L)
	{
		//变量定义
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//插入表情
		if (pExpressionItem!=NULL)
		{
			//获取路径
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//插入消息
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			m_ChatMessage.InsertExpression(pszSendUser,szImagePath);
		}
	}

	//私聊消息
	if (pUserExpression->dwTargetUserID!=0L)
	{
		//变量定义
		CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
		CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

		//插入表情
		if (pExpressionItem!=NULL)
		{
			//获取路径
			TCHAR szImagePath[MAX_PATH]=TEXT("");
			pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

			//获取用户
			IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pUserExpression->dwTargetUserID);
			if (pIRecvUserItem==NULL) return true;

			//显示消息
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
			m_ChatMessage.InsertExpression(pszSendUser,pszRecvUser,szImagePath);
		}
	}

	return true;
}

//用户私聊
bool CServerViewItem::OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GR_S_WisperChat * pWisperChat=(CMD_GR_S_WisperChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_S_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//接收用户
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//创建窗口
	DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//显示窗口
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventWisperChat(pISendUserItem,pWisperChat->szChatString,pWisperChat->dwChatColor);
	}

	return true;
}

//私聊表情
bool CServerViewItem::OnSocketSubWisperExpression(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_S_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_S_WisperExpression)) return false;

	//变量定义
	CMD_GR_S_WisperExpression * pWisperExpression=(CMD_GR_S_WisperExpression *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//接收用户
	IClientUserItem * pIRecvUserItem=m_PlazaUserManagerModule->SearchUserByUserID(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//消息过虑
	if (pISendUserItem!=m_pIMySelfUserItem)
	{
		//社团变量
		DWORD dwUserGroupID=pISendUserItem->GetGroupID();
		DWORD dwMySelfGroupID=m_pIMySelfUserItem->GetGroupID();

		//社团判断
		bool bSameGroup=false;
		if ((dwMySelfGroupID!=0L)&&(dwMySelfGroupID==dwUserGroupID)) bSameGroup=true;

		//变量定义
		BYTE cbUserCompanion=pISendUserItem->GetUserCompanion();
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//好友模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//创建窗口
	DWORD dwUserID[2]={0L,0L};
	dwUserID[0]=m_pIMySelfUserItem->GetUserID();
	dwUserID[1]=(pISendUserItem==m_pIMySelfUserItem)?pIRecvUserItem->GetUserID():pISendUserItem->GetUserID();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,dwUserID,CountArray(dwUserID),(pISendUserItem!=m_pIMySelfUserItem));

	//显示窗口
	if (pDlgWhisper!=NULL)
	{
		pDlgWhisper->OnEventExpression(pISendUserItem,pWisperExpression->wItemIndex);
	}

	return true;
}

//系统消息
bool CServerViewItem::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	//关闭处理
	if ((pSystemMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭处理
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//状态设置
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//显示消息
	if (pSystemMessage->wType&SMT_CHAT) 
	{
		m_ChatMessage.InsertSystemString(pSystemMessage->szString);
	}

	//弹出消息
	if (pSystemMessage->wType&SMT_EJECT)
	{
		CInformation Information;
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION);
	}

	//关闭房间
	if (pSystemMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//动作消息
bool CServerViewItem::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(WCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(WCHAR)))) return false;

	//关闭处理
	if ((pActionMessage->wType&(SMT_CLOSE_ROOM|SMT_CLOSE_LINK))!=0)
	{
		//关闭提示
		m_DlgStatus.HideStatusWindow();

		//关闭处理
		m_TCPSocketModule->CloseSocket();
		m_ProcessManager.CloseGameProcess(false);

		//状态设置
		m_ServiceStatus=ServiceStatus_NetworkDown;
	}

	//弹出消息
	CInformation Information;
	INT nResultCode=Information.ShowMessageBox(pActionMessage->szString,MB_ICONINFORMATION);

	//变量定义
	WORD wExcursion=wHeadSize+pActionMessage->wLength*sizeof(TCHAR);

	//提取动作
	while (wExcursion<wDataSize)
	{
		//变量定义
		tagActionHead * pActionHead=(tagActionHead *)((BYTE *)pData+wExcursion);

		//效验参数
		ASSERT((wExcursion+sizeof(tagActionHead))<=wDataSize);
		ASSERT((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)<=wDataSize);

		//效验参数
		if ((wExcursion+sizeof(tagActionHead))>wDataSize) return false;
		if ((wExcursion+sizeof(tagActionHead)+pActionHead->wAppendSize)>wDataSize) return false;

		//动作出来
		if (nResultCode==pActionHead->uResponseID)
		{
			switch (pActionHead->cbActionType)
			{
			case ACT_BROWSE:	//浏览动作
				{
					//变量定义
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionBrowse * pActionBrowse=(tagActionBrowse *)((BYTE *)pData+wDataPos);

					//I E 浏览
					if (pActionBrowse->cbBrowseType&BRT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					//大厅浏览
					if (pActionBrowse->cbBrowseType&BRT_PLAZA)
					{
						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true);
					}

					break;
				}
			case ACT_DOWN_LOAD:	//下载动作
				{
					//变量定义
					WORD wDataPos=wExcursion+sizeof(tagActionHead);
					tagActionDownLoad * pActionDownLoad=(tagActionDownLoad *)((BYTE *)pData+wDataPos);

					//I E 下载
					if (pActionDownLoad->cbDownLoadMode&DLT_IE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//下载模块
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("OPEN"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//增加偏移
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//关闭房间
	if (pActionMessage->wType&SMT_CLOSE_ROOM) PostMessage(WM_COMMAND,IDM_DELETE_SERVER_ITEM,0);

	return true;
}

//调整控件
VOID CServerViewItem::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//屏幕位置
	CRect rcMainWnd;
	AfxGetMainWnd()->GetClientRect(&rcMainWnd);

	//获取位置
	CRect rcVorSplitter;
	CRect rcHorSplitter;
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	m_HorSplitter.GetWindowRect(&rcHorSplitter);

	//位置转换
	ScreenToClient(&rcVorSplitter);
	ScreenToClient(&rcHorSplitter);

	//竖向范围
	INT nVorMinPos=rcMainWnd.Width()/2;
	INT nVorMaxPos=nWidth-rcMainWnd.Width()*6/25;
	m_VorSplitter.SetSplitterRange(nVorMinPos,nVorMaxPos);

	//横向范围
	INT nHorMinPos=nHeight*3/10;
	INT nHorMaxPos=nHeight*6/10;
	m_HorSplitter.SetSplitterRange(nHorMinPos,nHorMaxPos);

	//竖向拆分
	if (m_bHideUserList==false)
	{
		if (m_bRectifyVor==false)
		{
			m_bRectifyVor=true;
			rcVorSplitter.top=0;
			rcVorSplitter.bottom=nHeight;
			rcVorSplitter.left=nWidth-rcMainWnd.Width()*6/25;
			rcVorSplitter.right=rcVorSplitter.left+SPLITTER_CX;
		}
	}
	else
	{
		rcVorSplitter.top=0;
		rcVorSplitter.bottom=nHeight;
		rcVorSplitter.left=nWidth-SPLITTER_CX;
		rcVorSplitter.right=rcVorSplitter.left+SPLITTER_CX;
	}

	//横向拆分
	if (m_bRectifyHor==false)
	{
		m_bRectifyHor=true;
		rcHorSplitter.top=nHeight*INCISE_SCALE-PROPERTY_CY;
		rcHorSplitter.bottom=rcHorSplitter.top+SPLITTER_CY;
	}

	//限制位置
	if ((nWidth!=0)&&(nHeight!=0))
	{
		//竖向拆分
		if (m_bHideUserList==false)
		{
			if (rcVorSplitter.left<nVorMinPos)
			{
				rcVorSplitter.left=nVorMinPos;
				rcVorSplitter.right=rcVorSplitter.left+SPLITTER_CX;
			}
			if (rcVorSplitter.right>nVorMaxPos)
			{
				rcVorSplitter.right=nVorMaxPos;
				rcVorSplitter.left=rcVorSplitter.right-SPLITTER_CX;
			}
		}

		//横向拆分
		if (rcHorSplitter.top<nHorMinPos)
		{
			rcHorSplitter.top=nHorMinPos;
			rcHorSplitter.bottom=rcHorSplitter.top+SPLITTER_CY;
		}
		if (rcHorSplitter.bottom>nHorMaxPos)
		{
			rcHorSplitter.bottom=nHorMaxPos;
			rcHorSplitter.top=rcHorSplitter.bottom-SPLITTER_CY;
		}
	}

	//桌子区域
	CRect rcTableArea;
	rcTableArea.top=0;
	rcTableArea.left=0;
	rcTableArea.bottom=nHeight;
	rcTableArea.right=rcVorSplitter.left;

	//列表区域
	CRect rcListArea;
	rcListArea.right=nWidth;
	rcListArea.top=PUBLICIZE_CY;
	rcListArea.left=rcVorSplitter.right;
	rcListArea.bottom=rcHorSplitter.top;

	//聊天区域
	CRect rcChatArea;
	rcChatArea.right=nWidth;
	rcChatArea.bottom=nHeight;
	rcChatArea.left=rcVorSplitter.right;
	rcChatArea.top=rcHorSplitter.bottom+PROPERTY_CY;

	//环绕信息
	tagEncircleInfo EncircleInfoChat;
	m_ChatEncircle.GetEncircleInfo(EncircleInfoChat);

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//调整拆分
	DeferWindowPos(hDwp,m_VorSplitter,NULL,rcVorSplitter.left,rcVorSplitter.top,rcVorSplitter.Width(),rcVorSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_HorSplitter,NULL,rcVorSplitter.right,rcHorSplitter.top,nWidth-rcVorSplitter.right,rcHorSplitter.Height(),uFlags);

	//桌子按钮
	INT nXPosTable=rcTableArea.right-27;
	CSkinButton * TableButtonArray[]={&m_btTableButton5,&m_btTableButton4,&m_btTableButton3,&m_btTableButton2,&m_btTableButton1};

	//关闭按钮
	CRect rcQuitServer;
	m_btQuitServer.GetWindowRect(&rcQuitServer);
	DeferWindowPos(hDwp,m_btQuitServer,NULL,rcTableArea.right-rcQuitServer.Width()-4,2,0,0,uFlags|SWP_NOSIZE);

	//桌子按钮
	for (INT i=0;i<CountArray(TableButtonArray);i++)
	{
		//获取位置
		CRect rcTableButton;
		TableButtonArray[i]->GetWindowRect(&rcTableButton);

		//移动按钮
		nXPosTable-=(rcTableButton.Width()+TABLE_BUTTON_SX);
		DeferWindowPos(hDwp,TableButtonArray[i]->m_hWnd,NULL,nXPosTable,2,0,0,uFlags|SWP_NOSIZE);
	}

	//聊天按钮
	INT nChatButtonSpace=3;
	INT nXPosChat=rcChatArea.left+3;
	INT nYPosChat=rcChatArea.bottom-50;
	CSkinButton * ChatButtonArray[]={&m_btChatButton1,&m_btChatButton2,&m_btChatButton3,&m_btChatButton4};

	//聊天控件
	UINT uTargetFlag=(m_bHideUserList==false)?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
	DeferWindowPos(hDwp,m_btSendChat,NULL,rcChatArea.right-35,rcChatArea.bottom-22,0,0,uTargetFlag|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatShort,NULL,rcChatArea.right-53,rcChatArea.bottom-22,0,0,uTargetFlag|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_ChatEdit,NULL,rcChatArea.left+5,rcChatArea.bottom-19,rcChatArea.Width()-59,12,uTargetFlag);

	//聊天按钮
	for (INT i=0;i<CountArray(ChatButtonArray);i++)
	{
		//获取位置
		CRect rcChatButton;
		ChatButtonArray[i]->GetWindowRect(&rcChatButton);

		//显示判断
		bool bShowButton=(m_bHideUserList==false);
		if ((nXPosChat+rcChatButton.Width()+nChatButtonSpace)>(rcChatArea.right-7)) bShowButton=false;

		//移动按钮
		UINT uTargetFlag=(bShowButton==true)?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
		DeferWindowPos(hDwp,ChatButtonArray[i]->m_hWnd,NULL,nXPosChat,nYPosChat,0,0,uFlags|SWP_NOSIZE|uTargetFlag);

		//设置间距
		nXPosChat+=(rcChatButton.Width()+nChatButtonSpace);
	}

	//环绕控件
	m_TableEncircle.DeferWindowPos(&m_TableViewFrame,hDwp,rcTableArea);
	m_UserListEncircle.DeferWindowPos(&m_UserListControl,hDwp,rcListArea);

	//移动控件
	DeferWindowPos(hDwp,m_PlatformPublicize,NULL,rcListArea.left+1,0,rcListArea.Width()-6,PUBLICIZE_CY-3,uFlags);
	DeferWindowPos(hDwp,m_GamePropertyCtrl,NULL,rcListArea.left,rcHorSplitter.bottom,rcListArea.Width(),PROPERTY_CY,uFlags);

	//聊天控制
	if ((m_ChatControl.m_hWnd!=NULL)&&(m_ChatControl.GetChatTargetUserItem()!=NULL))
	{
		//获取位置
		CRect rcChatControl;
		m_ChatControl.GetWindowRect(&rcChatControl);

		//位置信息
		tagEncircleInfo EncircleInfo;
		m_ChatEncircle.GetEncircleInfo(EncircleInfo);

		//聊天控件
		rcChatArea.bottom-=rcChatControl.Height();
		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,rcChatArea);

		//聊天控制
		DeferWindowPos(hDwp,m_ChatControl,NULL,rcChatArea.left+EncircleInfo.nLBorder,rcChatArea.bottom-EncircleInfo.nBBorder,
			rcChatArea.Width()-EncircleInfo.nLBorder-EncircleInfo.nRBorder,rcChatControl.Height(),uFlags);
	}
	else
	{
		//聊天控件
		m_ChatEncircle.DeferWindowPos(&m_ChatMessage,hDwp,rcChatArea);
	}

	//移动结束
	EndDeferWindowPos(hDwp);

	//控制按钮
	CRect rcControl;
	m_btControl.GetClientRect(&rcControl);
	m_btControl.SetWindowPos(NULL,(rcVorSplitter.Width()-rcControl.Width())/2,(rcVorSplitter.Height()-rcControl.Height())/2,0,0,uFlags|SWP_NOSIZE);

	//重画界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

	return;
}

//配置房间
VOID CServerViewItem::InitServerViewItem(CGameServerItem * pGameServerItem)
{
	//设置状态
	SetServiceStatus(ServiceStatus_Entering);

	//房间属性
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
	CopyMemory(&m_GameKind,&pGameKindItem->m_GameKind,sizeof(m_GameKind));
	CopyMemory(&m_GameServer,&pGameServerItem->m_GameServer,sizeof(m_GameServer));

	//关闭判断
	ASSERT(m_GameServer.wServerID!=0);
	if (m_GameServer.wServerID==0) throw TEXT("很抱歉，此游戏房间已经关闭了，不允许继续进入！");

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//创建组件
	if (m_TCPSocketModule.CreateInstance()==false) throw TEXT("无法创建网络连接组件，游戏房间进入失败！");

	//加载配置
	m_pParameterGame=pParameterGlobal->GetParameterGame(&m_GameKind);
	m_pParameterServer=pParameterGlobal->GetParameterServer(&m_GameServer);

	//变量定义
	tagPropertyResource PropertyResource;
	ZeroMemory(&PropertyResource,sizeof(PropertyResource));

	//设置道具
	PropertyResource.uImageL=IDB_GAME_PROPERTY_L;
	PropertyResource.uImageM=IDB_GAME_PROPERTY_M;
	PropertyResource.uImageR=IDB_GAME_PROPERTY_R;
	PropertyResource.uImageItem=IDB_GAME_PROPERTY_ITEM;
	PropertyResource.hResInstance=AfxGetInstanceHandle();

	//道具位置
	PropertyResource.PointExcursion.x=4;
	PropertyResource.PointExcursion.y=10;
	PropertyResource.SizeExcursion.cx=38;
	PropertyResource.SizeExcursion.cy=39;

	//设置道具
	m_GamePropertyCtrl.SetPropertyResource(PropertyResource);
	m_GamePropertyCtrl.SetGamePropertySink(QUERY_ME_INTERFACE(IUnknownEx));

	//设置网络
	m_TCPSocketModule->SetTCPSocketSink(QUERY_ME_INTERFACE(IUnknownEx));
	m_TCPSocketModule->SetProxyServerInfo(pParameterGlobal->m_cbProxyType,pParameterGlobal->m_ProxyServer);

	//发起连接
	if (m_TCPSocketModule->Connect(m_GameServer.szServerAddr,m_GameServer.wServerPort)!=CONNECT_SUCCESS)
	{
		throw TEXT("游戏房间连接失败，请检查系统网络配置参数是否正确！");
		return;
	}

	//创建提示
	m_DlgStatus.ShowStatusWindow(TEXT("正在进入游戏房间，请稍后..."));

	//构造标题
	TCHAR szServerTitle[128]=TEXT("");
	_sntprintf(szServerTitle,CountArray(szServerTitle),TEXT("[ %s ] - %s"),m_GameKind.szKindName,m_GameServer.szServerName);

	//设置标题
	SetWindowText(szServerTitle);

	return;
}

//设置界面
VOID CServerViewItem::InitServerViewCtrl(CSkinRenderManager * pSkinRenderManager)
{
	//构造变量
	tagColorUserList ColorUserList;
	ZeroMemory(&ColorUserList,sizeof(ColorUserList));

	//颜色定义
	ColorUserList.crSelectTX=COLOR_SELECT_TX;
	ColorUserList.crSelectBK=COLOR_SELECT_BK;
	ColorUserList.crNormalTX=COLOR_NORMAL_TX;
	ColorUserList.crNormalBK=COLOR_NORMAL_BK;

	//颜色定义
	ColorUserList.crMyselfTX=COLOR_MYSELF_TX;
	ColorUserList.crMyselfBK=COLOR_MYSELF_BK;
	ColorUserList.crMasterTX=COLOR_MASTER_TX;
	ColorUserList.crMasterBK=COLOR_MASTER_BK;
	ColorUserList.crMemberTX=COLOR_MEMBER_TX;
	ColorUserList.crMemberBK=COLOR_MEMBER_BK;

	//设置控件
	m_UserListControl.SetColorUserList(ColorUserList);

	return;
}

//私聊会话
VOID CServerViewItem::WhisperConversation(IClientUserItem * pIClientUserItem[], WORD wUserCount)
{
	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng)
	{
		return;
	}

	//用户属性
	WORD wTableID=m_pIMySelfUserItem->GetTableID();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//变量定义
	bool bSameTable=false;
	bool bMasterUser=(cbMasterOrder>0);

	//管理判断
	if (bMasterUser==false)
	{
		for (WORD i=0;i<wUserCount;i++)
		{
			if (pIClientUserItem[i]->GetMasterOrder()>0)
			{
				bMasterUser=true;
				break;
			}
		}
	}

	//同桌判断
	if (wTableID!=INVALID_TABLE)
	{
		for (WORD i=0;i<wUserCount;i++)
		{
			if (pIClientUserItem[i]->GetTableID()==wTableID)
			{
				bSameTable=true;
				break;
			}
		}
	}

	//权限判断
	if (CUserRight::CanWisper(m_dwUserRight)==false)
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("很抱歉，您没有发送私聊信息的权限！"),MB_ICONQUESTION);
		return;
	}

	//私聊权限
	if (CServerRule::IsForfendWisperChat(m_dwServerRule)&&(bMasterUser==false))
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("很抱歉，此游戏房间不允许玩家私聊！"),MB_ICONQUESTION);
		return;
	}

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//创建参数
	CWHArray<DWORD> CUserIDArray;
	CUserIDArray.Add(pGlobalUserData->dwUserID);

	//插入用户
	for (WORD i=0;i<wUserCount;i++)
	{
		//用户过虑
		if (pIClientUserItem[i]->GetUserID()==0L) continue;
		if (pIClientUserItem[i]==m_pIMySelfUserItem) continue;

		//插入数组
		CUserIDArray.Add(pIClientUserItem[i]->GetUserID());
	}

	//创建私聊
	WORD wUserIDArray=(WORD)CUserIDArray.GetCount();
	CDlgWhisper * pDlgWhisper=CreateWhisperWnd(0L,CUserIDArray.GetData(),wUserIDArray,true);

	//显示窗口
	if (pDlgWhisper!=NULL)
	{
		//显示窗口
		pDlgWhisper->ShowWindow(SW_RESTORE);

		//置顶窗口
		pDlgWhisper->SetActiveWindow();
		pDlgWhisper->BringWindowToTop();

		//删除窗口
		CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
		if (pPlatformFrame!=NULL) pPlatformFrame->RemoveWhisperItem(pDlgWhisper);
	}

	return;
}

//创建私聊
CDlgWhisper * CServerViewItem::CreateWhisperWnd(DWORD dwConversationID, DWORD dwUserID[], WORD wUserCount, bool bCreate)
{
	//寻找现存
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//获取窗口
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if ((pDlgWhisper->m_hWnd==NULL)||(pDlgWhisper->CompareUserItem(dwUserID,wUserCount)==false)) continue;

		return pDlgWhisper;
	}

	//废弃判断
	for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
	{
		//获取窗口
		CDlgWhisper * pDlgWhisper=m_DlgWhisperItemArray[i];
		if ((pDlgWhisper->m_hWnd==NULL)||(pDlgWhisper->DisuseEstimate()==false)) continue;

		//变量定义
		CClientUserItemArray ClientUserItemArray;
		ClientUserItemArray.Add(m_pIMySelfUserItem);

		//构造参数
		for (WORD i=0;i<wUserCount;i++)
		{
			IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID[i]);
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem!=m_pIMySelfUserItem)) ClientUserItemArray.Add(pIClientUserItem);
		}

		//创建窗口
		ASSERT(ClientUserItemArray.GetCount()==wUserCount);
		WORD wUserItemCount=(WORD)ClientUserItemArray.GetCount();
		pDlgWhisper->CreateWhisperWnd(dwConversationID,ClientUserItemArray.GetData(),wUserItemCount,GetDesktopWindow());

		return pDlgWhisper;
	}

	//创建窗口
	if (bCreate==true)
	{
		//变量定义
		CDlgWhisper * pDlgWhisper=NULL;
		CClientUserItemArray ClientUserItemArray;
		ClientUserItemArray.Add(m_pIMySelfUserItem);

		//寻找关闭
		for (INT_PTR i=0;i<m_DlgWhisperItemArray.GetCount();i++)
		{
			//获取窗口
			CDlgWhisper * pDlgWhisperTemp=m_DlgWhisperItemArray[i];
			if (pDlgWhisperTemp->GetSafeHwnd()==NULL) 
			{
				pDlgWhisper=pDlgWhisperTemp;
				break;
			}
		}

		//创建窗口
		if (pDlgWhisper==NULL)
		{
			try
			{
				pDlgWhisper=new CDlgWhisper;
				if (pDlgWhisper==NULL) return NULL;
				m_DlgWhisperItemArray.Add(pDlgWhisper);
			}
			catch (...) { return NULL; }
		}

		//构造参数
		for (WORD i=0;i<wUserCount;i++)
		{
			IClientUserItem * pIClientUserItem=m_PlazaUserManagerModule->SearchUserByUserID(dwUserID[i]);
			if ((pIClientUserItem!=NULL)&&(pIClientUserItem!=m_pIMySelfUserItem)) ClientUserItemArray.Add(pIClientUserItem);
		}

		//用户数目
		ASSERT(ClientUserItemArray.GetCount()==wUserCount);
		WORD wUserItemCount=(WORD)ClientUserItemArray.GetCount();

		//设置窗口
		pDlgWhisper->SetServerViewItem(this);
		pDlgWhisper->SetMySelfUserItem(m_pIMySelfUserItem);
		pDlgWhisper->SetTCPSocket(m_TCPSocketModule.GetInterface());
		pDlgWhisper->SetGameLevelParser(m_GameLevelParserModule.GetInterface());
		pDlgWhisper->SetUserOrderParser(theApp.m_UserOrderParserModule.GetInterface());

		//创建窗口
		pDlgWhisper->CreateWhisperWnd(dwConversationID,ClientUserItemArray.GetData(),wUserItemCount,GetDesktopWindow());

		return pDlgWhisper;
	}

	return NULL;
}

//发送登录
bool CServerViewItem::SendLogonPacket()
{
	//变量定义
	CMD_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//变量定义
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//游戏版本
	LPCTSTR pszProcessName=m_GameKind.szProcessName;
	CWHService::GetModuleVersion(pszProcessName,LogonUserID.dwProcessVersion);

	//附加信息
	LogonUserID.dwPlazaVersion=pGlobalUnits->GetPlazaVersion();
	LogonUserID.dwFrameVersion=pGlobalUnits->GetFrameVersion();

	//登录信息
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;
	lstrcpyn(LogonUserID.szPassword,pGlobalUserData->szPassword,CountArray(LogonUserID.szPassword));

	//机器标识
	CWHService::GetMachineID(LogonUserID.szMachineID);

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//发送配置
bool CServerViewItem::SendUserRulePacket()
{
	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	CMD_GR_UserRule * pUserRule=(CMD_GR_UserRule *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GR_UserRule),sizeof(cbBuffer)-sizeof(CMD_GR_UserRule));

	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//构造数据
	pUserRule->cbRuleMask=0x00;

	//限制地址
	if (pParameterGlobal->m_bLimitSameIP==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_SAME_IP;
	}

	//限制胜率
	if (m_pParameterGame->m_bLimitWinRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_WIN_RATE;
		pUserRule->wMinWinRate=m_pParameterGame->m_wMinWinRate;
	}

	//限制逃率
	if (m_pParameterGame->m_bLimitFleeRate==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_FLEE_RATE;
		pUserRule->wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
	}

	//限制积分
	if (m_pParameterGame->m_bLimitGameScore==true)
	{
		pUserRule->cbRuleMask|=UR_LIMIT_GAME_SCORE;
		pUserRule->lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		pUserRule->lMinGameScore=m_pParameterGame->m_lMinGameScore;
	}

	//携带密码
	if ((CServerRule::IsForfendLockTable(m_dwServerRule)==false)&&(m_pParameterServer->m_bTakePassword==true))
	{
		SendPacket.AddPacket(m_pParameterServer->m_szPassword,DTP_GR_TABLE_PASSWORD);
	}

	//发送数据
	WORD wSendSize=sizeof(CMD_GR_UserRule)+SendPacket.GetDataSize();
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_RULE,cbBuffer,wSendSize);

	return true;
}

//发送旁观
bool CServerViewItem::SendLookonPacket(WORD wTableID, WORD wChairID)
{
	//变量定义
	CMD_GR_UserLookon UserLookon;
	ZeroMemory(&UserLookon,sizeof(UserLookon));

	//构造数据
	UserLookon.wTableID=wTableID;
	UserLookon.wChairID=wChairID;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON,&UserLookon,sizeof(UserLookon));

	return true;
}

//发送坐下
bool CServerViewItem::SendSitDownPacket(WORD wTableID, WORD wChairID)
{
	//变量定义
	CMD_GR_UserSitDown UserSitDown;
	ZeroMemory(&UserSitDown,sizeof(UserSitDown));

	//构造数据
	UserSitDown.wTableID=wTableID;
	UserSitDown.wChairID=wChairID;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitDown,sizeof(UserSitDown));

	return true;
}

//发送起立
bool CServerViewItem::SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	//变量定义
	CMD_GR_UserStandUp UserStandUp;
	ZeroMemory(&UserStandUp,sizeof(UserStandUp));

	//构造数据
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//发送数据
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));

	return true;
}

//发送聊天
bool CServerViewItem::SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//构造信息
	CMD_GR_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//构造数据
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//发送命令
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//发送表情
bool CServerViewItem::SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex)
{
	//变量定义
	CMD_GR_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//构造信息
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;

	//发送命令
	m_TCPSocketModule->SendData(MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//执行旁观
bool CServerViewItem::PerformLookonAction(WORD wTableID, WORD wChairID)
{
	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//权限判断
	if (CUserRight::CanLookon(m_dwUserRight)==false)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("抱歉，您暂时没有旁观游戏的权限！"));

		return false;
	}

	//清理界面
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_TableViewFrame.GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_TableViewFrame.SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//设置变量
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;

	//设置界面
	m_TableViewFrame.VisibleTable(wTableID);

	//发送命令
	SendLookonPacket(wTableID,wChairID);

	return true;
}

//执行起立
bool CServerViewItem::PerformStandUpAction(WORD wTableID, WORD wChairID)
{
	//效验数据
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID==wTableID)&&(m_wReqChairID==wChairID)) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//设置变量
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;

	//设置界面
	m_TableViewFrame.VisibleTable(wTableID);

	//发送命令
	SendStandUpPacket(wTableID,wChairID,FALSE);

	return true;
}

//执行坐下
bool CServerViewItem::PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass)
{
	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqTableID==wTableID)) return false;
	if ((m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID==wChairID)) return false;

	//自己状态
	if (m_pIMySelfUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"));

		return false;
	}

	//权限判断
	if (CUserRight::CanPlay(m_dwUserRight)==false)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(m_GameServer.szServerName,TEXT("抱歉，您暂时没有加入游戏的权限！"));

		return false;
	}

	//桌子效验
	if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		CString strDescribe;
		if ((wChairID!=INDEX_ENTER_CHAIR)&&(EfficacyTableRule(wTableID,wChairID,false,strDescribe)==false))
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(m_GameServer.szServerName,strDescribe,MB_ICONINFORMATION,30L);

			return false;
		}
	}

	//密码判断
	if ((bEfficacyPass==true)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR)&&(wChairID!=INDEX_ENTER_CHAIR))
	{
		//变量定义
		bool bLocker=m_TableViewFrame.GetLockerFlag(wTableID);
		bool bTakePassword=m_pParameterServer->m_bTakePassword;

		//密码判断
		if ((bLocker==true)&&(bTakePassword==false))
		{
			//设置密码
			CDlgTablePassword DlgTablePassword;
			if (DlgTablePassword.DoModal()!=IDOK) return false;

			//设置密码
			m_pParameterServer->m_bTakePassword=true;
			lstrcpyn(m_pParameterServer->m_szPassword,DlgTablePassword.m_szPassword,CountArray(m_pParameterServer->m_szPassword));

			//发送规则
			SendUserRulePacket();
		}
	}

	//清理界面
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR)&&(m_wReqChairID!=INDEX_ENTER_CHAIR))
	{
		IClientUserItem * pIClientUserItem=m_TableViewFrame.GetClientUserItem(m_wReqTableID,m_wReqChairID);
		if (pIClientUserItem==m_pIMySelfUserItem) m_TableViewFrame.SetClientUserItem(m_wReqTableID,m_wReqChairID,NULL);
	}

	//设置界面
	if ((wChairID!=INDEX_ENTER_CHAIR)&&(wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
	{
		m_TableViewFrame.VisibleTable(wTableID);
		m_TableViewFrame.SetClientUserItem(wTableID,wChairID,m_pIMySelfUserItem);
	}

	//设置变量
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;

	//发送命令
	SendSitDownPacket(wTableID,wChairID);

	return true;
}

//设置状态
VOID CServerViewItem::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//设置变量
	m_ServiceStatus=ServiceStatus;

	return;
}

//资源目录
VOID CServerViewItem::GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount)
{
	//变量定义
	WORD wStringIndex=0;

	//构造目录
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,m_GameKind.szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

	//字符终止
	szResDirectory[wStringIndex]=0;

	return;
}

//聊天效验
bool CServerViewItem::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	//变量定义
	ASSERT(m_pIMySelfUserItem!=NULL);
	BYTE cbMemberOrder=m_pIMySelfUserItem->GetMemberOrder();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//房间配置
	if (CServerRule::IsForfendRoomChat(m_dwServerRule)&&(cbMasterOrder==0))
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n“%s”发送失败"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("抱歉，当前此游戏房间禁止用户大厅聊天！"));

		return false;
	}

	//权限判断
	if (CUserRight::CanRoomChat(m_dwUserRight)==false)
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n“%s”发送失败"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("抱歉，您没有大厅发言的权限，若需要帮助，请联系游戏客服咨询！"));

		return false;
	}

	//速度判断
	DWORD dwCurrentTime=(DWORD)time(NULL);
	if ((cbMasterOrder==0)&&((dwCurrentTime-m_dwChatTime)<=TIME_USER_CHAT))
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n“%s”发送失败"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("您的说话速度太快了，请坐下来喝杯茶休息下吧！"));

		return false;
	}

	//设置变量
	m_dwChatTime=dwCurrentTime;
	m_strChatString=pszChatString;

	return true;
}

//桌子效验
bool CServerViewItem::EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe)
{
	//效验参数
	ASSERT(wTableID<m_TableViewFrame.GetTableCount());
	ASSERT(wChairID<m_TableViewFrame.GetChairCount());

	//状态过滤
	if (m_ServiceStatus!=ServiceStatus_ServiceIng) return false;
	if (wTableID>=m_TableViewFrame.GetTableCount()) return false;
	if (wChairID>=m_TableViewFrame.GetChairCount()) return false;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	ITableView * pITableView=m_TableViewFrame.GetTableViewItem(wTableID);
	IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//变量定义
	bool bGameStart=pITableView->GetPlayFlag();
	bool bDynamicJoin=CServerRule::IsDynamicJoin(m_dwServerRule);

	//游戏状态
	if ((bGameStart==true)&&(bDynamicJoin==false)&&(bReqLookon==false))
	{
		strDescribe=TEXT("此游戏桌已经开始游戏了，暂时不能加入！");
		return false;
	}

	//其他判断
	if ((bReqLookon==false)&&(m_pIMySelfUserItem->GetMasterOrder()==0))
	{
		//规则判断
		for (WORD i=0;i<m_TableViewFrame.GetChairCount();i++)
		{
			//获取用户
			IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==NULL)||(pITableUserItem==m_pIMySelfUserItem)) continue;

			//厌恶玩家
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pITableUserItem->GetUserCompanion()==CP_DETEST)
				{
					//设置提示
					LPCTSTR pszNickName=pITableUserItem->GetNickName();
					strDescribe.Format(TEXT("您设置了不与不受欢迎的玩家游戏，此桌有您不欢迎的玩家 [ %s ] ！"),pszNickName);

					return false;
				}
			}

			//胜率效验
			if (m_pParameterGame->m_bLimitWinRate==true)
			{
				if (((WORD)(pITableUserItem->GetUserWinRate()*1000L))<m_pParameterGame->m_wMinWinRate)
				{
					strDescribe.Format(TEXT("[ %s ] 的胜率太低了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//逃率效验
			if (m_pParameterGame->m_bLimitFleeRate)
			{
				if (((WORD)(pITableUserItem->GetUserFleeRate()*1000L))<m_pParameterGame->m_wMaxFleeRate)
				{
					strDescribe.Format(TEXT("[ %s ] 的逃率太高了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}

			//积分效验
			if (m_pParameterGame->m_bLimitGameScore)
			{
				//最高积分
				if (pITableUserItem->GetUserScore()>m_pParameterGame->m_lMaxGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] 的积分太高了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}

				//最低积分
				if (pITableUserItem->GetUserScore()<m_pParameterGame->m_lMinGameScore)
				{
					strDescribe.Format(TEXT("[ %s ] 的积分太低了，与您的设置不符！"),pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}

	return true;
}

//更多颜色
VOID CServerViewItem::OnSelectMoreColor()
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//设置颜色
	CColorDialog ColorDialog(pParameterGlobal->m_crChatTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pParameterGlobal->m_crChatTX=ColorDialog.GetColor();

	//设置界面
	m_ChatEdit.SetFocus();

	return;
}

//选择颜色
VOID CServerViewItem::OnSelectChatColor(UINT nCommandID)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//选择颜色
	ASSERT(nIndex<CountArray(crColor));
	if (nIndex<CountArray(crColor)) pParameterGlobal->m_crChatTX=crColor[nIndex];

	//设置焦点
	m_ChatEdit.SetFocus();

	return;
}

//选择短语
VOID CServerViewItem::OnSelectChatShort(UINT nCommandID)
{
	//变量定义
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_SHORT;

	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\PhraseInfo.ini"),szDirectory);

	//变量定义
	TCHAR szItemName[16]=TEXT("");
	TCHAR szShortcut[LEN_USER_CHAT]=TEXT("");

	//读取信息
	_sntprintf(szItemName,CountArray(szItemName),TEXT("Phrase%d"),nIndex);
	GetPrivateProfileString(TEXT("ServerChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),szFileName);

	//发送消息
	if (szShortcut[0]!=0)
	{
		//获取信息
		CString strMessage=szShortcut;
		strMessage.TrimLeft();strMessage.TrimRight();

		//发送信息
		if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
		{
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			SendUserChatPacket(m_ChatControl.GetChatTargetUserID(),strMessage,pParameterGlobal->m_crChatTX);
		}
	}

	//设置焦点
	m_ChatEdit.SetFocus();

	return;
}

//销毁消息
VOID CServerViewItem::OnNcDestroy()
{
	__super::OnNcDestroy();

	//删除房间
	try
	{
		delete this;
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//绘画背景
BOOL CServerViewItem::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//拆分位置
	CRect rcVorSplitter;
	CRect rcHorSplitter;
	m_VorSplitter.GetWindowRect(&rcVorSplitter);
	m_HorSplitter.GetWindowRect(&rcHorSplitter);

	//获取位置
	ScreenToClient(&rcVorSplitter);
	ScreenToClient(&rcHorSplitter);

	//绘画桌子
	CRect rcTable;
	rcTable.top=0;
	rcTable.left=0;
	rcTable.right=rcVorSplitter.left;
	rcTable.bottom=rcClient.Height();
	m_TableEncircle.DrawEncircleFrame(pDC,rcTable);

	//绘画聊天
	if (m_bHideUserList==false)
	{
		CRect rcChat;
		rcChat.right=rcClient.Width();
		rcChat.bottom=rcClient.Height();
		rcChat.left=rcVorSplitter.right;
		rcChat.top=rcHorSplitter.bottom+PROPERTY_CY;
		m_ChatEncircle.DrawEncircleFrame(pDC,rcChat);
	}

	//用户列表
	if (m_bHideUserList==false)
	{
		CRect rcUserList;
		rcUserList.top=PUBLICIZE_CY;
		rcUserList.right=rcClient.Width();
		rcUserList.left=rcVorSplitter.right;
		rcUserList.bottom=rcHorSplitter.top;
		m_UserListEncircle.DrawEncircleFrame(pDC,rcUserList);
	}

	//绘画衔接
	ASSERT(CWndViewItemCtrl::GetInstance()!=NULL);
	CWndViewItemCtrl::GetInstance()->DrawLinkBoradLine(this,pDC);

	//获取对象
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//设置 DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(13,45,75));
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//标题信息
	TCHAR szTitle[64]=TEXT("");
	GetWindowText(szTitle,CountArray(szTitle));

	//绘画标题
	CRect rcTitle(15,8,rcVorSplitter.left-TABLE_BUTTON_CX-8,20);
	pDC->DrawText(szTitle,lstrlen(szTitle),&rcTitle,DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

	//绘画广告
	COLORREF crBackGround=pSkinRenderManager->RenderColor(0.67,0.95);
	pDC->FillSolidRect(rcVorSplitter.right,0,rcClient.Width()-rcVorSplitter.right,PUBLICIZE_CY,crBackGround);

	return TRUE;
}

//位置消息
VOID CServerViewItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//设置变量
	m_bRectifyVor=false;

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//右键列表
VOID CServerViewItem::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//弹出菜单
	if (pListNotify->iItem!=-1)
	{
		//选择数目
		UINT nSelectCount=m_UserListControl.GetSelectedCount();

		//选择处理
		if (nSelectCount>1L)
		{
			//用户数组
			IClientUserItem * pIClientUserItem[MAX_WHISPER_USER];
			ZeroMemory(pIClientUserItem,sizeof(pIClientUserItem));

			//变量定义
			WORD wUserCount=0;
			POSITION nSelectPos=m_UserListControl.GetFirstSelectedItemPosition();

			//选择收集
			while ((nSelectPos!=NULL)&&(wUserCount<CountArray(pIClientUserItem)))
			{
				//获取选择
				INT nSelectItem=m_UserListControl.GetNextSelectedItem(nSelectPos);

				//插入用户
				DWORD_PTR lItemData=m_UserListControl.GetItemData(nSelectItem);
				if (lItemData!=NULL) pIClientUserItem[wUserCount++]=((IClientUserItem *)(lItemData));
			};

			//弹出菜单
			if (wUserCount>0) TrackUserItemMenu(pIClientUserItem,wUserCount);
		}
		else
		{
			//弹出菜单
			DWORD_PTR lItemData=m_UserListControl.GetItemData(pListNotify->iItem);
			if (lItemData!=NULL) TrackUserItemMenu((IClientUserItem *)(lItemData));
		}
	}

	return;
}

//双击列表
VOID CServerViewItem::OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	
	//消息处理
	if ((pListNotify->iItem!=-1)&&(pListNotify->lParam!=NULL))
	{
		//获取变量
		DWORD_PTR dwItemData=m_UserListControl.GetItemData(pListNotify->iItem);
		IClientUserItem * pIClientUserItem=(IClientUserItem *)dwItemData;

		//执行动作
		if (pIClientUserItem!=NULL)
		{
			//变量定义
			ASSERT(CParameterGlobal::GetInstance()!=NULL);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

			//执行动作
			switch (pParameterGlobal->m_cbActionLeftDoubleList)
			{
			case ACTION_ORIENTATION:			//定位用户
				{
					//获取属性
					WORD wTableID=pIClientUserItem->GetTableID();
					WORD wChairID=pIClientUserItem->GetChairID();
					BYTE cbUserStatus=pIClientUserItem->GetUserStatus();

					//定位位置
					if (wTableID!=INVALID_TABLE)
					{
						//设置可视
						m_TableViewFrame.VisibleTable(wTableID);

						//闪动椅子
						if (cbUserStatus==US_LOOKON)
						{
							m_TableViewFrame.FlashGameTable(wTableID);
						}
						else
						{
							m_TableViewFrame.FlashGameChair(wTableID,wChairID);
						}
					}

					break;
				}
			case ACTION_SEND_WHISPER:			//发送私聊
				{
					//创建私聊
					if ((m_ServiceStatus==ServiceStatus_ServiceIng)&&(pIClientUserItem!=m_pIMySelfUserItem))
					{
						IClientUserItem * pIClientUserItemArray[]={pIClientUserItem};
						WhisperConversation(pIClientUserItemArray,CountArray(pIClientUserItemArray));
					}

					break;
				}
			case ACTION_SHOW_USER_INFO:		//用户信息
				{
					break;
				}
			}
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
