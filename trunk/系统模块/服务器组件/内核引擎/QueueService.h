#ifndef QUEUE_SERVICE_HEAD_FILE
#define QUEUE_SERVICE_HEAD_FILE

#include "KernelEngineHead.h"


#define VER_IQueueServiceSink INTERFACE_VERSION(1,1)
static const GUID IID_IQueueServiceSink = {0x88b5bf59, 0x3a98, 0x43b1, 0xac, 0x85, 0xf1, 0x17, 0x5b, 0x29, 0x69, 0x40};

//���ݶ����๳�ӽӿ�
interface IQueueServiceSink : public IUnknownEx
{
	//֪ͨ�ص�����
	virtual void __cdecl OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize) = NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IQueueService INTERFACE_VERSION(1,1)
static const GUID IID_IQueueService = {0xcc5310b5, 0x3709, 0x40aa, 0x85, 0x24, 0xd6, 0xc5, 0x87, 0xb0, 0x32, 0x22};

//���нӿ�
interface IQueueService : public IServiceModule
{
	//���ýӿ�
	virtual bool __cdecl SetQueueServiceSink(IUnknownEx * pIUnknownEx) = NULL;
	//������Ϣ
	virtual bool __cdecl GetBurthenInfo(tagBurthenInfo & BurthenInfo) = NULL;
	//��������
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize) = NULL;
};

//////////////////////////////////////////////////////////////////////////

//�����߳���
class CQueueServiceThread : public CServiceThread
{
	//��Ա����
	friend class CQueueService;

	//���ö���
protected:
	HANDLE							m_hCompletionPort;					//��ɶ˿�

	//��������
private:
	BYTE							m_cbBuffer[MAX_ASYNCHRONISM_DATA];		//���ջ���

	//��������
protected:
	//���캯��
	CQueueServiceThread(void);
	//��������
	virtual ~CQueueServiceThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(HANDLE hCompletionPort);
	//ȡ������
	bool UnInitThread();

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//���ݶ�����
class CQueueService : public IQueueService
{
	friend class CQueueServiceThread;

	//��������
protected:
	bool							m_bService;							//�����־
	HANDLE							m_hCompletionPort;					//��ɶ˿�
	IQueueServiceSink				* m_pIQueueServiceSink;				//�ص��ӿ�

	//�������
protected:
	CCriticalSection				m_CriticalSection;					//�߳���
	CDataQueue						m_DataQueue;						//���ݴ洢
	CQueueServiceThread				m_QueueServiceThread;				//�����߳�

	//��������
public:
	//���캯��
	CQueueService(void);
	//��������
	virtual ~CQueueService(void);

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
		return AfxIsValidAddress(this, sizeof(CQueueService)) ? true : false;
	}
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);


	//����ӿ�
public:
	//��ʼ����
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl ConcludeService();
	//�߳̾��
	HANDLE GetThreadHandle();

	//���нӿ�
public:
	//��������
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize);
	//���ýӿ�
	virtual bool __cdecl SetQueueServiceSink(IUnknownEx * pIUnknownEx);
	//������Ϣ
	virtual bool __cdecl GetBurthenInfo(tagBurthenInfo & BurthenInfo);

	//��������
private:
	//��ȡ����
	bool GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize);
	//������Ϣ
	void OnQueueServiceThread(const tagDataHead & DataHead, void * pBuffer, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif