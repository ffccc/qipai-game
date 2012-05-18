#ifndef SHARE_CONTROL_HEAD_FILE
#define SHARE_CONTROL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//平台文件
#include "..\..\全局定义\Platform.h"

//网络命令
#include "..\..\消息定义\CMD_Commom.h"
#include "..\..\消息定义\CMD_GameServer.h"
#include "..\..\消息定义\CMD_LogonServer.h"

//组件文件
#include "..\..\客户端组件\图像控件\WHImageHead.h"
#include "..\..\客户端组件\下载组件\DownLoadHead.h"
#include "..\..\客户端组件\界面控件\SkinControlHead.h"
#include "..\..\客户端组件\用户服务\UserServiceHead.h"
#include "..\..\客户端组件\平台数据\PlatformDataHead.h"
#include "..\..\客户端组件\形象组件\AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef SHARE_CONTROL_CLASS
	#ifdef  SHARE_CONTROL_DLL
		#define SHARE_CONTROL_CLASS _declspec(dllexport)
	#else
		#define SHARE_CONTROL_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define SHARE_CONTROL_DLL_NAME	TEXT("ShareControl.dll")			//组件名字
#else
	#define SHARE_CONTROL_DLL_NAME	TEXT("ShareControlD.dll")			//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////

//警告警告
#pragma warning(disable:4192)

//FALSH 定义
#import "Flash.ocx" named_guids

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IFlashControlSink INTERFACE_VERSION(1,1)
	static const GUID IID_IFlashControlSink={0xb10c2bd0,0x1f03,0x431c,0xb2,0x4b,0xd9,0x9f,0x50,0xfb,0xff,0x70};
#else
	#define VER_IFlashControlSink INTERFACE_VERSION(1,1)
	static const GUID IID_IFlashControlSink={0x74d727ef,0x9847,0x4036,0x83,0xb8,0x9b,0xeb,0x1c,0x87,0xb9,0xa1};
#endif

//动画接口
interface IFlashControlSink : public IUnknownEx
{
	//播放完成
	virtual VOID OnEventFlashPlayFinish()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IRichEditMessageSink INTERFACE_VERSION(1,1)
	static const GUID IID_IRichEditMessageSink={0xd5956ee6,0x19e7,0x4567,0x87,0x31,0xf9,0x48,0x83,0x36,0x19,0x62};
#else
	#define VER_IRichEditMessageSink INTERFACE_VERSION(1,1)
	static const GUID IID_IRichEditMessageSink={0x79bfffe7,0xafef,0x4be6,0x84,0xad,0x07,0x79,0x3f,0x27,0x5d,0xec};
#endif

//消息接口
interface IRichEditMessageSink : public IUnknownEx
{
	//点击接口
	virtual bool OnEventStringLink(LPCTSTR pszString, UINT uMessage)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IStringMessage INTERFACE_VERSION(1,1)
	static const GUID IID_IStringMessage={0x2947cade,0xfd9a,0x40a2,0x9f,0x74,0xaa,0x7a,0xb4,0xfa,0x06,0x5d};
#else
	#define VER_IStringMessage INTERFACE_VERSION(1,1)
	static const GUID IID_IStringMessage={0xdd77eded,0x6b1e,0x4839,0xa6,0x05,0x92,0x9e,0xc6,0xec,0xd0,0x52};
#endif

//信息接口
interface IStringMessage : public IUnknownEx
{
	//事件消息
public:
	//进入事件
	virtual bool InsertUserEnter(LPCTSTR pszUserName)=NULL;
	//离开事件
	virtual bool InsertUserLeave(LPCTSTR pszUserName)=NULL;
	//断线事件
	virtual bool InsertUserOffLine(LPCTSTR pszUserName)=NULL;

	//字符消息
public:
	//普通消息
	virtual bool InsertNormalString(LPCTSTR pszString)=NULL;
	//系统消息
	virtual bool InsertSystemString(LPCTSTR pszString)=NULL;
	//提示消息
	virtual bool InsertPromptString(LPCTSTR pszString)=NULL;
	//公告消息
	virtual bool InsertAfficheString(LPCTSTR pszString)=NULL;

	//定制消息
public:
	//定制消息
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor)=NULL;
	//定制消息
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor)=NULL;

	//表情消息
public:
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath)=NULL;
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString)=NULL;
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath)=NULL;

	//聊天消息
public:
	//用户聊天
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor)=NULL;
	//用户聊天
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor)=NULL;
	//用户私聊
	virtual bool InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//导出文件

#ifndef SHARE_CONTROL_DLL
	#include "DlgBrowser.h"
	#include "WebBrowser.h"
	#include "WebMessage.h"
	#include "Expression.h"
	#include "Information.h"
	#include "WebPublicize.h"
	#include "FlashControl.h"
	#include "MissionManager.h"
	#include "WndChatControl.h"
	#include "SystemTrayIcon.h"
	#include "PasswordControl.h"
	#include "RichEditMessage.h"
	#include "UserInformation.h"
	#include "UserItemElement.h"
	#include "UserListControl.h"
	#include "DlgInsureCounter.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif