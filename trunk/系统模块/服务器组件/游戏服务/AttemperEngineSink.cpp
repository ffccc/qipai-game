#include "StdAfx.h"
#include "TableFrame.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//��Ϸ����
	m_wMaxUserItem=0;
	m_pTableFrame=NULL;
	m_pNormalParameter=NULL;
	m_pAndroidParameter=NULL;

	//���ͳ���
	m_cbSendLocation = 0;

	//��Ϣ����
	m_cbRoomMsgIndex=0;
	m_cbGameMsgIndex=0;

	//���ñ���
	m_dwCenterServer=0L;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//���Ʊ���
	m_bShallClose=false;
	m_bAllowWisper=true;
	m_bAllowRoomChat=true;
	m_bAllowGameChat=true;
	m_bAllowEnterRoom=true;
	m_bAllowEnterGame=true;

	//��������
	m_wColumnInfoSize=0;
	m_wOnLineInfoSize=0;
	memset(&m_ColumnInfo,0,sizeof(m_ColumnInfo));
	memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));

	//��Ϸ���
	m_hDllInstance=NULL;
	m_pIGameServiceManager=NULL;

	//�ӿڱ���
	m_pITimerEngine=NULL;
	m_pIAttemperEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketCorrespond=NULL;

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
	//ɾ���ڴ�
	SafeDeleteArray(m_pTableFrame);
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	return;
}

//�ӿڲ�ѯ
void * __cdecl CAttemperEngineSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameServiceFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//����ģ������
bool __cdecl CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx!=NULL);

	//���ñ���
	m_wColumnInfoSize=0;
	m_wOnLineInfoSize=0;
	memset(&m_ColumnInfo,0,sizeof(m_ColumnInfo));
	memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));

	//������������
	m_wMaxUserItem=m_pGameServiceOption->wMaxConnect;
	m_pNormalParameter=new tagConnectItemInfo [m_wMaxUserItem];
	m_pAndroidParameter=new tagConnectItemInfo [MAX_ANDROID];
	memset(m_pNormalParameter,0,sizeof(tagConnectItemInfo)*m_wMaxUserItem);
	memset(m_pAndroidParameter,0,sizeof(tagConnectItemInfo)*MAX_ANDROID);

	//���Ա���
	m_PropertyAttributeArrary.RemoveAll();
	m_FlowerAttributeArrary.RemoveAll();

	//������Ϸ���
	if (m_pIGameServiceManager->RectifyServiceOption(m_pGameServiceOption)==false) return false;

	//������Ϸ����
	ASSERT(m_pGameServiceOption->wTableCount!=0);
	m_pTableFrame=new CTableFrame [m_pGameServiceOption->wTableCount];
	if (m_pTableFrame==NULL) return false;

	//��ʼ������
	bool bSuccess=false;
	tagTableFrameParameter TableFrameParameter;
	TableFrameParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	TableFrameParameter.pGameServiceOption=m_pGameServiceOption;
	TableFrameParameter.pIGameServiceFrame=QUERY_ME_INTERFACE(IUnknownEx);
	TableFrameParameter.pIGameServiceManager=QUERY_OBJECT_PTR_INTERFACE(m_pIGameServiceManager,IUnknownEx);
	for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		bSuccess=(m_pTableFrame+i)->InitTableFrame(i,&TableFrameParameter);
		if (bSuccess==false) return false;
	}

	//��������
	m_AndroidUserManager.m_pITimerEngine=m_pITimerEngine;
	m_AndroidUserManager.m_pGameServiceAttrib=m_pGameServiceAttrib;
	m_AndroidUserManager.m_pGameServiceOption=m_pGameServiceOption;
	m_AndroidUserManager.m_pIGameServiceManager=m_pIGameServiceManager;
	m_AndroidUserManager.m_pIServerUserManager=QUERY_OBJECT_INTERFACE(m_ServerUserManager,IServerUserManager);
	m_AndroidUserManager.m_pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent);

	//�����б����
	switch (m_pGameServiceOption->wServerType)
	{
	case GAME_GENRE_GOLD:	//�������
		{
			//��������
			tagColumnItem ColumnItem[]=
			{
				115,DTP_USER_ACCOUNTS,TEXT("�û���"),
					60,DTP_GAME_ID,TEXT("ID ����"),
					60,DTP_USER_SCORE,TEXT("��Ϸ��"),
					40,DTP_USER_TABLE,TEXT("����"),
					60,DTP_LOVE_LINESS,TEXT("����"),
					85,DTP_GAME_LEVEL,TEXT("��Ϸ����"),
					50,DTP_USER_EXPERIENCE,TEXT("����ֵ"),
					60,DTP_WIN_RATE,TEXT("ʤ��"),
					60,DTP_FLEE_RATE,TEXT("����"),
					60,DTP_USER_PLAY_COUNT,TEXT("�ܾ�"),
					60,DTP_USER_WIN,TEXT("Ӯ��"),
					60,DTP_USER_LOST,TEXT("���"),
					60,DTP_USER_DRAW,TEXT("�;�"),
					60,DTP_USER_FLEE,TEXT("�Ӿ�"),
					200,DTP_UNDER_WRITE,TEXT("����ǩ��"),
			};

			//���ñ���
			m_ColumnInfo.wColumnCount=CountArray(ColumnItem);
			CopyMemory(m_ColumnInfo.ColumnItem,ColumnItem,sizeof(ColumnItem));

			break;
		}
	case GAME_GENRE_MATCH:	//��������
		{
			//��������
			tagColumnItem ColumnItem[]=
			{
				115,DTP_USER_ACCOUNTS,TEXT("�û���"),
					60,DTP_GAME_ID,TEXT("ID ����"),
					60,DTP_USER_SCORE,TEXT("����"),
					40,DTP_USER_TABLE,TEXT("����"),
					60,DTP_LOVE_LINESS,TEXT("����"),
					85,DTP_GAME_LEVEL,TEXT("��Ϸ����"),
					50,DTP_USER_EXPERIENCE,TEXT("����ֵ"),
					60,DTP_WIN_RATE,TEXT("ʤ��"),
					60,DTP_FLEE_RATE,TEXT("����"),
					60,DTP_USER_PLAY_COUNT,TEXT("�ܾ�"),
					60,DTP_USER_WIN,TEXT("Ӯ��"),
					60,DTP_USER_LOST,TEXT("���"),
					60,DTP_USER_DRAW,TEXT("�;�"),
					60,DTP_USER_FLEE,TEXT("�Ӿ�"),
					200,DTP_UNDER_WRITE,TEXT("����ǩ��"),
			};

			//���ñ���
			m_ColumnInfo.wColumnCount=CountArray(ColumnItem);
			CopyMemory(m_ColumnInfo.ColumnItem,ColumnItem,sizeof(ColumnItem));

			break;
		}
	default:
		{
			//��������
			tagColumnItem ColumnItem[]=
			{
				115,DTP_USER_ACCOUNTS,TEXT("�û���"),
					60,DTP_GAME_ID,TEXT("ID ����"),
					60,DTP_USER_SCORE,TEXT("����"),
					40,DTP_USER_TABLE,TEXT("����"),
					60,DTP_LOVE_LINESS,TEXT("����"),
					85,DTP_GAME_LEVEL,TEXT("��Ϸ����"),
					50,DTP_USER_EXPERIENCE,TEXT("����ֵ"),
					60,DTP_WIN_RATE,TEXT("ʤ��"),
					60,DTP_FLEE_RATE,TEXT("����"),
					60,DTP_USER_PLAY_COUNT,TEXT("�ܾ�"),
					60,DTP_USER_WIN,TEXT("Ӯ��"),
					60,DTP_USER_LOST,TEXT("���"),
					60,DTP_USER_DRAW,TEXT("�;�"),
					60,DTP_USER_FLEE,TEXT("�Ӿ�"),
					200,DTP_UNDER_WRITE,TEXT("����ǩ��"),
			};

			//���ñ���
			m_ColumnInfo.wColumnCount=CountArray(ColumnItem);
			CopyMemory(m_ColumnInfo.ColumnItem,ColumnItem,sizeof(ColumnItem));

			break;
		}
	}

	//�����б�
	WORD wColumnInfoHandSize=sizeof(m_ColumnInfo)-sizeof(m_ColumnInfo.ColumnItem);
	m_wColumnInfoSize=wColumnInfoHandSize+m_ColumnInfo.wColumnCount*sizeof(m_ColumnInfo.ColumnItem[0]);

	//�������
	srand((DWORD)time(NULL));

	//�������ķ�����
	m_pITCPSocketCorrespond->Connect(ntohl(m_dwCenterServer),PORT_CENTER_SERVER);

	//ϵͳ��Ϣ
	m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_MESSAGE,300000L,(DWORD)(-1),0);

	//��������
	if (m_AndroidUserManager.StartService()==false) return false;

	//�����û�
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_ANDROID,0,NULL,0);

	//���ص���
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_PROPERTY, 0,NULL, 0);

	//�����û�
	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_ANDROID,TIME_DISTRIBUTE_ANDROID*1000L,(DWORD)(-1),NULL);

	return true;
}

//����ģ��ر�
bool __cdecl CAttemperEngineSink::OnAttemperEngineStop(IUnknownEx * pIUnknownEx)
{
	//ע������
	CMD_CS_UnRegGameServer UnRegGameServer;
	memset(&UnRegGameServer,0,sizeof(UnRegGameServer));
	UnRegGameServer.wKindID=m_pGameServiceAttrib->wKindID;
	UnRegGameServer.wServerID=m_pGameServiceOption->wServerID;
	m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_MANAGER,SUB_CS_UNREG_GAME_SERVER,&UnRegGameServer,sizeof(UnRegGameServer));

	//�ر����
	m_AndroidUserManager.StopService();
	m_ServerUserManager.ResetUserManager();

	//ɾ������
	m_wMaxUserItem=0;
	SafeDeleteArray(m_pTableFrame);
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//���Ʊ���
	m_bShallClose=false;
	m_bAllowWisper=true;
	m_bAllowRoomChat=true;
	m_bAllowGameChat=true;
	m_bAllowEnterRoom=true;
	m_bAllowEnterGame=true;

	//��Ϣ����
	m_cbRoomMsgIndex=0;
	m_cbGameMsgIndex=0;

	//���ñ���
	m_wColumnInfoSize=0;
	m_wOnLineInfoSize=0;
	memset(&m_ColumnInfo,0,sizeof(m_ColumnInfo));
	memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));

	return true;
}

//����״̬
bool __cdecl CAttemperEngineSink::SendTableStatus(WORD wTableID)
{
	//Ч�����
	ASSERT(wTableID<m_pGameServiceOption->wTableCount);
	if (wTableID>=m_pGameServiceOption->wTableCount) return false;

	//�������
	CMD_GR_TableStatus TableStatus;
	memset(&TableStatus,0,sizeof(TableStatus));
	TableStatus.wTableID=wTableID;
	TableStatus.bTableLock=(m_pTableFrame+wTableID)->IsTableLocked()?TRUE:FALSE;
	TableStatus.bPlayStatus=(m_pTableFrame+wTableID)->IsGameStarted()?TRUE:FALSE;

	//��������
	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));
	m_AndroidUserManager.SendDataToClient(MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));

	return true;
}

//���ͷ���
bool __cdecl CAttemperEngineSink::SendUserScore(IServerUserItem * pIServerUserItem)
{
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//��������
	CMD_GR_UserScore UserScore;
	UserScore.dwUserID=pUserData->dwUserID;
	UserScore.UserScore=pUserData->UserScoreInfo;
	UserScore.lLoveliness = pUserData->lLoveliness;
	UserScore.UserScore.lScore=pUserData->UserScoreInfo.lScore+pUserData->lStorageScore;

	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));
	m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));

	return true;
}

//����״̬
bool __cdecl CAttemperEngineSink::SendUserStatus(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->IsActive()==true);
	if (pIServerUserItem->IsActive()==false) return false;

	//��������
	CMD_GR_UserStatus UserStatus;
	memset(&UserStatus,0,sizeof(UserStatus));
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//��������
	UserStatus.dwUserID=pUserData->dwUserID;
	UserStatus.wTableID=pUserData->wTableID;
	UserStatus.wChairID=pUserData->wChairID;
	UserStatus.cbUserStatus=pUserData->cbUserStatus;

	//��������
	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));

	return true;
}

//���͵���
bool __cdecl CAttemperEngineSink::SendResidualProperty(IServerUserItem * pIServerUserItem)
{
	//��������
	CMD_GF_ResidualProperty ResidualProperty;
	ZeroMemory(&ResidualProperty, sizeof(ResidualProperty));
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();

	//�Ϸ��ж�
	ASSERT(PROPERTY_COUNT <= CountArray(ResidualProperty.dwResidualTime));

	//�������
	for ( WORD wPropertyID = 0; wPropertyID < PROPERTY_COUNT; ++wPropertyID )
	{
		DWORD dwResidualTime = 0;
		if ( pServerUserData->PropertyInfo[wPropertyID].dwPropCount < pServerUserData->PropertyInfo[wPropertyID].dwConsumedCount)
			dwResidualTime = 0;
		else
            dwResidualTime = pServerUserData->PropertyInfo[wPropertyID].dwPropCount - pServerUserData->PropertyInfo[wPropertyID].dwConsumedCount;

		ResidualProperty.dwResidualTime[wPropertyID] = dwResidualTime;
	}

	//������Ϣ
	SendData(pIServerUserItem, MDM_GF_PRESENT, SUB_GF_RESIDUAL_PROPERTY, &ResidualProperty, sizeof(ResidualProperty));

	return true;
}

//ɾ���û�
bool __cdecl CAttemperEngineSink::DeleteUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsActive()==true));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->IsActive()==false)) return false;

	//Ч��״̬
	ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//��ȡ�û�
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();

	//д�����
	DBR_GR_WriteProperty WriteProperty;
	ZeroMemory(&WriteProperty, sizeof(WriteProperty));
	WriteProperty.dwUserID = pIServerUserItem->GetUserID();
	WriteProperty.dwClientIP = pIServerUserItem->GetClientIP();
	for ( int nIdex = 0; nIdex < PROPERTY_COUNT; ++nIdex )
	{
		//��Ա��
		if ( PROP_MEMBER_1 <= nIdex && nIdex <= PROP_MEMBER_8 ) continue;

		if ( 0 < pServerUserData->PropertyInfo[nIdex].dwPropCount )
		{
			LONG lLeftTime = pServerUserData->PropertyInfo[nIdex].dwPropCount - pServerUserData->PropertyInfo[nIdex].dwConsumedCount;
			if ( lLeftTime < 0 ) lLeftTime = 0;
			WriteProperty.lUseableTime = lLeftTime;
			WriteProperty.nPropertyID = nIdex;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_WRITE_PROPERTY, 0, &WriteProperty, sizeof(WriteProperty));
		}
	}

	//д�����
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//���ñ���
	LeaveGameServer.dwUserID=pIServerUserItem->GetUserID();
	LeaveGameServer.dwClientIP=pIServerUserItem->GetClientIP();
	pIServerUserItem->GetUserScoreInfo(LeaveGameServer.ScoreModifyInfo);//

	//������Ϣ
	LeaveGameServer.lLoveliness = pIServerUserItem->GetUserData()->lLoveliness;
	LeaveGameServer.lRevenue=pIServerUserItem->GetUserRevenue();
	LeaveGameServer.dwPlayTimeCount=pIServerUserItem->GetPlayTimeCount();
	LeaveGameServer.dwOnlineTimeCount=pIServerUserItem->GetOnlineTimeCount();

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0,&LeaveGameServer,sizeof(LeaveGameServer));

	//����״̬
	CMD_GR_UserStatus UserStatus;
	ZeroMemory(&UserStatus,sizeof(UserStatus));
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	UserStatus.dwUserID=pIServerUserItem->GetUserID();
	UserStatus.wTableID=INVALID_TABLE;
	UserStatus.wChairID=INVALID_CHAIR;
	UserStatus.cbUserStatus=US_NULL;
	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));

	//��������
	WORD wBindIndex=pIServerUserItem->GetUserIndex();
	if (wBindIndex<m_pGameServiceOption->wMaxConnect)
	{
		(m_pNormalParameter+wBindIndex)->pIServerUserItem=NULL;
	}
	else if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		(m_pAndroidParameter+(wBindIndex-INDEX_ANDROID))->pIServerUserItem=NULL;
	}

	//ɾ���û�
	m_ServerUserManager.DeleteUserItem(pIServerUserItem);

	return true;
}

//��������
bool __cdecl CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->IsActive()==true);
	if (pIServerUserItem->IsActive()==false) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		if (wIndex>=INDEX_ANDROID)
		{
			DWORD dwID=pConnectItemInfo->dwSocketID;
			m_AndroidUserManager.SendDataToClient(dwID,wMainCmdID,wSubCmdID,NULL,0);
		}
		else
		{
			m_pITCPNetworkEngine->SendData(pConnectItemInfo->dwSocketID,wMainCmdID,wSubCmdID);
		}
	}

	return true;
}

//��������
bool __cdecl CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->IsActive()==true);
	if (pIServerUserItem->IsActive()==false) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		if (wIndex>=INDEX_ANDROID)
		{
			DWORD dwID=pConnectItemInfo->dwSocketID;
			m_AndroidUserManager.SendDataToClient(dwID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else
		{
			m_pITCPNetworkEngine->SendData(pConnectItemInfo->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//���ö�ʱ��
bool __cdecl CAttemperEngineSink::SetTableTimer(WORD wTableID, DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)
{
	//Ч�����
	ASSERT(dwTimerID<RANGE_TABLE_TIME);
	ASSERT(wTableID<m_pGameServiceOption->wTableCount);

	//���ö�ʱ��
	if (wTableID>=m_pGameServiceOption->wTableCount) return false;
	DWORD dwTableTimeID=IDI_TABLE_FRAME_START+wTableID*RANGE_TABLE_TIME+dwTimerID;
	return m_pITimerEngine->SetTimer(dwTableTimeID,dwElapse,dwRepeat,wBindParam);
}

//ɾ����ʱ��
bool __cdecl CAttemperEngineSink::KillTableTimer(WORD wTableID, DWORD dwTimerID)
{
	//Ч�����
	ASSERT(dwTimerID<RANGE_TABLE_TIME);
	ASSERT(wTableID<m_pGameServiceOption->wTableCount);

	//ɾ����ʱ��
	if (wTableID>=m_pGameServiceOption->wTableCount) return false;
	DWORD dwTableTimeID=IDI_TABLE_FRAME_START+wTableID*RANGE_TABLE_TIME+dwTimerID;
	return m_pITimerEngine->KillTimer(dwTableTimeID);
}

//�����Ϣ
void __cdecl CAttemperEngineSink::ExportInformation(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	CTraceService::TraceString(pszString,TraceLevel);
	return;
}

//���͵�¼ʧ��
bool CAttemperEngineSink::SendLogonFailed(DWORD dwSocketID, LPCTSTR pszMessage, LONG lErrorCode)
{
	//��������
	CMD_GR_LogonError LogonError;
	LogonError.lErrorCode=lErrorCode;
	lstrcpyn(LogonError.szErrorDescribe,pszMessage,sizeof(LogonError.szErrorDescribe));
	WORD wDescribeSize=CountStringBuffer(LogonError.szErrorDescribe);

	//��������
	WORD wSendSize=sizeof(LogonError)-sizeof(LogonError.szErrorDescribe)+wDescribeSize;
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_ERROR,&LogonError,wSendSize);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_ERROR,&LogonError,wSendSize);

	return true;
}

//���͵�����Ϣ
bool CAttemperEngineSink::SendProMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType)
{
	//tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwSocketID));
	//IServerUserItem *pIServerUserItem = pConnectItemInfo->pIServerUserItem;
	//if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
	//	SendGameMessage(dwSocketID, lpszMessage, wMessageType);
	//else SendRoomMessage(dwSocketID, lpszMessage, wMessageType);

	//�жϳ���
	if(m_cbSendLocation==LOCATION_GAME_ROOM)
		SendGameMessage(dwSocketID, lpszMessage, wMessageType);
	else SendRoomMessage(dwSocketID, lpszMessage, wMessageType);

	return true;
}

//���ͷ�����Ϣ
bool CAttemperEngineSink::SendRoomMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType)
{
	//�������ݰ�
	CMD_GR_Message Message;
	Message.wMessageType=wMessageType;
	lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
	Message.wMessageLength=CountStringBuffer(Message.szContent);

	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	if (LOWORD(dwSocketID)==INVALID_WORD)
	{
		m_pITCPNetworkEngine->SendDataBatch(MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);
		m_AndroidUserManager.SendDataToClient(MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);
	}
	else
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);
		}
		else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);
	}

	return true;
}

//������Ϸ��Ϣ
bool CAttemperEngineSink::SendGameMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType)
{
	//�������ݰ�
	CMD_GF_Message Message;
	Message.wMessageType=wMessageType;
	lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
	Message.wMessageLength=CountStringBuffer(Message.szContent);

	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	if (LOWORD(dwSocketID)==INVALID_WORD)
	{
		m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);
		m_AndroidUserManager.SendDataToClient(MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);
	}
	else
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);
		}
		else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);
	}

	return true;
}

//���͵�����Ϣ
bool CAttemperEngineSink::SendProMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	//Ч���û�
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		SendProMessage(pConnectItemInfo->dwSocketID,lpszMessage,wMessageType);
		return true;
	}

	return true;
}

//���ͷ�����Ϣ
bool CAttemperEngineSink::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	//Ч���û�
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		SendRoomMessage(pConnectItemInfo->dwSocketID,lpszMessage,wMessageType);
		return true;
	}

	return false;
}

//������Ϸ��Ϣ
bool CAttemperEngineSink::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	//Ч���û�
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û�����
	if ((pIServerUserItem->GetTableID()==INVALID_TABLE)||(pIServerUserItem->GetChairID()==INVALID_CHAIR)) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		SendGameMessage(pConnectItemInfo->dwSocketID,lpszMessage,wMessageType);

		return true;
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	//�ں˶�ʱ��
	if ((dwTimerID>=IDI_ATTEMPER_START)&&(dwTimerID<=IDI_ATTEMPER_CONCLUDE))
	{
		switch (dwTimerID)
		{
		case IDI_CONNECT_CENTER_SERVER:		//���ӷ�����
			{
				//��������
				m_pITCPSocketCorrespond->Connect(ntohl(m_dwCenterServer),PORT_CENTER_SERVER);

				//������ʾ
				CTraceService::TraceString(TEXT("���ڳ��������������ķ�����...."),TraceLevel_Normal);

				return true;
			}
		case IDI_UPDATE_SERVER_LIST:		//�����б�
			{
				m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);
				return true;
			}
		case IDI_UPDATE_ONLINE_COUNT:		//��������
			{
				//��������
				CMD_CS_ServerOnLineCount ServerOnLineCount;
				ServerOnLineCount.wKindID=m_pGameServiceOption->wKindID;
				ServerOnLineCount.wServerID=m_pGameServiceOption->wServerID;
				ServerOnLineCount.dwOnLineCount=(m_ServerUserManager.GetOnLineCount()+m_ServerUserManager.GetOffLineCount());
				m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_MANAGER,SUB_CS_SERVER_ONLINE_COUNT,&ServerOnLineCount,sizeof(ServerOnLineCount));

				return true;
			}
		case IDI_SEND_SYSTEM_MESSAGE:		//ϵͳ��Ϣ
			{
				//��ȡĿ¼
				TCHAR szPath[MAX_PATH]=TEXT("");
				GetCurrentDirectory(sizeof(szPath),szPath);

				//��ȡ����
				TCHAR szFileName[MAX_PATH],szMessage[1024];
				_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\GameService.ini"),szPath);
				
				//ѭ����Ϣ
				TCHAR szRoomMsgName[64],szGameMsgName[64];
				_snprintf(szRoomMsgName,sizeof(szRoomMsgName),TEXT("RoomMessage%d"),m_cbRoomMsgIndex++);
				_snprintf(szGameMsgName,sizeof(szGameMsgName),TEXT("GameMessage%d"),m_cbGameMsgIndex++);

				//ϵͳ��Ϣ
				GetPrivateProfileString(TEXT("Message"),szRoomMsgName,TEXT(""),szMessage,sizeof(szMessage),szFileName);
				if (szMessage[0]!=0) SendRoomMessage(INVALID_WORD,szMessage,SMT_INFO);
				else m_cbRoomMsgIndex=0;

				//��Ϸ��Ϣ
				GetPrivateProfileString(TEXT("Message"),szGameMsgName,TEXT(""),szMessage,sizeof(szMessage),szFileName);
				if (szMessage[0]!=0) SendGameMessage(INVALID_WORD,szMessage,SMT_INFO);
				else m_cbGameMsgIndex=0;

				return true;
			}
		case IDI_LOAD_ANDROID_INFO:		//�����û�
			{
				//Ͷ������
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_ANDROID,0,NULL,0);

				return true;
			}
		case IDI_DISTRIBUTE_ANDROID:	//�����û�
			{
				//�û�ͳ��
				tagAndroidUserStatus AndroidUserStatus;
				WORD wAndroidCount=GetAndroidUserStatus(AndroidUserStatus);
				WORD wAndroidBusyCount=AndroidUserStatus.wPlayUserCount+AndroidUserStatus.wSitdownUserCount;

				//��������
				if (wAndroidCount>0)
				{
					//��������
					tagTableUserStatus TableUserStatus;
					ZeroMemory(&TableUserStatus,sizeof(TableUserStatus));

					//�����
					if (AndroidUserStatus.wFreeUserCount>0)
					{
						for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
						{
							//��ȡ����
							CTableFrame * pTableFrame=(m_pTableFrame+i);
							if (pTableFrame->IsGameStarted()==true) continue;

							//����״��
							WORD wUserSitCount=GetTableUserStatus(pTableFrame,TableUserStatus);

							//��Ŀ����
							if (wUserSitCount==0) continue;
							if (TableUserStatus.wTableUserCount==0) continue;

							if (wUserSitCount>=TableUserStatus.wLessUserCount) 
								continue;

							//���´���
							WORD wChairID=pTableFrame->GetNullChairID();
							if(wChairID==INVALID_CHAIR) continue ;

							IServerUserItem * pIServerUserItem=AndroidUserStatus.pServerUserFree[0];
							if (pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem,NULL)==true) break ;
						}

						//ȫ��������
						if(wAndroidBusyCount < (wAndroidCount*9)/10 && m_pGameServiceAttrib->wChairCount <= AndroidUserStatus.wFreeUserCount )
						{
							bool bSitSuccess = false ;
							for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
							{
								//��ȡ����
								CTableFrame * pTableFrame=(m_pTableFrame+i);
								if (pTableFrame->IsGameStarted()==true) continue;

								//����״��
								WORD wUserSitCount=GetTableUserStatus(pTableFrame,TableUserStatus);

								//��Ŀ����
								if (TableUserStatus.wTableUserCount>0) continue;
								if (wUserSitCount>=TableUserStatus.wLessUserCount) continue;

								//���´���
								for(BYTE wChairIndex=0; wChairIndex<pTableFrame->GetChairCount(); wChairIndex++)
								{
									//��ȡ��λ
									WORD wChairID=pTableFrame->GetNullChairID();
									if(wChairID==INVALID_CHAIR) break ;

									for(WORD wAndroidUser=0; wAndroidUser<AndroidUserStatus.wFreeUserCount; ++wAndroidUser)
									{
										IServerUserItem * pIServerUserItem=AndroidUserStatus.pServerUserFree[wAndroidUser];

										//״̬�ж�
										if(!pIServerUserItem) continue ;
										if(pIServerUserItem->GetUserStatus()>=US_SIT) continue ;
                                        
										//���´���
										if (pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem,NULL)==true) break ;
									}
								}

								//�鿴���
								GetTableUserStatus(pTableFrame,TableUserStatus);
								if(TableUserStatus.wTableAndroidCount>=TableUserStatus.wLessUserCount) return true;
							}
						}
					}

					//��������
					if (AndroidUserStatus.wSitdownUserCount>0)
					{
						for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
						{
							//��ȡ����
							CTableFrame * pTableFrame=(m_pTableFrame+i);
							if (pTableFrame->IsGameStarted()==true) continue;

							//����״��
							GetTableUserStatus(pTableFrame,TableUserStatus);

							//�û�����
							if (TableUserStatus.wTableUserCount>0) continue;
							if (TableUserStatus.wTableAndroidCount==0) continue;

							//��������
							for (WORD j=0;j<pTableFrame->GetChairCount();j++)
							{
								IServerUserItem * pIServerUserItem=pTableFrame->GetServerUserItem(j);
								if (pIServerUserItem==NULL) continue;
								if(pIServerUserItem->IsAndroidUser()==false) continue ;
								if (pTableFrame->PerformStandUpAction(pIServerUserItem)==true) return true;
							}
						}
					}
				}

				return true;
			}
		}

		return false;
	}

	//������ʱ��
	if ((dwTimerID>=IDI_ANDROID_START)&&(dwTimerID<=IDI_ANDROID_CONCLUDE))
	{
		//ʱ�䴦��
		m_AndroidUserManager.OnEventTimerPulse(dwTimerID,wBindParam);

		return false;
	}

	//���Ӷ�ʱ��
	if (dwTimerID>=IDI_TABLE_FRAME_START)
	{
		//��������
		WORD wTableID=(WORD)((dwTimerID-IDI_TABLE_FRAME_START)/RANGE_TABLE_TIME);
		DWORD dwTableTimeID=(DWORD)((dwTimerID-IDI_TABLE_FRAME_START)%RANGE_TABLE_TIME);

		//��ʱ������
		ASSERT(wTableID<m_pGameServiceOption->wTableCount);
		return (m_pTableFrame+wTableID)->OnEventTimer(dwTableTimeID,wBindParam);
	}

	return false;
}

//���ݿ��¼�
bool __cdecl CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GR_LOGON_SUCCESS:		//��¼�ɹ�
		{
			return OnDBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOGON_ERROR:		//��¼ʧ��
		{
			return OnDBLogonError(dwContextID,pData,wDataSize);
		}
	case DBR_GR_ANDROID_USER:		//������Ϣ
		{
			return OnDBAndroidUser(dwContextID,pData,wDataSize);
		}
	case DBR_GR_USER_RIGHT_RESULT:	//�û�Ȩ��
		{
			return OnDBUserRightResult(dwContextID,pData,wDataSize);
		}
	case DBR_GR_PROPERTY_ATTRIBUTE:	//���ص���
		{
			return OnDBLoadProperty(dwContextID,pData,wDataSize);
		}
	case DBR_GR_FLOWER_ATTRIBUTE:	//�����ʻ�
		{
			return OnDBLoadFlower(dwContextID,pData,wDataSize);
		}
	case DBR_GR_USER_PROPERTY:		//��ҵ���
		{
			return OnDBUserProperty(dwContextID,pData,wDataSize);
		}
	//case DBR_GR_EXCHANGE_RESULT:	//�һ����
	//	{
	//		return OnDBExchangeCharmResult(dwContextID,pData,wDataSize);
	//	}
	case DBR_GR_LOAD_PROP_FINISHI:	//��ɼ���
		{
			return OnDBLadPropFinish(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//�û���¼�ɹ�
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_LogonSuccess));
	if (wDataSize!=sizeof(DBR_GR_LogonSuccess)) return false;

	//�ж�����
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwContextID));
	if (pConnectItemInfo==NULL) return true;
	if (pConnectItemInfo->dwSocketID!=dwContextID) return true;
	ASSERT(pConnectItemInfo->pIServerUserItem==NULL);

	//��Ϣ����
	DBR_GR_LogonSuccess * pLogonSuccess=(DBR_GR_LogonSuccess *)pData;

	//�����ж�
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchOnLineUser(pLogonSuccess->dwUserID);
	if (pIServerUserItem!=NULL) return false;

	//��������
	WORD wIndex=LOWORD(dwContextID);

	//״̬�ж�
	if ((m_bAllowEnterRoom==false)&&(pLogonSuccess->dwMasterRight==0L))
	{
		//������Ϣ
		LPCTSTR pszMessage=TEXT("��Ǹ������ϵͳά����ԭ�򣬱���Ϸ���䲻�����κ���ҵ�¼���룡");
		SendRoomMessage(dwContextID,pszMessage,SMT_EJECT|SMT_INFO|SMT_INTERMIT_LINE|SMT_CLOSE_ROOM);

		//�������
		DBR_GR_LeaveGameServer LeaveGameServer;
		ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));
		LeaveGameServer.dwUserID=pLogonSuccess->dwUserID;
		LeaveGameServer.dwClientIP=pConnectItemInfo->dwClientIP;
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0,&LeaveGameServer,sizeof(LeaveGameServer));

		return true;
	}

	//�ж���Ŀ
	DWORD dwUserCount=m_ServerUserManager.GetOnLineCount()+m_ServerUserManager.GetOffLineCount();
	if ((pLogonSuccess->cbMemberOrder==0)&&(pLogonSuccess->cbMasterOrder==0)&&((dwUserCount+LESS_CONNECT_COUNT)>=m_pGameServiceOption->wMaxConnect))
	{
		//������Ϣ
		LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ�����Ѿ����ˣ���ֹ�ǻ�Ա��ҵ�¼���룡");
		SendRoomMessage(dwContextID,pszMessage,SMT_EJECT|SMT_INFO|SMT_INTERMIT_LINE|SMT_CLOSE_ROOM);

		//�������
		DBR_GR_LeaveGameServer LeaveGameServer;
		ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));
		LeaveGameServer.dwUserID=pLogonSuccess->dwUserID;
		LeaveGameServer.dwClientIP=pConnectItemInfo->dwClientIP;
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0,&LeaveGameServer,sizeof(LeaveGameServer));

		return true;
	}

	//�����û���Ϣ
	tagServerUserData ServerUserData;
	memset(&ServerUserData,0,sizeof(ServerUserData));
	ServerUserData.wFaceID=pLogonSuccess->wFaceID;
	ServerUserData.dwCustomFaceVer=pLogonSuccess->dwCustomFaceVer;
	ServerUserData.cbGender=pLogonSuccess->cbGender;
	ServerUserData.dwUserID=pLogonSuccess->dwUserID;
	ServerUserData.dwGameID=pLogonSuccess->dwGameID;
	ServerUserData.dwGroupID=pLogonSuccess->dwGroupID;
	ServerUserData.dwUserRight=pLogonSuccess->dwUserRight;
	ServerUserData.lLoveliness=pLogonSuccess->lLoveliness;
	ServerUserData.dwMasterRight=pLogonSuccess->dwMasterRight;
	ServerUserData.cbMemberOrder=pLogonSuccess->cbMemberOrder;
	ServerUserData.cbMasterOrder=pLogonSuccess->cbMasterOrder;
	ServerUserData.UserScoreInfo.lWinCount=pLogonSuccess->lWinCount;
	ServerUserData.UserScoreInfo.lLostCount=pLogonSuccess->lLostCount;
	ServerUserData.UserScoreInfo.lDrawCount=pLogonSuccess->lDrawCount;
	ServerUserData.UserScoreInfo.lFleeCount=pLogonSuccess->lFleeCount;
	ServerUserData.UserScoreInfo.lExperience=pLogonSuccess->lExperience;
	lstrcpyn(ServerUserData.szAccounts,pLogonSuccess->szAccounts,CountArray(ServerUserData.szAccounts));
	lstrcpyn(ServerUserData.szGroupName,pLogonSuccess->szGroupName,CountArray(ServerUserData.szGroupName));
	lstrcpyn(ServerUserData.szUnderWrite,pLogonSuccess->szUnderWrite,CountArray(ServerUserData.szUnderWrite));

	//��ȡ����
	ServerUserData.lStorageScore=0L;
	ServerUserData.UserScoreInfo.lScore=pLogonSuccess->lScore;

	//�洢����
	if ((m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)&&(m_pGameServiceOption->lRestrictScore>0L))
	{
		if (pLogonSuccess->lScore>m_pGameServiceOption->lRestrictScore)
		{
			ServerUserData.UserScoreInfo.lScore=m_pGameServiceOption->lRestrictScore;
			ServerUserData.lStorageScore=pLogonSuccess->lScore-m_pGameServiceOption->lRestrictScore;
		}
	}

	//���ѽ��
	ServerUserData.UserScoreInfo.lInsureScore=pLogonSuccess->lInsureScore;
	ServerUserData.UserScoreInfo.lGameGold=pLogonSuccess->lGameGold;

	//״̬��Ϣ
	ServerUserData.cbUserStatus=US_FREE;
	ServerUserData.wTableID=INVALID_TABLE;
	ServerUserData.wChairID=INVALID_CHAIR;

	//�����û�
	pConnectItemInfo->pIServerUserItem=m_ServerUserManager.ActiveUserItem(ServerUserData,pConnectItemInfo->dwClientIP,wIndex,pLogonSuccess->szPassWord,m_pGameServiceOption->wServerType);

	//���ͷ�����Ϣ
	SendGameServerInfo(pConnectItemInfo->pIServerUserItem,dwContextID);

	//�㲥�û���Ϣ
	SendUserItem(pConnectItemInfo->pIServerUserItem,INVALID_WORD);

	//������Ϣ
	SendRoomMessage(dwContextID,TEXT("�װ����û�����ӭ����ᱦ���飡"),SMT_INFO);

	//��ϰ�ж�
	if (m_pGameServiceOption->wServerType&GAME_GENRE_EDUCATE)
	{
		LPCTSTR pszMessage=TEXT("��ע�⣬������Ϸ��ϰ���䣬�㽫�õ�һ����Ŀ��ģ����Ϸ�ң���Ϸ�ɼ��������¼�����ݿ⣡");
		SendRoomMessage(dwContextID,pszMessage,SMT_INFO);
	}

	//�����ж�
	if (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)
	{
		LPCTSTR pszMessage=TEXT("��ע�⣬������Ϸ�������䣬�Ǳ���ѡ�ֽ������ڴ˷��������Ϸ��");
		SendRoomMessage(dwContextID,pszMessage,SMT_INFO);
	}

	//�����׷���
	if (m_pGameServiceOption->cbHideUserInfo==TRUE)
	{
		LPCTSTR pszMessage=TEXT("��ע�⣬���Ƿ�������Ϸ���䣬���� [ 1 ] �����ӽ��н�����Ϸ��");
		SendRoomMessage(dwContextID,pszMessage,SMT_INFO);
	}

	//������Ϣ
	if (m_bShallClose==true)
	{
		LPCTSTR pszMessage=TEXT("��ע�⣬������Ҫ������Ϸ����ά�����¹���������Ϸ���伴����ͣ����");
		SendRoomMessage(dwContextID,pszMessage,SMT_INFO);
	}

	//����Ⱥ��
	if (wIndex<INDEX_ANDROID) m_pITCPNetworkEngine->AllowBatchSend(dwContextID,true);

	return true;
}

//�û���¼ʧ��
bool CAttemperEngineSink::OnDBLogonError(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_LogonError));
	if (wDataSize!=sizeof(DBR_GR_LogonError)) return false;

	//�ж�����
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwContextID));
	if (pConnectItemInfo==NULL) return true;
	if (pConnectItemInfo->dwSocketID!=dwContextID) return true;

	//������Ϣ
	DBR_GR_LogonError * pLogonError=(DBR_GR_LogonError *)pData;
	pLogonError->szErrorDescribe[CountStringBuffer(pLogonError->szErrorDescribe)-1]=0;
	SendLogonFailed(dwContextID,pLogonError->szErrorDescribe,pLogonError->lErrorCode);

	//�ر�����
	if (LOWORD(dwContextID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
	}
	else
	{
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GR_AndroidUser * pAndroidUser=(DBO_GR_AndroidUser *)pData;
	WORD wHeadSize=sizeof(DBO_GR_AndroidUser)-sizeof(pAndroidUser->dwAndroidID);

	//Ч�����
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pAndroidUser->wAndroidCount*sizeof(DWORD))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pAndroidUser->wAndroidCount*sizeof(DWORD)))) return false;

	//�����û�
	if (pAndroidUser->lResultCode==0)
	{
		m_AndroidUserManager.SetAndroidStock(pAndroidUser->dwAndroidID,pAndroidUser->wAndroidCount);
	}

	//����ʱ��
	m_pITimerEngine->SetTimer(IDI_LOAD_ANDROID_INFO,TIME_LOAD_ANDROID_INFO*1000L,1,NULL);

	return true;
}

//�û�Ȩ��
bool CAttemperEngineSink::OnDBUserRightResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_UserRightResult));
	if (wDataSize!=sizeof(DBR_GR_UserRightResult)) return false;

	//��Ϣ����
	DBR_GR_UserRightResult * pUserRightResult=(DBR_GR_UserRightResult *)pData;

	//��ȡ���
	IServerUserItem * pSendIServerUserItem=m_ServerUserManager.SearchOnLineUser(pUserRightResult->dwMasterUserID);
	IServerUserItem * pRcvIServerUserItem=m_ServerUserManager.SearchOnLineUser(pUserRightResult->dwUserID);
	if(pSendIServerUserItem==NULL || pRcvIServerUserItem==NULL)return false;
	tagServerUserData * pSendUserData=pSendIServerUserItem->GetUserData();

	//�ɹ��ж�
	if (pUserRightResult->bGameSuccess || pUserRightResult->bAccountsSuccess)
	{
		//���͹���Ա
		WORD wEnumIndex=0;
		IServerUserItem * pIServerUserItemSend=NULL;
		while (true)
		{
			pIServerUserItemSend=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);
			if (pIServerUserItemSend==NULL) break;
			if (pIServerUserItemSend->GetUserData()->cbMasterOrder==FALSE) continue;

			//��������
			CMD_GR_UserRight UserRight;
			memset(&UserRight,0,sizeof(UserRight));
			UserRight.dwUserRight = pUserRightResult->dwUserRight;
			UserRight.dwUserID = pRcvIServerUserItem->GetUserData()->dwUserID;

			//��������
			SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_USER_RIGHT,&UserRight,sizeof(UserRight));
		}
	}
	else
	{
		//������Ϣ
		TCHAR szDescribe[128]=TEXT("");
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("�����û�Ȩ��ʧ�ܣ�"));
		SendGameMessage(pSendIServerUserItem,szDescribe,SMT_EJECT|SMT_INFO);		
		return true;
	}

	return true;
}

//���ص���
bool CAttemperEngineSink::OnDBLoadProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_PropertyAttribute));
	if (wDataSize!=sizeof(DBR_GR_PropertyAttribute)) return false;

	//��Ϣ����
	DBR_GR_PropertyAttribute * pPropertyAttribute=(DBR_GR_PropertyAttribute *)pData;

	tagPropertyInfo PropertyAttribute;
	ZeroMemory(&PropertyAttribute, sizeof(PropertyAttribute));
	PropertyAttribute.bNullity = false;
	PropertyAttribute.nPropertyID = pPropertyAttribute->wPropertyID;
	PropertyAttribute.cbDiscount = pPropertyAttribute->cbDiscount;
	PropertyAttribute.dwPropCount1 = pPropertyAttribute->dwPropCount1;
	PropertyAttribute.dwPropCount2 = pPropertyAttribute->dwPropCount2;
	PropertyAttribute.dwPropCount3 = pPropertyAttribute->dwPropCount3;
	PropertyAttribute.dwPropCount4 = pPropertyAttribute->dwPropCount4;
	PropertyAttribute.dwPropCount5 = pPropertyAttribute->dwPropCount5;
	PropertyAttribute.dwPropCount6 = pPropertyAttribute->dwPropCount6;
	PropertyAttribute.dwPropCount7 = pPropertyAttribute->dwPropCount7;
	PropertyAttribute.dwPropCount8 = pPropertyAttribute->dwPropCount8;
	PropertyAttribute.dwPropCount9 = pPropertyAttribute->dwPropCount9;
	PropertyAttribute.dwPropCount10 = pPropertyAttribute->dwPropCount10;

	PropertyAttribute.lPrice1 = pPropertyAttribute->lPrice1;
	PropertyAttribute.lPrice2 = pPropertyAttribute->lPrice2;
	PropertyAttribute.lPrice3 = pPropertyAttribute->lPrice3;
	PropertyAttribute.lPrice4 = pPropertyAttribute->lPrice4;
	PropertyAttribute.lPrice5 = pPropertyAttribute->lPrice5;
	PropertyAttribute.lPrice6 = pPropertyAttribute->lPrice6;
	PropertyAttribute.lPrice7 = pPropertyAttribute->lPrice7;
	PropertyAttribute.lPrice8 = pPropertyAttribute->lPrice8;
	PropertyAttribute.lPrice9 = pPropertyAttribute->lPrice9;
	PropertyAttribute.lPrice10 = pPropertyAttribute->lPrice10;

	//��������
	m_PropertyAttributeArrary.Add(PropertyAttribute);
	if(m_PropertyAttributeArrary.GetCount()==11)
	{
		UINT nCOunt = 0;
	}

	return true;
}

//�����ʻ�
bool CAttemperEngineSink::OnDBLoadFlower(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_FlowerAttribute));
	if (wDataSize!=sizeof(DBR_GR_FlowerAttribute)) return false;

	//��Ϣ����
	DBR_GR_FlowerAttribute * pFlowerAttribute=(DBR_GR_FlowerAttribute *)pData;

	tagFlowerInfo FlowerInfo;
	ZeroMemory(&FlowerInfo, sizeof(FlowerInfo));
	FlowerInfo.bNullity = false;
	FlowerInfo.cbDiscount = pFlowerAttribute->cbDiscount;
	FlowerInfo.lPrice = pFlowerAttribute->lPrice;
	FlowerInfo.lRcvUserCharm = pFlowerAttribute->lRcvUserCharm;
	FlowerInfo.lSendUserCharm = pFlowerAttribute->lSendUserCharm;
	FlowerInfo.nFlowerID = pFlowerAttribute->nFlowerID;

	m_FlowerAttributeArrary.Add(FlowerInfo);

	return true;
}

//��ҵ���
bool CAttemperEngineSink::OnDBUserProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_UserProperty));
	if (wDataSize!=sizeof(DBR_GR_UserProperty)) return false;

	//��Ϣ����
	DBR_GR_UserProperty * pUserProperty=(DBR_GR_UserProperty *)pData;

	int nPropertyID = pUserProperty->nPropertyID;
	ASSERT(nPropertyID < PROPERTY_COUNT );
	if ( nPropertyID >= PROPERTY_COUNT ) return true;

	//�������
	IServerUserItem *pServerUserItem = m_ServerUserManager.SearchOnLineUser(pUserProperty->dwUserID);
	if ( pServerUserItem == NULL ) pServerUserItem = m_ServerUserManager.SearchOffLineUser(pUserProperty->dwUserID);
	if ( pServerUserItem == NULL ) return true;

	tagServerUserData *pServerUserData = pServerUserItem->GetUserData();

	pServerUserData->PropertyInfo[nPropertyID].dwConsumedCount = 0;
	pServerUserData->PropertyInfo[nPropertyID].nPropertyID = pUserProperty->nPropertyID;
	pServerUserData->PropertyInfo[nPropertyID].dwPropCount= pUserProperty->dwCurCount;

	return true;
}

////�һ����
//bool CAttemperEngineSink::OnDBExchangeCharmResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
//{
//	//Ч�����
//	ASSERT(wDataSize==sizeof(DBR_GR_ExchangeCharmResult));
//	if (wDataSize!=sizeof(DBR_GR_ExchangeCharmResult)) return false;
//
//	//��Ϣ����
//	DBR_GR_ExchangeCharmResult * pExchangeCharmResult=(DBR_GR_ExchangeCharmResult *)pData;
//
//	IServerUserItem *pServerUserItem = m_ServerUserManager.SearchOnLineUser(pExchangeCharmResult->dwUserID);
//	
//	//����֪ͨ
//	if ( pExchangeCharmResult->cbResultCode == 0 )
//	{
//		if ( pServerUserItem == NULL ) pServerUserItem = m_ServerUserManager.SearchOffLineUser(pExchangeCharmResult->dwUserID);
//
//		if ( pServerUserItem != NULL )
//		{
//			tagServerUserData *pServerUserData = pServerUserItem->GetUserData();
//
//			ASSERT(pExchangeCharmResult->lLoveliness<= pServerUserData->lLoveliness);
//			if ( pExchangeCharmResult->lLoveliness > pServerUserData->lLoveliness) return true;
//
//			//�޸�����
//			pServerUserData->lLoveliness -= pExchangeCharmResult->lLoveliness;
//
//			//�޸��ʿ�
//			ModifyBankStorageGold(pServerUserItem, pExchangeCharmResult->lLoveliness * CHARM_EXCHANGE_RATE);
//
//			//ϵͳ��Ϣ
//			TCHAR szMessage[] = TEXT("��ϲ�㣬�һ��ɹ�");
//			SendGameMessage(pServerUserItem, szMessage, SMT_EJECT|SMT_INFO);
//		}
//	}
//	else
//	{
//		//ϵͳ��Ϣ
//		TCHAR szMessage[] = TEXT("�һ�ʧ��");
//		SendGameMessage(pServerUserItem, szMessage, SMT_EJECT|SMT_INFO);
//	}
//
//	return true;
//}

//��ɼ���
bool CAttemperEngineSink::OnDBLadPropFinish(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwContextID));
	if (pConnectItemInfo==NULL) return true;

	//Ч�����
	ASSERT(pConnectItemInfo->pIServerUserItem!=NULL);
	if ( pConnectItemInfo->pIServerUserItem==NULL) return true;

	//���͵���
	SendResidualProperty(pConnectItemInfo->pIServerUserItem);

	return true;
}

//����Ӧ���¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientIP, DWORD dwSocketID)
{
	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wBindIndex);

	//���ñ���
	pConnectItemInfo->dwSocketID=dwSocketID;
	pConnectItemInfo->dwClientIP=dwClientIP;

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:	//��¼��Ϣ
		{
			return OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GR_USER:	//�û���Ϣ
		{
			return OnSocketMainUser(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GR_MANAGER://������Ϣ
		{
			return OnSocketMainManager(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_FRAME:	//�����Ϣ
		{
			return OnSocketMainFrame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_GAME:	//��Ϸ��Ϣ
		{
			return OnSocketMainGame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_VIDEO:	//��Ƶ��Ϣ
		{
			return OnSocketMainVideo(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_PRESENT://������Ϣ
		{
			return OnSocketPresent(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_BANK:	//������Ϣ
		{
			return OnSocketBank(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//����ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�û�����
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwSocketID));
	if (pConnectItemInfo==NULL) return true;
	IServerUserItem * pIServerUserItem=pConnectItemInfo->pIServerUserItem;
	if (pIServerUserItem!=NULL)
	{
		//�ж���Ϸ
		bool bOffLine=false;
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			//��������
			CTableFrame * pTableFrame=(m_pTableFrame+pIServerUserItem->GetTableID());

			//����ע��
			WORD wChairID=pIServerUserItem->GetChairID();
			if ((pTableFrame->IsUserPlaying(pIServerUserItem)==true)&&(pTableFrame->OnUserReqOffLine(wChairID)==true))
			{
				bOffLine=m_ServerUserManager.RegOffLineUserItem(pIServerUserItem);
			}

			//���ߴ���
			if (bOffLine==true) pTableFrame->OnUserOffLine(wChairID);
			else pTableFrame->PerformStandUpAction(pIServerUserItem);
		}

		//ɾ���û�
		if (bOffLine==false) DeleteUserItem(pIServerUserItem);
	}

	//��������
	ZeroMemory(pConnectItemInfo,sizeof(tagConnectItemInfo));

	return true;
}

//���������¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//�����ж�
	if (nErrorCode!=0)
	{
		//����ʱ��
		m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

		//������ʾ
		CTraceService::TraceString(TEXT("���ķ���������ʧ�ܣ��Ժ󽫻��ٴγ���...."),TraceLevel_Warning);

		return true;
	}
	else
	{
		//������ʾ
		CTraceService::TraceString(TEXT("���ķ��������ӳɹ�"),TraceLevel_Normal);
	}

	//ע�᷿��
	CMD_CS_RegGameServer RegGameServer;
	memset(&RegGameServer,0,sizeof(RegGameServer));
	RegGameServer.GameServer.wSortID=m_pGameServiceOption->wSortID;
	RegGameServer.GameServer.wKindID=m_pGameServiceOption->wKindID;
	RegGameServer.GameServer.wServerID=m_pGameServiceOption->wServerID;
	RegGameServer.GameServer.wStationID=m_pGameServiceOption->wNodeID;
	RegGameServer.GameServer.wServerPort=m_pGameServiceOption->wServerPort;
	RegGameServer.GameServer.dwServerAddr=m_pGameServiceOption->dwServerAddr;
	RegGameServer.GameServer.dwOnLineCount=(m_ServerUserManager.GetOnLineCount()+m_ServerUserManager.GetOffLineCount());
	lstrcpyn(RegGameServer.GameServer.szServerName,m_pGameServiceOption->szGameRoomName,CountArray(RegGameServer.GameServer.szServerName));
	m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_MANAGER,SUB_CS_REG_GAME_SERVER,&RegGameServer,sizeof(RegGameServer));

	//��ȡ�б�
	m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);

	//���ö�ʱ��
	m_pITimerEngine->SetTimer(IDI_UPDATE_SERVER_LIST,TIME_UPDATE_LIST,TIMES_INFINITY,NULL);
	m_pITimerEngine->SetTimer(IDI_UPDATE_ONLINE_COUNT,TIME_UPDATE_ONLINE,TIMES_INFINITY,NULL);

	return true;
}

//���Ķ�ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_SERVER_LIST:	//�б���Ϣ
		{
			return OnCenterMainServerList(Command,pData,wDataSize);
		}
	}
	return true;
}

//���Ĺر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//��������
	m_pITimerEngine->KillTimer(IDI_UPDATE_SERVER_LIST);
	m_pITimerEngine->KillTimer(IDI_UPDATE_ONLINE_COUNT);
	m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

	//������ʾ
	CTraceService::TraceString(TEXT("���ķ����������жϣ��Ժ󽫻��ٴγ���...."),TraceLevel_Warning);

	return true;
}

//��¼��Ϣ����
bool CAttemperEngineSink::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_USERID:		//ID ��¼
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GR_LogonByUserID));
			if (wDataSize<sizeof(CMD_GR_LogonByUserID)) return false;

			//������Ϣ
			CMD_GR_LogonByUserID * pLogonByUserID=(CMD_GR_LogonByUserID *)pData;
			pLogonByUserID->szPassWord[CountArray(pLogonByUserID->szPassWord)-1]=0;

			//�ظ��ж�
			tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwSocketID));
			if (pConnectItemInfo==NULL) return false;
			IServerUserItem * pIServerUserItem=pConnectItemInfo->pIServerUserItem;
			if (pIServerUserItem!=NULL) return false;

			//�����ж�
			pIServerUserItem=m_ServerUserManager.SearchOnLineUser(pLogonByUserID->dwUserID);
			if (pIServerUserItem!=NULL)
			{
				//Ч������
				if (lstrcmp(pIServerUserItem->GetPassword(),pLogonByUserID->szPassWord)!=0)
				{
					SendLogonFailed(dwSocketID,TEXT("�ʻ������������֤���ٴγ��Ե�¼��"),0);

					if (LOWORD(dwSocketID)>=INDEX_ANDROID)
					{
						m_AndroidUserManager.DeleteAndroidUserItem(dwSocketID);
					}
					else
					{
						m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
					}

					return true;
				}

				//��������//�޸�
				WORD wSourceIndex=pIServerUserItem->GetUserIndex();
				tagConnectItemInfo * pConnectItemSource=GetBindParameter(wSourceIndex);
				tagConnectItemInfo * pConnectItemTarger=GetBindParameter(LOWORD(dwSocketID));

				//�Ͽ��û�
				if (pConnectItemSource!=NULL)
				{
					//���ñ���
					pConnectItemSource->pIServerUserItem=NULL;

					//������Ϣ
					LPCTSTR pszMessage=TEXT("��ע�⣬����ʺ�����һ�ط��������Ϸ�����ˣ��㱻���뿪��");
					SendRoomMessage(pConnectItemSource->dwSocketID,pszMessage,SMT_INFO|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

					//������Ϣ
					if (wSourceIndex>=INDEX_ANDROID)
					{
						m_AndroidUserManager.DeleteAndroidUserItem(pConnectItemSource->dwSocketID);
					}
					else
					{
						m_pITCPNetworkEngine->ShutDownSocket(pConnectItemSource->dwSocketID);
					}
				}

				//�����û�
				pConnectItemTarger->pIServerUserItem=pIServerUserItem;
				m_ServerUserManager.SwitchOnLineUserItem(pIServerUserItem,pConnectItemTarger->dwClientIP,LOWORD(dwSocketID));

				//���ͷ�����Ϣ
				SendGameServerInfo(pIServerUserItem,dwSocketID);

				//����Ⱥ��
				if (LOWORD(dwSocketID)<INDEX_ANDROID)
				{
					m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true);
				}

				return true;
			}

			//�����ж�
			pIServerUserItem=m_ServerUserManager.SearchOffLineUser(pLogonByUserID->dwUserID);
			if (pIServerUserItem!=NULL)
			{
				//Ч������
				if (lstrcmp(pIServerUserItem->GetPassword(),pLogonByUserID->szPassWord)!=0)
				{
					SendLogonFailed(dwSocketID,TEXT("�ʻ������������֤���ٴγ��Ե�¼��"),0);

					if (LOWORD(dwSocketID)>=INDEX_ANDROID)
					{
						m_AndroidUserManager.DeleteAndroidUserItem(dwSocketID);
					}
					else
					{
						m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
					}

					return true;
				}

				//�����û�
				pConnectItemInfo->pIServerUserItem=pIServerUserItem;
				m_ServerUserManager.ActiveOffLineUserItem(pIServerUserItem,pConnectItemInfo->dwClientIP,LOWORD(dwSocketID));

				//��������
				WORD wTableID=pIServerUserItem->GetTableID();
				WORD wChairID=pIServerUserItem->GetChairID();
				(m_pTableFrame+wTableID)->OnUserReConnect(wChairID);

				//���ͷ�����Ϣ
				SendGameServerInfo(pIServerUserItem,dwSocketID);

				//����Ⱥ��
				if (LOWORD(dwSocketID)<INDEX_ANDROID)
				{
					m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true);
				}

				return true;
			}

			//��������
			DBR_GR_LogonByUserID LogonByUserID;
			memset(&LogonByUserID,0,sizeof(LogonByUserID));
			LogonByUserID.dwUserID=pLogonByUserID->dwUserID;
			LogonByUserID.dwClientIP=pConnectItemInfo->dwClientIP;
			lstrcpyn(LogonByUserID.szPassWord,pLogonByUserID->szPassWord,sizeof(LogonByUserID.szPassWord));

			//��������
			VOID * pData=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonByUserID+1,wDataSize-sizeof(CMD_GR_LogonByUserID));

			//��չ��Ϣ
			while (true)
			{
				//��ȡ����
				pData=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;

				//���ݷ���
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_COMPUTER_ID:		//������ʶ
					{
						ASSERT(pData!=NULL);
						ASSERT(DataDescribe.wDataSize==sizeof(tagClientSerial));
						if (DataDescribe.wDataSize==sizeof(tagClientSerial))
						{
							BYTE * pcbByte=(BYTE *)pData;
							for (INT i=0;i<sizeof(tagClientSerial)/sizeof(BYTE);i++)
							{
								ASSERT(CountArray(LogonByUserID.szComputerID)>((i*2)+1));
								_stprintf(&LogonByUserID.szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
							}
						}

						break;
					}
				}
			}

			//Ͷ�����ݿ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOGON_BY_USERID,dwSocketID,&LogonByUserID,sizeof(LogonByUserID));

			return true;
		}
	}

	return false;
}

//�û���Ϣ����
bool CAttemperEngineSink::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_USER_SIT_REQ:		//��������
	case SUB_GR_USER_LOOKON_REQ:	//�Թ�����
		{
			//��������
			CMD_GR_UserSitReq * pUserSitReq=(CMD_GR_UserSitReq *)pData;
			WORD wHeadSize=sizeof(CMD_GR_UserSitReq)-sizeof(pUserSitReq->szTablePass);

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize<=sizeof(CMD_GR_UserSitReq)));
			if ((wDataSize<wHeadSize)||(wDataSize>sizeof(CMD_GR_UserSitReq))) return false;
			ASSERT((wHeadSize+pUserSitReq->cbPassLen)==wDataSize);
			if ((wHeadSize+pUserSitReq->cbPassLen)!=wDataSize) return false;

			//�ж�λ��
			if ((pUserSitReq->wChairID>=m_pGameServiceAttrib->wChairCount)&&(pUserSitReq->wChairID!=INVALID_CHAIR)) return false;
			if ((pUserSitReq->wTableID>=m_pGameServiceOption->wTableCount)&&(pUserSitReq->wTableID!=INVALID_TABLE)) return false;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return false;
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			//���λ��
			if ((wSubCmdID==SUB_GR_USER_SIT_REQ)&&(m_pGameServiceOption->cbHideUserInfo==TRUE))
			{
				//�뿪λ��
				if (pUserData->wTableID!=INVALID_TABLE)
				{
					CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
					if (pTableFrame->IsUserPlaying(pIServerUserItem)==true)
					{
						//��������
						CMD_GR_SitFailed SitFailed;
						memset(&SitFailed,0,sizeof(SitFailed));
						_snprintf(SitFailed.szFailedDescribe,sizeof(SitFailed.szFailedDescribe),TEXT("�����ڵ� [ %d ] ����Ϸ����Ϸ����ʱ�����뿪λ�ã�"),pUserData->wTableID+1);

						//��������
						WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
						SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);

						return true;
					}
					pTableFrame->PerformStandUpAction(pIServerUserItem);
				}

				//����λ��
				for (INT_PTR i=1;i<m_pGameServiceOption->wTableCount;i++)
				{
					//��ȡ����
					CTableFrame * pTableFrame=(m_pTableFrame+i);
					if (pTableFrame->IsGameStarted()==true) continue;

					//���´���
					WORD wChairID=pTableFrame->GetNullChairID();
					if (wChairID!=INVALID_CHAIR)
					{
						if (pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem,NULL)==true) return true;
						return true;
					}
				}

				//��������
				CMD_GR_SitFailed SitFailed;
				ZeroMemory(&SitFailed,sizeof(SitFailed));
				_snprintf(SitFailed.szFailedDescribe,sizeof(SitFailed.szFailedDescribe),TEXT("��ʱû���ܹ��������Ϸ�������Ժ�"));

				//��������
				WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);

				return true;
			}

			//�Թ�����
			if ((wSubCmdID==SUB_GR_USER_LOOKON_REQ)&&(m_pGameServiceOption->cbUnLookOnTag==TRUE))
			{
				//��������
				CMD_GR_SitFailed SitFailed;
				memset(&SitFailed,0,sizeof(SitFailed));
				_snprintf(SitFailed.szFailedDescribe,sizeof(SitFailed.szFailedDescribe),TEXT("�������������Թ���Ϸ!"),pUserData->wTableID+1);

				//��������
				WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);

				return true;
			}

			//�������
			if ((pUserData->wTableID==pUserSitReq->wTableID)&&(pUserData->wChairID==pUserSitReq->wChairID))
			{
				if ((pUserData->cbUserStatus!=US_LOOKON)&&(wSubCmdID==SUB_GR_USER_SIT_REQ)) return true;
				if ((pUserData->cbUserStatus==US_LOOKON)&&(wSubCmdID==SUB_GR_USER_LOOKON_REQ)) return true;
			}

			//�뿪λ��
			if (pUserData->wTableID!=INVALID_TABLE)
			{
				CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
				if (pTableFrame->IsUserPlaying(pIServerUserItem)==true)
				{
					//��������
					CMD_GR_SitFailed SitFailed;
					memset(&SitFailed,0,sizeof(SitFailed));
					_snprintf(SitFailed.szFailedDescribe,sizeof(SitFailed.szFailedDescribe),TEXT("�����ڵ� [ %d ] ����Ϸ����Ϸ����ʱ�����뿪λ�ã�"),pUserData->wTableID+1);

					//��������
					WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
					SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);

					return true;
				}
				pTableFrame->PerformStandUpAction(pIServerUserItem);
			}

			//����λ��
			CTableFrame * pTableFrame=(m_pTableFrame+pUserSitReq->wTableID);
			if (wSubCmdID==SUB_GR_USER_SIT_REQ)	
			{
				pTableFrame->PerformSitDownAction(pUserSitReq->wChairID,pIServerUserItem,pUserSitReq->szTablePass);
			}
			else
			{
				pTableFrame->PerformLookonAction(pUserSitReq->wChairID,pIServerUserItem,pUserSitReq->szTablePass);
			}

			return true;
		}
	case SUB_GR_USER_STANDUP_REQ:	//��������
	case SUB_GR_USER_LEFT_GAME_REQ:	//ǿ������
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return false;

			//�������
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) return true;

			//����λ��
			CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
			if ((wSubCmdID==SUB_GR_USER_STANDUP_REQ)&&(pTableFrame->IsUserPlaying(pIServerUserItem)==true))
			{
				TCHAR szMessage[256]=TEXT("");
				_snprintf(szMessage,sizeof(szMessage),TEXT("�����ڵ� [ %d ] ����Ϸ����Ϸ����ʱ�����뿪λ�ã�"),pUserData->wTableID+1);
				SendRoomMessage(dwSocketID,szMessage,SMT_EJECT);
				return true;
			}

			//�뿪λ��
			pTableFrame->PerformStandUpAction(pIServerUserItem);

			return true;
		}
	case SUB_GR_USER_CHAT:			//������Ϣ
		{
			//Ч�����
			CMD_GR_UserChat * pUserChat=(CMD_GR_UserChat *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)));
			ASSERT(wDataSize==(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength));
			if (wDataSize<(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage))) return false;
			if (wDataSize!=(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength)) return false;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return false;

			//Ѱ���û�
			IServerUserItem * pIServerUserItemRecv=NULL;
			if (pUserChat->dwTargetUserID!=0) 
			{
				pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pUserChat->dwTargetUserID);
				if (pIServerUserItemRecv==NULL) return true;
			}

			//״̬�ж�
			if (m_bAllowRoomChat==false)
			{
				LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ���䲻������д������ģ�");
				SendRoomMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
				return true;
			}

			//Ȩ���ж�
			if (CUserRight::CanRoomChat(pIServerUserItem->GetUserData()->dwUserRight)==false)
			{
				LPCTSTR pszMessage=TEXT("��Ǹ����û�д������ĵ�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��");
				SendRoomMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
				return true;
			}

			//ת����Ϣ
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_CHAT,pUserChat,wDataSize);
			m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_USER_CHAT,pUserChat,wDataSize);

			return true;
		}
	case SUB_GR_USER_WISPER:		//˽����Ϣ
		{
			//Ч�����
			CMD_GR_Wisper * pWisper=(CMD_GR_Wisper *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)));
			ASSERT(wDataSize==(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)+pWisper->wChatLength));
			if (wDataSize<(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage))) return false;
			if (wDataSize!=(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)+pWisper->wChatLength)) return false;

			//��ȡ�û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return false;
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pWisper->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//״̬�ж�
			DWORD dwMasterRightSend=pIServerUserItemSend->GetUserData()->dwMasterRight;
			DWORD dwMasterRightRecv=pIServerUserItemRecv->GetUserData()->dwMasterRight;
			if ((m_bAllowWisper==false)&&(dwMasterRightSend==0L)&&(dwMasterRightRecv==0L))
			{
				LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ���䲻�ܽ������˽�ģ�");
				SendRoomMessage(pIServerUserItemSend,pszMessage,SMT_EJECT|SMT_INFO);
				return true;
			}

			//Ȩ���ж�
			DWORD dwUserRightSend=pIServerUserItemSend->GetUserData()->dwUserRight;
			if ((dwMasterRightRecv==0L)&&(CUserRight::CanWisper(dwUserRightSend)==false))
			{
				TCHAR szMessage[256]=TEXT("����ʱû������ҷ���˽����Ϣ��Ȩ�ޣ�ֻ�������Ա˽�ģ�");
				SendRoomMessage(dwSocketID,szMessage,SMT_EJECT);
				return true;
			}

			//ת����Ϣ
			SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_USER_WISPER,pWisper,wDataSize);
			SendData(pIServerUserItemRecv,MDM_GR_USER,SUB_GR_USER_WISPER,pWisper,wDataSize);

			return true;
		}
	case SUB_GR_USER_RULE:		//�û�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_UserRule));
			if (wDataSize!=sizeof(CMD_GR_UserRule)) return false;

			//��Ϣ����
			CMD_GR_UserRule * pUserRule=(CMD_GR_UserRule *)pData;
			pUserRule->szPassword[CountArray(pUserRule->szPassword)-1]=0;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return false;

			//�������
			tagUserRule UserRule;
			ZeroMemory(&UserRule,sizeof(UserRule));
			UserRule.bLimitWin=pUserRule->bLimitWin;
			UserRule.bLimitFlee=pUserRule->bLimitFlee;
			UserRule.bLimitScore=pUserRule->bLimitScore;
			UserRule.bCheckSameIP=pUserRule->bCheckSameIP;
			UserRule.wWinRate=pUserRule->wWinRate;
			UserRule.wFleeRate=pUserRule->wFleeRate;
			UserRule.lMaxScore=pUserRule->lMaxScore;
			UserRule.lLessScore=pUserRule->lLessScore;
			if(m_pGameServiceAttrib->wChairCount<=MAX_CHAIR_NORMAL)
			{
				UserRule.bPassword=pUserRule->bPassword;
				lstrcpyn(UserRule.szPassword,pUserRule->szPassword,CountArray(UserRule.szPassword));
			}

			//���ù���
			pIServerUserItem->SetUserRule(UserRule);

			return true;
		}

	case SUB_GR_USER_INVITE_REQ:		//�����û�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_UserInviteReq));
			if (wDataSize!=sizeof(CMD_GR_UserInviteReq)) return false;

			//��Ϣ����
			CMD_GR_UserInviteReq * pUserInviteReq=(CMD_GR_UserInviteReq *)pData;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return false;

			//Ч��״̬
			if (pUserInviteReq->wTableID==INVALID_TABLE) return true;
			if (pIServerUserItem->GetTableID()!=pUserInviteReq->wTableID) return true;
			if (pIServerUserItem->GetUserStatus()==US_PLAY) return true;
			if (pIServerUserItem->GetUserStatus()==US_OFFLINE) return true;

			//Ŀ���û�
			IServerUserItem * pITargetUserItem=m_ServerUserManager.SearchOnLineUser(pUserInviteReq->dwUserID);
			if (pITargetUserItem==NULL) return true;
			if (pITargetUserItem->GetUserStatus()==US_PLAY) return true;

			//������Ϣ
			CMD_GR_UserInvite UserInvite;
			memset(&UserInvite,0,sizeof(UserInvite));
			UserInvite.wTableID=pUserInviteReq->wTableID;
			UserInvite.dwUserID=pIServerUserItem->GetUserID();
			SendData(pITargetUserItem,MDM_GR_USER,SUB_GR_USER_INVITE,&UserInvite,sizeof(UserInvite));

			return true;
		}
	}

	return true;
}

//������Ϣ����
bool CAttemperEngineSink::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��Ϣ����
	switch (wSubCmdID)
	{
	case SUB_GR_SEND_WARNING:	//������Ϣ
		{
			//Ч�����
			CMD_GR_SendWarning * pSendWarning=(CMD_GR_SendWarning *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_SendWarning)-sizeof(pSendWarning->szWarningMessage)));
			ASSERT(wDataSize==(sizeof(CMD_GR_SendWarning)-sizeof(pSendWarning->szWarningMessage)+pSendWarning->wChatLength));
			if (wDataSize<(sizeof(CMD_GR_SendWarning)-sizeof(pSendWarning->szWarningMessage))) return false;
			if (wDataSize!=(sizeof(CMD_GR_SendWarning)-sizeof(pSendWarning->szWarningMessage)+pSendWarning->wChatLength)) return false;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return false;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanSendWarning(dwMasterRight)==false) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pSendWarning->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//������Ϣ
			SendGameMessage(pIServerUserItemRecv,pSendWarning->szWarningMessage,SMT_INFO);
			SendRoomMessage(pIServerUserItemRecv,pSendWarning->szWarningMessage,SMT_INFO|SMT_EJECT);

			return true;
		}
	case SUB_GR_LOOK_USER_IP:	//�鿴��ַ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_LookUserIP));
			if (wDataSize!=sizeof(CMD_GR_LookUserIP)) return false;

			//��Ϣ����
			CMD_GR_LookUserIP * pLookUserIP=(CMD_GR_LookUserIP *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return false;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanSeeUserIP(dwMasterRight)==false) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pLookUserIP->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//��������
			DWORD dwUserIP=pIServerUserItemRecv->GetClientIP();

			//������Ϣ
			TCHAR szMessage[128]=TEXT("");
			BYTE * pcbUserIP=(BYTE *)(&dwUserIP);
			_snprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]��%d.%d.%d.%d"),pIServerUserItemRecv->GetAccounts(),
				pcbUserIP[0],pcbUserIP[1],pcbUserIP[2],pcbUserIP[3]);

			//������Ϣ
			SendRoomMessage(pIServerUserItemSend,szMessage,SMT_INFO);

			return true;
		}
	case SUB_GR_KILL_USER:		//�߳��û�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_KillUser));
			if (wDataSize!=sizeof(CMD_GR_KillUser)) return false;

			//��Ϣ����
			CMD_GR_KillUser * pKillUser=(CMD_GR_KillUser *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return false;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanCutUser(dwMasterRight)==false) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pKillUser->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//������Ϣ
			LPCTSTR szMessage=TEXT("��Ǹ����������Ա�߳����䣬�����κ����ʣ�����ϵ��Ϸ�ͷ���");
			SendRoomMessage(pIServerUserItemRecv,szMessage,SMT_INFO|SMT_EJECT|SMT_INTERMIT_LINE);

			return true;
		}
	case SUB_GR_LIMIT_ACCOUNS:	//�����ʻ�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_LimitAccounts));
			if (wDataSize!=sizeof(CMD_GR_LimitAccounts)) return false;

			//��Ϣ����
			CMD_GR_LimitAccounts * pLimitAccounts=(CMD_GR_LimitAccounts *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return false;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanForbidAccounts(dwMasterRight)==false) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pLimitAccounts->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//Ͷ�����ݿ�
			DBR_GR_LimitAccounts LimitAccounts;
			LimitAccounts.dwUserID=pLimitAccounts->dwTargetUserID;
			LimitAccounts.dwMasterUserID=pIServerUserItemSend->GetUserID();
			LimitAccounts.dwMasterClientIP=pIServerUserItemSend->GetClientIP();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LIMIT_ACCOUNTS,0,&LimitAccounts,sizeof(LimitAccounts));

			//������Ϣ
			LPCTSTR szMessage=TEXT("��Ǹ�������ʺű������ˣ������κ����ʣ�����ϵ��Ϸ�ͷ���");
			SendRoomMessage(pIServerUserItemRecv,szMessage,SMT_INFO|SMT_EJECT|SMT_INTERMIT_LINE);

			return true;
		}
	case SUB_GR_SET_USER_RIGHT:	//�û�Ȩ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_SetUserRight));
			if (wDataSize!=sizeof(CMD_GR_SetUserRight)) return false;

			//��Ϣ����
			CMD_GR_SetUserRight * pSetUserRight=(CMD_GR_SetUserRight *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return false;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pSetUserRight->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (dwMasterRight==0L) return true;

			//��ȡȨ��
			tagServerUserData * pUserData=pIServerUserItemRecv->GetUserData();

			//Ȩ������
			if (CUserRight::CanLimitPlay(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitPlayGame==TRUE) pUserData->dwUserRight|=UR_CANNOT_PLAY;
				else pUserData->dwUserRight&=~UR_CANNOT_PLAY;
			}
			if (CUserRight::CanLimitLookon(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitLookonGame==TRUE) pUserData->dwUserRight|=UR_CANNOT_LOOKON;
				else pUserData->dwUserRight&=~UR_CANNOT_LOOKON;
			}
			if (CUserRight::CanLimitWisper(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitSendWisper==TRUE) pUserData->dwUserRight|=UR_CANNOT_WISPER;
				else pUserData->dwUserRight&=~UR_CANNOT_WISPER;
			}
			if (CUserRight::CanLimitRoomChat(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitRoomChat==TRUE) pUserData->dwUserRight|=UR_CANNOT_ROOM_CHAT;
				else pUserData->dwUserRight&=~UR_CANNOT_ROOM_CHAT;
			}
			if (CUserRight::CanLimitGameChat(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitGameChat==TRUE) pUserData->dwUserRight|=UR_CANNOT_GAME_CHAT;
				else pUserData->dwUserRight&=~UR_CANNOT_GAME_CHAT;
			}

			//����Ȩ��
			DBR_GR_SetUserRight SetUserRight;
			SetUserRight.dwUserRight=pUserData->dwUserRight;
			SetUserRight.dwUserID=pSetUserRight->dwTargetUserID;
			SetUserRight.dwMasterUserID=pIServerUserItemSend->GetUserID();
			SetUserRight.dwMasterClientIP=pIServerUserItemSend->GetClientIP();
			SetUserRight.cbGame=(CUserRight::CanBindGame(dwMasterRight)==true)?TRUE:FALSE;
			SetUserRight.cbAccounts=(CUserRight::CanBindGlobal(dwMasterRight)==true)?TRUE:FALSE;

			//Ͷ������
			if ((SetUserRight.cbAccounts==TRUE)||(SetUserRight.cbGame==TRUE))
			{
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_SET_USER_RIGHT,0,&SetUserRight,sizeof(SetUserRight));
			}

			return true;
		}
	case SUB_GR_SEND_MESSAGE:	//������Ϣ
		{
			//Ч�����
			CMD_GR_SendMessage * pSendMessage=(CMD_GR_SendMessage *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)));
			ASSERT(wDataSize==(sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+pSendMessage->wChatLength));
			if (wDataSize<(sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage))) return false;
			if (wDataSize!=(sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+pSendMessage->wChatLength)) return false;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return false;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanIssueMessage(dwMasterRight)==false) return true;

			//������Ϣ
			if (pSendMessage->cbGame==TRUE) SendGameMessage(INVALID_WORD,pSendMessage->szSystemMessage,SMT_INFO);
			if (pSendMessage->cbRoom==TRUE) SendRoomMessage(INVALID_WORD,pSendMessage->szSystemMessage,SMT_INFO);

			return true;
		}
	case SUB_GR_OPTION_SERVER:	//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_OptionServer));
			if (wDataSize!=sizeof(CMD_GR_OptionServer)) return false;

			//��Ϣ����
			CMD_GR_OptionServer * pOptionServer=(CMD_GR_OptionServer *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return false;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanServerOption(dwMasterRight)==false) return true;

			//��������
			if (pOptionServer->cbOptionFlags==OSF_ROOM_CHAT)
			{
				m_bAllowRoomChat=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//��Ϸ����
			if (pOptionServer->cbOptionFlags==OSF_GAME_CHAT)
			{
				m_bAllowGameChat=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//����˽��
			if (pOptionServer->cbOptionFlags==OSF_ROOM_WISPER)
			{
				m_bAllowWisper=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//������Ϸ
			if (pOptionServer->cbOptionFlags==OSF_ENTER_GAME)
			{
				m_bAllowEnterGame=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//���뷿��
			if (pOptionServer->cbOptionFlags==OSF_ENTER_ROOM)
			{
				m_bAllowEnterRoom=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//�����ر�
			if (pOptionServer->cbOptionFlags==OSF_SHALL_CLOSE)
			{
				m_bShallClose=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//������Ϣ
			LPCTSTR pszMessage=TEXT("�������������ò���ѡ�����óɹ���");
			SendRoomMessage(pIServerUserItemSend,pszMessage,SMT_INFO);

			return true;
		}
	}

	return false;
}

//�����Ϣ����
bool CAttemperEngineSink::OnSocketMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return false;

	//�������
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
	return pTableFrame->OnEventSocketFrame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//��Ϸ��Ϣ����
bool CAttemperEngineSink::OnSocketMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return false;

	//�������
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
	return pTableFrame->OnEventSocketGame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//��Ƶ��Ϣ����
bool CAttemperEngineSink::OnSocketMainVideo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return false;

	//�������
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
	return pTableFrame->OnEventSocketVideo(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//������Ϣ����
bool CAttemperEngineSink::OnSocketPresent(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return false;

	//��Ϣ����
	bool bReturnValue = false;
	switch ( wSubCmdID )
	{
	case SUB_GF_FLOWER:					//�ʻ��¼�
		{
			CMD_GF_Gift * pTemp=(CMD_GF_Gift *)pData;
			m_cbSendLocation = pTemp->cbSendLocation;
			bReturnValue = OnEventFlower(pData,wDataSize,pIServerUserItem);
			break;
		}
	case SUB_GF_PROPERTY:				//�����¼�
		{
			CMD_GF_Property *pTemp = (CMD_GF_Property*)pData;
			m_cbSendLocation = pTemp->cbSendLocation;
			bReturnValue = OnEventProperty(pData,wDataSize,pIServerUserItem);
			break;
		}
	case SUB_GF_EXCHANGE_CHARM:			//�һ��¼�
		{
			CMD_GF_ExchangeCharm *pTemp = (CMD_GF_ExchangeCharm*)pData;
			m_cbSendLocation = pTemp->cbSendLocation;
			bReturnValue = OnEventExchangeCharm(pData,wDataSize,pIServerUserItem);
			break;
		}
	case SUB_GF_PROP_BUGLE:				//�����¼�
		{
			CMD_GF_BugleProperty *pTemp = (CMD_GF_BugleProperty*)pData;
			m_cbSendLocation = pTemp->cbSendLocation;
			bReturnValue = OnEventBugle(pData,wDataSize,pIServerUserItem);
			break;
		}
	}

	//��ʼ����
	m_cbSendLocation = 0;

	return bReturnValue;
}

//������Ϣ����
bool CAttemperEngineSink::OnSocketBank(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return false;

	//��Ϣ����
	switch ( wSubCmdID )
	{
	case SUB_GF_BANK_GET:					//��ȡ�¼�
		{
			return OnEventBankDrawoutGold(pData,wDataSize,dwSocketID);
		}
	case SUB_GF_BANK_STORAGE:				//�洢�¼�
		{
			return OnEventBankStorage(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�����û�
bool CAttemperEngineSink::SendUserItem(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);

	//��������
	BYTE cbBuffer[SOCKET_PACKET];
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//��д����
	memset(pUserInfoHead,0,sizeof(tagUserInfoHead));
	pUserInfoHead->wFaceID=pUserData->wFaceID;
	pUserInfoHead->dwCustomFaceVer=pUserData->dwCustomFaceVer;
	pUserInfoHead->wTableID=pUserData->wTableID;
	pUserInfoHead->wChairID=pUserData->wChairID;
	pUserInfoHead->cbGender=pUserData->cbGender;
	pUserInfoHead->cbUserStatus=pUserData->cbUserStatus;
	pUserInfoHead->dwUserID=pUserData->dwUserID;
	pUserInfoHead->dwGameID=pUserData->dwGameID;
	pUserInfoHead->dwGroupID=pUserData->dwGroupID;
	pUserInfoHead->dwUserRight=pUserData->dwUserRight;
	pUserInfoHead->lLoveliness=pUserData->lLoveliness;
	pUserInfoHead->dwMasterRight=pUserData->dwMasterRight;
	pUserInfoHead->cbGender=pUserData->cbGender;
	pUserInfoHead->cbMemberOrder=pUserData->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserData->cbMasterOrder;
	pUserInfoHead->UserScoreInfo=pUserData->UserScoreInfo;
	pUserInfoHead->UserScoreInfo.lScore=pUserData->UserScoreInfo.lScore+pUserData->lStorageScore;

	//��������
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));
	SendPacket.AddPacket(pUserData->szAccounts,CountStringBuffer(pUserData->szAccounts),DTP_USER_ACCOUNTS);
	if (pUserData->szUnderWrite[0]!=0) SendPacket.AddPacket(pUserData->szUnderWrite,CountStringBuffer(pUserData->szUnderWrite),DTP_UNDER_WRITE);
	if (pUserData->szGroupName[0]!=0) SendPacket.AddPacket(pUserData->szGroupName,CountStringBuffer(pUserData->szGroupName),DTP_USER_GROUP_NAME);

	//��������
	WORD wSendLength=sizeof(tagUserInfoHead)+SendPacket.GetDataSize();
	if (LOWORD(dwSocketID)!=INVALID_WORD) 
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_USER,SUB_GR_USER_COME,cbBuffer,wSendLength);
		}
		else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_USER,SUB_GR_USER_COME,cbBuffer,wSendLength);
	}
	else m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_COME,cbBuffer,wSendLength);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::SendGameServerInfo(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//���͵�¼�ɹ�
	CMD_GR_LogonSuccess LogonSuccess;
	memset(&LogonSuccess,0,sizeof(LogonSuccess));
	LogonSuccess.dwUserID=pIServerUserItem->GetUserID();

	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

	//����������Ϣ
	BYTE cbBuffer[SOCKET_PACKET];
	CMD_GR_ServerInfo * pServerInfo=(CMD_GR_ServerInfo *)cbBuffer;
	memset(pServerInfo,0,sizeof(CMD_GR_ServerInfo));
	pServerInfo->wKindID=m_pGameServiceAttrib->wKindID;
	pServerInfo->wGameGenre=m_pGameServiceOption->wServerType;
	pServerInfo->wTableCount=m_pGameServiceOption->wTableCount;
	pServerInfo->wChairCount=m_pGameServiceAttrib->wChairCount;
	pServerInfo->dwVideoAddr=m_pGameServiceOption->dwVideoServer;
	pServerInfo->cbHideUserInfo=m_pGameServiceOption->cbHideUserInfo;

	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_INFO,SUB_GR_SERVER_INFO,cbBuffer,sizeof(CMD_GR_ServerInfo));
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_INFO,SUB_GR_SERVER_INFO,cbBuffer,sizeof(CMD_GR_ServerInfo));

	//�����б����
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_INFO,SUB_GR_COLUMN_INFO,&m_ColumnInfo,m_wColumnInfoSize);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_INFO,SUB_GR_COLUMN_INFO,&m_ColumnInfo,m_wColumnInfoSize);

	//�����������
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_INFO,SUB_GR_CONFIG_FINISH,NULL,0);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_INFO,SUB_GR_CONFIG_FINISH);

	//�����Լ���Ϣ
	SendUserItem(pIServerUserItem,dwSocketID);

	//���������û�
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItemSend=NULL;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);
		if (pIServerUserItemSend==NULL) break;
		if (pIServerUserItemSend==pIServerUserItem) continue;
		SendUserItem(pIServerUserItemSend,dwSocketID);
	}

	//���Ͷ����û�
	wEnumIndex=0;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumOffLineUser(wEnumIndex++);
		if (pIServerUserItemSend==NULL) break;
		if (pIServerUserItemSend==pIServerUserItem) continue;
		SendUserItem(pIServerUserItemSend,dwSocketID);
	}

	//����������Ϣ
	CMD_GR_TableInfo TableInfo;
	TableInfo.wTableCount=m_pGameServiceOption->wTableCount;
	ASSERT(TableInfo.wTableCount<CountArray(TableInfo.TableStatus));
	for (WORD i=0;i<TableInfo.wTableCount;i++)
	{
		CTableFrame * pTableFrame=(m_pTableFrame+i);
		TableInfo.TableStatus[i].bTableLock=pTableFrame->IsTableLocked()?TRUE:FALSE;
		TableInfo.TableStatus[i].bPlayStatus=pTableFrame->IsGameStarted()?TRUE:FALSE;
	}
	WORD wSendSize=sizeof(TableInfo)-sizeof(TableInfo.TableStatus)+TableInfo.wTableCount*sizeof(TableInfo.TableStatus[0]);

	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_STATUS,SUB_GR_TABLE_INFO,&TableInfo,wSendSize);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_STATUS,SUB_GR_TABLE_INFO,&TableInfo,wSendSize);

	//�����ʻ�
	SendPresentAttribute(pIServerUserItem);

	//�������
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH);

	return true;
}

//��������
void CAttemperEngineSink::SendPresentAttribute(IServerUserItem *pIServerUserItem)
{
	//�ڴ����
	WORD wSendSize=0L;
	BYTE cbBuffer[SOCKET_PACKET];

	//��������
	for (INT_PTR nIndex =0; nIndex < m_PropertyAttributeArrary.GetCount(); ++nIndex)
	{
		if ((wSendSize+sizeof(tagPropertyInfo))>sizeof(cbBuffer))
		{
			SendData(pIServerUserItem,MDM_GF_PRESENT,SUB_GF_PROP_ATTRIBUTE,cbBuffer,wSendSize);
			wSendSize=0;
		}
		tagPropertyInfo &PropertyInfo = m_PropertyAttributeArrary[nIndex];
		CopyMemory(cbBuffer+wSendSize,&PropertyInfo,sizeof(PropertyInfo));
		wSendSize+=sizeof(PropertyInfo);
	}
	if (wSendSize>0) SendData(pIServerUserItem,MDM_GF_PRESENT,SUB_GF_PROP_ATTRIBUTE,cbBuffer,wSendSize);

	//�ʻ�����
	wSendSize=0L;
	for (INT_PTR nIndex =0; nIndex < m_FlowerAttributeArrary.GetCount(); ++nIndex)
	{
		if ((wSendSize+sizeof(tagFlowerInfo))>sizeof(cbBuffer))
		{
			SendData(pIServerUserItem,MDM_GF_PRESENT,SUB_GF_FLOWER_ATTRIBUTE,cbBuffer,wSendSize);
			wSendSize=0;
		}
		tagFlowerInfo &FlowerInfo = m_FlowerAttributeArrary[nIndex];
		CopyMemory(cbBuffer+wSendSize,&FlowerInfo,sizeof(FlowerInfo));
		wSendSize+=sizeof(FlowerInfo);
	}
	if (wSendSize>0) SendData(pIServerUserItem,MDM_GF_PRESENT,SUB_GF_FLOWER_ATTRIBUTE,cbBuffer,wSendSize);

}

//�б���Ϣ����
bool CAttemperEngineSink::OnCenterMainServerList(CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wSubCmdID)
	{
	case SUB_CS_LIST_INFO:		//�б���Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_ListInfo));
			if (wDataSize!=sizeof(CMD_CS_ListInfo)) return false;

			//��Ϣ����
			m_ServerList.ResetServerListBuffer();

			return true;
		}
	case SUB_CS_LIST_TYPE:		//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameType));
			ASSERT(wDataSize%sizeof(tagGameType)==0);

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameType);
			tagGameType * pGameType=(tagGameType *)pData;
			m_ServerList.AppendGameTypeBuffer(pGameType,dwCount);

			return true;
		}
	case SUB_CS_LIST_KIND:		//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameKind));
			ASSERT(wDataSize%sizeof(tagGameKind)==0);

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameKind);
			tagGameKind * pGameKind=(tagGameKind *)pData;
			m_ServerList.AppendGameKindBuffer(pGameKind,dwCount);

			return true;
		}
	case SUB_CS_LIST_STATION:	//վ����Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameStation));
			ASSERT(wDataSize%sizeof(tagGameStation)==0);

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameStation);
			tagGameStation * pGameStation=(tagGameStation *)pData;
			m_ServerList.AppendGameStationBuffer(pGameStation,dwCount);

			return true;
		}
	case SUB_CS_LIST_SERVER:	//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameServer));
			ASSERT(wDataSize%sizeof(tagGameServer)==0);

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;
			m_ServerList.AppendGameServerBuffer(pGameServer,dwCount);

			return true;
		}
	case SUB_CS_LIST_FINISH:	//�������
		{
			//�����
			m_ServerList.ActiveServerListBuffer();

			//��������
			WORD wInfoCount=m_ServerList.GetOnLineCountInfo(m_OnLineCountInfo,CountArray(m_OnLineCountInfo));
			m_wOnLineInfoSize=wInfoCount*sizeof(tagOnLineCountInfo);

			//������Ϣ
			if ((m_wOnLineInfoSize>0)&&(m_ServerUserManager.GetOnLineCount()>0))
			{
				m_pITCPNetworkEngine->SendDataBatch(MDM_GR_SERVER_INFO,SUB_GR_ONLINE_COUNT_INFO,&m_OnLineCountInfo,m_wOnLineInfoSize);
				m_AndroidUserManager.SendDataToClient(MDM_GR_SERVER_INFO,SUB_GR_ONLINE_COUNT_INFO,&m_OnLineCountInfo,m_wOnLineInfoSize);
			}

			return true;
		}
	}

	return true;
}

//�󶨲���
tagConnectItemInfo * CAttemperEngineSink::GetBindParameter(WORD wBindIndex)
{
	//��Ч����
	if (wBindIndex==INVALID_WORD) return NULL;

	//��������
	if (wBindIndex<m_pGameServiceOption->wMaxConnect)
	{
		return m_pNormalParameter+wBindIndex;
	}

	//��������
	if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		return m_pAndroidParameter+(wBindIndex-INDEX_ANDROID);
	}

	//�������
	ASSERT(FALSE);

	return NULL;
}

//���û�
IServerUserItem * CAttemperEngineSink::GetServerUserItem(WORD wBindIndex)
{
	//��ȡ����
	tagConnectItemInfo * pBindParameter=GetBindParameter(wBindIndex);

	//��ȡ�û�
	if (pBindParameter!=NULL)
	{
		return pBindParameter->pIServerUserItem;
	}

	//�������
	ASSERT(FALSE);

	return NULL;
}

//�û�״��
WORD CAttemperEngineSink::GetAndroidUserStatus(tagAndroidUserStatus & AndroidUserStatus)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//���ñ���
	ZeroMemory(&AndroidUserStatus,sizeof(AndroidUserStatus));

	//�û���Ϣ
	do
	{
		//��ȡ�û�
		pIServerUserItem=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);

		//ͳ���ж�
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser()==true))
		{
			//��ȡ����
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//�û�ͳ��
			switch (pIServerUserItem->GetUserStatus())
			{
			case US_FREE:
				{
					ASSERT(AndroidUserStatus.wFreeUserCount<CountArray(AndroidUserStatus.pServerUserFree));
					AndroidUserStatus.pServerUserFree[AndroidUserStatus.wFreeUserCount++]=pIServerUserItem;
					break;
				}
			case US_PLAY:
				{
					ASSERT(AndroidUserStatus.wPlayUserCount<CountArray(AndroidUserStatus.pServerUserPlay));
					AndroidUserStatus.pServerUserPlay[AndroidUserStatus.wPlayUserCount++]=pIServerUserItem;
					break;
				}
			case US_SIT:
			case US_READY:
				{
					ASSERT(AndroidUserStatus.wSitdownUserCount<CountArray(AndroidUserStatus.pServerUserSitdown));
					AndroidUserStatus.pServerUserSitdown[AndroidUserStatus.wSitdownUserCount++]=pIServerUserItem;
					break;
				}
			}
		}

	} while (pIServerUserItem!=NULL);

	return AndroidUserStatus.wFreeUserCount+AndroidUserStatus.wPlayUserCount+AndroidUserStatus.wSitdownUserCount;
}

//����״��
WORD CAttemperEngineSink::GetTableUserStatus(CTableFrame * pTableFrame, tagTableUserStatus & TableUserStatus)
{
	//���ñ���
	ZeroMemory(&TableUserStatus,sizeof(TableUserStatus));

	//�û�����
	for (WORD i=0;i<m_pGameServiceAttrib->wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=pTableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//�û�����
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			TableUserStatus.wTableUserCount++;
		}
		else
		{
			TableUserStatus.wTableAndroidCount++;
		}
	}

	//������Ŀ
	switch (pTableFrame->m_pITableFrameSink->GetGameStartMode())
	{
	case enStartMode_AllReady:	//����׼��
		{
			TableUserStatus.wLessUserCount=2;
			break;
		}
	default:					//Ĭ��ģʽ
		{
			TableUserStatus.wLessUserCount=m_pGameServiceAttrib->wChairCount;
			break;
		}
	}

	return TableUserStatus.wTableAndroidCount+TableUserStatus.wTableUserCount;
}

//�����ʻ�
bool CAttemperEngineSink::OnEventFlower(const void * pData, WORD wDataSize, IServerUserItem * pSendIServerUserItem)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GF_Gift));
	if (wDataSize!=sizeof(CMD_GF_Gift)) return false;                

	//��������
	CMD_GF_Gift * pGift=(CMD_GF_Gift *)pData;

	//Ŀ�����
	IServerUserItem *pIRcvUserItem = NULL;
	pIRcvUserItem = m_ServerUserManager.SearchOnLineUser(pGift->dwRcvUserID);
	if ( pIRcvUserItem == NULL ) pIRcvUserItem = m_ServerUserManager.SearchOffLineUser(pGift->dwRcvUserID);
	tagServerUserData *pSendUserData = pSendIServerUserItem->GetUserData();

	if ( pIRcvUserItem != NULL )
	{
		//��ȡ�ʻ�
		ASSERT(pGift->wGiftID < FLOWER_COUNT );
		if ( pGift->wGiftID >= FLOWER_COUNT ) return true;

		//�����ʻ�
		tagFlowerInfo *pFlowerInfo = NULL;
		for ( INT_PTR nIndex = 0; nIndex < m_FlowerAttributeArrary.GetCount(); ++nIndex )
		{
			tagFlowerInfo &FlowerInfo = m_FlowerAttributeArrary[nIndex];
			if ( FlowerInfo.bNullity == false && FlowerInfo.nFlowerID == pGift->wGiftID )
			{
				pFlowerInfo = &FlowerInfo;
				break;
			}
		}

		//�Ϸ��ж�
		ASSERT( pFlowerInfo != NULL );
		if ( pFlowerInfo == NULL )
		{
			SendProMessage(pSendIServerUserItem, TEXT("����ʧ�ܣ��ʻ����ش���"), SMT_EJECT|SMT_INFO);
			return true;
		}

		//��ȡ���
		tagServerUserData *pRcvUserData = pIRcvUserItem->GetUserData();

		LONG lFlowerPrice = pFlowerInfo->lPrice * pGift->wFlowerCount * (pSendUserData->cbMemberOrder > 0 ? pFlowerInfo->cbDiscount : 100) / 100;

		//��ȡ���
		LONG lInsureScore = GetBankStorageGold(pSendIServerUserItem);

		//�����ж�
		if ( lFlowerPrice <= lInsureScore )
		{
			//�ʿ�֧��
			ModifyBankStorageGold(pSendIServerUserItem, -lFlowerPrice);
		}
		else
		{
			//������Ϣ
			SendProMessage(pSendIServerUserItem,TEXT("����ʧ�ܣ��������Ҳ�����"),SMT_EJECT|SMT_INFO);
			return true;
		}

		//�޸�����
		LONG lSendUserCharm = pFlowerInfo->lSendUserCharm * pGift->wFlowerCount;
		LONG lRcvUserCharm = pFlowerInfo->lRcvUserCharm * pGift->wFlowerCount;
		if ( 0 < lSendUserCharm ) 
		{
			pSendUserData->lLoveliness += lSendUserCharm;
		}
		pRcvUserData->lLoveliness += lRcvUserCharm;

		//���ͻ���
		SendUserScore(pIRcvUserItem);
		SendUserScore(pSendIServerUserItem);

		//��Ϣ֪ͨ
		CMD_GF_GiftNotify GiftNotify;
		ZeroMemory( &GiftNotify, sizeof( GiftNotify ) );
		GiftNotify.cbSendLocation = pGift->cbSendLocation;
		GiftNotify.dwSendUserID = pSendUserData->dwUserID;
		GiftNotify.dwRcvUserID = pRcvUserData->dwUserID;
		GiftNotify.wGiftID = pGift->wGiftID;
		GiftNotify.wFlowerCount = pGift->wFlowerCount;

		//��Ϸ����
		if(pGift->cbSendLocation == LOCATION_GAME_ROOM)
		{
			ASSERT(pSendUserData->wTableID!=INVALID_TABLE);

			//���֪ͨ
			CTableFrame * pTableFrame=(m_pTableFrame+pSendUserData->wTableID);
			for ( WORD wChairID = 0; wChairID < m_pGameServiceAttrib->wChairCount; ++wChairID )
			{
				//��ȡ���
				IServerUserItem *pPlayServerUserItem = pTableFrame->GetServerUserItem( wChairID );
				if ( pPlayServerUserItem == NULL ) continue;

				//������Ϣ
				SendData( pPlayServerUserItem, MDM_GF_PRESENT, SUB_GF_FLOWER, &GiftNotify, sizeof( GiftNotify ) );
			}

			//�Թ�֪ͨ
			IServerUserItem *pLookonServerUserItem = NULL;
			WORD wIndex = 0;
			do
			{
				//��ȡ�Թ�
				pLookonServerUserItem = pTableFrame->EnumLookonUserItem( wIndex++ );

				//������Ϣ
				if ( pLookonServerUserItem != NULL ) SendData( pLookonServerUserItem, MDM_GF_PRESENT, SUB_GF_FLOWER, &GiftNotify, sizeof( GiftNotify ) );

			}while( pLookonServerUserItem != NULL );
		}
		//��������
		else
		{
			//Ⱥ����Ϣ
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItemSend=NULL;
			while (true)
			{
				pIServerUserItemSend=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);
				if (pIServerUserItemSend==NULL) break;

				//��������
				SendData( pIServerUserItemSend, MDM_GF_PRESENT, SUB_GF_FLOWER, &GiftNotify, sizeof( GiftNotify ) );
			}
		}

		//���ݿ�֪ͨ
		DBR_GR_SendGift SendGift;
		SendGift.dwSendUserID = pGift->dwSendUserID;
		SendGift.dwRcvUserID = pGift->dwRcvUserID;
		SendGift.dwClientIP = pSendIServerUserItem->GetClientIP();
		SendGift.wGiftID = pGift->wGiftID;
		SendGift.wFlowerCount = pGift->wFlowerCount;
		SendGift.lFlowerPrice = lFlowerPrice;
		SendGift.lRcvUserCharm = lSendUserCharm;
		SendGift.lSendUserCharm = lRcvUserCharm;

		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_SEND_GIFT,0,&SendGift, sizeof( SendGift ) );
	}
	else
	{
		//������Ϣ
		SendProMessage(pSendIServerUserItem,TEXT("�Է��Ѿ��뿪������ʧ�ܣ�"),SMT_EJECT|SMT_INFO);
		return true;
	}

	return true;
}

//���ߴ���
bool CAttemperEngineSink::OnEventProperty(const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GF_Property));
	if (wDataSize!=sizeof(CMD_GF_Property)) return false;     
	CMD_GF_Property *pPropertyHeadInfo = (CMD_GF_Property*)pData;

	//����I D
	int nPropertyID = pPropertyHeadInfo->nPropertyID;

	//�Ϸ��ж�
	ASSERT(nPropertyID < PROPERTY_COUNT );
	if ( nPropertyID >= PROPERTY_COUNT ) return true;

	//Ŀ�����
	IServerUserItem *pTargetUserItem = NULL;
	pTargetUserItem = m_ServerUserManager.SearchOnLineUser(pPropertyHeadInfo->dwTargetUserID);
	if ( pTargetUserItem == NULL ) pTargetUserItem = m_ServerUserManager.SearchOffLineUser(pPropertyHeadInfo->dwTargetUserID);

	//ʹ������
	if(!IsPropertyUseRight(nPropertyID,pIServerUserItem,pTargetUserItem))return true;

	//��ȡ��Ϣ
	tagPropertyInfo *pPropertyAttribute = NULL;
	for ( WORD wIndex = 0; wIndex < m_PropertyAttributeArrary.GetCount(); ++wIndex )
	{
		tagPropertyInfo *pTmpPropertyAttribute = &m_PropertyAttributeArrary[wIndex];
		if ( pTmpPropertyAttribute != NULL && pTmpPropertyAttribute->bNullity == false && pTmpPropertyAttribute->nPropertyID == nPropertyID )
		{
			pPropertyAttribute = pTmpPropertyAttribute;
			break;
		}
	}

	//�Ϸ��ж�
	ASSERT(pPropertyAttribute != NULL);
	if ( pPropertyAttribute == NULL )
	{
		SendProMessage(pIServerUserItem, TEXT("����ʧ�ܣ����߼��ش���"), SMT_EJECT|SMT_INFO);
		return true;
	}

	//�����Ϣ
	tagServerUserData *pConsumeUserData = pIServerUserItem->GetUserData();
	tagServerUserData *pTargetUserData = pTargetUserItem->GetUserData();

	//�����ж�
	int nDiscount = pConsumeUserData->cbMemberOrder > 0 ? pPropertyAttribute->cbDiscount : 100;

	//��������
	DWORD dwPropertyCount[] = {pPropertyAttribute->dwPropCount1, pPropertyAttribute->dwPropCount2, pPropertyAttribute->dwPropCount3,
		pPropertyAttribute->dwPropCount4, pPropertyAttribute->dwPropCount5, pPropertyAttribute->dwPropCount6,
		pPropertyAttribute->dwPropCount7, pPropertyAttribute->dwPropCount8, pPropertyAttribute->dwPropCount9, 
		pPropertyAttribute->dwPropCount10};
	LONG lPropertyPrice[] = {pPropertyAttribute->lPrice1, pPropertyAttribute->lPrice2, pPropertyAttribute->lPrice3,
		pPropertyAttribute->lPrice4, pPropertyAttribute->lPrice5, pPropertyAttribute->lPrice6, pPropertyAttribute->lPrice7,
		pPropertyAttribute->lPrice8, pPropertyAttribute->lPrice9, pPropertyAttribute->lPrice10};

	//�۸��ж�
	LONG lPrice = 0;
	for ( int nIndex = 0; nIndex < CountArray(dwPropertyCount); ++ nIndex )
	{
		if ( dwPropertyCount[nIndex] == pPropertyHeadInfo->dwOnceCount )
		{
			lPrice = lPropertyPrice[nIndex] * pPropertyHeadInfo->dwPachurseCount * nDiscount / 100;
			break;
		}

		//�����ж�
		ASSERT(nIndex < CountArray(dwPropertyCount) - 1);
		if ( nIndex >= CountArray(dwPropertyCount) - 1 )
		{
			SendProMessage(pIServerUserItem, TEXT("����ʧ�ܣ�������������"), SMT_EJECT|SMT_INFO);

			return true;
		}
	}

	//��ȡ���
	LONG lInsureScore = GetBankStorageGold(pIServerUserItem);

	//�����ж�
	if ( lPrice <= lInsureScore )
	{
		//�ʿ�֧��
		ModifyBankStorageGold(pIServerUserItem, -lPrice);
	}
	else
	{
		//������Ϣ
		SendProMessage(pIServerUserItem,TEXT("����ʧ�ܣ��������Ҳ�����"),SMT_EJECT|SMT_INFO);		
		return true;
	}

	//������Ŀ
	DWORD dwCurPropCount = 0;
	if ( nPropertyID != PROP_NEGAGIVE && nPropertyID != PROP_FLEE )
	{
		DWORD dwCurPropCount = pTargetUserData->PropertyInfo[nPropertyID].dwPropCount - pTargetUserData->PropertyInfo[nPropertyID].dwConsumedCount;
		if ( pTargetUserData->PropertyInfo[nPropertyID].dwPropCount < pTargetUserData->PropertyInfo[nPropertyID].dwConsumedCount) 
			dwCurPropCount = 0;
		dwCurPropCount += ( pPropertyHeadInfo->dwPachurseCount * pPropertyHeadInfo->dwOnceCount );
		pTargetUserData->PropertyInfo[nPropertyID].dwPropCount = dwCurPropCount;
		pTargetUserData->PropertyInfo[nPropertyID].dwConsumedCount = 0;

		//���͵���
		SendResidualProperty( pTargetUserItem );
	}

	//��������
	if ( nPropertyID == PROP_NEGAGIVE )
	{
		LONG lNowScore = pTargetUserData->UserScoreInfo.lScore;
		if ( lNowScore < 0)
		{
			//��������
			tagScoreInfo ScoreInfo;
			ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));

			//���ñ���
			ScoreInfo.lScore=-lNowScore;
			ScoreInfo.ScoreKind=enScoreKind_Present;

			//�޸Ļ���
			pTargetUserItem->WriteScore(ScoreInfo,0);

			//���ͻ���
			SendUserScore(pTargetUserItem);
		}
	}
	//��������
	else if ( nPropertyID == PROP_FLEE )
	{
		LONG lNowFleeCount = pTargetUserData->UserScoreInfo.lFleeCount;
		if ( 0 < lNowFleeCount )
		{
			tagUserScore *pScoreModify = pTargetUserItem->GetUserScoreModifyInfo();
			pScoreModify->lFleeCount = -lNowFleeCount;
			pTargetUserData->UserScoreInfo.lFleeCount = 0;

			//���ͻ���
			SendUserScore(pTargetUserItem);
		}
	}
	//��ԱȨ��
	else if( nPropertyID >= PROP_MEMBER_1 )
	{
		//���µȼ�
		BYTE cbMemberOrder = pTargetUserData->cbMemberOrder ;
		if(cbMemberOrder<(nPropertyID-PROP_MEMBER_1+1))
		{
			//���õȼ�
			pTargetUserData->cbMemberOrder = nPropertyID-PROP_MEMBER_1+1;

			//��������
			CMD_GR_MemberOrder MemberOrder;
			MemberOrder.dwUserID = pTargetUserData->dwUserID;
			MemberOrder.cbMemberOrder = nPropertyID-PROP_MEMBER_1+1;
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_MEMBER_ORDER,&MemberOrder,sizeof(MemberOrder));
			m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_MEMBER_ORDER,&MemberOrder,sizeof(MemberOrder));
		}
	}

	//������Ϣ
	DBR_GR_Property Property;
	ZeroMemory(&Property, sizeof(Property));

	Property.dwUserID = pIServerUserItem->GetUserID();
	Property.nPropertyID = nPropertyID;
	Property.dwCurCount = dwCurPropCount;
	Property.dwClientIP = pIServerUserItem->GetClientIP();
	Property.lPrice = lPrice;
	Property.dwOnceCount = pPropertyHeadInfo->dwOnceCount;
	Property.dwPachurseCount = pPropertyHeadInfo->dwPachurseCount;
	Property.dwTargetUserID = pTargetUserData->dwUserID;

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_BUY_PROPERTY,0,&Property, sizeof(Property));

	//������Ϣ
	if ( pConsumeUserData->dwUserID != pTargetUserData->dwUserID )
	{
		CString strRoomMsg;
		strRoomMsg.Format(TEXT("[ %s ]����[ %s ](%s)"), pConsumeUserData->szAccounts, pTargetUserData->szAccounts, 
			g_PropertyDescribe[nPropertyID].szName);
		SendProMessage(INVALID_WORD, LPCTSTR(strRoomMsg), SMT_INFO);

		CString strGameMsg;
		strGameMsg.Format(TEXT("[ %s ]������(%s)"), pConsumeUserData->szAccounts, g_PropertyDescribe[nPropertyID].szName);
		SendProMessage(pTargetUserItem, strGameMsg, SMT_INFO);
	}
	SendProMessage(pIServerUserItem, TEXT("��ϲ��������ɹ���"),SMT_EJECT|SMT_INFO);

	////�����ж�
	//if ( PROP_MEMBER_1 <= nPropertyID && nPropertyID <= PROP_MEMBER_8 )
	//{
	//	//������Ϣ
	//	CString strMsg;
	//	LONG lGold[] = {10000, 30000, 60000, 100000, 600000, 1200000};
	//	int nIndex = nPropertyID - PROP_MEMBER_1;
	//	strMsg.Format(TEXT("��ϲ�㣬����(%ld)������"), lGold[nIndex]);
	//	SendProMessage(pTargetUserItem, strMsg,SMT_INFO); 

	//	//�޸��ʿ�
	//	ModifyBankStorageGold(pTargetUserItem, lGold[nIndex]);
	//}
 
	return true;
}

//�����һ�
bool CAttemperEngineSink::OnEventExchangeCharm(const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//������֤
	ASSERT( sizeof(CMD_GF_ExchangeCharm) == wDataSize );
	if ( sizeof(CMD_GF_ExchangeCharm) != wDataSize ) return false;

	CMD_GF_ExchangeCharm *pExchangeLoveliness = (CMD_GF_ExchangeCharm*)pData;

	//�Ϸ���֤
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();
	if ( pServerUserData->lLoveliness< pExchangeLoveliness->lLoveliness ) return false;
	if ( pExchangeLoveliness->lGoldValue != pExchangeLoveliness->lLoveliness * CHARM_EXCHANGE_RATE ) return false;

	//�޸�����
	pServerUserData->lLoveliness -= pExchangeLoveliness->lLoveliness;

	//�޸��ʿ�
	ModifyBankStorageGold(pIServerUserItem, pExchangeLoveliness->lLoveliness * CHARM_EXCHANGE_RATE);

	//ϵͳ��Ϣ
	TCHAR szMessage[] = TEXT("��ϲ�㣬�һ��ɹ�");
	SendProMessage(pIServerUserItem, szMessage, SMT_EJECT|SMT_INFO);

	//���ݿ�֪ͨ
	DBR_GR_ExchangeLoveliness ExchangeLoveliness;
	ZeroMemory(&ExchangeLoveliness, sizeof(ExchangeLoveliness));
	ExchangeLoveliness.dwUserID = pServerUserData->dwUserID;
	ExchangeLoveliness.lLoveliness = pExchangeLoveliness->lLoveliness;
	ExchangeLoveliness.lGoldValue = pExchangeLoveliness->lGoldValue;
	ExchangeLoveliness.dwClientIP = pIServerUserItem->GetClientIP();

	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_EXCHANGE_CHARM,0,&ExchangeLoveliness, sizeof(ExchangeLoveliness));

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventBugle(const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//������֤
	ASSERT( sizeof(CMD_GF_BugleProperty) == wDataSize );
	if ( sizeof(CMD_GF_BugleProperty) != wDataSize ) return false;

	//����ת��
	CMD_GF_BugleProperty *pBugleProperty = (CMD_GF_BugleProperty*)pData;

	//��ȡ���
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();

	//��Ŀ�ж�
	if ( pServerUserData->PropertyInfo[PROP_BUGLE].dwPropCount <= pServerUserData->PropertyInfo[PROP_BUGLE].dwConsumedCount )
	{
		//��ȡ��Ϣ
		tagPropertyInfo *pPropertyAttribute = NULL;
		for ( WORD wIndex = 0; wIndex < m_PropertyAttributeArrary.GetCount(); ++wIndex )
		{
			tagPropertyInfo *pTmpPropertyAttribute = &m_PropertyAttributeArrary[wIndex];
			if ( pTmpPropertyAttribute != NULL && pTmpPropertyAttribute->bNullity == false && pTmpPropertyAttribute->nPropertyID == PROP_BUGLE )
			{
				pPropertyAttribute = pTmpPropertyAttribute;
				break;
			}
		}

		//�Ϸ��ж�
		ASSERT(pPropertyAttribute != NULL);
		if ( pPropertyAttribute == NULL )
		{
			SendProMessage(pIServerUserItem, TEXT("����ʧ�ܣ����߼��ش���"), SMT_EJECT|SMT_INFO);
			return true;
		}

		//��������
		CMD_GF_Property PropertyHeadInfo;
		ZeroMemory(&PropertyHeadInfo, sizeof(PropertyHeadInfo));
		PropertyHeadInfo.cbSendLocation = pBugleProperty->cbSendLocation;
		PropertyHeadInfo.nPropertyID = PROP_BUGLE;
		PropertyHeadInfo.dwSourceUserID = pServerUserData->dwUserID;
		PropertyHeadInfo.dwTargetUserID = pServerUserData->dwUserID;
		PropertyHeadInfo.dwPachurseCount = 1;
		PropertyHeadInfo.dwOnceCount = pPropertyAttribute->dwPropCount1;

		//��������
		OnEventProperty(&PropertyHeadInfo,sizeof(PropertyHeadInfo),pIServerUserItem);

		//����ɹ�
		tagServerUserData *pTargetUserData = pIServerUserItem->GetUserData();
		if(pTargetUserData->PropertyInfo[PROP_BUGLE].dwConsumedCount == 0
			&& pTargetUserData->PropertyInfo[PROP_BUGLE].dwPropCount==pPropertyAttribute->dwPropCount1)
		{
			//������Ŀ
			pServerUserData->PropertyInfo[PROP_BUGLE].dwConsumedCount++;

			//������Ŀ
			SendResidualProperty(pIServerUserItem);
		}
		else return true;
	}
	else
	{
		//������Ŀ
		pServerUserData->PropertyInfo[PROP_BUGLE].dwConsumedCount++;

		//������Ŀ
		SendResidualProperty(pIServerUserItem);
	}

	//��������
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_PRESENT,SUB_GF_PROP_BUGLE,pBugleProperty,wDataSize);
	m_AndroidUserManager.SendDataToClient(MDM_GF_PRESENT,SUB_GF_PROP_BUGLE,pBugleProperty,wDataSize);

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventBankDrawoutGold(const void * pData, WORD wDataSize, DWORD dwSocketID)
{
	//������֤
	ASSERT( sizeof(CMD_GF_BankGet) == wDataSize );
	if ( sizeof(CMD_GF_BankGet) != wDataSize ) return false;

	//��ȡ���
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	ASSERT(pIServerUserItem!=NULL);
	if(pIServerUserItem==NULL) return false;
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();

	//�ж�״̬
	if(m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
	{
		if(pServerUserData->wTableID!=INVALID_TABLE)
		{
			SendRoomMessage(dwSocketID,TEXT("���˳���Ϸ���ٽ�����ȡ����!"),SMT_EJECT);
			return true;
		}
	}

	//����ת��
	CMD_GF_BankGet *pBankGet= (CMD_GF_BankGet*)pData;
	LONG lStorageGold = pServerUserData->UserScoreInfo.lInsureScore;

	//Ч������
	ASSERT(pBankGet->lGetValue>0 && pBankGet->lGetValue<=lStorageGold);
	if(!(pBankGet->lGetValue>0 && pBankGet->lGetValue<=lStorageGold))return false;

	//����Ч��
	if (lstrcmp(pIServerUserItem->GetPassword(),pBankGet->szPassword)!=0)
	{
		SendRoomMessage(dwSocketID,TEXT("�����������֤���ٴγ��Բ�����"),SMT_EJECT);
		return true;
	}

	//�޸Ľ��
	ModifyGameGold(pIServerUserItem,pBankGet->lGetValue);

	//�޸��ʿ�
	ModifyBankStorageGold(pIServerUserItem,-pBankGet->lGetValue);
	SendRoomMessage(dwSocketID,TEXT("��ȡ�ɹ�!"),SMT_EJECT);

	//��������
	DBR_GR_BankDrawoutGold BankDrawoutGold;
	ZeroMemory(&BankDrawoutGold, sizeof(BankDrawoutGold));
	BankDrawoutGold.DrawoutGoldCount = pBankGet->lGetValue;
	BankDrawoutGold.dwUserID = pServerUserData->dwUserID;
	BankDrawoutGold.dwClientIP = pIServerUserItem->GetClientIP();

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_BANK_GET_GOLD,0,&BankDrawoutGold, sizeof(BankDrawoutGold));

	return true;
}

//�洢�¼�
bool CAttemperEngineSink::OnEventBankStorage(const void * pData, WORD wDataSize, DWORD dwSocketID)
{
	//������֤
	ASSERT( sizeof(CMD_GF_BankStorage) == wDataSize );
	if ( sizeof(CMD_GF_BankStorage) != wDataSize ) return false;

	//��ȡ���
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	ASSERT(pIServerUserItem!=NULL);
	if(pIServerUserItem==NULL) return false;
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();

	//�ж�״̬
	if(m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
	{
		if(pServerUserData->wTableID!=INVALID_TABLE)
		{
			SendRoomMessage(dwSocketID,TEXT("���˳���Ϸ���ٽ��д洢����!"),SMT_EJECT);
			return true;
		}
	}

	//����ת��
	CMD_GF_BankStorage *pBankStorage= (CMD_GF_BankStorage*)pData;
	LONG lGameGold = pServerUserData->UserScoreInfo.lGameGold;

	//Ч������
	ASSERT(pBankStorage->lStorageValue>0 && pBankStorage->lStorageValue<=lGameGold);
	if(!(pBankStorage->lStorageValue>0 && pBankStorage->lStorageValue<=lGameGold))return false;

	//����Ч��
	if (lstrcmp(pIServerUserItem->GetPassword(),pBankStorage->szPassword)!=0)
	{
		SendRoomMessage(dwSocketID,TEXT("�����������֤���ٴγ��Բ�����"),SMT_EJECT);
		return true;
	}

	//�޸Ľ��
	ModifyGameGold(pIServerUserItem,-pBankStorage->lStorageValue);

	//�޸��ʿ�
	ModifyBankStorageGold(pIServerUserItem,pBankStorage->lStorageValue);
	SendRoomMessage(dwSocketID,TEXT("�洢�ɹ�!"),SMT_EJECT);

	//��������
	DBR_GR_BankStorage BankStorage;
	ZeroMemory(&BankStorage, sizeof(BankStorage));
	BankStorage.lStorageCount = pBankStorage->lStorageValue;
	BankStorage.dwUserID = pServerUserData->dwUserID;
	BankStorage.dwClientIP = pIServerUserItem->GetClientIP();

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_BANK_STORAGE_GOLD,0,&BankStorage, sizeof(BankStorage));

	return true;
}

//ʹ��Ȩ��
bool CAttemperEngineSink::IsPropertyUseRight(INT nPropertyID,IServerUserItem *pIServerUserItem,IServerUserItem *pTargetUserItem)
{
	//Ŀ�����
	if ( pTargetUserItem == NULL )
	{
		SendProMessage(pIServerUserItem, TEXT("����ʧ�ܣ���Ҫ���͵�����Ѿ��뿪��"), SMT_EJECT|SMT_INFO);
		return false;
	}

	//��ȡ���
	ASSERT(pIServerUserItem!=NULL);
	tagServerUserData *pConsumeUserData = pIServerUserItem->GetUserData();
	tagServerUserData *pTargetUserData = pTargetUserItem->GetUserData();

	//�����ж�
	if ( (m_pGameServiceOption->wServerType == GAME_GENRE_GOLD || m_pGameServiceOption->wServerType == GAME_GENRE_MATCH )
		&& ( nPropertyID == PROP_NEGAGIVE || nPropertyID == PROP_DOUBLE || nPropertyID == PROP_FOURDOLD || nPropertyID == PROP_SHIELD) )
	{
		TCHAR szMessage[] = TEXT("�˷��䲻����ʹ�ô˵��ߣ�");
		SendProMessage(pIServerUserItem, szMessage, SMT_INFO|SMT_EJECT);
		return false;
	}

	//ʹ���ж�
	switch(nPropertyID)
	{
	case PROP_NEGAGIVE :			//��������
		{
			LONG lNowScore = pTargetUserData->UserScoreInfo.lScore;
			if( 0 <= lNowScore)
			{
				CString strMessage;
				if ( pIServerUserItem == pTargetUserItem ) 
					strMessage.Format(TEXT("�����ڵĻ����Ѿ��ǷǸ���������Ҫʹ�ø���������ߣ�"));
				else
					strMessage.Format(TEXT("[ %s ]���ڵĻ����Ѿ��ǷǸ���������Ҫʹ�ø���������ߣ�"), pTargetUserData->szAccounts);

				SendProMessage(pIServerUserItem, strMessage, SMT_INFO|SMT_EJECT);
				return false;
			}
			break;
		}
	case PROP_FLEE :				//��������
		{
			LONG lNowFleeCount = pTargetUserData->UserScoreInfo.lFleeCount;

			if ( lNowFleeCount == 0 )
			{
				CString strMessage;
				if ( pIServerUserItem == pTargetUserItem ) 
					strMessage.Format(TEXT("�����ڵ��������Ѿ�Ϊ0������Ҫʹ������������ߣ�"));
				else
					strMessage.Format(TEXT("[ %s ]���ڵ��������Ѿ�Ϊ0������Ҫʹ������������ߣ�"), pTargetUserData->szAccounts);

				SendProMessage(pIServerUserItem, strMessage, SMT_INFO|SMT_EJECT);
				return false;
			}
			break;
		}
	case PROP_BUGLE :				//�����ж�
		{
			//��������
			bool bMatchServer=((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0);
			bool bControlStart=((bMatchServer==true)&&m_pGameServiceOption->cbControlStart==TRUE);

			//�����Թ�
			if (pConsumeUserData->dwMasterRight==0L)
			{
				if ((bMatchServer==true)&&(pIServerUserItem->GetUserStatus()==US_LOOKON))
				{
					LPCTSTR pszMessage=TEXT("Ϊ�˲�Ӱ�����ѡ�ֱ������Թ۵��û���ֹ���ԣ�");
					SendProMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
					return false;
				}
			}

			//Ȩ���ж�
			if (CUserRight::CanGameChat(pConsumeUserData->dwUserRight)==false)
			{
				LPCTSTR pszMessage=TEXT("����ʱû�з��ͷ�����Ϣ��Ȩ�ޣ�ֻ�������Ա˽�ģ�");
				SendProMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
				return false;
			}

			//״̬��ѯ
			if ((pConsumeUserData->dwMasterRight==0L)&&(IsAllowGameChat()==false))
			{
				LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ���䲻������������Ϣ��");
				SendProMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
				return false;
			}
			break;
		}
	}

	return true;
}

//�޸Ľ��
void CAttemperEngineSink::ModifyGameGold(IServerUserItem * pIServerUserItem,LONG lGameGold)
{
	//�û���Ϣ
	ASSERT(pIServerUserItem!=NULL);
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();
	tagUserScore *pScoreModify = pIServerUserItem->GetUserScoreModifyInfo();

	//�޸Ľ��
	pServerUserData->UserScoreInfo.lGameGold += lGameGold;
	pScoreModify->lGameGold +=lGameGold;

	//��ҷ���
	if(m_pGameServiceOption->wServerType == GAME_GENRE_GOLD)
	{
		pServerUserData->UserScoreInfo.lScore += lGameGold;
		pScoreModify->lScore +=lGameGold;

		//��������
		pServerUserData->UserScoreInfo.lScore += pServerUserData->lStorageScore;
		pServerUserData->lStorageScore=0L;
		ASSERT(pServerUserData->UserScoreInfo.lGameGold==pServerUserData->UserScoreInfo.lScore);

		//�洢����
		if ( m_pGameServiceOption->lRestrictScore>0L && pServerUserData->UserScoreInfo.lScore>m_pGameServiceOption->lRestrictScore )
		{
			LONG lUserScore=pServerUserData->UserScoreInfo.lScore;
			pServerUserData->UserScoreInfo.lScore=m_pGameServiceOption->lRestrictScore;
			pServerUserData->lStorageScore=lUserScore-m_pGameServiceOption->lRestrictScore;
		}

		//���ٶ��
		LONG lLessScore=m_pGameServiceOption->lLessScore;
		if((pIServerUserItem->GetTableID()!=INVALID_TABLE) && (lLessScore>0) && (lLessScore>pServerUserData->UserScoreInfo.lScore))
		{
			CTableFrame* pTableFrame=m_pTableFrame+pIServerUserItem->GetTableID();

			if (!(pTableFrame->IsUserPlaying(pIServerUserItem)))
			{
				//������Ϣ
				TCHAR szDescribe[256]=TEXT("");
				_snprintf(szDescribe,sizeof(szDescribe),TEXT("������Ϸ������Ҫ %ld �Ľ�ң����Ľ�Ҳ��������ܼ��룡"),lLessScore);
				SendRoomMessage(pIServerUserItem,szDescribe,SMT_INFO);

				//�����û�
				pTableFrame->PerformStandUpAction(pIServerUserItem);
			}
		}

		//�����
		LONG lMaxScore=m_pGameServiceOption->lMaxScore;
		if((pIServerUserItem->GetTableID()!=INVALID_TABLE) && (lMaxScore>m_pGameServiceOption->lLessScore) && (lMaxScore<pServerUserData->UserScoreInfo.lScore))
		{
			CTableFrame* pTableFrame=m_pTableFrame+pIServerUserItem->GetTableID();

			if (!(pTableFrame->IsUserPlaying(pIServerUserItem)))
			{
				//������Ϣ
				TCHAR szDescribe[256]=TEXT("");
				_snprintf(szDescribe,sizeof(szDescribe),TEXT("���Ľ�ҳ����������������ƶ��%ld,�������Ϸ����!"),lMaxScore);
				SendRoomMessage(pIServerUserItem,szDescribe,SMT_INFO);

				//�����û�
				pTableFrame->PerformStandUpAction(pIServerUserItem);
			}
		}
	}

	return;
}

//��ȡ�ʿ�
LONG CAttemperEngineSink::GetBankStorageGold(IServerUserItem * pIServerUserItem)
{
	//��ȡ�ṹ
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();

	return pServerUserData->UserScoreInfo.lInsureScore;
}

//�޸��ʿ�
void CAttemperEngineSink::ModifyBankStorageGold(IServerUserItem * pIServerUserItem, LONG lStorageCount)
{
	//�޸Ľ��
	pIServerUserItem->ModifyBankStorageGold(lStorageCount);

	//����֪ͨ
	SendUserScore( pIServerUserItem );

	return ;
}


//////////////////////////////////////////////////////////////////////////
