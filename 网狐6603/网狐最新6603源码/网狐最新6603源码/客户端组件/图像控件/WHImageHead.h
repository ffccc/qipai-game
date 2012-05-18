#ifndef WH_IMAGE_HEAD_FILE
#define WH_IMAGE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//MFC 文件
#include <GdiPlus.h>
#include <AtlImage.h>

//命名空间
using namespace Gdiplus;

//平台文件
#include "..\..\全局定义\Platform.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef WH_IMAGE_CLASS
	#ifdef  WH_IMAGE_DLL
		#define WH_IMAGE_CLASS _declspec(dllexport)
	#else
		#define WH_IMAGE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define WH_IMAGE_DLL_NAME		TEXT("WHImage.dll")					//组件名字
#else
	#define WH_IMAGE_DLL_NAME		TEXT("WHImageD.dll")				//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#ifndef WH_IMAGE_DLL
	#include "BitImage.h"
	#include "GIFImage.h"
	#include "PngImage.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif