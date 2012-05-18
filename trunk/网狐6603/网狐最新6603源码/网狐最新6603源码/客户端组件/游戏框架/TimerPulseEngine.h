#ifndef TIMER_PULSE_ENGINE_HEAD_FILE
#define TIMER_PULSE_ENGINE_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CTimerPulseEngine;

//////////////////////////////////////////////////////////////////////////////////

//�����߳�
class GAME_FRAME_CLASS CTimerPulseThread : public CWHThread
{
	//��������
protected:
	DWORD							m_dwLastTickCount;					//����ʱ��
	DWORD							m_dwTimerPulseSpace;				//ʱ����

	//���ָ��
protected:
	CTimerPulseEngine *				m_pTimerPulseEngine;				//ʱ������

	//��������
public:
	//���캯��
	CTimerPulseThread();
	//��������
	virtual ~CTimerPulseThread();

	//���ܺ���
public:
	//���ú���
	bool InitThread(CTimerPulseEngine * pTimerPulseEngine, DWORD dwTimerPulseSpace);

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////////////

//ʱ������
class GAME_FRAME_CLASS CTimerPulseEngine
{
	friend class CTimerPulseThread;

	//״̬����
protected:
	bool							m_bService;							//���б�־
	DWORD							m_dwTimePass;						//����ʱ��
	DWORD							m_dwTimeLeave;						//����ʱ��

	//���ö���
protected:
	HWND							m_hWndKernel;						//�ں˴���
	DWORD							m_dwTimerPulseSpace;				//ʱ����

	//�������
protected:
	CWHArray<WORD>					m_TimerPulseItem;					//��������
	CCriticalSection				m_CriticalSection;					//��������
	CTimerPulseThread				m_TimerPulseThread;					//�̶߳���

	//��������
public:
	//���캯��
	CTimerPulseEngine();
	//��������
	virtual ~CTimerPulseEngine();

	//����ӿ�
public:
	//��������
	bool StartService();
	//ֹͣ����
	bool ConcludeService();

	//���ýӿ�
public:
	//���ýӿ�
	VOID SetKernelWnd(HWND hWndKernel) { m_hWndKernel=hWndKernel; }

	//�ӿں���
public:
	//ֹͣ����
	bool StopTimerPulse();
	//ֹͣ����
	bool StopTimerPulse(WORD wPulseID);
	//��������
	bool StartTimerPulse(WORD wPulseID);

	//�ڲ�����
private:
	//����֪ͨ
	VOID OnTimerPulseThreadSink();
};

//////////////////////////////////////////////////////////////////////////////////

#endif