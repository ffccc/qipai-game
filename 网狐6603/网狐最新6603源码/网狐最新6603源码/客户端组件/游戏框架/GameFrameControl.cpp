#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameControl.h"

//////////////////////////////////////////////////////////////////////////////////

//界面定义
#define PROPERTY_CY					89									//游戏道具
#define USER_VIEW_CY				223									//用户视图
#define MEDAL_VIEW_CY				36									//奖牌视图

//分割参数
#define INCISE_SCALE				50/100								//界面分割

//控件标识
#define IDC_PROPERTY				300									//游戏道具
#define IDC_USER_VIEW				301									//用户信息
#define IDC_MEDAL_VIEW				302									//奖牌信息
#define IDC_CHAT_CONTROL			303									//聊天控制

//////////////////////////////////////////////////////////////////////////////////
//菜单命令

//颜色菜单
#define MAX_CHAT_COLOR				16									//最大数目
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//选择颜色

//快捷短语
#define MAX_SHORT_COUNT				16									//最大数目
#define IDM_SELECT_CHAT_SHORT		(WM_USER+300)						//选择短语

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameControl, CDialog)

	//系统消息
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()

	//控件消息
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST_CONTROL, OnNMRclickUserList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_USER_LIST_CONTROL, OnLvnItemchangedUserList)

	//聊天命令
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_SHORT, (IDM_SELECT_CHAT_SHORT+MAX_SHORT_COUNT), OnSelectChatShort)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameControl::CGameFrameControl() : CDialog(IDD_GAME_CONTROL)
{
	//界面变量
	m_bCreateFlag=false;

	//聊天变量
	m_dwChatTime=0L;

	//接口变量
	m_pIClientKernel=NULL;
	m_pIMySelfUserItem=NULL;

	//控件变量
	m_pExpressionControl=NULL;

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
	m_ChatEncircle.InitEncircleResource(EncircleChat,GetModuleHandle(GAME_FRAME_DLL_NAME));

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
	m_UserListEncircle.InitEncircleResource(EncircleUserList,GetModuleHandle(GAME_FRAME_DLL_NAME));

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_CONTROL,QUERY_ME_INTERFACE(IUnknownEx));
	pGlobalUnits->RegisterGlobalModule(MODULE_STRING_MESSAGE,QUERY_OBJECT_INTERFACE(m_ChatMessage,IUnknownEx));

	return;
}

//析构函数
CGameFrameControl::~CGameFrameControl()
{
	//删除对象
	SafeDelete(m_pExpressionControl);

	return;
}

//接口查询
VOID * CGameFrameControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameFrameControl,Guid,dwQueryVer);
	QUERYINTERFACE(IGamePropertySink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameControl,Guid,dwQueryVer);
	return NULL;
}

//重置控件
VOID CGameFrameControl::ResetControl()
{
	//设置变量
	m_pIMySelfUserItem=NULL;
	m_MenuUserItemArray.RemoveAll();

	//设置聊天
	m_ChatControl.SetChatTargetUser(NULL);

	//设置控件
	m_UserListControl.DeleteAllItems();
	m_GameFrameUserView.SetUserViewInfo(NULL);

	return;
}

//设置广告
VOID CGameFrameControl::SetGameBillInfo(WORD wKindID, WORD wServerID)
{
	//设置广告
	m_GameFrameUserView.SetGameBillInfo(wKindID,wServerID);

	return;
}

//设置用户
VOID CGameFrameControl::SetUserViewInfo(IClientUserItem * pIClientUserItem)
{
	//设置用户
	m_GameFrameUserView.SetUserViewInfo(pIClientUserItem);

	return;
}

//列表配置
VOID CGameFrameControl::SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount)
{
	//设置列表
	m_UserListControl.SetColumnDescribe(ColumnItem,cbColumnCount);

	return;
}

//道具配置
VOID CGameFrameControl::SetPropertyDescribe(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount)
{
	//设置道具
	m_GamePropertyCtrl.SetGamePropertyInfo(PropertyInfo,cbPropertyCount,PT_ISSUE_AREA_GAME);

	return;
}

//购买道具
bool CGameFrameControl::OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex)
{
	return true;
}

//显示菜单
VOID CGameFrameControl::TrackUserItemMenu(IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

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
	IGameLevelParser * pGameLevelParser=CGlobalUnits::GetInstance()->m_GameLevelParserModule.GetInterface();

	//插入名片
	CImage ImageUserCard;
	pUserItemElement->ConstructNameCard(pIClientUserItem,pGameLevelParser,ImageUserCard);
	if (ImageUserCard.IsNull()==false) UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,ImageUserCard,MF_GRAYED);

	//插入分割
	UserInfoMenu.AppendSeparator();

	//常规菜单
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
/*	if ((bMeUserItem==false)&&(cbMeUserStatus!=US_PLAYING)&&(cbUserStatus==US_LOOKON))
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//邀请游戏
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("邀请 [ %s ] 一起玩游戏"),(LPCTSTR)strAccounts);
		UserInfoMenu.AppendMenu(IDM_INVITE_USER,szMenuString);
	}*/

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

	//会员菜单
	/*if( !bMeUserItem && CUserRight::CanKickUser(m_pIClientKernel->GetUserRight()) && pIClientUserItem->GetMasterOrder() == 0 )
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		bool bEnableKick = true;
		if( pMeUserInfo->cbMemberOrder<=pUserInfo->cbMemberOrder || (cbMeUserStatus!=US_SIT&&cbMeUserStatus!=US_READY) ||
			(cbUserStatus!=US_SIT&&cbUserStatus!=US_READY) )
			bEnableKick = false;

		//旁观菜单
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("踢 [ %s ] 离开"),(LPCTSTR)strAccounts);
		UserInfoMenu.AppendMenu(IDM_KICK_USER,szMenuString,bEnableKick?MF_ENABLED:MF_DISABLED);
	}

	tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();
	if(!(CServerRule::IsHideUserInfo(pServerAttribute->dwServerRule)) || pUserInfo->dwUserID==m_pIMySelfUserItem->GetUserID())
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//地区信息
		CW2CT strArea((pUserInfo->szArea[0]==0)?L"--":pUserInfo->szArea);
		CW2CT strCity((pUserInfo->szCity[0]==0)?L"--":pUserInfo->szCity);
		CW2CT strProvince((pUserInfo->szProvince[0]==0)?L"--":pUserInfo->szProvince);
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("省份：%s 城市：%s 地区：%s"),(LPCTSTR)strProvince,(LPCTSTR)strCity,(LPCTSTR)strArea);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

		//积分信息
		LONG lPlayCount=pIClientUserItem->GetUserPlayCount();
		float fUserFleeRate=pIClientUserItem->GetUserFleeRate();
		_sntprintf(szMenuString,CountArray(szMenuString),TEXT("成绩：%I64d  经验值：%ld 总局数：%ld  逃跑率：%5.2f%%"),
			pUserInfo->lScore,pUserInfo->lExperience,lPlayCount,fUserFleeRate);
		UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);

		//社团信息
		if (pUserInfo->szGroupName[0]!=0)
		{
			CW2CT strGroupName(pUserInfo->szGroupName);
			_sntprintf(szMenuString,CountArray(szMenuString),TEXT("游戏社团：%s"),(LPCTSTR)(strGroupName));
			UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
		}

		//备注信息
		LPCWSTR pszUserNote=pIClientUserItem->GetUserNoteInfo();
		if ((pszUserNote!=NULL)&&(pszUserNote[0]!=0))
		{
			CW2CT strUserNote(pIClientUserItem->GetUserNoteInfo());
			_sntprintf(szMenuString,CountArray(szMenuString),TEXT("备注信息：%s"),(LPCTSTR)strUserNote);
			UserInfoMenu.AppendMenu(IDM_NULL_COMMAND,szMenuString);
		}
	}*/

	//积分信息
	DWORD dwPlayCount=pIClientUserItem->GetUserPlayCount();
	FLOAT fUserFleeRate=pIClientUserItem->GetUserFleeRate();
	_sntprintf(szMenuString,CountArray(szMenuString),TEXT("成绩：%I64d  经验值：%ld 总局数：%ld  逃跑率：%5.2f%%"),
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

	//旁观菜单
	/*if ((cbMeUserStatus!=US_LOOKON)&&(cbUserStatus==US_LOOKON))
	{
		//插入分割
		UserInfoMenu.AppendSeparator();

		//旁观菜单
		UserInfoMenu.AppendMenu(IDM_ALLOW_LOOKON,TEXT("允许他旁观我游戏"));
		UserInfoMenu.AppendMenu(IDM_FORBID_LOOKON,TEXT("禁止他旁观我游戏"));
	}*/

	//弹出菜单
	UserInfoMenu.TrackPopupMenu(this);

	return;
}

//显示菜单
VOID CGameFrameControl::TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount)
{
	//效验参数
	ASSERT((pIClientUserItem!=NULL)&&(wItemCount>1));
	if ((pIClientUserItem==NULL)||(wItemCount<=1)) return;

	return;
}

//用户进入
VOID CGameFrameControl::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//设置自己
	if (m_pIMySelfUserItem==NULL)
	{
		//变量定义
		ASSERT(m_pIClientKernel->GetUserAttribute()!=NULL);
		tagUserAttribute * pUserAttribute=m_pIClientKernel->GetUserAttribute();

		//自己判断
		if (pUserAttribute->dwUserID==pIClientUserItem->GetUserID())
		{
			//设置变量
			m_pIMySelfUserItem=pIClientUserItem;

			//设置界面
			SetUserViewInfo(m_pIMySelfUserItem);
			m_UserListControl.SetMySelfUserID(m_pIMySelfUserItem->GetUserID());
			m_GameFrameMedalView.SetUserMedal(m_pIMySelfUserItem->GetUserMedal());
		}
	}

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//插入用户
	m_UserListControl.InsertDataItem(pIClientUserItem);

	//提示信息
	if ((pGlobalUnits->m_bNotifyUserInOut==true)&&(m_pIClientKernel->IsServiceStatus()==true))
	{
		m_ChatMessage.InsertUserEnter(pIClientUserItem->GetNickName());
	}

	return;
}

//用户离开
VOID CGameFrameControl::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//变量定义
	DWORD dwLeaveUserID=pIClientUserItem->GetUserID();

	//聊天对象
	m_ChatControl.DeleteUserItem(pIClientUserItem);

	//删除用户
	m_UserListControl.DeleteDataItem(pIClientUserItem);

	//菜单对象
	for (INT_PTR i=0;i<m_MenuUserItemArray.GetCount();i++)
	{
		//获取用户
		IClientUserItem * pIChatUserItem=m_MenuUserItemArray[i];
		if (pIChatUserItem->GetUserID()==dwLeaveUserID) m_MenuUserItemArray.RemoveAt(i);
	}

	//提示信息
	if (pGlobalUnits->m_bNotifyUserInOut==true)
	{
		m_ChatMessage.InsertUserLeave(pIClientUserItem->GetNickName());
	}

	return;
}

//用户积分
VOID CGameFrameControl::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//更新积分
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	//更新奖牌
	if (pIClientUserItem==m_pIMySelfUserItem)
	{
		m_GameFrameMedalView.SetUserMedal(pIClientUserItem->GetUserMedal());
	}

	return;
}

//用户状态
VOID CGameFrameControl::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//更新状态
	m_UserListControl.UpdateDataItem(pIClientUserItem);

	return;
}

//用户头像
VOID CGameFrameControl::OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//用户选择
VOID CGameFrameControl::OnChangeChatTarget(IClientUserItem * pIClientUserItem)
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
VOID CGameFrameControl::OnExpressionSelect(CExpression * pUserExpression, tagExpressionInfo * pExpressionInfo)
{
	//设置焦点
	m_ChatEdit.SetFocus();

	//发送表情
	if (EfficacyUserChat(NULL,pUserExpression->GetIndex())==true)
	{
		ASSERT(m_pIClientKernel!=NULL);
		m_pIClientKernel->SendUserExpression(m_ChatControl.GetChatTargetUserID(),pUserExpression->GetIndex());
	}

	return;
}

//控件绑定
VOID CGameFrameControl::DoDataExchange(CDataExchange * pDX)
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

	//常规控件
	DDX_Control(pDX, IDC_USER_LIST_CONTROL, m_UserListControl);

	return;
}

//配置函数
BOOL CGameFrameControl::OnInitDialog()
{
	__super::OnInitDialog();

	//设置变量
	m_bCreateFlag=true;

	//设置资源
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//变量定义
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	//创建控件
	CRect rcCreate(0,0,0,0);
	m_GamePropertyCtrl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_PROPERTY);
	m_GameFrameUserView.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_USER_VIEW);
	m_GameFrameMedalView.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_MEDAL_VIEW);

	//变量定义
	tagPropertyResource PropertyResource;
	ZeroMemory(&PropertyResource,sizeof(PropertyResource));

	//设置道具
	PropertyResource.uImageL=IDB_GAME_PROPERTY_L;
	PropertyResource.uImageM=IDB_GAME_PROPERTY_M;
	PropertyResource.uImageR=IDB_GAME_PROPERTY_R;
	PropertyResource.uImageItem=IDB_GAME_PROPERTY_ITEM;
	PropertyResource.hResInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	//道具位置
	PropertyResource.PointExcursion.x=4;
	PropertyResource.PointExcursion.y=10;
	PropertyResource.SizeExcursion.cx=38;
	PropertyResource.SizeExcursion.cy=39;

	//设置道具
	m_GamePropertyCtrl.SetPropertyResource(PropertyResource);
	m_GamePropertyCtrl.SetGamePropertySink(QUERY_ME_INTERFACE(IUnknownEx));

	//聊天控件
	m_ChatEdit.SetLimitText(LEN_USER_CHAT-1);
	m_ChatEdit.SetEnableColor(RGB(0,0,0),RGB(255,255,255),RGB(255,255,255));

	//聊天控件
	m_ChatControl.SetChatControlSink(this);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(255,255,255));

	//聊天按钮
	m_btSendChat.SetButtonImage(IDB_BT_SEND_CHAT,hInstance,false,false);
	m_btChatShort.SetButtonImage(IDB_BT_CHAT_SHORT,hInstance,false,false);

	//聊天按钮
	m_btChatButton1.SetButtonImage(IDB_BT_CHAT_BUTTON_1,TEXT("BT_CHAT_BUTTON_1"),hInstance,false,false);
	m_btChatButton2.SetButtonImage(IDB_BT_CHAT_BUTTON_2,TEXT("BT_CHAT_BUTTON_2"),hInstance,false,false);
	m_btChatButton3.SetButtonImage(IDB_BT_CHAT_BUTTON_3,TEXT("BT_CHAT_BUTTON_3"),hInstance,false,false);
	m_btChatButton4.SetButtonImage(IDB_BT_CHAT_BUTTON_4,TEXT("BT_CHAT_BUTTON_4"),hInstance,false,false);

	//查询接口
	ASSERT(pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel)!=NULL);
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);

	//查询接口
	IGameUserManager * pIGameUserManager=(IGameUserManager *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_USER_MANAGER,IID_IGameUserManager,VER_IGameUserManager);
	IGameLevelParser * pIGameLevelParser=(IGameLevelParser *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_LEVEL_PARSER,IID_IGameLevelParser,VER_IGameLevelParser);

	//设置接口
	m_UserListControl.SetGameLevelParser(pIGameLevelParser);
	m_UserListControl.SetGameUserManager(pIGameUserManager);
		
	//注册事件
	m_pIClientKernel->SetUserEventSink(QUERY_ME_INTERFACE(IUserEventSink));
	m_pIClientKernel->SetStringMessage(QUERY_OBJECT_INTERFACE(m_ChatMessage,IStringMessage));

	//设置界面
	RectifyControl(CSkinRenderManager::GetInstance());

	return FALSE;
}

//消息过虑
BOOL CGameFrameControl::PreTranslateMessage(MSG * pMsg)
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
BOOL CGameFrameControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID=LOWORD(wParam);

	//按钮命令
	switch (nCommandID)
	{
	case IDC_BT_SEND_CHAT:			//发送聊天
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
				CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
				m_pIClientKernel->SendUserChatMessage(m_ChatControl.GetChatTargetUserID(),strMessage,pGlobalUnits->m_crChatTX);
			}

			//设置焦点
			m_ChatEdit.SetFocus();
			m_ChatEdit.SetWindowText(TEXT(""));

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
				GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),szFileName);

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
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

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
				CImageDC ImageDC(ImageColor[i]);
				CDC * pDC=CDC::FromHandle(ImageDC);

				//绘画位图
				pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
				pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
				pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
			}

			//创建菜单
			CSkinMenu ColorMenu;
			ColorMenu.CreateMenu();

			//构造菜单
			ColorMenu.AppendMenu(IDM_MORE_COLOR,TEXT("选择其他颜色"));

			//颜色选项
			ColorMenu.AppendSeparator();
			for (INT i=0;i<CountArray(ImageColor);i++) 
			{
				ColorMenu.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
				if (pGlobalUnits->m_crChatTX==crColor[i]) ColorMenu.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
			}

			//弹出菜单
			CRect rcButton;
			m_btChatButton2.GetWindowRect(&rcButton);
			ColorMenu.TrackPopupMenu(rcButton.left,rcButton.top,this);

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
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

			//设置菜单
			BYTE cbMessageMode=pGlobalUnits->m_cbMessageMode;
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
	}

	//菜单命令
	switch (nCommandID)
	{
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
	case IDM_MESSAGE_SHOW_ALL:		//显示所有
		{
			//变量定义
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->m_cbMessageMode=MESSAGE_MODE_ALL;

			return TRUE;
		}
	case IDM_MESSAGE_HIDE_DETEST:	//屏蔽厌恶
		{
			//变量定义
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->m_cbMessageMode=MESSAGE_MODE_DETEST;

			return TRUE;
		}
	case IDM_MESSAGE_ONLY_FRIEND:	//显示好友
		{
			//变量定义
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits::GetInstance()->m_cbMessageMode=MESSAGE_MODE_FRIEND;

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//调整控件
VOID CGameFrameControl::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//聊天区域
	CRect rcChatArea;
	rcChatArea.left=0;
	rcChatArea.right=nWidth;
	rcChatArea.bottom=nHeight;
	rcChatArea.top=USER_VIEW_CY+MEDAL_VIEW_CY+(nHeight-USER_VIEW_CY-MEDAL_VIEW_CY)*INCISE_SCALE;

	//列表区域
	CRect rcListArea;
	rcListArea.left=0;
	rcListArea.right=nWidth;
	rcListArea.top=USER_VIEW_CY;
	rcListArea.bottom=USER_VIEW_CY+(nHeight-USER_VIEW_CY-MEDAL_VIEW_CY)*INCISE_SCALE-PROPERTY_CY;

	//环绕信息
	tagEncircleInfo EncircleInfoChat;
	tagEncircleInfo EncircleInfoUserList;
	m_ChatEncircle.GetEncircleInfo(EncircleInfoChat);
	m_UserListEncircle.GetEncircleInfo(EncircleInfoUserList);

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//聊天按钮
	INT nChatButtonSpace=3;
	INT nXPosChat=rcChatArea.left+7;
	INT nYPosChat=rcChatArea.bottom-50;
	CSkinButton * ChatButtonArray[]={&m_btChatButton1,&m_btChatButton2,&m_btChatButton3,&m_btChatButton4};

	//聊天控件
	DeferWindowPos(hDwp,m_btSendChat,NULL,rcChatArea.right-35,rcChatArea.bottom-22,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatShort,NULL,rcChatArea.right-53,rcChatArea.bottom-22,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_ChatEdit,NULL,rcChatArea.left+12,rcChatArea.bottom-19,rcChatArea.Width()-67,12,uFlags);

	//聊天按钮
	for (INT i=0;i<CountArray(ChatButtonArray);i++)
	{
		//获取位置
		CRect rcChatButton;
		ChatButtonArray[i]->GetWindowRect(&rcChatButton);

		//显示判断
		bool bShowButton=true;
		if ((nXPosChat+rcChatButton.Width()+nChatButtonSpace)>(rcChatArea.right-7)) bShowButton=false;

		//移动按钮
		UINT uTargetFlag=(bShowButton==true)?SWP_SHOWWINDOW:SWP_HIDEWINDOW;
		DeferWindowPos(hDwp,ChatButtonArray[i]->m_hWnd,NULL,nXPosChat,nYPosChat,0,0,uFlags|SWP_NOSIZE|uTargetFlag);

		//设置间距
		nXPosChat+=(rcChatButton.Width()+nChatButtonSpace);
	}

	//环绕控件
	m_UserListEncircle.DeferWindowPos(&m_UserListControl,hDwp,rcListArea);

	//其他控件
	DeferWindowPos(hDwp,m_GameFrameUserView,NULL,0,0,nWidth,USER_VIEW_CY,uFlags);
	DeferWindowPos(hDwp,m_GameFrameMedalView,NULL,0,rcListArea.bottom,nWidth,MEDAL_VIEW_CY,uFlags);
	DeferWindowPos(hDwp,m_GamePropertyCtrl,NULL,0,rcListArea.bottom+MEDAL_VIEW_CY,nWidth,PROPERTY_CY,uFlags);

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

	return;
}

//调整界面
VOID CGameFrameControl::RectifyControl(CSkinRenderManager * pSkinRenderManager)
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

//聊天效验
bool CGameFrameControl::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	//状态判断
	if (m_pIClientKernel==NULL) return false;
	if (m_pIMySelfUserItem==NULL) return false;

	//变量定义
	BYTE cbMemberOrder=m_pIMySelfUserItem->GetMemberOrder();
	BYTE cbMasterOrder=m_pIMySelfUserItem->GetMasterOrder();

	//属性定义
	tagUserAttribute * pUserAttribute=m_pIClientKernel->GetUserAttribute();
	tagServerAttribute * pServerAttribute=m_pIClientKernel->GetServerAttribute();

	//房间配置
	if (CServerRule::IsForfendGameChat(pServerAttribute->dwServerRule)&&(cbMasterOrder==0))
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n“%s”发送失败"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("抱歉，当前此游戏房间禁止用户房间聊天！"));

		return false;
	}

	//权限判断
	if (CUserRight::CanGameChat(pUserAttribute->dwUserRight)==false)
	{
		//原始消息
		if (wExpressionIndex==INVALID_WORD)
		{
			CString strDescribe;
			strDescribe.Format(TEXT("\n“%s”发送失败"),pszChatString);
			m_ChatMessage.InsertString(strDescribe,COLOR_EVENT);
		}

		//插入消息
		m_ChatMessage.InsertSystemString(TEXT("抱歉，您没有游戏发言的权限，若需要帮助，请联系游戏客服咨询！"));

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

//更多颜色
VOID CGameFrameControl::OnSelectMoreColor()
{
	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//设置颜色
	CColorDialog ColorDialog(pGlobalUnits->m_crChatTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pGlobalUnits->m_crChatTX=ColorDialog.GetColor();

	//设置界面
	m_ChatEdit.SetFocus();

	return;
}

//选择颜色
VOID CGameFrameControl::OnSelectChatColor(UINT nCommandID)
{
	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//变量定义
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//选择颜色
	ASSERT(nIndex<CountArray(crColor));
	if (nIndex<CountArray(crColor)) pGlobalUnits->m_crChatTX=crColor[nIndex];

	//设置焦点
	m_ChatEdit.SetFocus();

	return;
}

//选择短语
VOID CGameFrameControl::OnSelectChatShort(UINT nCommandID)
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
	GetPrivateProfileString(TEXT("GameChatShort"),szItemName,TEXT(""),szShortcut,CountArray(szShortcut),szFileName);

	//发送消息
	if (szShortcut[0]!=0)
	{
		//获取信息
		CString strMessage=szShortcut;
		strMessage.TrimLeft();strMessage.TrimRight();

		//发送信息
		if ((strMessage.IsEmpty()==FALSE)&&(EfficacyUserChat(strMessage,INVALID_WORD)==true))
		{
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
			m_pIClientKernel->SendUserChatMessage(m_ChatControl.GetChatTargetUserID(),strMessage,pGlobalUnits->m_crChatTX);
		}
	}

	//设置焦点
	m_ChatEdit.SetFocus();

	return;
}

//绘画背景
BOOL CGameFrameControl::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画聊天
	CRect rcChatArea;
	rcChatArea.left=0;
	rcChatArea.right=rcClient.Width();
	rcChatArea.bottom=rcClient.Height();
	rcChatArea.top=USER_VIEW_CY+MEDAL_VIEW_CY+(rcClient.Height()-USER_VIEW_CY-MEDAL_VIEW_CY)*INCISE_SCALE;

	//列表区域
	CRect rcUserArea;
	rcUserArea.left=0;
	rcUserArea.top=USER_VIEW_CY;
	rcUserArea.right=rcClient.Width();
	rcUserArea.bottom=USER_VIEW_CY+(rcClient.Height()-USER_VIEW_CY-MEDAL_VIEW_CY)*INCISE_SCALE-PROPERTY_CY;

	//绘画环绕
	m_ChatEncircle.DrawEncircleFrame(pDC,rcChatArea);
	m_UserListEncircle.DrawEncircleFrame(pDC,rcUserArea);

	return TRUE;
}

//位置消息
VOID CGameFrameControl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//右键列表
VOID CGameFrameControl::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
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
			IClientUserItem * pIClientUserItem[32];
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

//选择改变
VOID CGameFrameControl::OnLvnItemchangedUserList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//变量定义
	LPNMLISTVIEW pNmListView=reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	//设置用户
	if ((pNmListView->lParam!=NULL)&&(pNmListView->uNewState&(ODS_SELECTED|ODS_FOCUS)))
	{
		//获取用户
		IClientUserItem * pIClientUserItem=(IClientUserItem *)pNmListView->lParam;

		//设置用户
		SetUserViewInfo(pIClientUserItem);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
