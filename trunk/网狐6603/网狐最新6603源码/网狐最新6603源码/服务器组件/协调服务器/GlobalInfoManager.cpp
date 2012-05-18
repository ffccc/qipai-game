#include "StdAfx.h"
#include "GlobalInfoManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalUserItem::CGlobalUserItem()
{
	//�û�����
	m_dwUserID=0L;
	m_dwGameID=0L;
	m_szNickName[0]=0;

	//�ȼ���Ϣ
	m_cbMemberOrder=0;
	m_cbMasterOrder=0;

	//��������
	m_pNextUserItemPtr=NULL;

	return;
}

//��������
CGlobalUserItem::~CGlobalUserItem()
{
}

//ö�ٷ���
CGlobalServerItem * CGlobalUserItem::EnumServerItem(WORD wIndex)
{
	if (wIndex>=m_GlobalServerItemArray.GetCount()) return NULL;
	return m_GlobalServerItemArray[wIndex];
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalPlazaItem::CGlobalPlazaItem()
{
	//���ñ���
	m_wIndex=0;
	ZeroMemory(&m_GamePlaza,sizeof(m_GamePlaza));

	//��������
	m_pNextPlazaItemPtr=NULL;

	return;
}

//��������
CGlobalPlazaItem::~CGlobalPlazaItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalServerItem::CGlobalServerItem()
{
	//���ñ���
	m_wIndex=0;
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	//��������
	m_pNextServerItemPtr=NULL;

	//��������
	m_MapUserID.InitHashTable(PRIME_SERVER_USER);

	return;
}

//��������
CGlobalServerItem::~CGlobalServerItem()
{
}

//Ѱ���û�
CGlobalUserItem * CGlobalServerItem::SearchUserItem(DWORD dwUserID)
{
	//�����û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalInfoManager::CGlobalInfoManager()
{
	//�洢����
	m_pGlobalUserItem=NULL;
	m_pGlobalPlazaItem=NULL;
	m_pGlobalServerItem=NULL;

	//��������
	m_MapPlazaID.InitHashTable(PRIME_SERVER);
	m_MapServerID.InitHashTable(PRIME_SERVER);
	m_MapUserID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapGameID.InitHashTable(PRIME_PLATFORM_USER);
	m_MapNickName.InitHashTable(PRIME_PLATFORM_USER);

	return;
}

//��������
CGlobalInfoManager::~CGlobalInfoManager()
{
	//ɾ���û�
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		SafeDelete(pGlobalUserItem);
	}
	m_MapUserID.RemoveAll();

	//ɾ���㳡
	WORD wPlazaID=0;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	Position=m_MapPlazaID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);
		SafeDelete(pGlobalPlazaItem);
	}
	m_MapPlazaID.RemoveAll();

	//ɾ������
	WORD wServerID=0;
	CGlobalServerItem * pGlobalServerItem=NULL;
	Position=m_MapServerID.GetStartPosition();
	while (Position!=NULL)
	{
		m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);
		SafeDelete(pGlobalServerItem);
	}
	m_MapServerID.RemoveAll();

	//ɾ���û�
	while (m_pGlobalUserItem!=NULL)
	{
		pGlobalUserItem=m_pGlobalUserItem;
		m_pGlobalUserItem=m_pGlobalUserItem->m_pNextUserItemPtr;
		SafeDelete(pGlobalUserItem);
	}

	//ɾ���㳡
	while (m_pGlobalPlazaItem!=NULL)
	{
		pGlobalPlazaItem=m_pGlobalPlazaItem;
		m_pGlobalPlazaItem=m_pGlobalPlazaItem->m_pNextPlazaItemPtr;
		SafeDelete(pGlobalPlazaItem);
	}

	//ɾ������
	while (m_pGlobalServerItem!=NULL)
	{
		pGlobalServerItem=m_pGlobalServerItem;
		m_pGlobalServerItem=m_pGlobalServerItem->m_pNextServerItemPtr;
		SafeDelete(pGlobalServerItem);
	}

	return;
}

//��������
VOID CGlobalInfoManager::ResetData()
{
	//ɾ���û�
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=m_MapUserID.GetStartPosition();

	//ɾ���û�
	while (Position!=NULL)
	{
		m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);
		if (pGlobalUserItem!=NULL) FreeGlobalUserItem(pGlobalUserItem);
	}

	//ɾ���㳡
	WORD wPlazaID=0;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	Position=m_MapPlazaID.GetStartPosition();

	//ɾ���㳡
	while (Position!=NULL)
	{
		m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);
		if (pGlobalPlazaItem!=NULL) FreeGlobalPlazaItem(pGlobalPlazaItem);
	}

	//ɾ������
	WORD wServerID=0;
	CGlobalServerItem * pGlobalServerItem=NULL;
	Position=m_MapServerID.GetStartPosition();

	//ɾ������
	while (Position!=NULL)
	{
		m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);
		if (pGlobalServerItem!=NULL) FreeGlobalServerItem(pGlobalServerItem);
	}

	//ɾ������
	m_MapUserID.RemoveAll();
	m_MapGameID.RemoveAll();
	m_MapPlazaID.RemoveAll();
	m_MapServerID.RemoveAll();
	m_MapNickName.RemoveAll();

	return;
}

//ɾ���û�
bool CGlobalInfoManager::DeleteUserItem(DWORD dwUserID, WORD wServerID)
{
	//Ѱ���û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	if (m_MapUserID.Lookup(dwUserID,pGlobalUserItem)==FALSE) 
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	CGlobalServerItem * pGlobalServerItem=NULL;
	INT_PTR nServerCount=pGlobalUserItem->m_GlobalServerItemArray.GetCount();

	//�˳�����
	for (INT_PTR i=0;i<nServerCount;i++)
	{
		//��ȡ����
		pGlobalServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

		//�����ж�
		if (pGlobalServerItem->GetServerID()==wServerID)
		{
			//ɾ������
			pGlobalServerItem->m_MapUserID.RemoveKey(dwUserID);
			pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);

			//�ͷ��û�
			if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0L) 
			{
				//�ͷ�����
				m_MapUserID.RemoveKey(dwUserID);
				m_MapGameID.RemoveKey(pGlobalUserItem->GetGameID());
				m_MapNickName.RemoveKey(pGlobalUserItem->GetNickName());

				//�ͷŶ���
				FreeGlobalUserItem(pGlobalUserItem);
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//�����û�
bool CGlobalInfoManager::ActiveUserItem(tagGlobalUserInfo & GlobalUserInfo, WORD wServerID)
{
	//Ѱ�ҷ���
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(wServerID,pGlobalServerItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//Ѱ���û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	if (m_MapUserID.Lookup(GlobalUserInfo.dwUserID,pGlobalUserItem)==FALSE)
	{
		//�����û�
		pGlobalUserItem=CreateGlobalUserItem();
		if (pGlobalUserItem==NULL) 
		{
			ASSERT(FALSE);
			return false;
		}

		//�����ǳ�
		TCHAR szNickName[LEN_NICKNAME]=TEXT("");
		lstrcpyn(szNickName,GlobalUserInfo.szNickName,CountArray(szNickName));

		//�����û�
		pGlobalUserItem->m_dwUserID=GlobalUserInfo.dwUserID;
		pGlobalUserItem->m_dwGameID=GlobalUserInfo.dwGameID;
		lstrcpyn(pGlobalUserItem->m_szNickName,GlobalUserInfo.szNickName,CountArray(pGlobalUserItem->m_szNickName));

		//������Ϣ
		pGlobalUserItem->m_cbGender=GlobalUserInfo.cbGender;
		pGlobalUserItem->m_cbMemberOrder=GlobalUserInfo.cbMemberOrder;
		pGlobalUserItem->m_cbMasterOrder=GlobalUserInfo.cbMasterOrder;

		//�ǳ�����
		m_MapNickName[_tcslwr(szNickName)]=pGlobalUserItem;

		//��������
		m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;
		m_MapGameID[GlobalUserInfo.dwGameID]=pGlobalUserItem;
	}
	else
	{
		//�ظ��ж�
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			if (pGlobalUserItem->m_GlobalServerItemArray[i]->GetServerID()==wServerID)
			{
				ASSERT(FALSE);
				return false;
			}
		}
	}

	//���ù���
	pGlobalUserItem->m_GlobalServerItemArray.Add(pGlobalServerItem);
	pGlobalServerItem->m_MapUserID[GlobalUserInfo.dwUserID]=pGlobalUserItem;

	return true;
}

//ɾ���㳡
bool CGlobalInfoManager::DeletePlazaItem(WORD wPlazaID)
{
	//Ѱ�ҹ㳡
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	if (m_MapPlazaID.Lookup(wPlazaID,pGlobalPlazaItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//�ͷŹ㳡
	m_MapPlazaID.RemoveKey(wPlazaID);
	FreeGlobalPlazaItem(pGlobalPlazaItem);

	return true;
}

//����㳡
bool CGlobalInfoManager::ActivePlazaItem(WORD wBindIndex, tagGamePlaza & GamePlaza)
{
	//Ѱ�ҹ㳡
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	if (m_MapPlazaID.Lookup(GamePlaza.wPlazaID,pGlobalPlazaItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//�����㳡
	pGlobalPlazaItem=CreateGlobalPlazaItem();
	if (pGlobalPlazaItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ù㳡
	pGlobalPlazaItem->m_wIndex=wBindIndex;
	pGlobalPlazaItem->m_GamePlaza=GamePlaza;

	//��������
	m_MapPlazaID[GamePlaza.wPlazaID]=pGlobalPlazaItem;

	return true;
}

//ɾ������
bool CGlobalInfoManager::DeleteServerItem(WORD wServerID)
{
	//Ѱ�ҷ���
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(wServerID,pGlobalServerItem)==FALSE)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	DWORD dwUserKey=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;
	POSITION Position=pGlobalServerItem->m_MapUserID.GetStartPosition();

	//ɾ���û�
	while (Position!=NULL)
	{
		//��ȡ����
		pGlobalServerItem->m_MapUserID.GetNextAssoc(Position,dwUserKey,pGlobalUserItem);

		//�������
		for (INT_PTR i=0;i<pGlobalUserItem->m_GlobalServerItemArray.GetCount();i++)
		{
			//��ȡ����
			CGlobalServerItem * pTempServerItem=pGlobalUserItem->m_GlobalServerItemArray[i];

			//�����ж�
			if (pTempServerItem->GetServerID()==wServerID)
			{
				pGlobalUserItem->m_GlobalServerItemArray.RemoveAt(i);
				break;
			}
		}

		//�ͷ��û�
		if (pGlobalUserItem->m_GlobalServerItemArray.GetCount()==0)
		{
			m_MapUserID.RemoveKey(dwUserKey);
			FreeGlobalUserItem(pGlobalUserItem);
		}
	}

	//�ͷŷ���
	m_MapServerID.RemoveKey(wServerID);
	FreeGlobalServerItem(pGlobalServerItem);

	return true;
}

//�����
bool CGlobalInfoManager::ActiveServerItem(WORD wBindIndex, tagGameServer & GameServer)
{
	//Ѱ�ҷ���
	CGlobalServerItem * pGlobalServerItem=NULL;
	if (m_MapServerID.Lookup(GameServer.wServerID,pGlobalServerItem)==TRUE)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	pGlobalServerItem=CreateGlobalServerItem();
	if (pGlobalServerItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//���÷���
	pGlobalServerItem->m_wIndex=wBindIndex;
	pGlobalServerItem->m_GameServer=GameServer;

	//��������
	m_MapServerID[GameServer.wServerID]=pGlobalServerItem;

	return true;
}

//Ѱ�ҹ㳡
CGlobalPlazaItem * CGlobalInfoManager::SearchPlazaItem(WORD wPlazaID)
{
	//Ѱ�ҷ���
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;
	m_MapPlazaID.Lookup(wPlazaID,pGlobalPlazaItem);

	return pGlobalPlazaItem;
}

//Ѱ�ҷ���
CGlobalServerItem * CGlobalInfoManager::SearchServerItem(WORD wServerID)
{
	//Ѱ�ҷ���
	CGlobalServerItem * pGlobalServerItem=NULL;
	m_MapServerID.Lookup(wServerID,pGlobalServerItem);

	return pGlobalServerItem;
}

//Ѱ���û�
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByUserID(DWORD dwUserID)
{
	//Ѱ���û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapUserID.Lookup(dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//Ѱ���û�
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByGameID(DWORD dwGameID)
{
	//Ѱ���û�
	CGlobalUserItem * pGlobalUserItem=NULL;
	m_MapGameID.Lookup(dwGameID,pGlobalUserItem);

	return pGlobalUserItem;
}

//Ѱ���û�
CGlobalUserItem * CGlobalInfoManager::SearchUserItemByNickName(LPCTSTR pszNickName)
{
	//Ѱ���û�
	VOID * pGlobalUserItem=NULL;
	m_MapNickName.Lookup(pszNickName,pGlobalUserItem);

	return (CGlobalUserItem *)pGlobalUserItem;
}

//ö���û�
CGlobalUserItem * CGlobalInfoManager::EnumUserItem(POSITION & Position)
{
	//��������
	DWORD dwUserID=0L;
	CGlobalUserItem * pGlobalUserItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_MapUserID.GetStartPosition();
	if (Position!=NULL)	m_MapUserID.GetNextAssoc(Position,dwUserID,pGlobalUserItem);

	return pGlobalUserItem;
}

//ö�ٹ㳡
CGlobalPlazaItem * CGlobalInfoManager::EnumPlazaItem(POSITION & Position)
{
	//��������
	WORD wPlazaID=0L;
	CGlobalPlazaItem * pGlobalPlazaItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_MapPlazaID.GetStartPosition();
	if (Position!=NULL)	m_MapPlazaID.GetNextAssoc(Position,wPlazaID,pGlobalPlazaItem);

	return pGlobalPlazaItem;
}

//ö�ٷ���
CGlobalServerItem * CGlobalInfoManager::EnumServerItem(POSITION & Position)
{
	//��������
	WORD wServerID=0L;
	CGlobalServerItem * pGlobalServerItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_MapServerID.GetStartPosition();
	if (Position!=NULL)	m_MapServerID.GetNextAssoc(Position,wServerID,pGlobalServerItem);

	return pGlobalServerItem;
}

//�����û�
CGlobalUserItem * CGlobalInfoManager::CreateGlobalUserItem()
{
	//ʹ�ô洢
	if (m_pGlobalUserItem!=NULL)
	{
		CGlobalUserItem * pGlobalUserItem=m_pGlobalUserItem;
		m_pGlobalUserItem=m_pGlobalUserItem->m_pNextUserItemPtr;
		pGlobalUserItem->m_pNextUserItemPtr=NULL;
		return pGlobalUserItem;
	}

	//��������
	try 
	{ 
		CGlobalUserItem * pGlobalUserItem=new CGlobalUserItem;
		return pGlobalUserItem;
	}
	catch (...) {}

	return NULL;
}

//�����㳡
CGlobalPlazaItem * CGlobalInfoManager::CreateGlobalPlazaItem()
{
	//ʹ�ô洢
	if (m_pGlobalPlazaItem!=NULL)
	{
		CGlobalPlazaItem * pGlobalPlazaItem=m_pGlobalPlazaItem;
		m_pGlobalPlazaItem=m_pGlobalPlazaItem->m_pNextPlazaItemPtr;
		pGlobalPlazaItem->m_pNextPlazaItemPtr=NULL;
		return pGlobalPlazaItem;
	}

	//��������
	try 
	{ 
		CGlobalPlazaItem * pGlobalPlazaItem=new CGlobalPlazaItem;
		return pGlobalPlazaItem;
	}
	catch (...) {}

	return NULL;
}

//��������
CGlobalServerItem * CGlobalInfoManager::CreateGlobalServerItem()
{
	//ʹ�ô洢
	if (m_pGlobalServerItem!=NULL)
	{
		CGlobalServerItem * pGlobalServerItem=m_pGlobalServerItem;
		m_pGlobalServerItem=m_pGlobalServerItem->m_pNextServerItemPtr;
		pGlobalServerItem->m_pNextServerItemPtr=NULL;
		return pGlobalServerItem;
	}

	//��������
	try 
	{ 
		CGlobalServerItem * pGlobalServerItem=new CGlobalServerItem;
		return pGlobalServerItem;
	}
	catch (...) {}

	return NULL;
}

//�ͷ��û�
bool CGlobalInfoManager::FreeGlobalUserItem(CGlobalUserItem * pGlobalUserItem)
{
	//Ч�����
	ASSERT(pGlobalUserItem!=NULL);
	if (pGlobalUserItem==NULL) return false;

	//���ñ���
	pGlobalUserItem->m_dwUserID=0L;
	pGlobalUserItem->m_dwGameID=0L;
	pGlobalUserItem->m_szNickName[0]=0;
	pGlobalUserItem->m_GlobalServerItemArray.RemoveAll();

	//����洢
	pGlobalUserItem->m_pNextUserItemPtr=m_pGlobalUserItem;
	m_pGlobalUserItem=pGlobalUserItem;

	return true;
}

//�ͷŹ㳡
bool CGlobalInfoManager::FreeGlobalPlazaItem(CGlobalPlazaItem * pGlobalPlazaItem)
{
	//Ч�����
	ASSERT(pGlobalPlazaItem!=NULL);
	if (pGlobalPlazaItem==NULL) return false;

	//���ñ���
	pGlobalPlazaItem->m_wIndex=0;
	ZeroMemory(&pGlobalPlazaItem->m_GamePlaza,sizeof(pGlobalPlazaItem->m_GamePlaza));

	//����洢
	pGlobalPlazaItem->m_pNextPlazaItemPtr=m_pGlobalPlazaItem;
	m_pGlobalPlazaItem=pGlobalPlazaItem;

	return true;
}

//�ͷŷ���
bool CGlobalInfoManager::FreeGlobalServerItem(CGlobalServerItem * pGlobalServerItem)
{
	//Ч�����
	ASSERT(pGlobalServerItem!=NULL);
	if (pGlobalServerItem==NULL) return false;

	//��������
	pGlobalServerItem->m_MapUserID.RemoveAll();

	//���ñ���
	pGlobalServerItem->m_wIndex=0;
	ZeroMemory(&pGlobalServerItem->m_GameServer,sizeof(pGlobalServerItem->m_GameServer));

	//����洢
	pGlobalServerItem->m_pNextServerItemPtr=m_pGlobalServerItem;
	m_pGlobalServerItem=pGlobalServerItem;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
