#ifndef ATTEMPER_ENGINE_HEAD_FILE
#define ATTEMPER_ENGINE_HEAD_FILE

#pragma once

//���ͷ�ļ�

#include "KernelEngineHead.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//���ȹ�����
class CAttemperEngine : public IAttemperEngine, public IQueueServiceSink,
		public IDataBaseEngineEvent, public ITCPNetworkEngineEvent
{
	//���ı���
protected:
	bool							m_bService;							//���б�־
	CQueueService					m_QueueService;						//���ж���
	CCriticalSection				m_CriticalSection;					//ͬ������

	//�ӿڱ���
protected:
	ITCPSocketService				* m_pITCPSocketService;				//��������
	ITCPNetworkEngine				* m_pTCPNetworkEngine;				//��������
	IAttemperEngineSink				* m_pIAttemperEngineSink;			//�ҽӽӿ�

	//����������ά��
protected:
	CArrayTemplate<DWORD>			m_AndroidUserSocketIDArray;					//����������

	//��������
public:
	//���캯��
	CAttemperEngine(void);
	//��������
	virtual ~CAttemperEngine(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release()
	{
		if (IsValid()) delete this;
	}
	//�Ƿ���Ч
	virtual bool __cdecl IsValid()
	{
		return AfxIsValidAddress(this, sizeof(CAttemperEngine)) ? true : false;
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
public:
	//��������
	virtual bool __cdecl SetNetworkEngine(IUnknownEx * pIUnknownEx);
	//ע�ṳ��
	virtual bool __cdecl SetAttemperEngineSink(IUnknownEx * pIUnknownEx);
	//�Զ��¼�
	virtual bool __cdecl OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool __cdecl OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize);

	//���нӿ�
public:
	//�����ӿ�
	virtual void __cdecl OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize/*, DWORD dwInsertTime*/);

	//�ӿڶ���
public:
	//���ݿ���
	virtual bool __cdecl OnEventDataBaseResult(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//����ģ��ӿ�
public:
	//Ӧ���¼�
	virtual bool __cdecl OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientIP);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientIP, DWORD dwActiveTime);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPNetworkRead(DWORD dwSocketID, CMD_Command Command, VOID * pData, WORD wDataSize);

public:
	//��������
	virtual bool __cdecl SetSocketEngine(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual void * __cdecl GetQueueService(const IID & Guid, DWORD dwQueryVer);
};

//////////////////////////////////////////////////////////////////////////

#endif