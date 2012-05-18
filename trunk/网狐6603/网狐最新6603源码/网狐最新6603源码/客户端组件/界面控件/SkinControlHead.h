#ifndef SKIN_CONTROL_HEAD_FILE
#define SKIN_CONTROL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//平台文件
#include "..\..\全局定义\Platform.h"

//组件文件
#include "..\图像控件\WHImageHead.h"
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef SKIN_CONTROL_CLASS
	#ifdef  SKIN_CONTROL_DLL
		#define SKIN_CONTROL_CLASS _declspec(dllexport)
	#else
		#define SKIN_CONTROL_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define SKIN_CONTROL_DLL_NAME	TEXT("SkinControl.dll")			//组件名字
#else
	#define SKIN_CONTROL_DLL_NAME	TEXT("SkinControlD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#ifndef SKIN_CONTROL_DLL
	#include "SkinEdit.h"
	#include "SkinMenu.h"
	#include "SkinDialog.h"
	#include "SkinButton.h"
	#include "SkinTabCtrl.h"
	#include "SkinSplitter.h"
	#include "SkinComboBox.h"
	#include "SkinEncircle.h"
	#include "SkinListCtrl.h"
	#include "SkinRichEdit.h"
	#include "SkinScrollBar.h"
	#include "SkinHyperLink.h"
	#include "SkinRenderManager.h"
	#include "SkinResourceManager.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif