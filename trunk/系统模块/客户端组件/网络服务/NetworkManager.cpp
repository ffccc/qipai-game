#include "Stdafx.h"
#include "NetworkManager.h"


//�������

//���캯��
CNetworkManager::CNetworkManager()
{

}

//��������
CNetworkManager::~CNetworkManager()
{

}

//�����ӿ�
//�ӿڲ�ѯ
VOID * __cdecl CNetworkManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(INetworkManager, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(INetworkManager, Guid, dwQueryVer);
	return NULL;
}

//���ܲ�ѯ
//ö�ٴ���
bool __cdecl CNetworkManager::EnumProxyServerType(WORD wIndex, tagProxyTypeDescribe & ProxyTypeDescribe)
{
	return false;
}

//�������
DWORD __cdecl CNetworkManager::ProxyServerTesting(enProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateNetworkManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CNetworkManager * pNetworkManager = NULL;
	try
	{
		pNetworkManager = new CNetworkManager();
		if (pNetworkManager == NULL) throw TEXT("����ʧ��");
		void * pObject = pNetworkManager->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pNetworkManager);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
