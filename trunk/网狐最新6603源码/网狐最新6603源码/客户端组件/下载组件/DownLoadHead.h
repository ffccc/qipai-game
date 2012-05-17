#ifndef DOWN_LOAD_HEAD_HEAD_FILE
#define DOWN_LOAD_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//平台文件

//系统文件
#include "Wininet.h"

//平台文件
#include "..\..\全局定义\Platform.h"

//组件文件
#include "..\..\客户端组件\网络服务\WHSocketHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef DOWN_LOAD_CLASS
	#ifdef  DOWN_LOAD_DLL
		#define DOWN_LOAD_CLASS _declspec(dllexport)
	#else
		#define DOWN_LOAD_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define DOWN_LOAD_DLL_NAME		TEXT("DownLoad.dll")				//组件名字
#else
	#define DOWN_LOAD_DLL_NAME		TEXT("DownLoadD.dll")				//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//枚举定义

//错误状态
enum enDownLoadError
{
	DownLoadError_Server,			//服务错误
	DownLoadError_Connect,			//连接错误
};

//下载状态
enum enDownLoadStatus
{
	DownLoadStatus_Connect,			//连接状态
	DownLoadStatus_Service,			//服务状态
	DownLoadStatus_Conclude,		//完成状态
};

//////////////////////////////////////////////////////////////////////////////////

//请求信息
struct tagDownLoadInfo
{
	WORD							wHostPort;							//连接端口
	TCHAR							szUrlPath[MAX_PATH];				//请求路径
	TCHAR							szHostName[MAX_PATH];				//服务器名
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IWEBSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketSink={0x09843e16,0x0f5f,0x4997,0xb9,0x23,0xdb,0xf1,0x00,0x05,0xbb,0x46};
#else
	#define VER_IWEBSocketSink INTERFACE_VERSION(1,1)
	static const GUID IID_IWEBSocketSink={0x30c4a8e2,0xdb6b,0x4e5f,0xbb,0xdf,0x05,0xc9,0xc3,0x97,0x47,0xc3};
#endif

//下载回调
interface IWEBSocketSink : public IUnknownEx
{
	//状态接口
public:
	//连接消息
	virtual bool OnSocketLink(INT nErrorCode)=NULL;
	//关闭消息
	virtual bool OnSocketShut(BYTE cbShutReason)=NULL;

	//数据接口
public:
	//数据包流
	virtual VOID OnSocketDataMain(VOID * pcbMailData, WORD wStreamSize)=NULL;
	//数据包头
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

//下载回调
interface IDownLoadSink : public IUnknownEx
{
	//状态接口
public:
	//下载异常
	virtual bool OnDownLoadError(enDownLoadError DownLoadError)=NULL;
	//下载状态
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus)=NULL;

	//序列接口
public:
	//下载数据
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize)=NULL;
	//下载信息
	virtual bool OnDataInformation(DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#ifndef DOWN_LOAD_DLL
	#include "DownLoad.h"
	#include "WEBSocket.h"
	#include "HeadTranslate.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif
