#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"
#include "..\��Ϣ����\CMD_GoBang.h"

//////////////////////////////////////////////////////////////////////////

//ȫ�ֱ���
static CGameServiceManager			g_GameServiceManager;				//�������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager(void)
{
	//��������
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.cbJoinInGame=FALSE;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	lstrcpyn(m_GameServiceAttrib.szKindName,GAME_NAME,CountArray(m_GameServiceAttrib.szKindName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPGoBangDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szDescription,TEXT("��������Ϸ�������"),CountArray(m_GameServiceAttrib.szDescription));
	lstrcpyn(m_GameServiceAttrib.szClientModuleName,TEXT("GoBang.EXE"),CountArray(m_GameServiceAttrib.szClientModuleName));
	lstrcpyn(m_GameServiceAttrib.szServerModuleName,TEXT("GoBangServer.DLL"),CountArray(m_GameServiceAttrib.szServerModuleName));

	return;
}

//��������
CGameServiceManager::~CGameServiceManager(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//������Ϸ��
void * __cdecl CGameServiceManager::CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��ȡ����
void __cdecl CGameServiceManager::GetGameServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	GameServiceAttrib=m_GameServiceAttrib;
	return;
}

//�����޸�
bool __cdecl CGameServiceManager::RectifyServiceOption(tagGameServiceOption * pGameServiceOption)
{
	//Ч�����
	ASSERT(pGameServiceOption!=NULL);
	if (pGameServiceOption==NULL) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
