#ifndef GAME_ENGINE_HEAD_HEAD_FILE
#define GAME_ENGINE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//MFC 文件
#include <Afxmt.h>
#include <AtlBase.h>
#include <AfxTempl.h>
#include <MMSystem.h>

//D3D 文件
#include <D3D9.h>
#include <D3Dx9.h>
#include <DSound.h>

//OGL 文件
#include <GL/GL.h>		
#include <GL/GLU.h>
#include <GL/GLAux.h>

//平台文件
#include "..\..\全局定义\Platform.h"
#include "..\..\客户端组件\界面控件\SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef GAME_ENGINE_CLASS
	#ifdef  GAME_ENGINE_DLL
		#define GAME_ENGINE_CLASS _declspec(dllexport)
	#else
		#define GAME_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define GAME_ENGINE_DLL_NAME		TEXT("GameEngine.dll")			//组件名字
#else
	#define GAME_ENGINE_DLL_NAME		TEXT("GameEngineD.dll")			//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//资源信息
struct tagResourceInfo
{
	DWORD							dwSize;								//资源大小
	LPVOID							pcbBuffer;							//资源内存
};

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#ifndef GAME_ENGINE_DLL

	//D3D 引擎
	#include "D3DFont.h"
	#include "D3DDirect.h"
	#include "D3DDevice.h"
	#include "D3DSprite.h"
	#include "D3DSurface.h"
	#include "D3DTexture.h"

	//OGL 引擎
	//#include "OGLFont.h"
	//#include "OGLDevice.h"
	//#include "OGLTexture.h"

	//声音文件
	#include "D3DSound.h"
	#include "D3DMusic.h"
	#include "D3DSoundBuffer.h"
	#include "D3DSoundThread.h"

	//游戏引擎
	#include "LapseCount.h"
	#include "VirtualEngine.h"
	#include "VirtualWindow.h"
	#include "VirtualButton.h"

#endif

//////////////////////////////////////////////////////////////////////////////////

#endif