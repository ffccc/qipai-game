#ifndef TIMERENGINE_HEAD_FILE
#define TIMERENGINE_HEAD_FILE

#pragma once

//ϵͳͷ�ļ�

#include "KernelEngineHead.h"
#include "QueueServiceEvent.h"

#define TIMER_SPACE								25				//ʱ����

//��˵��
class CTimerEngine;

//////////////////////////////////////////////////////////////////////////

//��ʱ���߳�
class CTimerThread : public CServiceThread
{
	//��������
protected:
	CTimerEngine						* m_pTimerEngine;				//��ʱ������
	//��������
public:
	//���캯��
	CTimerThread(void);
	//��������
	virtual ~CTimerThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(CTimerEngine * pTimerEngine);

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//��ʱ������
struct tagTimerItem
{
	DWORD								wTimerID;						//��ʱ�� ID
	DWORD								dwElapse;						//��ʱʱ��
	DWORD								dwTimeLeave;					//����ʱ��
	DWORD								dwRepeatTimes;					//�ظ�����
	WPARAM								wBindParam;						//�󶨲���
};

//��ʱ���������鶨��
typedef CArrayTemplate<tagTimerItem *> CTimerItemPtr;

//////////////////////////////////////////////////////////////////////////

//��ʱ������
class CTimerEngine : public ITimerEngine
{
	friend class CTimerThread;

	//״̬����
protected:
	bool								m_bService;						//���б�־
	CTimerItemPtr						m_TimerItemFree;				//��������
	CTimerItemPtr						m_TimerItemActive;				//�����

	//�������
protected:
	CCriticalSection					m_CriticalSection; //	//�߳���
	CTimerThread						m_TimerThread;					//��ʱ���߳�
	IQueueServiceSink					*m_pIQueueServiceSink;			//֪ͨ�ص�

	//��������
public:
	//���캯��
	CTimerEngine(void);
	//��������
	virtual ~CTimerEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release()
	{
		if (IsValid()) delete this;    //
	}
	//�Ƿ���Ч
	virtual bool __cdecl IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CTimerEngine)) ? true : false;
	}
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�ӿں���
public:
	//���ö�ʱ��
	virtual bool __cdecl SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//ɾ����ʱ��
	virtual bool __cdecl KillTimer(DWORD dwTimerID);
	//ɾ����ʱ��
	virtual bool __cdecl KillAllTimer();
	//���ýӿ�
	virtual bool __cdecl SetTimerEngineEvent(IUnknownEx * pIUnknownEx);

	//����ӿ�
public:
	//��ʼ����
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl ConcludeService();

	//�ڲ�����
private:
	//��ʱ��֪ͨ
	void OnTimerThreadSink();
};

//////////////////////////////////////////////////////////////////////////

#endif