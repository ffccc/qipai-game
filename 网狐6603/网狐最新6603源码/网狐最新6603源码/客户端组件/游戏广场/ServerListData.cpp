#include "StdAfx.h"
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CServerListData * CServerListData::m_pServerListData=NULL;				//列表数据

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameListItem::CGameListItem(enItemGenre ItemGenre)
{
	//属性数据
	m_ItemGenre=ItemGenre;
	m_pParentListItem=NULL;

	//子项数据
	m_hTreeItemNormal=NULL;
	m_hTreeItemAttach=NULL;

	return;
}

//析构函数
CGameListItem::~CGameListItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameTypeItem::CGameTypeItem() : CGameListItem(ItemGenre_Type)
{
	//属性数据
	ZeroMemory(&m_GameType,sizeof(m_GameType));
}

//析构函数
CGameTypeItem::~CGameTypeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameKindItem::CGameKindItem() : CGameListItem(ItemGenre_Kind)
{
	//更新变量
	m_dwUpdateTime=0;
	m_bUpdateItem=false;

	//更新句柄
	m_hItemNormalUpdate=NULL;
	m_hItemAttachUpdate=NULL;

	//扩展数据
	m_dwProcessVersion=0L;

	//属性数据
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));

	return;
}

//析构函数
CGameKindItem::~CGameKindItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameNodeItem::CGameNodeItem() : CGameListItem(ItemGenre_Node)
{
	//属性数据
	ZeroMemory(&m_GameNode,sizeof(m_GameNode));
}

//析构函数
CGameNodeItem::~CGameNodeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameServerItem::CGameServerItem() : CGameListItem(ItemGenre_Server)
{
	//辅助变量
	m_pGameKindItem=NULL;

	//扩展数据
	m_ServerStatus=ServerStatus_Normal;

	//属性数据
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));
}

//析构函数
CGameServerItem::~CGameServerItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePageItem::CGamePageItem() : CGameListItem(ItemGenre_Page)
{
	//属性数据
	ZeroMemory(&m_GamePage,sizeof(m_GamePage));
}

//析构函数
CGamePageItem::~CGamePageItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameInsideItem::CGameInsideItem() : CGameListItem(ItemGenre_Inside)
{
}

//析构函数
CGameInsideItem::~CGameInsideItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CServerListData::CServerListData()
{
	//接口变量
	m_pIServerListDataSink=NULL;

	//设置质数
	m_GameTypeItemMap.InitHashTable(PRIME_TYPE);
	m_GameKindItemMap.InitHashTable(PRIME_KIND);
	m_GameNodeItemMap.InitHashTable(PRIME_NODE);
	m_GamePageItemMap.InitHashTable(PRIME_PAGE);
	m_GameServerItemMap.InitHashTable(PRIME_SERVER);

	//设置对象
	ASSERT(m_pServerListData==NULL);
	if (m_pServerListData==NULL) m_pServerListData=this;

	return;
}

//析构函数
CServerListData::~CServerListData()
{
	//变量定义
	WORD wKey=0;

	//删除种类
	CGameTypeItem * pGameTypeItem=NULL;
	POSITION Position=m_GameTypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameTypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);
		SafeDelete(pGameTypeItem);
	}
	m_GameTypeItemMap.RemoveAll();

	//删除类型
	CGameKindItem * pGameKindItem=NULL;
	Position=m_GameKindItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameKindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);
		SafeDelete(pGameKindItem);
	}
	m_GameKindItemMap.RemoveAll();

	//删除节点
	CGameNodeItem * pGameNodeItem=NULL;
	Position=m_GameNodeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameNodeItemMap.GetNextAssoc(Position,wKey,pGameNodeItem);
		SafeDelete(pGameNodeItem);
	}
	m_GameNodeItemMap.RemoveAll();

	//删除房间
	CGameServerItem * pGameServerItem=NULL;
	Position=m_GameServerItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GameServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);
		SafeDelete(pGameServerItem);
	}
	m_GameServerItemMap.RemoveAll();

	//删除定制
	CGamePageItem * pGamePageItem=NULL;
	Position=m_GamePageItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_GamePageItemMap.GetNextAssoc(Position,wKey,pGamePageItem);
		SafeDelete(pGamePageItem);
	}
	m_GamePageItemMap.RemoveAll();

	//释放对象
	ASSERT(m_pServerListData==this);
	if (m_pServerListData==this) m_pServerListData=NULL;

	return;
}

//设置接口
VOID CServerListData::SetServerListDataSink(IServerListDataSink * pIServerListDataSink)
{
	//设置变量
	m_pIServerListDataSink=pIServerListDataSink;

	return;
}

//完成通知
VOID CServerListData::OnEventListFinish()
{
	//事件通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemFinish();

	return;
}

//完成通知
VOID CServerListData::OnEventKindFinish(WORD wKindID)
{
	//事件通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameKindFinish(wKindID);

	return;
}

//下载通知
VOID CServerListData::OnEventDownLoadFinish(WORD wKindID)
{
	//变量定义
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.Lookup(wKindID,pGameKindItem);

	//更新信息
	if (pGameKindItem!=NULL)
	{
		//获取版本
		LPCTSTR pszProcessName=pGameKindItem->m_GameKind.szProcessName;
		CWHService::GetModuleVersion(pszProcessName,pGameKindItem->m_dwProcessVersion);

		//更新子项
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//设置人数
VOID CServerListData::SetKindOnLineCount(WORD wKindID, DWORD dwOnLineCount)
{
	//搜索类型
	CGameKindItem * pGameKindItem=SearchGameKind(wKindID);

	//设置人数
	if (pGameKindItem!=NULL)
	{
		//设置变量
		pGameKindItem->m_GameKind.dwOnLineCount=dwOnLineCount;

		//事件通知
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//设置人数
VOID CServerListData::SetServerOnLineCount(WORD wServerID, DWORD dwOnLineCount)
{
	//搜索房间
	CGameServerItem * pGameServerItem=SearchGameServer(wServerID);

	//设置人数
	if (pGameServerItem!=NULL)
	{
		//设置变量
		pGameServerItem->m_GameServer.dwOnLineCount=dwOnLineCount;

		//事件通知
		ASSERT(m_pIServerListDataSink!=NULL);
		if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return;
}

//插入种类
bool CServerListData::InsertGameType(tagGameType * pGameType)
{
	//效验参数
	ASSERT(pGameType!=NULL);
	if (pGameType==NULL) return false;

	//变量定义
	CGameTypeItem * pGameTypeItem=NULL;
	bool bInsert=(m_GameTypeItemMap.Lookup(pGameType->wTypeID,pGameTypeItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pGameTypeItem=new CGameTypeItem;
		if (pGameTypeItem==NULL) return false;
	}

	//设置数据
	CopyMemory(&pGameTypeItem->m_GameType,pGameType,sizeof(tagGameType));

	//寻找父项
	if (pGameType->wJoinID!=0)
	{
		//寻找父项
		pGameTypeItem->m_pParentListItem=SearchGameType(pGameType->wJoinID);

		//待决判断
		if (pGameTypeItem->m_pParentListItem==NULL) 
		{
			m_GameListItemWait.Add(pGameTypeItem);
			return true;
		}
	}

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameTypeItemMap[pGameType->wTypeID]=pGameTypeItem;

		//界面更新
		m_pIServerListDataSink->OnGameItemInsert(pGameTypeItem);
	}
	else
	{
		//界面更新
		m_pIServerListDataSink->OnGameItemUpdate(pGameTypeItem);
	}

	return true;
}

//插入类型
bool CServerListData::InsertGameKind(tagGameKind * pGameKind)
{
	//效验参数
	ASSERT(pGameKind!=NULL);
	if (pGameKind==NULL) return false;

	//变量定义
	CGameKindItem * pGameKindItem=NULL;
	bool bInsert=(m_GameKindItemMap.Lookup(pGameKind->wKindID,pGameKindItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pGameKindItem=new CGameKindItem;
		if (pGameKindItem==NULL) return false;
	}

	//获取版本
	if (bInsert==true)
	{
		LPCTSTR pszProcessName=pGameKind->szProcessName;
		CWHService::GetModuleVersion(pszProcessName,pGameKindItem->m_dwProcessVersion);
	}

	//设置数据
	CopyMemory(&pGameKindItem->m_GameKind,pGameKind,sizeof(tagGameKind));

	//寻找父项
	if (pGameKind->wJoinID!=0)
	{
		pGameKindItem->m_pParentListItem=SearchGameKind(pGameKind->wJoinID);
	}
	else
	{
		pGameKindItem->m_pParentListItem=SearchGameType(pGameKind->wTypeID);
	}

	//待决判断
	if (pGameKindItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameKindItem);
		return true;
	}

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameKindItemMap[pGameKind->wKindID]=pGameKindItem;

		//插入子项
		m_pIServerListDataSink->OnGameItemInsert(pGameKindItem);
	}
	else
	{
		//更新子项
		m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return true;
}

//插入节点
bool CServerListData::InsertGameNode(tagGameNode * pGameNode)
{
	//效验参数
	ASSERT(pGameNode!=NULL);
	if (pGameNode==NULL) return false;

	//变量定义
	CGameNodeItem * pGameNodeItem=NULL;
	bool bInsert=(m_GameNodeItemMap.Lookup(pGameNode->wNodeID,pGameNodeItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pGameNodeItem=new CGameNodeItem;
		if (pGameNodeItem==NULL) return false;
	}

	//设置数据
	CopyMemory(&pGameNodeItem->m_GameNode,pGameNode,sizeof(tagGameNode));

	//寻找父项
	if (pGameNode->wJoinID!=0)
	{
		pGameNodeItem->m_pParentListItem=SearchGameNode(pGameNode->wJoinID);
	}
	else
	{
		pGameNodeItem->m_pParentListItem=SearchGameKind(pGameNode->wKindID);
	}

	//待决判断
	if (pGameNodeItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameNodeItem);
		return true;
	}

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameNodeItemMap[pGameNode->wNodeID]=pGameNodeItem;

		//插入子项
		m_pIServerListDataSink->OnGameItemInsert(pGameNodeItem);
	}
	else 
	{
		//更新子项
		m_pIServerListDataSink->OnGameItemUpdate(pGameNodeItem);
	}

	return true;
}

//插入定制
bool CServerListData::InsertGamePage(tagGamePage * pGamePage)
{
	//效验参数
	ASSERT(pGamePage!=NULL);
	if (pGamePage==NULL) return false;

	//变量定义
	CGamePageItem * pGamePageItem=NULL;
	bool bInsert=(m_GamePageItemMap.Lookup(pGamePage->wPageID,pGamePageItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pGamePageItem=new CGamePageItem;
		if (pGamePageItem==NULL) return false;
	}

	//设置数据
	CopyMemory(&pGamePageItem->m_GamePage,pGamePage,sizeof(tagGamePage));

	//寻找父项
	if ((pGamePage->wNodeID!=0)||(pGamePage->wKindID!=0))
	{
		//寻找父项
		if (pGamePage->wNodeID!=0) pGamePageItem->m_pParentListItem=SearchGameNode(pGamePage->wNodeID);
		else if (pGamePage->wKindID!=0) pGamePageItem->m_pParentListItem=SearchGameKind(pGamePage->wKindID);

		//待决判断
		if (pGamePageItem->m_pParentListItem==NULL) 
		{
			m_GameListItemWait.Add(pGamePageItem);
			return true;
		}
	}

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GamePageItemMap[pGamePage->wPageID]=pGamePageItem;

		//插入子项
		m_pIServerListDataSink->OnGameItemInsert(pGamePageItem);
	}
	else 
	{
		//更新子项
		m_pIServerListDataSink->OnGameItemUpdate(pGamePageItem);
	}

	return true;
}

//插入房间
bool CServerListData::InsertGameServer(tagGameServer * pGameServer)
{
	//效验参数
	ASSERT(pGameServer!=NULL);
	if (pGameServer==NULL) return false;

	//变量定义
	CGameServerItem * pGameServerItem=NULL;
	bool bInsert=(m_GameServerItemMap.Lookup(pGameServer->wServerID,pGameServerItem)==FALSE);

	//获取存储
	if (bInsert==true)
	{
		pGameServerItem=new CGameServerItem;
		if (pGameServerItem==NULL) return false;
	}

	//设置数据
	CopyMemory(&pGameServerItem->m_GameServer,pGameServer,sizeof(tagGameServer));

	//寻找父项
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

	//待决判断
	if (pGameServerItem->m_pParentListItem==NULL) 
	{
		m_GameListItemWait.Add(pGameServerItem);
		return true;
	}

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameServerItemMap[pGameServer->wServerID]=pGameServerItem;

		//插入子项
		m_pIServerListDataSink->OnGameItemInsert(pGameServerItem);
	}
	else 
	{
		//更新子项
		m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return true;
}

//删除种类
bool CServerListData::DeleteGameType(WORD wTypeID)
{
	//查找种类
	CGameTypeItem * pGameTypeItem=NULL;
	if (m_GameTypeItemMap.Lookup(wTypeID,pGameTypeItem)==FALSE) return false;

	//删除通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameTypeItem);

	//重置数据
	SafeDelete(pGameTypeItem);

	//删除数据
	m_GameTypeItemMap.RemoveKey(wTypeID);

	return true;
}

//删除类型
bool CServerListData::DeleteGameKind(WORD wKindID)
{
	//查找类型
	CGameKindItem * pGameKindItem=NULL;
	if (m_GameKindItemMap.Lookup(wKindID,pGameKindItem)==FALSE) return false;

	//删除通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameKindItem);

	//删除数据
	SafeDelete(pGameKindItem);

	//删除数据
	m_GameKindItemMap.RemoveKey(wKindID);

	return true;
}

//删除节点
bool CServerListData::DeleteGameNode(WORD wNodeID)
{
	//查找节点
	CGameNodeItem * pGameNodeItem=NULL;
	if (m_GameNodeItemMap.Lookup(wNodeID,pGameNodeItem)==FALSE) return false;

	//删除通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameNodeItem);

	//删除数据
	SafeDelete(pGameNodeItem);

	//删除数据
	m_GameNodeItemMap.RemoveKey(wNodeID);

	return false;
}

//删除房间
bool CServerListData::DeleteGameServer(WORD wServerID)
{
	//查找房间
	CGameServerItem * pGameServerItem=NULL;
	if (m_GameServerItemMap.Lookup(wServerID,pGameServerItem)==FALSE) return false;

	//删除通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGameServerItem);

	//删除数据
	SafeDelete(pGameServerItem);

	//删除数据
	m_GameServerItemMap.RemoveKey(wServerID);

	return false;
}

//删除定制
bool CServerListData::DeleteGamePage(WORD wPageID)
{
	//查找类型
	CGamePageItem * pGamePageItem=NULL;
	if (m_GamePageItemMap.Lookup(wPageID,pGamePageItem)==FALSE) return false;

	//删除通知
	ASSERT(m_pIServerListDataSink!=NULL);
	if (m_pIServerListDataSink!=NULL) m_pIServerListDataSink->OnGameItemDelete(pGamePageItem);

	//删除数据
	SafeDelete(pGamePageItem);

	//删除数据
	m_GamePageItemMap.RemoveKey(wPageID);

	return true;
}

//枚举种类
CGameTypeItem * CServerListData::EmunGameTypeItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_GameTypeItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找房间
	WORD wKey=0;
	CGameTypeItem * pGameTypeItem=NULL;
	m_GameTypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);

	return pGameTypeItem;
}

//枚举类型
CGameKindItem * CServerListData::EmunGameKindItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_GameKindItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找房间
	WORD wKey=0;
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);

	return pGameKindItem;
}

//枚举节点
CGameNodeItem * CServerListData::EmunGameNodeItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_GameNodeItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找节点
	WORD wKey=0;
	CGameNodeItem * pGameNodeItem=NULL;
	m_GameNodeItemMap.GetNextAssoc(Position,wKey,pGameNodeItem);

	return pGameNodeItem;
}

//枚举定制
CGamePageItem * CServerListData::EmunGamePageItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_GamePageItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找房间
	WORD wKey=0;
	CGamePageItem * pGamePageItem=NULL;
	m_GamePageItemMap.GetNextAssoc(Position,wKey,pGamePageItem);

	return pGamePageItem;
}

//枚举房间
CGameServerItem * CServerListData::EmunGameServerItem(POSITION & Position)
{
	//调整参数
	if (Position==NULL) Position=m_GameServerItemMap.GetStartPosition();
	if (Position==NULL) return NULL;

	//查找房间
	WORD wKey=0;
	CGameServerItem * pGameServerItem=NULL;
	m_GameServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);

	return pGameServerItem;
}

//查找种类
CGameTypeItem * CServerListData::SearchGameType(WORD wTypeID)
{
	CGameTypeItem * pGameTypeItem=NULL;
	m_GameTypeItemMap.Lookup(wTypeID,pGameTypeItem);
	return pGameTypeItem;
}

//查找类型
CGameKindItem * CServerListData::SearchGameKind(WORD wKindID)
{
	CGameKindItem * pGameKindItem=NULL;
	m_GameKindItemMap.Lookup(wKindID,pGameKindItem);
	return pGameKindItem;
}

//查找节点
CGameNodeItem * CServerListData::SearchGameNode(WORD wNodeID)
{
	CGameNodeItem * pGameNodeItem=NULL;
	m_GameNodeItemMap.Lookup(wNodeID,pGameNodeItem);
	return pGameNodeItem;
}

//查找房间
CGameServerItem * CServerListData::SearchGameServer(WORD wServerID)
{
	CGameServerItem * pGameServerItem=NULL;
	m_GameServerItemMap.Lookup(wServerID,pGameServerItem);
	return pGameServerItem;
}

//查找定制
CGamePageItem * CServerListData::SearchGamePage(WORD wPageID)
{
	CGamePageItem * pGamePageItem=NULL;
	m_GamePageItemMap.Lookup(wPageID,pGamePageItem);
	return pGamePageItem;
}

//////////////////////////////////////////////////////////////////////////////////
