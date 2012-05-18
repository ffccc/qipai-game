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
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;

	lstrcpyn(m_GameServiceAttrib.szKindName,GAME_NAME,CountArray(m_GameServiceAttrib.szKindName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPChinaChessDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
#ifdef VIDEO_GAME
	lstrcpyn(m_GameServiceAttrib.szDescription,TEXT("��Ƶ�й�������Ϸ�������"),CountArray(m_GameServiceAttrib.szDescription));
	lstrcpyn(m_GameServiceAttrib.szClientModuleName,TEXT("ChinaChessVD.EXE"),CountArray(m_GameServiceAttrib.szClientModuleName));
	lstrcpyn(m_GameServiceAttrib.szServerModuleName,TEXT("ChinaChessVDServer.DLL"),CountArray(m_GameServiceAttrib.szServerModuleName));
#else 
	lstrcpyn(m_GameServiceAttrib.szDescription,TEXT("�й�������Ϸ�������"),CountArray(m_GameServiceAttrib.szDescription));
	lstrcpyn(m_GameServiceAttrib.szClientModuleName,TEXT("ChinaChess.EXE"),CountArray(m_GameServiceAttrib.szClientModuleName));
	lstrcpyn(m_GameServiceAttrib.szServerModuleName,TEXT("ChinaChessServer.DLL"),CountArray(m_GameServiceAttrib.szServerModuleName));
#endif
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

	//��Ԫ����
	pGameServiceOption->lCellScore=__max(1L,pGameServiceOption->lCellScore);

	//��������
	if (pGameServiceOption->wServerType!=GAME_GENRE_SCORE)
	{
		pGameServiceOption->lLessScore=__max(pGameServiceOption->lCellScore*5L,pGameServiceOption->lLessScore);
	}

	//�������(ԭ���Ļ�������)
	if (pGameServiceOption->lRestrictScore!=0L)
	{
		pGameServiceOption->lRestrictScore=__max(pGameServiceOption->lRestrictScore,pGameServiceOption->lLessScore);
	}

	//��������
	if (pGameServiceOption->wServerType!=GAME_GENRE_SCORE)
	{
		if(pGameServiceOption->lMaxScore<=pGameServiceOption->lLessScore)
			pGameServiceOption->lMaxScore=0L;
		else if(pGameServiceOption->lRestrictScore>0)
			pGameServiceOption->lMaxScore=__min(pGameServiceOption->lMaxScore,pGameServiceOption->lRestrictScore);
	}

	return true;
}

//��������
VOID * __cdecl CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
