#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_LOAD_GAME_LIST			1									//�����б�
#define IDI_CONNECT_CORRESPOND		2									//������ʶ
#define IDI_COLLECT_ONLINE_INFO		3									//ͳ������

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bNeekCorrespond=true;

	//״̬����
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

	//����ʱ��
	ASSERT(m_pITimerEngine!=NULL);
	m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,m_pInitParameter->m_wCollectTime*1000L,TIMES_INFINITY,0);

	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_bNeekCorrespond=true;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�б����
	m_ServerListManager.ResetServerList();

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case CT_CONNECT_CORRESPOND:		//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//ʱ���¼�
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_LOAD_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case IDI_CONNECT_CORRESPOND:	//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_COLLECT_ONLINE_INFO:	//ͳ������
		{
			//��������
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//��ȡ����
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo();

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//����ж�
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��������
			WORD wHeadSize=sizeof(OnLineCountInfo)-sizeof(OnLineCountInfo.OnLineCountKind);
			WORD wSendSize=wHeadSize+OnLineCountInfo.wKindCount*sizeof(OnLineCountInfo.OnLineCountKind[0]);
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ONLINE_COUNT_INFO,0,&OnLineCountInfo,wSendSize);

			return true;
		}
	}

	return false;
}

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:			//��¼����
		{
			return OnTCPNetworkMainPCLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainPCServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_USER_SERVICE:	//��������
		{
			return OnTCPNetworkMainPCUserService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_REMOTE_SERVICE:	//Զ�̷���
		{
			return OnTCPNetworkMainPCRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_LOGON:			//��¼����
		{
			return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainMBServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBPCLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBPCLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_FACE_INFO:			//�û�ͷ��
		{
			return OnDBPCUserFaceInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INDIVIDUAL:		//�û���Ϣ
		{
			return OnDBPCUserIndividual(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_INFO:		//��������
		{
			return OnDBPCUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_SUCCESS:	//���гɹ�
		{
			return OnDBPCUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_FAILURE:	//����ʧ��
		{
			return OnDBPCUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_SUCCESS:		//�����ɹ�
		{
			return OnDBPCOperateSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_FAILURE:		//����ʧ��
		{
			return OnDBPCOperateFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_TYPE_ITEM:			//��������
		{
			return OnDBPCGameTypeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_KIND_ITEM:			//��������
		{
			return OnDBPCGameKindItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_NODE_ITEM:			//�ڵ�����
		{
			return OnDBPCGameNodeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_PAGE_ITEM:			//��������
		{
			return OnDBPCGamePageItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_LIST_RESULT:		//���ؽ��
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBMBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBMBLogonFailure(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (nErrorCode!=0)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("Э������������ʧ�� [ %ld ]��%ld �����������"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		CTraceService::TraceString(TEXT("����ע����Ϸ��¼������..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//���ñ���
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//ע�����
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//������Ϣ
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//ע���¼�
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//ע��ʧ��
		{
			//��������
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//Ч�����
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//�رմ���
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//��ʾ��Ϣ
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0) CTraceService::TraceString(pszDescribeString,TraceLevel_Exception);

			//�¼�֪ͨ
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�б��¼�
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//������Ϣ
		{
			//�����б�
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//��������
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);

			//��������
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//�������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//�����޸�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//��������
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//���ҷ���
			ASSERT(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//���÷���
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//����ɾ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//��������
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//��������
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//�������
		{
			//�����б�
			m_ServerListManager.CleanServerItem();

			//�¼�����
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//Զ�̷���
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//Э������
		{
			//��������
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//Ч�����
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//�ж�����
			ASSERT(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//��������
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ñ���
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//����Ч��
				ASSERT(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//���ñ���
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//��������
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}

	return true;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubPCLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubPCLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubPCRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//��ȡ�б�
		{
			//�����б�
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);

			//�����б�
			if (m_pInitParameter->m_cbDelayList==TRUE)
			{
				//�����б�
				SendGamePageInfo(dwSocketID,INVALID_WORD);
				SendGameNodeInfo(dwSocketID,INVALID_WORD);
				SendGameServerInfo(dwSocketID,INVALID_WORD);
			}
			else
			{
				//����ҳ��
				SendGamePageInfo(dwSocketID,0);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_SERVER:			//��ȡ����
		{
			//Ч������
			ASSERT(wDataSize%sizeof(WORD)==0);
			if (wDataSize%sizeof(WORD)!=0) return false;

			//�����б�
			UINT nKindCount=wDataSize/sizeof(WORD);
			for (UINT i=0;i<nKindCount;i++)
			{
				SendGameNodeInfo(dwSocketID,((WORD *)pData)[i]);
				SendGamePageInfo(dwSocketID,((WORD *)pData)[i]);
				SendGameServerInfo(dwSocketID,((WORD *)pData)[i]);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_SERVER_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//��ȡ����
		{
			//��������
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//��������
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//����ж�
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��ȡ����
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//��ȡ����
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//���ñ���
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
				}

				//����ж�
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT(FALSE);
					break;
				}
			}

			//��������
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//��������
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	case SUB_GP_GET_COLLECTION:		//��ȡ�ղ�
		{
			return true;
		}
	}

	return false;
}

//������
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_MODIFY_MACHINE:		//�󶨻���
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyMachine));
			if (wDataSize!=sizeof(CMD_GP_ModifyMachine)) return false;

			//������Ϣ
			CMD_GP_ModifyMachine * pModifyMachine=(CMD_GP_ModifyMachine *)pData;
			pModifyMachine->szPassword[CountArray(pModifyMachine->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyMachine ModifyMachine;
			ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));

			//��������
			ModifyMachine.cbBind=pModifyMachine->cbBind;
			ModifyMachine.dwUserID=pModifyMachine->dwUserID;
			ModifyMachine.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyMachine.szPassword,pModifyMachine->szPassword,CountArray(ModifyMachine.szPassword));
			lstrcpyn(ModifyMachine.szMachineID,pModifyMachine->szMachineID,CountArray(ModifyMachine.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_MACHINE,dwSocketID,&ModifyMachine,sizeof(ModifyMachine));

			return true;
		}
	case SUB_GP_MODIFY_LOGON_PASS:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyLogonPass));
			if (wDataSize!=sizeof(CMD_GP_ModifyLogonPass)) return false;

			//������Ϣ
			CMD_GP_ModifyLogonPass * pModifyLogonPass=(CMD_GP_ModifyLogonPass *)pData;
			pModifyLogonPass->szDesPassword[CountArray(pModifyLogonPass->szDesPassword)-1]=0;
			pModifyLogonPass->szScrPassword[CountArray(pModifyLogonPass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyLogonPass ModifyLogonPass;
			ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

			//��������
			ModifyLogonPass.dwUserID=pModifyLogonPass->dwUserID;
			ModifyLogonPass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyLogonPass.szDesPassword,pModifyLogonPass->szDesPassword,CountArray(ModifyLogonPass.szDesPassword));
			lstrcpyn(ModifyLogonPass.szScrPassword,pModifyLogonPass->szScrPassword,CountArray(ModifyLogonPass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_LOGON_PASS,dwSocketID,&ModifyLogonPass,sizeof(ModifyLogonPass));

			return true;
		}
	case SUB_GP_MODIFY_INSURE_PASS:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//������Ϣ
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//��������
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_MODIFY_UNDER_WRITE:	//�޸�ǩ��
		{
			//��������
			CMD_GP_ModifyUnderWrite * pModifyUnderWrite=(CMD_GP_ModifyUnderWrite *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_GP_ModifyUnderWrite));
			ASSERT(wDataSize>=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)));
			ASSERT(wDataSize==(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite)));

			//Ч�����
			if (wDataSize>sizeof(CMD_GP_ModifyUnderWrite)) return false;
			if (wDataSize<(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite))) return false;
			if (wDataSize!=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite))) return false;

			//������Ϣ
			pModifyUnderWrite->szPassword[CountArray(pModifyUnderWrite->szPassword)-1]=0;
			pModifyUnderWrite->szUnderWrite[CountArray(pModifyUnderWrite->szUnderWrite)-1]=0;

			//��������
			DBR_GP_ModifyUnderWrite ModifyUnderWrite;
			ZeroMemory(&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			//��������
			ModifyUnderWrite.dwUserID=pModifyUnderWrite->dwUserID;
			ModifyUnderWrite.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyUnderWrite.szPassword,pModifyUnderWrite->szPassword,CountArray(ModifyUnderWrite.szPassword));
			lstrcpyn(ModifyUnderWrite.szUnderWrite,pModifyUnderWrite->szUnderWrite,CountArray(ModifyUnderWrite.szUnderWrite));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_UNDER_WRITE,dwSocketID,&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			return true;
		}
	case SUB_GP_SYSTEM_FACE_INFO:	//�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_SystemFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_SystemFaceInfo)) return false;

			//������Ϣ
			CMD_GP_SystemFaceInfo * pSystemFaceInfo=(CMD_GP_SystemFaceInfo *)pData;

			//��������
			DBR_GP_ModifySystemFace ModifySystemFace;
			ZeroMemory(&ModifySystemFace,sizeof(ModifySystemFace));

			//��������
			ModifySystemFace.wFaceID=pSystemFaceInfo->wFaceID;
			ModifySystemFace.dwUserID=pSystemFaceInfo->dwUserID;
			ModifySystemFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifySystemFace.szPassword,pSystemFaceInfo->szPassword,CountArray(ModifySystemFace.szPassword));
			lstrcpyn(ModifySystemFace.szMachineID,pSystemFaceInfo->szMachineID,CountArray(ModifySystemFace.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SYSTEM_FACE,dwSocketID,&ModifySystemFace,sizeof(ModifySystemFace));

			return true;
		}
	case SUB_GP_CUSTOM_FACE_INFO:	//�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_CustomFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_CustomFaceInfo)) return false;

			//������Ϣ
			CMD_GP_CustomFaceInfo * pCustomFaceInfo=(CMD_GP_CustomFaceInfo *)pData;

			//��������
			DBR_GP_ModifyCustomFace ModifyCustomFace;
			ZeroMemory(&ModifyCustomFace,sizeof(ModifyCustomFace));

			//��������
			ModifyCustomFace.dwUserID=pCustomFaceInfo->dwUserID;
			ModifyCustomFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyCustomFace.szPassword,pCustomFaceInfo->szPassword,CountArray(ModifyCustomFace.szPassword));
			lstrcpyn(ModifyCustomFace.szMachineID,pCustomFaceInfo->szMachineID,CountArray(ModifyCustomFace.szMachineID));
			CopyMemory(ModifyCustomFace.dwCustomFace,pCustomFaceInfo->dwCustomFace,sizeof(ModifyCustomFace.dwCustomFace));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_CUSTOM_FACE,dwSocketID,&ModifyCustomFace,sizeof(ModifyCustomFace));

			return true;
		}
	case SUB_GP_QUERY_INDIVIDUAL:	//��ѯ��Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryIndividual));
			if (wDataSize!=sizeof(CMD_GP_QueryIndividual)) return false;

			//������Ϣ
			CMD_GP_QueryIndividual * pQueryIndividual=(CMD_GP_QueryIndividual *)pData;

			//��������
			DBR_GP_QueryIndividual QueryIndividual;
			ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

			//��������
			QueryIndividual.dwUserID=pQueryIndividual->dwUserID;
			QueryIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INDIVIDUAL,dwSocketID,&QueryIndividual,sizeof(QueryIndividual));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//������Ϣ
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyIndividual ModifyIndividual;
			ZeroMemory(&ModifyIndividual,sizeof(ModifyIndividual));

			//���ñ���
			ModifyIndividual.dwUserID=pModifyIndividual->dwUserID;
			ModifyIndividual.cbGender=pModifyIndividual->cbGender;
			ModifyIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyIndividual.szPassword,pModifyIndividual->szPassword,CountArray(ModifyIndividual.szPassword));

			//��������
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pModifyIndividual+1,wDataSize-sizeof(CMD_GP_ModifyIndividual));

			//��չ��Ϣ
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GP_UI_NICKNAME:		//�û��ǳ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName))
						{
							CopyMemory(&ModifyIndividual.szNickName,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szNickName[CountArray(ModifyIndividual.szNickName)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_UNDER_WRITE:		//����ǩ��
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUnderWrite));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUnderWrite))
						{
							CopyMemory(&ModifyIndividual.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUnderWrite[CountArray(ModifyIndividual.szUnderWrite)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_USER_NOTE:		//�û���ע
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote))
						{
							CopyMemory(&ModifyIndividual.szUserNote,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUserNote[CountArray(ModifyIndividual.szUserNote)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_COMPELLATION:	//��ʵ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation))
						{
							CopyMemory(&ModifyIndividual.szCompellation,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szCompellation[CountArray(ModifyIndividual.szCompellation)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_SEAT_PHONE:		//�̶��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone))
						{
							CopyMemory(ModifyIndividual.szSeatPhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szSeatPhone[CountArray(ModifyIndividual.szSeatPhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_MOBILE_PHONE:	//�ƶ��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone))
						{
							CopyMemory(ModifyIndividual.szMobilePhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szMobilePhone[CountArray(ModifyIndividual.szMobilePhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_QQ:				//Q Q ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ))
						{
							CopyMemory(ModifyIndividual.szQQ,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szQQ[CountArray(ModifyIndividual.szQQ)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_EMAIL:			//�����ʼ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail))
						{
							CopyMemory(ModifyIndividual.szEMail,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szEMail[CountArray(ModifyIndividual.szEMail)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_DWELLING_PLACE:	//��ϵ��ַ
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace))
						{
							CopyMemory(ModifyIndividual.szDwellingPlace,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szDwellingPlace[CountArray(ModifyIndividual.szDwellingPlace)-1]=0;
						}
						break;
					}
				}
			}

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL,dwSocketID,&ModifyIndividual,sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_USER_SAVE_SCORE:	//������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserSaveScore));
			if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//������Ϣ
			CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//��������
			UserSaveScore.dwUserID=pUserSaveScore->dwUserID;
			UserSaveScore.lSaveScore=pUserSaveScore->lSaveScore;
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID,pUserSaveScore->szMachineID,CountArray(UserSaveScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:	//ȡ�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTakeScore));
			if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//������Ϣ
			CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//��������
			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lTakeScore=pUserTakeScore->lTakeScore;
			UserTakeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:	//��ѯ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureInfo)) return false;

			//������Ϣ
			CMD_GP_QueryInsureInfo * pQueryInsureInfo=(CMD_GP_QueryInsureInfo *)pData;

			//��������
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//��������
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	}

	return false;
}

//Զ�̴���
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//Э������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//������Ϣ
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//��������
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ӱ���
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//���ұ���
			SearchCorrespond.dwGameID=pSearchCorrespond->dwGameID;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	}

	return false;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubMBLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return false;
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonGameID));
	if (wDataSize<sizeof(CMD_GP_LogonGameID)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pLogonGameID->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_GP_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonAccounts));
	if (wDataSize<sizeof(CMD_GP_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_GP_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
	if (wDataSize<sizeof(CMD_GP_RegisterAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonGameID));
	if (wDataSize<sizeof(CMD_MB_LogonGameID)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonGameID * pLogonGameID=(CMD_MB_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;
	pLogonGameID->szMobilePhone[CountArray(pLogonGameID->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonGameID->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pLogonGameID->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	lstrcpyn(LogonGameID.szMobilePhone,pLogonGameID->szMobilePhone,CountArray(LogonGameID.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pRegisterAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbDataBuffer;

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_LogonSuccess));

	//��������
	pCMDLogonSuccess->wFaceID=pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwGameID=pDBOLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwUserID=pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwCustomID=pDBOLogonSuccess->dwCustomID;
	pCMDLogonSuccess->dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	pCMDLogonSuccess->dwExperience=pDBOLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	lstrcpyn(pCMDLogonSuccess->szAccounts,pDBOLogonSuccess->szAccounts,CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName,pDBOLogonSuccess->szNickName,CountArray(pCMDLogonSuccess->szNickName));

	//��Ա��Ϣ
	if (pDBOLogonSuccess->cbMemberOrder!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//����ǩ��
	if (pDBOLogonSuccess->szUnderWrite[0]!=0)
	{
		SendPacket.AddPacket(pDBOLogonSuccess->szUnderWrite,CountStringBuffer(pDBOLogonSuccess->szUnderWrite),DTP_GP_UNDER_WRITE);
	}

	//��¼�ɹ�
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_LogonSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbDataBuffer,wSendSize);

	//�����б�
	if (m_pInitParameter->m_cbDelayList==TRUE)
	{
		//�����б�
		SendGameTypeInfo(dwContextID);
		SendGameKindInfo(dwContextID);
		SendGamePageInfo(dwContextID,INVALID_WORD);
		SendGameNodeInfo(dwContextID,INVALID_WORD);
		SendGameServerInfo(dwContextID,INVALID_WORD);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}
	else
	{
		SendGameTypeInfo(dwContextID);
		SendGameKindInfo(dwContextID);
		SendGamePageInfo(dwContextID,0);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}

	//��¼���
	CMD_GP_LogonFinish LogonFinish;
	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_LogonFailure * pLogonFailure=(DBO_GP_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�û�ͷ��
bool CAttemperEngineSink::OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserFaceInfo UserFaceInfo;
	ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));
	DBO_GP_UserFaceInfo * pUserFaceInfo=(DBO_GP_UserFaceInfo *)pData;

	//���ñ���
	UserFaceInfo.wFaceID=pUserFaceInfo->wFaceID;
	UserFaceInfo.dwCustomID=pUserFaceInfo->dwCustomID;

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_FACE_INFO,&UserFaceInfo,sizeof(UserFaceInfo));

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserIndividual * pDBOUserIndividual=(DBO_GP_UserIndividual *)pData;
	CMD_GP_UserIndividual * pCMDUserIndividual=(CMD_GP_UserIndividual *)cbDataBuffer;
	CSendPacketHelper SendPacket(cbDataBuffer+sizeof(CMD_GP_UserIndividual),sizeof(cbDataBuffer)-sizeof(CMD_GP_UserIndividual));

	//���ñ���
	ZeroMemory(pCMDUserIndividual,sizeof(CMD_GP_UserIndividual));

	//��������
	pCMDUserIndividual->dwUserID=pDBOUserIndividual->dwUserID;

	//�û���Ϣ
	if (pDBOUserIndividual->szUserNote[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szUserNote);
		SendPacket.AddPacket(pDBOUserIndividual->szUserNote,wBufferSize,DTP_GP_UI_USER_NOTE);
	}

	//��ʵ����
	if (pDBOUserIndividual->szCompellation[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szCompellation);
		SendPacket.AddPacket(pDBOUserIndividual->szCompellation,wBufferSize,DTP_GP_UI_COMPELLATION);
	}

	//�绰����
	if (pDBOUserIndividual->szSeatPhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szSeatPhone);
		SendPacket.AddPacket(pDBOUserIndividual->szSeatPhone,wBufferSize,DTP_GP_UI_SEAT_PHONE);
	}

	//�ƶ��绰
	if (pDBOUserIndividual->szMobilePhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szMobilePhone);
		SendPacket.AddPacket(pDBOUserIndividual->szMobilePhone,wBufferSize,DTP_GP_UI_MOBILE_PHONE);
	}

	//��ϵ����
	if (pDBOUserIndividual->szQQ[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szQQ);
		SendPacket.AddPacket(pDBOUserIndividual->szQQ,wBufferSize,DTP_GP_UI_QQ);
	}

	//�����ʼ�
	if (pDBOUserIndividual->szEMail[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szEMail);
		SendPacket.AddPacket(pDBOUserIndividual->szEMail,wBufferSize,DTP_GP_UI_EMAIL);
	}

	//��ϵ��ַ
	if (pDBOUserIndividual->szDwellingPlace[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szDwellingPlace);
		SendPacket.AddPacket(pDBOUserIndividual->szDwellingPlace,wBufferSize,DTP_GP_UI_DWELLING_PLACE);
	}

	//������Ϣ
	WORD wSendSize=sizeof(CMD_GP_UserIndividual)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INDIVIDUAL,cbDataBuffer,wSendSize);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureInfo * pUserInsureInfo=(DBO_GP_UserInsureInfo *)pData;

	//��������
	CMD_GP_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//��������
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//���гɹ�
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//��������
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//���ñ���
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lSourceScore+pUserInsureSuccess->lVariationScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//������Ϣ
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	DBO_GP_UserInsureFailure * pUserInsureFailure=(DBO_GP_UserInsureFailure *)pData;

	//��������
	UserInsureFailure.lResultCode=pUserInsureFailure->lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�����ɹ�
bool CAttemperEngineSink::OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//��������
	DBO_GP_OperateSuccess * pOperateSuccess=(DBO_GP_OperateSuccess *)pData;

	//��������
	OperateSuccess.lResultCode=pOperateSuccess->lResultCode;
	lstrcpyn(OperateSuccess.szDescribeString,pOperateSuccess->szDescribeString,CountArray(OperateSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	DBO_GP_OperateFailure * pOperateFailure=(DBO_GP_OperateFailure *)pData;

	//��������
	OperateFailure.lResultCode=pOperateFailure->lResultCode;
	lstrcpyn(OperateFailure.szDescribeString,pOperateFailure->szDescribeString,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_MB_LogonSuccess * pDBOLogonSuccess=(DBO_MB_LogonSuccess *)pData;

	//��������
	CMD_MB_LogonSuccess LogonSuccess;
	ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

	//��������
	LogonSuccess.wFaceID=pDBOLogonSuccess->wFaceID;
	LogonSuccess.cbGender=pDBOLogonSuccess->cbGender;
	LogonSuccess.dwGameID=pDBOLogonSuccess->dwGameID;
	LogonSuccess.dwUserID=pDBOLogonSuccess->dwUserID;
	LogonSuccess.dwExperience=pDBOLogonSuccess->dwExperience;
	LogonSuccess.dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	lstrcpyn(LogonSuccess.szNickName,pDBOLogonSuccess->szNickName,CountArray(LogonSuccess.szNickName));

	//��¼�ɹ�
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

	//���ͷ���
	WORD wIndex=LOWORD(dwContextID);
	SendMobileKindInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	SendMobileServerInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_SERVER_LIST,SUB_MB_LIST_FINISH);

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//��������
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wJoinID=(pGameType+i)->wJoinID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//�����б�
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameKind)==0);
	if (wDataSize%sizeof(DBO_GP_GameKind)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameKind);
	DBO_GP_GameKind * pGameKind=(DBO_GP_GameKind *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameKind GameKind;
		ZeroMemory(&GameKind,sizeof(GameKind));

		//��������
		GameKind.wTypeID=(pGameKind+i)->wTypeID;
		GameKind.wJoinID=(pGameKind+i)->wJoinID;
		GameKind.wSortID=(pGameKind+i)->wSortID;
		GameKind.wKindID=(pGameKind+i)->wKindID;
		GameKind.wGameID=(pGameKind+i)->wGameID;
		GameKind.dwOnLineCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID);
		lstrcpyn(GameKind.szKindName,(pGameKind+i)->szKindName,CountArray(GameKind.szKindName));
		lstrcpyn(GameKind.szProcessName,(pGameKind+i)->szProcessName,CountArray(GameKind.szProcessName));

		//�����б�
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}

//��Ϸ�ڵ�
bool CAttemperEngineSink::OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameNode)==0);
	if (wDataSize%sizeof(DBO_GP_GameNode)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameNode);
	DBO_GP_GameNode * pGameNode=(DBO_GP_GameNode *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameNode GameNode;
		ZeroMemory(&GameNode,sizeof(GameNode));

		//��������
		GameNode.wKindID=(pGameNode+i)->wKindID;
		GameNode.wJoinID=(pGameNode+i)->wJoinID;
		GameNode.wSortID=(pGameNode+i)->wSortID;
		GameNode.wNodeID=(pGameNode+i)->wNodeID;
		lstrcpyn(GameNode.szNodeName,(pGameNode+i)->szNodeName,CountArray(GameNode.szNodeName));

		//�����б�
		m_ServerListManager.InsertGameNode(&GameNode);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GamePage)==0);
	if (wDataSize%sizeof(DBO_GP_GamePage)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GamePage);
	DBO_GP_GamePage * pGamePage=(DBO_GP_GamePage *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGamePage GamePage;
		ZeroMemory(&GamePage,sizeof(GamePage));

		//��������
		GamePage.wKindID=(pGamePage+i)->wKindID;
		GamePage.wNodeID=(pGamePage+i)->wNodeID;
		GamePage.wSortID=(pGamePage+i)->wSortID;
		GamePage.wPageID=(pGamePage+i)->wPageID;
		GamePage.wOperateType=(pGamePage+i)->wOperateType;
		lstrcpyn(GamePage.szDisplayName,(pGamePage+i)->szDisplayName,CountArray(GamePage.szDisplayName));

		//�����б�
		m_ServerListManager.InsertGamePage(&GamePage);
	}

	return true;
}

//��Ϸ�б�
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//��������
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//��Ϣ����
	if (pGameListResult->cbSuccess==TRUE)
	{
		//�����б�
		m_ServerListManager.CleanKernelItem();

		//�¼�֪ͨ
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	}
	else
	{
		//������ʾ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�������б����ʧ�ܣ�%ld ������¼���"),m_pInitParameter->m_wReLoadListTime);

		//��ʾ��Ϣ
		CTraceService::TraceString(szDescribe,TraceLevel_Warning);

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}

//�汾���
bool CAttemperEngineSink::CheckPlazaVersion(DWORD dwPlazaVersion, DWORD dwSocketID)
{
	//��������
	bool bMustUpdate=false;
	bool bAdviceUpdate=false;

	//�汾�ж�
	if (GetSubVer(dwPlazaVersion)<GetSubVer(VERSION_PLAZA)) bAdviceUpdate=true;
	if (GetMainVer(dwPlazaVersion)!=GetMainVer(VERSION_PLAZA)) bMustUpdate=true;
	if (GetProductVer(dwPlazaVersion)!=GetProductVer(VERSION_PLAZA)) bMustUpdate=true;

	//�����ж�
	if ((bMustUpdate==true)||(bAdviceUpdate==true))
	{
		//��������
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//��������
		UpdateNotify.cbMustUpdate=bMustUpdate;
		UpdateNotify.cbAdviceUpdate=bAdviceUpdate;
		UpdateNotify.dwCurrentVersion=VERSION_PLAZA;

		//������Ϣ
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//�ж��ж�
		if (bMustUpdate==true) 
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return true;
		}
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//��������
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//���ͽڵ�
VOID CAttemperEngineSink::SendGameNodeInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameNodeItem * pGameNodeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameNodeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameNode))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameNodeItem=m_ServerListManager.EmunGameNodeItem(Position);
		if (pGameNodeItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameNodeItem->m_GameNode.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameNodeItem->m_GameNode,sizeof(tagGameNode));
			wSendSize+=sizeof(tagGameNode);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);

	return;
}

//���Ͷ���
VOID CAttemperEngineSink::SendGamePageInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGamePageItem * pGamePageItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGamePageCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGamePage))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGamePageItem=m_ServerListManager.EmunGamePageItem(Position);
		if (pGamePageItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGamePageItem->m_GamePage.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGamePageItem->m_GamePage,sizeof(tagGamePage));
			wSendSize+=sizeof(tagGamePage);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);

	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID)
{
	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//��������
		if (pGameServerItem->m_GameServer.wKindID==wModuleID)
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);

	return;
}

//////////////////////////////////////////////////////////////////////////////////
