#include "StdAfx.h"
#include "GlobalUnits.h"
#include "ClientKernel.h"

//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_USER_CLOCK_PULSE		100									//用户时钟

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CClientKernel, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CClientKernel::CClientKernel()
{
	//状态变量
	m_bService=false;
	ZeroMemory(&m_UserAttribute,sizeof(m_UserAttribute));

	//时间信息
	m_wClockID=0L;
	m_nElapseCount=0L;
	m_wClockChairID=INVALID_CHAIR;

	//游戏状态
	m_bAllowLookon=false;
	m_pIMySelfUserItem=NULL;
	m_cbGameStatus=GAME_STATUS_FREE;

	//内核变量
	m_hProcessWnd=NULL;
	m_pIStringMessage=NULL;
	m_pIClientKernelSink=NULL;

	//房间属性
	ZeroMemory(&m_GameAttribute,sizeof(m_GameAttribute));
	ZeroMemory(&m_ServerAttribute,sizeof(m_ServerAttribute));

	return;
}

//析构函数
CClientKernel::~CClientKernel()
{
}

//接口查询
VOID * CClientKernel::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientKernel,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessControlSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IClientKernel,Guid,dwQueryVer);
	return NULL;
}

//内核配置
bool CClientKernel::InitClientKernel(LPCTSTR lpszComLine)
{
	//效验参数
	ASSERT(lpszComLine!=NULL);
	if (lpszComLine==NULL) return false;

	//变量定义
	CWHCommandLine CommandLine;
	TCHAR szTransmittal[32]=TEXT("");

	//创建窗口
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,GetDesktopWindow(),100);
	}

	//提取参数
	if (CommandLine.SearchCommandItem(lpszComLine,TEXT("/Transmittal:"),szTransmittal,CountArray(szTransmittal))==true)
	{
		//设置参数
		m_hProcessWnd=(HWND)(_tstoi64(szTransmittal));

		//效验状态
		ASSERT(IsWindow(m_hProcessWnd));
		if (IsWindow(m_hProcessWnd)==FALSE) m_hProcessWnd=NULL;
	}

	//创建组件
	if (m_ProcessControlModule.CreateInstance()==false) return false;
	if (m_GameUserManagerModule.CreateInstance()==false) return false;

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_USER_MANAGER,m_GameUserManagerModule.GetInterface());

	//设置组件
	if (m_GameUserManagerModule->SetUserManagerSink(QUERY_ME_INTERFACE(IUnknownEx))==false) return false;
	if (m_ProcessControlModule->SetProcessControlSink(QUERY_ME_INTERFACE(IUnknownEx))==false) return false;

	//联机模式
	if (m_hProcessWnd!=NULL)
	{
		//时间引擎
		m_TimerPulseEngine.StartService();
		m_TimerPulseEngine.SetKernelWnd(m_hWnd);

		//创建连接
		if (m_ProcessControlModule->InitializeLink()==false) return false;
	}

	return true;
}

//设置接口
bool CClientKernel::SetUserEventSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserEventSink)!=NULL);
	IUserEventSink * pIUserEventSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserEventSink);

	//成功判断
	if (pIUserEventSink!=NULL)
	{
		m_UserEventSinkArray.Add(pIUserEventSink);
		return true;
	}

	return false;
}

//设置接口
bool CClientKernel::SetStringMessage(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IStringMessage)!=NULL);
		m_pIStringMessage=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IStringMessage);

		//成功判断
		if (m_pIStringMessage==NULL) return false;
	}
	else m_pIStringMessage=NULL;

	return true;
}

//设置接口
bool CClientKernel::SetClientKernelSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientKernelSink)!=NULL);
		m_pIClientKernelSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientKernelSink);

		//成功判断
		if (m_pIClientKernelSink==NULL) return false;
	}
	else m_pIClientKernelSink=NULL;

	return true;
}

//游戏属性
bool CClientKernel::SetGameAttribute(WORD wKindID, WORD wPlayerCount, DWORD dwClientVersion, HICON hGameIcon, LPCTSTR pszGameName)
{
	//设置图标
	AfxGetMainWnd()->SetIcon(hGameIcon,TRUE);
	AfxGetMainWnd()->SetIcon(hGameIcon,FALSE);

	//设置变量
	m_GameAttribute.wKindID=wKindID;
	m_GameAttribute.wChairCount=wPlayerCount;
	m_GameAttribute.dwClientVersion=dwClientVersion;
	lstrcpyn(m_GameAttribute.szGameName,pszGameName,CountArray(m_GameAttribute.szGameName));

	//获取进程
	TCHAR szModuleName[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szModuleName,CountArray(szModuleName));

	//获取版本
	DWORD dwProcessVersion=0L;
	CWHService::GetModuleVersion(szModuleName,dwProcessVersion);

	//版本效验
	ASSERT(m_GameAttribute.dwClientVersion==dwProcessVersion);
	if (m_GameAttribute.dwClientVersion!=dwProcessVersion) throw TEXT("游戏程序版本信息被非法修改");

	//设置标题
	SetGameFrameTitle();

	return true;
}

//连接大厅
bool CClientKernel::CreateConnect()
{
	//效验状态
	if (IsSingleMode()==true) return false;
	
	//发送消息
	SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLIENT_READY);

	return true;
}

//中断连接
bool CClientKernel::IntermitConnect()
{
	//效验状态
	if (IsSingleMode()==true) return false;

	//发送消息
	SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLIENT_CLOSE);

	//关闭连接
	if (m_ProcessControlModule.GetInterface()!=NULL) m_ProcessControlModule->UnInitializeLink();

	return true;
}

//单机模式
bool CClientKernel::IsSingleMode()
{
	//连接判断
	if (m_hProcessWnd==NULL) return true;
	if (m_ProcessControlModule.GetInterface()==NULL) return true;

	return false;
}

//旁观用户
bool CClientKernel::IsLookonMode()
{
	if (m_pIMySelfUserItem==NULL) return true;
	return (m_pIMySelfUserItem->GetUserStatus()==US_LOOKON);
}

//允许旁观
bool CClientKernel::IsAllowLookon()
{
	return m_bAllowLookon;
}

//服务状态
bool CClientKernel::IsServiceStatus()
{
	return m_bService;
}

//自己位置
WORD CClientKernel::GetMeChairID()
{
	//效验状态
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return INVALID_CHAIR;

	return m_pIMySelfUserItem->GetChairID();
}

//自己位置
IClientUserItem * CClientKernel::GetMeUserItem()
{
	return m_pIMySelfUserItem;
}

//游戏用户
IClientUserItem * CClientKernel::GetTableUserItem(WORD wChariID)
{
	//效验状态
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//获取用户
	return m_GameUserManagerModule->GetTableUserItem(wChariID);
}

//查找用户
IClientUserItem * CClientKernel::SearchUserByUserID(DWORD dwUserID)
{
	//效验状态
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//查找用户
	IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(dwUserID);

	return pIClientUserItem;
}

//查找用户
IClientUserItem * CClientKernel::SearchUserByGameID(DWORD dwGameID)
{
	//效验状态
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//查找用户
	IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByGameID(dwGameID);

	return pIClientUserItem;
}

//旁观用户
IClientUserItem * CClientKernel::EnumLookonUserItem(WORD wEnumIndex)
{
	//效验状态
	ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
	if (m_GameUserManagerModule.GetInterface()==NULL) return NULL;

	//旁观用户
	IClientUserItem * pIClientUserItem=m_GameUserManagerModule->EnumLookonUserItem(wEnumIndex);

	return pIClientUserItem;
}

//发送函数
bool CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验状态
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//变量定义
	IPC_GF_SocketSend SocketSend;
	ZeroMemory(&SocketSend,sizeof(SocketSend));

	//构造数据
	SocketSend.CommandInfo.wSubCmdID=wSubCmdID;
	SocketSend.CommandInfo.wMainCmdID=wMainCmdID;

	//发送数据
	WORD wSendSize=sizeof(SocketSend)-sizeof(SocketSend.cbBuffer);
	m_ProcessControlModule->SendData(m_hProcessWnd,IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_SEND,&SocketSend,wSendSize);

	return true;
}

//发送函数
bool CClientKernel::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);
	if (wDataSize>SOCKET_TCP_PACKET) return false;

	//效验状态
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//变量定义
	IPC_GF_SocketSend SocketSend;
	ZeroMemory(&SocketSend,sizeof(SocketSend));

	//构造数据
	SocketSend.CommandInfo.wSubCmdID=wSubCmdID;
	SocketSend.CommandInfo.wMainCmdID=wMainCmdID;

	//叠加数据
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketSend.cbBuffer,pData,wDataSize);
	}

	//发送数据
	WORD wSendSize=sizeof(SocketSend)-sizeof(SocketSend.cbBuffer)+wDataSize;
	m_ProcessControlModule->SendData(m_hProcessWnd,IPC_CMD_GF_SOCKET,IPC_SUB_GF_SOCKET_SEND,&SocketSend,wSendSize);

	return true;
}

//发送数据
bool CClientKernel::SendProcessData(WORD wMainCmdID, WORD wSubCmdID)
{
	//效验状态
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//发送数据
	m_ProcessControlModule->SendData(m_hProcessWnd,wMainCmdID,wSubCmdID);

	return true;
}

//发送数据
bool CClientKernel::SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//发送数据
	m_ProcessControlModule->SendData(m_hProcessWnd,wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//执行命令
bool CClientKernel::SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验状态
	if (m_hProcessWnd==NULL) return false;
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//执行命令
	m_ProcessControlModule->SendCommand(m_hProcessWnd,wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//发送准备
bool CClientKernel::SendUserReady(VOID * pData, WORD wDataSize)
{
	//状态效验
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//发送准备
	SendSocketData(MDM_GF_FRAME,SUB_GF_USER_READY,pData,wDataSize);

	return true;
}

//旁观控制
bool CClientKernel::SendUserLookon(DWORD dwUserID, bool bAllowLookon)
{
	//发送消息
	if (IsLookonMode()==false)
	{
		//变量定义
		CMD_GF_LookonConfig LookonConfig;
		ZeroMemory(&LookonConfig,sizeof(LookonConfig));

		//设置变量
		LookonConfig.dwUserID=dwUserID;
		LookonConfig.cbAllowLookon=(bAllowLookon==true)?TRUE:FALSE;

		//发送数据
		SendSocketData(MDM_GF_FRAME,SUB_GF_LOOKON_CONFIG,&LookonConfig,sizeof(LookonConfig));
	}

	return true;
}

//发送表情
bool CClientKernel::SendUserExpression(DWORD dwTargetUserID, WORD wItemIndex)
{
	//变量定义
	CMD_GF_C_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//构造信息
	UserExpression.wItemIndex=wItemIndex;
	UserExpression.dwTargetUserID=dwTargetUserID;

	//发送命令
	SendSocketData(MDM_GF_FRAME,SUB_GF_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//发送聊天
bool CClientKernel::SendUserChatMessage(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor)
{
	//效验用户
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return false;

	//构造信息
	CMD_GF_C_UserChat UserChat;
	lstrcpyn(UserChat.szChatString,pszChatString,CountArray(UserChat.szChatString));

	//构造数据
	UserChat.dwChatColor=crColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//发送命令
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	SendSocketData(MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(TCHAR));

	return true;
}

//激活框架
VOID CClientKernel::ActiveGameFrame()
{
	//获取窗口
	CWnd * pFocusWnd=GetFocus();
	CWnd * pMainWnd=AfxGetMainWnd();

	//激活判断
	if ((pFocusWnd==NULL)||(pMainWnd->IsChild(pFocusWnd)==FALSE)) pMainWnd->FlashWindow(TRUE);

	return;
}

//切换椅子
WORD CClientKernel::SwitchViewChairID(WORD wChairID)
{
	//参数判断
	if (wChairID==INVALID_CHAIR) return INVALID_CHAIR;
	if (m_UserAttribute.dwUserID==0L) return INVALID_CHAIR;

	//转换椅子
	WORD wChairCount=m_GameAttribute.wChairCount;
	WORD wViewChairID=(wChairID+wChairCount*3/2-m_UserAttribute.wChairID)%wChairCount;

	return wViewChairID;
}

//时钟标识
UINT CClientKernel::GetClockID()
{
	return m_wClockID;
}

//时钟位置
WORD CClientKernel::GetClockChairID()
{
	return m_wClockChairID;
}

//删除时钟
VOID CClientKernel::KillGameClock(WORD wClockID)
{
	//效验状态
	ASSERT(m_hWnd!=NULL);
	ASSERT(m_pIClientKernelSink!=NULL);

	//逻辑处理
	if ((m_wClockID!=0)&&((m_wClockID==wClockID)||(wClockID==0)))
	{
		//设置界面
		m_TimerPulseEngine.StopTimerPulse(m_wClockID);

		//事件通知
		if (m_wClockChairID!=INVALID_CHAIR)
		{
			ASSERT(m_wClockChairID<m_GameAttribute.wChairCount);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->OnEventGameClockKill(m_wClockChairID);
		}

		//设置变量
		m_wClockID=0L;
		m_nElapseCount=0L;
		m_wClockChairID=INVALID_CHAIR;
	}

	return;
}

//设置时钟
VOID CClientKernel::SetGameClock(WORD wChairID, WORD wClockID, UINT nElapse)
{
	//效验参数
	ASSERT(m_hWnd!=NULL);
	ASSERT(m_pIClientKernelSink!=NULL);

	//删除时间
	if (m_wClockID!=0L) KillGameClock(m_wClockID);

	//设置时间
	if ((wChairID<m_GameAttribute.wChairCount)&&(nElapse>0L))
	{
		//设置变量
		m_wClockID=wClockID;
		m_nElapseCount=nElapse;
		m_wClockChairID=wChairID;

		//设置时间
		m_TimerPulseEngine.StartTimerPulse(wClockID);

		//事件通知
		if (m_pIClientKernelSink!=NULL)
		{
			m_pIClientKernelSink->OnEventGameClockInfo(m_wClockChairID,nElapse,wClockID);
		}
	}

	return;
}

//进程数据
bool CClientKernel::OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)
{
	switch (wMainCmdID)
	{
	case IPC_CMD_GF_SOCKET:		//网络消息
		{
			//消息处理
			bool bSuccess=OnProcessMainSocket(wSubCmdID,pData,wDataSize);

			//错误处理
			if (bSuccess==false)
			{
				ASSERT(m_pIClientKernelSink!=NULL);
				if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->CloseGameClient();
			}

			return true;
		}
	case IPC_CMD_GF_CONTROL:	//控制信息
		{
			return OnProcessMainControl(wSubCmdID,pData,wDataSize);
		}
	case IPC_CMD_GF_CONFIG:		//配置信息
		{
			return OnProcessMainConfig(wSubCmdID,pData,wDataSize);
		}
	case IPC_CMD_GF_USER_INFO:	//用户信息
		{
			return OnProcessMainUserInfo(wSubCmdID,pData,wDataSize);
		}
	}

	return false;
}

//用户激活
VOID CClientKernel::OnUserItemAcitve(IClientUserItem * pIClientUserItem)
{
	//效验状态
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//设置用户
	if (m_pIMySelfUserItem==NULL)
	{
		DWORD dwUserID=pIClientUserItem->GetUserID();
		if (dwUserID==m_UserAttribute.dwUserID) m_pIMySelfUserItem=pIClientUserItem;
	}

	//变量定义
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//用户通知
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		pIUserEventSink->OnEventUserEnter(pIClientUserItem,bLookonUser);
	}

	return;
}

//用户删除
VOID CClientKernel::OnUserItemDelete(IClientUserItem * pIClientUserItem)
{
	//效验状态
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//变量定义
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//用户通知
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		pIUserEventSink->OnEventUserLeave(pIClientUserItem,bLookonUser);
	}

	return;
}

//用户更新
VOID CClientKernel::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//效验状态
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//变量定义
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//用户通知
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		pIUserEventSink->OnEventCustomFace(pIClientUserItem,bLookonUser);
	}

	return;
}

//用户更新
VOID CClientKernel::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)
{
	//效验状态
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//变量定义
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//用户通知
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		pIUserEventSink->OnEventUserScore(pIClientUserItem,bLookonUser);
	}

	return;
}

//用户更新
VOID CClientKernel::OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)
{
	//效验状态
	ASSERT((m_UserEventSinkArray.GetCount()>0L)&&(pIClientUserItem!=NULL));
	if ((m_UserEventSinkArray.GetCount()==0L)||(pIClientUserItem==NULL)) return;

	//变量定义
	IUserEventSink * pIUserEventSink=NULL;
	bool bLookonUser=(pIClientUserItem->GetUserStatus()==US_LOOKON);

	//用户通知
	for (INT_PTR i=0;i<m_UserEventSinkArray.GetCount();i++)
	{
		pIUserEventSink=m_UserEventSinkArray[i];
		pIUserEventSink->OnEventUserStatus(pIClientUserItem,bLookonUser);
	}

	return;
}

//网络消息
bool CClientKernel::OnProcessMainSocket(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//网络接收
	if (wSubCmdID==IPC_SUB_GF_SOCKET_RECV)
	{
		//效验数据
		ASSERT(wDataSize>=sizeof(TCP_Command));
		if (wDataSize<sizeof(TCP_Command)) return false;

		//提取数据
		WORD wPacketSize=wDataSize-sizeof(TCP_Command);
		IPC_GF_SocketRecv * pSocketRecv=(IPC_GF_SocketRecv *)pData;

		//游戏处理
		WORD wSubCmdID=pSocketRecv->CommandInfo.wSubCmdID;
		WORD wMainCmdID=pSocketRecv->CommandInfo.wMainCmdID;
		bool bSuccess=m_pIClientKernelSink->OnEventSocket(wMainCmdID,wSubCmdID,pSocketRecv->cbBuffer,wPacketSize);

		//内核处理
		if ((bSuccess==false)&&(wMainCmdID==MDM_GF_FRAME))
		{
			switch (wSubCmdID)
			{
			case SUB_GF_USER_CHAT:			//用户聊天
				{
					return OnSocketSubUserChat(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_USER_EXPRESSION:	//用户表情
				{
					return OnSocketSubExpression(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_GAME_STATUS:		//游戏状态
				{
					return OnSocketSubGameStatus(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_GAME_SCENE:			//游戏场景
				{
					return OnSocketSubGameScene(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_LOOKON_STATUS:		//旁观状态
				{
					return OnSocketSubLookonStatus(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_SYSTEM_MESSAGE:		//系统消息
				{
					return OnSocketSubSystemMessage(pSocketRecv->cbBuffer,wPacketSize);
				}
			case SUB_GF_ACTION_MESSAGE:		//动作消息
				{
					return OnSocketSubActionMessage(pSocketRecv->cbBuffer,wPacketSize);
				}
			}

			return true;
		}

		return bSuccess;
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//控制信息
bool CClientKernel::OnProcessMainControl(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case IPC_SUB_GF_CLOSE_PROCESS:	//关闭进程
		{
			//终止连接
			m_hProcessWnd=NULL;

			//关闭进程
			ASSERT(m_pIClientKernelSink!=NULL);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->CloseGameClient();

			return true;
		}
	case IPC_SUB_GF_ACTIVE_PROCESS:	//激活进程
		{
			//获取窗口
			CWnd * pMainWnd=AfxGetMainWnd();

			//还原窗口
			if (pMainWnd->IsIconic()) 
			{
				pMainWnd->ShowWindow(SW_RESTORE);
			}

			//置顶窗口
			pMainWnd->SetActiveWindow(); 
			pMainWnd->BringWindowToTop(); 
			pMainWnd->SetForegroundWindow();

			return true;
		}
	case IPC_SUB_GF_BOSS_COME:		//老板来了
		{
			//获取窗口
			CWnd * pMainWnd=AfxGetMainWnd();

			//隐藏窗口
			pMainWnd->ShowWindow(SW_MINIMIZE);
			pMainWnd->ShowWindow(SW_HIDE);

			return true;
		}
	case IPC_SUB_GF_BOSS_LEFT:		//老板走了
		{
			//获取窗口
			CWnd * pMainWnd=AfxGetMainWnd();

			//隐藏窗口
			pMainWnd->ShowWindow(SW_RESTORE);
			pMainWnd->ShowWindow(SW_SHOW);

			//置顶窗口
			pMainWnd->SetActiveWindow(); 
			pMainWnd->BringWindowToTop();
			pMainWnd->SetForegroundWindow();

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//配置消息
bool CClientKernel::OnProcessMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case IPC_SUB_GF_LEVEL_INFO:		//等级信息
		{
			//变量定义
			IPC_GF_LevelInfo * pLevelInfo=(IPC_GF_LevelInfo *)pData;
			WORD wHeadSize=sizeof(IPC_GF_LevelInfo)-sizeof(pLevelInfo->LevelItem);

			//效验参数
			ASSERT((wHeadSize+pLevelInfo->cbItemCount*sizeof(pLevelInfo->LevelItem[0]))==wDataSize);
			if ((wHeadSize+pLevelInfo->cbItemCount*sizeof(pLevelInfo->LevelItem[0]))!=wDataSize) return false;

			//获取对象
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
			IGameLevelParser * pIGameLevelParser=pGlobalUnits->m_GameLevelParserModule.GetInterface();

			//设置解释
			pIGameLevelParser->SetKindName(m_GameAttribute.szGameName);
			pIGameLevelParser->SetGameLevelItem(pLevelInfo->LevelItem,pLevelInfo->cbItemCount);

			return true;
		}
	case IPC_SUB_GF_COLUMN_INFO:	//列表信息
		{
			//变量定义
			IPC_GF_ColumnInfo * pColumnInfo=(IPC_GF_ColumnInfo *)pData;
			WORD wHeadSize=sizeof(IPC_GF_ColumnInfo)-sizeof(pColumnInfo->ColumnItem);

			//效验参数
			ASSERT((wHeadSize+pColumnInfo->cbColumnCount*sizeof(pColumnInfo->ColumnItem[0]))==wDataSize);
			if ((wHeadSize+pColumnInfo->cbColumnCount*sizeof(pColumnInfo->ColumnItem[0]))!=wDataSize) return false;

			//列表信息
			BYTE cbColumnCount=0;
			tagColumnItem ColumnItem[MAX_COLUMN];

			//构造列表
			for (BYTE i=0;i<pColumnInfo->cbColumnCount;i++)
			{
				//完成判断
				if (cbColumnCount>=CountArray(ColumnItem)) 
				{
					ASSERT(FALSE);
					break;
				}

				//排除过虑
				if (pColumnInfo->ColumnItem[i].cbDataDescribe==UD_TABLE) continue;
				if (pColumnInfo->ColumnItem[i].cbDataDescribe==UD_CHAIR) continue;
				if (pColumnInfo->ColumnItem[i].cbDataDescribe==UD_NOTE_INFO) continue;

				//构造列表
				ColumnItem[cbColumnCount++]=pColumnInfo->ColumnItem[i];
			}

			//增加旁观
			if (cbColumnCount<CountArray(ColumnItem))
			{
				BYTE cbIndex=cbColumnCount++;
				ColumnItem[cbIndex].cbColumnWidth=100;
				ColumnItem[cbIndex].cbDataDescribe=UD_LOOKON_USER;
				lstrcpyn(ColumnItem[cbIndex].szColumnName,TEXT("旁观用户"),CountArray(ColumnItem[cbIndex].szColumnName));
			}

			//配置列表
			ASSERT(m_pIClientKernelSink!=NULL);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->SchemeColumnItem(ColumnItem,cbColumnCount);

			return true;
		}
	case IPC_SUB_GF_SERVER_INFO:	//房间配置
		{
			//效验参数
			ASSERT(wDataSize==sizeof(IPC_GF_ServerInfo));
			if (wDataSize!=sizeof(IPC_GF_ServerInfo)) return false;

			//变量定义
			IPC_GF_ServerInfo * pServerInfo=(IPC_GF_ServerInfo *)pData;

			//用户属性
			m_UserAttribute.dwUserID=pServerInfo->dwUserID;
			m_UserAttribute.wTableID=pServerInfo->wTableID;
			m_UserAttribute.wChairID=pServerInfo->wChairID;

			//权限属性
			m_UserAttribute.dwUserRight=pServerInfo->dwUserRight;
			m_UserAttribute.dwMasterRight=pServerInfo->dwMasterRight;

			//房间属性
			m_ServerAttribute.wKindID=pServerInfo->wKindID;
			m_ServerAttribute.wServerID=pServerInfo->wServerID;
			m_ServerAttribute.wServerType=pServerInfo->wServerType;
			m_ServerAttribute.dwServerRule=pServerInfo->dwServerRule;
			lstrcpyn(m_ServerAttribute.szServerName,pServerInfo->szServerName,CountArray(m_ServerAttribute.szServerName));

			return true;
		}
	case IPC_SUB_GF_PROPERTY_INFO:	//道具信息
		{
			//变量定义
			IPC_GF_PropertyInfo * pPropertyInfo=(IPC_GF_PropertyInfo *)pData;
			WORD wHeadSize=sizeof(IPC_GF_PropertyInfo)-sizeof(pPropertyInfo->PropertyInfo);

			//效验参数
			ASSERT((wHeadSize+pPropertyInfo->cbPropertyCount*sizeof(pPropertyInfo->PropertyInfo[0]))==wDataSize);
			if ((wHeadSize+pPropertyInfo->cbPropertyCount*sizeof(pPropertyInfo->PropertyInfo[0]))!=wDataSize) return false;

			//列表信息
			BYTE cbPropertyCount=0;
			tagPropertyInfo PropertyInfo[MAX_PROPERTY];

			//构造列表
			for (BYTE i=0;i<pPropertyInfo->cbPropertyCount;i++)
			{
				//完成判断
				if (cbPropertyCount>=CountArray(PropertyInfo)) 
				{
					ASSERT(FALSE);
					break;
				}

				//构造列表
				PropertyInfo[cbPropertyCount++]=pPropertyInfo->PropertyInfo[i];
			}

			//配置列表
			ASSERT(m_pIClientKernelSink!=NULL);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->SchemePropertyInfo(PropertyInfo,cbPropertyCount);

			return true;
		}
	case IPC_SUB_GF_CONFIG_FINISH:	//配置完成
		{
			//事件通知
			ASSERT(m_pIClientKernelSink!=NULL);
			if (m_pIClientKernelSink!=NULL) m_pIClientKernelSink->OnSchemeFinish();

			//获取对象
			ASSERT(CGlobalUnits::GetInstance()!=NULL);
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

			//设置变量
			m_bService=true;

			//设置标题
			SetGameFrameTitle();

			//变量定义
			CMD_GF_GameOption GameOption;
			ZeroMemory(&GameOption,sizeof(GameOption));

			//构造数据
			GameOption.dwFrameVersion=VERSION_FRAME;
			GameOption.cbAllowLookon=pGlobalUnits->m_bAllowLookon;
			GameOption.dwClientVersion=m_GameAttribute.dwClientVersion;

			//发送数据
			SendSocketData(MDM_GF_FRAME,SUB_GF_GAME_OPTION,&GameOption,sizeof(GameOption));

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//用户消息
bool CClientKernel::OnProcessMainUserInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case IPC_SUB_GF_USER_ENTER:		//用户进入
		{
			//效验参数
			ASSERT(wDataSize>=sizeof(IPC_GF_UserInfo));
			if (wDataSize<sizeof(IPC_GF_UserInfo)) return false;

			//消息处理
			IPC_GF_UserInfo * pIPCUserInfo=(IPC_GF_UserInfo *)pData;
			tagUserInfoHead * pUserInfoHead=&pIPCUserInfo->UserInfoHead;

			//变量定义
			TCHAR szUserNote[LEN_USERNOTE]=TEXT("");

			//变量定义
			tagUserInfo UserInfo;
			tagCustomFaceInfo CustomFaceInfo;
			ZeroMemory(&UserInfo,sizeof(UserInfo));
			ZeroMemory(&CustomFaceInfo,sizeof(CustomFaceInfo));

			//用户属性
			UserInfo.wFaceID=pUserInfoHead->wFaceID;
			UserInfo.dwGameID=pUserInfoHead->dwGameID;
			UserInfo.dwUserID=pUserInfoHead->dwUserID;
			UserInfo.dwGroupID=pUserInfoHead->dwGroupID;
			UserInfo.dwCustomID=pUserInfoHead->dwCustomID;

			//用户属性
			UserInfo.cbGender=pUserInfoHead->cbGender;
			UserInfo.cbMemberOrder=pUserInfoHead->cbMemberOrder;
			UserInfo.cbMasterOrder=pUserInfoHead->cbMasterOrder;

			//用户状态
			UserInfo.wTableID=pUserInfoHead->wTableID;
			UserInfo.wChairID=pUserInfoHead->wChairID;
			UserInfo.cbUserStatus=pUserInfoHead->cbUserStatus;

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
			CRecvPacketHelper RecvPacket(pIPCUserInfo+1,wDataSize-sizeof(IPC_GF_UserInfo));

			//扩展信息
			while (true)
			{
				//提取数据
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;

				//设置数据
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
				case DTP_GR_USER_NOTE:			//用户备注
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(szUserNote));
						if (DataDescribe.wDataSize<=sizeof(szUserNote))
						{
							CopyMemory(szUserNote,pDataBuffer,DataDescribe.wDataSize);
							szUserNote[CountArray(szUserNote)-1]=0;
						}
						break;
					}
				case DTP_GR_CUSTOM_FACE:		//自定头像
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize==sizeof(tagCustomFaceInfo));
						if (DataDescribe.wDataSize==sizeof(tagCustomFaceInfo))
						{
							CopyMemory(&CustomFaceInfo,pDataBuffer,DataDescribe.wDataSize);
						}
						break;
					}
				}
			}

			//激活用户
			m_GameUserManagerModule->ActiveUserItem(UserInfo,CustomFaceInfo,pIPCUserInfo->cbCompanion,szUserNote);

			return true;
		}
	case IPC_SUB_GF_USER_SCORE:		//用户积分
		{
			//效验参数
			ASSERT(wDataSize==sizeof(IPC_GF_UserScore));
			if (wDataSize!=sizeof(IPC_GF_UserScore)) return false;

			//寻找用户
			IPC_GF_UserScore * pUserScore=(IPC_GF_UserScore *)pData;
			IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserScore->dwUserID);

			//用户判断
			ASSERT(pIClientUserItem!=NULL);
			if (pIClientUserItem==NULL) return true;

			//设置积分
			m_GameUserManagerModule->UpdateUserItemScore(pIClientUserItem,&pUserScore->UserScore);

			return true;
		}
	case IPC_SUB_GF_USER_STATUS:	//用户状态
		{
			//效验参数
			ASSERT(wDataSize==sizeof(IPC_GF_UserStatus));
			if (wDataSize!=sizeof(IPC_GF_UserStatus)) return false;

			//寻找用户
			IPC_GF_UserStatus * pUserStatus=(IPC_GF_UserStatus *)pData;
			IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserStatus->dwUserID);

			//用户判断
			ASSERT(pIClientUserItem!=NULL);
			if (pIClientUserItem==NULL) return true;

			//状态定义
			BYTE cbUserStatus=pUserStatus->UserStatus.cbUserStatus;

			//离开判断
			if ((cbUserStatus==US_NULL)||(cbUserStatus==US_FREE))
			{
				if (m_pIMySelfUserItem==pIClientUserItem)
				{
					//设置变量
					m_bService=false;
					m_bAllowLookon=false;
					m_pIMySelfUserItem=NULL;
					m_cbGameStatus=GAME_STATUS_FREE;
					ZeroMemory(&m_UserAttribute,sizeof(m_UserAttribute));

					//删除时间
					KillGameClock(0L);

					//设置标题
					SetGameFrameTitle();

					//重置游戏
					m_pIClientKernelSink->ResetGameClient();

					//删除用户
					m_GameUserManagerModule->ResetUserItem();
				}
				else
				{
					//删除时间
					if (pIClientUserItem->GetUserStatus()!=US_LOOKON)
					{
						WORD wChairID=pIClientUserItem->GetChairID();
						if (m_wClockChairID==wChairID) KillGameClock(m_wClockID);
					}

					//删除用户
					m_GameUserManagerModule->DeleteUserItem(pIClientUserItem);
				}

				return true;
			}

			//设置状态
			m_GameUserManagerModule->UpdateUserItemStatus(pIClientUserItem,&pUserStatus->UserStatus);

			return true;
		}
	case IPC_SUB_GF_CUSTOM_FACE:	//自定头像
		{
			//效验参数
			ASSERT(wDataSize==sizeof(IPC_GF_CustomFace));
			if (wDataSize!=sizeof(IPC_GF_CustomFace)) return false;

			//寻找用户
			IPC_GF_CustomFace * pCustomFace=(IPC_GF_CustomFace *)pData;
			IClientUserItem * pIClientUserItem=m_GameUserManagerModule->SearchUserByUserID(pCustomFace->dwUserID);

			//用户判断
			ASSERT(pIClientUserItem!=NULL);
			if (pIClientUserItem==NULL) return true;

			//设置积分
			ASSERT(m_GameUserManagerModule.GetInterface()!=NULL);
			m_GameUserManagerModule->UpdateUserCustomFace(pIClientUserItem,pCustomFace->dwCustomID,pCustomFace->CustomFaceInfo);

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//用户聊天
bool CClientKernel::OnSocketSubUserChat(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GF_S_UserChat * pUserChat=(CMD_GF_S_UserChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GF_S_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//显示消息
	if (m_pIStringMessage!=NULL)
	{
		//获取用户
		IClientUserItem * pISendUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserChat->dwSendUserID);
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
			CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

			//好友模式
			if ((pGlobalUnits->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
			{
				return true;
			}

			//厌恶模式
			if ((pGlobalUnits->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
			{
				return true;
			}
		}

		//插入消息
		if (pUserChat->dwTargetUserID!=0L)
		{
			//获取用户
			IClientUserItem * pIRecvUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserChat->dwTargetUserID);

			if (pIRecvUserItem!=NULL)
			{
				//插入消息
				LPCTSTR pszSendUser=pISendUserItem->GetNickName();
				LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
				m_pIStringMessage->InsertUserChat(pszSendUser,pszRecvUser,pUserChat->szChatString,pUserChat->dwChatColor);
			}
		}
		else 
		{
			//插入消息
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			m_pIStringMessage->InsertUserChat(pszSendUser,pUserChat->szChatString,pUserChat->dwChatColor);
		}
	}

	return true;
}

//用户表情
bool CClientKernel::OnSocketSubExpression(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GF_S_UserExpression));
	if (wDataSize!=sizeof(CMD_GF_S_UserExpression)) return false;

	//变量定义
	CMD_GF_S_UserExpression * pUserExpression=(CMD_GF_S_UserExpression *)pData;

	//获取用户
	IClientUserItem * pISendUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserExpression->dwSendUserID);
	if (pISendUserItem==NULL) return true;

	//变量定义
	CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
	CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(pUserExpression->wItemIndex);

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
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

		//好友模式
		if ((pGlobalUnits->m_cbMessageMode==MESSAGE_MODE_FRIEND)&&(cbUserCompanion!=CP_FRIEND)&&(bSameGroup==false))
		{
			return true;
		}

		//厌恶模式
		if ((pGlobalUnits->m_cbMessageMode==MESSAGE_MODE_DETEST)&&(cbUserCompanion==CP_DETEST)&&(bSameGroup==false))
		{
			return true;
		}
	}

	//显示消息
	if ((m_pIStringMessage!=NULL)&&(pExpressionItem!=NULL))
	{
		//获取路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

		//插入消息
		if (pUserExpression->dwTargetUserID!=0L)
		{
			//获取用户
			IClientUserItem * pIRecvUserItem=m_GameUserManagerModule->SearchUserByUserID(pUserExpression->dwTargetUserID);

			//插入消息
			if (pIRecvUserItem!=NULL)
			{
				LPCTSTR pszSendUser=pISendUserItem->GetNickName();
				LPCTSTR pszRecvUser=pIRecvUserItem->GetNickName();
				m_pIStringMessage->InsertExpression(pszSendUser,pszRecvUser,szImagePath);
			}
		}
		else
		{
			//插入消息
			LPCTSTR pszSendUser=pISendUserItem->GetNickName();
			m_pIStringMessage->InsertExpression(pszSendUser,szImagePath);
		}
	}

	return true;
}

//游戏状态
bool CClientKernel::OnSocketSubGameStatus(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GF_GameStatus));
	if (wDataSize!=sizeof(CMD_GF_GameStatus)) return false;

	//消息处理
	CMD_GF_GameStatus * pGameStatus=(CMD_GF_GameStatus *)pData;

	//设置变量
	m_cbGameStatus=pGameStatus->cbGameStatus;
	m_bAllowLookon=pGameStatus->cbAllowLookon?true:false;

	return true;
}

//游戏场景
bool CClientKernel::OnSocketSubGameScene(VOID * pData, WORD wDataSize)
{
	//效验状态
	ASSERT(m_pIMySelfUserItem!=NULL);
	if (m_pIMySelfUserItem==NULL) return true;

	//场景处理
	bool bLookonUser=(m_pIMySelfUserItem->GetUserStatus()==US_LOOKON);
	return m_pIClientKernelSink->OnEventSceneMessage(m_cbGameStatus,bLookonUser,pData,wDataSize);
}

//旁观状态
bool CClientKernel::OnSocketSubLookonStatus(VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GF_LookonStatus));
	if (wDataSize!=sizeof(CMD_GF_LookonStatus)) return false;

	//消息处理
	if (IsLookonMode()==true)
	{
		//变量定义
		CMD_GF_LookonStatus * pLookonStatus=(CMD_GF_LookonStatus *)pData;

		//事件处理
		m_bAllowLookon=(pLookonStatus->cbAllowLookon==TRUE)?true:false;

		//事件通知
		ASSERT(m_pIMySelfUserItem!=NULL);
		m_pIClientKernelSink->OnEventLookonMode(pData,wDataSize);

		//提示消息
		if (m_bAllowLookon==true)
		{
			if (m_pIStringMessage!=NULL)
			{
				m_pIStringMessage->InsertNormalString(TEXT("您被允许观看玩家游戏"));
			}
		}
		else
		{
			if (m_pIStringMessage!=NULL)
			{
				m_pIStringMessage->InsertNormalString(TEXT("您被禁止观看玩家游戏"));
			}
		}
	}

	return true;
}

//系统消息
bool CClientKernel::OnSocketSubSystemMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_SystemMessage * pSystemMessage=(CMD_CM_SystemMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_SystemMessage)-sizeof(pSystemMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize==(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize!=(wHeadSize+pSystemMessage->wLength*sizeof(TCHAR)))) return false;

	//关闭处理
	if ((pSystemMessage->wType&SMT_CLOSE_GAME)!=0)
	{
		//设置变量
		m_bService=false;

		//删除时间
		KillGameClock(0);

		//中断连接
		IntermitConnect();
	}

	//显示消息
	if ((pSystemMessage->wType&SMT_CHAT)&&(m_pIStringMessage!=NULL))
	{
		m_pIStringMessage->InsertSystemString(pSystemMessage->szString);
	}

	//弹出消息
	if (pSystemMessage->wType&SMT_EJECT)
	{
		CInformation Information(AfxGetMainWnd());
		Information.ShowMessageBox(pSystemMessage->szString,MB_ICONINFORMATION,0);
	}

	//关闭房间
	if (pSystemMessage->wType&SMT_CLOSE_GAME)
	{
		m_pIClientKernelSink->CloseGameClient();
	}

	return true;
}

//动作消息
bool CClientKernel::OnSocketSubActionMessage(VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_CM_ActionMessage * pActionMessage=(CMD_CM_ActionMessage *)pData;
	WORD wHeadSize=sizeof(CMD_CM_ActionMessage)-sizeof(pActionMessage->szString);

	//效验参数
	ASSERT((wDataSize>wHeadSize)&&(wDataSize>=(wHeadSize+pActionMessage->wLength*sizeof(TCHAR))));
	if ((wDataSize<=wHeadSize)||(wDataSize<(wHeadSize+pActionMessage->wLength*sizeof(TCHAR)))) return false;

	//关闭处理
	if ((pActionMessage->wType&SMT_CLOSE_GAME)!=0)
	{
		//设置变量
		m_bService=false;

		//删除时间
		KillGameClock(0L);

		//中断连接
		IntermitConnect();
	}

	//弹出消息
	INT nResultCode=ShowInformation(pActionMessage->szString,pActionMessage->nButtonType,0);

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
						ShellExecute(NULL,TEXT("open"),pActionBrowse->szBrowseUrl,NULL,NULL,SW_NORMAL);
					}

					//大厅浏览
					if (pActionBrowse->cbBrowseType&BRT_PLAZA)
					{
//						CPlatformFrame::GetInstance()->WebBrowse(pActionBrowse->szBrowseUrl,true,true);
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
						ShellExecute(NULL,TEXT("open"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					//下载模块
					if (pActionDownLoad->cbDownLoadMode&DLT_MODULE)
					{
						ShellExecute(NULL,TEXT("open"),pActionDownLoad->szDownLoadUrl,NULL,NULL,SW_NORMAL);
					}

					break;
				}
			}
		}

		//增加偏移
		wExcursion+=(sizeof(tagActionHead)+pActionHead->wAppendSize);
	}

	//关闭房间
	if (pActionMessage->wType&SMT_CLOSE_GAME) m_pIClientKernelSink->CloseGameClient();

	return true;
}

//设置标题
VOID CClientKernel::SetGameFrameTitle()
{
	//变量定义
	TCHAR szTitle[128]=TEXT("");

	//构造标题
	if ((m_ServerAttribute.szServerName[0]==0)||(m_UserAttribute.dwUserID==0L))
	{
		DWORD dwClientVersion=m_GameAttribute.dwClientVersion;
		_sntprintf(szTitle,CountArray(szTitle),TEXT("%s － Build %ld.%ld.%ld.%ld"),m_GameAttribute.szGameName,GetProductVer(dwClientVersion),
			GetMainVer(dwClientVersion),GetSubVer(dwClientVersion),GetBuildVer(dwClientVersion));
	}
	else
	{
		if (CServerRule::IsAvertCheatMode(m_ServerAttribute.dwServerRule)==false)
		{
			_sntprintf(szTitle,CountArray(szTitle),TEXT("%s ［ %s - 第 %d 桌 ］"),m_GameAttribute.szGameName,m_ServerAttribute.szServerName,m_UserAttribute.wTableID+1);
		}
		else
		{
			_sntprintf(szTitle,CountArray(szTitle),TEXT("%s ［ %s ］"),m_GameAttribute.szGameName,m_ServerAttribute.szServerName);
		}
	}

	//设置标题
	CWnd * pMainWnd=AfxGetMainWnd();
	if ((pMainWnd!=NULL)&&(pMainWnd->m_hWnd!=NULL)) pMainWnd->SetWindowText(szTitle);
}

//时间消息
VOID CClientKernel::OnTimer(UINT nIDEvent)
{
	//时间处理
	if ((m_wClockID==nIDEvent)&&(m_wClockChairID!=INVALID_CHAIR))
	{
		//事件处理
		if (m_nElapseCount>0) m_nElapseCount--;
		bool bSuccess=m_pIClientKernelSink->OnEventGameClockInfo(m_wClockChairID,m_nElapseCount,m_wClockID);

		//删除处理
		if ((m_nElapseCount==0)||(bSuccess==false)) KillGameClock(m_wClockID);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(ClientKernel);

//////////////////////////////////////////////////////////////////////////////////
