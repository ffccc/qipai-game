#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"

//////////////////////////////////////////////////////////////////////////

//ȫ�ֱ���
static CGameServiceManager			g_GameServiceManager;				//�������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager(void)
{
	//��������
	m_GameServiceAttrib.wKindID=KIND_ID;
	//m_GameServiceAttrib.dwServerVersion=0;
	//m_GameServiceAttrib.dwMaxClientVersion=0;
	//m_GameServiceAttrib.dwLessClientVersion=0;
	//m_GameServiceAttrib.wSupporType=GAME_GENRE;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	lstrcpyn(m_GameServiceAttrib.szKindName,GAME_NAME,CountArray(m_GameServiceAttrib.szKindName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("WHPaoHuZiDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szDescription,TEXT("�ܺ�����Ϸ�������"),CountArray(m_GameServiceAttrib.szDescription));
	lstrcpyn(m_GameServiceAttrib.szClientModuleName,TEXT("HNphz.EXE"),CountArray(m_GameServiceAttrib.szClientModuleName));
	lstrcpyn(m_GameServiceAttrib.szServerModuleName,TEXT("HNphzServer.DLL"),CountArray(m_GameServiceAttrib.szServerModuleName));

	return;
}

//��������
CGameServiceManager::~CGameServiceManager(void)
{
}

VOID* CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
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

	//���ֵ���
	pGameServiceOption->lCellScore=__max(1L,pGameServiceOption->lCellScore);
	//if (pGameServiceOption->lHighScore!=0L) pGameServiceOption->lHighScore=__max(pGameServiceOption->lHighScore,pGameServiceOption->lLessScore);

	if ((pGameServiceOption->wServerType==GAME_GENRE_GOLD)||(pGameServiceOption->wServerType==GAME_GENRE_SCORE))
	{
		pGameServiceOption->lLessScore=__max(pGameServiceOption->lCellScore*50L,pGameServiceOption->lLessScore);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
