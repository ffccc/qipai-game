#ifndef MODULE_MANAGER_HEAD_HEAD_FILE
#define MODULE_MANAGER_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//平台定义
#include "..\..\全局定义\Platform.h"

//组件定义
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
#include "..\..\服务器组件\游戏服务\GameServiceHead.h"
#include "..\..\服务器组件\内核引擎\KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////
//导出定义

//导出定义
#ifndef MODULE_MANAGER_CLASS
	#ifdef  MODULE_MANAGER_DLL
		#define MODULE_MANAGER_CLASS _declspec(dllexport)
	#else
		#define MODULE_MANAGER_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define MODULE_MANAGER_DLL_NAME	TEXT("ModuleManager.dll")			//组件 DLL 名字
#else
	#define MODULE_MANAGER_DLL_NAME	TEXT("ModuleManagerD.dll")			//组件 DLL 名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//模块信息
struct tagGameModuleInfo
{
	//版本信息
	DWORD							dwClientVersion;					//游戏版本
	DWORD							dwServerVersion;					//服务版本
	DWORD							dwNativeVersion;					//本地版本

	//数据属性
	WORD							wGameID;							//模块标识
	TCHAR							szDataBaseAddr[15];					//数据库名
	TCHAR							szDataBaseName[32];					//数据库名

	//模块属性
	TCHAR							szGameName[LEN_KIND];				//游戏名字
	TCHAR							szServerDLLName[LEN_PROCESS];		//进程名字
	TCHAR							szClientEXEName[LEN_PROCESS];		//进程名字
};

//房间信息
struct tagGameServerInfo
{
	//索引变量
	WORD							wGameID;							//模块标识
	WORD							wServerID;							//房间号码

	//挂接属性
	WORD							wKindID;							//挂接类型
	WORD							wNodeID;							//挂接节点
	WORD							wSortID;							//排列标识

	//版本信息
	DWORD							dwClientVersion;					//游戏版本
	DWORD							dwServerVersion;					//服务版本
	DWORD							dwNativeVersion;					//本地版本

	//模块属性
	TCHAR							szGameName[LEN_KIND];				//游戏名字
	TCHAR							szServerDLLName[LEN_PROCESS];		//进程名字
	TCHAR							szClientEXEName[LEN_PROCESS];		//进程名字

	//税收配置
	LONG							lCellScore;							//单位积分
	BYTE							cbRevenueRatio;						//税收比例

	//房间配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分

	//会员限制
	BYTE							cbMinEnterMember;					//最低会员
	BYTE							cbMaxEnterMember;					//最高会员

	//房间配置
	DWORD							dwServerRule;						//房间规则
	DWORD							dwAttachUserRight;					//附加权限

	//房间属性
	WORD							wMaxPlayer;							//最大数目
	WORD							wTableCount;						//桌子数目
	WORD							wServerPort;						//服务端口
	WORD							wServerType;						//房间类型
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//分组设置
	BYTE							cbDistributeRule;					//分组规则
	WORD							wMinDistributeUser;					//最少人数
	WORD							wMaxDistributeUser;					//最多人数
	WORD							wDistributeTimeSpace;				//分组间隔
	WORD							wDistributeDrawCount;				//分组局数
	WORD							wDistributeStartDelay;				//开始延时

	//数据属性
	TCHAR							szDataBaseName[32];					//数据库名
	TCHAR							szDataBaseAddr[32];					//地址信息

	//数据设置
	BYTE							cbCustomRule[256];					//自定规则
	TCHAR							szServiceMachine[LEN_MACHINE_ID];	//机器序列
};

//房间信息
struct tagGameServerCreate
{
	//索引变量
	WORD							wGameID;							//模块标识
	WORD							wServerID;							//房间号码

	//挂接属性
	WORD							wKindID;							//挂接类型
	WORD							wNodeID;							//挂接节点
	WORD							wSortID;							//排列标识

	//税收配置
	LONG							lCellScore;							//单位积分
	BYTE							cbRevenueRatio;						//税收比例

	//限制配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分

	//会员限制
	BYTE							cbMinEnterMember;					//最低会员
	BYTE							cbMaxEnterMember;					//最高会员

	//房间配置
	DWORD							dwServerRule;						//房间规则
	DWORD							dwAttachUserRight;					//附加权限

	//房间属性
	WORD							wMaxPlayer;							//最大数目
	WORD							wTableCount;						//桌子数目
	WORD							wServerPort;						//服务端口
	WORD							wServerType;						//房间类型
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//分组设置
	BYTE							cbDistributeRule;					//分组规则
	WORD							wMinDistributeUser;					//最少人数
	WORD							wMaxDistributeUser;					//最多人数
	WORD							wDistributeTimeSpace;				//分组间隔
	WORD							wDistributeDrawCount;				//分组局数
	WORD							wDistributeStartDelay;				//开始延时

	//数据属性
	TCHAR							szDataBaseName[32];					//数据库名
	TCHAR							szDataBaseAddr[32];					//地址信息

	//数据设置
	BYTE							cbCustomRule[256];					//自定规则
	TCHAR							szServiceMachine[LEN_MACHINE_ID];	//机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//配置参数
struct tagModuleInitParameter
{
	tagGameServiceAttrib			GameServiceAttrib;					//服务属性
	tagGameServiceOption			GameServiceOption;					//服务配置
};

//////////////////////////////////////////////////////////////////////////////////

//导出文件
#ifndef MODULE_MANAGER_DLL 
	
	#include "ListControl.h"
	#include "DlgServerItem.h"
	#include "DlgServerWizard.h"
	
	#include "ModuleListControl.h"
	#include "ModuleDBParameter.h"
	#include "ModuleInfoManager.h"

	#include "ServerCustomRule.h"
	#include "ServerListControl.h"
	#include "ServerInfoManager.h"

#endif

//////////////////////////////////////////////////////////////////////////////////

#endif