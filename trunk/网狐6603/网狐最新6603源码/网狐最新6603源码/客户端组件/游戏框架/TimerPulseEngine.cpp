#include "StdAfx.h"
#include "TimerPulseEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//�궨��
#define PULSE_TIMER					1000L								//����ʱ��
#define NO_TIME_LEAVE				DWORD(-1)							//����ʱ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTimerPulseThread::CTimerPulseThread()
{
	//���ñ���
	m_dwLastTickCount=0L;
	m_dwTimerPulseSpace=PULSE_TIMER;

	//����ӿ�
	m_pTimerPulseEngine=NULL;

	return;
}

//��������
CTimerPulseThread::~CTimerPulseThread()
{
}

//���ú���
bool CTimerPulseThread::InitThread(CTimerPulseEngine * pTimerPulseEngine, DWORD dwTimerPulseSpace)
{
	//���ñ���
	m_dwLastTickCount=0L;
	m_dwTimerPulseSpace=dwTimerPulseSpace;

	//����ָ��
	m_pTimerPulseEngine=pTimerPulseEngine;

	return true;
}

//���к���
bool CTimerPulseThread::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_pTimerPulseEngine!=NULL);

	//��ȡʱ��
	DWORD dwNowTickCount=GetTickCount();
	DWORD dwTimerPulseSpace=m_dwTimerPulseSpace;

	//�ȴ�����
	if ((m_dwLastTickCount!=0L)&&(dwNowTickCount>m_dwLastTickCount))
	{
		DWORD dwHandleTickCount=dwNowTickCount-m_dwLastTickCount;
		dwTimerPulseSpace=(dwTimerPulseSpace>dwHandleTickCount)?(dwTimerPulseSpace-dwHandleTickCount):0L;
	}

	//��ʱ����
	Sleep(dwTimerPulseSpace);
	m_dwLastTickCount=GetTickCount();

	//ʱ�䴦��
	m_pTimerPulseEngine->OnTimerPulseThreadSink();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTimerPulseEngine::CTimerPulseEngine()
{
	//���ñ���
	m_bService=false;

	//״̬����
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEAVE;

	//���ñ���
	m_hWndKernel=NULL;
	m_dwTimerPulseSpace=PULSE_TIMER;

	return;
}

//��������
CTimerPulseEngine::~CTimerPulseEngine()
{
	//ֹͣ����
	ConcludeService();

	return;
}

//��ʼ����
bool CTimerPulseEngine::StartService()
{
	//״̬Ч��
	ASSERT(m_bService==false);
	if (m_bService==true) return false;

	//���ñ���
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEAVE;
	if (m_TimerPulseThread.InitThread(this,m_dwTimerPulseSpace)==false) return false;

	//��������
	if (m_TimerPulseThread.StartThread()==false) return false;

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool CTimerPulseEngine::ConcludeService()
{
	//���ñ���
	m_bService=false;

	//ɾ������
	StopTimerPulse();

	//ֹͣ�߳�
	m_TimerPulseThread.ConcludeThread(INFINITE);

	return true;
}

//ɾ������
bool CTimerPulseEngine::StopTimerPulse()
{
	//������Դ
	CWHDataLocker DataLocker(m_CriticalSection);

	//���ñ���
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEAVE;

	//ɾ������
	m_TimerPulseItem.RemoveAll();

	return true;
}

//ɾ��ʱ��
bool CTimerPulseEngine::StopTimerPulse(WORD wPulseID)
{
	//������Դ
	CWHDataLocker DataLocker(m_CriticalSection);

	//��������
	for (INT_PTR i=0;i<m_TimerPulseItem.GetCount();i++)
	{
		if (m_TimerPulseItem[i]==wPulseID)
		{
			//���ö���
			m_TimerPulseItem.RemoveAt(i);

			//ϵͳ����
			if (m_TimerPulseItem.GetCount()==0)
			{
				m_dwTimePass=0L;
				m_dwTimeLeave=NO_TIME_LEAVE;
			}

			return true;
		}
	}

	return false;
}

//����ʱ��
bool CTimerPulseEngine::StartTimerPulse(WORD wPulseID)
{
	//������Դ
	CWHDataLocker DataLocker(m_CriticalSection);

	//��������
	bool bPulseExist=false;

	//��������
	for (INT_PTR i=0;i<m_TimerPulseItem.GetCount();i++)
	{
		if (m_TimerPulseItem[i]==wPulseID)
		{
			bPulseExist=true;
			break;
		}
	}

	//��������
	if (bPulseExist==false)
	{
		m_TimerPulseItem.Add(wPulseID);
	}

	//ϵͳ����
	m_dwTimePass=0L;
	m_dwTimeLeave=PULSE_TIMER;

	return false;
}

//����֪ͨ
VOID CTimerPulseEngine::OnTimerPulseThreadSink()
{
	//������Դ
	CWHDataLocker DataLocker(m_CriticalSection);

	//����ʱ��
	if (m_dwTimeLeave==NO_TIME_LEAVE)
	{
		ASSERT(m_TimerPulseItem.GetCount()==0);
		return;
	}

	//Ч��״̬
	ASSERT(m_dwTimeLeave>=m_dwTimerPulseSpace);
	if (m_dwTimeLeave<m_dwTimerPulseSpace) m_dwTimeLeave=m_dwTimerPulseSpace;

	//����ʱ��
	m_dwTimePass+=m_dwTimerPulseSpace;
	m_dwTimeLeave-=m_dwTimerPulseSpace;

	//��ѯ����
	if (m_dwTimeLeave==0)
	{
		//����֪ͨ
		for (INT_PTR i=0;i<m_TimerPulseItem.GetCount();i++)
		{
			PostMessage(m_hWndKernel,WM_TIMER,m_TimerPulseItem[i],0L);
		}

		//ϵͳ����
		m_dwTimePass=0L;
		m_dwTimeLeave=PULSE_TIMER;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
