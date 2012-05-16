#include "Stdafx.h"
#include "NetworkManager.h"


//网络管理

//构造函数
CNetworkManager::CNetworkManager()
{

}

//析构函数
CNetworkManager::~CNetworkManager()
{

}

//基础接口
//接口查询
VOID * __cdecl CNetworkManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(INetworkManager, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(INetworkManager, Guid, dwQueryVer);
	return NULL;
}

//性能查询
//枚举代理
bool __cdecl CNetworkManager::EnumProxyServerType(WORD wIndex, tagProxyTypeDescribe & ProxyTypeDescribe)
{
	return false;
}

//代理测试
DWORD __cdecl CNetworkManager::ProxyServerTesting(enProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) void * __cdecl CreateNetworkManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	//建立对象
	CNetworkManager * pNetworkManager = NULL;
	try
	{
		pNetworkManager = new CNetworkManager();
		if (pNetworkManager == NULL) throw TEXT("创建失败");
		void * pObject = pNetworkManager->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pNetworkManager);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
