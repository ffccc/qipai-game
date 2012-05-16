#include "StdAfx.h"
#include "ServerListCenter.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerListCenter::CServerListCenter(void)
{
	//��ʼ��
	m_TypeItemMap.InitHashTable(GetMaxPrime(20));
	m_KindItemMap.InitHashTable(GetMaxPrime(50));
	m_ServerItemMap.InitHashTable(GetMaxPrime(500));
	m_StationItemMap.InitHashTable(GetMaxPrime(200));

	return;
}

//��������
CServerListCenter::~CServerListCenter(void)
{
	//ɾ������
	tagGameTypeItem * pGameTypeItem=NULL;
	for (INT_PTR i=0;i<m_TypeItemStore.GetCount();i++)
	{
		pGameTypeItem=m_TypeItemStore[i];
		SafeDelete(pGameTypeItem);
	}
	m_TypeItemStore.RemoveAll();

	//ɾ������
	tagGameKindItem * pGameKindItem=NULL;
	for (INT_PTR i=0;i<m_KindItemStore.GetCount();i++)
	{
		pGameKindItem=m_KindItemStore[i];
		SafeDelete(pGameKindItem);
	}
	m_KindItemStore.RemoveAll();

	//ɾ������
	tagGameServerItem * pGameServerItem=NULL;
	for (INT_PTR i=0;i<m_ServerItemStore.GetCount();i++)
	{
		pGameServerItem=m_ServerItemStore[i];
		SafeDelete(pGameServerItem);
	}
	m_ServerItemStore.RemoveAll();

	//ɾ��վ��
	tagGameStationItem * pGameStationItem=NULL;
	for (INT_PTR i=0;i<m_StationItemStore.GetCount();i++)
	{
		pGameStationItem=m_StationItemStore[i];
		SafeDelete(pGameStationItem);
	}
	m_StationItemStore.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CServerListCenter::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerListCenter,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerListCenter,Guid,dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CServerListCenter::InsertGameType(tagGameType * pGameType)
{
	//Ч�����
	ASSERT(pGameType!=NULL);
	if (pGameType==NULL) return false;

	//�����ִ�
	tagGameTypeItem * pGameTypeItem=NULL;
	if (m_TypeItemMap.Lookup(pGameType->wTypeID,pGameTypeItem)==TRUE) return false;

	try
	{
		//��������
		INT_PTR nStroeCount=m_TypeItemStore.GetCount();
		if (nStroeCount>0)
		{
			pGameTypeItem=m_TypeItemStore[nStroeCount-1];
			m_TypeItemStore.RemoveAt(nStroeCount-1);
		}
		else
		{
			pGameTypeItem=new tagGameTypeItem;
			if (pGameTypeItem==NULL) return false;
		}

		//��������
		CopyMemory(&pGameTypeItem->GameType,pGameType,sizeof(tagGameType));
		m_TypeItemMap[pGameType->wTypeID]=pGameTypeItem;

		return true;
	}
	catch (...) {}

	return false;
}

//��������
bool __cdecl CServerListCenter::InsertGameKind(tagGameKind * pGameKind)
{
	//Ч�����
	ASSERT(pGameKind!=NULL);
	if (pGameKind==NULL) return false;

	//�����ִ�
	tagGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(pGameKind->wKindID,pGameKindItem)==TRUE) return false;

	try
	{
		//��������
		INT_PTR nStroeCount=m_KindItemStore.GetCount();
		if (nStroeCount>0)
		{
			pGameKindItem=m_KindItemStore[nStroeCount-1];
			m_KindItemStore.RemoveAt(nStroeCount-1);
		}
		else
		{
			pGameKindItem=new tagGameKindItem;
			if (pGameKindItem==NULL) return false;
		}

		//��������
		memset(pGameKindItem,0,sizeof(pGameKindItem));
		CopyMemory(&pGameKindItem->GameKind,pGameKind,sizeof(tagGameKind));
		m_KindItemMap[pGameKind->wKindID]=pGameKindItem;

		return true;
	}
	catch (...) {}

	return false;
}

//���뷿��
bool __cdecl CServerListCenter::InsertGameServer(tagGameServer * pGameServer)
{
	//Ч�����
	ASSERT(pGameServer!=NULL);
	if (pGameServer==NULL) return false;

	//�����ִ�
	tagGameServerItem * pGameServerItem=NULL;;
	DWORD dwKey=MAKELONG(pGameServer->wKindID,pGameServer->wServerID);
	if (m_ServerItemMap.Lookup(dwKey,pGameServerItem)==TRUE) return false;

	try
	{
		//��������
		INT_PTR nStroeCount=m_ServerItemStore.GetCount();
		if (nStroeCount>0)
		{
			pGameServerItem=m_ServerItemStore[nStroeCount-1];
			m_ServerItemStore.RemoveAt(nStroeCount-1);
		}
		else
		{
			pGameServerItem=new tagGameServerItem;
			if (pGameServerItem==NULL) return false;
		}

		//��������
		pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
		CopyMemory(&pGameServerItem->GameServer,pGameServer,sizeof(tagGameServer));
		m_ServerItemMap[dwKey]=pGameServerItem;

		//�����¼�
		TCHAR szBuffer[256]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s  KindID��%ld StationID��%ld  ServerID��%ld��ע��ɹ�"),
			pGameServer->szServerName,pGameServer->wKindID,pGameServer->wStationID,pGameServer->wServerID);
		CTraceService::TraceString(szBuffer,TraceLevel_Debug);

		//��������
		UpdateKindOnLineCount(pGameServer->wKindID);

		return true;
	}
	catch (...) {}

	return false;
}

//����վ��
bool __cdecl CServerListCenter::InsertGameStation(tagGameStation * pGameStation)
{
	//Ч�����
	ASSERT(pGameStation!=NULL);
	if (pGameStation==NULL) return false;

	//�����ִ�
	tagGameStationItem * pGameStationItem=NULL;
	DWORD dwKey=MAKELONG(pGameStation->wKindID,pGameStation->wStationID);
	if (m_StationItemMap.Lookup(dwKey,pGameStationItem)==TRUE) return false;

	try
	{
		//��������
		INT_PTR nStroeCount=m_StationItemStore.GetCount();
		if (nStroeCount>0)
		{
			pGameStationItem=m_StationItemStore[nStroeCount-1];
			m_StationItemStore.RemoveAt(nStroeCount-1);
		}
		else
		{
			pGameStationItem=new tagGameStationItem;
			if (pGameStationItem==NULL) return false;
		}

		//��������
		CopyMemory(&pGameStationItem->GameStation,pGameStation,sizeof(tagGameStation));
		m_StationItemMap[dwKey]=pGameStationItem;

		return true;
	}
	catch (...) {}

	return false;
}

//ɾ������
bool __cdecl CServerListCenter::DeleteGameType(WORD wTypeID)
{
	//��������
	WORD wKey=0;
	tagGameTypeItem * pGameTypeItem=NULL;
	if (m_TypeItemMap.Lookup(wKey,pGameTypeItem)==FALSE) return false;

	//ɾ������
	m_TypeItemMap.RemoveKey(wKey);
	m_TypeItemStore.Add(pGameTypeItem);

	return true;
}

//ɾ������
bool __cdecl CServerListCenter::DeleteGameKind(WORD wKindID)
{
	//��������
	WORD wKey=0;
	tagGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(wKey,pGameKindItem)==FALSE) return false;

	//ɾ������
	m_KindItemMap.RemoveKey(wKey);
	m_KindItemStore.Add(pGameKindItem);

	return true;
}

//ɾ������
bool __cdecl CServerListCenter::DeleteGameServer(WORD wKindID, WORD wServerID)
{
	//���ҷ���
	tagGameServerItem * pGameServerItem=NULL;
	DWORD dwKey=MAKELONG(wKindID,wServerID);
	if (m_ServerItemMap.Lookup(dwKey,pGameServerItem)==FALSE) return false;

	//ɾ������
	m_ServerItemMap.RemoveKey(dwKey);
	m_ServerItemStore.Add(pGameServerItem);

	//��������
	UpdateKindOnLineCount(wKindID);

	return false;
}

//ɾ��վ��
bool __cdecl CServerListCenter::DeleteGameStation(WORD wKindID, WORD wStationID)
{
	//����վ��
	tagGameStationItem * pGameStationItem=NULL;
	DWORD dwKey=MAKELONG(wKindID,wStationID);
	if (m_StationItemMap.Lookup(dwKey,pGameStationItem)==FALSE) return false;

	//ɾ������
	m_StationItemMap.RemoveKey(dwKey);
	m_StationItemStore.Add(pGameStationItem);

	return false;
}

//ö������
tagGameTypeItem * const __cdecl CServerListCenter::EmunGameTypeItem(POSITION & Pos)
{
	//��������
	if (Pos==NULL) Pos=m_TypeItemMap.GetStartPosition();
	if (Pos==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	tagGameTypeItem * pGameTypeItem=NULL;
	m_TypeItemMap.GetNextAssoc(Pos,wKey,pGameTypeItem);

	return pGameTypeItem;
}

//ö������
tagGameKindItem * const __cdecl CServerListCenter::EmunGameKindItem(POSITION & Pos)
{
	//��������
	if (Pos==NULL) Pos=m_KindItemMap.GetStartPosition();
	if (Pos==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	tagGameKindItem * pGameKindItem=NULL;
	m_KindItemMap.GetNextAssoc(Pos,wKey,pGameKindItem);

	return pGameKindItem;
}

//ö�ٷ���
tagGameServerItem * const __cdecl CServerListCenter::EmunGameServerItem(POSITION & Pos)
{
	//��������
	if (Pos==NULL) Pos=m_ServerItemMap.GetStartPosition();
	if (Pos==NULL) return NULL;

	//���ҷ���
	DWORD dwKey=0;
	tagGameServerItem * pGameServerItem=NULL;
	m_ServerItemMap.GetNextAssoc(Pos,dwKey,pGameServerItem);

	return pGameServerItem;
}

//ö��վ��
tagGameStationItem * const __cdecl CServerListCenter::EmunGameStationItem(POSITION & Pos)
{
	//��������
	if (Pos==NULL) Pos=m_StationItemMap.GetStartPosition();
	if (Pos==NULL) return NULL;

	//����վ��
	DWORD dwKey=0;
	tagGameStationItem * pGameStationItem=NULL;
	m_StationItemMap.GetNextAssoc(Pos,dwKey,pGameStationItem);

	return pGameStationItem;
}

//��������
tagGameTypeItem * const __cdecl CServerListCenter::SearchGameType(WORD wTypeID)
{
	tagGameTypeItem * pGameTypeItem=NULL;
	m_TypeItemMap.Lookup(wTypeID,pGameTypeItem);
	return pGameTypeItem;
}

//��������
tagGameKindItem * const __cdecl CServerListCenter::SearchGameKind(WORD wKindID)
{
	tagGameKindItem * pGameKindItem=NULL;
	m_KindItemMap.Lookup(wKindID,pGameKindItem);
	return pGameKindItem;
}

//���ҷ���
tagGameServerItem * const __cdecl CServerListCenter::SearchGameServer(WORD wKindID, WORD wServerID)
{
	tagGameServerItem * pGameServerItem=NULL;
	m_ServerItemMap.Lookup(MAKELONG(wKindID,wServerID),pGameServerItem);
	return pGameServerItem;
}

//����վ��
tagGameStationItem * const __cdecl CServerListCenter::SearchGameStation(WORD wKindID, WORD wStationID)
{
	tagGameStationItem * pGameStationItem=NULL;
	m_StationItemMap.Lookup(MAKELONG(wKindID,wStationID),pGameStationItem);
	return pGameStationItem;
}

//�����б�
void __cdecl CServerListCenter::ResetServerList()
{
	//ɾ������
	tagGameTypeItem * pGameTypeItem=NULL;
	POSITION Pos=m_TypeItemMap.GetStartPosition();
	while (Pos!=NULL)
	{
		WORD wKey=0;
		m_TypeItemMap.GetNextAssoc(Pos,wKey,pGameTypeItem);
		m_TypeItemStore.Add(pGameTypeItem);
	}
	m_TypeItemMap.RemoveAll();

	//ɾ������
	tagGameKindItem * pGameKindItem=NULL;
	Pos=m_KindItemMap.GetStartPosition();
	while (Pos!=NULL)
	{
		WORD wKey=0;
		m_KindItemMap.GetNextAssoc(Pos,wKey,pGameKindItem);
		m_KindItemStore.Add(pGameKindItem);
	}
	m_KindItemMap.RemoveAll();

	//ɾ������
	Pos=m_ServerItemMap.GetStartPosition();
	tagGameServerItem * pGameServerItem=NULL;
	while (Pos!=NULL)
	{
		DWORD dwKey=0;
		m_ServerItemMap.GetNextAssoc(Pos,dwKey,pGameServerItem);
		m_ServerItemStore.Add(pGameServerItem);
	}
	m_ServerItemMap.RemoveAll();

	//ɾ��վ��
	Pos=m_StationItemMap.GetStartPosition();
	tagGameStationItem * pGameStationItem=NULL;
	while (Pos!=NULL)
	{
		DWORD dwKey=0;
		m_StationItemMap.GetNextAssoc(Pos,dwKey,pGameStationItem);
		m_StationItemStore.Add(pGameStationItem);
	}
	m_StationItemMap.RemoveAll();

	return;
}

//�����б�
bool __cdecl CServerListCenter::LoadServerList(tagDataBaseInfo & DataBaseInfo)
{
	//�����б�
	ResetServerList();

	//�������ݿ�
	CDataBaseAide  ServerInfoDBAide;
	CDataBaseHelper	ServerInfoDBModule;

	//��������
	if (ServerInfoDBModule.CreateInstance()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//ת����ַ
		TCHAR szDataBaseAdr[16]=TEXT("");
		BYTE * pcbDataBaseAdr=(BYTE *)&DataBaseInfo.dwDataBaseAddr;
		_snprintf(szDataBaseAdr,sizeof(szDataBaseAdr),TEXT("%d.%d.%d.%d"),pcbDataBaseAdr[0],pcbDataBaseAdr[1],pcbDataBaseAdr[2],pcbDataBaseAdr[3]);

		//�������ݿ�
		ServerInfoDBModule->SetConnectionInfo(szDataBaseAdr,DataBaseInfo.wDataBasePort,DataBaseInfo.szDataBaseName,DataBaseInfo.szDataBaseUser,DataBaseInfo.szDataBasePass);

		//��������
		ServerInfoDBModule->OpenConnection();
		ServerInfoDBAide.SetDataBase(ServerInfoDBModule.GetInterface());

		//������Ϣ
		LoadGameType(ServerInfoDBAide);
		LoadGameKind(ServerInfoDBAide);
		LoadGameStation(ServerInfoDBAide);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return false;
}

//��������
bool __cdecl CServerListCenter::UpdateServerOnLineCount(WORD wKindID, WORD wServerID, DWORD dwOnLineCount)
{
	tagGameServerItem * pGameServerItem=SearchGameServer(wKindID,wServerID);
	if (pGameServerItem!=NULL)
	{
		//���·���
		pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
		pGameServerItem->GameServer.dwOnLineCount=dwOnLineCount;

		//��������
		UpdateKindOnLineCount(wKindID);

		return true;
	}

	return false;
}

//��������
void CServerListCenter::LoadGameType(CDataBaseAide & DataBaseAide)
{
	//ִ������
	DataBaseAide.ResetParameter();
	DataBaseAide.ExecuteProcess(TEXT("GSP_LoadGameTypeItem"),true);

	//��������
	tagGameType GameTypeRead;
	ZeroMemory(&GameTypeRead,sizeof(GameTypeRead));

	//��ȡ����
	IDataBase * pIDataBase=(IDataBase *)DataBaseAide.GetDataBase(IID_IDataBase,VER_IDataBase);

	//��ȡ����
	while (pIDataBase->IsRecordsetEnd()==false)
	{
		//��ȡ��Ϣ
		GameTypeRead.wTypeID=DataBaseAide.GetValue_WORD(TEXT("TypeID"));
		GameTypeRead.wSortID=DataBaseAide.GetValue_WORD(TEXT("SortID"));
		DataBaseAide.GetValue_String(TEXT("TypeName"),GameTypeRead.szTypeName,CountArray(GameTypeRead.szTypeName));

		//�ƶ���¼
		pIDataBase->MoveToNext();

		//������Ϣ
		InsertGameType(&GameTypeRead);
	}

	//�رռ�¼
	pIDataBase->CloseRecordset();

	return;
}

//��������
void CServerListCenter::LoadGameKind(CDataBaseAide & DataBaseAide)
{
	//ִ������
	DataBaseAide.ResetParameter();
	DataBaseAide.ExecuteProcess(TEXT("GSP_LoadGameKindItem"),true);

	//��������
	tagGameKind GameKindRead;
	ZeroMemory(&GameKindRead,sizeof(GameKindRead));

	//��ȡ����
	IDataBase * pIDataBase=(IDataBase *)DataBaseAide.GetDataBase(IID_IDataBase,VER_IDataBase);

	//��ȡ��Ϣ
	while (pIDataBase->IsRecordsetEnd()==false)
	{
		//��ȡ��Ϣ
		GameKindRead.wTypeID=DataBaseAide.GetValue_WORD(TEXT("TypeID"));
		GameKindRead.wKindID=DataBaseAide.GetValue_WORD(TEXT("KindID"));
		GameKindRead.wSortID=DataBaseAide.GetValue_WORD(TEXT("SortID"));
		GameKindRead.dwMaxVersion=DataBaseAide.GetValue_DWORD(TEXT("MaxVersion"));
		DataBaseAide.GetValue_String(TEXT("KindName"),GameKindRead.szKindName,CountArray(GameKindRead.szKindName));
		DataBaseAide.GetValue_String(TEXT("ProcessName"),GameKindRead.szProcessName,CountArray(GameKindRead.szProcessName));

		//�ƶ���¼
		pIDataBase->MoveToNext();

		//������Ϣ
		InsertGameKind(&GameKindRead);
	}

	//�رռ�¼
	pIDataBase->CloseRecordset();

	return;
}

//����վ��
void CServerListCenter::LoadGameStation(CDataBaseAide & DataBaseAide)
{
	//ִ������
	DataBaseAide.ResetParameter();
	DataBaseAide.ExecuteProcess(TEXT("GSP_LoadGameNodeItem"),true);

	//��������
	tagGameStation GameStationRead;
	ZeroMemory(&GameStationRead,sizeof(GameStationRead));

	//��ȡ����
	IDataBase * pIDataBase=(IDataBase *)DataBaseAide.GetDataBase(IID_IDataBase,VER_IDataBase);

	//��ȡ��Ϣ
	while (pIDataBase->IsRecordsetEnd()==false)
	{
		//��ȡ��Ϣ
		GameStationRead.wSortID=DataBaseAide.GetValue_WORD(TEXT("SortID"));
		GameStationRead.wKindID=DataBaseAide.GetValue_WORD(TEXT("KindID"));
		GameStationRead.wJoinID=DataBaseAide.GetValue_WORD(TEXT("JoinID"));
		GameStationRead.wStationID=DataBaseAide.GetValue_WORD(TEXT("NodeID"));
		DataBaseAide.GetValue_String(TEXT("NodeName"),GameStationRead.szStationName,CountArray(GameStationRead.szStationName));

		//������Ϣ
		InsertGameStation(&GameStationRead);

		//�ƶ���¼
		pIDataBase->MoveToNext();
	}

	//�رռ�¼
	pIDataBase->CloseRecordset();

	return;
}

//��ȡ����
UINT CServerListCenter::GetMaxPrime(UINT uLessNum)
{
	UINT uPrime=uLessNum;
	for (;uPrime>2;uPrime--)
	{
		UINT u2=2;
		for(;u2<=uPrime/2;u2++) if (uPrime%u2==0) break;
		if (u2>uPrime/2) break;
	}

	return uPrime;
}

//��������
void CServerListCenter::UpdateKindOnLineCount(WORD wKindID)
{
	tagGameKindItem * pGameKindItem=SearchGameKind(wKindID);
	if (pGameKindItem!=NULL)
	{
		POSITION Pos=NULL;
		DWORD dwKindAllLineCount=0L;
		tagGameServerItem * pGameServerItem=NULL;
		do
		{
			pGameServerItem=EmunGameServerItem(Pos);
			if (pGameServerItem==NULL) break;
			if (pGameServerItem->GameServer.wKindID==wKindID) dwKindAllLineCount+=pGameServerItem->GameServer.dwOnLineCount;
		} while (Pos!=NULL);
		pGameKindItem->GameKind.dwOnLineCount=dwKindAllLineCount;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateServerListCenter(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CServerListCenter * pServerListCenter=NULL;
	try
	{
		pServerListCenter=new CServerListCenter();
		if (pServerListCenter==NULL) throw TEXT("����ʧ��");
		void * pObject=pServerListCenter->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pServerListCenter);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
