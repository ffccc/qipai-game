#ifndef AVATAR_CONTROL_HEAD_HEAD_FILE
#define AVATAR_CONTROL_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//系统文件
#include <VFW.h>

//平台文件
#include "..\..\全局定义\Platform.h"

//组件文件
#include "..\..\客户端组件\图像控件\WHImageHead.h"
#include "..\..\客户端组件\下载组件\DownLoadHead.h"
#include "..\..\客户端组件\界面控件\SkinControlHead.h"
#include "..\..\客户端组件\游戏控件\ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef AVATAR_CONTROL_CLASS
	#ifdef  AVATAR_CONTROL_DLL
		#define AVATAR_CONTROL_CLASS _declspec(dllexport)
	#else
		#define AVATAR_CONTROL_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define AVATAR_CONTROL_DLL_NAME	TEXT("AvatarControl.dll")			//组件名字
#else
	#define AVATAR_CONTROL_DLL_NAME	TEXT("AvatarControlD.dll")			//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//框架类型
#define ITEM_FRAME_NORMAL			0									//默认框架
#define ITEM_FRAME_HOVER			1									//默认框架
#define ITEM_FRAME_HITDOWN			2									//默认框架

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//头像资源
struct tagAvatarResource
{
	LPCTSTR							pszResource;						//资源名称
	HINSTANCE						hResInstance;						//资源句柄
};

//头像索引
struct tagCustomFaceIndex
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwCustomID;							//自定标识
};

//编辑结果
struct tagCustomFaceResult
{
	WORD							wFaceID;							//头像标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IFaceItemControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceItemControl={0xfc9067a9,0x162e,0x4e13,0xb0,0x01,0xf7,0x57,0x9e,0xa4,0x60,0x6b};
#else
	#define VER_IFaceItemControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceItemControl={0xaab85fd0,0x2483,0x4384,0xbb,0xe0,0x7d,0x5d,0x82,0x30,0xa7,0x1c};
#endif

//头像组件
interface IFaceItemControl : public IUnknownEx
{
	//信息接口
public:
	//头像数目
	virtual WORD GetFaceCount()=NULL;
	//获取尺寸
	virtual VOID GetFaceItemSize(CSize & SizeFace)=NULL;

	//资源接口
public:
	//获取资源
	virtual VOID GetAvatarResource(tagAvatarResource & AvatarResource)=NULL;
	//绘画框架
	virtual VOID DrawFaceItemFrame(CDC * pDC, INT nXPos, INT nYPos, BYTE cbFrameKind)=NULL;

	//绘画接口
public:
	//绘画头像
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)=NULL;
	//绘画头像
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)=NULL;

	//绘画接口
public:
	//绘画头像
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, DWORD dwCustomFace[FACE_CX*FACE_CY])=NULL;
	//绘画头像
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, DWORD dwCustomFace[FACE_CX*FACE_CY])=NULL;;

	//绘画接口
public:
	//绘画头像
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID)=NULL;
	//绘画头像
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IFaceSelectControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceSelectControl={0x8514020b,0x82a2,0x42d9,0x90,0x19,0x32,0xa4,0xea,0xce,0x9f,0x75};
#else
	#define VER_IFaceSelectControl INTERFACE_VERSION(1,1)
	static const GUID IID_IFaceSelectControl={0xf153d63f,0x8ed7,0x454c,0x8b,0x0c,0xcc,0x4b,0x12,0xc9,0xe1,0x2e};
#endif

//选择组件
interface IFaceSelectControl : public IUnknownEx
{
	//设置选择
public:
	//选择数目
	virtual VOID SetAllowItemFull()=NULL;
	//选择数目
	virtual VOID SetAllowItemCount(WORD wFaceCount)=NULL;

	//获取选择
public:
	//获取选择
	virtual bool GetSelectFaceID(WORD & wFaceID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ICustomFaceEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ICustomFaceEvent={0xaf4303d8,0x5cfd,0x4d62,0x82,0x78,0xd4,0xdf,0x1f,0xa9,0x73,0x2c};
#else
	#define VER_ICustomFaceEvent INTERFACE_VERSION(1,1)
	static const GUID IID_ICustomFaceEvent={0xfceb44d0,0x330e,0x4a9c,0xa6,0x42,0x38,0x82,0xd5,0x1b,0x0a,0x13};
#endif

//管理接口
interface ICustomFaceEvent : public IUnknownEx
{
	//头像数据
	virtual VOID OnEventSystemFace(DWORD dwUserID, WORD wFaceID)=NULL;
	//头像数据
	virtual VOID OnEventCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ICustomFaceManager INTERFACE_VERSION(1,1)
	static const GUID IID_ICustomFaceManager={0xbcd7ff05,0x0a79,0x4482,0x9b,0xa6,0x79,0xdf,0x4b,0xd9,0x80,0x16};
#else
	#define VER_ICustomFaceManager INTERFACE_VERSION(1,1)
	static const GUID IID_ICustomFaceManager={0xe5694b07,0x888e,0x45aa,0x85,0xa7,0xfe,0x89,0x84,0x1b,0x79,0x20};
#endif

//头像管理
interface ICustomFaceManager : public IUnknownEx
{
	//设置接口
	virtual bool SetCustomFaceEvent(IUnknownEx * pIUnknownEx)=NULL;
	//加载头像
	virtual bool LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID)=NULL;
	//加载头像
	virtual bool LoadUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)=NULL;
	//保存头像
	virtual bool SaveUserCustomFace(DWORD dwUserID, DWORD dwCustomID, DWORD dwCustomFace[FACE_CX*FACE_CY])=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//组件辅助类

DECLARE_MODULE_HELPER(FaceItemControl,AVATAR_CONTROL_DLL_NAME,"CreateFaceItemControl")
DECLARE_MODULE_HELPER(FaceSelectControl,AVATAR_CONTROL_DLL_NAME,"CreateFaceSelectControl")
DECLARE_MODULE_HELPER(CustomFaceManager,AVATAR_CONTROL_DLL_NAME,"CreateCustomFaceManager")

//////////////////////////////////////////////////////////////////////////////////

//组件文件
#ifndef AVATAR_CONTROL_DLL
	#include "FaceItemView.h"
	#include "FaceItemControl.h"
	#include "CustomFaceManager.h"
	#include "FaceSelectControl.h"
	#include "FaceCustomControl.h"
	#include "ImageEditorControl.h"
#endif
//////////////////////////////////////////////////////////////////////////////////

#endif