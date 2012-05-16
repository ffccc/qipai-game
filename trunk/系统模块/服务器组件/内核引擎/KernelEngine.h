#ifndef KERNEL_ENGINE_HEAD_FILE
#define KERNEL_ENGINE_HEAD_FILE

#pragma once

#define MAX_QUEUE_PACKET			10240	//������
#define INDEX_ALL_SOCKET			0xFFFF								//��������
#define TIMER_REPEAT_TIMER			DWORD(-1)							//�ظ�����

//#define EVENT_SOCKET_ACCEPT			0x0004								//����Ӧ��
//#define EVENT_SOCKET_READ			0x0005								//�����ȡ
//#define EVENT_SOCKET_CLOSE			0x0006								//����ر�


//////////////////////////////////////////////////////////////////////////
//�궨��

#define TIME_BREAK_READY			9000L								//�ж�ʱ��
#define TIME_BREAK_CONNECT			4000L								//�ж�ʱ��
#define TIME_DETECT_SOCKET			20000L								//���ʱ��

//////////////////////////////////////////////////////////////////////////

//��������
#define QUEUE_SEND_REQUEST			1									//���ͱ�ʶ
#define QUEUE_SAFE_CLOSE			2									//��ȫ�ر�
#define QUEUE_ALLOW_BATCH			3									//����Ⱥ��
#define QUEUE_DETECT_SOCKET			4									//�������


#define SOCKET_PACKAGE				SOCKET_PACKET
//��������ṹ
struct tagSendDataRequest
{
	WORD							wMainCmdID;							//��������
	WORD							wSubCmdID;							//��������
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	WORD							wDataSize;							//���ݴ�С
	BYTE							cbSendBuf[SOCKET_PACKAGE];			//���ͻ���
};

//����Ⱥ��
struct tagAllowBatchSend
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	bool							bAllow;							//�����־
};

//��ȫ�ر�
struct tagSafeCloseSocket
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
};

//////////////////////////////////////////////////////////////////////////

////��ʱ���¼�
//struct NTY_TimerEvent
//{
//	WORD							wTimerID;							//��ʱ�� ID
//	WPARAM							wBindParam;							//�󶨲���
//};
//
////���ݿ������¼�
//struct NTY_DataBaseEvent
//{
//	WORD							wIndex;								//��������
//	WORD							wRoundID;							//�����ʶ
//	WORD							wRequestID;							//�����ʶ
//};
//
////����Ӧ���¼�
//struct NTY_SocketAcceptEvent
//{
//	WORD							wIndex;								//��������
//	WORD							wRoundID;							//���ӱ�ʶ
//	DWORD							dwClientIP;							//���ӵ�ַ
//};
//
////�����ȡ�¼�
//struct NTY_SocketReadEvent
//{
//	WORD							wIndex;								//��������
//	WORD							wRoundID;							//���ӱ�ʶ
//	WORD							wDataSize;							//���ݴ�С
//	CMD_Command						Command;							//������Ϣ
//};
//
////����ر��¼�
//struct NTY_SocketCloseEvent
//{
//	WORD							wIndex;								//��������
//	WORD							wRoundID;							//���ӱ�ʶ
//	DWORD							dwClientIP;							//���ӵ�ַ
//	DWORD							dwConnectSecond;					//����ʱ��
//};




//����Ӧ���¼�
struct NTY_SocketAcceptEvent
{
	WORD							wIndex;								//��������
	WORD							wRoundID;							//���ӱ�ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
};

//�����ȡ�¼�
struct NTY_SocketReadEvent
{
	WORD							wIndex;								//��������
	WORD							wRoundID;							//���ӱ�ʶ
	WORD							wDataSize;							//���ݴ�С
	CMD_Command						Command;							//������Ϣ
	//TCHAR							sDataBuffer[SOCKET_PACKET];
};

//����ر��¼�
struct NTY_SocketCloseEvent
{
	WORD							wIndex;								//��������
	WORD							wRoundID;							//���ӱ�ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwConnectSecond;					//����ʱ��
};

//��¼����������Ӧ���¼�
struct NTY_DL_SocketLinkEvent
{
	WORD							wSocketID;							//��������
	INT								nErrorCode;							//�����
};

//��¼����������Ӧ���¼�
struct NTY_DL_SocketReadEvent
{
	WORD							wSocketID;							//��������
	WORD							wDataSize;							//���ݴ�С
	CMD_Command						Command;							//������Ϣ
	//TCHAR							sDataBuffer[SOCKET_PACKET];
};
//��¼�����������ȡ�¼�
struct NTY_DL_SocketCloseEvent
{
	WORD							wSocketID;							//��������
	BYTE							cbShutReason;							//���ӵ�ַ
};


////���ݿ�������
//enum enADOErrorType
//{
//	ErrorType_Nothing				=0,									//û�д���
//	ErrorType_Connect				=1,									//���Ӵ���
//	ErrorType_Other2					=2,									//��������
//};
//////////////////////////////////////////////////////////////////////////
//�ͻ���socket����
//�궨��

//�����ʶ
#define IREQ_CONNECT					1								//��������
#define IREQ_SEND_DATA					2								//��������
#define IREQ_CLOSE_SOCKET				3								//�ر�����

//////////////////////////////////////////////////////////////////////////
//�ṹ�嶨��

//��������
struct tagReqConnect
{
	WORD								wPort;							//���Ӷ˿�
	DWORD								dwServerIP;						//���ӵ�ַ
};

//��������
struct tagReqSendData
{
	WORD								wMainCmdID;						//��������
	WORD								wSubCmdID;						//��������
	WORD								wDataSize;						//���ݴ�С
	BYTE								cbDataBuffer[SOCKET_PACKAGE];	//��������
};

//�ر�����
struct tagReqCloseSocket
{
	bool								bNotify;						//�Ƿ�֪ͨ
};
//////////////////////////////////////////////////////////////////////////
//
//#define VER_IADOError INTERFACE_VERSION(1,1)
//static const GUID IID_IADOError={0x66463b5a,0x390c,0x42f9,0x85,0x19,0x13,0x31,0x39,0x36,0xfe,0x8f};
//
////���ݿ����ӿ�
//interface IADOError : public IUnknownEx
//{
//	//��������
//	virtual LPCTSTR __cdecl GetErrorDescribe()=NULL;
//	//��������
//	virtual enADOErrorType __cdecl GetErrorType()=NULL;
//
//	////�쳣����
//	//virtual HRESULT __cdecl GetExceptionResult()=NULL;
//	////�쳣����
//	//virtual LPCTSTR __cdecl GetExceptionDescribe()=NULL;
//	////�쳣����
//	//virtual enSQLException __cdecl GetExceptionType()=NULL;
//};
//////////////////////////////////////////////////////////////////////////

//#ifdef _UNICODE
//	#define VER_IDataBaseSink INTERFACE_VERSION(1,1)
//	static const GUID IID_IDataBaseSink={0x5852e135,0x18cf,0x4893,0xa2,0x19,0x49,0x51,0x99,0xa2,0xf4,0xa5};
//#else
//	#define VER_IDataBaseSink INTERFACE_VERSION(1,1)
//	static const GUID IID_IDataBaseSink={0x4ba5fef5,0x0fe4,0x4b3f,0x8f,0x28,0x79,0x6a,0x16,0x24,0x5b,0xd6};
//#endif
//
////���ݿ⹳�ӽӿ�
//interface IDataBaseSink : public IUnknownEx
//{
//	//���ݿ�ģ������
//	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx)=NULL;
//	//���ݿ�ģ��ر�
//	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx)=NULL;
//	//���ݲ�������
//	virtual bool __cdecl OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)=NULL;
//};

//////////////////////////////////////////////////////////////////////////

#define VER_IQueueService INTERFACE_VERSION(1,1)
static const GUID IID_IQueueService={0xcc5310b5,0x3709,0x40aa,0x85,0x24,0xd6,0xc5,0x87,0xb0,0x32,0x22};

//���ݶ��нӿ�
interface IQueueService : public IUnknownEx
{
	//��������
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)=NULL;
};
//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IQueueServiceEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IQueueServiceEngine={0xba9e9d45,0x81c8,0x4a18,0x88,0x86,0x22,0xa7,0x19,0x1c,0x8b,0x54};
#else
	#define VER_IQueueServiceEngine INTERFACE_VERSION(1,1)
	static const GUID IID_IQueueServiceEngine={0x3d205995,0x427b,0x4b30,0xaf,0xfe,0x7e,0x90,0xe4,0x77,0x80,0xc2};
#endif

//����������ӿ�
interface IQueueServiceEngine : public IUnknownEx
{
	//��ʼ����
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
	//���ýӿ�
	virtual bool __cdecl SetQueueServiceSink(IUnknownEx * pIUnknownEx)=NULL;
	//������Ϣ
	virtual bool __cdecl GetBurthenInfo(tagBurthenInfo & BurthenInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IQueueServiceSink INTERFACE_VERSION(1,1)
	static const GUID IID_IQueueServiceSink={0xbd0d29c2,0x5e92,0x4d8d,0x99,0x2c,0x74,0x3f,0xd6,0x7e,0x74,0x1f};
#else
	#define VER_IQueueServiceSink INTERFACE_VERSION(1,1)
	static const GUID IID_IQueueServiceSink={0x4d2c22dc,0xbcae,0x47e7,0x94,0x41,0x56,0x82,0x53,0x6d,0xa2,0xf0};
#endif

//���ݶ����๳�ӽӿ�
interface IQueueServiceSink : public IUnknownEx
{
	//֪ͨ�ص�����
	virtual void __cdecl OnQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITimerEngineManager INTERFACE_VERSION(1,1)
	static const GUID IID_ITimerEngineManager={0xac341f6d,0x1106,0x4e4f,0xbc,0x8f,0x87,0xd0,0x63,0x5f,0xe1,0xbc};
#else
	#define VER_ITimerEngineManager INTERFACE_VERSION(1,1)
	static const GUID IID_ITimerEngineManager={0x26f7f428,0x7196,0x4517,0xb8,0x79,0x1e,0x43,0x01,0x74,0xe2,0x07};
#endif

//��ʱ������ӿ�
interface ITimerEngineManager : public IUnknownEx
{
	//��ʼ����
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl ConcludeService()=NULL;
	//���ýӿ�
	//virtual bool __cdecl SetTimerEngineSink(IUnknownEx * pIUnknownEx)=NULL;
};


//////////////////////////////////////////////////////////////////////////
#endif