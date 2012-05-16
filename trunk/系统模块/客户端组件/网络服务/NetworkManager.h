#ifndef NETWORK_MANAGER_HEAD_FILE
#define NETWORK_MANAGER_HEAD_FILE

#pragma once

#include "NetworkServiceHead.h"

//////////////////////////////////////////////////////////////////////////

//�������
class CNetworkManager : public INetworkManager
{
	//��������
public:
	//���캯��
	CNetworkManager();
	//��������
	virtual ~CNetworkManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܲ�ѯ
public:
	//ö�ٴ���
	virtual bool __cdecl EnumProxyServerType(WORD wIndex, tagProxyTypeDescribe & ProxyTypeDescribe);
	//�������
	virtual DWORD __cdecl ProxyServerTesting(enProxyServerType ProxyServerType, const tagProxyServerInfo & ProxyServerInfo);
};

//////////////////////////////////////////////////////////////////////////

#endif