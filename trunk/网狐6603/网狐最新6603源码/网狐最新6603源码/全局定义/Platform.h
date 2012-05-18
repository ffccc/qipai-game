#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//定义文件
#include "Macro.h"
#include "Define.h"

//结构文件
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//模板文件
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//程序版本
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(6,0,3)				//大厅版本
#define VERSION_MOBILE				PROCESS_VERSION(6,0,3)				//手机版本

//开发版本
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//框架版本

//////////////////////////////////////////////////////////////////////////////////
//发布版本

#ifndef _DEBUG

//平台常量
const TCHAR	szProduct[]=TEXT("开心家园");							    //产品名字
const TCHAR szPlazaClass[]=TEXT("WHHNZDWPGamePlaza");					//广场类名
const TCHAR szProductKey[]=TEXT("WHHNZDWPGamePlatform");				//产品主键

//地址定义
const TCHAR szCookieUrl[]=TEXT("http://hnkaihe.com");					//记录地址
const TCHAR szLogonServer[]=TEXT("www.hnkaihe.com");					//登录地址
const TCHAR szPlatformLink[]=TEXT("http://www.hnkaihe.com");			//平台网站

#else

//////////////////////////////////////////////////////////////////////////////////
//内测版本

//平台常量
const TCHAR	szProduct[]=TEXT("网狐棋牌平台");							//产品名字
const TCHAR szPlazaClass[]=TEXT("WHQPGamePlaza");						//广场类名
const TCHAR szProductKey[]=TEXT("WHQPGamePlatform");					//产品主键

//地址定义
const TCHAR szCookieUrl[]=TEXT("http://foxuc.net");						//记录地址
const TCHAR szLogonServer[]=TEXT("qp2.foxuc.net");						//登录地址
const TCHAR szPlatformLink[]=TEXT("http://qp2.foxuc.net");				//平台网站

#endif

//////////////////////////////////////////////////////////////////////////////////

//数据库名
const TCHAR szPlatformDB[]=TEXT("QPPlatformDB");						//平台数据库
const TCHAR szAccountsDB[]=TEXT("QPAccountsDB");						//用户数据库
const TCHAR	szTreasureDB[]=TEXT("QPTreasureDB");						//财富数据库
const TCHAR	szExerciseDB[]=TEXT("QPExerciseDB");						//练习数据库

//////////////////////////////////////////////////////////////////////////////////

//授权信息
const TCHAR szCompilation[]=TEXT("4B536A49-1246-45EB-8363-FF4C7DF645CC");

//////////////////////////////////////////////////////////////////////////////////

#endif