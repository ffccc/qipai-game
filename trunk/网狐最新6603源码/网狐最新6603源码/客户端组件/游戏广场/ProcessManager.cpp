#include "StdAfx.h"
#include "GlobalUnits.h"
#include "ProcessManager.h"

//////////////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_PROCESS_DETECT			100									//���̼��

//ʱ���ʶ
#define TIME_DETECT_NORMAL			1000L								//������
#define TIME_DETECT_CREATE			8000L								//�������

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CProcessManager, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CProcessManager::CProcessManager()
{
	//ͨѶ����
	m_hProcessControl=NULL;

	//�ں˱���
	m_pIProcessManagerSink=NULL;
	ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

	return;
}

//��������
CProcessManager::~CProcessManager()
{
	//�رս���
	CloseGameProcess(false);

	return;
}

//�ӿڲ�ѯ
VOID * CProcessManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IProcessManager,Guid,dwQueryVer);
	QUERYINTERFACE(IProcessControlSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IProcessManager,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CProcessManager::OnProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, HWND hWndSend)
{
	//Ч��״̬
	ASSERT(m_pIProcessManagerSink!=NULL);
	if (m_pIProcessManagerSink==NULL) return false;

	//����Ч��
	ASSERT((m_hProcessControl==NULL)||(m_hProcessControl==hWndSend));
	if ((m_hProcessControl!=NULL)&&(m_hProcessControl!=hWndSend)) return false;

	//׼����Ϣ
	if (wMainCmdID==IPC_CMD_GF_CONTROL)
	{
		switch (wSubCmdID)
		{
		case IPC_SUB_GF_CLIENT_READY:	//׼������
			{
				//Ч��״̬
				ASSERT(m_hProcessControl==NULL);
				if (m_hProcessControl!=NULL) return false;

				//�¼�����
				m_hProcessControl=hWndSend;
				m_pIProcessManagerSink->OnGameProcessCreate(false);

				//�������
				SetTimer(IDI_PROCESS_DETECT,TIME_DETECT_NORMAL,NULL);

				return true;
			}
		case IPC_SUB_GF_CLIENT_CLOSE:	//���̹ر�
			{
				//���ñ���
				m_hProcessControl=NULL;

				//�������
				DetachGameProcess(true);

				return true;
			}
		}
	}

	//��Ϣ����
	m_pIProcessManagerSink->OnProcessManagerData(wMainCmdID,wSubCmdID,pData,wDataSize);

	return true;
}

//�رս���
bool CProcessManager::CloseGameProcess(bool bPerformSink)
{
	//ɾ��ʱ��
	if (m_hWnd!=NULL) KillTimer(IDI_PROCESS_DETECT);

	//�رս���
	if (m_ProcessInfo.hProcess!=NULL)
	{
		//�����ж�
		if ((m_hProcessControl!=NULL)&&(IsWindow(m_hProcessControl)!=FALSE))
		{
			//������Ϣ
			SendProcessCommand(IPC_CMD_GF_CONTROL,IPC_SUB_GF_CLOSE_PROCESS,NULL,0);

			//��������
			DWORD dwResult=WaitForSingleObject(m_ProcessInfo.hProcess,1000L);
			if (dwResult==WAIT_TIMEOUT) TerminateProcess(m_ProcessInfo.hProcess,0);
		}
		else
		{
			//��������
			DWORD dwResult=WaitForSingleObject(m_ProcessInfo.hProcess,0L);
			if (dwResult==WAIT_TIMEOUT) TerminateProcess(m_ProcessInfo.hProcess,0);
		}

		//�˳�����
		DWORD dwExitCode=0L;
		GetExitCodeProcess(m_ProcessInfo.hProcess,&dwExitCode);

		//�رվ��
		if (m_ProcessInfo.hThread!=NULL) CloseHandle(m_ProcessInfo.hThread);
		if (m_ProcessInfo.hProcess!=NULL) CloseHandle(m_ProcessInfo.hProcess);

		//���ñ���
		m_hProcessControl=NULL;
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

		//�¼�֪ͨ
		ASSERT(m_pIProcessManagerSink!=NULL);
		if (bPerformSink==true) m_pIProcessManagerSink->OnGameProcessClose(dwExitCode);
	}

	return true;
}

//�������
bool CProcessManager::DetachGameProcess(bool bPerformSink)
{
	//ɾ��ʱ��
	if (m_hWnd!=NULL) KillTimer(IDI_PROCESS_DETECT);

	//����֪ͨ
	if (m_ProcessInfo.hProcess!=NULL)
	{
		//�رվ��
		if (m_ProcessInfo.hThread!=NULL) CloseHandle(m_ProcessInfo.hThread);
		if (m_ProcessInfo.hProcess!=NULL) CloseHandle(m_ProcessInfo.hProcess);

		//���ñ���
		m_hProcessControl=NULL;
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));

		//�¼�֪ͨ
		ASSERT(m_pIProcessManagerSink!=NULL);
		if (bPerformSink==true) m_pIProcessManagerSink->OnGameProcessClose(0L);
	}

	return true;
}

//��������
bool CProcessManager::CreateGameProcess(LPCTSTR pszProcessName)
{
	//Ч��״̬
	ASSERT(m_pIProcessManagerSink!=NULL);
	if (m_pIProcessManagerSink==NULL) return false;

	//��������
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,AfxGetMainWnd(),100);
	}

	//�������
	if (m_ProcessControlModule.GetInterface()==NULL)
	{
		//�������
		if (m_ProcessControlModule.CreateInstance()==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ýӿ�
		if (m_ProcessControlModule->SetProcessControlSink(QUERY_ME_INTERFACE(IUnknownEx))==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//�������
		if (m_ProcessControlModule->InitializeLink()==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//�����ж�
	if ((m_ProcessInfo.hProcess!=NULL)&&(WaitForSingleObject(m_ProcessInfo.hProcess,0)==WAIT_TIMEOUT))
	{
		//�����ж�
		if ((m_hProcessControl!=NULL)&&(IsWindow(m_hProcessControl)!=FALSE))
		{
			//����֪ͨ
			m_pIProcessManagerSink->OnGameProcessCreate(true);

			//�������
			SendProcessData(IPC_CMD_GF_CONTROL,IPC_SUB_GF_ACTIVE_PROCESS);
		}

		return true;
	}

	//��������
	HWND hProcessTrade=m_ProcessControlModule->GetProcessControlWnd();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//�������
	TCHAR szCommomLine[MAX_PATH];
	_sntprintf(szCommomLine,CountArray(szCommomLine),TEXT("%s /Transmittal:%I64d"),pszProcessName,(LONGLONG)(hProcessTrade));

	//��������
	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo,sizeof(StartInfo));

	//��������
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	BOOL bSuccess=CreateProcess(NULL,szCommomLine,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_ProcessInfo);

	//������
	if (bSuccess==FALSE)
	{
		ZeroMemory(&m_ProcessInfo,sizeof(m_ProcessInfo));
		return false;
	}

	//�������
	SetTimer(IDI_PROCESS_DETECT,TIME_DETECT_CREATE,NULL);

	return true;
}

//���ýӿ�
bool CProcessManager::SetProcessManagerSink(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessManagerSink)!=NULL);
		m_pIProcessManagerSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IProcessManagerSink);

		//�ɹ��ж�
		if (m_pIProcessManagerSink==NULL) return false;
	}
	else m_pIProcessManagerSink=NULL;

	return true;
}

//��������
bool CProcessManager::SendProcessData(WORD wMainCmdID, WORD wSubCmdID)
{
	//״̬Ч��
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//��������
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendData(m_hProcessControl,wMainCmdID,wSubCmdID);
	}

	return true;
}

//��������
bool CProcessManager::SendProcessData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//��������
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendData(m_hProcessControl,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//ִ������
bool CProcessManager::SendProcessCommand(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_ProcessControlModule.GetInterface()!=NULL);
	if (m_ProcessControlModule.GetInterface()==NULL) return false;

	//ִ������
	if (m_hProcessControl!=NULL)
	{
		m_ProcessControlModule->SendCommand(m_hProcessControl,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//������Ϣ
bool CProcessManager::SendServerInfo(IClientUserItem * pIMyselfUserItem, DWORD dwUserRight, DWORD dwMasterRight, DWORD dwServerRule, WORD wServerType, tagGameServer & GameServer)
{
	//������Ϣ
	IPC_GF_ServerInfo ServerInfoData;
	ZeroMemory(&ServerInfoData,sizeof(ServerInfoData));

	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//�û���Ϣ
	ServerInfoData.dwUserRight=dwUserRight;
	ServerInfoData.dwMasterRight=dwMasterRight;
	ServerInfoData.dwUserID=pIMyselfUserItem->GetUserID();
	ServerInfoData.wTableID=pIMyselfUserItem->GetTableID();
	ServerInfoData.wChairID=pIMyselfUserItem->GetChairID();

	//������Ϣ
	ServerInfoData.wServerType=wServerType;
	ServerInfoData.dwServerRule=dwServerRule;
	ServerInfoData.wKindID=GameServer.wKindID;
	ServerInfoData.wServerID=GameServer.wServerID;
	lstrcpyn(ServerInfoData.szServerName,GameServer.szServerName,CountArray(ServerInfoData.szServerName));

	//��������
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_SERVER_INFO,&ServerInfoData,sizeof(ServerInfoData));

	return true;
}

//�û���Ϣ
bool CProcessManager::SendUserItem(IClientUserItem * pIClientUserItem)
{
	//���ͻ���
	BYTE cbBuffer[IPC_PACKET];
	ZeroMemory(cbBuffer,sizeof(cbBuffer));

	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//��������
	IPC_GF_UserInfo * pIPCUserInfo=(IPC_GF_UserInfo *)cbBuffer;
	tagUserInfoHead * pUserInfoHead=&pIPCUserInfo->UserInfoHead;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(IPC_GF_UserInfo),sizeof(cbBuffer)-sizeof(IPC_GF_UserInfo));

	//������Ϣ
	pIPCUserInfo->cbCompanion=pIClientUserItem->GetUserCompanion();

	//�û�����
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->dwGroupID=pUserInfo->dwGroupID;
	pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//�û�����
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserInfo->cbMasterOrder;

	//�û�״̬
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//�û�����
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

	//������Ϣ
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);
	SendPacket.AddPacket(pUserInfo->szGroupName,DTP_GR_GROUP_NAME);
	SendPacket.AddPacket(pUserInfo->szUnderWrite,DTP_GR_UNDER_WRITE);
	SendPacket.AddPacket(pIClientUserItem->GetUserNoteInfo(),DTP_GR_USER_NOTE);

	//�Զ�ͷ��
	if ((pUserInfo->dwCustomID!=0L)&&(pCustomFaceInfo->dwDataSize!=0L))
	{
		SendPacket.AddPacket(pCustomFaceInfo,sizeof(tagCustomFaceInfo),DTP_GR_CUSTOM_FACE);
	}

	//��������
	WORD wSendSize=sizeof(IPC_GF_UserInfo)+SendPacket.GetDataSize();
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_ENTER,cbBuffer,wSendSize);

	return true;
}

//�û�����
bool CProcessManager::SendUserScore(DWORD dwUserID, tagUserScore & UserScore)
{
	//�û�״̬
	IPC_GF_UserScore UserScoreData;
	ZeroMemory(&UserScoreData,sizeof(UserScoreData));

	//������Ϣ
	UserScoreData.dwUserID=dwUserID;
	UserScoreData.UserScore=UserScore;

	//��������
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_SCORE,&UserScoreData,sizeof(UserScoreData));

	return true;
}

//�û�״̬
bool CProcessManager::SendUserStatus(DWORD dwUserID, tagUserStatus & UserStatus)
{
	//�û�״̬
	IPC_GF_UserStatus UserStatusData;
	ZeroMemory(&UserStatusData,sizeof(UserStatusData));

	//������Ϣ
	UserStatusData.dwUserID=dwUserID;
	UserStatusData.UserStatus=UserStatus;

	//��������
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_USER_STATUS,&UserStatusData,sizeof(UserStatusData));

	return true;
}

//�û�ͷ��
bool CProcessManager::SendCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo * pCustomFaceInfo)
{
	//��������
	IPC_GF_CustomFace CustomFaceData;
	ZeroMemory(&CustomFaceData,sizeof(CustomFaceData));

	//������Ϣ
	CustomFaceData.dwUserID=dwUserID;
	CustomFaceData.dwCustomID=dwCustomID;
	CopyMemory(&CustomFaceData.CustomFaceInfo,pCustomFaceInfo,sizeof(CustomFaceData.CustomFaceInfo));

	//��������
	SendProcessData(IPC_CMD_GF_USER_INFO,IPC_SUB_GF_CUSTOM_FACE,&CustomFaceData,sizeof(CustomFaceData));

	return true;
}

//�ȼ���Ϣ
bool CProcessManager::SendLevelInfo(tagLevelItem LevelItem[], BYTE cbLevelCount)
{
	//��������
	IPC_GF_LevelInfo LevelInfoData;
	ZeroMemory(&LevelInfoData,sizeof(LevelInfoData));

	//������Ϣ
	LevelInfoData.cbItemCount=__min(cbLevelCount,CountArray(LevelInfoData.LevelItem));
	CopyMemory(LevelInfoData.LevelItem,LevelItem,sizeof(tagLevelItem)*LevelInfoData.cbItemCount);

	//��������
	WORD wHeadSize=sizeof(LevelInfoData)-sizeof(LevelInfoData.LevelItem);
	WORD wDataSize=sizeof(LevelInfoData.LevelItem[0])*LevelInfoData.cbItemCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_LEVEL_INFO,&LevelInfoData,wHeadSize+wDataSize);

	return true;
}

//�б���Ϣ
bool CProcessManager::SendColumnInfo(tagColumnItem ColumnItem[], BYTE cbColumnCount)
{
	//��������
	IPC_GF_ColumnInfo ColumnInfoData;
	ZeroMemory(&ColumnInfoData,sizeof(ColumnInfoData));

	//������Ϣ
	ColumnInfoData.cbColumnCount=__min(cbColumnCount,CountArray(ColumnInfoData.ColumnItem));
	CopyMemory(ColumnInfoData.ColumnItem,ColumnItem,sizeof(tagColumnItem)*ColumnInfoData.cbColumnCount);

	//��������
	WORD wHeadSize=sizeof(ColumnInfoData)-sizeof(ColumnInfoData.ColumnItem);
	WORD wDataSize=sizeof(ColumnInfoData.ColumnItem[0])*ColumnInfoData.cbColumnCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_COLUMN_INFO,&ColumnInfoData,wHeadSize+wDataSize);

	return true;
}

//������Ϣ
bool CProcessManager::SendPropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount)
{
	//��������
	IPC_GF_PropertyInfo PropertyInfoData;
	ZeroMemory(&PropertyInfoData,sizeof(PropertyInfoData));

	//������Ϣ
	PropertyInfoData.cbPropertyCount=__min(cbPropertyCount,CountArray(PropertyInfoData.PropertyInfo));
	CopyMemory(PropertyInfoData.PropertyInfo,PropertyInfo,sizeof(tagPropertyInfo)*PropertyInfoData.cbPropertyCount);

	//��������
	WORD wHeadSize=sizeof(PropertyInfoData)-sizeof(PropertyInfoData.PropertyInfo);
	WORD wDataSize=sizeof(PropertyInfoData.PropertyInfo[0])*PropertyInfoData.cbPropertyCount;
	SendProcessData(IPC_CMD_GF_CONFIG,IPC_SUB_GF_PROPERTY_INFO,&PropertyInfoData,wHeadSize+wDataSize);

	return true;
}

//ʱ����Ϣ
VOID CProcessManager::OnTimer(UINT_PTR nIDEvent)
{
	//���̼��
	if (nIDEvent==IDI_PROCESS_DETECT)
	{
		//״̬Ч��
		ASSERT(m_ProcessInfo.hProcess!=NULL);
		if (m_ProcessInfo.hProcess==NULL) return;

		//�����ж�
		if ((m_hProcessControl==NULL)||(IsWindow(m_hProcessControl)==FALSE))
		{
			//��ֹ����
			DetachGameProcess(false);

			//�¼�֪ͨ
			ASSERT(m_pIProcessManagerSink!=NULL);
			if (m_pIProcessManagerSink!=NULL) m_pIProcessManagerSink->OnGameProcessError();
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
