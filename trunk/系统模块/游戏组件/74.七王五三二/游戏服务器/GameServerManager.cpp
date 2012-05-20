#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"
#include "..\��Ϣ����\CMD_SevenThree.h"

//////////////////////////////////////////////////////////////////////////

//ȫ�ֱ���
static CGameServiceManager			g_GameServiceManager;				//�������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager(void)
{
	//��������
	m_GameServiceAttrib.wKindID=KIND_ID;
//	m_GameServiceAttrib.dwServerVersion=0;
//	m_GameServiceAttrib.dwMaxClientVersion=0;
//	m_GameServiceAttrib.dwLessClientVersion=0;
//	m_GameServiceAttrib.wSupporType=GAME_GENRE;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	lstrcpyn(m_GameServiceAttrib.szKindName,GAME_NAME,CountArray(m_GameServiceAttrib.szKindName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("TreasureDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szDescription,TEXT("������Ϸ�������"),CountArray(m_GameServiceAttrib.szDescription));
	lstrcpyn(m_GameServiceAttrib.szClientModuleName,TEXT("SevenThree.exe"),CountArray(m_GameServiceAttrib.szClientModuleName));
	lstrcpyn(m_GameServiceAttrib.szServerModuleName,TEXT("SevenThreeServer.dll"),CountArray(m_GameServiceAttrib.szServerModuleName));

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

	//���ֵ���
	pGameServiceOption->lCellScore=__max(1L,pGameServiceOption->lCellScore);

	//��ҷ���
	if (pGameServiceOption->wServerType&GAME_GENRE_GOLD)
	{
		pGameServiceOption->lLessScore=__max(pGameServiceOption->lCellScore*12L,pGameServiceOption->lLessScore);
		if (pGameServiceOption->lCellScore!=0L) pGameServiceOption->lCellScore=__max(pGameServiceOption->lCellScore,pGameServiceOption->lLessScore);
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
