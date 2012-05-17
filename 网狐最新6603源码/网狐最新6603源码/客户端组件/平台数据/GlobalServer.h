#ifndef GLOBAL_SERVER_HEAD_FILE
#define GLOBAL_SERVER_HEAD_FILE

#pragma once

#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////

//地址信息
struct tagServerItem
{
	WORD							wServerPort;						//服务端口
	DWORD							dwServerAddr;						//服务地址
	TCHAR							szServerHost[LEN_SERVER];			//域名信息
};

//////////////////////////////////////////////////////////////////////////////////

//全局服务器
class PLATFORM_DATA_CLASS CGlobalServer
{
	//函数定义
public:
	//构造函数
	CGlobalServer();
	//析构函数
	virtual ~CGlobalServer();

	//全局信息
public:
	//子项数目
	WORD GetItemCount();

	//上次登录
public:
	//上次登录
	VOID SetLastServerName(LPCTSTR pszLastServer);
	//上次登录
	VOID GetLastServerName(TCHAR szLastServer[LEN_SERVER]);

	//子项信息
public:
	//子项名字
	VOID GetServerItemName(WORD wIndex, TCHAR szResult[LEN_SERVER]);
	//子项信息
	bool GetServerItemInfo(LPCTSTR pszServerName, tagServerItem & ServerItem);

	//写入函数
protected:
	//读取数值
	VOID WriteFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nValue);
	//写入数据
	VOID WriteFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, LPCTSTR pszString);

	//读取函数
protected:
	//读取数值
	UINT ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault);
	//读取数据
	VOID ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif