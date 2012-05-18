#include "StdAfx.h"
#include "GlobalUnits.h"
#include "ProcessManager.h"

//////////////////////////////////////////////////////////////////////////////////

//定时器标识
#define IDI_PROCESS_DETECT			100									//进程检测

//时间标识
#define TIME_DETECT_NORMAL			1000L								//常规检测
#define TIME_DETECT_CREATE			8000L								//创建检测

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CProcessManager, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CProcessManager::CProcessManager()
{
	//通讯变量
	m_hProcessControl=NULL;

	//内核变量
	m_pIProcessManagerSink=NULL;
	ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

	return;
}

//析构函数
CProcessManager::~CProcessManager()
{
	//关闭进程
	CloseGameProcess(false);

	return;
}

//接口查询
VOID * CProcessManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IProcessManager,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessControlSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IProcessManager,Guid,dwQueryVer);
	return NULL;
}

//进程数据
bool CProcessManager::OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)
{
	//效验状态
	ASSERT(m_pIProcessManagerSink!=NULL);
	if (m_pIProcessManagerSink==NULL) return false;

	//窗口效验
	ASSERT((m_hProcessControl==NULL)||(m_hProcessControl==hWndSend));
	if ((m_hProcessControl!=NULL)&&(m_hProcessControl!=hWndSend)) return false;

	//准备消息
	if (wMainCmdID==IPC_CMD_GF_CONTROL)
	{
		switch (wSubCmdID)
		{
		case IPC_SUB_GF_CLIENT_READY:	//准备就绪
			{
				//效验状态
				ASSERT(m_hProcessControl==NULL);
				if (m_hProcessControl!=NULL) return false;

				//事件处理
				m_hProcessControl=hWndSend;
				m_pIProcessManagerSink->OnGameProcessCreate(false);

				//检测设置
				SetTimer(IDI_PROCESS_DETECT,TIME_DETECT_NORMAL,NULL);

				return true;
			}
		case IPC_SUB_GF_CLIENT_CLOSE:	//进程关闭
			{
				//设置变量
				m_hProcessControl=NULL;

				//分离进程
				DetachGameProcess(true);

				return true;
			}
		}
	}

	//消息处理
	m_pIProcessManagerSink->OnProcessManagerData(wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//关闭进程
bool CProcessManager::CloseGameProcess(bool bPerformSink)
{
	//删除时间
	if (m_hWnd!=NULL) KillTimer(IDI_PROCESS_DETECT);

	//关闭进程
	if (m_ProcessInfo.hProcess!=NULL)
	{
		//连接判断
		if ((m_hProcessControl!=NULL)&&(IsWindow(m_hProcessControl)!=FALSE))
		{
			//发送消息
			SendProcessCommand(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLOSE_PROCESS,NULL,0);

			//结束进程
			DWORD dwResult=WaitForSingleObject(m_ProcessInfo.hProcess,1000L);
			if (dwResult==WAIT_TIMEOUT) TerminateProcess(m_ProcessInfo.hProcess,0);
		}
		else
		{
			//结束进程
			DWORD dwResult=WaitForSingleObject(m_ProcessInfo.hProcess,0L);
			if (dwResult==WAIT_TIMEOUT) TerminateProcess(m_ProcessInfo.hProcess,0);
		}

		//退出代码
		DWORD dwExitCode=0L;
		GetExitCodeProcess(m_ProcessInfo.hProcess,&dwExitCode);

		//关闭句柄
		if (m_ProcessInfo.hThread!=NULL) CloseHandle(m_ProcessInfo.hThread);
		if (m_ProcessInfo.hProcess!=NULL) CloseHandle(m_ProcessInfo.hProcess);

		//设置变量
		m_hProcessControl=NULL;
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

		//事件通知
		ASSERT(m_pIProcessManagerSink!=NULL);
		if (bPerformSink==true) m_pIProcessManagerSink->OnGameProcessClose(dwExitCode);
	}

	return true;
}

//分离进程
bool CProcessManager::DetachGameProcess(bool bPerformSink)
{
	//删除时间
	if (m_hWnd!=NULL) KillTimer(IDI_PROCESS_DETECT);

	//分离通知
	if (m_ProcessInfo.hProcess!=NULL)
	{
		//关闭句柄
		if (m_ProcessInfo.hThread!=NULL) CloseHandle(m_ProcessInfo.hThread);
		if (m_ProcessInfo.hProcess!=NULL) CloseHandle(m_ProcessInfo.hProcess);

		//设置变量
		m_hProcessControl=NULL;
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

		//事件通知
		ASSERT(m_pIProcessManagerSink!=NULL);
		if (bPerformSink==true) m_pIProcessManagerSink->OnGameProcessClose(0L);
	}

	return true;
}

//启动进程
bool CProcessManager::CreateGameProcess(LPCTSTR pszProcessName)
{
	//效验状态
	ASSERT(m_pIProcessManagerSink!=NULL);
	if (m_pIProcessManagerSink==NULL) return false;

	//创建窗口
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,AfxGetMainWnd(),100);
	}

	//创建组件
	if (m_ProcessControlModule.GetInterface()==NULL)
	{
		//创建组件
		if (m_ProcessControlModule.CreateInstance()==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//设置接口
		if (m_ProcessControlModule->SetProcessControlSink(QUERY_ME_INTERFACE(IUnknownEx))==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//配置组件
		if (m_ProcessControlModule->InitializeLink()==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//启动判断
	if ((m_ProcessInfo.hProcess!=NULL)&&(WaitForSingleObject(m_ProcessInfo.hProcess,0)==WAIT_TIMEOUT))
	{
		//连接判断
		if ((m_hProcessControl!=NULL)&&(IsWindow(m_hProcessControl)!=FALSE))
		{
			//启动通知
			m_pIProcessManagerSink->OnGameProcessCreate(true);

			//激活进程
			SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
		}

		return true;
	}

	//变量定义
	HWND hProcessTrade=m_ProcessControlModule->GetProcessControlWnd();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//构造参数
	TCHAR szCommomLine[MAX_PATH];
	_sntprintf(szCommomLine,CountArray(szCommomLine),TEXT("%s /Transmittal:%I64d"),pszProcessName,(LONGLONG)(hProcessTrade));

	//变量定义
	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo,sizeof(StartInfo));

	//启动进程
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	BOOL bSuccess=CreateProcess(NULL,szCommomLine,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_ProcessInfo);

	//错误处理
	if (bSuccess==FALSE)
	{
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));
		return false;
	}

	//创建检测
	SetTimer(IDI_PROCESS_DETECT,TIME_DETECT_CREATE,NULL);

	return true;
}

//设置接口
bool CProcessManager::SetProcessManagerSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessManagerSink)!=NULL);
		m_pIProcessManagerSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessManagerSink);

		//成功判断
		if (m_pIProcessManagerSink==NULL) return false;
	}
	else m_pIProcessManagerSink=NULL;

	return true;
}

//发送数据
bool CProcessManager::SendProcessData(WORD wMainCmdID, WORD wSubCmdID)
{
	//状态效验
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//发送数据
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendData(m_hProcessControl,wMainCmdID,wSubCmdID);
	}

	return true;
}

//发送数据
bool CProcessManager::SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//状态效验
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//发送数据
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendData(m_hProcessControl,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//执行命令
bool CProcessManager::SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//状态效验
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//执行命令
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendCommand(m_hProcessControl,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//房间信息
bool CProcessManager::SendServerInfo(IClientUserItem * pIMyselfUserItem, DWORD dwUserRight, DWORD dwMasterRight, DWORD dwServerRule, WORD wServerType, tagGameServer & GameServer)
{
	//房间信息
	IPC_GF_ServerInfo ServerInfoData;
	ZeroMemory(&ServerInfoData,sizeof(ServerInfoData));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//用户信息
	ServerInfoData.dwUserRight=dwUserRight;
	ServerInfoData.dwMasterRight=dwMasterRight;
	ServerInfoData.dwUserID=pIMyselfUserItem->GetUserID();
	ServerInfoData.wTableID=pIMyselfUserItem->GetTableID();
	ServerInfoData.wChairID=pIMyselfUserItem->GetChairID();

	//房间信息
	ServerInfoData.wServerType=wServerType;
	ServerInfoData.dwServerRule=dwServerRule;
	ServerInfoData.wKindID=GameServer.wKindID;
	ServerInfoData.wServerID=GameServer.wServerID;
	lstrcpyn(ServerInfoData.szServerName,GameServer.szServerName,CountArray(ServerInfoData.szServerName));

	//发送数据
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_SERVER_INFO,&ServerInfoData,sizeof(ServerInfoData));

	return true;
}

//用户信息
bool CProcessManager::SendUserItem(IClientUserItem * pIClientUserItem)
{
	//发送缓冲
	BYTE cbBuffer[IPC_PACKET];
	ZeroMemory(cbBuffer,sizeof(cbBuffer));

	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//变量定义
	IPC_GF_UserInfo * pIPCUserInfo=(IPC_GF_UserInfo *)cbBuffer;
	tagUserInfoHead * pUserInfoHead=&pIPCUserInfo->UserInfoHead;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(IPC_GF_UserInfo),sizeof(cbBuffer)-sizeof(IPC_GF_UserInfo));

	//构造信息
	pIPCUserInfo->cbCompanion=pIClientUserItem->GetUserCompanion();

	//用户属性
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwGroupID=pUserInfo->dwGroupID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//用户属性
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserInfo->cbMasterOrder;

	//用户状态
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//用户积分
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lGrade=pUserInfo->lGrade;
	pUserInfoHead->lInsure=pUserInfo->lInsure;
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwUserMedal=pUserInfo->dwUserMedal;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;
	pUserInfoHead->dwLoveLiness=pUserInfo->dwLoveLiness;

	//叠加信息
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);
	SendPacket.AddPacket(pUserInfo->szGroupName,DTP_GR_GROUP_NAME);
	SendPacket.AddPacket(pUserInfo->szUnderWrite,DTP_GR_UNDER_WRITE);
	SendPacket.AddPacket(pIClientUserItem->GetUserNoteInfo(),DTP_GR_USER_NOTE);

	//自定头像
	if ((pUserInfo->dwCustomID!=0L)&&(pCustomFaceInfo->dwDataSize!=0L))
	{
		SendPacket.AddPacket(pCustomFaceInfo,sizeof(tagCustomFaceInfo),DTP_GR_CUSTOM_FACE);
	}

	//发送数据
	WORD wSendSize=sizeof(IPC_GF_UserInfo)+SendPacket.GetDataSize();
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_ENTER,cbBuffer,wSendSize);

	return true;
}

//用户积分
bool CProcessManager::SendUserScore(DWORD dwUserID, tagUserScore & UserScore)
{
	//用户状态
	IPC_GF_UserScore UserScoreData;
	ZeroMemory(&UserScoreData,sizeof(UserScoreData));

	//构造信息
	UserScoreData.dwUserID=dwUserID;
	UserScoreData.UserScore=UserScore;

	//发送数据
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_SCORE,&UserScoreData,sizeof(UserScoreData));

	return true;
}

//用户状态
bool CProcessManager::SendUserStatus(DWORD dwUserID, tagUserStatus & UserStatus)
{
	//用户状态
	IPC_GF_UserStatus UserStatusData;
	ZeroMemory(&UserStatusData,sizeof(UserStatusData));

	//构造信息
	UserStatusData.dwUserID=dwUserID;
	UserStatusData.UserStatus=UserStatus;

	//发送数据
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_STATUS,&UserStatusData,sizeof(UserStatusData));

	return true;
}

//用户头像
bool CProcessManager::SendCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo * pCustomFaceInfo)
{
	//变量定义
	IPC_GF_CustomFace CustomFaceData;
	ZeroMemory(&CustomFaceData,sizeof(CustomFaceData));

	//构造信息
	CustomFaceData.dwUserID=dwUserID;
	CustomFaceData.dwCustomID=dwCustomID;
	CopyMemory(&CustomFaceData.CustomFaceInfo,pCustomFaceInfo,sizeof(CustomFaceData.CustomFaceInfo));

	//发送数据
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_CUSTOM_FACE,&CustomFaceData,sizeof(CustomFaceData));

	return true;
}

//等级信息
bool CProcessManager::SendLevelInfo(tagLevelItem LevelItem[], BYTE cbLevelCount)
{
	//变量定义
	IPC_GF_LevelInfo LevelInfoData;
	ZeroMemory(&LevelInfoData,sizeof(LevelInfoData));

	//构造信息
	LevelInfoData.cbItemCount=__min(cbLevelCount,CountArray(LevelInfoData.LevelItem));
	CopyMemory(LevelInfoData.LevelItem,LevelItem,sizeof(tagLevelItem)*LevelInfoData.cbItemCount);

	//发送数据
	WORD wHeadSize=sizeof(LevelInfoData)-sizeof(LevelInfoData.LevelItem);
	WORD wDataSize=sizeof(LevelInfoData.LevelItem[0])*LevelInfoData.cbItemCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LEVEL_INFO,&LevelInfoData,wHeadSize+wDataSize);

	return true;
}

//列表信息
bool CProcessManager::SendColumnInfo(tagColumnItem ColumnItem[], BYTE cbColumnCount)
{
	//变量定义
	IPC_GF_ColumnInfo ColumnInfoData;
	ZeroMemory(&ColumnInfoData,sizeof(ColumnInfoData));

	//构造信息
	ColumnInfoData.cbColumnCount=__min(cbColumnCount,CountArray(ColumnInfoData.ColumnItem));
	CopyMemory(ColumnInfoData.ColumnItem,ColumnItem,sizeof(tagColumnItem)*ColumnInfoData.cbColumnCount);

	//发送数据
	WORD wHeadSize=sizeof(ColumnInfoData)-sizeof(ColumnInfoData.ColumnItem);
	WORD wDataSize=sizeof(ColumnInfoData.ColumnItem[0])*ColumnInfoData.cbColumnCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_COLUMN_INFO,&ColumnInfoData,wHeadSize+wDataSize);

	return true;
}

//道具信息
bool CProcessManager::SendPropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount)
{
	//变量定义
	IPC_GF_PropertyInfo PropertyInfoData;
	ZeroMemory(&PropertyInfoData,sizeof(PropertyInfoData));

	//构造信息
	PropertyInfoData.cbPropertyCount=__min(cbPropertyCount,CountArray(PropertyInfoData.PropertyInfo));
	CopyMemory(PropertyInfoData.PropertyInfo,PropertyInfo,sizeof(tagPropertyInfo)*PropertyInfoData.cbPropertyCount);

	//发送数据
	WORD wHeadSize=sizeof(PropertyInfoData)-sizeof(PropertyInfoData.PropertyInfo);
	WORD wDataSize=sizeof(PropertyInfoData.PropertyInfo[0])*PropertyInfoData.cbPropertyCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_PROPERTY_INFO,&PropertyInfoData,wHeadSize+wDataSize);

	return true;
}

//时间消息
VOID CProcessManager::OnTimer(UINT_PTR nIDEvent)
{
	//进程检测
	if (nIDEvent==IDI_PROCESS_DETECT)
	{
		//状态效验
		ASSERT(m_ProcessInfo.hProcess!=NULL);
		if (m_ProcessInfo.hProcess==NULL) return;

		//连接判断
		if ((m_hProcessControl==NULL)||(IsWindow(m_hProcessControl)==FALSE))
		{
			//终止进程
			DetachGameProcess(false);

			//事件通知
			ASSERT(m_pIProcessManagerSink!=NULL);
			if (m_pIProcessManagerSink!=NULL) m_pIProcessManagerSink->OnGameProcessError();
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
