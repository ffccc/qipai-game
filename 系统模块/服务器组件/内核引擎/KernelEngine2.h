#ifndef SERVER_KERNEL_HEAD_FILE2
#define SERVER_KERNEL_HEAD_FILE2
//////////////////////////////////////////////////////////////////////////
//包含文件

#include <Afxmt.h>
#include <Comutil.h>
#include <ICrsint.h>
#include <Process.h>
#include <WinSock2.h>

//平台文件
#include "..\..\模板库\Template.h"
#include "..\..\公共文件\Constant.h"
#include "..\..\公共文件\GlobalDef.h"
#include "..\..\共享组件\公共服务\ComService.h"

//////////////////////////////////////////////////////////////////////////
//ADO 定义

#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

typedef _com_error						CComError;						//COM 错误
typedef _variant_t						CDBVarValue;					//数据库数值




//////////////////////////////////////////////////////////////////////////
//导出定义

//导出定义
#ifndef KERNEL_ENGINE_CLASS
	#ifdef  KERNEL_ENGINE_DLL
		#define KERNEL_ENGINE_CLASS _declspec(dllexport)
	#else
		#define KERNEL_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define KERNEL_ENGINE_DLL_NAME	TEXT("KernelEngine.dll")			//组件 DLL 名字
#else
	#define KERNEL_ENGINE_DLL_NAME	TEXT("KernelEngineD.dll")			//组件 DLL 名字
#endif

//后来添加的start
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//常量宏定义
#define EVENT_LEVEL_COUNT			4									//事件等级
#define MAX_QUEUE_PACKET			10240								//最大队列
#define INDEX_ALL_SOCKET			0xFFFF								//所有连接
#define TIMER_REPEAT_TIMER			DWORD(-1)							//重复次数
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//后来添加的end

//
////输出等级
//enum enTraceLevel
//{
//	TraceLevel_Info					=0,									//信息消息
//	TraceLevel_Normal				=1,									//普通消息
//	TraceLevel_Warning				=2,									//警告消息
//	TraceLevel_Exception			=3,									//异常消息
//	TraceLevel_Debug				=4,									//调试消息
//};
//

//////////////////////////////////////////////////////////////////////////
//枚举定义

////输出等级
//enum enTraceLevel
//{
//	Level_Normal					=0,									//普通消息
//	Level_Warning					=1,									//警告消息
//	Level_Exception					=2,									//异常消息
//	Level_Debug						=3,									//调试消息
//};


//////////////////////////////////////////////////////////////////////////
//系统常量

//常量定义
#define TIME_CELL					200									//时间单元
#define TIMES_INFINITY				DWORD(-1)							//无限次数
#define MAX_ASYNCHRONISM_DATA		8192								//异步数据

//////////////////////////////////////////////////////////////////////////
//网络定义

//连接错误
#define CONNECT_SUCCESS				0									//连接成功
#define CONNECT_FAILURE				1									//连接失败
#define CONNECT_EXCEPTION			2									//参数异常

//关闭原因
#define SHUT_REASON_INSIDE			0									//内部原因
#define SHUT_REASON_NORMAL			1									//正常关闭
#define SHUT_REASON_REMOTE			2									//远程关闭
#define SHUT_REASON_TIME_OUT		3									//网络超时
#define SHUT_REASON_EXCEPTION		4									//异常关闭

//////////////////////////////////////////////////////////////////////////
//枚举定义

//输出等级
enum enTraceLevel
{
	TraceLevel_Info					=0,									//信息消息
	TraceLevel_Normal				=1,									//普通消息
	TraceLevel_Warning				=2,									//警告消息
	TraceLevel_Exception			=3,									//异常消息
	TraceLevel_Debug				=4,									//调试消息
};

//SQL 异常类型
enum enSQLException
{
	SQLException_None				=0,									//没有异常
	SQLException_Connect			=1,									//连接错误
	SQLException_Syntax				=2,									//语法错误
};

//连接状态
enum enTCPSocketStatus
{
	TCPSocketStatus_Idle,			//空闲状态
	TCPSocketStatus_Connecting,		//连接状态
	TCPSocketStatus_Connected,		//成功连接
};


//事件标识
#define EVENT_CONTROL				0x0001								//控制消息
#define EVENT_TIMER					0x0002								//定时器引擎


//定时器事件
struct NTY_TimerEvent
{
	WORD							wTimerID;							//定时器 ID
	WPARAM							wBindParam;							//绑定参数
};

//数据库请求事件
struct NTY_DataBaseEvent
{
	WORD							wIndex;								//对象索引
	WORD							wRoundID;							//对象标识
	WORD							wRequestID;							//请求标识
};


#endif