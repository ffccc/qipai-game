#include "StdAfx.h"
#include "AttemperEngineSink.h"
#include "Zip.h"
#include "LogonServer.h"

//��������
extern CLogonServerApp theApp; 
//////////////////////////////////////////////////////////////////////////

//�궨��
#define TIME_RECONNECT						15000L						//����ʱ��
#define TIME_UPDATE_LIST					30000L						//����ʱ��
#define TIME_DOWNLOAD_CUSTOM_FACE			200L						//����ͷ��

//��ʱ�� ID
#define IDI_CONNECT_CENTER_SERVER			1							//���ӷ�����
#define IDI_UPDATE_SERVER_LIST				2							//�����б�
#define IDI_DOWNLOAD_CUSTOM_FACE			3							//����ͷ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//���ñ���
	m_pInitParamter=NULL;
	m_pBindParameter=NULL;

	//���ñ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketCorrespond=NULL;

	return;
}

//��������
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
	//�������
	m_ServerList.SetSocketEngine(m_pITCPNetworkEngine);

	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParamter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParamter->m_wMaxConnect);

	//��������
	DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);
	m_pITCPSocketCorrespond->Connect(ntohl(dwServerIP),PORT_CENTER_SERVER);

	//��ʼ����
	//m_DownloadFaceInfoMap.InitHashTable(503);
	//m_DownloadFaceInfoMap.RemoveAll();

	//���ö�ʱ��
	//m_pITimerEngine->SetTimer(IDI_DOWNLOAD_CUSTOM_FACE,TIME_DOWNLOAD_CUSTOM_FACE,TIMES_INFINITY,NULL);

	return true;
}

//����ģ��ر�
bool __cdecl CAttemperEngineSink::OnAttemperEngineStop(IUnknownEx * pIUnknownEx)
{
	//���ñ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketCorrespond=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	return true;
}

//��ʱ���¼�
bool __cdecl CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_CONNECT_CENTER_SERVER:		//�������ķ�����
		{
			//��������
			DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);
			m_pITCPSocketCorrespond->Connect(dwServerIP,PORT_CENTER_SERVER);

			//������ʾ
			CTraceService::TraceString(TEXT("���ڳ��������������ķ�����...."),TraceLevel_Normal);

			return true;
		}
	case IDI_UPDATE_SERVER_LIST:		//���·������б�
		{
			m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);

			return true;
		}
	}

	return false;
}

//���ݿ��¼�
bool __cdecl CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOGON_ERROR:			//��¼ʧ��
		{
			return OnDBLogonError(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBR_GP_DOWNLOADFACE_SUCCESS:	//���سɹ�
		{
			return OnDBDownloadFaceSuccess(dwContextID,pData,wDataSize);
		}
	case DBR_GP_UPLOAD_FACE_RESULT:		//�ϴ����
		{
			return OnDBRCustomFaceUploadResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_DELETE_FACE_RESULT:		//ɾ�����
		{
			return OnDBRCustomFaceDeleteResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_RESULT:			//�޸Ľ��
		{
			return OnDBRModifyIndividual(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//�û���¼�ɹ�
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_LogonSuccess));
	if (wDataSize!=sizeof(DBR_GP_LogonSuccess)) return false;

	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParamter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbBuffer[SOCKET_PACKET];
	DBR_GP_LogonSuccess * pDBRLogonSuccess=(DBR_GP_LogonSuccess *)pData;
	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbBuffer;

	//��������
	pCMDLogonSuccess->wFaceID=pDBRLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBRLogonSuccess->cbGender;
	pCMDLogonSuccess->cbMember=pDBRLogonSuccess->cbMember;
	pCMDLogonSuccess->dwUserID=pDBRLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwGameID=pDBRLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwExperience=pDBRLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwCustomFaceVer=pDBRLogonSuccess->dwCustomFaceVer;
//	pCMDLogonSuccess->cbMoorMachine=pDBRLogonSuccess->cbMoorMachine;

	//��������
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_LogonSuccess),sizeof(cbBuffer)-sizeof(CMD_GP_LogonSuccess));
	SendPacket.AddPacket(pDBRLogonSuccess->szPassword,CountStringBuffer(pDBRLogonSuccess->szPassword),DTP_USER_PASS);
	SendPacket.AddPacket(pDBRLogonSuccess->szAccounts,CountStringBuffer(pDBRLogonSuccess->szAccounts),DTP_USER_ACCOUNTS);
	SendPacket.AddPacket(pDBRLogonSuccess->szUnderWrite,CountStringBuffer(pDBRLogonSuccess->szUnderWrite),DTP_UNDER_WRITE);

	//������Ϣ
	if (pDBRLogonSuccess->szGroupName[0]!=0)
	{
		SendPacket.AddPacket(pDBRLogonSuccess->szGroupName,CountStringBuffer(pDBRLogonSuccess->szGroupName),DTP_USER_GROUP_NAME);
	}

	//վ����ҳ
	if (m_pInitParamter->m_szMainPage[0]!=0)
	{
		SendPacket.AddPacket(m_pInitParamter->m_szMainPage,CountStringBuffer(m_pInitParamter->m_szMainPage),DTP_STATION_PAGE);
	}

	//���͵�¼���
	WORD wSendSize=sizeof(CMD_GP_LogonSuccess)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbBuffer,wSendSize);

	//�б�����
	CMD_GP_ListConfig ListConfig;
	ListConfig.bShowOnLineCount=TRUE;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_CONFIG,&ListConfig,sizeof(ListConfig));

	//������Ϸ�б�
	m_ServerList.SendGameTypeList(dwContextID);
	m_ServerList.SendGameKindList(dwContextID);
	m_ServerList.SendGameStationList(dwContextID);
	m_ServerList.SendGameServerList(dwContextID);

	//���͵�¼���
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH);

	//�ر�����
	if ( pDBRLogonSuccess->dwCustomFaceVer == 0 )
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�û���¼ʧ��
bool CAttemperEngineSink::OnDBLogonError(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_LogonError));
	if (wDataSize!=sizeof(DBR_GP_LogonError)) return false;

	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParamter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_LogonError LogonError;
	DBR_GP_LogonError * pLogonError=(DBR_GP_LogonError *)pData;
	pLogonError->szErrorDescribe[CountArray(pLogonError->szErrorDescribe)-1]=0;

	//��������
	LogonError.lErrorCode=pLogonError->lErrorCode;
	lstrcpyn(LogonError.szErrorDescribe,pLogonError->szErrorDescribe,CountArray(LogonError.szErrorDescribe));
	WORD wDescribeSize=CountStringBuffer(LogonError.szErrorDescribe);

	//��������
	WORD wSendSize=sizeof(LogonError)-sizeof(LogonError.szErrorDescribe)+wDescribeSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_ERROR,&LogonError,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//���سɹ�
bool CAttemperEngineSink::OnDBDownloadFaceSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����ת��
	DBR_GP_DownloadFaceSuccess *pDownloadFaceSuccess = (DBR_GP_DownloadFaceSuccess*)pData;

	//������֤
	WORD wPostSize = WORD(sizeof(DBR_GP_DownloadFaceSuccess)-sizeof(pDownloadFaceSuccess->bFaceData)+pDownloadFaceSuccess->dwCurrentSize);
	ASSERT(wDataSize == wPostSize);
	if ( wPostSize != wDataSize ) return false;

	//��������
	CMD_GP_DownloadFaceSuccess DownloadFaceSuccess;
	DownloadFaceSuccess.dwUserID = pDownloadFaceSuccess->dwUserID;
	DownloadFaceSuccess.dwToltalSize = pDownloadFaceSuccess->dwToltalSize;
	DownloadFaceSuccess.dwCurrentSize = pDownloadFaceSuccess->dwCurrentSize;
	CopyMemory(DownloadFaceSuccess.bFaceData, pDownloadFaceSuccess->bFaceData, pDownloadFaceSuccess->dwCurrentSize);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_USER_DOWNLOAD_FACE, &DownloadFaceSuccess, wPostSize);

	return true;
}

//�ɹ���Ϣ
bool CAttemperEngineSink::OnDBRCustomFaceUploadResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(DBR_GP_UploadFaceResult));
	if ( wDataSize != sizeof(DBR_GP_UploadFaceResult)) return false;

	//����ת��
	DBR_GP_UploadFaceResult *pUploadFaceResult = (DBR_GP_UploadFaceResult*)pData;
	pUploadFaceResult->szDescribeMsg[CountArray(pUploadFaceResult->szDescribeMsg)-1] = 0;

	//��������
	CMD_GP_UploadFaceResult UploadFaceResult;
	ZeroMemory(&UploadFaceResult, sizeof(UploadFaceResult));
	lstrcpy(UploadFaceResult.szDescribeMsg, pUploadFaceResult->szDescribeMsg);
	UploadFaceResult.dwFaceVer = pUploadFaceResult->dwFaceVer;
	UploadFaceResult.bOperateSuccess=pUploadFaceResult->bOperateSuccess;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_UPLOAD_FACE_RESULT, &UploadFaceResult, sizeof(UploadFaceResult));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//ɾ�����
bool CAttemperEngineSink::OnDBRCustomFaceDeleteResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_DeleteFaceResult));
	if (wDataSize != sizeof(DBR_GP_DeleteFaceResult)) return false;

	//����ת��
	DBR_GP_DeleteFaceResult *pDeleteFaceResult = (DBR_GP_DeleteFaceResult*)pData;
	pDeleteFaceResult->szDescribeMsg[CountArray(pDeleteFaceResult->szDescribeMsg)-1] = 0;

	//��������
	CMD_GP_DeleteFaceResult DeleteFaceResult;
	ZeroMemory(&DeleteFaceResult, sizeof(DeleteFaceResult));
	lstrcpy(DeleteFaceResult.szDescribeMsg, pDeleteFaceResult->szDescribeMsg);
	DeleteFaceResult.dwFaceVer=pDeleteFaceResult->dwFaceVer;
	DeleteFaceResult.bOperateSuccess = pDeleteFaceResult->bOperateSuccess;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_DELETE_FACE_RESULT, &DeleteFaceResult, sizeof(DeleteFaceResult));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	
	return true;
}

//�޸Ľ��
bool CAttemperEngineSink::OnDBRModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyIndividualResult));
	if (wDataSize != sizeof(DBR_GP_ModifyIndividualResult)) return false;

	//����ת��
	DBR_GP_ModifyIndividualResult *pModifyIndividualResul = (DBR_GP_ModifyIndividualResult*)pData;
	pModifyIndividualResul->szDescribeMsg[CountArray(pModifyIndividualResul->szDescribeMsg)-1] = 0;

	CMD_GP_ModifyIndividualResult ModifyIndividualResult;
	ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
	ModifyIndividualResult.bOperateSuccess = pModifyIndividualResul->bOperateSuccess;
	lstrcpyn(ModifyIndividualResult.szDescribeMsg, pModifyIndividualResul->szDescribeMsg, CountArray(ModifyIndividualResult.szDescribeMsg));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_MODIFY_INDIVIDUAL_RESULT, &ModifyIndividualResult, sizeof(ModifyIndividualResult));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�����¼�
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

	//��ȡ�б�
	m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);

	//���ö�ʱ��
	m_pITimerEngine->SetTimer(IDI_UPDATE_SERVER_LIST,TIME_UPDATE_LIST,TIMES_INFINITY,NULL);

	return true;
}

//�ر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//��������
	m_pITimerEngine->KillTimer(IDI_UPDATE_SERVER_LIST);
	m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

	//������ʾ
	CTraceService::TraceString(TEXT("���ķ����������жϣ��Ժ󽫻��ٴγ���...."),TraceLevel_Warning);

	return true;
}

//��ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_SERVER_LIST:	//�б���Ϣ
		{
			return OnCenterMainServerList(Command.wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//����Ӧ���¼�
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

//�����ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:				//��¼��Ϣ
		{
			return OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		} 
	case MDM_GP_USER:				//�û���Ϣ
		{
			return OnSocketMainUser(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//����ر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return true;
}

//��¼��Ϣ����
bool CAttemperEngineSink::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_LogonByAccounts));
			if (wDataSize<sizeof(CMD_GP_LogonByAccounts)) return false;

			//������Ϣ
			CMD_GP_LogonByAccounts * pLogonByAccounts=(CMD_GP_LogonByAccounts *)pData;
			pLogonByAccounts->szAccounts[CountArray(pLogonByAccounts->szAccounts)-1]=0;
			pLogonByAccounts->szPassWord[CountArray(pLogonByAccounts->szPassWord)-1]=0;

			//������Ϣ
			ASSERT(LOWORD(dwSocketID)<m_pInitParamter->m_wMaxConnect);
			DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientIP;

			//Ч��汾
			if (pLogonByAccounts->dwPlazaVersion!=VER_PLAZA_FRAME)
			{
				//��ȡ�汾
				WORD wLow=LOWORD(pLogonByAccounts->dwPlazaVersion);
				WORD wHigh=HIWORD(pLogonByAccounts->dwPlazaVersion);

				//��������
				CMD_GP_Version Version;
				Version.bNewVersion=TRUE;
				Version.bAllowConnect=(wHigh==VER_PLAZA_HIGH)?TRUE:FALSE;

				//��������
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SYSTEM,SUB_GP_VERSION,&Version,sizeof(Version));

				//�жϹر�
				if (Version.bAllowConnect==FALSE)
				{
					return true;
				}
			}

			//��������
			DBR_GP_LogonByAccounts LogonByAccounts;
			ZeroMemory(&LogonByAccounts,sizeof(LogonByAccounts));

			//���ñ���
			LogonByAccounts.dwClientIP=dwClientAddr;
			lstrcpyn(LogonByAccounts.szAccounts,pLogonByAccounts->szAccounts,CountArray(LogonByAccounts.szAccounts));
			lstrcpyn(LogonByAccounts.szPassWord,pLogonByAccounts->szPassWord,CountArray(LogonByAccounts.szPassWord));

			//��������
			VOID * pData=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonByAccounts+1,wDataSize-sizeof(CMD_GP_LogonByAccounts));

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
								ASSERT(CountArray(LogonByAccounts.szComputerID)>((i*2)+1));
								_stprintf(&LogonByAccounts.szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
							}
						}

						break;
					}
				}
			}

			//Ͷ�����ݿ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_BY_ACCOUNTS,dwSocketID,&LogonByAccounts,sizeof(LogonByAccounts));

			return true;
		}
	case SUB_GP_LOGON_USERID:		//ID ��¼
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_LogonByUserID));
			if (wDataSize<sizeof(CMD_GP_LogonByUserID)) return false;

			//������Ϣ
			CMD_GP_LogonByUserID * pLogonByUserID=(CMD_GP_LogonByUserID *)pData;
			pLogonByUserID->szPassWord[CountArray(pLogonByUserID->szPassWord)-1]=0;

			//������Ϣ
			ASSERT(LOWORD(dwSocketID)<m_pInitParamter->m_wMaxConnect);
			DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientIP;

			//Ч��汾
			if (pLogonByUserID->dwPlazaVersion!=VER_PLAZA_FRAME)
			{
				//��ȡ�汾
				WORD wLow=LOWORD(pLogonByUserID->dwPlazaVersion);
				WORD wHigh=HIWORD(pLogonByUserID->dwPlazaVersion);

				//��������
				CMD_GP_Version Version;
				Version.bNewVersion=TRUE;
				Version.bAllowConnect=(wHigh==VER_PLAZA_HIGH)?TRUE:FALSE;

				//��������
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SYSTEM,SUB_GP_VERSION,&Version,sizeof(Version));

				//�жϹر�
				if (Version.bAllowConnect==FALSE)
				{
					return true;
				}
			}

			//��������
			DBR_GP_LogonByUserID LogonByUserID;
			memset(&LogonByUserID,0,sizeof(LogonByUserID));
			LogonByUserID.dwClientIP=dwClientAddr;
			LogonByUserID.dwUserID=pLogonByUserID->dwUserID;
			lstrcpyn(LogonByUserID.szPassWord,pLogonByUserID->szPassWord,CountArray(LogonByUserID.szPassWord));

			//��������
			VOID * pData=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonByUserID+1,wDataSize-sizeof(CMD_GP_LogonByUserID));

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
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_BY_USERID,dwSocketID,&LogonByUserID,sizeof(LogonByUserID));

			return true;
		}
	case SUB_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
			if (wDataSize<sizeof(CMD_GP_RegisterAccounts)) return false;

			//������Ϣ
			CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pData;
			pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
			pRegisterAccounts->szPassWord[CountArray(pRegisterAccounts->szPassWord)-1]=0;

			//������Ϣ
			ASSERT(LOWORD(dwSocketID)<m_pInitParamter->m_wMaxConnect);
			DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientIP;

			//Ч��汾
			if (pRegisterAccounts->dwPlazaVersion!=VER_PLAZA_FRAME)
			{
				//��ȡ�汾
				WORD wLow=LOWORD(pRegisterAccounts->dwPlazaVersion);
				WORD wHigh=HIWORD(pRegisterAccounts->dwPlazaVersion);

				//��������
				CMD_GP_Version Version;
				Version.bNewVersion=TRUE;
				Version.bAllowConnect=(wHigh==VER_PLAZA_HIGH)?TRUE:FALSE;

				//��������
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SYSTEM,SUB_GP_VERSION,&Version,sizeof(Version));

				//�жϹر�
				if (Version.bAllowConnect==FALSE)
				{
					return true;
				}
			}

			//��������
			DBR_GP_RegisterAccounts RegisterAccounts;
			memset(&RegisterAccounts,0,sizeof(RegisterAccounts));
			RegisterAccounts.dwClientIP=dwClientAddr;
			RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
			RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
			lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
			lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
			lstrcpyn(RegisterAccounts.szPassWord,pRegisterAccounts->szPassWord,CountArray(RegisterAccounts.szPassWord));
			lstrcpyn(RegisterAccounts.szPassWordBank,pRegisterAccounts->szPassWordBank,CountArray(RegisterAccounts.szPassWordBank));

			//��������
			VOID * pData=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pRegisterAccounts+1,wDataSize-sizeof(CMD_GP_RegisterAccounts));

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
								ASSERT(CountArray(RegisterAccounts.szComputerID)>((i*2)+1));
								_stprintf(&RegisterAccounts.szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
							}
						}

						break;
					}
				}
			}

			//Ͷ�����ݿ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

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
	case SUB_GP_USER_UPLOAD_FACE:		//�ϴ�ͷ��
		{
			//����ת��
			CMD_GP_UploadCustomFace *pUploadCustomFace = (CMD_GP_UploadCustomFace*)pData;

			//������֤
			ASSERT(wDataSize == sizeof(CMD_GP_UploadCustomFace)-sizeof(pUploadCustomFace->bFaceData)+pUploadCustomFace->wCurrentSize);
			if (wDataSize != sizeof(CMD_GP_UploadCustomFace)-sizeof(pUploadCustomFace->bFaceData)+pUploadCustomFace->wCurrentSize) return false;

			//д���ļ�
			try
			{
				//�ļ�����
				CFile fileCustomFace;
				TCHAR szFileName[128];
				_snprintf(szFileName, CountArray(szFileName), TEXT("%s\\UploadFile_%ld.zip"), theApp.m_szDirWork, pUploadCustomFace->dwUserID);
				BOOL bSuccess=FALSE;
				if ( pUploadCustomFace->bFirstPacket == true )
					bSuccess=fileCustomFace.Open( szFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				else
					bSuccess=fileCustomFace.Open( szFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::modeNoTruncate);
				if ( bSuccess )
				{
					//�ƶ�ָ��
					fileCustomFace.SeekToEnd();

					//д���ļ�
					fileCustomFace.Write(pUploadCustomFace->bFaceData, pUploadCustomFace->wCurrentSize);

					//�ر��ļ�
					fileCustomFace.Close();

					//����ж�
					if ( pUploadCustomFace->bLastPacket == true )
					{
						//Ͷ������
						DBR_GP_UploadCustomFace UploadCustomFace;
						ZeroMemory(&UploadCustomFace, sizeof(UploadCustomFace));
						UploadCustomFace.dwUserID = pUploadCustomFace->dwUserID;
						m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_UPLOAD_FACE, dwSocketID, &UploadCustomFace, sizeof(DBR_GP_UploadCustomFace));
					}
				}
			}
			catch(...){}

			return true;
		}
	case SUB_GP_USER_DOWNLOAD_FACE:		//����ͷ��
		{
			//������֤
			ASSERT(sizeof(CMD_GP_DownloadFace)==wDataSize);
			if ( sizeof(CMD_GP_DownloadFace) != wDataSize) return false;

			CMD_GP_DownloadFace *pDownloadFace = (CMD_GP_DownloadFace*)pData;

			ASSERT( pDownloadFace->dwUserID != 0 );

			//Ͷ������
			DBR_GP_DownloadCustomFace DownloadCustomFace;
			ZeroMemory(&DownloadCustomFace, sizeof(DownloadCustomFace));
			DownloadCustomFace.dwUserID = pDownloadFace->dwUserID ;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DOWNLOAD_FACE, dwSocketID, &DownloadCustomFace, sizeof(DownloadCustomFace));

			return true;
		}
	case SUB_GP_CUSTOM_FACE_DELETE:		//ɾ��ͷ��
		{
			//������֤
			ASSERT(sizeof(CMD_GP_CustomFaceDelete) == wDataSize);
			if ( sizeof(CMD_GP_CustomFaceDelete) != wDataSize) return false;

			//����ת��
			CMD_GP_CustomFaceDelete *pCustomFaceDelete = (CMD_GP_CustomFaceDelete *)pData;

			DBR_GP_CustomFaceDelete CustomFaceDelete;
			ZeroMemory(&CustomFaceDelete, sizeof(CustomFaceDelete));
			CustomFaceDelete.dwUserID = pCustomFaceDelete->dwUserID;
			CustomFaceDelete.dwFaceVer = pCustomFaceDelete->dwFaceVer;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DELETE_FACE, dwSocketID, &CustomFaceDelete, sizeof(CustomFaceDelete));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:		//�޸�����
		{
			//������֤
			ASSERT(sizeof(CMD_GP_ModifyIndividual) == wDataSize);
			if ( sizeof(CMD_GP_ModifyIndividual) != wDataSize) return false;

			//����ת��
			CMD_GP_ModifyIndividual *pModifyIndividual = (CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szAddress[CountArray(pModifyIndividual->szAddress)-1] = 0;
			pModifyIndividual->szNickname[CountArray(pModifyIndividual->szNickname)-1] = 0;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1] = 0;
			pModifyIndividual->szUnderWrite[CountArray(pModifyIndividual->szUnderWrite)-1] = 0;

			DBR_GP_ModifyIndividual ModifyIndividual;

			ZeroMemory(&ModifyIndividual, sizeof(ModifyIndividual));

			ModifyIndividual.dwUserID = pModifyIndividual->dwUserID;
			ModifyIndividual.nAge = pModifyIndividual->nAge;
			ModifyIndividual.nGender = pModifyIndividual->nGender;
			lstrcpyn(ModifyIndividual.szAddress, pModifyIndividual->szAddress, CountArray(ModifyIndividual.szAddress));
			lstrcpyn(ModifyIndividual.szNickname, pModifyIndividual->szNickname, CountArray(ModifyIndividual.szNickname));
			lstrcpyn(ModifyIndividual.szPassword, pModifyIndividual->szPassword, CountArray(ModifyIndividual.szPassword));
			lstrcpyn(ModifyIndividual.szUnderWrite, pModifyIndividual->szUnderWrite, CountArray(ModifyIndividual.szUnderWrite));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL, dwSocketID, &ModifyIndividual, sizeof(ModifyIndividual));

			return true;
		}
	default:
		{
			ASSERT(FALSE);
			return false;
		}
	}

	return true;
}



//�б���Ϣ����
bool CAttemperEngineSink::OnCenterMainServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
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
			//��Ϣ����
			m_ServerList.ActiveServerListBuffer();

			return true;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
