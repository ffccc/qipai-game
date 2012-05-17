#ifndef DOWN_LOAD_HEAD_HEAD_FILE
#define DOWN_LOAD_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//ƽ̨�ļ�

//ϵͳ�ļ�
#include "Wininet.h"

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//����ļ�
#include "..\..\�ͻ������\�������\WHSocketHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef DOWN_LOAD_CLASS
	#ifdef  DOWN_LOAD_DLL
		#define DOWN_LOAD_CLASS _declspec(dllexport)
	#else
		#define DOWN_LOAD_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define DOWN_LOAD_DLL_NAME		TEXT("DownLoad.dll")				//�������
#else
	#define DOWN_LOAD_DLL_NAME		TEXT("DownLoadD.dll")				//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����״̬
enum enDownLoadError
{
	DownLoadError_Server,			//�������
	DownLoadError_Connect,			//���Ӵ���
};

//����״̬
enum enDownLoadStatus
{
	DownLoadStatus_Connect,			//����״̬
	DownLoadStatus_Service,			//����״̬
	DownLoadStatus_Conclude,		//���״̬
};

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagDownLoadInfo
{
	WORD							wHostPort;							//���Ӷ˿�
	TCHAR							szUrlPath[MAX_PATH];				//����·��
	TCHAR							szHostName[MAX_PATH];				//��������
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IWEBSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketSink={0x09843e16,0x0f5f,0x4997,0xb9,0x23,0xdb,0xf1,0x00,0x05,0xbb,0x46};
#else
	#define VER_IWEBSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketSink={0x30c4a8e2,0xdb6b,0x4e5f,0xbb,0xdf,0x05,0xc9,0xc3,0x97,0x47,0xc3};
#endif

//���ػص�
interface IWEBSocketSink : public IUnknownEx
{
	//״̬�ӿ�
public:
	//������Ϣ
	virtual bool OnSocketLink(INT nErrorCode)=NULL;
	//�ر���Ϣ
	virtual bool OnSocketShut(BYTE cbShutReason)=NULL;

	//���ݽӿ�
public:
	//���ݰ���
	virtual VOID OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize)=NULL;
	//���ݰ�ͷ
	virtual VOID OnSocketDataHead(VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IDownLoadSink INTERFACE_VERSION(1,1)
	static const GUID IID_IDownLoadSink={0xb98886a0,0x3b99,0x401d,0x009b,0xf9,0x56,0x04,0xa8,0x59,0x7b,0x76};
#else
	#define VER_IDownLoadSink INTERFACE_VERSION(1,1)
	static const GUID IID_IDownLoadSink={0x419b6da4,0x13bb,0x4463,0x00a5,0x4d,0x4a,0xe2,0xcb,0x35,0xb9,0x81};
#endif

//���ػص�
interface IDownLoadSink : public IUnknownEx
{
	//״̬�ӿ�
public:
	//�����쳣
	virtual bool OnDownLoadError(enDownLoadError DownLoadError)=NULL;
	//����״̬
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus)=NULL;

	//���нӿ�
public:
	//��������
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize)=NULL;
	//������Ϣ
	virtual bool OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef DOWN_LOAD_DLL
	#include "DownLoad.h"
	#include "WEBSocket.h"
	#include "HeadTranslate.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif
