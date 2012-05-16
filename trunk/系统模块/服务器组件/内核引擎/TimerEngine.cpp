#include "StdAfx.h"
#include "TimerEngine.h"
#include "TraceService.h" //

//�궨��
#define NO_TIME_LEFT						DWORD(-1)				//����Ӧʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerThread::CTimerThread(void)
{
	m_pTimerEngine = NULL;
}

//��������
CTimerThread::~CTimerThread(void)
{
}

//���ú���
bool CTimerThread::InitThread(CTimerEngine * pTimerEngine)
{
	if (pTimerEngine == NULL) return false;

	//���ñ���
	m_pTimerEngine = pTimerEngine;

	return true;
}

//���к���
bool CTimerThread::OnEventThreadRun()
{
	ASSERT(m_pTimerEngine != NULL);
	Sleep(TIMER_SPACE);
	m_pTimerEngine->OnTimerThreadSink();
	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerEngine::CTimerEngine(void)
{
	m_bService = false;
	m_pIQueueServiceSink = NULL;
}

//��������
CTimerEngine::~CTimerEngine(void)
{
	INT_PTR i = 0;
	//ֹͣ����
	ConcludeService();

	//�����ڴ�
	tagTimerItem * pTimerItem = NULL;
	for (i = 0; i < m_TimerItemFree.GetCount(); i++)
	{
		pTimerItem = m_TimerItemFree[i];
		ASSERT(pTimerItem != NULL);
		SafeDelete(pTimerItem);
	}
	for (i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		SafeDelete(pTimerItem);
	}
	m_TimerItemFree.RemoveAll();
	m_TimerItemActive.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CTimerEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITimerEngine, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITimerEngine, Guid, dwQueryVer);
	return NULL;
}

//���ö�ʱ��
bool __cdecl CTimerEngine::SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	//������Դ
	CThreadLock lock(m_CriticalSection);//

	//Ч�����
	ASSERT(dwRepeat > 0L);
	if (dwRepeat == 0) return false;

	//���Ҷ�ʱ��
	bool bTimerExist = false;
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		if (pTimerItem->wTimerID == dwTimerID)
		{
			bTimerExist = true;
			break;
		}
	}

	//������ʱ��
	if (bTimerExist == false)
	{
		INT_PTR nFreeCount = m_TimerItemFree.GetCount();
		if (nFreeCount > 0)
		{
			pTimerItem = m_TimerItemFree[nFreeCount-1];
			ASSERT(pTimerItem != NULL);
			m_TimerItemFree.RemoveAt(nFreeCount - 1);
		}
		else
		{
			try
			{
				pTimerItem = new tagTimerItem;
				ASSERT(pTimerItem != NULL);
				if (pTimerItem == NULL) return false;
			}
			catch (...)
			{
				return false;
			}
		}
	}

	//���ò���
	ASSERT(pTimerItem != NULL);
	pTimerItem->wTimerID = dwTimerID;
	pTimerItem->wBindParam = dwBindParameter;
	pTimerItem->dwElapse = dwElapse;
	pTimerItem->dwRepeatTimes = dwRepeat;

	//��ǰ20�����Ƚ���֪ͨ - TIMER_SPACE * 20
	if (pTimerItem->dwRepeatTimes == 1)
		pTimerItem->dwTimeLeave = __max(TIMER_SPACE, pTimerItem->dwElapse - TIMER_SPACE * 20);
	else
		pTimerItem->dwTimeLeave = pTimerItem->dwElapse;

	//���ʱ��
	if (bTimerExist == false)
		m_TimerItemActive.Add(pTimerItem);

	return true;
}

//ɾ����ʱ��
bool __cdecl CTimerEngine::KillTimer(DWORD dwTimerID)
{
	//������Դ
	CThreadLock lock(m_CriticalSection);//

	//���Ҷ�ʱ��
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		if (pTimerItem->wTimerID == dwTimerID)
		{
			m_TimerItemActive.RemoveAt(i);
			m_TimerItemFree.Add(pTimerItem);
			return true;;
		}
	}

	return false;
}

//ɾ����ʱ��
bool __cdecl CTimerEngine::KillAllTimer()
{
	//������Դ
	CThreadLock lock(m_CriticalSection);//

	//ɾ����ʱ��
	m_TimerItemFree.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	return true;
}

//��ʼ����
bool __cdecl CTimerEngine::StartService()
{
	//Ч��״̬
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("��ʱ�������ظ�������������������"), TraceLevel_Warning);
		return true;
	}

	//���ñ���
	if (m_TimerThread.InitThread(this) == false)
	{
		CTraceService::TraceString(TEXT("��ʱ�������̷߳����ʼ��ʧ��"), TraceLevel_Exception);
		return false;
	}

	//��������
	if (m_TimerThread.StartThread() == false)
	{
		CTraceService::TraceString(TEXT("��ʱ�������̷߳�������ʧ��"), TraceLevel_Exception);
		return false;
	}

	SetThreadPriority(m_TimerThread.GetThreadHandle(), REALTIME_PRIORITY_CLASS);


	//���ñ���
	m_bService = true;

	return true;
}

//ֹͣ����
bool __cdecl CTimerEngine::ConcludeService()
{
	//���ñ���
	m_bService = false;

	//ֹͣ�߳�
	m_TimerThread.ConcludeThread(INFINITE);

	//���ñ���
	m_TimerItemFree.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	return true;
}

//���ýӿ�
bool __cdecl CTimerEngine::SetTimerEngineEvent(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx != NULL);
	ASSERT(m_bService == false);
	if (m_bService == true) return false;
	if (pIUnknownEx == NULL) return false;

	//���ýӿ�
	ASSERT(pIUnknownEx != NULL);
	m_pIQueueServiceSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IQueueServiceSink);
	ASSERT(m_pIQueueServiceSink != NULL);
	return (m_pIQueueServiceSink != NULL);
}

//��ʱ��֪ͨ
void CTimerEngine::OnTimerThreadSink()
{
	//��������
	CThreadLock lock(m_CriticalSection);//

	//��ѯ��ʱ��
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = m_TimerItemActive.GetCount() - 1; i >= 0; i--)
	{
		//Ч�����
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		if (pTimerItem == NULL) return;
		ASSERT(pTimerItem->dwTimeLeave > 0);
		
		//��ʱ������
		bool bKillTimer = false;
		pTimerItem->dwTimeLeave -= TIMER_SPACE;
		if (pTimerItem->dwTimeLeave == 0L)
		{
			try
			{
				BYTE cbBuffer[MAX_ASYNCHRONISM_DATA] = {0};		//���ջ���
				//Ͷ����Ϣ
				NTY_TimerEvent * pTimerEvent = (NTY_TimerEvent *)cbBuffer;
				pTimerEvent->dwTimerID = pTimerItem->wTimerID;
				pTimerEvent->dwBindParameter = pTimerItem->wBindParam;
				m_pIQueueServiceSink->OnQueueServiceSink(EVENT_TIMER, cbBuffer, sizeof(NTY_TimerEvent));
			}
			catch (...) { }

			//���ô���
			if (pTimerItem->dwRepeatTimes != TIMES_INFINITY)
			{
				ASSERT(pTimerItem->dwRepeatTimes > 0);
				pTimerItem->dwRepeatTimes--;
				if (pTimerItem->dwRepeatTimes == 0L)
				{
					bKillTimer = true;
					m_TimerItemActive.RemoveAt(i);
					m_TimerItemFree.Add(pTimerItem);
				}
			}

			//����ʱ��
			if (bKillTimer == false)//��ǰ20�����Ƚ���֪ͨ - TIMER_SPACE * 20
			{
				if (pTimerItem->dwRepeatTimes == 1)
					pTimerItem->dwTimeLeave = __max(TIMER_SPACE, pTimerItem->dwElapse - TIMER_SPACE * 20);
				else
					pTimerItem->dwTimeLeave = pTimerItem->dwElapse;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateTimerEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CTimerEngine * pTimerEngine = NULL;
	try
	{
		pTimerEngine = new CTimerEngine();
		if (pTimerEngine == NULL) throw TEXT("����ʧ��");
		void * pObject = pTimerEngine->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTimerEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
