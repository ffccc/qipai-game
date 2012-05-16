#include "StdAfx.h"
#include "ServerUserManager.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerUserItem::CServerUserItem(void)
{
	InitServerUserItem();
}

//��������
CServerUserItem::~CServerUserItem(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CServerUserItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserItem,Guid,dwQueryVer);
	return NULL;
}

//�û�ʤ��
WORD __cdecl CServerUserItem::GetUserWinRate()
{
	//Ч�����
	ASSERT(m_bAcitve==true);

	//����ʤ��
	tagUserScore * pUserScore=&m_ServerUserData.UserScoreInfo;
	LONG lPlayCount=pUserScore->lWinCount+pUserScore->lLostCount+pUserScore->lDrawCount+pUserScore->lFleeCount;
	if (lPlayCount!=0L) return (WORD)(pUserScore->lWinCount*10000L/lPlayCount);

	return 0;
}

//�û�����
WORD __cdecl CServerUserItem::GetUserLostRate()
{
	//Ч�����
	ASSERT(m_bAcitve==true);

	//��������
	tagUserScore * pUserScore=&m_ServerUserData.UserScoreInfo;
	LONG lPlayCount=pUserScore->lWinCount+pUserScore->lLostCount+pUserScore->lDrawCount+pUserScore->lFleeCount;
	if (lPlayCount!=0L) return (WORD)(pUserScore->lLostCount*10000L/lPlayCount);

	return 0;
}

//�û�����
WORD __cdecl CServerUserItem::GetUserDrawRate()
{
	//Ч�����
	ASSERT(m_bAcitve==true);

	//�������
	tagUserScore * pUserScore=&m_ServerUserData.UserScoreInfo;
	LONG lPlayCount=pUserScore->lWinCount+pUserScore->lLostCount+pUserScore->lDrawCount+pUserScore->lFleeCount;
	if (lPlayCount!=0L) return (WORD)(pUserScore->lDrawCount*10000L/lPlayCount);

	return 0;
}

//�û�����
WORD __cdecl CServerUserItem::GetUserFleeRate()
{
	//Ч�����
	ASSERT(m_bAcitve==true);

	//��������
	tagUserScore * pUserScore=&m_ServerUserData.UserScoreInfo;
	LONG lPlayCount=pUserScore->lWinCount+pUserScore->lLostCount+pUserScore->lDrawCount+pUserScore->lFleeCount;
	if (lPlayCount!=0L) return (WORD)(pUserScore->lFleeCount*10000L/lPlayCount);

	return 0;
}

//���ù���
bool __cdecl CServerUserItem::SetUserRule(tagUserRule & UserRule)
{
	//Ч�����
	ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ù���
	m_UserRule=UserRule;

	return true;
}

//��Ϸ����
bool __cdecl CServerUserItem::SetServerType(WORD wServerType)
{
	m_wServerType = wServerType;

	return true;
}

//�޸Ļ���
bool __cdecl CServerUserItem::WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount)
{
	//Ч�����
	ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//�޸�����
	switch (ScoreInfo.ScoreKind)
	{
	case enScoreKind_Win: 
		{
			m_ScoreModify.lWinCount++;
			m_ServerUserData.UserScoreInfo.lWinCount++;
			break;
		}
	case enScoreKind_Lost: 
		{
			m_ScoreModify.lLostCount++;
			m_ServerUserData.UserScoreInfo.lLostCount++;
			break;
		}
	case enScoreKind_Draw:
		{
			m_ScoreModify.lDrawCount++;
			m_ServerUserData.UserScoreInfo.lDrawCount++;
			break;
		}
	case enScoreKind_Flee:
		{
			m_ScoreModify.lFleeCount++;
			m_ServerUserData.UserScoreInfo.lFleeCount++;
			break;
		}
	}

	//�޸�ʱ��
	for ( int nIndex = 0; nIndex < PROPERTY_COUNT; ++nIndex )
	{
		if ( nIndex == PROP_FOURDOLD && m_ServerUserData.PropertyInfo[PROP_DOUBLE].dwConsumedCount <= m_ServerUserData.PropertyInfo[PROP_DOUBLE].dwPropCount ) 
			continue;

		if ( nIndex == PROP_BUGLE || nIndex == PROP_FLEE || nIndex == PROP_NEGAGIVE ) continue;

		m_ServerUserData.PropertyInfo[nIndex].dwConsumedCount += dwPlayTimeCount;
	}

	//�޸Ļ���
	m_lRevenue+=ScoreInfo.lRevenue;
	m_ScoreModify.lScore+=ScoreInfo.lScore;
	m_ServerUserData.UserScoreInfo.lScore+=ScoreInfo.lScore;
	if(m_wServerType==GAME_GENRE_GOLD)
	{
		m_ScoreModify.lGameGold+=ScoreInfo.lScore;
		m_ServerUserData.UserScoreInfo.lGameGold+=ScoreInfo.lScore;
	}

	//�û�����
	if ((ScoreInfo.ScoreKind!=enScoreKind_Service)&&(ScoreInfo.ScoreKind!=enScoreKind_Present))
	{
		//��Ϸʱ��
		m_dwPlayTimeCount+=dwPlayTimeCount;

		//�û�����
		m_ScoreModify.lExperience+=dwPlayTimeCount;
		m_ServerUserData.UserScoreInfo.lExperience+=dwPlayTimeCount;
	}

	return true;
}

//����״̬
bool __cdecl CServerUserItem::SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID)
{
	//Ч��״̬
	ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ñ���
	m_ServerUserData.wTableID=wTableID;
	m_ServerUserData.wChairID=wChairID;
	m_ServerUserData.cbUserStatus=cbUserStatus;

	return true;
}

//�޸��ʿ�
bool __cdecl CServerUserItem::ModifyBankStorageGold(LONG lStorageCount)
{
	m_ServerUserData.UserScoreInfo.lInsureScore += lStorageCount;
	m_ScoreModify.lInsureScore += lStorageCount;

	//�Ϸ��ж�
	ASSERT(0<=m_ServerUserData.UserScoreInfo.lInsureScore);

	return true;	
}

//��ʼ��
void CServerUserItem::InitServerUserItem()
{
	m_bAcitve=false;
	m_bAndroid=false;
	m_lRevenue=0L;
	m_wServerType=0;
	m_dwClientIP=0L;
	m_szPassword[0]=0;
	m_wUserIndex=0xFFFF;
	m_dwPlayTimeCount=0L;
	m_dwLogonTime=(LONG)time(NULL);
	memset(&m_UserRule,0,sizeof(m_UserRule));
	memset(&m_ScoreBorn,0,sizeof(m_ScoreBorn));
	memset(&m_ScoreModify,0,sizeof(m_ScoreModify));
	memset(&m_ServerUserData,0,sizeof(m_ServerUserData));

	return;
}

//������Ϣ
bool __cdecl CServerUserItem::GetUserScoreInfo(tagUserScore & ScoreModifyInfo)
{
	//Ч��״̬
	ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ñ���
	ScoreModifyInfo=m_ScoreModify;

	return true;;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerUserManager::CServerUserManager(void)
{
}

//��������
CServerUserManager::~CServerUserManager(void)
{
	//�ͷ��û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_UserItemStore.GetCount();i++)
	{
		pServerUserItem=m_UserItemStore[i];
		ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	m_UserItemStore.RemoveAll();
	m_OnLineUserItem.RemoveAll();
	m_OffLineUserItem.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CServerUserManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserManager,Guid,dwQueryVer);
	return NULL;
}

//�����û�
bool __cdecl CServerUserManager::ResetUserManager()
{
	//�ָ�����
	m_UserItemStore.Append(m_OnLineUserItem);
	m_UserItemStore.Append(m_OffLineUserItem);
	m_OnLineUserItem.RemoveAll();
	m_OffLineUserItem.RemoveAll();
	
	return true;
}

//�л��û�
bool __cdecl CServerUserManager::SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//Ѱ���û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			//�����û�
			pServerUserItem->m_dwClientIP=dwClientIP;
			pServerUserItem->m_wUserIndex=wUserIndex;
			pServerUserItem->m_bAndroid=(wUserIndex>=INDEX_ANDROID);

			return true;
		}
	}
	ASSERT(FALSE);

	return true;
}

//�����û�
bool __cdecl CServerUserManager::ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->GetUserStatus()==US_OFFLINE);
	if (pIServerUserItem==NULL) return false;

	//Ѱ���û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			//�����û�
			pServerUserItem->m_dwClientIP=dwClientIP;
			pServerUserItem->m_wUserIndex=wUserIndex;
			pServerUserItem->m_bAndroid=(wUserIndex>=INDEX_ANDROID);

			//ע������
			m_OffLineUserItem.RemoveAt(i);
			m_OnLineUserItem.Add(pServerUserItem);

			return true;
		}
	}
	ASSERT(FALSE);

	return false;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN],WORD wServerType)
{
	//��ȡ���ж���
	CServerUserItem * pServerUserItem=NULL;
	if (m_UserItemStore.GetCount()>0)
	{
		INT_PTR nItemPostion=m_UserItemStore.GetCount()-1;
		pServerUserItem=m_UserItemStore[nItemPostion];
		ASSERT(pServerUserItem!=NULL);
		m_UserItemStore.RemoveAt(nItemPostion);
	}

	//�����¶���
	if (pServerUserItem==NULL)
	{
		try
		{
			pServerUserItem=new CServerUserItem;
			if (pServerUserItem==NULL) return NULL;
		}
		catch (...) { return NULL; }
	}

	//�����û�
	ASSERT(pServerUserItem!=NULL);
	m_OnLineUserItem.Add(pServerUserItem);
	pServerUserItem->InitServerUserItem();

	//�����û�
	pServerUserItem->m_bAcitve=true;
	pServerUserItem->m_wUserIndex=wUserIndex;
	pServerUserItem->m_bAndroid=(wUserIndex>=INDEX_ANDROID);

	//�����û�
	pServerUserItem->m_dwClientIP=dwClientIP;
	pServerUserItem->m_dwLogonTime=(LONG)time(NULL);
	pServerUserItem->m_ScoreBorn=ServerUserData.UserScoreInfo;
	pServerUserItem->SetServerType(wServerType);
	CopyMemory(&pServerUserItem->m_ServerUserData,&ServerUserData,sizeof(ServerUserData));
	lstrcpyn(pServerUserItem->m_szPassword,szPassword,CountArray(pServerUserItem->m_szPassword));

	return pServerUserItem;
}

//ö���û�
IServerUserItem * __cdecl CServerUserManager::EnumOnLineUser(WORD wEnumIndex)
{
	if (wEnumIndex>=m_OnLineUserItem.GetCount()) return NULL;
	return m_OnLineUserItem[wEnumIndex];
}

//ö���û�
IServerUserItem * __cdecl CServerUserManager::EnumOffLineUser(WORD wEnumIndex)
{
	if (wEnumIndex>=m_OffLineUserItem.GetCount()) return NULL;
	return m_OffLineUserItem[wEnumIndex];
}

//ɾ���û�
bool __cdecl CServerUserManager::DeleteUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	//tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	if (pIServerUserItem->GetUserIndex()!=0xFFFF)
	{
		for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
		{
			//��ȡ�û�
			CServerUserItem * pOnLineUserItem=m_OnLineUserItem[i];

			//�û��ж�
			if (pOnLineUserItem==pIServerUserItem)
			{
				pOnLineUserItem->m_bAcitve=false;
				m_OnLineUserItem.RemoveAt(i);
				m_UserItemStore.Add(pOnLineUserItem);
				return true;
			}
		}
	}
	else
	{
		for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
		{
			//��ȡ�û�
			CServerUserItem * pOffLineUserItem=m_OffLineUserItem[i];

			//�û��ж�
			if (pOffLineUserItem==pIServerUserItem)
			{
				pOffLineUserItem->m_bAcitve=false;
				m_OffLineUserItem.RemoveAt(i);
				m_UserItemStore.Add(pOffLineUserItem);
				return true;
			}
		}
	}

	ASSERT(FALSE);

	return false;
}

//ע�����
bool __cdecl CServerUserManager::RegOffLineUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CServerUserItem * pServerUserItem=NULL;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	ASSERT(pUserData->cbUserStatus!=US_OFFLINE);
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			pServerUserItem->m_wUserIndex=0xFFFF;
			m_OnLineUserItem.RemoveAt(i);
			m_OffLineUserItem.Add(pServerUserItem);
			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//ɾ������
bool __cdecl CServerUserManager::UnRegOffLineUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CServerUserItem * pServerUserItem=NULL;
	//tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			m_OffLineUserItem.RemoveAt(i);
			m_OnLineUserItem.Add(pServerUserItem);
			return true;
		}
	}

	return false;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOnLineUser(DWORD dwUserID)
{
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem->m_ServerUserData.dwUserID==dwUserID) return pServerUserItem;
	}

	return NULL;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOffLineUser(DWORD dwUserID)
{
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem->m_ServerUserData.dwUserID==dwUserID) return pServerUserItem;
	}

	return NULL;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOnLineUser(LPCTSTR pszAccounts)
{
	CString strAccounts=pszAccounts,strAccountsTemp;
	strAccounts.MakeLower();
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		strAccountsTemp=pServerUserItem->m_ServerUserData.szAccounts;
		strAccountsTemp.MakeLower();
		if (strAccounts==strAccountsTemp) return pServerUserItem;
	}

	return NULL;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOffLineUser(LPCTSTR pszAccounts)
{
	CString strAccounts=pszAccounts,strAccountsTemp;
	strAccounts.MakeLower();
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		strAccountsTemp=pServerUserItem->m_ServerUserData.szAccounts;
		strAccountsTemp.MakeLower();
		if (strAccounts==strAccountsTemp) return pServerUserItem;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////