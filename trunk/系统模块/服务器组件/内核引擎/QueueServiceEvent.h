#ifndef QUEUE_SERVICE_EVENT_HEAD_FILE
#define QUEUE_SERVICE_EVENT_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//���ݶ����¼�
class CQueueServiceEvent
{
	//��������
public:
	CCriticalSection				m_CriticalSection;						//ͬ������
	CQueueService					m_pIQueueService;						//���нӿ�
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];		//���ջ���

	//��������
public:
	//���캯��
	CQueueServiceEvent()
	{
	}
	//��������
	virtual ~CQueueServiceEvent()
	{
	}
	//�߳̾��
	HANDLE GetThreadHandle()
	{
		return m_pIQueueService.GetThreadHandle();
	}
	//������
public:
	//���ýӿ�
	bool SetQueueServiceSink(IUnknownEx * pIUnknownEx)
	{
		if (NULL != pIUnknownEx)
		{
			bool bRet = m_pIQueueService.SetQueueServiceSink(pIUnknownEx);
			if (bRet)
				bRet = m_pIQueueService.StartService();

			return bRet;
		}
		else
		{
			m_pIQueueService.ConcludeService();
			return true;
		}
	}

	//֪ͨ����
public:
	//��ʱ���¼�
	bool PostTimerEvent(DWORD wTimerID, WPARAM wBindParam)
	{
		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_TimerEvent * pTimerEvent = (NTY_TimerEvent *)m_cbBuffer;
		pTimerEvent->dwTimerID = wTimerID;
		pTimerEvent->dwBindParameter = wBindParam;
		m_pIQueueService.AddToQueue(EVENT_TIMER, m_cbBuffer, sizeof(NTY_TimerEvent));

		return true;
	}

	//���ݿ��¼�
	bool PostDataBaseEvent(WORD wRequestID, DWORD dwContextID, const void * pDataBuffer, WORD wDataSize)
	{
		ASSERT((wDataSize + sizeof(NTY_DataBaseEvent)) <= MAX_ASYNCHRONISM_DATA);
		if ((wDataSize + sizeof(NTY_DataBaseEvent)) > MAX_ASYNCHRONISM_DATA) return false;

		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)m_cbBuffer;
		pDataBaseEvent->wRequestID = wRequestID;
		pDataBaseEvent->dwContextID = dwContextID;
		if (wDataSize > 0)
		{
			ASSERT(pDataBuffer != NULL);
			CopyMemory(m_cbBuffer + sizeof(NTY_DataBaseEvent), pDataBuffer, wDataSize);
		}
		m_pIQueueService.AddToQueue(EVENT_DATABASE, m_cbBuffer, sizeof(NTY_DataBaseEvent) + wDataSize);

		return true;
	}

	//���ݿ�����¼�
	bool PostControlEvent(WORD wControlID, VOID * pData, WORD wDataSize)
	{
		//Ч�����
		ASSERT((wDataSize + sizeof(NTY_ControlEvent)) <= MAX_ASYNCHRONISM_DATA);
		if ((wDataSize + sizeof(NTY_ControlEvent)) > MAX_ASYNCHRONISM_DATA) return false;

		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_ControlEvent * pDataBaseEvent = (NTY_ControlEvent *)m_cbBuffer;
		pDataBaseEvent->wControlID = wControlID;
		if (wDataSize > 0)
		{
			ASSERT(pData != NULL);
			CopyMemory(m_cbBuffer + sizeof(NTY_ControlEvent), pData, wDataSize);
		}
		m_pIQueueService.AddToQueue(EVENT_CONTROL, m_cbBuffer, sizeof(NTY_ControlEvent) + wDataSize);

		return true;
	}

	//SOCKET����
	//////////////////////////////////////////////////////////////////////////
	//����Ӧ���¼�
	bool PostNetworkAcceptEvent(DWORD dwSocketID, DWORD dwClientIP)
	{
		//Ч�����
		ASSERT(sizeof(NTY_TCPNetworkAcceptEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPNetworkAcceptEvent) > MAX_ASYNCHRONISM_DATA) return false;

		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_TCPNetworkAcceptEvent * pSocketAcceptEvent = (NTY_TCPNetworkAcceptEvent *)m_cbBuffer;
		pSocketAcceptEvent->dwSocketID = dwSocketID;
		pSocketAcceptEvent->dwClientIP = dwClientIP;
		m_pIQueueService.AddToQueue(EVENT_TCP_NETWORK_ACCEPT, m_cbBuffer, sizeof(NTY_TCPNetworkAcceptEvent));

		return true;
	}

	//�����ȡ�¼�
	bool PostNetworkReadEvent(DWORD dwSocketID, CMD_Command Command, const void * pDataBuffer, WORD wDataSize)
	{
		//Ч�����
		ASSERT((wDataSize + sizeof(NTY_TCPNetworkReadEvent)) <= MAX_ASYNCHRONISM_DATA);
		if ((wDataSize + sizeof(NTY_TCPNetworkReadEvent)) > MAX_ASYNCHRONISM_DATA) return false;

		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_TCPNetworkReadEvent * pSocketReadEvent = (NTY_TCPNetworkReadEvent *)m_cbBuffer;
		pSocketReadEvent->dwSocketID = dwSocketID;
		pSocketReadEvent->Command = Command;
		pSocketReadEvent->wDataSize = wDataSize;
		if (wDataSize > 0)
		{
			ASSERT(pDataBuffer != NULL);
			CopyMemory(m_cbBuffer + sizeof(NTY_TCPNetworkReadEvent), pDataBuffer, wDataSize);
		}
		m_pIQueueService.AddToQueue(EVENT_TCP_NETWORK_READ, m_cbBuffer, sizeof(NTY_TCPNetworkReadEvent) + wDataSize);

		return true;
	}

	//����ر��¼�
	bool PostNetworkCloseEvent(DWORD dwSocketID, DWORD dwClientIP, DWORD dwActiveTime)
	{
		//Ч�����
		ASSERT(sizeof(NTY_TCPNetworkCloseEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPNetworkCloseEvent) > MAX_ASYNCHRONISM_DATA) return false;

		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_TCPNetworkCloseEvent * pSocketCloseEvent = (NTY_TCPNetworkCloseEvent *)m_cbBuffer;
		pSocketCloseEvent->dwSocketID = dwSocketID;
		pSocketCloseEvent->dwActiveTime = dwActiveTime;
		pSocketCloseEvent->dwClientIP = dwClientIP;
		m_pIQueueService.AddToQueue(EVENT_TCP_NETWORK_CLOSE, m_cbBuffer, sizeof(NTY_TCPNetworkCloseEvent));

		return true;
	}

	//SOCKET����
	//////////////////////////////////////////////////////////////////////////
	//����ر��¼�
	bool PostTCPSocketCloseEvent(WORD wServiceID, BYTE cbShutReason)
	{
		//Ч�����
		ASSERT(sizeof(NTY_TCPSocketCloseEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPSocketCloseEvent) > MAX_ASYNCHRONISM_DATA) return false;
		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_TCPSocketCloseEvent * pSocketCloseEvent = (NTY_TCPSocketCloseEvent *)m_cbBuffer;
		pSocketCloseEvent->wServiceID = wServiceID;
		pSocketCloseEvent->cbShutReason = cbShutReason;
		return m_pIQueueService.AddToQueue(EVENT_TCP_SOCKET_CLOSE, m_cbBuffer, sizeof(NTY_TCPSocketCloseEvent));
	}

	//����ر��¼�
	bool PostTCPSocketReadEvent(WORD wServiceID, CMD_Command Command, const void * pDataBuffer, WORD wDataSize)
	{
		//Ч�����
		ASSERT((wDataSize + sizeof(NTY_TCPSocketReadEvent)) <= MAX_ASYNCHRONISM_DATA);
		if ((wDataSize + sizeof(NTY_TCPSocketReadEvent)) > MAX_ASYNCHRONISM_DATA) return false;

		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_TCPSocketReadEvent * pSocketCloseEvent = (NTY_TCPSocketReadEvent *)m_cbBuffer;
		pSocketCloseEvent->wDataSize = wDataSize;
		pSocketCloseEvent->wServiceID = wServiceID;
		pSocketCloseEvent->Command = Command;
		if (wDataSize > 0)
		{
			ASSERT(pDataBuffer != NULL);
			CopyMemory(m_cbBuffer + sizeof(NTY_TCPSocketReadEvent), pDataBuffer, wDataSize);
		}
		m_pIQueueService.AddToQueue(EVENT_TCP_SOCKET_READ, m_cbBuffer, sizeof(NTY_TCPSocketReadEvent) + wDataSize);

		return true;
	}

	//����ر��¼�
	bool PostTCPSocketConnectedEvent(WORD wServiceID, INT nErrorCode)
	{
		//Ч�����
		ASSERT(sizeof(NTY_TCPSocketConnectEvent) <= MAX_ASYNCHRONISM_DATA);
		if (sizeof(NTY_TCPSocketConnectEvent) > MAX_ASYNCHRONISM_DATA) return false;
		//��������
		CThreadLock lock(m_CriticalSection);

		//Ͷ����Ϣ
		NTY_TCPSocketConnectEvent * pSocketCloseEvent = (NTY_TCPSocketConnectEvent *)m_cbBuffer;
		pSocketCloseEvent->wServiceID = wServiceID;
		pSocketCloseEvent->nErrorCode = nErrorCode;
		m_pIQueueService.AddToQueue(EVENT_TCP_SOCKET_CONNECT, m_cbBuffer, sizeof(NTY_TCPSocketConnectEvent));

		return true;
	}

};

//////////////////////////////////////////////////////////////////////////

#endif