#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CServiceUnits *			CServiceUnits::g_pServiceUnits=NULL;			//����ָ��

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServiceUnits, CWnd)
	ON_MESSAGE(WM_UICONTROL_REQUEST,OnUIControlRequest)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServiceUnits::CServiceUnits()
{
	//�������
	m_ServiceStatus=ServiceStatus_Stop;

	//���ýӿ�
	m_pIServiceUnitsSink=NULL;

	//���ö���
	ASSERT(g_pServiceUnits==NULL);
	if (g_pServiceUnits==NULL) g_pServiceUnits=this;

	//�������
	ZeroMemory(&m_GameParameter,sizeof(m_GameParameter));
	ZeroMemory(&m_DataBaseParameter,sizeof(m_DataBaseParameter));
	ZeroMemory(&m_GameServiceAttrib,sizeof(m_GameServiceAttrib));
	ZeroMemory(&m_GameServiceOption,sizeof(m_GameServiceOption));

	return;
}

//��������
CServiceUnits::~CServiceUnits()
{
}

//���ýӿ�
bool CServiceUnits::SetServiceUnitsSink(IServiceUnitsSink * pIServiceUnitsSink)
{
	//���ñ���
	m_pIServiceUnitsSink=pIServiceUnitsSink;

	return true;
}

//Ͷ������
bool CServiceUnits::PostControlRequest(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	//״̬�ж�
	ASSERT(IsWindow(m_hWnd));
	if (IsWindow(m_hWnd)==FALSE) return false;

	//�������
	CWHDataLocker DataLocker(m_CriticalSection);
	if (m_DataQueue.InsertData(wIdentifier,pData,wDataSize)==false) return false;

	//������Ϣ
	PostMessage(WM_UICONTROL_REQUEST,wIdentifier,wDataSize);

	return true;
}

//��������
bool CServiceUnits::StartService()
{
	//Ч��״̬
	ASSERT(m_ServiceStatus==ServiceStatus_Stop);
	if (m_ServiceStatus!=ServiceStatus_Stop) return false;

	//����״̬
	SetServiceStatus(ServiceStatus_Config);

	//��������
	if (m_hWnd==NULL)
	{
		CRect rcCreate(0,0,0,0);
		Create(NULL,NULL,WS_CHILD,rcCreate,AfxGetMainWnd(),100);
	}

	//����ģ��
	if (CreateServiceDLL()==false)
	{
		ConcludeService();
		return false;
	}

	//��������
	if (RectifyServiceParameter()==false)
	{
		ConcludeService();
		return false;
	}

	//���÷���
	if (InitializeService()==false)
	{
		ConcludeService();
		return false;
	}

	//�����ں�
	if (StartKernelService()==false)
	{
		ConcludeService();
		return false;
	}

	//��������
	SendControlPacket(CT_LOAD_SERVICE_CONFIG,NULL,0);

	return true;
}

//ֹͣ����
bool CServiceUnits::ConcludeService()
{
	//���ñ���
	SetServiceStatus(ServiceStatus_Stop);

	//�ں����
	if (m_TimerEngine.GetInterface()!=NULL) m_TimerEngine->ConcludeService();
	if (m_AttemperEngine.GetInterface()!=NULL) m_AttemperEngine->ConcludeService();
	if (m_TCPSocketService.GetInterface()!=NULL) m_TCPSocketService->ConcludeService();
	if (m_TCPNetworkEngine.GetInterface()!=NULL) m_TCPNetworkEngine->ConcludeService();

	//��������
	if (m_RecordDataBaseEngine.GetInterface()!=NULL) m_RecordDataBaseEngine->ConcludeService();
	if (m_KernelDataBaseEngine.GetInterface()!=NULL) m_KernelDataBaseEngine->ConcludeService();

	//ע�����
	if (m_GameServiceManager.GetInterface()!=NULL) m_GameServiceManager.CloseInstance();

	return true;
}

//��Ϸ����
bool CServiceUnits::CollocateService(LPCTSTR pszGameModule, tagGameServiceOption & GameServiceOption)
{
	//Ч��״̬
	ASSERT(m_ServiceStatus==ServiceStatus_Stop);
	if (m_ServiceStatus!=ServiceStatus_Stop) return false;

	//����ģ��
	m_GameServiceOption=GameServiceOption;
	m_GameServiceManager.SetModuleCreateInfo(pszGameModule,GAME_SERVICE_CREATE_NAME);

	return true;
}

//����ģ��
bool CServiceUnits::CreateServiceDLL()
{
	//ʱ������
	if ((m_TimerEngine.GetInterface()==NULL)&&(m_TimerEngine.CreateInstance()==false))
	{
		CTraceService::TraceString(m_TimerEngine.GetErrorDescribe(),TraceLevel_Exception);
		return false;
	}

	//��������
	if ((m_AttemperEngine.GetInterface()==NULL)&&(m_AttemperEngine.CreateInstance()==false))
	{
		CTraceService::TraceString(m_AttemperEngine.GetErrorDescribe(),TraceLevel_Exception);
		return false;
	}

	//�������
	if ((m_TCPSocketService.GetInterface()==NULL)&&(m_TCPSocketService.CreateInstance()==false))
	{
		CTraceService::TraceString(m_TCPSocketService.GetErrorDescribe(),TraceLevel_Exception);
		return false;
	}

	//��������
	if ((m_TCPNetworkEngine.GetInterface()==NULL)&&(m_TCPNetworkEngine.CreateInstance()==false))
	{
		CTraceService::TraceString(m_TCPNetworkEngine.GetErrorDescribe(),TraceLevel_Exception);
		return false;
	}

	//�������
	if ((m_KernelDataBaseEngine.GetInterface()==NULL)&&(m_KernelDataBaseEngine.CreateInstance()==false))
	{
		CTraceService::TraceString(m_KernelDataBaseEngine.GetErrorDescribe(),TraceLevel_Exception);
		return false;
	}

	//�������
	if ((m_RecordDataBaseEngine.GetInterface()==NULL)&&(m_RecordDataBaseEngine.CreateInstance()==false))
	{
		CTraceService::TraceString(m_RecordDataBaseEngine.GetErrorDescribe(),TraceLevel_Exception);
		return false;
	}

	//��Ϸģ��
	if ((m_GameServiceManager.GetInterface()==NULL)&&(m_GameServiceManager.CreateInstance()==false))
	{
		CTraceService::TraceString(m_GameServiceManager.GetErrorDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//�������
bool CServiceUnits::InitializeService()
{
	//���ز���
	m_InitParameter.LoadInitParameter();

	//���ò���
	m_GameParameter.wMedalRate=1L;
	m_GameParameter.wRevenueRate=1L;

	//������Ϣ
	LPCTSTR pszDataBaseAddr=m_GameServiceOption.szDataBaseAddr;
	LPCTSTR pszDataBaseName=m_GameServiceOption.szDataBaseName;
	if (LoadDataBaseParameter(pszDataBaseAddr,pszDataBaseName,m_DataBaseParameter)==false) return false;

	//����ӿ�
	IUnknownEx * pIAttemperEngine=m_AttemperEngine.GetInterface();
	IUnknownEx * pITCPNetworkEngine=m_TCPNetworkEngine.GetInterface();

	//�ص��ӿ�
	IUnknownEx * pIDataBaseEngineSink=QUERY_OBJECT_INTERFACE(m_DataBaseEngineSink,IUnknownEx);
	IUnknownEx * pIAttemperEngineSink=QUERY_OBJECT_INTERFACE(m_AttemperEngineSink,IUnknownEx);

	//��������
	IUnknownEx * pIDataBaseEngineRecordSink[4];
	IUnknownEx * pIDataBaseEngineKernelSink[8];
	for (WORD i=0;i<CountArray(pIDataBaseEngineRecordSink);i++) pIDataBaseEngineRecordSink[i]=pIDataBaseEngineSink;
	for (WORD i=0;i<CountArray(pIDataBaseEngineKernelSink);i++) pIDataBaseEngineKernelSink[i]=pIDataBaseEngineSink;

	//�󶨽ӿ�
	if (m_AttemperEngine->SetAttemperEngineSink(pIAttemperEngineSink)==false) return false;
	if (m_RecordDataBaseEngine->SetDataBaseEngineSink(pIDataBaseEngineRecordSink,CountArray(pIDataBaseEngineRecordSink))==false) return false;
	if (m_KernelDataBaseEngine->SetDataBaseEngineSink(pIDataBaseEngineKernelSink,CountArray(pIDataBaseEngineKernelSink))==false) return false;

	//�ں����
	if (m_TimerEngine->SetTimerEngineEvent(pIAttemperEngine)==false) return false;
	if (m_AttemperEngine->SetNetworkEngine(pITCPNetworkEngine)==false) return false;
	if (m_TCPNetworkEngine->SetTCPNetworkEngineEvent(pIAttemperEngine)==false) return false;

	//Э������
	if (m_TCPSocketService->SetServiceID(NETWORK_CORRESPOND)==false) return false;
	if (m_TCPSocketService->SetTCPSocketEvent(pIAttemperEngine)==false) return false;

	//���Ȼص�
	m_AttemperEngineSink.m_pInitParameter=&m_InitParameter;
	m_AttemperEngineSink.m_pGameParameter=&m_GameParameter;
	m_AttemperEngineSink.m_pGameServiceAttrib=&m_GameServiceAttrib;
	m_AttemperEngineSink.m_pGameServiceOption=&m_GameServiceOption;

	//���Ȼص�
	m_AttemperEngineSink.m_pITimerEngine=m_TimerEngine.GetInterface();
	m_AttemperEngineSink.m_pIAttemperEngine=m_AttemperEngine.GetInterface();
	m_AttemperEngineSink.m_pITCPSocketService=m_TCPSocketService.GetInterface();
	m_AttemperEngineSink.m_pITCPNetworkEngine=m_TCPNetworkEngine.GetInterface();
	m_AttemperEngineSink.m_pIGameServiceManager=m_GameServiceManager.GetInterface();
	m_AttemperEngineSink.m_pIRecordDataBaseEngine=m_RecordDataBaseEngine.GetInterface();
	m_AttemperEngineSink.m_pIKernelDataBaseEngine=m_KernelDataBaseEngine.GetInterface();

	//���ݻص�
	m_DataBaseEngineSink.m_pInitParameter=&m_InitParameter;
	m_DataBaseEngineSink.m_pGameParameter=&m_GameParameter;
	m_DataBaseEngineSink.m_pDataBaseParameter=&m_DataBaseParameter;
	m_DataBaseEngineSink.m_pGameServiceAttrib=&m_GameServiceAttrib;
	m_DataBaseEngineSink.m_pGameServiceOption=&m_GameServiceOption;
	m_DataBaseEngineSink.m_pIGameServiceManager=m_GameServiceManager.GetInterface();
	m_DataBaseEngineSink.m_pIDataBaseEngineEvent=QUERY_OBJECT_PTR_INTERFACE(pIAttemperEngine,IDataBaseEngineEvent);

	//��������
	m_TCPNetworkEngine->SetServiceParameter(m_GameServiceOption.wServerPort,m_GameServiceOption.wMaxPlayer,szCompilation);

	return true;
}

//�����ں�
bool CServiceUnits::StartKernelService()
{
	//ʱ������
	if (m_TimerEngine->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_AttemperEngine->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//Э������
	if (m_TCPSocketService->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_RecordDataBaseEngine->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if (m_KernelDataBaseEngine->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//��������
bool CServiceUnits::StartNetworkService()
{
	//��������
	if (m_TCPNetworkEngine->StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//��������
bool CServiceUnits::RectifyServiceParameter()
{
	//��ȡ����
	m_GameServiceManager->GetServiceAttrib(m_GameServiceAttrib);
	if (lstrcmp(m_GameServiceAttrib.szServerDLLName,m_GameServiceManager.m_szModuleDllName)!=0)
	{
		CTraceService::TraceString(TEXT("��Ϸģ��İ� DLL ����������ģ�� DLL ���ֲ�һ��"),TraceLevel_Exception);
		return false;
	}

	//��������
	if (m_GameServiceManager->RectifyParameter(m_GameServiceOption)==false)
	{
		CTraceService::TraceString(TEXT("��Ϸģ��������ò���ʧ��"),TraceLevel_Exception);
		return false;
	}

	//��������
	if ((m_GameServiceOption.wServerType&m_GameServiceAttrib.wSupporType)==0)
	{
		CTraceService::TraceString(TEXT("��Ϸģ�鲻֧��������ָ���ķ���ģʽ����"),TraceLevel_Exception);
		return false;
	}

	//����ģʽ
	if ((m_GameServiceOption.cbDistributeRule&DISTRIBUTE_ALLOW)!=0)
	{
		//��������
		CServerRule::SetAvertCheatMode(m_GameServiceOption.dwServerRule,true);

		//��������
		m_GameServiceOption.wMinDistributeUser=__max(m_GameServiceAttrib.wChairCount,m_GameServiceOption.wMinDistributeUser);

		//�������
		if (m_GameServiceOption.wMaxDistributeUser!=0)
		{
			m_GameServiceOption.wMaxDistributeUser=__max(m_GameServiceOption.wMaxDistributeUser,m_GameServiceOption.wMinDistributeUser);
		}
	}

	//��Ϸ��¼
	if (m_GameServiceOption.wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))
	{
		CServerRule::SetRecordGameScore(m_GameServiceOption.dwServerRule,true);
	}

	//��ʱд��
	if (m_GameServiceOption.wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))
	{
		CServerRule::SetImmediateWriteScore(m_GameServiceOption.dwServerRule,true);
	}

	//�ҽ�����
	if (m_GameServiceOption.wSortID==0) m_GameServiceOption.wSortID=500;
	if (m_GameServiceOption.wKindID==0) m_GameServiceOption.wKindID=m_GameServiceAttrib.wKindID;

	//�������
	WORD wMaxPlayer=m_GameServiceOption.wTableCount*m_GameServiceAttrib.wChairCount;
	m_GameServiceOption.wMaxPlayer=__max(m_GameServiceOption.wMaxPlayer,wMaxPlayer+RESERVE_USER_COUNT);

	//���Ƶ���
	if (m_GameServiceOption.lMinEnterScore!=0L)
	{
		m_GameServiceOption.lMinEnterScore=__max(m_GameServiceOption.lMinEnterScore,m_GameServiceOption.lMinTableScore);
	}

	//���Ƶ���
	if (m_GameServiceOption.lMaxEnterScore!=0L)
	{
		m_GameServiceOption.lMaxEnterScore=__max(m_GameServiceOption.lMaxEnterScore,m_GameServiceOption.lMinTableScore);
	}

	return true;
}

//����״̬
bool CServiceUnits::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//״̬�ж�
	if (m_ServiceStatus!=ServiceStatus)
	{
		//����֪ͨ
		if ((m_ServiceStatus!=ServiceStatus_Service)&&(ServiceStatus==ServiceStatus_Stop))
		{
			LPCTSTR pszString=TEXT("��������ʧ��");
			CTraceService::TraceString(pszString,TraceLevel_Exception);
		}

		//���ñ���
		m_ServiceStatus=ServiceStatus;

		//״̬֪ͨ
		ASSERT(m_pIServiceUnitsSink!=NULL);
		if (m_pIServiceUnitsSink!=NULL) m_pIServiceUnitsSink->OnServiceUnitsStatus(m_ServiceStatus);
	}

	return true;
}

//���Ϳ���
bool CServiceUnits::SendControlPacket(WORD wControlID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT(m_AttemperEngine.GetInterface()!=NULL);
	if (m_AttemperEngine.GetInterface()==NULL) return false;

	//���Ϳ���
	m_AttemperEngine->OnEventControl(wControlID,pData,wDataSize);

	return true;
}

//������Ϣ
bool CServiceUnits::LoadDataBaseParameter(LPCTSTR pszDataBaseAddr, LPCTSTR pszDataBaseName, tagDataBaseParameter & DataBaseParameter)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//��������
	if ((PlatformDBModule.GetInterface()==NULL)&&(PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	tagDataBaseParameter * pPlatformDBParameter=&m_InitParameter.m_PlatformDBParameter;

	//��������
	PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
		pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass);

	//��ȡ��Ϣ
	try
	{
		//��������
		PlatformDBModule->OpenConnection();
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//������Ϣ
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@strDataBaseAddr"),pszDataBaseAddr);

		//ִ�в�ѯ
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadDataBaseInfo"),true)!=DB_SUCCESS)
		{
			//������Ϣ
			TCHAR szErrorDescribe[128]=TEXT("");
			PlatformDBAide.GetValue_String(TEXT("ErrorDescribe"),szErrorDescribe,CountArray(szErrorDescribe));

			//��ʾ��Ϣ
			CTraceService::TraceString(szErrorDescribe,TraceLevel_Exception);

			return false;
		}

		//��ȡ����
		TCHAR szDBUserRead[512]=TEXT(""),szDBPassRead[512]=TEXT("");
		PlatformDBAide.GetValue_String(TEXT("DBUser"),szDBUserRead,CountArray(szDBUserRead));
		PlatformDBAide.GetValue_String(TEXT("DBPassword"),szDBPassRead,CountArray(szDBPassRead));

		//��ȡ��Ϣ
		DataBaseParameter.wDataBasePort=PlatformDBAide.GetValue_WORD(TEXT("DBPort"));
		lstrcpyn(DataBaseParameter.szDataBaseAddr,pszDataBaseAddr,CountArray(DataBaseParameter.szDataBaseAddr));
		lstrcpyn(DataBaseParameter.szDataBaseName,pszDataBaseName,CountArray(DataBaseParameter.szDataBaseName));

		//��������
		TCHAR szDataBaseUser[32]=TEXT(""),szDataBasePass[32]=TEXT("");
		CWHEncrypt::XorCrevasse(szDBUserRead,DataBaseParameter.szDataBaseUser,CountArray(DataBaseParameter.szDataBaseUser));
		CWHEncrypt::XorCrevasse(szDBPassRead,DataBaseParameter.szDataBasePass,CountArray(DataBaseParameter.szDataBasePass));
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//������Ϣ
LRESULT CServiceUnits::OnUIControlRequest(WPARAM wParam, LPARAM lParam)
{
	//��������
	tagDataHead DataHead;
	BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

	//��ȡ����
	CWHDataLocker DataLocker(m_CriticalSection);
	if (m_DataQueue.DistillData(DataHead,cbBuffer,sizeof(cbBuffer))==false)
	{
		ASSERT(FALSE);
		return NULL;
	}

	//���ݴ���
	switch (DataHead.wIdentifier)
	{
	case UI_CORRESPOND_RESULT:		//Э���ɹ�
		{
			//Ч����Ϣ
			ASSERT(DataHead.wDataSize==sizeof(CP_ControlResult));
			if (DataHead.wDataSize!=sizeof(CP_ControlResult)) return 0;

			//��������
			CP_ControlResult * pControlResult=(CP_ControlResult *)cbBuffer;

			//ʧ�ܴ���
			if ((m_ServiceStatus!=ServiceStatus_Service)&&(pControlResult->cbSuccess==ER_FAILURE))
			{
				ConcludeService();
				return 0;
			}

			//�ɹ�����
			if ((m_ServiceStatus!=ServiceStatus_Service)&&(pControlResult->cbSuccess==ER_SUCCESS))
			{
				//����״̬
				SetServiceStatus(ServiceStatus_Service);
			}

			return 0;
		}
	case UI_SERVICE_CONFIG_RESULT:	//���ý��
		{
			//Ч����Ϣ
			ASSERT(DataHead.wDataSize==sizeof(CP_ControlResult));
			if (DataHead.wDataSize!=sizeof(CP_ControlResult)) return 0;

			//��������
			CP_ControlResult * pControlResult=(CP_ControlResult *)cbBuffer;

			//ʧ�ܴ���
			if ((m_ServiceStatus!=ServiceStatus_Service)&&(pControlResult->cbSuccess==ER_FAILURE))
			{
				ConcludeService();
				return 0;
			}

			//�ɹ�����
			if ((m_ServiceStatus!=ServiceStatus_Service)&&(pControlResult->cbSuccess==ER_SUCCESS))
			{
				//��������
				if (StartNetworkService()==false)
				{
					ConcludeService();
					return 0;
				}

				//����Э��
				SendControlPacket(CT_CONNECT_CORRESPOND,NULL,0);
			}

			return 0;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
