#ifndef SERVER_INFO_MANAGER_HEAD_FILE
#define SERVER_INFO_MANAGER_HEAD_FILE

#pragma once

#include "AfxTempl.h"
#include "ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////

//数组定义
typedef CWHArray<tagGameServerInfo *> CGameServerInfoArray;

//索引定义
typedef CMap<WORD,WORD,tagGameServerInfo *,tagGameServerInfo *> CGameServerInfoMap;

//////////////////////////////////////////////////////////////////////////////////

//模块数据
class MODULE_MANAGER_CLASS CServerInfoBuffer
{
	//变量定义
public:
	CGameServerInfoMap				m_GameServerInfoMap;				//模块索引
	CGameServerInfoArray			m_GameServerInfoArray;				//模块数组

	//函数定义
public:
	//构造函数
	CServerInfoBuffer();
	//析构函数
	virtual ~CServerInfoBuffer();

	//管理函数
public:
	//重置数据
	bool ResetServerInfo();
	//删除数据
	bool DeleteServerInfo(WORD wServerID);
	//插入数据
	bool InsertServerInfo(tagGameServerInfo * pGameServerInfo);

	//信息函数
public:
	//获取数目
	DWORD GetServerInfoCount();
	//查找数据
	tagGameServerInfo * SearchServerInfo(WORD wServerID);

	//内部函数
private:
	//创建对象
	tagGameServerInfo * CreateServerInfo();
};

//////////////////////////////////////////////////////////////////////////////////

//模块信息
class MODULE_MANAGER_CLASS CServerInfoManager
{
	//函数定义
public:
	//构造函数
	CServerInfoManager();
	//析构函数
	virtual ~CServerInfoManager();

	//加载房间
public:
	//加载房间
	bool LoadGameServerInfo(TCHAR szMachineID[LEN_MACHINE_ID], CServerInfoBuffer & ServerInfoBuffer);
	//加载房间
	bool LoadGameServerInfo(TCHAR szMachineID[LEN_MACHINE_ID], WORD wServerID, tagGameServerInfo & GameServerResult);

	//注册管理
public:
	//删除房间
	bool DeleteGameServer(WORD wServerID);
	//插入房间
	bool InsertGameServer(tagGameServerCreate * pGameServerCreate, tagGameServerInfo & GameServerResult);
	//修改房间
	bool ModifyGameServer(tagGameServerCreate * pGameServerCreate, tagGameServerInfo & GameServerResult);

	//内部函数
private:
	//连接数据
	bool ConnectPlatformDB(CDataBaseHelper & PlatformDBModule);
	//读取房间
	bool ReadGameServerInfo(CDataBaseAide & PlatformDBAide, tagGameServerInfo & GameServerResult);
};

//////////////////////////////////////////////////////////////////////////////////

#endif