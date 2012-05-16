#include "StdAfx.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

CAttemperEngineSink::CAttemperEngineSink()
{
	//���ñ���
	m_pInitParamter=NULL;
	m_pBindParameter=NULL;

	//�ӿڱ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	return;
}

CAttemperEngineSink::~CAttemperEngineSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CAttemperEngineSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//����ģ������
bool __cdecl CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParamter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParamter->m_wMaxConnect);

	//�������
	if ((m_ServerListCenter.GetInterface()==NULL)&&(m_ServerListCenter.CreateInstance()==false))
	{
		CTraceService::TraceString(TEXT("���ķ������б��������ʧ��"),TraceLevel_Exception);
		return false;
	}

	//�����б�
	tagDataBaseInfo DataBaseInfo;
	DataBaseInfo.wDataBasePort=m_pInitParamter->m_wServerDataBasePort;
	DataBaseInfo.dwDataBaseAddr=inet_addr(m_pInitParamter->m_szServerDataBaseAddr);
	lstrcpyn(DataBaseInfo.szDataBaseUser,m_pInitParamter->m_szServerDataBaseUser,CountArray(DataBaseInfo.szDataBaseUser));
	lstrcpyn(DataBaseInfo.szDataBasePass,m_pInitParamter->m_szServerDataBasePass,CountArray(DataBaseInfo.szDataBasePass));
	lstrcpyn(DataBaseInfo.szDataBaseName,m_pInitParamter->m_szServerDataBaseName,CountArray(DataBaseInfo.szDataBaseName));
	if (m_ServerListCenter->LoadServerList(DataBaseInfo)==false) 
	{
		CTraceService::TraceString(TEXT("��Ϸ�б����ʧ��"),TraceLevel_Exception);
		return false;
	}

	return true;
}

//����ģ��ر�
bool __cdecl CAttemperEngineSink::OnAttemperEngineStop(IUnknownEx * pIUnknownEx)
{
	//���ñ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�����б�
	if (m_ServerListCenter.GetInterface()!=NULL)
	{
		m_ServerListCenter->ResetServerList();
	}

	return true;
}

//Ӧ���¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientIP, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParamter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParamter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientIP=dwClientIP;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//�ر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return true;
}

//��ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_SERVER_LIST:		//�б���Ϣ
		{
			return OnSocketMainServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_SERVER_MANAGER:		//�������
		{
			return OnSocketMainServerManager(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return true;
}

//�б���Ϣ����
bool CAttemperEngineSink::OnSocketMainServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_GET_SERVER_LIST:	//��ȡ�б�
		{
			//��������
			CMD_CS_ListInfo ListInfo;
			ZeroMemory(&ListInfo,sizeof(ListInfo));

			//���ñ���
			ListInfo.dwTypeCount=m_ServerListCenter->GetGameTypeCount();
			ListInfo.dwKindCount=m_ServerListCenter->GetGameKindCount();
			ListInfo.dwServerCount=m_ServerListCenter->GetGameServerCount();
			ListInfo.dwStationCount=m_ServerListCenter->GetGameStationCount();
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_INFO,&ListInfo,sizeof(ListInfo));

			//������Ϣ
			POSITION Pos=NULL;
			WORD wSendSize=0L;
			BYTE cbBuffer[SOCKET_PACKET];
			for (DWORD i=0;i<ListInfo.dwTypeCount;i++)
			{
				if ((wSendSize+sizeof(tagGameType))>sizeof(cbBuffer))
				{
					m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_TYPE,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameTypeItem * pGameTypeItem=m_ServerListCenter->EmunGameTypeItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameTypeItem->GameType,sizeof(tagGameType));
				wSendSize+=sizeof(tagGameType);
			}
			if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_TYPE,cbBuffer,wSendSize);

			//������Ϣ
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwKindCount;i++)
			{
				if ((wSendSize+sizeof(tagGameKind))>sizeof(cbBuffer))
				{
					m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_KIND,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameKindItem * pGameKindItem=m_ServerListCenter->EmunGameKindItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameKindItem->GameKind,sizeof(tagGameKind));
				wSendSize+=sizeof(tagGameKind);
			}
			if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_KIND,cbBuffer,wSendSize);

			//վ����Ϣ
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwStationCount;i++)
			{
				if ((wSendSize+sizeof(tagGameStation))>sizeof(cbBuffer))
				{
					m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_STATION,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameStationItem * pGameStationItem=m_ServerListCenter->EmunGameStationItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameStationItem->GameStation,sizeof(tagGameStation));
				wSendSize+=sizeof(tagGameStation);
			}
			if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_STATION,cbBuffer,wSendSize);

			//������Ϣ
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwServerCount;i++)
			{
				if ((wSendSize+sizeof(tagGameServer))>sizeof(cbBuffer))
				{
					m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_SERVER,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameServerItem * pGameServerItem=m_ServerListCenter->EmunGameServerItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameServerItem->GameServer,sizeof(tagGameServer));
				wSendSize+=sizeof(tagGameServer);
			}
			if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_SERVER,cbBuffer,wSendSize);

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_FINISH);

			return true;	
		}
	}

	return false;
}

//������Ϣ����
bool CAttemperEngineSink::OnSocketMainServerManager(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_REG_GAME_SERVER:		//����ע��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_RegGameServer));
			if (wDataSize!=sizeof(CMD_CS_RegGameServer)) return false;

			//��Ϣ����
			CMD_CS_RegGameServer * pRegGameServer=(CMD_CS_RegGameServer *)pData;
			tagGameServer * pGameServer=&pRegGameServer->GameServer;
			tagGameServerItem * pGameServerItem=m_ServerListCenter->SearchGameServer(pGameServer->wKindID,pGameServer->wServerID);
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
				CopyMemory(&pGameServerItem->GameServer,pGameServer,sizeof(tagGameServer));
			}
			else m_ServerListCenter->InsertGameServer(pGameServer);

			return true;
		}
	case SUB_CS_UNREG_GAME_SERVER:		//ע������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_UnRegGameServer));
			if (wDataSize!=sizeof(CMD_CS_UnRegGameServer)) return false;

			//��Ϣ����
			CMD_CS_UnRegGameServer * pUnRegGameServer=(CMD_CS_UnRegGameServer *)pData;
			m_ServerListCenter->DeleteGameServer(pUnRegGameServer->wKindID,pUnRegGameServer->wServerID);

			return true;
		}
	case SUB_CS_SERVER_ONLINE_COUNT:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_ServerOnLineCount));
			if (wDataSize!=sizeof(CMD_CS_ServerOnLineCount)) return false;

			//��Ϣ����
			CMD_CS_ServerOnLineCount * pServerOnLineCount=(CMD_CS_ServerOnLineCount *)pData;
			WORD wKindID=pServerOnLineCount->wKindID;
			WORD wServerID=pServerOnLineCount->wServerID;
			m_ServerListCenter->UpdateServerOnLineCount(wKindID,wServerID,pServerOnLineCount->dwOnLineCount);

			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
