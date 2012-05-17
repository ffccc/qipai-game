#include "StdAfx.h"
#include "GlobalInfoManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalUserItem::CGlobalUserItem()
{
	//用户属性
	m_dwUserID=0L;
	m_dwGameID=0L;
	m_szNickName[0]=0;

	//等级信息
	m_cbMemberOrder=0;
	m_cbMasterOrder=0;

	//链表属性
	m_pNextUserItemPtr=NULL;

	return;
}

//析构函数
CGlobalUserItem::~CGlobalUserItem()
{
}

//枚举房间
CGlobalServerItem * CGlobalUserItem::EnumServerItem(WORD wIndex)
{
	if (wIndex>=m_GlobalServerItemArray.GetCount()) return NULL;
	return m_GlobalServerItemArray[wIndex];
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalPlazaItem::CGlobalPlazaItem()
{
	//设置变量
	m_wIndex=0;
	ZeroMemory(&m_GamePlaza,sizeof(m_GamePlaza));

	//链表属性
	m_pNextPlazaItemPtr=NULL;

	return;
}

//析构函数
CGlobalPlazaItem::~CGlobalPlazaItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalServerItem::CGlobalServerItem()
{
	//设置变量
	m_wIndex=0;
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//链表属性
	m_pNextServerItemPtr=NULL;

	//设置质数
	m_MapUserID.InitHashTable(PRIME_SERVER_USER);

	return;
}

//析构函数
CGlobalServerItem::~CGlobalServerItem()
{
}

//寻找用户
CGlobalUserItem * CGlobalServerItem::SearchUserItem(DWORD dwUserID)
{
	//搜索用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalInfoManager::CGlobalInfoManager()
{
	//存储变量
	m_pGlobalUserItem=NULL;
	m_pGlobalPlazaItem=NULL;
	m_pGlobalServerItem=NULL;

	//设置索引
	m_MapPlazaID.InitHashTable(PRIME_SERVER);
	m_MapServerID.InitHashTable(PRIME_SERVER);
	m_MapUserID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapGameID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapNickName.InitHashTable(PRIME_PLATFORM_USER);

	return;
}

//析构函数
CGlobalInfoManager::~CGlobalInfoManager()
{
	//删除用户
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		SafeDelete(pGlobalUserItem);
	}
	m_MapUserID.RemoveAll();

	//删除广场
	WORD wPlazaID=0;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	Position=m_MapPlazaID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);
		SafeDelete(pGlobalPlazaItem);
	}
	m_MapPlazaID.RemoveAll();

	//删除房间
	WORD wServerID=0;
	CGlobalServerItem * pGlobalServerItem=NULL;
	Position=m_MapServerID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);
		SafeDelete(pGlobalServerItem);
	}
	m_MapServerID.RemoveAll();

	//删除用户
	while (m_pGlobalUserItem!=NULL)
	{
		pGlobalUserItem=m_pGlobalUserItem;
		m_pGlobalUserItem=m_pGlobalUserItem->m_pNextUserItemPtr;
		SafeDelete(pGlobalUserItem);
	}

	//删除广场
	while (m_pGlobalPlazaItem!=NULL)
	{
		pGlobalPlazaItem=m_pGlobalPlazaItem;
		m_pGlobalPlazaItem=m_pGlobalPlazaItem->m_pNextPlazaItemPtr;
		SafeDelete(pGlobalPlazaItem);
	}

	//删除房间
	while (m_pGlobalServerItem!=NULL)
	{
		pGlobalServerItem=m_pGlobalServerItem;
		m_pGlobalServerItem=m_pGlobalServerItem->m_pNextServerItemPtr;
		SafeDelete(pGlobalServerItem);
	}

	return;
}

//重置数据
VOID CGlobalInfoManager::ResetData()
{
	//删除用户
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();

	//删除用户
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		if (pGlobalUserItem!=NULL) FreeGlobalUserItem(pGlobalUserItem);
	}

	//删除广场
	WORD wPlazaID=0;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	Position=m_MapPlazaID.GetStartPosition();

	//删除广场
	while (Position!=NULL)
	{
		m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);
		if (pGlobalPlazaItem!=NULL) FreeGlobalPlazaItem(pGlobalPlazaItem);
	}

	//删除房间
	WORD wServerID=0;
	CGlobalServerItem * pGlobalServerItem=NULL;
	Position=m_MapServerID.GetStartPosition();

	//删除房间
	while (Position!=NULL)
	{
		m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);
		if (pGlobalServerItem!=NULL) FreeGlobalServerItem(pGlobalServerItem);
	}

	//删除索引
	m_MapUserID.RemoveAll();
	m_MapGameID.RemoveAll();
	m_MapPlazaID.RemoveAll();
	m_MapServerID.RemoveAll();
	m_MapNickName.RemoveAll();

	return;
}

//删除用户
bool CGlobalInfoManager::DeleteUserItem(DWORD dwUserID, WORD wServerID)
{
	//寻找用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	if (m_MapUserID.Lookup(dwUserID,pGlobalUserItem)==FALSE) 
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	CGlobalServerItem * pGlobalServerItem=NULL;
	INT_PTR nServerCount=pGlobalUserItem->m_GlobalServerItemArray.GetCount();

	//退出房间
	for (INT_PTR i=0;i<nServerCount;i++)
	{
		//获取房间
		pGlobalServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

		//房间判断
		if (pGlobalServerItem->GetServerID()==wServerID)
		{
			//删除关联
			pGlobalServerItem->m_MapUserID.RemoveKey(dwUserID);
			pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);

			//释放用户
			if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0L) 
			{
				//释放索引
				m_MapUserID.RemoveKey(dwUserID);
				m_MapGameID.RemoveKey(pGlobalUserItem->GetGameID());
				m_MapNickName.RemoveKey(pGlobalUserItem->GetNickName());

				//释放对象
				FreeGlobalUserItem(pGlobalUserItem);
			}

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//激活用户
bool CGlobalInfoManager::ActiveUserItem(tagGlobalUserInfo & GlobalUserInfo, WORD wServerID)
{
	//寻找房间
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(wServerID,pGlobalServerItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//寻找用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	if (m_MapUserID.Lookup(GlobalUserInfo.dwUserID,pGlobalUserItem)==FALSE)
	{
		//创建用户
		pGlobalUserItem=CreateGlobalUserItem();
		if (pGlobalUserItem==NULL) 
		{
			ASSERT(FALSE);
			return false;
		}

		//构造昵称
		TCHAR szNickName[LEN_NICKNAME]=TEXT("");
		lstrcpyn(szNickName,GlobalUserInfo.szNickName,CountArray(szNickName));

		//设置用户
		pGlobalUserItem->m_dwUserID=GlobalUserInfo.dwUserID;
		pGlobalUserItem->m_dwGameID=GlobalUserInfo.dwGameID;
		lstrcpyn(pGlobalUserItem->m_szNickName,GlobalUserInfo.szNickName,CountArray(pGlobalUserItem->m_szNickName));

		//辅助信息
		pGlobalUserItem->m_cbGender=GlobalUserInfo.cbGender;
		pGlobalUserItem->m_cbMemberOrder=GlobalUserInfo.cbMemberOrder;
		pGlobalUserItem->m_cbMasterOrder=GlobalUserInfo.cbMasterOrder;

		//昵称索引
		m_MapNickName[_tcslwr(szNickName)]=pGlobalUserItem;

		//设置索引
		m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;
		m_MapGameID[GlobalUserInfo.dwGameID]=pGlobalUserItem;
	}
	else
	{
		//重复判断
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			if (pGlobalUserItem->m_GlobalServerItemArray[i]->GetServerID()==wServerID)
			{
				ASSERT(FALSE);
				return false;
			}
		}
	}

	//设置关联
	pGlobalUserItem->m_GlobalServerItemArray.Add(pGlobalServerItem);
	pGlobalServerItem->m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;

	return true;
}

//删除广场
bool CGlobalInfoManager::DeletePlazaItem(WORD wPlazaID)
{
	//寻找广场
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	if (m_MapPlazaID.Lookup(wPlazaID,pGlobalPlazaItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//释放广场
	m_MapPlazaID.RemoveKey(wPlazaID);
	FreeGlobalPlazaItem(pGlobalPlazaItem);

	return true;
}

//激活广场
bool CGlobalInfoManager::ActivePlazaItem(WORD wBindIndex, tagGamePlaza & GamePlaza)
{
	//寻找广场
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	if (m_MapPlazaID.Lookup(GamePlaza.wPlazaID,pGlobalPlazaItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建广场
	pGlobalPlazaItem=CreateGlobalPlazaItem();
	if (pGlobalPlazaItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置广场
	pGlobalPlazaItem->m_wIndex=wBindIndex;
	pGlobalPlazaItem->m_GamePlaza=GamePlaza;

	//设置索引
	m_MapPlazaID[GamePlaza.wPlazaID]=pGlobalPlazaItem;

	return true;
}

//删除房间
bool CGlobalInfoManager::DeleteServerItem(WORD wServerID)
{
	//寻找房间
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(wServerID,pGlobalServerItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	DWORD dwUserKey=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=pGlobalServerItem->m_MapUserID.GetStartPosition();

	//删除用户
	while (Position!=NULL)
	{
		//获取数据
		pGlobalServerItem->m_MapUserID.GetNextAssoc(Position,dwUserKey,pGlobalUserItem);

		//房间关联
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			//获取房间
			CGlobalServerItem * pTempServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

			//房间判断
			if (pTempServerItem->GetServerID()==wServerID)
			{
				pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);
				break;
			}
		}

		//释放用户
		if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0)
		{
			m_MapUserID.RemoveKey(dwUserKey);
			FreeGlobalUserItem(pGlobalUserItem);
		}
	}

	//释放房间
	m_MapServerID.RemoveKey(wServerID);
	FreeGlobalServerItem(pGlobalServerItem);

	return true;
}

//激活房间
bool CGlobalInfoManager::ActiveServerItem(WORD wBindIndex, tagGameServer & GameServer)
{
	//寻找房间
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(GameServer.wServerID,pGlobalServerItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建房间
	pGlobalServerItem=CreateGlobalServerItem();
	if (pGlobalServerItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置房间
	pGlobalServerItem->m_wIndex=wBindIndex;
	pGlobalServerItem->m_GameServer=GameServer;

	//设置索引
	m_MapServerID[GameServer.wServerID]=pGlobalServerItem;

	return true;
}

//寻找广场
CGlobalPlazaItem * CGlobalInfoManager::SearchPlazaItem(WORD wPlazaID)
{
	//寻找房间
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	m_MapPlazaID.Lookup(wPlazaID,pGlobalPlazaItem);

	return pGlobalPlazaItem;
}

//寻找房间
CGlobalServerItem * CGlobalInfoManager::SearchServerItem(WORD wServerID)
{
	//寻找房间
	CGlobalServerItem * pGlobalServerItem=NULL;
	m_MapServerID.Lookup(wServerID,pGlobalServerItem);

	return pGlobalServerItem;
}

//寻找用户
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByUserID(DWORD dwUserID)
{
	//寻找用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//寻找用户
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByGameID(DWORD dwGameID)
{
	//寻找用户
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapGameID.Lookup(dwGameID,pGlobalUserItem);

	return pGlobalUserItem;
}

//寻找用户
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByNickName(LPCTSTR pszNickName)
{
	//寻找用户
	VOID * pGlobalUserItem=NULL;
	m_MapNickName.Lookup(pszNickName,pGlobalUserItem);

	return (CGlobalUserItem *)pGlobalUserItem;
}

//枚举用户
CGlobalUserItem * CGlobalInfoManager::EnumUserItem(POSITION & Position)
{
	//变量定义
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_MapUserID.GetStartPosition();
	if (Position!=NULL)	m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//枚举广场
CGlobalPlazaItem * CGlobalInfoManager::EnumPlazaItem(POSITION & Position)
{
	//变量定义
	WORD wPlazaID=0L;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_MapPlazaID.GetStartPosition();
	if (Position!=NULL)	m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);

	return pGlobalPlazaItem;
}

//枚举房间
CGlobalServerItem * CGlobalInfoManager::EnumServerItem(POSITION & Position)
{
	//变量定义
	WORD wServerID=0L;
	CGlobalServerItem * pGlobalServerItem=NULL;

	//获取对象
	if (Position==NULL) Position=m_MapServerID.GetStartPosition();
	if (Position!=NULL)	m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);

	return pGlobalServerItem;
}

//创建用户
CGlobalUserItem * CGlobalInfoManager::CreateGlobalUserItem()
{
	//使用存储
	if (m_pGlobalUserItem!=NULL)
	{
		CGlobalUserItem * pGlobalUserItem=m_pGlobalUserItem;
		m_pGlobalUserItem=m_pGlobalUserItem->m_pNextUserItemPtr;
		pGlobalUserItem->m_pNextUserItemPtr=NULL;
		return pGlobalUserItem;
	}

	//创建对象
	try 
	{ 
		CGlobalUserItem * pGlobalUserItem=new CGlobalUserItem;
		return pGlobalUserItem;
	}
	catch (...) {}

	return NULL;
}

//创建广场
CGlobalPlazaItem * CGlobalInfoManager::CreateGlobalPlazaItem()
{
	//使用存储
	if (m_pGlobalPlazaItem!=NULL)
	{
		CGlobalPlazaItem * pGlobalPlazaItem=m_pGlobalPlazaItem;
		m_pGlobalPlazaItem=m_pGlobalPlazaItem->m_pNextPlazaItemPtr;
		pGlobalPlazaItem->m_pNextPlazaItemPtr=NULL;
		return pGlobalPlazaItem;
	}

	//创建对象
	try 
	{ 
		CGlobalPlazaItem * pGlobalPlazaItem=new CGlobalPlazaItem;
		return pGlobalPlazaItem;
	}
	catch (...) {}

	return NULL;
}

//创建房间
CGlobalServerItem * CGlobalInfoManager::CreateGlobalServerItem()
{
	//使用存储
	if (m_pGlobalServerItem!=NULL)
	{
		CGlobalServerItem * pGlobalServerItem=m_pGlobalServerItem;
		m_pGlobalServerItem=m_pGlobalServerItem->m_pNextServerItemPtr;
		pGlobalServerItem->m_pNextServerItemPtr=NULL;
		return pGlobalServerItem;
	}

	//创建对象
	try 
	{ 
		CGlobalServerItem * pGlobalServerItem=new CGlobalServerItem;
		return pGlobalServerItem;
	}
	catch (...) {}

	return NULL;
}

//释放用户
bool CGlobalInfoManager::FreeGlobalUserItem(CGlobalUserItem * pGlobalUserItem)
{
	//效验参数
	ASSERT(pGlobalUserItem!=NULL);
	if (pGlobalUserItem==NULL) return false;

	//设置变量
	pGlobalUserItem->m_dwUserID=0L;
	pGlobalUserItem->m_dwGameID=0L;
	pGlobalUserItem->m_szNickName[0]=0;
	pGlobalUserItem->m_GlobalServerItemArray.RemoveAll();

	//加入存储
	pGlobalUserItem->m_pNextUserItemPtr=m_pGlobalUserItem;
	m_pGlobalUserItem=pGlobalUserItem;

	return true;
}

//释放广场
bool CGlobalInfoManager::FreeGlobalPlazaItem(CGlobalPlazaItem * pGlobalPlazaItem)
{
	//效验参数
	ASSERT(pGlobalPlazaItem!=NULL);
	if (pGlobalPlazaItem==NULL) return false;

	//设置变量
	pGlobalPlazaItem->m_wIndex=0;
	ZeroMemory(&pGlobalPlazaItem->m_GamePlaza,sizeof(pGlobalPlazaItem->m_GamePlaza));

	//加入存储
	pGlobalPlazaItem->m_pNextPlazaItemPtr=m_pGlobalPlazaItem;
	m_pGlobalPlazaItem=pGlobalPlazaItem;

	return true;
}

//释放房间
bool CGlobalInfoManager::FreeGlobalServerItem(CGlobalServerItem * pGlobalServerItem)
{
	//效验参数
	ASSERT(pGlobalServerItem!=NULL);
	if (pGlobalServerItem==NULL) return false;

	//设置索引
	pGlobalServerItem->m_MapUserID.RemoveAll();

	//设置变量
	pGlobalServerItem->m_wIndex=0;
	ZeroMemory(&pGlobalServerItem->m_GameServer,sizeof(pGlobalServerItem->m_GameServer));

	//加入存储
	pGlobalServerItem->m_pNextServerItemPtr=m_pGlobalServerItem;
	m_pGlobalServerItem=pGlobalServerItem;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
