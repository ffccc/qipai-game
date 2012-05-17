#include "StdAfx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CServerListData * CServerListData::m_pServerListData=NULL;				//�б�����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameListItem::CGameListItem(enItemGenre ItemGenre)
{
	//��������
	m_ItemGenre=ItemGenre;
	m_pParentListItem=NULL;

	//��������
	m_hTreeItemNormal=NULL;
	m_hTreeItemAttach=NULL;

	return;
}

//��������
CGameListItem::~CGameListItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameTypeItem::CGameTypeItem() : CGameListItem(ItemGenre_Type)
{
	//��������
	ZeroMemory(&m_GameType,sizeof(m_GameType));
}

//��������
CGameTypeItem::~CGameTypeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameKindItem::CGameKindItem() : CGameListItem(ItemGenre_Kind)
{
	//���±���
	m_dwUpdateTime=0;
	m_bUpdateItem=false;

	//���¾��
	m_hItemNormalUpdate=NULL;
	m_hItemAttachUpdate=NULL;

	//��չ����
	m_dwProcessVersion=0L;

	//��������
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));

	return;
}

//��������
CGameKindItem::~CGameKindItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameNodeItem::CGameNodeItem() : CGameListItem(ItemGenre_Node)
{
	//��������
	ZeroMemory(&m_GameNode,sizeof(m_GameNode));
}

//��������
CGameNodeItem::~CGameNodeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServerItem::CGameServerItem() : CGameListItem(ItemGenre_Server)
{
	//��������
	m_pGameKindItem=NULL;

	//��չ����
	m_ServerStatus=ServerStatus_Normal;

	//��������
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));
}

//��������
CGameServerItem::~CGameServerItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePageItem::CGamePageItem() : CGameListItem(ItemGenre_Page)
{
	//��������
	ZeroMemory(&m_GamePage,sizeof(m_GamePage));
}

//��������
CGamePageItem::~CGamePageItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameInsideItem::CGameInsideItem() : CGameListItem(ItemGenre_Inside)
{
}

//��������
CGameInsideItem::~CGameInsideItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerListData::CServerListData()
{
	//�ӿڱ���
	m_pIServerListDataSink=NULL;

	//��������
	m_GameTypeItemMap.InitHashTable(PRIME_TYPE);
	m_GameKindItemMap.InitHashTable(PRIME_KIND);
	m_GameNodeItemMap.InitHashTable(PRIME_NODE);
	m_GamePageItemMap.InitHashTable(PRIME_PAGE);
	m_GameServerItemMap.InitHashTable(PRIME_SERVER);

	//���ö���
	ASSERT(m_pServerListData==NULL);
	if (m_pServerListData==NULL) m_pServerListData=this;

	return;
}

//��������
CServerListData::~CServerListData()
{
	//��������
	WORD wKey=0;

	//ɾ������
	CGameTypeItem * pGameTypeItem=NULL;
	POSITION Position=m_GameTypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameTypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);
		SafeDelete(pGameTypeItem);
	}
	m_GameTypeItemMap.RemoveAll();

	//ɾ������
	CGameKindItem * pGameKindItem=NULL;
	Position=m_GameKindItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameKindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);
		SafeDelete(pGameKindItem);
	}
	m_GameKindItemMap.RemoveAll();

	//ɾ���ڵ�
	CGameNodeItem * pGameNodeItem=NULL;
	Position=m_GameNodeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameNodeItemMap.GetNextAssoc(Position,wKey,pGameNodeItem);
		SafeDelete(pGameNodeItem);
	}
	m_GameNodeItemMap.RemoveAll();

	//ɾ������
	CGameServerItem * pGameServerItem=NULL;
	Position=m_GameServerItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);
		SafeDelete(pGameServerItem);
	}
	m_GameServerItemMap.RemoveAll();

	//ɾ������
	CGamePageItem * pGamePageItem=NULL;
	Position=m_GamePageItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePageItemMap.GetNextAssoc(Position,wKey,pGamePageItem);
		SafeDelete(pGamePageItem);
	}
	m_GamePageItemMap.RemoveAll();

	//�ͷŶ���
	ASSERT(m_pServerListData==this);
	if (m_pServerListData==this) m_pServerListData=NULL;

	return;
}

//���ýӿ�
VOID CServerListData::SetServerListDataSink(IServerListDataSink * pIServerListDataSink)
{
	//���ñ���
	m_pIServerListDataSink=pIServerListDataSink;

	return;
}

//���֪ͨ
VOID CServerListData::OnEventListFinish()
{
	//�¼�֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemFinish();

	return;
}

//���֪ͨ
VOID CServerListData::OnEventKindFinish(WORD wKindID)
{
	//�¼�֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameKindFinish(wKindID);

	return;
}

//����֪ͨ
VOID CServerListData::OnEventDownLoadFinish(WORD wKindID)
{
	//��������
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.Lookup(wKindID,pGameKindItem);

	//������Ϣ
	if (pGameKindItem!=NULL)
	{
		//��ȡ�汾
		LPCTSTR pszProcessName=pGameKindItem->m_GameKind.szProcessName;
		CWHService::GetModuleVersion(pszProcessName,pGameKindItem->m_dwProcessVersion);

		//��������
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//��������
VOID CServerListData::SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount)
{
	//��������
	CGameKindItem * pGameKindItem=SearchGameKind(wKindID);

	//��������
	if (pGameKindItem!=NULL)
	{
		//���ñ���
		pGameKindItem->m_GameKind.dwOnLineCount=dwOnLineCount;

		//�¼�֪ͨ
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//��������
VOID CServerListData::SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount)
{
	//��������
	CGameServerItem * pGameServerItem=SearchGameServer(wServerID);

	//��������
	if (pGameServerItem!=NULL)
	{
		//���ñ���
		pGameServerItem->m_GameServer.dwOnLineCount=dwOnLineCount;

		//�¼�֪ͨ
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return;
}

//��������
bool CServerListData::InsertGameType(tagGameType * pGameType)
{
	//Ч�����
	ASSERT(pGameType!=NULL);
	if (pGameType==NULL) return false;

	//��������
	CGameTypeItem * pGameTypeItem=NULL;
	bool bInsert=(m_GameTypeItemMap.Lookup(pGameType->wTypeID,pGameTypeItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pGameTypeItem=new CGameTypeItem;
		if (pGameTypeItem==NULL) return false;
	}

	//��������
	CopyMemory(&pGameTypeItem->m_GameType,pGameType,sizeof(tagGameType));

	//Ѱ�Ҹ���
	if (pGameType->wJoinID!=0)
	{
		//Ѱ�Ҹ���
		pGameTypeItem->m_pParentListItem=SearchGameType(pGameType->wJoinID);

		//�����ж�
		if (pGameTypeItem->m_pParentListItem==NULL) 
		{
			m_GameListItemWait.Add(pGameTypeItem);
			return true;
		}
	}

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_GameTypeItemMap[pGameType->wTypeID]=pGameTypeItem;

		//�������
		m_pIServerListDataSink->OnGameItemInsert(pGameTypeItem);
	}
	else
	{
		//�������
		m_pIServerListDataSink->OnGameItemUpdate(pGameTypeItem);
	}

	return true;
}

//��������
bool CServerListData::InsertGameKind(tagGameKind * pGameKind)
{
	//Ч�����
	ASSERT(pGameKind!=NULL);
	if (pGameKind==NULL) return false;

	//��������
	CGameKindItem * pGameKindItem=NULL;
	bool bInsert=(m_GameKindItemMap.Lookup(pGameKind->wKindID,pGameKindItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pGameKindItem=new CGameKindItem;
		if (pGameKindItem==NULL) return false;
	}

	//��ȡ�汾
	if (bInsert==true)
	{
		LPCTSTR pszProcessName=pGameKind->szProcessName;
		CWHService::GetModuleVersion(pszProcessName,pGameKindItem->m_dwProcessVersion);
	}

	//��������
	CopyMemory(&pGameKindItem->m_GameKind,pGameKind,sizeof(tagGameKind));

	//Ѱ�Ҹ���
	if (pGameKind->wJoinID!=0)
	{
		pGameKindItem->m_pParentListItem=SearchGameKind(pGameKind->wJoinID);
	}
	else
	{
		pGameKindItem->m_pParentListItem=SearchGameType(pGameKind->wTypeID);
	}

	//�����ж�
	if (pGameKindItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameKindItem);
		return true;
	}

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_GameKindItemMap[pGameKind->wKindID]=pGameKindItem;

		//��������
		m_pIServerListDataSink->OnGameItemInsert(pGameKindItem);
	}
	else
	{
		//��������
		m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return true;
}

//����ڵ�
bool CServerListData::InsertGameNode(tagGameNode * pGameNode)
{
	//Ч�����
	ASSERT(pGameNode!=NULL);
	if (pGameNode==NULL) return false;

	//��������
	CGameNodeItem * pGameNodeItem=NULL;
	bool bInsert=(m_GameNodeItemMap.Lookup(pGameNode->wNodeID,pGameNodeItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pGameNodeItem=new CGameNodeItem;
		if (pGameNodeItem==NULL) return false;
	}

	//��������
	CopyMemory(&pGameNodeItem->m_GameNode,pGameNode,sizeof(tagGameNode));

	//Ѱ�Ҹ���
	if (pGameNode->wJoinID!=0)
	{
		pGameNodeItem->m_pParentListItem=SearchGameNode(pGameNode->wJoinID);
	}
	else
	{
		pGameNodeItem->m_pParentListItem=SearchGameKind(pGameNode->wKindID);
	}

	//�����ж�
	if (pGameNodeItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameNodeItem);
		return true;
	}

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_GameNodeItemMap[pGameNode->wNodeID]=pGameNodeItem;

		//��������
		m_pIServerListDataSink->OnGameItemInsert(pGameNodeItem);
	}
	else 
	{
		//��������
		m_pIServerListDataSink->OnGameItemUpdate(pGameNodeItem);
	}

	return true;
}

//���붨��
bool CServerListData::InsertGamePage(tagGamePage * pGamePage)
{
	//Ч�����
	ASSERT(pGamePage!=NULL);
	if (pGamePage==NULL) return false;

	//��������
	CGamePageItem * pGamePageItem=NULL;
	bool bInsert=(m_GamePageItemMap.Lookup(pGamePage->wPageID,pGamePageItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pGamePageItem=new CGamePageItem;
		if (pGamePageItem==NULL) return false;
	}

	//��������
	CopyMemory(&pGamePageItem->m_GamePage,pGamePage,sizeof(tagGamePage));

	//Ѱ�Ҹ���
	if ((pGamePage->wNodeID!=0)||(pGamePage->wKindID!=0))
	{
		//Ѱ�Ҹ���
		if (pGamePage->wNodeID!=0) pGamePageItem->m_pParentListItem=SearchGameNode(pGamePage->wNodeID);
		else if (pGamePage->wKindID!=0) pGamePageItem->m_pParentListItem=SearchGameKind(pGamePage->wKindID);

		//�����ж�
		if (pGamePageItem->m_pParentListItem==NULL) 
		{
			m_GameListItemWait.Add(pGamePageItem);
			return true;
		}
	}

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_GamePageItemMap[pGamePage->wPageID]=pGamePageItem;

		//��������
		m_pIServerListDataSink->OnGameItemInsert(pGamePageItem);
	}
	else 
	{
		//��������
		m_pIServerListDataSink->OnGameItemUpdate(pGamePageItem);
	}

	return true;
}

//���뷿��
bool CServerListData::InsertGameServer(tagGameServer * pGameServer)
{
	//Ч�����
	ASSERT(pGameServer!=NULL);
	if (pGameServer==NULL) return false;

	//��������
	CGameServerItem * pGameServerItem=NULL;
	bool bInsert=(m_GameServerItemMap.Lookup(pGameServer->wServerID,pGameServerItem)==FALSE);

	//��ȡ�洢
	if (bInsert==true)
	{
		pGameServerItem=new CGameServerItem;
		if (pGameServerItem==NULL) return false;
	}

	//��������
	CopyMemory(&pGameServerItem->m_GameServer,pGameServer,sizeof(tagGameServer));

	//Ѱ�Ҹ���
	if (pGameServer->wNodeID!=0)
	{
		pGameServerItem->m_pGameKindItem=SearchGameKind(pGameServer->wKindID);
		pGameServerItem->m_pParentListItem=SearchGameNode(pGameServer->wNodeID);
	}
	else
	{
		pGameServerItem->m_pGameKindItem=SearchGameKind(pGameServer->wKindID);
		pGameServerItem->m_pParentListItem=SearchGameKind(pGameServer->wKindID);
	}

	//�����ж�
	if (pGameServerItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameServerItem);
		return true;
	}

	//���봦��
	if (bInsert==true)
	{
		//��������
		m_GameServerItemMap[pGameServer->wServerID]=pGameServerItem;

		//��������
		m_pIServerListDataSink->OnGameItemInsert(pGameServerItem);
	}
	else 
	{
		//��������
		m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return true;
}

//ɾ������
bool CServerListData::DeleteGameType(WORD wTypeID)
{
	//��������
	CGameTypeItem * pGameTypeItem=NULL;
	if (m_GameTypeItemMap.Lookup(wTypeID,pGameTypeItem)==FALSE) return false;

	//ɾ��֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameTypeItem);

	//��������
	SafeDelete(pGameTypeItem);

	//ɾ������
	m_GameTypeItemMap.RemoveKey(wTypeID);

	return true;
}

//ɾ������
bool CServerListData::DeleteGameKind(WORD wKindID)
{
	//��������
	CGameKindItem * pGameKindItem=NULL;
	if (m_GameKindItemMap.Lookup(wKindID,pGameKindItem)==FALSE) return false;

	//ɾ��֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameKindItem);

	//ɾ������
	SafeDelete(pGameKindItem);

	//ɾ������
	m_GameKindItemMap.RemoveKey(wKindID);

	return true;
}

//ɾ���ڵ�
bool CServerListData::DeleteGameNode(WORD wNodeID)
{
	//���ҽڵ�
	CGameNodeItem * pGameNodeItem=NULL;
	if (m_GameNodeItemMap.Lookup(wNodeID,pGameNodeItem)==FALSE) return false;

	//ɾ��֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameNodeItem);

	//ɾ������
	SafeDelete(pGameNodeItem);

	//ɾ������
	m_GameNodeItemMap.RemoveKey(wNodeID);

	return false;
}

//ɾ������
bool CServerListData::DeleteGameServer(WORD wServerID)
{
	//���ҷ���
	CGameServerItem * pGameServerItem=NULL;
	if (m_GameServerItemMap.Lookup(wServerID,pGameServerItem)==FALSE) return false;

	//ɾ��֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameServerItem);

	//ɾ������
	SafeDelete(pGameServerItem);

	//ɾ������
	m_GameServerItemMap.RemoveKey(wServerID);

	return false;
}

//ɾ������
bool CServerListData::DeleteGamePage(WORD wPageID)
{
	//��������
	CGamePageItem * pGamePageItem=NULL;
	if (m_GamePageItemMap.Lookup(wPageID,pGamePageItem)==FALSE) return false;

	//ɾ��֪ͨ
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGamePageItem);

	//ɾ������
	SafeDelete(pGamePageItem);

	//ɾ������
	m_GamePageItemMap.RemoveKey(wPageID);

	return true;
}

//ö������
CGameTypeItem * CServerListData::EmunGameTypeItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_GameTypeItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	CGameTypeItem * pGameTypeItem=NULL;
	m_GameTypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);

	return pGameTypeItem;
}

//ö������
CGameKindItem * CServerListData::EmunGameKindItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_GameKindItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);

	return pGameKindItem;
}

//ö�ٽڵ�
CGameNodeItem * CServerListData::EmunGameNodeItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_GameNodeItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���ҽڵ�
	WORD wKey=0;
	CGameNodeItem * pGameNodeItem=NULL;
	m_GameNodeItemMap.GetNextAssoc(Position,wKey,pGameNodeItem);

	return pGameNodeItem;
}

//ö�ٶ���
CGamePageItem * CServerListData::EmunGamePageItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_GamePageItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	CGamePageItem * pGamePageItem=NULL;
	m_GamePageItemMap.GetNextAssoc(Position,wKey,pGamePageItem);

	return pGamePageItem;
}

//ö�ٷ���
CGameServerItem * CServerListData::EmunGameServerItem(POSITION & Position)
{
	//��������
	if (Position==NULL) Position=m_GameServerItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	CGameServerItem * pGameServerItem=NULL;
	m_GameServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);

	return pGameServerItem;
}

//��������
CGameTypeItem * CServerListData::SearchGameType(WORD wTypeID)
{
	CGameTypeItem * pGameTypeItem=NULL;
	m_GameTypeItemMap.Lookup(wTypeID,pGameTypeItem);
	return pGameTypeItem;
}

//��������
CGameKindItem * CServerListData::SearchGameKind(WORD wKindID)
{
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.Lookup(wKindID,pGameKindItem);
	return pGameKindItem;
}

//���ҽڵ�
CGameNodeItem * CServerListData::SearchGameNode(WORD wNodeID)
{
	CGameNodeItem * pGameNodeItem=NULL;
	m_GameNodeItemMap.Lookup(wNodeID,pGameNodeItem);
	return pGameNodeItem;
}

//���ҷ���
CGameServerItem * CServerListData::SearchGameServer(WORD wServerID)
{
	CGameServerItem * pGameServerItem=NULL;
	m_GameServerItemMap.Lookup(wServerID,pGameServerItem);
	return pGameServerItem;
}

//���Ҷ���
CGamePageItem * CServerListData::SearchGamePage(WORD wPageID)
{
	CGamePageItem * pGamePageItem=NULL;
	m_GamePageItemMap.Lookup(wPageID,pGamePageItem);
	return pGamePageItem;
}

//////////////////////////////////////////////////////////////////////////////////
