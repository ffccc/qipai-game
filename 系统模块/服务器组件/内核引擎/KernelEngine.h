#ifndef KERNEL_ENGINE_HEAD_FILE
#define KERNEL_ENGINE_HEAD_FILE

#pragma once

#define MAX_QUEUE_PACKET			10240	//最大队列
#define INDEX_ALL_SOCKET			0xFFFF								//所有连接
#define TIMER_REPEAT_TIMER			DWORD(-1)							//重复次数

//#define EVENT_SOCKET_ACCEPT			0x0004								//网络应答
//#define EVENT_SOCKET_READ			0x0005								//网络读取
//#define EVENT_SOCKET_CLOSE			0x0006								//网络关闭


//////////////////////////////////////////////////////////////////////////
//宏定义

#define TIME_BREAK_READY			9000L								//中断时间
#define TIME_BREAK_CONNECT			4000L								//中断时间
#define TIME_DETECT_SOCKET			20000L								//监测时间

//////////////////////////////////////////////////////////////////////////

//动作定义
#define QUEUE_SEND_REQUEST			1									//发送标识
#define QUEUE_SAFE_CLOSE			2									//安全关闭
#define QUEUE_ALLOW_BATCH			3									//允许群发
#define QUEUE_DETECT_SOCKET			4									//检测连接


#define SOCKET_PACKAGE				SOCKET_PACKET
//发送请求结构
struct tagSendDataRequest
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
	WORD							wDataSize;							//数据大小
	BYTE							cbSendBuf[SOCKET_PACKAGE];			//发送缓冲
};

//设置群发
struct tagAllowBatchSend
{
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
	bool							bAllow;							//允许标志
};

//安全关闭
struct tagSafeCloseSocket
{
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
};

//////////////////////////////////////////////////////////////////////////

////定时器事件
//struct NTY_TimerEvent
//{
//	WORD							wTimerID;							//定时器 ID
//	WPARAM							wBindParam;							//绑定参数
//};
//
////数据库请求事件
//struct NTY_DataBaseEvent
//{
//	WORD							wIndex;								//对象索引
//	WORD							wRoundID;							//对象标识
//	WORD							wRequestID;							//请求标识
//};
//
////网络应答事件
//struct NTY_SocketAcceptEvent
//{
//	WORD							wIndex;								//连接索引
//	WORD							wRoundID;							//连接标识
//	DWORD							dwClientIP;							//连接地址
//};
//
////网络读取事件
//struct NTY_SocketReadEvent
//{
//	WORD							wIndex;								//连接索引
//	WORD							wRoundID;							//连接标识
//	WORD							wDataSize;							//数据大小
//	CMD_Command						Command;							//命令信息
//};
//
////网络关闭事件
//struct NTY_SocketCloseEvent
//{
//	WORD							wIndex;								//连接索引
//	WORD							wRoundID;							//连接标识
//	DWORD							dwClientIP;							//连接地址
//	DWORD							dwConnectSecond;					//连接时间
//};




//网络应答事件
struct NTY_SocketAcceptEvent
{
	WORD							wIndex;								//连接索引
	WORD							wRoundID;							//连接标识
	DWORD							dwClientIP;							//连接地址
};

//网络读取事件
struct NTY_SocketReadEvent
{
	WORD							wIndex;								//连接索引
	WORD							wRoundID;							//连接标识
	WORD							wDataSize;							//数据大小
	CMD_Command						Command;							//命令信息
	//TCHAR							sDataBuffer[SOCKET_PACKET];
};

//网络关闭事件
struct NTY_SocketCloseEvent
{
	WORD							wIndex;								//连接索引
	WORD							wRoundID;							//连接标识
	DWORD							dwClientIP;							//连接地址
	DWORD							dwConnectSecond;					//连接时间
};

//登录服务器网络应答事件
struct NTY_DL_SocketLinkEvent
{
	WORD							wSocketID;							//连接索引
	INT								nErrorCode;							//错误号
};

//登录服务器网络应答事件
struct NTY_DL_SocketReadEvent
{
	WORD							wSocketID;							//连接索引
	WORD							wDataSize;							//数据大小
	CMD_Command						Command;							//命令信息
	//TCHAR							sDataBuffer[SOCKET_PACKET];
};
//登录服务器网络读取事件
struct NTY_DL_SocketCloseEvent
{
	WORD							wSocketID;							//连接索引
	BYTE							cbShutReason;							//连接地址
};


////数据库错误代码
//enum enADOErrorType
//{
//	ErrorType_Nothing				=0,									//没有错误
//	ErrorType_Connect				=1,									//连接错误
//	ErrorType_Other2					=2,									//其他错误
//};
//////////////////////////////////////////////////////////////////////////
//客户端socket定义
//宏定义

//请求标识
#define IREQ_CONNECT					1								//连接请求
#define IREQ_SEND_DATA					2								//发送请求
#define IREQ_CLOSE_SOCKET				3								//关闭请求

//////////////////////////////////////////////////////////////////////////
//结构体定义

//连接请求
struct tagReqConnect
{
	WORD								wPort;							//连接端口
	DWORD								dwServerIP;						//连接地址
};

//发送请求
struct tagReqSendData
{
	WORD								wMainCmdID;						//主命令码
	WORD								wSubCmdID;						//子命令码
	WORD								wDataSize;						//数据大小
	BYTE								cbDataBuffer[SOCKET_PACKAGE];	//发送数据
};

//关闭请求
struct tagReqCloseSocket
{
	bool								bNotify;						//是否通知
};
//////////////////////////////////////////////////////////////////////////
//
//#define VER_IADOError INTERFACE_VERSION(1,1)
//static const GUID IID_IADOError={0x66463b5a,0x390c,0x42f9,0x85,0x19,0x13,0x31,0x39,0x36,0xfe,0x8f};
//
////数据库错误接口
//interface IADOError : public IUnknownEx
//{
//	//错误描述
//	virtual LPCTSTR __cdecl GetErrorDescribe()=NULL;
//	//错误类型
//	virtual enADOErrorType __cdecl GetErrorType()=NULL;
//
//	////异常代码
//	//virtual HRESULT __cdecl GetExceptionResult()=NULL;
//	////异常描述
//	//virtual LPCTSTR __cdecl GetExceptionDescribe()=NULL;
//	////异常类型
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
////数据库钩子接口
//interface IDataBaseSink : public IUnknownEx
//{
//	//数据库模块启动
//	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx)=NULL;
//	//数据库模块关闭
//	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx)=NULL;
//	//数据操作处理
//	virtual bool __cdecl OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)=NULL;
//};

//////////////////////////////////////////////////////////////////////////

#define VER_IQueueService INTERFACE_VERSION(1,1)
static const GUID IID_IQueueService={0xcc5310b5,0x3709,0x40aa,0x85,0x24,0xd6,0xc5,0x87,0xb0,0x32,0x22};

//数据队列接口
interface IQueueService : public IUnknownEx
{
	//加入数据
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

//队列类引擎接口
interface IQueueServiceEngine : public IUnknownEx
{
	//开始服务
	virtual bool __cdecl StartService()=NULL;
	//停止服务
	virtual bool __cdecl StopService()=NULL;
	//设置接口
	virtual bool __cdecl SetQueueServiceSink(IUnknownEx * pIUnknownEx)=NULL;
	//负荷信息
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

//数据队列类钩子接口
interface IQueueServiceSink : public IUnknownEx
{
	//通知回调函数
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

//定时器引擎接口
interface ITimerEngineManager : public IUnknownEx
{
	//开始服务
	virtual bool __cdecl StartService()=NULL;
	//停止服务
	virtual bool __cdecl ConcludeService()=NULL;
	//设置接口
	//virtual bool __cdecl SetTimerEngineSink(IUnknownEx * pIUnknownEx)=NULL;
};


//////////////////////////////////////////////////////////////////////////
#endif