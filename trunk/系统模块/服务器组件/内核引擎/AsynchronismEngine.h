#ifndef ASYNCHRONISM_ENGINE_HEAD_FILE
#define ASYNCHRONISM_ENGINE_HEAD_FILE

#pragma once


#include "KernelEngineHead.h"

//��˵��
class CAsynchronismEngine;
typedef class CArrayTemplate<IAsynchronismEngineSink *> CAsynchronismEngineSinkArray;

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
	//��������
public:
	CAsynchronismEngine				* m_pAsynchronismEngine;			//�첽����

	//��������
public:
	//���캯��
	CControlWnd();
	//��������
	virtual ~CControlWnd();

	//��Ϣӳ��
protected:
	//������Ϣ
	HRESULT OnAsynRequest(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//��Ϣ�߳�
class CMessageThread : public CServiceThread
{
	//��������
public:
	CAsynchronismEngine				* m_pAsynchronismEngine;			//�첽����

	//��������
public:
	//���캯��
	CMessageThread();
	//��������
	virtual ~CMessageThread();

	//���ܺ���
public:
	//ֹͣ�߳�
	virtual bool StopThread(DWORD dwWaitSeconds = INFINITE);
	//��ֹ�߳�
	virtual bool TerminateThread(DWORD dwExitCode);

	//�¼�����
private:
	//��ʼ�¼�
	virtual bool OnThreadStratEvent();
	//ֹͣ�¼�
	virtual bool OnThreadStopEvent();

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//�첽����
class CAsynchronismEngine : public IAsynchronismEngine
{
	friend class CControlWnd;
	friend class CMessageThread;

	//�ں˱���
protected:
	bool							m_bService;							//�����־
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];	//���ݻ���
	CAsynchronismEngineSinkArray	m_AsynchronismEngineSinkArray;		//�첽����

	//�������
protected:
	CControlWnd						m_ControlWnd;						//���ƴ���
	CCriticalSection				m_CriticalSection;					//�߳���
	CDataQueue						m_DataStorage;						//���ݴ洢
	CMessageThread					m_MessageThread;					//�߳����

	//��������
public:
	//���캯��
	CAsynchronismEngine(void);
	//��������
	virtual ~CAsynchronismEngine(void);

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
		return AfxIsValidAddress(this, sizeof(CAsynchronismEngine)) ? true : false;
	}
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl ConcludeService();
	//��������
	virtual bool __cdecl InsertRequest(WORD wRequestID, void * const pBuffer, WORD wDataSize, IUnknownEx * pIUnknownEx);

	//���ܽӿ�
public:
	//ע�ṳ��
	virtual bool __cdecl SetAsynchronismSink(IUnknownEx * pIUnknownEx);
	//Ͷ������
	virtual bool __cdecl PostAsynchronismData(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//ȡ��ע��
	virtual bool __cdecl UnRegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx);

	//�̺߳���
protected:
	//�߳�����
	bool OnMessageThreadStart();
	//�߳�ֹͣ
	bool OnMessageThreadStop();
	//�첽����
	void OnAsynchronismRequest(WORD wRequestID, IAsynchronismEngineSink * pIAsynchronismEngineSink);
};

//////////////////////////////////////////////////////////////////////////

#endif