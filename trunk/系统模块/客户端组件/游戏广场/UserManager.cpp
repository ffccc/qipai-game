#include "StdAfx.h"
#include "UserManager.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
CUserItemPtrArray		CClientUserManager::m_UserItemStorage;			//�洢����

//////////////////////////////////////////////////////////////////////////

//���캯��
CUserItem::CUserItem() 
{ 
	CleanData(); 
}

//��������
CUserItem::~CUserItem() 
{
}

//�ӿڲ�ѯ
void * __cdecl CUserItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IUserItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IUserItem,Guid,dwQueryVer);
	return NULL;
}

//��Ϸ����
LONG CUserItem::GetUserPlayCount()
{
	return m_UserData.lWinCount+m_UserData.lLostCount+m_UserData.lDrawCount+m_UserData.lFleeCount;
}

//���û���
void __cdecl CUserItem::SetUserScore(const tagUserScore * pUserScore)
{
	//Ч�����
	ASSERT(m_bActive==true);
	ASSERT(pUserScore!=NULL);

	//���ñ���
	m_UserData.lScore=pUserScore->lScore;
	m_UserData.lWinCount=pUserScore->lWinCount;
	m_UserData.lLostCount=pUserScore->lLostCount;
	m_UserData.lDrawCount=pUserScore->lDrawCount;
	m_UserData.lFleeCount=pUserScore->lFleeCount;
	m_UserData.lExperience=pUserScore->lExperience;

	return;
}

//����״̬
void __cdecl CUserItem::SetUserStatus(const tagUserStatus * pUserStatus)
{
	//Ч�����
	ASSERT(m_bActive==true);
	ASSERT(pUserStatus!=NULL);

	//���ñ���
	m_UserData.wTableID=pUserStatus->wTableID;
	m_UserData.wChairID=pUserStatus->wChairID;
	m_UserData.cbUserStatus=pUserStatus->cbUserStatus;

	return;
}

//��������
void CUserItem::CleanData() 
{
	m_bActive=false;
	memset(&m_UserData,0,sizeof(m_UserData));
	m_UserData.wTableID=INVALID_TABLE;
	m_UserData.wChairID=INVALID_CHAIR;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CClientUserManager::CClientUserManager(void)
{
	m_pIUserManagerSink=NULL;
}

//�ӿڲ�ѯ
void * __cdecl CClientUserManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IClientUserManager,Guid,dwQueryVer);
	return NULL;
}

//��������
CClientUserManager::~CClientUserManager(void)
{
	//�ͷ��û�
	m_UserItemStorage.Append(m_UserItemActive);
	m_UserItemActive.RemoveAll();

	return;
}

//�����û�
IUserItem * __cdecl CClientUserManager::ActiveUserItem(tagUserData & UserData)
{
	//Ѱ���û�
	CUserItem * pUserItem=NULL;
	INT_PTR nStorageCount=m_UserItemStorage.GetCount();
	if (nStorageCount>0)
	{
		pUserItem=m_UserItemStorage[nStorageCount-1];
		ASSERT(pUserItem!=NULL);
		m_UserItemStorage.RemoveAt(nStorageCount-1);
	}
	else
	{
		pUserItem=new CUserItem;
		ASSERT(pUserItem!=NULL);
		if (pUserItem==NULL) return false;
	}

	//��������
	pUserItem->m_bActive=true;
	CopyMemory(&pUserItem->m_UserData,&UserData,sizeof(UserData));
	m_UserItemActive.Add(pUserItem);

	//������Ϣ
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserItemAcitve(pUserItem);

	return pUserItem;
}

//ɾ���û�
bool __cdecl CClientUserManager::DeleteUserItem(IUserItem * pIUserItem)
{
	//�����û�
	CUserItem * pUserItemActive=NULL;
	for (INT_PTR i=0;i<m_UserItemActive.GetCount();i++)
	{
		pUserItemActive=m_UserItemActive[i];
		ASSERT(pUserItemActive!=NULL);
		if (pIUserItem==pUserItemActive)
		{
			//ɾ���û�
			m_UserItemActive.RemoveAt(i);
			m_UserItemStorage.Add(pUserItemActive);
			m_pIUserManagerSink->OnUserItemDelete(pUserItemActive);
			pUserItemActive->CleanData();
			return true;
		}
	}
	ASSERT(FALSE);

	return false;
}

//���»���
bool __cdecl CClientUserManager::UpdateUserItemScore(IUserItem * pIUserItem, const tagUserScore * pUserScore)
{
	//Ч�����
	ASSERT(pUserScore!=NULL);
	if (pUserScore==NULL) return false;

	//��������
	pIUserItem->SetUserScore(pUserScore);

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserItemUpdate(pIUserItem);

	return true;
}

//����״̬
bool __cdecl CClientUserManager::UpdateUserItemStatus(IUserItem * pIUserItem, const tagUserStatus * pUserStatus)
{
	//��������
	pIUserItem->SetUserStatus(pUserStatus);

	//֪ͨ����
	ASSERT(m_pIUserManagerSink!=NULL);
	m_pIUserManagerSink->OnUserItemUpdate(pIUserItem);

	return true;
}

//ö���û�
IUserItem * __cdecl CClientUserManager::EnumUserItem(WORD wEnumIndex)
{
	if (wEnumIndex<m_UserItemActive.GetCount())
	{
		CUserItem * pUserItem=m_UserItemActive[wEnumIndex];
		ASSERT(pUserItem!=NULL);
		ASSERT(pUserItem->m_bActive==true);
		return pUserItem;
	}
	return NULL;
}

//�����û�
IUserItem * __cdecl CClientUserManager::SearchUserByUserID(DWORD dwUserID)
{
	CUserItem * pUserItem=NULL;
	for (INT_PTR i=0;i<m_UserItemActive.GetCount();i++)
	{
		pUserItem=m_UserItemActive[i];
		ASSERT(pUserItem!=NULL);
		ASSERT(pUserItem->m_bActive==true);
		if (pUserItem->m_UserData.dwUserID==dwUserID) return pUserItem;
	}
	return NULL;
}

//�����û�
IUserItem * __cdecl CClientUserManager::SearchUserByGameID(DWORD dwGameID)
{
	CUserItem * pUserItem=NULL;
	for (INT_PTR i=0;i<m_UserItemActive.GetCount();i++)
	{
		pUserItem=m_UserItemActive[i];
		ASSERT(pUserItem!=NULL);
		ASSERT(pUserItem->m_bActive==true);
		if (pUserItem->m_UserData.dwGameID==dwGameID) return pUserItem;
	}
	return NULL;
}

//���ûص��ӿ�
bool CClientUserManager::SetUserManagerSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIUserManagerSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IUserManagerSink);
	ASSERT(m_pIUserManagerSink!=NULL);
	return (m_pIUserManagerSink!=NULL);
}

//////////////////////////////////////////////////////////////////////////
