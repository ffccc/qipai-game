#ifndef PLATFORM_DATA_HEAD_HEAD_FILE
#define PLATFORM_DATA_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//平台文件

//平台文件
#include "..\..\全局定义\Platform.h"

//类头文件
#include "..\图像控件\WHImageHead.h"
#include "..\网络服务\WHSocketHead.h"
#include "..\界面控件\SkinControlHead.h"
#include "..\形象组件\AvatarControlHead.h"
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef PLATFORM_DATA_CLASS
	#ifdef  PLATFORM_DATA_DLL
		#define PLATFORM_DATA_CLASS _declspec(dllexport)
	#else
		#define PLATFORM_DATA_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define PLATFORM_DATA_DLL_NAME	TEXT("PlatformData.dll")			//组件 DLL 名字
#else
	#define PLATFORM_DATA_DLL_NAME	TEXT("PlatformDataD.dll")			//组件 DLL 名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#ifndef PLATFORM_DATA_DLL
	#include "GlobalServer.h"
	#include "GlobalUserInfo.h"
	#include "OptionParameter.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif