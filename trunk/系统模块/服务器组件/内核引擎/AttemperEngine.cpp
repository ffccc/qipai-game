#include "StdAfx.h"
#include "TraceService.h" // #include "EventService.h"
#include "AttemperEngine.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngine::CAttemperEngine(void)
{
	m_bService = false;
	m_pITCPSocketService = NULL;
	m_pTCPNetworkEngine = NULL;
	m_pIAttemperEngineSink = NULL;
	return;
}

//��������
CAttemperEngine::~CAttemperEngine(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CAttemperEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngine, Guid, dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink, Guid, dwQueryVer);
	QUERYINTERFACE(IDataBaseEngineEvent, Guid, dwQueryVer);
	QUERYINTERFACE(ITCPNetworkEngineEvent, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngine, Guid, dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CAttemperEngine::StartService()
{
	//�ж�״̬
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("���������ظ�������������������"), TraceLevel_Warning);
		return true;
	}

	//��ҽӿ�
	if (m_pIAttemperEngineSink == NULL)
	{
		CTraceService::TraceString(TEXT("����������ҷ��񲻴���"), TraceLevel_Exception);
		return false;
	}

	//���ö���
	if (m_QueueService.SetQueueServiceSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		CTraceService::TraceString(TEXT("������������з����ʧ��"), TraceLevel_Exception);
		return false;
	}

	//�������
	if (m_pIAttemperEngineSink->OnAttemperEngineStart(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		CTraceService::TraceString(TEXT("����������ҷ�������ʧ��"), TraceLevel_Exception);
		return false;
	}

	//��������
	if (m_QueueService.StartService() == false)
	{
		CTraceService::TraceString(TEXT("����������з�������ʧ��"), TraceLevel_Exception);
		return false;
	}

	//��ջ�����
	m_AndroidUserSocketIDArray.RemoveAll();

	//���ñ���
	m_bService = true;

	return true;
}

//ֹͣ����
bool __cdecl CAttemperEngine::ConcludeService()
{
	//���ñ���
	m_bService = false;

	//ֹͣ�������
	m_QueueService.ConcludeService();

	//ֹͣ���
	if (m_pIAttemperEngineSink != NULL)
	{
		m_pIAttemperEngineSink->OnAttemperEngineStop(QUERY_ME_INTERFACE(IUnknownEx));
		m_pIAttemperEngineSink = NULL;
	}

	//��ջ�����
	m_AndroidUserSocketIDArray.RemoveAll();

	return true;
}

//��������
bool __cdecl CAttemperEngine::SetSocketEngine(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx != NULL);
	m_pITCPSocketService = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPSocketService);
	ASSERT(m_pITCPSocketService != NULL);
	return (m_pITCPSocketService != NULL);
}

//��������
bool __cdecl CAttemperEngine::SetNetworkEngine(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx != NULL);
	m_pTCPNetworkEngine = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPNetworkEngine);
	ASSERT(m_pTCPNetworkEngine != NULL);
	return (m_pTCPNetworkEngine != NULL);
}

//ע�ṳ��
bool __cdecl CAttemperEngine::SetAttemperEngineSink(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx != NULL);
	ASSERT(m_pIAttemperEngineSink == NULL);
	if (pIUnknownEx == NULL) return false;
	if (m_pIAttemperEngineSink != NULL) return false;

	//��ѯ�ӿ�
	m_pIAttemperEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngineSink);
	if (m_pIAttemperEngineSink == NULL)
	{
		CTraceService::TraceString(TEXT("����������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"), TraceLevel_Exception);
		return false;
	}

	return true;
}

//��ȡ�ӿ�
void * __cdecl CAttemperEngine::GetQueueService(const IID & Guid, DWORD dwQueryVer)
{
	return m_QueueService.QueryInterface(Guid, dwQueryVer);
}

//�����¼�
//////////////////////////////////////////////////////////////////////////
//�Զ��¼�
bool __cdecl CAttemperEngine::OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	ASSERT(m_pIAttemperEngineSink);
	if (m_pIAttemperEngineSink == NULL) return false;

	return m_pIAttemperEngineSink->OnEventAttemperData(wRequestID, pData, wDataSize);
}

//�����¼�
bool __cdecl CAttemperEngine::OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	ASSERT(m_pIAttemperEngineSink);
	if (m_pIAttemperEngineSink == NULL) return false;

	return m_pIAttemperEngineSink->OnEventControl(wControlID, pData, wDataSize);
}

//���нӿ�
void __cdecl CAttemperEngine::OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)
{
	//�ں��¼�
	ASSERT(m_pIAttemperEngineSink != NULL);
	switch (wIdentifier)
	{
		case EVENT_TIMER:			//��ʱ���¼�
		{
			//Ч�����
			ASSERT(wDataSize == sizeof(NTY_TimerEvent));
			if (wDataSize != sizeof(NTY_TimerEvent)) return;

			//������Ϣ
			NTY_TimerEvent * pTimerEvent = (NTY_TimerEvent *)pBuffer;
			try
			{
				m_pIAttemperEngineSink->OnEventTimer(pTimerEvent->dwTimerID, pTimerEvent->dwBindParameter);
			}
			catch (...)
			{
			}

			return;
		}

		case EVENT_DATABASE:		//���ݿ��¼�
		{
			//Ч�����
			ASSERT(wDataSize >= sizeof(NTY_DataBaseEvent));
			if (wDataSize < sizeof(NTY_DataBaseEvent)) return;

			//������Ϣ
			NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)pBuffer;
			try
			{
				m_pIAttemperEngineSink->OnEventDataBase(pDataBaseEvent->wRequestID, pDataBaseEvent->dwContextID, pDataBaseEvent + 1, wDataSize - sizeof(NTY_DataBaseEvent));
			}
			catch (...)
			{
			}
			return;
		}

		case EVENT_TCP_NETWORK_ACCEPT:	//����Ӧ���¼�
		{
			//Ч���С
			ASSERT(wDataSize == sizeof(NTY_TCPNetworkAcceptEvent));
			if (wDataSize != sizeof(NTY_TCPNetworkAcceptEvent)) return;

			//������Ϣ
			NTY_TCPNetworkAcceptEvent * pSocketAcceptEvent = (NTY_TCPNetworkAcceptEvent *)pBuffer;
			bool bSuccess = false;
			try
			{
				bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkBind(pSocketAcceptEvent->dwClientIP, pSocketAcceptEvent->dwSocketID);
			}
			catch (...) { }
			if (bSuccess == false)
			{
				m_pTCPNetworkEngine->CloseSocket(pSocketAcceptEvent->dwSocketID);
			}

			return;
		}

		case EVENT_TCP_NETWORK_READ:	//�����ȡ�¼�
		{
			//Ч���С
			NTY_TCPNetworkReadEvent * pSocketReadEvent = (NTY_TCPNetworkReadEvent *)pBuffer;
			ASSERT(wDataSize >= sizeof(NTY_TCPNetworkReadEvent));
			ASSERT(wDataSize == (sizeof(NTY_TCPNetworkReadEvent) + pSocketReadEvent->wDataSize));
			if (wDataSize < sizeof(NTY_TCPNetworkReadEvent)) return;
			if (wDataSize != (sizeof(NTY_TCPNetworkReadEvent) + pSocketReadEvent->wDataSize)) return;

			//������Ϣ
			bool bSuccess = false;
			try
			{
				bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkRead(pSocketReadEvent->Command, pSocketReadEvent + 1, pSocketReadEvent->wDataSize, pSocketReadEvent->dwSocketID);
			}
			catch (...) { }
			if (bSuccess == false)
			{
				m_pTCPNetworkEngine->CloseSocket(pSocketReadEvent->dwSocketID);
			}

			return;
		}

		case EVENT_TCP_NETWORK_CLOSE:	//�رն�ȡ�¼�
		{
			//Ч���С
			ASSERT(wDataSize == sizeof(NTY_TCPNetworkCloseEvent));
			if (wDataSize != sizeof(NTY_TCPNetworkCloseEvent)) return;

			//������Ϣ
			NTY_TCPNetworkCloseEvent * pSocketCloseEvent = (NTY_TCPNetworkCloseEvent *)pBuffer;

			bool bSuccess = false;
			try
			{
				bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkShut(pSocketCloseEvent->dwClientIP, pSocketCloseEvent->dwActiveTime, pSocketCloseEvent->dwSocketID);
			}
			catch (...)	{ }
			if (bSuccess == false)
			{
				m_pTCPNetworkEngine->CloseSocket(pSocketCloseEvent->dwSocketID);
			}

			return;
		}

		case EVENT_TCP_SOCKET_READ:		//�����ȡ�¼�
		{
			//Ч���С
			NTY_TCPSocketReadEvent * pSocketReadEvent = (NTY_TCPSocketReadEvent *)pBuffer;
			ASSERT(wDataSize >= sizeof(NTY_TCPSocketReadEvent));
			ASSERT(wDataSize == (sizeof(NTY_TCPSocketReadEvent) + pSocketReadEvent->wDataSize));
			if (wDataSize < sizeof(NTY_TCPSocketReadEvent)) return;
			if (wDataSize != (sizeof(NTY_TCPSocketReadEvent) + pSocketReadEvent->wDataSize)) return;

			//������Ϣ
			bool bSuccess = false;
			try
			{
				bSuccess = m_pIAttemperEngineSink->OnEventTCPSocketRead(pSocketReadEvent->wServiceID, pSocketReadEvent->Command, pSocketReadEvent + 1, pSocketReadEvent->wDataSize);
			}
			catch (...)	{ }
			if (bSuccess == false)
				m_pITCPSocketService->CloseSocket();

			return;
		}

		case EVENT_TCP_SOCKET_CLOSE:	//����ر��¼�
		{
			//Ч���С
			ASSERT(wDataSize == sizeof(NTY_TCPSocketCloseEvent));
			if (wDataSize != sizeof(NTY_TCPSocketCloseEvent)) return;

			//������Ϣ
			NTY_TCPSocketCloseEvent * pSocketCloseEvent = (NTY_TCPSocketCloseEvent *)pBuffer;

			bool bSuccess = false;
			try
			{
				bSuccess = m_pIAttemperEngineSink->OnEventTCPSocketShut(pSocketCloseEvent->wServiceID, pSocketCloseEvent->cbShutReason);
			}
			catch (...)	{ }
			if (bSuccess == false)
				m_pITCPSocketService->CloseSocket();

			return;
		}

		case EVENT_TCP_SOCKET_CONNECT:
		{
			//Ч���С
			ASSERT(wDataSize == sizeof(NTY_TCPSocketConnectEvent));
			if (wDataSize != sizeof(NTY_TCPSocketConnectEvent)) return;

			//������Ϣ
			NTY_TCPSocketConnectEvent * pSocketCloseEvent = (NTY_TCPSocketConnectEvent *)pBuffer;

			bool bSuccess = false;
			try
			{
				bSuccess = m_pIAttemperEngineSink->OnEventTCPSocketLink(pSocketCloseEvent->wServiceID, pSocketCloseEvent->nErrorCode);
			}
			catch (...)	{ }
			if (bSuccess == false)
				m_pITCPSocketService->CloseSocket();

			return;
		}

		case EVENT_CONTROL:
		{
			//Ч���С
			ASSERT(wDataSize == sizeof(NTY_ControlEvent));
			if (wDataSize != sizeof(NTY_ControlEvent)) return;

			//������Ϣ
			NTY_ControlEvent * pControlEvent = (NTY_ControlEvent *)pBuffer;
			try
			{
				m_pIAttemperEngineSink->OnEventControl(wIdentifier, pBuffer, wDataSize);
			}
			catch (...)	{ }

			return;
		}
	}

	//�����¼�
	try
	{
		m_pIAttemperEngineSink->OnEventAttemperData(wIdentifier, pBuffer, wDataSize);
	}
	catch (...)	{ }

	return;
}

bool __cdecl CAttemperEngine::OnEventDataBaseResult(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		return m_pIAttemperEngineSink->OnEventDataBase(wRequestID, dwContextID, pData, wDataSize);
	}
	catch (...)
	{
		return false;
	}
}

//����ģ��ӿ�
//////////////////////////////////////////////////////////////////////////
//Ӧ���¼�
bool __cdecl CAttemperEngine::OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientIP)
{
	//��������
	CThreadLock lock(m_CriticalSection);

	try
	{
		m_AndroidUserSocketIDArray.Add(dwSocketID);
		m_pIAttemperEngineSink->OnEventTCPNetworkBind(dwClientIP, dwSocketID);
	}
	catch (...) { return false; }

	return true;
}

//�ر��¼�
bool __cdecl CAttemperEngine::OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientIP, DWORD dwActiveTime)
{
	//��������
	CThreadLock lock(m_CriticalSection);

	try
	{
		for (int i = 0; i < m_AndroidUserSocketIDArray.GetCount(); i++)
		{
			if (dwSocketID == m_AndroidUserSocketIDArray[i])
			{
				m_AndroidUserSocketIDArray.RemoveAt(i);
				break;
			}
		}
		m_pIAttemperEngineSink->OnEventTCPNetworkShut(dwClientIP, dwActiveTime, dwSocketID);
	}
	catch (...)	{ return false; }

	return true;
}

//��ȡ�¼�
bool __cdecl CAttemperEngine::OnEventTCPNetworkRead(DWORD dwSocketID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	//��������
	CThreadLock lock(m_CriticalSection);

	try
	{
		bool bIsConnected = false;
		for (int i = 0; i < m_AndroidUserSocketIDArray.GetCount(); i++)
		{
			if (dwSocketID == m_AndroidUserSocketIDArray[i])
			{
				bIsConnected = true;
				break;
			}
		}
		ASSERT(bIsConnected);
		if (bIsConnected == false) return false;
		
		m_pIAttemperEngineSink->OnEventTCPNetworkRead(Command, pData, wDataSize, dwSocketID);
	}
	catch (...) { return false; }

	return true;
}

//////////////////////////////////////////////////////////////////////////
//����������
//
extern "C" __declspec(dllexport) void * __cdecl CreateAttemperEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CAttemperEngine * pAttemperEngine = NULL;
	try
	{
		pAttemperEngine = new CAttemperEngine();
		if (pAttemperEngine == NULL) throw TEXT("����ʧ��");
		void * pObject = pAttemperEngine->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pAttemperEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
